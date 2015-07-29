/*
 * GM_Ann_Struct.h
 *
 *  Created on: May 8, 2014
 *      Author: xiaoliang
 */

#ifndef GM_ANN_STRUCT_H_
#define GM_ANN_STRUCT_H_

#include "Game_Typedef.h"

struct GM_Ann_Info {
	GM_Ann_Info(void);
	void reset(void);
	std::string contents;
	Time_Value stime;
	Time_Value etime;
	int32_t step;
	int32_t id;
	Time_Value send_time;
};

typedef boost::unordered_map<int32_t, GM_Ann_Info> GM_Ann_Info_Map;

#endif /* GM_ANN_STRUCT_H_ */
