/*
 * Covenant_Struct.cpp
 *
 *  Created on: 2014年7月15日
 *      Author: xiaoliang
 */

#include "Covenant_Struct.h"
#include "DB_Operator.h"

void Covenant_Data::reset(void) {
	id = 0;
	init_status = 0;
	over_time = Time_Value::zero;
	today_become_time = Time_Value::zero;
}

Covenant_Detail::Covenant_Detail(void) {
	reset();
}

Covenant_Detail::~Covenant_Detail(void) {

}

void Covenant_Detail::reset(void) {
	role_id = 0;
	covenant_data_map.clear();
}

int Covenant_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Covenant_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Covenant_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_covenant_detail(*this);
	return 0;
}

int Covenant_Detail::load(void) {
	return MONGO_OPERATOR->load_covenant_detail(*this);
}
