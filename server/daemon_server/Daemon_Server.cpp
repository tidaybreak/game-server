// -*- C++ -*-
/*
 * Daemon_Server.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: Ti
 */

#include "Daemon_Server.h"
#include "Daemon_Record.h"
#include "Daemon_Gate.h"
#include "Daemon_Logic.h"
#include "Daemon_Scene.h"
#include "Daemon_Chat.h"
#include "Daemon_Robot.h"
#include "DB_Manager.h"
#include "DB_Operator.h"
#include "Configurator.h"
#include "Config_Cache.h"
#include "Utility_Func.h"
#include "Epoll_Watcher.h"
#include "Global_Timer.h"
#include "Game_Version.h"
#include "Message_Unit.h"
#include "Service_Monitor.h"
#include <getopt.h>
#include <dirent.h>
#include "get_cpu.h"
#include "hot_update/Hot_Update.h"
#include "Config_Cache_Role.h"
#include <openssl/md5.h>
#include <boost/regex.hpp>
#include "DB_Merge_Server_Manager.h"

struct log_data {
	std::string fun;
	int amount;
};

bool Log_Greater(log_data l1, log_data l2){
	return l1.amount < l2.amount;
}

Daemon_Server::Daemon_Server(void)
: server_list_(0),
  wait_watcher_(0),
  unit_(0),
  service_connect_(0),
  client_map_(64),
  scene_server_map_(64),
  server_index_(0),
  c_port_(0),
  c_cid_(0),
  mongod_pid_(0),
  change_solution_restart_(false)
{ }

Daemon_Server::~Daemon_Server(void) { }

Daemon_Server *Daemon_Server::instance_;

std::string Daemon_Server::exec_name_ = "/server";

int Daemon_Server::server_identity_;

struct option Daemon_Server::long_options[] = {
		{"daemon",	no_argument,		0,	'd'},
		{"chat",	no_argument,		0,	'c'},
		{"authconfig",	no_argument,	0,	'a'},
		{"gate",	no_argument,		0,	'g'},
		{"logic",	no_argument,		0,	'l'},
		{"record",		no_argument,	0,	'r'},
		{"scene",	no_argument,		0,	's'},
		{"name",	required_argument,	0,	'n'},
		{"index",	required_argument,	0,	'i'},
		{"flag",	required_argument,	0,	'f'},
		{"version",	no_argument,		0,	'v'},
		{"log",		required_argument,	0,	'1'},
		{"robot",	no_argument,		0,	'2'},
		{"merge",   no_argument,        0,  'm'},
		{0, 0, 0, 0}
};

Daemon_Server *Daemon_Server::instance(void) {
    if (! instance_)
        instance_ = new Daemon_Server();
    return instance_;
}

void Daemon_Server::sigcld_handle(int signo) {
	LOG_DEBUG("Daemon_Server receive signo = %d.", signo);
	signal(SIGCHLD, sigcld_handle);
	//int   stat;
	//pid_t pid = waitpid(-1, &stat, WNOHANG);
	//if (pid < 0) {
	//	LOG_SYS("wait error");
	//}

	pid_t   pid;
	int   stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {

		DAEMON_SERVER_INSTANCE->restart_process(pid);
	}
}

void Daemon_Server::restart_process(int pid) {
	if (change_solution_restart_) {
		LOG_DEBUG("change_solution_restart pid:%d", pid);
		client_map_.erase(pid);

		if (client_map_.empty()) {
			change_solution_restart_ = false;
			MONGO_OPERATOR->drop_mmo_database();
			run_daemon_server();
			return ;
		}
	}

	Client_Map::iterator it = client_map_.find(pid);
	if (it == client_map_.end()) {
		LOG_USER("cannot find process, pid = %d.", pid);
		return;
	}

	LOG_DEBUG("start pid:%d, idx:%d", pid, it->second);

	run_daemon_server(it->second, true);

	client_map_.erase(pid);
}

