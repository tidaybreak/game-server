/*
 * Flowers_Config.cpp
 *
 *  Created on: 2014-07-29
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Flowers.h"
#include "Configurator.h"
#include "Json_Utility.h"

const Flowers_Config *Config_Cache_Flowers::get_flowers_conf_by_item_id(uint32_t item_id) {
	return get_map_second_pointer_by_key(item_id, flowers_conf_map_);
}

void Config_Cache_Flowers::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Flowers");
	flowers_conf_map_.clear();
	const Json::Value& config = CONFIG_INSTANCE->config_json()["flowers"]["list"];
	if (config == Json::Value::null || config.size() == 0) {
		return;
	}
	Flowers_Config value;
	for (uint i = 0; i < config.size(); ++i) {
		value.reset();
		const Json::Value& json = config[i];
		value.item_id = json["item_id"].asInt(); //道具ID
		value.have_msg = json["have_msg"].asInt(); //是否允许寄语
		value.effect = json["effect"].asInt();//飘花效果
		value.announ = json["announ"].asInt();//系统广播
		value.charm = json["charm"].asInt();//魅力值增加数
		value.friend_num = json["friend"].asInt();//友好度增加数
		set_map_second_value_by_key(value.item_id, flowers_conf_map_, value);
	}
}

