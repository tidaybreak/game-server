/*
 * FashionStruct.cpp
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#include "Fashion_Struct.h"
#include "DB_Operator.h"

Fashion_Detail::~Fashion_Detail(void) {

}

void Fashion_Detail::reset(void) {
	role_id = 0;
	is_show_ = Fashion_Show_State::SHOW_ON;
	lv_ = 1;
	exp_ = 0;
	fashion_part_map_.clear();
}

int Fashion_Detail::serialize(Block_Buffer &buffer) const {
	return 0;
}
int Fashion_Detail::deserialize(Block_Buffer &buffer) {
	return 0;
}

int Fashion_Detail::load(void) {
	return MONGO_OPERATOR->load_fashion_detail(*this);
}

int Fashion_Detail::save(void) {
	if (0 != role_id)
		return MONGO_OPERATOR->save_fashion_detail(*this);
	return 0;
}

void Fashion_Detail::set_show_state(int state) {
	if (state != Fashion_Show_State::SHOW_ON && state != Fashion_Show_State::SHOW_OFF) return;
	is_show_ = state;
}
