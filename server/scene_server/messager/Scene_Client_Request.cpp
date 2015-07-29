/*
 * Scene_Client_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "module/NPC/monster/Monster_Def.h"
#include "Scene_Client_Request.h"
#include "Scene_Monitor.h"
#include "Config_Cache.h"
#include "Game_Manager.h"
#include "DB_Manager.h"
#include "Pool_Manager.h"
#include "Scene_Player.h"
#include "Utility_Func.h"
#include "Global_Scene_Manager.h"
#include "Err_Code.h"
#include "Head_Enum.h"
#include "Head_Struct.h"
#include "world_boss/Gang_Boss_Manager.h"
#include "world_boss/World_Boss_Manager.h"
#include "NPC/NPC_Manager.h"
#include "Scene_Public.h"
#include "team/Scene_Team_Manager.h"
#include "Scene_Func.h"
#include "Config_Cache_Scene.h"
#include "Msg_Chat_Struct.h"
#include "Msg_Honor_Arena_Struct.h"
#include "Msg_Honor_Arena_Enum.h"
#include "Battle_Scene.h"
#include "Move_Scene.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "gang_war_scene/Gang_War_Scene.h"
#include "Config_Cache_Public.h"
#include "answer/Answer_Scene.h"
#include "team/Team_Func.h"
#include "expedition_scene/Expedition_Scene.h"
#include "Config_Cache_Expedition.h"
#include "module/campfire_scene/Campfire_Scene_Manager.h"

Scene_Client_Request::Scene_Client_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = SCENE_MONITOR;
}

Scene_Client_Request::~Scene_Client_Request() {
	// TODO Auto-generated destructor stub
}

int Scene_Client_Request::process_10200030(Scene_Player &player, Block_Buffer &buf) {
	player.move_scene_completed();

	return 0;
}

int Scene_Client_Request::process_10299999(Scene_Player &player, Block_Buffer &buf) {
	MSG_10299999 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.test_command(msg.command);
}

int Scene_Client_Request::process_10200006(Scene_Player &player, Block_Buffer &buf) {
	player.get_role_info();
	return 0;
}

int Scene_Client_Request::process_10200102(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200102 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	int ret = 0;

	// player move
	if (msg.role_id == player.role_id()) {
		ret = player.move(msg.path);
		if (ret) {
			if(ret != -1){
				player.reset_coord();
			}
		} else {
			player.sync_mover_detail_to_logic();
		}
		if(player.move_scene()){
			player.move_scene()->hook_player_move(&player);
		}
	// hero move
	} else {
		Hero *hero = SCENE_PUBLIC->find_mover_hero(player.role_id());
		if (hero) {
			if (msg.role_id == hero->hero_base_detail().hero_role_id) {
				ret = hero->move(msg.path);
				if (ret) {
					hero->reset_coord();
				} else {
					//player.sync_mover_detail_to_logic();
				}
			}
		}
	}

	return ret;
}

int Scene_Client_Request::process_10200103(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200103 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	int ret = 0;
	Hero *hero = SCENE_PUBLIC->find_mover_hero(player.role_id());
	if (hero) {
		ret = hero->move(msg.path);
		if (ret) {
			//player.reset_coord();
		} else {
			//player.sync_mover_detail_to_logic();
		}
	}

	return ret;
}

int Scene_Client_Request::process_10200032(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200032 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	Scene_Player* scene_player = SCENE_MONITOR->find_scene_player_by_role_id(msg.role_id);
	MSG_50200032 res_msg;
	res_msg.open_type = msg.open_type;
	res_msg.role_id = msg.role_id;
	if (scene_player) {
		scene_player->fighter_detail().property_list(res_msg.prop_value);
		Hero *hero = SCENE_PUBLIC->find_fight_hero(scene_player->role_id());
		if (hero) {
			int hero_init_morale_type = hero->fighter_detail().init_morale.type;
			double hero_init_morale_val = hero->fighter_detail().init_morale.total();
			for (uint16_t i = 0; i < res_msg.prop_value.size(); ++i) {
				if (res_msg.prop_value[i].type == hero_init_morale_type) {
					res_msg.prop_value[i].value += hero_init_morale_val;
				}
			}
		}

		res_msg.prop_value.push_back(Property(PT_CAREER, scene_player->career()));
		res_msg.prop_value.push_back(Property(PT_CHARM, scene_player->base_detail().charm)); /*魅力值*/

		res_msg.role_name = scene_player->role_name();/*人物姓名*/
		res_msg.gang_name = scene_player->base_detail().gang_name;/*人物id*/
		res_msg.gender = scene_player->base_detail().gender;/*性别（1男，2女）*/
		res_msg.gang_id = scene_player->base_detail().gang_id;/*职业*/
		res_msg.headship = scene_player->ganger_detail().headship;
		res_msg.avatar_vec = scene_player->base_detail().avatar_vec;

	} else {
		res_msg.role_id = 0;
		OBJ_SEND_TO_CLIENT(res_msg, player);
		return ERROR_PLAYER_NOT_ONLINE;
	}

	OBJ_SEND_TO_CLIENT(res_msg, player);

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_OPEN_ROLE_PANEL);
	inner_buf.write_int64(msg.role_id);
	inner_buf.finish_message();
	player.send_to_logic(inner_buf);

	return 0;
}

