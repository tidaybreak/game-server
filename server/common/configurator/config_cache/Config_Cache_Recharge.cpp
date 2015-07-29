/*
 * Config_Cache_Recharge.cpp
 *
 *  Created on: 2014-05-05
 *      Author: xiaoliang
 */

#include "Config_Cache_Recharge.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"
#include "public/Public_Config.h"

const First_Recharge_Reward_Config &Config_Cache_Recharge::get_first_recharge_item_reward(void) {
	return first_recharge_reward;
}

const First_Recharge_Reward_Config &Config_Cache_Recharge::get_two_recharge_item_reward(void) {
	return two_recharge_reward;
}

const Limit_Time_Recharge_Reward_Config &Config_Cache_Recharge::get_limit_time_recharge_reward(void) {
	return limit_time_recharge_reward;
}

const uint32_t Config_Cache_Recharge::get_common_config_by_key(const uint32_t key) {
	return common_config[key];
}

const Recharge_Rebate_Config_Map &Config_Cache_Recharge::get_recharge_rebate_reward(void) {
	return recharge_rebate_reward;
}

const uint32_t Config_Cache_Recharge::get_pay_gift_mail_id_by_pay(uint32_t pay) {
	UInt_UInt_Map::iterator it = pay_gift_conf.find(pay);
	if(it != pay_gift_conf.end()) {
		return it->second;
	}
	return 0;
}

void Config_Cache_Recharge::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Recharge");

	refresh_first_recharge();
	refresh_two_recharge();
	refresh_rebate_recharge();
	refresh_pay_gift_conf();
	refresh_limit_time_recharge();
	refresh_common();
}

void Config_Cache_Recharge::refresh_pay_gift_conf(void) {
	pay_gift_conf.clear();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["recharge"]["pay_gift"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		//LOG_ABORT("Config_Cache_Recharge:pay_gift.json");
		return;
	}
	for (uint i = 0; i < json_list.size(); ++i) {
		const Json::Value& value_json = json_list[i];
		set_map_second_value_by_key(value_json["pay"].asInt(), pay_gift_conf, value_json["mail_id"].asInt());
	}
}

void Config_Cache_Recharge::refresh_rebate_recharge(void) {
	recharge_rebate_reward.clear();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["recharge"]["pay_feedback"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Recharge:pay_feedback.json");
	}
	Recharge_Rebate_Config value;
	for (uint i = 0; i < json_list.size(); ++i) {
		value.reset();
		const Json::Value& value_json = json_list[i];


		if(value_json["reward"] != Json::Value::null && value_json["reward"].size() > 0) {
			for(uint j = 0; j < value_json["reward"].size(); ++j) {
				const Json::Value &json = value_json["reward"][j];

				uint32_t item = json["item_id"].asInt();
				uint32_t num = json["num"].asInt();
				if(item == PT_EXP_CURRENT) {
					value.reward.exp += num;
				} else {
					uint8_t type = json["type"].asInt();
					switch(type) {
						case 1:{
							set_map_second_value_by_key(item, value.reward.reward_item, num);
							break;
						}
						case 2:{
							set_map_second_value_by_key(item, value.reward.reward_money, num);
							break;
						}
					}
				}
			}
		}

		value.id = value_json["id"].asInt();
		value.money = value_json["money"].asInt();
		value.ann_id = value_json["ann_id"].asInt();
		value.act_id = value_json["act_id"].asInt();
		set_map_second_value_by_key(value.id, recharge_rebate_reward, value);
	}
}

void Config_Cache_Recharge::refresh_first_recharge(void) {
	first_recharge_reward.reset();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["recharge"]["first_pay"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Recharge:first_pay.json");
	}

	for (uint i = 0; i < json_list.size(); ++i) {
		const Json::Value& json = json_list[i];

		uint32_t item = json["item_id"].asInt();
		uint32_t num = json["num"].asInt();
		uint32_t time = json["time"].asInt();
		if(item == PT_EXP_CURRENT) {
			first_recharge_reward.exp += num;
		} else {
			uint8_t type = json["type"].asInt();
			switch(type) {
				case 1:{
					//set_map_second_value_by_key(item, first_recharge_reward.reward_item, num);
					first_recharge_reward.reward_item.push_back(First_Recharge_Item_Config(item, num, time));
					break;
				}
				case 2:{
					//set_map_second_value_by_key(item, first_recharge_reward.reward_money, num);
					first_recharge_reward.reward_money.push_back(First_Recharge_Item_Config(item, num, time));
					break;
				}
			}
		}
	}
}

void Config_Cache_Recharge::refresh_two_recharge(void) {
	two_recharge_reward.reset();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["recharge"]["two_pay"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		return;
		//LOG_ABORT("Config_Cache_Recharge:two_pay.json");
	}

	for (uint i = 0; i < json_list.size(); ++i) {
		const Json::Value& json = json_list[i];

		uint32_t item = json["item_id"].asInt();
		uint32_t num = json["num"].asInt();
		uint32_t time = json["time"].asInt();
		if(item == PT_EXP_CURRENT) {
			two_recharge_reward.exp += num;
		} else {
			uint8_t type = json["type"].asInt();
			switch(type) {
				case 1:{
					//set_map_second_value_by_key(item, first_recharge_reward.reward_item, num);
					two_recharge_reward.reward_item.push_back(First_Recharge_Item_Config(item, num, time));
					break;
				}
				case 2:{
					//set_map_second_value_by_key(item, first_recharge_reward.reward_money, num);
					two_recharge_reward.reward_money.push_back(First_Recharge_Item_Config(item, num, time));
					break;
				}
			}
		}
	}
}

void Config_Cache_Recharge::refresh_limit_time_recharge(void) {
	limit_time_recharge_reward.reset();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["recharge"]["limit_time_recharge"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		return;
	}

	for (uint i = 0; i < json_list.size(); ++i) {
		const Json::Value& json = json_list[i];

		uint32_t item = json["item_id"].asInt();
		uint32_t num = json["num"].asInt();
		uint32_t time = json["time"].asInt();
		if(item == PT_EXP_CURRENT) {
			limit_time_recharge_reward.exp += num;
		} else {
			uint8_t type = json["type"].asInt();
			switch(type) {
				case 1:{
					limit_time_recharge_reward.reward_item.push_back(First_Recharge_Item_Config(item, num, time));
					break;
				}
				case 2:{
					limit_time_recharge_reward.reward_money.push_back(First_Recharge_Item_Config(item, num, time));
					break;
				}
			}
		}
	}
}

void Config_Cache_Recharge::refresh_common(void) {
	common_config.clear();
	const Json::Value& json_list = CONFIG_INSTANCE->config_json()["recharge"]["comm"];
	if (json_list == Json::Value::null || json_list.size() == 0) {
		return;
	}
	for (uint i = 0; i < json_list.size(); ++i) {
		const Json::Value& json = json_list[i];
		uint32_t k = json["key"].asInt();
		uint32_t v = json["val"].asInt();
		common_config[k]=v;
	}
}
