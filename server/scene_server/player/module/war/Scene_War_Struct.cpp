/*
 * Scene_War_Struct.cpp
 *
 *  Created on: 2014年7月11日
 *      Author: root
 */

#include "Scene_War_Struct.h"

const int WAR_INACTIVE_KICK_TIME = 300;

Scene_War_Rank_Info::Scene_War_Rank_Info(void){
	reset();
}

void Scene_War_Rank_Info::reset(void){
	role_id = 0;
	role_name = "";
	server_name = "";
	league = 0;
	score = 100;
	total_score = 0;
	src_hp = 0;
	hp =0;
	hero_hp_map.clear();
	kill_num = 0;
	die_num = 0;
	assists_num = 0;
	streak_win_num = 0;
	max_streak_win_num = 0;
	add_streak_lose_num = false;
	streak_lose_num = 0;
	merit_coe = 100;
	merit = 0;
	total_merit = 0;
	chest_pieces_coe = 100;
	chest_pieces = 0;
	rep_coe = 100;
	rep = 0;
	arena_coin = 0;
	is_pull_mine = 0;
	heal_num = 0;
	last_attack_role_id = 0;
	revive = 0;
	revive_time = Time_Value::zero;
	safe_time = Time_Value::zero;
	offline_kick_time = Time_Value::zero;
	inactive_kick_time = Time_Value::zero;
	notice_kick_time = false;
}

int Scene_War_Rank_Info::get_hero_hp(role_id_t role_id){
	Hero_Hp_Map::iterator it = hero_hp_map.find(role_id);
	if(it != hero_hp_map.end()){
		return it->second;
	}
	return 0;
}

int Scene_War_Rank_Info::add_score(int score){
	this->score += score;
	this->total_score += score;
	inactive_kick_time = Time_Value::gettimeofday() + Time_Value(WAR_INACTIVE_KICK_TIME);
	notice_kick_time = true;
	return score;
}

int Scene_War_Rank_Info::dec_score(int score){
	this->score -= score;
	if(this->score < 0){
		this->score = 0;
	}
	inactive_kick_time = Time_Value::gettimeofday() + Time_Value(WAR_INACTIVE_KICK_TIME);
	notice_kick_time = true;
	return score;
}

int Scene_War_Rank_Info::add_merit(int merit){
	int add_merit = (double)(merit*this->merit_coe)/100.0+0.5;
	if(this->total_merit + add_merit > MAX_WAR_SCENE_MERIT){
		add_merit = MAX_WAR_SCENE_MERIT - this->total_merit;
	}
	if(add_merit < 0){
		add_merit = 0;
	}
	this->merit += add_merit;
	this->total_merit += add_merit;
	return add_merit;
}

int Scene_War_Rank_Info::add_piece(int piece){
	int add_piece = (double)(piece*this->chest_pieces_coe)/100.0+0.5;
	this->chest_pieces += add_piece;
	return add_piece;
}

int Scene_War_Rank_Info::add_rep(int rep){
	int add_rep = (double)(rep*this->rep_coe)/100.0+0.5;
	this->rep += add_rep;
	return add_rep;
}

int Scene_War_Rank_Info::add_arean_coin(int value){
	int add_value = (double)(value*this->chest_pieces_coe)/100.0+0.5;
	this->arena_coin += add_value;
	return add_value;
}

/////////////////////////////////////////////////////////////////////////////////

Honour_Player::Honour_Player(void){
	reset();
}

void Honour_Player::reset(void){
	role_id = 0;
	level = 0;
	force = 0;
	gang_id = 0;
	total_merit = 0;
	remain_income_num = 0;
	income_rate = 0;
	league = 0;
	remain_time = 0;
	act_time = 0;
	server_name = "";
	honor_field_id = 0;
}

////////////////////////////////////////////////////////////////////////////////////

Honour_Field::Honour_Field(void){
	reset();
}

void Honour_Field::reset(void){
	id = 0;
	remain_time = 0;
	act_time = 0;
	zone_id = 0;
	field_player_num = 0;
	for(int i=0; i<LEAGUE_COUNT; ++i){
		league_num[i] = 0;
		league_force[i] = 0;
		league_gang[i].clear();
	}
	player_set.clear();
	enter_weight = 0;
	enter_league = 0;
}

