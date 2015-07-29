/*
 * Chat_Server_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Chat_Server_Request.h"
#include "Msg_Inner_Struct.h"
#include "Chat_Monitor.h"
#include "Pool_Manager.h"
#include "Chat_Player.h"
#include "module/gang/Chat_Gang_Manager.h"
#include "team/Chat_Team_Manager.h"
#include "chater/Chater_Manager.h"
#include "gm_ann/GM_Ann_Manager.h"
#include "Msg_Php_Struct.h"
#include "chat_control/Chat_Control_Manager.h"

Chat_Server_Request::Chat_Server_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = CHAT_MONITOR;
}

Chat_Server_Request::~Chat_Server_Request() {
	// TODO Auto-generated destructor stub
}

int Chat_Server_Request::logic_login_ready(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	if (player) {
		MSG_USER("player exist!");
		return -1;
	}

	player = POOL_MANAGER->pop_chat_player();
	if (!player) {
		LOG_ABORT("pop player error");
		return -1;
	}

	player->load_detail(buf);
	player->module_init();
	CHAT_GANG_MANAGER->add_ganger( player->base_detail().gang_id, player->role_id() );

	monitor()->bind_chat_player(*player);

	Block_Buffer inner_buf;
	inner_buf.make_message(LOGIC_CHAT_LOGIN_READY);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), role_id);
	monitor()->send_to_server_by_cid(cid, inner_buf);

	MSG_DEBUG("login chat finish, role:%ld, role_name:%s", role_id, player->base_detail().role_name.c_str());

	return 0;
}

int Chat_Server_Request::logic_logout_ready(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	if (!player) {
		MSG_USER("can not find player, role:%ld", role_id);
		return 0;
	}

	CHAT_GANG_MANAGER->remove_ganger( player->base_detail().gang_id, player->role_id() );

	player->sign_out();
	monitor()->unbind_chat_player(role_id);

	MSG_DEBUG("chat logout success, role:%ld", role_id);


	Block_Buffer inner_buf;
	inner_buf.make_message(LOGIC_CHAT_LOGOUT_READY);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), role_id);
	monitor()->send_to_server_by_cid(cid, inner_buf);

	return 0;
}

int Chat_Server_Request::process_20100015(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Monitor_Unique_ID scene_muid;
	scene_muid.deserialize(buf);
	player->set_scene_muid(scene_muid);
	//MSG_DEBUG("set muid success, role:%ld", role_id);

	return 0;
}

int Chat_Server_Request::process_20100028(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	std::string new_name = "";
	if (0 != buf.read_string(new_name) || new_name.empty()) {
		return -1;
	}
	player->set_role_name(new_name);
	return 0;
}

int Chat_Server_Request::process_20100029(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {

	JUDGE_PLAYER_NULL(player, role_id);
	uint8_t gender = 0;
	buf.read_uint8(gender);
	player->modify_gender(gender);
	return 0;
}

int Chat_Server_Request::process_20170000(int cid, role_id_t role_id, Chat_Player* player, Block_Buffer& buf) {
	if ( !player ) return 0;
	MSG_20170000 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
//	Base_Detail detail = player->base_detail();
//	if ( msg.gang_id == 0 ) {
	CHAT_GANG_MANAGER->remove_ganger( player->base_detail().gang_id, player->role_id() );
//	}
//	detail.gang_id = msg.gang_id;
//	detail.gang_name = msg.gang_name;
//	player->set_base_detail( detail );
	player->update_gang_info(msg.gang_id, msg.gang_name);
	CHAT_GANG_MANAGER->add_ganger( player->base_detail().gang_id, player->role_id() );
	return 0;
}

int Chat_Server_Request::process_20500000(int cid, role_id_t role_id, Chat_Player* player, Block_Buffer& buf) {
	MSG_20500000 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return CHATER_MANAGER->send_announce(msg.district, msg.announce_id, msg.gang_id, msg.recievers, msg.contents);
}

int Chat_Server_Request::process_20010085(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20010085 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player->client_ready_now();
}

int Chat_Server_Request::process_20160000(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20160000 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->syn_black_list(msg.type, msg.role_id);
	return 0;
}

int Chat_Server_Request::process_20100301(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100301 msg;
	player->syn_teamer_info(buf);
	return 0;
}

int Chat_Server_Request::process_20100302(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	CHAT_TEAM_MANAGER->add_team(buf);
	return 0;
}

int Chat_Server_Request::process_20100099(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100099 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player->chat_in_bugle(msg.content);
}

int Chat_Server_Request::process_30100008(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	MSG_30100008 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	GM_ANN_MANAGER->set_gm_announce(msg.id, msg.type, msg.stime, msg.etime, msg.step, msg.contents);
	return 0;
}

int Chat_Server_Request::process_30100005(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	MSG_30100005 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	if(msg.ban_type == 1) {
		if(msg.role_id) {
			CHATER_CONTROL_MANAGER->set_role_id_account(msg.role_id, msg.unban_time);
		} else {
			CHATER_CONTROL_MANAGER->set_bag_account(msg.account, msg.unban_time);
		}
	} else {
		if(msg.role_id) {
			CHATER_CONTROL_MANAGER->unset_role_id_account(msg.role_id);
		} else {
			CHATER_CONTROL_MANAGER->unset_bag_account(msg.account);
		}
	}
	return 0;
}

int Chat_Server_Request::process_20200024(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	MSG_20200024 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	player->set_vip(msg.over_time, msg.vip_level);
	return 0;
}

int Chat_Server_Request::process_20200025(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	MSG_20200025 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	player->set_level(msg.level);
	return 0;
}


int Chat_Server_Request::process_20100098(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf) {
	//JUDGE_PLAYER_NULL(player, role_id);
	int32_t limit_level = 0;
	buf.read_int32(limit_level);
	Block_Buffer tmp_buf;
	tmp_buf.copy(&buf);
	return CHAT_MONITOR->send_to_all_players(tmp_buf, limit_level);
	//return player->broadcast_buf(tmp_buf);
}
