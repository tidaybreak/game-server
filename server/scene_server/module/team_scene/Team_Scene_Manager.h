/*
 * Team_Scene_Manager.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef TEAM_SCENE_MANAGER_H_
#define TEAM_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Move_Scene;
class Team_Scene;

class Team_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<role_id_t, Move_Scene *> Role_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Role_Scene_Map> Muid_Role_Scene_Map;
	typedef Object_Pool_Ex<Team_Scene, NULL_MUTEX> Scene_Pool;

	Team_Scene_Manager();
	virtual ~Team_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	int single_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

private:
	Scene_Pool scene_pool_;
	Muid_Role_Scene_Map scene_map_;
};

typedef Singleton<Team_Scene_Manager> Team_Scene_Manager_Team;
#define TEAM_SCENE_MANAGER    (Team_Scene_Manager_Team::instance())

#endif /* TEAM_SCENE_MANAGER_H_ */
