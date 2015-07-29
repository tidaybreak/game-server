/*
 * Equiper.h
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#ifndef EQUIPER_H_
#define EQUIPER_H_

#include "Logic_Player_Base.h"

class Packer;
struct Equiper_Detail;
struct Item_Detail;
struct Item_Detail_Config;
struct Master_Part_Info_Detail;
struct Enchant_Mix_Cfg;
struct Enchant_Part_Cfg;
struct Enchant_Color_Cfg;
struct Equip_Lvl_Cfg;
struct Enchant_Weight_Ratio_Cfg;
 typedef std::vector<Enchant_Weight_Ratio_Cfg> Enchant_Weight_Ratio_Vec;

class Equiper: virtual public Packer {
public:
	Equiper(void);
	virtual ~Equiper();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Equiper_Detail &detail);
	// 登录时detail赋值
	void load_detail(Equiper_Detail &detail);
	// 保存时detail赋值
	void save_detail(Equiper_Detail &detail);
	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	int client_ready_now(void);

	void equiper_tick(void);

	// MSG start ===================================
	int req_equip_on_off(const int index_from, const int index_to);
	int req_equip_unban(const uint32_t index, const uint32_t lucky_stone_id, const uint32_t auto_buy);

	int req_equip_master(const uint32_t index, const uint8_t auto_buy); // 装备精通
	int req_equip_compose(const int32_t level_up_id, const int amount, const int8_t use_diamond); // 装备合成
	int req_equip_master_info(void); // 获取精通信息
	int req_equip_breakdown(const Int_Vec &equip_index); //装备分解
	int req_equip_breakdown_prop(const Int_Vec &equip_index); //装备分解获得道具

	int req_equip_enchant(const int equip_index, const int8_t is_bat, const int8_t auto_buy_stone, const int8_t auto_buy_lock, const Int8_Vec &locks);
	int req_equip_enchant_replace_prop_group(const int equip_index, const int8_t group_index);
	int get_enchant_weight_ratio(const Enchant_Weight_Ratio_Vec& vec, const Item_Detail &equip);

	void active_item_addit_info_update(const Item_Detail *item);
	void set_item_bind(Item_Detail *item, bool notify = true);
	int active_equip_master_info(Logic_Player* target_player);
	// MSG end =====================================

	int equiper_prop_refresh(int from);
	int calculate_equip_force(const std::map<int, double> &pro_val_map);
	int calculate_equip_force(const uint equip_id);		// 装备基本属性分数
	int refresh_equip_force(Item_Detail *equip, bool filter_rune = false,  bool filter_master = false);
	inline int cnt_enchant_prop(const int equip_color);

	void refresh_over_due(Item_Detail *equip);

	/*
	 * interface:
	 */
	//get_xxxxx(void);
	int get_part_by_index(const int equip_index);
	int get_part_by_id(const int equip_id);
	int get_suit_ids(void);
	int get_equips_enchant_star(void);
	int get_equips_max_possible_enchant_star(void);
	int get_master_effect_lvl_sum_on_player(void);
	int get_master_effect_lvl_sum_on_heroer(void);

	int test_get_and_enhance_equips(const int suit_id, const int unban_lvl = 10);
	int test_unban_equips(const int unban_lvl);
	int test_master(const int master_lvl);

	Int_Vec statistic_suit_skill(const int suit_idx_begin, const int suit_idx_end);
	void init_suit_skill();
	Int_Vec get_equips_self_skills(const int idx_begin, const int idx_end);     // 获取指定index段装备本身技能
	void init_equips_self_skills(const int idx_begin, const int idx_end);    // 刷新指定index段装备本身技能

	bool has_put_on_suit_fully(const int suit_id, const int hero_index);
	int get_put_on_suit_count(const int suit_id, const int hero_index);

	inline Equiper_Detail const &equiper_detail(void) const;
	void get_jewelry_color_num(std::map<int32_t, int32_t>  &jewelry_map);
	//取得身上 各种颜色的 装备的个数
	void get_equip_color_num(int equip_part, std::map<int32_t, int32_t>  &equip_map);

	void refresh_equip_avatar(Int_Int_Map &avatar_map);

	// 装备宝石操作
	// 装备开宝石孔
	int handle_open_equip_gemstone_hole(int equip_id, int index);
	// 宝石镶嵌装备
	int handle_embed_gemstone(int equip_index, int index, int gemstone_id);
	// 卸装宝石
	int handle_unembed_gemstone(int equip_id, int index);
	// 宝石转化
	int handle_gemstone_convert(int src_gemstone_id, int des_gemstone_id, int num);
	// 宝石合成信息
	int handle_gem_enable_info(void);
	// 宝石合成id
	int set_gem_compose_id(int gem_id);
	// 卸装宝石
	int unembed_gemstone(Item_Detail *equip, int index);
	// 刷新宝石属性
	void gemstone_prop_refresh(const Item_Detail &equip, std::map<int, double> &prop_map);
	// 刷新宝石套装属性
	void gemstone_suit_prop_refresh(const Int_Int_Map &color_suit_map,
			std::map<int, double> &prop_map);
	// 计算各颜色宝石
	void calc_genstone(const Item_Detail& equip, Int_Int_Map &color_suit_map);
	// 获取X等级以上宝石数量
	int get_gemstone_count(int lv);
	int get_gemstone_count(int start_index, int end_index, int lv);
	// 获取身上所有宝石的属性
	void get_all_embed_gemstone_prop(std::map<int, double> &prop_map);
	void get_all_embed_gemstone_prop(int start_index, int end_index, std::map<int, double> &prop_map);
	void refresh_prop_when_gemstone_change(int equip_index);
	bool is_equip_embed_gemstone(const Item_Detail &equip);
	int req_gem_carve_info(int essence_num);  //宝石雕刻信息
	int req_gem_carve_lvl_info(void);  //宝石雕刻等级进度

	int gem_compose_announce(const int gem_id);
	int gem_carve_announce(void);