int Daemon_Server::init(int argc, char *argv[]) {
	if ((wait_watcher_ = new Epoll_Watcher) == 0) {
		LOG_ABORT("new Epoll_Watcher return 0 errno:%d", errno);
	}
	config_path_ = "config";

	service_connect_ = new Service_Connect;
	Time_Value recv_timeout = Time_Value::zero;
	unit_ = new Daemon_Message_Unit;
	service_connect_->set_monitor(Unit_Message::TYPE_UNIT_CONNECT_SERVER, unit_, recv_timeout);
	service_connect_->init();
	return 0;
}

int Daemon_Server::start(int argc, char *argv[]) {
	start_time_ = Time_Value::gettimeofday();

	signal(SIGCHLD, sigcld_handle);
	parse_cmd_arguments(argc, argv);

	{
		// 取中央服地址
		Monitor_Mark_Vec list;
		const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
		server_solution_cache.fetch_muid_by_type(list, Server_Enum::MONITOR_SCENE);
		for (Monitor_Mark_Vec::iterator it = list.begin(); it != list.end(); ++it) {
			if ((*it).id == 80000000) {
				c_ip_ = (*it).ip;
				c_port_ = (*it).port;
				break;
			}
		}

		// 逻辑服连接信息
		int client_port = server_solution_cache.fetch_logic_client_port();
		if (!list.empty()) {
			std::stringstream logic_stream;
			logic_stream << list.front().ip << ":" << client_port;
			logic_ip_port_ = logic_stream.str();
		}

		// Mongo连接信息
		std::string mongo_ip = CONFIG_CACHE->server_maintainer_cache().mongo_ip;
		Server_Id_IP_Map::const_iterator ip_find_it = CONFIG_CACHE->server_ip_cache().find(CONFIG_CACHE->server_maintainer_cache().server_identity);
		if (mongo_ip == "127.0.0.1" && ip_find_it != CONFIG_CACHE->server_ip_cache().end())
			mongo_ip = ip_find_it->second.telecom_ip;
		std::stringstream mongo_stream;
		mongo_stream << mongo_ip << ":" << CONFIG_CACHE->server_maintainer_cache().mongo_port;
		mongo_ip_port_ = mongo_stream.str();

		// 取path名
		char path_c[1024] = {0};
		getcwd(path_c, sizeof(path_c));
		path_name_.assign(path_c);
		std::vector<std::string> path_name_vec = split_str(path_name_, "/");
		if (path_name_vec.size() == 4) {
			path_name_ = path_name_vec[2];
		}
		// 取mangod进程号，只考虑mangod和daemon进程部署在同一主机
		mongod_pid_ = get_mpid(path_name_.c_str());
	}

	service_connect_->start();


	// 本地版或daemon服才启动配制热更
	if (is_local_debug() || (has_in_server_list(MONITOR_DAEMON) && !CONFIG_CACHE->server_maintainer_cache().config_not_update))
		HOT_UPDATE_INSTANCE->thr_create();


	Time_Value tv(1, 0);
	wait_watcher_->add(this, Epoll_Watcher::EVENT_TIMEOUT, &tv);
	wait_watcher_->loop();

	return 0;
}

void Daemon_Server::send_to_center(void) {
	Block_Buffer inner_buf;
	inner_buf.make_message(20999999);
	inner_buf.write_uint8(2);

	inner_buf.write_uint8(1);
	inner_buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag + ":" + path_name_);

	inner_buf.write_uint8(3);
	inner_buf.write_string(logic_ip_port_);

	std::string s;
	get_version_string(s);
	inner_buf.write_uint8(4);
	inner_buf.write_string(s);

	inner_buf.write_uint8(5);
	inner_buf.write_uint32(start_time_.sec());

	inner_buf.write_uint8(6);
	inner_buf.write_int64(CONFIG_CACHE->server_flag());

	inner_buf.write_uint8(8);
	inner_buf.write_uint8(CONFIG_CACHE->server_maintainer_cache().game_ver_type);

	inner_buf.write_uint8(9);
	inner_buf.write_string(mongo_ip_port_);

	inner_buf.write_uint8(13);
	inner_buf.write_int32(CONFIG_CACHE->server_maintainer_cache().game_solution_id);

	inner_buf.write_uint8(17);
	inner_buf.write_uint32(CONFIG_CACHE->server_maintainer_cache().game_open_time.sec());

	time_t t = time(NULL);
	struct tm lt = {0};
	localtime_r(&t, &lt);
	inner_buf.write_uint8(19);
	inner_buf.write_uint32(lt.tm_gmtoff / Time_Value::ONE_HOUR_IN_SECS);

	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), 0);


	c_cid_ = send_to_monitor(c_ip_, c_port_, inner_buf);
	if (c_cid_ >= 2) {
		Svc *svc = service_connect_->find_svc(c_cid_);
		if (svc) {
			svc->set_role_id(1);
		}
	}
}

