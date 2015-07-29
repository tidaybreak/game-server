/*
 * AI_Machine.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef AI_MACHINE_H_
#define AI_MACHINE_H_

#include "ai_machine/behavior_tree/Behavior_Tree.h"
#include "Configurator.h"
#include "AI_Struct.h"

class Monster;

struct AI_Data {
	Composite_Node *ai_behavior;
	//AI_Config *ai_config;
};

class AI_Machine  {
public:
	typedef boost::unordered_map<int, AI_Data *> AI_Data_Map;

	AI_Machine(void);
	virtual ~AI_Machine(void);
	AI_Machine(const AI_Machine &);

	void init(void);
	int behavior_init(Json::Value &node_json, Composite_Node* parent_node);

	int thinking(role_id_t npc_id, Time_Value& now);
	AI_Data *find_ai_behavior(int ai_id);

	AI_Node *normal_ai(void);
	AI_Node *blockhead_ai(void);
	AI_Node *auto_master_bev_tree(void);

private:

	const AI_Machine &operator=(const AI_Machine &);

private:

	AI_Data_Map ai_data_map_;
	AI_Node *normal_ai_;
	AI_Node *blockhead_ai_;
	AI_Node *auto_master_bev_tree_;
};

typedef Singleton<AI_Machine> AI_Machine_Single;
#define AI_MACHINE    (AI_Machine_Single::instance())

#endif /* AI_MACHINE_H_ */
