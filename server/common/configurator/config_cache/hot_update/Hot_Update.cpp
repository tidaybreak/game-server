/*
 * HotUpdate.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: ti
 */

#include "Hot_Update.h"
#include <dirent.h>
#include <openssl/md5.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "Config_Cache.h"
#include "Logic_Monitor.h"
#include "Daemon_Server.h"

Config_File::Config_File() {}
Config_File::Config_File(const std::string name, const std::string path) {
	file_md5 = name;
	file_path = path;
}

void Config_File::reset(void) {
	file_md5 = "";
	file_path = "";
}

int Config_File::serialize(Block_Buffer &buffer) const {
	buffer.write_string(file_md5);
	buffer.write_string(file_path);
	return 0;
}

int Config_File::deserialize(Block_Buffer &buffer) {
	buffer.read_string(file_md5);
	buffer.read_string(file_path);
	return 0;
}

Hot_Update::Hot_Update() {
	// TODO Auto-generated constructor stub

}

Hot_Update::~Hot_Update() {
	// TODO Auto-generated destructor stub
}

void Hot_Update::run_handler(void) {
	init_all_module();
	init_server_global_conf();

	Time_Value interval = Time_Value(60);
	if (CONFIG_CACHE->server_maintainer_cache().md5_key.empty()) interval = Time_Value(8);

	notify_interval_ = Time_Value(10);
	if (CONFIG_CACHE->server_maintainer_cache().md5_key.empty()) notify_interval_ = Time_Value(2);

	Time_Value::sleep(Time_Value(10));
	while (true) {
		Time_Value::sleep(interval);

		//Time_Value s_time = Time_Value::gettimeofday();
		for (MD5_STR_MAP::iterator it = md5_str_map_.begin(); it != md5_str_map_.end(); ++it) {
			check_config(it->first);
		}

		if (!update_module_.empty()) {
			MD5_STR_MAP::iterator find_it = update_module_.find("map_data");
			if (find_it != update_module_.end()) {
				notice_update(find_it->first, find_it->second);
				update_module_.erase(find_it);
			}

			find_it = update_module_.find("scene");
			if (find_it != update_module_.end()) {
				notice_update(find_it->first, find_it->second);
				update_module_.erase(find_it);
			}

			//全局主配置特殊处理
			find_it = update_module_.find("../../server_global_conf/");
			if(find_it != update_module_.end()) {
				notice_update("server_global_conf", find_it->second);
				update_module_.erase(find_it);
			}

			for (MD5_STR_MAP::iterator it = update_module_.begin(); it != update_module_.end(); ++it) {
				notice_update(it->first, it->second);
			}
			update_module_.clear();
		}

		//Time_Value e_time = Time_Value::gettimeofday();
		//std::cout << "cost sec:" << e_time.sec() - s_time.sec() << "cost usec:" << e_time.usec() - s_time.usec()  << std::endl;
	}
}

int Hot_Update::notice_update(const std::string module, MD5_STR_SET &files) {
	{
		Block_Buffer inner_buf;
		inner_buf.make_message(SERVER_INNER_CONFIG_UPDATE);
		inner_buf.write_string(module);
		inner_buf.write_uint16(files.size());
		for (MD5_STR_SET::iterator it = files.begin(); it != files.end(); ++it) {
			inner_buf.write_string((*it).file_path);
		}
		inner_buf.finish_message();

		Monitor_Mark_Vec list;
		const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
		const Server_Maintainer_Cache &server_maintainer_cache = CONFIG_CACHE->server_maintainer_cache();
		for (Server_Monitor_Vec::const_iterator monitor_it = server_solution_cache.monitor_list.begin(); monitor_it != server_solution_cache.monitor_list.end(); ++monitor_it) {
			//--// 大于10号机不更新
			//--//if ((*monitor_it).run_on > 10) continue;
			//只发自己开服
			if((*monitor_it).run_on != server_maintainer_cache.server_identity) continue;
			// 大于8001号机不更新
			//if ((*monitor_it).run_on >= 8000) continue;
			bool not_find_logic = true;
			for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
				if ((*accept_it).monitor_type == Server_Enum::MONITOR_TYPE::MONITOR_LOGIC){
					list.push_back((*accept_it).monitor_id_order_vec_.front());
					not_find_logic = false;
					break;
				}
			}
			if(not_find_logic){
				for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
					if ((*accept_it).monitor_type == Server_Enum::MONITOR_TYPE::MONITOR_LOGIC ||
							(*accept_it).monitor_type == Server_Enum::MONITOR_TYPE::MONITOR_SCENE ||
							(*accept_it).monitor_type == Server_Enum::MONITOR_TYPE::MONITOR_CHAT ||
							((*accept_it).monitor_type == Server_Enum::MONITOR_TYPE::MONITOR_GATE && module == "server_global_conf")) {
						list.push_back((*accept_it).monitor_id_order_vec_.front());
						break;
					}
				}
			}
		}
		Int_Vec cid_vec;
		for (Monitor_Mark_Vec::iterator it = list.begin(); it != list.end(); ++it) {
			LOG_DEBUG("send config update! module:%s,  ip:%s, port:%d", module.c_str(), it->ip.c_str(), it->port);
			int cid = DAEMON_SERVER_INSTANCE->send_to_monitor(it->ip, it->port, inner_buf);

			if(cid >= 2) {
				cid_vec.push_back(cid);
			}
			sleep(2);
		}
		sleep(3);
		for (Int_Vec::iterator it = cid_vec.begin();  it != cid_vec.begin(); ++it) {
			DAEMON_SERVER_INSTANCE->close_link_by_cid(*it, Link_Close(0, 0));
		}
	}

	return 0;
}

