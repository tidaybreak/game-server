/*
 * Expedition_Scene.cpp
 *
 *  Created on: 2014-10-14
 *      Author: XIAOLIANG
 */

#include "Expedition_Scene.h"
#include "Config_Cache_Expedition.h"
#include "Expedition_Scene_Def.h"
#include "fighter/Fighter.h"
#include "Msg_Active_Struct.h"
#include "Scene_Player.h"
#include "Msg_Inner_Struct.h"
#include "Expedition_Scene_Manager.h"
#include "expedition/Expedition_Func.h"
#include "Msg_Expedition_Struct.h"
#include "expedition/Expedition_Config.h"
#include "Pool_Manager.h"
#include "Expedition_Global_Scene_Manager.h"
#include "Base_Detail.h"
#include "Scene_Monitor.h"
#include "NPC/NPC_Manager.h"
#include "Battle_Scene.h"
#include "Err_Code.h"
#include "expedition/Expedition_Config.h"
#include "Config_Cache_NPC.h"
#include "expedition/Expedition_Def.h"
#include "Msg_Inner_Enum.h"
#include "Global_Scene_Manager.h"
#include "Config_Cache_Scene.h"
#include "team/Team_Func.h"
#include "Scene_Public.h"

Expedition_Scene::Expedition_Scene() {
	reset();
}

Expedition_Scene::~Expedition_Scene() {

}

void Expedition_Scene::reset(void) {
	map_data_.clear();
	for (Player_Soldier_Map::iterator it = player_soldier_map_.begin();
			it != player_soldier_map_.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	player_soldier_map_.clear();
//	map_gang_in_num_ = 0;
	point_gang_id_.clear();
	map_base_point_num_ = 0;

	cur_line_ = 0;
	manager_ = 0;
	gang_id_areavalue_.clear();
//	gang_id_territory_.clear();
	base_territorys_.clear();
	gang_ids_.clear();
	fighting_set_.clear();
	fighting_cancel_list_.clear();
	other_fighting_set_.clear();
	other_fighting_cancel_list_.clear();

	for (Soldier_List::iterator it = soldier_offline_.begin();
			it != soldier_offline_.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(*it);
	}
	soldier_offline_.clear();
	award_time_ = Time_Value::zero;
	demage_rank_.clear();
	demage_vec_.clear();

	settle_time_ = -1;
	settle_time_falg_ = Time_Value::zero;
	occupy_castle_rank_.clear();
	occupy_castle_vec_.clear();

	offline_data_.clear();

	refreshed_drop_or_material_.clear();
	refreshed_drop_or_material_time_history_.clear();
	refreshed_monster_timing_.clear();
	refreshed_monster_kill_gap_.clear();

//	monster_data_refresh_timing_.clear();
//	monster_data_refresh_kill_gap_.clear();

	need_broadcast_point_.clear();
}

int Expedition_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	if (mover->player_self()) {
		Scene_Player *player = mover->player_self();
		Expedition_Scene_Info* rank_info = find_offline_mover(mover->role_id());
		if (rank_info) {
			mover->player_self()->set_expedition_scene_info(*rank_info);

			player->expedition_scene_info().src_hp = player->blood_current();
			double dif = player->blood_current() - player->expedition_scene_info().hp;
			if(dif < 0.00001 && dif > -0.00001){
				// nothing
			}else{
				// 恢复血量
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->expedition_scene_info().hp;
				fight_setter.push_back(setter);
				//player->fight_modify_fight_prop(fight_setter, true);
				player->modify_props_normal(fight_setter, false, 12);
			}
			this->del_offline_mover(mover->role_id());

			// check if defending
			player->check_defending();
			del_offline_data(player->role_id());
		} else {
			player->expedition_scene_info().src_hp = player->blood_current();
			player->expedition_scene_info().hp = player->blood_max();

			// 满血
			if(player->current_blood_rate() < 0.99999){
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->blood_max();
				fight_setter.push_back(setter);
				//player->fight_modify_fight_prop(fight_setter, true);
				player->modify_props_normal(fight_setter, false, 30);
			}
		}
		player->expedition_scene_info().is_enter_scene = 1;
		change_move_able_by_gang_id(mover->player_self()->ganger_detail().gang_id);
//		mover->player_self()->check_and_init_territorys();
		check_and_init_territorys(player);

	}
	return 0;
}

int Expedition_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);

	if (mover->mover_type() == MOVER_T_PLAYER) {
		mover->player_self()->expedition_scene_info().hp = mover->player_self()->blood_current();
		add_offline_mover(mover->player_self()->expedition_scene_info());
		double src_hp = mover->player_self()->expedition_scene_info().src_hp;
		mover->player_self()->reset_morale_and_hp(-1.0f, src_hp, false);

		// look_point clear --
		Int_Hash_Set look_point = mover->player_self()->expedition_scene_info().look_point;
		for (Int_Hash_Set::const_iterator it = look_point.begin();
				it != look_point.end(); ++it) {
			this->exit_drop_out_looking(mover->player_self(), *it);
		}
		Int_Hash_Set fight_point = mover->player_self()->expedition_scene_info().fight_point;
		for (Int_Hash_Set::const_iterator it = fight_point.begin();
				it != fight_point.end(); ++it) {
			this->exit_drop_out_fighting(mover->player_self(), *it);
		}
		// -- look_point clear
//		recover_territorys(mover->player_self());
		if (!mover->player_self()->expedition_scene_info().fight_point.empty()) {
			add_offline_data(mover->player_self());
		}

		mover->player_self()->reset_expedition_scene_info();
		mover->player_self()->fight_fixed_prop_modify(Property_Type::PT_SPEED, AT_EXPEDITION, O_SET, 0);


//		for (Stronghold_Set::iterator it = fighting_set_.begin();
//					it != fighting_set_.end(); ++it) {
//			Stronghold* sh = *it;
//
//			Stronghold* sh = *it;
//			if (EBT_BEAST == sh->type) {
////				for (Role_ExpeditionFightInfo_Map::iterator it_rdmfb = sh->info.role_defend_map_for_beast.begin();
////						it_rdmfb != sh->info.role_defend_map_for_beast.end(); ++it_rdmfb) {
////					if (it_rdmfb->second.attack_list.empty() && it_rdmfb->second.attacking_map.empty()) {
////	//					fighting_cancel_list_.push_back(sh);
////						MSG_USER("attack:empty");
////					} else if (it_rdmfb->second.defend_list.empty() && it_rdmfb->second.defending_map.empty()) {
////	//					fighting_cancel_list_.push_back(sh);
////						MSG_USER("defend:empty");
////					} else {
////						if (it_rdmfb->second.attacking_map.empty() && it_rdmfb->second.defending_map.empty()) {
////							if (it_rdmfb->second.battle_finish_time < now) {
////								enter_battle(sh);
////							}
////						} else if (it_rdmfb->second.attacking_map.empty()) {
////							MSG_TRACE_ABORT("ERROR: expedition point_id(%d) only defending_map.", sh->point_id);
////
////							for (Soldier_Map::iterator it_am = it_rdmfb->second.defending_map.begin();
////									it_am != it_rdmfb->second.defending_map.end(); ++it_am) {
////								++it_am->second->nums;
////								if (0 == it_rdmfb->second.defend_map.count(it_am->first)) {
////									it_rdmfb->second.defend_map.insert(*it_am);
////								}
////								it_rdmfb->second.defend_list.push_front(it_am->second);
////							}
////							it_rdmfb->second.defending_map.clear();
////						} else if (it_rdmfb->second.defending_map.empty()) {
////							MSG_TRACE_ABORT("ERROR: expedition point_id(%d) only attacking_map.", sh->point_id);
////
////							for (Soldier_Map::iterator it_am = it_rdmfb->second.attacking_map.begin();
////									it_am != it_rdmfb->second.attacking_map.end(); ++it_am) {
////								++it_am->second->nums;
////								if (0 == it_rdmfb->second.attack_map.count(it_am->first)) {
////									it_rdmfb->second.attack_map.insert(*it_am);
////								}
////								it_rdmfb->second.attack_list.push_front(it_am->second);
////							}
////							it_rdmfb->second.attacking_map.clear();
////						}
////					}
////				}
//				//Role_ExpeditionFightInfo_Map
//			} else {
////				Soldier_Map::iterator it_al = sh->info.attack_map.find(mover->player_self()->role_id());
////				if (it_al != sh->info.attack_map.end()) {
////					sh->info.attack_list.erase(it_al->second);
////					sh->info.attack_map.erase(it_al);
////					break;
////				}
////				it_al = sh->info.defend_map.find(mover->player_self()->role_id());
////				if (it_al != sh->info.defend_map.end()) {
////					sh->info.defend_list.erase(it_al->second);
////					sh->info.defend_map.erase(it_al);
////					break;
////				}
//			}
//		}
		//通知逻辑服当前玩家的位置
		MSG_20610011 msg;
		msg.coord = mover->grid_coord();
		//封装buf
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		mover->player_self()->send_to_logic(buf);

	}

	return 0;
}

int Expedition_Scene::client_scene_complete(Fighter &fighter){
	Move_Scene::client_scene_complete(fighter);
	if(fighter.player_self()){
		fighter.player_self()->expedition_scene_info().role_id = fighter.player_self()->role_id();
		fighter.player_self()->send_expedition_scene_info();
		if (fighter.player_self()->expedition_scene_info().is_enter_scene) {
			send_strong_hold_data_to_client(fighter.player_self());
			if (fighter.player_self()->expedition_scene_info().is_enter_scene == 1) {
				fighter.player_self()->fight_fixed_prop_modify(Property_Type::PT_SPEED, AT_EXPEDITION, O_SUB, 95);
			}
			fighter.player_self()->active_fight_status();
		}
		fighter.player_self()->update_mover_expedition_info();
		fighter.player_self()->expedition_scene_info().is_enter_scene = 0;
		active_expedition_daily_award_info(fighter.player_self());

		if (!need_broadcast_point_.empty()) {
			Int_Hash_Set ihs;
			for(Stronghold_Map::iterator it = map_data_.begin(); it != map_data_.end(); ++it) {
				if (it->second.type == EBT_MONSTER_REFRESH_TIMING
						|| it->second.type == EBT_MONSTER_REFRESH_KILL_GAP) {
					ihs.insert(it->first);
				}
			}
			active_map_all_data_by_point_id(ihs);
			need_broadcast_point_.clear();
		}
	}

	return 0;
}

int Expedition_Scene::update_move_able_info(Mover* mover) {
	mover->clear_moveable_value();
	Int_Hash_Set areavalue;
	if (mover->player_self()) {
		for (Stronghold_Map::const_iterator it_md = map_data_.begin();
				it_md != map_data_.end(); ++it_md) {
			const int &point_id = it_md->first;
			switch(it_md->second.type) {
			case EBT_BEAST: {
				if (mover->player_self()->expedition_detail().collect_beast.count(point_id)) {
					areavalue.insert(it_md->second.areavalue);
					change_move_value(it_md->second.areavalue, MOVE_ABLE_FROM);
					continue;
				}
				break;
			}
			case EBT_CHEST: {
				if (mover->player_self()->expedition_detail().collect_chest.count(point_id)) {
					areavalue.insert(it_md->second.areavalue);
					change_move_value(it_md->second.areavalue, MOVE_ABLE_FROM);
					continue;
				}
				break;
			}
			case EBT_MONSTER_REFRESH_TIMING: {
				if (!this->refreshed_monster_timing_.count(point_id)) {
					areavalue.insert(it_md->second.areavalue);
					change_move_value(it_md->second.areavalue, MOVE_ABLE_FROM);
					continue;
				}
				break;
			}
			case EBT_MONSTER_REFRESH_KILL_GAP: {
				if (refreshed_monster_kill_gap_.count(point_id)) {
					areavalue.insert(it_md->second.areavalue);
					change_move_value(it_md->second.areavalue, MOVE_ABLE_FROM);
					continue;
				}
				break;
			}
			default: {
				if (it_md->second.info.occupier.gang_id == mover->player_self()->ganger_detail().gang_id) {
					areavalue.insert(it_md->second.areavalue);
					change_move_value(it_md->second.areavalue, MOVE_ABLE_FROM);
				}
				break;
			}
			}
		}
//		Int_Hash_Set *av = find_areavalue_by_gang_id(mover->player_self()->ganger_detail().gang_id);
//		if (av) {
//			for (Int_Hash_Set::const_iterator it = av->begin(); it != av->end(); ++it) {
//				areavalue.insert(*it);
//			}
//		}
	}
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		if (!(*it).move_value.empty()) {
			MSG_81000022 msg;
			Move_Value::iterator s_it;
			for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
				if(s_it->val_2 == MOVE_ABLE_FROM){
					if (s_it->val_1 == 1) {
						msg.moveable_info_vec.push_back(s_it->val_1);
						int16_t val1 = s_it->val_1;
						mover->add_moveable_value(val1);
					} else if (mover->player_self() && areavalue.count(s_it->val_1) > 0){
						if (0 == mover->player_self()->expedition_scene_info().revive) {
							msg.moveable_info_vec.push_back(s_it->val_1);
							int16_t val1 = s_it->val_1;
							mover->add_moveable_value(val1);
						}
					} else {
						//
					}
				}
			}
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}else{
			MSG_81000022 msg;
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}
		break;
	}
	return 0;
}

