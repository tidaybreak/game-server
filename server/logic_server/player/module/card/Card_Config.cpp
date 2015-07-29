/*
 * Card_Config.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: root
 */

#include "Card_Config.h"

Card_Label::Card_Label(int qlt, int tp, int lvl) {
	quality = qlt;
	type = tp;
	lv = lvl;
}

void Card_Label::reset() {
	quality = 0;
	type		= 0;
	lv 		= 0;
}

void Card_Lv_Data::reset() {
	card_label.reset();

	coin 			= 0;
	init_exp 	= 0;
	exp 			= 0;
	price		= 0;
	merge_rate = 0;
	merge_cost = 0;
}

void Item_Cost::reset() {

	id = 0;
	amount = 0;
	bind = 0;
}

void Card_Poten_Interval::reset() {
	id = 0;
	merge_poten_rate = 0;
	interval.reset();
	merge_potent_interval.reset();
	item_cost_vec.clear();
	copper_cost = 0;
}

void Card_Attr_Config::reset() {
	id = 0;
	attr_lv_map.clear();
}

void Card_Config::reset() {
	id				= 0;
	type			= 0;
	max_star		= 0;
	sub_attr_rate	= 0;
	init_lv			= 0;
	max_lv			= 0;
	weapon.reset();
	hat.reset();
	cloth.reset();
	shoe.reset();
	necklace.reset();
	ring.reset();
	poten_weight_map.clear();
	sub_attr_map.clear();
}
