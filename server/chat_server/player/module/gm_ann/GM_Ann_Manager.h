/*
 * GM_Ann_Manager.h
 *
 *  Created on: 2014-06-19
 *      Author: xiaoliang
 */

#ifndef GM_ANN_MANAGER_H_
#define GM_ANN_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Msg_Struct.h"
#include "GM_Ann_Struct.h"

class GM_Ann_Manager {
public:
	GM_Ann_Manager();
	~GM_Ann_Manager();
	void reset();

	void start(void);

	void tick(const Time_Value &now);

	void send_ann_loop(const Time_Value &now);

	void send_ann(const std::string &contents);

	void set_gm_announce(int32_t id, int8_t type, int32_t stime, int32_t etime, int32_t step, std::string &contents);

private:
	GM_Ann_Info_Map ann_info_map;
};

typedef Singleton<GM_Ann_Manager> GM_Ann_Manager_Single;
#define GM_ANN_MANAGER GM_Ann_Manager_Single::instance()

#endif /* GM_ANN_MANAGER_H_ */
