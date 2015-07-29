/*
 * Exchange_Struct.cpp
 *
 *  Created on: 2014年6月26日
 *      Author: Linqiyou
 */

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "Exchange_Struct.h"
#include "Msg_Struct.h"

Exchange_Detail::Exchange_Detail(void) {
	reset();
}

Exchange_Detail::~Exchange_Detail(void) {
}

void Exchange_Detail::reset(void) {
	role_id = 0;
	ex_data.clear();
	ex_data_every_day.clear();
}

int Exchange_Detail::serialize(Block_Buffer &buf) const {
	return 0;
}

int Exchange_Detail::deserialize(Block_Buffer &buf) {
	return 0;
}

int Exchange_Detail::load(void) {
	if (role_id)
		MONGO_OPERATOR->load_exchange_detail(*this);
	return 0;
}

int Exchange_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_exchange_detail(*this);
	return 0;
}
