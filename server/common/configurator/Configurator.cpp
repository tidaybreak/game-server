// -*- C++ -*-
/*
 * Configurator.cpp
 *
 *  Created on: May 3, 2012
 *      Author: Ti
 */

#include <dirent.h>
#include "Configurator.h"
#include "Mutex_Guard.h"
#include "Config_Cache.h"
#include "Daemon_Server.h"

Configurator::Configurator(void) {

}

Configurator::~Configurator(void) { }

int Configurator::init_config(bool c_check) {
	load_config();
	CONFIG_CACHE->refresh_all_config_cache("");

	if (c_check)
		CONFIG_CACHE->check_all_config_cache();

	return 0;
}

int Configurator::update_config(String_Vec &files, std::string module) {
	load_all_config(files, module);

	CONFIG_CACHE->refresh_all_config_cache(module);

	if (module != "")
		module_update_times[module]++;

	return 0;
}

int Configurator::load_json_file(const char *file_path, Json::Value &conf_value) {
	int fd = 0;
	if ((fd = open(file_path, O_RDONLY)) < 0) {
		LOG_SYS("open");
		LOG_ABORT();
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		perror("fstat"), exit(1);

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		LOG_SYS("mmap");
		LOG_ABORT();
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

	if (! reader_.parse(static_cast<const char *>(src) + begin_off, static_cast<const char *>(src) + statbuf.st_size, conf_value)) {
        LOG_ABORT("Failed to parse file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	if (munmap(src, statbuf.st_size) < 0) {
		LOG_SYS("munmap");
		LOG_ABORT();
	}

	close(fd);

	return 0;
}

int Configurator::load_json_int_map(const std::string &dir, JsonKey_Func func, Config_Entry &config) {
	config.get_int_map_json().clear();
	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	pDir = opendir(dir.c_str());
	if (pDir == NULL) {
		//被当作目录，但是执行opendir后发现又不是目录，比如软链接就会发生这样的情况。
		return 0;
	}
	Json::Value json;
	while (NULL != (ent = readdir(pDir))) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
			continue;
		}

		if (!strstr(ent->d_name, ".json")) continue;

		if (strstr(ent->d_name, ".swp")) continue;

		if (strstr(ent->d_name, ".bak")) continue;

		//directory
		int key = 0;
		std::string file_name(ent->d_name);
		std::string full_dir_path = dir + file_name;
		json.clear();
		load_json_file(full_dir_path.c_str(), json);
		func(json, key);
		if (!config.get_int_map_json().insert(std::make_pair(key, json)).second) {
			LOG_ABORT();
		}
	}
	if (pDir) {
		closedir(pDir);
		pDir = NULL;
	}

	return 0;
}

const Json::Value &Configurator::config_json(void) {
	return config_json_;
}

const Json::Value &Configurator::open_solution(void) {
	return open_solution_;
}

void Configurator::load_all_config(Json::Value &json, std::string path) {
	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	pDir = opendir(path.c_str());
	if (pDir == NULL) {
		LOG_ABORT("");
	}

	while (NULL != (ent = readdir(pDir))) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
			continue;
		}

		if (ent->d_type & DT_DIR) {
			load_all_config(json[ent->d_name], path + "/" + ent->d_name);
		} else {
			if (strstr(ent->d_name, ".json") && !strstr(ent->d_name, ".swp")) {
				Json::Value json_tmp;
				std::string file_path(path + "/" + ent->d_name);
				load_json_file(file_path.c_str(), json_tmp);
				std::string name = std::string(ent->d_name).substr(0, std::string(ent->d_name).length() - 5);
				json[name] = (json_tmp);
			}
		}
	}

	closedir(pDir);
}

int Configurator::load_all_config(String_Vec &file_path, std::string module) {
	PERF_MON("load json file_path");

	// 方案和场景要一起更
	if (module == "server" || module == "scene") {
		load_open_solution();
		load_all_config(config_json_["server"], DAEMON_SERVER_INSTANCE->config_path() + "/server");
	}

	// 更新地图时先清空数据,剩下变化的,加载会快点
	if (module == "map_data") {
		config_json_["map_data"] = Json::Value::null;
	}

	// 只更新变化的文件
	for (String_Vec::iterator it = file_path.begin(); it != file_path.end(); ++it) {
		String_Vec json_deep = split(*it, "/");
		json_deep.pop_back();
		std::string path;
		Json::Value *json_tmp = &config_json_;
		uint i = 1;
		for (String_Vec::iterator d_it = json_deep.begin(); d_it != json_deep.end(); ++d_it, ++i) {
			if (*d_it != DAEMON_SERVER_INSTANCE->config_path())
				json_tmp = &((*json_tmp)[*d_it]);
			path += *d_it;
			if (i != json_deep.size())
				path += "/";
		}
		load_all_config(*json_tmp, path);
	}

	return 0;
}

int Configurator::load_module_config(std::string module, String_Vec &files) {
	if (!config_json_.isMember(module)) {
		return -1;
	}

	std::string path = DAEMON_SERVER_INSTANCE->config_path() + "/" + module;
	struct dirent *ent = NULL;
	DIR *pDir = NULL;
	pDir = opendir(path.c_str());
	if (pDir == NULL) {
		LOG_ABORT("");
	}

	while (NULL != (ent = readdir(pDir))) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".svn") == 0) {
			continue;
		}

		if (ent->d_type & DT_DIR) {

		} else {
			if (strstr(ent->d_name, ".json") && !strstr(ent->d_name, ".swp")) {
				std::string file_path(path + "/" + ent->d_name);
				files.push_back(file_path);
				break;
			}
		}
	}

	closedir(pDir);

	return 0;
}

int Configurator::load_config(void) {
	PERF_MON("load json cost");

	load_all_config();

	load_open_solution();

	return 0;
}

int Configurator::load_all_config(void) {
	load_all_config(config_json_, DAEMON_SERVER_INSTANCE->config_path());

	return 0;
}

int Configurator::load_global_config(void) {
	load_all_config(config_json_["server_global_conf"], "../server_global_conf");

	return 0;
}

int Configurator::load_open_solution(void) {
	{ 	// server_solution_x.json
		Config_Entry solution_config;
		std::string path("./config/server/open_solution/");
		load_json_int_map(path, get_json_dir_solution_id, solution_config);
		int game_solution_id = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_maintainer"]["game_solution_id"].asInt();
		Int_Json_Map::iterator find_it = solution_config.get_int_map_json().find(game_solution_id);
		if (find_it != solution_config.get_int_map_json().end()) {
			int inter_solution_id = find_it->second["inter_solution_id"].asInt();
			Int_Json_Map::iterator inter_find_it = solution_config.get_int_map_json().find(inter_solution_id);
			if (inter_find_it != solution_config.get_int_map_json().end()) {
				for (uint i = 0; i < (inter_find_it->second)["monitor_list"].size(); ++i) {
					find_it->second["monitor_list"].append((inter_find_it->second)["monitor_list"][i]);
				}
			} else {
				LOG_DEBUG("can not find inter solution:%d", inter_solution_id);
			}
			open_solution_ = find_it->second;
		} else {
			LOG_ABORT("can not find solution:%d", game_solution_id);
		}
	}
	return 0;
}

//字符串分割函数
String_Vec Configurator::split(std::string str,std::string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    uint size=str.size();
    for(uint i=0; i<size; i++) {

        pos=str.find(pattern,i);
        if(pos<size) {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}
