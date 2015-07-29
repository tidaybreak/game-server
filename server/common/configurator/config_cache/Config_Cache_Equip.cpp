/*
 * Config_Cache_Equip.cpp
 *
 *  Created on: May 13, 2014
 *      Author: chenyulin
 */

#include "Config_Cache_Equip.h"
#include "Config_Cache_Role.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Fighter_Detail.h"
#include "pack/Pack_Struct.h"
#include "Config_Cache_Item.h"
#include "pack/Pack_Func.h"

void Config_Cache_Equip::refresh_unban_lvl(void) {
	Int_Double prop_value;
	Equip_Unban_Lvl_Cfg unban_lvl_cfg;
	const Json::Value &json_unban_lvl = CONFIG_INSTANCE->config_json()["equip"]["equip_unban_lvl"];
	for (Json::Value::iterator it_unban_lvl = json_unban_lvl.begin();
			it_unban_lvl != json_unban_lvl.end(); ++it_unban_lvl) {
		unban_lvl_cfg.reset();

		int unban_lvl = (*it_unban_lvl)["unban_lvl"].asInt();
		ABORT_INT_ZERO(unban_lvl);
		unban_lvl_cfg.unban_lvl = unban_lvl;
		unban_lvl_cfg.success_rate = (*it_unban_lvl)["success_rate"].asInt();
		unban_lvl_cfg.cost_copper = (*it_unban_lvl)["cost_copper"].asInt();
		unban_lvl_cfg.add_prop_rate = (*it_unban_lvl)["add_prop_rate"].asInt();
		unban_lvl_cfg.add_sell_price = (*it_unban_lvl)["sell"].asInt();

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_unban_lvl)[prop_stream.str()]) {
				prop_value.value = (*it_unban_lvl)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					unban_lvl_cfg.prop_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		set_map_second_value_by_key(unban_lvl_cfg.unban_lvl, unban_lvl_cfg_map_, unban_lvl_cfg);
	}
}

void Config_Cache_Equip::refresh_unban_lvl_whole(void) {
	Int_Double prop_value;
	Equip_Unban_Lvl_Whole_Cfg unban_lvl_whole_cfg;
	const Json::Value &json_unban_whole = CONFIG_INSTANCE->config_json()["equip"]["equip_unban_lvl_whole"];
	for (Json::Value::iterator it_unban_whole = json_unban_whole.begin();
			it_unban_whole != json_unban_whole.end(); ++it_unban_whole) {
		unban_lvl_whole_cfg.reset();

		int unban_lvl_whole = (*it_unban_whole)["unban_lvl_whole"].asInt();
		ABORT_INT_ZERO(unban_lvl_whole);
		unban_lvl_whole_cfg.unban_lvl_whole = unban_lvl_whole;

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			prop_value.reset();
			prop_value.id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << prop_value.id;
			if (Json::Value::null != (*it_unban_whole)[prop_stream.str()]) {
				prop_value.value = (*it_unban_whole)[prop_stream.str()].asDouble();
				if (prop_value.value > 0.0) {
					unban_lvl_whole_cfg.prop_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
				}
			}
		}

		set_map_second_value_by_key(unban_lvl_whole_cfg.unban_lvl_whole, unban_lvl_whole_cfg_map_, unban_lvl_whole_cfg);
	}
}

void Config_Cache_Equip::refresh_unban_misc(void) {
	const Json::Value &json_unban_misc = CONFIG_INSTANCE->config_json()["equip"]["equip_unban_misc"];
	if (! json_unban_misc.empty()) {
		Json_Utility::assign_int_int_map((*json_unban_misc.begin())["lucky_stone"],
				unban_misc_cfg_.locky_stone_map);
	}
}

