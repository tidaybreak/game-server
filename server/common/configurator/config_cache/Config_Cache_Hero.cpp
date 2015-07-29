/*
 * Hero_Config.cpp
 *
 *  Created on: Apr 12, 2014
 *      Author: chenyulin
 */

#include "Config_Cache_Hero.h"
#include "Config_Cache_Role.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Fighter_Detail.h"

void Config_Cache_Hero::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Hero");

	refresh_hero_correction();
	refresh_hero_conversion();
	refresh_hero();
	refresh_hero_property();
	refresh_hero_lvl_misc();

	refresh_artifact_cfg_id();
	refresh_artifact_cfg_lvl();

	refresh_hero_intimacy_base();
	refresh_hero_intimacty_common();
	refresh_hero_intimacy_gift();
	refresh_hero_intimacy_community();
}

void Config_Cache_Hero::refresh_hero_correction(void) {
	/*
	 * hero_correction.json
	 */
	hero_id_correct_cfg_map_.clear();
	Int_Double prop_value;
	Int_Double_Map prop_value_map;
	const Json::Value &json_hero_correction = CONFIG_INSTANCE->config_json()["hero"]["hero_correction"];
	for (Json::Value::iterator it_hero_correction = json_hero_correction.begin();
			it_hero_correction != json_hero_correction.end(); ++it_hero_correction) {
		prop_value_map.clear();

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_hero_correction)[prop_stream.str()]) {
				prop_value.value = (*it_hero_correction)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					prop_value_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		int hero_id = (*it_hero_correction)["heroID"].asInt();
		ABORT_INT_ZERO(hero_id);

		set_map_second_value_by_key(hero_id, hero_id_correct_cfg_map_, prop_value_map);
	}
}

void Config_Cache_Hero::refresh_hero_conversion(void) {
	/*
	 * hero_conversion.json
	 */
	hero_id_conver_cfg_map_.clear();
	Int_Double prop_value;
	Int_Double_Map prop_value_map;
	const Json::Value &json_hero_conversion = CONFIG_INSTANCE->config_json()["hero"]["hero_conversion"];
	for (Json::Value::iterator it_hero_conversion = json_hero_conversion.begin();
			it_hero_conversion != json_hero_conversion.end(); ++it_hero_conversion) {
		prop_value_map.clear();

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_hero_conversion)[prop_stream.str()]) {
				prop_value.value = (*it_hero_conversion)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					prop_value_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		int hero_id = (*it_hero_conversion)["heroID"].asInt();
		ABORT_INT_ZERO(hero_id);

		set_map_second_value_by_key(hero_id, hero_id_conver_cfg_map_, prop_value_map);
	}
}

