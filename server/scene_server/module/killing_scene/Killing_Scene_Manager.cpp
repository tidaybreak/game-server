/*
 * Killing_Scene_Manager.cpp
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */


#include "Killing_Scene_Manager.h"
#include "Killing_Scene.h"
#include "Config_Cache_Scene.h"


Killing_Scene_Manager::Killing_Scene_Manager() {

}

Killing_Scene_Manager::~Killing_Scene_Manager() {
}

int Killing_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Role_Scene_Map role_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, role_scene_map));
	return 0;
}

int Killing_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Role_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Role_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				scene->release_scene_normal();
				scene_pool_.push((Killing_Scene*)scene);
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

void Killing_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "killing scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Killing_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(csa.id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Killing_Scene *scene = scene_pool_.pop();
			scene->set_create_scene_args(csa);
			scene->init_scene_normal(muid.id, muid.order);
			muid_it_find_it->second.insert(std::make_pair(csa.id, scene));
			return scene;
		}
	} else {
		MSG_TRACE_ABORT("");
	}

	return 0;
}

Move_Scene *Killing_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		role_id_t id = csa.id;
		const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(muid.id);
		if(cfg){
			if(cfg->dungeon.can_team == 0){
				id = role_id;
			}
		}
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}


