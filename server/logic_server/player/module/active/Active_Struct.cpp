/*
 * Active_Struct.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Active_Struct.h"
#include "DB_Operator.h"

void Switch_On_Event::reset(void) {
	on_time = Time_Value::zero;
	action = 0;
	argv.clear();
	is_run = false;
}

void Switch_On_Active::reset(void) {
	start_time = Time_Value::zero;
	before_start_time = Time_Value::zero;
	end_time = Time_Value::zero;
	id = 0;
	type = 0;
	icon = 0;
	openLevel = 0;
	CLEAR_VECTOR(Switch_On_Event_Vec, event_action);
}

void Active_Data_Stauts::reset(void) {
	id = 0;
	num = 0;
	begin_time = Time_Value::zero;
	end_time = Time_Value::zero;
}
