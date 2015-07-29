/*
 * Gate_Client_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Gate_Client_Request.h"
#include "Gate_Monitor.h"
#include "Pool_Manager.h"
#include "Gate_Player.h"
#include "Msg_Role_Scene_Struct.h"


Gate_Client_Request::Gate_Client_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = GATE_MONITOR;
}

Gate_Client_Request::~Gate_Client_Request() {
	// TODO Auto-generated destructor stub
}

int Gate_Client_Request::process_client_block(Gate_Player *player, int cid, int msg_id, Block_Buffer &buf) {
	switch (msg_id) {
	case 10400005: {
		process_transmit(player, buf);
		break;
	}
	case 10499999: {
		std::string command;
		buf.read_string(command);
		player->test_command(command);
		break;
	}
	default : {
		monitor()->send_error_to_client(cid, 10000002, msg_id);
		MSG_USER("error msg_id:%d, role:%ld, player cid:%d, cid:%d", msg_id, player->role_id(), player->cid(), cid);
		break;
	}
	}

	return 0;
}

int Gate_Client_Request::process_player_login(int cid, Block_Buffer &buf) {
	std::string session_id;
	role_id_t role_id = 0;
	if (buf.read_string(session_id) || buf.read_int64(role_id)) {
		return 10000101;
	}

	Gate_Player *player = monitor()->find_player_by_role_id(role_id);
	if (!player) {
		MSG_USER("player not in gate, role:%ld", role_id);
		return 10000200;
	}

	if (player->login_session() != session_id) {
		MSG_USER("error repeat, role:%ld", role_id);
		return 10000017;
	}

	// 如果超时下线先发生，这里就不继续
	if (player->recv_pace_off()) {
		MSG_USER("player sign out maybe timeout first, role:%ld", role_id);
		return 10000111;
	}

	int repeat_login = 0;
	if (player->cid()) {
		if (player->cid() == cid) {
			return 10000200;
		} else {
			// 顶号
			MSG_DEBUG("repeat login gate, role:%ld, old cid:%d, new cid:%d, session_id:%s", role_id, player->cid(), cid, session_id.c_str());
			repeat_login = 1;

			// 断开原玩家链接
			monitor()->close_client_by_cid(player->cid(), Time_Value(2), 10000115);

			monitor()->unbind_cid_player(player->cid());

			player->reset_msg_detail();
		}
	} else {
		// 正常登录或战斗中下线再上线
		if (player->msg_detail().is_inited) {
			repeat_login = 1;
		}

		player->reset_msg_detail();

		MSG_DEBUG("login gate, role:%ld, cid:%d, session_id:%s", role_id, player->cid(), session_id.c_str());
	}

	player->set_cid(cid);

	monitor()->bind_cid_player(*player);

	monitor()->set_client_svc_id(player->cid(), player->role_id());

	Block_Buffer inner_buf;
	inner_buf.make_message(GATE_LOGIC_CLIENT_LOGIN);
	inner_buf.write_int32(repeat_login);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), player->role_id());
	monitor()->send_to_monitor(MONITOR_LOGIC, 0, 0, inner_buf);

	return 0;
}

int Gate_Client_Request::process_transmit(Gate_Player *player, Block_Buffer &buf) {
	int des_scene = 0, des_scene_order = 0;
	buf.peek_int32(des_scene);
	buf.peek_int32(des_scene_order, sizeof(int));

	MSG_DEBUG("transmit start, role:%ld. src scene:%d, des scene:%d %d", player->role_id(), player->monitor_link().scene_muid.id, des_scene, des_scene_order);

	if (player->battle_status()) {
		MSG_USER("in battle not transmit, role:%ld", player->role_id());
		return 10200019;
	}

	monitor()->tran_player_add(player->role_id());

	player->set_recv_pack_off(true);

	Block_Buffer inner_buf;
	Coord coord;
	inner_buf.make_message(INNER_GATE_ASK_LOGIC_TRAN);
	inner_buf.copy(&buf);
	coord.serialize(inner_buf);
	inner_buf.write_bool(false);
	inner_buf.finish_message();
	return player->send_to_logic(inner_buf);
}
