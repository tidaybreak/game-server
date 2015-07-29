/*
 * Chat_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Chator: Ti
 */

#include "Chat_Message_Unit.h"
#include "Chat_Monitor.h"
#include "Chat_Server_Request.h"
#include "Chat_Client_Request.h"
#include "Chat_DB_Request.h"
#include "Chat_Timer_Request.h"
#include "Err_Code.h"
#include "DB_Def.h"
#include "chater/Chater_Def.h"
#include "Chat_Player.h"

Chat_Message_Unit::Chat_Message_Unit(void) {

}

Chat_Message_Unit::~Chat_Message_Unit(void) {

}

Chat_Monitor *Chat_Message_Unit::monitor(void) {
	return Message_Unit::monitor()->chat_monitor();
}

int Chat_Message_Unit::register_client_process(const uint32_t msg_id, Client_Func callback) {
	Client_Func_Map::iterator find_it = client_message_func_.find(msg_id);
	if (find_it != client_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	client_message_func_[msg_id] = callback;
	return 0;
}

int Chat_Message_Unit::process_client_message(Chat_Player &player, const uint32_t msg_id, Block_Buffer &buf) {
	Client_Func_Map::iterator mapi = client_message_func_.find(msg_id);
	if (mapi == client_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(player, buf);
}

int Chat_Message_Unit::register_server_process(const uint32_t msg_id, Server_Func callback) {
	Server_Func_Map::iterator find_it = server_message_func_.find(msg_id);
	if (find_it != server_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	server_message_func_[msg_id] = callback;
	return 0;
}

int Chat_Message_Unit::process_server_message(int cid, role_id_t role_id, Chat_Player *player, const uint32_t msg_id, Block_Buffer &buf) {
	Server_Func_Map::iterator mapi = server_message_func_.find(msg_id);
	if (mapi == server_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(cid, role_id, player, buf);
}

int Chat_Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {
	switch (msg_id) {
	case 1: {

		break;
	}

	default : {

		break;
	}
	}
	return 0;
}

int Chat_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));

	int error_ret = 0;
	role_id_t role_id = 0;
	Chat_Player *player = 0;
	if (get_msg_type(msg_id) == CLIENT_TO_CHAT) {

		JUDGE_ERROR_RETURN(buf.read_int64(role_id));

		player = monitor()->find_chat_player_by_role_id(role_id);
		if (! player) {
			MSG_USER("find player error, role:%ld", role_id);
			return -1;
		}

		// 跳过头
		buf.set_read_idx(buf.get_read_idx() + 8);
		error_ret = process_client_message(*player, msg_id, buf);
	} else {
		role_id_t role_id = 0;
		JUDGE_ERROR_RETURN(buf.read_int64(role_id));

		player = monitor()->find_chat_player_by_role_id(role_id);

		// 跳过头
		buf.set_read_idx(buf.get_read_idx() + 8);
		process_server_message(cid, role_id, player, msg_id, buf);
	}


	if (error_ret && player) {
		if (error_ret < 0) {
			MSG_USER("do not return -1 to client:%d, error:%d", msg_id, error_ret);
		} else {
			if (error_ret == 10000000) {
				MSG_USER("server inner error to client:%d", msg_id);
			}
			player->send_err_to_client(error_ret, msg_id);
		}
	}

	return 0;
}

int Chat_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_buff(msg_id, buf));

	switch (msg_id) {
	case MONGO_READ_CHAT_RECORD: {
		int type = 0;
		buf.read_int32(type);
		switch(type) {
			case READ_OFFLINE: {
				CHAT_DB_REQUEST->process_load_offline_private_chat_buff(buf);
				break;
			}
			case READ_HISTORY: {
				CHAT_DB_REQUEST->process_load_private_chat_buff(buf);
				break;
			}
		}
		break;
	}
	default : {
		return CALL_CONTINUE;
		break;
	}
	}

	return 0;
}

int Chat_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));

	switch (msg_id) {
	case TIMER_GLOBAL_MONITOR_SELF: {
		CHAT_TIMER_REQUEST->monitor_tick();
		break;
	}
	case TIMER_TRIGGER_DAILY_ZERO: {
		CHAT_TIMER_REQUEST->player_trigger_daily_zero();
		break;
	}
	default: {
		MSG_USER("unknow msg:%d", msg_id);
		break;
	}
	}

	return 0;
}


