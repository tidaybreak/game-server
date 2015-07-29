/*
 * Team_Config.h
 *
 *  Created on: 2014年4月22日
 *      Author: Linqiyou
 */

#ifndef TEAM_CONFIG_H_
#define TEAM_CONFIG_H_

#include "Base_Struct.h"

struct Team_Robot_Config {
	Team_Robot_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		name.clear();
		career = 0;
		lvl = 0;
		equip.clear();
		skill.clear();

		hero_id = 0;
		hero_name.clear();
		hero_fight = 0;
		hero_lvl = 0;
		hero_awake_lvl = 0;
		hero_equip.clear();
		hero_skill.clear();
	}
	int8_t id;
	std::string name;
	int8_t career;
	int lvl;
	Int_Hash_Set equip;
	Int_Hash_Set skill;

	int hero_id;
	std::string hero_name;
	int8_t hero_fight; // 5 8
	int hero_lvl;
	int hero_awake_lvl;
	Int_Hash_Set hero_equip;
	Int_Hash_Set hero_skill;
};
typedef boost::unordered_map<int8_t, Team_Robot_Config> Team_Robot_Config_Map;

struct Team_Scene_Config {
	enum Scene_Type {
		SCENE_TYPE_START 	= 0,
		SCENE_TYPE_FB 		= 1,
		SCENE_TYPE_BATTLE 	= 2,
		SCENE_TYPE_ACTIVITY = 3,
		SCENE_TYPE_END
	};
	typedef boost::unordered_map<Scene_Type, int> Scene_Type_Map;
	Team_Scene_Config(void) { reset(); }
	void reset(void) {
		data.clear();
	}
	Scene_Type_Map data;
};
struct Team_Config {
	Team_Config(void) { reset(); }
	void reset(void) {
		scene.reset();
		profile_num_fb = 0;
		profile_num_sports = 0;
		profile_num_hero_dream = 0;
		profile_num_defend = 0;
//		words.clear();
		offline_limit = Time_Value::zero;
		scene_id = 0;
		robot_task = 0;
		friendship_timelimit = 0;
		world_invite_cost.clear();
		max_player = 3;
	}

	Team_Scene_Config scene;
	int max_player;		// 组队竞技最大玩家
	int profile_num_fb;
	int profile_num_sports;
	int profile_num_hero_dream;
	int profile_num_defend;
//	std::string words;
	Time_Value offline_limit;
	int scene_id;
	int robot_task;
	int friendship_timelimit;
	Item_Info_Vec world_invite_cost;
};

struct Arena_Multi_Detail {
	Arena_Multi_Detail(void) {
		reset();
	}
	void reset(void) {
		level_ = 0;
		victory_ = 0;
		friendship_ = 0;
		integrate_ = 0;
		exp_ = 0;
		gold_ = 0;
		item_vec_.clear();
		merit_ = 0;
	}
	int32_t level_;
	int8_t victory_;
	int64_t friendship_;
	int64_t integrate_;
	int32_t exp_;
	int32_t gold_;
	Item_Info_Vec item_vec_;
	int merit_;
};
typedef boost::unordered_map<int32_t, Arena_Multi_Detail> Level_ArenaMultiDetail_Map;

//清榜结算奖励
struct Arena_Multi_Checkout_Rank {
	Arena_Multi_Checkout_Rank(void) { reset(); }
	void reset(void) {
		low = 0;
		high = 0;
	}
	int low;
	int high;
};
struct Arena_Multi_Checkout_Detail {
	Arena_Multi_Checkout_Detail(void) { reset(); }
	void reset(void) {
		index = 0;
		rank.reset();
		item_vec.clear();
	}
	int index;
	Arena_Multi_Checkout_Rank rank;
	Item_Info_Vec item_vec;
};
typedef std::map<int, Arena_Multi_Checkout_Detail> Index_ArenaMultiCheckout_Map;

struct Team_Defend_Cfg_Detail {
	Team_Defend_Cfg_Detail(void) { reset(); }
	void reset(void) {
		scene_id = 0;
		birth_coord.clear();
		monster_ids.clear();
		boss_id = 0;
		wave = 0;
		init_time = 0;
		award_time = 0;
		wave_time = 0;
		first_wave_time = 0;
	}
//	副本ID	守卫帝都boss随机点	怪物列表名	boss名	波数	初始时间	奖励时间
//	sceneId	birth_coord	name_list	boss_name	wave	init_time	award_time
	int scene_id;
	Coord_Vec birth_coord;
	Int_Hash_Set monster_ids;
	int boss_id;
	int wave;
	int init_time;
	int award_time;
	int wave_time;
	int first_wave_time;
};
typedef boost::unordered_map<int, Team_Defend_Cfg_Detail> Team_Defend_Cfg_Map;

struct Team_Arena_Correct {
	Team_Arena_Correct(void) { reset(); }
	void reset(void) {
		gap.reset();
		correct.reset();
	}
	Arena_Multi_Checkout_Rank gap;
	Arena_Multi_Checkout_Rank correct;
};
typedef std::vector<Team_Arena_Correct> Team_Arena_Correct_Vec;

#endif /* TEAM_CONFIG_H_ */
