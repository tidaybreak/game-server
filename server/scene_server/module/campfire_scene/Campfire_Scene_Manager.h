/*
 * Campfire_Scene_Manager.h
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#ifndef CAMPFIRE_SCENE_MANAGER_H_
#define CAMPFIRE_SCENE_MANAGER_H_


#include "Scene_Manager.h"
#include "Msg_Campfire_Struct.h"

class Move_Scene;
class Campfire_Scene;

class Campfire_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<Monitor_Unique_ID, Move_Scene *> Scene_Map;
	typedef Object_Pool_Ex<Campfire_Scene, NULL_MUTEX> Scene_Pool;

	Campfire_Scene_Manager();
	virtual ~Campfire_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	int send_campfire_personal_info(Scene_Player* player);
	int player_req_battle(Scene_Player* player, role_id_t monster_id);
private:
	Scene_Map scene_map_;
	Scene_Pool scene_pool_;
};

typedef Singleton<Campfire_Scene_Manager> Campfire_Scene_Manager_Single;
#define CAMPFIRE_SCENE_MANAGER    (Campfire_Scene_Manager_Single::instance())



#endif /* CAMPFIRE_SCENE_MANAGER_H_ */