void Config_Cache_Equip::refresh_suit(void) {
	suit_equip_ids_.clear();
	suit_id_cfg_vec_.clear();
	Int_Double prop_value;
	//Int_Set skills;
	Suit_Count_Cfg suit_count_cfg;
	Suit_ID_Cfg suit_id_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["suit"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		suit_id_cfg.reset();

		suit_id_cfg.suit_id = (*it_json)["suit_id"].asInt();
		ABORT_INT_ZERO(suit_id_cfg.suit_id);

		suit_id_cfg.max_cnt_put_on = (*it_json)["max_cnt_put_on"].asInt();
		suit_id_cfg.name = (*it_json)["suit_name"].asString();

		Json_Utility::assign_int_vec((*it_json)["item_ids"], suit_id_cfg.item_ids);
		for (Int_Vec::const_iterator it_item_ids = suit_id_cfg.item_ids.begin(); it_item_ids != suit_id_cfg.item_ids.end(); ++it_item_ids) {
			suit_equip_ids_.insert(*it_item_ids);
		}

		for (int cnt = 2; cnt < 5; ++cnt) {
			char cnt_char[3];
			memset(cnt_char, 0x00, sizeof(cnt_char));
			snprintf(cnt_char, sizeof(cnt_char), "%d", cnt);
			if (Json::Value::null != (*it_json)[cnt_char]) {
				const Json::Value &json_cnt = (*it_json)[cnt_char];
				suit_count_cfg.reset();
				Json_Utility::assign_set_list(json_cnt["skills"], suit_count_cfg.skills);

				for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin();
						it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
					prop_value.reset();
					prop_value.id = *it_prop;
					std::ostringstream prop_stream;
					prop_stream << prop_value.id;
					if (Json::Value::null != json_cnt[prop_stream.str()]) {
						prop_value.value = json_cnt[prop_stream.str()].asDouble();
						if (prop_value.value > 0.0) {
							suit_count_cfg.prop_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
						}
					}
				}

				suit_id_cfg.count_cfg_map_.insert(std::make_pair(cnt, suit_count_cfg));
			}
		}

		set_map_second_value_by_key(suit_id_cfg.suit_id, suit_id_cfg_map_, suit_id_cfg);
		suit_id_cfg_vec_.push_back(suit_id_cfg);
	}
}

void Config_Cache_Equip::refresh_master(void) {
	this->increase_cfg_.reset();
	Equip_Master_Cost_Detail cost;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["master"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		int32_t master_lv = (*it_json)["master_lv"].asInt();
		--master_lv;
		if (this->increase_cfg_.max_lvl_ < static_cast<uint32_t>(master_lv)) {
			this->increase_cfg_.max_lvl_ = master_lv;
		}
		int32_t luck_max = (*it_json)["luck_max"].asInt();
		int32_t luck_once = (*it_json)["luck_once"].asInt();
		int32_t percentage = (*it_json)["percentage"].asInt();
		int32_t success_rate_base = (*it_json)["success_rate_base"].asInt();
		int32_t success_rate_once = (*it_json)["success_rate_once"].asInt();
		int32_t role_lv_limit = (*it_json)["role_lv"].asInt();
		set_map_second_value_by_key(master_lv, this->increase_cfg_.lvl_success_rate_map_, success_rate_base);
		set_map_second_value_by_key(master_lv, this->increase_cfg_.lvl_fail_add_rate_map_, success_rate_once);
		set_map_second_value_by_key(master_lv, this->increase_cfg_.lvl_max_luck_value_map_, luck_max);
		set_map_second_value_by_key(master_lv, this->increase_cfg_.lvl_fail_add_luck_value_map_, luck_once);
		set_map_second_value_by_key(master_lv, this->increase_cfg_.lvl_role_lv_limit_map_, role_lv_limit);
		cost.reset();
		cost.copper = (*it_json)["gold"].asInt();
		if ((*it_json)["material"] != Json::Value::null
				&& (*it_json)["material"].isArray()
				&& (*it_json)["material"].size() == 2) {
			cost.id = (*it_json)["material"][0u].asInt();
			cost.amount = (*it_json)["material"][1u].asInt();
		}
		set_map_second_value_by_key(master_lv, this->increase_cfg_.lvl_cost_map_, cost);
		set_map_second_value_by_key(master_lv+1, this->increase_cfg_.lvl_prop_map_, percentage);
		int need_broadcast = (*it_json)["broadcast"].asInt();
		if (need_broadcast) {
			this->increase_cfg_.need_broadcast_set_.insert(master_lv + 1);
		}
	}

	const Json::Value &json_award = CONFIG_INSTANCE->config_json()["equip"]["master_award"];
	for (Json::Value::iterator it_json = json_award.begin(); it_json != json_award.end(); ++it_json) {
		int32_t master_lv_stage = (*it_json)["master_lv_stage"].asInt();
		int32_t nature = (*it_json)["nature"].asInt();
		set_map_second_value_by_key(master_lv_stage, this->increase_cfg_.lvl_whole_prop_map_, nature);
	}
}

