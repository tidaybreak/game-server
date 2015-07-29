/*
 * Config_Cache_Rune_Stone.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: chenyulin
 */

#include "Config_Cache_Rune_Stone.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Fighter_Detail.h"
#include "Config_Cache_Role.h"

void Config_Cache_Rune_Stone::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Rune_Stone");

	refresh_item_weight_id();	// 要放在refresh_smelter_idx()之前
	refresh_smelter_idx();
	refresh_exp_lvl();
	refresh_color();
	refresh_mix();
	refresh_combine();
}

void Config_Cache_Rune_Stone::refresh_item_weight_id(void) {
	item_weight_id_cfg_map_.clear();
	Item_Weight_Id_Cfg item_weight_id_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune_stone"]["item_weight_ids"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		item_weight_id_cfg.reset();

		item_weight_id_cfg.item_weight_id = (*it_json)["item_weight_ids"].asInt();
		ABORT_INT_ZERO(item_weight_id_cfg.item_weight_id);
		item_weight_id_cfg.item_id = (*it_json)["item_id"].asInt();
		item_weight_id_cfg.weight = (*it_json)["weight"].asInt();

		set_map_second_value_by_key(item_weight_id_cfg.item_weight_id, item_weight_id_cfg_map_, item_weight_id_cfg);
	}
}

void Config_Cache_Rune_Stone::refresh_smelter_idx(void) {
	smelter_idx_cfg_map_.clear();
	Smelter_Idx_Cfg smelter_idx_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune_stone"]["smelter_idx"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		smelter_idx_cfg.reset();

		smelter_idx_cfg.smelter_idx = (*it_json)["smelter_idx"].asInt();
		ABORT_INT_ZERO(smelter_idx_cfg.smelter_idx);
		smelter_idx_cfg.cost_copper = (*it_json)["cost_copper"].asInt();
		smelter_idx_cfg.point = (*it_json)["point"].asInt();
		smelter_idx_cfg.front_rate = (*it_json)["front_rate"].asInt();
		Json_Utility::assign_int_vec((*it_json)["item_weight_ids"], smelter_idx_cfg.item_weight_ids);

		for (uint16_t i = 0; i < smelter_idx_cfg.item_weight_ids.size(); ++i) {
			const Item_Weight_Id_Cfg* item_weight_id_cfg = find_item_weight_id(smelter_idx_cfg.item_weight_ids[i]);
			if (!item_weight_id_cfg && !item_weight_id_cfg->item_id) {
				continue;
			}
			smelter_idx_cfg.item_ids.push_back(item_weight_id_cfg->item_id);
			smelter_idx_cfg.item_weights.push_back(item_weight_id_cfg->weight);
		}

		set_map_second_value_by_key(smelter_idx_cfg.smelter_idx, smelter_idx_cfg_map_, smelter_idx_cfg);
	}
}

void Config_Cache_Rune_Stone::refresh_exp_lvl(void) {
	exp_lvl_cfg_map_.clear();
	Exp_Lvl_Cfg exp_lvl_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune_stone"]["exp_lvl"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		exp_lvl_cfg.reset();

		exp_lvl_cfg.exp_lvl = (*it_json)["exp_lvl"].asInt();
		ABORT_INT_ZERO(exp_lvl_cfg.exp_lvl);
		exp_lvl_cfg.prop_lvl_factor = (*it_json)["prop_lvl_factor"].asDouble();

		for (int16_t i_color = 1; i_color <= 7; ++i_color) {
			std::ostringstream color_stream;
			color_stream << i_color;
			const Json::Value &jv_color_exp = (*it_json)[color_stream.str()];
			if (Json::Value::null != jv_color_exp) {
				int exp = jv_color_exp.asInt();
				exp_lvl_cfg.color_exp_map.insert(std::make_pair(i_color, exp));
			}
		}

		set_map_second_value_by_key(exp_lvl_cfg.exp_lvl, exp_lvl_cfg_map_, exp_lvl_cfg);
	}
}

void Config_Cache_Rune_Stone::refresh_color(void) {
	color_cfg_map_.clear();
	Rune_Stone_Color_Cfg color_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune_stone"]["rune_stone_color"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		color_cfg.reset();

		color_cfg.rune_stone_color = (*it_json)["rune_stone_color"].asInt();
		ABORT_INT_ZERO(color_cfg.rune_stone_color);
		color_cfg.prop_lvl_factor = (*it_json)["prop_lvl_factor"].asDouble();

		set_map_second_value_by_key(color_cfg.rune_stone_color, color_cfg_map_, color_cfg);
	}
}

