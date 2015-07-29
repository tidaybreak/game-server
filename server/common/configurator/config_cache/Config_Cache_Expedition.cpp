/*
 * Expedition_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Base_Struct.h"
#include "Config_Cache_Expedition.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "expedition/Expedition_Config.h"
#include "expedition_scene/Expedition_Scene_Def.h"


const Expedition_Map_Point_Cfg *Config_Cache_Expedition::get_map_point_conf_by_id(uint32_t id) const {
	return get_map_second_pointer_by_key(id, map_point_conf_map_);
}

const ExpeditionGangHomeCfg_Map& Config_Cache_Expedition::gang_home_conf_map(void) const {
	return gang_home_conf_map_;
}

const Expedition_Gang_Home_Cfg *Config_Cache_Expedition::get_gang_home_conf_by_rank(uint32_t rank) const {
	return get_map_second_pointer_by_key(rank, gang_home_conf_map_);
}

const ExpeditionMapPointCfg_Map &Config_Cache_Expedition::get_map_point_conf_map(void) const {
	return map_point_conf_map_;
}

const Expedition_Common_Cfg *Config_Cache_Expedition::get_common_conf_by_key(uint32_t key) const {
	return get_map_second_pointer_by_key(key, common_conf_map_);
}
int Config_Cache_Expedition::get_territory_area_by_point_id(const int point_id, Int_Hash_Set &territorys, Int_Hash_Set &used_point_ids) const {
	used_point_ids.insert(point_id);
	const Expedition_Map_Point_Cfg *pconf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
	if(pconf) {
		territorys.insert(pconf->areavalue);
		for (Int_Set::const_iterator it_ter = pconf->territory.begin(); it_ter != pconf->territory.end(); ++it_ter) {
			if (used_point_ids.count(*it_ter)) {
				continue;
			}
			get_territory_area_by_point_id(*it_ter, territorys, used_point_ids);
		}
	}
	return 0;
}

int Config_Cache_Expedition::get_territory_by_point_id(const int point_id, Int_Hash_Set &territorys) const {
	const Expedition_Map_Point_Cfg *pconf = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
	if(pconf) {
		territorys.insert(point_id);
		for (Int_Set::const_iterator it_ter = pconf->territory.begin(); it_ter != pconf->territory.end(); ++it_ter) {
			if (territorys.count(*it_ter)) {
				continue;
			}
			get_territory_by_point_id(*it_ter, territorys);
		}
	} else {
		LOG_ABORT("ERROR: Expedition conf cant find point(%d)!", point_id);
	}
	return 0;
}

const Int_Hash_Set *Config_Cache_Expedition::get_base_territory_by_point_id_from_conf(const int point_id) const {
	return get_map_second_pointer_by_key(point_id, base_territorys_conf_);
}

const Int_IntHashSet_Map &Config_Cache_Expedition::base_territorys_conf(void) const {
	return base_territorys_conf_;
}

int Config_Cache_Expedition::find_monster_group_force(const int monster_point_id) {
	Int_Int_Map::const_iterator it = monster_group_force_.find(monster_point_id);
	if (it != monster_group_force_.end()) {
		return it->second;
	}
	return 0;
}

const Int_Int_Map &Config_Cache_Expedition::monster_group_force(void) {
	return monster_group_force_;
}

int Config_Cache_Expedition::get_percent_for_beast_by_count(const int count) {
	for (ExpeditionBoxData_Vec::const_iterator it = beast_award_.begin();
			it != beast_award_.end(); ++it) {
		if (count >= it->min && count <= it->max) {
			return it->percent;
		}
	}
	return 0;
}

int Config_Cache_Expedition::get_percent_for_chest_by_count(const int count) {
	int count_bak = count;
	if (count_bak == 0) {
		count_bak = 1;
	}
	for (ExpeditionBoxData_Vec::const_iterator it = chest_award_.begin();
			it != chest_award_.end(); ++it) {
		if (count_bak >= it->min && count_bak <= it->max) {
			return it->percent;
		}
	}
	return 0;
}

const Expedition_Award_Demage_Cfg & Config_Cache_Expedition::demage_award(void) {
	return demage_award_;
}

const Item_Info_Vec *Config_Cache_Expedition::find_settle_award(const int type) const {
	Int_Item_Info_Vec_Map::const_iterator it = settle_award_.find(type + 40000);
	if (it != settle_award_.end()) {
		return &(it->second);
	}
	return 0;
}

const Expedition_Daily_Award_Big_Data *Config_Cache_Expedition::get_big_settle_award_by_nums(const int nums) {
	int n = 0;
	for (ExpeditionDailyAwardBigData_Map::const_iterator it = settle_award_big_.begin();
			it != settle_award_big_.end(); ++it) {
		if (nums >= it->second.nums) {
			if (n < it->second.nums) {
				n = it->second.nums;
			}
		}
	}
	ExpeditionDailyAwardBigData_Map::const_iterator it = settle_award_big_.find(n);
	if (it != settle_award_big_.end()) {
		return &(it->second);
	}
	return 0;
}

const ExpeditionDailyAwardBigData_Map &Config_Cache_Expedition::settle_award_big(void) {
	return settle_award_big_;
}

int Config_Cache_Expedition::get_settle_award_decay(const int line) {
	int decay = 100;
	for (Int_Int_Std_Map::const_iterator it = settle_award_decay_.begin();
			it != settle_award_decay_.end(); ++it) {
		if (line == it->first) {
			return it->second;
		}
		if (line < it->first) {
			return it->second;
		}
		decay = it->second;
	}
	return decay;
}

const Item_Info_Vec *Config_Cache_Expedition::find_demage_rank_award(const int rank) const {
	Int_Item_Info_Vec_Map::const_iterator it = demage_rank_award_.find(rank);
	if (it != demage_rank_award_.end()) {
		return &(it->second);
	}
	return 0;
}

const Item_Info_Vec *Config_Cache_Expedition::find_occupy_castle_rank_award(const int nums) const {
	Int_ItemInfoVec_Map::const_iterator it = occupy_castle_rank_award_.find(nums);
	if (it != occupy_castle_rank_award_.end()) {
		return &(it->second);
	}
	return 0;
}

Item_Info_Vec Config_Cache_Expedition::find_occupy_castle_rank_award(const int nums, const int had_get_nums) const {
	Item_Info_Vec iiv;
	if (nums <= had_get_nums) {
		return iiv;
	}
	typedef boost::unordered_map<int, Item_Info> ID_ItemInfo_Map;
	ID_ItemInfo_Map iiim;
	Item_Info_Vec::iterator it_iiv = iiv.begin();
	for (Int_ItemInfoVec_Map::const_iterator it = occupy_castle_rank_award_.begin();
			it != occupy_castle_rank_award_.end(); ++it) {
		if (nums < it->first) {
			break;
		}
		if (it->first <= had_get_nums) {
			continue;
		}
		for (Item_Info_Vec::const_iterator it_award = it->second.begin();
				it_award != it->second.end(); ++it_award) {
			ID_ItemInfo_Map::iterator it_iiim = iiim.find(it_award->id);
			if (it_iiim != iiim.end()) {
				it_iiim->second.amount += it_award->amount;
			} else {
				iiim.insert(std::make_pair(it_award->id, *it_award));
			}
		}
	}

	for (ID_ItemInfo_Map::const_iterator it = iiim.begin(); it != iiim.end(); ++it) {
		iiv.push_back(it->second);
	}
	return iiv;
}

const Config_Cache_Expedition::Int_ItemInfoVec_Map &Config_Cache_Expedition::occupy_castle_rank_award(void) {
	return occupy_castle_rank_award_;
}

const Expedition_World_Level_Monster_Modify *Config_Cache_Expedition::get_monster_modify_by_world_level(const int world_level) {
	if (world_level < 1) {
		return NULL;
	}
	const Expedition_World_Level_Monster_Modify *ewlmm = 0;
	for (Expe_WorldLevelMonsterModify_Map::const_iterator it = world_level_monster_modify_.begin();
			it != world_level_monster_modify_.end(); ++it) {
		if (world_level < it->first) {
			break;
		}
		ewlmm = &(it->second);
	}
	return ewlmm;
}

bool Config_Cache_Expedition::is_demage_count_time(void) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	int h = dt.hour() * 100 + dt.minute();

	for (ExpeditionDemageCountTime_Vec::const_iterator it = demage_count_time_cfg_.begin();
			it != demage_count_time_cfg_.end(); ++it) {
		if (h >= it->low && h <= it->high) {
			return true;
		}
	}

	return false;
}

bool Config_Cache_Expedition::is_demage_settle_time(int &settle_time) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	int h = dt.hour() * 60 + dt.minute();

	if (demage_settle_time_cfg_.count(h)) {
		settle_time = h;
		return true;
	}
	return false;
}

int Config_Cache_Expedition::get_next_settle_time(const int settle_time) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	int h = dt.hour() * 60 + dt.minute();

	int nex = h;
	if (settle_time == h) {
		if (demage_settle_time_cfg_.size() == 1) {
			return Time_Value::ONE_DAY_IN_SECS;
		} else {
			for (Int_Set::const_iterator it = demage_settle_time_cfg_.begin();
					it != demage_settle_time_cfg_.end(); ++it) {
				if (nex < *it) {
					nex = *it;
					break;
				}
			}
			if (nex == h) {
				for (Int_Set::const_iterator it = demage_settle_time_cfg_.begin();
						it != demage_settle_time_cfg_.end(); ++it) {
					if (nex == *it) {
						break;
					}
					nex = *it;
				}
			}
		}
	} else if (demage_settle_time_cfg_.count(h)) {
		return 0;
	} else {
		if (demage_settle_time_cfg_.size() == 1) {
			nex = *demage_settle_time_cfg_.begin();
		} else {
			for (Int_Set::const_iterator it = demage_settle_time_cfg_.begin();
					it != demage_settle_time_cfg_.end(); ++it) {
				if (nex < *it) {
					nex = *it;
					break;
				}
			}
			if (nex == h) {
				for (Int_Set::const_iterator it = demage_settle_time_cfg_.begin();
						it != demage_settle_time_cfg_.end(); ++it) {
					if (nex == *it) {
						break;
					}
					nex = *it;
				}
			}
		}
	}

	int gap = nex - h;
	if (gap < 0) {
		gap += Time_Value::ONE_DAY_HOURS * Time_Value::ONE_HOURS_MINUTE;
	}

	return gap * Time_Value::ONE_MINUTE_IN_SECS;
}

void Config_Cache_Expedition::refresh_monster_timing(const Time_Value &now, Int_Hash_Set &need_refresh_points) {
	need_refresh_points.clear();
	Date_Time dt(now);
	int h = dt.hour() * 60 + dt.minute();

	for (Expedition_Refresh_Monster_Vec::const_iterator it = refresh_monster_cfg_.begin(); it != refresh_monster_cfg_.end(); ++it) {
		if (!it->refresh_time.count(h) || it->monster_vec.empty()) {
			continue;
		}
		uint16_t pos = rand() % it->monster_vec.size();
		need_refresh_points.insert(it->monster_vec[pos]);
	}
}

void Config_Cache_Expedition::refresh_drop_or_material(const Time_Value &now, Int_Hash_Set &need_refresh_points, Int_Hash_Set &need_clear_points) {
	need_refresh_points.clear();
	Date_Time dt(now);
	int h = dt.hour() * 60 + dt.minute();

	for (Expedition_Refresh_Drop_Or_Material_Vec::const_iterator it = refresh_drop_or_material_cfg_.begin(); it != refresh_drop_or_material_cfg_.end(); ++it) {
		if (!it->refresh_time.count(h) || it->monster_vec.empty()) {
			continue;
		}

		uint16_t pos = rand() % it->monster_vec.size();
		for (uint16_t i = 0; i < it->monster_vec.size(); ++i) {
			if (i == pos) {
				continue;
			}
			need_clear_points.insert(it->monster_vec[i]);
		}
		need_refresh_points.insert(it->monster_vec[pos]);
	}
}

int Config_Cache_Expedition::find_monster_refresh_gap_by_point_id(const int point_id) {
	for (Expedition_Refresh_Monster_Kill_Gap_Vec::const_iterator it = refresh_monster_by_kill_gap_cfg_.begin();
			it != refresh_monster_by_kill_gap_cfg_.end(); ++it) {
		if (it->monster_vec.count(point_id) && !it->refresh_time.empty()) {
			uint16_t pos = rand() % it->refresh_time.size();
			return it->refresh_time[pos];
		}
	}
	return 15;
}

bool Config_Cache_Expedition::is_drop_need_boradcast(const int item_id) {
	return 0 != drop_need_broadcast_cfg_.count(item_id);
}

void Config_Cache_Expedition::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Expedition");
	refresh_map_point_conf_cache();
	refresh_gang_home_conf_cache();
	refresh_common_conf_cache();
	refresh_monster_group();
	refresh_award();
}

void Config_Cache_Expedition::refresh_map_point_conf_cache(void) {
	map_point_conf_map_.clear();
	base_points_conf_.clear();
	Expedition_Map_Point_Cfg value;
	const Json::Value& value_json = CONFIG_INSTANCE->config_json()["expedition"]["map"];
	if (value_json == Json::Value::null || value_json.size() == 0) {
		LOG_ABORT("not expedition map.json");
	}
	for (uint i = 0; i < value_json.size(); ++i) {
		value.reset();
		const Json::Value& json = value_json[i];
		value.point_id = json["point_id"].asInt();//建筑ID
		value.name = json["name_str"].asString();//描述
		value.bewrite = json["bewrite_str"].asString();//描述
		value.type = json["type"].asInt();//类型
		value.x = json["x"].asInt();//
		value.y = json["y"].asInt();//

		if(!value.type) {
			continue;
			LOG_ABORT("地图建筑类型不能为空或者0");
		}

		const Json::Value& abut_conf = json["abut"];//邻近的建筑ID
		if(abut_conf != Json::Value::null && abut_conf.size()) {
			for (uint j = 0; j < abut_conf.size(); ++j) {
				value.abut.insert(abut_conf[j].asInt());
			}
		}

		value.attribution = json["attribution"].asInt();;//占领所属建筑ID

		const Json::Value& territory_conf = json["territory"];//建筑领土
		if(territory_conf != Json::Value::null && territory_conf.size()) {
			for (uint j = 0; j < territory_conf.size(); ++j) {
				value.territory.insert(territory_conf[j].asInt());
			}
		}
		value.areavalue = json["areaValue"].asInt();
		Json_Utility::assign_int_int_map(json["monster_group"], value.monster_group);

		Json_Utility::assign_int_vec(json["drop"], value.drops);

		set_map_second_value_by_key(value.point_id, map_point_conf_map_, value);

		if (EBT_GANG_BASE == value.type) {
			base_points_conf_.insert(value.point_id);
		}
	}
	check_territorys();
	init_point_data();
}

void Config_Cache_Expedition::refresh_gang_home_conf_cache(void) {
	gang_home_conf_map_.clear();
	Expedition_Gang_Home_Cfg value;
	const Json::Value& value_json = CONFIG_INSTANCE->config_json()["expedition"]["gang_home"];
	if (value_json == Json::Value::null || value_json.size() == 0) {
		LOG_ABORT("not expedition gang_home.json");
	}
	for (uint i = 0; i < value_json.size(); ++i) {
		value.reset();
		const Json::Value& json = value_json[i];
		value.rank = json["rank"].asInt();//
		value.point_id = json["point_id"].asInt();//建筑ID

		const Expedition_Map_Point_Cfg *pconf = get_map_point_conf_by_id(value.point_id);
		if(!pconf) {
			LOG_ABORT("公会基地排名安放配置没有对应的建筑ID:%d", value.point_id);
		}
		if(pconf->type != EBT_GANG_BASE) {
			continue;
			LOG_ABORT("公会基地排名安放配置对应的建筑ID:%d , 不是基地类型", value.point_id);
		}

		set_map_second_value_by_key(value.rank, gang_home_conf_map_, value);
	}
}

void Config_Cache_Expedition::refresh_common_conf_cache(void) {
	common_conf_map_.clear();
	Expedition_Common_Cfg value;
	const Json::Value& value_json = CONFIG_INSTANCE->config_json()["expedition"]["common"];
	if (value_json == Json::Value::null || value_json.size() == 0) {
		LOG_ABORT("not expedition common.json");
	}
	for (uint i = 0; i < value_json.size(); ++i) {
		value.reset();
		const Json::Value& json = value_json[i];
		value.key = json["key"].asInt();//配置键值
		if (json["value"].isArray()) {
			if (json["value"].size() > 1) {
				value.value = json["value"][0u].asInt();//配置值
				value.val_2 = json["value"][1u].asInt();//配置值
			}
		} else {
			value.value = json["value"].asInt();//配置值
		}

		set_map_second_value_by_key(value.key, common_conf_map_, value);
	}
}

void Config_Cache_Expedition::refresh_monster_group(void) {
	monster_group_force_.clear();
	const Json::Value& value_json = CONFIG_INSTANCE->config_json()["expedition"]["monster"];
	if (value_json == Json::Value::null || value_json.size() == 0) {
		LOG_ABORT("not expedition monster_group.json");
	}
	int force = 0;
	int group_id = 0;
	for (uint i = 0; i < value_json.size(); ++i) {
		const Json::Value& json = value_json[i];
		group_id = json["group_id"].asInt();//配置键值
		force = json["force"].asInt();//配置值

		set_map_second_value_by_key(group_id, monster_group_force_, force);
	}
}

void Config_Cache_Expedition::refresh_award(void) {
	beast_award_.clear();
	chest_award_.clear();
	demage_award_.reset();
	settle_award_.clear();
	settle_award_big_.clear();
	demage_rank_award_.clear();
	settle_award_decay_.clear();
	demage_count_time_cfg_.clear();
	demage_settle_time_cfg_.clear();
	occupy_castle_rank_award_.clear();
	world_level_monster_modify_.clear();
	refresh_monster_cfg_.clear();
	refresh_drop_or_material_cfg_.clear();
	refresh_monster_by_kill_gap_cfg_.clear();
	drop_need_broadcast_cfg_.clear();

	Expedition_Box_Data ebd;
	Expedition_Daily_Award_Big_Data edabd;
	const Json::Value &buff_cfg = CONFIG_INSTANCE->config_json()["expedition"]["award"];
	for (Json::Value::const_iterator cfg_it = buff_cfg.begin();
					cfg_it != buff_cfg.end(); ++cfg_it) {
		int id = (*cfg_it)["id"].asInt();
		if (id >= 10000 && id < 20000) {
			if (!(*cfg_it)["count"].isArray() || (*cfg_it)["count"].size() < 2) {
				LOG_ABORT("ERROR: expedition id(%d) size need 2 which is (%d).", (*cfg_it)["count"].size());
			}
			ebd.reset();
			ebd.percent = (*cfg_it)["percent"].asInt();
			ebd.min = (*cfg_it)["count"][0u].asInt();
			ebd.max = (*cfg_it)["count"][1u].asInt();
			beast_award_.push_back(ebd);
		} else if (id >= 20000 && id < 30000) {
			if (!(*cfg_it)["count"].isArray() || (*cfg_it)["count"].size() < 2) {
				LOG_ABORT("ERROR: expedition id(%d) size need 2 which is (%d).", (*cfg_it)["count"].size());
			}
			ebd.reset();
			ebd.percent = (*cfg_it)["percent"].asInt();
			ebd.min = (*cfg_it)["count"][0u].asInt();
			ebd.max = (*cfg_it)["count"][1u].asInt();
			chest_award_.push_back(ebd);
		} else if (id == 30001) {
			if (!(*cfg_it)["count"].isArray() || (*cfg_it)["count"].size() < 3) {
				LOG_ABORT("ERROR: expedition id(%d) size is need 3 which is(%d).", id, (*cfg_it)["count"].size());
			}
			demage_award_.gold = (*cfg_it)["count"][0u].asInt();
			demage_award_.souls = (*cfg_it)["count"][1u].asInt();
			demage_award_.modify = (*cfg_it)["count"][2u].asInt();
		} else if (id >= 40000 && id < 50000) {
			if (id < 41000) {
				Item_Info_Vec iiv;
				Json_Utility::assign_item_vec((*cfg_it)["count"], iiv);
				settle_award_.insert(std::make_pair(id, iiv));
			} else {
				int line = (*cfg_it)["count"].asInt();
				int decay = (*cfg_it)["percent"].asInt();
				if (line == 0) {
					line = INT_MAX;
				}
				settle_award_decay_.insert(std::make_pair(line, decay));
			}
		} else if (id >= 50000 && id < 60000) {
			if (!(*cfg_it)["count"].isArray() || (*cfg_it)["count"].size() < 2) {
				LOG_ABORT("ERROR: expedition id(%d) size is need 2 which is(%d).", id, (*cfg_it)["count"].size());
			}
			edabd.reset();
			edabd.nums = (*cfg_it)["count"][0u].asInt();
			edabd.amount = (*cfg_it)["count"][1u].asInt();
			Json_Utility::assign_item_vec((*cfg_it)["percent"], edabd.award);
			settle_award_big_.insert(std::make_pair(edabd.nums, edabd));
		} else if (id >= 60000 && id < 70000) {
			int rank = (*cfg_it)["count"].asInt();
			Item_Info_Vec iiv;
			Json_Utility::assign_item_vec((*cfg_it)["percent"], iiv);
			demage_rank_award_.insert(std::make_pair(rank, iiv));
		} else if (id >= 70000 && id < 80000) {
			if ((*cfg_it)["count"].isArray() && !(*cfg_it)["count"].isNull()) {
				uint16_t array_size = (*cfg_it)["count"].size();
				for (uint16_t i = 0; i < array_size; ++i) {
					if (!(*cfg_it)["count"][i].isArray() || (*cfg_it)["count"][i].isNull() || (*cfg_it)["count"][i].size() < 2) {
						LOG_ABORT("ERROR: expedition demage count time.");
					}
					Expedition_Demage_Count_Time edct;
					edct.low = (*cfg_it)["count"][i][0u].asInt();
					edct.high = (*cfg_it)["count"][i][1u].asInt();

					// check --
					int low_h = edct.low / 100;
					int low_m = edct.low % 100;
					low_h %= 24;
					low_m %= 61;
					if (low_h == 23 && low_m == 60) {
						low_h = 0;
						low_m = 0;
					}
					int high_h = edct.high / 100;
					int high_m = edct.high % 100;
					high_h %= 24;
					high_m %= 61;
					if (high_h == 23 && high_m > 60) {
						high_m = 60;
					}
					edct.low = low_h * 100 + low_m;
					edct.high = high_h * 100 + high_m;
					if (edct.low > edct.high) {
						Expedition_Demage_Count_Time edct1 = edct;
						edct1.high = 2360;
						demage_count_time_cfg_.push_back(edct1);
						edct.low = 0;
						demage_count_time_cfg_.push_back(edct);
					} else {
						demage_count_time_cfg_.push_back(edct);
					}
					// -- check
				}
				if (demage_count_time_cfg_.empty()) {
					LOG_ABORT("ERROR: expedition demage count time empty.");
				}
			} else {
				LOG_ABORT("ERROR: expedition demage count time empty or now array.");
			}
			if ((*cfg_it)["percent"].isArray() && !(*cfg_it)["percent"].isNull()) {
				Int_Set need_clear, need_insert, bak;
				Json_Utility::assign_set_list((*cfg_it)["percent"], bak);

				for (Int_Set::iterator it_dstc = bak.begin();
						it_dstc != bak.end(); ++it_dstc) {
					int h = *it_dstc / 100;
					int m = *it_dstc % 100;
					h %= 24;
					m %= 61;
					if (h == 23 && m == 60) {
						h = 0;
						m = 0;
					}
					h = h * 100 + m;
					if (*it_dstc != h) {
						need_clear.insert(*it_dstc);
						need_insert.insert(h);
					}
				}
				for (Int_Set::const_iterator it_nc = need_clear.begin();
						it_nc != need_clear.end(); ++it_nc) {
					bak.erase(*it_nc);
				}
				bak.insert(need_insert.begin(), need_insert.end());
				if (bak.empty()) {
					LOG_ABORT("ERROR: expedition demage settle time empty.");
				}

				for (Int_Set::iterator it_nc = bak.begin();
						it_nc != bak.end(); ++it_nc) {
					demage_settle_time_cfg_.insert(*it_nc / 100 * 60 + *it_nc % 100);
				}
			} else {
				LOG_ABORT("ERROR: expedition demage settle time empty or now array.");
			}
		} else if (id >= 90000 && id < 100000) {
			int rank = (*cfg_it)["count"].asInt();
			Item_Info_Vec iiv;
			Json_Utility::assign_item_vec((*cfg_it)["percent"], iiv);
			occupy_castle_rank_award_.insert(std::make_pair(rank, iiv));
		} else if (id >= 100000 && id < 110000) {
			int world_level = (*cfg_it)["count"].asInt();
			Expedition_World_Level_Monster_Modify monster_modify;
			Json_Utility::assign_int_array((*cfg_it)["percent"], monster_modify.modify, WORLD_LEVEL_MONSTER_MODIFY);
			set_map_second_value_by_key(world_level, world_level_monster_modify_, monster_modify);
		} else if (id >= 110000 && id < 120000) {
			if (id < 111000) {
				Expedition_Refresh_Drop_Or_Material erm;
				erm.id = id;
				Json_Utility::assign_int_vec((*cfg_it)["count"], erm.monster_vec);
				{
					Int_Set need_clear, need_insert, bak;
					Json_Utility::assign_set_list((*cfg_it)["percent"], bak);

					for (Int_Set::iterator it_dstc = bak.begin();
							it_dstc != bak.end(); ++it_dstc) {
						int h = *it_dstc / 100;
						int m = *it_dstc % 100;
						h %= 24;
						m %= 61;
						if (h == 23 && m == 60) {
							h = 0;
							m = 0;
						}
						h = h * 100 + m;
						if (*it_dstc != h) {
							need_clear.insert(*it_dstc);
							need_insert.insert(h);
						}
					}
					for (Int_Set::const_iterator it_nc = need_clear.begin();
							it_nc != need_clear.end(); ++it_nc) {
						bak.erase(*it_nc);
					}
					bak.insert(need_insert.begin(), need_insert.end());
					if (bak.empty()) {
						MSG_USER("ERROR: expedition refresh monster time empty.");
						continue;
					}

					for (Int_Set::iterator it_nc = bak.begin();
							it_nc != bak.end(); ++it_nc) {
						erm.refresh_time.insert(*it_nc / 100 * 60 + *it_nc % 100);
					}
				}
				refresh_drop_or_material_cfg_.push_back(erm);
			} else if (id == 111000) {
				Json_Utility::assign_set_list((*cfg_it)["count"], drop_need_broadcast_cfg_);
			}
		} else if (id >= 120000 && id < 130000) {
			if (id < 121000) {
				Expedition_Refresh_Monster erm;
				erm.id = id;
				Json_Utility::assign_int_vec((*cfg_it)["count"], erm.monster_vec);
				{
					Int_Set need_clear, need_insert, bak;
					Json_Utility::assign_set_list((*cfg_it)["percent"], bak);

					for (Int_Set::iterator it_dstc = bak.begin();
							it_dstc != bak.end(); ++it_dstc) {
						int h = *it_dstc / 100;
						int m = *it_dstc % 100;
						h %= 24;
						m %= 61;
						if (h == 23 && m == 60) {
							h = 0;
							m = 0;
						}
						h = h * 100 + m;
						if (*it_dstc != h) {
							need_clear.insert(*it_dstc);
							need_insert.insert(h);
						}
					}
					for (Int_Set::const_iterator it_nc = need_clear.begin();
							it_nc != need_clear.end(); ++it_nc) {
						bak.erase(*it_nc);
					}
					bak.insert(need_insert.begin(), need_insert.end());
					if (bak.empty()) {
//						MSG_USER("ERROR: expedition refresh monster time empty.");
						continue;
					}

					for (Int_Set::iterator it_nc = bak.begin();
							it_nc != bak.end(); ++it_nc) {
						erm.refresh_time.insert(*it_nc / 100 * 60 + *it_nc % 100);
					}
				}
				refresh_monster_cfg_.push_back(erm);
			} else {
				Expedition_Refresh_Monster_Kill_Gap erm;
				erm.id = id;
				Json_Utility::assign_set_list((*cfg_it)["count"], erm.monster_vec);
				Json_Utility::assign_int_vec((*cfg_it)["percent"], erm.refresh_time);
				refresh_monster_by_kill_gap_cfg_.push_back(erm);
			}
		}

	}

}

int Config_Cache_Expedition::check_territorys(void) {
//	Int_Int_Map territory_to_occupy;
//	Int_Hash_Set beast_and_chest;
	for (ExpeditionMapPointCfg_Map::const_iterator it = map_point_conf_map_.begin();
			it != map_point_conf_map_.end(); ++it) {
		for (Int_Set::const_iterator it_ter = it->second.territory.begin();
				it_ter != it->second.territory.end(); ++it_ter) {
			const Expedition_Map_Point_Cfg *empc = get_map_point_conf_by_id(*it_ter);
			if (!empc) {
				LOG_ABORT("ERROR: expedition cant find point(%d)'s territory(%d).", it->first, *it_ter);
			}
			if (empc->type == EBT_CHEST || empc->type == EBT_BEAST) {
				LOG_ABORT("ERROR: expedition point(%d)'s territory(%d) is beast or chest.", it->first, *it_ter);
			}
//			territory_to_occupy.insert(std::make_pair(*it_ter, it->first));
		}
//		if (EBT_BEAST ==it->second.type
//				|| it->second.type == EBT_CHEST) {
//			beast_and_chest.insert(it->first);
//		}
	}

//	for (Int_Hash_Set::const_iterator it = beast_and_chest.begin(); it != beast_and_chest.end(); ++it) {
//		if (territory_to_occupy.count(*it)) {
//			LOG_ABORT("ERROR: expedition point(%d) is beast or chest but attribution.", *it);
//		}
//	}
	return 0;
}

int Config_Cache_Expedition::init_point_data(void) {
	for (ExpeditionMapPointCfg_Map::const_iterator it = map_point_conf_map_.begin();
			it != map_point_conf_map_.end(); ++it) {
		switch (it->second.type) {
		case EBT_GANG_BASE: {
			Int_IntHashSet_Map::iterator it_pit = base_territorys_conf_.find(it->first);
			if (it_pit == base_territorys_conf_.end()) {
				base_territorys_conf_.insert(std::make_pair(it->first, Int_Hash_Set()));
				it_pit = base_territorys_conf_.find(it->first);
				if (it_pit == base_territorys_conf_.end()) {
					LOG_ABORT("ERROR: Expedition conf cant insert unordered_map!");
				}
			}
			for (Int_Set::const_iterator it_ter = it->second.territory.begin();
					it_ter != it->second.territory.end(); ++it_ter) {
				it_pit->second.insert(*it_ter);
			}
			break;
		}
		case EBT_CASTLE_BIG:
		case EBT_CASTLE_MEDIUM:
		case EBT_CASTLE_SMALL:
		case EBT_CHECKPOINT: {
			if (base_points_conf_.count(it->first)
					|| 0 == base_points_conf_.count(it->second.attribution)) { // 城堡、关卡级
				continue;
			}
	//		MSG_USER("it->second.attribution:(%d), it->first(%d)", it->second.attribution, it->first);
			Int_IntHashSet_Map::iterator it_pit = base_territorys_conf_.find(it->second.attribution);
			if (it_pit == base_territorys_conf_.end()) {
				base_territorys_conf_.insert(std::make_pair(it->second.attribution, Int_Hash_Set(it->first)));
				it_pit = base_territorys_conf_.find(it->second.attribution);
				if (it_pit == base_territorys_conf_.end()) {
					LOG_ABORT("ERROR: Expedition conf cant insert unordered_map!");
				}
			}
			get_territory_by_point_id(it->first, it_pit->second);
			break;
		}
		default:
			break;
		}
	}
	return 0;
}
