/*
 * Covenant_Config.cpp
 *
 *  Created on: 2014-07-15
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Covenant.h"
#include "Configurator.h"
#include "Json_Utility.h"


const Covenant_Conf *Config_Cache_Covenant::find_covenant_conf_by_id(uint32_t id) {
	return get_map_second_pointer_by_key(id, covenant_conf_map_);
}

const Covenant_Conf_Map &Config_Cache_Covenant::covenant_conf_map(void) {
	return covenant_conf_map_;
}

void Config_Cache_Covenant::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Covenant");
	covenant_conf_map_.clear();
	const Json::Value& covenant_config = CONFIG_INSTANCE->config_json()["covenant"]["covenant"];
	if (covenant_config == Json::Value::null || covenant_config.size() == 0) {
		return;
	}
	Covenant_Conf covenant;

	for (uint i = 0; i < covenant_config.size(); ++i) {
		covenant.reset();

		const Json::Value& covenant_json = covenant_config[i];

		covenant.id = covenant_json["id"].asInt(); //盟约ID
		covenant.name = covenant_json["name"].asString(); //精灵名称
		covenant.days = covenant_json["days"].asInt();//开通/续费天数
		covenant.price = covenant_json["price"].asInt();//开通或续费价格
		covenant.become_time = covenant_json["become_time"].asInt();//特殊精灵,在当天有持续时间, 表示当天开启后持续时间过期时间
		covenant.mail_id = covenant_json["mail_id"].asInt();//过期提示邮件id

		const Json::Value& prop_json = covenant_json["status_val"];
		Effect_Argv_Prop prop;
		if(prop_json != Json::Value::null && prop_json.size()) {
			for (uint i = 0; i < prop_json.size(); ++i) {
				const Json::Value& prop_json_val = prop_json[i];
				if(!prop_json_val["status_id"].empty()) {
					int32_t status_id =  prop_json_val["status_id"].asInt();
					prop.reset();
					prop.effect_value = prop_json_val["effect_value"].asDouble();
					prop.i_effect_val1 = prop_json_val["i_effect_val1"].asInt();
					prop.i_effect_val2 = prop_json_val["i_effect_val2"].asInt();
					prop.i_effect_val3 = prop_json_val["i_effect_val3"].asInt();
					prop.d_effect_val1 = prop_json_val["d_effect_val1"].asDouble();
					prop.d_effect_val2 = prop_json_val["d_effect_val2"].asDouble();
					prop.d_effect_val3 = prop_json_val["d_effect_val3"].asDouble();
					prop.d_effect_val4 = prop_json_val["d_effect_val4"].asDouble();
					Effect_Argv_Prop_Map::iterator it = covenant.prop_map.find(status_id);
					if(it == covenant.prop_map.end()) {
						Effect_Argv_Prop_Vec prop_vec;
						prop_vec.clear();
						if(prop.i_effect_val1) {
							prop_vec.push_back(prop);
						}
						covenant.prop_map.insert(std::make_pair(status_id, prop_vec));
					} else {
						it->second.push_back(prop);
					}
				}
			}
		}

		set_map_second_value_by_key(covenant.id, covenant_conf_map_, covenant);
	}
}
