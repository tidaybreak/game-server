/*
 * War_Scene_Manager.h
 *
 *  Created on: 2014年7月1日
 *      Author: root
 */

#ifndef WAR_SCENE_MANAGER_H_
#define WAR_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class War_Scene;

class War_Scene_Manager: public Scene_Manager{
public:
	typedef boost::unordered_set<role_id_t> Order_Set;
	typedef boost::unordered_map<role_id_t, Move_Scene *> Role_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Role_Scene_Map> Muid_Role_Scene_Map;
	typedef Object_Pool_Ex<War_Scene, NULL_MUTEX> Scene_Pool;
public:
	War_Scene_Manager(void);

	virtual ~War_Scene_Manager(void);
public:
	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

public:
	role_id_t genarate_war_scene_order(void);
private:
	Scene_Pool scene_pool_;
	Muid_Role_Scene_Map scene_map_;
	Order_Set order_set_;
	role_id_t war_scene_order_;
};


typedef Singleton<War_Scene_Manager> War_Scene_Manager_Singleton;
#define WAR_SCENE_MANAGER    (War_Scene_Manager_Singleton::instance())

#endif /* WAR_SCENE_MANAGER_H_ */
