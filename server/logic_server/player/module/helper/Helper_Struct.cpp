/*
 * Helper_Struct.cpp
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 */

#include "Helper_Struct.h"
#include "DB_Operator.h"

Helper_Detail::Helper_Detail(void) {
	reset();
}

Helper_Detail::~Helper_Detail(void) {

}

void Helper_Detail::reset(void) {
	role_id = 0;
	daily_num_map_.clear();
	activity_num_map_.clear();
	today_activity = 0;
	all_activity = 0;
	today_award_receive.clear();
	all_award_receive.clear();
	last_join_arena_force = 0;
	no_join_areana_tip = false;
	receive_vip_activity = 0;
	check_in_day = 0;
	last_check_in_refresh_time = Time_Value::zero;
	today_is_check_in = 0;
	last_tip_force = 0;
}

int Helper_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Helper_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Helper_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_daily_detail(*this);
	return 0;
}

int Helper_Detail::load(void) {
	return MONGO_OPERATOR->load_daily_detail(*this);
}