void Config_Cache_Rune_Stone::refresh_mix(void) {
	mix_cfg_.reset();
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune_stone"]["rune_stone_mix"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		mix_cfg_.daily_smelt_d_times = (*it_json)["daily_smelt_d_times"].asInt();
		Json_Utility::assign_set_list((*it_json)["exchangeable_items"], mix_cfg_.exchangeable_items);
		Json_Utility::assign_set_list((*it_json)["smelt_output_copper_items"], mix_cfg_.smelt_output_copper_items);
		Json_Utility::assign_set_list((*it_json)["smelt_bufs"], mix_cfg_.smelt_bufs);

		for (int16_t i_color = 1; i_color <= 7; ++i_color) {
			std::ostringstream color_stream;
			color_stream << i_color;
			const Json::Value &jv_color_exp = (*it_json)[color_stream.str()];
			if (Json::Value::null != jv_color_exp) {
				int exp = jv_color_exp.asInt();
				mix_cfg_.color_exp_map.insert(std::make_pair(i_color, exp));
			}
		}

		break;
	}
}

void Config_Cache_Rune_Stone::refresh_combine(void) {
	Int_Double prop_value;
	Combine_Count_Cfg combine_count_cfg;
	Combine_Id_Cfg combine_id_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["rune_stone"]["combine_id"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		combine_id_cfg.reset();

		combine_id_cfg.combine_id = (*it_json)["combine_id"].asInt();
		ABORT_INT_ZERO(combine_id_cfg.combine_id);
		Json_Utility::assign_int_vec((*it_json)["item_ids"], combine_id_cfg.item_ids);

		for (int cnt = 2; cnt < 5; ++cnt) {
			char cnt_char[3];
			memset(cnt_char, 0x00, sizeof(cnt_char));
			snprintf(cnt_char, sizeof(cnt_char), "%d", cnt);
			if (Json::Value::null != (*it_json)[cnt_char]) {
				const Json::Value &json_cnt = (*it_json)[cnt_char];
				combine_count_cfg.reset();

				for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin();
						it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
					prop_value.reset();
					prop_value.id = *it_prop;
					std::ostringstream prop_stream;
					prop_stream << prop_value.id;
					if (Json::Value::null != json_cnt[prop_stream.str()]) {
						prop_value.value = json_cnt[prop_stream.str()].asDouble();
						if (prop_value.value > 0.0) {
							combine_count_cfg.prop_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
						}
					}
				}

				combine_id_cfg.count_cfg_map_.insert(std::make_pair(cnt, combine_count_cfg));
			}
		}

		set_map_second_value_by_key(combine_id_cfg.combine_id, combine_id_cfg_map_, combine_id_cfg);
	}
}

const Item_Weight_Id_Cfg* Config_Cache_Rune_Stone::find_item_weight_id(const int item_weight_id) const {
	return get_map_second_pointer_by_key(item_weight_id, item_weight_id_cfg_map_);
}

const Smelter_Idx_Cfg* Config_Cache_Rune_Stone::find_smelter_idx(const int smelter_idx) const {
	return get_map_second_pointer_by_key(smelter_idx, smelter_idx_cfg_map_);
}

const Exp_Lvl_Cfg* Config_Cache_Rune_Stone::find_exp_lvl(const int exp_lvl) const {
	return get_map_second_pointer_by_key(exp_lvl, exp_lvl_cfg_map_);
}

const Rune_Stone_Color_Cfg* Config_Cache_Rune_Stone::find_rune_stone_color(const int rune_stone_color) const {
	return get_map_second_pointer_by_key(rune_stone_color, color_cfg_map_);
}

const Rune_Stone_Mix_Cfg& Config_Cache_Rune_Stone::rune_stone_mix(void) const {
	return mix_cfg_;
}

const Exp_Lvl_Cfg_Map& Config_Cache_Rune_Stone::exp_lvl_cfg_map(void) const {
	return exp_lvl_cfg_map_;
}

const Combine_Id_Cfg* Config_Cache_Rune_Stone::find_combine(const int combine_id) const {
	Combine_Id_Cfg_Map::const_iterator it = combine_id_cfg_map_.find(combine_id);
	if (it != combine_id_cfg_map_.end()) {
		return &it->second;
	}
	return NULL;
}

const Combine_Id_Cfg_Map& Config_Cache_Rune_Stone::combine_id_cfg_map(void) {
	return combine_id_cfg_map_;
}
