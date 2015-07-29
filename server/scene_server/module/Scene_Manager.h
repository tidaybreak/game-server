/*
 * Scene_Manager.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "Server_Struct.h"
#include "Monitor_Struct.h"

class Move_Scene;

class Scene_Manager {
public:
	Scene_Manager();
	virtual ~Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid) = 0;

	virtual int tick(Time_Value &now) = 0;

	virtual void show_scene_status(void) = 0;

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) = 0;

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa) = 0;

};

#endif /* SCENE_MANAGER_H_ */
