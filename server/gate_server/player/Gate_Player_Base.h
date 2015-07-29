/*
 * Gate_Player_Base.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef GATE_PLAYER_BASE_H_
#define GATE_PLAYER_BASE_H_

#include "Player_Base.h"

class Gate_Player;

class Gate_Player_Base : public Player_Base<Gate_Player> {
public:
	Gate_Player_Base(void);
	virtual ~Gate_Player_Base(void);

	void reset(void);

	// player send
	virtual int send_to_client(Block_Buffer &buf) = 0;

	virtual int send_to_logic(Block_Buffer &buf) = 0;

	virtual int send_to_scene(Block_Buffer &buf) = 0;

	virtual int send_to_local_chat(Block_Buffer &buf) = 0;

	virtual int send_to_inter_chat(Block_Buffer &buf) = 0;
};


#endif /* GATE_PLAYER_BASE_H_ */
