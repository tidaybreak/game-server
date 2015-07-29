/*
 * Robot_Struct.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: ti
 */
#include "Robot_Struct.h"

Robot_Detail::Robot_Detail(void) {
	reset();
}

Robot_Detail::~Robot_Detail(void) {

}

void Robot_Detail::reset(void) {
	role_id = 0;
	account = "";
	role_name = "";
	level = 0;
	gender = 0;
	career = 0;
	create_time = Time_Value::zero;
	faction_id = 0;
	faction_name = "";
	last_sign_in_time = Time_Value::zero;
	last_sign_out_time = Time_Value::zero;
	last_trigger_daily_zero = Time_Value::zero;
	last_trigger_weekly_zero = Time_Value::zero;

	online = 0;
	vip = 0;
	status = LOGIN;
	sub_status = 0;
	task_status = TASK_IDLE;

	need_walk_monster_points.clear();
	walked_mosters_points = false;
	loop_exit_diff_space = 0;
	loop_transfer_scene = 0;
	loop_initing = 0;
	loop_path_finding = 0;
	loop_collecting = 0;
	loop_fb_wayfinding = 0;
	loop_fb_wayfind_end = 0;
}



////////////////////////////////////////////////////////////////////

