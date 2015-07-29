/*
 * Single_Scene_Manager.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#include "Single_Scene_Manager.h"
#include "Single_Scene.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"
#include "Config_Cache_Scene.h"

Single_Scene_Manager::Single_Scene_Manager() {
	// TODO Auto-generated constructor stub

}

Single_Scene_Manager::~Single_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Single_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Role_Scene_Map role_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, role_scene_map));

	return 0;
}

int Single_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Role_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Role_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				scene->release_scene_normal();
				scene_pool_.push((Single_Scene*)scene);
				scene_it = muid_it->second.erase(scene_it);
			} else {
				scene_it->second->tick(now);
				scene_it->second->flush_tick(now);
				++scene_it;
			}
		}
	}

	return 0;
}

Move_Scene *Single_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(role_id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Single_Scene *scene = scene_pool_.pop();
			scene->set_create_scene_args(csa);
			scene->set_role_id(role_id);
			// 双倍收益
			const Scene_Config * scene_config =
					CONFIG_CACHE_SCENE->scene_config_cache(muid.id);
			if(scene_config && scene_config->type == Elite_Scene_Type){
				if(csa.type == 1){
					scene->set_double_income(true);
				}
			}
			scene->init_scene_normal(muid.id, muid.order);
			muid_it_find_it->second.insert(std::make_pair(role_id, scene));
			return scene;
		}
	} else {
		MSG_TRACE_ABORT("");
	}

	return 0;
}

Move_Scene *Single_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(role_id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

void Single_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "single scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}
