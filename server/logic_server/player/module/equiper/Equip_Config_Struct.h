/*
 * Equip_Config_Struct.h
 *
 *  Created on: May 12, 2014
 *      Author: chenyulin
 */

#ifndef EQUIP_CONFIG_STRUCT_H_
#define EQUIP_CONFIG_STRUCT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"
#include "pack/Pack_Struct.h"

// 解封等级
struct Equip_Unban_Lvl_Cfg {
	Equip_Unban_Lvl_Cfg(void) { reset(); }
	void reset(void);

	int unban_lvl;
	int success_rate;
	int cost_copper;
	int add_prop_rate;
	int add_sell_price;

	Int_Double_Map prop_val_map;
};
typedef boost::unordered_map<int, Equip_Unban_Lvl_Cfg> Equip_Unban_Lvl_Cfg_Map;

// 解封全身等级
struct Equip_Unban_Lvl_Whole_Cfg {
	Equip_Unban_Lvl_Whole_Cfg(void) { reset(); }
	void reset(void);

	int unban_lvl_whole;
	Int_Double_Map prop_val_map;
};
typedef boost::unordered_map<int, Equip_Unban_Lvl_Whole_Cfg> Equip_Unban_Lvl_Whole_Cfg_Map;

// 解封杂项
struct Equip_Unban_Misc_Cfg {
	Equip_Unban_Misc_Cfg(void) { reset(); }
	void reset(void);

	Int_Int_Map locky_stone_map;
};

struct Suit_Count_Cfg {
	Suit_Count_Cfg(void) { reset(); }
	inline void reset(void) {
		prop_val_map.clear();
		skills.clear();
	}

	Int_Double_Map prop_val_map;
	Int_Set skills;
};
typedef std::map<int, Suit_Count_Cfg> Suit_Count_Cfg_Map;	// 套装不能改成unorder_map，属性按套装数顺序加成

// 套装
struct Suit_ID_Cfg {
	Suit_ID_Cfg(void) { reset(); }
	inline void reset(void) {
		suit_id = 0;
		max_cnt_put_on = 0;
		name = "";
		count_cfg_map_.clear();
		item_ids.clear();
	}

	int suit_id;
	int max_cnt_put_on;
	std::string name;
	Int_Vec item_ids;
	Suit_Count_Cfg_Map count_cfg_map_;
};
typedef std::map<int, Suit_ID_Cfg> Suit_ID_Cfg_Map;
typedef std::vector<Suit_ID_Cfg> Suit_ID_Cfg_Vec;

// 附魔部位
struct Enchant_Part_Cfg {
	Enchant_Part_Cfg(void) { reset(); }
	inline void reset(void) {
		part = 0;
		props.clear();
		//weights.clear();
		prop_weight_map.clear();
		factor = 0.0;
		race_factor = 0.0;
	}

	int part;
	Int_Vec props;
	//Int_Vec weights;
	std::map<int, int> prop_weight_map;
	Int_Int_Map props_ratio;
	double factor;
	double race_factor;
};
typedef std::map<int, Enchant_Part_Cfg> Enchant_Part_Cfg_Map;

// 附魔颜色
struct Enchant_Color_Cfg {
	Enchant_Color_Cfg(void) { reset(); }
	inline void reset(void) {
		color = 0;
		stars.clear();
		weights.clear();
	}

	int color;
	Int_Vec stars;
	Int_Vec weights;
};
typedef std::map<int, Enchant_Color_Cfg> Enchant_Color_Cfg_Map;

// 附魔星数
struct Enchant_Star_Cfg {
	Enchant_Star_Cfg(void) { reset(); }
	inline void reset(void) {
		star = 0;
		base_prop_factor = 0.0;
		race_prop_factor = 0.0;
	}

	int star;
	double base_prop_factor;
	double race_prop_factor;
};
typedef std::map<int, Enchant_Star_Cfg> Enchant_Star_Cfg_Map;

// 附魔属性
struct Enchant_Prop_Cfg {
	Enchant_Prop_Cfg(void) { reset(); }
	inline void reset(void) {
		prop = 0;
		value = 0.0;
	}

	int prop;
	double value;
};
typedef std::map<int, Enchant_Prop_Cfg> Enchant_Prop_Cfg_Map;

// 附魔混合
struct Enchant_Mix_Cfg {
	Enchant_Mix_Cfg(void) { reset(); }
	inline void reset(void) {
		cost_copper = 0;
		material_id = 0;
		material_num = 0;
		lock_id = 0;
		sub_weight_once = 0;
		cntlock_cntfail_map.clear();
		ann_enchant_prop.clear();
		ann_enchant_star = 0;
	}

	int cost_copper;
	int material_id;
	int material_num;
	int lock_id;
	int sub_weight_once;
	std::map<int, int> cntlock_cntfail_map;
	Int_Set ann_enchant_prop;
	int ann_enchant_star;
};

struct Enchant_Weight_Ratio_Cfg {
	Enchant_Weight_Ratio_Cfg (void) { reset(); }
	void reset(void) {
		times_min = 0;
		times_max = 0;
		ratio = 0;
	}

	int times_min;
	int times_max;
	int ratio;
};
typedef std::vector<Enchant_Weight_Ratio_Cfg> Enchant_Weight_Ratio_Vec;

