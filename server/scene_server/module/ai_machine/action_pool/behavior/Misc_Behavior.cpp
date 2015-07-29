/*
 * Misc_Behavior.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#include "ai_machine/action_pool/behavior/Misc_Behavior.h"
#include "NPC/NPC_Manager.h"
//#include "Map_Manager.h"
//#include "Map_Player.h"

///  空闲
Bev_Running_Status Be_Idle::on_update(NPC *npc) {
	int time = 3+random()%3;
	npc->push_watcher_tick(time);
	if(npc->monster_self()){
		npc->monster_self()->play_chat();
	}
	return k_BRS_SUCCESS;
}


/// be_回收尸体
Bev_Running_Status Be_NPC_Recover::on_update(NPC *npc) {
	npc->recover_self();

	return k_BRS_SUCCESS;
}


/// be_设置死亡
Bev_Running_Status Be_NPC_Die::on_update(NPC *npc) {
	npc->normal_die_self();

	return k_BRS_SUCCESS;
}


///  be_恢复满血
Bev_Running_Status Be_Hp_Full::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	monster->restore_full_status();

	return k_BRS_SUCCESS;
}


///  be_清空仇恨列表
Bev_Running_Status Be_Clean_Hatred_List::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	monster->hatred_map().clear();
//	monster->most_hatred_role().reset();

	return k_BRS_SUCCESS;
}

///   be_清空巡逻路径
Bev_Running_Status Be_Clean_Walk_Path::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();

	monster->walk_path().reset();

	return k_BRS_SUCCESS;
}


///  be_解除休眠满状态复活
Bev_Running_Status Be_Dormancy_Breaking_And_Full_State::on_update(NPC *npc) {
//	if (npc->is_monster()) {
//		Monster *monster = npc->monster_self();
//		if (monster->dormancy_state()) {
//			Grid_Coord coord = monster->birth_coord();
//			NPC_Addition_Info npc_addition_info;
//			npc_addition_info.birth_coord = coord;
//			monster->init(monster->npc_type_id(), monster->npc_record(), npc_addition_info);
//
//			monster->appear();
//
//			monster->enter_scene();
//
//			monster->insert_default_buff();
//
//			monster->appear_broadcasting();
//
//			monster->push_watcher_tick(0, 100000);
//		} else {
//			MSG_USER("AI: monster:%ld type:%d dormancy breaking error... ", monster->npc_id(), monster->npc_type_id());
//		}
//	} else if (npc->is_material()) {
//		Material *material = npc->material_self();
//		if (material->dormancy_state()) {
//			material->appear();
//			material->set_npc_state(NPC::ALIVE_STATUS);
//		}
//	}

	return k_BRS_SUCCESS;
}


///  be_设置为休眠状态
Bev_Running_Status Be_Set_Dormancy_Status::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	monster->move_disappear();
//
//	role_id_t npc_id = npc->npc_id();
//	int npc_type_id = npc->npc_type_id();
//	Grid_Coord birth_coord = monster->birth_coord();
//	const AI_Config_Cache *ai_config =npc->npc_detail().ai_config;
//	NPC_Record *npc_record =npc->npc_record();
//	Scene *scene = npc->scene_s();
//
//	monster->reset();
//	monster->npc_detail().birth_coord = birth_coord;
//	monster->npc_detail().ai_config = ai_config;
//	monster->npc_detail().npc_type_id = npc_type_id;
//	monster->set_scene_pointer(scene);
//	monster->set_record(npc_record);
//	monster->set_npc_state(NPC::DORMANCY_STATUS);
//
//	// 更换id 客户端要延迟死亡
//	NPC_MANAGER->monster_map().erase(npc_id);
//	monster->npc_detail().npc_id = NPC_MANAGER->generate_monster_order();
//	NPC_MANAGER->monster_map()[monster->npc_id()] = monster;
//
//	// 重新插进record的alive_map
//	NPC_Record::NPC_Type_Map::iterator npc_find_it = npc_record->npc_type_map.find(npc_type_id);
//	if (npc_find_it == npc_record->npc_type_map.end()) {
//		npc_record->npc_type_map.insert(std::make_pair(npc_type_id, NPC_Record_Info()));
//		npc_find_it = npc_record->npc_type_map.find(npc_type_id);
//	}
//
//	if (npc_find_it != npc_record->npc_type_map.end()) {
//		npc_find_it->second.alive_npc_map[npc_id] = monster;
//	}

	return k_BRS_SUCCESS;
}

///  be_延时到下次复活时间复活
Bev_Running_Status Be_Delay_Until_Next_Resurrection_Time_Resurrection::on_update(NPC *npc) {
//	// 假死也要退出场景
//	// npc->exit_scene();
//	long delay_time = get_next_tick_time(npc->ai_config().date_appear);
//	if (delay_time > 0) {
//		Time_Value tv;
//		npc->pop_watcher_tick(tv);
//		npc->push_watcher_tick(delay_time/1000, (delay_time%1000) * 1000);
//	} else {
//		npc->push_watcher_tick(0, 200000);
//		MSG_USER("AI: boss:%ld sleep error, sleep long long ... 10000 %ld", npc->npc_type_id(), delay_time);
//	}

	return k_BRS_SUCCESS;
}


///  be_采集物模式
Bev_Running_Status Be_Material_Mode::on_update(NPC *npc) {
//	if (npc->ai_config().date_appear.no_set) {
//		npc->recover_self();
//	} else {
//		long delay_time = get_next_tick_time(npc->ai_config().date_appear);
//		bool insert_section = true;
//		if (delay_time == 0) {
//			npc->set_npc_state(NPC::ALIVE_STATUS);
//			npc->stop_ai();
//		} else if (delay_time > 0) {
//			npc->move_disappear();
//			npc->set_npc_state(NPC::DORMANCY_STATUS);
//
//			Time_Value tv;
//			npc->pop_watcher_tick(tv);
//			npc->push_watcher_tick(delay_time/1000, (delay_time%1000) * 1000);
//		} else {
//			insert_section = false;
//			MSG_USER("material:%ld sleep error, time:%ld", npc->npc_id(), delay_time);
//		}
//
//		if (insert_section) {
//			Material *material = npc->material_self();
//			if (material) {
//				// 重新插进record的alive_map
//				NPC_Record::NPC_Type_Map::iterator npc_find_it = material->npc_record()->npc_type_map.find(npc->npc_type_id());
//				if (npc_find_it == material->npc_record()->npc_type_map.end()) {
//					material->npc_record()->npc_type_map.insert(std::make_pair(npc->npc_type_id(), NPC_Record_Info()));
//					npc_find_it = material->npc_record()->npc_type_map.find(npc->npc_type_id());
//				}
//
//				if (npc_find_it != material->npc_record()->npc_type_map.end()) {
//					npc_find_it->second.alive_npc_map[material->role_id()] = material;
//				}
//			}
//		}
//	}

	return k_BRS_SUCCESS;
}