int Scene_Client_Request::process_10200105(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200105 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.battle_scene_completed();
}

int Scene_Client_Request::process_10200150(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200150 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	if (0 == msg.role_id) return ERROR_INNER_SERVER;

	// 是否在相同场景
	Mover *mover = player.find_mover_in_move_scene(msg.role_id);
	if (! mover) {
		MSG_USER("can not find scene player:%ld", msg.role_id);
		return ERROR_PLAYER_NOT_ONLINE;
	}

	if (! mover->player_self()) {
		MSG_USER("is not player:%ld", msg.role_id);
		return ERROR_CLIENT_OPERATE;
	}

	if(mover->player_self()->is_safe_time()){
		return ERROR_FIGHT_SAFE_TIME;
	}

	if (mover->player_self()->gang_war_info().revive == 1 ||
			mover->player_self()->war_info()->revive == 1 ||
			mover->player_self()->expedition_scene_info().revive == 1 ||
			mover->player_self()->is_revive()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.gang_war_info().revive == 1 ||
			player.war_info()->revive == 1 ||
			player.expedition_scene_info().revive == 1 ||
			player.is_revive()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}

//	if (!mover->is_in_mover_scene()) {
//		return ERROR_PLAYER_IN_BATTLE;
//	}

	if (mover->is_in_battle()) {
		if(!mover->player_self()
				|| !mover->move_scene() || 0 == mover->move_scene()->scene_id()){
			MSG_USER("has in battle");
			return ERROR_PLAYER_IN_BATTLE;
		}
		const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(mover->move_scene()->scene_id());
		if (!cfg) {
			return ERROR_FIGHT_NOT_EMPTY;
		}
		int pos = -1;
		if (Scene_Gang_War == cfg->manager
				&& mover->player_self()->gang_war_info().gang_id > 0) {
			for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
					it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
				if (it->second == mover->player_self()->role_id()) {
					pos = it->first.pos;
					break;
				}
			}
			const Gang_War_Matched_Gang_Info *info_mover = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(mover->player_self()->ganger_detail().gang_id);
			int64_t league_mover = mover->player_self()->gang_war_info().league;
			if (info_mover) {
				league_mover = info_mover->camp;
			}
			const Gang_War_Matched_Gang_Info *info_player = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(player.ganger_detail().gang_id);
			int64_t league_player = player.gang_war_info().league;
			if (info_player) {
				league_player = info_player->camp;
			}
			if (league_mover != league_player) {
				if (pos == 0) {
					pos = 1;
				} else if (pos == 1) {
					pos = 0;
				} else {
					return -1;
				}
			}
		} else if (0 != mover->player_self()->get_team_id()
					&& mover->player_self()->get_team_id() == player.get_team_id()) {
			for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
					it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
				if (it->second == mover->player_self()->role_id()) {
					pos = it->first.pos;
					break;
				}
			}
		} else if (Scene_Expedition_Manager == cfg->manager
				&& mover->player_self()->gang_war_info().gang_id > 0) {
			Role_Hash_Set rhs;
			for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
					it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
				if (it->second == mover->player_self()->role_id()) {
					pos = it->first.pos;
//					break;
				}
				Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(it->second);
				if (pl && pl->gang_id()) {
					rhs.insert(pl->gang_id());
				}
			}
			if (!rhs.count(player.ganger_detail().gang_id)) {
				return ERROR_EXPEDITION_OTHER_ATTACKER;
			}
			if (player.ganger_detail().gang_id != mover->player_self()->ganger_detail().gang_id) {
				if (pos == 0) {
					pos = 1;
				} else if (pos == 1) {
					pos = 0;
				} else {
					return -1;
				}
			}
		}
		if (pos == -1) {
			return -1;
		}
		Int_Hash_Set idx_set;
		for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
				it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
			if (it->first.pos != pos) continue;
			idx_set.insert(it->first.idx);
		}
		int ret = ensure_battle_pos_unique(idx_set);
		if (0 != ret) {
			return ret;
		}
		int idx = Position_Key::LINE_ONE_FRONT;
		get_empty_pos(idx_set, idx);
		if (-1 == idx) {
			return ERROR_FIGHT_NOT_EMPTY;
		}
		if(player.set_battle_position(pos, idx, mover->battle_scene()) == 0){
			player.enter_battle_scene(mover->battle_scene());
		}
		return 0;
	}

	if (player.dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (mover->fighter()->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player.base_detail().location.scene_id);
	if (scene_config) {
		switch(scene_config->manager) {
		case Scene_Gang_War:
		case Scene_Expedition_Manager: {
			if (player.ganger_detail().gang_id == mover->player_self()->ganger_detail().gang_id) {
				return -1;
			}
			break;
		}
		case Scene_War: {
			if (player.war_info()->league == mover->player_self()->war_info()->league) {
				return -1;
			}
			break;
		}
		default:
			break;
		}
	}
	int64_t team_id = player.get_team_id();
	Map_Team *team = SCENE_TEAM_MANAGER->get_team(team_id);
	if (team && team->is_in_team(player.role_id())) {
		return ERROR_TEAM_BATTLE_LIMIT;
	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player.move_scene());

	if (Expedition_Type == get_scene_type(player.move_scene_id()) ) {
		int map_id = 0;
		const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(BATTLE_SCENE);
		if(conf) {
			map_id = conf->value;
		}
		scene->set_map_id(map_id);
	}

	//player.set_battle_position(0, Position_Key::LINE_TWO_FRONT);
	Position_Key player_pos_0(0, Position_Key::LINE_TWO_FRONT);
	if(player.set_battle_position(player_pos_0.pos, player_pos_0.idx, scene) == 0){
		player.enter_battle_scene(scene);
	}

	//mover->set_battle_position(1, Position_Key::LINE_TWO_FRONT);
	Position_Key player_pos_1(1, Position_Key::LINE_TWO_FRONT);
	scene->init_battle_hero_info(mover->fighter(), player_pos_1);
	if(mover->set_battle_position(player_pos_1.pos, player_pos_1.idx, scene) == 0){
		mover->player_self()->enter_battle_scene(scene);
	}
	return 0;
}

