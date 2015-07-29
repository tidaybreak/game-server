/*
 * Auth_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#include "Gate_Message_Unit.h"
#include "Gate_Monitor.h"
#include "Pool_Manager.h"
#include "Gate_Player.h"
#include "Gate_Client_Request.h"
#include "Gate_Server_Request.h"
#include "Gate_Timer_Request.h"
#include "Err_Code.h"

Gate_Message_Unit::Gate_Message_Unit(void) {
	pause_ts_.tv_sec = 0;
	pause_ts_.tv_nsec = 5 * 1000 * 1000;
}

Gate_Message_Unit::~Gate_Message_Unit(void) {

}

Gate_Monitor *Gate_Message_Unit::monitor(void) {
	return Message_Unit::monitor()->gate_monitor();
}

int Gate_Message_Unit::register_client_message(void) {

	return 0;
}

void Gate_Message_Unit::count_process_time(int msg_id) {
	// 为了效率,什么都不做
}

void Gate_Message_Unit::show_server_status(void) {
	POOL_MANAGER->show_pool_status();

	Message_Unit::show_server_status();
}

int Gate_Message_Unit::register_client_process(const uint32_t msg_id, Client_Func callback) {
	Client_Func_Map::iterator find_it = client_message_func_.find(msg_id);
	if (find_it != client_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	client_message_func_[msg_id] = callback;
	return 0;
}

int Gate_Message_Unit::process_client_message(Gate_Player &player, const uint32_t msg_id, Block_Buffer &buf) {
	Client_Func_Map::iterator mapi = client_message_func_.find(msg_id);
	if (mapi == client_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(player, buf);
}

int Gate_Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {
	Gate_Player *player = monitor()->find_player_by_cid(cid);
	if (! player) {
		if (msg_id == 10400001) {
			int result = GATE_CLIENT_REQUEST->process_player_login(cid, buf);
			if (result) monitor()->send_error_to_client(cid, result, result);
			return 0;
		} else if (msg_id != 99999999) {
			// 可能将要被下线,客户端还在发|包异常时要通知关闭,这边不做直接关
			MSG_USER("error cid:%d msg_id:%d", cid, msg_id);
			return 0;//monitor()->close_client_by_cid(cid, Time_Value(1), 10000201);
		} else {
			MSG_USER("error cid:%d msg_id:%d", cid, msg_id);
			return 0;
		}
	}

	// 包检查，防外挂
	int msg_ret = player->verify_msg_detail(msg_id, serial_cipher, msg_time_cipher);
	if (msg_ret) {
		player->notice_logic_sign_out(msg_ret);
		MSG_USER("close client:%ld msg:%d, ret:%d", player->role_id(), msg_id, msg_ret);
		return 0;
	}

	// 场景加载完成能解开recv pack off
	if (msg_id != 99999999 && player->recv_pace_off()) {
		//MSG_DEBUG("recv pace off, role:%ld, msg_id:%d", player->role_id(), msg_id);
		return 0;
	}

	switch (get_msg_type(msg_id)) {
	case CLIENT_TO_LOGIC: {
		int get_write_idx = buf.get_write_idx();
		int get_read_idx = buf.get_read_idx();
		buf.set_read_idx(get_read_idx - sizeof(int) - sizeof(int));
		buf.set_write_idx(get_read_idx - sizeof(int) - sizeof(int));
		buf.write_int32(get_write_idx - get_read_idx + sizeof(int));
		buf.write_uint32(msg_id);
		buf.set_write_idx(get_write_idx);

		player->send_to_logic(buf);
		break;
	}
	case CLIENT_TO_SCENE: {
		int get_write_idx = buf.get_write_idx();
		int get_read_idx = buf.get_read_idx();
		buf.set_read_idx(get_read_idx - sizeof(int) - sizeof(int));
		buf.set_write_idx(get_read_idx - sizeof(int) - sizeof(int));
		buf.write_int32(get_write_idx - get_read_idx + sizeof(int));
		buf.write_uint32(msg_id);
		buf.set_write_idx(get_write_idx);

		player->send_to_scene(buf);
		break;
	}
	case CLIENT_TO_CHAT: {
		//todo 跨服聊天频道发到跨服聊天服,其它发到本服聊天服
		int get_write_idx = buf.get_write_idx();
		int get_read_idx = buf.get_read_idx();
		buf.set_read_idx(get_read_idx - sizeof(int) - sizeof(int));
		buf.set_write_idx(get_read_idx - sizeof(int) - sizeof(int));
		buf.write_int32(get_write_idx - get_read_idx + sizeof(int));
		buf.write_uint32(msg_id);
		buf.set_write_idx(get_write_idx);

		player->send_to_local_chat(buf);
		break;
	}
	case CLIENT_TO_GATE: {
		GATE_CLIENT_REQUEST->process_client_block(player, cid, msg_id, buf);
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
		player->send_to_client(t_buf);
		break;
	}
	default : {
		MSG_USER("error msg_id:%d", msg_id);
		break;
	}
	}

	return 0;
}

int Gate_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));

	if (!msg_id) {
		role_id_t role_id = 0;
		buf.read_int64(role_id);
		Gate_Player *player = monitor()->find_player_by_role_id(role_id);
		if (player && player->cid())
			monitor()->send_to_client_by_cid(player->cid(), buf, true);
	} else {
		process_inner_message(msg_id, cid, buf);
	}

	return 0;
}

int Gate_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
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

int Gate_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));

	switch (msg_id) {
	case TIMER_LOGIC_LOGIN_GATE_TIMEOUT: {
		GATE_TIMER_REQUEST->login_gate_timeout(buf);

		break;
	}
	case TIMER_GLOBAL_MONITOR_SELF: {

		monitor()->tran_timeout(now);
		break;
	}
	default: {
		MSG_USER("unknow msg:%d", msg_id);
		break;
	}
	}

	return 0;
}

