/*
 * War_Scene_Manager.cpp
 *
 *  Created on: 2014年7月1日
 *      Author: root
 */


#include "War_Scene_Manager.h"
#include "War_Scene.h"
#include "war/Scene_War_Manager.h"

War_Scene_Manager::War_Scene_Manager():war_scene_order_(0) {
	scene_pool_.clear();
	scene_map_.clear();
	order_set_.clear();

}

War_Scene_Manager::~War_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int War_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
	Role_Scene_Map role_scene_map;
	scene_map_.insert(std::make_pair(scene_uid, role_scene_map));

	return 0;
}

int War_Scene_Manager::tick(Time_Value &now) {
	for (Muid_Role_Scene_Map::iterator muid_it = scene_map_.begin(); muid_it != scene_map_.end(); ++muid_it) {
		for (Role_Scene_Map::iterator scene_it = muid_it->second.begin(); scene_it != muid_it->second.end(); ) {
			if(scene_it->second->is_recover_status() && scene_it->second->is_recover_time(now)){
				Move_Scene* scene = scene_it->second;
				scene->release_scene_normal();
				scene_pool_.push((War_Scene*)scene);
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

void War_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "war scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *War_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		// csa.id = 战场id
		role_id_t war_id = csa.id;
		if(war_id == 0){
			Honour_Player* h_player = SCENE_WAR_MANAGER->get_honour_player(role_id);
			if(h_player){
				war_id = h_player->honor_field_id;
			}
		}
		Role_Scene_Map::iterator scene_it_find_it = muid_it_find_it->second.find(war_id);
		if (scene_it_find_it != muid_it_find_it->second.end()) {
			return scene_it_find_it->second;
		} else {
			War_Scene *scene = scene_pool_.pop();
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

Move_Scene *War_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Muid_Role_Scene_Map::iterator muid_it_find_it = scene_map_.find(muid);
	if (muid_it_find_it != scene_map_.end()) {
		// csa.id = 战场id
		role_id_t war_id = 0;
		Honour_Player* h_player = SCENE_WAR_MANAGER->get_honour_player(role_id);
		if(h_player){
			war_id = h_player->honor_field_id;
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

role_id_t War_Scene_Manager::genarate_war_scene_order(void){
	role_id_t order = ++war_scene_order_;
	if (war_scene_order_ >= 9999999999999999) {
		war_scene_order_ = 0;
		order = war_scene_order_;
	}

	while (1) {
		if (! order_set_.count(order)) {
			break;
		} else {
			order = ++war_scene_order_;
		}
	}

	return order;
}


