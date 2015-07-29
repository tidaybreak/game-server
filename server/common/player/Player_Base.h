/*
 * Player_Base.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef PLAYER_BASE_H_
#define PLAYER_BASE_H_

#include "Game_Typedef.h"
#include "Record_Client.h"

template <typename PLAYER>
class Player_Base {
public:
	typedef void (PLAYER::*event_fun)(void);
	struct Player_Event {
		Player_Event() : type(0), fun(0), loop_times(INT_MAX) {};

		void reset(void) {
			type = 0;
			fun = 0;
			interval = Time_Value::zero;
			next_opt = Time_Value::zero;
			loop_times = INT_MAX;
		}

		int type;
		event_fun fun;
		int loop_times;
		Time_Value interval;
		Time_Value next_opt;

		bool operator==(const Player_Event &key) const {
			return type == key.type;
		}
	};

	class Player_Timer_Greater {
	public:
		inline bool operator()(const Player_Event *a, const Player_Event *b) {
			return (a->next_opt) > (b->next_opt);
		}
	};
	typedef boost::unordered_set<int> Event_Set;
	typedef Object_Pool_Ex<Player_Event, NULL_MUTEX> Event_Pool;
	typedef std::priority_queue<Player_Event *, std::vector<Player_Event *>, Player_Timer_Greater> Event_Queue;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Player_Base(void);
	virtual ~Player_Base(void);

	void reset(void);

	virtual PLAYER *player_self(void) { return 0; };

	virtual role_id_t role_id(void) = 0;

	virtual std::string role_name(void) = 0;

	virtual int level(void) = 0;

	virtual int career(void) = 0;

	int create_event(int type, event_fun fun, Time_Value interval, int loop_times = INT_MAX);

	int remove_event(int type);

	int process_event(void);

	void make_message(Block_Buffer &buf, int msg_id);

	virtual int send_err_to_client(int error_id, int source);

	virtual int send_tip_to_client(int error_id, int arg1);

	virtual int send_tip_to_client(int error_id, std::string &str1);

private:
	Event_Set event_set_;
	Event_Pool event_pool_;
	Event_Queue event_queue_;
};

#include "Player_Base.hpp"

#endif /* PLAYER_BASE_H_ */
