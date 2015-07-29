/*
 * Config_Cache_Card.cpp
 *
 *  Created on: 2014年11月26日
 *      Author: root
 */

#include "Config_Cache_Card.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "equiper/Equiper_Def.h"

void Config_Cache_Card::refresh_card_config_cache() {
	PERF_MON("Config_Card_Config");
	refresh_base();
	refresh_poten();
	refresh_sub_attr();
	refresh_max_lv_map();
	refresh_star();
	refresh_type();
	refresh_attr();
	refresh_upgrade_data();
	refresh_vip_times();
	refresh_vip_upgrade_card_map();
	refresh_poten_upgrade_config_vec();
}

void Config_Cache_Card::refresh_base() {

	Int_Int_Map main_attr_map;
	const Json::Value &p_json = CONFIG_INSTANCE->config_json()["card"]["main_attr"];
	for (Json::Value::const_iterator it = p_json.begin(); it != p_json.end(); ++it) {
		int id = (*it)["part"].asInt();
		int value = (*it)["attr_id"].asInt();
		main_attr_map[id] = value;
	}

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["base"];
	Card_Config cfg;

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		cfg.reset();
		cfg.id = (*it)["id"].asInt();
		cfg.type = (*it)["type"].asInt();

		if ((*it)["poten"].empty()) LOG_ABORT("no poten data, id=%d", cfg.id);
		const Json::Value &poten_json = (*it)["poten"];
		for (Json::Value::const_iterator a_it = poten_json.begin();
				a_it != poten_json.end(); ++a_it) {
			cfg.poten_weight_map[(*a_it)[0u].asInt()] = (*a_it)[1].asInt();
		}

		cfg.weapon.val_1 = (*it)["weapon_weight"].asInt();
		cfg.weapon.val_2 = get_main_attr_id(EP_WEAPONS, main_attr_map);
		cfg.hat.val_1 = (*it)["hat_weight"].asInt();
		cfg.hat.val_2 = get_main_attr_id(EP_HAT, main_attr_map);
		cfg.cloth.val_1 = (*it)["cloth_weight"].asInt();
		cfg.cloth.val_2 = get_main_attr_id(EP_CLOTHING, main_attr_map);
		cfg.shoe.val_1 = (*it)["shoe_weight"].asInt();
		cfg.shoe.val_2 = get_main_attr_id(EP_SHOES, main_attr_map);
		cfg.necklace.val_1 = (*it)["necklace_weight"].asInt();
		cfg.necklace.val_2 = get_main_attr_id(EP_JEWELRY, main_attr_map);
		cfg.ring.val_1 = (*it)["ring_weight"].asInt();
		cfg.ring.val_2 = get_main_attr_id(EP_RING, main_attr_map);
		if (0 == (cfg.weapon.val_1 + cfg.hat.val_1 + cfg.cloth.val_1 + cfg.shoe.val_1 + cfg.necklace.val_1 + cfg.ring.val_1))
			LOG_ABORT("card cfg weight zero, id=%d", cfg.id);

		if ((*it)["sub_attr_set_rate"].empty()) LOG_ABORT("no sub_attr set, id=%d", cfg.id);
		const Json::Value &sub_json = (*it)["sub_attr_set_rate"];
		for (Json::Value::const_iterator a_it = sub_json.begin();
				a_it != sub_json.end(); ++a_it) {
			cfg.sub_attr_map[(*a_it)[0u].asInt()] = (*a_it)[1].asInt();
		}

		cfg.max_star = (*it)["star_max"].asInt();
		cfg.sub_attr_rate = (*it)["sub_attr_rate"].asInt();

		cfg.init_lv = (*it)["init_lv"].asInt();
		cfg.max_lv = (*it)["lv_max"].asInt();

		card_config_map_[cfg.id] = cfg;
	}
}

int Config_Cache_Card::get_main_attr_id(int type, Int_Int_Map &map) {

	Int_Int_Map::const_iterator find_it = map.find(type);
	if (find_it == map.end()) {
		LOG_ABORT("not attr map, type = %d", type);
		return 0;
	}
	return find_it->second;
}

