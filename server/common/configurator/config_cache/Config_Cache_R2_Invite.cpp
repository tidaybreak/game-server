/*
 * Config_Cache_R2_Invite.cpp
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 */

#include <Config_Cache_R2_Invite.h>
#include "Configurator.h"
#include "Json_Utility.h"
#include "fighter/Fighter_Def.h"

uint32_t Config_Cache_R2_Invite::get_common_config_val(uint32_t type) {
	if(common_conf.find(type) != common_conf.end()) {
		return common_conf[type];
	}
	return 0;
}

void Config_Cache_R2_Invite::refresh_config_cache(void) {
	PERF_MON("Config_Cache_R2_Invite");
	refresh_common_config_cache();
}

void Config_Cache_R2_Invite::refresh_common_config_cache(void) {
	common_conf.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["r2_invite"]["common"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return;
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];

		uint32_t key = json["type"].asInt();
		uint32_t val = json["val"].asInt();
		common_conf[key] = val;
	}
}
