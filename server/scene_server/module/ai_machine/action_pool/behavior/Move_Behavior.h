/*
 * Move_Behavior.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef MOVE_BEHAVIOR_H_
#define MOVE_BEHAVIOR_H_

#include "ai_machine/behavior_tree/Behavior_Tree.h"

///   be_A移动模式	[随机移动]
class Be_Move_Mode_A : public AI_Task {
public:
	Be_Move_Mode_A(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Move_Mode_A, Be_Move_Mode_A);


///   be_B移动模式	[循环移动]
class Be_Move_Mode_B : public AI_Task {
public:
	Be_Move_Mode_B(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Move_Mode_B, Be_Move_Mode_B);


///   be_D移动模式	[定向移动]
class Be_Move_Mode_D : public AI_Task {
public:
	Be_Move_Mode_D(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Move_Mode_D, Be_Move_Mode_D);


///  be_A巡逻模式
class Be_Patrol_Mode_A : public AI_Task {
public:
	Be_Patrol_Mode_A(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Patrol_Mode_A, Be_Patrol_Mode_A);

///  be_追击
class Be_Chase_Mode : public AI_Task {
public:
	Be_Chase_Mode(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Chase_Mode, Be_Chase_Mode);

///  be_返回
class Be_Back_Mode : public AI_Task {
public:
	Be_Back_Mode(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Back_Mode, Be_Back_Mode);


///  be_宠物模式
class Be_Pet_Mode : public AI_Task {
public:
	Be_Pet_Mode(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Pet_Mode, Be_Pet_Mode);

#endif /* MOVE_BEHAVIOR_H_ */
