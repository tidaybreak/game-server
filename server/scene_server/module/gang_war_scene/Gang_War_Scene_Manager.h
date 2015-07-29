/*
 * Gang_War_Scene_Manager.h
 *
 *  Created on: 2014年8月1日
 *      Author: Linqiyou
 */

#ifndef GANG_WAR_SCENE_MANAGER_H_
#define GANG_WAR_SCENE_MANAGER_H_


#include "Scene_Manager.h"

class Gang_War_Scene;

class Gang_War_Scene_Manager: public Scene_Manager{
public:
	typedef boost::unordered_set<role_id_t> Order_Set;
	typedef boost::unordered_map<role_id_t, Move_Scene *> Role_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Role_Scene_Map> Muid_Role_Scene_Map;
	typedef Object_Pool_Ex<Gang_War_Scene, NULL_MUTEX> Scene_Pool;
public:
	Gang_War_Scene_Manager(void);

	virtual ~Gang_War_Scene_Manager(void);
public:
	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual void close_all_gang_war_scene(const Role_Hash_Set &scene_ids);

public:
//	role_id_t genarate_scene_order(void);
private:
	Scene_Pool scene_pool_;
	Muid_Role_Scene_Map scene_map_;
	Order_Set order_set_;
//	role_id_t gang_war_scene_order_;
};


typedef Singleton<Gang_War_Scene_Manager> Gang_War_Scene_Manager_Singleton;
#define GANG_WAR_SCENE_MANAGER    (Gang_War_Scene_Manager_Singleton::instance())


#endif /* GANG_WAR_SCENE_MANAGER_H_ */
