/*
 * World_Boss_Scene_Manager.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef WORLD_BOSS_SCENE_MANAGER_H_
#define WORLD_BOSS_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class World_Boss_Scene;

class World_Boss_Scene_Manager: public Scene_Manager{
	typedef boost::unordered_map<Monitor_Unique_ID, Move_Scene *> Scene_Map;
	typedef Object_Pool_Ex<World_Boss_Scene, NULL_MUTEX> Scene_Pool;
public:

	World_Boss_Scene_Manager(void);

	virtual ~World_Boss_Scene_Manager(void);
public:

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);
private:
	Scene_Map scene_map_;
	Scene_Pool scene_pool_;
};

typedef Singleton<World_Boss_Scene_Manager> World_Boss_Scene_Manager_Single;
#define WORLD_BOSS_SCENE_MANAGER    (World_Boss_Scene_Manager_Single::instance())


#endif /* WORLD_BOSS_SCENE_MANAGER_H_ */
