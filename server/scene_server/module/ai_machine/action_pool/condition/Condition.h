/*
 * Condition.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef CONDITION_H_
#define CONDITION_H_

#include "ai_machine/behavior_tree/Behavior_Tree.h"

///  co_是否是死亡状态
class Co_Die_Status : public AI_Task {
public:
	Co_Die_Status(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Die_Status, Co_Die_Status);


///  co_是否是休眠状态
class Co_Dormancy_Status : public AI_Task {
public:
	Co_Dormancy_Status(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Dormancy_Status, Co_Dormancy_Status);


///  co_是否有仇恨对象
class Co_Hatred_List_Not_Null : public AI_Task {
public:
	Co_Hatred_List_Not_Null(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Hatred_List_Not_Null, Co_Hatred_List_Not_Null);


///  co_仇恨列表是否为空
class Co_Hatred_List_Is_Null : public AI_Task {
public:
	Co_Hatred_List_Is_Null(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Hatred_List_Is_Null, Co_Hatred_List_Is_Null);


///  co_血量未满
class Co_HP_Not_Full : public AI_Task {
public:
	Co_HP_Not_Full(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_HP_Not_Full, Co_HP_Not_Full);


///  co_血量是否为空
class Co_HP_Is_Empty : public AI_Task {
public:
	Co_HP_Is_Empty(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_HP_Is_Empty, Co_HP_Is_Empty);


///   co_是否有移动目标
class Co_Has_Move_Target : public AI_Task {
public:
	Co_Has_Move_Target(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Has_Move_Target, Co_Has_Move_Target);

///   co_是否返回状态
class Co_Back_Status : public AI_Task {
public:
	Co_Back_Status(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Back_Status, Co_Back_Status);


///  co_是否空闲状态
class Co_Idle_Status : public AI_Task {
public:
	Co_Idle_Status(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Co_Idle_Status, Co_Idle_Status);

#endif /* CONDITION_H_ */
