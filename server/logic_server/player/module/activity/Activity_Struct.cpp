/*
 * Activity_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include <activity/Activity_Struct.h>
#include "DB_Operator.h"

Activity_Detail::Activity_Detail(void) {
	reset();
}

Activity_Detail::~Activity_Detail(void) {

}

void Activity_Detail::reset(void) {
	role_id = 0;
	vip37_receive_id.clear();
	is_receive_37phone = false;
	vip37_level = 0;

	is_rec_r2_gift = false;
}

int Activity_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Activity_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Activity_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_activity_detail(*this);
	return 0;
}

int Activity_Detail::load(void) {
	return MONGO_OPERATOR->load_activity_detail(*this);
}