int Expedition_Scene::change_move_value(int16_t mp_value, bool moveable) {
	int val_2 = UNMOVE_ABLE_VAL;
	if(moveable){
		val_2 = MOVE_ABLE_FROM;
	}
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		bool exist = false;
		for(Move_Value::iterator s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
			if((*s_it).val_1 == mp_value){
				exist = true;
				(*s_it).val_2 = val_2;
				break;
			}
		}
		if(exist == false){
			Int_Int d_int;
			d_int.val_1 = mp_value;
			d_int.val_2 = val_2;
			(*it).move_value.push_back(d_int);
		}
		//
//		for(Mover_Map::iterator player_it = (*it).mover_map_.begin();
//				player_it != (*it).mover_map_.end(); ++player_it){
//			if(player_it->second->player_self()){
//				MSG_81000022 msg;
//				Move_Value::iterator s_it;
//				for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
//					if(s_it->val_2 == MOVE_ABLE_FROM && player_it->second->player_self()){
//						Scene_Player *pl = player_it->second->player_self();
//						if (s_it->val_1 == 1) {
//							msg.moveable_info_vec.push_back(s_it->val_1);
//							int16_t val1 = s_it->val_1;
//							pl->add_moveable_value(val1);
//						} else if (player_it->second->player_self()->player_self() && areavalue->count(s_it->val_1) > 0){
//							if (pl->player_self()->expedition_scene_info().revive) {
//								msg.moveable_info_vec.push_back(s_it->val_1);
//								int16_t val1 = s_it->val_1;
//								pl->add_moveable_value(val1);
//							}
//						} else {
//							//
//						}
//					}
//				}
//				OBJ_SEND_TO_CLIENT(msg, (*player_it->second));
//			} else {
//				if(moveable){
//					player_it->second->add_moveable_value(mp_value);
//				}else{
//					player_it->second->del_moveable_value(mp_value);
//				}
//			}
//		}
	}
	return 0;
}

int Expedition_Scene::tick(const Time_Value &now) {
	Move_Scene::tick(now);

	fight_castle_tick(now);
	fight_other_tick(now);

	award_tick(now);

	settle_tick(now);

	refresh_monster_timing_tick(now);
	refresh_monster_kill_gap_tick(now);
	refresh_drop_or_material(now);
	return 0;
}

int Expedition_Scene::fight_castle_tick(const Time_Value &now) {
	for (Stronghold_Set::iterator it = fighting_set_.begin();
			it != fighting_set_.end(); ++it) {
		Stronghold* sh = *it;
		if (sh->info.attack_list.empty() && sh->info.attacking_map.empty()) {
			fighting_cancel_list_.push_back(sh);
			MSG_USER("attack:empty");
		} else if (sh->info.defend_list.empty() && sh->info.defending_map.empty()) {
			fighting_cancel_list_.push_back(sh);
			MSG_USER("defend:empty");
		} else {
			if (sh->info.attacking_map.empty() && sh->info.defending_map.empty()) {
				if (sh->info.battle_finish_time < now) {
					if (0 == enter_battle(sh)) {
						active_map_all_data_by_point_id(sh->point_id);
					}
				}
			}  else if (sh->info.defending_map.empty()) {
//				MSG_TRACE_ABORT("ERROR: expedition point_id(%d) only attacking_map.", sh->point_id);

				for (Soldier_Map::iterator it_am = sh->info.attacking_map.begin();
						it_am != sh->info.attacking_map.end(); ++it_am) {
					++it_am->second->nums;
					if (0 == sh->info.attack_map.count(it_am->first)) {
						sh->info.attack_map.insert(*it_am);
					}
					sh->info.attack_list.push_front(it_am->second);
				}
				sh->info.attacking_map.clear();
			} else if (sh->info.attacking_map.empty()) {
//				MSG_TRACE_ABORT("ERROR: expedition point_id(%d) only defending_map.", sh->point_id);

				for (Soldier_Map::iterator it_am = sh->info.defending_map.begin();
						it_am != sh->info.defending_map.end(); ++it_am) {
					++it_am->second->nums;
					if (0 == sh->info.defend_map.count(it_am->first)) {
						sh->info.defend_map.insert(*it_am);
					}
					sh->info.defend_list.push_front(it_am->second);
				}
				sh->info.defending_map.clear();
			}else if (sh->info.attacking_map.size() < 3 && !sh->info.attack_list.empty()) {
				if (0 == enter_battle_more_attack(sh)) {
					active_map_all_data_by_point_id(sh->point_id);
					active_expedition_help_info(sh);
				}
			} else if (sh->info.defending_map.size() < 3 && !sh->info.defend_list.empty()) {
				if (0 == enter_battle_more_defend(sh)) {
					active_map_all_data_by_point_id(sh->point_id);
					active_expedition_help_info(sh);
				}
			}
		}
	}

	while(!fighting_cancel_list_.empty()) {
		Stronghold *sh = fighting_cancel_list_.front();
		fighting_set_.erase(sh);
		change_point_force(sh->point_id);
		sh->info.attacker.reset();
		tick_check_and_clear_defend_when_attack_is_empty(sh);
		fighting_cancel_list_.pop_front();
	}
	return 0;
}

int Expedition_Scene::fight_other_tick(const Time_Value &now) {
	boost::unordered_map<role_id_t, Stronghold*> rim;
	for (Stronghold_Set::iterator it = other_fighting_set_.begin();
			it != other_fighting_set_.end(); ++it) {
		Stronghold* sh = *it;
		for (Role_ExpeditionFightInfo_Map::iterator it_rdmfb = sh->info.role_defend_map_for_beast.begin();
				it_rdmfb != sh->info.role_defend_map_for_beast.end(); ++it_rdmfb) {
			Expedition_Fight_Info &efi = it_rdmfb->second;

			if ((efi.attack_list.empty() && efi.attacking_map.empty())
					|| (efi.defend_list.empty() && efi.defending_map.empty())) {
				rim.insert(std::make_pair(it_rdmfb->first, sh));
				efi.fight_status = EBFS_PEACE;
			} else {
				if (efi.attacking_map.empty() && efi.defending_map.empty()) {
					if (efi.battle_finish_time < now) {
						enter_battle_for_other(it_rdmfb->first, sh);
					}
				} else if (efi.defending_map.empty()) {
//					MSG_TRACE_ABORT("ERROR: expedition point_id(%d) only attacking_map.", sh->point_id);

					for (Soldier_Map::iterator it_am = efi.attacking_map.begin();
							it_am != efi.attacking_map.end(); ++it_am) {
						++it_am->second->nums;
						if (0 == efi.attack_map.count(it_am->first)) {
							efi.attack_map.insert(*it_am);
						}
						efi.attack_list.push_front(it_am->second);
					}
					efi.attacking_map.clear();
					efi.fight_status = EBFS_PEACE;
					rim.insert(std::make_pair(it_rdmfb->first, sh));
				} else if (efi.attacking_map.empty()) {
//					MSG_TRACE_ABORT("ERROR: expedition point_id(%d) only defending_map.", sh->point_id);

					for (Soldier_Map::iterator it_am = efi.defending_map.begin();
							it_am != efi.defending_map.end(); ++it_am) {
						++it_am->second->nums;
						if (0 == efi.defend_map.count(it_am->first)) {
							efi.defend_map.insert(*it_am);
						}
						efi.defend_list.push_front(it_am->second);
					}
					efi.defending_map.clear();
					efi.fight_status = EBFS_PEACE;
					rim.insert(std::make_pair(it_rdmfb->first, sh));
				}
			}

		}
	}

	for (boost::unordered_map<role_id_t, Stronghold*>::iterator it = rim.begin();
			it != rim.end(); ++it) {
		Stronghold* sh = it->second;
		Role_ExpeditionFightInfo_Map::iterator it_beast = sh->info.role_defend_map_for_beast.find(it->first);
		if (it_beast == sh->info.role_defend_map_for_beast.end()) {
			continue;
		}
		if ((it_beast->second.defend_list.empty() && it_beast->second.defending_map.empty())
				|| it_beast->second.has_complete) {
			Scene_Player *sp = SCENE_MONITOR->find_scene_player_by_role_id(it->first);
			if (!sp) {
				continue;
			}
			if (!sp->expedition_detail().collect_beast.count(it->second->point_id)) {
				create_drop_by_point_id(it->second->point_id, *sp);
			}

			MSG_50201306 res_msg;
			res_msg.point_id = it->second->point_id;
			res_msg.result = (0 == this->drop_out_fighting(sp, it->second->point_id)) ? 0 : 1;
			res_msg.state = 0;
			OBJ_SEND_TO_CLIENT(res_msg, (*sp));
			it->second->info.role_defend_map_for_beast.erase(it->first);
			active_map_data_by_point_id(sp, it->second->point_id);
		}
	}

	for (Stronghold_Set::iterator it = other_fighting_set_.begin();
				it != other_fighting_set_.end(); ++it) {
		Stronghold* sh = *it;
		bool all_attack_empty = true;
		bool all_defned_empty = true;
		for (Role_ExpeditionFightInfo_Map::iterator it_rdmfb = sh->info.role_defend_map_for_beast.begin();
							it_rdmfb != sh->info.role_defend_map_for_beast.end(); ++it_rdmfb) {
			if (!it_rdmfb->second.attack_list.empty() || !it_rdmfb->second.attacking_map.empty()) {
				all_attack_empty = false;
			}
			if (!it_rdmfb->second.defend_list.empty() || !it_rdmfb->second.defending_map.empty()) {
				all_defned_empty = false;
			}
		}
		if (all_attack_empty || all_defned_empty) {
			other_fighting_cancel_list_.push_back(sh);
		}
	}


	while(!other_fighting_cancel_list_.empty()) {
		Stronghold *sh = other_fighting_cancel_list_.front();
		other_fighting_set_.erase(sh);
		other_fighting_cancel_list_.pop_front();
	}
	return 0;
}

int Expedition_Scene::tick_check_and_clear_defend_when_attack_is_empty(Stronghold *sh) {
	if (!sh->info.attack_map.empty()) {
		return -1;
	}
	Role_Hash_Set clear_role;
	for (Soldier_Map::iterator it = sh->info.defend_map.begin();
			it != sh->info.defend_map.end(); ++it) {
		if (it->second->type == EST_PLAYER && find_offline_data(it->second->id)) {
			clear_role.insert(it->second->id);
		}
	}
	for (Role_Hash_Set::const_iterator it = clear_role.begin();
			it != clear_role.end(); ++it) {
		del_offline_data(*it);
		Soldier_Map::iterator it_dm = sh->info.defend_map.find(*it);
		if (it_dm != sh->info.defend_map.end()) {
			sh->info.defend_list.remove(it_dm->second);
			POOL_MANAGER->push_soldier_pool(it_dm->second);
			sh->info.defending_map.erase(it_dm->first);
			sh->info.defend_map.erase(it_dm);

			Mover* mover = find_mover_with_layer(0, it_dm->second->id);
			if(mover && mover->player_self()){
				continue;
			}
			mover->player_self()->active_fight_status_for_all();
		}
	}
	return 0;
}

