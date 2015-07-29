/*
 * Gate_Server_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Gate_Server_Request.h"
#include "Gate_Monitor.h"
#include "Pool_Manager.h"
#include "Gate_Player.h"
#include "Global_Timer.h"
#include "Config_Cache_Scene.h"

Gate_Server_Request::Gate_Server_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = GATE_MONITOR;
}

Gate_Server_Request::~Gate_Server_Request() {
	// TODO Auto-generated destructor stub
}

int Gate_Server_Request::find_player_by_buf(Block_Buffer &buf, Gate_Player *&player) {
	role_id_t role_id = 0;
	if (buf.read_int64(role_id)) {
		MSG_USER_TRACE("read role id:%ld", role_id);
		return CALL_RETURN;
	}

	player = monitor()->find_player_by_role_id(role_id);
	if (! player) {
		MSG_USER_TRACE("can not find logic player:%ld", role_id);
		return CALL_RETURN;
	}

	return 0;
}

int Gate_Server_Request::logic_session(int cid, Block_Buffer &buf) {
	role_id_t role_id = 0;
	buf.read_int64(role_id);
	std::string session;
	buf.read_string(session);
	int p_cid = 0;
	buf.read_int32(p_cid);

	Gate_Player *player = monitor()->find_player_by_role_id(role_id);

	if (player) {
		player->set_login_session(session);
	} else {
		MSG_USER("can not find player, role:%ld", role_id);
	}

	Block_Buffer inner_buf;
	inner_buf.make_message(LOGIC_SYNC_LOGIN_SESSION);
	inner_buf.write_string(session);
	inner_buf.write_int32(p_cid);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), role_id);
	monitor()->send_to_server_by_cid(cid, inner_buf);

	return 0;
}

int Gate_Server_Request::logic_login_ready(int cid, Block_Buffer &buf) {
	role_id_t role_id = 0;
	buf.read_int64(role_id);
	std::string session;
	buf.read_string(session);

	int result = 0;
	Gate_Player *player = monitor()->find_player_by_role_id(role_id);
	if (!player) {
		player = POOL_MANAGER->pop_gate_player();
		if (! player) {
			LOG_ABORT("logic_player_pool_.pop() return 0.");
			return 0;
		}

		player->load_detail(buf);
		player->set_login_session(session);
		monitor()->bind_rid_player(*player);

		Block_Buffer inner_buf;
		inner_buf.make_message(LOGIC_GATE_LOGIN_READY);
		inner_buf.write_int32(result);
		inner_buf.finish_message();
		inner_buf.make_head(inner_buf.get_msg_id(), role_id);
		monitor()->send_to_server_by_cid(cid, inner_buf);

		Block_Buffer timer_buf;
		timer_buf.make_message(TIMER_LOGIC_LOGIN_GATE_TIMEOUT);
		timer_buf.write_int64(role_id);
		timer_buf.finish_message();
		GLOBAL_TIMER->register_once_handler(timer_buf, Time_Value(LOGIN_GATE_TIMEOUT, 0), monitor()->message_unit());

		MSG_DEBUG("player login gate success, role:%ld, role_name:%s", player->role_id(), player->base_detail().role_name.c_str());
	} else {
		MSG_USER("player in gate, role:%ld", role_id);

		Block_Buffer inner_buf;
		inner_buf.make_message(LOGIC_GATE_LOGIN_READY);
		inner_buf.write_int32(1);
		inner_buf.finish_message();
		inner_buf.make_head(inner_buf.get_msg_id(), role_id);
		monitor()->send_to_server_by_cid(cid, inner_buf);
	}

	return 0;
}

int Gate_Server_Request::logic_logout_ready(int cid, Block_Buffer &buf) {
	role_id_t role_id = 0;
	int out_reason = 0, need_return = 0;
	buf.read_int64(role_id);
	buf.read_int32(out_reason);
	buf.read_int32(need_return);
	Gate_Player *player = monitor()->find_player_by_role_id(role_id);
	if (player) {
		// 1.客户端主动关闭链接cid=0,不需要处理
		// 2.客户端被主动关闭,要让客户端断线
		if (player->cid()) {
			monitor()->close_client_by_cid(player->cid(), Time_Value(1), out_reason);
			monitor()->unbind_cid_player(player->cid());
		}
		monitor()->unbind_rid_player(role_id);
		MSG_DEBUG("player logout gate, role:%ld, out_reason:%d, need return:%d", role_id, out_reason, need_return);
	} else {
		MSG_USER("login out, gate has not player role:%ld, out_reason:%d, need return:%d", role_id, out_reason, need_return);
	}

	// 强制(core,强制断线)断线不要再通知(core时可能有玩家在转场景中,网关不能正常下线)
	if (need_return) {
		Block_Buffer inner_buf;
		inner_buf.make_message(LOGIC_GATE_LOGOUT_READY);
		inner_buf.write_int32(out_reason);
		inner_buf.finish_message();
		inner_buf.make_head(inner_buf.get_msg_id(), role_id);
		monitor()->send_to_server_by_cid(cid, inner_buf);
	}


	return 0;
}

int Gate_Server_Request::logic_transmit_finish(int cid, Block_Buffer &buf) {
	Gate_Player *player = 0;
	JUDGE_CALL_RETURN(find_player_by_buf(buf, player));

	monitor()->tran_player_del(player->role_id());
	player->set_recv_pack_off(false);


	int result = 0;
	buf.read_int32(result);
	if (result) {
		Block_Buffer res_buf;
		res_buf.make_message(50400005);
		player->base_detail().location.serialize(res_buf);
		res_buf.write_int32(result);
		res_buf.finish_message();
		player->send_to_client(res_buf);
		MSG_DEBUG("transmit fail, role:%ld, des scene:%d, des order:%d, why:%d", player->role_id(), player->base_detail().location.scene_id, player->base_detail().location.scene_order, result);
	} else {
		Coord coord;
		Location location;
		Monitor_Unique_ID scene_muid;
		scene_muid.deserialize(buf);
		location.deserialize(buf);
		player->set_scene_muid(scene_muid);
		const Scene_Config *config = CONFIG_CACHE_SCENE->scene_config_cache(scene_muid.id);
		int order_total = 1;
		if (0 != config) {
			order_total = config->order_total;
		}

		Block_Buffer res_buf;
		res_buf.make_message(81000030);
		location.coord = location.coord.pixel_coord();
		location.serialize(res_buf);
		res_buf.write_int8(order_total);
		res_buf.finish_message();
		player->send_to_client(res_buf);
		MSG_DEBUG("transmit success, role:%ld, des scene:%d, des order:%d", player->role_id(), scene_muid.id, scene_muid.order);
	}

	return 0;
}

int Gate_Server_Request::scene_battle_update(int cid, Block_Buffer &buf) {
	Gate_Player *player = 0;
	JUDGE_CALL_RETURN(find_player_by_buf(buf, player));

	int battle_status = 0;
	buf.read_int32(battle_status);
	player->set_battle_status(battle_status);

	// 如果在托管就下线
	if (player->cid() == 0) {
		MSG_DEBUG("player link drop, let it sign out role:%ld", player->role_id());
		player->notice_logic_sign_out(0);
	}

	return 0;
}

int Gate_Server_Request::logic_inner_transmit(int cid, Block_Buffer &buf) {
	Gate_Player *player = 0;
	JUDGE_CALL_RETURN(find_player_by_buf(buf, player));

	if (player->recv_pace_off()) {
		MSG_USER("player in off");
		return 0;
	}
	// 战斗没有结束，不能转场景
	if(player->battle_status()){
		return 0;
	}

	monitor()->tran_player_add(player->role_id());

	player->set_recv_pack_off(true);

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_GATE_ASK_LOGIC_TRAN);
	inner_buf.copy(&buf);
	inner_buf.write_bool(true);
	inner_buf.finish_message();
	return player->send_to_logic(inner_buf);
}

int Gate_Server_Request::caht_gate_broadcast_buff(int cid, Block_Buffer &buf) {
	return monitor_->send_to_all_player(buf);
}
