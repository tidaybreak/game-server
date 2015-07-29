/*
 * Knight_Trials_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Knight_Trials_Struct.h"
#include "DB_Operator.h"
#include "Logic_Player.h"

//int Knight_Trials_Player::serialize(Block_Buffer &buffer) const {
//	buffer.write_int64(role_id);
//	buffer.write_int64(uuid);
//	buffer.write_string(server_name);
//	base.serialize(buffer);
//	fighter.serialize(buffer);
//	heroer.serialize_fighter(buffer);
//	return 0;
//}
//
//int Knight_Trials_Player::deserialize(Block_Buffer &buffer) {
//
//	return 0;
//}


Knight_Trials_Detail::Knight_Trials_Detail(void) {
	reset();
}

Knight_Trials_Detail::~Knight_Trials_Detail(void) {

}

void Knight_Trials_Detail::reset(void) {
	role_id = 0;
	cur_points = 0;
	role_id_set.clear();
	match_day = 0;
	to_role_id = 0;
	to_uuid = 0;
	cur_receive = 0;
	CLEAR_VECTOR(std::vector<Uint_Uint>, reward);
	reset_status = false;
	recode_box_reward.clear();
}

int Knight_Trials_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Knight_Trials_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Knight_Trials_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_knight_trials_detail(*this);
	return 0;
}

int Knight_Trials_Detail::load(void) {
	return MONGO_OPERATOR->load_knight_trials_detail(*this);
}


/*======================================================================*/

Knight_Trials_Player_Detail::Knight_Trials_Player_Detail(void) {
	reset();
}

Knight_Trials_Player_Detail::~Knight_Trials_Player_Detail(void) {

}

void Knight_Trials_Player_Detail::reset(void) {
	role_id = 0;
	CLEAR_VECTOR(Knight_Trials_Role_Info_Vec, cur_player_fighter_vec);
	CLEAR_VECTOR(Knight_Trials_Role_Info_Vec, hide_player_fighter_vec);
}

int Knight_Trials_Player_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Knight_Trials_Player_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Knight_Trials_Player_Detail::save(void) {
//	if (role_id)
//		MONGO_OPERATOR->save_knight_trials_player_detail(*this);
	return 0;
}

int Knight_Trials_Player_Detail::load(void) {
	return MONGO_OPERATOR->load_knight_trials_player_detail(*this);
}
