/*
 * World_Boss_Config.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */


#include "World_Boss_Config.h"

World_Boss_Base_Config::World_Boss_Base_Config(void){
	reset();
}

World_Boss_Base_Config::~World_Boss_Base_Config(void){

}

void World_Boss_Base_Config::reset(void){
	open_time.clear();

	power_up_num.clear();
	power_up_cost.clear();
	reborn_num.clear();
	reborn_cost.clear();
	gold_bless.clear();
	gold_bless_effect.clear();
	common_reborn_cost = 0;
	common_reborn_num = 0;
	leader_hurt = 0;
	reborn_time = 0;
	reborn_time_fix_max = 0;
	reborn_time_fix = 0;
	powerup_costtype = 0;
	reborn_costtype = 0;
	gold_bless_costtype = 0;
	boss_open_tiem = 0;
	rapid_kill_time = 60;
	gold_bless_num = 0;
	boss_max_lv = 0;
	power_up_buff.reset();
	leader_buff.reset();
	boss_weak_buff1.reset();
	boss_weak_buff2.reset();
	boss_weak_hp.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////
////
///////////////////////////////////////////////////////////////////////////////////////////

World_Boss_Level_Config::World_Boss_Level_Config(void){
	reset();
}

World_Boss_Level_Config::~World_Boss_Level_Config(void){

}

void World_Boss_Level_Config::reset(void){
	boss_level = 0;
	boss_kill = 0;
	monster_id = 0;
	rank_reward_money = 0;
	hurt_reward = 0;
	low_hero_fix = 0;
	mid_hero_fix = 0;
	high_hero_fix = 0;
	drop_box.clear();
	low_hero_item.clear();
	mid_hero_item.clear();
	high_hero_item.clear();
	first_blood_reward.clear();
	first_kill_reward.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////
////////////////////////////////////////////////////////////////////////////////////////////////

World_Boss_Reward_Config::World_Boss_Reward_Config(void){
	reset();
}

World_Boss_Reward_Config::~World_Boss_Reward_Config(void){

}

void World_Boss_Reward_Config::reset(void){
	level_group.reset();
	ranb_high = 0;
	rank_low = 0;
	reward_rank_base_money.clear();
	reward_rank_base_item.clear();
}

