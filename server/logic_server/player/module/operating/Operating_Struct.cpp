/*
 * Operating_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Operating_Struct.h"
#include "DB_Operator.h"

Operating_Detail::Operating_Detail(void) {
	reset();
}

Operating_Detail::~Operating_Detail(void) {

}

void Operating_Detail::reset(void) {
	role_id = 0;
	CLEAR_VECTOR(Offline_Mail, mail_list);
	CLEAR_VECTOR(Operating_Recharge_Vec, recharge_gold);
	CLEAR_VECTOR(Offline_Ganger_Info, ganger_info);
	CLEAR_VECTOR(Offline_Arena_Reward, arena_three_day_reward);
	CLEAR_VECTOR(Offline_Arena_Reward, arena_seven_day_reward);
	CLEAR_VECTOR(Offline_Flowers_Vec, receive_flowers_list);

	building_notice_map.clear();
	rob_fight_result_map.clear();
	gain_rob_production_set.clear();
	is_dragon_vale_special = false;
	inv_time = 0;
	like_success = 0;
	wbrank = 0;
	damage = 0;
	damage_rate = 0;
	team_award.clear();
}

int Operating_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Operating_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Operating_Detail::save(void) {
	// 离线操作不能保存
	//if (role_id)
	//	MONGO_OPERATOR->save_operating_detail(*this);
	return 0;
}

int Operating_Detail::load(void) {
	return MONGO_OPERATOR->load_operating_detail(*this);
}
