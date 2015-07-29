/*
 * Gang_War_Config.h
 *
 *  Created on: 2014年8月14日
 *      Author: Linqiyou
 */

#ifndef GANG_WAR_CONFIG_H_
#define GANG_WAR_CONFIG_H_

#include "Base_Struct.h"

struct Gang_War_Rank_Detail_Cfg {
	Gang_War_Rank_Detail_Cfg(void) { reset(); }
	void reset(void) {
		index = 0;
//		fund = 0;
		list.clear();
		self_list.clear();
		lose_list.clear();
		lose_self_list.clear();
//		win_self_contri = 0;
//		win_self_mercit = 0;
	}
	int8_t index;
//	int fund;
	Item_Info_Vec list;
	Item_Info_Vec self_list;
	Item_Info_Vec lose_list;
	Item_Info_Vec lose_self_list;
//	int win_self_contri;
//	int win_self_mercit;
};
typedef boost::unordered_map<int8_t, Gang_War_Rank_Detail_Cfg> Gang_War_Rank_Cfg;

struct Gang_War_Rank_Award_Detail_Cfg {
	Gang_War_Rank_Award_Detail_Cfg(void) { reset(); }
	void reset(void) {
		rank = 0;
		stage = 0;
		list.clear();
		all_list.clear();
	}
	int8_t rank;
	int8_t stage;
	Item_Info_Vec list;
	Item_Info_Vec all_list;
};
typedef boost::unordered_map<int8_t, Gang_War_Rank_Award_Detail_Cfg> Gang_War_Rank_Award_Cfg;

struct Gang_War_Setting_Obstacles {
	Gang_War_Setting_Obstacles(void) { reset(); }
	void reset(void) {
		npc_id = 0;
		coord.reset();
	}
	int npc_id;
	Coord coord;
};
typedef std::vector<Gang_War_Setting_Obstacles> Gang_war_Setting_Obstacles_Vec;
struct Gang_War_Setting_Cfg {
	Gang_War_Setting_Cfg(void) { reset(); }
	void reset(void) {
		contribute = 0;
		mercit = 0;
		finish_time = 0;
		ready_time = 0;
		tower_change_time = 0;
		tower_score = 0;
		tower_score_center = 0;
		tower_score_tick = 0;
		win_score = 0;
		skill_cd = 0;
		revive_time = 0;
		level_limit = 0;
		box_limit = 0;
		win_scores.clear();
		lose_scores.clear();
		empty_score = 0;
		kill_score = 0;
		hold_score = 0;
	}
	int contribute;
	int mercit;
	int finish_time;
	int ready_time;
	int tower_change_time;
	int tower_score;
	int tower_score_center;
	int tower_score_tick;
	int win_score;
	int skill_cd;
	int revive_time;
	int level_limit;
	int box_limit;
	Int_Int_Map win_scores;
	Int_Int_Map lose_scores;
	int empty_score;
	int kill_score;
	int hold_score;

};


#endif /* GANG_WAR_CONFIG_H_ */