void Config_Cache_Equip::refresh_compose(void) {
	Equip_Compose_Id_Cost_Detail cost;
	Equip_Compose_Id_Cost_Detail cost_equip;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["equip_compose"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		cost.reset();
		cost_equip.reset();
		cost.level_up_id = (*it_json)["id"].asInt();
		cost_equip.level_up_id = (*it_json)["id"].asInt();
		cost.copper = (*it_json)["gold"].asInt();
		cost.item_id_amount.id = (*it_json)["price_id"].asInt();
		cost.item_id_amount.amount = (*it_json)["num_material"].asInt();
		cost.broadcast_id = (*it_json)["broadcast_id"].asInt();
		cost.name = (*it_json)["name"].asString();
		if (!(*it_json)["material"].isNull() && (*it_json)["material"].isArray()) {
			Json_Utility::assign_item_vec((*it_json)["material"], cost.item_vec);
		}

		for (Item_Info_Vec::const_iterator it = cost.item_vec.begin(); it != cost.item_vec.end(); ++it) {
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item((*it).id);
			if (! item_config) {
				LOG_ABORT("ERROR equip_id %d", (*it).id);
			}
			if (0 == is_item_type((*it).id, Item_Detail::EQUIP)) {
				if ((*it).amount != 1) {
					LOG_ABORT("ERROR %d, material need more than 1 amount!", (*it).id);
				}
				cost_equip.item_vec.push_back(*it);
			}
		}
		if (cost_equip.item_vec.size() > 1) {
			LOG_ABORT("error %d, material need more than 1 equip!", cost_equip.level_up_id);
		}
		for (Item_Info_Vec::const_iterator it_equip = cost_equip.item_vec.begin(); it_equip != cost_equip.item_vec.end(); ++it_equip) {
			for (Item_Info_Vec::iterator it = cost.item_vec.begin(); it != cost.item_vec.end(); ++it) {
				if ((*it_equip).id == it->id) {
					cost.item_vec.erase(it);
					break;
				}
			}
		}

		set_map_second_value_by_key(cost.level_up_id, level_up_cfg_.id_cost_map_, cost);
		if (!cost_equip.item_vec.empty()) {
			set_map_second_value_by_key(cost_equip.level_up_id, level_up_cfg_.id_cost_equip_map_, cost_equip);
		}
	}
}

void Config_Cache_Equip::refresh_enchant_part(void) {
	enchant_part_cfg_map_.clear();
	Enchant_Part_Cfg enchant_part_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["enchant_part"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		enchant_part_cfg.reset();

		enchant_part_cfg.part = (*it_json)["part"].asInt();
		ABORT_INT_ZERO(enchant_part_cfg.part);
		//Json_Utility::assign_int_vec((*it_json)["props"], enchant_part_cfg.props);
		//Json_Utility::assign_int_vec((*it_json)["weights"], enchant_part_cfg.weights);

		uint props_size = (*it_json)["props"].size();
		if (props_size == (*it_json)["weights"].size()) {
			for (uint i = 0; i < props_size; ++i) {
				int prop = (*it_json)["props"][i].asInt();
				if (! prop) {
					continue;
				}
				enchant_part_cfg.props.push_back(prop);

				int weight = (*it_json)["weights"][i].asInt();
				enchant_part_cfg.prop_weight_map.insert(std::make_pair(prop, weight));
			}
		}

		for (Json::Value::iterator it = (*it_json)["props_ratio"].begin();
				it != (*it_json)["props_ratio"].end(); ++it) {
			int prop_ratio_id = (*it).asInt();
			enchant_part_cfg.props_ratio[prop_ratio_id] = 0;
		}

		enchant_part_cfg.factor = (*it_json)["factor"].asDouble();
		enchant_part_cfg.race_factor = (*it_json)["race_factor"].asDouble();

		set_map_second_value_by_key(enchant_part_cfg.part, enchant_part_cfg_map_, enchant_part_cfg);
	}
}

