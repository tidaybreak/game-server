/*
 * Robot_Player_Base.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef ROBOT_PLAYER_BASE_H_
#define ROBOT_PLAYER_BASE_H_

#include "Player_Base.h"

class Robot_Player;

class Robot_Player_Base {
public:
	Robot_Player_Base(void);
	virtual ~Robot_Player_Base(void);

	void reset(void);

	// player send
	virtual int send_to_server(Block_Buffer &buf) = 0;

};


#endif /* ROBOT_PLAYER_BASE_H_ */