void Config_Cache_Card::refresh_poten() {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["poten"];
	Card_Poten_Interval interval;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		interval.reset();
		interval.id = (*it)["quality"].asInt();
		interval.merge_poten_rate = (*it)["combine_rate"].asInt();

		if ((*it)["poten_interval"].empty())
			LOG_ABORT("no poten interval, id=%d", interval.id);
		if (!((*it)["poten_interval"].isArray() && (*it)["poten_interval"].size() == 2))
			LOG_ABORT("poten interval array, id=%d", interval.id);

		interval.interval.val_1 = (*it)["poten_interval"][0u].asInt();
		interval.interval.val_2 = (*it)["poten_interval"][1].asInt();

		if ((*it)["spirit_num"].empty())
			LOG_ABORT("no spirit_num, id=%d", interval.id);
		if (!((*it)["spirit_num"].isArray() && (*it)["spirit_num"].size() == 2))
			LOG_ABORT("spirit_num array, id=%d", interval.id);

		interval.merge_potent_interval.val_1 = (*it)["spirit_num"][0u].asInt();
		interval.merge_potent_interval.val_2 = (*it)["spirit_num"][1].asInt();

		if (interval.merge_potent_interval.val_1 >= interval.merge_potent_interval.val_2)
			LOG_ABORT("merge_potent_interval err, id=%d", interval.id);

		Item_Cost item;
		for (Json::Value::const_iterator item_it = (*it)["cost_item"].begin();
				item_it != (*it)["cost_item"].end(); ++item_it) {
			if (!(*item_it).isArray() || 3 != (*item_it).size()) continue;
			item.reset();
			item.id = (*item_it)[0u].asInt();
			item.amount = (*item_it)[1].asInt();
			item.bind = (*item_it)[2].asInt();
			interval.item_cost_vec.push_back(item);
		}
		interval.copper_cost = (*it)["cost"].asInt();

		card_poten_interval_map_[interval.id] = interval;
	}
}

void Config_Cache_Card::refresh_sub_attr() {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["sub_attr"];

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		int set_id = (*it)["set_id"].asInt();
		int attr_id = (*it)["attr_id"].asInt();
		int weight = (*it)["weight"].asInt();

		Card_Sub_Attr_Map::iterator find_it = card_sub_attr_map_.find(set_id);
		if (card_sub_attr_map_.end() == find_it) {
			Int_Int_Map map;
			map[attr_id] = weight;
			card_sub_attr_map_[set_id] = map;
		} else {
			(find_it->second)[attr_id] = weight;
		}
	}
}

void Config_Cache_Card::refresh_max_lv_map(void) {
	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["level"];

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		int role_lv = (*it)["role_level"].asInt();
		int max_lv = (*it)["max_level"].asInt();

		card_max_lv_map_[role_lv] = max_lv;
	}
}

void Config_Cache_Card::refresh_star() {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["star"];

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		star_map_[(*it)["star"].asInt()] = (*it)["percent"].asInt();
	}
}

void Config_Cache_Card::refresh_type(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["type"];

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		type_map_[(*it)["type"].asInt()] = (*it)["percent"].asInt();
	}
}

void Config_Cache_Card::refresh_attr(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["attr"];

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		int id = (*it)["attr_id"].asInt();
		Card_Lv_Map::iterator find_it = card_lv_map_.find(id);
		if (card_lv_map_.end() == find_it) {
			card_lv_map_[id] = Card_Attr_Lv_Map();
		}

		std::ostringstream str;
		for (int i = 1; i <= 20; ++i) {
			str.str("");
			str.clear();
			str << i;
			if ((*it)[str.str()].isNull()) LOG_ABORT("no lv attr, id =%d, lv = %d", id, i);
			card_lv_map_[id][i] = (*it)[str.str()].asDouble();
		}
	}
}

void Config_Cache_Card::refresh_upgrade_data(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["upgrade"];

	Card_Lv_Data data;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		data.reset();
		data.card_label.quality 	= (*it)["quality"].asInt();
		data.card_label.type 		= (*it)["type"].asInt();
		data.card_label.lv 			= (*it)["level"].asInt();
		data.coin 							= (*it)["coin"].asInt();
		data.init_exp					= (*it)["init_exp"].asInt();
		data.exp							= (*it)["exp"].asInt();
		data.price						= (*it)["price"].asInt();
		data.merge_rate				= (*it)["success_rate"].asInt();
		data.merge_cost 			= (*it)["cost"].asInt();

		card_lv_data_map_[data.card_label] = data;
	}
}

