/*
 * Expedition_Scene_Ext.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: linqiyou
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


int Expedition_Scene::update_move_able_info_by_gang_id(const int64_t gang_id) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()
					&& it->second->player_self()->ganger_detail().gang_id == gang_id) {
				update_move_able_info(it->second);
			}
		}
	}

	return 0;
}

int Expedition_Scene::change_move_able_by_gang_id(const int64_t gang_id) {
	Int_Hash_Set *areavalue = find_areavalue_by_gang_id(gang_id);
	if (0 != areavalue) {
		for (Int_Hash_Set::iterator it = areavalue->begin(); it != areavalue->end(); ++it) {
			change_move_value(*it, MOVE_ABLE_FROM);
		}
	}
	return 0;
}

int Expedition_Scene::active_daily_award_by_gang_id(const int64_t gang_id) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()
					&& it->second->player_self()->ganger_detail().gang_id == gang_id) {
				active_expedition_daily_award_info(it->second->player_self());
			}
		}
	}

	return 0;
}

int Expedition_Scene::active_expedition_daily_award_info(Scene_Player *pl, const bool send_un_create) {
	if (!pl) {
		return -1;
	}
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
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

	MSG_81101302 res_msg;
	res_msg.type = 2;
	double decay = CONFIG_CACHE_EXPEDITION->get_settle_award_decay(cur_line_) / 100.0;

	if (award_time_ == Time_Value::zero || send_un_create) {
		res_msg.state = 0;
		res_msg.award_time = dt_10.time_sec() - dt.time_sec();

		int nums_all = 0;
		const Item_Info_Vec *iiv = 0;
		for (int i = 0; i < 5; ++i) {
			int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
			iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
			if (iiv) {
				for (Item_Info_Vec::const_iterator it = iiv->begin();
						it != iiv->end(); ++it) {
					res_msg.time += it->amount * hold_nums;
					nums_all += it->amount * hold_nums;
				}
			}
		}
		const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
		if (big_award) {
			for (Item_Info_Vec::const_iterator it = big_award->award.begin();
					it != big_award->award.end(); ++it) {
				res_msg.time += it->amount * big_award->amount;
			}
		}
		res_msg.time *= decay;
		res_msg.state = 0;
		res_msg.award_time = dt_10.time_sec() - dt.time_sec();
		OBJ_SEND_TO_CLIENT(res_msg, (*pl));
		return 0;
	}

	if (dt_aw.time_sec() < dt_last_10.time_sec()) {
		// not collect catch yesterday 10  had sent
//		Block_Buffer buf;
//		buf.make_message(INNER_EXPEDITION_PLAYER);
//		buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CASTLE);
//		buf.write_int8(2); // ask active
//		buf.finish_message();
//		pl->send_to_logic(buf);
		return 0;
	} else if (dt_aw.time_sec() < dt_10.time_sec()) {
		if (dt.time_sec() < dt_10.time_sec()) {
			// alredy and not ready
			res_msg.state = 0;
			res_msg.award_time = dt_10.time_sec() - dt.time_sec();

			int nums_all = 0;
			const Item_Info_Vec *iiv = 0;
			for (int i = 0; i < 5; ++i) {
				int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
				iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
				if (iiv) {
					for (Item_Info_Vec::const_iterator it = iiv->begin();
							it != iiv->end(); ++it) {
						res_msg.time += it->amount * hold_nums;
						nums_all += it->amount * hold_nums;
					}
				}
			}
			const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
			if (big_award) {
				for (Item_Info_Vec::const_iterator it = big_award->award.begin();
						it != big_award->award.end(); ++it) {
					res_msg.time += it->amount * big_award->amount;
				}
			}
		} else {
			// not collect had sent
//			Block_Buffer buf;
//			buf.make_message(INNER_EXPEDITION_PLAYER);
//			buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_CASTLE);
//			buf.write_int8(0); // ask
//			buf.finish_message();
//			pl->send_to_logic(buf);
			return 0;
		}
	} else {
		// already catch time
		res_msg.state = 2;
		if (dt.time_sec() > dt_10.time_sec()) {
			res_msg.award_time = dt_10.time_sec() - dt.time_sec() + Time_Value::ONE_DAY_IN_SECS;
		} else {
			res_msg.award_time = dt_10.time_sec() - dt.time_sec();
		}

		int nums_all = 0;
		const Item_Info_Vec *iiv = 0;
		for (int i = 0; i < 5; ++i) {
			int hold_nums = get_hold_nums_by_gang_id(pl->ganger_detail().gang_id, i + 1);
			iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
			if (iiv) {
				for (Item_Info_Vec::const_iterator it = iiv->begin();
						it != iiv->end(); ++it) {
					res_msg.time += it->amount * hold_nums;
					nums_all += it->amount * hold_nums;
				}
			}
		}
		const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
		if (big_award) {
			for (Item_Info_Vec::const_iterator it = big_award->award.begin();
					it != big_award->award.end(); ++it) {
				res_msg.time += it->amount * big_award->amount;
			}
		}
	}
	res_msg.time *= decay;

	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Expedition_Scene::change_point_force(const int point_id) {
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		Role_Vec gang_ids;
		bool need_clear_defend = false;

		switch(sh->type) {
		case EBT_GANG_BASE: {
			if (sh->info.defend_map.empty()) {
				need_clear_defend = true;
			}
			break;
		}
		case EBT_CASTLE_BIG:
		case EBT_CASTLE_MEDIUM:
		case EBT_CASTLE_SMALL: {
			if (sh->info.defend_map.empty()) {
				need_clear_defend = true;
			}
			break;
		}
		case EBT_CHECKPOINT: {
			if (sh->info.defend_map.empty()) {
				need_clear_defend = true;
			}
			break;
		}
		case EBT_CHEST:
		case EBT_BEAST: {
			if (sh->info.belong_status == EBBS_NEUTRAL) {
				if (sh->info.defend_map.empty()) {
					need_clear_defend = true;
				}
			} else {
//				create_drop_by_point_id(point_id);
			}
			break;
		}
		case EBT_MONSTER_REFRESH_TIMING: {
			if (sh->info.defend_map.empty()) {
				this->refreshed_monster_timing_.erase(sh->point_id);
				save_tick();
				sync_refresh_monster_timing_kill_to_logic(sh->point_id);
			}
			need_broadcast_point_.insert(sh->point_id);
			break;
		}
		case EBT_MONSTER_REFRESH_KILL_GAP: {
			if (sh->info.defend_map.empty()) {
				int gap = CONFIG_CACHE_EXPEDITION->find_monster_refresh_gap_by_point_id(sh->point_id);
				this->refreshed_monster_kill_gap_[sh->point_id] = Time_Value::gettimeofday() + Time_Value(gap);
				save_tick();
			}
			need_broadcast_point_.insert(sh->point_id);
			break;
		}
		default:
			break;
		}

		sh->info.fight_status = EBFS_PEACE;
		if (need_clear_defend) {
			gang_ids.push_back(sh->info.attacker.gang_id);
			gang_ids.push_back(sh->info.occupier.gang_id);

			change_territorys(sh);

			sh->info.belong_status = EBBS_OCCUPY;
			sh->info.attacker.reset();
		}
		sh->info.battle_finish_time = Time_Value::zero;

		recover_resource(sh->info, need_clear_defend);

		for (Role_Vec::const_iterator it_gi = gang_ids.begin();
				it_gi != gang_ids.end(); ++it_gi) {
			update_move_able_info_by_gang_id(*it_gi);
			active_daily_award_by_gang_id(*it_gi);
			if (need_clear_defend) {
				active_map_data_by_gang_id(*it_gi);
			}
			if (manager_) {
				Expedition_Gang_Rank_Info *gi = manager_->find_gang_rank_info_by_gang_id(*it_gi);
				if (0 == gi) {
					continue;
				}
				gi->asset = get_hold_nums_by_gang_id(gi->gang_id);
				manager_->sort_gang_rank_info_to_vec();
				manager_->make_ranking_num_info_on_vec_for_rank();
			}
		}

		if (need_clear_defend) {
			save_tick();
		}
	}
	return 0;
}

int Expedition_Scene::check_fight_point(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}

	const role_id_t role_id = pl->role_id();
	if (!pl->expedition_scene_info().fight_point.empty()) {
		Int_Hash_Set::iterator it = pl->expedition_scene_info().fight_point.begin();
		if (it != pl->expedition_scene_info().fight_point.end()) {
			Stronghold *sh = this->find_map_data_by_point_id(*it);
			if (!sh) {
				return -1;
			}
			Soldier_Map *attacking_map = 0;
			Soldier_Map *attack_map = 0;
			Soldier_List *attack_list = 0;
			Soldier_Map *defending_map = 0;
			Soldier_Map *defend_map = 0;
			Soldier_List *defend_list = 0;
			if (EBT_BEAST == sh->type) {
				if (sh->info.role_defend_map_for_beast.count(role_id)
					&& EBFS_FIGHT == sh->info.role_defend_map_for_beast[role_id].fight_status) {
					return -1;
				}
				if (sh->info.role_defend_map_for_beast.count(role_id)) {
					attacking_map = &(sh->info.role_defend_map_for_beast[role_id].attacking_map);
					attack_map = &(sh->info.role_defend_map_for_beast[role_id].attack_map);
					attack_list = &(sh->info.role_defend_map_for_beast[role_id].attack_list);
					defending_map = &(sh->info.role_defend_map_for_beast[role_id].defending_map);
					defend_map = &(sh->info.role_defend_map_for_beast[role_id].defend_map);
					defend_list = &(sh->info.role_defend_map_for_beast[role_id].defend_list);
				}
			} else {
				if (EBFS_FIGHT == sh->info.fight_status) {
					return -1;
				}
				attacking_map = &(sh->info.attacking_map);
				attack_map = &(sh->info.attack_map);
				attack_list = &(sh->info.attack_list);
				defending_map = &(sh->info.defending_map);
				defend_map = &(sh->info.defend_map);
				defend_list = &(sh->info.defend_list);
			}

			if (0 != defend_map) {
				if (attacking_map->count(role_id)
						|| defending_map->count(role_id)) {
					return -1;
				}
				if (defend_map->count(role_id)) {
					Soldier_Map::iterator it_dm = defend_map->find(role_id);
					if (it_dm != defend_map->end()) {
						defend_list->remove(it_dm->second);
						POOL_MANAGER->push_soldier_pool(it_dm->second);
						defending_map->erase(it_dm->first);
						defend_map->erase(it_dm);
					}
				} else if (attack_map->count(role_id)) {
					Soldier_Map::iterator it_dm = attack_map->find(role_id);
					if (it_dm != attack_map->end()) {
						attack_list->remove(it_dm->second);
						POOL_MANAGER->push_soldier_pool(it_dm->second);
						attacking_map->erase(it_dm->first);
						attack_map->erase(it_dm);
					}
				}
			}
			pl->expedition_scene_info().fight_point.erase(it);
		}
	}
	return !pl->expedition_scene_info().fight_point.empty();
}

int Expedition_Scene::join_in_fighting(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	int ret = check_fight_point(pl);
	if (0 != ret) {
		return ret;
	}
	const role_id_t role_id = pl->role_id();

	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (!sh) {
		return -1;
	}

	Soldier_Map *map = 0;
	Soldier_List *list = 0;
	bool is_attack = false;
	bool is_defend = false;
	int8_t *fight_status = 0;
	if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
		if (EBT_CHEST == sh->type || pl->expedition_detail().collect_beast.count(sh->point_id)) {
			return -1;
		}

		if (!sh->info.role_defend_map_for_beast.count(role_id)) {
			init_defend_for_beast(pl, point_id);
			if (!sh->info.role_defend_map_for_beast.count(role_id)) {
				return -1;
			}
		}
		map = &(sh->info.role_defend_map_for_beast[role_id].attack_map);
		list = &(sh->info.role_defend_map_for_beast[role_id].attack_list);
		fight_status = &(sh->info.role_defend_map_for_beast[role_id].fight_status);
	} else {
		if (sh->info.fight_status == EBFS_FIGHT
				&& sh->info.occupier.gang_id != pl->ganger_detail().gang_id
				&& sh->info.attacker.gang_id != pl->ganger_detail().gang_id) {
			return ERROR_EXPEDITION_OTHER_ATTACKER;
		}
		if (sh->info.occupier.gang_id == pl->ganger_detail().gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
			map = &(sh->info.defend_map);
			list = &(sh->info.defend_list);
			is_defend = true;
			fight_status = &(sh->info.fight_status);
		} else {
			map = &(sh->info.attack_map);
			list = &(sh->info.attack_list);
			is_attack = true;
			fight_status = &(sh->info.fight_status);
		}
	}

	Soldier_Map::iterator it = map->find(role_id);
	if (it == map->end()) {

		// soldier init --
		Soldier *soldier = POOL_MANAGER->pop_soldier_pool();
		if (!soldier) {
			MSG_TRACE_ABORT("ERROR: cant pop soldier.");
			return -1;
		}
		soldier->id = role_id;//EXPEDITION_GLOBAL_SCENE_MANAGER->make_soldier_id_max();
		soldier->role_id = role_id;
		soldier->role_name = pl->role_name();
		soldier->force = pl->fighter_detail().ttl_force;
		soldier->blood = pl->blood_current();
		soldier->level = pl->level();
		soldier->nums = 1;
		soldier->type = EST_PLAYER;
		soldier->point_id = point_id;
		soldier->head_id = pl->base_detail().head_id;
		soldier->occupier.gang_id = pl->ganger_detail().gang_id;
		if (manager_) {
			const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(soldier->occupier.gang_id);
			if (egri) {
				soldier->occupier.gang_name = egri->gang_name;
				soldier->occupier.server_uuid = egri->uuid;
				soldier->occupier.server_name = egri->server_name;
			} else {
				MSG_TRACE_ABORT("ERROR: expedition role(%ld) gang_id(%ld) point(%d) cant find gang_rank_info in join_fighting.", role_id, soldier->occupier.gang_id, sh->point_id);
				POOL_MANAGER->push_soldier_pool(soldier);
				return 0;
			}
		}
		// -- soldier end

		map->insert(std::make_pair(soldier->id, soldier));
		list->push_back(soldier);
		if (is_attack) {
			sh->info.attacker = soldier->occupier;
		}
	} else {
		it->second->exit = false;
	}

	if (is_attack && sh->info.occupier.gang_id > 0 && EBFS_FIGHT != sh->info.fight_status) {
		Block_Buffer block_buf;
		block_buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
		block_buf.write_uint8(EXPEDITION_CHANNEL_DEFEND_NOTICE);
		block_buf.write_int64(sh->info.occupier.gang_id); // ask
		block_buf.write_int32(sh->point_id);
		std::string gang_name;
		if (manager_) {
			const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(pl->ganger_detail().gang_id);
			if (egri) {
				gang_name = egri->gang_name;
			}
		}
		block_buf.write_string(gang_name);
		std::string castle_name;
		const Expedition_Map_Point_Cfg *cfg = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(sh->point_id);
		if (cfg) {
			castle_name = cfg->name;
		}
		block_buf.write_string(castle_name);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(sh->info.occupier.server_uuid, block_buf);
	}
	if (!is_defend) {
		*fight_status = EBFS_FIGHT;
	}
	if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
		other_fighting_set_.insert(sh);
	} else {
		if (!is_defend) {
			fighting_set_.insert(sh);
		}
	}
	this->active_map_all_data_by_point_id(point_id);

	pl->expedition_scene_info().fight_point.insert(point_id);

	return 1;
}

int Expedition_Scene::drop_out_fighting(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		Soldier_Map *map = 0;
		Soldier_List *list = 0;
		Soldier_Map *maping = 0;
		bool is_beast = false;
		if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
			if (!sh->info.role_defend_map_for_beast.count(pl->role_id())) {
				return 0;
			}
			map = &(sh->info.role_defend_map_for_beast[pl->role_id()].attack_map);
			maping = &(sh->info.role_defend_map_for_beast[pl->role_id()].attacking_map);
			list = &(sh->info.role_defend_map_for_beast[pl->role_id()].attack_list);
			is_beast = true;
		} else {
			if (sh->info.occupier.gang_id == pl->ganger_detail().gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
				map = &(sh->info.defend_map);
				maping = &(sh->info.defending_map);
				list = &(sh->info.defend_list);
			} else {
				map = &(sh->info.attack_map);
				maping = &(sh->info.attacking_map);
				list = &(sh->info.attack_list);
			}
		}

		if (!maping->count(pl->role_id())) {
			Soldier_Map::iterator it = map->find(pl->role_id());
			if (it != map->end()) {
				list->remove(it->second);
				POOL_MANAGER->push_soldier_pool(it->second);
				map->erase(it);
			}
		} else {
			Soldier_Map::iterator it = map->find(pl->role_id());
			if (it != map->end()) {
				it->second->exit = true;
			}
		}

		pl->expedition_scene_info().fight_point.erase(point_id);
	}
	return 0;
}

int Expedition_Scene::exit_drop_out_fighting(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}

	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (!sh || EBT_CHEST == sh->type
			|| (EBT_BEAST != sh->type
					&& sh->info.occupier.gang_id > 0
					&& sh->info.occupier.gang_id == pl->ganger_detail().gang_id
					&& !sh->info.attack_map.empty())) {
		return -2;
	}

	return drop_out_fighting(pl, point_id);
}

int Expedition_Scene::join_in_looking(Scene_Player *pl, const int point_id) {
	if (!pl || pl->expedition_scene_info().look_point.count(point_id)) {
		return -1;
	}
	if (!pl->expedition_scene_info().look_point.empty()) {
		Int_Hash_Set look_point = pl->expedition_scene_info().look_point;
		for (Int_Hash_Set::iterator it = look_point.begin(); it != look_point.end(); ++it) {
			drop_out_looking(pl, *it);
		}
		int ret = check_fight_point(pl);
		if (0 != ret) {
			return ret;
		}
	}

	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		int64_t gang_id = pl->ganger_detail().gang_id;
		GangID_SoldierMap_Map::iterator it_lm;
		GangID_SoldierList_Map::iterator it_ll;
		Soldier_Map *map = 0;
		Soldier_List *list = 0;
		if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
			if (EBT_CHEST == sh->type || pl->expedition_detail().collect_beast.count(sh->point_id)) {
				return -1;
			}
//			init_defend_for_beast(pl, point_id);

			it_lm = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.find(gang_id);
			if (it_lm == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.end()) {
				sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.insert(std::make_pair(gang_id, Soldier_Map()));
				it_lm = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.find(gang_id);
				if (it_lm == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.end()) {
					MSG_TRACE_ABORT("ERROR:cant insert map");
					return -1;
				}
			}
			it_ll = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.find(gang_id);
			if (it_ll == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.end()) {
				sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.insert(std::make_pair(gang_id, Soldier_List()));
				it_ll = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.find(gang_id);
				if (it_ll == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.end()) {
					MSG_TRACE_ABORT("ERROR:cant insert map");
					return -1;
				}
			}
			map = &(it_lm->second);
			list = &(it_ll->second);
		} else {
			if (sh->info.fight_status == EBFS_FIGHT
					&& sh->info.occupier.gang_id != 0
					&& sh->info.occupier.gang_id != gang_id
					&& sh->info.attacker.gang_id != 0
					&& sh->info.attacker.gang_id != gang_id) {
				return ERROR_EXPEDITION_OTHER_ATTACKER;
			}
			if (sh->info.occupier.gang_id == gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
				map = &(sh->info.looking_defend_map);
				list = &(sh->info.looking_defend_list);
			} else {
				it_lm = sh->info.looking_attack_map.find(gang_id);
				if (it_lm == sh->info.looking_attack_map.end()) {
					sh->info.looking_attack_map.insert(std::make_pair(gang_id, Soldier_Map()));
					it_lm = sh->info.looking_attack_map.find(gang_id);
					if (it_lm == sh->info.looking_attack_map.end()) {
						MSG_TRACE_ABORT("ERROR:cant insert map");
						return -1;
					}
				}
				it_ll = sh->info.looking_attack_list.find(gang_id);
				if (it_ll == sh->info.looking_attack_list.end()) {
					sh->info.looking_attack_list.insert(std::make_pair(gang_id, Soldier_List()));
					it_ll = sh->info.looking_attack_list.find(gang_id);
					if (it_ll == sh->info.looking_attack_list.end()) {
						MSG_TRACE_ABORT("ERROR:cant insert map");
						return -1;
					}
				}
				map = &(it_lm->second);
				list = &(it_ll->second);
			}
		}

		Soldier_Map::iterator it = map->find(pl->role_id());
		if (it == map->end()) {

			// soldier init --
			Soldier *soldier = POOL_MANAGER->pop_soldier_pool();
			if (!soldier) {
				MSG_TRACE_ABORT("ERROR: cant pop soldier.");
				return -1;
			}
			soldier->id = pl->role_id();//EXPEDITION_GLOBAL_SCENE_MANAGER->make_soldier_id_max();
			soldier->role_id = pl->role_id();
			soldier->role_name = pl->role_name();
			soldier->force = pl->fighter_detail().ttl_force;
			soldier->blood = pl->blood_current();
			soldier->level = pl->level();
			soldier->nums = 1;
			soldier->type = EST_PLAYER;
			soldier->point_id = point_id;
			soldier->head_id = pl->base_detail().head_id;
			soldier->occupier.gang_id = pl->ganger_detail().gang_id;
			if (manager_) {
				const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(soldier->occupier.gang_id);
				if (egri) {
					soldier->occupier.gang_name = egri->gang_name;
					soldier->occupier.server_uuid = egri->uuid;
					soldier->occupier.server_name = egri->server_name;
				} else {
					MSG_TRACE_ABORT("ERROR: expedition role(%ld) gang_id(%ld) point(%d) cant find gang_rank_info in join_fighting.", pl->role_id(), soldier->occupier.gang_id, sh->point_id);
					POOL_MANAGER->push_soldier_pool(soldier);
					return 0;
				}
			}
			// -- soldier end

			map->insert(std::make_pair(soldier->id, soldier));
			list->push_back(soldier);
		}
		pl->expedition_scene_info().look_point.insert(point_id);
	}
	return 1;
}

int Expedition_Scene::join_in_looking_front(Scene_Player *pl, const int point_id) {
	if (!pl || pl->expedition_scene_info().look_point.count(point_id)) {
		return -1;
	}
	if (!pl->expedition_scene_info().look_point.empty()) {
		Int_Hash_Set look_point = pl->expedition_scene_info().look_point;
		for (Int_Hash_Set::iterator it = look_point.begin(); it != look_point.end(); ++it) {
			drop_out_looking(pl, *it);
		}
		int ret = check_fight_point(pl);
		if (0 != ret) {
			return ret;
		}
	}

	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		int64_t gang_id = pl->ganger_detail().gang_id;
		GangID_SoldierMap_Map::iterator it_lm;
		GangID_SoldierList_Map::iterator it_ll;
		Soldier_Map *map = 0;
		Soldier_List *list = 0;
		if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
			if (EBT_CHEST == sh->type || pl->expedition_detail().collect_beast.count(sh->point_id)) {
				return -1;
			}
//			init_defend_for_beast(pl, point_id);

			it_lm = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.find(gang_id);
			if (it_lm == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.end()) {
				sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.insert(std::make_pair(gang_id, Soldier_Map()));
				it_lm = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.find(gang_id);
				if (it_lm == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.end()) {
					MSG_TRACE_ABORT("ERROR:cant insert map");
					return -1;
				}
			}
			it_ll = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.find(gang_id);
			if (it_ll == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.end()) {
				sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.insert(std::make_pair(gang_id, Soldier_List()));
				it_ll = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.find(gang_id);
				if (it_ll == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.end()) {
					MSG_TRACE_ABORT("ERROR:cant insert map");
					return -1;
				}
			}
			map = &(it_lm->second);
			list = &(it_ll->second);
		} else {
			if (sh->info.fight_status == EBFS_FIGHT
					&& sh->info.occupier.gang_id != 0
					&& sh->info.occupier.gang_id != gang_id
					&& sh->info.attacker.gang_id != 0
					&& sh->info.attacker.gang_id != gang_id) {
				return ERROR_EXPEDITION_OTHER_ATTACKER;
			}
			if (sh->info.occupier.gang_id == gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
				map = &(sh->info.looking_defend_map);
				list = &(sh->info.looking_defend_list);
			} else {
				it_lm = sh->info.looking_attack_map.find(gang_id);
				if (it_lm == sh->info.looking_attack_map.end()) {
					sh->info.looking_attack_map.insert(std::make_pair(gang_id, Soldier_Map()));
					it_lm = sh->info.looking_attack_map.find(gang_id);
					if (it_lm == sh->info.looking_attack_map.end()) {
						MSG_TRACE_ABORT("ERROR:cant insert map");
						return -1;
					}
				}
				it_ll = sh->info.looking_attack_list.find(gang_id);
				if (it_ll == sh->info.looking_attack_list.end()) {
					sh->info.looking_attack_list.insert(std::make_pair(gang_id, Soldier_List()));
					it_ll = sh->info.looking_attack_list.find(gang_id);
					if (it_ll == sh->info.looking_attack_list.end()) {
						MSG_TRACE_ABORT("ERROR:cant insert map");
						return -1;
					}
				}
				map = &(it_lm->second);
				list = &(it_ll->second);
			}
		}

		Soldier_Map::iterator it = map->find(pl->role_id());
		if (it == map->end()) {

			// soldier init --
			Soldier *soldier = POOL_MANAGER->pop_soldier_pool();
			if (!soldier) {
				MSG_TRACE_ABORT("ERROR: cant pop soldier.");
				return -1;
			}
			soldier->id = pl->role_id();//EXPEDITION_GLOBAL_SCENE_MANAGER->make_soldier_id_max();
			soldier->role_id = pl->role_id();
			soldier->role_name = pl->role_name();
			soldier->force = pl->fighter_detail().ttl_force;
			soldier->blood = pl->blood_current();
			soldier->level = pl->level();
			soldier->nums = 1;
			soldier->type = EST_PLAYER;
			soldier->point_id = point_id;
			soldier->head_id = pl->base_detail().head_id;
			soldier->occupier.gang_id = pl->ganger_detail().gang_id;
			if (manager_) {
				const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(soldier->occupier.gang_id);
				if (egri) {
					soldier->occupier.gang_name = egri->gang_name;
					soldier->occupier.server_uuid = egri->uuid;
					soldier->occupier.server_name = egri->server_name;
				} else {
					MSG_TRACE_ABORT("ERROR: expedition role(%ld) gang_id(%ld) point(%d) cant find gang_rank_info in join_fighting.", pl->role_id(), soldier->occupier.gang_id, sh->point_id);
					POOL_MANAGER->push_soldier_pool(soldier);
					return 0;
				}
			}
			// -- soldier end

			map->insert(std::make_pair(soldier->id, soldier));
			list->push_front(soldier);
		}
		pl->expedition_scene_info().look_point.insert(point_id);
	}
	return 1;
}

int Expedition_Scene::drop_out_looking(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		int64_t gang_id = pl->ganger_detail().gang_id;
		GangID_SoldierMap_Map::iterator it_lm;
		GangID_SoldierList_Map::iterator it_ll;
		Soldier_Map *map = 0;
		Soldier_List *list = 0;
		if ((EBT_CHEST == sh->type || EBT_BEAST == sh->type)) {
			it_ll = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.find(gang_id);
			it_lm = sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.find(gang_id);
			if (it_lm == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_map.end()) {
				return 0;
			}
			if (it_ll == sh->info.role_defend_map_for_beast[pl->role_id()].looking_attack_list.end()) {
				Soldier_Map::iterator it = it_lm->second.find(pl->role_id());
				if (it != it_lm->second.end()) {
					POOL_MANAGER->push_soldier_pool(it->second);
					it_lm->second.erase(it);
				}
				return 0;
			}
			map = &(it_lm->second);
			list = &(it_ll->second);
		} else {
			if (sh->info.occupier.gang_id == pl->ganger_detail().gang_id) { // exact player_self()->monitor_link().logic_muid.uuid
				map = &(sh->info.looking_defend_map);
				list = &(sh->info.looking_defend_list);
			} else {
				it_lm = sh->info.looking_attack_map.find(gang_id);
				it_ll = sh->info.looking_attack_list.find(gang_id);
				if (it_lm == sh->info.looking_attack_map.end()) {
					sh->info.looking_attack_list.clear();
					return 0;
				}
				if (it_ll == sh->info.looking_attack_list.end()) {
					Soldier_Map::iterator it = it_lm->second.find(pl->role_id());
					if (it != it_lm->second.end()) {
						POOL_MANAGER->push_soldier_pool(it->second);
						it_lm->second.erase(it);
					}
					return 0;
				}
				map = &(it_lm->second);
				list = &(it_ll->second);
			}
		}

		Soldier_Map::iterator it = map->find(pl->role_id());
		if (it != map->end()) {
			POOL_MANAGER->push_soldier_pool(it->second);
			list->remove(it->second);
			map->erase(it);
		}
		pl->expedition_scene_info().look_point.erase(point_id);
	}
	return 0;
}

int Expedition_Scene::exit_drop_out_looking(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (!sh || EBT_CHEST == sh->type
			|| (EBT_BEAST != sh->type
					&& sh->info.occupier.gang_id > 0
					&& sh->info.occupier.gang_id == pl->ganger_detail().gang_id
					&& !sh->info.attack_map.empty())) {
		return -2;
	}

	return drop_out_looking(pl, point_id);
}

int Expedition_Scene::init_defend_for_beast(Scene_Player *pl, const int point_id) {
	if (!pl) {
		return -1;
	}
	if (pl->expedition_detail().collect_beast.count(point_id)) {
		return -1;
	}

	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	const Expedition_Map_Point_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
	if (!sh || (sh->info.role_defend_map_for_beast.count(pl->role_id())
			&& (sh->info.role_defend_map_for_beast[pl->role_id()].has_complete || !sh->info.role_defend_map_for_beast[pl->role_id()].defend_map.empty()))
			|| !conf || conf->type != EBT_BEAST
			|| EBT_BEAST != sh->type
//			|| EBBS_OCCUPY != sh->info.belong_status
//			|| sh->info.occupier.gang_id <= 0
			) {
//		MSG_USER("sh->info.role_defend_map_for_beast[pl->role_id()].has_complete:%d", sh->info.role_defend_map_for_beast[pl->role_id()].has_complete);
		return -1;
	}

	for (Int_Int_Map::const_iterator it = conf->monster_group.begin();
			it != conf->monster_group.end(); ++it) {
		Soldier *soldier = POOL_MANAGER->pop_soldier_pool();
		if (!soldier) {
			MSG_TRACE_ABORT("ERROR: cant pop soldier!");
			continue;
		}
		soldier->id = EXPEDITION_GLOBAL_SCENE_MANAGER->make_soldier_id_max();
		soldier->point_id = point_id;
		soldier->role_id = it->first;
		soldier->nums = it->second;
		soldier->type = EST_MONSTER;
		if (soldier->nums == 0) {
			MSG_TRACE_ABORT("ERROR: expedition point_id(%d) monster_gfp roup(%d) nums = 0.", point_id, it->first);
			continue;
		}

		bool need_continue = true;
		Monster_Section_Config* section_config = CONFIG_CACHE_NPC->get_monster_section_config(EXPEDITION_INNER_SCENE_ID);
		if (section_config) {
			Monster_Group_Config_Map::iterator it_gm = section_config->group_map.find(it->first);
			if (it_gm != section_config->group_map.end()) {
				if (!it_gm->second.monster.empty()) {
					//  find monster cfg start ---
					const NPC_Config_Cache* ncc = CONFIG_CACHE_NPC->find_npc_config_cache(it_gm->second.monster[0]);
					if (!ncc) {
						MSG_TRACE_ABORT("ERROR: cant find monster(%d) from monster conf!", it->first);
						continue;
					}
					soldier->role_name = ncc->name;
					soldier->level = ncc->level;
					soldier->head_id = ncc->id;
					for (std::vector<Int_Double>::const_iterator it_pvv = ncc->prop_value_vec.begin();
							it_pvv != ncc->prop_value_vec.end(); ++it_pvv) {
						if ((*it_pvv).id == PT_BLOOD_CURRENT) {
							soldier->blood = (int)(*it_pvv).value;
							break;
						}
					}
					// -- find monster cfg end
					// force
					soldier->force = CONFIG_CACHE_EXPEDITION->find_monster_group_force(it->first);
					if (it_gm->second.monster.size() > 1) {
						ncc = CONFIG_CACHE_NPC->find_npc_config_cache(it_gm->second.monster[1]);
						if (!ncc) {
							MSG_TRACE_ABORT("ERROR: cant find monster(%d) from monster conf 2!", it->first);
							continue;
						}
						soldier->role_name = ncc->name;
						soldier->level = ncc->level;
						soldier->head_id = ncc->id;
						for (std::vector<Int_Double>::const_iterator it_pvv = ncc->prop_value_vec.begin();
								it_pvv != ncc->prop_value_vec.end(); ++it_pvv) {
							if ((*it_pvv).id == PT_BLOOD_CURRENT) {
								Int_Double id = *it_pvv;
								modify_prop_for_monster_modify(id);
								soldier->blood = (int)id.value;
								break;
							}
						}
						soldier->hero_info.type = 1;
						soldier->hero_info.facade_id = ncc->id;
						soldier->hero_info.name = ncc->name;
						soldier->hero_info.level = ncc->level;
						soldier->hero_info.force = soldier->force;
					}
					need_continue = false;
				}
			}
		}
		if (need_continue) {
			MSG_TRACE_ABORT("ERROR: Expedition group_id(%d) error.", it->first);
			continue;
		}
		sh->info.role_defend_map_for_beast[pl->role_id()].defend_map.insert(std::make_pair(soldier->id, soldier));
		sh->info.role_defend_map_for_beast[pl->role_id()].defend_list.push_back(soldier);
		Soldier *soldier_look = POOL_MANAGER->pop_soldier_pool();
		if (!soldier_look) {
			MSG_TRACE_ABORT("ERROR: cant pop soldier!");
			continue;
		}
		*soldier_look = *soldier;
		sh->info.role_defend_map_for_beast[pl->role_id()].looking_defend_map.insert(std::make_pair(soldier_look->id, soldier_look));
		sh->info.role_defend_map_for_beast[pl->role_id()].looking_defend_list.push_back(soldier_look);
	}
	return 0;
}

int Expedition_Scene::recover_defend_for_beast(Scene_Player *pl, Stronghold &sh) {
	if (!pl || EBT_BEAST != sh.type
			|| !sh.info.role_defend_map_for_beast.count(pl->role_id())) {
		return -1;
	}

	Role_ExpeditionFightInfo_Map::iterator it_info = sh.info.role_defend_map_for_beast.find(pl->role_id());
	if (it_info != sh.info.role_defend_map_for_beast.end()) {
		if (!it_info->second.attack_map.empty()) {
			return -1;
		}
		for (Soldier_Map::iterator it = it_info->second.defend_map.begin();
				it != it_info->second.defend_map.end(); ++it) {
			POOL_MANAGER->push_soldier_pool(it->second);
		}
		it_info->second.defend_map.clear();
		it_info->second.defend_list.clear();
		it_info->second.defending_map.clear();
		for (Soldier_Map::iterator it = it_info->second.attack_map.begin();
				it != it_info->second.attack_map.end(); ++it) {
			POOL_MANAGER->push_soldier_pool(it->second);
		}
		it_info->second.attack_map.clear();
		it_info->second.attack_list.clear();
		it_info->second.attacking_map.clear();
		sh.info.role_defend_map_for_beast.erase(it_info);
	}
	return 0;
}

int Expedition_Scene::enter_battle(Stronghold* sh) {
	if (!sh) {
		return -1;
	}
	Soldier *soldier_a = sh->info.attack_list.front();
	Soldier *soldier_d = sh->info.defend_list.front();
	bool need_return = false, need_return_b = false;
	if (!soldier_a) {
		sh->info.attack_list.pop_front();
//		sh->info.attack_map.erase(soldier_a->id);
//		POOL_MANAGER->push_soldier_pool(soldier_a);
		need_return = true;
	}
	if (soldier_a->nums <= 0) {
		if (!need_return) {
			sh->info.attack_list.pop_front();
		}
		sh->info.attack_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		need_return = true;
	}
	if (!soldier_d) {
		sh->info.defend_list.pop_front();
//		sh->info.defend_map.erase(soldier_d->id);
//		POOL_MANAGER->push_soldier_pool(soldier_d);
		need_return_b = true;
	}
	if (soldier_d->nums <= 0) {
		if (!need_return) {
			sh->info.defend_list.pop_front();
		}
		sh->info.defend_map.erase(soldier_d->id);
		POOL_MANAGER->push_soldier_pool(soldier_d);
		need_return_b = true;
	}
	if (need_return || need_return_b) {
		return -1;
	}
	int ret = enter_battle(soldier_a, soldier_d, sh->point_id);
	switch(ret) {
	case 0: {
		sh->info.attacking_map.insert(std::make_pair(soldier_a->id, soldier_a));
		sh->info.defending_map.insert(std::make_pair(soldier_d->id, soldier_d));
		--soldier_a->nums;
		--soldier_d->nums;
		sh->info.attack_list.pop_front();
		sh->info.defend_list.pop_front();
		MSG_USER("enter_battle.");
		Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_a->id);
		if (pl_a) {
			active_player_battle_state(pl_a, sh->point_id, 1);
			drop_out_looking(pl_a, sh->point_id);
		}
		Scene_Player *pl_d = SCENE_MONITOR->find_scene_player_by_role_id(soldier_d->id);
		if (pl_d) {
			active_player_battle_state(pl_d, sh->point_id, 1);
			drop_out_looking(pl_d, sh->point_id);
		}
		drop_out_looking_defend_on_monster(sh, soldier_d);
		break;
	}
	case -1: {
		sh->info.defending_map.insert(std::make_pair(soldier_d->id, soldier_d));
		--soldier_a->nums;
		--soldier_d->nums;
		sh->info.attack_list.pop_front();
		sh->info.defend_list.pop_front();
		sh->info.attack_map.erase(soldier_a->id);
		Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_a->id);
		if (pl_a) {
			drop_out_looking(pl_a, sh->point_id);
		}
		POOL_MANAGER->push_soldier_pool(soldier_a);
		Scene_Player *pl_d = SCENE_MONITOR->find_scene_player_by_role_id(soldier_d->id);
		if (pl_d) {
			drop_out_looking(pl_d, sh->point_id);
		}
		MSG_USER("attack offline.");
		break;
	}
	case -2: {
		sh->info.attacking_map.insert(std::make_pair(soldier_a->id, soldier_a));
		--soldier_a->nums;
		--soldier_d->nums;
		sh->info.attack_list.pop_front();
		sh->info.defend_list.pop_front();
		sh->info.defend_map.erase(soldier_d->id);
		Scene_Player *pl_d = SCENE_MONITOR->find_scene_player_by_role_id(soldier_d->id);
		if (pl_d) {
			drop_out_looking(pl_d, sh->point_id);
		}
		POOL_MANAGER->push_soldier_pool(soldier_d);
		MSG_USER("defend offline.");
		Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_a->id);
		if (pl_a) {
			drop_out_looking(pl_a, sh->point_id);
		}
		break;
	}
	case -3: {
		break;
	}
	case -4: {
		MSG_USER("ERROR: expedition player(%ld) is in battle.", soldier_a->id);
//		sh->info.defending_map.insert(std::make_pair(soldier_d->id, soldier_d));
//		--soldier_a->nums;
//		--soldier_d->nums;
//		sh->info.attack_list.pop_front();
//		sh->info.defend_list.pop_front();
//		sh->info.attack_map.erase(soldier_a->id);
//		POOL_MANAGER->push_soldier_pool(soldier_a);
		break;
	}
	default:
		break;
	}

//	if (0 == soldier_d->nums) {
//		drop_out_looking_defend_on_monster(sh, soldier_d);
//	}
	return ret;
}

int Expedition_Scene::enter_battle_more_attack(Stronghold* sh) {
	if (!sh) {
		return -1;
	}
	Soldier *soldier_a = sh->info.attack_list.front();
	if (!soldier_a) {
		sh->info.attack_list.pop_front();
//		sh->info.attack_map.erase(soldier_a->id);
//		POOL_MANAGER->push_soldier_pool(soldier_a);
		return -1;
	}
	if (soldier_a->nums <= 0) {
		sh->info.attack_list.pop_front();
		sh->info.attack_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		return -1;
	}
	if (soldier_a->type != EST_PLAYER) {
		return -1;
	}
	Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_a->id);
	if (!pl_a) {
		--soldier_a->nums;
		sh->info.attacking_map.erase(soldier_a->id);
		sh->info.attack_list.pop_front();
		sh->info.attack_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		MSG_USER("attack offline.");
		return -1;
	}
	if (pl_a->is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}
	if (pl_a->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	Soldier_Map::iterator it = sh->info.attacking_map.begin();
	if (it != sh->info.attacking_map.end()) {
		// player only
		Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(it->second->role_id);
		if (pl && pl->battle_scene()) {
			Battle_Scene *bs = pl->battle_scene();

			int pos = -1;
			for (Battle_Scene::Debug_Mover_Map::const_iterator it = bs->pos_debug_map().begin();
					it != bs->pos_debug_map().end(); ++it) {
				if (it->second == pl->role_id()) {
					pos = it->first.pos;
					break;
				}
			}

			if (pos == -1) {
				return -1;
			}
			Int_Hash_Set idx_set;
			for (Battle_Scene::Debug_Mover_Map::const_iterator it = bs->pos_debug_map().begin();
					it != bs->pos_debug_map().end(); ++it) {
				if (it->first.pos != pos) continue;
				idx_set.insert(it->first.idx);
			}
			int ret = ensure_battle_pos_unique(idx_set);
			if (0 != ret) {
				return ret;
			}
			int idx = Position_Key::LINE_ONE_FRONT;
			get_empty_pos(idx_set, idx);
			if (-1 == idx) {
				return ERROR_FIGHT_NOT_EMPTY;
			}
			if(pl_a->set_battle_position(pos, idx, bs) == 0){
				pl_a->enter_battle_scene(bs);
				active_player_battle_state(pl_a, sh->point_id, 1);
				drop_out_looking(pl_a, sh->point_id);
			}

		}

		sh->info.attacking_map.insert(std::make_pair(soldier_a->id, soldier_a));
		--soldier_a->nums;
		sh->info.attack_list.pop_front();
		MSG_USER("enter_battle.");
	}

	return 0;
}

int Expedition_Scene::enter_battle_more_defend(Stronghold* sh) {
	if (!sh) {
		return -1;
	}
	Soldier *soldier_a = sh->info.defend_list.front();
	if (!soldier_a) {
		sh->info.defend_list.pop_front();
//		sh->info.defending_map.erase(soldier_a->id);
//		sh->info.defend_map.erase(soldier_a->id);
//		POOL_MANAGER->push_soldier_pool(soldier_a);
		return -1;
	}
	if (soldier_a->nums <= 0) {
		sh->info.attack_list.pop_front();
		sh->info.attack_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		return -1;
	}
	if (soldier_a->type != EST_PLAYER) {
		return -1;
	}
	Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_a->id);
	if (!pl_a) {
		const Expedition_Offline_Player_Data *eopd = find_offline_data(soldier_a->role_id);
		if (eopd) {
			if (0 == enter_battle_for_offline_defend_more(eopd, sh)) {
				sh->info.defending_map.insert(std::make_pair(soldier_a->id, soldier_a));
				--soldier_a->nums;
				sh->info.defend_list.pop_front();
				MSG_USER("enter_battle.");
				return 0;
			}
		}
		--soldier_a->nums;
		sh->info.defend_list.pop_front();
		sh->info.defending_map.erase(soldier_a->id);
		sh->info.defend_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		MSG_USER("attack offline.");
		return -1;
	}

	if (pl_a->is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}
	if (pl_a->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	Soldier_Map::iterator it = sh->info.defending_map.begin();
	if (it != sh->info.defending_map.end()) {
		if (it->second->type == EST_PLAYER) {
			// player only
			Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(it->second->role_id);
			if (pl && pl->battle_scene()) {
				Battle_Scene *bs = pl->battle_scene();

				int pos = -1;
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = bs->pos_debug_map().begin();
						it != bs->pos_debug_map().end(); ++it) {
					if (it->second == pl->role_id()) {
						pos = it->first.pos;
						break;
					}
				}

				if (pos == -1) {
					return -1;
				}
				Int_Hash_Set idx_set;
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = bs->pos_debug_map().begin();
						it != bs->pos_debug_map().end(); ++it) {
					if (it->first.pos != pos) continue;
					idx_set.insert(it->first.idx);
				}
				int ret = ensure_battle_pos_unique(idx_set);
				if (0 != ret) {
					return ret;
				}
				int idx = Position_Key::LINE_ONE_FRONT;
				get_empty_pos(idx_set, idx);
				if (-1 == idx) {
					return ERROR_FIGHT_NOT_EMPTY;
				}
				if(pl_a->set_battle_position(pos, idx, bs) == 0){
					pl_a->enter_battle_scene(bs);
					active_player_battle_state(pl_a, sh->point_id, 1);
					drop_out_looking(pl_a, sh->point_id);
				}

			}
			sh->info.defending_map.insert(std::make_pair(soldier_a->id, soldier_a));
			--soldier_a->nums;
			sh->info.defend_list.pop_front();
			MSG_USER("enter_battle.");
		} else {
			// monster
		}
	}


	return 0;
}

int Expedition_Scene::enter_battle(Soldier *soldier_attack, Soldier *soldier_defend, const int point_id) {
	if (!soldier_attack || !soldier_defend) {
		return -3;
	}
	Scene_Player *pl_a = 0;
	switch(soldier_attack->type) {
	case EST_PLAYER: {
		pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_attack->role_id);
		if (!pl_a) {
//			soldier_offline_.push_back(soldier_attack);
			return -1;
		}
		if (pl_a->is_in_battle()) {
			MSG_USER("has in battle");
			return ERROR_PLAYER_IN_BATTLE;
		}
		if (pl_a->dead_status()) {
			MSG_USER("has die");
			return ERROR_PLAYER_DIE;
		}
		break;
	}
	case EST_MONSTER: {
		break;
	}
	default:
		break;
	}

	switch(soldier_defend->type) {
	case EST_PLAYER: {
		Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(soldier_defend->role_id);
		if (!pl) {
			const Expedition_Offline_Player_Data *eopd = find_offline_data(soldier_defend->role_id);
			if (eopd) {
				if (0 == enter_battle_for_offline_defend(pl_a, eopd, point_id)) {
					return 0;
				}
			}
//			soldier_offline_.push_back(soldier_attack);
			return -2;
		}
		if (pl->is_in_battle()) {
			MSG_USER("has in battle");
			return ERROR_PLAYER_IN_BATTLE;
		}
		if (pl->dead_status()) {
			MSG_USER("has die");
			return ERROR_PLAYER_DIE;
		}
		return enter_battle(pl_a, pl, point_id);
		break;
	}
	case EST_MONSTER: {
		int map_id = 0;
		const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(BATTLE_SCENE);
		if(conf) {
			map_id = conf->value;
		}
		CreateMonsterArgv_Vec wave_count;
		Create_Monster_Argc ii;
		ii.i_v1 = soldier_defend->role_id;
		ii.i_v2 = soldier_defend->nums;
		ii.i64_v1 = soldier_defend->id;
		wave_count.push_back(ii);
		Other_Info ot;
		ot.type = OT_EXPEDITION;
		ot.int_v1 = point_id;
		ot.int_v2 = soldier_defend->nums;
		ot.iv_2.push_back(soldier_defend->id);
		Battle_Scene *bs =NPC_MANAGER->create_battle_with_monster(pl_a, soldier_defend->role_id, 2, map_id, &wave_count, &ot);
		if (!bs) {
			return -3;
		}
		bs->set_battle_type(Battle_Scene::BATTLE_TYPE_TEAM);
//		bs->set_other_info(ot);
		active_player_battle_state(pl_a, point_id, 1);
		soldier_defend->nums_battle = soldier_defend->nums;
		soldier_defend->nums = 1; // use for --
		break;
	}
	default:
		break;
	}
	return 0;
}

int Expedition_Scene::enter_battle(Scene_Player *pl_a, Scene_Player *pl_d, const int point_id) {
	if (!pl_a || !pl_d) {
		return -5;
	}
//	if(mover->player_self()->is_safe_time()){
//		return ERROR_FIGHT_SAFE_TIME;
//	}

	if (pl_a->expedition_scene_info().revive == 1 ||
			pl_a->is_revive()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (pl_d->expedition_scene_info().revive == 1 ||
			pl_d->is_revive()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (pl_d->is_in_battle()) {
		MSG_USER("has in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}

	if (Expedition_Type != get_scene_type(pl_a->move_scene_id())
			|| Expedition_Type != get_scene_type(pl_d->move_scene_id())) {
		return -5;
	}

	if (pl_a->is_in_battle()) {
//		if(!pl_a->move_scene() || 0 == pl_a->move_scene()->scene_id()){
//			MSG_USER("has in battle");
//			return ERROR_PLAYER_IN_BATTLE;
//		}
//		const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(pl_a->move_scene()->scene_id());
//		if (!cfg) {
//			return ERROR_FIGHT_NOT_EMPTY;
//		}
//		int pos = -1;
//		if (Scene_Expedition_Manager == cfg->manager) {
//			for (Battle_Scene::Debug_Mover_Map::const_iterator it = pl_a->battle_scene()->pos_debug_map().begin();
//					it != pl_a->battle_scene()->pos_debug_map().end(); ++it) {
//				if (it->second == pl_a->role_id()) {
//					pos = it->first.pos;
//					break;
//				}
//			}
//		}
//		if (pos == -1) {
//			return -1;
//		}
//		Int_Hash_Set idx_set;
//		for (Battle_Scene::Debug_Mover_Map::const_iterator it = pl_a->battle_scene()->pos_debug_map().begin();
//				it != pl_a->battle_scene()->pos_debug_map().end(); ++it) {
//			if (it->first.pos != pos) continue;
//			idx_set.insert(it->first.idx);
//		}
//		int ret = ensure_battle_pos_unique(idx_set);
//		if (0 != ret) {
//			return ret;
//		}
//		int idx = Position_Key::LINE_ONE_FRONT;
//		get_empty_pos(idx_set, idx);
//		if (-1 == idx) {
//			return ERROR_FIGHT_NOT_EMPTY;
//		}
//		if(pl_d->set_battle_position(pos, idx, pl_a->battle_scene()) == 0){
//			pl_d->enter_battle_scene(pl_a->battle_scene());
//		}
		return -4;
	}

	if (pl_d->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (pl_a->fighter()->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(pl_d->base_detail().location.scene_id);
	if (scene_config) {
		switch(scene_config->manager) {
		case Scene_Expedition_Manager: {
			if (pl_a->ganger_detail().gang_id == pl_d->ganger_detail().gang_id) {
				return -1;
			}
			break;
		}
		default:
			break;
		}
	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(pl_d->move_scene());

	int map_id = 0;
	const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(BATTLE_SCENE);
	if(conf) {
		map_id = conf->value;
	}
	scene->set_map_id(map_id);

	scene->set_battle_type(Battle_Scene::BATTLE_TYPE_TEAM);
	Other_Info ot;
	ot.type = OT_EXPEDITION;
	ot.int_v1 = point_id;
	scene->set_other_info(ot);

	Position_Key player_pos_0(0, Position_Key::LINE_TWO_FRONT);
	if(pl_a->set_battle_position(player_pos_0.pos, player_pos_0.idx, scene) == 0){
		pl_a->enter_battle_scene(scene);
	}

	//mover->set_battle_position(1, Position_Key::LINE_TWO_FRONT);
	Position_Key player_pos_1(1, Position_Key::LINE_TWO_FRONT);
	scene->init_battle_hero_info(pl_d->fighter(), player_pos_1);
	if(pl_d->set_battle_position(player_pos_1.pos, player_pos_1.idx, scene) == 0){
		pl_d->enter_battle_scene(scene);
	}
	active_player_battle_state(pl_a, point_id, 1);
	return 0;
}

int Expedition_Scene::enter_battle_for_offline_defend(Scene_Player *pl_a,
		const Expedition_Offline_Player_Data *eopd, const int point_id) {
	if (!pl_a || !eopd) {
		return -5;
	}
//	if(mover->player_self()->is_safe_time()){
//		return ERROR_FIGHT_SAFE_TIME;
//	}

	if (pl_a->expedition_scene_info().revive == 1 ||
			pl_a->is_revive()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (pl_a->is_in_battle()) {
		return -4;
	}

	if (pl_a->fighter()->dead_status()) {
		MSG_USER("has die");
		return ERROR_PLAYER_DIE;
	}

	if (Expedition_Type != get_scene_type(pl_a->move_scene_id())) {
		return -5;
	}

//	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(pl_a->base_detail().location.scene_id);
//	if (scene_config) {
//		switch(scene_config->manager) {
//		case Scene_Expedition_Manager: {
//			if (pl_a->ganger_detail().gang_id == eopd->player_fighter_detail.gang_id) {
//				return -1;
//			}
//			break;
//		}
//		default:
//			break;
//		}
//	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(pl_a->move_scene());

	int map_id = 0;
	const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(BATTLE_SCENE);
	if(conf) {
		map_id = conf->value;
	}
	scene->set_map_id(map_id);

	scene->set_battle_type(Battle_Scene::BATTLE_TYPE_TEAM);
	Other_Info ot;
	ot.type = OT_EXPEDITION;
	ot.int_v1 = point_id;
	scene->set_other_info(ot);

	// player monster --
//	Position_Key player_pos_1(1, Position_Key::LINE_TWO_FRONT);
	Block_Buffer buf;
	eopd->player_fighter_detail.serialize(buf);
	NPC_Addition_Info add_info;
	add_info.birth_coord.x = 1;
	add_info.birth_coord.y = Position_Key::LINE_TWO_FRONT;
	add_info.ex_val1 = eopd->player_base_detail.career;
	add_info.ex_val2 = eopd->player_base_detail.gender;
	add_info.name = eopd->player_base_detail.role_name;
	int monster_type = 63800108;
	Player_Monster* player_monster = NPC_MANAGER->create_player_monster(monster_type, NULL, add_info, scene, buf);
	if (!player_monster) {
		return -1;
	}
//	Int_Vec avatar_vec;
//	if (i == 0) {
//		avatar_vec.push_back(10731102);
//		avatar_vec.push_back(10931102);
//		avatar_vec.push_back(10831102);
//	} else {
//		avatar_vec.push_back(10711102);
//		avatar_vec.push_back(10911102);
//		avatar_vec.push_back(10811102);
//	}
//	player_monster->set_player_monster_avatar(avatar_vec);
	player_monster->set_clear_label(false);
	player_monster->restore_hp(player_monster->blood_max(), true);
	// -- player monster

	Position_Key player_pos_0(0, Position_Key::LINE_TWO_FRONT);
	if(pl_a->set_battle_position(player_pos_0.pos, player_pos_0.idx, scene) == 0){
		pl_a->enter_battle_scene(scene);
	}

	active_player_battle_state(pl_a, point_id, 1);
	return 0;
}

int Expedition_Scene::enter_battle_for_offline_defend_more(const Expedition_Offline_Player_Data *eopd, const Stronghold *sh) {
	if (!eopd || !sh) {
		return -1;
	}

	Soldier_Map::const_iterator it = sh->info.defending_map.begin();
	if (it == sh->info.defending_map.end() || it->second->type != EST_PLAYER) {
		return -1;
	}

	// player only
	Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(it->second->role_id);
	if (pl && pl->battle_scene()) {
		Battle_Scene *bs = pl->battle_scene();

		int pos = -1;
		for (Battle_Scene::Debug_Mover_Map::const_iterator it = bs->pos_debug_map().begin();
				it != bs->pos_debug_map().end(); ++it) {
			if (it->second == pl->role_id()) {
				pos = it->first.pos;
				break;
			}
		}

		if (pos == -1) {
			return -1;
		}
		Int_Hash_Set idx_set;
		for (Battle_Scene::Debug_Mover_Map::const_iterator it = bs->pos_debug_map().begin();
				it != bs->pos_debug_map().end(); ++it) {
			if (it->first.pos != pos) continue;
			idx_set.insert(it->first.idx);
		}
		int ret = ensure_battle_pos_unique(idx_set);
		if (0 != ret) {
			return ret;
		}
		int idx = Position_Key::LINE_ONE_FRONT;
		get_empty_pos(idx_set, idx);
		if (-1 == idx) {
			return ERROR_FIGHT_NOT_EMPTY;
		}

		// player monster --
	//	Position_Key player_pos_1(1, Position_Key::LINE_TWO_FRONT);
		Block_Buffer buf;
		eopd->player_fighter_detail.serialize(buf);
		NPC_Addition_Info add_info;
		add_info.birth_coord.x = 1;
		add_info.birth_coord.y = Position_Key::LINE_TWO_FRONT;
		add_info.ex_val1 = eopd->player_base_detail.career;
		add_info.ex_val2 = eopd->player_base_detail.gender;
		add_info.name = eopd->player_base_detail.role_name;
		int monster_type = 63800108;
		Player_Monster* player_monster = NPC_MANAGER->create_player_monster(monster_type, NULL, add_info, bs, buf);
		if (!player_monster) {
			return -1;
		}
	//	Int_Vec avatar_vec;
	//	if (i == 0) {
	//		avatar_vec.push_back(10731102);
	//		avatar_vec.push_back(10931102);
	//		avatar_vec.push_back(10831102);
	//	} else {
	//		avatar_vec.push_back(10711102);
	//		avatar_vec.push_back(10911102);
	//		avatar_vec.push_back(10811102);
	//	}
	//	player_monster->set_player_monster_avatar(avatar_vec);
		player_monster->set_clear_label(false);
		player_monster->restore_hp(player_monster->blood_max(), true);
		// -- player monster
	}
	return 0;
}

int Expedition_Scene::enter_battle_for_other(const role_id_t role_id, Stronghold* sh) {
	if (!sh) {
		return -1;
	}
	Role_ExpeditionFightInfo_Map::iterator it_rdmfb = sh->info.role_defend_map_for_beast.find(role_id);
	if (it_rdmfb == sh->info.role_defend_map_for_beast.end()) {
		return -1;
	}
	Soldier *soldier_a = it_rdmfb->second.attack_list.front();
	Soldier *soldier_d = it_rdmfb->second.defend_list.front();
	bool need_return = false, need_return_b = false;
	if (!soldier_a) {
		it_rdmfb->second.attack_list.pop_front();
//		it_rdmfb->second.attack_map.erase(soldier_a->id);
//		POOL_MANAGER->push_soldier_pool(soldier_a);
		need_return = true;
	}
	if (soldier_a->nums <= 0) {
		if (!need_return) {
			it_rdmfb->second.attack_list.pop_front();
		}
		it_rdmfb->second.attack_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		need_return = true;
	}
	if (!soldier_d) {
		it_rdmfb->second.defend_list.pop_front();
//		it_rdmfb->second.defend_map.erase(soldier_d->id);
//		POOL_MANAGER->push_soldier_pool(soldier_d);
		need_return_b = true;
	}
	if (!soldier_d) {
		if (!need_return_b) {
			it_rdmfb->second.defend_list.pop_front();
		}
		it_rdmfb->second.defend_map.erase(soldier_d->id);
		POOL_MANAGER->push_soldier_pool(soldier_d);
		need_return_b = true;
	}
	if (need_return) {
		return -1;
	}
	int ret = enter_battle(soldier_a, soldier_d, sh->point_id);
	switch(ret) {
	case 0: {
		it_rdmfb->second.attacking_map.insert(std::make_pair(soldier_a->id, soldier_a));
		it_rdmfb->second.defending_map.insert(std::make_pair(soldier_d->id, soldier_d));
		--soldier_a->nums;
		--soldier_d->nums;
		it_rdmfb->second.attack_list.pop_front();
		it_rdmfb->second.defend_list.pop_front();
		MSG_USER("enter_battle.");
		Scene_Player *pl_a = SCENE_MONITOR->find_scene_player_by_role_id(soldier_a->id);
		if (pl_a) {
			active_player_battle_state(pl_a, sh->point_id, 1);
			drop_out_looking(pl_a, sh->point_id);
		}
		Scene_Player *pl_d = SCENE_MONITOR->find_scene_player_by_role_id(soldier_d->id);
		if (pl_d) {
			active_player_battle_state(pl_d, sh->point_id, 1);
			drop_out_looking(pl_d, sh->point_id);
		}

		Soldier_Map::iterator it = it_rdmfb->second.looking_defend_map.find(soldier_d->id);
		if (it != it_rdmfb->second.looking_defend_map.end()) {
			it_rdmfb->second.looking_defend_list.remove(it->second);
			POOL_MANAGER->push_soldier_pool(it->second);
			it_rdmfb->second.looking_defend_map.erase(it);
		}
		break;
	}
	case -1: {
		it_rdmfb->second.defending_map.insert(std::make_pair(soldier_d->id, soldier_d));
		--soldier_a->nums;
		--soldier_d->nums;
		it_rdmfb->second.attack_list.pop_front();
		it_rdmfb->second.defend_list.pop_front();
		it_rdmfb->second.attack_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		MSG_USER("attack offline.");
		break;
	}
	case -2: {
		it_rdmfb->second.attacking_map.insert(std::make_pair(soldier_a->id, soldier_a));
		--soldier_a->nums;
		--soldier_d->nums;
		it_rdmfb->second.attack_list.pop_front();
		it_rdmfb->second.defend_list.pop_front();
		it_rdmfb->second.defend_map.erase(soldier_a->id);
		POOL_MANAGER->push_soldier_pool(soldier_a);
		MSG_USER("defend offline.");
		break;
	}
	case -3: {
		break;
	}
	case -4: {
		MSG_USER("ERROR: expedition player(%ld) is in battle.", soldier_a->id);
//		sh->info.defending_map.insert(std::make_pair(soldier_d->id, soldier_d));
//		--soldier_a->nums;
//		--soldier_d->nums;
//		sh->info.attack_list.pop_front();
//		sh->info.defend_list.pop_front();
//		sh->info.attack_map.erase(soldier_a->id);
//		POOL_MANAGER->push_soldier_pool(soldier_a);
		break;
	}
	default:
		break;
	}
	return 0;
}

int Expedition_Scene::active_player_battle_state(const std::vector<Mover*> &mover_vec, const int point_id, const int state) {
	for (std::vector<Mover*>::const_iterator it_mv = mover_vec.begin();
			it_mv != mover_vec.end(); ++it_mv) {
		if ((**it_mv).player_self()) {
			if ((**it_mv).player_self()->dead_status()) {
				active_player_battle_state((**it_mv).player_self(), point_id, 0, true);
			} else {
//				active_player_battle_state((**it_mv).player_self(), point_id, state);
			}
		}
	}
	return 0;
}

int Expedition_Scene::active_player_battle_state(Scene_Player *pl, const int point_id, const int state, const bool is_dead) {
	if (!pl) {
		return -1;
	}
	MSG_50201306 res_msg;
	res_msg.point_id = point_id;
	res_msg.state = state;
	res_msg.result = 1;
	if (is_dead) {
		res_msg.result = 0;
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

const Expedition_Gang_Home_Cfg * Expedition_Scene::get_one_point_conf(void) {
	const ExpeditionGangHomeCfg_Map conf = CONFIG_CACHE_EXPEDITION->gang_home_conf_map();//get_gang_home_conf_by_rank(scene->point_gang_id() + 1);
	for (uint16_t i = 0; i < conf.size(); ++i) {
		const Expedition_Gang_Home_Cfg *eghc = CONFIG_CACHE_EXPEDITION->get_gang_home_conf_by_rank(i+1);
		if (!eghc) {
			MSG_TRACE_ABORT("ERROR: expedition cant find point for rank(%d).", i+1);
			continue;
		}
		if (0 == point_gang_id_.count(eghc->point_id)) {
			return eghc;
		}
	}
	return 0;
}

int Expedition_Scene::add_offline_mover(const Expedition_Scene_Info &expedition_scene_info) {
	expedition_scene_info_.insert(std::make_pair(expedition_scene_info.role_id, expedition_scene_info));
	return 0;
}

Expedition_Scene_Info *Expedition_Scene::find_offline_mover(const int64_t role_id) {
	Role_ExpeditionSceneInfo_Map::iterator it = expedition_scene_info_.find(role_id);
	if (it != expedition_scene_info_.end()) {
		return &(it->second);
	}
	return 0;
}

int Expedition_Scene::del_offline_mover(const int64_t role_id) {
	expedition_scene_info_.erase(role_id);
	return 0;
}

bool Expedition_Scene_Demage_Rank_Data_Greater(Expedition_Scene_Demage_Rank_Data l1, Expedition_Scene_Demage_Rank_Data l2){
	if (l1.demage > l2.demage) {
		return true;
	} else if (l1.demage < l2.demage) {
		return false;
	}
	if (l1.force > l2.force) {
		return true;
	} else if (l1.force < l2.force) {
		return false;
	}
	if (l1.level > l2.level) {
		return true;
	} else if (l1.level < l2.level) {
		return false;
	}

	if (l1.role_id < l2.role_id) {
		return true;
	}
	return false;
}

int Expedition_Scene::add_demage_rank(Scene_Player *pl, const double demage) {
	if (!pl || !CONFIG_CACHE_EXPEDITION->is_demage_count_time()) {
		return -1;
	}
	Role_ExpeditionSceneDemageRankData_Map::iterator it = demage_rank_.find(pl->role_id());
	if (it != demage_rank_.end()) {
		it->second.role_name = pl->role_name();
		it->second.demage += demage;
	} else {
		Expedition_Scene_Demage_Rank_Data esdrd;
		esdrd.role_id = pl->role_id();
		esdrd.role_name = pl->role_name();
		esdrd.force = pl->fighter_detail().ttl_force;
		esdrd.level = pl->level();
		esdrd.gang_id = pl->ganger_detail().gang_id;
		if (manager_) {
			const Expedition_Gang_Rank_Info *egr = manager_->find_gang_rank_info_by_gang_id(esdrd.gang_id);
			if (egr) {
				esdrd.gang_name = egr->gang_name;
				esdrd.uuid = egr->uuid;
				esdrd.server_name = egr->server_name;
			}
		}
		esdrd.demage += demage;
		demage_rank_.insert(std::make_pair(esdrd.role_id, esdrd));
	}
	reset_demage_vec();

	this->save_tick();
	return 0;
}

void Expedition_Scene::reset_demage_vec(void) {
	demage_vec_.clear();
	for (Role_ExpeditionSceneDemageRankData_Map::const_iterator it = demage_rank_.begin();
			it != demage_rank_.end(); ++it) {
		demage_vec_.push_back(it->second);
	}
	std::sort(demage_vec_.begin(), demage_vec_.end(), Expedition_Scene_Demage_Rank_Data_Greater);
}

bool Expedition_Scene_Occupy_Castle_Rank_Greater(Expedition_Occupy_Castle_Data c1, Expedition_Occupy_Castle_Data c2) {
	if (c1.nums > c2.nums) {
		return true;
	} else if (c1.nums < c2.nums) {
		return false;
	}

	if (c1.force > c2.force) {
		return true;
	} else if (c1.force < c2.force) {
		return false;
	}

	if (c1.level > c2.level) {
		return true;
	} else if (c1.level < c2.level) {
		return false;
	}

	if (c1.role_id > c2.role_id) {
		return true;
	}
	return false;
}
void Expedition_Scene::add_occupy_castle_rank(Soldier* pl) {
	if (!pl) {
		return ;
	}
	Role_ExpeditionOccupyCastleData_Map::iterator it = occupy_castle_rank_.find(pl->role_id);
	if (it != occupy_castle_rank_.end()) {
		++it->second.nums;
	} else {
		Expedition_Occupy_Castle_Data eocd;
		eocd.role_id = pl->role_id;
		eocd.role_name = pl->role_name;
		eocd.force = pl->force;
		eocd.level = pl->level;
		eocd.gang_id = pl->occupier.gang_id;
		eocd.gang_name = pl->occupier.gang_name;
		eocd.server_uuid = pl->occupier.server_uuid;
		eocd.server_name = pl->occupier.server_name;
		eocd.nums = 1;
		occupy_castle_rank_.insert(std::make_pair(eocd.role_id, eocd));
	}

	occupy_castle_vec_.clear();
	for (Role_ExpeditionOccupyCastleData_Map::const_iterator it = occupy_castle_rank_.begin();
			it != occupy_castle_rank_.end(); ++it) {
		occupy_castle_vec_.push_back(it->second);
	}
	std::sort(occupy_castle_vec_.begin(), occupy_castle_vec_.end(), Expedition_Scene_Occupy_Castle_Rank_Greater);

	this->save_tick();
}

void Expedition_Scene::active_occupy_castle_rank(Scene_Player *pl) {
	MSG_50201311 res_msg;
	Expedition_Occupy_Castle err;
	int rank = 1;
	role_id_t role_id = pl->role_id();
	bool has_exist = false;
	for (ExpeditionOccupyCastleData_Vec::iterator it = occupy_castle_vec_.begin();
			it != occupy_castle_vec_.end(); ++it) {
		err.reset();
		(*it).set_msg_data(err);
		err.rank = rank;
		res_msg.list.push_back(err);
		if (role_id == (*it).role_id) {
			has_exist = true;
			if (0 == (*it).get_nums) {
				Config_Cache_Expedition::Int_ItemInfoVec_Map::const_iterator it_ocra = CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().begin();
				if (it_ocra != CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().end()) {
					res_msg.award_list = it_ocra->second;
					res_msg.get_nums = it_ocra->first;
				}
			} else {
				for (Config_Cache_Expedition::Int_ItemInfoVec_Map::const_iterator it_ocra = CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().begin();
						it_ocra != CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().end(); ++it_ocra) {
					if (it_ocra->first > (*it).get_nums) {
						res_msg.award_list = it_ocra->second;
						res_msg.get_nums = it_ocra->first;
						break;
					}
				}
			}
			if (res_msg.get_nums != 0 && (*it).nums >= res_msg.get_nums) {
				res_msg.state = 1;
			}
		}
		++rank;
	}
	if (!has_exist) {
		Config_Cache_Expedition::Int_ItemInfoVec_Map::const_iterator it_ocra = CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().begin();
		if (it_ocra != CONFIG_CACHE_EXPEDITION->occupy_castle_rank_award().end()) {
			res_msg.award_list = it_ocra->second;
			res_msg.get_nums = it_ocra->first;
		}
	}

	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
}

void Expedition_Scene::init_refreshed_monster(const Point_ExpeditionDropOrMaterial &refreshed_monster) {
	if (refreshed_monster.empty()) {
		return;
	}
	Int_Set points;
	for (NPC_Record_Map::iterator it = npc_section_.record_map.begin();
			it != npc_section_.record_map.end(); ++it) {
		int point_id = it->first;
		Point_ExpeditionDropOrMaterial::const_iterator it_rm = refreshed_monster.find(point_id);
		if (it_rm == refreshed_monster.end()) {
			continue;
		}
		if (it->second.alive_npc_map.empty()) {
			points.insert(point_id);
		} else {
			for (RoleID_NPC_Map::iterator it_rc = it->second.alive_npc_map.begin();
					it_rc != it->second.alive_npc_map.end(); ++it_rc) { // must be one
				it_rc->second->clear_role_collect();
				it_rc->second->add_role_collect(it_rm->second.had_collected);
			}
		}
		refreshed_drop_or_material_[point_id] = it_rm->second;
	}

	for (Int_Set::const_iterator it = points.begin(); it != points.end(); ++it) {
		refresh_monster_point(*it, 0);
	}

	for (Int_Set::const_iterator it = points.begin(); it != points.end(); ++it) {
		int point_id = *it;
		Point_ExpeditionDropOrMaterial::const_iterator it_rm = refreshed_monster.find(point_id);
		if (it_rm == refreshed_monster.end()) {
			continue;
		}
		NPC_Record* point = get_npc_record(point_id);
		if (!point) {
			continue;
		}
		for (RoleID_NPC_Map::iterator it_rc = point->alive_npc_map.begin();
				it_rc != point->alive_npc_map.end(); ++it_rc) { // must be one
			it_rc->second->clear_role_collect();
			it_rc->second->add_role_collect(it_rm->second.had_collected);
			if (it_rc->second->is_material()) {
				Material *npc = (Material*)it_rc->second;
				npc->add_addition(0);
			} else if (it_rc->second->is_drops()) {
				Drops *npc = (Drops*)it_rc->second;
				npc->add_addition(0);
			}
		}
	}
}

void Expedition_Scene::del_refreshed_monster(const int point_id) {
	this->refreshed_drop_or_material_time_history_.erase(point_id);
	this->refreshed_drop_or_material_.erase(point_id);
	save_tick();
}

int Expedition_Scene::add_role_collected(const int point_id, const int64_t role_id) {
	Point_ExpeditionDropOrMaterial::iterator it = refreshed_drop_or_material_.find(point_id);
	if (it != refreshed_drop_or_material_.end()) {
		it->second.had_collected.insert(role_id);
		save_tick();
	}
	return 0;
}


int Expedition_Scene::add_offline_data(Scene_Player *pl) {
	if (!pl || offline_data_.count(pl->role_id())) {
		return -1;
	}
	Expedition_Offline_Player_Data eopd;
	eopd.player_base_detail = pl->base_detail();
	eopd.player_fighter_detail = pl->fighter_detail();
	Hero *hero = SCENE_PUBLIC->find_fight_hero(pl->role_id());
	if (hero) {
		eopd.hero_base_detail = hero->hero_base_detail();		// 主人id
		eopd.hero_fighter_detail = hero->fighter_detail();
	}
	return 0;
}

int Expedition_Scene::del_offline_data(const role_id_t role_id) {
	offline_data_.erase(role_id);
	return 0;
}

const Expedition_Offline_Player_Data *Expedition_Scene::find_offline_data(const role_id_t role_id) {
	ExpeditionOfflinePlayerData_Map::const_iterator it = offline_data_.find(role_id);
	if (it != offline_data_.end()) {
		return &(it->second);
	}
	return 0;
}
// ------------------ interface end ---------------------------//

int Expedition_Scene::create_drop_by_point_id(const int point_id, Scene_Player &pl) {
	Stronghold *sh = find_map_data_by_point_id(point_id);
	if (sh && sh->type == EBT_BEAST) {
		// notice logic --
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_PLAYER);
		buf.write_int8((int8_t)Data_Channel::COULD_COLLECT_BEAST);
		buf.write_int32(point_id);
		buf.finish_message();
		pl.send_to_logic(buf);
		pl.expedition_detail().collect_beast.insert(point_id);
		// -- notice logic

		const Expedition_Map_Point_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
		if (!conf || conf->drops.size() < 1) {
			return -1;
		}
		int drops_id = conf->drops[0u];
		const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(drops_id);
		if (drop_config == NULL) return -1;
		Drop_Vec drops_all;
		NPC_MANAGER->process_drop_mode(pl.career(), drops_all, drop_config, 1, 1, 1, 1);
		Coord coord(conf->x, conf->y, Coord::PIXEL_TYPE);
		NPC_MANAGER->create_drops("", Drops::DROP_TYPE_BOX, drops_id, drop_config->jid, drops_all, this, NULL, coord, pl.role_id(), 0, 0);
	}
	update_move_able_info(&pl);
	return 0;
}

void Expedition_Scene::award_tick(const Time_Value &now) {
	if (!manager_ || award_time_ > now) {
		return ;
	}
	Date_Time dt(now);
	if (award_time_ == Time_Value::zero) {
		if (dt.hour() < 22) {
			return;
		}
	}

	create_and_send_daily_award_to_logic();

	dt.hour(21);
	dt.day(dt.day()+1);
	dt.minute(56);
	dt.second(0);
	dt.microsec(0);
	award_time_.sec(dt.time_sec());
	save_tick();
}

void Expedition_Scene::settle_tick(const Time_Value &now) {
	Date_Time dt(now);
	Date_Time dt_settle(this->settle_time_falg_);
	if ((dt.year() != dt_settle.year()
			|| dt.month() != dt_settle.month()
			|| dt.day() != dt_settle.day()
			|| dt.hour() != dt_settle.hour()
			|| dt.minute() != dt_settle.minute())
			&& CONFIG_CACHE_EXPEDITION->is_demage_settle_time(settle_time_)) {
		settle_handle();
		this->settle_time_falg_ = now;
	}
}

void Expedition_Scene::settle_handle(void) {
	int rank = 1;
	for (ExpeditionSceneDemageRankData_Vec::iterator it = demage_vec_.begin();
			it != demage_vec_.end(); ++it) {
//		const Item_Info_Vec *iiv = CONFIG_CACHE_EXPEDITION->find_demage_rank_award(rank);
//		if (iiv) {
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
		buf.write_uint8(EXPEDITION_CHANNEL_DEMAGE_RANK_AWARD);
		buf.write_int64(it->role_id);
		buf.write_int32(rank);
		buf.finish_message();
		SCENE_MONITOR->send_to_logic(it->uuid, buf);
//		MSG_USER("demage rank award: role(%ld).", it->role_id);
//		}
		++rank;
	}
	demage_vec_.clear();
	demage_rank_.clear();
}

void Expedition_Scene::refresh_monster_timing_tick(const Time_Value &now) {
	Int_Hash_Set need_refresh_points, need_clear_points;
	CONFIG_CACHE_EXPEDITION->refresh_monster_timing(now, need_refresh_points);
	if (need_refresh_points.empty()) {
		return;
	}

	for (Int_Hash_Set::const_iterator it = need_refresh_points.begin();
			it != need_refresh_points.end(); ++it) {
		int point_id = *it;
		if (refreshed_monster_timing_.count(point_id)) {
			Date_Time dt(now);
			Date_Time dt_m(refreshed_monster_timing_[point_id]);
			if (dt.year() == dt_m.year()
					&& dt.month() == dt_m.month()
					&& dt.day() == dt_m.day()
					&& dt.hour() == dt_m.hour()
					&& dt.minute() == dt_m.minute()) {
				continue;
			}
		}
		refreshed_monster_timing_[point_id] = now;
		show_monster(point_id);
	}

	sync_refresh_monster_timing_to_logic(need_refresh_points);


	for (Int_Time_Map::iterator it = refreshed_monster_timing_.begin(); it != refreshed_monster_timing_.end();) {
		if (!need_refresh_points.count(it->first)) {
			refreshed_monster_timing_.erase(it++);
			continue;
		}
		++it;
	}
	save_tick();
}

void Expedition_Scene::sync_refresh_monster_timing_to_logic(const Int_Hash_Set &need_refresh_monster) {
	if (need_refresh_monster.empty()) {
		return;
	}
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPE_MONSTER_TIMING_REFRESH);
	Time_Value::gettimeofday().serialize(buf);
	buf.write_uint16(need_refresh_monster.size());
	for (Int_Hash_Set::const_iterator it = need_refresh_monster.begin(); it != need_refresh_monster.end(); ++it) {
		buf.write_int32(*it);
	}
	buf.finish_message();

	Role_Hash_Set uuids;
	get_uuids(uuids);
	for (Role_Hash_Set::const_iterator it = uuids.begin(); it != uuids.end(); ++it) {
		SCENE_MONITOR->send_to_logic(*it, buf);
	}
}

void Expedition_Scene::sync_refresh_monster_timing_kill_to_logic(const int point_id) {
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (!sh) { // sh begin --
		return;
	}
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPE_MONSTER_TIMING_KILLED);
//	Time_Value::gettimeofday().serialize(buf);
	buf.write_int32(point_id);
	sh->win_attack_people.serialize(buf);
	buf.finish_message();

	Role_Hash_Set uuids;
	get_uuids(uuids);
	for (Role_Hash_Set::const_iterator it = uuids.begin(); it != uuids.end(); ++it) {
		SCENE_MONITOR->send_to_logic(*it, buf);
	}
}

void Expedition_Scene::sync_refresh_open_box_to_logic(const int drop_id, const Int_Hash_Set items, Scene_Player *sp) {
	if (items.empty() || !sp) {
		return;
	}
	const Drop_Config *dc = CONFIG_CACHE_NPC->find_drop_config_cache(drop_id);
	if (!dc) {
		return;
	}

	Int_Hash_Set ihs;
	for (Int_Hash_Set::const_iterator it = items.begin(); it != items.end(); ++it) {
		if (CONFIG_CACHE_EXPEDITION->is_drop_need_boradcast(*it)) {
			ihs.insert(*it);
		}
	}

	if (ihs.empty()) {
		return;
	}
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	buf.write_uint8(EXPE_BOX_OPEND);

	buf.write_string(dc->name);

	Role_Info role_info;
	// role_info
	role_info.career = sp->career();
	role_info.combat = 0;
	role_info.gender = sp->base_detail().gender;
	role_info.level  = sp->level();
	role_info.role_id = sp->role_id();
	role_info.role_name = sp->role_name();
	role_info.vip_level = sp->vip();
	role_info.guild_name = sp->base_detail().gang_name;
	role_info.serialize(buf);

	buf.write_uint16(ihs.size());
	for (Int_Hash_Set::const_iterator it = ihs.begin(); it != ihs.end(); ++it) {
		buf.write_int32(*it);
	}
	buf.finish_message();

	Role_Hash_Set uuids;
	get_uuids(uuids);
	for (Role_Hash_Set::const_iterator it = uuids.begin(); it != uuids.end(); ++it) {
		SCENE_MONITOR->send_to_logic(*it, buf);
	}
}

void Expedition_Scene::refresh_monster_kill_gap_tick(const Time_Value &now) {
	for (Int_Time_Map::iterator it = refreshed_monster_kill_gap_.begin(); it != refreshed_monster_kill_gap_.end();) {
		if (it->second > now) {
			++it;
			continue;
		}
		show_monster(it->first);
		refreshed_monster_kill_gap_.erase(it++);
	}
	save_tick();
}

void Expedition_Scene::refresh_drop_or_material(const Time_Value &now) {
	Int_Hash_Set need_refresh_points, need_clear_points;
	CONFIG_CACHE_EXPEDITION->refresh_drop_or_material(now, need_refresh_points, need_clear_points);
	if (need_refresh_points.empty()) {
		return;
	}

	for (Int_Hash_Set::const_iterator it = need_refresh_points.begin();
			it != need_refresh_points.end(); ++it) {
		int point_id = *it;
		if (refreshed_drop_or_material_time_history_.count(point_id)) {
			Date_Time dt(now);
			Date_Time dt_m(refreshed_drop_or_material_time_history_[point_id]);
			if (dt.year() == dt_m.year()
					&& dt.month() == dt_m.month()
					&& dt.day() == dt_m.day()
					&& dt.hour() == dt_m.hour()
					&& dt.minute() == dt_m.minute()) {
				continue;
			}
		}
		if (refreshed_drop_or_material_.count(point_id)) {
			Date_Time dt(now);
			Date_Time dt_m(refreshed_drop_or_material_[point_id].birth_time);
			if (dt.year() == dt_m.year()
					&& dt.month() == dt_m.month()
					&& dt.day() == dt_m.day()
					&& dt.hour() == dt_m.hour()
					&& dt.minute() == dt_m.minute()) {
				continue;
			}
		}

		Expedition_Drop_Or_Material edom;
		edom.birth_time = now;
		refreshed_drop_or_material_[point_id] = edom;

		refreshed_drop_or_material_time_history_[point_id] = now;
		refresh_monster_point(point_id, 0);
	}

	for (Int_Hash_Set::const_iterator it = need_clear_points.begin();
			it != need_clear_points.end(); ++it) {
		NPC_Record* point = get_npc_record(*it);
		if (!point) {
			continue;
		}
		point->clear_point();
	}
	save_tick();
}

void Expedition_Scene::show_monster(const int point_id) {
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (!sh
			|| !sh->info.defend_map.empty()) {
		return;
	}
	const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(sh->point_id);
	if (empc) {
		sh->init_monster_group(*empc, false);
		sh->init_defend_map_occupier();
		sh->info.fight_status = EBFS_PEACE;
	}
	this->active_map_all_data_by_point_id(point_id);
}

int Expedition_Scene::get_hold_nums_by_gang_id(const int64_t gang_id, const int8_t type) {
	if (!manager_) {
		if (type == EBT_GANG_BASE || type == EBT_UNDEFINE) {
			return 1;
		} else {
			return 0;
		}
	}
	int hold_nums = 0;
	int point_id = manager_->find_point_id_by_gang_id(gang_id);
	Stronghold *sh = this->find_map_data_by_point_id(point_id);
	if (sh) {
		const Int_Hash_Set *ihs = find_base_territorys(point_id);
		if (ihs) {
			for (Int_Hash_Set::const_iterator it = ihs->begin(); it != ihs->end(); ++it) {
				Stronghold *sh = find_map_data_by_point_id(*it);
				if (!sh || (type != EBT_UNDEFINE && type != sh->type)
						|| (type == EBT_UNDEFINE && sh->type != EBT_GANG_BASE
						&& EBT_CHECKPOINT != sh->type
						&& EBT_CASTLE_BIG != sh->type
						&& EBT_CASTLE_MEDIUM != sh->type
						&& EBT_CASTLE_SMALL != sh->type)) {
					continue;
				}
				++hold_nums;
			}
		}
	}
	if (type == EBT_GANG_BASE || type == EBT_UNDEFINE) {
		++hold_nums;
	}
	return hold_nums;
}

int Expedition_Scene::create_and_send_daily_award_to_logic(void) {
	typedef boost::unordered_map<role_id_t, std::map<int, int> > GangID_IntIntMap_Map;
	GangID_IntIntMap_Map rim;
	for (Stronghold_Map::const_iterator it = map_data_.begin();
			it != map_data_.end(); ++it) {
		const Stronghold *sh = &(it->second);
		if (0 == sh->info.occupier.gang_id
				|| (sh->type != EBT_GANG_BASE
				&& EBT_CHECKPOINT != sh->type
				&& EBT_CASTLE_BIG != sh->type
				&& EBT_CASTLE_MEDIUM != sh->type
				&& EBT_CASTLE_SMALL != sh->type)) {
			continue;
		}
		GangID_IntIntMap_Map::iterator it_rim = rim.find(sh->info.occupier.gang_id);
		if (it_rim != rim.end()) {
			if (it_rim->second.count(sh->type)) {
				it_rim->second[sh->type] += 1;
			} else {
				it_rim->second.insert(std::make_pair(sh->type, 1));
			}
		} else {
			std::map<int, int> iim;
			iim.insert(std::make_pair(sh->type, 1));
			rim.insert(std::make_pair(sh->info.occupier.gang_id, iim));
		}
	}
	for (GangID_IntIntMap_Map::iterator it = rim.begin(); it != rim.end(); ++it) {
		send_gang_hold_nums_to_logic_by_gang_id(it->first, it->second);
	}
	return 0;
}

int Expedition_Scene::send_gang_hold_nums_to_logic_by_gang_id(const int64_t gang_id, const std::map<int, int> nums) {
	if (!manager_) {
		return -1;
	}
	const Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(gang_id);
	if (!egri) {
		return -1;
	}
	Block_Buffer block_buf;
	block_buf.make_message(INNER_EXPEDITION_DATA_CHANNEL);
	block_buf.write_uint8(EXPEDITION_CHANNEL_DAILY_AWARD);
	block_buf.write_int64(gang_id);
	for (int i = 0; i < 5; ++i) {
		if (nums.count(i+1)) {
			int num = nums.at(i+1);
			block_buf.write_int32(num);
		} else {
			block_buf.write_int32(0);
		}
	}
	block_buf.finish_message();
	SCENE_MONITOR->send_to_logic(egri->uuid, block_buf);
	return 0;
}

int Expedition_Scene::init_gang_base_territory(const int point_id, const bool is_add) {
	if (point_id <= 0) {
		return -1;
	}
	Stronghold *sh = find_map_data_by_point_id(point_id);
	if (0 == sh) {
		MSG_TRACE_ABORT("ERROR: cant find point(%d) from map_data_ in cur_line(%d)!", point_id, cur_line_);
		return 0;
	}
	point_gang_id_[point_id] = sh->info.occupier.gang_id;

	gang_id_areavalue_.erase(sh->info.occupier.gang_id);

	gang_ids_.insert(sh->info.occupier.gang_id);

	const Expedition_Map_Point_Cfg *point_conf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
	if (NULL == point_conf) {
		MSG_TRACE_ABORT("ERROR: cant find point(%d) from conf.", point_id);
		return -1;
	}
	gang_id_areavalue_[ sh->info.occupier.gang_id ].insert(point_conf->areavalue);

	// -----基地归属点初始化数据
	Int_Hash_Set territory;
	const Int_Hash_Set *ihs = find_base_territorys(point_id);
	if (0 == ihs) {
		//MSG_TRACE_ABORT("ERROR: cant find base point(%d) from conf.", point_id);
		return -1;
	}

	for (Int_Hash_Set::const_iterator it = ihs->begin(); it != ihs->end(); ++it) {
		Stronghold_Map::iterator it_ter = map_data_.find(*it);
		if (it_ter == map_data_.end()) {
			MSG_USER("ERROR: cant find point(%d) from map_data_ in cur_line(%d)!", *it, cur_line_);
//			MSG_TRACE_ABORT("ERROR: cant find point(%d) from map_data_ in cur_line(%d)!", *it, cur_line_);
			continue;
		}
		it_ter->second.info.occupier = sh->info.occupier;
		it_ter->second.info.fight_status = sh->info.fight_status;
		it_ter->second.info.belong_status = sh->info.belong_status;
		it_ter->second.init_defend_map_occupier();

		point_conf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(*it);
		if (NULL == point_conf) {
			MSG_TRACE_ABORT("ERROR: cant find point(%d) from conf.", *it);
			continue;
		}
		gang_id_areavalue_[ sh->info.occupier.gang_id ].insert(point_conf->areavalue);
	}

	init_area_for_beast_and_chest(gang_id_areavalue_[ sh->info.occupier.gang_id ]);
	return 0;
}

const Int_Hash_Set *Expedition_Scene::find_base_territorys(const int point_id) {
	Int_IntHashSet_Map::iterator it_bt = base_territorys_.find(point_id);
	if (it_bt != base_territorys_.end()) {
		return &(it_bt->second);
	}
	return 0;
}

int Expedition_Scene::set_msg_by_type(Scene_Player &pl, const Stronghold &sh, Expedition_Build_Brief_Info &ebbi) {
	switch(sh.type) {
	case EBT_BEAST: {
//		if (sh.info.occupier.gang_id == pl.ganger_detail().gang_id) {
			Role_ExpeditionFightInfo_Map::const_iterator it = sh.info.role_defend_map_for_beast.find(pl.role_id());
			if (it != sh.info.role_defend_map_for_beast.end()) {
//				Soldier_Map::const_iterator it_sm = it->second.attack_map.begin();
//				if (it_sm != it->second.attack_map.end()) {
//					ebbi.val1 = it_sm->second->nums;
//				}
//				it_sm = it->second.defend_map.begin();
//				if (it_sm != it->second.defend_map.end()) {
//					ebbi.val2 = it_sm->second->nums;
//				}
//				ebbi.val1 = (int16_t)it->second.attack_map.size();
				for (Soldier_Map::const_iterator it_dm = it->second.defend_map.begin();
						it_dm != it->second.defend_map.end(); ++it_dm) {
					ebbi.val1 += it_dm->second->nums;
				}
//				if (EBFS_PEACE == it->second.fight_status) {
//					ebbi.val1 = ebbi.val2;
//					ebbi.val2 = 0;
//				}
//				Int_Int_Map::iterator it = pl.expedition_detail().kill_beast.find(sh.point_id);
//				if (it != pl.expedition_detail().kill_beast.end()) {
//					ebbi.val1 = it->second;
//				}
			}
			if ((ebbi.val1 == 0 && ebbi.val2 == 0)
				|| pl.expedition_detail().collect_beast.count(sh.point_id)) {
				ebbi.val1 = 0;
				ebbi.val2 = 0;
				ebbi.val3 = 2;
			}
//		} else {
//			ebbi.val1 = 0;
//			ebbi.val2 = 0;
//			ebbi.val3 = 0;
//		}
		break;
	}
	case EBT_CHEST: {
//		if (sh.info.occupier.gang_id == pl.ganger_detail().gang_id) {
			ebbi.val3 = pl.expedition_detail().collect_chest.count(sh.point_id) + 1;
//		} else {
//			ebbi.val3 = 0;
//		}
		break;
	}
	case EBT_MONSTER_REFRESH_TIMING: {
		ebbi.val3 = ERS_HIDE;
		Soldier_Map::const_iterator it_sm = sh.info.attack_map.begin();
		if (it_sm != sh.info.attack_map.end()) {
			ebbi.val1 = it_sm->second->nums;
		}

		for (Soldier_Map::const_iterator it_dm = sh.info.defend_map.begin();
				it_dm != sh.info.defend_map.end(); ++it_dm) {
			ebbi.val2 += it_dm->second->nums;
		}

		if (EBFS_PEACE == sh.info.fight_status) {
			ebbi.val1 = ebbi.val2;
			ebbi.val2 = 0;
		}
		if (this->refreshed_monster_timing_.count(sh.point_id)) {
			ebbi.val3 = ERS_SHOW;
		}
		if ((ebbi.val1 == 0 && ebbi.val2 == 0)
				|| !this->refreshed_monster_timing_.count(sh.point_id)) {
			ebbi.val1 = 0;
			ebbi.val2 = 0;
			ebbi.val3 = ERS_HIDE;
		}
		break;
	}
	case EBT_MONSTER_REFRESH_KILL_GAP: {
		ebbi.val3 = ERS_HIDE;
		Soldier_Map::const_iterator it_sm = sh.info.attack_map.begin();
		if (it_sm != sh.info.attack_map.end()) {
			ebbi.val1 = it_sm->second->nums;
		}

		for (Soldier_Map::const_iterator it_dm = sh.info.defend_map.begin();
				it_dm != sh.info.defend_map.end(); ++it_dm) {
			ebbi.val2 += it_dm->second->nums;
		}

		if (EBFS_PEACE == sh.info.fight_status) {
			ebbi.val1 = ebbi.val2;
			ebbi.val2 = 0;
		}
		if (!this->refreshed_monster_kill_gap_.count(sh.point_id)) {
			ebbi.val3 = ERS_SHOW;
		}
		if ((ebbi.val1 == 0 && ebbi.val2 == 0)
				|| this->refreshed_monster_kill_gap_.count(sh.point_id)) {
			ebbi.val1 = 0;
			ebbi.val2 = 0;
			ebbi.val3 = ERS_HIDE;
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

int Expedition_Scene::init_area_for_beast_and_chest(Int_Hash_Set &gang_id_areavalue) {
	const ExpeditionMapPointCfg_Map &empcm = CONFIG_CACHE_EXPEDITION->get_map_point_conf_map();
	for (ExpeditionMapPointCfg_Map::const_iterator it = empcm.begin(); it != empcm.end(); ++it) {
		if ((Expedition_Build_Type)it->second.type == EBT_BEAST || EBT_CHEST == (Expedition_Build_Type)it->second.type) {
			gang_id_areavalue.insert(it->second.areavalue);
		}
	}
	return 0;
}

Stronghold_Map &Expedition_Scene::map_data() {
	return map_data_;
}

Player_Soldier_Map &Expedition_Scene::player_soldier_map() {
	return player_soldier_map_;
}
//uint32_t &Expedition_Scene::map_gang_in_num() {
//	return map_gang_in_num_;
//}

Int_Uid &Expedition_Scene::point_gang_id(void) {
	return point_gang_id_;
}

uint32_t &Expedition_Scene::map_base_point_num() {
	return map_base_point_num_;
}
uint32_t &Expedition_Scene::cur_line() {
	return cur_line_;
}
Expedition_Scene_Manager *Expedition_Scene::get_manager(void) {
	return manager_;
}
Int_Hash_Set *Expedition_Scene::find_areavalue_by_gang_id(const int64_t gang_id) {
	Role_IntHashSet_Map::iterator it = gang_id_areavalue_.find(gang_id);
	if (it != gang_id_areavalue_.end()) {
		return &(it->second);
	}
	return 0;
}

Stronghold *Expedition_Scene::find_map_data_by_point_id(const int point_id) {
	Stronghold_Map::iterator it = map_data_.find(point_id);
	if (it != map_data_.end()) {
		return &(it->second);
	}
	return 0;
}

const Role_Hash_Set &Expedition_Scene::gang_ids(void) const {
	return gang_ids_;
}

void Expedition_Scene::get_uuids(Role_Hash_Set &uuids) {
	if (!manager_) {
		return ;
	}
	uuids.clear();
	for (Role_Hash_Set::const_iterator it = gang_ids_.begin(); it != gang_ids_.end(); ++it) {
		Expedition_Gang_Rank_Info *egri = manager_->find_gang_rank_info_by_gang_id(*it);
		if (!egri) {
			continue;
		}
		uuids.insert(egri->uuid);
	}
}

int Expedition_Scene::recover_resource(Stronghold_Info &info, const bool need_clear_defend) {
	for (Soldier_Map::const_iterator it = info.attack_map.begin();
			it != info.attack_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	info.attack_map.clear();
	info.attack_list.clear();
//	if (need_clear_defend) {
//		for (Soldier_Map::const_iterator it = info.defend_map.begin();
//				it != info.defend_map.end(); ++it) {
//			POOL_MANAGER->push_soldier_pool(it->second);
//		}
//		info.defend_map.clear();
//		info.defend_list.clear();
//	} else {
		for (Soldier_Map::iterator it_am = info.defending_map.begin();
				it_am != info.defending_map.end(); ++it_am) {
			if (!info.defend_map.count(it_am->first)) {
				info.defend_list.remove(it_am->second);
				MSG_TRACE_ABORT("ERROR: expedition defending_map(%ld) not empty on change_point_force.", it_am->first);
				POOL_MANAGER->push_soldier_pool(it_am->second);
			} else {
				bool is_exist = false;
				for (Soldier_List::iterator it_list = info.defend_list.begin();
						it_list != info.defend_list.end(); ++it_list) {
					if ((**it_list).id == it_am->second->id) {
						is_exist = true;
						break;
					}
				}
				if (!is_exist) {
					info.defend_list.push_front(it_am->second);
				}
			}
		}
		info.defending_map.clear();
//	}

	// error -
//	for (Soldier_Map::iterator it_am = info.attacking_map.begin();
//			it_am != info.attacking_map.end(); ++it_am) {
//		MSG_TRACE_ABORT("ERROR: expedition attacking_map(%ld) not empty on change_point_force.", it_am->first);
//		POOL_MANAGER->push_soldier_pool(it_am->second);
//	}
//	info.attacking_map.clear();
	// -- error
	return 0;
}

int Expedition_Scene::join_in_looking_defend_on_monster_front(Stronghold *sh, const Soldier *soldier_d) {
	if (soldier_d->type != EST_MONSTER
			&& soldier_d->type != EST_M_KNIGHTAGE
			&& soldier_d->type != EST_M_EXPE_ARMY_STRONG
			&& soldier_d->type != EST_M_EXPE_ARMY
			&& soldier_d->type != EST_M_NPC) {
		return -1;
	}
	Soldier_Map::iterator it = sh->info.looking_defend_map.find(soldier_d->id);
	if (it == sh->info.looking_defend_map.end()) {

		// soldier init --
		Soldier *soldier = POOL_MANAGER->pop_soldier_pool();
		if (!soldier) {
			MSG_TRACE_ABORT("ERROR: cant pop soldier.");
			return -1;
		}
		*soldier = *soldier_d;
		// -- soldier end

		sh->info.looking_defend_map.insert(std::make_pair(soldier->id, soldier));
		sh->info.looking_defend_list.push_front(soldier);
	}
	return 0;
}

int Expedition_Scene::drop_out_looking_defend_on_monster(Stronghold *sh, const Soldier *soldier_d) {
	if (soldier_d->type != EST_MONSTER
			&& soldier_d->type != EST_M_KNIGHTAGE
			&& soldier_d->type != EST_M_EXPE_ARMY_STRONG
			&& soldier_d->type != EST_M_EXPE_ARMY
			&& soldier_d->type != EST_M_NPC) {
		return -1;
	}
	Soldier_Map::iterator it = sh->info.looking_defend_map.find(soldier_d->id);
	if (it != sh->info.looking_defend_map.end()) {
		POOL_MANAGER->push_soldier_pool(it->second);
		sh->info.looking_defend_list.remove(it->second);
		sh->info.looking_defend_map.erase(it);
	}
	return 0;
}

int Expedition_Scene::change_territorys(Stronghold *sh) {
	MSG_USER("change_territorys.");
	if (manager_) {
		int point_attack = manager_->find_point_id_by_gang_id(sh->info.attacker.gang_id);
		int point_occupier = manager_->find_point_id_by_gang_id(sh->info.occupier.gang_id);
//		int64_t gang_id_attack = sh->info.attacker.gang_id;
		int64_t gang_id_defend = sh->info.occupier.gang_id;

		for (Soldier_Map::const_iterator it = sh->info.attack_map.begin(); it != sh->info.attack_map.end(); ++it) {
			if (it->second->type == EST_PLAYER) {
				add_occupy_castle_rank(it->second);
			}
		}
		MSG_USER("change_territorys: point_id(%d), type(%d).", sh->point_id, sh->type);
		switch(sh->type) {
		case EBT_GANG_BASE: {
			const Int_Hash_Set *ihs = find_base_territorys(point_occupier);
			if (ihs) {
				for (Int_Hash_Set::const_iterator it = ihs->begin(); it != ihs->end(); ++it) {
					add_base_territorys(point_attack, *it);
				}
			}
			del_base_territorys(sh->point_id);
			add_base_territorys(point_attack, sh->point_id);
			sh->type = EBT_CASTLE_BIG;
			init_gang_base_territory(point_attack);
			clear_gang_info(gang_id_defend);
			manager_->add_gang_to_scene(gang_id_defend);
			break;
		}
		case EBT_CASTLE_BIG:
		case EBT_CASTLE_MEDIUM:
		case EBT_CASTLE_SMALL:
		case EBT_CHECKPOINT: {
			const Expedition_Map_Point_Cfg * empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(sh->point_id);
			if (empc) {
				for (Int_Set::const_iterator it_ter = empc->territory.begin();
						it_ter != empc->territory.end(); ++it_ter) {
					del_base_territorys(point_occupier, *it_ter);
					add_base_territorys(point_attack, *it_ter);
				}
			}
			del_base_territorys(point_occupier, sh->point_id);
			add_base_territorys(point_attack, sh->point_id);
			init_gang_base_territory(point_attack);
			init_gang_base_territory(point_occupier);
			break;
		}
		case EBT_CHEST:
		case EBT_BEAST: {
			if (sh->info.belong_status == EBBS_NEUTRAL) {
				del_base_territorys(point_occupier, sh->point_id);
				add_base_territorys(point_attack, sh->point_id);
				init_gang_base_territory(point_attack);
				init_gang_base_territory(point_occupier);
			}
			break;
		}
		default:
			break;
		}
	} else {
		MSG_TRACE_ABORT("ERROR: expedition point(%d) change force cant find manager.", sh->point_id);
	}
	return 0;
}
int Expedition_Scene::set_base_territorys(const Int_IntHashSet_Map &conf) {
	base_territorys_ = conf;
	return 0;
}

const Int_IntHashSet_Map &Expedition_Scene::get_base_territorys(void) {
	return base_territorys_;
}

int Expedition_Scene::add_base_territorys(const int base_point_id, const int point_id) {
	Int_IntHashSet_Map::iterator it_bt = base_territorys_.find(base_point_id);
	if (it_bt != base_territorys_.end()) {
		it_bt->second.insert(point_id);
		change_force_from_one_to_one(point_id, base_point_id);
	}
	return 0;
}

int Expedition_Scene::del_base_territorys(const int base_point_id) {
	base_territorys_.erase(base_point_id);
	return 0;
}

int Expedition_Scene::del_base_territorys(const int base_point_id, const int point_id) {
	Int_IntHashSet_Map::iterator it_bt = base_territorys_.find(base_point_id);
	if (it_bt != base_territorys_.end()) {
		it_bt->second.erase(point_id);
	}
	return 0;
}

int Expedition_Scene::check_and_init_territorys(Scene_Player *pl) {
	if (!pl || !manager_) {
		return -1;
	}
	for (Stronghold_Map::iterator it = map_data_.begin();
			it != map_data_.end(); ++it) {
		if (EBT_BEAST != it->second.type) {
			continue;
		}
		init_defend_for_beast(pl, it->first);
	}
//	int point_id = manager_->find_point_id_by_gang_id(pl->ganger_detail().gang_id);
//	const Int_Hash_Set *ihs = find_base_territorys(point_id);
//	if (ihs) {
//		for (Int_Hash_Set::const_iterator it = ihs->begin(); it != ihs->end(); ++it) {
//			Stronghold *sh = this->find_map_data_by_point_id(*it);
//			if (!sh
//					|| EBT_BEAST != sh->type) {
//				continue;
//			}
//			init_defend_for_beast(pl, *it);
//		}
//	}
	return 0;
}

int Expedition_Scene::check_and_init_beast(Stronghold *sh) {
	if (!sh
			|| EBT_BEAST != sh->type
			|| EBBS_OCCUPY != sh->info.belong_status
			|| sh->info.occupier.gang_id <= 0) {
		return -1;
	}

	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		MSG_81101303 acv_msg;
		Expedition_Build_Brief_Info ebbi;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()
					&& it->second->player_self()->ganger_detail().gang_id == sh->info.occupier.gang_id) {
			}
			init_defend_for_beast(it->second->player_self(), sh->point_id);
		}
	}

	return 0;
}

int Expedition_Scene::recover_territorys(Scene_Player *pl) {
	if (!pl || !manager_) {
		return -1;
	}
	for (Stronghold_Map::iterator it = map_data_.begin();
			it != map_data_.end(); ++it) {
		if (EBT_BEAST != it->second.type
				|| !it->second.info.role_defend_map_for_beast.count(pl->role_id())) {
			continue;
		}
		recover_defend_for_beast(pl, it->second);
	}
	return 0;
}

int Expedition_Scene::change_force_from_one_to_one(const int point_from, const int point_to) {
	Stronghold *sh_from = this->find_map_data_by_point_id(point_from);
	Stronghold *sh_to = this->find_map_data_by_point_id(point_to);
	if (!sh_from || !sh_to) {
		MSG_TRACE_ABORT("ERRROR: cant find map_data to point(%d) or point(%d).", point_from, point_to);
	}
	if (0 == change_force_from_one_to_one(sh_from, sh_to)) {
//		check_and_init_beast(sh_from);
		active_map_all_data_by_point_id(point_from);
	}
	return 0;
}

int Expedition_Scene::change_force_from_one_to_one(Stronghold *sh_from, const Stronghold *sh_to) {
	if (!sh_from || !sh_to) {
		return -1;
	}
	MSG_USER("sh_from(%d) gang_id(%ld) to sh_to(%d) gang_id(%ld).", sh_from->point_id, sh_from->info.occupier.gang_id, sh_to->point_id, sh_to->info.occupier.gang_id);
	sh_from->info.occupier = sh_to->info.occupier;
	sh_from->info.belong_status = EBBS_OCCUPY;
	sh_from->info.fight_status = EBFS_PEACE;

	for (Soldier_Map::iterator it = sh_from->info.defend_map.begin();
			it != sh_from->info.defend_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	sh_from->info.defend_map.clear();
	sh_from->info.defend_list.clear();

	for (Role_ExpeditionFightInfo_Map::iterator it_info = sh_from->info.role_defend_map_for_beast.begin();
			it_info != sh_from->info.role_defend_map_for_beast.end(); ++it_info) {
		for (Soldier_Map::iterator it = it_info->second.defend_map.begin();
				it != it_info->second.defend_map.end(); ++it) {
			POOL_MANAGER->push_soldier_pool(it->second);
		}
		it_info->second.defend_map.clear();
		it_info->second.defend_list.clear();
		it_info->second.defending_map.clear();
		for (Soldier_Map::iterator it = it_info->second.attack_map.begin();
				it != it_info->second.attack_map.end(); ++it) {
			POOL_MANAGER->push_soldier_pool(it->second);
		}
		it_info->second.attack_map.clear();
		it_info->second.attack_list.clear();
		it_info->second.attacking_map.clear();
	}

	for (Soldier_Map::iterator it = sh_from->info.looking_defend_map.begin();
			it != sh_from->info.looking_defend_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	sh_from->info.looking_defend_map.clear();
	sh_from->info.looking_defend_list.clear();

	const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(sh_from->point_id);
	if (empc) {
		sh_from->init_monster_group(*empc, false);
		sh_from->init_defend_map_occupier();
	}
	// defend_map
	return 0;
}

int Expedition_Scene::clear_gang_info(const int64_t gang_id, const bool from_del) {
	if (gang_id <= 0) {
		return -1;
	}

//	--map_gang_in_num_;
	gang_id_areavalue_.erase(gang_id);

	if (manager_) {
		int point_attack = manager_->find_point_id_by_gang_id(gang_id);
		if (from_del) {
			point_gang_id_.erase(point_attack);
			Stronghold *sh = this->find_map_data_by_point_id(point_attack);
			if (sh) {
				sh->info.occupier.reset();
				sh->info.fight_status = EBFS_PEACE;
				sh->info.belong_status = EBBS_NEUTRAL;
			}
			const Int_Hash_Set *ihs = find_base_territorys(point_attack);
			if (ihs) {
				for (Int_Hash_Set::const_iterator it = ihs->begin(); it != ihs->end(); ++it) {
					sh = this->find_map_data_by_point_id(*it);
					if (!sh) {
						continue;
					}
					sh->info.occupier.reset();
					sh->info.fight_status = EBFS_PEACE;
					sh->info.belong_status = EBBS_NEUTRAL;
					//xxxxxx defend
				}
			}
			clear_occupier_by_gang_id(gang_id);
		}

		base_territorys_[point_attack].clear();
		const Int_Hash_Set *ihs_conf = CONFIG_CACHE_EXPEDITION->get_base_territory_by_point_id_from_conf(point_attack);
		if (ihs_conf) {
			for (Int_Hash_Set::const_iterator it = ihs_conf->begin(); it != ihs_conf->end(); ++it) {
				Stronghold *sh = this->find_map_data_by_point_id(*it);
				if (!sh || sh->info.occupier.gang_id != 0) {
					continue;
				}
				base_territorys_[point_attack].insert(*it);
			}
		}
	}
	gang_ids_.erase(gang_id);
	kick_player_by_gang_id(gang_id);
	return 0;
}

int Expedition_Scene::clear_occupier_by_gang_id(const int64_t gang_id) {
	Stronghold_Map::iterator it = map_data_.begin();
	for (; it != map_data_.end(); ++it) {
		if (it->second.info.occupier.gang_id != gang_id) {
			continue;
		}

		it->second.info.occupier.reset();
		it->second.info.fight_status = EBFS_PEACE;
		it->second.info.belong_status = EBBS_NEUTRAL;
	}
	return 0;
}

int Expedition_Scene::kick_player_by_gang_id(const int64_t gang_id) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()
					&& it->second->player_self()->ganger_detail().gang_id == gang_id) {
				kick_player(it->second->player_self());
			}
		}
	}
	return 0;
}

int Expedition_Scene::kick_player(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}
	pl->update_mover_expedition_info();
	int dest = 0;
	pl->inner_transmit(dest,0,0);
	return 0;
}

void Expedition_Scene::save_tick(void) {
	if (manager_) {
		manager_->save_tick();
	}
}