int Scene_Client_Request::process_10200156(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200156 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	if (0 == msg.role_id) return ERROR_INNER_SERVER;

	Mover *mover = player.find_mover_in_move_scene(msg.role_id);
	if (0 == mover) {
		MSG_USER("can't find scene player:%ld", msg.role_id);
		return ERROR_PLAYER_NOT_ONLINE;
	}

	if (0 == mover->player_self()) {
		MSG_USER("not a player:%ld", msg.role_id);
		return ERROR_CLIENT_OPERATE;
	}

	if(msg.type  == 1) {
		const Challenge_Area_Cfg *challenge_area_cfg =  CONFIG_CACHE_PUBLIC->find_challenge_area_cfg((uint32_t)player.move_scene_id());
		if(!challenge_area_cfg) {
			MSG_USER("scene not area, role_name=%s, scene_id=%d", player.role_name().c_str(), player.move_scene_id());
			return ERROR_NOT_IN_CHALLENGE_SCENE;
		}

		if(challenge_area_cfg) {
			if(player.pixel_coord().x < (uint16_t)challenge_area_cfg->left_border
					|| player.pixel_coord().x > (uint16_t)challenge_area_cfg->right_border
					|| player.pixel_coord().y < (uint16_t)challenge_area_cfg->up_border
					|| player.pixel_coord().y > (uint16_t)challenge_area_cfg->down_border) {
				return ERROR_CHALLENGE_SELF_NOT_IN_AREA;
			}
		}

		if(mover->move_scene_id()!= player.move_scene_id()) {
			return ERROR_NO_SAME_SCENE_NOT_CHALLENGE;
		}

		if(mover->pixel_coord().x < (uint16_t)challenge_area_cfg->left_border
				|| mover->pixel_coord().x > (uint16_t)challenge_area_cfg->right_border
				|| mover->pixel_coord().y < (uint16_t)challenge_area_cfg->up_border
				|| mover->pixel_coord().y > (uint16_t)challenge_area_cfg->down_border) {
			return ERROR_CHALLENGE_TARGET_NOT_IN_AREA;
		}
	}

	MSG_50200153 res_msg;
	res_msg.role_id = player.role_id();
	res_msg.role_name = player.role_name();
	res_msg.type = msg.type;

	OBJ_SEND_TO_CLIENT(res_msg, (*mover->player_self()));

	return 0;
}

