/*
 * Logic_Player_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Logic_Player_Struct.h"

Player_DB_View::Player_DB_View(void) {
	reset();
}

void Player_DB_View::reset(void) {
	role_id = 0;
	account_info.reset();
	location.reset();
	role_name = "";
	gender = 0;
	career = 0;
	head_id = 0;
	force = 0;
	ttl_force = 0;
	level = 0;
	vip_level = 0;
	country = 0;
	gang_name.clear();
	gang_id = 0;
	gang_war_award_get_times_by_day = 0;
	last_sign_out_time = Time_Value::zero;
	last_sign_in_time = Time_Value::zero;
	vip_over_time = Time_Value::zero;
	account_mode = 0;
	mode_value = 0;

	add_up_online_time = 0;
	fcm = 1;

	friend_.reset();

	dragon_vale_view.reset();
	title_id = 0;

	avatar_vec.clear();

	sign_out_flag_ = Time_Value::zero;
}

int Player_DB_View::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Player_DB_View::deserialize(Block_Buffer &buffer) {

	return 0;
}

void Player_DB_View::set_account_status(int status, int val) {
	account_mode |= status;
	mode_value = val;
}

void Player_DB_View::cancel_account_status(int status) {
	account_mode &= ~status;
	mode_value = 0;
}

uint32_t Player_DB_View::vip(void){
	Time_Value now = Time_Value::gettimeofday();
	if(vip_over_time <= now)
		return 0;
	return vip_level;
}

bool Player_DB_View::is_dragon_vale_special() {
	return dragon_vale_view.is_special;
}

void Player_DB_View::hook_dragon_vale_special_change() {

}

int Player_DB_View::db_view_size(void) {
	int db_size = 0;
	db_size += sizeof(*this);

	db_size += friend_.firend_list_.size() * sizeof(Friend_Info);

	db_size += avatar_vec.size() * sizeof(Int_Vec);

	db_size += dragon_vale_view.cancel_create_building_set.size() * sizeof(uint8_t);
	db_size += dragon_vale_view.dragon_building_info_map.size() * (sizeof(int64_t) + sizeof(DragonBuildingInfo));
	db_size += dragon_vale_view.thief_id_quality_map.size() * sizeof(int64_t) * 2;
	db_size += dragon_vale_view.foot_info_map.size() * (sizeof(int64_t) + sizeof(Foot_Info));
	db_size += dragon_vale_view.foot_id_set.size() * sizeof(int64_t);
	db_size += dragon_vale_view.notice_board_list.size() * sizeof(Notice_Board);

	return db_size;
}

//////////////////////////////////////////////////////////////////////////////////////

DB_View::DB_View(void)
: id_player_view_map(player_view_map_bucket_num),
  name_player_view_map(player_view_map_bucket_num),
  account_player_view_map(player_view_map_bucket_num),
  account_role_view_map(player_view_map_bucket_num),
  role_num_map(6) {
	role_num_map.clear();
	for (int i=1; i<=3; ++i) {
		for (int j=1; j<=2; ++j) {
			role_num_map[Role_Key(i, j)] = 0;
		}
	}
}

void DB_View::reset(void) {
	id_player_view_map.clear();
	name_player_view_map.clear();
	account_player_view_map.clear();
	account_role_view_map.clear();
	server_amount_map.clear();
}

int DB_View::insert_new_player(Player_DB_View *player) {
	if (! id_player_view_map.insert(std::make_pair(player->role_id, player)).second) {
		MSG_USER("insert failure, role_id:%ld", player->role_id);
	}

	if (! name_player_view_map.insert(std::make_pair(player->role_name, player)).second) {
		MSG_USER("insert failure, role_id:%ld", player->role_id);
	}

	if (! account_player_view_map.insert(std::make_pair(player->account_info, player)).second) {
		MSG_TRACE_ABORT("insert failure, role_id:%ld, account:%s, agent:%d, server:%d", player->role_id, player->account_info.account_name.c_str(), player->account_info.agent_num, player->account_info.server_num);
	}

	DB_View::Account_Role_View_Map::iterator find_it = account_role_view_map.find(player->account_info.account_name);
	if (find_it != account_role_view_map.end()) {
		find_it->second.push_back(player);
	} else {
		std::vector<Player_DB_View *> player_db_view;
		player_db_view.push_back(player);
		account_role_view_map.insert(std::make_pair(player->account_info.account_name, player_db_view));
	}

	Server_Key_Amount_Map::iterator s_find_it = server_amount_map.find(Server_Key(player->account_info.agent_num, player->account_info.server_num));
	if (s_find_it != server_amount_map.end()) {
		if (player->role_id > s_find_it->second) {
			s_find_it->second = player->role_id;
		}
	} else {
		server_amount_map.insert(std::make_pair(Server_Key(player->account_info.agent_num, player->account_info.server_num), player->role_id));
	}

	DB_View::Role_Num_Map::iterator r_find_it = role_num_map.find(Role_Key(player->career, player->gender));
	if (r_find_it != role_num_map.end()) {
		++r_find_it->second;
	} else {
		role_num_map.insert(std::make_pair(Role_Key(player->career, player->gender), 1));
	}

	return 0;
}

void DB_View::abnormal_account_add(role_id_t role_id, int delay_sec) {
	abnormal_account.insert(std::make_pair(role_id, Time_Value::gettimeofday() + Time_Value(delay_sec)));
}

Role_Key DB_View::get_min_career(void) {
	Role_Key key;
	Role_Num_Map::iterator begin_it = role_num_map.begin();
	if (begin_it == role_num_map.end()) return key;

	key.career = begin_it->first.career;
	key.gender = begin_it->first.gender;
	int32_t temp = -1;
	for (Role_Num_Map::iterator it = role_num_map.begin(); it != role_num_map.end(); ++it) {
		// 暂时不推荐法师
//		if (it->first.career == 3) continue;

		if (-1 != temp && temp <= it->second) continue;
		key.career = it->first.career;
		key.gender = it->first.gender;
		temp = it->second;
	}
	return key;
}
