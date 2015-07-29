/*
 * Log_Client.h
 *
 *  Created on: May 10, 2012
 *      Author: Ti
 */

#ifndef RECORD_CLIENT_H_
#define RECORD_CLIENT_H_

#include <stdarg.h>
#include "Singleton.h"
#include "Time_Value.h"

class Server_Monitor;

class Record_Client {
public:
	enum RECORD_DEFINE {
		F_STREAM		= 0x10,
		F_USER_TRACE	= 0x8,
		F_USER			= 0x4,
		F_DEBUG			= 0x2,
		F_TRACE_ABORT	= 0x1,

		M_STREAM		= 0,
		M_USER_TRACE	= 1,
		M_USER			= 2,
		M_DEBUG			= 3,
		M_TRACE_ABORT	= 4,

		LOGGING_LOGIC	= 1,
		LOGGING_SCENE	= 2,
		LOGGING_CHAT	= 3,
		LOGGING_GATE	= 4,
		LOGGING_RECORD	= 5,
		LOGGING_ALL		= 10,
		LOGGING_CORE	= 11,

		NULL_STUB		= 13,
	};

	Record_Client(void);
	virtual ~Record_Client(void);

	static int msg_buf_size;
	static int backtrace_size;
	static std::string msg_head[];

	int init(int record_id, int output_type);

	int set_monitor(Server_Monitor *monitor);
	int sync_pid(int type, int sub_type);

	void msg_user(const char *fmt, ...);
	void msg_user_trace(const char *fmt, ...);
	void msg_debug(const char *fmt, ...);
	void msg_stream(const char *fmt, ...);
	void msg_trace_abort(const char *fmt, ...);

	void set_switcher(int switcher);

	void show_msg_time(Time_Value &now);

	int set_cid(int cid);
	int send_to_record_server(Block_Buffer &buf);

private:

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging_file(std::ostringstream &msg_stream, int msg_id);
	void logging_mysql(std::ostringstream &msg_stream);
	int check_core_amount(void);
private:
    Server_Monitor *monitor_;
    int record_id_;

	int output_type_;				// 0:本地输出 1:记录到日志服

	int switcher_;
	Time_Value show_time_;

    int cid_;
    int port_;
    std::string telecom_ip_, unicom_ip_;
};
typedef Singleton<Record_Client> Record_Client_Single;
#define RECORD_CLIENT    (Record_Client_Single::instance())


/// 用户层代码错误信息
#define MSG_USER(FMT, ...) do {						\
		RECORD_CLIENT->msg_user("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误信息(显示堆栈)
#define MSG_USER_TRACE(FMT, ...) do {						\
		RECORD_CLIENT->msg_user_trace("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 调试信息
#define MSG_DEBUG(FMT, ...) do {					\
		RECORD_CLIENT->msg_debug("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 流水信息
#define MSG_STREAM(FMT, ...) do {					\
		RECORD_CLIENT->msg_stream("INSERT INTO "FMT, ##__VA_ARGS__); \
	} while (0)



/// 用这个表示内网要core,外网不要
#define MSG_TRACE_ABORT(FMT, ...) do {						\
		RECORD_CLIENT->msg_trace_abort("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)


#endif /* RECORD_CLIENT_H_ */
