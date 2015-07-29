/*
 * Auction_Config.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Config_Cache_Auction.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "auction/Auction_Config.h"

const Auction_Goods_Config_Map &Config_Cache_Auction::get_sys_auction_goods_map(void) {
	return sys_auction_goods_map;
}

const Auction_Costs_Config *Config_Cache_Auction::get_auction_costs_by_continue_time(uint32_t continue_time) {
	return get_map_second_pointer_by_key(continue_time, auction_costs_map);
}

const uint32_t Config_Cache_Auction::get_comm_config_val(const uint32_t key) {
	return comm_map[key];
}

void Config_Cache_Auction::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Auction");
	refresh_sys_auction_goods_config_cache();
	refresh_costs_config_cache();
	refresh_comm_config_cache();
}

void Config_Cache_Auction::refresh_comm_config_cache(void) {
	comm_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["auction"]["comm"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		//LOG_ABORT("not have Active config: costs.json");
		return;
	}
	for (uint i = 0; i < config_json.size(); ++i) {
		const Json::Value& json = config_json[i];

		uint32_t key = json["key"].asInt();
		uint32_t val = json["val"].asInt();
		comm_map[key] = val;
	}
}

void Config_Cache_Auction::refresh_costs_config_cache(void) {
	auction_costs_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["auction"]["costs"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		LOG_ABORT("not have Active config: costs.json");
	}
	Auction_Costs_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		value.reset();
		const Json::Value& json = config_json[i];

		value.keep_costs = json["keep_costs"].asInt();
		value.ann_costs = json["ann_costs"].asInt();
		value.ann_id = json["ann_id"].asInt();
		value.continue_time = json["continue_time"].asInt();

		set_map_second_value_by_key(value.continue_time, auction_costs_map, value);
	}
}

void Config_Cache_Auction::refresh_sys_auction_goods_config_cache(void) {
	sys_auction_goods_map.clear();
	const Json::Value& config_json = CONFIG_INSTANCE->config_json()["auction"]["hot_goods"];
	if (config_json == Json::Value::null || config_json.size() == 0) {
		return ;
		//LOG_ABORT("not have Active config: hot_goods.json");
	}
	Auction_Goods_Config value;
	for (uint i = 0; i < config_json.size(); ++i) {
		value.reset();
		const Json::Value& json = config_json[i];

		value.id = json["id"].asInt();
		value.item_id = json["item_id"].asInt();
		value.num = json["num"].asInt();
		value.continue_time = json["continue_time"].asInt();
		value.auction_price = json["auction_price"].asInt();
		value.ones_price = json["ones_price"].asInt();

		uint32_t itime = json["start_time"].asInt();
		uint32_t second = itime % 100;
		uint32_t minute = (itime % 10000)/100;
		uint32_t hour = (itime % 1000000)/10000;
		uint32_t day = (itime % 100000000)/1000000;
		uint32_t month = (itime % 10000000000)/100000000;
		uint32_t year = itime/10000000000;
		Date_Time date = Date_Time(day, month, year, hour, minute, second);
		value.start_time.sec(date.time_sec());

		set_map_second_value_by_key(value.id, sys_auction_goods_map, value);

	}
}
