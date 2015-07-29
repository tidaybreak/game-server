/*
 * Example_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Example_Struct.h"
#include "DB_Operator.h"

Example_Detail::Example_Detail(void) {
	reset();
}

Example_Detail::~Example_Detail(void) {

}

void Example_Detail::reset(void) {
	role_id = 0;
}

int Example_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Example_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Example_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_example_detail(*this);
	return 0;
}

int Example_Detail::load(void) {
	return MONGO_OPERATOR->load_example_detail(*this);
}