void Expedition_Scene::hook_player_enter_battle(Scene_Player* player){
	std::pair<Role_Id_Set::iterator, bool> ret;
	ret = this->player_enter_battle_set_.insert(player->role_id());
}

void Expedition_Scene::hook_scene_pass(void) {

}

void Expedition_Scene::hook_battle_finish(int battle_type, int win_pos,
		std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters){
//	Scene_Layer *layer = scene_layer(0);
	if (battle_type == Battle_Scene::BATTLE_TYPE_TEAM) {
		Battle_Scene *bs = 0;
		for (std::vector<Mover*>::const_iterator it = win_mover_vec.begin();
				it != win_mover_vec.end(); ++it) {
			if ((**it).battle_scene()) {
				bs = (**it).battle_scene();
				break;
			}
		}

		if (!bs) {
			for (std::vector<Mover*>::const_iterator it = lose_mover_vec.begin();
					it != lose_mover_vec.end(); ++it) {
				if ((**it).battle_scene()) {
					bs = (**it).battle_scene();
					break;
				}
			}
			if (!bs) {
				return ;
			}
		}

		Other_Info ot = bs->get_other_info();
		if (ot.type == OT_EXPEDITION) {
			bool is_castle = false;
			Stronghold *sh = this->find_map_data_by_point_id(ot.int_v1);
			if (sh) { // sh begin --
				if (EBT_BEAST == sh->type || EBT_CHEST == sh->type) {
					hook_battle_finish_for_other(bs, sh, ot, battle_type, win_pos, win_mover_vec, lose_mover_vec, kill_monters);
				} else {
					hook_battle_finish_for_castle(bs, sh, ot, battle_type, win_pos, win_mover_vec, lose_mover_vec, kill_monters);
					is_castle = true;
				}
				active_map_all_data_by_point_id(sh->point_id);
			} // -- sh end

			const Expedition_Award_Demage_Cfg cfg = CONFIG_CACHE_EXPEDITION->demage_award();
			for (std::vector<Mover*>::const_iterator it_mv = win_mover_vec.begin();
					it_mv != win_mover_vec.end(); ++it_mv) {
				if ((**it_mv).player_self()) {
//					if (is_castle) {
//						add_occupy_castle_rank((**it_mv).player_self());
//					}
					if ((**it_mv).player_self()->dead_status()) {
						// back to
						this->drop_out_fighting((**it_mv).player_self(), ot.int_v1);
						this->drop_out_looking((**it_mv).player_self(), ot.int_v1);
						(**it_mv).player_self()->expedition_die_revive_winner();
						(**it_mv).player_self()->update_mover_expedition_info();
					}
					double demage = (**it_mv).player_self()->get_ttl_damage_in_battle();
					if (demage > 1) {
						this->add_demage_rank((**it_mv).player_self(), demage);
						int gold = (int) (std::sqrt(demage) * cfg.gold * cfg.modify / 10000);
						int souls = (int) (std::sqrt(demage) * cfg.modify * cfg.souls / 10000);
						Block_Buffer buf;
						buf.make_message(INNER_EXPEDITION_PLAYER);
						buf.write_int8((int8_t)Data_Channel::DEMAGE_AWARD);
						buf.write_int32(gold); // ask
						buf.write_int32(souls); // ask
						buf.finish_message();
						(**it_mv).player_self()->send_to_logic(buf);
					}
				}
			}
			for (std::vector<Mover*>::const_iterator it_mv = lose_mover_vec.begin();
					it_mv != lose_mover_vec.end(); ++it_mv) {
				if ((**it_mv).player_self()) {
					// back to
					this->drop_out_fighting((**it_mv).player_self(), ot.int_v1);
					this->drop_out_looking((**it_mv).player_self(), ot.int_v1);
					(**it_mv).player_self()->expedition_die_revive();
					(**it_mv).player_self()->update_mover_expedition_info();
					double demage = (**it_mv).player_self()->get_ttl_damage_in_battle();
					if (demage > 1) {
						this->add_demage_rank((**it_mv).player_self(), demage);
						int gold = (int) (std::sqrt(demage) * cfg.gold * cfg.modify / 10000);
						int souls = (int) (std::sqrt(demage) * cfg.modify * cfg.souls/ 10000);
						Block_Buffer buf;
						buf.make_message(INNER_EXPEDITION_PLAYER);
						buf.write_int8((int8_t)Data_Channel::DEMAGE_AWARD);
						buf.write_int32(gold); // ask
						buf.write_int32(souls); // ask
						buf.finish_message();
						(**it_mv).player_self()->send_to_logic(buf);
					}
				}
			}
			if (sh) { // sh begin --
				if (EBT_BEAST == sh->type || EBT_CHEST == sh->type) {
					Time_Value now = Time_Value::gettimeofday();
					fight_other_tick(now);
				} else {
					Time_Value now = Time_Value::gettimeofday();
					fight_castle_tick(now);
				}
			} // -- sh end
		}
	} else if (battle_type == Battle_Scene::BATTLE_TYPE_PVP) {
		Battle_Scene *bs = 0;
		for (std::vector<Mover*>::const_iterator it = win_mover_vec.begin();
				it != win_mover_vec.end(); ++it) {
			if ((**it).battle_scene()) {
				bs = (**it).battle_scene();
				break;
			} else {
//				MSG_TRACE_ABORT("ERROR: expedition role(%ld) cant find battle_scene on hook battle_finish.", (**it).role_id());
			}
		}
		if (!bs) {
			return ;
		}
		Other_Info ot = bs->get_other_info();
		if (ot.type == OT_EXPEDITION) {
			active_map_all_data_by_point_id(ot.int_v1);
		}
		const Expedition_Award_Demage_Cfg cfg = CONFIG_CACHE_EXPEDITION->demage_award();
		for (std::vector<Mover*>::const_iterator it_mv = win_mover_vec.begin();
				it_mv != win_mover_vec.end(); ++it_mv) {
			if ((**it_mv).player_self()) {
				if ((**it_mv).player_self()->dead_status()) {
					(**it_mv).player_self()->expedition_die_revive_winner();
					(**it_mv).player_self()->update_mover_expedition_info();
				}
				double demage = (**it_mv).player_self()->get_ttl_damage_in_battle();
				if (demage > 1) {
					this->add_demage_rank((**it_mv).player_self(), demage);
					int gold = (int) (std::sqrt(demage) * cfg.gold * cfg.modify / 10000);
					int souls = (int) (std::sqrt(demage) * cfg.modify * cfg.souls / 10000);
					Block_Buffer buf;
					buf.make_message(INNER_EXPEDITION_PLAYER);
					buf.write_int8((int8_t)Data_Channel::DEMAGE_AWARD);
					buf.write_int32(gold); // ask
					buf.write_int32(souls); // ask
					buf.finish_message();
					(**it_mv).player_self()->send_to_logic(buf);
				}
			}
		}
		for (std::vector<Mover*>::const_iterator it_mv = lose_mover_vec.begin();
				it_mv != lose_mover_vec.end(); ++it_mv) {
			if ((**it_mv).player_self()) {
				this->drop_out_fighting((**it_mv).player_self(), ot.int_v1);
				this->drop_out_looking((**it_mv).player_self(), ot.int_v1);
				(**it_mv).player_self()->expedition_die_revive();
				(**it_mv).player_self()->update_mover_expedition_info();
				double demage = (**it_mv).player_self()->get_ttl_damage_in_battle();
				if (demage > 1) {
					this->add_demage_rank((**it_mv).player_self(), demage);
					int gold = (int) (std::sqrt(demage) * cfg.gold * cfg.modify / 10000);
					int souls = (int) (std::sqrt(demage) * cfg.modify * cfg.souls/ 10000);
					Block_Buffer buf;
					buf.make_message(INNER_EXPEDITION_PLAYER);
					buf.write_int8((int8_t)Data_Channel::DEMAGE_AWARD);
					buf.write_int32(gold); // ask
					buf.write_int32(souls); // ask
					buf.finish_message();
					(**it_mv).player_self()->send_to_logic(buf);
				}
			}
		}
	}
}

