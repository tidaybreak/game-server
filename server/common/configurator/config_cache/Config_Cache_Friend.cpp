/*
 * Friend_Config.cpp
 *
 *  Created on: 2014-06-09
 *      Author: xiaoliang
 */

#include "Config_Cache_Friend.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "friend/Friend_Config.h"


void Config_Cache_Friend::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Friend");
	refresh_set_head_photo_config_cache();
	refresh_default_head_photo_config_cache();
	refresh_common_config_cache();
}

void Config_Cache_Friend::refresh_common_config_cache(void) {
	common_config.clear();
	const Json::Value& filend_setting_json = CONFIG_INSTANCE->config_json()["friend"]["common"];
	if (filend_setting_json == Json::Value::null || filend_setting_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < filend_setting_json.size(); ++i) {
		const Json::Value& json = filend_setting_json[i];
		uint32_t key = json["key"].asInt();
		uint32_t val = json["val"].asInt();

		set_map_second_value_by_key(key, common_config, val);
	}
}

void Config_Cache_Friend::refresh_default_head_photo_config_cache(void) {
	const Json::Value& filend_setting_json = CONFIG_INSTANCE->config_json()["friend"]["head_default"];
	if (filend_setting_json == Json::Value::null || filend_setting_json.size() == 0) {
		LOG_ABORT("");
	}

	Friend_Default_Head_Photo_Config base_set;
	for (uint i = 0; i < filend_setting_json.size(); ++i) {
		base_set.reset();
		const Json::Value& mail_json = filend_setting_json[i];
		base_set.head_photo_id = mail_json["ID"].asInt();
		base_set.ob = mail_json["ob"].asInt();

		set_map_second_value_by_key(base_set.ob, default_head_photo_map, base_set);
	}
}

void Config_Cache_Friend::refresh_set_head_photo_config_cache(void) {
	const Json::Value& filend_setting_json = CONFIG_INSTANCE->config_json()["friend"]["head_set"];
	if (filend_setting_json == Json::Value::null || filend_setting_json.size() == 0) {
		LOG_ABORT("");
	}

	Friend_Head_Photo_Config base_set;
	for (uint i = 0; i < filend_setting_json.size(); ++i) {
		base_set.reset();
		const Json::Value& mail_json = filend_setting_json[i];
		base_set.head_photo_id = mail_json["ID"].asInt();
		base_set.hero_id = mail_json["hero_ID"].asInt();
		base_set.open_status = mail_json["open"].asInt();

		set_map_second_value_by_key(base_set.head_photo_id, photo_id_head_photo_map, base_set);
	}

}


const Friend_Head_Photo_Config *Config_Cache_Friend::find_set_head_config_by_photo_id(int32_t photo_id) const {
	return get_map_second_pointer_by_key(photo_id, photo_id_head_photo_map);
}
const Friend_Default_Head_Photo_Config *Config_Cache_Friend::find_default_head_config(uint8_t career, uint8_t gender) const {
	int32_t ob = career*10 + gender;
	return get_map_second_pointer_by_key(ob, default_head_photo_map);
}
const Friend_Head_Photo_Map &Config_Cache_Friend::get_set_head_photo_map(void) const {
	return photo_id_head_photo_map;
}

const uint32_t Config_Cache_Friend::find_common_config_val(uint32_t key) const {
	UInt_UInt_Map::const_iterator iter = common_config.find(key);
	if(iter != common_config.end()) {
		return iter->second;
	}
	return 0;
}
