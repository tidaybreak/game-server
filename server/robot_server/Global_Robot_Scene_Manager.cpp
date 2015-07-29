/*
 * Global_Robot_Scene_Manager.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: xiaoliang
 */

#include "Global_Robot_Scene_Manager.h"
#include "Config_Cache.h"
#include "Config_Cache_Scene.h"

Global_Robot_Scene_Manager::Global_Robot_Scene_Manager() {
	reset();
}

Global_Robot_Scene_Manager::~Global_Robot_Scene_Manager() {

}
void Global_Robot_Scene_Manager::reset(void) {
	move_scene_map_.clear();
	path_map_.clear();
}

int Global_Robot_Scene_Manager::init_global_scene(int idx) {
	const Scene_Config_Map_Cache scene_map = CONFIG_CACHE_SCENE->scene_config_cache_map();
	for(Scene_Config_Map_Cache::const_iterator it = scene_map.begin(); it != scene_map.end(); ++it) {
		const Scene_Config *scene_config = (&(it->second));
		if (! scene_config) {
			LOG_ABORT("scene:%d", it->second.scene_id);
		}

		Move_Scene *scene = scene_pool_.pop();
		if (!scene) {
			LOG_ABORT("scene null");
		}

		scene->init_scene_normal(it->second.scene_id,0);

		move_scene_map_.insert(std::make_pair(it->second.scene_id, scene));
	}
	return 0;
}

Move_Scene *Global_Robot_Scene_Manager::find_scene(const int &muid) {
	Move_Scene_Map::iterator find_it = move_scene_map_.find(muid);
	if (find_it != move_scene_map_.end()) {
		return find_it->second;
	} else {
		LOG_ABORT("scene id:%d", muid);
	}
	return 0;
}

int Global_Robot_Scene_Manager::path_save_cache(int src_scene_id, int det_scene_id, const Coord &src, const Coord &det, const Coord_Vec &path) {
	Path_ID path_id;
	path_id.reset();
	path_id.src_scene_id = src_scene_id;
	path_id.det_scene_id = det_scene_id;
	path_id.src = src;
	path_id.det = det;
	Path_Map::iterator find_it = path_map_.find(path_id);
	if (find_it == path_map_.end()) {
		path_map_.insert(std::make_pair(path_id, path));
	}
	return 0;
}
int Global_Robot_Scene_Manager::path_find_cache(int src_scene_id, int det_scene_id, const Coord &src, const Coord &det, Coord_Vec &path) {
	Path_ID path_id;
	path_id.reset();
	path_id.src_scene_id = src_scene_id;
	path_id.det_scene_id = det_scene_id;
	path_id.src = src;
	path_id.det = det;
	Path_Map::iterator find_it = path_map_.find(path_id);
	if (find_it != path_map_.end()) {
		path.clear();
		path = find_it->second;
		return 1;
	}
	return 0;
}
