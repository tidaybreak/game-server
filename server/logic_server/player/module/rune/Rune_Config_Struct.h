/*
 * Rune_Config_Struct.h
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_CONFIG_STRUCT_H_
#define RUNE_CONFIG_STRUCT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"

// 符文孔等级配置
struct Rune_Hole_Lvl_Cfg {
	Rune_Hole_Lvl_Cfg(void) { reset(); }
	void reset(void);

	int hole_lvl;
	int energy;
	int cost_copper;
	int material_id;
	int material_num_once;
	int prop_val;
	int prop_val_rate;
	int open_level;
};
typedef boost::unordered_map<int, Rune_Hole_Lvl_Cfg> Rune_Hole_Lvl_Cfg_Map;

// 符文部位配置
struct Rune_Part_Cfg {
	Rune_Part_Cfg(void) { reset(); }
	void reset(void);

	int rune_part;
	int prop_type;
};
typedef boost::unordered_map<int, Rune_Part_Cfg> Rune_Part_Cfg_Map;

// 全身符文配置
struct Hole_Lvl_Whole_Cfg {
	Hole_Lvl_Whole_Cfg(void) { reset(); };
	void reset(void) {
		hole_lvl_whole = 0;
		prop_val_map.clear();
	}

	int hole_lvl_whole;
	Int_Double_Map prop_val_map;
};
typedef std::map<int, Hole_Lvl_Whole_Cfg> Hole_Lvl_Whole_Cfg_Map;	// need order map

#endif /* RUNE_CONFIG_STRUCT_H_ */