void Expedition_Scene::hook_battle_finish_for_other(Battle_Scene *bs, Stronghold *sh, Other_Info ot, int battle_type, int win_pos,
			std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters) {
	if (!bs || !sh) {
		return ;
	}
	// pos begin --
	if (win_pos == 0) {
		// player
		for (std::vector<Mover*>::const_iterator it_av = win_mover_vec.begin();
				it_av != win_mover_vec.end(); ++it_av) {
			if ((**it_av).player_self()) {
				Scene_Player *pl = (**it_av).player_self();
				role_id_t role_id = pl->role_id();
				if (sh->info.role_defend_map_for_beast.count(role_id)) {
					Expedition_Fight_Info &role_defend_map_for_beast = sh->info.role_defend_map_for_beast[role_id];

					Soldier_Map::const_iterator it_am = role_defend_map_for_beast.attacking_map.find(role_id);
					if (it_am != role_defend_map_for_beast.attacking_map.end()) {
						if (it_am->second->exit) {
							role_defend_map_for_beast.attack_list.remove(it_am->second);
							POOL_MANAGER->push_soldier_pool(it_am->second);
							role_defend_map_for_beast.attacking_map.erase(role_id);
							role_defend_map_for_beast.attack_map.erase(role_id);
						} else {
							++it_am->second->nums;
							it_am->second->nums_battle = 0;
							role_defend_map_for_beast.attack_list.push_front(it_am->second);
							role_defend_map_for_beast.attacking_map.erase(it_am);
						}
//					} else {
//						MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find attacking_map(%ld) on battle_finish.", sh->point_id, (**it_av).player_self()->role_id());
					}

					for (Soldier_Map::iterator it_am = role_defend_map_for_beast.defending_map.begin();
							it_am != role_defend_map_for_beast.defending_map.end(); ++it_am) {
						if (it_am->second->exit) {
							role_defend_map_for_beast.defend_list.remove(it_am->second);
							POOL_MANAGER->push_soldier_pool(it_am->second);
							role_defend_map_for_beast.defend_map.erase(it_am->first);
							continue;
						}
						if (it_am->second->nums == 0) {
							role_defend_map_for_beast.defend_list.remove(it_am->second);
							POOL_MANAGER->push_soldier_pool(it_am->second);
							role_defend_map_for_beast.defend_map.erase(it_am->first);
							if (EST_PLAYER == it_am->second->type) {
								//revive
							}
						} else {
							if (0 == role_defend_map_for_beast.defend_map.count(it_am->first)) {
								role_defend_map_for_beast.defend_list.remove(it_am->second);
								POOL_MANAGER->push_soldier_pool(it_am->second);
								continue;//sh->info.defend_map.insert(*it_am);
							}
							role_defend_map_for_beast.defend_list.push_front(it_am->second);
						}
					}
					role_defend_map_for_beast.defending_map.clear();
					role_defend_map_for_beast.has_complete = true;
				}

				if (pl->dead_status()) {
					this->drop_out_fighting(pl, ot.int_v1);
					this->drop_out_looking(pl, ot.int_v1);
				}
				pl->update_mover_expedition_info();
			} else if ((**it_av).monster_self()) {
				//
			}
		}

		// monster
//		for (Int_Vec::const_iterator it_iv = ot.iv_1.begin();
//				it_iv != ot.iv_1.end(); ++it_iv) {
//			Soldier_Map::const_iterator it_am = sh->info.attacking_map.find(*it_iv);
//			if (it_am != sh->info.attacking_map.end()) {
//				++it_am->second->nums;
//				sh->info.attack_list.push_front(it_am->second);
//				sh->info.attacking_map.erase(it_am);
//			} else {
//				MSG_TRACE_ABORT("ERROR: expedition point_id(%d) cant find attacking_map(%ld) on battle_finish.", sh->point_id, *it_iv);
//			}
//		}

		// empty check
//		for (Soldier_Map::iterator it_am = sh->info.attacking_map.begin();
//				it_am != sh->info.attacking_map.end(); ++it_am) {
//			MSG_TRACE_ABORT("ERROR: expedition point(%d) cant handle attacking_map(%ld) on battle_finish.", sh->point_id, it_am->first);
//			++it_am->second->nums;
//			if (0 == sh->info.attack_map.count(it_am->first)) {
//				continue;//sh->info.attack_map.insert(*it_am);
//			}
//			sh->info.attack_list.push_front(it_am->second);
//		}
//		sh->info.attacking_map.clear();

		save_tick();
	} else {

		Int_Hash_Set win_monster_pos;
		for (std::vector<Mover*>::const_iterator it_av = win_mover_vec.begin();
				it_av != win_mover_vec.end(); ++it_av) {
			if ((**it_av).player_self()) {
				//
			} else if ((**it_av).player_monster_self()) {
				//
			} else if ((**it_av).monster_self()) {
				win_monster_pos.insert((**it_av).fighter()->battle_position().idx);
			}
		}

		// monster
		// wave begin --
		// not use wave
		Role_Int_Map ii;
		int len = 1;
		for (std::vector<Monster_Wave_Info>::const_iterator it = bs->monster_battle_info().wave_vec.begin();
				it != bs->monster_battle_info().wave_vec.end(); ++it, ++len) {
			if (bs->monster_battle_info().cur_wave >= len) {
				continue;
			}
			int8_t idx = 0;
			Int_Int_Map iim;
			for (std::vector<int8_t>::const_iterator it_pv = it->pos_vec.begin(); it_pv != it->pos_vec.end(); ++it_pv, ++idx) {
				iim.insert(std::make_pair(*it_pv, idx));
			}

			int64_t id = -1;
			if (iim.count(Position_Key::LINE_ONE_FRONT)) {
				id = it->int64_1[ iim[Position_Key::LINE_ONE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_ONE_BEHIND)) {
				id = it->int64_1[ iim[Position_Key::LINE_ONE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_TWO_FRONT)) {
				id = it->int64_1[ iim[Position_Key::LINE_TWO_FRONT] ];
			} else if (iim.count(Position_Key::LINE_TWO_BEHIND)) {
				id = it->int64_1[ iim[Position_Key::LINE_TWO_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_THREE_FRONT)) {
				id = it->int64_1[ iim[Position_Key::LINE_THREE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_THREE_BEHIND)) {
				id = it->int64_1[ iim[Position_Key::LINE_THREE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
		}
		// cur_wave
		if (bs->monster_battle_info().cur_wave <= (int)bs->monster_battle_info().wave_vec.size()) {
			const Monster_Wave_Info &mwi = bs->monster_battle_info().wave_vec[bs->monster_battle_info().cur_wave - 1];
			int8_t idx = 0;
			Int_Int_Map iim;
			for (std::vector<int8_t>::const_iterator it_pv = mwi.pos_vec.begin(); it_pv != mwi.pos_vec.end(); ++it_pv, ++idx) {
				if (!win_monster_pos.count(*it_pv)) {
					continue;
				}
				iim.insert(std::make_pair(*it_pv, idx));
			}
			int64_t id = -1;
			if (iim.count(Position_Key::LINE_ONE_FRONT)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_ONE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_ONE_BEHIND)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_ONE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_TWO_FRONT)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_TWO_FRONT] ];
			} else if (iim.count(Position_Key::LINE_TWO_BEHIND)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_TWO_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_THREE_FRONT)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_THREE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_THREE_BEHIND)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_THREE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
		}
		// -- wave end

		// player
		for (std::vector<Mover*>::const_iterator it_av = lose_mover_vec.begin();
				it_av != lose_mover_vec.end(); ++it_av) {
			if ((**it_av).player_self()) {
				role_id_t role_id = (**it_av).player_self()->role_id();
				if (sh->info.role_defend_map_for_beast.count(role_id)) {
					Soldier_Map::const_iterator it_am = sh->info.role_defend_map_for_beast[role_id].attacking_map.find(role_id);
					if (it_am != sh->info.role_defend_map_for_beast[role_id].attacking_map.end()) {
						if (it_am->second->exit) {
							sh->info.role_defend_map_for_beast[role_id].attack_list.remove(it_am->second);
							POOL_MANAGER->push_soldier_pool(it_am->second);
							sh->info.role_defend_map_for_beast[role_id].attacking_map.erase(role_id);
							sh->info.role_defend_map_for_beast[role_id].attack_map.erase(role_id);
						} else {
//							++it_am->second->nums;
//							it_am->second->nums_battle = 0;
							sh->info.role_defend_map_for_beast[role_id].attack_list.remove(it_am->second);
							POOL_MANAGER->push_soldier_pool(it_am->second);
							sh->info.role_defend_map_for_beast[role_id].attacking_map.erase(role_id);
							sh->info.role_defend_map_for_beast[role_id].attack_map.erase(role_id);
						}
//					} else {
//						MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find attacking_map(%ld) on battle_finish.", sh->point_id, (**it_av).player_self()->role_id());
					}
					for (Role_Int_Map::const_iterator it = ii.begin(); it != ii.end(); ++it) {
						Soldier_Map::const_iterator it_am = sh->info.role_defend_map_for_beast[role_id].defending_map.find(it->first);
						if (it_am != sh->info.role_defend_map_for_beast[role_id].defending_map.end()) {
							if (it_am->second->type != EST_PLAYER) {
								it_am->second->nums += it->second;
							} else {
								++it_am->second->nums;
							}
							it_am->second->nums_battle = 0;
							sh->info.role_defend_map_for_beast[role_id].defend_list.push_front(it_am->second);
//							join_in_looking_defend_on_monster_front(sh, it_am->second);
							// soldier init --
							Soldier *soldier = POOL_MANAGER->pop_soldier_pool();
							if (soldier) {
								*soldier = *it_am->second;
								// -- soldier end

								sh->info.role_defend_map_for_beast[role_id].looking_defend_map.insert(std::make_pair(soldier->id, soldier));
								sh->info.role_defend_map_for_beast[role_id].looking_defend_list.push_front(soldier);
							}

							sh->info.role_defend_map_for_beast[role_id].defending_map.erase(it_am);
						}
					}
					sh->info.role_defend_map_for_beast[role_id].defending_map.clear();
				}
//				if ((**it_av).player_self()->dead_status()) {
					this->drop_out_fighting((**it_av).player_self(), ot.int_v1);
					this->drop_out_looking((**it_av).player_self(), ot.int_v1);
//				}
				(**it_av).player_self()->update_mover_expedition_info();
			} else if ((**it_av).monster_self()) {
				//
			}
		}

	}
	// -- pos end

	int gap = 1;
	const Expedition_Common_Cfg *conf =
							CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::FIGHT_GAP);
	if (conf) {
		gap = conf->value < 1 ? 1 : (int)conf->value;
	}
	sh->info.battle_finish_time = Time_Value::gettimeofday() + Time_Value(gap);

	active_player_battle_state(win_mover_vec, sh->point_id, 0);
	active_player_battle_state(lose_mover_vec, sh->point_id, 0);
}

void Expedition_Scene::hook_battle_finish_for_castle(Battle_Scene *bs, Stronghold *sh, Other_Info ot, int battle_type, int win_pos,
			std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters) {
	if (!bs || !sh) {
		return ;
	}
	// pos begin --
	if (win_pos == 0) {
		// player
		for (std::vector<Mover*>::const_iterator it_av = win_mover_vec.begin();
				it_av != win_mover_vec.end(); ++it_av) {
			if ((**it_av).player_self()) {
				role_id_t role_id = (**it_av).player_self()->role_id();

				// role_info
				sh->win_attack_people.career = (**it_av).player_self()->career();
				sh->win_attack_people.combat = 0;
				sh->win_attack_people.gender = (**it_av).player_self()->base_detail().gender;
				sh->win_attack_people.level  = (**it_av).player_self()->level();
				sh->win_attack_people.role_id = (**it_av).player_self()->role_id();
				sh->win_attack_people.role_name = (**it_av).player_self()->role_name();
				sh->win_attack_people.vip_level = (**it_av).player_self()->vip();
				sh->win_attack_people.guild_name = (**it_av).player_self()->base_detail().gang_name;

				Soldier_Map::const_iterator it_am = sh->info.attacking_map.find(role_id);
				if (it_am != sh->info.attacking_map.end()) {
					if (it_am->second->exit) {
						sh->info.attack_list.remove(it_am->second);
						POOL_MANAGER->push_soldier_pool(it_am->second);
						sh->info.attacking_map.erase(role_id);
						sh->info.attack_map.erase(role_id);
					} else {
						++it_am->second->nums;
						it_am->second->nums_battle = 0;
						sh->info.attack_list.push_front(it_am->second);
						Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(it_am->second->id);
						if (pl_a) {
							join_in_looking_front(pl_a, sh->point_id);
						}
						sh->info.attacking_map.erase(it_am);
					}
				} else {
//					MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find attacking_map(%ld) on battle_finish.", sh->point_id, (**it_av).player_self()->role_id());
				}
				(**it_av).player_self()->update_mover_expedition_info();
			} else if ((**it_av).monster_self()) {
				//
			} else if ((**it_av).player_monster_self()) {

			}
		}

		// monster
		for (Int_Vec::const_iterator it_iv = ot.iv_1.begin();
				it_iv != ot.iv_1.end(); ++it_iv) {
			Soldier_Map::const_iterator it_am = sh->info.attacking_map.find(*it_iv);
			if (it_am != sh->info.attacking_map.end()) {
				++it_am->second->nums;
				it_am->second->nums_battle = 0;
				sh->info.attack_list.push_front(it_am->second);
				sh->info.attacking_map.erase(it_am);
			} else {
//				MSG_TRACE_ABORT("ERROR: expedition point_id(%d) cant find attacking_map(%ld) on battle_finish.", sh->point_id, *it_iv);
			}
		}

		// empty check
		for (Soldier_Map::iterator it_am = sh->info.attacking_map.begin();
				it_am != sh->info.attacking_map.end(); ++it_am) {
//			MSG_TRACE_ABORT("ERROR: expedition point(%d) cant handle attacking_map(%ld) on battle_finish.", sh->point_id, it_am->first);
			++it_am->second->nums;
			it_am->second->nums_battle = 0;
			if (0 == sh->info.attack_map.count(it_am->first)) {
				sh->info.attack_list.remove(it_am->second);
				POOL_MANAGER->push_soldier_pool(it_am->second);
				continue;//sh->info.attack_map.insert(*it_am);
			}
			sh->info.attack_list.push_front(it_am->second);
		}
		sh->info.attacking_map.clear();

		for (Soldier_Map::iterator it_am = sh->info.defending_map.begin();
				it_am != sh->info.defending_map.end(); ++it_am) {
			if (it_am->second->exit) {
				sh->info.defend_list.remove(it_am->second);
				POOL_MANAGER->push_soldier_pool(it_am->second);
				sh->info.defend_map.erase(it_am->first);
				continue;
			}
			if (it_am->second->nums == 0) {
				sh->info.defend_list.remove(it_am->second);
				POOL_MANAGER->push_soldier_pool(it_am->second);
				sh->info.defend_map.erase(it_am->first);
				if (EST_PLAYER == it_am->second->type) {
					//revive
				}
			} else {
				if (0 == sh->info.defend_map.count(it_am->first)) {
					sh->info.defend_list.remove(it_am->second);
					POOL_MANAGER->push_soldier_pool(it_am->second);
					continue;//sh->info.defend_map.insert(*it_am);
				}
				sh->info.defend_list.push_front(it_am->second);
				join_in_looking_defend_on_monster_front(sh, it_am->second);
			}
		}
		sh->info.defending_map.clear();
		save_tick();
	} else {
		Int_Hash_Set win_monster_pos;
		for (std::vector<Mover*>::const_iterator it_av = win_mover_vec.begin();
				it_av != win_mover_vec.end(); ++it_av) {
			if ((**it_av).player_self()) {
				role_id_t role_id = (**it_av).player_self()->role_id();
				Soldier_Map::const_iterator it_am = sh->info.defending_map.find(role_id);
				if (it_am != sh->info.defending_map.end()) {
					if (it_am->second->exit) {
						sh->info.defend_list.remove(it_am->second);
						POOL_MANAGER->push_soldier_pool(it_am->second);
						sh->info.defending_map.erase(role_id);
						sh->info.defend_map.erase(role_id);
					} else {
						++it_am->second->nums;
						it_am->second->nums_battle = 0;
						sh->info.defend_list.push_front(it_am->second);
						Scene_Player *pl_d = SCENE_MONITOR->find_scene_player_by_role_id(it_am->second->id);
						if (pl_d) {
							join_in_looking_front(pl_d, sh->point_id);
						}
						sh->info.defending_map.erase(it_am);
					}
				} else {
//					MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find defending_map(%ld) on battle_finish.", sh->point_id, (**it_av).player_self()->role_id());
				}
				(**it_av).player_self()->update_mover_expedition_info();
			} else if ((**it_av).player_monster_self()) {
				role_id_t role_id = (**it_av).player_monster_self()->fighter_detail().role_id;
				Soldier_Map::const_iterator it_am = sh->info.defending_map.find(role_id);
				if (it_am != sh->info.defending_map.end()) {
					if (it_am->second->exit) {
						sh->info.defend_list.remove(it_am->second);
						POOL_MANAGER->push_soldier_pool(it_am->second);
						sh->info.defending_map.erase(role_id);
						sh->info.defend_map.erase(role_id);
					} else {
						++it_am->second->nums;
						it_am->second->nums_battle = 0;
						sh->info.defend_list.push_front(it_am->second);
						Scene_Player *pl_d = SCENE_MONITOR->find_scene_player_by_role_id(it_am->second->id);
						if (pl_d) {
							join_in_looking_front(pl_d, sh->point_id);
						}
						sh->info.defending_map.erase(it_am);
					}
				} else {
//					MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find defending_map(%ld) on battle_finish.", sh->point_id, (**it_av).player_self()->role_id());
				}
				(**it_av).player_self()->update_mover_expedition_info();
			} else if ((**it_av).monster_self()) {
				win_monster_pos.insert((**it_av).fighter()->battle_position().idx);
			}
		}

		// monster
		// wave begin --
		// not use wave
		Role_Int_Map ii;
		int len = 1;
		for (std::vector<Monster_Wave_Info>::const_iterator it = bs->monster_battle_info().wave_vec.begin();
				it != bs->monster_battle_info().wave_vec.end(); ++it, ++len) {
			if (bs->monster_battle_info().cur_wave >= len) {
				continue;
			}
			int8_t idx = 0;
			Int_Int_Map iim;
			for (std::vector<int8_t>::const_iterator it_pv = it->pos_vec.begin(); it_pv != it->pos_vec.end(); ++it_pv, ++idx) {
				iim.insert(std::make_pair(*it_pv, idx));
			}

			int64_t id = -1;
			if (iim.count(Position_Key::LINE_ONE_FRONT)) {
				id = it->int64_1[ iim[Position_Key::LINE_ONE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_ONE_BEHIND)) {
				id = it->int64_1[ iim[Position_Key::LINE_ONE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_TWO_FRONT)) {
				id = it->int64_1[ iim[Position_Key::LINE_TWO_FRONT] ];
			} else if (iim.count(Position_Key::LINE_TWO_BEHIND)) {
				id = it->int64_1[ iim[Position_Key::LINE_TWO_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_THREE_FRONT)) {
				id = it->int64_1[ iim[Position_Key::LINE_THREE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_THREE_BEHIND)) {
				id = it->int64_1[ iim[Position_Key::LINE_THREE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
		}
		// cur_wave
		if (bs->monster_battle_info().cur_wave > 0 && (bs->monster_battle_info().cur_wave <= (int)bs->monster_battle_info().wave_vec.size())) {
			const Monster_Wave_Info &mwi = bs->monster_battle_info().wave_vec[bs->monster_battle_info().cur_wave - 1];
			int8_t idx = 0;
			Int_Int_Map iim;
			for (std::vector<int8_t>::const_iterator it_pv = mwi.pos_vec.begin(); it_pv != mwi.pos_vec.end(); ++it_pv, ++idx) {
				if (!win_monster_pos.count(*it_pv)) {
					continue;
				}
				iim.insert(std::make_pair(*it_pv, idx));
			}
			int64_t id = -1;
			if (iim.count(Position_Key::LINE_ONE_FRONT)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_ONE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_ONE_BEHIND)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_ONE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_TWO_FRONT)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_TWO_FRONT] ];
			} else if (iim.count(Position_Key::LINE_TWO_BEHIND)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_TWO_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
			if (iim.count(Position_Key::LINE_THREE_FRONT)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_THREE_FRONT] ];
			} else if (iim.count(Position_Key::LINE_THREE_BEHIND)) {
				id = mwi.int64_1[ iim[Position_Key::LINE_THREE_BEHIND] ];
			}
			if (id > 0) {
				ii[ id ] += 1;
				id = -1;
			}
		}
		// -- wave end

		for (Role_Int_Map::const_iterator it = ii.begin(); it != ii.end(); ++it) {
			Soldier_Map::const_iterator it_am = sh->info.defending_map.find(it->first);
			if (it_am != sh->info.defending_map.end()) {
				if (it_am->second->type != EST_PLAYER) {
					it_am->second->nums += it->second;
				} else {
					++it_am->second->nums;
				}
				it_am->second->nums_battle = 0;
				sh->info.defend_list.push_front(it_am->second);
				join_in_looking_defend_on_monster_front(sh, it_am->second);
				sh->info.defending_map.erase(it_am);
//			} else {
//				MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find defending_map(%ld) on battle_finish.", sh->point_id, *it_iv);
			}
		}

		for (Int_Vec::const_iterator it_iv = ot.iv_2.begin();
				it_iv != ot.iv_2.end(); ++it_iv) {
			Soldier_Map::const_iterator it_am = sh->info.defending_map.find(*it_iv);
			if (it_am != sh->info.defending_map.end()) {
				++it_am->second->nums;
				it_am->second->nums_battle = 0;
				sh->info.defend_list.push_front(it_am->second);
				join_in_looking_defend_on_monster_front(sh, it_am->second);
				sh->info.defending_map.erase(it_am);
//			} else {
//				MSG_TRACE_ABORT("ERROR: expedition point(%d) cant find defending_map(%ld) on battle_finish.", sh->point_id, *it_iv);
			}
		}

		// empty check
		for (Soldier_Map::iterator it_am = sh->info.defending_map.begin();
				it_am != sh->info.defending_map.end(); ++it_am) {
			++it_am->second->nums;
			it_am->second->nums_battle = 0;
			if (0 == sh->info.defend_map.count(it_am->first)) {
				sh->info.defend_list.remove(it_am->second);
				POOL_MANAGER->push_soldier_pool(it_am->second);
				continue;//sh->info.defend_map.insert(*it_am);
			}
			sh->info.defend_list.push_front(it_am->second);
//			MSG_TRACE_ABORT("ERROR: expedition point(%d) cant handle defending_map(%ld) on battle_finish.", sh->point_id, it_am->first);
		}
		sh->info.defending_map.clear();

		for (Soldier_Map::iterator it_am = sh->info.attacking_map.begin();
				it_am != sh->info.attacking_map.end(); ++it_am) {
			if (it_am->second->exit) {
				sh->info.attack_list.remove(it_am->second);
				POOL_MANAGER->push_soldier_pool(it_am->second);
				sh->info.attack_map.erase(it_am->first);
				continue;
			}
			if (it_am->second->nums == 0) {
				sh->info.attack_list.remove(it_am->second);
				POOL_MANAGER->push_soldier_pool(it_am->second);
				sh->info.attack_map.erase(it_am->first);
				if (EST_PLAYER == it_am->second->type) {
					//revive
				}
			} else {
				if (0 == sh->info.attack_map.count(it_am->first)) {
					sh->info.attack_list.remove(it_am->second);
					POOL_MANAGER->push_soldier_pool(it_am->second);
					continue;//sh->info.attack_map.insert(*it_am);
				}
				sh->info.attack_list.push_front(it_am->second);
			}
		}
		sh->info.attacking_map.clear();
	}
	// -- pos end

	int gap = 1;
	const Expedition_Common_Cfg *conf =
							CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::FIGHT_GAP);
	if (conf) {
		gap = conf->value < 1 ? 1 : (int)conf->value;
	}
	sh->info.battle_finish_time = Time_Value::gettimeofday() + Time_Value(gap);

	active_player_battle_state(win_mover_vec, sh->point_id, 0);
	active_player_battle_state(lose_mover_vec, sh->point_id, 0);
}

void Expedition_Scene::hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums) {
	if (!player || (prop.empty() && kill_monster_nums == 0)) {
		return;
	}
//	Cumulate_Award_Info::iterator it = cumulate_award_.find(player->role_id());
//	if (it != cumulate_award_.end()) {
//		for (Int_Int_Map::const_iterator it_prop = prop.begin(); it_prop != prop.end(); ++it_prop) {
//			it->second[it_prop->first] += it_prop->second;
//		}
//	} else {
//		if (!cumulate_award_.insert(std::make_pair(player->role_id(), prop)).second) {
//			return;
//		}
//	}
//	{
//		Block_Buffer buf;
//		buf.make_message(INNER_SYNC_TEAM_CUMULATE_AWARD);
//		//
////		Team_Fb_Scene_Type   	= 80204, 	   /// 组队推图副本
////		HERO_DREAM_Scene_Type   = 80205, 	   /// 英雄梦境副本
//		buf.write_int32(scene_config_->type);
//		buf.write_int64(start_time_.sec());
////		start_time_.serialize(buf);
//		buf.write_uint16(prop.size());
//		for (Int_Int_Map::const_iterator it = prop.begin(); it != prop.end(); ++it) {
//			buf.write_int32(it->first);
//			buf.write_int32(it->second);
//		}
//		buf.finish_message();
//		player->send_to_logic(buf);
//	}
//	Role_Int_Map::iterator it_kn = kill_nums_.find(player->role_id());
//	if (it_kn != kill_nums_.end()) {
//		it_kn->second += kill_monster_nums;
//	} else {
//		kill_nums_.insert(std::make_pair(player->role_id(), kill_monster_nums));
//	}
//	active_cumulate_award(player);
	return;
}

void Expedition_Scene::hook_monster_point_clear(const int point_id) {

}

void Expedition_Scene::create_scene_map_data(uint32_t line, Expedition_Scene_Manager *manager) {
	cur_line_ = line;
	manager_ = manager;
	const ExpeditionMapPointCfg_Map &conf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_map();
	for(ExpeditionMapPointCfg_Map::const_iterator it = conf.begin(); it != conf.end(); ++it) {
		Stronghold sh;
		sh.make_from_cfg(it->second);
		map_data_.insert(std::make_pair(sh.point_id, sh));
		if(sh.type == EBT_GANG_BASE) {
			++map_base_point_num_;
		}
//		Stronghold *sh_p = find_map_data_by_point_id(it->first);
//		if (sh_p) {
//			if (sh_p->type == EBT_MONSTER_REFRESH_TIMING) {
//				monster_data_refresh_timing_.insert(std::make_pair(it->first, sh_p));
//			} else if (sh_p->type == EBT_MONSTER_REFRESH_KILL_GAP) {
//				monster_data_refresh_kill_gap_.insert(std::make_pair(it->first, sh_p));
//			}
//		}
	}

	for (Int_IntHashSet_Map::const_iterator it_btc = CONFIG_CACHE_EXPEDITION->base_territorys_conf().begin();
			it_btc != CONFIG_CACHE_EXPEDITION->base_territorys_conf().end(); ++it_btc) {
		base_territorys_.insert(std::make_pair(it_btc->first, Int_Hash_Set()));
		for (Int_Hash_Set::const_iterator it_ihs = it_btc->second.begin();
				it_ihs != it_btc->second.end(); ++it_ihs) {
			base_territorys_[it_btc->first].insert(*it_ihs);
		}
	}
}

void Expedition_Scene::send_strong_hold_data_to_client(Scene_Player *player) {
	if(!player){
		return;
	}
//	if (player->expedition_scene_info().is_enter_scene) {
		active_map_data(player);
//	}
}


// ------------------ msg start ---------------------------//
int Expedition_Scene::req_expedition_daily_award_get(Scene_Player *pl, const MSG_10201300 &msg) {
	if (!pl) {
		return -1;
	}
	MSG_50201300 res_msg;
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	if (manager_) {
		Time_Value *open_time = manager_->find_open_time_by_uuid(pl->monitor_link().logic_muid.uuid);
		if (open_time) {
			Date_Time dt_opentime(*open_time);
			if (now < *open_time
					|| (dt.year() == dt_opentime.year() && dt.month() == dt_opentime.month() && dt.day() == dt_opentime.day() && dt.hour() < 22)) {
				res_msg.result = 0;
				OBJ_SEND_TO_CLIENT(res_msg, (*pl));
			}
		}
	}
	Date_Time dt_aw(pl->expedition_detail().award_time);
	Date_Time dt_last_10(now);
	dt_last_10.day(dt_last_10.day() -1);
	dt_last_10.hour(22);
	dt_last_10.minute(0);
	dt_last_10.second(0);
	dt_last_10.microsec(0);

	Date_Time dt_10(now);
	dt_10.hour(22);
	dt_10.minute(0);
	dt_10.second(0);
	dt_10.microsec(0);

	if (dt_aw.time_sec() < dt_last_10.time_sec()) {
		// not collect catch yesterday 10
//		res_msg.state = 1;
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_PLAYER);
		buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CASTLE);
		buf.write_int8(1); // get

		for (int i = 0; i < 5; ++i) {
			int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
			buf.write_int32(hold_nums);
		}
		buf.write_uint32(cur_line_);

		buf.finish_message();
		pl->send_to_logic(buf);
		return 0;
	} else if (dt_aw.time_sec() < dt_10.time_sec()) {
		if (dt.time_sec() < dt_10.time_sec()) {
			res_msg.result = 0;
		} else {
			// not collect
//			res_msg.state = 1;
			Block_Buffer buf;
			buf.make_message(INNER_EXPEDITION_PLAYER);
			buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CASTLE);
			buf.write_int8(1); // get

			for (int i = 0; i < 5; ++i) {
				int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
				buf.write_int32(hold_nums);
			}
			buf.write_uint32(cur_line_);

			buf.finish_message();
			pl->send_to_logic(buf);
			return 0;
		}
	} else {
		// already catch time
		res_msg.result = 0;
	}

	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::req_expedition_daily_award_info(Scene_Player *pl, const MSG_10201301 &msg) {
	if (!pl) {
		return -1;
	}
	MSG_50201301 res_msg;
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	if (manager_) {
		Time_Value *open_time = manager_->find_open_time_by_uuid(pl->monitor_link().logic_muid.uuid);
		if (open_time) {
			Date_Time dt_opentime(*open_time);
			if (now < *open_time
					|| (dt.year() == dt_opentime.year() && dt.month() == dt_opentime.month() && dt.day() == dt_opentime.day() && dt.hour() < 22)) {
				OBJ_SEND_TO_CLIENT(res_msg, (*pl));
			}
		}
	}

	Date_Time dt_aw(pl->expedition_detail().award_time);
	Date_Time dt_last_10(now);
	dt_last_10.day(dt_last_10.day() -1);
	dt_last_10.hour(22);
	dt_last_10.minute(0);
	dt_last_10.second(0);
	dt_last_10.microsec(0);

	Date_Time dt_10(now);
	dt_10.hour(22);
	dt_10.minute(0);
	dt_10.second(0);
	dt_10.microsec(0);

	double decay = CONFIG_CACHE_EXPEDITION->get_settle_award_decay(cur_line_) / 100.0;

	if (dt_aw.time_sec() < dt_last_10.time_sec()) {
		// not collect catch yesterday 10
//		res_msg.state = 1;
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_PLAYER);
		buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CASTLE);
		buf.write_int8(0); // ask
		buf.finish_message();
		pl->send_to_logic(buf);
		return 0;
	} else if (dt_aw.time_sec() < dt_10.time_sec()) {
		if (dt.time_sec() < dt_10.time_sec()) {
			// alredy and not ready
			res_msg.state = 0;

			int nums_all = 0;
			Id_ItemInfo_Map iiivm;
			const Item_Info_Vec *iiv = 0;
			for (int i = 0; i < 5; ++i) {
				int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
				iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
				if (iiv) {
					for (Item_Info_Vec::const_iterator it = iiv->begin();
							it != iiv->end(); ++it) {
						Item_Info item_info = *it;
						item_info.amount *= hold_nums;
						if (item_info.amount <= 0) {
							continue;
						}
						Id_ItemInfo_Map::iterator it_iiivm = iiivm.find(it->id);
						if (it_iiivm != iiivm.end()) {
							it_iiivm->second.amount += item_info.amount;
						} else {
							iiivm.insert(std::make_pair(it->id, item_info));
						}
						nums_all += it->amount * hold_nums;
					}
				}
			}

			const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
			if (big_award) {
				for (Item_Info_Vec::const_iterator it = big_award->award.begin();
						it != big_award->award.end(); ++it) {
					Item_Info item_info = *it;
					item_info.amount *= big_award->amount;
					if (item_info.amount <= 0) {
						continue;
					}
					Id_ItemInfo_Map::iterator it_iiivm = iiivm.find(it->id);
					if (it_iiivm != iiivm.end()) {
						it_iiivm->second.amount += item_info.amount;
					} else {
						iiivm.insert(std::make_pair(it->id, item_info));
					}
				}
			}
			for (Id_ItemInfo_Map::iterator it = iiivm.begin();
					it != iiivm.end(); ++it) {
				it->second.amount *= decay;
				if (it->second.amount == 0) {
					continue;
				}
				res_msg.award_info.push_back(it->second);
			}
		} else {
			// not collect
//			res_msg.state = 1;
			Block_Buffer buf;
			buf.make_message(INNER_EXPEDITION_PLAYER);
			buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CASTLE);
			buf.write_int8(0); // ask
			buf.finish_message();
			pl->send_to_logic(buf);
			return 0;
		}
	} else {
		// already catch time
		res_msg.state = 2;

		int nums_all = 0;
		Id_ItemInfo_Map iiivm;
		const Item_Info_Vec *iiv = 0;
		for (int i = 0; i < 5; ++i) {
			int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
			iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
			if (iiv) {
				for (Item_Info_Vec::const_iterator it = iiv->begin();
						it != iiv->end(); ++it) {
					Item_Info item_info = *it;
					item_info.amount *= hold_nums;
					if (item_info.amount <= 0) {
						continue;
					}
					Id_ItemInfo_Map::iterator it_iiivm = iiivm.find(it->id);
					if (it_iiivm != iiivm.end()) {
						it_iiivm->second.amount += item_info.amount;
					} else {
						iiivm.insert(std::make_pair(it->id, item_info));
					}
					nums_all += item_info.amount;
				}
			}
		}
		const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
		if (big_award) {
			for (Item_Info_Vec::const_iterator it = big_award->award.begin();
					it != big_award->award.end(); ++it) {
				Item_Info item_info = *it;
				item_info.amount *= big_award->amount;
				if (item_info.amount <= 0) {
					continue;
				}
				Id_ItemInfo_Map::iterator it_iiivm = iiivm.find(it->id);
				if (it_iiivm != iiivm.end()) {
					it_iiivm->second.amount += item_info.amount;
				} else {
					iiivm.insert(std::make_pair(it->id, item_info));
				}
			}
		}

		for (Id_ItemInfo_Map::iterator it = iiivm.begin();
				it != iiivm.end(); ++it) {
			it->second.amount *= decay;
			if (it->second.amount == 0) {
				continue;
			}
			res_msg.award_info.push_back(it->second);
		}
	}

	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::req_expedition_other_build_info(Scene_Player *pl, const MSG_10201302 &msg) {
	if (!pl) {
		return -1;
	}
//	init_defend_for_beast(pl, msg.point_id);
	MSG_50201302 res_msg;
	res_msg.point_id = msg.point_id;
	res_msg.line = msg.line;
	Expedition_Scene *es = this;
	if (msg.line != (int)cur_line_ && manager_) {
		if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(manager_->get_inter())) {
			Move_Scene *scene = manager_->find_scene_by_cur_line(pl->monitor_link().logic_muid.uuid, msg.line);
			if (scene) {
				es = (Expedition_Scene*) scene;
			}
		} else {
			Move_Scene *scene = manager_->find_scene_by_cur_line(msg.line);
			if (scene) {
				es = (Expedition_Scene*) scene;
			}
		}
	}

	Stronghold *sh = es->find_map_data_by_point_id(msg.point_id);
	if (sh) {
		Expedition_Build_Brief_Info ebbi;
		set_msg_by_type(*pl, *sh, ebbi);
		res_msg.collect = ebbi.val3;
		res_msg.state = sh->info.get_status();
		res_msg.type = (int8_t)sh->type;
		if (manager_) {
			const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(sh->info.occupier.gang_id);
			if (egri) {
				res_msg.defend_name = egri->gang_name;
			}
			egri = manager_->find_gang_rank_info_by_gang_id(sh->info.attacker.gang_id);
			if (egri) {
				res_msg.attack_name = egri->gang_name;
			}
		}
		Expedition_Defend_Info ebri;
		Soldier_Map *sm = &(sh->info.defend_map);
		res_msg.attack_nums = sh->info.attack_map.size();
		if (EBT_BEAST == sh->type) {
			if (sh->info.role_defend_map_for_beast.count(pl->role_id())) {
				sm = &(sh->info.role_defend_map_for_beast[pl->role_id()].defend_map);
			} else {
				sm = 0;
			}
		}

		if (sm) {
			res_msg.defend_nums = sm->size();
			for (Soldier_Map::const_iterator it = sm->begin();
					it != sm->end(); ++it) {
				ebri.reset();
				ebri.role_id = it->second->role_id;
				ebri.name = it->second->role_name;
				ebri.level = it->second->level;
				ebri.force = it->second->force;
				ebri.point_id = msg.point_id;
				res_msg.defend_info.push_back(ebri);
			}
		}
	}

	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}
