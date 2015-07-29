/*
 * Reward_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Reward_Struct.h"
#include "DB_Operator.h"

Reward_Detail::Reward_Detail(void) {
	reset();
}

Reward_Detail::~Reward_Detail(void) {

}

void Reward_Detail::reset(void) {
	role_id = 0;
	online_reward_num = 0;
	online_reward_time = Time_Value::zero;
	last_online_refresh_time = Time_Value::zero;
	online_reward_over = 0;
	last_retime = Time_Value::zero;
//	login_seven_num = 0;
//	login_seven_is_receive = 0;
	login_days = 0;
	receive_seven_days.clear();

	activation_code_receive_info.clear();

	reward_back_all_num.clear();
	reward_back_item_value.clear();
	reward_back_receive_time.clear();
	reward_back_first_time.clear();
	reward_back_item_value_tmp.clear();

	keep_reward_status = 0;
}

int Reward_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Reward_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Reward_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_reward_detail(*this);
	return 0;
}

int Reward_Detail::load(void) {
	return MONGO_OPERATOR->load_reward_detail(*this);
}
