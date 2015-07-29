/*
 * File_Record.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: Ti
 */

#include "File_Record.h"
#include "Record_Client.h"
#include "Time_Value.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <cstdio>
#include <string>
#include <sstream>
#include "Config_Cache.h"



File_Record::File_Record(void) {

	init();
}

File_Record::~File_Record(void) { }

int File_Record::init(void) {
	global_file_.tv = Time_Value::gettimeofday();
	global_file_.type = Record_Client::LOGGING_ALL;
	record_output_type_ = CONFIG_CACHE->server_maintainer_cache().record_output_type;

	if (record_output_type_ == 2) {
		make_log_dir();
		make_log_filepath(global_file_);
		if ((global_file_.fp = fopen(global_file_.file_path.c_str(), "a")) == NULL) {
			perror("fopen error");
			return -1;
		}
	}

	return 0;
}

void File_Record::make_log_dir(void) {
	int ret = mkdir("./record_file", 0775);
	if (ret == -1 && errno != EEXIST) {
		perror("mkdir error");
	}
}

int File_Record::make_log_filepath(File_Info &file_info) {
	time_t time_v = file_info.tv.sec();
	struct tm tm_v;
	localtime_r(&time_v, &tm_v);

	file_info.file_path.clear();

	switch (file_info.type) {
	case Record_Client::LOGGING_CHAT: {
		file_info.file_path.append("./record_file/chat-");
		break;
	}
	case Record_Client::LOGGING_LOGIC: {
		file_info.file_path.append("./record_file/logic-");
		break;
	}
	case Record_Client::LOGGING_SCENE: {
		file_info.file_path.append("./record_file/scene-");
		break;
	}
	case Record_Client::LOGGING_RECORD: {
		file_info.file_path.append("./record_file/record-");
		break;
	}
	case Record_Client::LOGGING_GATE: {
		file_info.file_path.append("./record_file/gate-");
		break;
	}
	case Record_Client::LOGGING_ALL: {
		file_info.file_path.append("./record_file/all-");
		break;
	}
	case Record_Client::LOGGING_CORE: {
		file_info.file_path.append("./core.r-");
		break;
	}
	default: {
		file_info.file_path.append("./record_file/misc-");
		break;
	}
	}

	std::stringstream stream;
	if (file_info.type == Record_Client::LOGGING_SCENE ||
			file_info.type == Record_Client::LOGGING_GATE) {
		stream << (file_info.sub_type) << "-";
	}

	if (file_info.type == Record_Client::LOGGING_CORE) {
		stream 	<< (tm_v.tm_year + 1900) << "-" << (tm_v.tm_mon + 1) << "-" << (tm_v.tm_mday)
			<< "-" << (tm_v.tm_hour) << "-" << (tm_v.tm_min) << "-" << (tm_v.tm_sec) << ".log";
	} else {
		stream 	<< (tm_v.tm_year + 1900) << "-" << (tm_v.tm_mon + 1) << "-" << (tm_v.tm_mday)
			<< "-" << (tm_v.tm_hour) << ".log";
	}

	file_info.file_path.append(stream.str());

	return 0;
}

int File_Record::get_key_val(int type, int sub_type) {
	return type * 1000 + sub_type;
}

int File_Record::logging_file(Record_Info &msg) { //int type, std::string &log_str) {
	File_Info *file_info = 0;
	int type = 0, sub_type = 0;
	Pid_Info_Map::iterator pid_find_it = pid_info_map_.find(msg.tid);
	if (pid_find_it != pid_info_map_.end()) {
		type = pid_find_it->second.type;
		sub_type = pid_find_it->second.sub_type;
	}

	int key = get_key_val(type, sub_type);

	File_Info_Map::iterator file_it = file_info_map_.find(key);
	if (file_it == file_info_map_.end()) {
		make_log_dir();

		File_Info info;
		info.type = type;
		info.sub_type = sub_type;

		info.tv = Time_Value::gettimeofday();
		make_log_filepath(info);

		if ((info.fp = fopen(info.file_path.c_str(), "a")) == NULL) {
			perror("fopen error");
			return -1;
		}

		file_info_map_[key] = info;
		file_info = &(file_info_map_[key]);
	} else {
		file_info = &(file_it->second);
		if (! is_same_hour(file_it->second.tv, Time_Value::gettimeofday())) {
			fclose(file_it->second.fp);
			file_it->second.tv = Time_Value::gettimeofday();
			make_log_filepath(file_it->second);

			if ((file_it->second.fp = fopen(file_it->second.file_path.c_str(), "a")) == NULL) {
				LOG_USER("fopen error, %s", file_it->second.file_path.c_str());
				perror("fopen error");
				return -1;
			}
		}
	}

	fputs(msg.log_str.c_str(), file_info->fp);
	fflush(file_info->fp);

	if (record_output_type_ == 2) {
		if (! is_same_hour(global_file_.tv, Time_Value::gettimeofday())) {
			fclose(global_file_.fp);
			global_file_.tv = Time_Value::gettimeofday();
			make_log_filepath(global_file_);

			if ((global_file_.fp = fopen(global_file_.file_path.c_str(), "a")) == NULL) {
				LOG_USER("fopen error, %s", global_file_.file_path.c_str());
				perror("fopen error");
				return -1;
			}
		}

		if (global_file_.fp) {
			fputs(msg.log_str.c_str(), global_file_.fp);
			fflush(global_file_.fp);
		}
	}

	return 0;
}

int File_Record::process_log_file_block(int msg_id, Block_Buffer &buf) {
	Record_Info msg;
	msg.deserialize(buf);
	logging_file(msg);

	return 0;
}

int File_Record::process_log_file_core(int msg_id, Block_Buffer &buf) {
	Record_Info msg;
	msg.deserialize(buf);
	logging_file(msg);

	if (!core_file_.fp) {
		core_file_.type = Record_Client::LOGGING_CORE;
		core_file_.tv = Time_Value::gettimeofday();
		make_log_filepath(core_file_);

		if ((core_file_.fp = fopen(core_file_.file_path.c_str(), "a")) == NULL) {
			perror("fopen error");
			return -1;
		}
	}

	fputs(msg.log_str.c_str(), core_file_.fp);
	fflush(core_file_.fp);

	fclose(core_file_.fp);
	core_file_.fp = NULL;

	return 0;
}

int File_Record::set_pid_info(int64_t pid, int type, int sub_type) {
	pid_info_map_[pid] = Pid_Info(type, sub_type);

	return 0;
}
