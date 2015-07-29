/*
 * GM_Ann_Struct.cpp
 *
 *  Created on: May 8, 2014
 *      Author: xiaoliang
 */

#include "GM_Ann_Struct.h"
#include "Msg_Struct.h"

GM_Ann_Info::GM_Ann_Info(void) {
	reset();
}

void GM_Ann_Info::reset(void) {
	contents.clear();
	stime = Time_Value::zero;
	etime = Time_Value::zero;
	send_time = Time_Value::zero;
	step = 0;
	id = 0;
}