// 装备等级
struct Equip_Lvl_Cfg {
	Equip_Lvl_Cfg(void) { reset(); }
	inline void reset(void) {
		equip_lvl = 0;
		base_prop_factor = 0.0;
		race_prop_factor = 0.0;
	}

	int equip_lvl;
	double base_prop_factor;
	double race_prop_factor;
};
typedef std::map<int, Equip_Lvl_Cfg> Equip_Lvl_Cfg_Map;

// 宝石合成
struct Equip_Formula_Cfg {
	Equip_Formula_Cfg(void) { reset(); }
	inline void reset(void) {
		formula.clear();
		weight.clear();
	}

	Int_Vec formula;
	Int_Vec weight;
};
typedef std::map<int, Equip_Formula_Cfg> Equip_Formula_Cfg_Map;

// 装备精通
struct Equip_Master_Cost_Detail {// 装备精通:消耗
	Equip_Master_Cost_Detail(void) {
		reset();
	}
	inline void reset(void) {
		id = 0;
		amount = 0;
		copper = 0;
	}

	int32_t id; // 道具id
	int32_t amount; // 数量
	int32_t copper; // 金币数量
};
typedef std::map<uint32_t, Equip_Master_Cost_Detail> Equip_Master_Cost;
struct Equip_Master {
	Equip_Master(void){reset(); }
	inline void reset(void) {
		max_lvl_ = 0;
		lvl_success_rate_map_.clear();
		lvl_fail_add_rate_map_.clear();
		lvl_max_luck_value_map_.clear();
		lvl_fail_add_luck_value_map_.clear();
		lvl_cost_map_.clear();
		lvl_prop_map_.clear();
		lvl_whole_prop_map_.clear();
		lvl_role_lv_limit_map_.clear();
		need_broadcast_set_.clear();
	}

	uint32_t max_lvl_; // 精通等级上限
	UInt_UInt_Map lvl_success_rate_map_; // 等级-成功率
	UInt_UInt_Map lvl_fail_add_rate_map_; // 等级[失败提升的成功率]
	UInt_UInt_Map lvl_max_luck_value_map_; // 等级-幸运值上限
	UInt_UInt_Map lvl_fail_add_luck_value_map_; // 等级[失败提升的幸运值]
	Equip_Master_Cost lvl_cost_map_; // 等级-消耗
	UInt_UInt_Map lvl_prop_map_; // 等级-增加的属性
	UInt_UInt_Map lvl_whole_prop_map_; // 等级-增加的属性(全身精通达到X级（X可配）将获得装备共鸣)
	Int_Int_Map lvl_role_lv_limit_map_; //
	Int_Set need_broadcast_set_;
};

// 装备锻造
struct Equip_Compose_Id_Cost_Detail {// 装备锻造:消耗
	Equip_Compose_Id_Cost_Detail(void) {
		reset();
	}
	inline void reset(void) {
		item_vec.clear();
		copper = 0;
		level_up_id = 0;
		equip_id = 0;
		broadcast_id = 0;
		item_id_amount.reset();
	}
	Item_Info_Vec item_vec;// 道具id-数量
	int32_t copper; // 金币数量
	int32_t level_up_id;
	int32_t equip_id;
	int32_t broadcast_id;
	Item_Info item_id_amount;
	std::string name;
};
typedef std::map<int32_t, Equip_Compose_Id_Cost_Detail> Equip_Compose_Id_Cost;
struct Equip_Compose {
	Equip_Compose(void) {
		reset();
	}
	void reset(void) {
		id_cost_map_.clear();
		id_cost_equip_map_.clear();
	}

	Equip_Compose_Id_Cost id_cost_map_; // 要生成的物品-消耗
	Equip_Compose_Id_Cost id_cost_equip_map_; // 要生成的物品-消耗中的装备
};


// 装备分解可获得道具
struct Equip_Breakdown_Prop {
	Equip_Breakdown_Prop (void) {
		reset();
	}
	inline void reset(void) {
		equip_lv = 0;
		resolve_prop.clear();
		chance_1.clear();
		chance_2.clear();
		chance_3.clear();
		chance_4.clear();
		chance_5.clear();
	}

	int32_t equip_lv; // 装备等级
	Id_Amount_Vec resolve_prop; // 可获得道具
	Int_Vec chance_1; // 白色品质概率
	Int_Vec chance_2; // 绿色品质概率
	Int_Vec chance_3; // 蓝色品质概率
	Int_Vec chance_4; // 紫色品质概率
	Int_Vec chance_5; // 金色品质概率
};
typedef std::map<int, Equip_Breakdown_Prop > Equip_Breakdown_Prop_Map;

//宝石雕刻
struct Gem_Carve_Prop {
	Gem_Carve_Prop(void){reset(); }
	inline void reset(void) {
		carve_lvl = 0;
		gem_stats = 0.0;
		carve_max = 0;
		item_base.reset();
		broadcast_id = 0;
		max = 0;
	}

	uint32_t carve_lvl; // 宝石雕刻等级
	double gem_stats; // 宝石雕刻属性加成（百分比）
	uint32_t carve_max; // 宝石雕刻进度条上限
	Int_Int item_base; // 宝石精华基数
	uint32_t broadcast_id; //广播ID
	uint32_t max;  //宝石雕刻最大值


};
typedef std::map<int, Gem_Carve_Prop> Gem_Carve_Prop_Map;
#endif /* EQUIP_CONFIG_STRUCT_H_ */
