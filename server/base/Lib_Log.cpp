/*
 * Lib_Log.cpp
 *
 *  Created on: Oct 24, 2012
 *      Author: Root
 */

#include "Lib_Log.h"
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include <stdarg.h>
#include <fcntl.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include<sys/file.h>
#include <dirent.h>

Lib_Log::Lib_Log(void)
: switcher_(F_SYS | F_USER | F_USER_TRACE | F_DEBUG | F_TRACE_ABORT), output_type_(0)
{ }

Lib_Log::~Lib_Log(void) { }

Lib_Log *Lib_Log::instance_ = 0;

int Lib_Log::msg_buf_size = 4096;

int Lib_Log::backtrace_size = 512;

std::string Lib_Log::msg_head[] = {
		"[M_SYS] ",			/// M_SYS 			= 0,
		"[M_USER_TRACE] ",	/// M_USER_TRACE 	= 1,
		"[M_USER] ",		/// M_USER			= 2,
		"[M_DEBUG] ",		/// M_DEBUG			= 3
		"[M_TRACE_ABORT] ",	/// M_TRACE_ABORT	= 4,
		"[M_ABORT] ",		/// M_ABORT			= 5,
		"[M_TEXT] ",		/// M_TEXT			= 6,
		"[NULL]"			/// NULL_STUB,
};

std::string Lib_Log::lib_log_dir = "./local_log";

Lib_Log *Lib_Log::instance(void) {
	if (! instance_)
		instance_ = new Lib_Log;
	return instance_;
}

void Lib_Log::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Lib_Log::on_flag(int v) {
	switcher_ |= v;
}

void Lib_Log::off_flag(int v) {
	switcher_ &= (~v);
}

void Lib_Log::msg_abort(const char *fmt, ...) {
	va_list	ap;

	va_start(ap, fmt);
	assembly_msg(M_ABORT, fmt, ap);
	va_end(ap);
}

void Lib_Log::msg_sys(const char *fmt, ...) {
	if (switcher_ & F_SYS) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_SYS, fmt, ap);
		va_end(ap);
	}
}

void Lib_Log::msg_user(const char *fmt, ...) {
	if (switcher_ & F_USER) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_USER, fmt, ap);
		va_end(ap);
	}
}

void Lib_Log::msg_user_trace(const char *fmt, ...) {
	if (switcher_ & F_USER) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_USER_TRACE, fmt, ap);
		va_end(ap);
	}
}

void Lib_Log::msg_trace_abort(const char *fmt, ...) {
	if (switcher_ & F_TRACE_ABORT) {
		LOG_ABORT();
	} else {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_TRACE_ABORT, fmt, ap);
		va_end(ap);
	}
}

void Lib_Log::msg_debug(const char *fmt, ...) {
	if (switcher_ & F_DEBUG) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_DEBUG, fmt, ap);
		va_end(ap);
	}
}

void Lib_Log::msg_text(const char *fmt, ...) {

	va_list	ap;

	va_start(ap, fmt);
	assembly_msg(M_TEXT, fmt, ap);
	va_end(ap);

}

void Lib_Log::assembly_msg(int log_flag, const char *fmt, va_list ap) {

	std::ostringstream msg_stream;

	struct tm tm_v;
	time_t time_v = time(NULL);

	localtime_r(&time_v, &tm_v);

	//Time_Value ms_time = Time_Value::gettimeofday();

	if (1) {
		msg_stream << "<pid=" << (int)getpid()
				<< ">(" << (tm_v.tm_hour) << ":" << (tm_v.tm_min) << ":" << (tm_v.tm_sec) << ")";
	}

	msg_stream << msg_head[log_flag];

	char line_buf[msg_buf_size];
	memset(line_buf, 0, sizeof(line_buf));
	vsnprintf(line_buf, sizeof(line_buf), fmt, ap);

	msg_stream << line_buf;

	switch (log_flag) {
	case M_ABORT: {
		msg_stream << std::endl;
		logging_file(msg_stream, true);
		sleep(3);
		abort();

		break;
	}
	case M_SYS: {
		msg_stream << ", errno = " << errno;

		memset(line_buf, 0, sizeof(line_buf));
		msg_stream << ", errstr=[" << (strerror_r(errno, line_buf, sizeof(line_buf))) << "]" << std::endl;

		logging_file(msg_stream);

		break;
	}
	case M_USER: {
		msg_stream << std::endl;
		logging_file(msg_stream);

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

		logging_file(msg_stream);

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

		logging_file(msg_stream);
		core_file(msg_stream);

		break;
	}
	case M_DEBUG: {
		msg_stream << std::endl;
		logging_file(msg_stream);

		break;
	}
	case M_TEXT: {
		msg_stream << std::endl;
		text_cache_ << msg_stream.str();

		break;
	}
	default: {
		break;
	}
	}


	return ;
}

