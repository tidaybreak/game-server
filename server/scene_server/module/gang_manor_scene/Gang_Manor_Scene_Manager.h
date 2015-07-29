/*
 * Gang_Manor_Scene_Manager.h
 *
 *  Created on: Jun 26, 2014
 *      Author: chenyulin
 */

#ifndef GANG_MANOR_SCENE_MANAGER_H_
#define GANG_MANOR_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Move_Scene;
class Gang_Manor_Scene;

class Gang_Manor_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<gang_id_t, Move_Scene *> Gang_Manor_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Gang_Manor_Scene_Map> Muid_Gang_Manor_Scene_Map;
	typedef Object_Pool_Ex<Gang_Manor_Scene, NULL_MUTEX> Scene_Pool;

	Gang_Manor_Scene_Manager();
	virtual ~Gang_Manor_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

private:
	Scene_Pool scene_pool_;
	Muid_Gang_Manor_Scene_Map scene_map_;
};

typedef Singleton<Gang_Manor_Scene_Manager> Gang_Manor_Scene_Manager_Single;
#define GANG_MANOR_MANAGER    (Gang_Manor_Scene_Manager_Single::instance())


#endif /* GANG_MANOR_SCENE_MANAGER_H_ */