int Scene_Client_Request::process_10200155(Scene_Player &player, Block_Buffer &buf) {

	MSG_10200155 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	if (0 == msg.role_id) return ERROR_INNER_SERVER;

	// 是否在相同场景
	Mover *mover = player.find_mover_in_move_scene(msg.role_id);
	if (! mover) {
		MSG_USER("can not find scene player:%ld", msg.role_id);
		return ERROR_PLAYER_NOT_ONLINE;
	}

	if (! mover->player_self()) {
		MSG_USER("is not player:%ld", msg.role_id);
		return ERROR_CLIENT_OPERATE;
	}

	MSG_50200155 res_msg;
	res_msg.duel = msg.duel;
	res_msg.role_name = player.role_name();
	OBJ_SEND_TO_CLIENT(res_msg, (*mover->player_self()));

	if (0 == msg.duel) {			// 不接受挑战
		return 0;
	}

	if(mover->player_self()->is_safe_time()){
		return ERROR_FIGHT_SAFE_TIME;
	}

	if (mover->player_self()->gang_war_info().revive == 1
			|| mover->player_self()->expedition_scene_info().revive == 1) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.gang_war_info().revive == 1
			|| player.expedition_scene_info().revive == 1) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}

	if (mover->is_in_battle()) {
		if(mover->player_self()){
			if (mover->move_scene() && mover->move_scene()->scene_id()) {
				const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(mover->move_scene()->scene_id());
				if (cfg
						&& (Scene_Expedition_Manager == cfg->manager || Scene_Gang_War == cfg->manager)
						&& mover->player_self()->gang_war_info().gang_id > 0) {
					int pos = -1;
					for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
							it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
						if (it->second != mover->player_self()->role_id()) continue;
						pos = it->first.pos;
						break;
					}
					if (pos == -1) {
						return ERROR_FIGHT_NOT_EMPTY;
					}
					Int_Hash_Set idx_set;
					for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
							it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
						if (it->first.pos == pos) continue;
						idx_set.insert(it->first.idx);
					}
					int ret = ensure_battle_pos_unique(idx_set);
					if (0 != ret) {
						return ret;
					}
					if (pos == 0) {
						pos = 1;
					} else {
						pos = 0;
					}
					int idx = Position_Key::LINE_ONE_FRONT;
					get_empty_pos(idx_set, idx);
					if (-1 == idx) {
						return ERROR_FIGHT_NOT_EMPTY;
					}
					if(player.set_battle_position(pos, idx, mover->battle_scene()) == 0){
						player.enter_battle_scene(mover->battle_scene());
					}
					return 0;
				}
			}
		}
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}

	if (player.dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (mover->fighter()->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

//	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player.base_detail().location.scene_id);
//	if (scene_config && Scene_Team == scene_config->manager) {
	int64_t team_id = player.get_team_id();
	Map_Team *team = SCENE_TEAM_MANAGER->get_team(team_id);
	if (team && team->is_in_team(player.role_id())) {
		return ERROR_TEAM_BATTLE_LIMIT;
	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player.move_scene());

	if(msg.type == 1) {
		scene->set_battle_type(Battle_Scene::BATTLE_TYPE_CHALLENGE);
	}

	//player.set_battle_position(1, Position_Key::LINE_TWO_FRONT);
	Position_Key player_pos_1(1, Position_Key::LINE_TWO_FRONT);
	scene->init_battle_hero_info(&player, player_pos_1);
	if(player.set_battle_position(player_pos_1.pos, player_pos_1.idx, scene) == 0){
		player.enter_battle_scene(scene);
	}

	//mover->set_battle_position(0, Position_Key::LINE_TWO_FRONT);
	Position_Key player_pos_0(0, Position_Key::LINE_TWO_FRONT);
	scene->init_battle_hero_info(mover->fighter(), player_pos_0);
	if(mover->set_battle_position(player_pos_0.pos, player_pos_0.idx, scene) == 0){
		mover->player_self()->enter_battle_scene(scene);
	}
	return 0;
}

int Scene_Client_Request::process_10200110(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200110 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	Fighter *fighter = player.find_fighter_in_battle_scene(msg.role_id);
	if (! fighter) {
		MSG_USER("can not find scene player:%ld, mostly battle is end", msg.role_id);
		return 0;
	}

	int ret = fighter->use_skill(msg.skill);
	if (0 != ret) {
		MSG_81000070 ret_msg;
		ret_msg.attack_id = msg.role_id;
		ret_msg.skill_id = msg.skill;
		ret_msg.errcode = ret;
		OBJ_SEND_TO_CLIENT(ret_msg, player);
	}
	return 0;
}

int Scene_Client_Request::process_10200111(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200111 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	return player.handle_fight_pause(msg.type);
}

int Scene_Client_Request::process_10200113(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200113 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_give_up_battle();
}

int Scene_Client_Request::process_10200117(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200117 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.handle_give_up_battle_multi(msg.type);
}

int Scene_Client_Request::process_10210000(Scene_Player &player, Block_Buffer &buf){
	MSG_10210000 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	Drops* drop = NPC_MANAGER->find_drops_npc(msg.role_id);
	if(!drop){
		MSG_USER("can not find scene drops:%ld", msg.role_id);
		return ERROR_ROLE_ID_NOEXIST;
	}
	Block_Buffer res_buff;
	int result = drop->fetch_drops_list(&player, res_buff);
	if(result != 0){
		return result;
	}
	player.send_to_client(res_buff);
	return 0;
}

int Scene_Client_Request::process_10210001(Scene_Player &player, Block_Buffer &buf){
	MSG_10210001 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	player.get_drops_goods(msg);
	return 0;
}

int Scene_Client_Request::process_10210100(Scene_Player &player, Block_Buffer &buf){
	MSG_10210100 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.start_gather(msg);
}

int Scene_Client_Request::process_10210101(Scene_Player &player, Block_Buffer &buf){
	MSG_10210101 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.stop_gather(msg);
}

int Scene_Client_Request::process_10210103(Scene_Player &player, Block_Buffer &buf){
	MSG_10210103 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.open_machine(msg);
}

int Scene_Client_Request::process_10210200(Scene_Player &player, Block_Buffer &buf){
	MSG_10210200 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.create_battle_monster(msg);
}

int Scene_Client_Request::process_10200005(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200005 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	if (player.validate_move_value(msg.coord.x, msg.coord.y)) {
		MSG_81000025 res_msg;
		res_msg.coord = msg.coord;
		OBJ_SEND_TO_CLIENT(res_msg, player);
	} else {
		return ERROR_MOVE_TO_TARGET_COORD;
	}

	return 0;
}

int Scene_Client_Request::process_10200007(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200007 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	if (player.validate_move_value(msg.coord.x, msg.coord.y)) {
		player.change_coord_by_pixel(msg.coord.x, msg.coord.y);

		Hero *hero = SCENE_PUBLIC->find_mover_hero(player.role_id());
		if (hero) {
			hero->change_coord_by_pixel(msg.coord.x, msg.coord.y);
		}
	}

	return 0;
}


int Scene_Client_Request::process_10200149(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200149 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	if (player.is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}
	if (player.dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}
	NPC_MANAGER->create_battle_with_monster(&player, msg.monster_type_id, msg.type);
	return 0;
}

int Scene_Client_Request::process_10200151(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200151 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	Mover *mover = player.find_mover_in_move_scene(msg.role_id);
	if (! mover) {
		MSG_USER("can not find fight target:%ld", msg.role_id);
		return ERROR_FIGHT_NO_TARGET;
	}

	if (player.is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}
	// 世界boss
	if(mover->move_scene()){
		if(mover->move_scene()->scene_type() == World_Boss_Scene_Type){
			return WORLD_BOSS_MANAGER->req_fight_world_boss(&player);
		}else if(mover->move_scene()->scene_type() == Gang_Boss_Scene_Type){
			World_Boss_Manager* wb_manager = GANG_BOSS_MANAGER->world_boss_manager(player.gang_id());
			if(wb_manager){
				return wb_manager->req_fight_world_boss(&player);
			}
		}else if (mover->move_scene()->scene_type() == Campfire_Scene_Type) {
			//TODO
			int ret = CAMPFIRE_SCENE_MANAGER->player_req_battle(&player, msg.role_id);
			if (ret != 1) //1代表继续处理
				return ret;
		}
	}

	if(mover->npc_self()->npc_status() == NPC::FIGHT_STATUS && mover->npc_self()->npc_record()->battle_scene){
		if(mover->npc_self()->npc_record() && mover->npc_self()->npc_record()->is_teams_up()){//组队打怪
			int pos = 0;
			Int_Hash_Set idx_set;
			{
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->npc_self()->npc_record()->battle_scene->pos_debug_map().begin();
						it != mover->npc_self()->npc_record()->battle_scene->pos_debug_map().end(); ++it) {
					if (it->first.pos != pos) continue;
					idx_set.insert(it->first.idx);
				}
				int ret = ensure_battle_pos_unique(idx_set);
				if (0 != ret) {
					return ret;
				}
			}
			for (Fighter_Set::iterator it = mover->npc_self()->npc_record()->fight_player_set.begin();
					it != mover->npc_self()->npc_record()->fight_player_set.end(); ++it) {
				if (!(*it)->mover_detail().is_player()) {
					continue;
				}
				Map_Team* team = SCENE_TEAM_MANAGER->get_team((*it)->player_self()->get_team_id());
				if (NULL == team) {
					continue;
				}
				int idx = Position_Key::LINE_ONE_FRONT;
				if (!team->is_in_team(player.role_id())) {
					continue;
				}
				if (0 != player.team_position().idx) {
					idx = player.team_position().idx;
				}
				get_empty_pos_check_idx(idx_set, idx);
				if (-1 == idx) {
					continue;
				}
				idx_set.insert(idx);
				mover->npc_self()->npc_record()->join_battle(player.fighter(), pos, idx);
				break;
			}
			return 0;
		}
		MSG_USER("has in battle");
		return ERROR_FIGHT_IN_BATTLE;
	}

	if (player.dead_status() || player.is_revive() || player.war_info()->revive == 1
			|| player.gang_war_info().revive == 1){
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

//	if (mover->fighter()->dead_status()) {
//		MSG_USER("has die");
//		return ERROR_FIGHT_IN_DIE;
//	}

	if(mover->monster_self() == NULL){
		MSG_USER("fight is not monster");
		return ERROR_FIGHT_NO_TARGET;
	}
	Monster* monster = mover->monster_self();
	if(monster->npc_record() == NULL){
		MSG_USER("fight monster point is null");
		return ERROR_FIGHT_NO_TARGET;
	}
	if(monster->can_attack(&player) == false){
		return -1;
	}
	monster->npc_record()->enter_battle(&player);
	player.team_fb_create_player_monster();

	return 0;
}

int Scene_Client_Request::process_10200008(Scene_Player &player, Block_Buffer &buf) {
//	MSG_10200008 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
//	player.beat_back(buf);
	return 0;
}


int Scene_Client_Request::process_10210104(Scene_Player &player, Block_Buffer &buf) {
	MSG_10210104 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	player.beat_back(msg);
	return 0;
}

int Scene_Client_Request::process_10210002(Scene_Player &player, Block_Buffer &buf) {
	MSG_10210002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.pick_up_goods(msg.role_id_vec);
}

int Scene_Client_Request::process_10210010(Scene_Player &player, Block_Buffer &buf) {
	MSG_10210010 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_refresh_scene_point(msg.point_id);
}


int Scene_Client_Request::process_10210011(Scene_Player &player, Block_Buffer &buf) {
	MSG_10210011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fight_scene_point(msg.point_id);
}

int Scene_Client_Request::process_10200009(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200009 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_take_lift_finish(msg.lift_id);
}

int Scene_Client_Request::process_10211001(Scene_Player &player, Block_Buffer &buf) {
	return player.req_war_scene_fetch_rank();
}

int Scene_Client_Request::process_10211002(Scene_Player &player, Block_Buffer &buf) {
	return player.req_war_scene_heal();
}

int Scene_Client_Request::process_10200200(Scene_Player &player, Block_Buffer &buf) {
	return player.req_dungeon_panel_close();
}

int Scene_Client_Request::process_10200215(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200215 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_single_dungeon_revive(msg.type);
}

int Scene_Client_Request::process_10200011(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200011 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_scene_chat(msg.content);
}

int Scene_Client_Request::process_10200152(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200152 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_attck_rob_match_member(msg.to_uuid, msg.to_server_name, msg.uuid,
											 msg.role_id, msg.server_name, msg.role_name, msg.coin);
}

int Scene_Client_Request::process_10200153(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200153 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_accept_fight_with_robber(msg.uuid, msg.role_id);
}

int Scene_Client_Request::process_10200154(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200154 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	SCENE_DRAGON_VALE_MANAGER->process_drive_thief(player.role_id(), (uint8_t)2, msg.role_id, msg.monster_id, msg.quality);
	return 0;
}

int Scene_Client_Request::process_10200701(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200701 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_campfire_add_firewood(msg.role_id);
}


int Scene_Client_Request::process_10211100(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211100 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_world_boss_use_ability(msg.icon_id, msg.type);
}

int Scene_Client_Request::process_10211101(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211101 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_world_boss_act_guild_bless(msg.icon_id);
}


int Scene_Client_Request::process_10211201(Scene_Player &player, Block_Buffer &buf) {
//	MSG_10211201 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_honor_arena_match();
}

int Scene_Client_Request::process_10211202(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211202 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_honor_arena_receive(msg.type);
}

int Scene_Client_Request::process_10211205(Scene_Player &player, Block_Buffer &buf) {
//	MSG_10211205 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_honor_arena_rank();
}

int Scene_Client_Request::process_10211207(Scene_Player &player, Block_Buffer &buf) {
//	MSG_10211207 msg;
//	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_honor_arena_panel_info();
}

int Scene_Client_Request::process_10211208(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211208 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_honor_arena_set_auto_match(msg.is_auto);
}

int Scene_Client_Request::process_10211300(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211300 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_vagrant_item_info(msg.monster_role_id);
}

int Scene_Client_Request::process_10211301(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211301 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_vagrant_buy_item(msg.monster_role_id, msg.item_index);
}

int Scene_Client_Request::process_10211310(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211310 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_fetch_tbay_info();
}

int Scene_Client_Request::process_10211311(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211311 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_enter_scene_outpost(msg.monster_role_id);
}

int Scene_Client_Request::process_10211312(Scene_Player &player, Block_Buffer &buf) {
	MSG_10211312 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_tbay_bless(msg.bless_index);
}

int Scene_Client_Request::process_10200702(Scene_Player &player, Block_Buffer &buf) {
	MSG_10200702 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_campfire_fire(msg.role_id);
}

int Scene_Client_Request::process_10200703(Scene_Player& player,
		Block_Buffer& buf) {
	MSG_10200703 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.req_campfire_rank_list();
}

int Scene_Client_Request::process_10200704(Scene_Player& player,
		Block_Buffer& buf) {
	MSG_10200704 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.get_campfire_award();
}

int Scene_Client_Request::process_10200705(Scene_Player& player,
		Block_Buffer& buf) {
	MSG_10200705 msg;
	JUDGE_MSG_ERROR_RETURN(msg);
	return player.get_campfire_rank_award();
}

int Scene_Client_Request::process_10270001(Scene_Player &player, Block_Buffer &buf) {
	return player.req_gang_war_heal();
}

int Scene_Client_Request::process_10270002(Scene_Player &player, Block_Buffer &buf) {
	MSG_10270002 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	Mover *mover = player.find_mover_in_move_scene(msg.role_id);
	if (! mover) {
		MSG_USER("can not find scene player:%ld", msg.role_id);
		return ERROR_GANG_WAR_NOT_IN_SCENE;
	}

	if (! mover->player_self()) {
		MSG_USER("is not player:%ld", msg.role_id);
		return ERROR_CLIENT_OPERATE;
	}

	if(mover->player_self()->is_safe_time()){
		return ERROR_FIGHT_SAFE_TIME;
	}

	if (mover->player_self()->gang_war_info().revive == 1) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.gang_war_info().revive == 1) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}

	if (mover->is_in_battle()) {
		if (mover->move_scene() && mover->move_scene()->scene_id()) {
			const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(mover->move_scene()->scene_id());
			if (cfg && Scene_Gang_War == cfg->manager && mover->player_self()->gang_war_info().gang_id > 0) {
				int pos = -1;
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
						it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
					if (it->second != mover->player_self()->role_id()) continue;
					pos = it->first.pos;
					break;
				}
				if (pos == -1) {
					return ERROR_FIGHT_NOT_EMPTY;
				}
				Int_Hash_Set idx_set;
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = mover->player_self()->battle_scene()->pos_debug_map().begin();
						it != mover->player_self()->battle_scene()->pos_debug_map().end(); ++it) {
					if (it->first.pos == pos) continue;
					idx_set.insert(it->first.idx);
				}
				int ret = ensure_battle_pos_unique(idx_set);
				if (0 != ret) {
					return ret;
				}
				if (pos == 0) {
					pos = 1;
				} else {
					pos = 0;
				}
				int idx = Position_Key::LINE_ONE_FRONT;
				get_empty_pos(idx_set, idx);
				if (-1 == idx) {
					return ERROR_FIGHT_NOT_EMPTY;
				}
				if(player.set_battle_position(pos, idx, mover->battle_scene()) == 0){
					player.enter_battle_scene(mover->battle_scene());
				}
				return 0;
			}
		}
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	} else {
		if (mover->fighter()->dead_status()) {
			MSG_USER("has die");
			return ERROR_PLAYER_DIE;
		}

		if (player.dead_status()) {
			MSG_USER("has die");
			return ERROR_PLAYER_DIE;
		}
		if (player.gang_id()) {
			Gang_DB_View* gdv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(player.gang_id());
			if (!gdv || gdv->leader != player.role_id()) {
				return -1;
			}
		}
		if (mover->move_scene() && mover->move_scene()->scene_id()) {
			const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(mover->move_scene()->scene_id());
			if (cfg && Scene_Gang_War == cfg->manager && mover->player_self()->gang_war_info().gang_id > 0) {
				Gang_War_Scene* dws = dynamic_cast<Gang_War_Scene*>(mover->move_scene());
				if (dws->scene_status() == Move_Scene::SCENE_NORMAL_STATUS) {
					Gang_DB_View *dbv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(mover->player_self()->gang_war_info().gang_id);
					if (dbv && dbv->leader == mover->player_self()->role_id()) {
						MSG_81170008 acv_msg;
						acv_msg.type = 3;
						acv_msg.role_id = player.role_id();
						OBJ_SEND_TO_CLIENT(acv_msg, (*mover->player_self()));
						player.gang_war_scene_announce(1307, mover->player_self()->role_id(), mover->player_self()->role_name(), dbv->name);
						return 0;
					}
				}
			}
		}
	}
	return ERROR_CLIENT_OPERATE;