void Lib_Log::make_lib_log_dir(void) {
	int ret = mkdir(lib_log_dir.c_str(), 0775);
	if (ret == -1 && errno != EEXIST) {
		perror("mkdir error");
	}
}

void Lib_Log::make_log_filepath(std::string &path) {
	time_t time_v = time(NULL);
	struct tm tm_v;
	localtime_r(&time_v, &tm_v);

	std::stringstream stream;
	stream 	<< lib_log_dir << "/" << (tm_v.tm_year + 1900) << "-" << (tm_v.tm_mon + 1) << "-" << (tm_v.tm_mday)
			<< "-" << (tm_v.tm_hour) << ".log";

	path = stream.str().c_str();
}

void Lib_Log::logging_file(std::ostringstream &msg_stream, bool out_cout, bool file_lock) {
	if (output_type_) {
		GUARD(Thread_Mutex, mon, log_lock_);

		if (! log_file_.fp) {
			make_lib_log_dir();
			make_log_filepath(log_file_.filepath);

			if ((log_file_.fp = fopen(log_file_.filepath.c_str(), "a")) == NULL) {
				printf("filepath=[%s]\n", log_file_.filepath.c_str());
				perror("fopen error");
				return;
			}
			log_file_.tv = Time_Value::gettimeofday();
		} else {
			if (! is_same_hour(log_file_.tv, Time_Value::gettimeofday())) {
				fclose(log_file_.fp);
				log_file_.tv = Time_Value::gettimeofday();
				make_lib_log_dir();
				make_log_filepath(log_file_.filepath);

				if ((log_file_.fp = fopen(log_file_.filepath.c_str(), "a")) == NULL) {
					printf("filepath=[%s]", log_file_.filepath.c_str());
					perror("fopen error");
					return;
				}
			}
		}

		if (file_lock) {
			flock(log_file_.fp->_fileno, LOCK_EX);
			fputs(msg_stream.str().c_str(), log_file_.fp);
			fflush(log_file_.fp);
			flock(log_file_.fp->_fileno, LOCK_UN);
		} else {
			fputs(msg_stream.str().c_str(), log_file_.fp);
			fflush(log_file_.fp);
		}
	} else {
		std::cerr << msg_stream.str();

		if (out_cout)
			std::cout << msg_stream.str();

	}
}

void Lib_Log::core_file(std::ostringstream &msg_stream) {
	GUARD(Thread_Mutex, mon, core_log_lock_);

	if (!core_log_file_.fp) {
		core_log_file_.tv = Time_Value::gettimeofday();

		time_t time_v = time(NULL);
		struct tm tm_v;
		localtime_r(&time_v, &tm_v);

		std::stringstream stream;
		stream << "core.l-" << (tm_v.tm_year + 1900) << "-" << (tm_v.tm_mon + 1) << "-" << (tm_v.tm_mday)
				<< "-" << (tm_v.tm_hour) << "-" << (tm_v.tm_min) << "-" << (tm_v.tm_sec) << ".log";

		core_log_file_.filepath = stream.str().c_str();

		if ((core_log_file_.fp = fopen(core_log_file_.filepath.c_str(), "a")) == NULL) {
			perror("fopen error");
			return ;
		}
	}

	fputs(msg_stream.str().c_str(), core_log_file_.fp);
	fflush(core_log_file_.fp);

	fclose(core_log_file_.fp);
	core_log_file_.fp = NULL;
}

void Lib_Log::refresh_text_log(void) {

	logging_file(text_cache_, false, true);
	text_cache_.str("");
}

int Lib_Log::check_core_amount(void) {
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
