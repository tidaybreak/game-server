/*
 * Auth_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#include "Robot_Message_Unit.h"
#include "Robot_Monitor.h"
#include "Pool_Manager.h"
#include "Robot_Player.h"
#include "Robot_Client_Request.h"
#include "Robot_Server_Request.h"
#include "Robot_Timer_Request.h"
#include "Robot_Def.h"

Robot_Message_Unit::Robot_Message_Unit(void) {

}

Robot_Message_Unit::~Robot_Message_Unit(void) {

}

Robot_Monitor *Robot_Message_Unit::monitor(void) {
	return Message_Unit::monitor()->robot_monitor();
}

int Robot_Message_Unit::register_client_message(void) {

	return 0;
}

int Robot_Message_Unit::process_inner_message(const uint32_t msg_id, const int32_t cid, Block_Buffer &buf) {
	Inner_Func_Map::iterator mapi = inner_message_func_.find(msg_id);
	if (mapi == inner_message_func_.end()) {
		return -1;
	}
	return mapi->second(cid, buf);
}

int Robot_Message_Unit::register_client_process(const uint32_t msg_id, Client_Func callback) {
	Client_Func_Map::iterator find_it = client_message_func_.find(msg_id);
	if (find_it != client_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	client_message_func_[msg_id] = callback;
	return 0;
}

int Robot_Message_Unit::process_client_message(Robot_Player &player, const uint32_t msg_id, Block_Buffer &buf) {
	Client_Func_Map::iterator mapi = client_message_func_.find(msg_id);
	if (mapi == client_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return 10000002;
	}
	return mapi->second(player, buf);
}

int Robot_Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {
	Robot_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) {
		if (msg_id == 10400001) {
			return ROBOT_CLIENT_REQUEST->process_player_login(cid, buf);
		} else if (msg_id != 99999999) {
			MSG_USER("error cid:%d msg_id:%d", cid, msg_id);
		} else {
			MSG_USER("error cid:%d msg_id:%d", cid, msg_id);
			return 0;
		}
	}


	int msg_ret = player->verify_msg_detail(serial_cipher, msg_time_cipher);
	if (msg_ret) {
		player->notice_logic_sign_out(msg_ret);
		MSG_USER("close client msg:%d", msg_id);
		return 0;
	}

	switch (get_msg_type(msg_id)) {
	case CLIENT_TO_LOGIC: {
		remake_buffer(msg_id, player->role_id(), buf);
		//player->send_to_logic(buf);
		break;
	}
	case CLIENT_TO_SCENE: {
		remake_buffer(msg_id, player->role_id(), buf);
		//player->send_to_scene(buf);
		break;
	}
	case CLIENT_TO_RECORD: {

		break;
	}
	case CLIENT_HEART: {
		// 更新客户端时间
		int timestamp = Time_Value::gettimeofday().sec();
		Block_Buffer t_buf;
		t_buf.make_message(80000010);
		t_buf.write_int32(timestamp);
		t_buf.finish_message();
		player->send_to_server(t_buf);
		break;
	}
	default : {
		MSG_USER("error msg_id:%d", msg_id);
		break;
	}
	}

	return 0;
}

int Robot_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));

	process_inner_message(msg_id, cid, buf);

	return 0;
}

int Robot_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
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

int Robot_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));

	switch (msg_id) {
	case TIMER_LOGIN: {
		monitor()->login_logic_server();
		break;
	}
	case TIMER_TICK: {
		//MSG_USER("have a timer global monitor self....");
		monitor()->tick();
		break;
	}
	case TIMER_GLOBAL_MONITOR_SELF: {
		//MSG_USER("have a timer global monitor self....");
		//monitor()->tick();
		break;
	}
	default: {
		MSG_USER("unknow msg:%d", msg_id);
		break;
	}
	}

	return 0;
}

