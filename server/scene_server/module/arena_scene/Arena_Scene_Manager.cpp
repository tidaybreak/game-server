/*
 * Arena_Scene_Manager.cpp
 *
 *  Created on: 15 2 2015
 *      Author: zhoujin
 */

#include "Arena_Scene_Manager.h"
#include "Arena_Scene.h"

Arena_Scene_Manager::Arena_Scene_Manager() {
	// TODO Auto-generated constructor stub

}

Arena_Scene_Manager::~Arena_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Arena_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Arena_Scene *scene = scene_pool_.pop();
	if (!scene) {
		MSG_TRACE_ABORT("scene null");
		return 0;
	}

	scene->init_scene_normal(scene_uid.id, scene_uid.order);

	scene_map_.insert(std::make_pair(scene_uid, scene));

	return 0;
}

int Arena_Scene_Manager::tick(Time_Value &now) {
	for (Scene_Map::iterator it = scene_map_.begin(); it != scene_map_.end(); ++it) {
		it->second->tick(now);
		it->second->flush_tick(now);
	}

	return 0;
}

void Arena_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "main scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Arena_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		return find_it->second;
	} else {
		MSG_TRACE_ABORT("");
	}
	return 0;
}

Move_Scene *Arena_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		return find_it->second;
	} else {
		return 0;
	}
}

int Arena_Scene_Manager::single_scene_status(void) {
	LOG_TEXT("arena scene size:%d", scene_map_.size());

	return 0;
}
