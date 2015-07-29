/*
 * Wayfind_Struct.cpp
 *
 *  Created on: 2014-06-07
 *      Author: xiaoliang
 */
#include "Wayfind_Struct.h"

Wayfind_Info::Wayfind_Info(void) {
	reset();
}

Wayfind_Info::~Wayfind_Info(void) {

}

void Wayfind_Info::reset(void) {
	det_coord.reset();
	det_scene_id = 0;
	scene = 0;
	cur_location.reset();
	last_move_time = Time_Value::zero;
	move_interval_time = Time_Value::zero;
	first_move = true;
}



////////////////////////////////////////////////////////////////////

