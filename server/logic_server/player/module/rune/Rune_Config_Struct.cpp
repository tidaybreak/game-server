/*
 * Rune_Config_Struct.cpp
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#include "Rune_Config_Struct.h"

void Rune_Hole_Lvl_Cfg::reset(void) {
	hole_lvl = 0;
	energy = 0;
	cost_copper = 0;
	material_id = 0;
	material_num_once = 0;
	prop_val = 0;
	prop_val_rate = 0;
	open_level = 0;
}

void Rune_Part_Cfg::reset(void) {
	rune_part = 0;
	prop_type = 0;
}
