/*
 * Arena_Rank_Player.cpp
 *
 *  Created on: 2014年5月19日
 *      Author: root
 */


#include "Arena_Rank_Player.h"
#include "Arena_Manager.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Arena.h"

Arena_Rank_Player::Arena_Rank_Player(void):need_notice_client_(false),
			match_role_id_(0),match_difficult_lv_(0){

}

Arena_Rank_Player::~Arena_Rank_Player(void){

}

void Arena_Rank_Player::reset(void){
	need_notice_client_ = 0;
	battle_start_time_ = Time_Value::zero;
	match_role_id_ = 0;
	match_role_name_ = "";
	match_server_name_ = "";
	match_difficult_lv_ = 0;
	rank_info_.reset();
}

void Arena_Rank_Player::sign_in(Logic_Player* player){

}

void Arena_Rank_Player::sign_out(Logic_Player* player){

}

role_id_t Arena_Rank_Player::role_id(void){
	return rank_info_.role_id;
}

std::string Arena_Rank_Player::role_name(void){
	return rank_info_.role_name;
}


int Arena_Rank_Player::local_rank(void){
	return rank_info_.rank;
}
int Arena_Rank_Player::inner_rank(void){
	return rank_info_.inner_rank;
}
int Arena_Rank_Player::fight(void){
	return rank_info_.fight;
}
int Arena_Rank_Player::group(void){
	return rank_info_.group;
}
int Arena_Rank_Player::seg(void){
	return rank_info_.seg;
}
int Arena_Rank_Player::level(void){
	return rank_info_.level;
}
int Arena_Rank_Player::score(void){
	return rank_info_.score;
}
int Arena_Rank_Player::career(void){
	return rank_info_.career;
}
int Arena_Rank_Player::gender(void){
	return rank_info_.gender;
}

uint32_t Arena_Rank_Player::head_id(void){
	return rank_info_.head_id;
}

int Arena_Rank_Player::be_worship(void) {
	return rank_info_.be_worship;
}

int Arena_Rank_Player::rise_fail(void){
	Logic_Player* player = this->logic_player();
	if(player){
		return player->rise_fail();
	}
	return 0;
}

int Arena_Rank_Player::load_fighter_detail(Block_Buffer& buf){
	//this->arenaer_detail()->fighter_detail_.deserialize(buf);
	return 0;
}

int Arena_Rank_Player::save_fighter_detail(Block_Buffer& buf){
	//this->arenaer_detail()->fighter_detail_.serialize(buf);
	return 0;
}

int Arena_Rank_Player::load_detail(void){
	//arenaer_detail_.load();
	return 0;
}

int Arena_Rank_Player::save_detail(void){
	//arenaer_detail_.save();
	return 0;
}

Logic_Player* Arena_Rank_Player::logic_player(void){
	return LOGIC_MONITOR->find_logic_player_by_role_id(rank_info_.role_id);
}

Player_Rank_Info* Arena_Rank_Player::player_rank_info(void){
	return &rank_info_;
}

const Player_Rank_Info& Arena_Rank_Player::player_rank_info_const(void) const{
	return rank_info_;
}

void Arena_Rank_Player::set_role_id(role_id_t value){
	rank_info_.role_id = value;
	return;
}

void Arena_Rank_Player::set_role_name(const std::string& value){
	rank_info_.role_name = value;
	return;
}

void Arena_Rank_Player::set_server_name(const std::string& value){
	rank_info_.server_name = value;
	return;
}
void Arena_Rank_Player::set_local_rank(int value){
	rank_info_.rank = value;
	Logic_Player* l_player = logic_player();
	if(l_player){
		l_player->set_local_rank(value);
	}
	return;
}

void Arena_Rank_Player::set_inner_rank(int value, bool notice/* = false*/){
	rank_info_.inner_rank = value;
	Logic_Player* l_player = logic_player();
	if(l_player){
		l_player->set_inner_rank(value);
		l_player->notice_inner_rank();
	}
	return;
}

