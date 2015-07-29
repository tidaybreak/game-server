/*
 * Alchemy_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Alchemy.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "alchemy/Alchemy_Config.h"

const Cards_Info_Map &Config_Cache_Alchemy::hero_critical_config(void) const {
	return hero_critical_config_;
}

const Cards_Info_Map &Config_Cache_Alchemy::hero_gold_config(void) const {
	return hero_gold_config_;
}

const Cards_info *Config_Cache_Alchemy::find_critical_config(uint32_t id) const {
	return get_map_second_pointer_by_key(id, hero_critical_config_);
}

const Cards_info *Config_Cache_Alchemy::find_gold_config(uint32_t id) const {
	return get_map_second_pointer_by_key(id, hero_gold_config_);
}



const int Config_Cache_Alchemy::get_initial_gold_by_level(int role_level) const {
	Int_Int_Map::const_iterator it = initial_gold_map.find(role_level);
	if (it != initial_gold_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}
const int Config_Cache_Alchemy::get_chances_free_by_vip_level(int level) const {
	Int_Int_Map::const_iterator it = chances_free_map.find(level);
	if (it != chances_free_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}
const int Config_Cache_Alchemy::get_chances_pay_by_vip_level(int level) const {
	Int_Int_Map::const_iterator it = chances_pay_map.find(level);
	if (it != chances_pay_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}
const int Config_Cache_Alchemy::get_pay_num_by_count(int count) const {
	Int_Int_Map::const_iterator it = chances_charged_map.find(count);
	if (it != chances_charged_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}

const int Config_Cache_Alchemy::get_pay_type_by_count(int count) const {
	Int_Int_Map::const_iterator it = chances_money_type_map.find(count);
	if (it != chances_money_type_map.end()) {
		return it->second;
	} else {
		return 3;
	}
}
//////////////_souls
const Cards_Info_Map &Config_Cache_Alchemy::hero_souls_critical_config(void) const {
	return hero_souls_critical_config_;
}

const Cards_Info_Map &Config_Cache_Alchemy::hero_souls_gold_config(void) const {
	return hero_souls_gold_config_;
}

const Cards_info *Config_Cache_Alchemy::find_souls_critical_config(uint32_t id) const {
	return get_map_second_pointer_by_key(id, hero_souls_critical_config_);
}

const Cards_info *Config_Cache_Alchemy::find_souls_gold_config(uint32_t id) const {
	return get_map_second_pointer_by_key(id, hero_souls_gold_config_);
}



const int Config_Cache_Alchemy::get_initial_souls_gold_by_level(int role_level) const {
	Int_Int_Map::const_iterator it = initial_souls_gold_map.find(role_level);
	if (it != initial_souls_gold_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}
const int Config_Cache_Alchemy::get_chances_souls_free_by_vip_level(int level) const {
	Int_Int_Map::const_iterator it = chances_souls_free_map.find(level);
	if (it != chances_souls_free_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}
const int Config_Cache_Alchemy::get_chances_souls_pay_by_vip_level(int level) const {
	Int_Int_Map::const_iterator it = chances_souls_pay_map.find(level);
	if (it != chances_souls_pay_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}
const int Config_Cache_Alchemy::get_pay_souls_num_by_count(int count) const {
	Int_Int_Map::const_iterator it = chances_souls_charged_map.find(count);
	if (it != chances_souls_charged_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}

const int Config_Cache_Alchemy::get_pay_souls_type_by_count(int count) const {
	Int_Int_Map::const_iterator it = chances_souls_money_type_map.find(count);
	if (it != chances_souls_money_type_map.end()) {
		return it->second;
	} else {
		return 3;
	}
}

void Config_Cache_Alchemy::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Alchemy");

	refresh_hero_critical_config_cache();
	refresh_hero_gold_config_cache();
	refresh_initial_gold_config_cache();
	refresh_chances_config_cache();
	refresh_chances_chargend_config_cache();
//souls
	refresh_hero_souls_critical_config_cache();
	refresh_hero_souls_gold_config_cache();
	refresh_initial_souls_gold_config_cache();
	refresh_chances_souls_config_cache();
	refresh_chances_souls_chargend_config_cache();
}

void Config_Cache_Alchemy::refresh_hero_critical_config_cache(void) {
	Cards_info base_set;
	const Json::Value& hero_critical_json = CONFIG_INSTANCE->config_json()["alchemy"]["hero_critical"];
	if (hero_critical_json == Json::Value::null || hero_critical_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < hero_critical_json.size(); ++i) {
		base_set.reset();
		const Json::Value& critical_json = hero_critical_json[i];
		base_set.heroID = critical_json["heroID"].asInt();
		base_set.hero_name = critical_json["hero_name"].asString();
		base_set.recruit = critical_json["recruit"].asInt();
		base_set.index = critical_json["Number"].asInt();
		base_set.initial_pro_1 = critical_json["initial_pro_1"].asInt();
		base_set.initial_pro_3 = critical_json["initial_pro_3"].asInt();
		base_set.initial_pro_6 = critical_json["initial_pro_6"].asInt();
		base_set.critical_1 = critical_json["critical_1"].asInt();
		base_set.critical_3 = critical_json["critical_3"].asInt();
		base_set.critical_6 = critical_json["critical_6"].asInt();
		base_set.gold_added = critical_json["gold_added"].asInt();
		base_set.critical_added_3 = critical_json["critical_added_3"].asInt();
		base_set.critical_added_6 = critical_json["critical_added_6"].asInt();
		set_map_second_value_by_key(base_set.heroID, hero_critical_config_, base_set);
	}
}

void Config_Cache_Alchemy::refresh_hero_gold_config_cache(void) {
	Cards_info base_set;
	const Json::Value& hero_gold_json = CONFIG_INSTANCE->config_json()["alchemy"]["hero_gold"];
	if (hero_gold_json == Json::Value::null || hero_gold_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < hero_gold_json.size(); ++i) {
		base_set.reset();
		const Json::Value& gold_json = hero_gold_json[i];
		base_set.heroID = gold_json["heroID"].asInt();
		base_set.hero_name = gold_json["hero_name"].asString();
		base_set.recruit = gold_json["recruit"].asInt();
		base_set.index = gold_json["Number"].asInt();
		base_set.initial_pro_1 = gold_json["initial_pro_1"].asInt();
		base_set.initial_pro_3 = gold_json["initial_pro_3"].asInt();
		base_set.initial_pro_6 = gold_json["initial_pro_6"].asInt();
		base_set.critical_1 = gold_json["critical_1"].asInt();
		base_set.critical_3 = gold_json["critical_3"].asInt();
		base_set.critical_6 = gold_json["critical_6"].asInt();
		base_set.gold_added = gold_json["gold_added"].asInt();
		base_set.critical_added_3 = gold_json["critical_added_3"].asInt();
		base_set.critical_added_6 = gold_json["critical_added_6"].asInt();
		set_map_second_value_by_key(base_set.heroID, hero_gold_config_, base_set);
	}
}

void Config_Cache_Alchemy::refresh_initial_gold_config_cache(void) {
	const Json::Value& initial_gold_json = CONFIG_INSTANCE->config_json()["alchemy"]["initial_gold"];
	if (initial_gold_json == Json::Value::null || initial_gold_json.size() == 0) {
		LOG_ABORT("");
	}
	initial_gold_map.clear();
	for (uint i = 0; i < initial_gold_json.size(); ++i) {
		const Json::Value& gold_json = initial_gold_json[i];
		int level = gold_json["level"].asInt();
		int gold = gold_json["gold"].asInt();
		set_map_second_value_by_key(level, initial_gold_map, gold);
	}
}

void Config_Cache_Alchemy::refresh_chances_config_cache(void) {
	const Json::Value& chances_free_json = CONFIG_INSTANCE->config_json()["alchemy"]["chances"];
	if (chances_free_json == Json::Value::null || chances_free_json.size() == 0) {
		LOG_ABORT("");
	}
	chances_free_map.clear();
	chances_pay_map.clear();
	for (uint i = 0; i < chances_free_json.size(); ++i) {
		const Json::Value& iter_json = chances_free_json[i];
		int vip_level = iter_json["vip_level"].asInt();
		int chances_free = iter_json["chances_free"].asInt();
		int chances_charged = iter_json["chances_charged"].asInt();
		set_map_second_value_by_key(vip_level, chances_free_map, chances_free);
		set_map_second_value_by_key(vip_level, chances_pay_map, chances_charged);
	}
}

void Config_Cache_Alchemy::refresh_chances_chargend_config_cache(void) {
	const Json::Value& chances_chargend_json = CONFIG_INSTANCE->config_json()["alchemy"]["chances_charged"];
	if (chances_chargend_json == Json::Value::null || chances_chargend_json.size() == 0) {
		LOG_ABORT("");
	}
	chances_charged_map.clear();
	chances_money_type_map.clear();
	for (uint i = 0; i < chances_chargend_json.size(); ++i) {
		const Json::Value& iter_json = chances_chargend_json[i];
		int chances = iter_json["chances"].asInt();
		int charged = iter_json["charged"].asInt();
		int type = iter_json["money_sub_type"].asInt();
		set_map_second_value_by_key(chances, chances_charged_map, charged);
		if(type) {
			set_map_second_value_by_key(chances, chances_money_type_map, type);
		}
	}
}
////////////////////souls//////////////////////////////////////
void Config_Cache_Alchemy::refresh_hero_souls_critical_config_cache(void) {
	Cards_info base_set;
	const Json::Value& hero_critical_json = CONFIG_INSTANCE->config_json()["alchemy"]["aima_hero_critical"];
	if (hero_critical_json == Json::Value::null || hero_critical_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < hero_critical_json.size(); ++i) {
		base_set.reset();
		const Json::Value& critical_json = hero_critical_json[i];
		base_set.heroID = critical_json["heroID"].asInt();
		base_set.hero_name = critical_json["hero_name"].asString();
		base_set.recruit = critical_json["recruit"].asInt();
		base_set.index = critical_json["Number"].asInt();
		base_set.initial_pro_1 = critical_json["initial_pro_1"].asInt();
		base_set.initial_pro_3 = critical_json["initial_pro_3"].asInt();
		base_set.initial_pro_6 = critical_json["initial_pro_6"].asInt();
		base_set.critical_1 = critical_json["critical_1"].asInt();
		base_set.critical_3 = critical_json["critical_3"].asInt();
		base_set.critical_6 = critical_json["critical_6"].asInt();
		base_set.gold_added = critical_json["gold_added"].asInt();
		base_set.critical_added_3 = critical_json["critical_added_3"].asInt();
		base_set.critical_added_6 = critical_json["critical_added_6"].asInt();
		set_map_second_value_by_key(base_set.heroID, hero_souls_critical_config_, base_set);
	}
}

void Config_Cache_Alchemy::refresh_hero_souls_gold_config_cache(void) {
	Cards_info base_set;
	const Json::Value& hero_gold_json = CONFIG_INSTANCE->config_json()["alchemy"]["aima_hero_gold"];
	if (hero_gold_json == Json::Value::null || hero_gold_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < hero_gold_json.size(); ++i) {
		base_set.reset();
		const Json::Value& gold_json = hero_gold_json[i];
		base_set.heroID = gold_json["heroID"].asInt();
		base_set.hero_name = gold_json["hero_name"].asString();
		base_set.recruit = gold_json["recruit"].asInt();
		base_set.index = gold_json["Number"].asInt();
		base_set.initial_pro_1 = gold_json["initial_pro_1"].asInt();
		base_set.initial_pro_3 = gold_json["initial_pro_3"].asInt();
		base_set.initial_pro_6 = gold_json["initial_pro_6"].asInt();
		base_set.critical_1 = gold_json["critical_1"].asInt();
		base_set.critical_3 = gold_json["critical_3"].asInt();
		base_set.critical_6 = gold_json["critical_6"].asInt();
		base_set.gold_added = gold_json["gold_added"].asInt();
		base_set.critical_added_3 = gold_json["critical_added_3"].asInt();
		base_set.critical_added_6 = gold_json["critical_added_6"].asInt();
		set_map_second_value_by_key(base_set.heroID, hero_souls_gold_config_, base_set);
	}
}

void Config_Cache_Alchemy::refresh_initial_souls_gold_config_cache(void) {
	const Json::Value& initial_gold_json = CONFIG_INSTANCE->config_json()["alchemy"]["initial_gold"];
	if (initial_gold_json == Json::Value::null || initial_gold_json.size() == 0) {
		LOG_ABORT("");
	}
	initial_souls_gold_map.clear();
	for (uint i = 0; i < initial_gold_json.size(); ++i) {
		const Json::Value& gold_json = initial_gold_json[i];
		int level = gold_json["level"].asInt();
		int gold = gold_json["aima"].asInt();
		set_map_second_value_by_key(level, initial_souls_gold_map, gold);
	}
}

void Config_Cache_Alchemy::refresh_chances_souls_config_cache(void) {
	const Json::Value& chances_free_json = CONFIG_INSTANCE->config_json()["alchemy"]["chances"];
	if (chances_free_json == Json::Value::null || chances_free_json.size() == 0) {
		LOG_ABORT("");
	}
	chances_souls_free_map.clear();
	chances_souls_pay_map.clear();
	for (uint i = 0; i < chances_free_json.size(); ++i) {
		const Json::Value& iter_json = chances_free_json[i];
		int vip_level = iter_json["vip_level"].asInt();
		int chances_free = iter_json["aima_chances_free"].asInt();
		int chances_charged = iter_json["aima_chances_charged"].asInt();
		set_map_second_value_by_key(vip_level, chances_souls_free_map, chances_free);
		set_map_second_value_by_key(vip_level, chances_souls_pay_map, chances_charged);
	}
}

void Config_Cache_Alchemy::refresh_chances_souls_chargend_config_cache(void) {
	const Json::Value& chances_chargend_json = CONFIG_INSTANCE->config_json()["alchemy"]["chances_charged"];
	if (chances_chargend_json == Json::Value::null || chances_chargend_json.size() == 0) {
		LOG_ABORT("");
	}
	chances_souls_charged_map.clear();
	chances_souls_money_type_map.clear();
	for (uint i = 0; i < chances_chargend_json.size(); ++i) {
		const Json::Value& iter_json = chances_chargend_json[i];
		int chances = iter_json["chances"].asInt();
		int charged = iter_json["aima_charged"].asInt();
		int type = iter_json["aima_money_sub_type"].asInt();
		set_map_second_value_by_key(chances, chances_souls_charged_map, charged);
		if(type) {
			set_map_second_value_by_key(chances, chances_souls_money_type_map, type);
		}
	}
}
