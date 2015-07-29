/*
 * Config_Cache_Icon.cpp
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#include "Config_Cache_Icon.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "icon/Icon_Config.h"

const Icon_Config_Map& Config_Cache_Icon::icon_config(void) const {
	return icon_config_map;
}

const Icon_Base_Config* Config_Cache_Icon::find_config(uint16_t id) const {
	return get_map_second_pointer_by_key(id, icon_config_map);
}

void Config_Cache_Icon::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Icon");
	const Json::Value& icon_json_list = CONFIG_INSTANCE->config_json()["icon"]["icon"];
	if (icon_json_list == Json::Value::null || icon_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Icon");
	}

	Icon_Base_Config icon_base_cfg;
	for (uint i = 0; i < icon_json_list.size(); ++i) {
		icon_base_cfg.reset();
		const Json::Value& icon_json = icon_json_list[i];
		icon_base_cfg.icon_id = (uint16_t)icon_json["id"].asInt();
		icon_base_cfg.level = icon_json["level"].asInt();
		icon_base_cfg.item_id = (uint32_t)icon_json["item_id"].asUInt();
		icon_base_cfg.is_join_gang = icon_json["is_join_gang"].asInt() ? true : false;
		icon_base_cfg.type = (uint8_t)icon_json["type"].asInt();
		icon_base_cfg.is_forever_close = (icon_json["is_forever_close"].asUInt() == 1 ) ? true : false;
		icon_base_cfg.forver_close_level = (uint32_t)icon_json["forver_close_level"].asUInt();

		if(icon_json["task"] != Json::Value::null && icon_json["task"].size() > 0) {
			for(uint j = 0; j < icon_json["task"].size(); ++j) {
				const Json::Value &task_json = icon_json["task"][j];
				Task_Open_Condition task_open_condition;
				task_open_condition.reset();
				task_open_condition.task_id = task_json["task_id"].asUInt();
				task_open_condition.task_status = task_json["task_status"].asUInt();
				icon_base_cfg.task_open_condition_list.push_back(task_open_condition);
			}
		}

		set_map_second_value_by_key(icon_base_cfg.icon_id, icon_config_map, icon_base_cfg);
	}
}
