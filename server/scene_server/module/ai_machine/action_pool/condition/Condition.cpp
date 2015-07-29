/*
 * Condition.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#include "Condition.h"
#include "NPC/NPC_Manager.h"
//#include "Move_Scene.h"

Bev_Running_Status Co_Die_Status::on_update(NPC *npc) {
	if (npc->npc_dead_state())
		return k_BRS_SUCCESS;

	return k_BRS_Failure;
}


///  co_是否是休眠状态
Bev_Running_Status Co_Dormancy_Status::on_update(NPC *npc) {
	if (npc->dormancy_state())
		return k_BRS_SUCCESS;

	return k_BRS_Failure;
}


Bev_Running_Status Co_Hatred_List_Not_Null::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();

	if (monster->most_hatred_role().role_id == 0)
	{
		return k_BRS_Failure;
	}
	return k_BRS_SUCCESS;
}


///  co_仇恨列表是否为空
Bev_Running_Status Co_Hatred_List_Is_Null::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	if (monster->hatred_map().empty())
//		return k_BRS_SUCCESS;

	return k_BRS_Failure;
}


///  co_血量未满
Bev_Running_Status Co_HP_Not_Full::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	if (!monster->hp_is_full())
//		return k_BRS_SUCCESS;

	return k_BRS_Failure;
}


///  co_血量是否为空
Bev_Running_Status Co_HP_Is_Empty::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	if (!monster->hp_is_full())
//		return k_BRS_SUCCESS;

	return k_BRS_Failure;
}


///  co_是否有移动目标
Bev_Running_Status Co_Has_Move_Target::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();

	if (monster->walk_path().has_walk_path())
		return k_BRS_SUCCESS;

	return k_BRS_Failure;
}

///  co_是否返回状态
Bev_Running_Status Co_Back_Status::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();
	if (monster->npc_status() == NPC::BACK_STATUS)
	{
		return k_BRS_SUCCESS;
	}
	return k_BRS_Failure;
}


///  co_是否空闲状态
Bev_Running_Status Co_Idle_Status::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();
	if (monster->npc_status() != NPC::FIGHT_STATUS &&
			monster->npc_status() != NPC::RECOVER_STATUS)
	{
		return k_BRS_SUCCESS;
	}
	return k_BRS_Failure;
}


