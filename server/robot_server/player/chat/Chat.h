/*
 * Chat.h
 *
 *  Created on: Dec 17, 2013
 *      Author: xiaoliang
 */

#ifndef CHAT_H_
#define CHAT_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"

class Chat {
public:
	Chat();
	virtual ~Chat();
	void reset(void);
	void say(Time_Value &now, Location &location);

private:
	Time_Value say_interval;
	Time_Value last_say_time;
};



#endif /* ROBOT_PLAYER_H_ */
