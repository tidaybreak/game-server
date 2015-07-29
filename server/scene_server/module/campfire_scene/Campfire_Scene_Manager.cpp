/*
 * Campfire_Scene_Manager.cpp
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */


#include "Campfire_Scene_Manager.h"
#include "Campfire_Scene.h"
#include "campfire/Campfire_Data_Manager.h"
#include "Scene_Player.h"

Campfire_Scene_Manager::Campfire_Scene_Manager() {
	// TODO Auto-generated constructor stub

}

Campfire_Scene_Manager::~Campfire_Scene_Manager() {
	// TODO Auto-generated destructor stub
}

int Campfire_Scene_Manager::init(const Monitor_Unique_ID &scene_uid) {
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

int Campfire_Scene_Manager::tick(Time_Value &now) {
	for (Scene_Map::iterator it = scene_map_.begin(); it != scene_map_.end();  ) {
		if(it->second->is_recover_status() && it->second->is_recover_time(now)){
			Move_Scene* scene = it->second;
			scene->release_scene_normal();
			scene_pool_.push((Campfire_Scene*)scene);
			it = scene_map_.erase(it);
		}else{
			it->second->tick(now);
			it->second->flush_tick(now);
			++it;
		}
	}
	return 0;
}

void Campfire_Scene_Manager::show_scene_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d, map:%d", "compfire scene", scene_pool_.free_obj_list_size(), scene_pool_.used_obj_list_size(), scene_map_.size());
}

Move_Scene *Campfire_Scene_Manager::find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		return find_it->second;
	} else {
		Campfire_Scene *scene = scene_pool_.pop();
		scene->set_create_scene_args(csa);
		scene->init_scene_normal(muid.id, muid.order);
		scene_map_.insert(std::make_pair(muid, scene));
		return scene;
	}
	return 0;
}

Move_Scene *Campfire_Scene_Manager::find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) {
	Scene_Map::iterator find_it = scene_map_.find(muid);
	if (find_it != scene_map_.end()) {
		Scene_Campfire_Info* cmf_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(role_id);
		if(cmf_info){
			return find_it->second;
		}
		return 0;
	} else {
		return 0;
	}
}

int Campfire_Scene_Manager::send_campfire_personal_info(Scene_Player* player) {
	Campfire_Scene *scene = dynamic_cast<Campfire_Scene *>(find_scene(player->monitor_link().scene_muid, player->role_id(), player->base_detail().create_scene_args_));
	if (!scene)
		return 0;

	scene->send_campfire_personal_info(player);
	return 0;
}

int Campfire_Scene_Manager::player_req_battle(Scene_Player* player,
		role_id_t monster_id) {
	Campfire_Scene *scene = dynamic_cast<Campfire_Scene *>(find_scene(player->monitor_link().scene_muid, player->role_id(), player->base_detail().create_scene_args_));
	if (!scene)
		return 0;

	return scene->player_req_battle(player, monster_id);
}
