/*
 * Oper_Activity_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include <oper_activity/Oper_Activity_Struct.h>
#include "DB_Operator.h"

Oper_Activity_Detail::Oper_Activity_Detail(void) {
	reset();
}

Oper_Activity_Detail::~Oper_Activity_Detail(void) {

}

void Oper_Activity_Detail::reset(void) {
	role_id = 0;
	last_item_start_time.clear();
	actid_type_record_val.clear();
	actid_receive_id.clear();
	actid_receive_id_num.clear();
	actid_append_effect.clear();
	actid_recid_last_refresh_time.clear();
	actid_login_day.clear();
}

int Oper_Activity_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Oper_Activity_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Oper_Activity_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_oper_activity_detail(*this);
	return 0;
}

int Oper_Activity_Detail::load(void) {
	return MONGO_OPERATOR->load_oper_activity_detail(*this);
}
