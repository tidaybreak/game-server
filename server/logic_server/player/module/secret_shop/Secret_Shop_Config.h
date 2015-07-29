/*
 * Secret_Shop_Config.h
 *
 *  Created on: 2014年7月31日
 *      Author: root
 */

#ifndef SECRET_SHOP_CONFIG_H_
#define SECRET_SHOP_CONFIG_H_


#include "Game_Header.h"
#include "Base_Struct.h"

struct Secret_Base_Config{
	Int_Int refresh_time;
	Int_Int duration_time;
	std::vector<Int_Int> refresh_zone;

	Secret_Base_Config(void){
		reset();
	}
	void reset(void){
		refresh_time.reset();
		duration_time.reset();
		refresh_zone.clear();
	}
};

struct Secret_Item_Config{
	int item;
	int num;
	int money_type;
	int cost;
	int limit_num;
	int type;
	int rate;
	int chapman;
	int binding;

	Secret_Item_Config(void){
		reset();
	}

	void reset(void){
		item = 0;
		num = 0;
		money_type = 0;
		cost = 0;
		limit_num = 0;
		type = 0;
		rate = 0;
		chapman = 0;
		binding = 0;
	}
};
typedef boost::unordered_map<int, Secret_Item_Config> Secret_Item_Config_Map;

struct Secret_Point_Config{
	int scene_id;
	Coord coord;
	std::vector<int> ann_id_vec;
	int ann_space;

	Secret_Point_Config(void){
		reset();
	}
	void reset(void){
		scene_id = 0;
		coord.reset();
		ann_id_vec.clear();
		ann_space = 0;
	}
};
typedef boost::unordered_map<int, std::vector<Secret_Point_Config>> Secret_Point_Map;

struct Campfire_Base_Config{
	int fire_range;
	Int_Int meat_cont;
	int level_limit;
	int dancing_time;
	int exp_time;
	Coord goddess_offset;
	std::vector<Int_Int> open_time;
	std::string text_fire_success;				// 点火成功文字
	std::string text_firewood_success;			// 添火成功文字
	std::string text_enter_own_fire;				// 进入自己公会篝火
	std::string text_enter_other_fire;			// 进入其他他公会篝火
	std::string text_leave_fire;					// 远离篝火
	Campfire_Base_Config(void){
		reset();
	}
	void reset(void){
		fire_range = 0;
		meat_cont.reset();
		level_limit = 0;
		dancing_time = 0;
		open_time.clear();
		exp_time = 0;
		text_fire_success = "";
		text_firewood_success = "";
		text_enter_own_fire = "";
		text_enter_other_fire = "";
		text_leave_fire = "";
		goddess_offset.reset();
		goddess_offset.type = Coord::PIXEL_TYPE;
	}
};

struct Campfire_Point_Config{
	std::vector<Coord> meat_coord;

	Campfire_Point_Config(void){
		reset();
	}
	void reset(void){
		meat_coord.clear();
	}
};
typedef boost::unordered_map<int, Campfire_Point_Config> Campfire_Point_Config_Map;

struct Campfire_Gain_Config{
	int level;
	double exp_add;
	double fs_add;
	int firewood_num;

	Campfire_Gain_Config(void){
		reset();
	}
	void reset(void){
		level = 0;
		exp_add = 0;
		fs_add = 0;
		firewood_num = 0;
	}
};
typedef boost::unordered_map<int, Campfire_Gain_Config> Campfire_Gain_Map;

struct Campfire_Award_Item {
	uint64_t item_id;
	int item_count;
	int bind_type;
};
typedef std::vector<Campfire_Award_Item> Campfire_Award_Item_Vec;

struct Rank_Award_Config {
	uint32_t rank_start;
	uint32_t rank_stop;
	Campfire_Award_Item_Vec awards;
};
typedef std::vector<Rank_Award_Config> Rank_Award_Config_Vec;

struct Campfire_Award_Config {
	enum AWARD_TYPE {
//		奖励1点燃，2添柴，3安慰
		CAMPFIRE_AWARD_LIT = 1,
		CAMPFIRE_AWARD_ADD_WOOD = 2,
		CAMPFIRE_AWARD_CONSOLATION = 3,
		CAMPFIRE_AWARD_RANK
	};
	boost::unordered_map<int, Campfire_Award_Item_Vec> award_configs;

	Rank_Award_Config_Vec rank_award_configs;

	void reset(void) {
		award_configs.clear();
		rank_award_configs.clear();
	}
};

typedef boost::unordered_map<int, int> Campfire_Level_Exp_Map;

#endif /* SECRET_SHOP_CONFIG_H_ */
