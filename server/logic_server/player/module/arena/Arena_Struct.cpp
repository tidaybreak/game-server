/*
 * Arena_Struct.cpp
 *
 *  Created on: 2014年5月17日
 *      Author: lys
 */

#include "Arena_Struct.h"
#include "DB_Def.h"
#include "DB_Operator.h"
#include "DB_Manager.h"
#include "Config_Cache_Arena.h"


Arenaer_Detail::Arenaer_Detail(void){
	reset();
}

Arenaer_Detail::~Arenaer_Detail(void){

}

void Arenaer_Detail::reset(void){
	role_id = 0;
	is_change = false;
	group = 1;
	seg = 1;
	score = 0;
	rise_fail = 0;
	rise_win = 0;
	rise_lose = 0;
	rank = 0;
	inner_rank = 0;
	king_time = 0;
	enter_num = 0;
	pay_times = 0;
	buy_pay_times = 0;
	streak_win = 0;
	streak_lose = 0;
	compensate_times = 0;
	three_day_rank = 0;
	seven_day_rank = 0;
	cd = Time_Value::zero;
	CLEAR_VECTOR(std::vector<Arena_Battle_Report>, report_vec);
	match_time = Time_Value::zero;
	match_player_1 = 0;
	match_player_2 = 0;
	match_player_3 = 0;
	match_dlv_1 = 0;
	match_dlv_2 = 0;
	match_dlv_3 = 0;
	fight_times = 0;
//	arena_worship = 0;
	be_worship = 0;
}

int Arenaer_Detail::serialize(Block_Buffer &buffer) const{
	return 0;
}
int Arenaer_Detail::deserialize(Block_Buffer &buffer){
	return 0;
}

int Arenaer_Detail::load(void){
	return MONGO_OPERATOR->load_arenaer_detail(*this);
}

int Arenaer_Detail::save(void){
	if(role_id != 0 && rank > 0){
		return MONGO_OPERATOR->save_arenaer_detail(*this);
	}
	return 0;
}

int Arenaer_Detail::load_arena_fighter_detail(void){
	return 0;
	//return MONGO_OPERATOR->load_arena_fighter_detail(fighter_detail_);
}

///////////////////////////////////////////////////////////////

