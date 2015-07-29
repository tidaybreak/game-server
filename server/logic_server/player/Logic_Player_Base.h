/*
 * Logic_Player_Base.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef LOGIC_PLAYER_BASE_H_
#define LOGIC_PLAYER_BASE_H_

#include "Player_Base.h"

class Logic_Player;

class Logic_Player_Base : public Player_Base<Logic_Player> {
public:
	typedef void (Logic_Player::*event_fun)(void);

	Logic_Player_Base(void);
	virtual ~Logic_Player_Base(void);

	void reset(void);

	// player send
	virtual int send_to_gate(Block_Buffer &buf, bool use_swap = false) = 0;
	virtual int send_to_scene(Block_Buffer &buf, bool use_swap = false) = 0;
	virtual int send_to_client(Block_Buffer &buf, bool use_swap = false) = 0;
	virtual int send_to_local_chat(Block_Buffer &buf, bool use_swap = false) = 0;

	virtual bool player_has_init(void) = 0;
};



#endif /* LOGIC_PLAYER_BASE_H_ */
