/*
 * arena_config.h
 *
 *  Created on: 2014年5月23日
 *      Author: root
 */

#ifndef ARENA_CONFIG_H_
#define ARENA_CONFIG_H_

#include "Game_Header.h"
#include "Base_Struct.h"


enum ARENA_BATTLE_STATUS{
	ARENA_BATTLE_STATUS_NULL = 0,
	ARENA_BATTLE_STATUS_OVER = 1,
	ARENA_BATTLE_STATUS_COMING = 2,
	ARENA_BATTLE_STATUS_RUNNING = 3,
};

struct Arena_Award_Config{
	int rank_begin;
	int rank_end;
	int rank_award_soul;
	int rank_award_gold;
	int rank_award_exploit;
	std::vector<Int_Int> rank_award_item_vec;

	Arena_Award_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Arena_Award_Config> Arena_Award_Config_Map;

struct Arena_Season_Award_Config{
	int rank_award_soul;
	int rank_award_gold;
	std::vector<Int_Int> rank_award_item_vec;

	Arena_Season_Award_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Arena_Season_Award_Config> Arena_Season_Award_Config_Map;

struct Arena_Base_Config{
	int arena_group;
	int arena_part;
	int arena_award;
	int arena_punish;

	int arena_award_hard;
	int arena_punish_hard;
	int arena_award_gold;
	int arena_punish_gold;

	int winer_award;
	int loser_award;
	std::vector<Id_Int_Int> winer_award_item_vec;
	std::vector<Id_Int_Int> loser_award_item_vec;

	std::vector<Id_Int_Int> winer_award_item_hard;
	std::vector<Id_Int_Int> loser_award_item_hard;
	std::vector<Id_Int_Int> winer_award_item_gold;
	std::vector<Id_Int_Int> loser_award_item_gold;
	int arena_serve;
	int arena_promote;
	int arena_promote_award;
	int arena_promote_punish;
	std::vector<Id_Int_Int> arena_promote_awarditem_vec;
	std::string name;
	int bronze_refer_force;		//匹配参考战斗力
	Arena_Base_Config(void);
	void reset(void);
	int get_arena_award(int8_t d_lv){
		if(d_lv == 1){
			return arena_award_hard;
		}else if(d_lv == 2){
			return arena_award_gold;
		}else{
			return arena_award;
		}
	}
	int get_arena_punish(int8_t d_lv){
		if(d_lv == 1){
			return arena_punish_hard;
		}else if(d_lv == 2){
			return arena_punish_gold;
		}else{
			return arena_punish;
		}
	}
	std::vector<Id_Int_Int> get_winer_award_item(int8_t d_lv){
		if(d_lv == 1){
			std::vector<Id_Int_Int> rs_vec = winer_award_item_vec;
			rs_vec.insert(rs_vec.end(), winer_award_item_hard.begin(), winer_award_item_hard.end());
			return rs_vec;
		}else if(d_lv == 2){
			std::vector<Id_Int_Int> rs_vec = winer_award_item_vec;
			rs_vec.insert(rs_vec.end(), winer_award_item_gold.begin(), winer_award_item_gold.end());
			return rs_vec;
		}else{
			return winer_award_item_vec;
		}
	}
	std::vector<Id_Int_Int> get_loser_award_item(int8_t d_lv){
		if(d_lv == 1){
			std::vector<Id_Int_Int> rs_vec = loser_award_item_vec;
			rs_vec.insert(rs_vec.end(), loser_award_item_hard.begin(), loser_award_item_hard.end());
			return rs_vec;
		}else if(d_lv == 2){
			std::vector<Id_Int_Int> rs_vec = loser_award_item_vec;
			rs_vec.insert(rs_vec.end(), loser_award_item_gold.begin(), loser_award_item_gold.end());
			return rs_vec;
		}else{
			return loser_award_item_vec;
		}
	}
};
typedef boost::unordered_map<int, Arena_Base_Config> Arena_Base_Config_Map;


struct Arena_Setting_Config{
	int athletics_number;
	int athletics_buy_limit;
	int athletics_initial_cost;
	int athletics_cost_increase;
	int cd;
	int min_cd;
	int reset_cd_cost;
	int no_cd_vip_lv;
	int fight_map_id;
	int arena_open_lv;
	int pay_time_vip_lv;
	int refresh_match_cost;
	int refresh_match_time;
	std::string mail_sender;
	std::string mail_title;
	std::string mail_content;
	std::string text_copper;
	std::string text_soul;
	std::string text_exploit;
//	std::vector<Int_Int> adore_cost_1;
//	std::vector<Int_Int> adore_cost_2;
//	std::vector<Int_Int> adore_cost_3;
//	std::vector<Int_Int> adore_item_1;
//	std::vector<Int_Int> adore_item_2;
//	std::vector<Int_Int> adore_item_3;
//	int adore_times;
	Arena_Setting_Config(void);
	void reset(void);
};


struct Arena_Levelfix_Config{
	int level;
	double fix_level;

	Arena_Levelfix_Config(void);
	void reset(void);
};

typedef boost::unordered_map<int, Arena_Levelfix_Config> Arena_Levelfix_Config_Map;

struct Arena_Season_Config{
	int season_status;							// o over 1 coming 2 running
	Time_Value begin_time;
	Time_Value finish_time;
	Time_Value ref_time;							// 1 day refresh time
	Time_Value ref_time1;						// 3 days reward time
	Time_Value ref_time2;						// 7 days reward time

	Arena_Season_Config(void);
	void reset(void);
	void save(void);
};

typedef std::map<int, Int_Int, std::less<int>> Arena_Force_Level_Map;

#endif /* ARENA_CONFIG_H_ */
