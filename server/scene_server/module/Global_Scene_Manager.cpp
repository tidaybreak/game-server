/*
 * Global_Scene_Manager.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#include "Global_Scene_Manager.h"
#include "Config_Cache.h"
#include "Config_Cache_Scene.h"
#include "Pool_Manager.h"
#include "Battle_Scene.h"
#include "Scene_Def.h"
#include "Module_Manager.h"
#include "main_scene/Main_Scene_Manager.h"
#include "single_scene/Single_Scene_Manager.h"
#include "team_scene/Team_Scene_Manager.h"
#include "gang_manor_scene/Gang_Manor_Scene_Manager.h"
#include "war_scene/War_Scene_Manager.h"
#include "dragon_vale_scene/Dragon_Vale_Scene_Manager.h"
#include "gang_war_scene/Gang_War_Scene_Manager.h"
#include "campfire_scene/Campfire_Scene_Manager.h"
#include "task_scene/Task_Scene_Manager.h"
#include "expedition_scene/Expedition_Global_Scene_Manager.h"
#include "answer/Answer_Scene_Manager.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "team/Team_Arena_Controler.h"
#include "team_defend/Team_Defend_Manager.h"
#include "honor_arena_scene/Honor_Arena_Scene_Manager.h"
#include "killing_scene/Killing_Scene_Manager.h"
#include "tbay_scene/Tbay_Scene_Manager.h"
#include "gang_boss_scene/Gang_Boss_Scene_Manager.h"
#include "world_boss_scene/World_Boss_Scene_Manager.h"
#include "arena_scene/Arena_Scene_Manager.h"

Global_Scene_Manager::Global_Scene_Manager() : battle_order_(0) {
	// TODO Auto-generated constructor stub

}

Global_Scene_Manager::~Global_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Global_Scene_Manager::tick(void) {
	Time_Value now = Time_Value::gettimeofday();
	for (Scene_Manager_Set::iterator it = scene_manager_set_.begin(); it != scene_manager_set_.end(); ++it) {
		(*it)->tick(now);
	}

	if (!destroy_scene_id_set_.empty()) {
		for (Long_Id_Set::iterator it = destroy_scene_id_set_.begin(); it != destroy_scene_id_set_.end(); ++it) {
			destroy_battle_scene(*it);
		}
		destroy_scene_id_set_.clear();
	}

	for (Battle_Scene_Map::iterator it = battle_scene_map_.begin(); it != battle_scene_map_.end(); ++it) {
		it->second->tick(now);
	}

	return 0;
}

void Global_Scene_Manager::module_tick(Time_Value &now) {
	for (Module_Manager_Set::iterator it = module_manager_tick_.begin(); it != module_manager_tick_.end(); ++it) {
		(*it)->tick(now);
	}
}

int Global_Scene_Manager::init_global_scene(int idx) {
	const Server_Monitor_Vec &monitor_list = CONFIG_CACHE->server_solution_cache().monitor_list;
	const Monitor_Acceptor_Vec &acceptor_list = monitor_list[idx].acceptor_list;
	for (Monitor_Acceptor_Vec::const_iterator scene_it = acceptor_list.begin(); scene_it != acceptor_list.end(); ++scene_it) {
		if ((*scene_it).monitor_type == Server_Enum::MONITOR_SCENE) {

			for (Monitor_Mark_Vec::const_iterator it = (*scene_it).monitor_id_order_vec_.begin(); it != (*scene_it).monitor_id_order_vec_.end(); ++it) {
				scene_set_.insert((*it));

				Scene_Manager *scene_manager = 0;
				const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache((*it).id);
				if (! scene_config) {
					LOG_ABORT("scene:%d", (*it).id);
				}
				switch (scene_config->manager) {
				case Scene_Multi: {
					scene_manager = MAIN_SCENE_MANAGER;
					break;
				}
				case Scene_Single : {
					scene_manager = SINGLE_SCENE_MANAGER;
					break;
				}
				case Scene_Team : {
					scene_manager = TEAM_SCENE_MANAGER;
					module_manager_tick_.insert(TEAM_ARENA_CONTROLER);
					break;
				}
				case Scene_Gang_Manor : {
					scene_manager = GANG_MANOR_MANAGER;
					break;
				}
				case Scene_War:{
					scene_manager = WAR_SCENE_MANAGER;
					break;
				}
				case Scene_Gang_Boss:{
					scene_manager = GANG_BOSS_SCENE_MANAGER;
					break;
				}
				case Scene_Honor_Arena:{
					scene_manager = HONOR_ARENA_SCENE_MANAGER;
					break;
				}
				case Scene_Dragon_Vale: {
					scene_manager = DRAGON_VALE_SCENE_MANAGER;
					module_manager_tick_.insert(SCENE_DRAGON_VALE_MANAGER);
					break;
				}
				case Scene_Gang_War: {
					scene_manager = GANG_WAR_SCENE_MANAGER;
					module_manager_tick_.insert(SCENE_GANG_GLOBAL_MANAGER);
					break;
				}
				case Scene_Campfire:{
					scene_manager = CAMPFIRE_SCENE_MANAGER;
					break;
				}
				case Scene_Task_Manager: {
					scene_manager = TASK_SCENE_MANAGER;
					break;
				}
				case Scene_Expedition_Manager: {
					scene_manager = EXPEDITION_GLOBAL_SCENE_MANAGER;
					break;
				}
				case Scene_Answer_Manager: {
					scene_manager = ANSWER_SCENE_MANAGER;
					break;
				}
				case Scene_Team_Defend: {
					scene_manager = TEAM_DEFEND_SCENE_MANAGER;
					break;
				}
				case Scene_World_Boss:{
					scene_manager = WORLD_BOSS_SCENE_MANAGER;
					break;
				}
				case Scene_Killing:{
					scene_manager = KILLING_SCENE_MANAGER;
					break;
				}
				case Scene_Tbay:{
					scene_manager = TBAY_SCENE_MANAGER;
					break;
				}
				case Scene_Arena:{
					scene_manager = ARENA_SCENE_MANAGER;
					break;
				}
				default: {
					LOG_ABORT("unknow scene:%d", (*it).id);
					break;
				}
				}

				scene_manager->init((*it));
				scene_manager_set_.insert(scene_manager);
				muid_manager_map_.insert(std::make_pair(*it, scene_manager));

			}

			break;
		}
	}
	return 0;
}

int Global_Scene_Manager::show_scene_status(void) {

	for (Scene_Manager_Set::iterator it = scene_manager_set_.begin(); it != scene_manager_set_.end(); ++it) {
		(*it)->show_scene_status();
	}

	return 0;
}

Scene_Manager *Global_Scene_Manager::get_scene_manager(Monitor_Unique_ID &muid) {
	Muid_Scene_Manager_Map::iterator find_it = muid_manager_map_.find(muid);
	if (find_it != muid_manager_map_.end()) {
		return find_it->second;
	}
	return 0;
}

long_id_t Global_Scene_Manager::generate_battle_order(void) {
	long_id_t order = ++battle_order_;
	if (battle_order_ >= 9999999999999999) {
		battle_order_ = 0;
		order = battle_order_;
	}

	while (1) {
		if (! battle_scene_map_.count(order)) {
			break;
		} else {
			order = ++battle_order_;
		}
	}

	return order;
}

Battle_Scene *Global_Scene_Manager::create_battle_scene(Move_Scene* move_scene) {
	Battle_Scene *scene = POOL_MANAGER->pop_battle_scene_pool();
	if (! scene) {
		MSG_TRACE_ABORT();
		return 0;
	}

	long_id_t battle_id = generate_battle_order();
	scene->init_scene_normal(battle_id, move_scene);
	battle_scene_map_.insert(std::make_pair(battle_id, scene));

	return scene;
}

Battle_Scene *Global_Scene_Manager::find_battle_scene(long_id_t battle_id) {
	Battle_Scene_Map::iterator find_it = battle_scene_map_.find(battle_id);
	if (find_it != battle_scene_map_.end())
		return find_it->second;
	return 0;
}

void Global_Scene_Manager::push_destroy_scene(Battle_Scene* battle_scene) {
	if(battle_scene && battle_scene->battle_status() != Battle_Scene::STATUS_RECOVER){
		battle_scene->release_scene_normal();
		destroy_scene_id_set_.insert(battle_scene->battle_id());
	}
}

int Global_Scene_Manager::destroy_battle_scene(long_id_t battle_id) {
	Battle_Scene_Map::iterator find_it = battle_scene_map_.find(battle_id);
	if (find_it != battle_scene_map_.end()) {
		POOL_MANAGER->push_battle_scene_pool(find_it->second);
		battle_scene_map_.erase(find_it);
	} else {
		MSG_USER_TRACE("can not find battle:%ld", battle_id);
		return 0;
	}

	return 0;
}

void Global_Scene_Manager::insert_mover(Mover *mover) {
    if (this->all_battle_mover_set_.insert(mover).second == false) {
    	MSG_TRACE_ABORT("mover id:%ld", 0);
    }
}

void Global_Scene_Manager::remove_mover(Mover *mover) {
	this->all_battle_mover_set_.erase(mover);
}
