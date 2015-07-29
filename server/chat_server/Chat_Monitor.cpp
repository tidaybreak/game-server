/*
 * Chat_Monitor.cpp
 *
 *  Created on: Nov 25, 2013
 *      Chator: ti
 */

#include "Chat_Monitor.h"
#include "Pool_Manager.h"
#include "Chat_Player.h"
#include "Daemon_Server.h"
#include "module/gang/Chat_Gang_Manager.h"
#include "Global_Timer.h"
#include "chater/Chater_Manager.h"
#include "gm_ann/GM_Ann_Manager.h"
#include "Config_Cache.h"
#include "server_define.h"
#include "http_client/Http_Manager.h"
#include "chat_control/Chat_Control_Manager.h"

Chat_Monitor::Chat_Monitor() {
	// TODO Auto-generated constructor stub
}

Chat_Monitor::~Chat_Monitor() {
	// TODO Auto-generated destructor stub
}

int Chat_Monitor::init(int monitor_index) {
	Server_Monitor::init(Server_Enum::MONITOR_CHAT, monitor_index, &message_unit_);

	//初始化得到所有网关UUid
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(gate_monitor_id_order, Server_Enum::MONITOR_GATE);


	return 0;
}

int Chat_Monitor::start(void) {
	Server_Monitor::start();

	CHAT_GANG_MANAGER->start();

	message_unit_.thr_create();

	start_monitor_timer();

	GM_ANN_MANAGER->start();
	CHATER_CONTROL_MANAGER->start();

	HTTP_MANAGER->init(1);
	HTTP_MANAGER->start();

	return 0;
}

int Chat_Monitor::start_monitor_timer(void) {
	Block_Buffer buf;
	buf.make_message(TIMER_GLOBAL_MONITOR_SELF);
	buf.finish_message();
	GLOBAL_TIMER->register_loop_handler(buf, Time_Value(30, 0), unit_);

	{
	Block_Buffer buf;
	buf.make_message(TIMER_TRIGGER_DAILY_ZERO);
	buf.finish_message();
	Time_Value timeout = next_relative_time(24, 00, 00);
	GLOBAL_TIMER->register_loop_handler(buf, Time_Value(timeout), unit_);
	}
	return 0;
}

int Chat_Monitor::server_monitor_status(void) {
	LOG_TEXT("chat player amount:%d", player_role_id_map_.size());
	return 0;
}

int Chat_Monitor::process_monitor_close(void) {
	Server_Monitor::process_monitor_close();

	LOG_DEBUG("chat monitor close player:%d", player_role_id_map_.size());

	if (player_role_id_map_.empty()) {
		DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_CHAT);
	}

	return 0;
}

int Chat_Monitor::monitor_link_drop(const int cid) {
	Monitor_Mark_Vec muid_list;
	get_muid_connect_by_cid(cid, muid_list);
	for (Monitor_Mark_Vec::iterator m_it = muid_list.begin(); m_it != muid_list.end(); ++m_it) {
		process_muid_drop(*m_it);
	}

	if (!muid_list.empty())
		Server_Monitor::monitor_link_drop(cid);

	return 0;
}

int Chat_Monitor::process_muid_drop(Monitor_Unique_ID &muid) {
	LOG_DEBUG("muid drop uuid:%ld type:%d, id:%d order:%d player size:%d", muid.uuid, muid.type, muid.id, muid.order, player_role_id_map_.size());

	Chat_Player_Role_Id_Map player_map;
	for (Chat_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if (muid.type == MONITOR_LOGIC) {
			if (it->second->monitor_link().logic_muid == muid) {
				player_map.bind(it->first, it->second);
			}
		} else if (muid.type == MONITOR_GATE) {
			if (it->second->monitor_link().gate_muid == muid) {
				player_map.bind(it->first, it->second);
			}
		}
	}

	for (Chat_Player_Role_Id_Map::iterator it = player_map.begin(); it != player_map.end(); ++it) {
		LOG_DEBUG("server core sign out, role:%ld", it->second->role_id());

		unbind_chat_player(it->first);
	}

	return 0;
}

int Chat_Monitor::send_to_all_players(Block_Buffer& buf, int limit_level) {
	if(limit_level <= 1) {
		return send_to_all_gate(buf);
	}
	for( Chat_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if(it->second->base_detail().level >= limit_level) {
			it->second->send_to_client(buf);
		}
	}
	return 0;
}

int Chat_Monitor::send_to_gate(Monitor_Unique_ID &idx, Block_Buffer &buf) {
	return send_to_monitor(MONITOR_GATE, idx.id, idx.order, buf, idx.uuid);
}

int Chat_Monitor::send_to_all_gate(Block_Buffer &buf) {
	Block_Buffer tmp_buf;
	tmp_buf.make_message(CHAT_GATE_BROADCAST_BUFF);
	tmp_buf.copy(&buf);
	tmp_buf.finish_message();
	for(Monitor_Mark_Vec::iterator iter = gate_monitor_id_order.begin(); iter != gate_monitor_id_order.end(); ++iter) {
		send_to_gate(*iter, tmp_buf);
	}
	return 0;
}

int Chat_Monitor::bind_chat_player(Chat_Player &player) {
	player_role_id_map_.bind(player.role_id(), &player);
	return 0;
}

int Chat_Monitor::unbind_chat_player(role_id_t role_id) {
	Chat_Player_Role_Id_Map::iterator find_it = player_role_id_map_.find(role_id);
	if (find_it != player_role_id_map_.end()) {
		POOL_MANAGER->push_chat_player(find_it->second);
		player_role_id_map_.unbind(role_id);
	}

	{
		if (status_ == MONITOR_STATUS_CLOSING) {
			LOG_DEBUG("process_monitor_close player:%d, role id:%ld", player_role_id_map_.size(), role_id);
		}

		if (status_ == MONITOR_STATUS_CLOSING && player_role_id_map_.empty()) {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_CHAT);
		}
	}

	return 0;
}

int Chat_Monitor::player_trigger_daily_zero(void) {
	CHATER_MANAGER->player_trigger_daily_zero();
	return 0;
}