//	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player.base_detail().location.scene_id);
//	if (scene_config && Scene_Team == scene_config->manager) {
//	int64_t team_id = player.get_team_id();
//	Map_Team *team = SCENE_TEAM_MANAGER->get_team(team_id);
//	if (team && team->is_in_team(player.role_id())) {
//		return ERROR_TEAM_BATTLE_LIMIT;
//	}
//
//	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene();
//
//	player.set_battle_position(0, Position_Key::LINE_TWO_FRONT);
//	player.enter_battle_scene_team(scene);
//
//	mover->set_battle_position(1, Position_Key::LINE_TWO_FRONT);
//	mover->player_self()->enter_battle_scene_team(scene);

	return 0;
}

int Scene_Client_Request::process_10270004(Scene_Player &player, Block_Buffer &buf) {
	MSG_10270004 msg;
	JUDGE_MSG_ERROR_RETURN(msg);

	Mover *mover = player.find_mover_in_move_scene(msg.role_id);
	if (! mover) {
		MSG_USER("can not find scene player:%ld", msg.role_id);
		return ERROR_PLAYER_NOT_ONLINE;
	}

	if (! mover->player_self()) {
		MSG_USER("is not player:%ld", msg.role_id);
		return ERROR_CLIENT_OPERATE;
	}

	if (msg.type == 0) {
		Gang_DB_View *dbv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(mover->player_self()->gang_war_info().gang_id);
		if (dbv) {
			player.gang_war_scene_announce(1308, mover->player_self()->role_id(), mover->player_self()->role_name(), dbv->name);
			return 0;
		}
		return 0;
	}

	if(mover->player_self()->is_safe_time()){
		return ERROR_FIGHT_SAFE_TIME;
	}

	if (mover->player_self()->gang_war_info().revive == 1) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.gang_war_info().revive == 1) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (mover->fighter()->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (player.is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}

	if (mover->is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	} else {
		if (player.gang_id()) {
			Gang_DB_View* gdv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(player.gang_id());
			if (!gdv || gdv->leader != player.role_id()) {
				return -1;
			}
		}
		if (mover->move_scene() && mover->move_scene()->scene_id()) {
			const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(mover->move_scene()->scene_id());
			const Scene_Config *cfg_p = CONFIG_CACHE_SCENE->scene_config_cache(player.move_scene()->scene_id());
			if (cfg && Scene_Gang_War == cfg->manager && mover->player_self()->gang_war_info().gang_id > 0
					&& cfg_p && Scene_Gang_War == cfg_p->manager && player.gang_war_info().gang_id > 0) {
				Gang_War_Scene* dws = dynamic_cast<Gang_War_Scene*>(mover->move_scene());
				if (dws->scene_status() == Move_Scene::SCENE_NORMAL_STATUS) {
					Block_Buffer buf;
					buf.make_message(INNER_SYNC_GANG_WAR_HELP);
					Gang_DB_View *dbv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(mover->player_self()->gang_war_info().gang_id);
					if (dbv){// && dbv->leader == mover->player_self()->role_id()) {
						buf.write_int64(mover->player_self()->gang_war_info().gang_id);
						buf.write_int64(player.role_id());
						buf.finish_message();
						player.send_to_logic(buf);
					}

					dbv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(player.gang_war_info().gang_id);

					Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player.move_scene());

					scene->set_battle_type(Battle_Scene::BATTLE_TYPE_GANG_WAR_LEADER);
					if(player.set_battle_position(0, Position_Key::LINE_TWO_FRONT, scene) == 0){
						player.enter_battle_scene(scene);
					}

					if(mover->set_battle_position(1, Position_Key::LINE_TWO_FRONT, scene) == 0){
						mover->player_self()->enter_battle_scene(scene);
					}

					buf.reset();
					buf.make_message(INNER_SYNC_GANG_WAR_HELP);
					if (dbv){// && dbv->leader == player.role_id()) {
						buf.write_int64(player.gang_war_info().gang_id);
						buf.write_int64(mover->player_self()->role_id());
						buf.finish_message();
						mover->player_self()->send_to_logic(buf);
					}
				}
			}
		}
	}

	return 0;
}

