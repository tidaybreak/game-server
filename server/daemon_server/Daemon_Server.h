// -*- C++ -*-
/*
 * Daemon_Server.h
 *
 *  Created on: Apr 19, 2012
 *      Author: Ti
 */

#ifndef DAEMON_SERVER_H_
#define DAEMON_SERVER_H_

#include "boost/unordered_map.hpp"
#include <vector>
#include "Event_Handler.h"
#include "Thread_Mutex.h"
#include "messager/Daemon_Message_Unit.h"

class Epoll_Watcher;
class Daemon_Base;
class Message_Unit;
class Service_Connect;

class Daemon_Server: public Event_Handler {
	typedef boost::unordered_map<int, int> Client_Map;

public:
    enum SERVER_TYPE {
    	MONITOR_ROBOT	= 0x00,		// 正常不能用0x00,反正机器人这个没用,先这样
    	MONITOR_LOGIC	= 0x01,
    	MONITOR_SCENE	= 0x02,
    	MONITOR_CHAT	= 0x04,
    	MONITOR_GATE	= 0x08,
    	MONITOR_RECORD	= 0x10,
    	MONITOR_DAEMON	= 0x20,
    	MONITOR_CLOSING	= 0x40,
    	MONITOR_SHUTDOWN	= 0x80
    };

	const static int max_core_dump_num = 5;

	static Daemon_Server *instance(void);

	int init(int argc, char *argv[]);
	int start(int argc, char *argv[]);

	void del_server_list(int server);

	bool has_in_server_list(int server);

	int check_core_amount(bool debug_core = true);

	virtual int handle_timeout(const Time_Value &tv);

	void set_server_list(int server);

	int send_to_monitor(std::string &ip, int port, Block_Buffer &buf);

	int close_link_by_cid(int cid, Link_Close close_info);

	std::vector<std::string> split_str(std::string str, std::string pattern);

	std::string config_path(void);

	void get_server_md5(void);

	int change_open_time(int open_time);


private:
	Daemon_Server(void);
	~Daemon_Server(void);
	Daemon_Server(const Daemon_Server &);
	const Daemon_Server &operator=(const Daemon_Server &);

	void record_pid_file(void);

	int fork_exec_args(const char *exec_str, int monitor_index);

	int parse_cmd_arguments(int argc, char* argv[]);

	void usage(void);

	void svn_revision(void);

	static void sigcld_handle(int signo);
	void restart_process(int pid);

	void run_daemon_server(int monitor_index = -1, bool is_restart = false);
	void run_record_server(int monitor_index = 0);
	void run_gate_server(int monitor_index = 0);
	void run_chat_server(int monitor_index = 0);
	void run_logic_server(int monitor_index = 0);
	void run_scene_server(int monitor_index = 0);
	void run_player_server(int monitor_index = 0, char *name = 0);
	void int_server_common(void);

	bool is_local_debug(void);
	bool is_client_config(void);

	void send_to_center(void);

	uint32_t file_size_time(std::string log_path);
	int log_test(const char *file_path);

private:
	static Daemon_Server *instance_;
	static std::string exec_name_;
	static int server_identity_;
	static struct option long_options[];

	int server_list_;
	Thread_Mutex server_list_lock_;
	std::string config_path_;
	Epoll_Watcher *wait_watcher_;
	Message_Unit *unit_;
	Service_Connect *service_connect_;

	Client_Map client_map_, scene_server_map_, chat_server_map_, core_dump_num_;

public:
	int server_index_;
	Time_Value start_time_;
	std::string path_name_;
	std::string c_ip_;
	int c_port_;
	int c_cid_;
	long mongod_pid_;
	Time_Value send_center_sleep_;
	Time_Value log_file_tick_;
	std::string logic_ip_port_;
	std::string mongo_ip_port_;
	std::string server_md5_;

	bool change_solution_restart_;		// 判断是否是改方案时重启
};

#define DAEMON_SERVER_INSTANCE Daemon_Server::instance()

#endif /* DAEMON_SERVER_H_ */
