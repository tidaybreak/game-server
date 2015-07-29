/*
 * Arena_Scene_Manager.h
 *
 *  Created on: 15 2 2015
 *      Author: zhoujin
 */

#ifndef ARENA_SCENE_MANAGER_H_
#define ARENA_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Move_Scene;
class Arena_Scene;

class Arena_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<Monitor_Unique_ID, Move_Scene *> Scene_Map;
	typedef Object_Pool_Ex<Arena_Scene, NULL_MUTEX> Scene_Pool;

	Arena_Scene_Manager();
	virtual ~Arena_Scene_Manager();

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

typedef Singleton<Arena_Scene_Manager> Arena_Scene_Manager_Single;
#define ARENA_SCENE_MANAGER    (Arena_Scene_Manager_Single::instance())

#endif /* MAIN_SCENE_MANAGER_H_ */
