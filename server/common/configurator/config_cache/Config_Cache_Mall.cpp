/*
 * Config_Cache_Mall.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Mall.h"
#include "Configurator.h"
#include "Json_Utility.h"

const UInt_Vec &Config_Cache_Mall::get_fast_buy_tabs_sort(void) {
	return fast_buy_tabs_sort;
}

const UInt_Set &Config_Cache_Mall::get_activity_id_set(void) {
	return activity_id_set;
}

const Mall_Activity_Price_Vec* Config_Cache_Mall::find_activity_price_set_by_tabs_item(uint32_t tabs, uint32_t item_id) {
	long long int fin_key = item_id * pow(10,4) + tabs;
	return get_map_second_pointer_by_key(fin_key, activity_price_map);
}

void Config_Cache_Mall::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Mall");

	refresh_fast_buy_tabs_sort_config_cache();
	refresh_activity_price_config_cache();
}

void Config_Cache_Mall::refresh_fast_buy_tabs_sort_config_cache(void) {
	fast_buy_tabs_sort.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["mall"]["fast_buy_tabs_order"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		LOG_ABORT("fast_buy_tabs_order");
	}

	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		uint32_t tabs = json["tabs"].asInt();
		fast_buy_tabs_sort.push_back(tabs);
	}
}

void Config_Cache_Mall::refresh_activity_price_config_cache(void) {
	activity_price_map.clear();activity_id_set.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["mall"]["activity_price"];
	if (config_json == Json::Value::null || config_json.size() <= 0) {
		return;
	}
	Mall_Activity_Price set;
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];
		set.reset();
		set.act_id =  json["act_id"].asInt();
		if(set.act_id) {
			set.tabs =  json["tabs"].asInt();
			set.item_id =  json["item_id"].asInt();
			set.gender =  json["gender"].asInt();
			set.price =  json["price"].asInt();

			long long int fin_key = set.item_id * pow(10,4) + set.tabs;
			//set_map_second_value_by_key(fin_key, activity_price_map, set);
			activity_price_map[fin_key].push_back(set);
			if(activity_id_set.find(set.act_id) == activity_id_set.end()) {
				activity_id_set.insert(set.act_id);
			}
		}
	}
}