void Config_Cache_Hero::refresh_hero(void) {
	/*
	 * hero.json
	 */

	Hero_Intimacy_Map hero_intimacy_map;
	refresh_hero_intimacy(hero_intimacy_map);

	hero_config_map_.clear();
	Hero_Config hero_config;
	const Json::Value &json_hero = CONFIG_INSTANCE->config_json()["hero"]["hero"];
	for (Json::Value::iterator it_hero = json_hero.begin();
			it_hero != json_hero.end(); ++it_hero) {
		hero_config.reset();

		// hero.json start
		hero_config.hero_id = (*it_hero)["heroID"].asInt();
		ABORT_INT_ZERO(hero_config.hero_id);
		hero_config.hero_name = (*it_hero)["hero_name"].asString();
		hero_config.hero_career = (*it_hero)["hero_career"].asInt();
		hero_config.hero_init_lvl = (*it_hero)["hero_initial_lv"].asInt();
		hero_config.hero_initial_awake = (*it_hero)["hero_initial_awake"].asInt();
		hero_config.hero_quality = (*it_hero)["hero_quality"].asInt();
		hero_config.hero_sm_type = (*it_hero)["hero_sm_type"].asInt();
		hero_config.hero_open_lvl = (*it_hero)["recruit_lv"].asInt();
		hero_config.recruit_cost_soul = (*it_hero)["hero_cost_soul"].asInt();
		hero_config.recruit_exploit_lvl = (*it_hero)["recruit_exploit_lvl"].asInt();
		hero_config.vip_recruit_diamond = (*it_hero)["vip_recruit_diamond"].asInt();
		hero_config.max_awake_lvl = (*it_hero)["max_awake_lvl"].asInt();
		hero_config.factor_lvl_up_soul = (*it_hero)["factor_lvl_up_soul"].asDouble();

		Json_Utility::assign_set_list((*it_hero)["artifact_id"], hero_config.artifact_ids);
		Json_Utility::assign_int_vec((*it_hero)["hero_skill_id"], hero_config.skill_id_vec);
		Json_Utility::assign_int_vec((*it_hero)["hero_taskid"], hero_config.recruit_tasks);
		Json_Utility::assign_int_vec((*it_hero)["awake_skill"], hero_config.awake_skill_id_vec);
		Json_Utility::assign_item_vec((*it_hero)["recruit_cost_items"], hero_config.recruit_cost_items);
		Json_Utility::assign_int_vec((*it_hero)["hero_lvl_awake_limit"], hero_config.hero_lvl_awake_limit);
		Json_Utility::assign_int_vec((*it_hero)["assist_skill"], hero_config.assist_skill_id_vec);

		const Json::Value &json_awake_task = (*it_hero)["awake_task"];
		if (! json_awake_task.empty()) {
			Int_Vec awake_tasks;
			for (uint8_t i = 0; i < json_awake_task.size(); ++i) {
				awake_tasks.clear();
				Json_Utility::assign_int_vec(json_awake_task[i], awake_tasks);
				if (! awake_tasks.empty()) {
					hero_config.awake_tasks_map.insert(std::make_pair(i + 1, awake_tasks));
				}
			}
		}

		const Json::Value &json_awake_cost_items = (*it_hero)["awake_cost_items"];
		Id_Amount_Vec id_amount_vec;
		for (uint16_t i = 0; i < json_awake_cost_items.size(); ++i) {
			id_amount_vec.clear();
			Json_Utility::assign_item_vec(json_awake_cost_items[i], id_amount_vec);
			hero_config.awake_lvl_cost_items.push_back(id_amount_vec);
		}

		Awake_Lvl_Cfg awake_lvl_cfg;
		for (int awake_lvl = 1; awake_lvl < 9; ++awake_lvl) {
			char char_awake_lvl[20];
			memset(char_awake_lvl, 0x00, sizeof(char_awake_lvl));
			snprintf(char_awake_lvl, sizeof(char_awake_lvl), "awake_prop_val_%d", awake_lvl);
			if (Json::Value::null != (*it_hero)[char_awake_lvl]) {
				const Json::Value &json_awake_lvl = (*it_hero)[char_awake_lvl];
				if (json_awake_lvl.empty()) {
					continue;
				}
				//awake_lvl_cfg.reset();
				for (uint16_t cfg_awake_i = 0; cfg_awake_i < json_awake_lvl.size(); ++cfg_awake_i) {
					if (json_awake_lvl[cfg_awake_i].size() != 2) {
						LOG_ABORT("hero.json error: awake_prop_val_%", awake_lvl);
					} else {
						int prop = json_awake_lvl[cfg_awake_i][0u].asInt();
						double value = json_awake_lvl[cfg_awake_i][1u].asDouble();
						if(value > 0.00001f){
							if (FIGHT_PROP_SET.count(prop)) {
								if(awake_lvl_cfg.prop_val_map.find(prop) != awake_lvl_cfg.prop_val_map.end()){
									awake_lvl_cfg.prop_val_map[prop] += value;
								}else{
									awake_lvl_cfg.prop_val_map[prop] = value;
								}
							}
						}
					}
				}
			}
			hero_config.awake_lvl_cfg_map.insert(std::make_pair(awake_lvl, awake_lvl_cfg));
		}

		// hero.json end


		// merge hero_correction
		if (!find_hero_id_correct_prop(hero_config.hero_id)) {
			LOG_ABORT("英雄correct_prop属性没加 hero:%d", hero_config.hero_id);
		}
		hero_config.hero_prop_correct = *find_hero_id_correct_prop(hero_config.hero_id);

		// merge hero_conversion
		if (!find_hero_id_conver_prop(hero_config.hero_id)) {
			LOG_ABORT("英雄conver_prop属性没加 hero:%d", hero_config.hero_id);
		}
		hero_config.hero_prop_conver = *find_hero_id_conver_prop(hero_config.hero_id);


		Hero_Intimacy_Map::iterator find_it = hero_intimacy_map.find(hero_config.hero_id);
		if (find_it != hero_intimacy_map.end()) {
			hero_config.hero_intimacy = find_it->second;
		}

		set_map_second_value_by_key(hero_config.hero_id, hero_config_map_, hero_config);
	}
}

void Config_Cache_Hero::refresh_hero_property(void) {
	/*
	 * hero_property.json
	 */
	hero_lvl_prop_cfg_map_.clear();
	Int_Double prop_value;
	Int_Double_Map prop_value_map;
	const Json::Value &json_lvl_prop = CONFIG_INSTANCE->config_json()["hero"]["hero_property"];
	for (Json::Value::iterator it_lvl_prop = json_lvl_prop.begin();
			it_lvl_prop != json_lvl_prop.end(); ++it_lvl_prop) {
		prop_value_map.clear();

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_lvl_prop)[prop_stream.str()]) {
				prop_value.value = (*it_lvl_prop)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					prop_value_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		int hero_lvl = (*it_lvl_prop)["hero_lv"].asInt();
		ABORT_INT_ZERO(hero_lvl);

		set_map_second_value_by_key(hero_lvl, hero_lvl_prop_cfg_map_, prop_value_map);
	}
}

