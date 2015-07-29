/*
 * Wing_Struct.cpp
 *
 *  Created on: 2014年10月13日
 *      Author: root
 */

#include "Wing_Struct.h"
#include "DB_Operator.h"

Wing_Detail::~Wing_Detail() {
}

void Wing_Detail::reset() {
	state 						= WING_NOT_ACTIVE;
	role_id						= 0;
	is_show 					= Wing_Show_State::SHOW_ON;
	lv 							= 0;
	rate 							= 0;
	progress					= 0;
	times 						= 0;
	got_reward_stage 	= 0;
	active_time 				= Time_Value::zero;
	last_time 				= Time_Value::zero;
}

int Wing_Detail::load(void) {
	return MONGO_OPERATOR->load_wing_detail(*this);
}

int Wing_Detail::save(void) {
	if (0 != role_id)
		return MONGO_OPERATOR->save_wing_detail(*this);
	return 0;
}

int Wing_Detail::serialize(Block_Buffer &buffer) const {
	return 0;
}

int Wing_Detail::deserialize(Block_Buffer &buffer) {
	return 0;
}

void Wing_Detail::set_wing_state(int state) {
	if (Wing_Show_State::SHOW_ON != state ||
		Wing_Show_State::SHOW_OFF != state) return;
	this->is_show = state;
}

