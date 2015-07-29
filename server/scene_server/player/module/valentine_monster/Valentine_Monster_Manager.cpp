/*
 * Valentine_Monster_Manager.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#include "Valentine_Monster_Manager.h"
#include "main_scene/Main_Scene_Manager.h"
#include "Config_Cache_Valentine_Monster.h"
#include "Move_Scene.h"
#include "NPC/NPC_Struct.h"
#include "Config_Cache_NPC.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Active_Struct.h"
#include "Battle_Scene.h"
#include "Scene_Player.h"
#include "NPC/monster/Monster.h"
#include "NPC/hero/Hero.h"


const int valentine_monster_icon_id = 20;

Valentine_Monster_Manager::Valentine_Monster_Manager(void){
	reset();
}

Valentine_Monster_Manager::~Valentine_Monster_Manager(void){

}

void Valentine_Monster_Manager::reset(void){
	act_open_ = false;
	act_end_time_ = Time_Value::zero;
	main_scene_map_.clear();
}

void Valentine_Monster_Manager::tick(const Time_Value& now){
	if(!act_open_){
		return;
	}
	Main_Scene_Map::iterator it;
	for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
		if(it->second.boss_kill_result == 0){
			if(now > it->second.boss_end_time){// lose 时间到，失败重新刷boss
				Create_Scene_Args cs_args;
				Move_Scene* scene = MAIN_SCENE_MANAGER->find_scene_simple(it->second.muid, 0, cs_args);
				if(!scene){
					continue;
				}
				NPC_Record* boss_record = scene->get_npc_record(MAIN_SCENE_M_P_ID_VT_1);
				if(!boss_record){
					continue;
				}
				boss_record->remove_point();
				boss_record->cur_ref_times = 0;
				boss_record->config.refresh_times = 1;
				boss_record->set_refresh();
				it->second.boss_hp_cur = it->second.boss_hp_max;
				it->second.boss_end_time = Time_Value::gettimeofday() +
						Time_Value(boss_record->config.refresh_time+it->second.boss_fight_time);

				//to client
				MSG_81000610 msg;
				msg.result = 0;
				Block_Buffer buf;
				MAKE_MSG_BUF(msg, buf);
				Role_Id_Set::iterator r_it;
				for(r_it = it->second.fighter_set_.begin(); r_it != it->second.fighter_set_.end();
						++r_it){
					role_id_t fight_role_id = (*r_it);
					Scene_Player* player = NULL;
					Mover* mover = scene->find_mover_with_layer(0, fight_role_id);
					if(mover){
						player = mover->player_self();
					}
					if(player){
						player->send_to_client(buf);
					}
				}


			}
		}else if(it->second.boss_kill_result == 1){// win
			if(now > it->second.boss_reward_time){
				Create_Scene_Args cs_args;
				Move_Scene* scene = MAIN_SCENE_MANAGER->find_scene_simple(it->second.muid, 0, cs_args);
				if(!scene){
					continue;
				}
				NPC_Record* boss_record = scene->get_npc_record(MAIN_SCENE_M_P_ID_VT_1);
				if(!boss_record){
					continue;
				}
				boss_record->remove_point();
				boss_record->cur_ref_times = 0;
				boss_record->config.refresh_times = 1;
				boss_record->set_refresh();
				it->second.boss_kill_result = 0;
				it->second.boss_hp_cur = it->second.boss_hp_max;
				it->second.boss_end_time = Time_Value::gettimeofday() +
						Time_Value(boss_record->config.refresh_time+it->second.boss_fight_time);
				//reward
				{
					//result
					MSG_81000610 msg;
					msg.result = 1;
					Block_Buffer buf;
					MAKE_MSG_BUF(msg, buf);

					int8_t send_mail = 0;
					Block_Buffer reward_buf;
					reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
					reward_buf.write_int32(valentine_monster_icon_id);
					reward_buf.write_int8(1);
					int act_player_num = it->second.fighter_set_.size();
					reward_buf.write_int32(act_player_num);
					Role_Id_Set::iterator r_it;
					for(r_it = it->second.fighter_set_.begin(); r_it != it->second.fighter_set_.end();
							++r_it){
						role_id_t fight_role_id = (*r_it);
						Scene_Player* player = NULL;
						Mover* mover = scene->find_mover_with_layer(0, fight_role_id);
						if(mover){
							player = mover->player_self();
						}
						if(player){
							send_mail = 0;
						}else{
							send_mail = 1;
						}
						reward_buf.write_int64(fight_role_id);
						reward_buf.write_int8(send_mail);
						int reward_amount = it->second.boss_reward.size();
						reward_buf.write_int32(reward_amount);
						for(int i = 0; i < reward_amount; ++i){
							reward_buf.write_int32(it->second.boss_reward[i].val_1);
							reward_buf.write_int32(it->second.boss_reward[i].val_2);
						}//item end
						if(player){// to client result
							player->send_to_client(buf);
						}
					}//player end
					//to logic
					reward_buf.finish_message();
					SCENE_MONITOR->send_to_logic(it->second.muid.uuid, reward_buf);
				}//reward end
			}
		}
	}
}

void Valentine_Monster_Manager::be_hurt_and_sub_blood(const int scene_id, Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id){
	if(!act_open_ || !battle_scene){
		return;
	}
	role_id_t attacker_master_id = attacker_id;
	Fighter* fighter = battle_scene->find_mover_with_role_id(attacker_id);
	if(fighter){
		if(fighter->monster_self()){
			attacker_master_id = fighter->monster_self()->master_role_id();
		}else if(fighter->hero_self()){
			attacker_master_id = fighter->hero_self()->master_role_id();
		}
	}
	Main_Scene_Map::iterator it = main_scene_map_.find(scene_id);
	if(it != main_scene_map_.end()){
		if(point_id == it->second.boss_point_id){
			if(it->second.boss_kill_result == 0){
				it->second.fighter_set_.insert(attacker_master_id);
				it->second.boss_hp_cur -= sub_blood;
				if(it->second.boss_hp_cur <= 0){
					it->second.boss_hp_cur = 0;
					it->second.boss_kill_result = 1;
					it->second.boss_reward_time = Time_Value::gettimeofday() + Time_Value(5);
				}
			}
		}
	}
}

int Valentine_Monster_Manager::inner_sync_info(Scene_Player* player, Block_Buffer& buf){
	return 0;
}

int Valentine_Monster_Manager::inner_act_start(const role_id_t gang_id, Block_Buffer& buf){
	if(act_open_){
		return 0;
	}
	int remain_time = 0;
	int boss_type_id = 0;
	int scene_amount = 0;
	buf.read_int32(remain_time);
	buf.read_int32(boss_type_id);
	buf.read_int32(scene_amount);
	for(int i = 0; i < scene_amount; ++i){
		Monitor_Unique_ID muid;
		muid.deserialize(buf);
		Create_Scene_Args cs_args;
		Move_Scene* scene = MAIN_SCENE_MANAGER->find_scene_simple(muid, 0, cs_args);
		if(scene){
			Vlt_Monster_Point_Map* point_map = CONFIG_CACHE_VALENTINE_MONSTER->get_monster_point_map(scene->scene_id());
			if(point_map){
				Valentine_Monster_Info info;
				info.muid = muid;
				main_scene_map_[muid.id] = info;
			}
		}
	}
	act_open_ = true;
	act_end_time_ = Time_Value::gettimeofday() + Time_Value(remain_time);
	act_start_add_monster(boss_type_id);
	return 0;
}

int Valentine_Monster_Manager::inner_act_end(const role_id_t gang_id, Block_Buffer& buf){
	if(!act_open_){
		return 0;
	}
	act_end_del_monster();
	act_open_ = false;
	main_scene_map_.clear();
	return 0;
}

void Valentine_Monster_Manager::act_start_add_monster(const int boss_tid){
	Main_Scene_Map::iterator it;
	for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
		Create_Scene_Args cs_args;
		Move_Scene* scene = MAIN_SCENE_MANAGER->find_scene_simple(it->second.muid, 0, cs_args);
		if(!scene){
			continue;
		}
		int scene_id = scene->scene_id();
		Vlt_Monster_Point_Map* point_map = CONFIG_CACHE_VALENTINE_MONSTER->get_monster_point_map(scene_id);
		if(!point_map){
			continue;
		}
		int point_id_index = MAIN_SCENE_M_P_ID_VT_2;
		int point_amount = 0;
		Vlt_Monster_Point_Map::iterator p_it;
		for(p_it = point_map->begin(); p_it != point_map->end(); ++p_it){
			int refresh_time = p_it->second.refresh_time;
			uint64_t base_record_id = p_it->first;
			NPC_Record* base_record = scene->get_npc_record(base_record_id);
			if(!base_record){
				continue;
			}
			if(p_it->second.type == 0){// normal monster
				int coord_num = p_it->second.birth_coord.size();
				int birth_radius = p_it->second.birth_radius;
				if(birth_radius <= 0){
					birth_radius = 36;
				}
				for(int c_i = 0; c_i < coord_num; ++c_i){
					Coord refresh_coord = p_it->second.birth_coord[c_i];
					int monster_num = p_it->second.monster_amount;
					for(int m_i = 0; m_i < monster_num; ++m_i){
						Coord birth_coord;
						int a = random()%birth_radius;
						int b = random()%birth_radius;
						birth_coord.x = refresh_coord.x + a;
						birth_coord.y = refresh_coord.y + b;
						if (scene->mpt_value_by_coord(birth_coord.grid_coord().x,
								birth_coord.grid_coord().y) < Move_Scene::MOVE_ABLE_FROM) {
							a = random()%birth_radius;
							b = random()%birth_radius;
							birth_coord.x = refresh_coord.x + a;
							birth_coord.y = refresh_coord.y + b;
							if(scene->mpt_value_by_coord(birth_coord.grid_coord().x,
								birth_coord.grid_coord().y) < Move_Scene::MOVE_ABLE_FROM){
								birth_coord = refresh_coord;
							}
						}
						NPC_Record record;
						record.config = base_record->config;
						record.config.id = point_id_index+point_amount;
						++point_amount;
						record.config.birth_coord = birth_coord;
						record.config.refresh_times = 0;
						record.config.refresh_time = refresh_time;
						record.birth_freq = Time_Value::gettimeofday();
						record.check_refresh = true;
						scene->add_npc_record(record.config.id, record);
					}
				}

			}else{// boss
				int coord_num = p_it->second.birth_coord.size();
				if(coord_num <= 0){
					continue;
				}
				Coord birth_coord = p_it->second.birth_coord[0];
				NPC_Record record;
				record.config = base_record->config;
				// init boss hp
				if(boss_tid == 0){
					if(record.config.npc_vec.empty() == false){
						it->second.boss_type_id = record.config.npc_vec[0].val_1;
					}
				}else{
					it->second.boss_type_id = boss_tid;
					if(record.config.npc_vec.empty() == false){
						record.config.npc_vec[0].val_1 = boss_tid;
					}
				}
				record.config.id = MAIN_SCENE_M_P_ID_VT_1;
				record.config.birth_coord = birth_coord;
				record.config.refresh_times = 1;
				record.config.refresh_time = refresh_time;
				record.birth_freq = Time_Value::gettimeofday();
				record.check_refresh = true;
				scene->add_npc_record(record.config.id, record);

				it->second.boss_fight_time = p_it->second.fight_time;
				it->second.boss_end_time = Time_Value::gettimeofday()+Time_Value(p_it->second.fight_time);
				it->second.boss_point_id = record.config.id;
				it->second.boss_reward = p_it->second.boss_reward;

				const NPC_Config_Cache* cfg_npc = CONFIG_CACHE_NPC->find_npc_config_cache(it->second.boss_type_id);
				if(cfg_npc){
					int count = cfg_npc->prop_value_vec.size();
					for(int i = 0; i < count; ++i){
						if(cfg_npc->prop_value_vec[i].id == 506205){
							it->second.boss_hp_cur = cfg_npc->prop_value_vec[i].value;
							it->second.boss_hp_max = cfg_npc->prop_value_vec[i].value;
							break;
						}
					}
				}

			}
		}
		it->second.normal_point_amount = point_amount;
	}
}

void Valentine_Monster_Manager::act_end_del_monster(void){
	Main_Scene_Map::iterator it;
	for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
		Create_Scene_Args cs_args;
		Move_Scene* scene = MAIN_SCENE_MANAGER->find_scene_simple(it->second.muid, 0, cs_args);
		if(!scene){
			continue;
		}
		uint64_t boss_point_id = MAIN_SCENE_M_P_ID_VT_1;
		scene->del_npc_record(boss_point_id);
		int point_amount = it->second.normal_point_amount;
		for(int i = 0; i < point_amount; ++i){
			uint64_t pt_id = MAIN_SCENE_M_P_ID_VT_2+i;
			scene->del_npc_record(pt_id);
		}
	}
}
