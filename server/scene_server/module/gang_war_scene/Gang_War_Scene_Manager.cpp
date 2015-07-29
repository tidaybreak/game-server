/*
 * Gang_War_Scene_Manager.cpp
 *
 *  Created on: 2014年8月1日
 *      Author: Linqiyou
 */

#include "Gang_War_Scene_Manager.h"
#include "Gang_War_Scene.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"

Gang_War_Scene_Manager::Gang_War_Scene_Manager() {
	scene_pool_.clear();
	scene_map_.clear();
	order_set_.clear();
//	gang_war_scene_info_map_.clear();

}

Gang_War_Scene_Manager::~Gang_War_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Gang_War_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Role_Scene_Map role_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, role_scene_map));

	return 0;
}

int Gang_War_Scene_Manager::tick(Time_Value &now) {
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
				((Gang_War_Scene*)scene)->kick_all_player();
				scene_pool_.push((Gang_War_Scene*)scene);
				order_set_.erase(scene_it->first);
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

void Gang_War_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "gang war scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Gang_War_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		// csa.id = 战场id
		role_id_t war_id = csa.id;
		if(war_id == 0){
			Scene_Player* s_player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
			if (s_player) {
				const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(s_player->gang_id());
				if (info) {
					war_id = info->scene_index;
				}
			}
		}
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(war_id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			Gang_War_Scene *scene = scene_pool_.pop();
			scene->set_create_scene_args(csa);
			scene->init_scene_normal(muid.id, muid.order);
			muid_it_find_it->second.insert(std::make_pair(csa.id, scene));
			order_set_.insert(csa.id);
			return scene;
		}
	} else {
		MSG_TRACE_ABORT("");
	}

	return 0;
}

Move_Scene *Gang_War_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		// csa.id = 战场id
		role_id_t war_id = csa.id;
		if(war_id == 0){
			Scene_Player* s_player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
			if (s_player) {
				const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(s_player->gang_id());
				if (info) {
					war_id = info->scene_index;
				}
			}
		}
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(war_id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			if(scene_it_find_it->second->is_recover_status() == false){
				return scene_it_find_it->second;
			}
		}
	}
	return 0;
}

void Gang_War_Scene_Manager::close_all_gang_war_scene(const Role_Hash_Set &scene_ids) {
	for (Muid_Role_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Role_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ++scene_it) {
			if (scene_ids.count(scene_it->first) && !scene_it->second->is_recover_status()) {
				Move_Scene* scene = scene_it->second;
				((Gang_War_Scene*)scene)->set_war_over_force();
			}
		}
	}
}
//role_id_t Gang_War_Scene_Manager::genarate_scene_order(void){
//	role_id_t order = ++gang_war_scene_order_;
//	if (gang_war_scene_order_ >= 9999999999999999) {
//		gang_war_scene_order_ = 0;
//		order = gang_war_scene_order_;
//	}
//
//	while (1) {
//		if (! order_set_.count(order)) {
//			break;
//		} else {
//			order = ++gang_war_scene_order_;
//		}
//	}
//
//	return order;
//}


