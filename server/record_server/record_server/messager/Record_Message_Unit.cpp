/*
 * Record_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#include "Record_Message_Unit.h"
#include "Record_Monitor.h"
#include "db_mysql/Mysql_Operator.h"

Record_Message_Unit::Record_Message_Unit(void) {

}

Record_Message_Unit::~Record_Message_Unit(void) {

}

int Record_Message_Unit::register_client_message(void) {

	return 0;
}

int Record_Message_Unit::register_inner_message(void) {

	return 0;
}

int Record_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));

	switch (msg_id) {
	case RECORD_SYNC_PID: {
		int64_t pid = 0;
		int type = 0, sub_type = 0;
		buf.read_int64(pid);
		buf.read_int32(type);
		buf.read_int32(sub_type);
		monitor()->record_monitor()->file_record().set_pid_info(pid, type, sub_type);
		break;
	}
	case RECORD_LOG_FILE_RECORD: {
		monitor()->record_monitor()->file_record().process_log_file_block(msg_id, buf);
		break;
	}
	case RECORD_LOG_FILE_CORE: {
		monitor()->record_monitor()->file_record().process_log_file_core(msg_id, buf);

		break;
	}
	case RECORD_LOGINOUT_STREAM: {
		MYSQL_OPERATOR->execute_collector(buf);
		break;
	}
	default: {
		break;
	}
	}

	return 0;
}

int Record_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_buff(msg_id, buf));

	switch (msg_id) {
	case 1: {
		break;
	}
	default : {
		return CALL_CONTINUE;
		break;
	}
	}

	return 0;
}

int Record_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));
	switch (msg_id) {
		case TIMER_GLOBAL_MONITOR_SELF: {
			MYSQL_OPERATOR->commit();
			break;
		}
		default: {
			MSG_USER("unknow msg:%d", msg_id);
			break;
		}
	}
	return 0;
}