void Arena_Rank_Player::set_king_time(int value){
	rank_info_.king_time = value;
	Logic_Player* l_player = logic_player();
	if(l_player){
		l_player->set_king_time(value);
	}
}

void Arena_Rank_Player::set_fight(int value){
	rank_info_.fight = value;
	//arenaer_detail_.detail_change();
	return;
}
void Arena_Rank_Player::set_group_seg(const int value1, const int value2){
	rank_info_.group = value1;
	rank_info_.seg = value2;
	Logic_Player* l_player = logic_player();
	if(l_player){
		l_player->set_group_seg(value1, value2);
	}
	return;
}

void Arena_Rank_Player::set_level(int value){
	rank_info_.level = value;
	//arenaer_detail_.detail_change();
	return;
}
void Arena_Rank_Player::set_score(int value){
	rank_info_.score = value;
	Logic_Player* l_player = logic_player();
	if(l_player){
		l_player->set_score(value);
	}
	return;
}
void Arena_Rank_Player::set_career(int value){
	rank_info_.career = value;
	//arenaer_detail_.detail_change();
	return;
}
void Arena_Rank_Player::set_gender(int value){
	rank_info_.gender = value;
	//arenaer_detail_.detail_change();
	return;
}
void Arena_Rank_Player::set_head_id(uint32_t value){
	rank_info_.head_id = value;
	//arenaer_detail_.detail_change();
	return;
}

void Arena_Rank_Player::set_fight_times(int value){
	rank_info_.fight_times = value;
	Logic_Player* l_player = logic_player();
	if(l_player){
		l_player->set_fight_times(value);
	}
	return;
}

void Arena_Rank_Player::set_be_worship(int value) {
	rank_info_.be_worship = value;
	Logic_Player* l_player = logic_player();
	if ( l_player ) {
		l_player->set_be_worship(value);
	}
	return;
}

void Arena_Rank_Player::set_battle_start_time(const Time_Value& time, role_id_t match_role_id, std::string match_role_name, std::string match_server_name){
	battle_start_time_ = time;
	match_role_id_ = match_role_id;
	match_role_name_ = match_role_name;
	match_server_name_ = match_server_name;
}

bool Arena_Rank_Player::check_battle_start_time(const Time_Value& now){
	return now >= battle_start_time_;
}

role_id_t Arena_Rank_Player::get_match_id(void){
	return match_role_id_;
}

void Arena_Rank_Player::set_match_id(role_id_t value){
	match_role_id_ = value;
}

void Arena_Rank_Player::set_match_role_name(std::string role_name){
	match_role_name_ = role_name;
}

void Arena_Rank_Player::check_rise_announce(void){
	if(this->group() >= 3 && this->seg() == 1){
		Logic_Player* l_player = logic_player();
		if(l_player){
			std::string name = CONFIG_CACHE_ARENA->get_part_name(group(), seg());
			l_player->create_arena_rise_announce(name);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

bool Arena_Rank_Player_Greater::operator ()(const Arena_Rank_Player *player1,
		const Arena_Rank_Player *player2) const {
	//return player1->arenaer_detail_const() > player2->arenaer_detail_const();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////

bool Rank_Greater(Arena_Rank_Player* player1, Arena_Rank_Player* player2){
	return player1->player_rank_info_const() > player2->player_rank_info_const();
}

bool Fight_Greater(Arena_Rank_Player* player1, Arena_Rank_Player* player2){
	if(player1->fight() > player2->fight()){
		return true;
	}else if(player1->fight() < player2->fight()){
		return false;
	}else{
		if(player1->group() > player2->group()){
			return true;
		}else if(player1->group() < player2->group()){
			return false;
		}else{
			if(player1->seg() > player2->seg()){
				return true;
			}else if(player1->seg() < player2->seg()){
				return false;
			}else{
				return false;
			}
		}
	}
	return false;
}
