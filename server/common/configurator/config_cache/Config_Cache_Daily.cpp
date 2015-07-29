/*
 * Daily_Config.cpp
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Daily.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"
#include "Config_Cache.h"


const Daily_Conf_Map &Config_Cache_Daily::daily_config_map(void) const {
	return daily_config_map_;
}

const Conf_Key_Map &Config_Cache_Daily::daily_config_key_map(void) const {
	return daily_config_key_map_;
}

const Activity_Conf_Map &Config_Cache_Daily::activity_config_map(void) const {
	return activity_config_map_;
}

const Conf_Key_Map &Config_Cache_Daily::activity_config_key_map(void) const {
	return activity_config_key_map_;
}

const Activity_Award_Map &Config_Cache_Daily::today_act_award_map(void) {
	const uint32_t group = get_activity_group_id();
	return today_act_award_map_[group];
}

const uint32_t Config_Cache_Daily::get_activity_group_id(void) {
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dnow = Date_Time(now);
	int days = get_time_subdays(server_maintainer.game_open_time, now);
	days = days % today_act_group_max_end;
	if(dnow.hour() < 6) {
		--days;
	}
	if(days <= 0) {
		days = 1;
	}
	for(Activity_Group_Conf_Map::iterator it = today_act_group_map_.begin(); it != today_act_group_map_.end(); ++it) {
		if(it->second.start <= (uint32_t)days && it->second.end >= (uint32_t)days) {
			return it->second.group;
		}
	}
	return 0;
}

const Activity_Award_Map &Config_Cache_Daily::all_act_award_map(void) const {
	return all_act_award_map_;
}

const int Config_Cache_Daily::get_force_by_elite_explore_id(const int elite_explore_id) const {
	return get_value_by_key(elite_explore_id, elite_explore_map_);
}

const Check_In_Conf_Map& Config_Cache_Daily::get_check_in_map_by_group_id(const uint32_t group_id) const {
	Check_In_Group_Conf_Map::const_iterator iter = check_in_group_map_.find(group_id);
	if(iter != check_in_group_map_.end()) {
		return iter->second;
	} else {
		return check_in_group_map_.rbegin()->second;
	}
}

const uint32_t Config_Cache_Daily::get_can_equip_by_level_index(const uint32_t level, const uint32_t index) const {
	UInt_UIntUIntMap_Map::const_iterator it =	can_equip_lv_map_.find(level);
	if(it != can_equip_lv_map_.end()) {
		UInt_UInt_Map::const_iterator iter = it->second.find(index);
		if(iter != it->second.end()) {
			return iter->second;
		}
	}
	return 0;
}

void Config_Cache_Daily::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Daily");

	refresh_daily_config_cache();
	refresh_activity_config_cache();
	refresh_today_activity_award_config_cache();
	refresh_all_activity_award_config_cache();
	refresh_all_elite_explore_config_cache();
	refresh_all_check_in_config_cache();
	refresh_can_equip_lv_map_config_cache();
}

void Config_Cache_Daily::refresh_can_equip_lv_map_config_cache(void) {
	can_equip_lv_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["helper"]["canEquipLv"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	UInt_UInt_Map value;
	for (uint i = 0; i < config.size(); ++i) {
		value.clear();
		const Json::Value& json = config[i];

		uint32_t level = json["level"].asInt();
		value.insert(std::make_pair(10001,json["index10001"].asInt()));
		value.insert(std::make_pair(10002,json["index10002"].asInt()));
		value.insert(std::make_pair(10003,json["index10003"].asInt()));
		value.insert(std::make_pair(10004,json["index10004"].asInt()));
		value.insert(std::make_pair(10005,json["index10005"].asInt()));
		value.insert(std::make_pair(10006,json["index10006"].asInt()));
		value.insert(std::make_pair(10007,json["index10007"].asInt()));
		value.insert(std::make_pair(10008,json["index10008"].asInt()));

		set_map_second_value_by_key(level, can_equip_lv_map_, value);
	}
}

void Config_Cache_Daily::refresh_activity_config_cache(void) {
	activity_config_map_.clear();
	activity_config_key_map_.clear();
	const Json::Value& activity_config = CONFIG_INSTANCE->config_json()["helper"]["activity"];
	if (activity_config == Json::Value::null || activity_config.size() == 0) {
		return;
	}
	Config_Key key;
	Activity_Config value;
	for (uint i = 0; i < activity_config.size(); ++i) {
		key.reset();
		value.reset();

		const Json::Value& activity_json = activity_config[i];

		key.task_type = activity_json["task_type"].asInt();
		key.type_id = activity_json["type_id"].asInt();

		value.task_type = activity_json["task_type"].asInt();
		value.type_id = activity_json["type_id"].asInt();
		value.id = activity_json["id"].asInt();
		value.fulfil_num = activity_json["fulfil_num"].asInt();
		value.activity = activity_json["activity"].asInt();

		set_map_second_value_by_key(key, activity_config_map_, value);
		set_map_second_value_by_key(value.id, activity_config_key_map_, key);
	}
}

void Config_Cache_Daily::refresh_daily_config_cache(void) {
	daily_config_map_.clear();
	daily_config_key_map_.clear();
	const Json::Value& daily_config = CONFIG_INSTANCE->config_json()["helper"]["daily"];
	if (daily_config == Json::Value::null || daily_config.size() == 0) {
		return;
	}

	Config_Key key;
	Daily_Config value;

	for (uint i = 0; i < daily_config.size(); ++i) {
		key.reset();
		value.reset();

		const Json::Value& daily_json = daily_config[i];

		key.task_type = daily_json["task_type"].asInt();
		key.type_id = daily_json["type_id"].asInt();

		value.task_type = daily_json["task_type"].asInt();
		value.type_id = daily_json["type_id"].asInt();
		value.id = daily_json["id"].asInt();
		value.fulfil_num = daily_json["fulfil_num"].asInt();

		value.open_type = daily_json["open_type"].asInt();
		if(value.open_type){
			for (uint t = 0; t < daily_json["time"].size(); ++t) {
				const Json::Value& time_json = daily_json["time"][t];
				if(time_json.size() < 2) {
					continue;
				}
				int stime = time_json[0u].asInt();
				int etime = time_json[1].asInt();
				value.time.insert(std::make_pair(stime, etime));
			}
		}

		set_map_second_value_by_key(key, daily_config_map_, value);
		set_map_second_value_by_key(value.id, daily_config_key_map_, key);
	}
}


void Config_Cache_Daily::refresh_today_activity_award_config_cache(void) {
	const Json::Value& config = CONFIG_INSTANCE->config_json()["helper"]["today_activity_award"];
	today_act_group_map_.clear();today_act_award_map_.clear();
	today_act_group_max_end = 0;
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Activity_Award_Config aac;
	Activity_Group_Config agc;
	for (uint i = 0; i < config.size(); ++i) {
		const Json::Value& json = config[i];
		aac.reset();
		aac.id = json["id"].asInt();
		aac.act = json["activity"].asInt();
		aac.award_id = json["award_id"].asInt();

		agc.reset();
		agc.group = json["group"].asInt();
		agc.start = json["start"].asInt();
		agc.end = json["end"].asInt();

		if(agc.end > today_act_group_max_end) {
			today_act_group_max_end = agc.end;
		}

		set_map_second_value_by_key(agc.group, today_act_group_map_,  agc);
		set_map_second_value_by_key(aac.id, today_act_award_map_[agc.group], aac);
	}
	if(today_act_group_max_end == 0) {
		LOG_ABORT("");
	}
}

void Config_Cache_Daily::refresh_all_activity_award_config_cache(void) {
	const Json::Value& config = CONFIG_INSTANCE->config_json()["helper"]["all_activity_award"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Activity_Award_Config aac;
	for (uint i = 0; i < config.size(); ++i) {
		const Json::Value& json = config[i];
		aac.reset();
		aac.id = json["id"].asInt();
		aac.act = json["activity"].asInt();
		aac.award_id = json["award_id"].asInt();
		aac.num = json["num"].asInt();
		set_map_second_value_by_key(aac.id, all_act_award_map_, aac);
	}
}

void Config_Cache_Daily::refresh_all_elite_explore_config_cache(void) {
	const Json::Value& config = CONFIG_INSTANCE->config_json()["helper"]["elite_explore"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	for (uint i = 0; i < config.size(); ++i) {
		const Json::Value& json = config[i];
		int id = json["id"].asInt();
		int force = json["force"].asInt();
		set_map_second_value_by_key(id, elite_explore_map_, force);
	}
}

void Config_Cache_Daily::refresh_all_check_in_config_cache(void) {
	const Json::Value& config = CONFIG_INSTANCE->config_json()["helper"]["check_in"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Check_In_Conf_Map check_in_conf_map_;
	for (uint i = 0; i < config.size(); ++i) {
		const Json::Value& json = config[i];
		int group_id = json["group_id"].asInt();
		check_in_conf_map_.clear();
		Check_In_Conf value;
		for (uint t = 0; t < json["val"].size(); ++t) {
			value.reset();
			const Json::Value& val_json = json["val"][t];
			if(val_json.size() < 2) {
				continue;
			}

			value.day = val_json["day"].asInt();
			uint32_t item = val_json["item_id"].asInt();
			if(item == PT_EXP_CURRENT) {
				value.exp = val_json["num"].asInt();
			} else {
				uint8_t type = val_json["type"].asInt();
				switch(type) {
					case 1:{
						value.item_id = item;
						break;
					}
					case 2:{
						value.money_id = item;
						break;
					}
				}
			}

			value.num = val_json["num"].asInt();
			value.vip_double = val_json["vip_double"].asInt();

			check_in_conf_map_.insert(std::make_pair(value.day, value));
		}


		set_map_second_value_by_key(group_id, check_in_group_map_, check_in_conf_map_);
	}
}
