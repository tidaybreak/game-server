/*
 * ConfigCacheWing.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: root
 */

#include "Config_Cache_Wing.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Role.h"

void Config_Cache_Wing::refresh_config_cache() {
	PERF_MON("Config_Cache_Wing");

	const Json::Value &json = CONFIG_INSTANCE->config_json()["devilWing"]["master"];
	Wing_Config cfg;

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		cfg.reset();
		cfg.lv = (*it)["master_lv"].asInt();
		cfg.show_stage = (*it)["master_stage"].asInt();
		cfg.show_lv = (*it)["wing_lv"].asInt();
		cfg.gold_cost = (*it)["gold"].asInt();
		if (!(*it)["material"].empty() && (*it)["material"].isArray()) {
			cfg.goods_cost.val_1 = (*it)["material"][0u].asInt();
			cfg.goods_cost.val_2 = (*it)["material"][1].asInt();
		}
		cfg.rate_base = (*it)["success_rate_base"].asInt();
		cfg.rate_add_value = (*it)["success_rate_once"].asInt();
		cfg.rate_show_min = (*it)["luck_min"].asInt();
		cfg.rate_show_max = (*it)["luck_max"].asInt();
		if (!(*it)["luck_once"].empty() && (*it)["luck_once"].isArray()) {
			cfg.rate_min_max.val_1 = (*it)["luck_once"][0u].asInt();
			cfg.rate_min_max.val_2 = (*it)["luck_once"][1].asInt();
			if (cfg.rate_min_max.val_1 > cfg.rate_min_max.val_2)
				LOG_ABORT("luck once error, min > max, min = %d, max=%d, lv=%d",
						cfg.rate_min_max.val_1, cfg.rate_min_max.val_2, cfg.lv);
		}
		cfg.is_cast = (*it)["broadcast"].asInt();

		if (!(*it)["avatar"].empty() && (*it)["avatar"].isArray()) {
			const Json::Value &a_json = (*it)["avatar"];
			for (Json::Value::const_iterator a_it = a_json.begin(); a_it != a_json.end(); ++a_it) {
				cfg.wing_avatar_map[(*a_it)[0u].asInt()][(*a_it)[1].asInt()] = (*a_it)[2].asInt();
			}
		}

		std::stringstream str;
		Wing_Prop_Value prop;
		if (!(*it)["major"].empty()) {
			const Json::Value &mj_json = (*it)["major"];
			for (Int_Set::const_iterator p_it = FIGHT_PROP_SET.begin();
					p_it != FIGHT_PROP_SET.end(); ++p_it) {
				str.str("");
				str.clear();
				prop.reset();

				int id = (*p_it);
				str << id;
				if (Json::Value::null != mj_json[str.str()] && !mj_json[str.str()].empty()) {
					if (!mj_json[str.str()].isArray()) continue;
					prop.fix = mj_json[str.str()][0u].asDouble();
					prop.percent = mj_json[str.str()][1].asDouble();
					cfg.player_prop_map[id] = prop;
				}
			}
		}

		if (!(*it)["hero"].empty()) {
			const Json::Value &hr_json = (*it)["hero"];
			for (Int_Set::const_iterator p_it = FIGHT_PROP_SET.begin();
					p_it != FIGHT_PROP_SET.end(); ++p_it) {
				str.str("");
				str.clear();
				prop.reset();

				int id = (*p_it);
				str << id;
				if (Json::Value::null != hr_json[str.str()] && !hr_json[str.str()].empty()) {
					if (!hr_json[str.str()].isArray()) continue;
					prop.fix = hr_json[str.str()][0u].asDouble();
					prop.percent = hr_json[str.str()][1].asDouble();
					cfg.hero_prop_map[id] = prop;
				}
			}
		}

		if (!(*it)["skill"].empty() && (*it)["skill"].isArray()) {
			const Json::Value &sk_json = (*it)["skill"];
			for (Json::Value::const_iterator sk_it = sk_json.begin();
					sk_it != sk_json.end(); ++sk_it) {
				cfg.skill_vec.push_back((*sk_it).asInt());
			}
		}

		wing_cfg_map_[cfg.lv] = cfg;
	}

	refresh_ret_reward_config_cache();
}

void Config_Cache_Wing::refresh_ret_reward_config_cache(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["devilWing"]["master_ui"];
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		if ((*it)["wing_reward"].empty()) continue;
		if (!(*it)["wing_reward"].isArray()) continue;

		const Json::Value &r_json = (*it)["wing_reward"];
		Int_Int reward;
		Int_Int_Vec reward_vec;
		for (Json::Value::const_iterator r_it = r_json.begin(); r_it != r_json.end(); ++r_it) {
			if ((*r_it).empty()) continue;
			if (!(*r_it).isArray()) continue;
			if (3 != (*r_it).size()) {
				LOG_ABORT("wing return reward config error!");
			}
			int stage = (*r_it)[0u].asInt();
			reward.reset();
			reward_vec.clear();
			reward.val_1 = (*r_it)[1].asInt();
			reward.val_2 = (*r_it)[2].asInt();
			reward_vec.push_back(reward);
			wing_ret_reward_map_[stage] = reward_vec;
		}
	}
}

const Wing_Config* Config_Cache_Wing::get_wing_config(int lv) const {
	Wing_Cfg_Map::const_iterator find_it = wing_cfg_map_.find(lv);
	if (find_it == wing_cfg_map_.end()) return 0;
	return &find_it->second;
}

const Wing_Cfg_Map& Config_Cache_Wing::get_wing_config_map(void) const {
	return wing_cfg_map_;
}

const Wing_Ret_Reward_Map& Config_Cache_Wing::get_wing_ret_reward_map(void) const {
	return wing_ret_reward_map_;
}

const Int_Int_Vec* Config_Cache_Wing::get_wing_ret_reward(int stage) const {
	Wing_Ret_Reward_Map::const_iterator find_it = wing_ret_reward_map_.find(stage);
	if (find_it == wing_ret_reward_map_.end()) return 0;
	return &find_it->second;
}