bool operator>(const Arenaer_Detail &detail1 , const Arenaer_Detail &detail2){
	if(detail1.group > detail2.group){
		return true;
	}else if( detail1.group < detail2.group){
		return false;
	}else{
		if(detail1.seg > detail2.seg){
			return true;
		}else if(detail1.seg < detail2.seg){
			return false;
		}else{
			if(detail1.score > detail2.score){
				return true;
			}else if(detail1.score < detail2.score){
				return false;
			}else{
				//if(detail1.fight > detail2.fight){
					return true;
				//}
			}

		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////

Arena_Season_Result::Arena_Season_Result(void){
	reset();
}

Arena_Season_Result::~Arena_Season_Result(void){

}

void Arena_Season_Result::reset(void){
	player_id = 0;
	level = 0;
	group_index = 0;
	rank = 0;
	inner_rank = 0;
}

int Arena_Season_Result::serialize(Block_Buffer &buffer) const{
	buffer.write_int64(player_id);
	buffer.write_int32(level);
	buffer.write_int32(group_index);
	buffer.write_int32(rank);
	buffer.write_int32(inner_rank);
	return 0;
}
int Arena_Season_Result::deserialize(Block_Buffer &buffer){
	buffer.read_int64(player_id);
	buffer.read_int32(level);
	buffer.read_int32(group_index);
	buffer.read_int32(rank);
	buffer.read_int32(inner_rank);
	return 0;
}
///////////////////////////////////////////////////////////////////////

Arena_Season_Detail::Arena_Season_Detail(void){
	reset();
}

Arena_Season_Detail::~Arena_Season_Detail(void){

}

void Arena_Season_Detail::reset(void){
	season_status = 0;
	season_begin = Time_Value::zero;
	season_finish = Time_Value::zero;
	season_result_map.clear();
}

int Arena_Season_Detail::serialize(Block_Buffer &buffer) const{
	int result_num = season_result_map.size();
	int begin_time = season_begin.sec();
	int finish_time = season_finish.sec();

	buffer.write_int8(season_status);
	buffer.write_int32(begin_time);
	buffer.write_int32(finish_time);
	buffer.write_int32(result_num);
	Arena_Season_Result_Map::const_iterator it;
	for(it = season_result_map.begin(); it != season_result_map.end(); ++it){
		it->second.serialize(buffer);
	}
	return 0;
}

int Arena_Season_Detail::deserialize(Block_Buffer &buffer){
	int result_num = 0;
	int begin_time = 0;
	int finish_time = 0;

	buffer.read_int8(season_status);
	buffer.read_int32(begin_time);
	buffer.read_int32(finish_time);
	buffer.read_int32(result_num);
	season_begin.sec(begin_time);
	season_finish.sec(finish_time);
	for(int i = 0; i < result_num; ++i){
		Arena_Season_Result info;
		info.deserialize(buffer);
		season_result_map.insert(std::make_pair(info.player_id, info));
	}
	return 0;
}

int Arena_Season_Detail::load(void){
	return 0;
}

int Arena_Season_Detail::save(void){
	Block_Buffer buf;
	buf.make_message(MANGO_SAVE_ARENA_SEASON_DATA);
	this->serialize(buf);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 2);
	return 0;
}

void Arena_Season_Detail::clear_season_rank(const role_id_t role_id){
	Arena_Season_Result_Map::const_iterator it = season_result_map.find(role_id);
	if(it != season_result_map.end()){
		season_result_map.erase(it);
		this->detail_change();
		this->save();
	}
}

int Arena_Season_Detail::get_season_rank(const role_id_t role_id) const{
	Arena_Season_Result_Map::const_iterator it = season_result_map.find(role_id);
	if(it != season_result_map.end()){
		if(it->second.inner_rank == 0){
			return it->second.rank;
		}else{
			return it->second.inner_rank;
		}

	}
	return 0;
}

void Arena_Season_Detail::get_season_result(const role_id_t role_id, int &rank, int &group_index, int &level){
	Arena_Season_Result_Map::const_iterator it = season_result_map.find(role_id);
	if(it != season_result_map.end()){
		if(it->second.inner_rank == 0){
			rank = it->second.rank;
		}else{
			rank = it->second.inner_rank;
		}
		group_index = it->second.group_index;
		level = it->second.level;
	}
}

//////////////////////////////////////////////////////////////////////

Player_Rank_Info::Player_Rank_Info(void){
	reset();
}

void Player_Rank_Info::reset(void){
	role_id = 0;
	role_name = "";
	server_name = "";
	level = 0;
	career = 0;
	gender = 0;
	head_id = 0;
	fight = 0;
	score = 0;
	group = 1;
	seg = 1;
	rank = 0;
	inner_rank = 0;
	king_time = 0;
	fight_times = 0;
	be_worship = 0;
}

void Player_Rank_Info::season_reset(void){
	score = 0;
	group = 1;
	seg = 1;
	rank = 0;
	inner_rank = 0;
	king_time = 0;
	fight_times = 0;
}

bool operator>(const Player_Rank_Info &detail1 , const Player_Rank_Info &detail2){
	if(detail1.king_time > detail2.king_time){
		return true;
	}else if(detail1.king_time < detail2.king_time){
		return false;
	}else{
		if(detail1.group > detail2.group){
			return true;
		}else if( detail1.group < detail2.group){
			return false;
		}else{
			if(detail1.seg > detail2.seg){
				return true;
			}else if(detail1.seg < detail2.seg){
				return false;
			}else{
				if(detail1.score > detail2.score){
					return true;
				}else if(detail1.score < detail2.score){
					return false;
				}else{
					if(detail1.fight > detail2.fight){
						return true;
					}
				}
			}
		}
	}
	return false;
}

int Player_Rank_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(role_name);
	buffer.write_string(server_name);
	buffer.write_int16(level);
	buffer.write_int32(fight);
	buffer.write_int16(score);
	buffer.write_int8(group);
	buffer.write_int8(seg);
	buffer.write_int32(king_time);
	buffer.write_int16(fight_times);
	return 0;
}

int Player_Rank_Info::deserialize(Block_Buffer &buffer) {
	int16_t tmp_level = 0;
	int16_t tmp_score = 0;
	int8_t tmp_group = 0;
	int8_t tmp_seg = 0;
	int16_t tmp_fight_times = 0;
	buffer.read_int64(role_id);
	buffer.read_string(role_name);
	buffer.read_string(server_name);
	buffer.read_int16(tmp_level);
	buffer.read_int32(fight);
	buffer.read_int16(tmp_score);
	buffer.read_int8(tmp_group);
	buffer.read_int8(tmp_seg);
	buffer.read_int32(king_time);
	buffer.read_int16(tmp_fight_times);
	level = tmp_level;
	score = tmp_score;
	group = tmp_group;
	seg = tmp_seg;
	fight_times = tmp_fight_times;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Arena_Reward_Info::Arena_Reward_Info(void){
	reset();
}

void Arena_Reward_Info::reset(void){
	rank = 0;
	rank_time = Time_Value::zero;
}

int Arena_Reward_Info::serialize(Block_Buffer &buffer) const{
	buffer.write_int32(rank);
	buffer.write_int32(rank_time.sec());
	return 0;
}
int Arena_Reward_Info::deserialize(Block_Buffer &buffer){
	int time = 0;
	if( buffer.read_int32(rank)  ||  buffer.read_int32(time)){
		return -1;
	}
	rank_time.sec(time);
	return 0;
}

////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////
Arena_Match_Status::Arena_Match_Status(void):refresh(0){

}

Arena_Match_Status::Arena_Match_Status(Time_Value time, std::vector<role_id_t> match_id){
	refresh = 0;
	match_time = time + Time_Value(60);
	int count = match_id.size();
	for(int i = 0; i < count; ++i){
		match_players.push_back(Arena_Load_Status(match_id[i]));
	}
}

Arena_Match_Status::Arena_Match_Status(Time_Value time, std::vector<Arena_Load_Status> match_status, int8_t ref/* = 0*/){
	refresh = ref;
	match_time = time + Time_Value(60);
	match_players = match_status;
}

void Arena_Match_Status::reset(void){
	match_time = Time_Value::zero;
	CLEAR_VECTOR(std::vector<Arena_Load_Status>, match_players);
}

bool Arena_Match_Status::is_timeout(const Time_Value &now){
	if(now > match_time){
		return true;
	}
	return false;
}

bool Arena_Match_Status::is_complete(void){
	std::vector<Arena_Load_Status>::iterator it;
	for(it = match_players.begin(); it != match_players.end(); ++it){
		if(it->is_complete() == false){
			return false;
		}
	}
	return true;
}

void Arena_Match_Status::load_complete(const role_id_t id){
	std::vector<Arena_Load_Status>::iterator it;
	for(it = match_players.begin(); it != match_players.end(); ++it){
		it->load_complete(id);
	}
}

int Arena_Match_Status::get_match_pos(const role_id_t match_id){
	int pos = 0;
	std::vector<Arena_Load_Status>::iterator it;
	for(it = match_players.begin(); it != match_players.end(); ++it){
		if(it->role_id == match_id){
			return pos;
		}
		pos += 1;
	}
	return -1;
}


////////////////////////////////////////////////////////////////////////////////////////

Arena_Seg::Arena_Seg(void){
	reset();
}

void Arena_Seg::reset(void){
	seg_key = 0;
	seg_set.clear();
	force_set_map.clear();
}

void Arena_Seg::add_player(role_id_t role_id, int force_lv){
	force_set_map[force_lv].insert(role_id);
	seg_set.insert(role_id);
}

void Arena_Seg::del_player(role_id_t role_id, int force_lv){
	force_set_map[force_lv].erase(role_id);
	seg_set.erase(role_id);
}

void Arena_Seg::force_change(role_id_t role_id, int old_force_lv, int new_force_lv){
	force_set_map[old_force_lv].erase(role_id);
	force_set_map[new_force_lv].insert(role_id);
	seg_set.insert(role_id);
}

///////////////////////////////////////////////////////////////////////////////////


Arena_Match_Player_Data::Arena_Match_Player_Data(void){

}

Arena_Match_Player_Data::~Arena_Match_Player_Data(void){

}

void Arena_Match_Player_Data::reset(void){
	base_detail.reset();
	fighter_detail.reset();
	heroer_detail.reset();
}

/////////////////////////////////////////////////////////////////////////////////

Arena_Match_P::Arena_Match_P(void){
	reset();
}

Arena_Match_P::Arena_Match_P(const role_id_t r_id, const int8_t d_lv):role_id(r_id), difficult_lv(d_lv){

}

Arena_Match_P::~Arena_Match_P(void){

}

void Arena_Match_P::reset(void){
	role_id = 0;
	difficult_lv = 0;
}
