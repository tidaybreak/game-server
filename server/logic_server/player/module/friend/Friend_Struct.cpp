/*
 * Friend_Struct.cpp
 *
 *  Created on: 2014-4-1
 *      Author: zhouyajun
 */
#include "Friend_Struct.h"
#include "DB_Operator.h"

#include "Configurator.h"
#include "Config_Cache.h"
#include "Utility_Func.h"

int Friend_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}
int Friend_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}
int Friend_Detail::save(void) {
	if (is_change)
		return MONGO_OPERATOR->save_friend_detail(*this);
	else
		return 0;
}

int Friend_Detail::load(void) {
	return MONGO_OPERATOR->load_friend_detail(*this);
}
void Friend_DB_View::reset(void) {
	head_photo_id = 0;
	apply_list_size = 0;
	invite_list_size = 0;
	firend_list_.clear();
}
void Friend_Detail::reset(void) {
	is_change = false;
	firend_list_.clear();
	stranger_list_.clear();
	black_list_.clear();
	invite_list_.clear();
	apply_list_.clear();
	head_photo_id = 0;
	del_firend_list_.clear();
}