int Daemon_Server::handle_timeout(const Time_Value &tv) {
	if (has_in_server_list(MONITOR_DAEMON)) {
		if (MONGO_OPERATOR->update_db_cross_info(true)) {
			if (CONFIG_CACHE->server_maintainer_cache().game_solution_id != CONFIG_CACHE->server_maintainer_cache().default_solution_id) {
				change_solution_restart_ = true;
				CONFIG_INSTANCE->load_global_config();
				CONFIG_CACHE->refresh_maintainer_cache();
				CONFIG_INSTANCE->load_open_solution();
				CONFIG_CACHE->refresh_server_solution_cache();
				LOG_DEBUG("change solutione:%d ", CONFIG_CACHE->server_maintainer_cache().game_solution_id);
			}
		}
	}

	// 检测core数量传到中央服务器
	if (send_center_sleep_ < tv && c_ip_ != "127.0.0.1" && c_port_) {
		Block_Buffer inner_buf;
		inner_buf.make_message(20999999);
		inner_buf.write_uint8(2);

		inner_buf.write_uint8(1);
		inner_buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag + ":" + path_name_);

		if (has_in_server_list(MONITOR_DAEMON)) {

			inner_buf.write_uint8(2);
			inner_buf.write_uint8(check_core_amount(false));

			inner_buf.write_uint8(10);
			Time_Value::gettimeofday().serialize(inner_buf);

			inner_buf.write_uint8(16);
			inner_buf.write_uint32(CONFIG_CACHE->server_maintainer_cache().game_inter_flag);

			if (tv > log_file_tick_) {
				log_file_tick_ = tv + Time_Value(3600);
				inner_buf.write_uint8(14);
				inner_buf.write_uint32(file_size_time("local_log"));

				inner_buf.write_uint8(15);
				inner_buf.write_uint32(file_size_time("record_file"));
			}
			if ( mongod_pid_ ) {
				inner_buf.write_uint8(22);
				inner_buf.write_int32(get_phy_mem(mongod_pid_));

				inner_buf.write_uint8(23);
				inner_buf.write_int32(get_pcpu(mongod_pid_));
			}

		} else {
			inner_buf.write_uint8(11);
			inner_buf.write_uint8(server_index_);
			inner_buf.write_int32(get_phy_mem(getpid()));

			inner_buf.write_uint8(12);
			inner_buf.write_uint8(server_index_);
			inner_buf.write_int32(get_pcpu(getpid()));
		}
		inner_buf.finish_message();
		inner_buf.make_head(inner_buf.get_msg_id(), 0);

		if (c_cid_ >= 2) {
			Svc *svc = service_connect_->find_svc(c_cid_);
			if (svc && svc->get_role_id() == 1) {
				service_connect_->push_data_block_with_len(c_cid_, inner_buf, false);
			} else {
				send_to_center();
			}
		} else {
			send_to_center();
		}
		send_center_sleep_ = tv + Time_Value(30,0);
	}


	pid_t ppid = getppid();
	if (ppid == 1 || has_in_server_list(MONITOR_SHUTDOWN)) {
		if (has_in_server_list(MONITOR_DAEMON)) {
			return 0;
		}

		if (has_in_server_list(MONITOR_CLOSING)) {
			if (!has_in_server_list(MONITOR_LOGIC) &&
				!has_in_server_list(MONITOR_SCENE) &&
				!has_in_server_list(MONITOR_CHAT) &&
				!has_in_server_list(MONITOR_GATE) &&
				!has_in_server_list(MONITOR_RECORD)) {
				sleep(2);
				exit(0);
			}
		} else {
			if (CONFIG_CACHE->server_maintainer_cache().server_close_delay > 0) {
				if (has_in_server_list(MONITOR_LOGIC)) {
					DAEMON_LOGIC_INSTANCE->close_tip();
				}
				sleep(CONFIG_CACHE->server_maintainer_cache().server_close_delay);
			}
			DAEMON_CHAT_INSTANCE->stop();

			DAEMON_LOGIC_INSTANCE->stop();

			DAEMON_SCENE_INSTANCE->stop();

			DAEMON_RECORD_INSTANCE->stop();

			DAEMON_GATE_INSTANCE->stop();

			set_server_list(MONITOR_CLOSING);
		}
	}

	return 0;
}