std::string Hot_Update::file_md5(std::string file_name) {
	MD5_CTX md5;
	unsigned char md[16];
	char tmp[33] = {'\0'};
	int length = 0, i = 0;
	char buffer[1024];
	std::string hash = "";
	MD5_Init(&md5);

	int fd = 0;
	if ((fd = open(file_name.c_str(), O_RDONLY)) < 0) {
		return hash;
	}

	/*
	static struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	lock.l_pid = getpid();
	if ( fcntl(fd, F_SETLKW, &lock) == -1 ) {
		close(fd);
		return hash;
	}
	*/

	while (true) {
		length = read(fd, buffer, 1024);
		if (length == 0 || ((length == -1) && (errno != EINTR))) {
			break;
		} else if (length > 0) {
			MD5_Update(&md5, buffer, length);
		}
	}

	/*
	lock.l_type = F_UNLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	lock.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lock);
	*/

	MD5_Final(md, &md5);
	for(i=0; i < 16; i++) {
		sprintf(tmp, "%02X", md[i]);
		hash += (std::string)tmp;
	}
	close(fd);

	return hash;
}

void Hot_Update::check_config(std::string module) {
	MD5_STR_MAP::iterator find_it = md5_str_map_.find(module);
	if (find_it == md5_str_map_.end()) return;

	bool need_once = false;
	MD5_STR_SET md5_str_set, change_file;
	get_md5_str(module, md5_str_set);
	for (MD5_STR_SET::iterator it = md5_str_set.begin(); it != md5_str_set.end(); ++it) {
		MD5_STR_SET::iterator f_find_it = find_it->second.find(*it);
		if (f_find_it == find_it->second.end()) {
			change_file.insert(*it);
			//MSG_DEBUG("config change:%s md5:%s", full_dir_path.c_str(), md5_str.c_str());
			need_once = true;
		}
	}

	if (need_once) {
		update_module_[module] = change_file;
		update_time_ = Time_Value::gettimeofday() + notify_interval_;
		find_it->second = md5_str_set;
	}
}

void Hot_Update::init_server_global_conf(void) {
	std::string module_path = "../../server_global_conf/";
	MD5_STR_SET md5_str_set;
	md5_str_set.clear();
	get_md5_str(module_path, md5_str_set);
	md5_str_map_.insert(std::make_pair(module_path, md5_str_set));
}

void Hot_Update::init_all_module(void) {
	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	std::string module_path = DAEMON_SERVER_INSTANCE->config_path() + "/";
	pDir = opendir(module_path.c_str());
	if (pDir == NULL) {
		MSG_USER("hot update open file error:%s", module_path.c_str());
		//被当作目录，但是执行opendir后发现又不是目录，比如软链接就会发生这样的情况。
		return;
	}
	MD5_STR_SET md5_str_set;
	while (NULL != (ent = readdir(pDir))) {
		if (ent->d_type & DT_DIR) {
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
				continue;
			}
			//MSG_USER("hot update module :%s", ent->d_name);
			//init_md5_str(ent->d_name);
			md5_str_set.clear();
			get_md5_str(ent->d_name, md5_str_set);
			md5_str_map_.insert(std::make_pair(ent->d_name, md5_str_set));
		}
	}

	if (pDir) {
		closedir(pDir);
		pDir = NULL;
	}
}

void Hot_Update::get_md5_str(std::string path, MD5_STR_SET &md5_str_set) {
	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	std::string module_path = DAEMON_SERVER_INSTANCE->config_path() + "/" + path;
	if(path == "../../server_global_conf/") {
		module_path = "../server_global_conf/";
	}
	pDir = opendir(module_path.c_str());
	if (pDir == NULL) {
		//被当作目录，但是执行opendir后发现又不是目录，比如软链接就会发生这样的情况。
		return;
	}
	while (NULL != (ent = readdir(pDir))) {
		if (ent->d_type == 8) {
			if (std::string(ent->d_name).find(".json") != std::string(ent->d_name).length() - 5) {
				continue;
			}

			if (strstr(ent->d_name, ".swp")) continue;

			//directory
			std::string config_name(ent->d_name);
			std::string full_dir_path = module_path + "/" + config_name;

			std::string md5_str = file_md5(full_dir_path);
			if (md5_str != "") {
				md5_str_set.insert(Config_File(md5_str, full_dir_path));
			}
		} else if (ent->d_type & DT_DIR) {
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
				continue;
			}
			get_md5_str(path + "/" +ent->d_name, md5_str_set);
		}
	}

	if (pDir) {
		closedir(pDir);
		pDir = NULL;
	}
}