void Config_Cache_Equip::refresh_enchant_color(void) {
	enchant_color_cfg_map_.clear();
	Enchant_Color_Cfg enchant_color_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["enchant_color"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		enchant_color_cfg.reset();

		enchant_color_cfg.color = (*it_json)["color"].asInt();
		ABORT_INT_ZERO(enchant_color_cfg.color);
		Json_Utility::assign_int_vec((*it_json)["stars"], enchant_color_cfg.stars);
		Json_Utility::assign_int_vec((*it_json)["weights"], enchant_color_cfg.weights);

		set_map_second_value_by_key(enchant_color_cfg.color, enchant_color_cfg_map_, enchant_color_cfg);
	}
}

void Config_Cache_Equip::refresh_enchant_star(void) {
	enchant_star_cfg_map_.clear();
	Enchant_Star_Cfg enchant_star_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["enchant_star"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		enchant_star_cfg.reset();

		enchant_star_cfg.star = (*it_json)["enchant_star"].asInt();
		ABORT_INT_ZERO(enchant_star_cfg.star);
		enchant_star_cfg.base_prop_factor = (*it_json)["base_prop_factor"].asDouble();
		enchant_star_cfg.race_prop_factor = (*it_json)["race_prop_factor"].asDouble();

		set_map_second_value_by_key(enchant_star_cfg.star, enchant_star_cfg_map_, enchant_star_cfg);
	}
}

void Config_Cache_Equip::refresh_enchant_prop(void) {
	enchant_prop_cfg_map_.clear();
	Enchant_Prop_Cfg enchant_prop_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["enchant_prop"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		enchant_prop_cfg.reset();

		enchant_prop_cfg.prop = (*it_json)["enchant_prop"].asInt();
		ABORT_INT_ZERO(enchant_prop_cfg.prop);
		enchant_prop_cfg.value = (*it_json)["value"].asDouble();

		set_map_second_value_by_key(enchant_prop_cfg.prop, enchant_prop_cfg_map_, enchant_prop_cfg);
	}
}

void Config_Cache_Equip::refresh_enchant_mix(void) {
	enchant_mix_cfg_.reset();
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["enchant_mix"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		enchant_mix_cfg_.cost_copper = (*it_json)["cost_copper"].asInt();
		enchant_mix_cfg_.material_id = (*it_json)["material_id"].asInt();
		enchant_mix_cfg_.material_num = (*it_json)["material_num"].asInt();
		enchant_mix_cfg_.lock_id = (*it_json)["lock_id"].asInt();
		enchant_mix_cfg_.sub_weight_once = (*it_json)["sub_weight_once"].asInt();
		Json_Utility::assign_set_list((*it_json)["ann_enchant_prop"], enchant_mix_cfg_.ann_enchant_prop);
		enchant_mix_cfg_.ann_enchant_star = (*it_json)["ann_enchant_star"].asInt();

		const Json::Value &json_compensat = (*it_json)["compensatory_cnt"];
		for (uint8_t i = 0; i < json_compensat.size(); ++i) {
			int cnt = json_compensat[i].asInt();
			if (cnt) {
				enchant_mix_cfg_.cntlock_cntfail_map.insert(std::make_pair(i + 1, cnt));
			}
		}

		break;
	}
}

