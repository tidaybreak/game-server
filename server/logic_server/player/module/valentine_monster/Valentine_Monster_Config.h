/*
 * Valentine_Monster_Config.h
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#ifndef VALENTINE_MONSTER_CONFIG_H_
#define VALENTINE_MONSTER_CONFIG_H_

#include "Game_Header.h"
#include "Base_Struct.h"

struct Valentine_Monster_Config{
	Valentine_Monster_Config(void){
		reset();
	}
	~Valentine_Monster_Config(void){

	}
	void reset(void){
		scene_key = 0;
		type = 0;
		id = 0;
		monster_amount = 0;
		refresh_time = 0;
		fight_time = 0;
		birth_radius = 0;
		birth_coord.clear();
		boss_reward.clear();
	}

	int scene_key;
	int type;
	int id;
	int monster_amount;
	int refresh_time;
	int fight_time;
	int birth_radius;
	std::vector<Coord> birth_coord;
	std::vector<Int_Int> boss_reward;
};
typedef boost::unordered_map<int, Valentine_Monster_Config> Vlt_Monster_Point_Map;
typedef boost::unordered_map<int, Vlt_Monster_Point_Map> Vlt_Monster_Scene_Map;

#endif /* VALENTINE_MONSTER_CONFIG_H_ */
