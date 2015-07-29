/*
 * Wayfind.cpp
 *
 *  Created on: 2014-06-07
 *      Author: xiaoliang
 */

#include "Wayfind.h"
#include "Config_Cache.h"
#include "Global_Robot_Scene_Manager.h"

Wayfind::Wayfind() {
	reset();
}

Wayfind::~Wayfind() {
	// TODO Auto-generated destructor stub
}

void Wayfind::path_find(void) {
	Coord src = wayfind_info_.cur_location.coord.grid_coord();
	Coord det = wayfind_info_.det_coord.grid_coord();
	if(GLOBAL_ROBOT_SCENE_MANAGER->path_find_cache(wayfind_info_.cur_location.scene_id, wayfind_info_.det_scene_id,src, det, wayfind_info_.path)) {
		//MSG_USER("寻路命中√√√");
		if (! wayfind_info_.path.empty()) {
			return;
		}
	}
	wayfind_info_.scene = GLOBAL_ROBOT_SCENE_MANAGER->find_scene(wayfind_info_.cur_location.scene_id);
	if(wayfind_info_.scene) {
		//MSG_USER("寻路未命中×××");
		Int_Int moveable;
		wayfind_info_.path.clear();
		int ret = wayfind_info_.scene->path_find_sight_line(0, src, det, wayfind_info_.path, moveable, false, true);
		if (ret < 0) {
			wayfind_info_.scene->path_find_a_star(0, src, det, wayfind_info_.path, moveable, false, true, INT_MAX);
		}
		if (! wayfind_info_.path.empty()) {
			GLOBAL_ROBOT_SCENE_MANAGER->path_save_cache(wayfind_info_.cur_location.scene_id, wayfind_info_.det_scene_id,src, det, wayfind_info_.path);
		}
	}
}
void Wayfind::reset(void) {
	wayfind_info_.reset();
}

Wayfind_Info &Wayfind::wayfind_info(void) {
	return wayfind_info_;
}
