/*
 * Config_Cache_Reward_Task.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: gaojianshan
 */

#include "Config_Cache_Reward_Task.h"
#include "Configurator.h"
#include "Json_Utility.h"


const Reward_Task_Config& Config_Cache_Reward_Task::get_config() const {
	return reward_task_cfg;
}

Reward_Task_Config& Config_Cache_Reward_Task::get_config_no_const() {
	return reward_task_cfg;
}

void Config_Cache_Reward_Task::refresh_config_cache() {
	PERF_MON("Config_Cache_Reward_Task");

	//热更前，先清空
	reward_task_cfg.reset();

	const Json::Value& reward_task_json_list = CONFIG_INSTANCE->config_json()["reward_task"]["reward_task"];
	if (reward_task_json_list == Json::Value::null || reward_task_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Reward_Task");
	}

	for (uint i = 0; i < reward_task_json_list.size(); ++i) {
		const Json::Value& reward_task_json = reward_task_json_list[i];
		reward_task_cfg.auto_refresh = (uint32_t)reward_task_json["auto_refresh"].asInt();
		reward_task_cfg.free_refresh_max_count_per_day = (uint32_t)reward_task_json["free_refresh_max_count_per_day"].asInt();
		reward_task_cfg.accept_max_count_per_day = (uint32_t)reward_task_json["accept_max_count_per_day"].asInt();
		reward_task_cfg.do_max_count_at_same_time = (uint32_t)reward_task_json["do_max_count_at_same_time"].asInt();
		reward_task_cfg.open_level = (uint8_t)reward_task_json["open_level"].asInt();
		reward_task_cfg.money_type = (uint32_t)reward_task_json["money_type"].asInt();
		reward_task_cfg.money_num = (uint32_t)reward_task_json["money_num"].asInt();
		reward_task_cfg.green = (uint8_t)reward_task_json["green"].asInt();
		reward_task_cfg.blue = (uint8_t)reward_task_json["blue"].asInt();
		reward_task_cfg.purple = (uint8_t)reward_task_json["purple"].asInt();
		reward_task_cfg.orange = (uint8_t)reward_task_json["orange"].asInt();

		reward_task_cfg.white_chance = (uint8_t)reward_task_json["white_chance"].asInt();
		reward_task_cfg.green_chance = (uint8_t)reward_task_json["green_chance"].asInt();
		reward_task_cfg.blue_chance = (uint8_t)reward_task_json["blue_chance"].asInt();
		reward_task_cfg.purple_chance = (uint8_t)reward_task_json["purple_chance"].asInt();
		reward_task_cfg.orange_chance = (uint8_t)reward_task_json["orange_chance"].asInt();
		reward_task_cfg.item_id = (uint32_t)reward_task_json["item_id"].asInt();
		reward_task_cfg.item_num = (uint32_t)reward_task_json["item_num"].asInt();

		if(reward_task_json["white"] != Json::Value::null && reward_task_json["white"].size() > 0) {
			for(uint j = 0; j < reward_task_json["white"].size(); ++j) {
				const Json::Value &white_reward = reward_task_json["white"][j];
				White_Reward white;
				white.reset();
				white.accept_player_level = (uint16_t)white_reward["accept_player_level"].asInt();
				white.exp = (uint32_t)white_reward["exp"].asInt();
				white.coin = (uint32_t)white_reward["coin"].asInt();
				reward_task_cfg.white_reward_list.push_back(white);
			}
		}

		break;
	}
}
