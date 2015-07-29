/*
 * Gang_Boss_Scene_Manager.h
 *
 *  Created on: 2015年2月7日
 *      Author: root
 */

#ifndef GANG_BOSS_SCENE_MANAGER_H_
#define GANG_BOSS_SCENE_MANAGER_H_


#include "Scene_Manager.h"

class Gang_Boss_Scene;

class Gang_Boss_Scene_Manager: public Scene_Manager{
	typedef boost::unordered_map<role_id_t, Move_Scene *> Role_Scene_Map;
	typedef boost::unordered_map<Monitor_Unique_ID, Role_Scene_Map> Muid_Role_Scene_Map;
	typedef Object_Pool_Ex<Gang_Boss_Scene, NULL_MUTEX> Scene_Pool;
public:

	Gang_Boss_Scene_Manager(void);

	virtual ~Gang_Boss_Scene_Manager(void);
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

typedef Singleton<Gang_Boss_Scene_Manager> Gang_Boss_Scene_Manager_Single;
#define GANG_BOSS_SCENE_MANAGER    (Gang_Boss_Scene_Manager_Single::instance())



#endif /* GANG_BOSS_SCENE_MANAGER_H_ */
