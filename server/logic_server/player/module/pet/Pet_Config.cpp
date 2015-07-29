/*
 * Pet_Config.cpp
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#include "Pet_Config.h"

void Pet_Config::reset() {

	lv = 0;
	ui_stage = 0;
	ui_lv = 0;
	gold_cost = 0;
	avatar = 0;
	grid_num = 0;
	broadcast = 0;
	item_cost.reset();
	upgrade_val.reset();
	skill_vec.clear();
	role_prop_map.clear();
	hero_prop_map.clear();
}

void Pet_Illusion_Config::reset() {

	id = 0;
	model = 0;
	active_cond = 0;
	attr_map.clear();
}

void Pet_Learn_Skill_Config::reset() {

	num = 0;
	rate = 0;
	money_cost = 0;
}

void Pet_Skill_Config::reset() {

	skill_id = 0;
	item_id = 0;
	category = 0;
	type = 0;
}
