/*
 * Scene_Public.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Scene_Public.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Utility_Func.h"
#include "Scene_Player.h"
#include "Daemon_Server.h"
#include "Global_Timer.h"
#include "Global_Scene_Manager.h"
#include "NPC/hero/Hero.h"
#include "DB_Manager.h"
#include "DB_Def.h"

Scene_Public::Scene_Public() : world_level_(0) {
}

Scene_Public::~Scene_Public() {
	// TODO Auto-generated destructor stub
}

int Scene_Public::init(int monitor_index) {

	return 0;
}

int Scene_Public::start(void) {

	return 0;
}

int Scene_Public::bind_master_id_hero(role_id_t master_id, Hero &hero) {
	master_id_hero_map_.bind(master_id, &hero);
	return 0;
}

int Scene_Public::unbind_master_id_hero(role_id_t master_id) {
	Master_ID_Hero_Map::iterator find_it = master_id_hero_map_.find(master_id);
	if (find_it != master_id_hero_map_.end()) {
		master_id_hero_map_.unbind(master_id);
	}
	return 0;
}

Hero *Scene_Public::find_mover_hero(role_id_t role_id) {
	// 查找已进入移动场景的英雄
	Master_ID_Hero_Map::iterator it = master_id_hero_map_.find(role_id);
	if (it != master_id_hero_map_.end()) {
		if (it->second->has_init() && it->second->validate_mover_scene()) {
			return it->second;
		}
	}
	return NULL;
}

Hero *Scene_Public::find_battle_hero(role_id_t role_id) {
	// 查找已进入战斗场景的英雄
	Master_ID_Hero_Map::iterator it = master_id_fight_hero_map_.find(role_id);
	if (it != master_id_fight_hero_map_.end()) {
		if (it->second->has_init() && it->second->validate_battle_scene()) {
			return it->second;
		}
	}
	return NULL;
}

void Scene_Public::recycle_all_hero(role_id_t master_id) {
	// 跟随英雄
	Hero *outer_hero = SCENE_PUBLIC->find_master_id_hero(master_id);
	if (outer_hero) {
		outer_hero->outer_hero_recycle();
	}
	// 战斗英雄
	Hero *fight_hero = SCENE_PUBLIC->find_fight_hero(master_id);
	if (fight_hero) {
		fight_hero->fight_hero_recycle();
	}
}

int Scene_Public::sync_dispath(Block_Buffer &r) {
	int8_t type = 0;
	r.read_int8(type);
	switch(type) {
	case SP_WORLD_LEVEL: {
		int world_level = 0;
		r.read_int32(world_level);
		if (world_level_ < world_level) {
			world_level_ = world_level;
		}
		MSG_USER("set world level %d", world_level_);
		break;
	}
	default:
		break;
	}
	return 0;
}

int Scene_Public::world_level(void) {
	return world_level_;
}

int Scene_Public::set_world_level(const int world_level) {
	world_level_ = world_level;
	return 0;
}

bool Scene_Public::had_ask_world_level(void) {
	return had_ask_world_level_;
}
bool Scene_Public::set_asked_world_level(void) {
	had_ask_world_level_ = true;
	return had_ask_world_level_;
}

void Scene_Public::hero_time_up(const Time_Value &now) {
	for (Master_ID_Hero_Map::iterator it =
			master_id_fight_hero_map_.begin(); it != master_id_fight_hero_map_.end();
			++it) {
		it->second->time_up(now);
	}
}

int Scene_Public::inner_check_info_on_center_server(Block_Buffer &buf) {
	Block_Buffer inner_buf;
	inner_buf.make_message(MONGO_SAVE_INNER_CHECK_INFO);
	inner_buf.copy(&buf);
	DB_MANAGER->push_data_block(inner_buf, 0);
	return 0;
}
