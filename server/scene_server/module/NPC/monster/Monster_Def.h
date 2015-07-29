/*
 * Monster_Def.h
 *
 *  Created on: 2014年6月9日
 *      Author: Linqiyou
 */

#ifndef MONSTER_DEF_H_
#define MONSTER_DEF_H_

#include "Game_Typedef.h"

const int FIGHT_DISTANCE = 64;
const int FIGHT_TEAM_DISTANCE = 150;

struct Monster_Chest_Info{
	Monster_Chest_Info(void){
		reset();
	}
	void reset(void){
		monster_type_id = 0;
		monster_model_id = 0;
		monster_name = "";
		drops_vec.clear();
		drop_type = 0;
		gold_fix_coe = 0;
		item_fix_coe = 0;
	}
	// 是否宝箱怪
	bool is_monster_chest(int value){
		return (value == monster_type_id);
	}
	//
	int monster_type_id;			//  怪物类型
	int monster_model_id;		//  怪物模型
	std::string monster_name;	//  怪物名称
	std::vector<int> drops_vec;		//  掉落包
	int drop_type;					//  掉落形式
	double gold_fix_coe;				//  金钱修正系数
	double item_fix_coe;				//  物品修正系数
};

#endif /* MONSTER_DEF_H_ */
