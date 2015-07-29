/*
 * World_Boss_Config.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef WORLD_BOSS_CONFIG_H_
#define WORLD_BOSS_CONFIG_H_

#include "Game_Header.h"
#include "Base_Struct.h"
#include "status/Status_Struct.h"

struct World_Boss_Base_Config{
	World_Boss_Base_Config(void);
	~World_Boss_Base_Config(void);
	void reset(void);

	std::vector<Int_Int> open_time;

	std::vector<int> power_up_num;
	std::vector<int> power_up_cost;
	std::vector<int> reborn_num;
	std::vector<int> reborn_cost;
	std::vector<int> gold_bless;
	std::vector<int> gold_bless_effect;
	int common_reborn_cost;
	int common_reborn_num;
	int leader_hurt;
	int reborn_time;
	int reborn_time_fix_max;
	int reborn_time_fix;
	int powerup_costtype;
	int reborn_costtype;
	int gold_bless_costtype;
	int boss_open_tiem;
	int rapid_kill_time;
	int gold_bless_num;
	int boss_max_lv;
	Status_Effect_Argv power_up_buff;
	Status_Effect_Argv leader_buff;
	Status_Effect_Argv boss_weak_buff1;
	Status_Effect_Argv boss_weak_buff2;
	std::vector<int> boss_weak_hp;
};

struct World_Boss_Level_Config{
	World_Boss_Level_Config(void);
	~World_Boss_Level_Config(void);
	void reset(void);

	int boss_level;
	int boss_kill;
	int monster_id;
	int rank_reward_money;
	int hurt_reward;
	int low_hero_fix;
	int mid_hero_fix;
	int high_hero_fix;
	std::vector<int> drop_box;
	std::vector<Int_Int> low_hero_item;
	std::vector<Int_Int> mid_hero_item;
	std::vector<Int_Int> high_hero_item;
	std::vector<Int_Int> first_blood_reward;
	std::vector<Int_Int> first_kill_reward;
};
typedef std::map<int, World_Boss_Level_Config> World_Boss_Level_Config_Map;

struct World_Boss_Reward_Config{
	World_Boss_Reward_Config(void);
	~World_Boss_Reward_Config(void);
	void reset(void);

	Int_Int level_group;
	int ranb_high;
	int rank_low;
	std::vector<Int_Int> reward_rank_base_money;
	std::vector<Int_Int> reward_rank_base_item;
};
typedef boost::unordered_map<int, World_Boss_Reward_Config> World_Boss_Reward_Config_Map;

#endif /* WORLD_BOSS_CONFIG_H_ */
