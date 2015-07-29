/*
 * Task_Scene.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: gaojianshan
 */


#include "Task_Scene.h"
#include "mover/Mover.h"
#include "Scene_Player.h"
#include "Battle_Scene.h"
#include "Scene_Monitor.h"
#include "Config_Cache_Task.h"
#include "NPC/NPC_Manager.h"
#include "task/Task_Def.h"
#include "Err_Code.h"

Task_Scene::Task_Scene() {
	reset();
}

Task_Scene::~Task_Scene() {
}

void Task_Scene::reset(void) {
	Move_Scene::reset();

	player_id_set.clear();
	task_id = 0;
	is_win = false;
	end_time_ = Time_Value::zero;
}

int Task_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	if(mover->mover_type() == MOVER_T_PLAYER){
		player_id_set.insert(mover->role_id());
//		set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);
	}

	return 0;
}

int Task_Scene::tick(const Time_Value &now){
	Move_Scene::tick(now);

	if (end_time_ != Time_Value::zero && end_time_ <= now) {
		end_time_ = Time_Value::zero;
		kick_all_player();
		recover_time_ = Time_Value::gettimeofday() + Time_Value(4);
		this->set_scene_status(SCENE_RECOVER_STATUS);
	}
	return 0;
}


int Task_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);

	if(mover->mover_type() == MOVER_T_PLAYER){
		player_id_set.erase(mover->role_id());
		if (player_id_set.empty()) {
			this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
			if (offline) {
				MSG_DEBUG("task scene recover offline, role:%ld scene id:%d", mover->role_id(), this->scene_id());
				recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
			}else{
				MSG_DEBUG("task scene recover normal, role:%ld scene id:%d", mover->role_id(), this->scene_id());
				recover_time_ = Time_Value::zero;
			}
		}
	}

	return 0;
}

int Task_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);

	return 0;
}


int Task_Scene::client_scene_complete(Fighter &fighter) {
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

	if (0 == player->get_team_id()) {
		int dest = 0;
		player->inner_transmit(dest,0,0);
		return 0;
	}

	if(is_win) {
		return 0;
	}

	if(get_task_scene_monster_num() >= 1) {
		return 0;
	}

	refresh_task_limit_time_kill_boss();
	return 0;
}

void Task_Scene::hook_battle_finish(int battle_type, int win_pos,
								std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec,
								std::vector<int> kill_monters) {
	int win_count = win_mover_vec.size();
	for(int i=0; i<win_count; ++i){
		Scene_Player* player  = win_mover_vec[i]->player_self();
		if(player){
			is_win = true;
			end_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
		}
	}
	if (is_win) {
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self()) {
					it->second->player_self()->send_err_to_client(ERROR_BOSS_KILLED_ROOM_DESTORY, 0);
				}
			}
		}
	}
}

void Task_Scene::refresh_task_limit_time_kill_boss() {
    const Task_Mission_Detail_Config *task_json = CONFIG_CACHE_TASK->find_task(task_id);
    if (0 == task_json) {
    	return;
    }

    for (size_t i = 0; i < task_json->execute.size(); ++i) {
    	if(task_json->execute[i].type == TaskExecType::TASK_EXEC_LIMIT_TIME_KILL_BOSS) {
    		NPC_Record *npc_record = this->get_npc_record(task_json->execute[i].param1);
    		if(!npc_record) {
    			return;
    		}

    		NPC_MANAGER->record_refresh(npc_record);
    		break;
    	}
    }
}

void Task_Scene::kick_all_player(void) {
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
