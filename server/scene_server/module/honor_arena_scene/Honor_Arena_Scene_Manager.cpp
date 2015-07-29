/*
 * Honor_arena_Scene_Manager.cpp
 *
 *  Created on: 2015年3月16日
 *      Author: root
 */


#include "Honor_Arena_Scene_Manager.h"
#include "Honor_Arena_Scene.h"
#include "honor_arena/Honor_Arena_Manager.h"


Honor_Arena_Scene_Manager::Honor_Arena_Scene_Manager() {
	// TODO Auto-generated constructor stub

}

Honor_Arena_Scene_Manager::~Honor_Arena_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Honor_Arena_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
//	Campfire_Scene *scene = scene_pool_.pop();
//	if (!scene) {
//		MSG_TRACE_ABORT("scene null");
//	}
//
//	scene->init_scene_normal(scene_uid.id, scene_uid.order);
//
//	scene_map_.insert(std::make_pair(scene_uid, scene));

	return 0;
}

int Honor_Arena_Scene_Manager::tick(Time_Value &now) {
	for (Scene_Map::iterator it = scene_map_.begin(); it != scene_map_.end();  ) {
		if(it->second->is_recover_status() && it->second->is_recover_time(now)){
			Move_Scene* scene = it->second;
			scene->release_scene_normal();
			scene_pool_.push((Honor_Arena_Scene*)scene);
			it = scene_map_.erase(it);
		}else{
			it->second->tick(now);
			it->second->flush_tick(now);
			++it;
		}
	}
	return 0;
}

void Honor_Arena_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "honor arena scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Honor_Arena_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		return find_it->second;
	} else {
		Honor_Arena_Scene *scene = scene_pool_.pop();
		scene->set_create_scene_args(csa);
		scene->init_scene_normal(muid.id, muid.order);
		scene->set_logic_uuid(muid.uuid);
		scene_map_.insert(std::make_pair(muid, scene));
		return scene;
	}
	return 0;
}

Move_Scene *Honor_Arena_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		bool in_act = HONOR_ARENA_MANAGER->find_player(role_id);
		if(in_act){
			return find_it->second;
		}
		return 0;
	} else {
		return 0;
	}
}


