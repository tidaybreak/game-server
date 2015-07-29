/*
 * Pet_Config.h
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#ifndef PET_CONFIG_H_
#define PET_CONFIG_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"

struct Pet_Prop_Value {

	Pet_Prop_Value(void) { reset(); }
	void reset() {
		fix = 0.0;
		percent = 0.0;
	}

	double fix;
	double percent;
};
typedef boost::unordered_map<int , Pet_Prop_Value> Pet_Prop_Value_Map;

struct Pet_Config {

	Pet_Config(void) { reset(); }
	void reset();

	int lv;
	int ui_stage;
	int ui_lv;
	int gold_cost;
	int avatar;
	int grid_num;
	int broadcast;
	Int_Int item_cost;
	Int_Int upgrade_val;
	Int_Vec skill_vec;
	Pet_Prop_Value_Map role_prop_map;
	Pet_Prop_Value_Map hero_prop_map;
};
typedef boost::unordered_map<int, Pet_Config> Pet_Config_Map;

struct Pet_Illusion_Config {

	Pet_Illusion_Config(void) { reset(); }
	void reset();

	int id;
	int model;
	int active_cond;
	Int_Int_Map attr_map;
};
typedef boost::unordered_map<int, Pet_Illusion_Config> Pet_Illusion_Config_Map;

struct Pet_Learn_Skill_Config {

	Pet_Learn_Skill_Config(void) {  reset(); }
	void reset();

	int num;
	int rate;
	int money_cost;
};
typedef boost::unordered_map<int, Pet_Learn_Skill_Config> Pet_Learn_Skill_Config_Map;

struct Pet_Skill_Config {

	Pet_Skill_Config(void) { reset(); }
	void reset();

	int skill_id;
	int item_id;
	int category;
	int type;
};
typedef boost::unordered_map<int, Pet_Skill_Config> Pet_Skill_Config_Map;

#endif /* PET_CONFIG_H_ */