int Scene_Client_Request::process_10209001(Scene_Player &player, Block_Buffer &buf) {
	Answer_Scene* scene = dynamic_cast<Answer_Scene* >(player.move_scene());
	if(!scene) {
		return ERROR_ANSWER_NOT_IN_SCENE;
	}
	return scene->req_use_answer_free(player);
}

int Scene_Client_Request::process_10209002(Scene_Player &player, Block_Buffer &buf) {
	Answer_Scene* scene = dynamic_cast<Answer_Scene* >(player.move_scene());
	if(!scene) {
		return ERROR_ANSWER_NOT_IN_SCENE;
	}
	return scene->req_use_answer_double(player);
}

int Scene_Client_Request::process_10209003(Scene_Player &player, Block_Buffer &buf) {
	Answer_Scene* scene = dynamic_cast<Answer_Scene* >(player.move_scene());
	if(!scene) {
		return ERROR_ANSWER_NOT_IN_SCENE;
	}
	return scene->req_answer_free_double_num(player);
}

// expedition start --//
int Scene_Client_Request::process_10201300(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201300 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_daily_award_get(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201301(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201301 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_daily_award_info(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201302(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201302 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_other_build_info(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201303(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201303 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_gang_rank(&player);
	}
	return -1;
}

int Scene_Client_Request::process_10201304(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201304 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_battle_info(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201305(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201305 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_gang_base_build_info(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201306(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201306 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_fighting(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201307(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201307 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_help_info(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201308(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201308 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_change_line(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201309(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201309 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_demage_rank(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201310(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201310 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_looking(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201311(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201311 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_occupy_castle(&player, msg);
	}
	return -1;
}

int Scene_Client_Request::process_10201312(Scene_Player &player, Block_Buffer &buf) {
	if (player.move_scene() && Expedition_Type == get_scene_type(player.move_scene()->scene_id())) {
		Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(player.move_scene());
		MSG_10201312 msg;
		JUDGE_MSG_ERROR_RETURN(msg);
		return sce->req_expedition_occupy_castle_award_get(&player, msg);
	}
	return -1;
}
	// -- expedition end //