void Daemon_Server::usage(void) {
	std::cout << "usage:" << std::endl
			<< "\t-d\tstart daemon server\n"
			<< "\t-c\tstart chat server\n"
			<< "\t-l\tstart logic server\n"
			<< "\t-m xxx,yyy -n abc,def -i map_config_idx\tstart map server\n"
			<< "\t-r\tstart record server\n"
			<< "\t-v\tshow svn revision\n"
			<< " in general, you just user -d arguement to start server.\n" << std::endl;
}

void Daemon_Server::svn_revision(void) {
	std::string s;
	get_version_string(s);
	std::cout << s;
}

int Daemon_Server::parse_cmd_arguments(int argc, char *argv[]) {
	bool switch_daemon_server = false;
	bool switch_chat_server = false;
	bool switch_logic_server = false;
	bool switch_scene_server = false;
	bool switch_record_server = false;
	bool switch_gate_server = false;
	bool switch_player_server = false;
	bool switch_config_check = false;


	char *monitor_name = 0;
	int c = 0, monitor_index = 0;

	while ((c = getopt_long(argc, argv, "vdm:", long_options, NULL)) != -1) {
		switch (c) {
		case 'd': { /// daemon server manager
			switch_daemon_server = true;
			set_server_list(MONITOR_DAEMON);
			break;
		}
		case 'a': { /// daemon server manager
			switch_config_check = true;
			break;
		}
		case 'r': { /// record server
			switch_record_server = true;
			set_server_list(MONITOR_RECORD);
			break;
		}
		case 'g': { /// record server
			switch_gate_server = true;
			set_server_list(MONITOR_GATE);
			break;
		}
		case 'c': { /// chat server
			switch_chat_server = true;
			set_server_list(MONITOR_CHAT);
			break;
		}
		case 'l': { /// logic server
			switch_logic_server = true;
			set_server_list(MONITOR_LOGIC);
			break;
		}
		case 's': { /// scene server
			switch_scene_server = true;
			set_server_list(MONITOR_SCENE);
			break;
		}
		case 'n': { /// monitor name (comment)
			monitor_name = optarg;
			break;
		}
		case 'i': { /// monitor config index
			monitor_index = atoi(optarg);
			break;
		}
		case 'f': { /// flag
			config_path_ = optarg;
			break;
		}
		case 'v': {
			svn_revision();
			exit(0);
			break;
		}
		case '1': {
			log_test(optarg);
			exit(0);
			break;
		}
		case '2': { /// airobot
			switch_player_server = true;
			break;
		}
		case '?': {
			LOG_USER("unknow option '-%c'\n", optopt);
			return 0;
			break;
		}
		case 'm': {
			CONFIG_INSTANCE->load_all_config();
			CONFIG_CACHE->refresh_merge_cache();
			DB_MERGE_SERVER_MANAGER->merge_server();
			exit(0);
			return 0;
			break;
		}
		default: {
			usage();
			return 0;
			break;
		}
		}
	}

	server_index_ = monitor_index;

	if (switch_config_check) {
		std::cout << "config check!!!";
		if (is_client_config()) {
			CONFIG_INSTANCE->load_all_config();
		} else {
			CONFIG_INSTANCE->load_global_config();
			CONFIG_CACHE->refresh_maintainer_cache();
			CONFIG_INSTANCE->init_config(true);
		}

		exit(0);
	}

	// 初始化公共数据
	int_server_common();

	// 启动前操作数据库
	if (switch_daemon_server || this->is_local_debug()) {
		MONGO_OPERATOR->mongo_update_on_start();
		MONGO_OPERATOR->update_db_cross_info();
	}

	if (switch_daemon_server)
		run_daemon_server();

	if (switch_record_server)
		run_record_server(monitor_index);

	if (switch_gate_server)
		run_gate_server(monitor_index);

	if (switch_chat_server)
		run_chat_server(monitor_index);

	if (switch_scene_server)
		run_scene_server(monitor_index);

	if (switch_logic_server)
		run_logic_server(monitor_index);

	if (switch_player_server)
		run_player_server(monitor_index, monitor_name);

	return 0;
}

