/*
 * Auth_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#include "Daemon_Message_Unit.h"

Daemon_Message_Unit::Daemon_Message_Unit(void) {
	pause_ts_.tv_sec = 0;
	pause_ts_.tv_nsec = 8 * 1000 * 1000;
}

Daemon_Message_Unit::~Daemon_Message_Unit(void) {

}


int Daemon_Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {
	int result = 0;
	switch (msg_id) {
	case 1: {

		break;
	}
	default : {
		MSG_USER("error msg_id:%d", msg_id);
		result = 1;
		break;
	}
	}


	return 0;
}

int Daemon_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));



	return 0;
}

int Daemon_Message_Unit::process_async_data(int data_type, void *ptr_data) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_data(data_type, ptr_data));

	return 0;
}

int Daemon_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_buff(msg_id, buf));



	return 0;
}

int Daemon_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));


	return 0;
}

int Daemon_Message_Unit::recover_ptr_body(Unit_Message *ptr_data) {
	JUDGE_CALL_RETURN(Message_Unit::recover_ptr_body(ptr_data));

	if (ptr_data->data_type == Unit_Message::TYPE_DATA_PLAYER) {
		// 逻辑自己回收
		//POOL_MANAGER->push_player_data_pool((Player_Detail *)ptr_data->ptr_data);
	}

	return 0;
}

void Daemon_Message_Unit::show_server_status(void) {
	Message_Unit::show_server_status();
}