void Config_Cache_Hero::refresh_hero_lvl_misc(void) {
	/*
	 * hero_lvl_misc.json
	 */
	hero_lvl_misc_cfg_map_.clear();
	Hero_Lvl_Misc_Config hero_lvl_misc;
	const Json::Value &json_hero_lvl_misc = CONFIG_INSTANCE->config_json()["hero"]["hero_lvl_misc"];
	for (Json::Value::iterator it_misc = json_hero_lvl_misc.begin();
			it_misc != json_hero_lvl_misc.end(); ++it_misc) {
		hero_lvl_misc.reset();

		hero_lvl_misc.hero_lvl = (*it_misc)["hero_lv"].asInt();
		ABORT_INT_ZERO(hero_lvl_misc.hero_lvl);
		hero_lvl_misc.lvl_up_cost_souls = (*it_misc)["lvl_up_cost_souls"].asInt();

		set_map_second_value_by_key(hero_lvl_misc.hero_lvl, hero_lvl_misc_cfg_map_, hero_lvl_misc);
	}
}

void Config_Cache_Hero::refresh_artifact_cfg_id(void) {
	/*
	 * artifacts.json
	 */
	artifact_cfg_id_map_.clear();
	Artifact_Cfg_Id cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["artifacts"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.artifact_id = (*it_json)["artifact_id"].asInt();
		ABORT_INT_ZERO(cfg_row_info.artifact_id);
		cfg_row_info.artifact_name = (*it_json)["artifact_name"].asString();
		cfg_row_info.max_lvl = (*it_json)["max_lvl"].asInt();
		cfg_row_info.cost_soul_param = (*it_json)["cost_soul_param"].asInt();
		cfg_row_info.cost_item_param = (*it_json)["cost_item_param"].asInt();

		const Json::Value& json_aptitude = (*it_json)["aptitude"];
		if (! json_aptitude.empty()) {
			for (uint16_t i = 0; i < json_aptitude.size(); ++i) {
				int prop = json_aptitude[i][0u].asInt();
				double val = json_aptitude[i][1u].asDouble();
				if (prop && val > 0.0) {
					cfg_row_info.aptitude_map.insert(std::make_pair(prop, val));
				}
			}
		}

		set_map_second_value_by_key(cfg_row_info.artifact_id, artifact_cfg_id_map_, cfg_row_info);
	}
}

void Config_Cache_Hero::refresh_artifact_cfg_lvl(void) {
	/*
	 * artifact_lvl.json
	 */
	artifact_cfg_lvl_map_.clear();
	Artifact_Cfg_Lvl cfg_row_info;
	Int_Double prop_value;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["artifact_lvl"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.artifact_lvl = (*it_json)["artifact_lvl"].asInt();
		ABORT_INT_ZERO(cfg_row_info.artifact_lvl);
		cfg_row_info.need_player_lvl = (*it_json)["need_player_lvl"].asInt();
		cfg_row_info.cost_soul = (*it_json)["cost_soul"].asInt();
		Json_Utility::assign_item_vec((*it_json)["cost_material"], cfg_row_info.cost_items);

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_json)[prop_stream.str()]) {
				prop_value.value = (*it_json)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					cfg_row_info.pro_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		set_map_second_value_by_key(cfg_row_info.artifact_lvl, artifact_cfg_lvl_map_, cfg_row_info);
	}
}

void Config_Cache_Hero::refresh_hero_intimacy(Hero_Intimacy_Map& map) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["hero_intimacy"];

	Hero_Intimacy hero_intimacy;
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {

		hero_intimacy.reset();
		int hero_id = (*it)["hero_id"].asInt();
		hero_intimacy.assist_lv_limit = (*it)["assist_intimacy_lvl"].asInt();
		hero_intimacy.lvl_limit = (*it)["intimacy_lvl_limit"].asInt();
		hero_intimacy.exp_fix = (*it)["intimacy_exp_fix"].asDouble();
		hero_intimacy.convert_fix = (*it)["intimacy_convert_fix"].asInt();
		hero_intimacy.favor_gift_type = (*it)["hero_hobby_gift_type"].asInt();
		hero_intimacy.disfavor_gift_type = (*it)["hero_hate_gift_type"].asInt();
		map[hero_id] = hero_intimacy;
	}
}

