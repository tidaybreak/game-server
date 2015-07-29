/*
 * Valentine_Monster_Struct.h
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#ifndef VALENTINE_MONSTER_STRUCT_H_
#define VALENTINE_MONSTER_STRUCT_H_

#include "Game_Typedef.h"
#include "Monitor_Struct.h"
#include "Base_Struct.h"

struct Valentine_Monster_Info{
	Valentine_Monster_Info(void);
	~Valentine_Monster_Info(void);
	void reset(void);

	Monitor_Unique_ID muid;
	int boss_kill_result;
	int boss_point_id;
	int boss_type_id;
	int boss_hp_cur;
	int boss_hp_max;
	int boss_fight_time;
	int normal_point_amount;
	std::vector<Int_Int> boss_reward;
	Time_Value boss_end_time;
	Time_Value boss_reward_time;
	Role_Id_Set fighter_set_;
};



#endif /* VALENTINE_MONSTER_STRUCT_H_ */