void Daemon_Server::run_daemon_server(int monitor_index, bool is_restart) {
	/// cond core dump max times
	if (check_core_amount() > 20) {
		remove("./daemon.pid");
		std::cout << "too many core.xxx files!!!!!";
		LOG_ABORT("many core!!!!!!");
	}


	std::string path_str;
	char *path_c;
	path_c = getcwd(NULL, 0);
	path_str.assign(path_c);

	std::string s_path = path_str + exec_name_;
	if((access(s_path.c_str(), F_OK))==-1)   {
		LOG_USER("server not exist!!!");
		exit(0);
	}

	LOG_DEBUG("============================== server start ======================================");

	if (!is_restart) {
		if (daemon(1, 0) != 0) {
			perror("error daemon.../n");
			LOG_ABORT("daemon return != 0");
		}

		// 启动脚本
		std::string command = "sh " + path_str + "/server_init.sh";
		system(command.c_str());
	}

	record_pid_file();

	LOG_DEBUG("============================== server start ======================================");

	Daemon_Server::server_identity_ = CONFIG_CACHE->server_maintainer_cache().server_identity;
	std::string game_server_flag = CONFIG_CACHE->server_maintainer_cache().game_server_flag;

	const Server_Solution_Cache &server_solution = CONFIG_CACHE->server_solution_cache();

	for (Server_Monitor_Vec::const_iterator it = server_solution.monitor_list.begin(); it != server_solution.monitor_list.end(); ++it) {
		if ((*it).run_on != Daemon_Server::server_identity_) continue;

		if (monitor_index >= 0 && monitor_index != (*it).index) continue;

		std::stringstream execname_stream;
		if ((*it).check != "null" && CONFIG_CACHE->server_maintainer_cache().valgrind_allow) {
			mkdir("./valgrind", 0775);
			const char *check_str = (*it).check.c_str();
			char exec_path[PATH_MAX] = {0};
			Date_Time date(Time_Value::gettimeofday());
			std::stringstream date_str;
			date_str << date.year() << "-" << date.month() << "-" << date.day() << "_" << date.hour() << "-" << date.minute() << "-" << date.second();
			snprintf(exec_path, sizeof(exec_path), check_str, date_str.str().c_str());
			execname_stream << exec_path << " ";
		}

		execname_stream << path_str;
		execname_stream << exec_name_ << " " << (*it).command
				<< " --name=" << (*it).name
				<< " --index=" << (*it).index;
				//<< " --flag=" << game_server_flag; // 重启脚本用路径判断

		fork_exec_args(execname_stream.str().c_str(), (*it).index);
		Time_Value::sleep(Time_Value(1, 100000));
	}

}