int Expedition_Scene::req_expedition_gang_rank(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}
	MSG_50201303 res_msg;
	if (manager_) {//xxxxx
		const Expedition_Gang_Rank_Info_Vec *egriv = manager_->gang_rank(pl->monitor_link().logic_muid.uuid);
		if (egriv) {
			for (Expedition_Gang_Rank_Info_Vec::const_iterator it = egriv->begin();
					it != egriv->end(); ++it) {
				res_msg.rank_info.push_back(**it);
			}
		}
		res_msg.time = (int)manager_->last_reset_time().sec();
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}
int Expedition_Scene::req_expedition_battle_info(Scene_Player *pl, const MSG_10201304 &msg) {
	if (!pl) {
		return -1;
	}

	Expedition_Scene *es = this;
//	if (msg.line != (int)cur_line_) {
//		if (manager_) {
//			if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(manager_->get_inter())) {
//				Move_Scene *scene = manager_->find_scene_by_cur_line(pl->monitor_link().logic_muid.uuid, cur_line_);
//				if (scene) {
//					es = (Expedition_Scene*) scene;
//				}
//			} else {
//				Move_Scene *scene = manager_->find_scene_by_cur_line(cur_line_);
//				if (scene) {
//					es = (Expedition_Scene*) scene;
//				}
//			}
//		}
//	} else {
		join_in_looking(pl, msg.point_id);
//	}

	MSG_50201304 res_msg;
	res_msg.point_id = msg.point_id;
	Stronghold *sh = es->find_map_data_by_point_id(msg.point_id);
	if (sh) {
		if (EBT_BEAST == sh->type) {
			return req_expedition_battle_info_for_beast(pl, sh);
		}
		int i = 0;
		Expedition_Battle_Role_Info ebri;
		GangID_SoldierList_Map::iterator it_ll = sh->info.looking_attack_list.find(pl->ganger_detail().gang_id);
		if (it_ll != sh->info.looking_attack_list.end()) {
			for (Soldier_List::const_iterator it = it_ll->second.begin();
					it != it_ll->second.end() && i < LOOKING_PLAYER_MAX; ++it) {
				ebri.reset();
				ebri.role_id = (**it).role_id;
				ebri.type = (**it).type;
				ebri.head_photo_id = (**it).head_id;
				ebri.name = (**it).role_name;
				ebri.level = (**it).level;
				ebri.force = (**it).force;
				ebri.monster_num = 1;
				if (ebri.type != EST_PLAYER) {
					ebri.hero_info.type = 1;
					ebri.hero_info.facade_id = (**it).head_id;
					ebri.hero_info.name = (**it).role_name;
					ebri.hero_info.level = (**it).level;
					ebri.hero_info.force = (**it).force;
					ebri.force += ebri.hero_info.force;
				} else {
					Hero *hero = SCENE_PUBLIC->find_fight_hero(ebri.role_id);
					if (hero) {
						ebri.hero_info.type = 0;
						ebri.hero_info.facade_id = hero->hero_base_detail().hero_facade_id;
						ebri.hero_info.name = hero->hero_base_detail().hero_name;
						ebri.hero_info.level = hero->hero_base_detail().hero_level;
						ebri.hero_info.force = hero->fighter_detail().force;
						(**it).hero_info = ebri.hero_info;
					} else {
						ebri.hero_info = (**it).hero_info;
					}
					Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(ebri.role_id);
					if (pl) {
						ebri.force = pl->fighter_detail().ttl_force;
						(**it).force = ebri.force;
					}
				}
				res_msg.attack_list.push_back(ebri);
				++i;
			}
		}
		i = 0;
		for (Soldier_List::iterator it = sh->info.looking_defend_list.begin();
				it != sh->info.looking_defend_list.end() && i < LOOKING_PLAYER_MAX; ++it) {
			if ((**it).type != EST_PLAYER) {
				for (int j = 0; j < (**it).nums && i < LOOKING_PLAYER_MAX; ++j) {
					ebri.reset();
					ebri.role_id = (**it).role_id;
					ebri.type = (**it).type;
					ebri.head_photo_id = (**it).head_id;
					ebri.name = (**it).role_name;
					ebri.level = (**it).level;
					ebri.force = (**it).force;
					ebri.monster_num = 1;
					ebri.hero_info = (**it).hero_info;
					res_msg.defend_list.push_back(ebri);
					++i;
				}
			} else {
				ebri.reset();
				ebri.role_id = (**it).role_id;
				ebri.type = (**it).type;
				ebri.head_photo_id = (**it).head_id;
				ebri.name = (**it).role_name;
				ebri.level = (**it).level;
				ebri.force = (**it).force;
				ebri.monster_num = (**it).nums;

				Hero *hero = SCENE_PUBLIC->find_fight_hero(ebri.role_id);
				if (hero) {
					ebri.hero_info.type = 0;
					ebri.hero_info.facade_id = hero->hero_base_detail().hero_facade_id;
					ebri.hero_info.name = hero->hero_base_detail().hero_name;
					ebri.hero_info.level = hero->hero_base_detail().hero_level;
					ebri.hero_info.force = hero->fighter_detail().force;
					(**it).hero_info = ebri.hero_info;
				} else {
					ebri.hero_info = (**it).hero_info;
				}
				Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(ebri.role_id);
				if (pl) {
					ebri.force = pl->fighter_detail().ttl_force;
					(**it).force = ebri.force;
				}
				res_msg.defend_list.push_back(ebri);
			}
			++i;
		}
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::req_expedition_battle_info_for_beast(Scene_Player *pl, Stronghold *sh) {
	if (!pl || !sh || EBT_BEAST != sh->type || !sh->info.role_defend_map_for_beast.count(pl->role_id())) {
		return -1;
	}

	MSG_50201304 res_msg;
	res_msg.point_id = sh->point_id;

//	int nums = 1;
//	const Expedition_Common_Cfg *cfg = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(MONSTER_NUMS);
//	if (cfg && cfg->value > 0) {
//		nums = cfg->value;
//	}
	int i = 0;
	Expedition_Battle_Role_Info ebri;
	GangID_SoldierList_Map::iterator it_ll = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.find(pl->ganger_detail().gang_id);
	if (it_ll != sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.end()) {
		for (Soldier_List::iterator it = it_ll->second.begin();
				it != it_ll->second.end() && i < LOOKING_PLAYER_MAX; ++it) {
			ebri.reset();
			ebri.role_id = (**it).role_id;
			ebri.type = (**it).type;
			ebri.head_photo_id = (**it).head_id;
			ebri.name = (**it).role_name;
			ebri.level = (**it).level;
			ebri.force = (**it).force;
			ebri.monster_num = 1;
			if (ebri.type == EST_PLAYER) {
				Hero *hero = SCENE_PUBLIC->find_fight_hero(ebri.role_id);
				if (hero) {
					ebri.hero_info.type = 0;
					ebri.hero_info.facade_id = hero->hero_base_detail().hero_facade_id;
					ebri.hero_info.name = hero->hero_base_detail().hero_name;
					ebri.hero_info.level = hero->hero_base_detail().hero_level;
					ebri.hero_info.force = hero->fighter_detail().force;
					(**it).hero_info = ebri.hero_info;
				} else {
					ebri.hero_info = (**it).hero_info;
				}
				Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(ebri.role_id);
				if (pl) {
					ebri.force = pl->fighter_detail().ttl_force;
					(**it).force = ebri.force;
				}
			}
			res_msg.attack_list.push_back(ebri);
			++i;
		}
	}
	i = 0;
	for (Soldier_List::const_iterator it = sh->info.role_defend_map_for_beast[pl->role_id()].looking_defend_list.begin();
			it != sh->info.role_defend_map_for_beast[pl->role_id()].looking_defend_list.end() && i < LOOKING_PLAYER_MAX; ++it) {
		for (int j = 0; j < (**it).nums && i < LOOKING_PLAYER_MAX; ++j) {
			ebri.reset();
			ebri.role_id = (**it).role_id;
			ebri.type = (**it).type;
			ebri.head_photo_id = (**it).head_id;
			ebri.name = (**it).role_name;
			ebri.level = (**it).level;
			ebri.force = (**it).force;
			ebri.monster_num = 1;
			if (ebri.type != EST_PLAYER) {
				ebri.hero_info = (**it).hero_info;
			}
			res_msg.defend_list.push_back(ebri);
			++i;
		}
	}

	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::req_expedition_gang_base_build_info(Scene_Player *pl, const MSG_10201305 &msg) {
	if (!pl) {
		return -1;
	}
	MSG_50201305 res_msg;
	res_msg.point_id = msg.point_id;
	res_msg.line = msg.line;
	Expedition_Scene *es = this;
	if (msg.line != (int)cur_line_ && manager_) {
		if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(manager_->get_inter())) {
			Move_Scene *scene = manager_->find_scene_by_cur_line(pl->monitor_link().logic_muid.uuid, msg.line);
			if (scene) {
				es = (Expedition_Scene*) scene;
			}
		} else {
			Move_Scene *scene = manager_->find_scene_by_cur_line(msg.line);
			if (scene) {
				es = (Expedition_Scene*) scene;
			}
		}
	}
	Stronghold *sh = es->find_map_data_by_point_id(msg.point_id);
	if (sh) {
		res_msg.base_info.gang_id = sh->info.occupier.gang_id;
		if (manager_) {
			const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(sh->info.occupier.gang_id);
			if (egri) {
				res_msg.base_info.gang_name = egri->gang_name;
			}
		}
		res_msg.base_info.hold_nums = 1;
		const Int_Hash_Set *ihs = find_base_territorys(msg.point_id);
		if (ihs) {
			for (Int_Hash_Set::const_iterator it = ihs->begin(); it != ihs->end(); ++it) {
				Stronghold *sh = es->find_map_data_by_point_id(*it);
				if (!sh || (sh->type != EBT_GANG_BASE
						&& EBT_CHECKPOINT != sh->type
						&& EBT_CASTLE_BIG != sh->type
						&& EBT_CASTLE_MEDIUM != sh->type
						&& EBT_CASTLE_SMALL != sh->type)) {
					continue;
				}
				++res_msg.base_info.hold_nums;
			}
		}
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}
int Expedition_Scene::req_expedition_fighting(Scene_Player *pl, const MSG_10201306 &msg) {
	if (!pl || pl->is_in_battle()) {
		return -1;
	}
	MSG_50201306 res_msg;
	res_msg.point_id = msg.point_id;
	int ret = 0;
	if (msg.type == 0) { // in
		ret = join_in_fighting(pl, msg.point_id);
		if (ret != 1) {
			res_msg.result = 0;
		} else {
			res_msg.result = ret;
		}
		res_msg.state = 0;
		join_in_looking(pl, msg.point_id);
		if (ret == 1) {
			ret = 0;
		}
	} else { // out
		res_msg.result = (0 == drop_out_fighting(pl, msg.point_id)) ? 0 : 1;
		res_msg.state = 0;
		drop_out_looking(pl, msg.point_id);
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return ret;
}
int Expedition_Scene::req_expedition_help_info(Scene_Player *pl, const MSG_10201307 &msg) {
	if (!pl) {
		return -1;
	}
	MSG_50201307 res_msg;

//	int nums = 1;
//	const Expedition_Common_Cfg *cfg = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(MONSTER_NUMS);
//	if (cfg && cfg->value > 0) {
//		nums = cfg->value;
//	}

	res_msg.point_id = msg.point_id;
	Expedition_Help_Info ehi;
	Stronghold *sh = this->find_map_data_by_point_id(msg.point_id);
	if (sh) {
		Soldier_Map *self = 0;
		Soldier_Map *enemy = 0;
		if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
			if (!sh->info.role_defend_map_for_beast.count(pl->role_id())) {
				return -1;
			}
			self = &(sh->info.role_defend_map_for_beast[pl->role_id()].attacking_map);
			enemy = &(sh->info.role_defend_map_for_beast[pl->role_id()].defending_map);
		} else {
			if (sh->info.occupier.gang_id == pl->ganger_detail().gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
				self = &(sh->info.defending_map);
				enemy = &(sh->info.attacking_map);
			} else {
				self = &(sh->info.attacking_map);
				enemy = &(sh->info.defending_map);
			}
		}
		int i = 0;
		for (Soldier_Map::const_iterator it = self->begin();
				it != self->end(); ++it) {
			ehi.reset();
			if (it->second->type != EST_PLAYER) {
				for (uint16_t j = 0; j < it->second->nums_battle; ++j) {
					ehi.index = i + 1;
					ehi.role_name = it->second->role_name;
					ehi.force = it->second->force + it->second->hero_info.force;
					res_msg.self.push_back(ehi);
					++i;
				}
			} else {
				ehi.index = i + 1;
				ehi.role_name = it->second->role_name;
				ehi.force = it->second->force;
//				Hero *hero = SCENE_PUBLIC->find_fight_hero(it->second->role_id);
//				if (hero) {
//					ehi.force += hero->fighter_detail().force;
//				}
				res_msg.self.push_back(ehi);
				++i;
			}
		}
		i = 0;
		for (Soldier_Map::iterator it = enemy->begin();
				it != enemy->end(); ++it) {
			ehi.reset();
			if (it->second->type != EST_PLAYER) {
				for (uint16_t j = 0; j < it->second->nums_battle; ++j) {
					ehi.index = i + 1;
					ehi.role_name = it->second->role_name;
					ehi.force = it->second->force + it->second->hero_info.force;
					res_msg.enemy.push_back(ehi);
					++i;
				}
			} else {
				ehi.index = i + 1;
				ehi.role_name = it->second->role_name;
				ehi.force = it->second->force;
//				Hero *hero = SCENE_PUBLIC->find_fight_hero(it->second->role_id);
//				if (hero) {
//					ehi.force += hero->fighter_detail().force;
//				}
				res_msg.enemy.push_back(ehi);
				++i;
			}
		}
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::active_expedition_help_info(Stronghold *sh) {
	if (!sh) {
		return -1;
	}
	MSG_50201307 res_msg;

//	int nums = 1;
//	const Expedition_Common_Cfg *cfg = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(MONSTER_NUMS);
//	if (cfg && cfg->value > 0) {
//		nums = cfg->value;
//	}

	res_msg.point_id = sh->point_id;
	Expedition_Help_Info ehi;
	if (EBT_BEAST == sh->type) {
		return -1;
	}

	for (Soldier_Map::const_iterator it = sh->info.attacking_map.begin();
			it != sh->info.attacking_map.end(); ++it) {
		if (it->second->type !=EST_PLAYER) {
			continue;
		}
		Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(it->second->role_id);
		if (!pl) {
			continue;
		}
		active_expedition_help_info(pl, sh->point_id);
	}
	for (Soldier_Map::const_iterator it = sh->info.defending_map.begin();
			it != sh->info.defending_map.end(); ++it) {
		if (it->second->type !=EST_PLAYER) {
			continue;
		}
		Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(it->second->role_id);
		if (!pl) {
			continue;
		}
		active_expedition_help_info(pl, sh->point_id);
	}

	return 0;
}

int Expedition_Scene::active_expedition_help_info(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	MSG_50201307 res_msg;

//	int nums = 1;
//	const Expedition_Common_Cfg *cfg = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(MONSTER_NUMS);
//	if (cfg && cfg->value > 0) {
//		nums = cfg->value;
//	}

	res_msg.point_id = point_id;
	Expedition_Help_Info ehi;
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		Soldier_Map *self = 0;
		Soldier_Map *enemy = 0;
		if (sh->info.occupier.gang_id == pl->ganger_detail().gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
			if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
				if (!sh->info.role_defend_map_for_beast.count(pl->role_id())) {
					return -1;
				}
				self = &(sh->info.role_defend_map_for_beast[pl->role_id()].attacking_map);
				enemy = &(sh->info.role_defend_map_for_beast[pl->role_id()].defending_map);
			} else {
				self = &(sh->info.defending_map);
				enemy = &(sh->info.attacking_map);
			}
		} else {
			self = &(sh->info.attacking_map);
			enemy = &(sh->info.defending_map);
		}
		int i = 0;
		for (Soldier_Map::const_iterator it = self->begin();
				it != self->end(); ++it) {
			ehi.reset();
			ehi.index = ++i;
			ehi.role_name = it->second->role_name;
			ehi.force = it->second->force;
			if (it->second->type != EST_PLAYER) {
//				if (it->second->nums == 1) {
					ehi.force *= 2;
//				}
			} else {
				Hero *hero = SCENE_PUBLIC->find_fight_hero(it->second->role_id);
				if (hero) {
					ehi.force += hero->fighter_detail().force;
				}
			}
			res_msg.self.push_back(ehi);
		}
		i = 0;
		for (Soldier_Map::iterator it = enemy->begin();
				it != enemy->end(); ++it) {
			ehi.reset();
			ehi.index = ++i;
			ehi.role_name = it->second->role_name;
			ehi.force = it->second->force;
			if (it->second->type != EST_PLAYER) {
//				if (it->second->nums == 1) {
					ehi.force *= 2;
//				}
			} else {
				Hero *hero = SCENE_PUBLIC->find_fight_hero(it->second->role_id);
				if (hero) {
					ehi.force += hero->fighter_detail().force;
				}
			}
			res_msg.enemy.push_back(ehi);
		}
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::req_expedition_change_line(Scene_Player *pl, const MSG_10201308 &msg) {
	if (!pl) {
		return -1;
	}
	if (msg.to == (int)cur_line_) {
		active_map_data_for_change_line(pl);
	} else if (manager_) {
		if (EXPEDITION_GLOBAL_SCENE_MANAGER->is_first_month(manager_->get_inter())) {
			Move_Scene *scene = manager_->find_scene_by_cur_line(pl->monitor_link().logic_muid.uuid, msg.to);
			if (scene) {
				Expedition_Scene *es = (Expedition_Scene*) scene;
				es->active_map_data_for_change_line(pl);
			}
		} else {
			Move_Scene *scene = manager_->find_scene_by_cur_line(msg.to);
			if (scene) {
				Expedition_Scene *es = (Expedition_Scene*) scene;
				es->active_map_data_for_change_line(pl);
			}
		}
	}
	return 0;
}

int Expedition_Scene::req_expedition_demage_rank(Scene_Player *pl, const MSG_10201309 &msg) {
	if (!pl) {
		return -1;
	}
	MSG_50201309 res_msg;
	Expedition_Role_Rank err;
	int rank = 1;
	for (ExpeditionSceneDemageRankData_Vec::iterator it = demage_vec_.begin();
			it != demage_vec_.end(); ++it) {
		err.reset();
		err.role_name = it->role_name;
		err.gang_name = it->gang_name;
		err.force = it->force;
		err.demage = it->demage;
		const Item_Info_Vec *iiv = CONFIG_CACHE_EXPEDITION->find_demage_rank_award(rank);
		if (iiv) {
			err.award_list = *iiv;
		}
		res_msg.rank.push_back(err);
		++rank;
	}
	res_msg.setttle_time = CONFIG_CACHE_EXPEDITION->get_next_settle_time(settle_time_);
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));

	return 0;
}

int Expedition_Scene::req_expedition_looking(Scene_Player *pl, const MSG_10201310 &msg) {
	if (!pl) {
		return -1;
	}
	MSG_50201310 res_msg;
	res_msg.point_id = msg.point_id;
	int ret = 0;
	if (msg.type == 0) { // in
		ret = join_in_looking(pl, msg.point_id);
		if (ret != 1) {
			res_msg.result = 0;
		} else {
			res_msg.result = ret;
		}
	} else { // out
		drop_out_looking(pl, msg.point_id);
		res_msg.result = 0;
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return ret;
}

int Expedition_Scene::req_expedition_occupy_castle(Scene_Player *pl, const MSG_10201311 &msg) {
	if (!pl) {
		return -1;
	}
	active_occupy_castle_rank(pl);

	return 0;
}

int Expedition_Scene::req_expedition_occupy_castle_award_get(Scene_Player *pl, const MSG_10201312 &msg) {
	if (!pl) {
		return -1;
	}

	Role_ExpeditionOccupyCastleData_Map::iterator it = occupy_castle_rank_.find(pl->role_id());
	if (it != occupy_castle_rank_.end()) {
		int16_t get_nums = 0;
		if (0 == it->second.get_nums) {
			Config_Cache_Expedition::Int_ItemInfoVec_Map::const_iterator it_ocra = CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().begin();
			if (it_ocra != CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().end()) {
				get_nums = it_ocra->first;
			}
		} else {
			for (Config_Cache_Expedition::Int_ItemInfoVec_Map::const_iterator it_ocra = CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().begin();
					it_ocra != CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().end(); ++it_ocra) {
				if (it_ocra->first > it->second.get_nums) {
					get_nums = it_ocra->first;
					break;
				}
			}
		}
		if (get_nums != 0 && it->second.nums >= get_nums) {
			it->second.get_nums = it->second.nums;
			Block_Buffer buf;
			buf.make_message(INNER_EXPEDITION_PLAYER);
			buf.write_uint8(OCCUPY_CASTLE_AWARD_GET);
			buf.write_int64(pl->role_id());
			buf.write_int16(it->second.nums);
			buf.write_int16(get_nums);

			buf.finish_message();
			pl->send_to_logic(buf);
			return 0;
		}
	}

	active_occupy_castle_rank(pl);
	return 0;
}

// ------------------ msg end ---------------------------//


// ------------------ interface start ---------------------------//

int Expedition_Scene::active_map_data(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}
	MSG_81101300 acv_msg;
	acv_msg.line_people = cur_line_;
	if (manager_) {
		acv_msg.line_nums = manager_->line_nums();
		Expedition_Gang_Brief_Info egbi;
		for (Role_Hash_Set::const_iterator it = gang_ids_.begin(); it != gang_ids_.end(); ++it) {
			egbi.reset();
			const Expedition_Gang_Rank_Info *gi = manager_->find_gang_rank_info_by_gang_id(*it);
			if (0 == gi) {
				continue;
			}
			expedition_set_brief_info_from_gang_rank_info(*gi, egbi);
			acv_msg.gang_info.push_back(egbi);
		}

		Expedition_Build_Brief_Info ebbi;
		for(Stronghold_Map::iterator it = map_data_.begin(); it != map_data_.end(); ++it) {
			ebbi.reset();
			it->second.make_to_build_brief_info(ebbi);
			set_msg_by_type(*pl, it->second, ebbi);
			acv_msg.build_info.push_back(ebbi);
		}

		this->active_expedition_daily_award_info(pl);
	}

	OBJ_SEND_TO_CLIENT(acv_msg, (*pl));
	return 0;
}

int Expedition_Scene::active_map_data_by_gang_id(const int64_t gang_id) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self() && it->second->player_self()->ganger_detail().gang_id == gang_id) {
				active_map_data(it->second->player_self());
			}
		}
	}
	return 0;
}

