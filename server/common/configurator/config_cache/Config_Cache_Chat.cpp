/*
 * Config_Cache_Chat.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Chat.h"
#include "Configurator.h"
#include "Json_Utility.h"

const uint32_t Config_Cache_Chat::get_common_cfg_by_key(uint32_t key) const {
	UInt_UInt_Map::const_iterator it = common_cfg.find(key);
	if(it != common_cfg.end()) {
		return it->second;
	}
	return 0;
}

const String_Set &Config_Cache_Chat::get_white_list(void) const {
	return white_list;
}

const String_Set &Config_Cache_Chat::get_black_list(void) const {
	return black_list;
}

void Config_Cache_Chat::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Chat");

	refresh_common_config_cache();
	refresh_while_list_config_cache();
	refresh_black_list_config_cache();
}
void Config_Cache_Chat::refresh_common_config_cache(void) {
	common_cfg.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["chat"]["common"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		//MSG_DEBUG("not have answer common.json");
		return;
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		uint32_t key = json["key"].asInt();
		uint32_t value = json["val"].asInt();
		common_cfg[key] = value;
	}
}

void Config_Cache_Chat::refresh_while_list_config_cache(void) {
	white_list.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["chat"]["whitelist"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		return;
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		std::string value;
		value.clear();
		if(json["val"].isInt()) {
			int b = json["val"].asInt();
			std::stringstream str;
			str << b;
			value = str.str();
		} else if(json["val"].isString()) {
			value = json["val"].asString();
		} else {
			MSG_DEBUG("NOT DO WHITE_LIST CFG TYPE: %d", i);
		}
		if(!value.empty()) {
			white_list.insert(value);
		}
	}
}

void Config_Cache_Chat::refresh_black_list_config_cache(void) {
	black_list.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["chat"]["blacklist"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		return;
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		std::string value;
		value.clear();
		if(json["val"].isInt()) {
			int b = json["val"].asInt();
			std::stringstream str;
			str << b;
			value = str.str();
		} else if(json["val"].isString()) {
			value = json["val"].asString();
		} else {
			MSG_DEBUG("NOT DO BLACK_LIST CFG TYPE: %d", i);
		}
		if(!value.empty()) {
			black_list.insert(value);
		}
	}
}