void Config_Cache_Equip::refresh_enchant_weight_ratio(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["modifier_formula"];
	Enchant_Weight_Ratio_Cfg cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		if (!(*it)["enchant_num"].isArray() || 2 != (*it)["enchant_num"].size())
			LOG_ABORT("modifier_formula error, echant_num is not array or size not equip to 2");
		cfg.reset();
		cfg.times_min = (*it)["enchant_num"][0u].asInt();
		cfg.times_max = (*it)["enchant_num"][1].asInt();
		if (cfg.times_min >= cfg.times_max)
			LOG_ABORT("modifier_formula enchant_num error, min greater than max");
		cfg.ratio = (*it)["ratio"].asInt();
		enchant_weight_ratio_vec_.push_back(cfg);
	}
}

void Config_Cache_Equip::refresh_equip_lvl(void) {
	equip_lvl_cfg_map_.clear();
	Equip_Lvl_Cfg equip_lvl_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["equip_lvl"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		equip_lvl_cfg.reset();

		equip_lvl_cfg.equip_lvl = (*it_json)["equip_lvl"].asInt();
		ABORT_INT_ZERO(equip_lvl_cfg.equip_lvl);
		equip_lvl_cfg.base_prop_factor = (*it_json)["base_prop_factor"].asDouble();
		equip_lvl_cfg.race_prop_factor = (*it_json)["race_prop_factor"].asDouble();

		set_map_second_value_by_key(equip_lvl_cfg.equip_lvl, equip_lvl_cfg_map_, equip_lvl_cfg);
	}
}

void Config_Cache_Equip::refresh_equip_formula(void) {
	equip_formula_cfg_map_.clear();
	Equip_Formula_Cfg equip_formula_cfg;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["equip_formula"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		equip_formula_cfg.reset();

		Json_Utility::assign_int_vec((*it_json)["formula"], equip_formula_cfg.formula);
		Json_Utility::assign_int_vec((*it_json)["weight"], equip_formula_cfg.weight);
		if (equip_formula_cfg.formula.size() != equip_formula_cfg.weight.size()) {
			LOG_ABORT("id:%d", (*it_json)["item_id"].asInt());
		}
		set_map_second_value_by_key((*it_json)["item_id"].asInt(), equip_formula_cfg_map_, equip_formula_cfg);
	}
}

void Config_Cache_Equip::refresh_equip_breakdown_enchant(void) {
	equip_breakdown_enchant_map_.clear();
    const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["resolve_enchant"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		set_map_second_value_by_key((*it_json)["enchant_start"].asInt(), equip_breakdown_enchant_map_, (*it_json)["enchant_num"].asInt());
	}
}

void Config_Cache_Equip::refresh_equip_breakdown_gold(void) {
	equip_breakdown_gold_map_.clear();
    const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["resolve_unban"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		set_map_second_value_by_key((*it_json)["unban_lvl"].asInt(), equip_breakdown_gold_map_, (*it_json)["gold"].asInt());
	}
}

