/*
 * AI_Event.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef AI_EVENT_H_
#define AI_EVENT_H_


#include "NPC.h"
#include "Event_Handler.h"

class AI_Event: public Event_Handler {
public:
	AI_Event();
	virtual ~AI_Event();

	void reset(void);

	virtual int handle_timeout(const Time_Value &tv);

	int init(role_id_t npc_id, role_id_t event_id, int condition_action_id);
	int process_event(void);

	int set_cache_coord(Coord coord);

private:
	role_id_t event_id_;
	role_id_t npc_id_;
	AI_Action_Running ai_action_running_;
};

#endif /* AI_EVENT_H_ */
