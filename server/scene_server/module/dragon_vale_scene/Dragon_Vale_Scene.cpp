/*
 * Dragon_Vale_Scene.cpp
 *
 *  Created on: 2014年7月12日
 *      Author: gaojianshan
 */

#include "Dragon_Vale_Scene.h"
#include "Msg_Inner_Struct.h"
#include "mover/Mover.h"
#include "Scene_Player.h"
#include "Msg_Active_Struct.h"
#include "Battle_Scene.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "Msg_Inner_Enum.h"
#include "Scene_Monitor.h"
#include "Config_Cache_Scene.h"

Dragon_Vale_Scene::Dragon_Vale_Scene() {
	reset();
}

Dragon_Vale_Scene::~Dragon_Vale_Scene() {
}

void Dragon_Vale_Scene::reset(void) {
	Move_Scene::reset();

	player_id_set.clear();
	creater = 0;
	creater_uuid = 0;
}

int Dragon_Vale_Scene::enter_scene(Mover *mover) {
	if(!mover) {
		return 0;
	}

	Move_Scene::enter_scene(mover);

	if(mover->mover_type() == MOVER_T_PLAYER){
		player_id_set.insert(mover->role_id());
		set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);
	}

	return 0;
}

int Dragon_Vale_Scene::exit_scene(Mover *mover, bool offline) {
	if(!mover) {
		return 0;
	}

	Move_Scene::exit_scene(mover, offline);

	if(mover->mover_type() == MOVER_T_PLAYER){
		SCENE_DRAGON_VALE_MANAGER->notify_total_rob_production(*mover->player_self(), this);

		MSG_20600013 msg;
		msg.creater = this->creater;
		msg.role_id = mover->role_id();
		Block_Buffer buf;
		buf.make_message(INNER_PLAYER_EXIT_DRAGON_VALE);
		msg.serialize(buf);
		buf.finish_message();

		if(this->creater_uuid) {
			SCENE_MONITOR->send_to_logic(this->creater_uuid, buf);
		}
		else {
			mover->player_self()->send_to_logic(buf);
		}

		player_id_set.erase(mover->role_id());
		if (player_id_set.empty()) {
			this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
			if (offline) {
				MSG_DEBUG("dragon vale scene recover offline, scene id:%ld", this->scene_id());
				recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
			}else{
				MSG_DEBUG("dragon vale scene recover normal, scene id:%ld", this->scene_id());
				recover_time_ = Time_Value::zero;
			}
		}

		SCENE_DRAGON_VALE_MANAGER->notify_online_player_battle_result(*mover->player_self(), this);
		SCENE_DRAGON_VALE_MANAGER->porcess_dragon_vale_rob_offline(*mover->player_self());
	}

	return 0;
}

int Dragon_Vale_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);

	return 0;
}

int Dragon_Vale_Scene::release_scene_normal(void){
	Move_Scene::release_scene_normal();

	return 0;
}

int Dragon_Vale_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene_Base::client_scene_complete(fighter);
	Scene_Player *player = fighter.player_self();
	if (!player) {
		return -1;
	}

	if(this->get_scene_status() == Move_Scene::SCENE_RECOVER_STATUS) {
		int dest = 0;
		player->inner_transmit(dest,0,0);
		return 0;
	}


	//龙谷特殊处理
	bool is_hide_tran_point = false;
	role_id_t creater = this->get_scene_creater();
	if(creater != 0 && player->role_id() != creater) {
		is_hide_tran_point = true;
	}

	//龙谷特殊处理
	if(!is_hide_tran_point)
	{
		// 更新地图传送点
		MSG_81000040 tran_point;
		int count = transmit_point.size();
		for (int i = 0; i < count; ++i) {
			if (transmit_point[i].visible == 0) {
				tran_point.transmit_point.push_back(transmit_point[i]);
			} else if (transmit_point[i].visible == 1 || transmit_point[i].visible == 2) {
				if (is_scene_complete_ && is_show_ts_point_) {
					// 精英副本最后一关，不显示继续挑战
					if(this->scene_type() == Elite_Scene_Type){
						const Scene_Config *des_scene_config =
								CONFIG_CACHE_SCENE->scene_config_cache(transmit_point[i].des_scene_id);
						if(des_scene_config){
							tran_point.transmit_point.push_back(transmit_point[i]);
						}
					}else{
						tran_point.transmit_point.push_back(transmit_point[i]);
					}
				}
			}
		}
		OBJ_SEND_TO_CLIENT(tran_point, fighter);
	}
	//发送指引路径点
	if (this->npc_section_.path_guide_vec.empty() == false) {
		int point = this->npc_section_.get_next_path_guide();
		if (point != -1) {
			send_path_guide(point, player);
		}
	}
	//
	if (enter_players_set_.count(fighter.role_id()) == 0) {
		enter_players_set_.insert(fighter.role_id());
		init_player_own_point(fighter.role_id());
	}
	// 副本状态更改通知
	{
		MSG_81000019 msg;
		msg.status = get_pass_status();
		msg.value1 = scene_param_1_;
		msg.value2 = scene_param_2_;
		msg.value3 = scene_param_3_;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}

	SCENE_DRAGON_VALE_MANAGER->notify_online_player_battle_result(*player, this);

	return 0;
}

void Dragon_Vale_Scene::hook_battle_finish(int battle_type, int win_pos,
								std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec,
								std::vector<int> kill_monters){

	if(Battle_Scene::BATTLE_TYPE_DRAGON_VALE != battle_type){
		return;
	}

	Scene_Player* win_player = NULL;
	Scene_Player* lose_player = NULL;

	int win_count = win_mover_vec.size();
	for(int i= 0; i< win_count; ++i) {
		Scene_Player* player  = win_mover_vec[i]->player_self();
		if(player){
			win_player = player;
			break;
		}
	}

	int lose_count = lose_mover_vec.size();
	for(int i=0; i<lose_count; ++i){
		Scene_Player* player  = lose_mover_vec[i]->player_self();
		if(player){
			lose_player = player;
			break;
		}
	}
	if(!win_player && !lose_player){
		MSG_USER("win_player and lose_player is offline");
		return;
	}

	if(win_player && lose_player) {
		SCENE_DRAGON_VALE_MANAGER->process_win_and_lose(*win_player, *lose_player);
		return;
	}

	if(win_player && !lose_player) {
		SCENE_DRAGON_VALE_MANAGER->process_win(*win_player);
		return;
	}

	if(!win_player && lose_player) {
		SCENE_DRAGON_VALE_MANAGER->process_lose(*lose_player);
		return;
	}
}
