/*
 * Activities_Stores_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include <activities_stores/Activities_Stores_Struct.h>
#include "DB_Operator.h"

Activities_Stores_Detail::Activities_Stores_Detail(void) {
	reset();
}

Activities_Stores_Detail::~Activities_Stores_Detail(void) {

}

void Activities_Stores_Detail::reset(void) {
	role_id = 0;
	id_num_buy.clear();
}

int Activities_Stores_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Activities_Stores_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Activities_Stores_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_activities_stores_detail(*this);
	return 0;
}

int Activities_Stores_Detail::load(void) {
	return MONGO_OPERATOR->load_activities_stores_detail(*this);
}
