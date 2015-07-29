/*
 * Vip_Struct.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: xiaoliang
 */

#include "Vip_Struct.h"
#include "DB_Operator.h"

Vip_Detail::Vip_Detail(void) {
	reset();
}

Vip_Detail::~Vip_Detail(void) {

}

void Vip_Detail::reset(void) {
	role_id = 0;
	world_tree_val.clear();
	grow_val = 0;
	cost_money = 0;
	expired_prompt = false;
	turntable_data.reset();
	last_settle_grow = Time_Value::zero;
	exp_over_time = Time_Value::zero;
	turntable_cost = 0;
	turntable_reward.clear();
	has_receive_daily_reward = false;
}

int Vip_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Vip_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Vip_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_vip_detail(*this);
	return 0;
}

int Vip_Detail::load(void) {
	return MONGO_OPERATOR->load_vip_detail(*this);
}
