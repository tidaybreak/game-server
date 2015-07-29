/*
 * Icon_Struct.cpp
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#include "Icon_Struct.h"
#include "DB_Operator.h"

Icon_Detail::Icon_Detail(void) {
	reset();
}

Icon_Detail::~Icon_Detail(void) {

}

void Icon_Detail::reset(void) {
	role_id = 0;
	icon_map.clear();
}

int Icon_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Icon_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Icon_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_icon_detail(*this);
	return 0;
}

int Icon_Detail::load(void) {
	return MONGO_OPERATOR->load_icon_detail(*this);
}