int Daemon_Server::fork_exec_args(const char *exec_str, int monitor_index) {
	LOG_DEBUG("exec_str = [%s], monitor_index = %d", exec_str, monitor_index);

	std::vector<std::string> exec_str_tok;
	std::istringstream exec_str_stream(exec_str);
	std::copy(std::istream_iterator<std::string>(exec_str_stream), std::istream_iterator<std::string>(), std::back_inserter(exec_str_tok));
	if (exec_str_tok.empty()) {
		LOG_ABORT("exec_str = %s", exec_str);
	}

	const char *pathname = (*exec_str_tok.begin()).c_str();
	std::vector<const char *> vargv;
	for (std::vector<std::string>::iterator tok_it = exec_str_tok.begin(); tok_it != exec_str_tok.end(); ++tok_it) {
		vargv.push_back((*tok_it).c_str());
	}
	vargv.push_back((const char *)0);

	pid_t pid = fork();
	if (pid < 0) {
		LOG_SYS("fork");
		return -1;
	} else if (pid == 0) { /// child
		if (execvp(pathname, (char* const*)&*vargv.begin()) < 0) {
			LOG_SYS("execl");
			LOG_ABORT();
		}
	} else { /// parent
		client_map_.insert(std::make_pair(pid, monitor_index));
	}

	return pid;
}

void Daemon_Server::run_chat_server(int monitor_index) {
	DAEMON_CHAT_INSTANCE->start_server(monitor_index);
}

void Daemon_Server::run_logic_server(int monitor_index) {
	// 创建所有索引
	MONGO_OPERATOR->ensure_all_index();

	DAEMON_LOGIC_INSTANCE->start_server(monitor_index);
}

void Daemon_Server::run_scene_server(int monitor_index) {
	DAEMON_SCENE_INSTANCE->start_server(monitor_index);
}

void Daemon_Server::run_player_server(int monitor_index, char *name) {
	set_server_list(MONITOR_ROBOT);
	DAEMON_ROBOT_INSTANCE->start_server(monitor_index, name);
}

void Daemon_Server::run_record_server(int monitor_index) {

	DAEMON_RECORD_INSTANCE->start_server(monitor_index);
}

void Daemon_Server::run_gate_server(int monitor_index) {
	DAEMON_GATE_INSTANCE->start_server(monitor_index);
}

void Daemon_Server::int_server_common(void) {
	// 先加载主配制
	CONFIG_INSTANCE->load_global_config();
	CONFIG_CACHE->refresh_maintainer_cache();

	// 数据库连接检查
	MONGO_OPERATOR->conn_check();

	// 初始化日志服连接
	RECORD_CLIENT->init(CONFIG_CACHE->server_maintainer_cache().record_id, CONFIG_CACHE->server_maintainer_cache().record_output_type);

	// 初始化配制文件(本地版和Daemon上才检查配制,Daemon上检查一次就够,避免重复检查)
	CONFIG_INSTANCE->init_config(is_local_debug() || has_in_server_list(MONITOR_DAEMON));

	RECORD_CLIENT->set_switcher(CONFIG_CACHE->server_maintainer_cache().record_log_level);

	Lib_Log::instance()->set_switcher(CONFIG_CACHE->server_maintainer_cache().local_log_level);

	// 初始化全局定时器
	GLOBAL_TIMER->thr_create();

	// 初始化数据库
	int db_work_num = CONFIG_CACHE->server_maintainer_cache().db_work_num;
	db_work_num = Utility_Func::get_hash_table_size(db_work_num);
	if (!db_work_num) LOG_ABORT("error work num");
	DB_MANAGER->init(db_work_num);
	DB_MANAGER->start();
	MONGO_OPERATOR->init();
}

void Daemon_Server::record_pid_file(void) {
	FILE *fp = fopen("./daemon.pid", "w");
	if (! fp) {
		LOG_SYS("fopen");
		return;
	}
	char pid_str[128] = {0};
	snprintf(pid_str, sizeof(pid_str), "%d", getpid());
	fputs(pid_str, fp);
	fclose(fp);
}

void Daemon_Server::set_server_list(int server) {
	GUARD(Thread_Mutex, mon, server_list_lock_);
	server_list_ |= server;
}

bool Daemon_Server::has_in_server_list(int server) {
	GUARD(Thread_Mutex, mon, server_list_lock_);
	return (server_list_ & server);
}

bool Daemon_Server::is_local_debug(void) {
	return !access("main.cpp", F_OK);
}

