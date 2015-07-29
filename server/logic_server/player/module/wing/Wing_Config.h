/*
 * Wing_Config.h
 *
 *  Created on: 2014年10月13日
 *      Author: root
 */

#ifndef WING_CONFIG_H_
#define WING_CONFIG_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"

struct Wing_Prop_Value {
	Wing_Prop_Value(void) { reset(); }
	void reset(void) {
		fix = 0.0;
		percent = 0.0;
	}

	double fix;
	double percent;
};
typedef boost::unordered_map<int, Wing_Prop_Value> Wing_Prop_Value_Map;

typedef boost::unordered_map<int, Int_Int_Map> Wing_Avatar_Map;

typedef boost::unordered_map<int, Int_Int_Vec> Wing_Ret_Reward_Map;

struct Wing_Config {

	Wing_Config(void) { reset(); }
	void reset(void) {
		lv = 0;
		show_stage = 0;
		show_lv = 0;
		gold_cost = 0;
		is_cast = 0;
		rate_base = 0;
		rate_add_value = 0;
		rate_show_min = 0;
		rate_show_max = 0;
		rate_min_max.reset();
		goods_cost.reset();
		skill_vec.clear();
		player_prop_map.clear();
		hero_prop_map.clear();
		wing_avatar_map.clear();
	}

	int lv;
	int show_stage;
	int show_lv;
	int gold_cost;
	int is_cast;
	int rate_base;
	int rate_add_value;
	int rate_show_min;
	int rate_show_max;
	Int_Int rate_min_max;
	Int_Int goods_cost;
	Int_Vec skill_vec;
	Wing_Prop_Value_Map player_prop_map;
	Wing_Prop_Value_Map hero_prop_map;
	Wing_Avatar_Map wing_avatar_map;
};
typedef boost::unordered_map<int, Wing_Config> Wing_Cfg_Map;

#endif /* WING_CONFIG_H_ */
