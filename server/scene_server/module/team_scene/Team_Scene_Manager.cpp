/*
 * Team_Scene_Manager.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#include "Team_Scene_Manager.h"
#include "Team_Scene.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"

Team_Scene_Manager::Team_Scene_Manager() {
	// TODO Auto-generated constructor stub

}

Team_Scene_Manager::~Team_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Team_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Role_Scene_Map role_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, role_scene_map));

	return 0;
}

int Team_Scene_Manager::single_scene_status(void) {
	LOG_TEXT("team scene size:%d", scene_map_.size());

	return 0;
}

int Team_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Role_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Role_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ++scene_it) {
			scene_it->second->tick(now);
			scene_it->second->flush_tick(now);
		}
	}

	for (Muid_Role_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Role_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				scene->release_scene_normal();
				scene_pool_.push((Team_Scene*)scene);
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

void Team_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "team scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Team_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		// csa.id = 队伍id
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(csa.id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Team_Scene *scene = scene_pool_.pop();
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

Move_Scene *Team_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {

	return 0;
}
