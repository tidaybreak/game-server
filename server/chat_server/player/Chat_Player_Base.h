/*
 * Chat_Player_Base.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef CHAT_PLAYER_BASE_H_
#define CHAT_PLAYER_BASE_H_

#include "Player_Base.h"

class Chat_Player;

class Chat_Player_Base : public Player_Base<Chat_Player> {
public:
	Chat_Player_Base(void);
	virtual ~Chat_Player_Base(void);

	void reset(void);

	// player send
	virtual int send_to_client(Block_Buffer &buf, bool use_swap = false) = 0;

	virtual int send_to_gate(Block_Buffer &buf, bool use_swap = false) = 0;

	virtual int send_to_logic(Block_Buffer &buf, bool use_swap = false) = 0;

	virtual int send_to_scene(Block_Buffer &buf, bool use_swap = false) = 0;

};


#endif /* CHAT_PLAYER_BASE_H_ */
