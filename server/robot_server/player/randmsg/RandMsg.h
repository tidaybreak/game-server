/*
 * RandMsg.h
 *
 *  Created on: Dec 17, 2013
 *      Author: xiaoliang
 */

#ifndef RAND_MSG_H_
#define RAND_MSG_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"

struct Robot_Base_Msg;
typedef std::vector<Robot_Base_Msg*> Robot_Msg_Vec;
class RandMsg {
public:
	RandMsg();
	virtual ~RandMsg();
	void reset(void);
	void send(Time_Value &now);

private:
	void rand_send(void);
	Time_Value send_interval;
	Time_Value last_send_time;
	Robot_Msg_Vec* msg_vec;
};



#endif /* RAND_MSG_H_ */
