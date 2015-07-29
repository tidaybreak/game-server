/*
 * Record_Client.cpp
 *
 *  Created on: May 10, 2012
 *      Author: Ti
 */

#include <execinfo.h>
#include <errno.h>
#include <iostream>
#include "Record_Struct.h"
#include "Record_Client.h"
#include "Server_Monitor.h"
#include "Config_Cache.h"
#include <dirent.h>

Record_Client::Record_Client(void)
: monitor_(0),
  record_id_(0),
  output_type_(0),
  switcher_(F_USER | F_USER_TRACE | F_DEBUG | F_STREAM),
  cid_(-1),
  port_(0)
{

}

Record_Client::~Record_Client(void) { }

int Record_Client::msg_buf_size = 4096;

std::string Record_Client::msg_head[] = {
		"[M_STREAM] ",			/// M_STREAM 		= 0,
		"[M_USER_TRACE] ",		/// M_USER_TRACE 	= 1,
		"[M_USER] ",			/// M_USER 			= 2,
		"[M_DEBUG] ",			/// M_DEBUG 		= 3,
		"[M_TRACE_ABORT] ",		/// M_TRACE_ABORT	= 4,
		"[NULL]"				/// NULL_STUB		= 5,
};

int Record_Client::backtrace_size = 512;

int Record_Client::init(int record_id, int output_type) {
	record_id_ = record_id;
	output_type_ = output_type;
	Lib_Log::instance()->set_output_type(output_type_);

	return 0;
}

int Record_Client::set_monitor(Server_Monitor *monitor) {
	monitor_ = monitor;

	return 0;
}

int Record_Client::sync_pid(int type, int sub_type) {
	if (output_type_ == 0 || switcher_ == 0) return 0;

	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	if (!server_solution_cache.exist_muid(MONITOR_RECORD, record_id_, 0, CONFIG_CACHE->server_maintainer_cache().server_identity)) {
		return 0;
	}

	Block_Buffer msg_buf;
	msg_buf.make_message(RECORD_SYNC_PID);
	msg_buf.write_int64(pthread_self());
	msg_buf.write_int32(type);
	msg_buf.write_int32(sub_type);
	msg_buf.finish_message();
	return send_to_record_server(msg_buf);
}

void Record_Client::msg_user(const char *fmt, ...) {
	if (switcher_ & F_USER) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_USER, fmt, ap);
		va_end(ap);
	}
}

void Record_Client::msg_user_trace(const char *fmt, ...) {
	if (switcher_ & F_USER_TRACE) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_USER_TRACE, fmt, ap);
		va_end(ap);
	}
}

void Record_Client::msg_trace_abort(const char *fmt, ...) {
	if (switcher_ & F_TRACE_ABORT) {
		LOG_ABORT();
	} else {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_TRACE_ABORT, fmt, ap);
		va_end(ap);
	}
}

void Record_Client::msg_debug(const char *fmt, ...) {
	if (switcher_ & F_DEBUG) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_DEBUG, fmt, ap);
		va_end(ap);
	}
}

void Record_Client::msg_stream(const char *fmt, ...) {
	if (switcher_ & F_STREAM) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_STREAM, fmt, ap);
		va_end(ap);
	}
}

void Record_Client::set_switcher(int switcher) {
	switcher_ = switcher;
}


void Record_Client::assembly_msg(int log_flag, const char *fmt, va_list ap) {
	std::ostringstream msg_stream;

	struct tm tm_v;
	time_t time_v = time(NULL);

	localtime_r(&time_v, &tm_v);

	//Time_Value ms_time = Time_Value::gettimeofday();
	//<< "|tid=" << pthread_self()
	if (log_flag != M_STREAM) {
		msg_stream << "<pid=" << (int)getpid()
				   << ">(" << (tm_v.tm_hour) << ":" << (tm_v.tm_min) << ":" << (tm_v.tm_sec) << ")";
	}

	if(log_flag != M_STREAM){
		msg_stream << msg_head[log_flag];
	}

	char line_buf[msg_buf_size];
	memset(line_buf, 0, sizeof(line_buf));
	vsnprintf(line_buf, sizeof(line_buf), fmt, ap);

	msg_stream << line_buf;


	switch (log_flag) {
	case M_USER: {
		msg_stream << std::endl;
		logging_file(msg_stream, RECORD_LOG_FILE_RECORD);

		break;
	}
	case M_USER_TRACE: {
		int nptrs;
		void *buffer[backtrace_size];
		char **strings;

		nptrs = backtrace(buffer, backtrace_size);
		strings = backtrace_symbols(buffer, nptrs);
		if (strings == NULL)
			return ;

		msg_stream << std::endl;

		for (int i = 0; i < nptrs; ++i) {
			msg_stream << (strings[i]) << std::endl;
		}

		free(strings);

		logging_file(msg_stream, RECORD_LOG_FILE_RECORD);

		break;
	}
	case M_TRACE_ABORT: {
		int core_amount = check_core_amount();
		if (core_amount > 100) break;

		int nptrs;
		void *buffer[backtrace_size];
		char **strings;

		nptrs = backtrace(buffer, backtrace_size);
		strings = backtrace_symbols(buffer, nptrs);
		if (strings == NULL)
			return ;

		msg_stream << std::endl;

		for (int i = 0; i < nptrs; ++i) {
			msg_stream << (strings[i]) << std::endl;
		}

		free(strings);

		logging_file(msg_stream, RECORD_LOG_FILE_CORE);

		break;
	}
	case M_DEBUG: {
		msg_stream << std::endl;
		logging_file(msg_stream, RECORD_LOG_FILE_RECORD);

		break;
	}
	case M_STREAM: {
		msg_stream << std::endl;
		logging_mysql(msg_stream);

		break;
	}
	default: {
		break;
	}
	}

	return ;
}

void Record_Client::logging_file(std::ostringstream &msg_stream, int msg_id) {
	if (output_type_) {
		Record_Info msg;
		msg.log_str = msg_stream.str();

		msg.pid = getpid();
		msg.tid = pthread_self();
		msg.time = time(NULL);

		Block_Buffer msg_buf;
		msg_buf.make_message(msg_id);
		msg.serialize(msg_buf);
		msg_buf.finish_message();

		send_to_record_server(msg_buf);

	} else {
		std::cerr << msg_stream.str();
	}
}

void Record_Client::logging_mysql(std::ostringstream &msg_stream) {
	Record_Info msg;
	msg.log_str = msg_stream.str();

	msg.pid = getpid();
	msg.tid = pthread_self();
	msg.time = time(NULL);

	Block_Buffer msg_buf;
	msg_buf.make_message(RECORD_LOGINOUT_STREAM);
	msg.serialize(msg_buf);
	msg_buf.finish_message();

	send_to_record_server(msg_buf);

}

void Record_Client::show_msg_time(Time_Value &now) {
	if (now < show_time_) return ;

	show_time_ += Time_Value(86400);
}


int Record_Client::set_cid(int cid) {
	cid_ = cid;
	return 0;
}

int Record_Client::send_to_record_server(Block_Buffer &buf) {
	return monitor_->send_to_monitor(MONITOR_RECORD, record_id_, 0, buf);
}

int Record_Client::check_core_amount(void) {
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