void Config_Cache_Card::refresh_vip_times(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["vip"];

	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		int vip = (*it)["level"].asInt();
		int times = (*it)["times"].asInt();
		vip_times_map_[vip] = times;
	}
}
void Config_Cache_Card::refresh_vip_upgrade_card_map(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["diamonds_upgrade"];

	Int_Int value;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {
		value.reset();
		int times = (*it)["times"].asInt();
		value.val_1 = (*it)["cost"].asInt();
		value.val_2 = (*it)["card_exp"].asInt();
		vip_upgrade_card_map_[times] = value;
	}
}

void Config_Cache_Card::refresh_poten_upgrade_config_vec(void) {

	const Json::Value &json = CONFIG_INSTANCE->config_json()["card"]["golden"];

	Poten_Upgrade_Config cfg;
	for (Json::Value::const_iterator it = json.begin(); it != json.end(); ++it) {

		cfg.reset();
		cfg.weight = (*it)["Weight"].asInt();
		cfg.min = (*it)["Section"][0u].asInt();
		cfg.max = (*it)["Section"][1].asInt();
		if (cfg.max < cfg.min) {
			LOG_ABORT("inteval error, min=%d, max =%d", cfg.min, cfg.max);
		}

		poten_upgrade_config_vec_.push_back(cfg);
	}
}

const Card_Config* Config_Cache_Card::get_card_config(int id) const {

	return get_map_second_pointer_by_key(id, card_config_map_);
}

const Card_Attr_Lv_Map* Config_Cache_Card::get_card_attr_lv_map(int attr_id) const {

	return get_map_second_pointer_by_key(attr_id, card_lv_map_);
}

const Card_Poten_Interval* Config_Cache_Card::get_card_poten_interval(int quality) const {

	return get_map_second_pointer_by_key(quality, card_poten_interval_map_);
}

const Int_Int_Map* Config_Cache_Card::get_sub_attr_random_map(int set_id) const {

	return get_map_second_pointer_by_key(set_id, card_sub_attr_map_);
}

const Card_Lv_Data* Config_Cache_Card::get_card_lv_data(int quality, int type, int lv) const {
	Card_Label label;
	label.quality = quality;
	label.type = type;
	label.lv = lv;
	return get_map_second_pointer_by_key(label, card_lv_data_map_);
}

const Poten_Upgrade_Config_Vec &Config_Cache_Card::get_poten_upgrade_config_vec(void) const {

	return poten_upgrade_config_vec_;
}

double Config_Cache_Card::get_attr_lv_value(int attr_id, int lv) const {

	const Card_Attr_Lv_Map *map = get_map_second_pointer_by_key(attr_id, card_lv_map_);
	if (0 == map) return 0.0;
	Card_Attr_Lv_Map::const_iterator find_it = map->find(lv);
	if (map->end() == find_it) return 0.0;
	return find_it->second;
}

int Config_Cache_Card::get_quality_id(int poten) const {

	for (Card_Poten_Interval_Map::const_iterator it = card_poten_interval_map_.begin();
			it != card_poten_interval_map_.end(); ++it) {
		if (poten >= it->second.interval.val_1 && poten <= it->second.interval.val_2) {
			return it->first;
		}
	}

	return 0;
}

int Config_Cache_Card::get_init_exp(int quality, int type, int lv) const {

	const Card_Lv_Data* data = get_card_lv_data(quality, type, lv);
	if (0 == data) return 0;
	return data->init_exp;
}

int Config_Cache_Card::get_star_percent(int star) {

	const int *ret = get_map_second_pointer_by_key(star, star_map_);
	if (0 == ret) return 0;
	return *ret;
}

int Config_Cache_Card::get_type_percent(int type) {

	const int *ret = get_map_second_pointer_by_key(type, type_map_);
	if (0 == ret) return 0;
	return *ret;
}

const Int_Int_Map& Config_Cache_Card::get_type_percent() {
	return type_map_;
}

int Config_Cache_Card::get_max_lv_by_role_lv(int role_lv) {

	const int *ret = get_map_second_pointer_by_key(role_lv, card_max_lv_map_);
	if (0 == ret) return 0;
	return *ret;
}

int Config_Cache_Card::get_vip_max_upgrade_times(int vip_lv) {

	const int*ret = get_map_second_pointer_by_key(vip_lv, vip_times_map_);
	if (0 == ret) return 0;
	return *ret;
}

const Int_Int* Config_Cache_Card::get_vip_upgrade_value(int times) {

	return get_map_second_pointer_by_key(times, vip_upgrade_card_map_);
}
