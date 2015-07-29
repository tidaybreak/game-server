/*
 * Package_Config.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: ChenLong
 */

#include "Config_Cache_Package.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "pack/Pack_Def.h"
#include "fighter/Fighter_Def.h"

const Package_Config &Config_Cache_Package::package_config(void) const {
	return package_config_;
}

const Trade_Limit_Config &Config_Cache_Package::trade_limit_config(bool is_inner) const {
	if (is_inner) {
		return package_config_.inner_trade_limit_conf_;
	} else {
		return package_config_.normal_trade_limit_conf_;
	}
}

void Config_Cache_Package::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Package");

	const Json::Value &pack_config = CONFIG_INSTANCE->config_json()["package"]["package"];

	package_config_.super_item_id = pack_config["super_item_id"].asInt();

	package_config_.normal_trade_limit_conf_.reset();
	package_config_.inner_trade_limit_conf_.reset();
	const Json::Value &normal_config_json = pack_config["trade_limit"]["normal"];
	package_config_.normal_trade_limit_conf_.gold_pay_limit = normal_config_json["gold_pay_limit"].asInt();
	package_config_.normal_trade_limit_conf_.gold_get_limit = normal_config_json["gold_get_limit"].asInt();
	package_config_.normal_trade_limit_conf_.copper_pay_limit = normal_config_json["copper_pay_limit"].asInt();
	package_config_.normal_trade_limit_conf_.copper_get_limit = normal_config_json["copper_get_limit"].asInt();

	const Json::Value &inner_config_json = pack_config["trade_limit"]["inner"];
	package_config_.inner_trade_limit_conf_.gold_pay_limit = inner_config_json["gold_pay_limit"].asInt();
	package_config_.inner_trade_limit_conf_.gold_get_limit = inner_config_json["gold_get_limit"].asInt();
	package_config_.inner_trade_limit_conf_.copper_pay_limit = inner_config_json["copper_pay_limit"].asInt();
	package_config_.inner_trade_limit_conf_.copper_get_limit = inner_config_json["copper_get_limit"].asInt();


	//line_cap_conf_
	const Json::Value &cap_conf = CONFIG_INSTANCE->config_json()["package"]["pack_cap"];
	if (cap_conf == Json::Value::null) {
		LOG_ABORT("error capacity empty");
	}

	Line_Capacity_Detail line_cap;
	UInt_UInt_Map lvl_grid;
	size_t size = cap_conf.size();
	for (size_t i = 0; i < size; ++i) {
		line_cap.line = cap_conf[i]["line"].asInt();
		line_cap.lvl = cap_conf[i]["lvl"].asInt();
		line_cap.gold = cap_conf[i]["diamond"].asInt();
		for (int j = 0; j < Pack::PACKAGE_GRID_PER_LINE; j++) {
			line_cap.grid = (line_cap.line - 1) * Pack::PACKAGE_GRID_PER_LINE + j;
			set_map_second_value_by_key(line_cap.grid, package_config_.pack_line_cap_conf_, line_cap);//从0开始,to251
		}

		UInt_UInt_Map::iterator it = lvl_grid.find(line_cap.lvl);
		if (it != lvl_grid.end()) {
			if (it->second < line_cap.grid) {
				it->second = line_cap.grid;
			}
		} else {
			lvl_grid.insert(std::make_pair(line_cap.lvl, line_cap.grid));
		}
	}
	uint32_t lvl_prev = 1;
	int last_grid = -1;
	for (UInt_UInt_Map::iterator it = lvl_grid.begin(); it != lvl_grid.end(); ++it) {
		for (; lvl_prev < it->first; ++lvl_prev) {
			this->package_config_.pack_grid_lvl_conf_.insert(std::make_pair(lvl_prev, last_grid));
		}
		last_grid = it->second;
		this->package_config_.pack_grid_lvl_conf_.insert(std::make_pair(lvl_prev, last_grid));
		++lvl_prev;
	}
//	if (lvl_prev < MAX_LEVEL) {
		for (; lvl_prev <= MAX_LEVEL; ++lvl_prev) {
			this->package_config_.pack_grid_lvl_conf_.insert(std::make_pair(lvl_prev, last_grid));
		}
//	}

	refresh_rune_stone_grid();
}

void Config_Cache_Package::refresh_rune_stone_grid(void) {
	package_config_.rune_stone_pack_grid_cfg_map_.clear();
	Rune_Stone_Pack_Grid_Cfg cfg_row_info;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["package"]["rune_stone_pack"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.grid = (*it_json)["grid"].asInt();
		ABORT_INT_ZERO(cfg_row_info.grid);
		cfg_row_info.diamond = (*it_json)["diamond"].asInt();

		set_map_second_value_by_key(cfg_row_info.grid, package_config_.rune_stone_pack_grid_cfg_map_, cfg_row_info);
	}
}

const Rune_Stone_Pack_Grid_Cfg *Config_Cache_Package::find_rune_stone_pack_grid_cfg(const int grid) const {
	return get_map_second_pointer_by_key(grid, package_config_.rune_stone_pack_grid_cfg_map_);
}
