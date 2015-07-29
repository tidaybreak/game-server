/*
 * Rune_Stone_Cfg_Struct.h
 *
 *  Created on: Aug 5, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_STONE_CFG_STRUCT_H_
#define RUNE_STONE_CFG_STRUCT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"

// 熔炉产出道具权重配置
struct Item_Weight_Id_Cfg {
	Item_Weight_Id_Cfg(void) { reset(); }
	inline void reset(void) {
		item_weight_id = 0;
		item_id = 0;
		weight = 0;
	}

	int item_weight_id;
	int item_id;
	int weight;
};
typedef std::map<int, Item_Weight_Id_Cfg> Item_Weight_Id_Cfg_Map;

// 符石经验等级配置
struct Exp_Lvl_Cfg {
	Exp_Lvl_Cfg(void) { reset(); }
	inline void reset(void) {
		exp_lvl = 0;
		prop_lvl_factor = 0.0;
		color_exp_map.clear();
	}

	int exp_lvl;
	double prop_lvl_factor;
	std::map<int, int> color_exp_map;
};
typedef std::map<int, Exp_Lvl_Cfg> Exp_Lvl_Cfg_Map;

// 符石颜色配置
struct Rune_Stone_Color_Cfg {
	Rune_Stone_Color_Cfg(void) { reset(); }
	inline void reset(void) {
		rune_stone_color = 0;
		prop_lvl_factor = 0.0;
	}

	int rune_stone_color;
	double prop_lvl_factor;
};
typedef std::map<int, Rune_Stone_Color_Cfg> Rune_Stone_Color_Cfg_Map;

// 熔炉index配置
struct Smelter_Idx_Cfg {
	Smelter_Idx_Cfg(void) { reset(); }
	inline void reset(void) {
		smelter_idx = 0;
		cost_copper = 0;
		point = 0;
		front_rate = 0;
		item_weight_ids.clear();
		item_ids.clear();
		item_weights.clear();
	}

	int smelter_idx;
	int cost_copper;
	int point;
	int front_rate;
	//int back_rate;
	Int_Vec item_weight_ids;
	Int_Vec item_ids;
	Int_Vec item_weights;
};
typedef std::map<int, Smelter_Idx_Cfg> Smelter_Idx_Cfg_Map;

// 符石混合配置
struct Rune_Stone_Mix_Cfg {
	Rune_Stone_Mix_Cfg(void) { reset(); }
	inline void reset(void) {
		daily_smelt_d_times = 0;
		exchangeable_items.clear();
		smelt_output_copper_items.clear();
		smelt_bufs.clear();
		color_exp_map.clear();
	}

	int daily_smelt_d_times;
	Int_Set exchangeable_items;
	Int_Set smelt_output_copper_items;
	Int_Set smelt_bufs;
	std::map<int, int> color_exp_map;		// provide to other item
};

struct Combine_Count_Cfg {
	Combine_Count_Cfg(void) { reset(); }
	inline void reset(void) {
		prop_val_map.clear();
		//skills.clear();
	}

	Int_Double_Map prop_val_map;
	//Int_Set skills;
};
typedef std::map<int, Combine_Count_Cfg> Combine_Count_Cfg_Map;

// 组合
struct Combine_Id_Cfg {
	Combine_Id_Cfg(void) { reset(); }
	inline void reset(void) {
		combine_id = 0;
		count_cfg_map_.clear();
		item_ids.clear();
	}

	int combine_id;
	Int_Vec item_ids;
	Combine_Count_Cfg_Map count_cfg_map_;
};
typedef std::map<int, Combine_Id_Cfg> Combine_Id_Cfg_Map;

#endif /* RUNE_STONE_CFG_STRUCT_H_ */
