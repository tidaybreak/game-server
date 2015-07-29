/*
 * Task_Scene_Manager.h
 *
 *  Created on: Sep 3, 2014
 *      Author: gaojianshan
 */

#ifndef TASK_SCENE_MANAGER_H_
#define TASK_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Move_Scene;
class Task_Scene;

class Task_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<role_id_t, Move_Scene *> Task_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Task_Scene_Map> Muid_Task_Scene_Map;
	typedef Object_Pool_Ex<Task_Scene, NULL_MUTEX> Scene_Pool;

	Task_Scene_Manager();
	virtual ~Task_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	int single_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

private:
	Scene_Pool scene_pool_;
	Muid_Task_Scene_Map scene_map_;
};

typedef Singleton<Task_Scene_Manager> Task_Scene_Manager_Singleton;
#define TASK_SCENE_MANAGER    (Task_Scene_Manager_Singleton::instance())



#endif /* TASK_SCENE_MANAGER_H_ */