int Honour_Field::add_player(Honour_Player* honour_player){
	if(honour_player->league < 0 || honour_player->league >= LEAGUE_COUNT){
		return -1;
	}
	player_set.insert(honour_player->role_id);
	field_player_num += 1;
	league_num[honour_player->league] += 1;
	league_force[honour_player->league] += honour_player->force;
	if(honour_player->gang_id != 0){
		league_gang[honour_player->league].insert(honour_player->gang_id);
	}
	return 0;
}

int Honour_Field::del_player(Honour_Player* honour_player){
	if(honour_player->league < 0 || honour_player->league >= LEAGUE_COUNT){
		return -1;
	}
	player_set.erase(honour_player->role_id);
	field_player_num -= 1;
	league_num[honour_player->league] -= 1;
	league_force[honour_player->league] -= honour_player->force;
	if(honour_player->gang_id != 0){
		league_gang[honour_player->league].erase(honour_player->gang_id);
	}
	return 0;
}

bool Honour_Field::check_enter_weight(const Honour_Player* honour_player){
	this->enter_weight = 0;
	if(field_player_num >= MAX_WAR_SCENE_PLAYER_NUM){
		return false;
	}
	int belong_side = -1;
	if(honour_player->gang_id != 0){
		for(int i=0; i<LEAGUE_COUNT; ++i){
			if(league_gang[i].count(honour_player->gang_id) > 0){
				if(league_num[i] < MAX_WAR_SCENE_PLAYER_NUM/2){
					belong_side = i;
				}
				break;
			}
		}
	}
	if(belong_side != -1){
		this->enter_weight += WEIGHT_GANG;
		if(league_num[belong_side] < league_num[(belong_side+1)%LEAGUE_COUNT]){
			this->enter_weight += WEIGHT_SIDE;
		}
		if(league_force[belong_side] < league_force[(belong_side+1)%LEAGUE_COUNT]){
			this->enter_weight += WEIGHT_FORCE;
		}
		this->enter_league = belong_side;
	}else{
		if(league_num[0] > league_num[1]){
			belong_side = 1;
			this->enter_weight += WEIGHT_SIDE;
		}else if(league_num[0] < league_num[1]){
			belong_side = 0;
			this->enter_weight += WEIGHT_SIDE;
		}else{
			if(league_force[0] <= league_force[1]){
				belong_side = 0;
				this->enter_weight += WEIGHT_FORCE;
			}else{
				belong_side = 1;
				this->enter_weight += WEIGHT_FORCE;
			}
		}
		this->enter_league = belong_side;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

Honour_Zone::Honour_Zone(void){
	reset();
}

void Honour_Zone::reset(void){
	id = 0;
	zone_player_num = 0;
	honour_field_p_map.clear();
}

int Honour_Zone::add_player(Honour_Player* honour_player){
	Honour_Field_P_Map::iterator it = honour_field_p_map.find(honour_player->honor_field_id);
	if(it != honour_field_p_map.end()){
		this->zone_player_num += 1;
		return it->second->add_player(honour_player);
	}
	return -1;
}

int Honour_Zone::del_player(Honour_Player* honour_player){
	Honour_Field_P_Map::iterator it = honour_field_p_map.find(honour_player->honor_field_id);
	if(it != honour_field_p_map.end()){
		this->zone_player_num -= 1;
		return it->second->del_player(honour_player);
	}
	return -1;
}

int Honour_Zone::add_field(Honour_Field* honour_field){
	honour_field->zone_id = this->id;
	this->honour_field_p_map.insert(std::make_pair(honour_field->id, honour_field));
	this->zone_player_num += honour_field->field_player_num;
	return 0;
}

int Honour_Zone::del_field(Honour_Field* honour_field){
	this->zone_player_num -= honour_field->field_player_num;
	this->honour_field_p_map.erase(honour_field->id);
	return 0;
}

Honour_Field* Honour_Zone::get_best_honour_field(Honour_Player* honour_player){
	int best_weight = 0;
	role_id_t best_honour_field = 0;
	Honour_Field* h_field = NULL;
	Honour_Field_P_Map::iterator it;
	for(it = honour_field_p_map.begin(); it != honour_field_p_map.end(); ++it){
		it->second->check_enter_weight(honour_player);
		if( it->second->enter_weight > best_weight){
			best_weight = it->second->enter_weight;
			best_honour_field = it->second->id;
			h_field = it->second;
		}
	}
	return h_field;
}

//////////////////////////////////////////////////////////////////////////////////
