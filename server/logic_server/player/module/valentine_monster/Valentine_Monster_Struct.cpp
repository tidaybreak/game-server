/*
 * Valentine_Monster_Struct.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */


#include "Valentine_Monster_Struct.h"

Valentine_Monster_Info::Valentine_Monster_Info(void){
	reset();
}

Valentine_Monster_Info::~Valentine_Monster_Info(void){

}

void Valentine_Monster_Info::reset(void){
	muid.reset();
	boss_kill_result = 0;
	boss_point_id = 0;
	boss_type_id = 0;
	boss_hp_cur = 0;
	boss_hp_max = 0;
	boss_fight_time = 0;
	normal_point_amount = 0;
	boss_reward.clear();
	boss_end_time = Time_Value::zero;
	boss_reward_time = Time_Value::zero;
	fighter_set_.clear();
}
