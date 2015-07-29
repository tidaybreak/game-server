/*
 * Dragon_Vale_Scene_Manager.cpp
 *
 *  Created on: 2014年7月12日
 *      Author: gaojianshan
 */

#include "Dragon_Vale_Scene_Manager.h"
#include "Dragon_Vale_Scene.h"


Dragon_Vale_Scene_Manager::Dragon_Vale_Scene_Manager() {
	scene_pool_.clear();
	scene_map_.clear();
	dragon_vale_scene_map_.clear();
}

Dragon_Vale_Scene_Manager::~Dragon_Vale_Scene_Manager() {
}

int Dragon_Vale_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Dragon_Vale_Scene_Map dragon_vale_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, dragon_vale_scene_map));

	return 0;
}

void Dragon_Vale_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "dragon scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

int Dragon_Vale_Scene_Manager::single_scene_status(void) {
	LOG_TEXT("dragon vale scene size:%d", scene_map_.size());

	return 0;
}

int Dragon_Vale_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Dragon_Vale_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Dragon_Vale_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ++scene_it) {
			scene_it->second->tick(now);
			scene_it->second->flush_tick(now);
		}
	}

	for (Muid_Dragon_Vale_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		Dragon_Vale_Scene_Map temp_dragon_vale_scene_map;
		temp_dragon_vale_scene_map.clear();
		temp_dragon_vale_scene_map.insert(muid_it->second.begin(), muid_it->second.end());
		for (Dragon_Vale_Scene_Map::iterator scene_it = temp_dragon_vale_scene_map.begin();
				scene_it != temp_dragon_vale_scene_map.end(); ++scene_it) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				scene->release_scene_normal();
				scene_pool_.push((Dragon_Vale_Scene*)scene);
				role_id_t creater = scene_it->first;
				muid_it->second.erase(creater);
				dragon_vale_scene_map_.erase(creater);
			} else {
				scene_it->second->tick(now);
				scene_it->second->flush_tick(now);
			}
		}
		temp_dragon_vale_scene_map.clear();
	}

	return 0;
}

Move_Scene *Dragon_Vale_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Dragon_Vale_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		Dragon_Vale_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(csa.id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Move_Scene *scene = scene_pool_.pop();

			scene->init_scene_normal(muid.id, muid.order);
			scene->set_create_scene_args(csa);

			muid_it_find_it->second.insert(std::make_pair(csa.id, scene));
			dragon_vale_scene_map_.insert(std::make_pair(csa.id, scene));

			Dragon_Vale_Scene *dragon_vale_scene = 0;
			dragon_vale_scene = dynamic_cast<Dragon_Vale_Scene*>(scene);
			if(dragon_vale_scene) {
				dragon_vale_scene->set_scene_creater(csa.id);
				if(1 == csa.sort) {
					dragon_vale_scene->set_creater_uuid((int64_t)csa.sub_sort);
				}
			}

			return scene;
		}
	} else {
		MSG_TRACE_ABORT("");
	}

	return 0;
}

Move_Scene *Dragon_Vale_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	return find_scene(muid, role_id, csa);
}

Move_Scene* Dragon_Vale_Scene_Manager::find_scene(role_id_t creater) {
	Dragon_Vale_Scene_Map::iterator it = dragon_vale_scene_map_.find(creater);
	if(it == dragon_vale_scene_map_.end()) {
		return 0;
	}
	return it->second;
}
