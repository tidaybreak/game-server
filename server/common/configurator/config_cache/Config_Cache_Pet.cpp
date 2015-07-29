/*
 * Config_Cache_Pet.cpp
 *
 *  Created on: 2015年4月10日
 *      Author: root
 */

#include "Config_Cache_Pet.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Role.h"

void Config_Cache_Pet::refresh_config_cache() {

	PERF_MON("Config_Cache_Pet");
	refresh_pet_config_map();
	refresh_pet_illusion_config_map();
	refresh_pet_learn_skill_config_map();
	refresh_pet_skill_config_map();
}

void Config_Cache_Pet::refresh_pet_config_map() {

	Int_Int_Map grips_map;
	const Json::Value &grips_json = CONFIG_INSTANCE->config_json()["pet"]["pet_skill_grid"];
	for (Json::Value::const_iterator it = grips_json.begin(); it != grips_json.end(); ++it) {

		grips_map[(*it)["stage"].asInt()] = (*it)["grid_num"].asInt();
	}

	const Json::Value &json = CONFIG_INSTANCE->config_json()["pet"]["pet"];
	Pet_Config cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {

		cfg.reset();
		cfg.lv = (*it)["lv"].asInt();
		cfg.ui_stage = (*it)["ui_stage"].asInt();
		cfg.ui_lv = (*it)["ui_lv"].asInt();
		cfg.gold_cost = (*it)["gold_cost"].asInt();
		cfg.avatar = (*it)["avatar"].asInt();
		cfg.item_cost.val_1 = (*it)["item_cost_id"].asInt();
		cfg.item_cost.val_2 = (*it)["item_cost_amount"].asInt();
		cfg.upgrade_val.val_1 = (*it)["val_add"].asInt();
		cfg.upgrade_val.val_2 = (*it)["val_max"].asInt();
		cfg.broadcast = (*it)["broadcast"].asInt();

		// read attr for role
		read_pet_sub_config(cfg.role_prop_map, (*it)["pet_role"]);
		// read attr for hero
		read_pet_sub_config(cfg.hero_prop_map, (*it)["pet_hero"]);

		// read skills
		if (!(*it)["skill"].isArray()) {
			LOG_ABORT("pet config error, skill is not array, lv =%d", cfg.lv);
		}
		for (Json::Value::const_iterator skill_it = (*it)["skill"].begin(); skill_it != (*it)["skill"].end(); ++skill_it) {

			cfg.skill_vec.push_back((*skill_it).asInt());
		}

		Int_Int_Map::iterator find_it = grips_map.find(cfg.ui_stage);
		if (find_it != grips_map.end()) {
			cfg.grid_num = find_it->second;
		}

		pet_config_map_[cfg.lv] = cfg;
	}
}

void Config_Cache_Pet::read_pet_sub_config(Pet_Prop_Value_Map &map, const Json::Value &json) {

	std::stringstream str;
	Pet_Prop_Value prop;
	if (json.empty()) return;
	for (Int_Set::const_iterator prop_it = FIGHT_PROP_SET.begin();
			prop_it != FIGHT_PROP_SET.end(); ++prop_it) {

		str.str("");
		str.clear();
		prop.reset();

		int id = (*prop_it);
		str << id;
		if (Json::Value::null == json[str.str()]) continue;
		if (json[str.str()].empty()) continue;
		if (json[str.str()].size() != 2) {
			LOG_ABORT("pet_role config error, id =%d", id);
		}
		prop.fix = json[str.str()][0u].asDouble();
		prop.percent = json[str.str()][1].asDouble();
	}
}

void Config_Cache_Pet::refresh_pet_illusion_config_map() {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["pet"]["pet_illusion"];
	Pet_Illusion_Config cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {

		cfg.reset();
		cfg.id = (*it)["id"].asInt();
		cfg.model = (*it)["model"].asInt();
		cfg.active_cond = (*it)["active_cond"].asInt();
		if (!(*it)["attrs"].isArray()) {
			LOG_ABORT("pet illusion attrs not array, id =%d", cfg.id);
		}
		for (Json::Value::const_iterator atr_it = (*it)["attrs"].begin(); atr_it != (*it)["attrs"].end(); ++atr_it) {

			if ((*atr_it).size() != 2) {
				LOG_ABORT("pet illusion attrs error, id =%d", cfg.id);
				cfg.attr_map[(*atr_it)[0u].asInt()] = (*atr_it)[1].asInt();
			}
		}

		pet_illusion_config_map_[cfg.id] = cfg;
	}
}

void Config_Cache_Pet::refresh_pet_learn_skill_config_map() {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["pet"]["pet_learn_skill"];
	Pet_Learn_Skill_Config cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {

		cfg.reset();
		cfg.num 				= (*it)["num"].asInt();
		cfg.rate 				= (*it)["rate"].asInt();
		cfg.money_cost 	= (*it)["money_cost"].asInt();
		pet_learn_skill_config_map_[cfg.num] = cfg;
	}
}

void Config_Cache_Pet::refresh_pet_skill_config_map() {

	Pet_Skill_Config_Map temp_map;
	Pet_Skill_Config cfg;
	const Json::Value &d_json = CONFIG_INSTANCE->config_json()["pet"]["pet_skill_display"];
	for (Json::Value::const_iterator it = d_json.begin(); it != d_json.end(); ++it) {

		cfg.reset();
		cfg.skill_id = (*it)["skill"].asInt();
		cfg.category = (*it)["num"].asInt();
		cfg.type = (*it)["type"].asInt();
		temp_map[cfg.skill_id] = cfg;
	}

	const Json::Value &s_json = CONFIG_INSTANCE->config_json()["pet"]["pet_skill_item"];

	for (Json::Value::const_iterator it = s_json.begin(); it != s_json.end(); ++it) {

		cfg.reset();
		cfg.skill_id = (*it)["skill_id"].asInt();
		cfg.item_id = (*it)["item_id"].asInt();
		Pet_Skill_Config_Map::iterator find_it = temp_map.find(cfg.skill_id);
		if (find_it == temp_map.end()) {
			LOG_ABORT("cannot find skill category in table sheet pet_skill_display, id=%d", cfg.skill_id);
		}
		cfg.category = find_it->second.category;
		cfg.type = find_it->second.type;
		pet_skill_config_map_[cfg.skill_id] = cfg;
	}
}

const Pet_Config_Map &Config_Cache_Pet::get_pet_config_map(void) {
	return pet_config_map_;
}

const Pet_Config *Config_Cache_Pet::get_pet_config(int lv) {
	return get_map_second_pointer_by_key(lv, pet_config_map_);
}

const Pet_Illusion_Config *Config_Cache_Pet::get_pet_illusion(int id) {
	return get_map_second_pointer_by_key(id, pet_illusion_config_map_);
}

const Pet_Illusion_Config_Map& Config_Cache_Pet::get_pet_illusion_map(void) {
	return pet_illusion_config_map_;
}

const Pet_Learn_Skill_Config *Config_Cache_Pet::get_pet_learn_skill(int num) {

	return get_map_second_pointer_by_key(num, pet_learn_skill_config_map_);
}

const Pet_Skill_Config*  Config_Cache_Pet::get_pet_skill_config(int skill_id) {

	return get_map_second_pointer_by_key(skill_id, pet_skill_config_map_);
}
