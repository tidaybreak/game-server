/*
 * Scene_Outpost_Manager.h
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */

#ifndef SCENE_OUTPOST_MANAGER_H_
#define SCENE_OUTPOST_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "secret_shop/Secret_Shop_Struct.h"

class Move_Scene;

class Scene_Outpost_Manager{
public:
	Scene_Outpost_Manager(void);
	~Scene_Outpost_Manager(void);
public:
	void reset(void);
public:
	int add_new_monster_point(Move_Scene* scene, const int point, bool is_boss, Coord point_coord);
	int clear_vagrant_store(const int scene_id);
public:
	int inner_sync_info(Scene_Player* player, Block_Buffer& buf);
	// 获取流浪商人物品
	int req_vagrant_item_info(Scene_Player* player, const int64_t monster_role_id);
	// 请求流浪商人物品信息
	int req_vagrant_buy_item(Scene_Player* player, const int64_t monster_role_id, const int item_index);
private:
	Vagrant_Item_Info* get_vagrant_item_info(const int scene_id, const int64_t monster_role_id, const int item_index);
    int vagrant_store_trigger(Move_Scene* scene, const int point, bool is_boss, Coord point_coord);
    int tbay_trigger(Move_Scene* scene, const int point, bool is_boss, Coord point_coord);
private:
	Vagrant_Store_Scene_Map vagrant_item_map_;					// 流浪商人物品
};


typedef Singleton<Scene_Outpost_Manager> Scene_Outpost_Manager_Single;
#define SCENE_OUTPOST_MANAGER (Scene_Outpost_Manager_Single::instance())

#endif /* SCENE_OUTPOST_MANAGER_H_ */
