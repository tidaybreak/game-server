/*
 * Expedition_Scene_Struct.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: xiaoliang
 */

#include "Expedition_Scene_Struct.h"
#include "expedition/Expedition_Config.h"
#include "Expedition_Scene_Def.h"
#include "Config_Cache_Expedition.h"
#include "Config_Cache_NPC.h"
#include "expedition/Expedition_Def.h"
#include "Pool_Manager.h"
#include "Expedition_Global_Scene_Manager.h"
#include "expedition/Expedition_Func.h"


int Expedition_Force::serialize(Block_Buffer &w) const {
	w.write_int64(server_uuid);
	w.write_string(server_name);
	w.write_int64(gang_id);
	w.write_string(gang_name);
	return 0;
}

int Expedition_Force::deserialize(Block_Buffer &r) {
	r.read_int64(server_uuid);
	r.read_string(server_name);
	r.read_int64(gang_id);
	r.read_string(gang_name);
	return 0;
}
Soldier::Soldier(void) {
	reset();
}

void Soldier::reset(void) {
	id = 0;
	occupier.reset();
	point_id = 0;
	role_id = 0; //角色ID //非玩家类型,改表示NPCid
	role_name.clear();//角色名
	type = 0;//类型; 0.玩家,1.中立驻守怪,2.公会骑士团,3.精锐远征军,4.远征军,5.公会守卫npc
	force = 0;//战斗力
	blood = 0;//剩余血量
	head_id = 0;//头像ID
	level = 0; //等级
	nums = 0;
	nums_battle = 0;
	exit = false;
	hero_info.reset();
}

