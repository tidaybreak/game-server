/*
 * Killing_Scene_Manager.h
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */

#ifndef KILLING_SCENE_MANAGER_H_
#define KILLING_SCENE_MANAGER_H_


#include "Scene_Manager.h"

class Move_Scene;
class Killing_Scene;

class Killing_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<role_id_t, Move_Scene *> Role_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Role_Scene_Map> Muid_Role_Scene_Map;
	typedef Object_Pool_Ex<Killing_Scene, NULL_MUTEX> Scene_Pool;

	Killing_Scene_Manager();
	virtual ~Killing_Scene_Manager();
public:

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

private:
	Scene_Pool scene_pool_;
	Muid_Role_Scene_Map scene_map_;
};

typedef Singleton<Killing_Scene_Manager> Killing_Scene_Manager_Singleton;
#define KILLING_SCENE_MANAGER    (Killing_Scene_Manager_Singleton::instance())


#endif /* KILLING_SCENE_MANAGER_H_ */
