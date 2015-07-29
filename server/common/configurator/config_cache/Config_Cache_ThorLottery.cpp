/*
 * Config_Cache_ThorLottery.cpp
 *
 *  Created on: 2014年11月12日
 *      Author: root
 */

#include "Config_Cache_ThorLottery.h"
#include "Configurator.h"
#include "Json_Utility.h"

void Config_Cache_ThorLottery::refresh_config_cache(void) {
	PERF_MON("Config_Cache_ThorLottery");

	const Json::Value &season_json = CONFIG_INSTANCE->config_json()["thorDraw"]["season"];
	ThorLottery_Config cfg;
	for (Json::Value::const_iterator it = season_json.begin(); it != season_json.end(); ++it) {
		cfg.reset();
		cfg.season_id 	= (*it)["season_id"].asInt();
		cfg.start_weeks = (*it)["stat"].asInt();
		cfg.end_weeks 	= (*it)["end"].asInt();
		thorlottery_map_[cfg.season_id] = cfg;
	}

	const Json::Value &order_json = CONFIG_INSTANCE->config_json()["thorDraw"]["order"];
	ThorLottery_Order_Config order_cfg;
	for (Json::Value::const_iterator it = order_json.begin(); it != order_json.end(); ++it) {
		order_cfg.reset();
		int season_id = (*it)["season_id"].asInt();
		order_cfg.order = (*it)["order"].asInt();
		order_cfg.weight = (*it)["weight"].asInt();
		order_cfg.min_lottery = (*it)["lotterty_min"].asInt();
		order_cfg.max_lottery = (*it)["lotterty_max"].asInt();
		order_cfg.server_limit = (*it)["server_limit"].asInt();

		ThorLottery_Map::iterator find_it = thorlottery_map_.find(season_id);
		if(thorlottery_map_.end() == find_it) {
			LOG_ABORT("there is no this season, season id = %d", season_id);
		}
		find_it->second.thorlottery_order_config_map[order_cfg.order] = order_cfg;
	}

	const Json::Value &json = CONFIG_INSTANCE->config_json()["thorDraw"]["reward"];
	ThorLottery_Item_Config item_cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		item_cfg.reset();
		int season_id = (*it)["season_id"].asInt();
		int order_id = (*it)["order"].asInt();
		item_cfg.order = order_id;
		item_cfg.id = (*it)["id"].asInt();
		item_cfg.weight = (*it)["weight"].asInt();
		item_cfg.num = (*it)["num"].asInt();
		item_cfg.bind_state = (*it)["bind_state"].asInt();
		item_cfg.is_broad = (*it)["is_broad"].asInt();
		item_cfg.exchange_cost = (*it)["exchange_cost"].asInt();

		ThorLottery_Map::iterator find_it = thorlottery_map_.find(season_id);
		if (thorlottery_map_.end() == find_it) {
			LOG_ABORT("there is no this season, season id = %d", season_id);
		}
		ThorLottery_Order_Config_Map::iterator find_it_1 = find_it->second.thorlottery_order_config_map.find(order_id);
		if (find_it->second.thorlottery_order_config_map.end() == find_it_1) {
			LOG_ABORT("there is no this order, season_id = %d, order_id =%d, goods_id=%d", season_id, order_id, item_cfg.id);
		}
		find_it_1->second.thorlottery_item_config_map[item_cfg.id] = item_cfg;
	}
}

const ThorLottery_Item_Config* Config_Cache_ThorLottery::get_thorlottery_item_config(int season_id, int order, int item_id) const {

	ThorLottery_Map::const_iterator find_it = thorlottery_map_.find(season_id);
	if (thorlottery_map_.end() == find_it) return 0;

	ThorLottery_Order_Config_Map::const_iterator order_it = find_it->second.thorlottery_order_config_map.find(order);
	if (find_it->second.thorlottery_order_config_map.end() == order_it) return 0;

	ThorLottery_Item_Config_Map::const_iterator item_it = order_it->second.thorlottery_item_config_map.find(item_id);
	if (order_it->second.thorlottery_item_config_map.end() == item_it) return 0;
	return &(item_it->second);
}

const ThorLottery_Config* Config_Cache_ThorLottery::get_thorlottery_config(int season_id) const {
	ThorLottery_Map::const_iterator find_it = thorlottery_map_.find(season_id);
	if (thorlottery_map_.end() == find_it) return 0;
	return &(find_it->second);
}

const ThorLottery_Map& Config_Cache_ThorLottery::get_thorlottery_map(void) const {
	return thorlottery_map_;
}

