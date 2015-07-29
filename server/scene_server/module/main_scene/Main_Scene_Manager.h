/*
 * Main_Scene_Manager.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef MAIN_SCENE_MANAGER_H_
#define MAIN_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Move_Scene;
class Main_Scene;

class Main_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<Monitor_Unique_ID, Move_Scene *> Scene_Map;
	typedef Object_Pool_Ex<Main_Scene, NULL_MUTEX> Scene_Pool;

	Main_Scene_Manager();
	virtual ~Main_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	int single_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

private:
	Scene_Map scene_map_;
	Scene_Pool scene_pool_;
};

typedef Singleton<Main_Scene_Manager> Normal_Scene_Manager_Single;
#define MAIN_SCENE_MANAGER    (Normal_Scene_Manager_Single::instance())

#endif /* MAIN_SCENE_MANAGER_H_ */
