/*
 * File_Record.h
 *
 *  Created on: Aug 29, 2012
 *      Author: Ti
 */

#ifndef FILE_RECORD_H_
#define FILE_RECORD_H_

#include "Time_Value.h"
#include "boost/unordered_map.hpp"
#include "Record_Struct.h"


class File_Record {
public:
	typedef boost::unordered_map<int64_t, Pid_Info> Pid_Info_Map;
	typedef boost::unordered_map<int, File_Info> File_Info_Map;
	typedef boost::unordered_map<int64_t, Buf_File_Info> Buf_File_Info_Map;

	File_Record(void);

	virtual ~File_Record(void);

	int init(void);

	int process_log_file_block(int msg_id, Block_Buffer &buf);

	int process_log_file_core(int msg_id, Block_Buffer &buf);

	int set_pid_info(int64_t pid, int type, int sub_type);

private:
	int get_key_val(int type, int sub_type);

	void make_log_dir(void);

	int make_log_filepath(File_Info &file_info);

	int logging_file(Record_Info &msg);

private:
	int record_output_type_;
    Pid_Info_Map pid_info_map_;
    File_Info global_file_;
    File_Info core_file_;
	File_Info_Map file_info_map_;
	Buf_File_Info_Map buf_file_info_map_;
};

////////////////////////////////////////////////////////////////////////////////



#endif /* FILE_RECORD_H_ */
