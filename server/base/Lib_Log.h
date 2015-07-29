/*
 * Lib_Log.h
 *
 *  Created on: Oct 24, 2012
 *      Author: Root
 *
 *  记录底层组件日志写本地文件
 *
 */

#ifndef LIB_LOG_H_
#define LIB_LOG_H_

#include "Thread_Mutex.h"
#include "Time_Value.h"
#include <cstdarg>
#include <string>
#include <sstream>

struct Lib_Log_File {
	Lib_Log_File(void): tv(Time_Value::zero), fp(0)	{ }

	Time_Value tv;
	std::string filepath;
	FILE *fp;
};


class Lib_Log {
public:
	enum {
		F_SYS 			= 0x10,
		F_USER_TRACE	= 0x8,
		F_USER			= 0x4,
		F_DEBUG			= 0x2,
		F_TRACE_ABORT	= 0x1,

		M_SYS 			= 0,
		M_USER_TRACE	= 1,
		M_USER			= 2,
		M_DEBUG			= 3,
		M_TRACE_ABORT	= 4,
		M_ABORT			= 5,	// 不受配制控制
		M_TEXT			= 6,	// 不受配制控制

		NULL_STUB		= 9,
	};
	static int msg_buf_size;
	static int backtrace_size;
	static std::string msg_head[];
	static std::string lib_log_dir;

	static Lib_Log *instance(void);
	static void destroy(void);

	void on_flag(int v);
	void off_flag(int v);

	void msg_abort(const char *fmt, ...);
	void msg_sys(const char *fmt, ...);
	void msg_user(const char *fmt, ...);
	void msg_user_trace(const char *fmt, ...);
	void msg_debug(const char *fmt, ...);
	void msg_trace_abort(const char *fmt, ...);
	void msg_text(const char *fmt, ...);

	void set_switcher(int switcher);
	void set_output_type(int output_type);

	void refresh_text_log(void);

	int check_core_amount(void);

private:
	Lib_Log(void);
	virtual ~Lib_Log(void);

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging_file(std::ostringstream &msg_stream, bool out_cout = false, bool file_lock = false);

	void core_file(std::ostringstream &msg_stream);

	void make_lib_log_dir(void);
	void make_log_filepath(std::string &path);

private:
	static Lib_Log *instance_;
	int switcher_;				// 11111111
	int output_type_;			// 0:调试版输出 1:输出到文件

	Lib_Log_File log_file_;
	Thread_Mutex log_lock_;

	Lib_Log_File core_log_file_;
	Thread_Mutex core_log_lock_;

	std::ostringstream text_cache_;
};

////////////////////////////////////////////////////////////////////////////////

inline void Lib_Log::set_switcher(int switcher) {
	switcher_ = switcher;
}

inline void Lib_Log::set_output_type(int output_type) {
	output_type_ = output_type;
}

////////////////////////////////////////////////////////////////////////////////

/// 调用abort产生core文件, 结束程序
#define LOG_ABORT(FMT, ...) do {					\
		Lib_Log::instance()->msg_abort("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 消息内包含errno和对应的错误描述
#define LOG_SYS(FMT, ...) do {						\
		Lib_Log::instance()->msg_sys("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误信息
#define LOG_USER(FMT, ...) do {						\
		Lib_Log::instance()->msg_user("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误跟踪信息
#define LOG_USER_TRACE(FMT, ...) do {						\
		Lib_Log::instance()->msg_user_trace("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用这个表示内网要core,外网不要
#define LOG_TRACE_ABORT(FMT, ...) do {						\
		Lib_Log::instance()->msg_trace_abort("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)


/// 调试信息
#define LOG_DEBUG(FMT, ...) do {					\
		Lib_Log::instance()->msg_debug("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 文本信息,LOG_TEXT_FLUSH手动刷到文件
#define LOG_TEXT(FMT, ...) do {					\
		Lib_Log::instance()->msg_text(" "#FMT, ##__VA_ARGS__); \
	} while (0)
#define LOG_TEXT_FLUSH			\
		Lib_Log::instance()->refresh_text_log(); \


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/// 耗时统计
class Perf_Mon {
public:
	Perf_Mon(std::string name)
		: id_(0), name_(name),tv_(Time_Value::gettimeofday()) { }

	~Perf_Mon(void) {
		Time_Value res_tv = Time_Value::gettimeofday() - tv_;
		LOG_DEBUG("id:%d, name:%s, cost time:%d(ms), %d(us)", id_, name_.c_str(), res_tv.sec() * 1000 + res_tv.usec() / 1000, res_tv.sec() * 1000000 + res_tv.usec());
	}
private:
	int id_;
	std::string name_;
	Time_Value tv_;
};

#define PERF_MON(NAME) \
		Perf_Mon perf_mon(NAME);	\

#endif /* LIB_LOG_H_ */
