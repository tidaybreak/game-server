/*
 * Scene_Campfire_Struct.h
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#ifndef SCENE_CAMPFIRE_STRUCT_H_
#define SCENE_CAMPFIRE_STRUCT_H_

#include "Game_Header.h"
#include "Game_Typedef.h"
#include "Base_Struct.h"

struct Scene_Campfire_Info{
	role_id_t role_id; //玩家id
	std::string role_name; //玩家名
	Time_Value finish_time; //点燃或添柴的完成时间
	uint32_t rank; //名次
	boost::unordered_set<int32_t> killed_match_man;
	int8_t is_lighter;
	int32_t recieve_stat;//0:不可领 1:可领 2:已领
	int32_t rank_rev_stat;//0:不可领 1:可领 2:已领

	int8_t add_firewood;
	int64_t gang_id;
	std::string gang_name;
	int8_t headship;
	Scene_Campfire_Info(void){
		reset();
	}
	void reset(void){
		role_id = 0;
		role_name = "";
		finish_time.set(0, 0);
		rank = 0;
		killed_match_man.clear();
		recieve_stat = 0;
		rank_rev_stat = 0;
		is_lighter = 0;

		add_firewood = 0;
		gang_id = 0;
		gang_name = "";
		headship = 0;
	}
};
typedef boost::unordered_map<role_id_t, Scene_Campfire_Info> Player_Campfire_Map;

struct Scene_Campfire_Gain_Info{
	role_id_t role_id;
	int point_id;
	bool is_gang_gain;
	Time_Value time;
	Scene_Campfire_Gain_Info(void){
		reset();
	}
	void reset(void){
		role_id = 0;
		point_id = 0;
		is_gang_gain = false;
		time = Time_Value::zero;
	}
};

struct Scene_Campfire_MatchMan_Info {
	role_id_t id;
	Coord birth_coord;
	std::string name;
};
typedef boost::unordered_map<role_id_t, Scene_Campfire_MatchMan_Info> Campfire_Matchman_Map;

#endif /* SCENE_CAMPFIRE_STRUCT_H_ */
