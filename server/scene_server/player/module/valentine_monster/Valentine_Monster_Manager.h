/*
 * Valentine_Monster_Manager.h
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#ifndef VALENTINE_MONSTER_MANAGER_H_
#define VALENTINE_MONSTER_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "Monitor_Struct.h"
#include "valentine_monster/Valentine_Monster_Struct.h"

class Battle_Scene;

class Valentine_Monster_Manager{
	typedef boost::unordered_map<int, Valentine_Monster_Info> Main_Scene_Map;
	typedef boost::unordered_map<int, int> Scene_Pt_Amount_Map;
	typedef boost::unordered_map<int, Time_Value> Boss_End_Time_Map;
public:
	Valentine_Monster_Manager(void);
	~Valentine_Monster_Manager(void);
public:
	void reset(void);
	void tick(const Time_Value& now);
	void be_hurt_and_sub_blood(const int scene_id, Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id);
public:
	int inner_sync_info(Scene_Player* player, Block_Buffer& buf);
	int inner_act_start(const role_id_t gang_id, Block_Buffer& buf);
	int inner_act_end(const role_id_t gang_id, Block_Buffer& buf);
private:
	void act_start_add_monster(const int boss_tid);
	void act_end_del_monster(void);
private:
	bool act_open_;
	Time_Value act_end_time_;
	Main_Scene_Map main_scene_map_;
};


typedef Singleton<Valentine_Monster_Manager> Valentine_Monster_Manager_Single;
#define VALENTINE_MONSTER_MANAGER (Valentine_Monster_Manager_Single::instance())

#endif /* VALENTINE_MONSTER_MANAGER_H_ */
