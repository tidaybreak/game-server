/*
 * Auth_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#include "Logic_Message_Unit.h"
#include "Logic_Monitor.h"
#include "Pool_Manager.h"
#include "Logic_Client_Request.h"
#include "Logic_Server_Request.h"
#include "Logic_DB_Request.h"
#include "Logic_Timer_Request.h"
#include "Err_Code.h"
#include "Msg_Role_Scene_Enum.h"
#include "arena/Arena_Manager.h"
#include "DB_Def.h"
#include "Logic_Player.h"
#include "Config_Cache.h"
#include "dragon/Dragon_Vale_Manager.h"
#include "knight_trials/Knight_Trials_Manager.h"
#include "ranking/Ranking_Manager.h"

Logic_Message_Unit::Logic_Message_Unit(void) {
	pause_ts_.tv_sec = 0;
	pause_ts_.tv_nsec = 8 * 1000 * 1000;
}

Logic_Message_Unit::~Logic_Message_Unit(void) {

}

Logic_Monitor *Logic_Message_Unit::monitor(void) {
	return Message_Unit::monitor()->logic_monitor();
}

int Logic_Message_Unit::register_client_process(const uint32_t msg_id, Client_Func callback) {
	Client_Func_Map::iterator find_it = client_message_func_.find(msg_id);
	if (find_it != client_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);

	client_message_func_[msg_id] = callback;
	return 0;
}

int Logic_Message_Unit::process_client_message(Logic_Player &player, const uint32_t msg_id, Block_Buffer &buf) {
	Client_Func_Map::iterator mapi = client_message_func_.find(msg_id);
	if (mapi == client_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(player, buf);
}

int Logic_Message_Unit::register_server_process(const uint32_t msg_id, Server_Func callback) {
	Server_Func_Map::iterator find_it = server_message_func_.find(msg_id);
	if (find_it != server_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	server_message_func_[msg_id] = callback;
	return 0;
}

int Logic_Message_Unit::process_server_message(int cid, role_id_t role_id, Logic_Player *player, const uint32_t msg_id, Block_Buffer &buf) {
	Server_Func_Map::iterator mapi = server_message_func_.find(msg_id);
	if (mapi == server_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(cid, role_id, player, buf);
}

int Logic_Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {
	int result = 0;
	switch (msg_id) {
	case REQ_FETCH_ROLE_LIST: {
		MSG_10100000 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		result = LOGIC_CLIENT_REQUEST->process_fetch_role(cid, msg);
		break;
	}
	case REQ_PLAYER_LOGIN: {
		MSG_10100001 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		result = LOGIC_CLIENT_REQUEST->process_player_login(cid, msg);
		break;
	}
	case REQ_CREATE_ROLE: {
		MSG_10100005 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		result = LOGIC_CLIENT_REQUEST->process_player_create(cid, msg);
		break;
	}
	case REQ_RANDOM_NAME: {
		MSG_10100007 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		result = LOGIC_CLIENT_REQUEST->process_random_name(cid, msg);
		break;
	}
	case REQ_ACCESS_DATE_COUNT: {
		MSG_10188888 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		result = LOGIC_CLIENT_REQUEST->record_access_count(cid, msg);
		break;
	}
	case SERVER_CLIEN_KEEP_ALIVE: {
		// 更新客户端时间
		int timestamp = Time_Value::gettimeofday().sec();
		Block_Buffer t_buf;
		t_buf.make_message(80000010);
		t_buf.write_int32(timestamp);
		t_buf.finish_message();
		monitor()->send_to_client(cid, t_buf);
		break;
	}
	default : {
		MSG_USER("error msg_id:%d", msg_id);
		result = ERROR_SWITCH_NOT_EXIST_MSG;
		break;
	}
	}

	if (result) {
		monitor()->send_error_to_client(cid, result, msg_id);
	}

	return 0;
}

int Logic_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));

	int error_ret = 0;
	role_id_t role_id = 0;
	Logic_Player *player = 0;
	if (get_msg_type(msg_id) == CLIENT_TO_LOGIC) {
		JUDGE_ERROR_RETURN(buf.read_int64(role_id));

		player = monitor()->find_logic_player_by_role_id(role_id);
		if (! player) {
			LOG_USER("find player error, role:%ld msg_id:%d", role_id, msg_id);
			return -1;
		}

		// 跳过头
		buf.set_read_idx(buf.get_read_idx() + 8);
		error_ret = process_client_message(*player, msg_id, buf);
	} else {
		JUDGE_ERROR_RETURN(buf.read_int64(role_id));

		player = monitor()->find_logic_player_by_role_id(role_id);

		// 跳过头
		buf.set_read_idx(buf.get_read_idx() + 8);
		error_ret = process_server_message(cid, role_id, player, msg_id, buf);
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

int Logic_Message_Unit::process_async_data(int data_type, void *ptr_data) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_data(data_type, ptr_data));

	switch (data_type) {
	case Unit_Message::TYPE_DATA_PLAYER: {
		LOGIC_DB_REQUEST->process_load_player_data((Logic_Player_Detail *)ptr_data);
		break;
	}
	case Unit_Message::TYPE_DATA_ARENA_FIGHTER: {
		Logic_Player_Detail * arena_data = (Logic_Player_Detail *)ptr_data;
		role_id_t fight_id = arena_data->arenaer_detail.role_id;
		ARENA_MANAGER->process_player_data_complete(arena_data, fight_id);
		break;
	}
	case Unit_Message::TYPE_DATA_DRAGON_VALE_ROB_MATCH: {
		DRAGON_VALE_MANAGER->process_rob_match((Logic_Player_Detail *)ptr_data);
		break;
	}
	case Unit_Message::TYPE_DATA_DRAGON_VALE_ROB_FIGHT: {
		DRAGON_VALE_MANAGER->process_rob_fight_load_player_data((Logic_Player_Detail *)ptr_data);
		break;
	}
	case Unit_Message::TYPE_DATA_KNIGHT_TRIALS_MATCH_PLAYER: {
		KNIGHT_TRIALS_MANAGER->process_load_player_data((Logic_Player_Detail *)ptr_data);
		break;
	}
	case Unit_Message::TYPE_DATA_RANKING: {
		RANKING_MANAGER->process_refresh_ranking_data_p((Ranking *)ptr_data);
		break;
	}
	case Unit_Message::TYPE_DATA_KNIGHT_TRIALS_LOAD_FIGHTER_PLAYER: {
		KNIGHT_TRIALS_MANAGER->process_load_fighter_player_data((Knight_Trials_Player *)ptr_data);
		break;
	}
	}

	return 0;
}

int Logic_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_buff(msg_id, buf));

	switch (msg_id) {
	case MONGO_REFRESH_RANKING_RECORD: {
		LOGIC_DB_REQUEST->process_refresh_ranking_data(buf);
		break;
	}
	case SERVER_INNER_CLOSE_TIP: {
		Block_Buffer c_buf;
		c_buf.make_message(80000006);
		c_buf.write_int32(CONFIG_CACHE->server_maintainer_cache().server_close_delay);
		c_buf.finish_message();
		monitor()->send_to_all_client(c_buf);
		break;
	}
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

int Logic_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));

	switch (msg_id) {
	case TIMER_GLOBAL_MONITOR_SELF: {
		LOGIC_TIMER_REQUEST->monitor_tick();
		break;
	}
	case TIMER_TRIGGER_FIRST:{
		int32_t trigger_type = 0;
		buf.read_int32(trigger_type);
		LOGIC_TIMER_REQUEST->player_trigger_first(trigger_type);
		break;
	}
	case TIMER_TRIGGER_DAILY_ZERO: {
		LOGIC_TIMER_REQUEST->player_trigger_daily_zero();
		break;
	}
	case TIMER_TRIGGER_DAILY_SIX: {
		LOGIC_TIMER_REQUEST->player_trigger_daily_six();
		break;
	}
	case TIMER_TRIGGER_DAILY_TWENTY: {
		LOGIC_TIMER_REQUEST->player_trigger_daily_twenty();
		break;
	}
	case TIMER_TRIGGER_WEEKLY_ZERO: {
		LOGIC_TIMER_REQUEST->player_trigger_weekly_zero();
		break;
	}
	case TIMER_REBOOT_SERVER: {
		LOGIC_TIMER_REQUEST->reboot_server(buf);
		break;
	}
	default: {
		MSG_USER("unknow msg:%d", msg_id);
		break;
	}
	}

	return 0;
}

int Logic_Message_Unit::recover_ptr_body(Unit_Message *ptr_data) {
	JUDGE_CALL_RETURN(Message_Unit::recover_ptr_body(ptr_data));

	if (ptr_data->data_type == Unit_Message::TYPE_DATA_PLAYER) {
		// 逻辑自己回收
		//POOL_MANAGER->push_player_data_pool((Player_Detail *)ptr_data->ptr_data);
	}

	return 0;
}

void Logic_Message_Unit::show_server_status(void) {
	POOL_MANAGER->show_pool_status();

	Message_Unit::show_server_status();
}

int Logic_Message_Unit::process_config_hook(std::string &module, bool restart) {
	if (restart) {
		Block_Buffer new_buf;
		new_buf.make_message(SERVER_INNER_SELF_CLOSE);
		new_buf.finish_message();
		LOGIC_MONITOR->send_to_all_process(new_buf);
	}

	// 外网不广播
	if (CONFIG_CACHE->server_maintainer_cache().command_mode == 2) {
		return 0;
	}

	std::stringstream str_stream;
	str_stream << "热更配制成功:" << module;

	Block_Buffer inner_buf;
	inner_buf.make_message(83000000);
	inner_buf.write_uint8(100);
	inner_buf.write_int32(0);
	inner_buf.write_uint16(1);
	inner_buf.write_int8(0);
	inner_buf.write_string(str_stream.str());
	inner_buf.finish_message();
	LOGIC_MONITOR->send_to_all_client(inner_buf);

	return 0;
}
