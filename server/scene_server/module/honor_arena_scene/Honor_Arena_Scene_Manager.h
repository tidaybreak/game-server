/*
 * Honor_Arena_Scene_Manager.h
 *
 *  Created on: 2015年3月16日
 *      Author: root
 */

#ifndef HONOR_ARENA_SCENE_MANAGER_H_
#define HONOR_ARENA_SCENE_MANAGER_H_


#include "Scene_Manager.h"

class Honor_Arena_Scene;

class Honor_Arena_Scene_Manager: public Scene_Manager{
	typedef boost::unordered_map<Monitor_Unique_ID, Move_Scene *> Scene_Map;
	typedef Object_Pool_Ex<Honor_Arena_Scene, NULL_MUTEX> Scene_Pool;
public:

	Honor_Arena_Scene_Manager(void);

	virtual ~Honor_Arena_Scene_Manager(void);
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

typedef Singleton<Honor_Arena_Scene_Manager> Honor_Arena_Scene_Manager_Single;
#define HONOR_ARENA_SCENE_MANAGER    (Honor_Arena_Scene_Manager_Single::instance())



#endif /* HONOR_ARENA_SCENE_MANAGER_H_ */