protected:
	int base_prop_refresh(const Item_Detail_Config *item_config, std::map<int, double> &pro_val_map);
	int unban_prop_refresh(const Item_Detail_Config *item_config, const int unban_lvl, std::map<int, double> &pro_val_map);
	int unban_whole_prop_refresh(const int unban_whole_lvl, std::map<int, double> &pro_val_map);
	int suit_prop_refresh(const std::map<int, int> &suit_cnt_map, std::map<int, double> &pro_val_map);
	int enchant_prop_refresh(const Item_Detail *equip, std::map<int, double> &pro_val_map);

	// 装备精通
	int equip_master(Master_Part_Info_Detail& info, const int auto_buy, int &is_success, int32_t &copper, std::vector<Item_Basic_Info> &item);
	int master_base_prop_refresh(const Item_Detail_Config *item_config,
			const int32_t master_level, std::map<int, double> &pro_val_map); // 精通加成
	int master_unban_prop_refresh(const Item_Detail_Config *item_config,
			const int unban_lvl, const int32_t master_level, std::map<int, double> &pro_val_map); // 精通加成
	int master_whole_prop_refresh(const int master_whole_lvl, std::map<int, double> &pro_val_map); // 精通装备共鸣加成
	int master_whole_base_prop_refresh(const Item_Detail_Config *item_config,
			const int32_t master_level, std::map<int, double> &pro_val_map); // 精通装备共鸣加成
	int master_whole_unban_prop_refresh(const Item_Detail_Config *item_config,
			const int unban_lvl, const int32_t master_level, std::map<int, double> &pro_val_map); // 精通装备共鸣加成


private:
	int cal_cnt_enchant(const int is_bat, const int8_t auto_buy_stone, const int8_t auto_buy_lock, const Int8_Vec &locks, const Enchant_Mix_Cfg& enchant_mix_cfg,
			int &cnt_enchant, int &need_copper, int &need_gold, int &erase_stone_amount, int &erase_lock_amount);
	int cal_enchant_prop_weights(const Item_Detail *equip, const Enchant_Part_Cfg* enchant_part_cfg, const Enchant_Mix_Cfg& enchant_mix_cfg,
			 const std::map<int, int> &saved_lock_prop_cnt_map, const int idx_enchant_groups, Int_Vec &weights, int enchant_weight_rate);
	int enchant_make_saved_lock_prop_cnt_map(const Item_Detail *equip, std::map<int, int> &saved_lock_prop_cnt_map);
	int init_enchant_lock_prop_fail(Item_Detail *equip, const Int8_Vec &locks, uint8_t cnt_enchant_prop, std::map<int, int>& fail_prop_cnt_map);
	int enchant_one_prop(Item_Detail *equip, const Enchant_Part_Cfg* enchant_part_cfg, const Enchant_Color_Cfg* enchant_color_cfg,
			const Equip_Lvl_Cfg* equip_lvl_cfg,	const Enchant_Mix_Cfg& enchant_mix_cfg,	const std::map<int, int>& saved_lock_prop_cnt_map,
			const Int8_Vec& locks, std::map<int, int>& fail_prop_cEquipernt_map, uint8_t cnt_enchant_prop, uint8_t idx_enchant_groups,
			uint8_t idx_enchant_props, Int_Set& output_props, int enchant_weight_rate);
	bool in_array_enchant_fail(const int enchant_prop, const Item_Detail *equip);
	void set_enchant_lock_prop_fail(Item_Detail *equip, const std::map<int, int>& fail_prop_cnt_map);
	void enchant_broadcast(const Item_Detail *equip);

private:
	Equiper_Detail *equiper_detail_;
	Item_Vec equiper_breakdown_prop;
};

/////////////////////////////////////////////////////////////////////////////

inline Equiper_Detail const &Equiper::equiper_detail(void) const{
	return *equiper_detail_;
}

inline int Equiper::cnt_enchant_prop(const int equip_color) {
	switch (equip_color) {
	case Item_Detail::WHITE:
		return 1;
	case Item_Detail::GREEN:
		return 2;
	case Item_Detail::BLUE:
		return 3;
	case Item_Detail::PURPLE:
		return 5;
	case Item_Detail::ORANGE:
		return 5;
	default:
		return 5;
	}
}

#endif /* EQUIPER_H_ */
