/*
 * Ranking_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Ranking.h"
#include "Configurator.h"
#include "Json_Utility.h"

void Config_Cache_Ranking::refresh_config_cache(void) {
	Ranking_Config base_set;
	const Json::Value& hero_critical_json = CONFIG_INSTANCE->config_json()["rank"]["conf"];
	if (hero_critical_json == Json::Value::null || hero_critical_json.size() == 0) {
		LOG_ABORT("");
	}
	for (uint i = 0; i < hero_critical_json.size(); ++i) {
		base_set.reset();
		const Json::Value& critical_json = hero_critical_json[i];
		base_set.type = critical_json["type"].asInt();
		base_set.num = critical_json["num"].asInt();
		base_set.time = critical_json["time"].asInt() * 60;
		base_set.day = critical_json["day"].asInt();
		base_set.week = critical_json["week"].asInt();
		set_map_second_value_by_key(base_set.type, ranking_config_, base_set);
	}
}

uint32_t Config_Cache_Ranking::get_time_by_ranking_type(uint32_t type) {
	Ranking_Config_Map::iterator  iter = ranking_config_.find(type);
	if(iter != ranking_config_.end()){
		return iter->second.time;
	}
	return 0;
}

uint32_t Config_Cache_Ranking::get_num_by_ranking_type(uint32_t type) {
	Ranking_Config_Map::iterator  iter = ranking_config_.find(type);
	if(iter != ranking_config_.end()){
		return iter->second.num;
	}
	return 0;
}

Ranking_Config_Map& Config_Cache_Ranking::get_config() {
	return ranking_config_;
}
