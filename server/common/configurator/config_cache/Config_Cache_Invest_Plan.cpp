/*
 * Config_Cache_Invest_Plan.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */

#include "Config_Cache_Invest_Plan.h"
#include "Configurator.h"
#include "Json_Utility.h"

const Month_Card_Invest_Cfg& Config_Cache_Invest_Plan::get_month_card_invest_cfg(void) const {
	return month_card_invest_cfg;
}

const Upgrade_Invest_Cfg& Config_Cache_Invest_Plan::get_upgrade_invest_cfg(void) const {
	return upgrade_invest_cfg;
}

const Level_Profit_Cfg_Map& Config_Cache_Invest_Plan::get_level_profit_cfg_map() const {
	return level_profit_cfg_map;
}

const Vip_Defualt_Invest_Cfg_Map& Config_Cache_Invest_Plan::get_vip_defualt_invest_cfg_map() const {
	return vip_default_invest_cfg_map;
}

const uint32_t* Config_Cache_Invest_Plan::find_Level_Profit_Cfg(const uint32_t level) {
	Level_Profit_Cfg_Map::const_iterator it = level_profit_cfg_map.find(level);
	if(it != level_profit_cfg_map.end()) {
		return &(it->second);
	}
	return 0;
}

const uint32_t* Config_Cache_Invest_Plan::find_vip_default_invest_cfg(const uint32_t vip) {
	Vip_Defualt_Invest_Cfg_Map::const_iterator it = vip_default_invest_cfg_map.find(vip);
	if(it != vip_default_invest_cfg_map.end()) {
		return &(it->second);
	}
	return 0;
}

void Config_Cache_Invest_Plan::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Invest_Plan");
	refresh_month_card_invest_cfg();
	refresh_upgrade_invest_cfg();
	refresh_level_profit_cfg();
	refresh_vip_default_upgrade_invest_cfg();
}

void Config_Cache_Invest_Plan::refresh_month_card_invest_cfg() {
	const Json::Value& month_card_invest_json_list = CONFIG_INSTANCE->config_json()["invest_plan"]["month_card_invest"];
	if (month_card_invest_json_list == Json::Value::null || month_card_invest_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Invest_Plan::refresh_month_card_invest_cfg");
	}

	for (uint i = 0; i < month_card_invest_json_list.size(); ++i) {
		month_card_invest_cfg.reset();
		const Json::Value& month_card_json = month_card_invest_json_list[i];
		month_card_invest_cfg.invest_money = (uint32_t)month_card_json["invest_money"].asUInt();
		month_card_invest_cfg.profit_immediately = (uint32_t)month_card_json["profit_immediately"].asUInt();
		month_card_invest_cfg.profit_per_day = (uint32_t)month_card_json["profit_per_day"].asUInt();
		break;
	}
}

void Config_Cache_Invest_Plan::refresh_upgrade_invest_cfg() {
	const Json::Value& upgrade_invest_json_list = CONFIG_INSTANCE->config_json()["invest_plan"]["upgrade_invest"];
	if (upgrade_invest_json_list == Json::Value::null || upgrade_invest_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Invest_Plan::refresh_upgrade_invest_cfg");
	}

	for (uint i = 0; i < upgrade_invest_json_list.size(); ++i) {
		upgrade_invest_cfg.reset();
		const Json::Value& upgrade_invest_json = upgrade_invest_json_list[i];
		upgrade_invest_cfg.min_invest_money = (uint32_t)upgrade_invest_json["min_invest_money"].asUInt();
		upgrade_invest_cfg.max_invest_money = (uint32_t)upgrade_invest_json["max_invest_money"].asUInt();
		upgrade_invest_cfg.profit_immediately = (uint32_t)upgrade_invest_json["profit_immediately"].asUInt();
		upgrade_invest_cfg.max_invest_level = (uint32_t)upgrade_invest_json["max_level"].asUInt();
		break;
	}
}

void Config_Cache_Invest_Plan::refresh_level_profit_cfg() {
	const Json::Value& level_profit_json_list = CONFIG_INSTANCE->config_json()["invest_plan"]["level_profit"];
	if (level_profit_json_list == Json::Value::null || level_profit_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Invest_Plan::refresh_level_profit_cfg");
	}

	level_profit_cfg_map.clear();
	for (uint i = 0; i < level_profit_json_list.size(); ++i) {
		const Json::Value& level_profit_json = level_profit_json_list[i];
		uint32_t player_level = (uint32_t)level_profit_json["player_level"].asUInt();
		uint32_t profit_percent = (uint32_t)level_profit_json["profit_percent"].asUInt();
		level_profit_cfg_map.insert(std::make_pair(player_level, profit_percent));
	}
}

void Config_Cache_Invest_Plan::refresh_vip_default_upgrade_invest_cfg() {
	const Json::Value& vip_default_upgrade_invest_json_list = CONFIG_INSTANCE->config_json()["invest_plan"]["vip_default_upgrade_invest"];
	if (vip_default_upgrade_invest_json_list == Json::Value::null || vip_default_upgrade_invest_json_list.size() == 0) {
		LOG_ABORT("Config_Cache_Invest_Plan::refresh_vip_default_upgrade_invest_cfg");
	}

	vip_default_invest_cfg_map.clear();
	for (uint i = 0; i < vip_default_upgrade_invest_json_list.size(); ++i) {
		const Json::Value& vip_default_upgrade_invest_json = vip_default_upgrade_invest_json_list[i];
		uint32_t vip_level = (uint32_t)vip_default_upgrade_invest_json["vip_level"].asUInt();
		uint32_t default_invest = (uint32_t)vip_default_upgrade_invest_json["default_invest"].asUInt();
		vip_default_invest_cfg_map.insert(std::make_pair(vip_level, default_invest));
	}
}
