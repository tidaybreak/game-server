/*
 * Card_Func.cpp
 *
 *  Created on: 2014年11月26日
 *      Author: root
 */

#include "Card_Func.h"
#include "Card_Config.h"
#include "Config_Cache_Card.h"
#include "equiper/Equiper_Def.h"
#include "Msg_Card_Struct.h"
#include "pack/Pack_Struct.h"

int handle_rand_card_part(const Card_Config& cfg, Int_Int &result) {

	int ttl = cfg.weapon.val_1;
	ttl += cfg.hat.val_1;
	ttl += cfg.cloth.val_1;
	ttl += cfg.shoe.val_1;
	ttl += cfg.necklace.val_1;
	ttl += cfg.ring.val_1;

	int rand = random() % ttl;

	if (rand < cfg.weapon.val_1) {
		result.val_1 = EP_WEAPONS;
		result.val_2 = cfg.weapon.val_2;
		return 0;
	}
	rand -= cfg.weapon.val_1;

	if (rand < cfg.hat.val_1) {
		result.val_1 = EP_HAT;
		result.val_2 = cfg.hat.val_2;
		return 0;
	}
	rand -= cfg.hat.val_1;

	if (rand < cfg.cloth.val_1) {
		result.val_1 = EP_CLOTHING;
		result.val_2 = cfg.cloth.val_2;
		return 0;
	}
	rand -= cfg.cloth.val_1;

	if (rand < cfg.shoe.val_1) {
		result.val_1 = EP_SHOES;
		result.val_2 = cfg.shoe.val_2;
		return 0;
	}
	rand -= cfg.shoe.val_1;

	if (rand < cfg.necklace.val_1) {
		result.val_1 = EP_JEWELRY;
		result.val_2 = cfg.necklace.val_2;
		return 0;
	}
	rand -= cfg.necklace.val_1;

	if (rand < cfg.ring.val_1) {
		result.val_1 = EP_RING;
		result.val_2 = cfg.ring.val_2;
		return 0;
	}

	return -1;
}

int get_quality(const Card_Config &cfg) {

	if (cfg.poten_weight_map.empty()) return 1;

	int ttl = 0;
	for (Int_Int_Map::const_iterator it = cfg.poten_weight_map.begin();
			it != cfg.poten_weight_map.end(); ++it) {
		ttl += it->second;
	}

	int rand = random() % ttl;
	for (Int_Int_Map::const_iterator it = cfg.poten_weight_map.begin();
			it != cfg.poten_weight_map.end(); ++it) {
		if (rand < it->second) return it->first;
		rand -= it->second;
	}

	return 1;
}

int handle_get_potential(const Card_Config &cfg) {

	int quality = get_quality(cfg);
	const Card_Poten_Interval* interval = CONFIG_CACHE_CARD->get_card_poten_interval(quality);
	if (0 == interval) return 0;

	int delta = interval->interval.val_2 - interval->interval.val_1 + 1;
	if (delta <= 0) return interval->interval.val_1;

	int rand = random() % delta;
	return interval->interval.val_1 + rand;
}

int handle_get_sub_attr(const Card_Config &cfg) {

	int rand_1 = random() % 1000;
	if (rand_1 >= cfg.sub_attr_rate) return 0;

	int ttl = 0;
	for (Int_Int_Map::const_iterator it = cfg.sub_attr_map.begin();
			it != cfg.sub_attr_map.end(); ++it) {
		ttl += it->second;
	}
	if (0 == ttl) return 0;

	int rand_2 = random() % ttl;
	int set_id = 0;
	for (Int_Int_Map::const_iterator it = cfg.sub_attr_map.begin();
			it != cfg.sub_attr_map.end(); ++it) {
		if (rand_2 < it->second) {
			set_id = it->first;
			break;
		}
		rand_2 -= it->second;
	}
	if (0 == set_id) return 0;

	const Int_Int_Map* sub_random_map = CONFIG_CACHE_CARD->get_sub_attr_random_map(set_id);
	if (0 == sub_random_map) return 0;

	ttl = 0;
	for (Int_Int_Map::const_iterator it = sub_random_map->begin();
			it != sub_random_map->end(); ++it) {
		ttl += it->second;
	}
	if(0 == ttl) return 0;

	int rand_3 = random() % ttl;
	for (Int_Int_Map::const_iterator it = sub_random_map->begin();
			it != sub_random_map->end(); ++it) {
		if (rand_3 < it->second) return it->first;
		rand_3 -= it->second;
	}

	return 0;
}

double handle_get_attr_value(int id, int attr_id, int star, int lv, int poten) {

	const Card_Config *cfg = CONFIG_CACHE_CARD->get_card_config(id);
	if (0 == cfg) return 0.0;
	const Card_Attr_Lv_Map *map = CONFIG_CACHE_CARD->get_card_attr_lv_map(attr_id);
	if (0 == map) return 0.0;
	Card_Attr_Lv_Map::const_iterator find_it = map->find(lv);
	if (map->end() == find_it) return 0.0;

	int star_per = CONFIG_CACHE_CARD->get_star_percent(star);
	int type_per = CONFIG_CACHE_CARD->get_type_percent(cfg->type);

	return find_it->second * poten / 1000 * (1 + star_per / 100.0) * (1 + type_per / 100.0);
}

int handle_get_ttl_exp(int cur_lv, int qlt, int type) {

	if (cur_lv <= 1) return 0;

	int his_exp = 0;
	for (int lv=1; lv<cur_lv; ++lv) {
		const Card_Lv_Data* lv_data = CONFIG_CACHE_CARD->get_card_lv_data(qlt, type, lv);
		if (0 == lv_data) continue;
		his_exp += lv_data->exp;
	}

	return his_exp;
}

bool is_card_max_type(int type) {

	const Int_Int_Map type_percent_map = CONFIG_CACHE_CARD->get_type_percent();
	for (Int_Int_Map::const_iterator it = type_percent_map.begin();
			it != type_percent_map.end(); ++it) {
		if (it->first > type) return false;
	}
	return true;
}

int get_next_id_dif_by_type(int type) {

	const Int_Int_Map type_percent_map = CONFIG_CACHE_CARD->get_type_percent();
	int ret_val = 0;
	for (Int_Int_Map::const_iterator it = type_percent_map.begin();
			it != type_percent_map.end(); ++it) {
		const int &temp_type = it->first;
		if (temp_type <= type) continue;
		if (ret_val > type && ret_val < temp_type) continue;
		ret_val = temp_type;
	}
	return (ret_val - type) * 100000;
}
