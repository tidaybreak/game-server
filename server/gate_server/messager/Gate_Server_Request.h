/*
 * Gate_Server_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef GATE_SERVER_REQUEST_H_
#define GATE_SERVER_REQUEST_H_

#include "Game_Typedef.h"

class Gate_Monitor;
class Gate_Player;

class Gate_Server_Request {
public:
	Gate_Server_Request();
	virtual ~Gate_Server_Request();

	Gate_Monitor *monitor(void);

	int find_player_by_buf(Block_Buffer &buf, Gate_Player *&player);

	int logic_login_ready(int cid, Block_Buffer &buf);

	int logic_session(int cid, Block_Buffer &buf);

	int logic_logout_ready(int cid, Block_Buffer &buf);

	int logic_transmit_finish(int cid, Block_Buffer &buf);

	int scene_battle_update(int cid, Block_Buffer &buf);

	int logic_inner_transmit(int cid, Block_Buffer &buf);

	int caht_gate_broadcast_buff(int cid, Block_Buffer &buf);


private:
	Gate_Monitor *monitor_;
};

typedef Singleton<Gate_Server_Request> Gate_Server_Request_Single;
#define GATE_SERVER_REQUEST    (Gate_Server_Request_Single::instance())

inline Gate_Monitor *Gate_Server_Request::monitor(void) {
	return monitor_;
}

#endif /* GATE_SERVER_REQUEST_H_ */
