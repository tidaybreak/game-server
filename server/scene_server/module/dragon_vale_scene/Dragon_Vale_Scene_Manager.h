/*
 * Dragon_Vale_Scene_Manager.h
 *
 *  Created on: 2014年7月12日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_SCENE_MANAGER_H_
#define DRAGON_VALE_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Move_Scene;
class Dragon_Vale_Scene;

class Dragon_Vale_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<role_id_t, Move_Scene *> Dragon_Vale_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Dragon_Vale_Scene_Map> Muid_Dragon_Vale_Scene_Map;
	typedef Object_Pool_Ex<Dragon_Vale_Scene, NULL_MUTEX> Scene_Pool;

	Dragon_Vale_Scene_Manager();
	virtual ~Dragon_Vale_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	int single_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	Move_Scene* find_scene(role_id_t creater);

private:
	Scene_Pool scene_pool_;
	Muid_Dragon_Vale_Scene_Map scene_map_;
	Dragon_Vale_Scene_Map dragon_vale_scene_map_;
};

typedef Singleton<Dragon_Vale_Scene_Manager> Dragon_Vale_Scene_Manager_Singleton;
#define DRAGON_VALE_SCENE_MANAGER    (Dragon_Vale_Scene_Manager_Singleton::instance())



#endif /* DRAGON_VALE_SCENE_MANAGER_H_ */
