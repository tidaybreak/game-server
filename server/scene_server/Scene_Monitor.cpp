/*
 * Scene_Monitor.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Scene_Monitor.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Utility_Func.h"
#include "Scene_Player.h"
#include "Daemon_Server.h"
#include "Global_Timer.h"
#include "Scene_Public.h"
#include "valentine_monster/Valentine_Monster_Manager.h"
#include "NPC/NPC_Manager.h"
#include "Global_Scene_Manager.h"

Scene_Monitor::Scene_Monitor() {
	// TODO Auto-generated constructor stub
}

Scene_Monitor::~Scene_Monitor() {
	// TODO Auto-generated destructor stub
}

int Scene_Monitor::init(int monitor_index) {
	Server_Monitor::init(Server_Enum::MONITOR_SCENE, monitor_index, &message_unit_);


	return 0;
}

int Scene_Monitor::start_monitor_timer(void) {
	Block_Buffer buf;
	buf.make_message(TIMER_GLOBAL_MONITOR_SELF);
	buf.finish_message();
	GLOBAL_TIMER->register_loop_handler(buf, Time_Value(0, 50 * 1000), unit_);

	return 0;
}

int Scene_Monitor::process_monitor_close(void) {
	Server_Monitor::process_monitor_close();

	// 如果是跨服要直接关闭(是否有逻辑服来判断)
	Monitor_Mark_Vec monitor_id_order;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(monitor_id_order, Server_Enum::MONITOR_LOGIC);

	LOG_DEBUG("scene monitor close player:%d, is_inter:%d", player_role_id_map_.size(), monitor_id_order.size());

	if (player_role_id_map_.empty() || monitor_id_order.empty()) {
		DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_SCENE);
	}

	return 0;
}

int Scene_Monitor::monitor_link_drop(const int cid) {
	Monitor_Mark_Vec muid_list;
	get_muid_connect_by_cid(cid, muid_list);
	for (Monitor_Mark_Vec::iterator m_it = muid_list.begin(); m_it != muid_list.end(); ++m_it) {
		process_muid_drop(*m_it);
	}

	if (!muid_list.empty())
		Server_Monitor::monitor_link_drop(cid);

	return 0;
}

int Scene_Monitor::process_muid_drop(Monitor_Unique_ID &muid) {
	LOG_DEBUG("muid drop uuid:%ld type:%d, id:%d order:%d player size:%d", muid.uuid, muid.type, muid.id, muid.order, player_role_id_map_.size());

	Scene_Player_Role_Id_Map player_map;
	for (Scene_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
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

	for (Scene_Player_Role_Id_Map::iterator it = player_map.begin(); it != player_map.end(); ++it) {
		abnormal_logout(it->second, 0);
	}

	return 0;
}

int Scene_Monitor::abnormal_logout(Scene_Player *player, int close_reason) {
	LOG_DEBUG("server core sign out, role:%ld, close_reason:%d", player->role_id(), close_reason);

	player->force_end_battle_scene();
	player->sign_out(true);
	unbind_scene_player(player->role_id());

	return 0;
}

int Scene_Monitor::bind_scene_player(Scene_Player &player) {
	player_role_id_map_.bind(player.role_id(), &player);
	return 0;
}

int Scene_Monitor::unbind_scene_player(role_id_t role_id) {
	Scene_Player_Role_Id_Map::iterator find_it = player_role_id_map_.find(role_id);
	if (find_it != player_role_id_map_.end()) {
		POOL_MANAGER->push_scene_player(find_it->second);
		player_role_id_map_.unbind(role_id);
	}

	{
		if (status_ == MONITOR_STATUS_CLOSING) {
			LOG_DEBUG("process_monitor_close player:%d, role id:%ld", player_role_id_map_.size(), role_id);
		}

		if (status_ == MONITOR_STATUS_CLOSING && player_role_id_map_.empty()) {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_SCENE);
		}
	}

	return 0;
}

int Scene_Monitor::player_total(void) {
	return player_role_id_map_.size();
}

int Scene_Monitor::start(void) {
	Server_Monitor::start();

	message_unit_.thr_create();

	GLOBAL_SCENE_MANAGER->init_global_scene(monitor_idx_);

	return 0;
}

int Scene_Monitor::send_to_logic(const Monitor_Unique_ID &idx, Block_Buffer &buf) {
	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), 0);
		ret = send_to_monitor(idx, buf);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), 0);
		ret = send_to_monitor(idx, tmp_buf);
	}

	return ret;
}

int Scene_Monitor::send_to_logic(long_id_t uuid, Block_Buffer &buf) {
	int ret = 0;
	const Monitor_Unique_ID *muid = get_monitor_unique_id_by_uuid(uuid, Server_Enum::MONITOR_TYPE::MONITOR_LOGIC);
	if (muid) {
		ret = send_to_logic(*muid, buf);
	} else {
		MSG_USER_TRACE("can not find muid:%ld", uuid);
		ret = -1;
	}

	return ret;
}

int Scene_Monitor::send_to_gate(Monitor_Unique_ID &idx, Block_Buffer &buf) {
	return send_to_monitor(MONITOR_GATE, idx.id, idx.order, buf, idx.uuid);
}

int Scene_Monitor::process_time_up(void) {
	Time_Value now = Time_Value::gettimeofday();

	// 玩家time_up
	for (Scene_Player_Role_Id_Map::iterator it =
			player_role_id_map_.begin(); it != player_role_id_map_.end();
			++it) {
		it->second->time_up(now);
	}

	// 英雄time_up
	SCENE_PUBLIC->hero_time_up(now);

	// NPC time_up
	NPC_MANAGER->npc_time_up(now);

	// valentine_monster
	VALENTINE_MONSTER_MANAGER->tick(now);

	return 0;
}

int Scene_Monitor::server_monitor_status(void) {
	LOG_TEXT("scene player amount:%d", player_role_id_map_.size());
	return 0;
}
