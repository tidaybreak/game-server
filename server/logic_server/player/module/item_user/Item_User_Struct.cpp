/*
 * Item_User_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "DB_Operator.h"
#include "Item_User_Struct.h"
#include "Item_User_Def.h"


Item_User_Detail::Item_User_Detail() {
	reset();
}

Item_User_Detail::~Item_User_Detail() {
}

int Item_User_Detail::serialize(Block_Buffer &buffer) const {
	// not necessary
	return 0;
}

int Item_User_Detail::deserialize(Block_Buffer &buffer) {
	// not necessary
	return 0;
}

int Item_User_Detail::load(void) {
	return MONGO_OPERATOR->load_item_user_detail(*this);
}

int Item_User_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_item_user_detail(*this);
	else
		return 0;
}

void Item_User_Detail::reset(void) {
	role_id = 0;
	CLEAR_VECTOR(Item_Use_Time, can_use_time_);
	can_use_time_.resize(CD_T_VEC_SIZE, Time_Value::zero);
	extra_prop_.clear();
	item_had_used_times_.clear();
	is_change = false;
}
