/*
 * ConfigCacheFashion.cpp
 *
 *  Created on: 2014年9月10日
 *      Author: root
 */

#include "Config_Cache_Fashion.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Role.h"
#include "fighter/Fighter_Def.h"

void Config_Cache_Fashion::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Fashion");

	refresh_base_map();
	refresh_soul_map();
	refresh_reinforced_map();
}

void Config_Cache_Fashion::refresh_base_map(void) {

	const Json::Value& json = CONFIG_INSTANCE->config_json()["fashion"]["fashion_lvl_base_new"];
	Fashion_Base_Cfg cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		cfg.reset();
		cfg.id = (*it)["fashion_id"].asInt();
		cfg.part = (*it)["fashion_part"].asInt();
		cfg.exp = (*it)["exp"].asInt();
		cfg.suit_id = (*it)["suit_id"].asInt();

		if (!(*it)["prop_hide"].isArray()) continue;
		for (Json::Value::const_iterator prop_it = (*it)["prop_hide"].begin();
				prop_it != (*it)["prop_hide"].end(); ++prop_it) {
			if (!(*prop_it).isArray() || 2 != (*prop_it).size()) {
				LOG_ABORT("prop_hide error, fashion_id=%d", cfg.id);
			}
			cfg.hidden_props_map[(*prop_it)[0u].asInt()] = (*prop_it)[1].asDouble();
		}
		if (!(*it)["prop_wire"].isArray()) continue;
		for (Json::Value::const_iterator prop_it = (*it)["prop_wire"].begin();
				prop_it != (*it)["prop_wire"].end(); ++prop_it) {
			if (!(*prop_it).isArray() || 2 != (*prop_it).size()) {
				LOG_ABORT("prop_wire error, fashion_id=%d", cfg.id);
			}
			cfg.wore_props_map[(*prop_it)[0u].asInt()] = (*prop_it)[1].asDouble();
		}
		if (!(*it)["avatars"].isArray())
			LOG_ABORT("fashion no avatar config, id=%d", cfg.id);
		for (Json::Value::const_iterator avatar_it = (*it)["avatars"].begin();
				avatar_it != (*it)["avatars"].end(); ++avatar_it) {
			if (!(*avatar_it).isArray() || 3 != (*avatar_it).size()) {
				LOG_ABORT("avatars error, fashion_id=%d", cfg.id);
			}
			cfg.fashion_avatar_map[(*avatar_it)[0u].asInt()][(*avatar_it)[1].asInt()] = (*avatar_it)[2].asInt();
		}
		fashion_base_map_[cfg.id] = cfg;
	}
}

void Config_Cache_Fashion::refresh_soul_map(void) {

	const Json::Value& json = CONFIG_INSTANCE->config_json()["fashion"]["fashion_soul"];
	Fashion_Soul_Cfg cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		cfg.reset();
		cfg.lv = (*it)["soul_lvl"].asInt();
		cfg.exp = (*it)["soul_exp"].asInt();

		if (!(*it)["soul_prop"].isArray()) continue;
		for (Json::Value::const_iterator prop_it = (*it)["soul_prop"].begin();
				prop_it != (*it)["soul_prop"].end(); ++prop_it) {
			if (!(*prop_it).isArray() || 3 != (*prop_it).size()) {
				LOG_ABORT("fashion_soul error, lv=%d", cfg.lv);
			}
			cfg.props_map[(*prop_it)[0u].asInt()] = (*prop_it)[1].asDouble();
		}

		fashion_soul_map_[cfg.lv] = cfg;
	}
}

void Config_Cache_Fashion::refresh_reinforced_map(void) {

	const Json::Value& json = CONFIG_INSTANCE->config_json()["fashion"]["fashion_reinforced"];
	Fashion_Reinforced_Cfg cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		cfg.reset();
		cfg.lv = (*it)["reinforced_lvl"].asInt();
		cfg.part = (*it)["fashion_part"].asInt();
		cfg.percent = (*it)["reinforced_perce"].asInt();
		cfg.coin_cost = (*it)["reinforced_cost"].asInt();
		cfg.suc_rate = (*it)["reinforcedl_rate"].asInt();
		cfg.exp_add = (*it)["power_add"].asInt();
		cfg.exp_max = (*it)["power_max"].asInt();

		if (!(*it)["reinforced_prop"].isArray()) continue;
		for (Json::Value::const_iterator prop_it = (*it)["reinforced_prop"].begin();
				prop_it != (*it)["reinforced_prop"].end(); ++prop_it) {
			if (!(*prop_it).isArray() || 2 != (*prop_it).size()) {
				LOG_ABORT("fashion_reinforced prop error, lv=%d, part=%d", cfg.lv, cfg.part);
			}
			cfg.wore_props_map[(*prop_it)[0u].asInt()] = (*prop_it)[1].asDouble();
		}

		if (!(*it)["reinforced_cost_item"].isArray()) continue;
		cfg.item_cost.val_1 = (*it)["reinforced_cost_item"][0u].asInt();
		cfg.item_cost.val_2 = (*it)["reinforced_cost_item"][1].asInt();

		fashion_reinforced_map_[cfg.part][cfg.lv] = cfg;
	}
}

const Fashion_Base_Cfg* Config_Cache_Fashion::get_fashion_base_cfg(int id) const {

	return get_map_second_pointer_by_key(id, fashion_base_map_);
}

const Fashion_Soul_Cfg* Config_Cache_Fashion::get_fashion_soul_cfg(int id) const {

	return get_map_second_pointer_by_key(id, fashion_soul_map_);
}

const Fashion_Reinforced_Cfg* Config_Cache_Fashion::get_fashion_reinforced_cfg(int lv, int part)const {

	Fashion_Reinforced_Map::const_iterator find_it = fashion_reinforced_map_.find(part);
	if (fashion_reinforced_map_.end() == find_it) return 0;
	return get_map_second_pointer_by_key(lv, find_it->second);
}

int Config_Cache_Fashion::get_fashion_soul_max_lv(void) const {
	return fashion_soul_map_.size();
}

int Config_Cache_Fashion::get_fashion_reinforced_max_lv(int part) const {

	Fashion_Reinforced_Map::const_iterator find_it = fashion_reinforced_map_.find(part);
	if (fashion_reinforced_map_.end() == find_it) return 0;
	return find_it->second.size();
}
