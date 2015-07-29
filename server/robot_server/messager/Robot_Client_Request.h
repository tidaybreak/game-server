/*
 * Robot_Client_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef ROBOT_CLIENT_REQUEST_H_
#define ROBOT_CLIENT_REQUEST_H_

#include "Game_Typedef.h"

class Robot_Monitor;
class Robot_Player;

class Robot_Client_Request {
public:
	Robot_Client_Request();
	virtual ~Robot_Client_Request();

	Robot_Monitor *monitor(void);

    int process_client_block(Robot_Player *player, int cid, int msg_id, Block_Buffer &buf);

	int process_player_login(int cid, Block_Buffer &buf);

	int process_transmit(Robot_Player *player, Block_Buffer &buf);

private:
	Robot_Monitor *monitor_;
};

typedef Singleton<Robot_Client_Request> Robot_Client_Request_Single;
#define ROBOT_CLIENT_REQUEST    (Robot_Client_Request_Single::instance())

inline Robot_Monitor *Robot_Client_Request::monitor(void) {
	return monitor_;
}

#endif /* ROBOT_CLIENT_REQUEST_H_ */
