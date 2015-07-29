/*
 * Scene_Message.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Ti
 */

#include "Scene_Message_Unit.h"
#include "Scene_Monitor.h"
#include "Scene_Client_Request.h"
#include "Scene_Server_Request.h"
#include "Global_Scene_Manager.h"
#include "Server_Monitor.h"
#include "Err_Code.h"
#include "Scene_Player.h"
#include "Scene_Timer_Request.h"
#include "Pool_Manager.h"
#include "NPC/NPC_Manager.h"
#include "Msg_Inner_Enum.h"


Scene_Message_Unit::Scene_Message_Unit(void) {
	pause_ts_.tv_sec = 0;
	pause_ts_.tv_nsec = 8 * 1000 * 1000;
}

Scene_Message_Unit::~Scene_Message_Unit(void) {

}

Scene_Monitor *Scene_Message_Unit::monitor(void) {
	return Message_Unit::monitor()->scene_monitor();
}

int Scene_Message_Unit::register_client_process(const uint32_t msg_id, Client_Func callback) {
	Client_Func_Map::iterator find_it = client_message_func_.find(msg_id);
	if (find_it != client_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	client_message_func_[msg_id] = callback;
	return 0;
}

int Scene_Message_Unit::process_client_message(Scene_Player &player, const uint32_t msg_id, Block_Buffer &buf) {
	Client_Func_Map::iterator mapi = client_message_func_.find(msg_id);
	if (mapi == client_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(player, buf);
}

int Scene_Message_Unit::register_server_process(const uint32_t msg_id, Server_Func callback) {
	Server_Func_Map::iterator find_it = server_message_func_.find(msg_id);
	if (find_it != server_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	server_message_func_[msg_id] = callback;
	return 0;
}

int Scene_Message_Unit::process_server_message(int cid, role_id_t role_id, Scene_Player *player, const uint32_t msg_id, Block_Buffer &buf) {
	Server_Func_Map::iterator mapi = server_message_func_.find(msg_id);
	if (mapi == server_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return ERROR_MSG_NOT_EXIST;
	}
	return mapi->second(cid, role_id, player, buf);
}

int Scene_Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {
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

int Scene_Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_server_msg(msg_id, cid, buf));

	int error_ret = 0;
	role_id_t role_id = 0;
	Scene_Player *player = 0;
	if (get_msg_type(msg_id) == CLIENT_TO_SCENE) {
		JUDGE_ERROR_RETURN(buf.read_int64(role_id));

		player = monitor()->find_scene_player_by_role_id(role_id);
		if (! player) {
			MSG_USER("find player error, role:%ld msg_id:%d", role_id, msg_id);
			return -1;
		}

		if (!player->validate_mover_scene()) {
			MSG_USER("mover scene error, role:%ld msg_id:%d, scene:%d", role_id, msg_id, player->move_scene_id());
			return -1;
		}

		// 跳过头
		buf.set_read_idx(buf.get_read_idx() + 8);
		error_ret = process_client_message(*player, msg_id, buf);

	} else {
		JUDGE_ERROR_RETURN(buf.read_int64(role_id));

		player = monitor()->find_scene_player_by_role_id(role_id);
		if (player && !player->validate_mover_scene()) {
			int scene_id = player ? player->move_scene_id() : -2;
			MSG_USER("mover scene error, role:%ld msg_id:%d, scene:%d", role_id, msg_id, scene_id);

			if (msg_id == LOGIC_SCENE_LOGOUT_READY || msg_id == INNER_ASK_SCENE_SIGN_OUT) {
				// 如果是在退出流程，场景空是也继续, 不正常 要查场景为什么空
				MSG_USER("scene null sign out");
			} else {
				return -1;
			}
		}

		// 跳过头
		buf.set_read_idx(buf.get_read_idx() + 8);
		error_ret = process_server_message(cid, role_id, player, msg_id, buf);
	}

	if (error_ret && player) {
		if (error_ret < 0) {
			// MSG_USER("do not return -1 to client:%d, error:%d", msg_id, error_ret);
		} else {
			if (error_ret == 10000000) {
				MSG_USER("server inner error to client:%d", msg_id);
			}
			player->send_err_to_client(error_ret, msg_id);
		}
	}

	return 0;
}

int Scene_Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
	JUDGE_CALL_RETURN(Message_Unit::process_async_buff(msg_id, buf));

	switch (msg_id) {
	case INNER_ASYNC_STATUS: {
		role_id_t role_id = 0;
		Attack_Param attack_param;
		int status_id = 0;
		int effect_id = 0;
		int status_effect_type = 0;
		buf.read_int64(role_id);
		attack_param.buff_deserialize(buf);
		buf.read_int32(status_id);
		buf.read_int32(effect_id);
		buf.read_int32(status_effect_type);
		Scene_Player *sp = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
		if (sp) {
			sp->fighter()->actual_sub_blood_for_buff(attack_param, status_id, effect_id, 0, status_effect_type);
			return 0;
		}
		Monster *monster = NPC_MANAGER->find_monster(role_id);
		if (monster) {
			monster->fighter()->actual_sub_blood_for_buff(attack_param, status_id, effect_id, 0, status_effect_type);
			return 0;
		}
		Hero *hero = NPC_MANAGER->find_hero(role_id);
		if (hero) {
			hero->fighter()->actual_sub_blood_for_buff(attack_param, status_id, effect_id, 0, status_effect_type);
			return 0;
		}
		Machine *machine = NPC_MANAGER->find_machine_npc(role_id);
		if (machine) {
			machine->fighter()->actual_sub_blood_for_buff(attack_param, status_id, effect_id, 0, status_effect_type);
			return 0;
		}
		Material *material = NPC_MANAGER->find_material_npc(role_id);
		if (material) {
			material->fighter()->actual_sub_blood_for_buff(attack_param, status_id, effect_id, 0, status_effect_type);
			return 0;
		}
		Drops *drops = NPC_MANAGER->find_drops_npc(role_id);
		if (drops) {
			drops->fighter()->actual_sub_blood_for_buff(attack_param, status_id, effect_id, 0, status_effect_type);
			return 0;
		}
		break;
	}
	case INNER_ASYNC_BE_SKILL_CALL_ACTIVE_SKILL: {
		role_id_t role_id = 0;
		int8_t type = 0;
		buf.read_int64(role_id);
		buf.read_int8(type);
		Scene_Player *sp = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
		if (0 != sp) {
			sp->fighter()->async_be_skill_trigger_active_skill(type);
		}
		Monster *monster = NPC_MANAGER->find_monster(role_id);
		if (0 != monster) {
			monster->fighter()->async_be_skill_trigger_active_skill(type);
		}
		Hero *hero = NPC_MANAGER->find_hero(role_id);
		if (0 != hero) {
			hero->fighter()->async_be_skill_trigger_active_skill(type);
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

int Scene_Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	JUDGE_CALL_RETURN(Message_Unit::process_timer_msg(msg_id, buf, now));

	switch (msg_id) {
	case TIMER_GLOBAL_MONITOR_SELF: {
		SCENE_TIMER_REQUEST->scene_tick(now);
		break;
	}
	default: {
		MSG_USER("unknow msg:%d", msg_id);
		break;
	}
	}

	return 0;
}

void Scene_Message_Unit::show_server_status(void) {
	GLOBAL_SCENE_MANAGER->show_scene_status();
	LOG_TEXT("-----npc pool status -----");
	NPC_MANAGER->show_pool_status();
	LOG_TEXT("-----global pool status -----");
	POOL_MANAGER->show_pool_status();

	Message_Unit::show_server_status();

}