bool Daemon_Server::is_client_config(void) {
	// check json file only;
	DIR *pDir = NULL;
	pDir = opendir("config/cfg");
	if (pDir != NULL) {
		return true;
	}
	return false;
}

void Daemon_Server::del_server_list(int server) {
	GUARD(Thread_Mutex, mon, server_list_lock_);
	LOG_DEBUG("delete server:%d", server);
	server_list_ &= ~server;
}

int Daemon_Server::check_core_amount(bool debug_core) {
	std::string path_str;
	char *path_c;
	path_c = getcwd(NULL, 0);
	path_str.assign(path_c);

	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	pDir = opendir(path_str.c_str());
	if (pDir == NULL) {
		LOG_ABORT("");
	}

	int core_amount = 0;
	while (NULL != (ent = readdir(pDir))) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
			continue;
		}

		if (debug_core) {
			if (strstr(ent->d_name, "core.r")|| strstr(ent->d_name, "core.l"))
				continue;
		}

		if (ent->d_type & DT_DIR) {

		} else {
			if (strstr(ent->d_name, "core.")) {
				core_amount++;
			}
		}
	}

	closedir(pDir);

	return core_amount;
}

int Daemon_Server::send_to_monitor(std::string &ip, int port, Block_Buffer &buf) {
	CHECK_BUFFER_LEN(buf);

	int cid = service_connect_->connect(ip.c_str(), port);
	if (cid >= 2) {
		service_connect_->push_data_block_with_len(cid, buf, false);
	} else {
		LOG_USER("daemon connect error");
		return -1;
	}

	return cid;
}

int Daemon_Server::close_link_by_cid(int cid, Link_Close close_info) {
	LOG_DEBUG("monitor closing client, cid:%d reason:%d, value:%ld", cid, close_info.close_reason, close_info.close_value);

	Svc *svc = service_connect_->find_svc(cid);
	if (svc) {
		svc->handle_close(close_info);
	} else {
		LOG_DEBUG("may be aleady close link cid:%d", cid);
	}
	return 0;
}

std::string Daemon_Server::config_path(void) {
	return config_path_;
}

