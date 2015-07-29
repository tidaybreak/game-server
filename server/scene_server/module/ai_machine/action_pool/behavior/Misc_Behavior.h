/*
 * Misc_Behavior.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef MISC_BEHAVIOR_H_
#define MISC_BEHAVIOR_H_

#include "ai_machine/behavior_tree/Behavior_Tree.h"

///  空闲
class Be_Idle : public AI_Task {
public:
	Be_Idle(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Idle, Be_Idle);


/// be_回收尸体
class Be_NPC_Recover : public AI_Task {
public:
	Be_NPC_Recover(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_NPC_Recover, Be_NPC_Recover);


/// be_设置死亡
class Be_NPC_Die : public AI_Task {
public:
	Be_NPC_Die(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_NPC_Die, Be_NPC_Die);


///  be_恢复满血
class Be_Hp_Full : public AI_Task {
public:
	Be_Hp_Full(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Hp_Full, Be_Hp_Full);


///  be_清空仇恨列表
class Be_Clean_Hatred_List : public AI_Task {
public:
	Be_Clean_Hatred_List(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Clean_Hatred_List, Be_Clean_Hatred_List);


///   be_清空巡逻路线
class Be_Clean_Walk_Path : public AI_Task {
public:
	Be_Clean_Walk_Path(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Clean_Walk_Path, Be_Clean_Walk_Path);


///  be_设置为休眠状态
class Be_Set_Dormancy_Status : public AI_Task {
public:
	Be_Set_Dormancy_Status(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Set_Dormancy_Status, Be_Set_Dormancy_Status);


///  be_延时到下次复活时间复活
class Be_Delay_Until_Next_Resurrection_Time_Resurrection : public AI_Task {
public:
	Be_Delay_Until_Next_Resurrection_Time_Resurrection(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Delay_Until_Next_Resurrection_Time_Resurrection, Be_Delay_Until_Next_Resurrection_Time_Resurrection);


///  be_解除休眠满状态复活
class Be_Dormancy_Breaking_And_Full_State : public AI_Task {
public:
	Be_Dormancy_Breaking_And_Full_State(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Dormancy_Breaking_And_Full_State, Be_Dormancy_Breaking_And_Full_State);


///  be_采集物模式
class Be_Material_Mode : public AI_Task {
public:
	Be_Material_Mode(AI_Node *pNode) : AI_Task(pNode) {}
	virtual void on_init(NPC *npc) {}
	virtual void on_terminate(NPC *npc) {}

	virtual Bev_Running_Status on_update(NPC *npc);
};
DEF_TERMINATE_NODE(Be_Material_Mode, Be_Material_Mode);

#endif /* MISC_BEHAVIOR_H_ */
