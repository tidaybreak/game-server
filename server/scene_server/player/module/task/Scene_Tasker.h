/*
 * Tasker.h
 *
 *  Created on: Apr 11, 2014
 *      Author: Linqiyou
 */

#ifndef SCENE_TASKER_H_
#define SCENE_TASKER_H_

#include "Player_Base.h"
#include "task/Task_Def.h"

class Scene_Tasker {
public:
	typedef boost::unordered_set<int> TaskSet;
	typedef boost::unordered_set<int> TaskIdSet;
//	typedef boost::unordered_set<int> ItemIdSet;
//	typedef Hash_Map<int, ItemIdSet, NULL_MUTEX> Npc_ItemIdSet_Map; //key: npc_id  value:ItemIdSet

	Scene_Tasker();
	virtual ~Scene_Tasker();

//	void load_detail(Block_Buffer &buf);
	// 登录时detail赋值
	void load_scene_detail(Block_Buffer &buf);

/*	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);*/

    void reset(void);

    int add_source_to_item(int npc_id, const Item_Chance &item_chance);
    int erase_source_to_item(int npc_id, const Item_Chance &item_chance);

//    int add_task_accept(int task_id);
//    int erase_task_accept(int task_id);

    Int_Int_Vec get_item_list(const int npc_id);  //收集打怪掉落物品接口
    bool is_task_exist(Scene_Player *player, const int task_id);    //采集任务物品，检查任务是否存在
protected:
    TaskIdSet __task_id_set;                       //采集物品的任务的id集合(已接任务)
    Npc_Item_Chance_Set_Map __npc_ItemIdSet_map;	       //收集打怪获得掉落物品的Npc_ItemIdSet_Map
//    Role_Id_Set npc_id_;
//    Int_Uid npc_type_;
};

#endif /* SCENE_TASKER_H_ */
