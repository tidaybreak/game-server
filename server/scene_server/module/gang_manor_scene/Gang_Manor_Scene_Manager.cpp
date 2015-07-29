/*
 * Gang_Manor_Scene_Manager.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: chenyulin
 */

#include "Gang_Manor_Scene_Manager.h"
#include "Gang_Manor_Scene.h"
#include "Base_Detail.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"

Gang_Manor_Scene_Manager::Gang_Manor_Scene_Manager() {

}

Gang_Manor_Scene_Manager::~Gang_Manor_Scene_Manager() {

}

int Gang_Manor_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Gang_Manor_Scene_Map gang_manor_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, gang_manor_scene_map));

	return 0;
}

int Gang_Manor_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Gang_Manor_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Gang_Manor_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ++scene_it) {
			scene_it->second->tick(now);
			scene_it->second->flush_tick(now);
		}
	}

	for (Muid_Gang_Manor_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Gang_Manor_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				scene->release_scene_normal();
				scene_pool_.push((Gang_Manor_Scene*)scene);
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

void Gang_Manor_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "gang manor scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Gang_Manor_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Gang_Manor_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		if (! csa.id) {
			MSG_USER("find gang_manor role_id:%ld", role_id);
		}
		Gang_Manor_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(csa.id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Gang_Manor_Scene *scene = scene_pool_.pop();

			scene->init_scene_normal(muid.id, muid.order);
			scene->set_create_scene_args(csa);

			muid_it_find_it->second.insert(std::make_pair(csa.id, scene));

			return scene;
		}
	} else {
		MSG_TRACE_ABORT("");
	}

	return 0;
}

Move_Scene *Gang_Manor_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {

	return 0;
}


