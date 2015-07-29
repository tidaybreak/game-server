/*
 * Logic_DB_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef LOGIC_DB_REQUEST_H_
#define LOGIC_DB_REQUEST_H_

#include "Game_Typedef.h"

class Logic_Monitor;
struct Logic_Player_Detail;

class Logic_DB_Request {
public:
	Logic_DB_Request();
	virtual ~Logic_DB_Request();

	Logic_Monitor *monitor(void);

	int process_load_player_data(Logic_Player_Detail *detail);
	int process_refresh_ranking_data(Block_Buffer &buf);
	//int process_activation_code_data(Block_Buffer &buf);

private:
	Logic_Monitor *monitor_;
};

typedef Singleton<Logic_DB_Request> Logic_DB_Request_Single;
#define LOGIC_DB_REQUEST    (Logic_DB_Request_Single::instance())

inline Logic_Monitor *Logic_DB_Request::monitor(void) {
	return monitor_;
}

#endif /* LOGIC_DB_REQUEST_H_ */
