/*
 * Record_Struct.h
 *
 *  Created on: Dec 18, 2013
 *      Author: ti
 */

#ifndef RECORD_STRUCT_H_
#define RECORD_STRUCT_H_

#include "Block_Buffer.h"

enum Record_Type {
	RECORD_LOG_FILE_RECORD 		= 700061,				//记录日志文件
	RECORD_LOG_FILE_CORE 		= 700062,				//生成core日志
	RECORD_LOGINOUT_STREAM 		= 700063,
	RECORD_BACK_DB_TEST 		= 700064,
	RECORD_SYNC_PID		 		= 700065,

};

struct Record_Info {
	int32_t pid;/*进程id*/
	int64_t tid;/*线程id*/
	uint32_t time;/**/
	std::string log_str;/**/
	Record_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(pid);
		w.write_int64(tid);
		w.write_uint32(time);
		w.write_string(log_str);
	}

	int deserialize(Block_Buffer & r) {
		if(r.read_int32(pid)  ||  r.read_int64(tid)  ||  r.read_uint32(time)  ||  r.read_string(log_str)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pid = 0;
		tid = 0;
		time = 0;
		log_str.clear();
	}
};

struct Pid_Info {
	int type;
	int sub_type;

	Pid_Info(void) : type(0), sub_type(0) {};
	Pid_Info(int type_, int sub_type_) : type(type_), sub_type(sub_type_) {};

};

struct File_Info {
	int type;
	int sub_type;
	Time_Value tv;
	std::string file_path;
	FILE *fp;

	File_Info(void) : type(0), sub_type(0), fp(0) {};
};

struct Buf_File_Info {
	int type;
	int sub_type;
	Time_Value tv;
	std::string file_path;
	std::string file_name;
	FILE *fp;
};

#endif /* RECORD_STRUCT_H_ */
