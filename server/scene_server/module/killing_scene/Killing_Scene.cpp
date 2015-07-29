/*
 * Killing_Scene.cpp
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */

#include "Killing_Scene.h"
#include "Scene_Player.h"
#include "Battle_Scene.h"

Killing_Scene::Killing_Scene() {
	reset();
}

Killing_Scene::~Killing_Scene() {
}

void Killing_Scene::reset(void) {
	Move_Scene::reset();

	player_id_set.clear();
	end_time_ = Time_Value::zero;
}


int Killing_Scene::tick(const Time_Value &now){
	Move_Scene::tick(now);

	if (end_time_ != Time_Value::zero && end_time_ <= now) {
		end_time_ = Time_Value::zero;
		kick_all_player();
		recover_time_ = Time_Value::gettimeofday() + Time_Value(4);
		this->set_scene_status(SCENE_RECOVER_STATUS);
	}
	return 0;
}

int Killing_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	if(mover->mover_type() == MOVER_T_PLAYER){
		player_id_set.insert(mover->role_id());
		set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);
	}

	return 0;
}

int Killing_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);

	if(mover->mover_type() == MOVER_T_PLAYER){
		player_id_set.erase(mover->role_id());
		if (player_id_set.empty()) {
			this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
			if (offline) {
				MSG_DEBUG("killing scene recover offline, role:%ld scene id:%d", mover->role_id(), this->scene_id());
				recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
			}else{
				MSG_DEBUG("killing scene recover normal, role:%ld scene id:%d", mover->role_id(), this->scene_id());
				recover_time_ = Time_Value::zero;
			}
		}
	}
	return 0;
}

int Killing_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);
	return 0;
}

int Killing_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene::client_scene_complete(fighter);

	Scene_Player *player = fighter.player_self();
	if(!player) {
		return 0;
	}
	if(this->get_scene_status() == Move_Scene::SCENE_RECOVER_STATUS) {
		int dest = 0;
		player->inner_transmit(dest,0,0);
		return 0;
	}
	if(scene_config() && scene_config()->dungeon.can_team != 0){
		if (0 == player->get_team_id()) {
			int dest = 0;
			player->inner_transmit(dest,0,0);
			return 0;
		}
	}
	return 0;
}

void Killing_Scene::kick_all_player(void) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				// battle
				if(player->battle_scene()){
					player->force_finish_battle_scene();
				}
				int dest = 0;
				player->inner_transmit(dest,0,0);
			}
		}
	}
}


