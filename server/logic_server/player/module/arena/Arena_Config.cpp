/*
 * Arena_Config.cpp
 *
 *  Created on: 2014年5月23日
 *      Author: root
 */

#include "Arena_Config.h"
#include "Config_Cache_Arena.h"

Arena_Award_Config::Arena_Award_Config(void){
	reset();
}

void Arena_Award_Config::reset(void){
	rank_begin = 0;
	rank_end = 0;
	rank_award_soul = 0;
	rank_award_gold = 0;
	rank_award_exploit = 0;
	rank_award_item_vec.clear();
}

///////////////////////////////////////////////////////////////////////////


Arena_Season_Award_Config::Arena_Season_Award_Config(void){
	reset();
}

void Arena_Season_Award_Config::reset(void){
	rank_award_soul = 0;
	rank_award_gold = 0;
	rank_award_item_vec.clear();
}

///////////////////////////////////////////////////////////////////////////

Arena_Base_Config::Arena_Base_Config(void){
	reset();
}

void Arena_Base_Config::reset(void){
	arena_group = 0;
	arena_part = 0;
	arena_award = 0;
	arena_punish = 0;

	arena_award_hard = 0;
	arena_punish_hard = 0;
	arena_award_gold = 0;
	arena_punish_gold = 0;

	winer_award = 0;
	loser_award = 0;
	winer_award_item_vec.clear();
	loser_award_item_vec.clear();

	winer_award_item_hard.clear();
	loser_award_item_hard.clear();
	winer_award_item_gold.clear();
	loser_award_item_gold.clear();
	arena_serve = 0;
	arena_promote = 0;
	arena_promote_award = 0;
	arena_promote_punish = 0;
	arena_promote_awarditem_vec.clear();
	name = "";
	bronze_refer_force = 0;
}

/////////////////////////////////////////////////////////////////////////////////

Arena_Setting_Config::Arena_Setting_Config(void){
	reset();
}

void Arena_Setting_Config::reset(void){
	athletics_number = 0;
	athletics_buy_limit = 0;
	athletics_initial_cost = 0;
	athletics_cost_increase = 0;
	cd = 0;
	min_cd = 0;
	reset_cd_cost = 0;
	no_cd_vip_lv = 0;
	fight_map_id = 0;
	arena_open_lv = 0;
	pay_time_vip_lv = 0;
	refresh_match_cost = 0;
	refresh_match_time = 0;
	mail_sender = "";
	mail_title = "";
	mail_content = "";
	text_copper = "";
	text_soul = "";
	text_exploit = "";
//	adore_cost_1.clear();
//	adore_cost_2.clear();
//	adore_cost_3.clear();
//	adore_item_1.clear();
//	adore_item_2.clear();
//	adore_item_3.clear();
//	adore_times = 0;
}

//////////////////////////////////////////////////////////////////////////////////

Arena_Levelfix_Config::Arena_Levelfix_Config(void){
	reset();
}

void Arena_Levelfix_Config::reset(void){
	level = 0;
	fix_level = 1.0;
}

/////////////////////////////////////////////////////////////////////////////////


Arena_Season_Config::Arena_Season_Config(void){
	reset();
}

void Arena_Season_Config::reset(void){
	season_status = 0;
	begin_time = Time_Value::zero;
	finish_time = Time_Value::zero;
	ref_time = Time_Value::zero;
	ref_time1 = Time_Value::zero;
	ref_time2 = Time_Value::zero;
}

void Arena_Season_Config::save(void){
	CONFIG_CACHE_ARENA->save_arena_season_to_json();
}

///////////////////////////////////////////////////////////////