void Expedition_Fight_Info::reset(void) {
	for (Soldier_Map::iterator it = defend_map.begin();
			it != defend_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	defend_map.clear();
	defend_list.clear();
	for (Soldier_Map::iterator it = attack_map.begin();
			it != attack_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	attack_map.clear();
	attack_list.clear();
	defending_map.clear();
	attacking_map.clear();
	battle_finish_time = Time_Value::zero;

	for (Soldier_Map::iterator it = looking_defend_map.begin();
			it != looking_defend_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	looking_defend_map.clear();
	for (GangID_SoldierMap_Map::iterator it_ldm = looking_attack_map.begin();
			it_ldm != looking_attack_map.end(); ++it_ldm) {
		for (Soldier_Map::iterator it = it_ldm->second.begin();
				it != it_ldm->second.end(); ++it) {
			POOL_MANAGER->push_soldier_pool(it->second);
		}
	}
	looking_attack_map.clear();
	looking_defend_list.clear();
	looking_attack_list.clear();
	fight_status = EBFS_PEACE;

	has_complete = false;
}

Stronghold_Info::Stronghold_Info(void) {
	reset();
}

void Stronghold_Info::reset(void) {
	occupier.reset();
	attacker.reset();
	fight_status = EBFS_PEACE;
	belong_status = EBBS_UNINIT;

	for (Soldier_Map::iterator it = defend_map.begin();
			it != defend_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	defend_map.clear(); //防守队列
	for (Soldier_Map::iterator it = attack_map.begin();
			it != attack_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	attack_map.clear();//攻占队列
	defend_list.clear();
	attack_list.clear();
//	npc_soldier_aid = 0;

	defending_map.clear();
	attacking_map.clear();
	battle_finish_time = Time_Value::zero;

	for (Soldier_Map::iterator it = looking_defend_map.begin();
			it != looking_defend_map.end(); ++it) {
		POOL_MANAGER->push_soldier_pool(it->second);
	}
	looking_defend_map.clear();
	for (GangID_SoldierMap_Map::iterator it_ldm = looking_attack_map.begin();
			it_ldm != looking_attack_map.end(); ++it_ldm) {
		for (Soldier_Map::iterator it = it_ldm->second.begin();
				it != it_ldm->second.end(); ++it) {
			POOL_MANAGER->push_soldier_pool(it->second);
		}
	}
	looking_attack_map.clear();
	looking_defend_list.clear();
	looking_attack_list.clear();

	defend_id_map.clear();

	for (Role_ExpeditionFightInfo_Map::iterator it = role_defend_map_for_beast.begin();
			it != role_defend_map_for_beast.end(); ++it) {
		it->second.reset();
	}
	role_defend_map_for_beast.clear();
}

int Stronghold_Info::serialize(Block_Buffer &w) const {
	occupier.serialize(w);
//	attacker.serialize(w);
//	int8_t fight_status; // 0(和平状态) 1(攻占状态)
//	int8_t belong_status; // 0.未被初始化状态, 1.中立, 2.占领

//	 ; //防守队列
	uint16_t len = 0;
	for (Soldier_Map::const_iterator it = defend_map.begin();
			it != defend_map.end(); ++it) {
		if (it->second->type == EST_PLAYER) {
			continue;
		}
		++len;
	}
	w.write_uint16(len);
	for (Soldier_Map::const_iterator it = defend_map.begin();
			it != defend_map.end(); ++it) {
		if (it->second->type == EST_PLAYER) {
			continue;
		}
		w.write_int64(it->second->role_id);
		w.write_int16(it->second->nums);
	}
//	Soldier_Map attack_map; //攻占队列
//	Soldier_List defend_list;
//	Soldier_List attack_list;
	//	uint32_t npc_soldier_aid;//npc怪的id自增长

//	Soldier_Map defending_map;
//	Soldier_Map attacking_map;
//	Time_Value battle_finish_time;

//	Soldier_Map looking_defend_map;
//	GangID_SoldierMap_Map looking_attack_map;
//	Soldier_List looking_defend_list;
//	GangID_SoldierList_Map looking_attack_list;
	return 0;
}

int Stronghold_Info::deserialize(Block_Buffer &r)  {
	occupier.deserialize(r);

	uint16_t len = 0;
	int64_t i_64 = 0;
	int i_32 = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		r.read_int32(i_32);
		defend_id_map.insert(std::make_pair(i_64, i_32));
	}
	return 0;
}

int8_t Stronghold_Info::get_status(void) const {
	switch(belong_status) {
	case EBBS_UNINIT: {
		return EBS_UNINIT;
	}
	case EBBS_NEUTRAL: {
		if (fight_status == EBFS_PEACE) {
			return EBS_NEUTRAL_PEACE;
		} else {
			return EBS_NEUTRAL_FIGHTING;
		}
	}
	case EBBS_OCCUPY: {
		if (fight_status == EBFS_PEACE) {
			return EBS_OCCUPY_PEACE;
		} else {
			return EBS_OCCUPY_FIGHTING;
		}
		break;
	}
	default:
		break;
	}
	return EBS_UNINIT;
}

Stronghold::Stronghold(void) {
	reset();
}

void Stronghold::reset(void) {
	point_id = 0; //建筑ID
	type = EBT_UNDEFINE; //类型	1.基地  2.关卡	3.大城堡	4.中城堡	5.小城堡	6.宝箱区	7.野兽区
//	bewrite.clear(); //描述
//	location.reset(); //位置
//	abut.clear(); //邻近的建筑ID
//	attribution = 0; //占领所属建筑ID
//	territory.clear();//建筑领土
//	areavalue = 0;

	info.reset();//据点信息

	win_attack_people.reset();
}

int Stronghold::serialize(Block_Buffer &w) const {
	w.write_uint32(point_id);
	w.write_int8((int8_t)type);

	info.serialize(w);
	return 0;
}

int Stronghold::deserialize(Block_Buffer &r) {
	r.read_uint32(point_id);
	int8_t tp = 0;
	r.read_int8(tp);
	type = (Expedition_Build_Type)tp;

	info.deserialize(r);
	return 0;
}

void Stronghold::make_from_cfg(const Expedition_Map_Point_Cfg &conf) {
	point_id = conf.point_id;
//	bewrite = conf.bewrite;
	type = (Expedition_Build_Type)conf.type;
//	abut.insert(conf.abut.begin(), conf.abut.end());
//	attribution = conf.attribution;
//	territory.insert(conf.territory.begin(), conf.territory.end());
//	const Int_Hash_Set *ihs = CONFIG_CACHE_EXPEDITION->get_base_territory_by_point_id_from_conf(point_id);
//	if (ihs) {
//		territory.insert(ihs->begin(), ihs->end());
//	}
	areavalue = conf.areavalue;
	if (type != EBT_GANG_BASE) {
		info.fight_status = EBFS_PEACE;
		info.belong_status = EBBS_NEUTRAL;
	} else {
		info.fight_status = EBFS_PEACE;
		info.belong_status = EBBS_UNINIT;
	}
	init_monster_group(conf);
}

void Stronghold::make_to_build_brief_info(Expedition_Build_Brief_Info &ebbi) const {
	ebbi.point_id = point_id;
	ebbi.type = type;

	ebbi.gang_id = this->info.occupier.gang_id;
	//据点状态 0.未被初始化状态, 1.中立(和平状态), 2.中立(攻占状态), 11.占领(和平状态), 12.占领(攻占状态)
	ebbi.state = this->info.get_status();
//	it_sm = this->info.attack_map.begin();
//	if (it_sm != this->info.attack_map.end()) {
//		ebbi.val1 = it_sm->second->nums;
//	}
//	Soldier_Map::const_iterator it_sm = this->info.defend_map.begin();
//	if (it_sm != this->info.defend_map.end()) {
//		ebbi.val2 = it_sm->second->nums;
//	}
	ebbi.val1 = (int16_t)this->info.attack_map.size();
	ebbi.val2 = (int16_t)this->info.defend_map.size();
}

void Stronghold::init_monster_group(const Expedition_Map_Point_Cfg &conf, const bool from_cfg) {
	if (EBT_BEAST == (Expedition_Build_Type)conf.type || EBT_CHEST == (Expedition_Build_Type)conf.type) {
		return;
	}
	Int_Int_Map monster_group = conf.monster_group;
	if (!from_cfg) {
		const Expedition_Common_Cfg *conf =
				CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::GANG_BASE_DEFEND_MONSTER_GROUP);
		if(conf && conf->value) {
			int amount = conf->val_2;
			if (amount < 1) {
				amount = 1;
			}
			monster_group.clear();
			monster_group.insert(std::make_pair(conf->value, amount));
		}
	}
	for (Int_Int_Map::const_iterator it = monster_group.begin();
			it != monster_group.end(); ++it) {
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
			LOG_ABORT("ERROR: expedition point_id(%d) monster_gfp roup(%d) nums = 0.", point_id, it->first);
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
							Int_Double id = *it_pvv;
							modify_prop_for_monster_modify(id);
							soldier->blood = (int)id.value;
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
		info.defend_map.insert(std::make_pair(soldier->id, soldier));
		info.defend_list.push_back(soldier);
		Soldier *soldier_look = POOL_MANAGER->pop_soldier_pool();
		if (!soldier_look) {
			MSG_TRACE_ABORT("ERROR: cant pop soldier!");
			continue;
		}
		*soldier_look = *soldier;
		info.looking_defend_map.insert(std::make_pair(soldier_look->id, soldier_look));
		info.looking_defend_list.push_front(soldier_look);
	}
}

void Stronghold::init_defend_map_occupier(void) {
	for (Soldier_Map::iterator it = info.defend_map.begin();
			it != info.defend_map.end(); ++it) {
		it->second->occupier = this->info.occupier;
	}
}


// for db save --
void Expedition_Scene_Demage_Rank_Data::serialize(Block_Buffer &w) const {
	w.write_string(role_name);
	w.write_int64(role_id);
	w.write_int64(gang_id);
	w.write_string(gang_name);
	w.write_int64(uuid);
	w.write_string(server_name);
	w.write_int32(force);
	w.write_int32(level);
	w.write_double(demage);
}

void Expedition_Scene_Demage_Rank_Data::deserialize(Block_Buffer &r) {
	r.read_string(role_name);
	r.read_int64(role_id);
	r.read_int64(gang_id);
	r.read_string(gang_name);
	r.read_int64(uuid);
	r.read_string(server_name);
	r.read_int32(force);
	r.read_int32(level);
	r.read_double(demage);
}

void Expedition_Scene_Data::reset(void) {
	map_data_.clear();
	point_gang_id_.clear();
	map_base_point_num_ = 0;
	cur_line_ = 0;
	demage_rank_.clear();
	award_time_ = Time_Value::zero;
	settle_time_ = -1;
	occupy_castle_rank_.clear();
	refreshed_drop_or_material_.clear();
	refreshed_monster_timing_.clear();
	refreshed_monster_kill_gap_.clear();
}

int Expedition_Scene_Data::serialize(Block_Buffer &w) const {
	w.write_uint16(map_data_.size());
	for (Stronghold_Map::const_iterator it = map_data_.begin();//地图的总数据; 是所有据点(大中小城堡,关卡,宝箱点,怪物点)的数据;据点里面包含所有数据(含有攻防的玩家,状态等等)
			it != map_data_.end(); ++it) {
		w.write_uint32(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(point_gang_id_.size());
	for (Int_Uid::const_iterator it = point_gang_id_.begin();
			it != point_gang_id_.end(); ++it) {
		w.write_int32(it->first);
		w.write_int64(it->second);
	}
	w.write_uint32(map_base_point_num_);//地图数据中基地的总数
	w.write_uint32(cur_line_); //场景的当前线
	award_time_.serialize(w);
	w.write_int32(settle_time_);

	w.write_uint16(occupy_castle_rank_.size());
	for (Role_ExpeditionOccupyCastleData_Map::const_iterator it = occupy_castle_rank_.begin();
			it != occupy_castle_rank_.end(); ++it) {
		it->second.serialize(w);
	}

	w.write_uint16(demage_rank_.size());
	for (Role_ExpeditionSceneDemageRankData_Map::const_iterator it = demage_rank_.begin();
			it != demage_rank_.end(); ++it) {
		w.write_int64(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(refreshed_drop_or_material_.size());
	for (Point_ExpeditionDropOrMaterial::const_iterator it = refreshed_drop_or_material_.begin(); it != refreshed_drop_or_material_.end(); ++it) {
		w.write_int32(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(refreshed_monster_timing_.size());
	for (Int_Time_Map::const_iterator it = refreshed_monster_timing_.begin();
			it != refreshed_monster_timing_.end(); ++it) {
		w.write_int32(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(refreshed_monster_kill_gap_.size());
	for (Int_Time_Map::const_iterator it = refreshed_monster_kill_gap_.begin();
			it != refreshed_monster_kill_gap_.end(); ++it) {
		w.write_int32(it->first);
		it->second.serialize(w);
	}
	return 0;
}

int Expedition_Scene_Data::deserialize(Block_Buffer &r) {
	uint16_t len = 0;
	Stronghold sh;
	uint32_t ui_32 = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_uint32(ui_32);
		sh.deserialize(r);
		map_data_.insert(std::make_pair(ui_32, sh));
	}

	int i_32 = 0;
	int64_t i_64 = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(i_32);
		r.read_int64(i_64);
		point_gang_id_.insert(std::make_pair(i_32, i_64));
	}
	r.read_uint32(map_base_point_num_);
	r.read_uint32(cur_line_);
	award_time_.deserialize(r);
	r.read_int32(settle_time_);

	r.read_uint16(len);
	Expedition_Occupy_Castle_Data eocd;
	for (uint16_t i = 0; i < len; ++i) {
		eocd.reset();
		eocd.deserialize(r);
		occupy_castle_rank_.insert(std::make_pair(eocd.role_id, eocd));
	}

	Expedition_Scene_Demage_Rank_Data esdrd;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		esdrd.reset();
		r.read_int64(i_64);
		esdrd.deserialize(r);
		demage_rank_.insert(std::make_pair(i_64, esdrd));
	}

	Expedition_Drop_Or_Material edom;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(i_32);
		edom.reset();
		edom.deserialize(r);
		refreshed_drop_or_material_.insert(std::make_pair(i_32, edom));
	}

	Time_Value tv(Time_Value::zero);
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(i_32);
		tv.deserialize(r);
		refreshed_monster_timing_[i_32] = tv;
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(i_32);
		tv.deserialize(r);
		refreshed_monster_kill_gap_[i_32] = tv;
	}

	return 0;
}

void Expedition_Manager_Data::reset(void) {
	inter_ = 0;

	opened_uuid_opentime_map_.clear();//服务器的开服时间
	gang_id_uuid_map_.clear();

	last_reset_time_ = Time_Value::zero;
	first_gang_rank_.clear();

	has_init_ = false;
	uuid_has_init_.clear();

	gang_rank_map_.clear();
	uuid_gang_rank_map_.clear();

	gang_rank_sync_success_.clear();
	gang_rank_sync_new_vec_.clear();
	gang_id_point_id_.clear();
	gang_id_cur_line_.clear();
	uuid_gang_id_point_id_.clear();
	uuid_gang_id_cur_line_.clear();

	line_scene_map_.clear();
	uuid_line_scene_map_.clear();
}
int Expedition_Manager_Data::serialize(Block_Buffer &w) const {
	w.write_uint32(inter_);

	w.write_uint16(opened_uuid_opentime_map_.size());
	for (Role_Id_Time_Map::const_iterator it = opened_uuid_opentime_map_.begin();//服务器的开服时间
			it != opened_uuid_opentime_map_.end(); ++it) {
		w.write_int64(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(gang_id_uuid_map_.size());
	for (RoleID_RoleID_Map::const_iterator it = gang_id_uuid_map_.begin();
			it != gang_id_uuid_map_.end(); ++it) {
		w.write_int64(it->first);
		w.write_int64(it->second);
	}

	last_reset_time_.serialize(w);
	w.write_uint16(first_gang_rank_.size());
	for (Role_UInt_Map::const_iterator it = first_gang_rank_.begin();
			it != first_gang_rank_.end(); ++it) {
		w.write_int64(it->first);
		w.write_uint32(it->second);
	}

	w.write_bool(has_init_);
	w.write_uint16(uuid_has_init_.size());
	for (Role_Bool_Map::const_iterator it = uuid_has_init_.begin();
			it != uuid_has_init_.end(); ++it) {
		w.write_int64(it->first);
		w.write_bool(it->second);
	}

	w.write_uint16(gang_rank_map_.size());
	for (Gang_ExpeditionGangRankInfo_Map::const_iterator it = gang_rank_map_.begin();
			it != gang_rank_map_.end(); ++it) {
		w.write_int64(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(uuid_gang_rank_map_.size());
	for (Role_GangExpeditionGangRankInfoMap_Map::const_iterator it_igrm = uuid_gang_rank_map_.begin();
			it_igrm != uuid_gang_rank_map_.end(); ++it_igrm) {
		w.write_int64(it_igrm->first);
		w.write_uint16(gang_rank_map_.size());
		for (Gang_ExpeditionGangRankInfo_Map::const_iterator it = it_igrm->second.begin();
				it != it_igrm->second.end(); ++it) {
			w.write_int64(it->first);
			it->second.serialize(w);
		}
	}

	w.write_uint16(gang_rank_sync_success_.size());
	for (Role_Hash_Set::iterator it = gang_rank_sync_success_.begin();//公会排名获取成功服务器uuid列表
			it != gang_rank_sync_success_.end(); ++it) {
		w.write_int64(*it);
	}

	w.write_uint16(gang_rank_sync_new_vec_.size());
	for (Role_Vec::const_iterator it = gang_rank_sync_new_vec_.begin();
			it != gang_rank_sync_new_vec_.end(); ++it) {
		w.write_int64(*it);
	}

	w.write_uint16(gang_id_point_id_.size());
	for (Role_UInt_Map::const_iterator it = gang_id_point_id_.begin();
			it != gang_id_point_id_.end(); ++it) {
		w.write_int64(it->first);
		w.write_uint32(it->second);
	}

	w.write_uint16(gang_id_cur_line_.size());
	for (Role_Int_Map::const_iterator it = gang_id_cur_line_.begin();
			it != gang_id_cur_line_.end(); ++it) {
		w.write_int64(it->first);
		w.write_int32(it->second);
	}

	w.write_uint16(uuid_gang_id_point_id_.size());
	for (Role_RoleUIntMap_Map::const_iterator it = uuid_gang_id_point_id_.begin();
			it != uuid_gang_id_point_id_.end(); ++it) {
		w.write_int64(it->first);
		w.write_uint16(it->second.size());
		for (Role_UInt_Map::const_iterator it_t = it->second.begin();
				it_t != it->second.end(); ++it_t) {
			w.write_int64(it_t->first);
			w.write_uint32(it_t->second);
		}
	}

	w.write_uint16(uuid_gang_id_cur_line_.size());
	for (Role_RoleIntMap_Map::const_iterator it = uuid_gang_id_cur_line_.begin();
			it != uuid_gang_id_cur_line_.end(); ++it) {
		w.write_int64(it->first);
		w.write_uint16(it->second.size());
		for (Role_Int_Map::const_iterator it_t = it->second.begin();
				it_t != it->second.end(); ++it_t) {
			w.write_int64(it_t->first);
			w.write_int32(it_t->second);
		}
	}
	 ;
	// scene--
	w.write_uint16(line_scene_map_.size());
	for (Line_ExpeditionSceneData_Map::const_iterator it = line_scene_map_.begin();
			it != line_scene_map_.end(); ++it) {
		w.write_uint32(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(uuid_line_scene_map_.size());
	for (UUID_LineExpeditionSceneData_Map_Map::const_iterator it_ulsm = uuid_line_scene_map_.begin();
			it_ulsm != uuid_line_scene_map_.end(); ++it_ulsm) {
		w.write_int64(it_ulsm->first);
		w.write_uint16(it_ulsm->second.size());
		for (Line_ExpeditionSceneData_Map::const_iterator it = it_ulsm->second.begin();
				it != it_ulsm->second.end(); ++it) {
			w.write_uint32(it->first);
			it->second.serialize(w);
		}
	}
	return 0;
}
int Expedition_Manager_Data::deserialize(Block_Buffer &r) {
	r.read_uint32(inter_);

	uint16_t len = 0;
	int64_t i_64 = 0;
	Time_Value tv = Time_Value::zero;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		tv.deserialize(r);
		opened_uuid_opentime_map_.insert(std::make_pair(i_64, tv));
	}

	int64_t i_64_2 = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		r.read_int64(i_64_2);
		gang_id_uuid_map_.insert(std::make_pair(i_64, i_64_2));
	}

	last_reset_time_.deserialize(r);

	uint32_t ui_32 = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		r.read_uint32(ui_32);
		first_gang_rank_.insert(std::make_pair(i_64, ui_32));
	}

	r.read_bool(has_init_);
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		bool hi = false;
		r.read_int64(i_64);
		r.read_bool(hi);
		uuid_has_init_[i_64] = hi;
	}

	r.read_uint16(len);
	Expedition_Gang_Rank_Info egri;
	for (uint16_t i = 0; i < len; ++i) {
		egri.reset();
		r.read_int64(i_64);
		egri.deserialize(r);
		gang_rank_map_.insert(std::make_pair(i_64, egri));
	}

	int64_t i_64_t = 0;
	r.read_uint16(len);
	uint16_t len_t = 0;
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64_t);
		r.read_uint16(len_t);
		for (uint16_t j = 0; j < len_t; ++j) {
			r.read_int64(i_64);
			egri.deserialize(r);
			uuid_gang_rank_map_[i_64_t].insert(std::make_pair(i_64, egri));
		}
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		gang_rank_sync_success_.insert(i_64);
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		gang_rank_sync_new_vec_.push_back(i_64);
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		r.read_uint32(ui_32);
		gang_id_point_id_.insert(std::make_pair(i_64, ui_32));
	}

	int i_32 = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		r.read_int32(i_32);
		gang_id_cur_line_.insert(std::make_pair(i_64, i_32));
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64_t);
		r.read_uint16(len_t);
		for (uint16_t j = 0; j < len_t; ++j) {
			r.read_int64(i_64);
			r.read_uint32(ui_32);
			uuid_gang_id_point_id_[i_64_t].insert(std::make_pair(i_64, ui_32));
		}
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64_t);
		r.read_uint16(len_t);
		for (uint16_t j = 0; j < len_t; ++j) {
			r.read_int64(i_64);
			r.read_int32(i_32);
			uuid_gang_id_cur_line_[i_64_t].insert(std::make_pair(i_64, i_32));
		}
	}

	Expedition_Scene_Data esd;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		esd.reset();
		r.read_uint32(ui_32);
		esd.deserialize(r);
		line_scene_map_.insert(std::make_pair(ui_32, esd));
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(i_64);
		r.read_uint16(len_t);
		for (uint16_t j = 0; j < len_t; ++j) {
			esd.reset();
			r.read_uint32(ui_32);
			esd.deserialize(r);
			uuid_line_scene_map_[i_64].insert(std::make_pair(ui_32, esd));
		}
	}
	return 0;
}

void Expedition_Global_Manager_Data::reset(void) {
	opened_uuid_opentime_map_.clear();

	uuid_inter_map_.clear();

	opentime_.clear();

	mmuid_.reset();

//	inter_manager_data_map_.clear();
}

int Expedition_Global_Manager_Data::serialize(Block_Buffer &w) const {
	w.write_uint16(opened_uuid_opentime_map_.size());
	for (Role_Id_Time_Map::const_iterator it = opened_uuid_opentime_map_.begin();
			it != opened_uuid_opentime_map_.end(); ++it) {
		w.write_int64(it->first);
		it->second.serialize(w);
	}

	w.write_uint16(uuid_inter_map_.size());
	for (Role_Int_Map::const_iterator it = uuid_inter_map_.begin();
			it != uuid_inter_map_.end(); ++it) {
		w.write_int64(it->first);
		w.write_int32(it->second);
	}

	w.write_uint16(opentime_.size());
	for (Int_Time_Map::const_iterator it = opentime_.begin();
			it != opentime_.end(); ++it) {
		w.write_int32(it->first);
		it->second.serialize(w);
	}

	mmuid_.serialize(w);

//	w.write_uint16(inter_manager_data_map_.size());
//	for (Inter_ExpeditionManagerData_Map::const_iterator it = inter_manager_data_map_.begin();
//			it != inter_manager_data_map_.end(); ++it) {
//		w.write_uint32(it->first);
//		it->second.serialize(w);
//	}
	return 0;
}

int Expedition_Global_Manager_Data::deserialize(Block_Buffer &r) {
	uint16_t len = 0;
	int64_t uuid = 0;
	Time_Value tv = Time_Value::zero;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(uuid);
		tv.deserialize(r);
		opened_uuid_opentime_map_.insert(std::make_pair(uuid, tv));
	}

	int inter = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int64(uuid);
		r.read_int32(inter);
		uuid_inter_map_.insert(std::make_pair(uuid, inter));
	}

	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		r.read_int32(inter);
		tv.deserialize(r);
		opentime_.insert(std::make_pair(inter, tv));
	}

	mmuid_.deserialize(r);

