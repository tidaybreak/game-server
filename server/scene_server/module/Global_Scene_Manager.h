/*
 * Global_Scene_Manager.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef GLOBAL_SCENE_MANAGER_H_
#define GLOBAL_SCENE_MANAGER_H_

#include "Scene_Manager.h"

class Mover;
class Battle_Scene;
class Module_Manager;

class Global_Scene_Manager {
public:
	typedef boost::unordered_set<Mover *> Battle_Scene_Mover_Set;
	typedef boost::unordered_set<Monitor_Unique_ID> Scene_Set;
	typedef boost::unordered_set<Scene_Manager *> Scene_Manager_Set;
	typedef boost::unordered_set<Module_Manager *> Module_Manager_Set;
	typedef boost::unordered_map<Monitor_Unique_ID, Scene_Manager *> Muid_Scene_Manager_Map;

	typedef boost::unordered_map<long_id_t, Battle_Scene *> Battle_Scene_Map;

	Global_Scene_Manager();
	virtual ~Global_Scene_Manager();

	int tick(void);

	void module_tick(Time_Value &now);

	int init_global_scene(int idx);

	int show_scene_status(void);

	Scene_Manager *get_scene_manager(Monitor_Unique_ID &muid);

	role_id_t generate_battle_order(void);
	Battle_Scene *create_battle_scene(Move_Scene* move_scene);
	Battle_Scene *find_battle_scene(long_id_t battle_id);
	int destroy_battle_scene(long_id_t battle_id);
	void push_destroy_scene(Battle_Scene* battle_scene);;

	void insert_mover(Mover *mover);
	void remove_mover(Mover *mover);

private:
	Scene_Set scene_set_;
	Scene_Manager_Set scene_manager_set_;
	Muid_Scene_Manager_Map muid_manager_map_;

	long_id_t battle_order_;
	Battle_Scene_Map battle_scene_map_;
	Long_Id_Set destroy_scene_id_set_;
	Module_Manager_Set module_manager_tick_;		// 各模块tick, 根据相应场景设置

	Battle_Scene_Mover_Set all_battle_mover_set_;
};

typedef Singleton<Global_Scene_Manager> Global_Scene_Manager_Single;
#define GLOBAL_SCENE_MANAGER    (Global_Scene_Manager_Single::instance())

#endif /* GLOBAL_SCENE_MANAGER_H_ */
