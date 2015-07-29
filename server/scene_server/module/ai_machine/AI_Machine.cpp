/*
 * AI_Machine.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#include "AI_Machine.h"
#include "NPC/NPC_Manager.h"
#include "ai_machine/action_pool/AI_Action_Pool.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "ai_machine/action_pool/condition/Condition.h"
#include "ai_machine/action_pool/behavior/Move_Behavior.h"
#include "ai_machine/action_pool/behavior/Misc_Behavior.h"
#include "ai_machine/action_pool/behavior/Battle_Behavior.h"
#include "Move_Scene.h"



AI_Machine::AI_Machine(void) {
	init();
}

AI_Machine::~AI_Machine(void) { }

int AI_Machine::behavior_init(Json::Value &node_json, Composite_Node* parent_node)
{
	const Json::Value &ai_action_pool = CONFIG_INSTANCE->config_json()["ai"]["ai_pool"];
	if (ai_action_pool == Json::Value::null) {
		MSG_USER("configure file error.");
	}

	for (Json::Value::iterator iter = node_json.begin(); iter != node_json.end(); ++iter) {
		std::string node = (*iter)["node"].asString();
		std::string type = (*iter)["type"].asString();

		Composite_Node* this_node = NULL;
		if (type == "Selector") {
			this_node = new CompositeNode_Selector();
		} else if (type == "Sequence") {
			this_node = new CompositeNode_Sequence();
		} else if (type == "Parallel") {
			this_node = new CompositeNode_Parallel();
		} else if (type == "Condition") {
			if (ai_action_pool[node].asString() == "Co_Die_Status")
				parent_node->add_child(CREATE_TERMINATE_NODE(Co_Die_Status));
//			else if (ai_action_pool[node].asString() == "Co_Dormancy_Status")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Co_Dormancy_Status));
			else if (ai_action_pool[node].asString() == "Co_Back_Status")
				parent_node->add_child(CREATE_TERMINATE_NODE(Co_Back_Status));
			else if (ai_action_pool[node].asString() == "Co_Hatred_List_Not_Null")
				parent_node->add_child(CREATE_TERMINATE_NODE(Co_Hatred_List_Not_Null));
//			else if (ai_action_pool[node].asString() == "Co_HP_Not_Full")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Co_HP_Not_Full));
//			else if (ai_action_pool[node].asString() == "Co_HP_Is_Empty")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Co_HP_Is_Empty));
//			else if (ai_action_pool[node].asString() == "Co_Has_Move_Target")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Co_Has_Move_Target));
//			else if (ai_action_pool[node].asString() == "Co_Hatred_List_Is_Null")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Co_Hatred_List_Is_Null));
			else
				MSG_USER("AI:no condition:%s", node.c_str());
		} else if (type == "Behavior") {
			if (ai_action_pool[node].asString() == "Be_Idle")
				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Idle));
//			else if (ai_action_pool[node].asString() == "Be_NPC_Recover")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_NPC_Recover));
//			else if (ai_action_pool[node].asString() == "Be_Battle_Mode_A")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Battle_Mode_A));
//			else if (ai_action_pool[node].asString() == "Be_Battle_Mode_B")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Battle_Mode_B));
			else if (ai_action_pool[node].asString() == "Be_Patrol_Mode_A")
				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Patrol_Mode_A));
			else if (ai_action_pool[node].asString() == "Be_Chase_Mode")
				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Chase_Mode));
			else if (ai_action_pool[node].asString() == "Be_Back_Mode")
				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Back_Mode));
//			else if (ai_action_pool[node].asString() == "Be_Pet_Mode")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Pet_Mode));
//			else if (ai_action_pool[node].asString() == "Be_Hp_Full")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Hp_Full));
//			else if (ai_action_pool[node].asString() == "Be_Clean_Hatred_List")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Clean_Hatred_List));
//			else if (ai_action_pool[node].asString() == "Be_Clean_Walk_Path")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Clean_Walk_Path));
//			else if (ai_action_pool[node].asString() == "Be_Dormancy_Breaking_And_Full_State")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Dormancy_Breaking_And_Full_State));
//			else if (ai_action_pool[node].asString() == "Be_Set_Dormancy_Status")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Set_Dormancy_Status));
			else if (ai_action_pool[node].asString() == "Be_NPC_Die")
				parent_node->add_child(CREATE_TERMINATE_NODE(Be_NPC_Die));
//			else if (ai_action_pool[node].asString() == "Be_Delay_Until_Next_Resurrection_Time_Resurrection")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Delay_Until_Next_Resurrection_Time_Resurrection));
//			else if (ai_action_pool[node].asString() == "Be_Material_Mode")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Material_Mode));
//			else if (ai_action_pool[node].asString() == "Be_Move_Mode_A")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Move_Mode_A));
			else if (ai_action_pool[node].asString() == "Be_Move_Mode_B")
				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Move_Mode_B));
//			else if (ai_action_pool[node].asString() == "Be_Move_Mode_D")
//				parent_node->add_child(CREATE_TERMINATE_NODE(Be_Move_Mode_D));

			else
				MSG_USER("AI:no behavior:%s", node.c_str());
		}

		if (this_node != NULL) {
			parent_node->add_child(this_node);
		}

		if ((*iter)["child_node"] != Json::Value::null)
			behavior_init((*iter)["child_node"], this_node);
	}

	return 0;
}

void AI_Machine::init(void) {

	const Json::Value &all_ai_json = CONFIG_INSTANCE->config_json()["ai"]["ai_behavior"];
	if (all_ai_json == Json::Value::null) {
		LOG_ABORT("configure file error.");
	}

	for (Json::Value::iterator iter = all_ai_json.begin(); iter != all_ai_json.end(); ++iter) {
		int ai_id = atoi(iter.key().asCString());
		AI_Data *ai_data = new AI_Data;
		Composite_Node *ai_behavior = new CompositeNode_Selector();
		ai_data->ai_behavior = ai_behavior;
		behavior_init(*iter, ai_behavior);
		ai_data_map_.insert(std::make_pair(ai_id, ai_data));
	}

	{
		Composite_Node* pselect_idle = new CompositeNode_Selector();
		pselect_idle->add_child(CREATE_TERMINATE_NODE(Be_Idle));
		blockhead_ai_ = pselect_idle;
	}

	{
		Composite_Node* psequence_ai_behavior = new CompositeNode_Sequence();
		psequence_ai_behavior->add_child(CREATE_TERMINATE_NODE(Co_Idle_Status));
		psequence_ai_behavior->add_child(CREATE_TERMINATE_NODE(Be_Idle));
		//psequence_ai_behavior->add_child(CREATE_TERMINATE_NODE(Co_Die_Status));
		//psequence_ai_behavior->add_child(CREATE_TERMINATE_NODE(Be_NPC_Recover));
		normal_ai_ = psequence_ai_behavior;
	}

}

int AI_Machine::thinking(role_id_t npc_id, Time_Value& now) {
	//Perf_Mon perf_mon(npc_id);

	switch (get_player_type(npc_id)) {
	case Monster_Type: {
		Monster *monster = NPC_MANAGER->find_monster(npc_id);
		if (! monster) {
			MSG_USER("ERROR_NPC_THINKING: cannot find monster_id %ld monster object.", npc_id);
			return -1;
		}

		if (monster->recover_state()) {
			monster->fini();
			NPC_MANAGER->monster_map().erase(npc_id);
			NPC_MANAGER->monster_pool().push(monster);

			return 0;
		} else if (!monster->validate_mover_scene()) {
			monster->fini();
			NPC_MANAGER->monster_map().erase(npc_id);
			NPC_MANAGER->monster_pool().push(monster);

			MSG_USER("ERROR_NPC_SCENE npc scene null id:%ld, npc type:%d scene:%d", npc_id, monster->npc_type_id(), monster->move_scene()->scene_id());
			return 0;
		}

		monster->tick(now);
		break;
	}
	case Special_Monster_Type: {
		Monster *monster = NPC_MANAGER->find_monster(npc_id);
		if (! monster) {
			MSG_USER("ERROR_NPC_THINKING: cannot find monster_id %ld monster object.", npc_id);
			return -1;
		}

		if (monster->recover_state() || !monster->validate_mover_scene()) {
			monster->fini();
			NPC_MANAGER->special_monster_map().erase(npc_id);
			NPC_MANAGER->special_monster_pool().push(dynamic_cast<Special_Monster*>(monster));

			return 0;
		} else if (!monster->validate_mover_scene()) {
			monster->fini();
			NPC_MANAGER->special_monster_map().erase(npc_id);
			NPC_MANAGER->special_monster_pool().push(dynamic_cast<Special_Monster*>(monster));

			MSG_USER("ERROR_NPC_SCENE npc scene null id:%ld, npc type:%d scene:%d", npc_id, monster->npc_type_id(), monster->move_scene()->scene_id());
			return 0;
		}

		monster->tick(now);
		break;
	}
	case Player_Monster_Type: {
		Monster *monster = NPC_MANAGER->find_monster(npc_id);
		if (! monster) {
			MSG_USER("ERROR_NPC_THINKING: cannot find monster_id %ld monster object.", npc_id);
			return -1;
		}

		if (monster->recover_state()) {
			monster->fini();
			NPC_MANAGER->player_monster_map().erase(npc_id);
			NPC_MANAGER->player_monster_pool().push(dynamic_cast<Player_Monster*>(monster));

			return 0;
		} else if (!monster->validate_mover_scene()) {
			monster->fini();
			NPC_MANAGER->player_monster_map().erase(npc_id);
			NPC_MANAGER->player_monster_pool().push(dynamic_cast<Player_Monster*>(monster));

			MSG_USER("ERROR_NPC_SCENE npc scene null id:%ld, npc type:%d scene:%d", npc_id, monster->npc_type_id(), monster->move_scene()->scene_id());
			return 0;
		}

		monster->tick(now);
		break;
	}
	case Pet_Monster_Type: {
		Monster *monster = NPC_MANAGER->find_monster(npc_id);
		if (! monster) {
			MSG_USER("ERROR_NPC_THINKING: cannot find monster_id %ld monster object.", npc_id);
			return -1;
		}

		if (monster->recover_state()) {
			monster->fini();
			NPC_MANAGER->pet_monster_map().erase(npc_id);
			NPC_MANAGER->pet_monster_pool().push(dynamic_cast<Pet_Monster*>(monster));

			return 0;
		} else if (!monster->validate_mover_scene()) {
			monster->fini();
			NPC_MANAGER->pet_monster_map().erase(npc_id);
			NPC_MANAGER->pet_monster_pool().push(dynamic_cast<Pet_Monster*>(monster));

			MSG_USER("ERROR_NPC_SCENE npc scene null id:%ld, npc type:%d scene:%d", npc_id, monster->npc_type_id(), monster->move_scene()->scene_id());
			return 0;
		}

		monster->tick(now);
		break;
	}
	case Drops_Type: {
		Drops *drops = NPC_MANAGER->find_drops_npc(npc_id);
		if (! drops) {
			MSG_USER("ERROR_NPC_THINKING: cannot find drops_id %ld fb monster object.", npc_id);
			return -1;
		}

		if (drops->recover_state()) {
			drops->fini();
			NPC_MANAGER->drops_npc_map().erase(npc_id);
			NPC_MANAGER->drops_npc_pool().push(drops);

			return 0;
		} else if (!drops->validate_mover_scene()) {
			drops->fini();
			NPC_MANAGER->drops_npc_map().erase(npc_id);
			NPC_MANAGER->drops_npc_pool().push(drops);

			MSG_USER("ERROR_NPC_SCENE npc scene null id:%ld, npc type:%d scene:%d", npc_id, drops->npc_type_id(), drops->move_scene()->scene_id());
			return 0;
		}

		drops->tick(now);
		break;
	}
	case Material_Type: {
		Material *material = NPC_MANAGER->find_material_npc(npc_id);
		if (! material) {
			MSG_USER("ERROR_NPC_THINKING: cannot find material_id %ld fb material object.", npc_id);
			return -1;
		}

		if (material->recover_state()) {
			material->fini();
			NPC_MANAGER->material_map().erase(npc_id);
			NPC_MANAGER->material_pool().push(material);

			return 0;
		} else if (!material->validate_mover_scene()) {
			material->fini();
			NPC_MANAGER->material_map().erase(npc_id);
			NPC_MANAGER->material_pool().push(material);

			MSG_USER("ERROR_NPC_SCENE npc scene null id:%ld, npc type:%d scene:%d", npc_id, material->npc_type_id(), material->move_scene()->scene_id());
			return 0;
		}

		material->tick(now);
		break;
	}
	default: {
		MSG_USER("unknow npc type id:%ld", npc_id);
		break;
	}
	}

	return 0;
}

AI_Data *AI_Machine::find_ai_behavior(int ai_id) {
	AI_Data_Map::iterator ai = ai_data_map_.find(ai_id);
	if (ai != ai_data_map_.end())
		return ai->second;
	return NULL;
}

AI_Node *AI_Machine::normal_ai(void) {
	return normal_ai_;
}

AI_Node *AI_Machine::blockhead_ai(void) {
	return blockhead_ai_;
}

AI_Node *AI_Machine::auto_master_bev_tree(void) {
	return auto_master_bev_tree_;
}
