/*
 * Upgrade_Config.cpp
 *
 *  Created on: 2014-07-07
 *      Author: "xiaoliang"
 */

#include "Config_Cache_Upgrade.h"
#include "Configurator.h"
#include "Json_Utility.h"

const Upgrade_Conf_Map &Config_Cache_Upgrade::upgrade_conf_map(void) const {
	return upgrade_conf_map_;
}
void Config_Cache_Upgrade::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Upgrade");
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["helper"]["upgrade"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Helper config: upgrade.json");
	}
	upgrade_conf_map_.clear();
	Upgrade_Conf upg_conf;
	for (uint i = 0; i < config_json.size(); ++i) {
		upg_conf.reset();
		const Json::Value& json = config_json[i];
		upg_conf.id = json["id"].asInt();
		for (uint t = 0; t < json["expect_val"].size(); ++t) {
			upg_conf.level_expect.insert(std::make_pair(json["expect_val"][t]["level"].asInt(),json["expect_val"][t]["expect_val"].asInt()));
		}
		set_map_second_value_by_key(upg_conf.id, upgrade_conf_map_, upg_conf);
	}

}