void Config_Cache_Equip::refresh_equip_breakdown_prop(void) {
	equip_breakdown_prop_map_.clear();
	Equip_Breakdown_Prop equip_breakdown_prop;
	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["resolve_gift"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		equip_breakdown_prop.reset();

		if ((*it_json)["equip_lv"].size() != 2) {
			LOG_ABORT("装备等级不存在");
		}
		int level_from = (*it_json)["equip_lv"][0u].asInt();
		int level_to = (*it_json)["equip_lv"][1u].asInt();
		if (!(*it_json)["resolve_prop"].isNull() && (*it_json)["resolve_prop"].isArray()) {
			Json_Utility::assign_item_vec((*it_json)["resolve_prop"], equip_breakdown_prop.resolve_prop);
		}
		Json_Utility::assign_int_vec((*it_json)["chance_1"], equip_breakdown_prop.chance_1);
		Json_Utility::assign_int_vec((*it_json)["chance_2"], equip_breakdown_prop.chance_2);
		Json_Utility::assign_int_vec((*it_json)["chance_3"], equip_breakdown_prop.chance_3);
		Json_Utility::assign_int_vec((*it_json)["chance_4"], equip_breakdown_prop.chance_4);
		Json_Utility::assign_int_vec((*it_json)["chance_5"], equip_breakdown_prop.chance_5);
		if (equip_breakdown_prop.resolve_prop.size() != equip_breakdown_prop.chance_1.size()) {
			LOG_ABORT("id:%d", (*it_json)["equip_lv"].asInt());
		}
		if (equip_breakdown_prop.resolve_prop.size() != equip_breakdown_prop.chance_2.size()) {
			LOG_ABORT("id:%d", (*it_json)["equip_lv"].asInt());
		}
		if (equip_breakdown_prop.resolve_prop.size() != equip_breakdown_prop.chance_3.size()) {
			LOG_ABORT("id:%d", (*it_json)["equip_lv"].asInt());
		}
		if (equip_breakdown_prop.resolve_prop.size() != equip_breakdown_prop.chance_4.size()) {
			LOG_ABORT("id:%d", (*it_json)["equip_lv"].asInt());
		}
		if (equip_breakdown_prop.resolve_prop.size() != equip_breakdown_prop.chance_5.size()) {
			LOG_ABORT("id:%d", (*it_json)["equip_lv"].asInt());
		}

		for (int i = level_from; i <= level_to; i++) {
			equip_breakdown_prop.equip_lv = i;
			set_map_second_value_by_key(i, equip_breakdown_prop_map_, equip_breakdown_prop);
		}
	}
}

void Config_Cache_Equip::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Equip");

	refresh_unban_lvl();
	refresh_unban_lvl_whole();
	refresh_unban_misc();
	refresh_suit();
	refresh_master();
	refresh_compose();
	refresh_enchant_part();
	refresh_enchant_color();
	refresh_enchant_star();
	refresh_enchant_prop();
	refresh_enchant_mix();
	refresh_enchant_weight_ratio();
	refresh_equip_lvl();
	refresh_equip_formula();
	refresh_equip_breakdown_gold();
	refresh_equip_breakdown_enchant();
	refresh_equip_breakdown_prop();
}

const Equip_Unban_Lvl_Cfg *Config_Cache_Equip::find_unban_lvl_cfg(const int unban_lvl) const {
	return get_map_second_pointer_by_key(unban_lvl, unban_lvl_cfg_map_);
}

const Equip_Unban_Lvl_Whole_Cfg *Config_Cache_Equip::find_unban_lvl_whole_cfg(const int unban_lvl) const {
	Equip_Unban_Lvl_Whole_Cfg_Map::const_iterator it = unban_lvl_whole_cfg_map_.find(unban_lvl);
	if (it != unban_lvl_whole_cfg_map_.end()) {
		return &it->second;
	}
	return NULL;
}

const Equip_Unban_Lvl_Whole_Cfg_Map &Config_Cache_Equip::get_unban_lvl_whole_cfg_map(void) const {
	return unban_lvl_whole_cfg_map_;
}

const Equip_Unban_Misc_Cfg *Config_Cache_Equip::unban_misc(void) const {
	return &unban_misc_cfg_;
}

const Suit_ID_Cfg *Config_Cache_Equip::find_suit(const int suit_id) const {
	Suit_ID_Cfg_Map::const_iterator it = suit_id_cfg_map_.find(suit_id);
	if (it != suit_id_cfg_map_.end()) {
		return &it->second;
	}
	return NULL;
}

const Suit_ID_Cfg_Map &Config_Cache_Equip::suit_id_cfg_map(void) {
	return this->suit_id_cfg_map_;
}

const Suit_ID_Cfg_Vec &Config_Cache_Equip::suit_id_cfg_vec(void) {
	return this->suit_id_cfg_vec_;
}

bool Config_Cache_Equip::is_suit_part(const int id) const {
	return suit_equip_ids_.count(id) > 0;
}

const Enchant_Part_Cfg *Config_Cache_Equip::find_enchant_part(const int enchant_part) const {
	return get_map_second_pointer_by_key(enchant_part, enchant_part_cfg_map_);
}

