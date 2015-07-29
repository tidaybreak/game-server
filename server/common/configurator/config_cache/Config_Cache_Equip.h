/*
 * Config_Cache_Equip.h
 *
 *  Created on: May 13, 2014
 *      Author: chenyulin
 */

#ifndef CONFIG_CACHE_EQUIP_H_
#define CONFIG_CACHE_EQUIP_H_

#include "Cache_Base.h"
#include "equiper/Equip_Config_Struct.h"

class Config_Cache_Equip : public Cache_Base {
public:
	const Equip_Unban_Lvl_Cfg *find_unban_lvl_cfg(const int unban_lvl) const;
	const Equip_Unban_Lvl_Whole_Cfg *find_unban_lvl_whole_cfg(const int unban_lvl) const;
	const Equip_Unban_Lvl_Whole_Cfg_Map &get_unban_lvl_whole_cfg_map(void) const;
	const Equip_Unban_Misc_Cfg *unban_misc(void) const;
	const Suit_ID_Cfg *find_suit(const int suit_id) const;
	const Suit_ID_Cfg_Map &suit_id_cfg_map(void);
	const Suit_ID_Cfg_Vec &suit_id_cfg_vec(void);
	bool is_suit_part(const int id) const;
	const Enchant_Part_Cfg *find_enchant_part(const int enchant_part) const;
	const Enchant_Color_Cfg *find_enchant_color(const int enchant_color) const;
	const Enchant_Star_Cfg *find_enchant_star(const int enchant_star) const;
	const Enchant_Prop_Cfg *find_enchant_prop(const int enchant_prop) const;
	const Enchant_Mix_Cfg& enchant_mix(void) const;
	const Equip_Lvl_Cfg *find_equip_lvl(const int equip_lvl) const;
	const Equip_Formula_Cfg *find_equip_formula(const int item_id) const;
	const uint32_t find_equip_breakdown_enchant(const int enchant_start) const;
	const uint32_t find_equip_breakdown_gold(const int unban_lvl) const;
	const Equip_Breakdown_Prop *find_equip_breakdown_prop(const int equip_lvl) const;

	const uint32_t &incr_max_lvl(void) const;
	const UInt_UInt_Map &incr_lvl_success_rate_map(void) const; // 等级-成功率
	const UInt_UInt_Map &incr_lvl_fail_add_rate_map(void) const; // 等级[次数，提升的成功率]
	const UInt_UInt_Map &incr_lvl_max_luck_value_map(void) const; // 等级-幸运值上限
	const UInt_UInt_Map &incr_lvl_fail_add_luck_value_map(void) const;
	const Equip_Master_Cost &incr_lvl_cost_map(void) const;
	const UInt_UInt_Map &incr_lvl_prop_map(void) const;
	const UInt_UInt_Map &incr_lvl_whole_prop_map(void) const;
	const Equip_Compose_Id_Cost &level_up_id_cost_map(void) const;
	const Equip_Compose_Id_Cost &level_up_id_cost_equip_map(void) const;
	const Int_Set &need_broadcast_set(void) const;
	const Enchant_Weight_Ratio_Vec& enchant_weight_ratio_vec(void) const;
	int get_role_lv_limit(int lv) const;

	void refresh_config_cache(void);

private:
	void refresh_unban_lvl(void);
	void refresh_unban_lvl_whole(void);
	void refresh_unban_misc(void);
	void refresh_suit(void);
	void refresh_master(void);
	void refresh_compose(void);
	void refresh_enchant_part(void);
	void refresh_enchant_color(void);
	void refresh_enchant_star(void);
	void refresh_enchant_prop(void);
	void refresh_enchant_mix(void);
	void refresh_enchant_weight_ratio(void);
	void refresh_equip_lvl(void);
	void refresh_equip_formula(void);
	void refresh_equip_breakdown_enchant(void);
	void refresh_equip_breakdown_gold(void);
	void refresh_equip_breakdown_prop(void);

private:
	Equip_Unban_Lvl_Cfg_Map unban_lvl_cfg_map_;
	Equip_Unban_Lvl_Whole_Cfg_Map unban_lvl_whole_cfg_map_;
	Equip_Unban_Misc_Cfg unban_misc_cfg_;
	Suit_ID_Cfg_Map suit_id_cfg_map_;
	Suit_ID_Cfg_Vec suit_id_cfg_vec_;
	Int_Hash_Set suit_equip_ids_;
	Equip_Master increase_cfg_;
	Equip_Compose level_up_cfg_;
	Enchant_Part_Cfg_Map enchant_part_cfg_map_;
	Enchant_Color_Cfg_Map enchant_color_cfg_map_;
	Enchant_Star_Cfg_Map enchant_star_cfg_map_;
	Enchant_Prop_Cfg_Map enchant_prop_cfg_map_;
	Enchant_Mix_Cfg enchant_mix_cfg_;
	Enchant_Weight_Ratio_Vec enchant_weight_ratio_vec_;
	Equip_Lvl_Cfg_Map equip_lvl_cfg_map_;
	Equip_Formula_Cfg_Map equip_formula_cfg_map_;
	Int_Int_Map equip_breakdown_enchant_map_; 	// 装备分解所得附魔券数
	Int_Int_Map equip_breakdown_gold_map_;		// 装备分解所得金币
	Equip_Breakdown_Prop_Map equip_breakdown_prop_map_;  // 装备分解可获得道具

};

typedef Singleton<Config_Cache_Equip> Config_Cache_Equip_Single;
#define CONFIG_CACHE_EQUIP    (Config_Cache_Equip_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_EQUIP_H_ */
