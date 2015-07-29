/*
 * Scene_War_Manager.h
 *
 *  Created on: 2014年7月3日
 *      Author: root
 */

#ifndef SCENE_WAR_MANAGER_H_
#define SCENE_WAR_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "boost/unordered_map.hpp"
#include "Scene_War_Struct.h"

struct Monitor_Unique_ID;

class Scene_War_Manager{
public:
	Scene_War_Manager(void);
	virtual ~Scene_War_Manager(void);

public:
	int req_enter_war_scene(Monitor_Unique_ID& logic_muid, role_id_t role_id , int level, int force, role_id_t gang_id,
			int remain_income_num, int income_rate, int total_merit, int remain_time, const int act_time, std::string server_name);
	int req_leave_war_scene(role_id_t role_id);
	int honour_field_finish(role_id_t field_id);
public:
	int join_honour_field(Honour_Player* h_player);
	Honour_Field* genarate_honour_field(void);
	Honour_Player* get_honour_player(role_id_t role_id);
	Honour_Field* get_honour_field(role_id_t field_id);
	Honour_Zone* get_honour_zone(int zone_id);
	void del_honour_player(role_id_t role_id);
	void del_honour_field(role_id_t field_id);
	int get_zone_id_from_level(int level);
	int get_largest_num_honour_zone(int& zone_id, int& player_num);
private:
	Honour_Player_Map honour_player_map_;
	Honour_Field_Map honour_field_map_;
	Honour_Zone_Map honour_zone_map_;
};

typedef Singleton<Scene_War_Manager> Scene_War_Manager_Singleton;
#define SCENE_WAR_MANAGER (Scene_War_Manager_Singleton::instance())


#endif /* SCENE_WAR_MANAGER_H_ */