//	uint32_t inter_u = 0;
//	Expedition_Manager_Data emd;
//	r.read_uint16(len);
//	for (uint16_t i = 0; i < len; ++i) {
//		emd.reset();
//		r.read_uint32(inter_u);
//		emd.deserialize(r);
//		inter_manager_data_map_.insert(std::make_pair(inter_u, emd));
//	}
	return 0;
}

void Expedition_Occupy_Castle_Data::reset(void) {
	role_id = 0;
	gang_id = 0;
	server_uuid = 0;
	role_name.clear();
	gang_name.clear();
	server_name.clear();
	level = 0;
	force = 0;
	nums = 0;
	get_nums = 0;
}

int Expedition_Occupy_Castle_Data::serialize(Block_Buffer &w) const {
	w.write_int64(role_id);
	w.write_int64(gang_id);
	w.write_int64(server_uuid);
	w.write_string(role_name);
	w.write_string(gang_name);
	w.write_string(server_name);
	w.write_int32(level);
	w.write_int32(force);
	w.write_int16(nums);
	w.write_int16(get_nums);
	return 0;
}

int Expedition_Occupy_Castle_Data::deserialize(Block_Buffer &r) {
	r.read_int64(role_id);
	r.read_int64(gang_id);
	r.read_int64(server_uuid);
	r.read_string(role_name);
	r.read_string(gang_name);
	r.read_string(server_name);
	r.read_int32(level);
	r.read_int32(force);
	r.read_int16(nums);
	r.read_int16(get_nums);
	return 0;
}

void Expedition_Occupy_Castle_Data::set_msg_data(Expedition_Occupy_Castle &eoc) const {
	eoc.role_name = role_name;
	eoc.gang_name = gang_name;
	eoc.level = level;
	eoc.force = force;
	eoc.nums = nums;
}
// -- for db save


void Expedition_Drop_Or_Material::serialize(Block_Buffer &w) const {
	this->birth_time.serialize(w);
	w.write_uint16(had_collected.size());
	for (Role_Hash_Set::const_iterator it = had_collected.begin(); it != had_collected.end(); ++it) {
		w.write_int64(*it);
	}
}

void Expedition_Drop_Or_Material::deserialize(Block_Buffer &r) {
	this->birth_time.deserialize(r);
	uint16_t len = 0;
	r.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		int64_t role_id = 0;
		r.read_int64(role_id);
		had_collected.insert(role_id);
	}
}
