/*
 * AI_Event.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "AI_Event.h"
#include "ai_machine/AI_Struct.h"
#include "NPC_Manager.h"

AI_Event::AI_Event() : event_id_(0), npc_id_(0) {
	// TODO Auto-generated constructor stub

}

AI_Event::~AI_Event() {
	// TODO Auto-generated destructor stub
}

int AI_Event::handle_timeout(const Time_Value &tv) {
//	Block_Buffer buf;
//	buf.make_message(INNER_AI_ACTION);
//	buf.write_int64(event_id_);
//	buf.finish_message();
//	MAP_MANAGER_INSTANCE->push_self_loop_message(buf);
	return 0;
}

void AI_Event::reset(void) {
	event_id_ = 0;
	npc_id_ = 0;
	ai_action_running_.reset();
}

int AI_Event::init(role_id_t npc_id, role_id_t event_id, int condition_action_id) {
	AI_Action_Running_Info running_info = AI_Action_Running_Info(condition_action_id);
	if (running_info.action_config) {
		event_id_ = event_id;
		npc_id_ = npc_id;
		ai_action_running_.condition_action_vec.push_back(running_info);

		return 0;
	}
	return -1;
}

int AI_Event::process_event(void) {
	Monster *monster = NPC_MANAGER->find_monster(npc_id_);
	if (!monster) {
		MSG_USER("can not find event npc:%ld", npc_id_);
		return 0;
	}
	Time_Value now = Time_Value::gettimeofday();
//	monster->condition_action(now, ai_action_running_);

	return 0;
}

int AI_Event::set_cache_coord(Coord coord) {
	for (Condition_Action_Vec::iterator it = ai_action_running_.condition_action_vec.begin(); it != ai_action_running_.condition_action_vec.end(); ++it) {
		(*it).grid_coord_set.clear();
		(*it).grid_coord_set.insert(coord);
	}
	return 0;
}
