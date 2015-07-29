/*
 * Battle_Behavior.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef BATTLE_BEHAVIOR_H_
#define BATTLE_BEHAVIOR_H_

#include "ai_machine/behavior_tree/Behavior_Tree.h"

///  be_A战斗模式
class Be_Battle_Mode_A : public AI_Task {
public:
	Be_Battle_Mode_A(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc);

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Battle_Mode_A, Be_Battle_Mode_A);


///  be_B战斗模式
class Be_Battle_Mode_B : public AI_Task {
public:
	Be_Battle_Mode_B(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Battle_Mode_B, Be_Battle_Mode_B);

#endif /* BATTLE_BEHAVIOR_H_ */
