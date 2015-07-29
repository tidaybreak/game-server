/*
 * Config_Cache_Rune.cpp
 *
 *  Created on: May 16, 2014
 *      Author: chenyulin
 */

#include "Config_Cache_Rune.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Fighter_Detail.h"
#include "Config_Cache_Role.h"

void Config_Cache_Rune::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Rune");

	refresh_rune_hole_lvl();
	refresh_rune_part();
	refresh_hole_lvl_whole();
}

void Config_Cache_Rune::refresh_rune_hole_lvl(void) {
	Rune_Hole_Lvl_Cfg hole_lvl_cfg;
	const Json::Value &json_hole_lvl = CONFIG_INSTANCE->config_json()["rune"]["rune_hole_lvl"];
	for (Json::Value::iterator it_hole_lvl = json_hole_lvl.begin();
			it_hole_lvl != json_hole_lvl.end(); ++it_hole_lvl) {
		hole_lvl_cfg.reset();

		int hole_lvl = (*it_hole_lvl)["hole_lvl"].asInt();
		ABORT_INT_ZERO(hole_lvl);
		hole_lvl_cfg.hole_lvl = hole_lvl;
		hole_lvl_cfg.energy = (*it_hole_lvl)["energy"].asInt();
		hole_lvl_cfg.cost_copper = (*it_hole_lvl)["cost_copper"].asInt();
		hole_lvl_cfg.material_id = (*it_hole_lvl)["material_id"].asInt();
		hole_lvl_cfg.material_num_once = (*it_hole_lvl)["material_num_once"].asInt();
		hole_lvl_cfg.prop_val = (*it_hole_lvl)["prop_val"].asInt();
		hole_lvl_cfg.prop_val_rate = (*it_hole_lvl)["prop_val_rate"].asInt();
		hole_lvl_cfg.open_level = (*it_hole_lvl)["open_class"].asInt();

		hole_lvl_energy_vec_.push_back(hole_lvl_cfg.energy);

		set_map_second_value_by_key(hole_lvl_cfg.hole_lvl, hole_lvl_cfg_map_, hole_lvl_cfg);
	}
}

void Config_Cache_Rune::refresh_rune_part(void) {
	Rune_Part_Cfg part_cfg;
	const Json::Value &json_part = CONFIG_INSTANCE->config_json()["rune"]["rune_part"];
	for (Json::Value::iterator it_part = json_part.begin();
			it_part != json_part.end(); ++it_part) {
		part_cfg.reset();

		int rune_part = (*it_part)["rune_part"].asInt();
		ABORT_INT_ZERO(rune_part);
		part_cfg.rune_part = rune_part;
		part_cfg.prop_type = (*it_part)["prop_type"].asInt();

		set_map_second_value_by_key(part_cfg.rune_part, part_cfg_map_, part_cfg);
	}
}

void Config_Cache_Rune::refresh_hole_lvl_whole(void) {
	Hole_Lvl_Whole_Cfg cfg_row_info;
	Int_Double prop_value;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune"]["hole_lvl_whole"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cfg_row_info.reset();

		cfg_row_info.hole_lvl_whole = (*it_json)["hole_lvl_whole"].asInt();
		ABORT_INT_ZERO(cfg_row_info.hole_lvl_whole);

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_json)[prop_stream.str()]) {
				prop_value.value = (*it_json)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					cfg_row_info.prop_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		set_map_second_value_by_key(cfg_row_info.hole_lvl_whole, hole_lvl_whole_cfg_map_, cfg_row_info);
	}
}

const Rune_Hole_Lvl_Cfg *Config_Cache_Rune::find_hole_lvl_cfg(const int hole_lvl) const {
	return get_map_second_pointer_by_key(hole_lvl, hole_lvl_cfg_map_);
}

const Rune_Part_Cfg *Config_Cache_Rune::find_part_cfg(const int part) const {
	return get_map_second_pointer_by_key(part, part_cfg_map_);
}

const Rune_Part_Cfg_Map *Config_Cache_Rune::part_cfg_map(void) const {
	return &part_cfg_map_;
}

const Int_Vec *Config_Cache_Rune::hole_lvl_energy_vec(void) const {
	return &hole_lvl_energy_vec_;
}

const Hole_Lvl_Whole_Cfg *Config_Cache_Rune::find_hole_lvl_whole_cfg(const int hole_lvl_whole) const {
	return get_map_second_pointer_by_key(hole_lvl_whole, hole_lvl_whole_cfg_map_);
}

const Hole_Lvl_Whole_Cfg_Map &Config_Cache_Rune::hole_lvl_whole_cfg_map(void) const {
	return hole_lvl_whole_cfg_map_;
}
