/*
 * World_Boss_Scene_Struct.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#include "World_Boss_Scene_Struct.h"

World_Boss_Hp_Info::World_Boss_Hp_Info(void){
	reset();
}

World_Boss_Hp_Info::~World_Boss_Hp_Info(void){

}

void World_Boss_Hp_Info::reset(void){
	boss_lv = 0;
	cfg_boss_lv = 0;
	hurt_reward_coe = 0;
	boss_type_id = 0;
	pre_hp_cur = 0;
	hp_cur = 0;
	hp_max = 0;
	bonus_id = 0;
	buffer_step = 0;
	bonus_role_id = 0;
	bonus_role_name = "";
	boss_name = "";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


World_Boss_Player_Info::World_Boss_Player_Info(void){
	reset();
}

World_Boss_Player_Info::~World_Boss_Player_Info(void){

}

void World_Boss_Player_Info::reset(void){
	achieve_1 = 0;							// 初级勇士
	achieve_2 = 0;							// 中级勇士
	achieve_3 = 0;							// 高级勇士
	achieve_4 = 0;							// 一血
	achieve_5 = 0;							// 击杀boss
	ability_0.reset();						// 公会祝福 val1 cur val2 max
	ability_1.reset();						// 浴火重生
	ability_2.reset();						// 激发潜能
	ability_3.reset();

	ability_3_coe = 0;
	fight_damage = 0;
	gang_id = 0;						// 公会id
	career_rank = 0;
	revive_time = Time_Value::zero;
	rank_info.reset();
}

int World_Boss_Player_Info::get_revive_time(void){
	int time = revive_time.sec() - Time_Value::gettimeofday().sec();
	if(time < 0){
		time = 0;
	}
	return time;
}

void World_Boss_Player_Info::set_revive_time(const int time){
	revive_time = Time_Value::gettimeofday() + Time_Value(time);
}

bool World_Boss_Rank_Greater(World_Boss_Player_Info* info1, World_Boss_Player_Info* info2){
	if(info1->rank_info.damage > info2->rank_info.damage){
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////

World_Boss_Gang_Bless::World_Boss_Gang_Bless(void){
	reset();
}

World_Boss_Gang_Bless::~World_Boss_Gang_Bless(void){

}

void World_Boss_Gang_Bless::reset(void){
	gang_pos = 0;
	role_id = 0;
	role_name = "";
	bless_num = 0;
}
