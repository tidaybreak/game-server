/*
 * Config_Cache_Activity.cpp
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 */

#include <Config_Cache_Activity.h>
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"

const Vip37_Conf_Map &Config_Cache_Activity::get_vip_config(void) {
	return vip37_conf;
}

void Config_Cache_Activity::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Activity");
	refreshvip_config_cache();
	refreshphone_config_cache();
}

void Config_Cache_Activity::refreshvip_config_cache(void) {
	vip37_conf.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["37activity"]["vip"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
	}
	Vip37_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		value.reset();

		value.id = json["id"].asInt();
		value.vip_lv = json["vip_lv"].asInt();

		refresh_active_reward_info_config(json["reward"], value.reward);

		set_map_second_value_by_key(value.id, vip37_conf, value);
	}
}

void Config_Cache_Activity::refreshphone_config_cache(void) {
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["37activity"]["phone_reward"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
	}
	refresh_active_reward_info_config(config_json, phone37_reward);
}

void Config_Cache_Activity::refresh_active_reward_info_config(const Json::Value& item_conf, Active_Reward_Info_Config &info) {
	info.reset();
	if(item_conf != Json::Value::null && item_conf.size()) {
		for (uint i = 0; i < item_conf.size(); ++i) {
			const Json::Value& reward_item_json = item_conf[i];

			uint32_t item_id = reward_item_json["item_id"].asInt();
			uint32_t value_ = reward_item_json["num"].asInt();
			uint32_t career = reward_item_json["career"].asInt();

			if(item_id == PT_EXP_CURRENT) {
				info.exp += value_;
				info.career = career;
			} else {
				uint8_t type = reward_item_json["type"].asInt();
				switch(type) {
					case 1:{
						Active_Item_Config item;
						item.reset();
						item.item_id = item_id;
						item.num = value_;
						item.career = career;
						info.item.push_back(item);
						break;
					}
					case 2:{
						Active_Money_Config money;
						money.reset();
						money.money_id = item_id;
						money.num = value_;
						money.career = career;
						info.money.push_back(money);
						break;
					}
				}
			}

		}
	}
}