std::vector<std::string> Daemon_Server::split_str(std::string str, std::string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    uint size=str.size();

    for(uint i=0; i<size; i++) {
        pos=str.find(pattern,i);
        if(pos < size) {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

uint32_t Daemon_Server::file_size_time(std::string log_path) {
	std::string path_str;
	char *path_c;
	path_c = getcwd(NULL, 0);
	path_str.assign(path_c);
	path_str += "/" + log_path;

	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	pDir = opendir(path_str.c_str());
	if (pDir == NULL) {
		LOG_USER("log file now exist!!");
		return 0;
	}

	uint32_t file_size = 0;
	Time_Value now = Time_Value::gettimeofday();
	while (NULL != (ent = readdir(pDir))) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
			continue;
		}

		if (!(ent->d_type & DT_DIR)) {
			FILE * fp;
			int fd;
			struct stat buf;
			std::string file_str;
			file_str.assign(ent->d_name);
			file_str = path_str + "/" + file_str;
			fp = fopen(file_str.c_str(), "r"); //C.zip in current directory, I use it as a test
			fd = fileno(fp);
			fstat(fd, &buf);
			if (buf.st_mtime > now.sec() - 86400 * 3) {
				//latest modification time (seconds passed from 01/01/00:00:00 1970 UTC)
				file_size = buf.st_size > file_size ? buf.st_size : file_size;
			}
			fclose(fp);
		}
	}

	closedir(pDir);

	return file_size;
}

int Daemon_Server::log_test(const char *file_path) {

	int fd = 0;
	if ((fd = open(file_path, O_RDONLY)) < 0) {
		std::cout << "file not exist" << std::endl;
		return 0;
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		perror("fstat"), exit(1);

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		return 0;
	}

	// utf8 bom
	int begin_off = 0;
	if (statbuf.st_size >= 3) {
		if (*((char *)src) == -17 && *((char *)src + 1) == -69 && *((char *)src + 2) == -65) {
			begin_off = 3;
		}
	}

	std::string json_text;
	json_text.assign((char *)src, 3);

	boost::unordered_map<std::string, int> log_map;
	int len = (static_cast<const char *>(src) + statbuf.st_size) - (static_cast<const char *>(src) + begin_off);
	std::cout << len << std::endl;
	int begin = -1, end = -1;
	for (int i = 0; i < len; ++i) {
		if (end == -1 && (i + 2 < len)) {
			if (*(static_cast<char *>(src) + begin_off + i) == 41 && *(static_cast<char *>(src) + begin_off + i + 1) == 91) {
				begin = i;
				i += 20;
				continue;
				if (i >= len) break;
			}
		}

		if (begin >= 0) {
			if (*(static_cast<const char *>(src) + begin_off + i) == 32) {
				end = i;
			}
		}

		if (begin >= 0 && end >= 0) {
			std::string key;
			key.assign(static_cast<char *>(src) + begin_off + begin, (end - begin));
			log_map[key]++;
			end = begin = -1;
		}
	}

	if (munmap(src, statbuf.st_size) < 0) {
		return 0;
	}

	close(fd);

	std::vector<log_data> log_vec;
	for (boost::unordered_map<std::string, int>::iterator it = log_map.begin(); it != log_map.end(); ++it) {
		log_data log;
		log.fun = it->first;
		log.amount = it->second;
		log_vec.push_back(log);
	}

	std::sort(log_vec.begin(), log_vec.end(), Log_Greater);
	for (std::vector<log_data>::iterator it = log_vec.begin(); it != log_vec.end(); ++it) {
		std::cout << (*it).fun << ":" << (*it).amount << std::endl; // prints !!!Hello World!!!
	}

	return 0;
}

void Daemon_Server::get_server_md5(void) {
	int fd = 0;

	std::string path_str;
	char path_c[1024] = {0};
	getcwd(path_c, sizeof(path_c));
	path_str.assign(path_c);
	path_str += exec_name_;
	if ((fd = open(path_str.c_str(), O_RDONLY)) < 0) {
		server_md5_ = path_str;
		return ;
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0) {
		server_md5_ = "fstat失败!";
		return ;
	}

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		server_md5_ = "mmap失败!";
		return ;
	}

    char mine_md5[256 + 1];
    memset(mine_md5, 0x00, 256 + 1);
    const unsigned char *tmp_md5 = MD5((const unsigned char *) src, statbuf.st_size, 0);
    for (uint i = 0; i < 16; i++) {
        sprintf(&mine_md5[i * 2], "%.2x", tmp_md5[i]);
    }

	munmap(src, statbuf.st_size);

	close(fd);

	server_md5_.assign(mine_md5);
}

int Daemon_Server::change_open_time(int open_time) {
	int fd = 0;
	if ((fd = open("../server_global_conf/server_maintainer.json", O_RDONLY)) < 0) {
		std::cout << "file not exist" << std::endl;
		return -1;
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		perror("fstat"), exit(1);

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		return -1;
	}

	// utf8 bom
	int begin_off = 0;
	if (statbuf.st_size >= 3) {
		if (*((char *)src) == -17 && *((char *)src + 1) == -69 && *((char *)src + 2) == -65) {
			begin_off = 3;
		}
	}

	std::string json_text;
	json_text.assign(static_cast<const char *>(src) + begin_off, static_cast<const char *>(src) + statbuf.st_size);

	if (munmap(src, statbuf.st_size) < 0) {
		return -1;
	}

	close(fd);

	std::stringstream time_s;
	time_s << "$1\"game_open_time\":";
	time_s << open_time;
	time_s << ",$3";

	//问号(?)表示之前的可以选择
	std::string phone = "\\G(.*)?\"game_open_time\"([0-9:\\s]*)?,(.*)?";
	boost::regex r(phone);
	//代替
	std::string fmt(time_s.str());
	std::string result = regex_replace(json_text, r, fmt);
	std::cout << result << std::endl;

	FILE *fp = fopen("../server_global_conf/server_maintainer.json", "w");
	if (! fp) {
		return -1;
	}
	fputs(result.c_str(), fp);
	fclose(fp);

	return 0;
}

