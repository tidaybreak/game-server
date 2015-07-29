/*
 * AI_Struct.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */


#include "AI_Struct.h"
#include "Config_Cache.h"

Condition::Condition(void) {
	reset();
}

void Condition::reset(void) {
	condition_type = 0;
	i_val1 = i_val2 = i_val3 = i_val4 = i_val5 = 0;
	d_val1 = d_val2 = 0.0;
	t_val1 = Time_Value::zero;
	reqeat_count = 0;
	iv_val1.clear();
	grid_coord_vec.clear();
}

Action::Action(void) {
	reset();
}

void Action::reset(void) {
	action_type = 0;
	i_val1 = i_val2 = i_val3 = i_val4 = i_val5 = i_val6 = i_val7 = i_val8 = i_val9 = 0;
	d_val1 = d_val2 = 0.0;
	s_val1 = "";
	t_val1 = t_val2 = t_val3 = Time_Value::zero;
	iv_val1.clear();
	grid_coord_vec.clear();
	sub_action_vec.clear();
}

AI_Condition_Action::AI_Condition_Action(void) {
	reset();
};

void AI_Condition_Action::reset(void) {
	id = 0;
	do_action_times = 0;
	condition.clear();
	action.clear();
	do_action_cool = Time_Value::zero;
	not_do_action_probability = 0.0;
	do_condition_cool = Time_Value(1);
}


////////////////////////////////////////////////////////////////////////

AI_Action_Running_Info::AI_Action_Running_Info(void) {
	reset();
}

AI_Action_Running_Info::AI_Action_Running_Info(int action_id) {
	reset();
	this->action_id = action_id;
	//action_config = CONFIG_CACHE->find_ai_action_cache(action_id);
}

void AI_Action_Running_Info::reset(void) {
	action_config = 0;

	action_id = 0;
	pending_time = Time_Value::zero;
	last_do_action_time = Time_Value::zero;
	last_do_condition_time = Time_Value(1);
	has_do_action_times = 0;
	action_step_now = 0;
	i_val1 = 0;
	i_val2 = 0;

	grid_coord.reset();
	grid_coord_set.clear();
	grid_coord_vec.clear();
	mover_set.clear();
}


//////////////////////////////////////////////////////////////////////////
//	AI_Time
AI_Time_Data::AI_Time_Data(void) {
	reset();
}

AI_Time_Data::Hour_Min::Hour_Min(void) {
	reset();
}

void AI_Time_Data::Hour_Min::reset(void) {
	hour = 0;
	min = 0;
}

void AI_Time_Data::reset(void) {
	date_loop = -1;
	no_set = true;
	date_time.clear();
	date_week.clear();
}


///////////////////////////////////////////////////////////////////////////
// AI_Info
AI_Info::AI_Info()
{
	reset();
}

void AI_Info::reset(void) {
	id = 0;
	vision_type = 0;
	vision_always = 0;
	tick_time = Time_Value::zero;
	life_time = Time_Value::zero;
	distance_of_patrol = 0;
	vision_area  = 0;
	vision_tick = Time_Value::zero;
	patrol_tick = Time_Value::zero;
	fight_timeout = Time_Value::zero;
	distance_of_fight = 0;
	exit_combat_buf = 0;
	date_appear.reset();
	hatred_value.clear();

	walk_path.clear();

	die_share = false;
	ignore_scene = false;
}