void Config_Cache_Hero::refresh_hero_intimacy_base(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["intimacy_base"];
	Intimacy_Base intimacy_base;
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {

		intimacy_base.reset();
		intimacy_base.lvl = (*it)["intimacy_lvl"].asInt();
		intimacy_base.exp = (*it)["intimacy_exp"].asInt();
		intimacy_base.convert = (*it)["intimacy_convert"].asDouble();

		set_map_second_value_by_key(intimacy_base.lvl, intimacy_base_map, intimacy_base);
	}
}

void Config_Cache_Hero::refresh_hero_intimacty_common(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["hero_common"];
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
		int type = (*it)["key"].asInt();
		int val = (*it)["val"].asInt();

		set_map_second_value_by_key(type, intimacy_common_map, val);
	}
}

void Config_Cache_Hero::refresh_hero_intimacy_gift(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["hero_gift"];
	Hero_Intimacy_Gift hero_intimacy_gift;
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
		hero_intimacy_gift.reset();
		hero_intimacy_gift.item_id = (*it)["gift_item_id"].asInt();
		hero_intimacy_gift.type = (*it)["gift_type"].asInt();
		hero_intimacy_gift.gift_intimacty = (*it)["gift_intimacy"].asInt();

		set_map_second_value_by_key(hero_intimacy_gift.item_id, hero_intimacy_gift_map, hero_intimacy_gift);
	}
}

void Config_Cache_Hero::refresh_hero_intimacy_community(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["hero"]["hero_community"];
	Intimacy_Community intimacy_community;
	for (Json::Value::iterator it = json.begin(); it != json.end(); ++it) {
		intimacy_community.reset();
		intimacy_community.id = (*it)["interact_id"].asInt();
		intimacy_community.hero_id = (*it)["hero_id"].asInt();
		if ((*it)["interact_intimacys"].size() != 2) {
			LOG_ABORT("intimacy community error, id=%d", intimacy_community.id);
		}
		intimacy_community.val.val_1 = (*it)["interact_intimacys"][0u].asInt();
		intimacy_community.val.val_2 = (*it)["interact_intimacys"][1].asInt();
		set_map_second_value_by_key(intimacy_community.id, intimacy_community_map, intimacy_community);
	}
}

const Hero_Config *Config_Cache_Hero::find_hero(const int hero_id) const {
	return get_map_second_pointer_by_key(hero_id, hero_config_map_);
}

const Int_Double_Map *Config_Cache_Hero::find_hero_lvl_prop(const int hero_lvl) const {
	return get_map_second_pointer_by_key(hero_lvl, hero_lvl_prop_cfg_map_);
}

const Hero_Lvl_Misc_Config *Config_Cache_Hero::find_hero_lvl_misc(const int hero_lvl) const {
	return get_map_second_pointer_by_key(hero_lvl, hero_lvl_misc_cfg_map_);
}

const Int_Double_Map *Config_Cache_Hero::find_hero_id_correct_prop(const int hero_id) const {
	return get_map_second_pointer_by_key(hero_id, hero_id_correct_cfg_map_);
}

const Int_Double_Map *Config_Cache_Hero::find_hero_id_conver_prop(const int hero_id) const {
	return get_map_second_pointer_by_key(hero_id, hero_id_conver_cfg_map_);
}

const Artifact_Cfg_Id *Config_Cache_Hero::find_artifact(const int artifact_id) const {
	return get_map_second_pointer_by_key(artifact_id, artifact_cfg_id_map_);
}

const Artifact_Cfg_Lvl *Config_Cache_Hero::find_artifact_lvl(const int artifact_lvl) const {
	return get_map_second_pointer_by_key(artifact_lvl, artifact_cfg_lvl_map_);
}

//const Artifact_Cfg_Id_Map& Config_Cache_Hero::artifact_cfg_id_map(void) const {
//	return artifact_cfg_lvl_map_;
//}

const Intimacy_Base* Config_Cache_Hero::find_intimacy_base(int lv) const {
	return get_map_second_pointer_by_key(lv, intimacy_base_map);
}

const int Config_Cache_Hero::find_intimacy_common(int type) const {
	const int *val = get_map_second_pointer_by_key(type, intimacy_common_map);
	if (0 == val) return 0;
	return *val;
}
const Hero_Intimacy_Gift* Config_Cache_Hero::find_intimacy_gift(int item_id) const {
	return get_map_second_pointer_by_key(item_id, hero_intimacy_gift_map);
}
const Intimacy_Community* Config_Cache_Hero::find_intimacy_community(int id) const {
	return get_map_second_pointer_by_key(id, intimacy_community_map);
}
