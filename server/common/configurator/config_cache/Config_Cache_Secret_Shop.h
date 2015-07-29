/*
 * Config_Cache_Secret_Shop.h
 *
 *  Created on: 2014年7月31日
 *      Author: root
 */

#ifndef CONFIG_CACHE_SECRET_SHOP_H_
#define CONFIG_CACHE_SECRET_SHOP_H_


#include "Cache_Base.h"
#include "secret_shop/Secret_Shop_Config.h"

struct SC_Item_Info;


class Config_Cache_Secret_Shop : public Cache_Base {
public:
	void refresh_config_cache(void);
public:
	const Secret_Base_Config* base_config(void);
	int get_secret_point(const int scene_id, Coord& coord, std::vector<int>& ann_ids, int& ann_space);
	bool is_secret_point(const int scene_id);
	int get_secret_shop_item(const int chapman, std::vector<SC_Item_Info>& items);
	bool is_valid_activate_time(const int time, int& valid_time);

	Campfire_Base_Config* campfire_base_config(void);
	Campfire_Point_Config* campfire_meat_point_config();
	Campfire_Gain_Config* get_campfire_gain(int level);
	Campfire_Award_Config* campfire_award_config(void);
	int get_campfire_level_max_firewood(int cur_level);
	Coord get_campfire_birth_coord(const int point_id);
	std::vector<Coord> get_campfire_monster_points();
	int get_campfire_level_exp(int level);
	int get_campfire_level_limit(void);
	int get_campfire_dancing_time(void);
	int get_campfire_exp_time(void);

	int cal_campfire_open_time(const int cur_time, int& valid_status, int& valid_time, int& valid_last_time, bool& next_day);

	void get_month_hero_day(const int game_open_day, int& cur_day, int& next_day);

	// 点火成功文字
	std::string get_text_fire_success(void);
	// 添火成功文字
	std::string get_text_firewood_success(void);
	// 进入自己公会篝火文字
	std::string get_text_enter_own_fire(void);
	// 进入其他他公会篝火文字
	std::string get_text_enter_other_fire(void);
	// 远离篝火文字
	std::string get_text_leave_fire(void);
	//女神相对于篝火的位置
	Coord get_goddess_offset(void);
private:
	void refresh_base(void);
	void refresh_point(void);
	void refresh_shop(void);

	void refresh_campfire_base(void);
	void refresh_campfire_point(void);
	void refresh_campfire_gain(void);
	void refresh_campfire_level_exp(void);
	void refresh_campfire_award(void);

	void refresh_month_hero(void);

	void add_secret_point(const Secret_Point_Config& config);
	void get_random_shop_item(const Secret_Item_Config_Map& item_map, int count, std::vector<SC_Item_Info>& items);
	void get_chapman_item(int chapman, const Secret_Item_Config_Map& item_map, std::vector<const Secret_Item_Config*>& item_vec);
private:
	Secret_Base_Config base_config_;
	Secret_Point_Map point_map_;
	Secret_Item_Config_Map copper_item_map_;
	Secret_Item_Config_Map gold_item_map_;
	Secret_Item_Config_Map rare_item_map_;
	// 每月英雄信息
	std::vector<int> month_hero_info_;
	// 篝火晚会
	Campfire_Base_Config campfire_base_;
	Campfire_Point_Config campfire_meat_point_;
	Campfire_Point_Config campfire_monster_point_;
	Campfire_Point_Config_Map campfire_birth_point_map_;
	Campfire_Gain_Map campfire_gain_map_;
	Campfire_Level_Exp_Map campfire_level_exp_map_;
	Campfire_Award_Config campfire_award_cfg_;
};

typedef Singleton<Config_Cache_Secret_Shop> Config_Cache_Secret_Shop_Single;
#define CONFIG_CACHE_SECRET_SHOP (Config_Cache_Secret_Shop_Single::instance())



#endif /* CONFIG_CACHE_SECRET_SHOP_H_ */
