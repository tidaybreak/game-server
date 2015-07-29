/*
 * Reward_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Reward.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "reward/Reward_Config.h"
#include "fighter/Fighter_Def.h"



const Reward_Back_Conf_Map* Config_Cache_Reward::get_reward_back_map(void) {
	return &reward_back_conf_map_;
}

const Keep_Reward_Config* Config_Cache_Reward::get_keep_reward_config(void) const {
	return &keep_reward_conf_;
}

const Online_Group_Conf* Config_Cache_Reward::get_online_group_conf_by_group_id(uint32_t group_id) const {
	return get_map_second_pointer_by_key(group_id, online_group_conf_map_);
}

const Online_Gift_Conf_Vec* Config_Cache_Reward::get_online_gif_conf_vec_by_group_id(uint32_t group_id) const {
	Online_Gift_Conf_Vec_Map::const_iterator it = online_gif_conf_vec_map_.find(group_id);
	if(it != online_gif_conf_vec_map_.end()) {
		return &(it->second);
	} else {
		Online_Gift_Conf_Vec_Map::const_reverse_iterator itit = online_gif_conf_vec_map_.rbegin();
		if(itit != online_gif_conf_vec_map_.rend()) {
			return &(itit->second);
		}
	}
	return 0;
}

const uint32_t Config_Cache_Reward::get_group_id_by_week(uint32_t week) const {
	UInt_UInt_Map::const_iterator it = online_week_group_.find(week);
	if(it != online_week_group_.end()) {
		return it->second;
	}
	return 0;
}

const Login_Seven_Conf_Map& Config_Cache_Reward::get_login_seven_config(void) const {
	return login_seven_conf_map_;
}

const Login_Seven_Config* Config_Cache_Reward::get_login_seven_conf_by_day(uint32_t day) const {
	return get_map_second_pointer_by_key(day, login_seven_conf_map_);
}

const Activation_Code_Config* Config_Cache_Reward::get_activation_code_conf_by_type(uint32_t type) const {
	return get_map_second_pointer_by_key(type, activation_code_conf_map_);
}

const Reward_Back_Config* Config_Cache_Reward::get_reward_back_conf(uint32_t item_id) const {
	return get_map_second_pointer_by_key(item_id, reward_back_conf_map_);
}

void Config_Cache_Reward::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Reward");
	refresh_online_gif_config_cache();
	refresh_login_seven_config_cache();
	refresh_activation_code_config_cache();
	refresh_reward_back_config_cache();
	refresh_online_week_group_config_cache();
	refresh_keep_reward_config_cache();
}


void Config_Cache_Reward::refresh_keep_reward_config_cache(void) {
	keep_reward_conf_.reset();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["reward"]["keep_reward"];
	if (json == Json::Value::null || json.size() == 0) {
			LOG_ABORT("keep_reward.json");
	}
	for (uint i = 0; i < json.size(); ++i) {
		const Json::Value& reward_item_json = json[i];
		uint32_t item = reward_item_json["item_id"].asInt();
		uint32_t value_ = reward_item_json["num"].asInt();
		if(item == PT_EXP_CURRENT) {
			keep_reward_conf_.exp += value_;
		} else {
			uint8_t type = reward_item_json["type"].asInt();
			switch(type) {
				case 1:{
					set_map_second_value_by_key(item, keep_reward_conf_.reward_item, value_);
					break;
				}
				case 2:{
					set_map_second_value_by_key(item, keep_reward_conf_.reward_money, value_);
					break;
				}
			}
		}
	}
}

void Config_Cache_Reward::refresh_online_week_group_config_cache(void) {
	online_week_group_.clear();
	online_group_conf_map_.clear();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["reward"]["online_group"];
	if (json == Json::Value::null || json.size() == 0) {
		LOG_ABORT("Config_Cache_Online_Group");
	}
	Online_Group_Conf value;
	for (uint i = 0; i < json.size(); ++i) {
		const Json::Value& value_json = json[i];
		value.reset();
		value.group_id = value_json["group_id"].asInt();
		value.start = value_json["start"].asInt();
		value.end = value_json["end"].asInt();
		set_map_second_value_by_key(value.group_id, online_group_conf_map_, value);

		for(uint32_t tmp = value.start; tmp <= value.end; ++tmp) {
			set_map_second_value_by_key(tmp, online_week_group_, value.group_id);
		}
	}
}

void Config_Cache_Reward::refresh_login_seven_config_cache(void) {
	login_seven_conf_map_.clear();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["reward"]["login_seven"];
	if (json == Json::Value::null || json.size() == 0) {
		LOG_ABORT("Config_Cache_Login_Seven");
	}
	Login_Seven_Config value;
	for (uint i = 0; i < json.size(); ++i) {
		const Json::Value& value_json = json[i];
		value.reset();

		value.day = value_json["day"].asInt();
		value.item_id = value_json["item_id"].asInt();

		set_map_second_value_by_key(value.day, login_seven_conf_map_, value);
	}
}

void Config_Cache_Reward::refresh_activation_code_config_cache(void) {
	activation_code_conf_map_.clear();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["reward"]["activation_code"];
	if (json == Json::Value::null || json.size() == 0) {
		LOG_ABORT("Config_Cache_Activation_Code");
	}
	Activation_Code_Config value;
	for (uint i = 0; i < json.size(); ++i) {
		const Json::Value& value_json = json[i];
		value.reset();

		value.type_id = value_json["type_id"].asInt();
		value.num = value_json["num"].asInt();
		value.item_id = value_json["item_id"].asInt();

		set_map_second_value_by_key(value.type_id, activation_code_conf_map_, value);
	}
}

void Config_Cache_Reward::refresh_reward_back_config_cache(void) {
	reward_back_conf_map_.clear();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["reward"]["reward_back"];
	if (json == Json::Value::null || json.size() == 0) {
		LOG_ABORT("Config_Cache_Reward_Back");
	}
	Reward_Back_Config value;
	for (uint i = 0; i < json.size(); ++i) {
		const Json::Value& value_json = json[i];
		value.reset();

		value.id = value_json["id"].asInt();
		value.day_num = value_json["day_num"].asInt();
		value.copper_ratio = value_json["copper_ratio"].asInt();
		value.copper_cost = value_json["copper_cost"].asInt();
		value.gold_ratio = value_json["gold_ratio"].asInt();
		value.gold_cost = value_json["gold_cost"].asInt();
		value.gold_cost_type = value_json["gold_cost_type"].asInt();

		value.reward_item.clear();
		if(value_json["reward_item"] != Json::Value::null && value_json["reward_item"].size() > 0) {
			for(uint j = 0; j < value_json["reward_item"].size(); ++j) {
				const Json::Value &reward_item_json = value_json["reward_item"][j];

				uint32_t item = reward_item_json["item"].asInt();
				uint32_t value_ = reward_item_json["value"].asInt();

				if(item == PT_EXP_CURRENT) {
					value.exp += value_;
				} else {
					uint8_t type = reward_item_json["type"].asInt();
					switch(type) {
						case 1:{
							set_map_second_value_by_key(item, value.reward_item, value_);
							break;
						}
						case 2:{
							set_map_second_value_by_key(item, value.reward_money, value_);
							break;
						}
					}
				}
			}
		}

		value.group.clear();
		if(value_json["group"] != Json::Value::null && value_json["group"].size() > 0) {
			for(uint j = 0; j < value_json["group"].size(); ++j) {
				const Json::Value &group_json = value_json["group"][j];
				value.group.push_back(group_json.asInt());
			}
		}

		value.is_be_group = value_json["is_be_group"].asInt();
		value.pid = value_json["pid"].asInt();

		value.level_item_val.clear();
		if(value_json["level_item_val"] != Json::Value::null && value_json["level_item_val"].size() > 0) {
			Reward_Back_Level_Item_Val v;
			Reward_Back_Level_Item_Key k;
			for(uint j = 0; j < value_json["level_item_val"].size(); ++j) {
				const Json::Value &level_item_val_json = value_json["level_item_val"][j];

				v.reset();
				k.reset();

				k.item = level_item_val_json["item"].asInt();
				k.level = level_item_val_json["level"].asInt();

				v.item = level_item_val_json["item"].asInt();
				v.level = level_item_val_json["level"].asInt();
				v.value = level_item_val_json["value"].asInt();
				set_map_second_value_by_key(k, value.level_item_val, v);
			}
		}

		value.ratio_cost_map.clear();
		if(value_json["ratio_cost"] != Json::Value::null && value_json["ratio_cost"].size() > 0) {
			Reward_Back_Ratio_Cost v;
			for(uint j = 0; j < value_json["ratio_cost"].size(); ++j) {
				const Json::Value &ratio_cost_json = value_json["ratio_cost"][j];

				v.level = ratio_cost_json["level"].asInt();

				v.copper_ratio = ratio_cost_json["copper_ratio"].asInt();
				v.copper_cost = ratio_cost_json["copper_cost"].asInt();
				v.gold_ratio = ratio_cost_json["gold_ratio"].asInt();
				v.gold_cost = ratio_cost_json["gold_cost"].asInt();
				v.gold_cost_type = ratio_cost_json["gold_cost_type"].asInt();

				set_map_second_value_by_key(v.level, value.ratio_cost_map, v);
			}
		}

		set_map_second_value_by_key(value.id, reward_back_conf_map_, value);
	}

}

void Config_Cache_Reward::refresh_online_gif_config_cache(void) {
	online_gif_conf_vec_map_.clear();
	const Json::Value& json = CONFIG_INSTANCE->config_json()["reward"]["online_gift"];
	if (json == Json::Value::null || json.size() == 0) {
		LOG_ABORT("Config_Cache_Online_Gift");
	}
	Online_Gift_Config value;
	for (uint i = 0; i < json.size(); ++i) {
		const Json::Value& online_gift_json = json[i];
		value.reset();

		value.id = online_gift_json["id"].asInt();
		value.online_time = online_gift_json["online_time"].asInt();
		value.item_list.clear();
		value.money_list.clear();
		if(online_gift_json["list"] != Json::Value::null && online_gift_json["list"].size() > 0) {
			Online_Item_Config online_item;
			Online_Money_Config online_money;
			for(uint j = 0; j < online_gift_json["list"].size(); ++j) {
				online_item.reset();
				online_money.reset();
				const Json::Value &list_json = online_gift_json["list"][j];
				if(list_json["type"].asInt() == 1) {
					online_item.item_id = list_json["item_id"].asInt();
					online_item.num = list_json["num"].asInt();

					value.item_list.push_back(online_item);
				} else {
					online_money.money_id = list_json["item_id"].asInt();
					online_money.num = list_json["num"].asInt();

					value.money_list.push_back(online_money);
				}
			}
		}

		//分组
		uint32_t group_id = online_gift_json["group_id"].asInt();
		Online_Gift_Conf_Vec_Map::iterator git = online_gif_conf_vec_map_.find(group_id);
		if(git != online_gif_conf_vec_map_.end()) {
			git->second.push_back(value);
		} else {
			Online_Gift_Conf_Vec online_gif_conf_vec_;
			online_gif_conf_vec_.clear();
			online_gif_conf_vec_.push_back(value);
			online_gif_conf_vec_map_.insert(std::make_pair(group_id, online_gif_conf_vec_));
		}
	}
}

