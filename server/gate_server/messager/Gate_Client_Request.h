/*
 * Gate_Client_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef GATE_CLIENT_REQUEST_H_
#define GATE_CLIENT_REQUEST_H_

#include "Game_Typedef.h"

class Gate_Monitor;
class Gate_Player;

class Gate_Client_Request {
public:
	Gate_Client_Request();
	virtual ~Gate_Client_Request();

	Gate_Monitor *monitor(void);

    int process_client_block(Gate_Player *player, int cid, int msg_id, Block_Buffer &buf);

	int process_player_login(int cid, Block_Buffer &buf);

	int process_transmit(Gate_Player *player, Block_Buffer &buf);

private:
	Gate_Monitor *monitor_;
};

typedef Singleton<Gate_Client_Request> Gate_Client_Request_Single;
#define GATE_CLIENT_REQUEST    (Gate_Client_Request_Single::instance())

inline Gate_Monitor *Gate_Client_Request::monitor(void) {
	return monitor_;
}

#endif /* GATE_CLIENT_REQUEST_H_ */
