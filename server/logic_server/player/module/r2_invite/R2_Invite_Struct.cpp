/*
 * R2_Invite_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include <r2_invite/R2_Invite_Struct.h>
#include "DB_Operator.h"

R2_Invite_Detail::R2_Invite_Detail(void) {
	reset();
}

R2_Invite_Detail::~R2_Invite_Detail(void) {

}

void R2_Invite_Detail::reset(void) {
	role_id = 0;
	invite_user = 0;
	today_success_num = 0;
	all_success_num = 0;
	invite_mail.clear();
	note_level.clear();
	my_mail.clear();
}

int R2_Invite_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int R2_Invite_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int R2_Invite_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_r2_invite_detail(*this);
	return 0;
}

int R2_Invite_Detail::load(void) {
	return MONGO_OPERATOR->load_r2_invite_detail(*this);
}