int Expedition_Scene::active_map_all_data(void) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()) {
				active_map_data(it->second->player_self());
			}
		}
	}
	return 0;
}

int Expedition_Scene::active_map_data_for_change_line(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}
	MSG_81101304 acv_msg;
	if (manager_) {
		acv_msg.cur_line = cur_line_;
		Expedition_Gang_Brief_Info egbi;
		for (Role_Hash_Set::const_iterator it = gang_ids_.begin(); it != gang_ids_.end(); ++it) {
			egbi.reset();
			const Expedition_Gang_Rank_Info *gi = manager_->find_gang_rank_info_by_gang_id(*it);
			if (0 == gi) {
				continue;
			}
			expedition_set_brief_info_from_gang_rank_info(*gi, egbi);
			acv_msg.gang_info.push_back(egbi);
		}

		Expedition_Build_Brief_Info ebbi;
		for(Stronghold_Map::iterator it = map_data_.begin(); it != map_data_.end(); ++it) {
			ebbi.reset();
			it->second.make_to_build_brief_info(ebbi);
			set_msg_by_type(*pl, it->second, ebbi);
			acv_msg.build_info.push_back(ebbi);
		}
	}

	OBJ_SEND_TO_CLIENT(acv_msg, (*pl));
	return 0;
}

int Expedition_Scene::active_map_all_data_by_point_id(const Int_Hash_Set points) {
	if (points.empty()) {
		return 0;
	}
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		MSG_81101303 acv_msg;
		Expedition_Build_Brief_Info ebbi;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()) {
				acv_msg.reset();
				for (Int_Hash_Set::const_iterator it_p = points.begin(); it_p != points.end(); ++it_p) {
					Stronghold *sh = find_map_data_by_point_id(*it_p);
					if (!sh) {
						continue;
					}
					sh->make_to_build_brief_info(ebbi);
					set_msg_by_type(*it->second->player_self(), *sh, ebbi);
					acv_msg.build_list.push_back(ebbi);
				}
				OBJ_SEND_TO_CLIENT(acv_msg, (*it->second->player_self()));
			}
		}
	}
	return 0;
}

int Expedition_Scene::active_map_all_data_by_point_id(const int point_id) {
	Stronghold *sh = find_map_data_by_point_id(point_id);
	if (sh) {
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			MSG_81101303 acv_msg;
			Expedition_Build_Brief_Info ebbi;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second->player_self()) {
					acv_msg.reset();
					sh->make_to_build_brief_info(ebbi);
					set_msg_by_type(*it->second->player_self(), *sh, ebbi);
					acv_msg.build_list.push_back(ebbi);
					OBJ_SEND_TO_CLIENT(acv_msg, (*it->second->player_self()));
				}
			}
		}
	}
	return 0;
}

int Expedition_Scene::active_map_data_by_point_id(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	MSG_81101303 acv_msg;
	Stronghold *sh = find_map_data_by_point_id(point_id);
	if (sh) {
		Expedition_Build_Brief_Info ebbi;
		sh->make_to_build_brief_info(ebbi);
		set_msg_by_type(*pl, *sh, ebbi);
		acv_msg.build_list.push_back(ebbi);
		OBJ_SEND_TO_CLIENT(acv_msg, (*pl));
	}
	return 0;
}
