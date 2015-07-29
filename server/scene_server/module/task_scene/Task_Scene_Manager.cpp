/*
 * Task_Scene_Manager.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: gaojianshan
 */


#include "Task_Scene_Manager.h"
#include "Task_Scene.h"


Task_Scene_Manager::Task_Scene_Manager() {
	scene_pool_.clear();
	scene_map_.clear();
}

Task_Scene_Manager::~Task_Scene_Manager() {
}

int Task_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Task_Scene_Map task_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, task_scene_map));

	return 0;
}

int Task_Scene_Manager::single_scene_status(void) {
	LOG_TEXT("task scene size:%d", scene_map_.size());

	return 0;
}

int Task_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Task_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Task_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ++scene_it) {
			scene_it->second->tick(now);
			scene_it->second->flush_tick(now);
		}
	}

	for (Muid_Task_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		Task_Scene_Map temp_task_scene_map;
		temp_task_scene_map.clear();
		temp_task_scene_map.insert(muid_it->second.begin(), muid_it->second.end());
		for (Task_Scene_Map::iterator scene_it = temp_task_scene_map.begin();
				scene_it != temp_task_scene_map.end(); ++scene_it) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				MSG_DEBUG("release task scene, role:%ld", scene_it->first);
				scene->release_scene_normal();
				scene_pool_.push((Task_Scene*)scene);
				role_id_t creater = scene_it->first;
				muid_it->second.erase(creater);
			} else {
				scene_it->second->tick(now);
				scene_it->second->flush_tick(now);
			}
		}
		temp_task_scene_map.clear();
	}

	return 0;
}

void Task_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "task scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Task_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Task_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		Task_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(csa.id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Move_Scene *scene = scene_pool_.pop();

			scene->init_scene_normal(muid.id, muid.order);
			scene->set_create_scene_args(csa);

			muid_it_find_it->second.insert(std::make_pair(csa.id, scene));

			Task_Scene *task_scene = 0;
			task_scene = dynamic_cast<Task_Scene*>(scene);
			if(task_scene) {
				task_scene->set_scene_task_id(csa.sort);
			}

			return scene;
		}
	} else {
		MSG_TRACE_ABORT("");
	}

	return 0;
}

Move_Scene *Task_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	if(!csa.id) {
		return 0;
	}
	return find_scene(muid, role_id, csa);
}
