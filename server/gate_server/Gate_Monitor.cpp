/*
 * Gate_Monitor.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Gate_Monitor.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Gate_Player.h"
#include "Daemon_Server.h"
#include "Global_Timer.h"

Gate_Monitor::Gate_Monitor() {
	// TODO Auto-generated constructor stub
}

Gate_Monitor::~Gate_Monitor() {
	// TODO Auto-generated destructor stub
}

int Gate_Monitor::init(int monitor_index) {
	Server_Monitor::init(Server_Enum::MONITOR_GATE, monitor_index, &message_unit_);


	return 0;
}

int Gate_Monitor::start(void) {
	Server_Monitor::start();

	message_unit_.thr_create();

	return 0;
}

int Gate_Monitor::server_monitor_status(void) {
	LOG_TEXT("gate player amount:%d", player_role_id_map_.size());
	return 0;
}

int Gate_Monitor::process_monitor_close(void) {
	Server_Monitor::process_monitor_close();

	LOG_DEBUG("gate monitor close, player:%d", player_role_id_map_.size());

	if (player_role_id_map_.empty()) {
		DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_GATE);
	}

	return 0;
}

int Gate_Monitor::start_monitor_timer(void) {
	Block_Buffer buf;
	buf.make_message(TIMER_GLOBAL_MONITOR_SELF);
	buf.finish_message();
	GLOBAL_TIMER->register_loop_handler(buf, Time_Value(3, 0), unit_);
	return 0;
}

int Gate_Monitor::client_link_drop(const int cid) {
	Gate_Player *player = find_player_by_cid(cid);
	// 主动踢或顶号时发生
	if (! player) {
		MSG_DEBUG("may be repeat login cid:%d", cid);
		return 0;
	}

	// 底层已经回收cid,可能被后来链接用,这里要解绑定cid
	unbind_cid_player(cid);
	player->set_cid(0);

	// 战斗中下线托管玩家
	if (player->battle_status()) {
		Block_Buffer inner_buf;
		inner_buf.make_message(GATE_LOGIC_TRUSTEESHIP);
		inner_buf.finish_message();
		player->send_to_logic(inner_buf);
		MSG_DEBUG("in battle, role:%ld, cid:%d", player->role_id(), cid);

		Block_Buffer inner_buf_1;
		inner_buf_1.make_message(GATE_SCENE_LGOUT_IN_BATTLE);
		inner_buf_1.finish_message();
		player->send_to_scene(inner_buf_1);
		return 0;
	} else {
		MSG_DEBUG("gate link drop, role:%ld, cid:%d", player->role_id(), cid);
	}

	player->notice_logic_sign_out(0);

	return 0;
}

int Gate_Monitor::monitor_link_drop(const int cid) {
	Monitor_Mark_Vec muid_list;
	get_muid_connect_by_cid(cid, muid_list);
	for (Monitor_Mark_Vec::iterator m_it = muid_list.begin(); m_it != muid_list.end(); ++m_it) {
		process_muid_drop(*m_it);
	}

	if (!muid_list.empty())
		Server_Monitor::monitor_link_drop(cid);

	return 0;
}

int Gate_Monitor::process_muid_drop(Monitor_Unique_ID &muid) {
	LOG_DEBUG("muid drop uuid:%ld type:%d, id:%d order:%d player size:%d", muid.uuid, muid.type, muid.id, muid.order, player_role_id_map_.size());

	Gate_Player_Role_Id_Map player_map;
	for (Gate_Player_Role_Id_Map::iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if (muid.type == MONITOR_LOGIC) {
			if (it->second->monitor_link().logic_muid == muid) {
				player_map.bind(it->first, it->second);
			}
		} else if (muid.type == MONITOR_SCENE) {
			if (it->second->monitor_link().scene_muid == muid) {
				it->second->abnormal_logout();
				player_map.bind(it->first, it->second);
			}
		}
	}

	int close_reason = status_ == MONITOR_STATUS_CLOSING ? 10000125 : 10000126;
	for (Gate_Player_Role_Id_Map::iterator it = player_map.begin(); it != player_map.end(); ++it) {
		abnormal_logout(it->second, close_reason);
	}

	return 0;
}

int Gate_Monitor::abnormal_logout(Gate_Player *player, int close_reason) {
	LOG_DEBUG("server core sign out, role:%ld, reason:%d", player->role_id(), close_reason);

	// 如果有在转场景异常的也移除
	tran_player_del(player->role_id());

	player->set_recv_pack_off(true);
	int cid = player->cid();

	unbind_rid_player(player->role_id());
	// 可能有些player还没有cid
	if (cid) {
		unbind_cid_player(cid);
		close_client_by_cid(cid, Time_Value(2), close_reason);
	}
	return 0;
}

int Gate_Monitor::send_error_to_client(int cid, int error_id, int source) {
	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(source);
	buf.write_string("");
	buf.finish_message();
	return send_to_client_by_cid(cid, buf);
}

int Gate_Monitor::bind_rid_player(Gate_Player &player) {
	return player_role_id_map_.bind(player.role_id(), &player);
}

int Gate_Monitor::unbind_rid_player(role_id_t role_id) {
	Gate_Player_Role_Id_Map::iterator find_it = player_role_id_map_.find(role_id);
	if (find_it != player_role_id_map_.end()) {
		POOL_MANAGER->push_gate_player(find_it->second);
		player_role_id_map_.unbind(role_id);
	}

	{
		if (status_ == MONITOR_STATUS_CLOSING) {
			LOG_DEBUG("process_monitor_close player:%d, role id:%ld", player_role_id_map_.size(), role_id);
		}
		if (status_ == MONITOR_STATUS_CLOSING && player_role_id_map_.empty()) {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_GATE);
		}
	}

	return 0;
}

int Gate_Monitor::bind_cid_player(Gate_Player &player) {
	return gate_player_cid_map_.bind(player.cid(), &player);
}

int Gate_Monitor::unbind_cid_player(int cid) {
	return gate_player_cid_map_.unbind(cid);
}

void Gate_Monitor::tran_player_add(role_id_t role_id, int delay_sec) {
	tran_player_.insert(std::make_pair(role_id, Time_Value::gettimeofday() + Time_Value(delay_sec)));
}

void Gate_Monitor::tran_player_del(role_id_t role_id) {
	tran_player_.erase(role_id);
}

int Gate_Monitor::tran_timeout(Time_Value &now) {
	for (Role_Id_Time_Map::iterator it = tran_player_.begin(); it != tran_player_.end();) {
		if (now > it->second) {
			role_id_t role_id = it->first;
			MSG_USER("tran Abnormal, role:%ld", role_id);
			Gate_Player *player = find_player_by_role_id(role_id);
			if (player) {
				player->notice_logic_sign_out(10000050, 0);
			}

			tran_player_.erase(it++);
		} else {
			it++;
		}
	}

	return 0;
}

int Gate_Monitor::send_to_all_player(Block_Buffer &buf) {
	Gate_Player_Role_Id_Map::iterator it = player_role_id_map_.begin();
	for(; it != player_role_id_map_.end(); ++it) {
		if (it->second->cid())
			it->second->send_to_client(buf);
	}
	return 0;
}

