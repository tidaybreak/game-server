/*
 * Knight_Trials_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Knight_Trials.h"
#include "Configurator.h"
#include "Json_Utility.h"

const Knight_Trials_Level_Conf *Config_Cache_Knight_Trials::get_level_conf_by_level(const uint32_t level) {
	for(Knight_Trials_Level_Conf_List::iterator it = knight_trials_level_conf_list_.begin(); it != knight_trials_level_conf_list_.end(); ++it) {
		if(it->slevel <= level && it->elevel >= level) {
			return &(*it);
		}
	}
	return 0;
}

const Knight_Trials_Points_Conf_Map &Config_Cache_Knight_Trials::get_points_conf(void) {
	return knight_trials_points_conf_map_;
}

const Knight_Trials_Points_Conf *Config_Cache_Knight_Trials::get_points_conf_by_points(const uint8_t points) {
	return get_map_second_pointer_by_key(points, knight_trials_points_conf_map_);
}

const uint32_t Config_Cache_Knight_Trials::get_exp_buy_level_points(uint32_t level, uint8_t points) {
	Knight_Trials_Exp_Reward_Conf_Map::iterator it = exp_reward_conf_map.find(level);
	if(it != exp_reward_conf_map.end()) {
		if(points < it->second.exp_reward.size()) {
			return it->second.exp_reward[points];
		}
	}
	return 0;
}

const Knight_Trials_Box_Reward_Conf_List *Config_Cache_Knight_Trials::get_box_reward_list_by_type(uint32_t type) {
	Knight_Trials_Box_Reward_Conf_Map::iterator iter = box_reward_conf_map.find(type);
	if(iter != box_reward_conf_map.end()) {
		return &iter->second;
	}
	return 0;
}

void Config_Cache_Knight_Trials::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Knight_Trials");
	refresh_knight_trials_points_conf_cache();
	refresh_knight_trials_level_conf_cache();
	refresh_exp_reward_conf_cache();
	refresh_box_reward_conf_cache();
}

void Config_Cache_Knight_Trials::refresh_box_reward_conf_cache(void) {
	box_reward_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["knight_trials"]["box_reward"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
		LOG_ABORT("not have knight_trials config: box_reward.json");
	}
	Knight_Trials_Box_Reward_Conf value;
	for (uint i = 0; i < config_json.size(); ++i) {
		value.reset();
		const Json::Value& json = config_json[i];

		value.box_type = json["boxType"].asInt();
		value.reward_id = json["reward_id"].asInt();
		value.slevel = json["slevel"].asInt();
		value.elevel = json["elevel"].asInt();
		value.type = json["type"].asInt();
		value.rate = json["rate"].asInt();
		value.start = json["start"].asInt();
		value.end = json["end"].asInt();

		Knight_Trials_Box_Reward_Conf_Map::iterator iter = box_reward_conf_map.find(value.box_type);
		if(iter != box_reward_conf_map.end()) {
			iter->second.push_back(value);
		} else {
			Knight_Trials_Box_Reward_Conf_List list;
			list.clear();
			list.push_back(value);
			box_reward_conf_map.insert(std::make_pair(value.box_type, list));
		}

	}
}

void Config_Cache_Knight_Trials::refresh_knight_trials_points_conf_cache(void) {
	knight_trials_points_conf_map_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["knight_trials"]["points"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
		LOG_ABORT("not have knight_trials config: points.json");
	}
	Knight_Trials_Points_Conf value;
	for (uint i = 0; i < config_json.size(); ++i) {
		value.reset();
		const Json::Value& json = config_json[i];

		value.points = json["points"].asInt();
		value.fdnum = json["fdnum"].asInt();
		value.fdpro = json["fdpro"].asInt();
		value.epro = json["epro"].asInt();
		value.spro = json["spro"].asInt();
		value.boxType = json["boxType"].asInt();
		value.num = json["num"].asInt();

		set_map_second_value_by_key(value.points, knight_trials_points_conf_map_, value);
	}
}

void Config_Cache_Knight_Trials::refresh_exp_reward_conf_cache(void) {
	exp_reward_conf_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["knight_trials"]["exp_reward"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have knight_trials config: exp_reward.json");
	}

	Knight_Trials_Exp_Reward_Conf value;
	for (uint i = 0; i < config_json.size(); ++i) {
		value.reset();
		const Json::Value& json = config_json[i];

		value.level = json["level"].asInt();

		const Json::Value& exp_conf = json["exp"];
		if(exp_conf != Json::Value::null && exp_conf.size()) {
			for (uint j = 0; j < exp_conf[j].size(); ++j) {
				uint32_t exp = exp_conf[j][j].asInt();
				value.exp_reward.push_back(exp);
			}
		}

		set_map_second_value_by_key(value.level, exp_reward_conf_map, value);
	}
}

void Config_Cache_Knight_Trials::refresh_knight_trials_level_conf_cache(void) {
	knight_trials_level_conf_list_.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["knight_trials"]["level_interval"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
		LOG_ABORT("not have knight_trials config: level_interval.json");
	}
	Knight_Trials_Level_Conf value;
	for (uint i = 0; i < config_json.size(); ++i) {
		value.reset();
		const Json::Value& json = config_json[i];

		value.slevel = json["slevel"].asInt();
		value.elevel = json["elevel"].asInt();

		knight_trials_level_conf_list_.push_back(value);
	}
}