const Enchant_Color_Cfg *Config_Cache_Equip::find_enchant_color(const int enchant_color) const {
	return get_map_second_pointer_by_key(enchant_color, enchant_color_cfg_map_);
}

const Enchant_Star_Cfg *Config_Cache_Equip::find_enchant_star(const int enchant_star) const {
	return get_map_second_pointer_by_key(enchant_star, enchant_star_cfg_map_);
}

const Enchant_Prop_Cfg *Config_Cache_Equip::find_enchant_prop(const int enchant_prop) const {
	return get_map_second_pointer_by_key(enchant_prop, enchant_prop_cfg_map_);
}

const Enchant_Mix_Cfg& Config_Cache_Equip::enchant_mix(void) const {
	return enchant_mix_cfg_;
}

const Equip_Lvl_Cfg *Config_Cache_Equip::find_equip_lvl(const int equip_lvl) const {
	return get_map_second_pointer_by_key(equip_lvl, equip_lvl_cfg_map_);
}

const Equip_Formula_Cfg *Config_Cache_Equip::find_equip_formula(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, equip_formula_cfg_map_);
}

const uint32_t Config_Cache_Equip::find_equip_breakdown_enchant(const int enchant_start) const {
	return get_value_by_key(enchant_start, equip_breakdown_enchant_map_);
}

const uint32_t Config_Cache_Equip::find_equip_breakdown_gold(const int unban_lvl) const {
	return get_value_by_key(unban_lvl, equip_breakdown_gold_map_);
}

const Equip_Breakdown_Prop *Config_Cache_Equip::find_equip_breakdown_prop(const int equip_lvl) const {
	return get_map_second_pointer_by_key(equip_lvl, equip_breakdown_prop_map_);
}

const uint32_t &Config_Cache_Equip::incr_max_lvl(void) const {
	return this->increase_cfg_.max_lvl_;
}
const UInt_UInt_Map &Config_Cache_Equip::incr_lvl_success_rate_map(void) const {
	return this->increase_cfg_.lvl_success_rate_map_;
}
const UInt_UInt_Map &Config_Cache_Equip::incr_lvl_fail_add_rate_map(void) const {
	return this->increase_cfg_.lvl_fail_add_rate_map_;
}
const UInt_UInt_Map &Config_Cache_Equip::incr_lvl_max_luck_value_map(void) const {
	return this->increase_cfg_.lvl_max_luck_value_map_;
}
const UInt_UInt_Map &Config_Cache_Equip::incr_lvl_fail_add_luck_value_map(void) const {
	return this->increase_cfg_.lvl_fail_add_luck_value_map_;
}
const Equip_Master_Cost &Config_Cache_Equip::incr_lvl_cost_map(void) const {
	return this->increase_cfg_.lvl_cost_map_;
}
const UInt_UInt_Map &Config_Cache_Equip::incr_lvl_prop_map(void) const {
	return this->increase_cfg_.lvl_prop_map_;
}
const UInt_UInt_Map &Config_Cache_Equip::incr_lvl_whole_prop_map(void) const {
	return this->increase_cfg_.lvl_whole_prop_map_;
}
const Equip_Compose_Id_Cost &Config_Cache_Equip::level_up_id_cost_map(void) const {
	return this->level_up_cfg_.id_cost_map_;
}
const Equip_Compose_Id_Cost &Config_Cache_Equip::level_up_id_cost_equip_map(void) const {
	return this->level_up_cfg_.id_cost_equip_map_;
}

const Int_Set &Config_Cache_Equip::need_broadcast_set(void) const {
	return this->increase_cfg_.need_broadcast_set_;
}

const Enchant_Weight_Ratio_Vec& Config_Cache_Equip::enchant_weight_ratio_vec(void) const {
	return this->enchant_weight_ratio_vec_;
}

int Config_Cache_Equip::get_role_lv_limit(int lv) const {
	const int *p_lv_limit = get_map_second_pointer_by_key(lv, this->increase_cfg_.lvl_role_lv_limit_map_);
	if (0 == p_lv_limit) return 0;
	return *p_lv_limit;
}
