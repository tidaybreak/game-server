/*
 * Equip_Config_Struct.cpp
 *
 *  Created on: May 12, 2014
 *      Author: chenyulin
 */

#include "Equip_Config_Struct.h"

void Equip_Unban_Lvl_Cfg::reset(void) {
	unban_lvl = 0;
	success_rate = 0;
	cost_copper = 0;
	add_prop_rate = 0;
	add_sell_price = 0;
	prop_val_map.clear();
}

void Equip_Unban_Lvl_Whole_Cfg::reset(void) {
	unban_lvl_whole = 0;
	prop_val_map.clear();
}

void Equip_Unban_Misc_Cfg::reset(void) {
	locky_stone_map.clear();
}

