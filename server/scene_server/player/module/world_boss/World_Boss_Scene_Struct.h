/*
 * World_Boss_Scene_Struct.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef WORLD_BOSS_SCENE_STRUCT_H_
#define WORLD_BOSS_SCENE_STRUCT_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"


struct World_Boss_Hp_Info{
	World_Boss_Hp_Info(void);
	~World_Boss_Hp_Info(void);
	void reset(void);

	int boss_lv;
	int cfg_boss_lv;
	int hurt_reward_coe;
	int boss_type_id;
	int pre_hp_cur;
	int hp_cur;
	int hp_max;
	int bonus_id;
	int buffer_step;
	role_id_t bonus_role_id;
	std::string bonus_role_name;
	std::string boss_name;
};


struct World_Boss_Player_Info{
	World_Boss_Player_Info(void);
	~World_Boss_Player_Info(void);
	void reset(void);
	int get_revive_time(void);
	void set_revive_time(const int time);

	int8_t achieve_1;							// 初级勇士
	int8_t achieve_2;							// 中级勇士
	int8_t achieve_3;							// 高级勇士
	int8_t achieve_4;							// 一血
	int8_t achieve_5;							// 击杀boss
	Int_Int ability_0;						// 公会祝福 val1 cur val2 max
	Int_Int ability_1;						// 浴火重生
	Int_Int ability_2;						// 激发潜能
	Int_Int ability_3;						// 财神祝福
	int ability_3_coe;						// 财神祝福效果
	int fight_damage;
	role_id_t gang_id;						// 公会id
	int career_rank;
	Time_Value revive_time;
	World_Boss_Rank_Info rank_info;
};
typedef boost::unordered_map<role_id_t, World_Boss_Player_Info> World_Boss_Player_Info_Map;

extern bool World_Boss_Rank_Greater(World_Boss_Player_Info* info1, World_Boss_Player_Info* info2);

struct World_Boss_Gang_Bless{
	World_Boss_Gang_Bless(void);
	~World_Boss_Gang_Bless(void);
	void reset(void);

	int gang_pos;
	role_id_t role_id;
	std::string role_name;
	int bless_num;
};
typedef boost::unordered_map<role_id_t, World_Boss_Gang_Bless> World_Boss_Gang_Bless_Map;

#endif /* WORLD_BOSS_STRUCT_H_ */
