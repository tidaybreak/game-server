/*
 * Arena_Manager.cpp
 *
 *  Created on: 2014年5月16日
 *      Author: lys
 */

#include "Arena_Manager.h"
#include "DB_Operator.h"
#include "Msg_Struct.h"
#include "Logic_Player.h"
#include "Err_Code.h"
#include "Msg_Arena_Struct.h"
#include "Pool_Manager.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Arena.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Pool_Manager.h"
#include "DB_Def.h"
#include "Logic_Player_Detail.h"
#include <time.h>
#include "helper/Helper_Def.h"
#include "vip/Vip_Def.h"
#include "announcement/Announcement_Struct.h"
#include "Config_Cache.h"

const int ARENA_RISE_MAX = 2;
const int ARENA_MAX_SEG = 5;
const int ARENA_MAX_GROUP = 6;
const int ARENA_ELITE_RANK = 20;

const int REANA_KING_TIME_MAX = 1726675200;

const int ARENA_INNER_SCENE_ID = 80207001;

const int MIN_ARENA_SEG_KEY = 1;
const int MAX_ARENA_SEG_KEY = 26;

const int MIN_ARENA_INNER_SYNC_GROUP = 3;

const int ARENA_LOW_FORCE_FIGHT_TIMES = 3;//竞技场头3场

const int MAX_ARENA_MATCH_RD_NUM = 100;


const int NORMAL_MATCH_MIN_NUM = 5;

const int RISE_MATCH_MIN_NUM = 3;

Arena_Manager::Arena_Manager(void):rank_change_(false),
		sync_to_inner_scene_(false), logic_uuid_(0){

}

Arena_Manager::~Arena_Manager(void){

}

void Arena_Manager::reset(void){
	logic_uuid_ = 0;
}

int Arena_Manager::start(void){
	Monitor_Mark_Vec muid_vec;
	CONFIG_CACHE->server_solution_cache().fetch_muid_single_monitor(muid_vec, Server_Enum::MONITOR_LOGIC);
	if(muid_vec.empty() == false){
		logic_uuid_ = muid_vec[0].uuid;
	}
	this->load_all_arena_db_info();
	Time_Value now = Time_Value::gettimeofday();
	if(arena_season_detail_.season_status == ARENA_BATTLE_STATUS_NULL){
		//赛季结束时间 每月1号，6点
		arena_season_detail_.season_begin = this->get_season_begin_time(now);
		arena_season_detail_.season_finish = this->get_season_finish_time(now);
		arena_season_detail_.season_status = ARENA_BATTLE_STATUS_RUNNING;
		arena_season_detail_.save();
		init_all_arena_player();
		sort_rank(true);
		int count = (now.sec() - arena_season_detail_.season_begin.sec())/(Time_Value::ONE_DAY_IN_SECS*3);
		inner_award_time_ = arena_season_detail_.season_begin + Time_Value(Time_Value::ONE_DAY_IN_SECS*3*count) + Time_Value(60*60*24*7, 0);
		Time_Value timeout = next_relative_time(22, 00, 00);
		local_award_time_ = now + timeout;
	}else if(arena_season_detail_.season_status == ARENA_BATTLE_STATUS_RUNNING){
		init_all_arena_player();
		sort_rank(true);
		int count = (now.sec() - arena_season_detail_.season_begin.sec())/(Time_Value::ONE_DAY_IN_SECS*3);
		inner_award_time_ = arena_season_detail_.season_begin + Time_Value(Time_Value::ONE_DAY_IN_SECS*3*count) + Time_Value(60*60*24*7, 0);
		Time_Value timeout = next_relative_time(22, 00, 00);
		local_award_time_ = now + timeout;
	}else if(arena_season_detail_.season_status == ARENA_BATTLE_STATUS_COMING){
		Time_Value now = Time_Value::gettimeofday();
		if(arena_season_detail_.season_begin > now){
			arena_season_detail_.season_status = ARENA_BATTLE_STATUS_RUNNING;
			arena_season_detail_.save();
			init_all_arena_player();
			sort_rank(true);
			int count = (now.sec() - arena_season_detail_.season_begin.sec())/(Time_Value::ONE_DAY_IN_SECS*3);
			inner_award_time_ = arena_season_detail_.season_begin + Time_Value(Time_Value::ONE_DAY_IN_SECS*3*count) + Time_Value(60*60*24*7, 0);
			Time_Value timeout = next_relative_time(22, 00, 00);
			local_award_time_ = now + timeout;
		}
	}
	refresh_time_ = Time_Value::gettimeofday() + Time_Value(5,0);
	return 0;
}

int Arena_Manager::tick(const Time_Value& now){
	if(arena_season_detail_.season_status == ARENA_BATTLE_STATUS_COMING){
		if(now > arena_season_detail_.season_begin){
			arena_season_detail_.season_status = ARENA_BATTLE_STATUS_RUNNING;
			arena_season_detail_.save();
			init_all_arena_player();
			sort_rank(true);
			int count = (now.sec() - arena_season_detail_.season_begin.sec())/(Time_Value::ONE_DAY_IN_SECS*3);
			inner_award_time_ = arena_season_detail_.season_begin + Time_Value(Time_Value::ONE_DAY_IN_SECS*3*count) + Time_Value(60*60*24*7, 0);
			Time_Value timeout = next_relative_time(22, 00, 00);
			local_award_time_ = now + timeout;
		}
	}else if(arena_season_detail_.season_status == ARENA_BATTLE_STATUS_RUNNING){
		if(now > arena_season_detail_.season_finish){// 赛季结束
			season_finish();
		}
	}

	if(this->is_arena_running() == false){
		return 0;
	}
	if(sync_to_inner_scene_){// 同步到跨服
		this->sync_all_to_inner_scene();
		sync_to_inner_scene_ = false;
	}
	if(rank_change_){
		if(now > refresh_time_){
			rank_change_ = false;
			Arena_Rank_Player_Map::iterator it;
			for(it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it){
				Logic_Player* logic_player = it->second->logic_player();
				if(logic_player){
					logic_player->notice_local_rank();
				}
			}
		}
	}
	check_match_status(now);
	return 0;
}

int Arena_Manager::trigger_daily_twenty(const Time_Value& now){
	if(this->is_arena_running() == false){
		return 0;
	}
	this->three_day_refresh(now);
	// 竞技场奖励发布广播
	role_id_t first_role_id = 0;
	std::string first_role_name = "";
	get_local_first(first_role_id, first_role_name);
	if(first_role_id != 0){
		this->make_announce(ARENA_ANNOUNCE_ID_LOCAL_AWARD, first_role_id, first_role_name);
	}
	return 0;
}

void Arena_Manager::daily_refresh(bool check_one_day/* = false*/){
	if(this->is_arena_running() == false){
		return;
	}
	//check 3 day
	Time_Value now = Time_Value::gettimeofday();
   if(now > local_award_time_){//three day
		this->three_day_refresh(now);
	}
}

void Arena_Manager::sign_in(Logic_Player* player){
	if(this->is_arena_running() == false){
		return;
	}
	if(player == NULL){
		return;
	}
	int arena_open_lv = CONFIG_CACHE_ARENA->get_arena_open_lv();
	if(player->level() < arena_open_lv){
		return;
	}
	Arena_Rank_Player* rank_player = this->find_rank_player(player->role_id());
	if(rank_player == NULL){
		rank_player = this->get_rank_player(player->role_id());
		if(rank_player == NULL){
			return;
		}
		rank_player->set_role_name(player->role_name());
		rank_player->set_level(player->level());
		rank_player->set_career(player->career());
		rank_player->set_gender(player->gender());
		rank_player->set_head_id(player->head_id());
		rank_player->rank_info_.group = 1;
		rank_player->rank_info_.seg = 1;
		rank_player->rank_info_.score = 0;
		rank_player->rank_info_.fight_times = 0;
		rank_player->rank_info_.fight = player->fighter_detail().ttl_force;
		rank_player->rank_info_.fight_times = player->arenaer_detail()->be_worship;
		player->arenaer_detail()->group = 1;
		player->arenaer_detail()->seg = 1;
		player->arenaer_detail()->score = 0;
		player->arenaer_detail()->fight_times = 0;
//		player->arenaer_detail()->be_worship = 0;
		player->arenaer_detail()->detail_change();

		int seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
		int force = rank_player->fight();
		this->arena_seg_add(player->role_id(), seg_key, force);
		// 战斗过的参与排名
		int fight_times = rank_player->rank_info_.fight_times;
		if(fight_times > 0){
			int rank = this->get_rank_player_size() + 1;
			rank_player->set_local_rank(rank);
			player->arenaer_detail()->rank = rank_player->local_rank();
			player->arenaer_detail()->detail_change();
			if(ranking_set_.count(rank_player->role_id()) <= 0){
				ranking_set_.insert(rank_player->role_id());
				this->ranking_vec_.push_back(rank_player);
			}
		}
	}else{
		rank_player->set_role_name(player->role_name());
		rank_player->set_level(player->level());
		rank_player->set_career(player->career());
		rank_player->set_gender(player->gender());
		rank_player->set_head_id(player->head_id());
		int old_force = rank_player->fight();
		int old_seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
		int old_fight_times = rank_player->rank_info_.fight_times;
		if(player->arenaer_detail()->rank != rank_player->local_rank()){
			player->arenaer_detail()->rank = rank_player->local_rank();
			player->arenaer_detail()->detail_change();
		}
		if(player->arenaer_detail()->inner_rank != rank_player->inner_rank()){
			player->arenaer_detail()->inner_rank = rank_player->inner_rank();
			player->arenaer_detail()->detail_change();
		}
		if(rank_player->rank_info_.group != player->arenaer_detail()->group){
			player->arenaer_detail()->group = rank_player->rank_info_.group;
			player->arenaer_detail()->detail_change();
		}
		if(rank_player->rank_info_.seg != player->arenaer_detail()->seg){
			player->arenaer_detail()->seg = rank_player->rank_info_.seg;
			player->arenaer_detail()->detail_change();
		}
		if(rank_player->rank_info_.score != player->arenaer_detail()->score){
			player->arenaer_detail()->score = rank_player->rank_info_.score;
			player->arenaer_detail()->detail_change();
		}
		if(rank_player->rank_info_.fight_times != player->arenaer_detail()->fight_times){
			player->arenaer_detail()->fight_times = rank_player->rank_info_.fight_times;
			player->arenaer_detail()->detail_change();
		}
		rank_player->rank_info_.fight = player->fighter_detail().ttl_force;

		int new_force = rank_player->fight();
		int new_seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
		int new_fight_times = rank_player->rank_info_.fight_times;
		this->arena_seg_change(rank_player->role_id(), old_seg_key, new_seg_key, old_force, new_force);
		// 战斗过的参与排名
		if(old_fight_times == 0 && new_fight_times > 0){
			if(ranking_set_.count(rank_player->role_id()) <= 0){
				ranking_set_.insert(rank_player->role_id());
				this->ranking_vec_.push_back(rank_player);
			}
		}
	}
	// 同步到跨服
	this->sync_one_to_inner_scene(rank_player);
	if(rank_player->rank_info_.fight_times > 0){
		this->sort_rank();
	}
}

void Arena_Manager::sign_out(Logic_Player* player){

}

void Arena_Manager::hook_fighter_force(role_id_t role_id, int new_force){

}

void Arena_Manager::hook_fighter_ttl_force(role_id_t role_id, int new_force){
	Arena_Rank_Player* rank_player = find_rank_player(role_id);
	if(rank_player){
		int old_force = rank_player->fight();
		int seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
		rank_player->set_fight(new_force);
		this->arena_seg_change(rank_player->role_id(), seg_key, seg_key, old_force, new_force);
		this->sync_one_to_inner_scene(rank_player);
		if(rank_player->rank_info_.fight_times > 0){
			ARENA_MANAGER->sort_rank();
		}
	}
}

//for test
void Arena_Manager::start_arena_battle(void){
	return;
}

void Arena_Manager::refresh_1_debug(void){
	if(this->is_arena_running() == false){
		return;
	}
	Time_Value now = Time_Value::gettimeofday();
	this->sort_rank();
	this->one_day_refresh(now);
}

void Arena_Manager::refresh_3_debug(void){
	if(this->is_arena_running() == false){
		return;
	}
	Time_Value now = Time_Value::gettimeofday();
	this->three_day_refresh(now);
}
void Arena_Manager::refresh_7_debug(void){
	if(this->is_arena_running() == false){
		return;
	}
}

void Arena_Manager::set_grou_seg(Logic_Player* logic_player, int group , int seg){
	if(this->is_arena_running() == false){
		return;
	}
	Arena_Rank_Player* rank_player = this->find_rank_player(logic_player->role_id());
	if(rank_player){// 进入王者组时间参数
		if(rank_player->group() == ARENA_MAX_GROUP){
			rank_player->set_king_time(0);
		}
		if(group == ARENA_MAX_GROUP){			// 进入王者组时间参数
			Time_Value time = Time_Value::gettimeofday();
			int king_time = REANA_KING_TIME_MAX - time.sec();
			rank_player->set_king_time(king_time);
		}

		int old_seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
		int force = rank_player->fight();

		rank_player->set_group_seg(group, seg);
		logic_player->set_group_seg(group,seg);

		int new_seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
		this->arena_seg_change(rank_player->role_id(), old_seg_key, new_seg_key, force,  force);
		if(rank_player->rank_info_.fight_times > 0){
			sort_rank();
		}
		logic_player->notice_arena_info();
		this->sync_one_to_inner_scene(rank_player);
	}
}

void Arena_Manager::set_score_debug(Logic_Player* logic_player, int score){
	if(this->is_arena_running() == false){
		return;
	}
	Arena_Rank_Player* rank_player = this->find_rank_player(logic_player->role_id());
	if(rank_player){
		if(score >= 120){
			score = 120;
		}
		rank_player->set_score(score);
		sort_rank();
		logic_player->notice_arena_info();
		this->sync_one_to_inner_scene(rank_player);
	}
}
void Arena_Manager::save_season_rank(void){
	arena_season_detail_.season_result_map.clear();
	Arena_Ranking_Vec::const_iterator it;
	for(it = ranking_vec_.begin(); it != ranking_vec_.end(); ++it){
		Arena_Season_Result info;
		info.player_id = (*it)->role_id();
		info.level = (*it)->level();
		int group_index = (*it)->group()*10+(*it)->seg();
		info.group_index = group_index;
		info.rank = (*it)->local_rank();
		info.inner_rank = (*it)->inner_rank();
		arena_season_detail_.season_result_map.insert(std::make_pair(info.player_id, info));
		Logic_Player* l_player = (*it)->logic_player();
		if(l_player){
			l_player->notice_award_info();
		}
	}
}
void Arena_Manager::season_clear_data(void){
	//清空数据库数据
	Block_Buffer buf;
	buf.make_message(MANGO_CLAER_ARENA_PLAYER_DATA);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 1);
	// 在线玩家清空竞技场数据
	LOGIC_MONITOR->arena_season_clear_data();
	//
	Arena_Rank_Player_Map::iterator it;
	for(it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it){
		Arena_Rank_Player* rank_player = it->second;
		rank_player->rank_info_.season_reset();
	}
	ranking_vec_.clear();
	ranking_set_.clear();
	ranked_map_.clear();
	arena_seg_map_.clear();
	ready_player_set_.clear();
	inner_load_player_map_.clear();
	arena_match_status_map_.clear();
	arena_ready_status_map_.clear();
}

Time_Value Arena_Manager::get_season_begin_time(const Time_Value& now){
	Date_Time date_now(now);
	Date_Time date_des(date_now);
	date_des.day(1);
	date_des.hour(6);
	date_des.minute(0);
	date_des.second(0);
	return Time_Value(date_des.time_sec());
}

Time_Value Arena_Manager::get_season_finish_time(const Time_Value& now){
	Date_Time date_now(now);
	Date_Time date_des(date_now);
	long int year = date_now.year();
	long int month = date_now.month();
	if(month == 12){
		year += 1;
		month = 1;
	}else{
		month += 1;
	}
	date_des.year(year);
	date_des.month(month);
	date_des.day(1);
	date_des.hour(6);
	date_des.minute(0);
	date_des.second(0);
	return Time_Value(date_des.time_sec());
}

int Arena_Manager::one_day_refresh(const Time_Value& now){
	//clear free_times
	Arena_Rank_Player_Map::iterator it;
	for(it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it){
		Logic_Player* logic_player = it->second->logic_player();
		if(logic_player){
			//补偿剩余次数
			int compensate_times = logic_player->compensate_times();
			int total_num;
			int enter_num;
			logic_player->get_arena_enter_num_info(total_num, enter_num);
			int free_times = total_num - enter_num;	//
			if(free_times < 0){
				free_times = 0;
			}
			compensate_times += free_times;
			if(compensate_times > 10){
				compensate_times = 10;
			}
			logic_player->set_compensate_times(compensate_times);
			logic_player->set_enter_num(0);
			logic_player->set_pay_times(0);
			logic_player->set_buy_pay_times(0);
			//notice client
			logic_player->notice_arena_info();
		}
	}
	return 0;
}

int Arena_Manager::three_day_refresh(const Time_Value& now){
	this->sort_rank();
	//int count = (now.sec() - arena_battle_detail_->begin_time.sec())/(Time_Value::ONE_DAY_IN_SECS*3);
	//arena_battle_detail_->ref_time1 = arena_battle_detail_->begin_time + Time_Value(Time_Value::ONE_DAY_IN_SECS*3*count) + Time_Value(60*60*24*3, 0);
	local_award_time_ = now + Time_Value(Time_Value::ONE_DAY_IN_SECS);
	//clear free_times
	Arena_Rank_Player_Map::iterator it;
	for(it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it){
		three_day_award(it->second, now);
	}
	return 0;
}

int Arena_Manager::three_day_award(Arena_Rank_Player* rank_player, const Time_Value& now){
	Logic_Player* logic_player = rank_player->logic_player();
	if(logic_player){
		int three_day_rank = logic_player->three_day_rank();
		if(three_day_rank != 0){
			logic_player->email_three_day_reward(three_day_rank, now);
		}
		logic_player->set_three_day_rank(rank_player->local_rank());
		logic_player->notice_award_info();
	}else{
		Arena_Reward_Info info;
		info.rank = rank_player->local_rank();
		info.rank_time = now;
		Logic_Player::offline_arena_three_day_reward(rank_player->role_id(), info);
	}
	return 0;
}

void Arena_Manager::sort_rank(bool first_tims/* = false*/){
	if(ranking_vec_.empty()){
		return;
	}
	ranked_map_.clear();
	//group_rank_map_.clear();
	std::sort(ranking_vec_.begin(), ranking_vec_.end(), Rank_Greater);
	Arena_Ranking_Vec::iterator it;
	int rank = 0;
	for(it = ranking_vec_.begin(); it != ranking_vec_.end(); ++it){
		rank += 1;
		if((*it)->local_rank() != rank){
			(*it)->set_local_rank(rank);
		}
		ranked_map_.insert(std::make_pair((*it)->local_rank(), (*it)));
	}
	if(!rank_change_){
		rank_change_ = true;
		refresh_time_  = Time_Value::gettimeofday() + Time_Value(6);
	}
}

void Arena_Manager::check_battle_start(const Time_Value& now){
//	Ready_Player_Set::iterator it;
//	for(it = ready_player_set_.begin(); it != ready_player_set_.end(); ++it){
//		if((*it)->check_battle_start_time(now)){
//			//this->battle_start((*it), (*it)->get_match_player());
//			ready_player_set_.erase(it);
//			break;
//		}
//	}
}

int Arena_Manager::battle_start(role_id_t fight_id, role_id_t match_id, std::string match_role_name, int8_t difficult_lv){
	Arena_Rank_Player* fight_player = find_rank_player(fight_id);
	if(fight_player == NULL){
		return -1;
	}
	Logic_Player* player = fight_player->logic_player();
	if(player == NULL){
		return -1;
	}
	fight_player->set_match_id(match_id);
	fight_player->set_match_role_name(match_role_name);
	fight_player->set_match_difficult_lv(difficult_lv);
	Logic_Player *match_logic_player = POOL_MANAGER->pop_logic_player();
	if (!match_logic_player) {
		MSG_DEBUG("logic_player_pool_.pop() return 0.");
		return -1;
	}
	Base_Detail tmp_base_detail;
	Fighter_Detail tmp_fighter_detail;
	Heroer_Detail tmp_heroer_detail;
	//online
	Logic_Player_Detail* match_data = NULL;
	Arena_Rank_Player* match_player = this->find_rank_player(match_id);
	if(match_player && match_player->logic_player()){
		match_data = match_player->logic_player()->logic_player_detail();
		if(match_data == NULL){
			POOL_MANAGER->push_logic_player(match_logic_player);
			MSG_DEBUG("a_m battle_start error% rank:%ld, match:%ld, match_data NULL", fight_id, match_id);
			return -1;
		}
		tmp_base_detail = match_data->base_detail;
		tmp_fighter_detail = match_data->fighter_detail;
		tmp_heroer_detail = match_data->heroer_detail;
//		match_logic_player->load_base_detail(match_data->base_detail);
//		match_logic_player->Logic_Fighter::load_detail(match_data->fighter_detail);
//		match_logic_player->Logic_Heroer::load_detail(match_data->heroer_detail);
	}else{
		Arena_Match_Player_Data* arena_match_player_data = player->get_arena_match_player_data(match_id);
		if(!arena_match_player_data){
			POOL_MANAGER->push_logic_player(match_logic_player);
			MSG_DEBUG("a_m battle_start error% rank:%ld, match:%ld, arena_match_player_data NULL", fight_id, match_id);
			return -1;
		}
		tmp_base_detail = arena_match_player_data->base_detail;
		tmp_fighter_detail = arena_match_player_data->fighter_detail;
		tmp_heroer_detail = arena_match_player_data->heroer_detail;
//		match_logic_player->load_base_detail(arena_match_player_data->base_detail);
//		match_logic_player->Logic_Fighter::load_detail(arena_match_player_data->fighter_detail);
//		match_logic_player->Logic_Heroer::load_detail(arena_match_player_data->heroer_detail);
	}
	match_logic_player->load_base_detail(tmp_base_detail);
	match_logic_player->Logic_Fighter::load_detail(tmp_fighter_detail);
	match_logic_player->Logic_Heroer::load_detail(tmp_heroer_detail);


	MSG_DEBUG("a_m battle_start% rank:%ld, match:%ld", fight_id, match_id);

	Block_Buffer sync_buff;
	sync_buff.make_message(INNER_ARENA_BATTLE_START);
	int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
	sync_buff.write_int32(inner_area);
	int8_t value;
	value = match_logic_player->career();
	sync_buff.write_int8(value);
	value = match_logic_player->gender();
	sync_buff.write_int8(value);
	sync_buff.write_string(match_role_name);

	int av_count = match_logic_player->base_detail().avatar_vec.size();
	sync_buff.write_int8(av_count);
	for (Int_Vec::const_iterator it = match_logic_player->base_detail().avatar_vec.begin();
			it != match_logic_player->base_detail().avatar_vec.end(); ++it) {
		sync_buff.write_int32(*it);
	}

	{	// 人物战斗信息
		if(fight_player->rank_info_.fight_times < ARENA_LOW_FORCE_FIGHT_TIMES){//前3场，对手减50%攻击
			Prop_Setter_Vec setter_vec;
			Prop_Setter prop;
			prop.prop_type = PT_PHY_ATTACK;
			prop.addi_type = AT_STATUS;
			prop.operate = O_SUB;
			prop.percent = 0.5;
			setter_vec.push_back(prop);

			prop.prop_type = PT_SPELL_ATTACK;
			setter_vec.push_back(prop);

			prop.prop_type = PT_BLOOD_MAX;
			prop.percent = 0.5;
			setter_vec.push_back(prop);

			prop.prop_type = PT_PHY_DEFENSE;
			prop.percent = 0.8;
			setter_vec.push_back(prop);

			prop.prop_type = PT_SPELL_DEFENSE;
			prop.percent = 0.8;
			setter_vec.push_back(prop);

			match_logic_player->fight_prop_modify(setter_vec, 207);
		}
		match_logic_player->fighter_detail().serialize(sync_buff);

	}

	{
		// 战斗英雄信息

		bool has_fight_hero = false;
		int fight_hero_index = match_logic_player->heroer_detail().fighter_index;
		Heroer_Detail::Hero_Map::const_iterator it_hero = match_logic_player->heroer_detail().hero_map.find(fight_hero_index);
		if (it_hero != match_logic_player->heroer_detail().hero_map.end()) {
			has_fight_hero = true;
			if(fight_player->rank_info_.fight_times < ARENA_LOW_FORCE_FIGHT_TIMES){//前3场，对手减50%攻击
				Prop_Setter prop;
				prop.prop_type = PT_PHY_ATTACK;
				prop.addi_type = AT_STATUS;
				prop.operate = O_SUB;
				prop.percent = 0.5;
				Prop_Setter_Vec setter_vec;
				setter_vec.push_back(prop);

				prop.prop_type = PT_SPELL_ATTACK;
				setter_vec.push_back(prop);

				prop.prop_type = PT_BLOOD_MAX;
				prop.percent = 0.5;
				setter_vec.push_back(prop);


				prop.prop_type = PT_PHY_DEFENSE;
				prop.percent = 0.8;
				setter_vec.push_back(prop);

				prop.prop_type = PT_SPELL_DEFENSE;
				prop.percent = 0.8;
				setter_vec.push_back(prop);

				match_logic_player->hero_fight_prop_modify(setter_vec);
			}
			sync_buff.write_int8(1);
			match_logic_player->refresh_heroer_base_detail();
			match_logic_player->heroer_detail().hero_base_detail_.serialize(sync_buff);
			match_logic_player->heroer_skill_refresh(it_hero->second.hero_index, false, false, false);
			it_hero->second.hero_fighter_detail.serialize(sync_buff);
		}
		if (has_fight_hero == false) {
			sync_buff.write_int8(0);
		}
	}

	sync_buff.finish_message();
	player->send_to_scene(sync_buff);
	player->helper_listen_join_arena_force(player->fighter_detail().force);
	POOL_MANAGER->push_logic_player(match_logic_player);

	MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`) VALUES (%d,%ld,%ld,%d)",  7,  player->role_id(), Time_Value::gettimeofday().sec(),
			player->level());
	return 0;
}

Arena_Rank_Player* Arena_Manager::get_random_player(Arena_Rank_Player* rank_player){
	int count = ranked_map_.size();
	if(count == 1){
		return rank_player;
	}
	Arena_Rank_Player* match_player;
	int rank;
	rank = rank_player->local_rank();
	int region;
	region = count - rank;
	if(region > 0){
		if(region > 8){
			region = 8;
		}
	   match_player = get_rank_player_from_rank(rank+random()%region+1);
		if(match_player){
			return match_player;
		}
	}
	region = rank-1;
	if(region > 6){
		region = 6;
	}
	match_player = get_rank_player_from_rank(rank-random()%region+1);
	if(match_player){
		return match_player;
	}
	return rank_player;
}

Arena_Rank_Player* Arena_Manager::get_rank_player_from_rank(int rank){
	Arena_Ranked_Map::iterator it = ranked_map_.find(rank);
	if(it != ranked_map_.end()){
		return it->second;
	}
	return NULL;
}

void Arena_Manager::add_ready_player(Arena_Rank_Player* rank_player, role_id_t match_role_id, std::string match_role_name, std::string match_server_name){
	if(rank_player == NULL){
		return;
	}
	Time_Value start_time = Time_Value::gettimeofday() + Time_Value(2,0);
	rank_player->set_battle_start_time(start_time, match_role_id, match_role_name, match_server_name);
	ready_player_set_.insert(rank_player);
}
int Arena_Manager::fetch_match_player_data(const role_id_t fight_id, const role_id_t match_id){
	Block_Buffer buf;
	buf.make_message(MONGO_LOAD_ARENA_FIGHT_DATA);
	buf.write_int64(fight_id);
	buf.write_int64(match_id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, fight_id);
	return 0;
}


void Arena_Manager::add_match_status(const role_id_t fight_id, const std::vector<Arena_Load_Status> match_player, int8_t refresh/* = 0*/){
	Time_Value now = Time_Value::gettimeofday();
	Arena_Match_Status match_status = Arena_Match_Status(now, match_player, refresh);
	arena_match_status_map_[fight_id] = match_status;
}

void Arena_Manager::del_match_status(const role_id_t fight_id){
	Arena_Match_Status_Map::iterator it = arena_match_status_map_.find(fight_id);
	if(it != arena_match_status_map_.end()){
		arena_match_status_map_.erase(it);
	}
}

bool Arena_Manager::get_match_status(const role_id_t fight_id, std::vector<role_id_t>& match_id_vec){
	Arena_Match_Status_Map::iterator it = arena_match_status_map_.find(fight_id);
	if(it != arena_match_status_map_.end()){
		//match_id_vec = it->second.match_players;
		return true;
	}
	return false;
}

void Arena_Manager::add_inner_load_player(const role_id_t fight_id, const role_id_t match_id){
	inner_load_player_map_[fight_id].push_back(match_id);
}

void Arena_Manager::del_inner_load_player(const role_id_t fight_id, const role_id_t match_id){
	Load_Data_Player_Map::iterator it = inner_load_player_map_.find(fight_id);
	if(it != inner_load_player_map_.end()){
		std::vector<role_id_t>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			if(*s_it == match_id){
				it->second.erase(s_it);
				break;
			}
		}
		if(it->second.empty()){
			inner_load_player_map_.erase(it);
		}
	}
}

bool Arena_Manager::is_inner_fighter(const role_id_t fight_id, const role_id_t match_id){
	Load_Data_Player_Map::iterator it = inner_load_player_map_.find(fight_id);
	if(it != inner_load_player_map_.end()){
		std::vector<role_id_t>::iterator s_it;
		for(s_it = it->second.begin(); s_it != it->second.end(); ++s_it){
			if(*s_it == match_id){
				return true;
			}
		}
	}
	return false;
}

bool Arena_Manager::get_inner_load_player(const role_id_t load_id, std::vector<role_id_t>& fight_id_vec){
	Load_Data_Player_Map::iterator it = inner_load_player_map_.find(load_id);
	if(it != inner_load_player_map_.end()){
		fight_id_vec = it->second;
		return true;
	}
	return false;
}

void Arena_Manager::load_data_complete(const role_id_t fight_id, const role_id_t loaded_id){
	Arena_Match_Status_Map::iterator it = arena_match_status_map_.find(fight_id);
	if(it != arena_match_status_map_.end()){
		it->second.load_complete(loaded_id);
	}
}

bool Arena_Manager::is_local_fighter(const role_id_t fight_id, const role_id_t match_id, int& match_pos){
	Arena_Match_Status_Map::iterator it = arena_match_status_map_.find(fight_id);
	if(it != arena_match_status_map_.end()){
		match_pos = it->second.get_match_pos(match_id);
		return true;
	}
	return false;
}

void Arena_Manager::check_match_status(const Time_Value &now){
	Arena_Match_Status_Map::iterator it;
	for(it = arena_match_status_map_.begin(); it != arena_match_status_map_.end();){
		if(it->second.is_complete()){
			role_id_t fight_id = it->first;
			process_match_complete(fight_id, it->second.match_players, it->second.refresh);
			it = arena_match_status_map_.erase(it);
		} else {
			++it;
		}
	}
}

bool Arena_Manager::is_arena_running(void){
	if(this->arena_season_detail_.season_status == ARENA_BATTLE_STATUS_RUNNING){
		return true;
	}
	return false;
}

int Arena_Manager::process_player_data_complete(Logic_Player_Detail* match_data, const role_id_t fight_id, bool push_pool/* = true*/){
	MSG_DEBUG("a_i_m process_player_data_complete");
	if (! match_data) {
		MSG_USER("player_data == 0.");
		return -1;
	}
	//
	role_id_t match_id = match_data->base_detail.role_id;
	int match_pos = 0;
	bool is_local = this->is_local_fighter(fight_id, match_id, match_pos);
	if(is_local){
		Arena_Rank_Player* fight_rank = this->find_rank_player(fight_id);
		if(fight_rank){
			Logic_Player* fight_logic = fight_rank->logic_player();
			if(fight_logic){
				fight_logic->set_arena_match_data(match_data, match_pos);
			}
			this->load_data_complete(fight_id, match_id);
		}
	}else{//跨服请求加载的
		bool is_inner = this->is_inner_fighter(fight_id, match_id);
		if(is_inner){
			this->del_inner_load_player(fight_id, match_id);
			Block_Buffer buf;
			buf.make_message(INNER_ARENA_FETCH_FIGHT_DATA);
			int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
			buf.write_int32(inner_area);
			buf.write_int64(fight_id);
			match_data->base_detail.serialize(buf);
			match_data->fighter_detail.serialize(buf);
			match_data->heroer_detail.serialize_fighter(buf);
			buf.finish_message();
			this->send_to_inner_arena(buf);
		}
	}
	if(push_pool){
		POOL_MANAGER->push_player_data_pool(match_data, 2);
	}
	return 0;
}
//
//int Arena_Manager::process_player_match_complete(role_id_t rank_role_id, Logic_Player_Detail* match_data, std::string server_name, int score, int group, int seg, bool push_cache/* = true*/){
//	if(match_data == NULL){
//		MSG_USER_TRACE("process_player_match_complete error match_data NULL rank_role_id:%ld", rank_role_id);
//		return -1;
//	}
//	MSG_DEBUG("a_m process_player_match_complete% rank:%ld, match:%ld", rank_role_id, match_data->base_detail.role_id);
//	Arena_Rank_Player* rank_player = find_rank_player(rank_role_id);
//	if(!rank_player){
//		if(push_cache){
//			this->push_player_data_cache(match_data);
//		}
//		MSG_USER_TRACE("arena process_player_match_complete error rank_player NULL:%ld", rank_role_id);
//		return -1;
//	}
//	Logic_Player* logic_player = rank_player->logic_player();
//	if(!logic_player){
//		if(push_cache){
//			this->push_player_data_cache(match_data);
//		}
//		MSG_USER_TRACE("arena process_player_match_complete error rank_logic_player NULL:%ld", rank_role_id);
//		return -1;
//	}
//	add_ready_player(rank_player, match_data->base_detail.role_id, match_data->base_detail.role_name, server_name);
//	Block_Buffer buf;
//	MSG_50100410 msg;
//	msg.level = match_data->base_detail.level;
//	msg.name = match_data->base_detail.role_name;
//	msg.server_name = server_name;
//	msg.fight = match_data->fighter_detail.force;
//	msg.score = score;
//	msg.group = group;
//	msg.seg = seg;
//	msg.career = match_data->base_detail.career;
//	msg.gender = match_data->base_detail.gender;
//	msg.head_id = match_data->base_detail.head_id;
//	// 战斗英雄信息
//	int fighter_idx = match_data->heroer_detail.fighter_index;
//	Heroer_Detail::Hero_Map::const_iterator it_hero = match_data->heroer_detail.hero_map.find(fighter_idx);
//	if(it_hero != match_data->heroer_detail.hero_map.end()){
//		msg.hero_id = it_hero->second.hero_id;
//		msg.hero_name = it_hero->second.hero_name;
//		msg.hero_level = it_hero->second.level;
//	}
//	OBJ_SEND_TO_CLIENT(msg, (*logic_player));
//	if(push_cache){
//		this->push_player_data_cache(match_data);
//	}
//	return 0;
//
//}

int Arena_Manager::init_all_arena_player(bool first/* = true*/){
	int arena_open_lv = CONFIG_CACHE_ARENA->get_arena_open_lv();
	DB_View::ID_Player_View_Map::iterator it;
	for(it = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
			it != LOGIC_MONITOR->db_view()->id_player_view_map.end(); ++it){
		Player_DB_View* view = it->second;
		if(view->level >= arena_open_lv){
			Arena_Rank_Player* rank_player = ARENA_MANAGER->get_rank_player(view->role_id);
		   rank_player->set_role_id(view->role_id);
			rank_player->set_role_name(view->role_name);
			rank_player->set_level(view->level);
			rank_player->set_career(view->career);
			rank_player->set_gender(view->gender);
			rank_player->set_head_id(view->head_id);
			rank_player->set_fight(view->ttl_force);

			//登录
			Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(view->role_id);
			if(player){
				rank_player->set_fight(player->fighter_detail().ttl_force);
			}
			int seg_key = get_arena_seg_key(rank_player->group(), rank_player->seg());
			this->arena_seg_add(rank_player->role_id(), seg_key, rank_player->fight());
			// 战斗过的参与排名
			int fight_times = rank_player->rank_info_.fight_times;
			if(fight_times > 0){
				if(ranking_set_.count(rank_player->role_id()) <= 0){
					ranking_set_.insert(rank_player->role_id());
					this->ranking_vec_.push_back(rank_player);
				}
			}
		}
	}
	return 0;
}

int Arena_Manager::load_all_arena_db_info(void){
	MONGO_OPERATOR->load_arena_season_detail(arena_season_detail_);
	MONGO_OPERATOR->load_all_arena_player(false);
	return 0;
}

Arena_Rank_Player* Arena_Manager::get_rank_player(role_id_t role_id){
	Arena_Rank_Player_Map::iterator it = rank_player_map_.find(role_id);
	if(it != rank_player_map_.end()){
		return it->second;
	}else{
		Arena_Rank_Player* rank_player = POOL_MANAGER->pop_arena_player_pool();
		rank_player->set_role_id(role_id);
		std::string server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
		rank_player->set_server_name(server_name);

		rank_player_map_.insert(std::make_pair(role_id, rank_player));
		Arena_Rank_Player_Map::iterator it = rank_player_map_.find(role_id);
		if(it != rank_player_map_.end()){
			return it->second;
		}
	}
	return NULL;
}

void Arena_Manager::clean_arena_mem(void) {
	for (Arena_Rank_Player_Map::iterator it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it) {
		POOL_MANAGER->push_arena_player_pool(it->second);
	}
	rank_player_map_.clear();
}

Arena_Rank_Player* Arena_Manager::find_rank_player(role_id_t role_id){
	Arena_Rank_Player_Map::iterator it = rank_player_map_.find(role_id);
	if(it != rank_player_map_.end()){
		return it->second;
	}
	return NULL;
}

int Arena_Manager::get_rank_player_size(void){
	if(rank_player_map_.empty()){
		return 0;
	}
	return rank_player_map_.size();
}

int Arena_Manager::get_pay_times_cost(int& cost, int& increase){
	Arena_Setting_Config* arena_setting = CONFIG_CACHE_ARENA->get_arena_setting_conifg();
	if(arena_setting != NULL){
		cost = arena_setting->athletics_initial_cost;
		increase = arena_setting-> athletics_cost_increase;
	}else{
		cost = 20;
		increase = 10;
	}
	return 0;
}

int Arena_Manager::fetch_arena_local_rank_info(role_id_t role_id, uint32_t& self_rank, std::vector<Arena_Rank_Info>& rank_vec){
	Arena_Rank_Player* rank_player = find_rank_player(role_id);
	if(rank_player){
		self_rank = rank_player->local_rank();
	}else{
		self_rank = 0;
	}

	Arena_Rank_Info info;
	Arena_Ranked_Map::iterator it;
	int rank = 0;
	for(it = ranked_map_.begin(); it != ranked_map_.end(); ++it){
		info.reset();
		info.fight = it->second->fight();
		info.group = it->second->group();
		info.level = it->second->level();
		info.name = it->second->role_name();
		info.rank = it->second->local_rank();
		info.score = it->second->score();
		info.seg = it->second->seg();
		info.role_id = it->second->role_id();
		rank_vec.push_back(info);
		rank += 1;
		if(rank >= 100){
			break;
		}
	}
	return 0;
}

int Arena_Manager::fetch_arena_inner_rank_info(const role_id_t role_id){
	Block_Buffer buf;
	buf.make_message(INNER_ARENA_FETCH_RANK_INFO);
	int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
	buf.write_int32(inner_area);
	buf.write_int64(logic_uuid_);
	buf.write_int64(role_id);
	buf.finish_message();
	this->send_to_inner_arena(buf);
	return 0;
}

int Arena_Manager::fetch_arena_info(role_id_t role_id, MSG_50100405& msg){
	Arena_Rank_Player* rank_player = find_rank_player(role_id);
	if(rank_player == NULL){
		msg.personal_info.group = 1;
		msg.personal_info.seg = 1;
		msg.personal_info.local_rank = 0;
		msg.personal_info.inter_rank = 0;
		msg.personal_info.score = 0;
		msg.personal_info.max_pay_times = 0;
		msg.personal_info.reward_times1 = local_award_time_.sec();
		Time_Value now = Time_Value::gettimeofday();
		int remain_sec = this->arena_season_detail_.season_finish.sec() - now.sec();
		int remain_day = remain_sec/Time_Value::ONE_DAY_IN_SECS+1;
		msg.season_info.season_time_remain = remain_day;
		this->get_the_first_three(msg.season_info.the_first_three);
		return -1;
	}
	rank_player->set_need_notice_client(true);
	msg.personal_info.group = rank_player->group();
	msg.personal_info.seg = rank_player->seg();
	msg.personal_info.local_rank = rank_player->local_rank();
	msg.personal_info.inter_rank = rank_player->inner_rank();
	msg.personal_info.score = rank_player->score();
	msg.personal_info.max_pay_times = max_buy_pay_times();
	msg.personal_info.reward_times1 = local_award_time_.sec();
	Time_Value now = Time_Value::gettimeofday();
	int remain_sec = this->arena_season_detail_.season_finish.sec() - now.sec();
	int remain_day = remain_sec/Time_Value::ONE_DAY_IN_SECS+1;
	msg.season_info.season_time_remain = remain_day;
	this->get_the_first_three(msg.season_info.the_first_three);
	return 0;
}
int Arena_Manager::close_arena_panel(role_id_t role_id){
	Arena_Rank_Player* rank_player = find_rank_player(role_id);
	if(rank_player == NULL){
		return -1;
	}
	rank_player->set_need_notice_client(false);
	return 0;
}

int Arena_Manager::battle_result(role_id_t role_id, uint8_t result){
	Arena_Rank_Player* rank_player = find_rank_player(role_id);
	if(rank_player == NULL){
		return -1;
	}
	Logic_Player* logic_player = rank_player->logic_player();
	if(logic_player == NULL){
		return -1;
	}
	role_id_t match_id = rank_player->get_match_id();
	int8_t d_lv = rank_player->get_match_difficult_lv();
	Arena_Rank_Player* match_player = find_rank_player(match_id);

	// === 累加cd
	int arena_cd = CONFIG_CACHE_ARENA->get_arena_cd();
	int remain_cd = logic_player->get_arena_cd();
	logic_player->set_arena_cd(arena_cd+remain_cd);

	int part_id = rank_player->group()*10 + rank_player->seg();
	Arena_Base_Config* arena_base = CONFIG_CACHE_ARENA->get_arena_base_conifg(part_id);
	if(arena_base == NULL){
		return -1;
	}
	// 最高段位积分不变
	int king_coe = 1;
	bool in_max_group = false;
	if(rank_player->group() == ARENA_MAX_GROUP){
		in_max_group = true;
		king_coe = 0;
	}
	std::vector<Arena_Battle_Report> report_vec;
	Arena_Battle_Report report;
	int rise_score = arena_base->arena_promote;
	int add_score = 0;
	int add_copper = 0;
	int add_exploit = 0;
	int rise_gold = 0;
	Item_Vec pack_item_vec;
	if(result){
		report.reset();
		report.role_name = rank_player->match_role_name();
		report.result = BATTLE_REPORT_WIN;
		report.score = arena_base->get_arena_award(d_lv)*king_coe;
		add_score = arena_base->get_arena_award(d_lv)*king_coe;

		{
		Reward_Info report_item;
		report_item.reward_id = COPPER;
		report_item.quantity = arena_base->winer_award;
		report.item_vec.push_back(report_item);
		}

		add_copper += arena_base->winer_award;

		std::vector<Id_Int_Int> award_item = arena_base->get_winer_award_item(d_lv);
		int item_count = award_item.size();
		for(int i=0; i<item_count; ++i){
			if(award_item[i].val1 > random()%1000){
				if(award_item[i].id == COPPER){
					add_copper += award_item[i].val2;
				}else if(award_item[i].id == 503003){
					Reward_Info report_item;
					report_item.reward_id = award_item[i].id;
					report_item.quantity = award_item[i].val2;
					report.item_vec.push_back(report_item);
					add_exploit += award_item[i].val2;
				}else{
					//package item
					Item_Detail item(award_item[i].id, award_item[i].val2,Item_Detail::BIND);
					pack_item_vec.push_back(item);
					//report
					Reward_Info report_item;
					report_item.reward_id = item.id_;
					report_item.quantity = item.amount_;
					report.item_vec.push_back(report_item);
				}
			}
		}
		report_vec.push_back(report);

		//streak win
		int compensate_times = logic_player->compensate_times();
		int streak_win = logic_player->streak_win();
		streak_win += 1;
		logic_player->set_streak_win(streak_win);
		logic_player->set_streak_lose(0);
		if(streak_win >= 3){
			report.reset();
			report.result = BATTLE_REPORT_STREAK_WIN;
			report.val1 = streak_win;
			//连胜补偿
			if(compensate_times > 0 &&(rank_player->score() + add_score < rise_score )){
				int compensate_score;
				if(compensate_times == 1){
					compensate_score = add_score/2;
					logic_player->set_compensate_times(0);
				}else{
					compensate_score = add_score;
					logic_player->set_compensate_times(compensate_times-2);
				}
				add_score += compensate_score*king_coe;
				report.score = compensate_score*king_coe;
			}
			report_vec.push_back(report);
		}
	}else{
		report.reset();
		report.role_name = rank_player->match_role_name();
		report.result = BATTLE_REPORT_LOSE;
		report.score = arena_base->get_arena_punish(d_lv)*king_coe;
		add_score = arena_base->get_arena_punish(d_lv)*king_coe;
		//report
		{
			Reward_Info report_item;
			report_item.reward_id = COPPER;
			report_item.quantity = arena_base->loser_award;
			report.item_vec.push_back(report_item);
		}
		add_copper += arena_base->loser_award;

		std::vector<Id_Int_Int> award_item = arena_base->get_loser_award_item(d_lv);
		int item_count = award_item.size();
		for(int i=0; i<item_count; ++i){
			if(award_item[i].val1 > random()%1000){
				if(award_item[i].id == COPPER){
					//report
					Reward_Info report_item;
					report_item.reward_id = award_item[i].id;
					report_item.quantity = award_item[i].val2;
					report.item_vec.push_back(report_item);
					add_copper += award_item[i].val2;
				}else if(award_item[i].id == 503003){
					Reward_Info report_item;
					report_item.reward_id = award_item[i].id;
					report_item.quantity = award_item[i].val2;
					report.item_vec.push_back(report_item);
					add_exploit += award_item[i].val2;
				}else{
					//package item
					Item_Detail item(award_item[i].id, award_item[i].val2,Item_Detail::BIND);
					pack_item_vec.push_back(item);
					//report
					Reward_Info report_item;
					report_item.reward_id = item.id_;
					report_item.quantity = item.amount_;
					report.item_vec.push_back(report_item);
				}
			}
		}
		report_vec.push_back(report);

		//streak win reset
		logic_player->set_streak_win(0);
		int streak_lose = logic_player->streak_lose();
		streak_lose += 1;
		logic_player->set_streak_lose(streak_lose);
	}
	if(rank_player->score() < rise_score || in_max_group){
		if(in_max_group == false){
			int score = rank_player->score();
			if(add_score + score > rise_score){
				rank_player->set_score(rise_score);
			}else{
				rank_player->set_score(add_score + score);
			}
		}
	}else{
		//rise
		if(result){
			int rise_win = logic_player->rise_win();
			if(rise_win+1 >= ARENA_RISE_MAX){
				//rise success
				logic_player->set_rise_win(0);
				logic_player->set_rise_lose(0);
				logic_player->set_rise_fail(0);
				int group = rank_player->group();
				int seg = rank_player->seg();
				int old_seg_key = get_arena_seg_key(group, seg);
				int new_seg_key = 0;
				int force = rank_player->fight();
				if(seg == ARENA_MAX_SEG){
					rank_player->set_group_seg(group+1, 1);
					new_seg_key = get_arena_seg_key(group+1, 1);
				}else{
					rank_player->set_group_seg(group, seg+1);
					new_seg_key = get_arena_seg_key(group, seg+1);
				}

				this->arena_seg_change(rank_player->role_id(), old_seg_key, new_seg_key, force, force);

				rank_player->check_rise_announce();
				rank_player->set_score(0);
				report.reset();
				report.result = BATTLE_REPORT_RISE_WIN;
				report.val1 = rank_player->group();
				report.val2 = rank_player->seg();
				int item_count = arena_base->arena_promote_awarditem_vec.size();
				for(int i=0; i<item_count; ++i){
					if(arena_base->arena_promote_awarditem_vec[i].val1 > random()%1000){
						if(arena_base->arena_promote_awarditem_vec[i].id == COPPER){
							//report
							Reward_Info report_item;
							report_item.reward_id = arena_base->arena_promote_awarditem_vec[i].id;
							report_item.quantity = arena_base->arena_promote_awarditem_vec[i].val2;
							report.item_vec.push_back(report_item);
							add_copper += arena_base->arena_promote_awarditem_vec[i].val2;
						}else if(arena_base->arena_promote_awarditem_vec[i].id == 503003){
							Reward_Info report_item;
							report_item.reward_id = arena_base->arena_promote_awarditem_vec[i].id;
							report_item.quantity = arena_base->arena_promote_awarditem_vec[i].val2;
							report.item_vec.push_back(report_item);
							add_exploit += arena_base->arena_promote_awarditem_vec[i].val2;
						}else if(arena_base->arena_promote_awarditem_vec[i].id == BIND_GOLD){
							//report
							Reward_Info report_item;
							report_item.reward_id = arena_base->arena_promote_awarditem_vec[i].id;
							report_item.quantity = arena_base->arena_promote_awarditem_vec[i].val2;
							report.item_vec.push_back(report_item);
							rise_gold += arena_base->arena_promote_awarditem_vec[i].val2;
						}else{
							//package item
							Item_Detail item(arena_base->arena_promote_awarditem_vec[i].id,
									arena_base->arena_promote_awarditem_vec[i].val2,Item_Detail::BIND);
							pack_item_vec.push_back(item);
							//report
							Reward_Info report_item;
							report_item.reward_id = item.id_;
							report_item.quantity = item.amount_;
							report.item_vec.push_back(report_item);
						}
					}
				}
				report_vec.push_back(report);
				//任务侦听段位提升
				logic_player->task_listen_arena_level(logic_player->group(), logic_player->seg());
				logic_player->active_content_listen_arena_levelup(logic_player->group(), logic_player->seg());

				// 跨组晋级广播
				if(logic_player->seg() == 1){
					role_id_t ann_role_id = logic_player->role_id();
					std::string ann_role_name = logic_player->role_name();
					std::string text1 = CONFIG_CACHE_ARENA->get_part_name(logic_player->group(), logic_player->seg());
					if(ann_role_id != 0){
						this->make_announce(ARENA_ANNOUNCE_ID_LOCAL_RISE, ann_role_id, ann_role_name, text1);
					}
				}

				// 进入王者组时间参数
				if(rank_player->group() == ARENA_MAX_GROUP){
					Time_Value time = Time_Value::gettimeofday();
					int king_time = REANA_KING_TIME_MAX - time.sec();
					logic_player->set_king_time(king_time);
				}
			}else{
				logic_player->set_rise_win(rise_win+1);
			}
		}else{
			int rise_lose = logic_player->rise_lose();
			if(rise_lose+1 >= ARENA_RISE_MAX){
				//rise fail
				report.reset();
				report.result = BATTLE_REPORT_RISE_LOSE;
				report.score = -arena_base->arena_promote_punish;
				report.val1 = rank_player->group();
				report.val2 = rank_player->seg();

				logic_player->set_rise_win(0);
				logic_player->set_rise_lose(0);
				int rise_fail = logic_player->rise_fail();
				logic_player->set_rise_fail(rise_fail+1);
				int score = rank_player->score();
				add_score = -arena_base->arena_promote_punish;
				rank_player->set_score(score+add_score);

				report_vec.push_back(report);
			}else{
				logic_player->set_rise_lose(rise_lose+1);
			}
		}
	}

	int src_enter_num = logic_player->enter_num();
	int pay_times = logic_player->pay_times();
	logic_player->set_enter_num(src_enter_num+1);
	if(src_enter_num >= logic_player->get_arena_free_num()){
		logic_player->set_pay_times(pay_times-1);
	}

	int old_fight_times = rank_player->rank_info_.fight_times;
	rank_player->set_fight_times(old_fight_times+1);
	// 参与过竞技场的可以参与排名
	if(old_fight_times <= 0){
		if(ranking_set_.count(rank_player->role_id()) <= 0){
			ranking_set_.insert(rank_player->role_id());
			this->ranking_vec_.push_back(rank_player);
		}
	}
	int match_src_rank = 0;
	if(match_player){
		match_src_rank = match_player->local_rank();
	}

	int src_rank = logic_player->local_rank();
	this->sort_rank();
	int last_rank = logic_player->local_rank();

	int match_last_rank = 0;
	if(match_player){
		match_last_rank = match_player->local_rank();
	}
	if(src_rank == 0){
		if(last_rank > 0 && last_rank <= ARENA_ELITE_RANK){
			report.reset();
			report.result = BATTLE_REPORT_RANK_UP;
			report.val1 = last_rank;
			report_vec.push_back(report);
		}
	}else{
		if(src_rank <= ARENA_ELITE_RANK && last_rank > src_rank ){
			report.reset();
			report.result = BATTLE_REPORT_RANK_DOWN;
			report.val1 = last_rank;
			report_vec.push_back(report);
		}
		if(last_rank < src_rank && last_rank <= ARENA_ELITE_RANK){
			report.reset();
			report.result = BATTLE_REPORT_RANK_UP;
			report.val1 = last_rank;
			report_vec.push_back(report);
		}
	}

	//mathc_player 名次给超越
	if(match_player && match_player->logic_player()){
		if(match_src_rank != 0 && match_src_rank <= ARENA_ELITE_RANK && match_last_rank > match_src_rank){
			report.reset();
			report.result = BATTLE_REPORT_RANK_DOWN;
			report.val1 = match_last_rank;
			match_player->logic_player()->add_battle_report(report);
		}
	}
	logic_player->add_battle_report_vec(report_vec);
	// 排名杀进广播
	if(last_rank != 0){
		if(last_rank == 1){// 排名杀进1
			if(src_rank > 1){
				role_id_t ann_role_id = logic_player->role_id();
				std::string ann_role_name = logic_player->role_name();
				if(ann_role_id != 0){
					this->make_announce(ARENA_ANNOUNCE_ID_ENTER_ONE, ann_role_id, ann_role_name);
				}
			}
		}
//		else if(last_rank == 2){// 排名杀进2
//			if(src_rank > 2){
//				role_id_t ann_role_id = logic_player->role_id();
//				std::string ann_role_name = logic_player->role_name();
//				if(ann_role_id != 0){
//					this->make_announce(ARENA_ANNOUNCE_ID_ENTER_TWO, ann_role_id, ann_role_name);
//				}
//			}
//		}else if(last_rank == 3){// 排名杀进3
//			if(src_rank > 3){
//				role_id_t ann_role_id = logic_player->role_id();
//				std::string ann_role_name = logic_player->role_name();
//				if(ann_role_id != 0){
//					this->make_announce(ARENA_ANNOUNCE_ID_ENTER_THREE, ann_role_id, ann_role_name);
//				}
//			}
//		}else if(last_rank <= 10){// 排名杀进10
//			if(src_rank > 10){
//				role_id_t ann_role_id = logic_player->role_id();
//				std::string ann_role_name = logic_player->role_name();
//				if(ann_role_id != 0){
//					this->make_announce(ARENA_ANNOUNCE_ID_ENTER_TEN, ann_role_id, ann_role_name);
//				}
//			}
//		}else if(last_rank <= 30){// 排名杀进30
//			if(src_rank > 30){
//				role_id_t ann_role_id = logic_player->role_id();
//				std::string ann_role_name = logic_player->role_name();
//				if(ann_role_id != 0){
//					this->make_announce(ARENA_ANNOUNCE_ID_ENTER_THIRTY, ann_role_id, ann_role_name);
//				}
//			}
//		}else if(last_rank <= 50){// 排名杀进50
//			if(src_rank > 50){
//				role_id_t ann_role_id = logic_player->role_id();
//				std::string ann_role_name = logic_player->role_name();
//				if(ann_role_id != 0){
//					this->make_announce(ARENA_ANNOUNCE_ID_ENTER_FIFTY, ann_role_id, ann_role_name);
//				}
//			}
//		}
	}

	//notice client
	Block_Buffer buf;
	MSG_81000304 msg;
	msg.personal_info.score = rank_player->score();
	msg.personal_info.buy_pay_times = logic_player->buy_pay_times();
	int total_num;
	int enter_num;
	logic_player->get_arena_enter_num_info(total_num, enter_num);
	int free_times = total_num - enter_num - logic_player->pay_times();;
	if(free_times < 0){
		free_times = 0;
	}
	msg.personal_info.free_times = free_times;
	int total_free_times = total_num - logic_player->buy_pay_times();
	if(total_free_times < 0){
		total_free_times = 0;
	}
	msg.personal_info.total_free_times = total_free_times;
	msg.personal_info.group = rank_player->group();
	msg.personal_info.seg = rank_player->seg();
	msg.personal_info.local_rank = rank_player->local_rank();
	msg.personal_info.max_pay_times = this->max_buy_pay_times();
	msg.personal_info.pay_times = logic_player->pay_times();
	msg.personal_info.reward_times1 = local_award_time_.sec();
	msg.personal_info.three_day_rank = logic_player->three_day_rank();
	msg.personal_info.rise = logic_player->is_arena_rise();
	if(msg.personal_info.rise == 1){
		if(logic_player->streak_lose() > 0){
			if(logic_player->rise_win() > 0){
				msg.personal_info.rise_result_vec.push_back(1);
			}
			if(logic_player->rise_lose() > 0){
				msg.personal_info.rise_result_vec.push_back(0);
			}
		}else{
			if(logic_player->rise_lose() > 0){
				msg.personal_info.rise_result_vec.push_back(0);
			}
			if(logic_player->rise_win() > 0){
				msg.personal_info.rise_result_vec.push_back(1);
			}
		}
	}
	msg.personal_info.report_vec = report_vec;
	msg.personal_info.remain_cd_time = logic_player->get_arena_cd();
	// 前三名玩家
	this->get_the_first_three(msg.the_first_three);
	OBJ_SEND_TO_CLIENT(msg, (*logic_player));
	Arena_Rank_Player* first_player = get_local_first_player();
	if ( first_player ) {
		MSG_81000306 first_mag;
		first_mag.first_info.fight = first_player->fight();
		first_mag.first_info.group = first_player->group();
		first_mag.first_info.level = first_player->level();
		first_mag.first_info.name = first_player->role_name();
		first_mag.first_info.rank = first_player->local_rank();
		first_mag.first_info.role_id = first_player->role_id();
		first_mag.first_info.score = first_player->score();
		first_mag.first_info.seg = first_player->seg();
		first_mag.be_worship = first_player->be_worship();
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(first_player->role_id());
		if (player ) {
			first_mag.avatar_vec = player->base_detail().avatar_vec;
			first_mag.career = player->base_detail().career;
			first_mag.gender = player->base_detail().gender;
		}else {
			Player_DB_View *db_view = LOGIC_MONITOR->find_player_db_view_by_role_id(first_player->role_id());
			if ( db_view ) {
				first_mag.avatar_vec = db_view->avatar_vec;
				first_mag.career = db_view->career;
				first_mag.gender = db_view->gender;
			}
		}
		OBJ_SEND_TO_CLIENT(first_mag, (*logic_player));
	}

	// 战斗结果同步跨服
	this->sync_result_to_inner_scene(rank_player, match_id, result);

	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if(add_copper > 0){
		logic_player-> pack_add_money(Money_Add_Info(COPPER, add_copper, Money_DM_Info(MONEY_ADD_ARENA_FIHGT)), MONEY_OPT_REAL);
		logic_player->listen_vip_reward(VIP_WORLD_TREE_ARENA, COPPER, add_copper);
		acv_msg.property.push_back(Property(COPPER, add_copper));
	}
	if(add_exploit > 0){
		logic_player->change_exploit_val(add_exploit);
		logic_player->listen_vip_reward(VIP_WORLD_TREE_ARENA, 503003, add_exploit);
		acv_msg.property.push_back(Property(503003, add_exploit));
	}
	if(rise_gold > 0){
		logic_player-> pack_add_money(Money_Add_Info(BIND_GOLD, rise_gold, Money_DM_Info(MONEY_ADD_ARENA_FIHGT)), MONEY_OPT_REAL);
		logic_player->listen_vip_reward(VIP_WORLD_TREE_ARENA, BIND_GOLD, rise_gold);
		acv_msg.property.push_back(Property(BIND_GOLD, rise_gold));
	}
	if(pack_item_vec.empty() == false){
		rank_player->logic_player()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
					pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_ARENA_FIGHT));
		for (Item_Vec::const_iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
			Item_Basic_Info ibi;
			it->set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		}
	}
	if(acv_msg.item_info.empty() == false || acv_msg.property.empty() == false){
		OBJ_SEND_TO_CLIENT(acv_msg, (*rank_player->logic_player()));
	}
	//胜率刷新匹配的玩家
	if(result == 1){
		logic_player->set_match_player_info(Time_Value::zero, 0, 0, 0, 0, 0, 0);
	}
	return 0;
}

void Arena_Manager::get_zero_time(const Time_Value &tv1, Time_Value &tv2) {
	int zeor_time = tv1.sec() + (86400 - tv1.sec()% 86400);
	tv2.set(zeor_time, 0);
}

int Arena_Manager::max_free_match_times(void){
	Arena_Setting_Config* arena_setting = CONFIG_CACHE_ARENA->get_arena_setting_conifg();
	if(arena_setting){
		return arena_setting->athletics_number;
	}
	return 10;
}

int Arena_Manager::max_buy_pay_times(void){
	Arena_Setting_Config* arena_setting = CONFIG_CACHE_ARENA->get_arena_setting_conifg();
	if(arena_setting){
		return arena_setting->athletics_buy_limit;
	}
	return 5;
}

Arena_Manager::Arena_Ranking_Vec& Arena_Manager::arena_ranking_vec(void){
	return ranking_vec_;
}

int32_t Arena_Manager::get_next_local_award_time(void){
	return local_award_time_.sec();
}

int32_t Arena_Manager::get_next_inner_award_time(void){
	return inner_award_time_.sec();
}

void Arena_Manager::open_activity(const Time_Value& begin_time, const Time_Value& end_time){

}

void Arena_Manager::close_activity(const Time_Value& begin_time, const Time_Value& end_time){

}

void Arena_Manager::make_announce(int announce_id, role_id_t role_id, std::string role_name, std::string text1/* = ""*/, std::string text2/* = ""*/, std::string text3/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if(role_id != 0){
		role_info.reset();
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.reset();
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
	}
	if(text1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = text1;
		ann_contents.push_back(new_content);
	}
	if(text2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = text1;
		ann_contents.push_back(new_content);
	}
	if(text3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = text1;
		ann_contents.push_back(new_content);
	}

	MSG_20500000 msg;
	msg.district = ANNOUNCE_DISTRICT_SYSTEM;
	msg.announce_id = announce_id;
	//msg.gang_id = gang_id;
	msg.contents = ann_contents;
	//msg.recievers = recievers;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
	return;
}

int Arena_Manager::get_local_first(role_id_t& role_id, std::string& role_name){
	if(ranking_vec_.empty()){
		return -1;
	}
	role_id = ranking_vec_[0]->role_id();
	role_name = ranking_vec_[0]->role_name();
	return 0;
}

void Arena_Manager::get_the_first_three(std::vector<Uid_Name>& first_three_vec){
	int count = ranking_vec_.size();
	for(int i = 0; i < count; ++i){
		Uid_Name uid_name;
		uid_name.uid = ranking_vec_[i]->role_id();
		uid_name.name = ranking_vec_[i]->role_name();
		first_three_vec.push_back(uid_name);
		if(i >= 3){
			break;
		}
	}
}

void Arena_Manager::clear_season_rank(const role_id_t role_id){
	return this->arena_season_detail_.clear_season_rank(role_id);
}

int Arena_Manager::get_season_rank(const role_id_t role_id) const{
	return this->arena_season_detail_.get_season_rank(role_id);
}

void Arena_Manager::get_season_result(const role_id_t role_id, int &rank, int &group_index, int &level){
	return arena_season_detail_.get_season_result(role_id, rank, group_index, level);
}

//void Arena_Manager::get_local_group_rank(int group_index, Arena_Ranking_Vec& rank_vec){
//	Group_Rank_Map::iterator it = group_rank_map_.find(group_index);
//	if(it != group_rank_map_.end()){
//		rank_vec = it->second;
//	}
//}


void Arena_Manager::sync_all_to_inner_scene(void){
	Block_Buffer buf;
	buf.make_message(INNER_ARENA_SYNC_RANK_INFO);
	if(this->rank_player_map_.empty()){
		return;
	}
	int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
	buf.write_int32(inner_area);
	buf.write_int64(logic_uuid_);
	// 确保有50个低段位的在跨服匹配
	int max_low_count = 20;
	int low_count = 0;
	std::vector<Arena_Rank_Player*> low_vec;

	int count = 0;
	Arena_Rank_Player_Map::const_iterator it;
	for(it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it){
		if(it->second->group() < MIN_ARENA_INNER_SYNC_GROUP){
			if(low_count < max_low_count){
				low_count += 1;
				low_vec.push_back(it->second);
			}
		}else{
			count += 1;
		}
	}
	buf.write_int32(count+low_count);
	for(int i = 0; i < low_count; ++i){
		low_vec[i]->player_rank_info()->serialize(buf);
	}
	for(it = rank_player_map_.begin(); it != rank_player_map_.end(); ++it){
		if(it->second->group() < MIN_ARENA_INNER_SYNC_GROUP){
			continue;
		}
		it->second->player_rank_info()->serialize(buf);
	}
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(ARENA_INNER_SCENE_ID, 0, buf);
}

void Arena_Manager::sync_one_to_inner_scene(Arena_Rank_Player* rank_player){
	if(!rank_player){
		return;
	}
	if(rank_player->group() < MIN_ARENA_INNER_SYNC_GROUP){
		return;
	}
	Block_Buffer buf;
	buf.make_message(INNER_ARENA_SYNC_RANK_INFO);
	int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
	buf.write_int32(inner_area);
	buf.write_int64(logic_uuid_);
	int count = 1;
	buf.write_int32(count);
	rank_player->player_rank_info()->serialize(buf);
	buf.finish_message();
	this->send_to_inner_arena(buf);
}

void Arena_Manager::sync_result_to_inner_scene(Arena_Rank_Player* rank_player, const role_id_t match_id, const int8_t result){
	if(!rank_player){
		return;
	}
	if(rank_player->group() < MIN_ARENA_INNER_SYNC_GROUP){
		return;
	}
	Block_Buffer buf;
	buf.make_message(INNER_ARENA_BATTLE_RESULT);
	int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
	buf.write_int32(inner_area);
	buf.write_int8(result);
	buf.write_int64(match_id);
	rank_player->player_rank_info()->serialize(buf);
	buf.finish_message();
	this->send_to_inner_arena(buf);
}

int Arena_Manager::send_to_inner_arena(Block_Buffer& buf){
	return LOGIC_MONITOR->send_to_scene(ARENA_INNER_SCENE_ID, 0, buf);
}

int Arena_Manager::inner_arena_save_king_time(role_id_t role_id, int king_time){
	Arena_Rank_Player* rank_player = this->find_rank_player(role_id);
	if(rank_player){
		rank_player->rank_info_.king_time = king_time;
	}
	Logic_Player* logic_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(logic_player){// online
		logic_player->set_king_time(king_time);
	}else{
		Block_Buffer buf;
		buf.make_message(MANGO_SAVE_ARENA_KING_TIME);
		buf.write_int64(role_id);
		buf.write_int32(king_time);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, role_id);
	}
	return 0;
}

int Arena_Manager::inner_arena_fetch_fight_data(int8_t inner_fetch, role_id_t fight_id, role_id_t match_id){
	MSG_DEBUG("a_m inner_arena_fetch_fight_data");
	Arena_Rank_Player* match_player = this->find_rank_player(match_id);
	if(!match_player){
		MSG_USER_TRACE("inner_arena_fetch_fight_data error match_player NULL %ld", match_id);
		return -1;
	}
	if(inner_fetch == 1){
		this->add_inner_load_player(fight_id, match_id);
	}
	Logic_Player* match_logic = match_player->logic_player();
	if(match_logic){// online
		Logic_Player_Detail* match_data = match_logic->logic_player_detail();
		this->process_player_data_complete(match_data, fight_id, false);
	}else{//offline
		this->fetch_match_player_data(fight_id, match_id);
	}
	return 0;
}

int Arena_Manager::inner_arena_match_player(Block_Buffer& buf){
	MSG_DEBUG("a_m inner_arena_match_player");
	int8_t match_result = 0;
	int8_t match = 0;
	int8_t refresh = 0;
	role_id_t fight_id = 0;
	int8_t match_num = 0;
	buf.read_int8(match_result);
	buf.read_int8(match);
	buf.read_int8(refresh);
	buf.read_int64(fight_id);
	buf.read_int8(match_num);

	std::vector<Arena_Load_Status> load_status_vec;;
	for(int i = 0; i < match_num; ++i){
		role_id_t match_id = 0;
		int8_t match_dlv = 0;
		std::string server_name = "";
		int score = 0;
		int group = 0;
		int seg = 0;
		int rank = 0;
		buf.read_int64(match_id);
		buf.read_int8(match_dlv);
		buf.read_string(server_name);
		buf.read_int32(score);
		buf.read_int32(group);
		buf.read_int32(seg);
		buf.read_int32(rank);
		Arena_Load_Status load_status;
		load_status.role_id = match_id;
		load_status.difficult_lv = match_dlv;
		load_status.score = score;
		load_status.group = group;
		load_status.seg = seg;
		load_status.rank = rank;
		load_status.server_name = server_name;
		load_status_vec.push_back(load_status);
	}
	Arena_Rank_Player* fight_player = find_rank_player(fight_id);
	if(fight_player == NULL){
		MSG_USER_TRACE("inner_arena_match_player error fight player NULL: %ld", fight_id);
		return -1;
	}
	Logic_Player* fight_logic = fight_player->logic_player();
	if(fight_logic == NULL){
		MSG_USER_TRACE("inner_arena_match_player error fight logic NULL: %ld", fight_id);
		return -1;
	}
	if(match_result == 1){
		this->add_match_status(fight_id, load_status_vec, refresh);
	}else{
		if(match == 1){
			fight_logic->set_match_player_info(Time_Value::zero, 0, 0, 0, 0, 0, 0);
		}
	}

	return 0;
}

int Arena_Manager::inner_arena_player_data_complete(Block_Buffer& buf){
	MSG_DEBUG("a_m inner_arena_player_data_complete");
	role_id_t fight_id = 0;
	buf.read_int64(fight_id);
	Logic_Player_Detail *match_data = POOL_MANAGER->pop_player_data_pool(8);
	if(match_data == NULL){
		MSG_USER_TRACE("inner_arena_player_data_complete error player_data NULL %ld", fight_id);
		return -1;
	}
	match_data->base_detail.deserialize(buf);
	match_data->fighter_detail.deserialize(buf);
	match_data->heroer_detail.deserialize_fighter(buf);
	this->process_player_data_complete(match_data, fight_id, true);
	return 0;
}

int Arena_Manager::inner_arena_sync_inner_rank(std::vector<Id_Int_Int>& inner_rank_vec){
	int count = inner_rank_vec.size();
	for(int i = 0; i < count; ++i){
		role_id_t role_id = inner_rank_vec[i].id;
		int rank = inner_rank_vec[i].val1;
		int king_time = inner_rank_vec[i].val2;
		Arena_Rank_Player* rank_player = this->find_rank_player(role_id);
		if(rank_player){
			rank_player->set_inner_rank(rank);
			rank_player->set_king_time(king_time);
		}
	}
	return 0;
}

int Arena_Manager::arena_save_be_worship(const role_id_t role_id) {
    Arena_Rank_Player* rank_player = this->find_rank_player(role_id);
	if(rank_player){
		rank_player->rank_info_.be_worship += 1;
	}
	Logic_Player* logic_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(logic_player){// online
		logic_player->set_be_worship(logic_player->be_worship() + 1);
	}else{
		Block_Buffer buf;
		buf.make_message(MANGO_SAVE_ARENA_BE_WORSHIP);
		buf.write_int64(role_id);
		if ( rank_player )
		    buf.write_int32(rank_player->rank_info_.be_worship);
		else
			buf.write_int32(1);   // 为保险
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, role_id);
//		Player_DB_View *player_db = LOGIC_MONITOR->find_player_db_view_by_role_id(role_id);
//		if ( player_db )
//			player_db->be_worship = value;
	}
	return 0;
}

void Arena_Manager::set_sync_to_inner_scene_flag(bool value){
	sync_to_inner_scene_ = value;
}

int Arena_Manager::get_season_remain_day(void){
	int remain_sec = this->arena_season_detail_.season_finish.sec() - Time_Value::gettimeofday().sec();
	int remain_day = remain_sec/Time_Value::ONE_DAY_IN_SECS+1;
	if(remain_day < 0){
		remain_day = 0;
	}
	return remain_day;
}

Arena_Rank_Player* Arena_Manager::get_local_first_player() {

	if ( ranking_vec_.size() > 0)
		return ranking_vec_[0];
	else
		return NULL;
}

int Arena_Manager::req_arena_match(role_id_t role_id, int highest_force, int8_t refresh/* = 0*/){
	MSG_DEBUG("a_i_m match_arena_battle");
	if(is_arena_running() == false){
		MSG_USER_TRACE("arena match error no running");
		return -1;
	}
	Arena_Rank_Player* fight_player = find_rank_player(role_id);
	if(fight_player == NULL){
		MSG_USER_TRACE("arena match error fight player NULL: %ld", role_id);
		return -1;
	}
	Logic_Player* fight_logic = fight_player->logic_player();
	if(fight_logic == NULL){
		MSG_USER_TRACE("arena match error logic player NULL: %ld", role_id);
		return -1;
	}
	int group = fight_player->group();
	int seg = fight_player->seg();
	int group_index = group*10+seg;
	int rise = fight_logic->is_arena_rise();

	Arena_Base_Config* arena_base_cfg = CONFIG_CACHE_ARENA->get_arena_base_conifg(group_index);
	if(!arena_base_cfg){
		MSG_USER_TRACE("arena match error config NULL group_index:%d", group_index);
		return -1;
	}
	role_id_t match_player1 = 0;
	role_id_t match_player2 = 0;
	role_id_t match_player3 = 0;
	int8_t match_dlv_1 = 0;
	int8_t match_dlv_2 = 0;
	int8_t match_dlv_3 = 0;
	bool match = fight_logic->match_player_info(match_player1, match_player2, match_player3,
			match_dlv_1, match_dlv_2, match_dlv_3);

	std::vector<Arena_Match_P> match_players;
	if(arena_base_cfg->arena_serve == 1){// 匹配跨服玩家
		Block_Buffer buf;
		buf.make_message(INNER_ARENA_MATCH_PLAYER);
		int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
		buf.write_int32(inner_area);
		buf.write_int8(refresh);
		buf.write_int64(fight_logic->role_id());
		buf.write_int32(fight_logic->streak_lose());
		buf.write_int32(fight_logic->rise_fail());
		buf.write_int32(highest_force);
		if(rise == 0 && match){// 已经匹配过的
			buf.write_int8(1);
			buf.write_int64(match_player1);
			buf.write_int64(match_player2);
			buf.write_int64(match_player3);
			buf.write_int8(match_dlv_1);
			buf.write_int8(match_dlv_2);
			buf.write_int8(match_dlv_3);
		}else{// 没有匹配过
			buf.write_int8(0);
		}
		buf.finish_message();
		this->send_to_inner_arena(buf);
	}else{// 匹配本服玩家
		// 匹配玩家
		if(rise == 0 && match == true){
			match_players.push_back(Arena_Match_P(match_player1, match_dlv_1));
			match_players.push_back(Arena_Match_P(match_player2, match_dlv_2));
			match_players.push_back(Arena_Match_P(match_player3, match_dlv_3));
		}else{
			match_player(fight_player, highest_force, rise, group, match_players);
		}
		if(match_players.empty()){
			MSG_USER_TRACE("arena match error local match NULL %ld", role_id);
			return -1;
		}

		// 匹配后添加到等待加载完成队列中
		std::vector<Arena_Load_Status> load_status_vec;
		int result_num = match_players.size();
		for(int i = 0; i < result_num; ++i){
			role_id_t match_id = match_players[i].role_id;
			int8_t match_lv = match_players[i].difficult_lv;
			Arena_Load_Status load_status;
			Arena_Rank_Player* match_player = find_rank_player(match_id);
			if(match_player == NULL){
				MSG_USER_TRACE("arena match error local match NULL %ld", role_id);
				continue;
			}
			load_status.role_id = match_id;
			load_status.difficult_lv = match_lv;
			load_status.server_name = "";
			load_status.score = match_player->score();
			load_status.group = match_player->group();
			load_status.seg = match_player->seg();
			load_status.rank = match_player->local_rank();
			load_status_vec.push_back(load_status);
		}
		this->add_match_status(fight_player->role_id(), load_status_vec, refresh);

		// 请求加载匹配到的玩家的数据
		for(int i = 0; i < result_num; ++i){
			role_id_t match_id = match_players[i].role_id;
			Arena_Rank_Player* match_player = find_rank_player(match_id);
			if(match_player == NULL){
				MSG_USER_TRACE("arena match error local match NULL %ld", role_id);
				continue;
			}
			Logic_Player* match_logic = match_player->logic_player();
			if(match_logic){
				Logic_Player_Detail* match_data = match_logic->logic_player_detail();
				this->process_player_data_complete(match_data, role_id, false);
			}else{
				this->fetch_match_player_data(role_id, match_player->role_id());
			}
		}

	}
	return 0;
}
int Arena_Manager::req_arena_fight(role_id_t role_id, int index){
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player == NULL){
		return -1;
	}
	Arena_Match_Status_Map::iterator it = arena_ready_status_map_.find(role_id);
	if(it == arena_ready_status_map_.end()){
		return -1;
	}
	int match_num = it->second.match_players.size();
	if(index < 0 || index > match_num){
		return -1;
	}
	role_id_t match_id = it->second.match_players[index].role_id;
	std::string match_role_name = it->second.match_players[index].role_name;
	int8_t difficult_lv = it->second.match_players[index].difficult_lv;
	int result = battle_start(role_id, match_id, match_role_name, difficult_lv);
	return result;
}
int Arena_Manager::req_arena_match_refresh(role_id_t role_id){
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player == NULL){
		return -1;
	}
	role_id_t match_id_1 = 0;
	role_id_t match_id_2 = 0;
	role_id_t match_id_3 = 0;
	int8_t match_dlv_1 = 0;
	int8_t match_dlv_2 = 0;
	int8_t match_dlv_3 = 0;
	bool match = player->match_player_info(match_id_1, match_id_2, match_id_3,
			match_dlv_1, match_dlv_2, match_dlv_3);
	if(!match){
		return -1;
	}
	int cost = CONFIG_CACHE_ARENA->get_refresh_match_cost();
	if(cost > 0){
		int result = player->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), cost, MONEY_SUB_ARENA_REFRESH_MATCH));
		if(result){
			return result;
		}
	}
	player->set_match_player_info(Time_Value::zero, 0, 0, 0, 0, 0, 0);
	req_arena_match(role_id, player->get_highest_force(), 1);
	return 0;
}

int Arena_Manager::req_arena_role_panel(Logic_Player* player, role_id_t target_id, int8_t index){
	if(!player){
		return -1;
	}
	Arena_Match_Status_Map::iterator it = arena_ready_status_map_.find(player->role_id());
	if(it == arena_ready_status_map_.end()){
		return -1;
	}
	int match_num = it->second.match_players.size();
	if(index > match_num){
		return -1;
	}
	std::string target_role_name = it->second.match_players[index].role_name;
	Logic_Player* target = LOGIC_MONITOR->find_logic_player_by_role_id(target_id);
	if(target){// online
		MSG_50100415 msg;
		msg.role_id = target_id;
		msg.gang_name = target->base_detail().gang_name;
		msg.gender = target->base_detail().gender;
		msg.role_name = target_role_name;
		msg.gang_id = target->base_detail().gang_id;
		target->fighter_detail().property_list(msg.prop_value);
		msg.prop_value.push_back(Property(PT_CAREER, target->career()));
		OBJ_SEND_TO_CLIENT(msg, (*player));
		//hero
		player->active_panal_hero_tags(target);
		player->active_panal_hero_info(target);
		return 0;
	}else{// offline
		Arena_Match_Player_Data* match_data = player->get_arena_match_player_data(target_id);
		if(!match_data){
			MSG_50100415 msg;
			msg.role_id = 0;
			Block_Buffer buf;
			OBJ_SEND_TO_CLIENT(msg, (*player));
			return -1;
		}
		target = POOL_MANAGER->pop_logic_player();
		if (!target) {
			MSG_DEBUG("logic_player_pool_.pop() return 0.");
			MSG_50100415 msg;
			msg.role_id = 0;
			OBJ_SEND_TO_CLIENT(msg, (*player));
			return -1;
		}
		target->load_base_detail(match_data->base_detail);
		target->Logic_Fighter::load_detail(match_data->fighter_detail);
		target->Logic_Heroer::load_detail(match_data->heroer_detail);
		MSG_50100415 msg;
		msg.role_id = target_id;
		msg.gang_name = target->base_detail().gang_name;
		msg.gender = target->base_detail().gender;
		msg.role_name = target_role_name;
		msg.gang_id = target->base_detail().gang_id;
		target->fighter_detail().property_list(msg.prop_value);
		msg.prop_value.push_back(Property(PT_CAREER, target->career()));
		OBJ_SEND_TO_CLIENT(msg, (*player));
		//hero
		player->active_panal_hero_tags(target);
		player->active_panal_hero_info(target);
		POOL_MANAGER->push_logic_player(target);
		return 0;
	}
	return 0;
}

void Arena_Manager::season_finish(void){
	Time_Value now = Time_Value::gettimeofday();
	//========保存排名==========
	this->sort_rank();
	save_season_rank();
	//========清理竞技场数据====
	this->season_clear_data();
	//========通知跨服赛季结束======
	{
		Block_Buffer buf;
		buf.make_message(INNER_ARENA_SEASON_FINISH);
		int inner_area = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
		buf.write_int32(inner_area);
		buf.write_int64(logic_uuid_);
		buf.finish_message();
		this->send_to_inner_arena(buf);
	}
	//====开启下一个赛季=======
	//赛季结束时间 每月1号，6点
	arena_season_detail_.season_begin = this->get_season_begin_time(now);
	arena_season_detail_.season_finish = this->get_season_finish_time(now);
	arena_season_detail_.season_status = ARENA_BATTLE_STATUS_RUNNING;
	arena_season_detail_.save();
	init_all_arena_player();
	sort_rank(true);
	int count = (now.sec() - arena_season_detail_.season_begin.sec())/(Time_Value::ONE_DAY_IN_SECS*3);
	inner_award_time_ = arena_season_detail_.season_begin + Time_Value(Time_Value::ONE_DAY_IN_SECS*3*count) + Time_Value(60*60*24*7, 0);
	Time_Value timeout = next_relative_time(22, 00, 00);
	local_award_time_ = now + timeout;
	sync_all_to_inner_scene();
}
void Arena_Manager::season_begin(void){

}

void Arena_Manager::match_player(Arena_Rank_Player* fight_player, int highest_force, const int rise, const int group, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	int seg = fight_player->seg();
	int match_num = 1;
	if(rise == 1){
		int ref_force = CONFIG_CACHE_ARENA->get_bronze_refer_force(group, seg);
		if(ref_force > 0){
			return match_first_group_rise(fight_player, highest_force, match_num, match_players);
		}else{
			if(fight_player->rise_fail() == 0){
				return match_nor_group_rise(fight_player, highest_force, match_num, match_players);
			}else if(fight_player->rise_fail() == 1){
				return match_nor_group_rise1(fight_player, highest_force, match_num, match_players);
			}else{
				return match_nor_group_rise2(fight_player, highest_force, match_num, match_players);
			}
		}
	}else{
		Logic_Player* logic_player = fight_player->logic_player();
		int streak_lose = 0;
		if(logic_player != NULL){
			streak_lose = logic_player->streak_lose();
		}
		// god
		bool on_top_seg_key = is_on_top_seg_key(group, seg);
		role_id_t difficult_id_1 = 0;
		role_id_t difficult_id_2 = 0;
		role_id_t difficult_id_3 = 0;
		int8_t difficult_type = 0;//
		if(!on_top_seg_key && match_difficult_god(fight_player, highest_force, difficult_id_1)){
			match_num = 2;
			if(match_difficult_hard(fight_player, highest_force, difficult_id_1, difficult_id_2)){
				match_num = 1;
				std::vector<Arena_Match_P> match_easy;
				if( streak_lose >= 2){//连败2场或以上
					//只匹配低端段位,战力低于玩家的对手
					match_low_group_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
				}else{//常规匹配
					match_low_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
				}
				if(match_easy.empty() == false){
					difficult_id_3 = match_easy[0].role_id;
				}
				difficult_type = 0;
			}else{
				match_num = 2;
				std::vector<Arena_Match_P> match_easy;
				if( streak_lose >= 2){//连败2场或以上
					//只匹配低端段位,战力低于玩家的对手
					match_low_group_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
				}else{//常规匹配
					match_low_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
				}
				if(match_easy.empty() == false){
					difficult_id_2 = match_easy[0].role_id;
				}
				if(match_easy.size() >= 2){
					difficult_id_3 = match_easy[1].role_id;
				}
				difficult_type = 1;
			}
		}else{
			if(match_difficult_hard(fight_player, highest_force, 0, difficult_id_1)){
				if(match_difficult_hard(fight_player, highest_force, difficult_id_1, difficult_id_2)){
					match_num = 1;
					std::vector<Arena_Match_P> match_easy;
					if( streak_lose >= 2){//连败2场或以上
						//只匹配低端段位,战力低于玩家的对手
						match_low_group_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
					}else{//常规匹配
						match_low_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
					}
					if(match_easy.empty() == false){
						difficult_id_3 = match_easy[0].role_id;
					}
					difficult_type = 2;
				}else{
					match_num = 2;
					std::vector<Arena_Match_P> match_easy;
					if( streak_lose >= 2){//连败2场或以上
						//只匹配低端段位,战力低于玩家的对手
						match_low_group_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
					}else{//常规匹配
						match_low_fight(fight_player, highest_force, match_num, match_easy, difficult_id_1, difficult_id_2);
					}
					if(match_easy.empty() == false){
						difficult_id_2 = match_easy[0].role_id;
					}
					if(match_easy.size() >= 2){
						difficult_id_3 = match_easy[1].role_id;
					}
					difficult_type = 3;
				}
			}else{
				match_num = 3;
				std::vector<Arena_Match_P> match_easy;
				if( streak_lose >= 2){//连败2场或以上
					//只匹配低端段位,战力低于玩家的对手
					match_low_group_fight(fight_player, highest_force, match_num, match_easy);
				}else{//常规匹配
					match_low_fight(fight_player, highest_force, match_num, match_easy);
				}
				if(match_easy.size() >= 1){
					difficult_id_1 = match_easy[0].role_id;
				}
				if(match_easy.size() >= 2){
					difficult_id_2 = match_easy[1].role_id;
				}
				if(match_easy.size() >= 3){
					difficult_id_3 = match_easy[2].role_id;
				}
				difficult_type = 4;
			}
		}
		// sort
		Arena_Rank_Player* dc_rank;
		std::vector<Arena_Rank_Player*> dc_rank_vec;
		dc_rank = this->find_rank_player(difficult_id_1);
		if(dc_rank){
			dc_rank_vec.push_back(dc_rank);
		}
		dc_rank = this->find_rank_player(difficult_id_2);
		if(dc_rank){
			dc_rank_vec.push_back(dc_rank);
		}
		dc_rank = this->find_rank_player(difficult_id_3);
		if(dc_rank){
			dc_rank_vec.push_back(dc_rank);
		}
		std::sort(dc_rank_vec.begin(), dc_rank_vec.end(), Fight_Greater);
		int dc_count = dc_rank_vec.size();
		for(int i = 0; i < dc_count; ++i){
			if(i == 0){
//				if(difficult_type == 4){
//					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 0));
//				}else if(difficult_type == 0 || difficult_type == 1){
//					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 2));
//				}else{
//					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
//				}
				if(on_top_seg_key){// 最高段位的玩家,无法匹配到神级,但会有两个困难级(奖励,显示也是)
					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
				}else{
					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 2));
				}
			}else if(i == 1){
//				if(difficult_type == 0 || difficult_type == 2){
//					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
//				}else{
//					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 0));
//				}
				match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
			}else{
				match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 0));
			}
		}
	}
}

void Arena_Manager::match_first_group_rise(Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int rise_fail = fight_player->rise_fail();
	//参考战力
	int group = fight_player->group();
	int seg = fight_player->seg();
	int ref_force = CONFIG_CACHE_ARENA->get_bronze_refer_force(group, seg);
	ref_force = ref_force-(ref_force*rise_fail*0.2);//是第二次晋级,参考战力计算时降低20%
	int ref_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(ref_force);
	//若设置了参考战力,在+3~-3个段位内筛选出参考战力-1~+1阶段进行随机匹配
	Role_Id_Set result_set;
	int result_num;
	int seg_key = this->get_arena_seg_key(group, seg);
	for(int i = seg_key-3; i <= seg_key+3; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv-1; k <= ref_force_lv+1; ++k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//如果人数少于3人,则在+1~+3个段位内随机
	result_set.clear();
	for(int i = seg_key+1; i <= seg_key+3; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 若还是少于3人,在-1~+3内随机
	result_set.clear();
	for(int i = seg_key-1; i <= seg_key+3; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 若还是少于3人,在-3~+3段位内随机
	result_set.clear();
	for(int i = seg_key-3; i <= seg_key+3; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 若还是少于3人,在全服内随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 2){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Arena_Rank_Player* match_player = get_random_player(fight_player);
	if(match_player){
		int remain_num = match_num - match_players.size();
		if(remain_num > 0){
			for(int i = 0; i < remain_num; ++i){
				match_players.push_back(Arena_Match_P(match_player->role_id(), 0));
			}
		}
	}
	return;
}

void Arena_Manager::match_nor_group_rise(Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);

	int rise_seg_num = 0;
	if(seg_key >= MIN_ARENA_SEG_KEY && seg_key <= MAX_ARENA_SEG_KEY){
		rise_seg_num = arena_seg_map_[seg_key].seg_set.size();
	}
	int rise_seg_num_41 = rise_seg_num/4;
	if(rise_seg_num_41 < RISE_MATCH_MIN_NUM){
		rise_seg_num_41 = RISE_MATCH_MIN_NUM;
	}

	Role_Id_Set result_set;
	int result_num = 0;
	// 优先在本段位内随机匹配, 匹配战力最高段的round(1/4*本段位人数)个人进行匹配
	// 若低于本段1/4的人数4人,则需要补全的人数根据以下优先级进行匹配 +1段位-->-1段-->-2段-->-3
	result_set.clear();
	for(int i = seg_key; i <= seg_key; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=CONFIG_CACHE_ARENA->get_arena_max_force_lv(); k >= 1; --k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
			int result_num = result_set.size();
			if(result_num > rise_seg_num_41){
				break;
			}
		}
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 若低于本段1/4的人数4人,则需要补全的人数根据以下优先级进行匹配 +1段位-->-1段-->-2段-->-3
	for(int i = seg_key+1; i >= seg_key-3; --i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		if(i == seg_key){
			continue;
		}
		bool out_break = false;
		for(int k=CONFIG_CACHE_ARENA->get_arena_max_force_lv(); k >= 1; --k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
			int result_num = result_set.size();
			if(result_num > RISE_MATCH_MIN_NUM){
				out_break = true;
				break;
			}
		}
		if(out_break){
			break;
		}
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//若人数还是不足,在+1段~-3段之间随机
	result_set.clear();
	for(int i = seg_key-3; i <= seg_key+1; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//若人数还是不足,在+1段之后所有段位随机
	result_set.clear();
	for(int i = seg_key; i <= MAX_ARENA_SEG_KEY; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
		result_num = result_set.size();
		if(result_num > RISE_MATCH_MIN_NUM){
			break;
		}
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//若人数还是不足,在全服随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Arena_Rank_Player* match_player = get_random_player(fight_player);
	if(match_player){
		int remain_num = match_num - match_players.size();
		if(remain_num > 0){
			for(int i = 0; i < remain_num; ++i){
				match_players.push_back(Arena_Match_P(match_player->role_id(), 0));
			}
		}
	}
	return;
}


void Arena_Manager::match_nor_group_rise1(Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);
	Role_Id_Set result_set;
	int result_num = 0;

	//优先在本段位内,~所有低于自己段位之间随机
	result_set.clear();
	for(int i = seg_key; i >= MIN_ARENA_SEG_KEY; --i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
		result_num = result_set.size();
		if(result_num > RISE_MATCH_MIN_NUM){
			break;
		}
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//若人数还是不足,在全服随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Arena_Rank_Player* match_player = get_random_player(fight_player);
	if(match_player){
		int remain_num = match_num - match_players.size();
		if(remain_num > 0){
			for(int i = 0; i < remain_num; ++i){
				match_players.push_back(Arena_Match_P(match_player->role_id(),0));
			}
		}
	}
	return;
}


void Arena_Manager::match_nor_group_rise2(Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);
	int ref_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(highest_force);

	Role_Id_Set result_set;
	int result_num = 0;

	//优先在本段位内,在本段位~-3段之间低于自己战力层的人全随机,至少4个人
	result_set.clear();
	for(int i = seg_key; i >= seg_key-3; --i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv-1; k >= 1; --k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
		result_num = result_set.size();
		if(result_num > RISE_MATCH_MIN_NUM){
			break;
		}
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//若人数不足3人,全服低于自己战力的人全随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv-1; k >= 1; --k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id,0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//若人数还是不足,在全服随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
				arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > RISE_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Arena_Rank_Player* match_player = get_random_player(fight_player);
	if(match_player){
		int remain_num = match_num - match_players.size();
		if(remain_num > 0){
			for(int i = 0; i < remain_num; ++i){
				match_players.push_back(Arena_Match_P(match_player->role_id(), 0));
			}
		}
	}
	return;
}

void Arena_Manager::match_low_group_fight(Arena_Rank_Player* fight_player,int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
		const role_id_t df_role_id_1/* = 0*/, const role_id_t df_role_id_2/* = 0*/){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int ref_force = highest_force;
	int ref_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(ref_force);
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);
	Role_Id_Set result_set;
	int result_num = 0;
	// 优先在本段位内,战力在本阶层-1~-3阶层之间随机
	result_set.clear();
	for(int i = seg_key; i <= seg_key; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv-3; k <= ref_force_lv-1; ++k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	result_num = result_set.size();
	if(result_num > NORMAL_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//如果随机出来不足10人,在本段位 扩大到-1~-5阶层之间随机战力-1~-5
	result_set.clear();
	for(int i = seg_key; i >= seg_key-5; --i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv-1; k >= ref_force_lv-5; --k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
			int count = result_set.size();
			if(count > NORMAL_MATCH_MIN_NUM){
				break;
			}
		}
	}
	result_num = result_set.size();
	if(result_num > NORMAL_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 如果人数依然不足,在本段位~低段位内全随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= seg_key; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=1; k <= ref_force_lv-1; ++k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//如果人数再不足,在全服随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
							arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Arena_Rank_Player* match_player = get_random_player(fight_player);
	if(match_player){
		int remain_num = match_num - match_players.size();
		if(remain_num > 0){
			for(int i = 0; i < remain_num; ++i){
				match_players.push_back(Arena_Match_P(match_player->role_id(), 0));
			}
		}
	}
	return;
}

void Arena_Manager::match_low_fight(Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
		const role_id_t df_role_id_1/* = 0*/, const role_id_t df_role_id_2/* = 0*/){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int ref_force = highest_force;
	int ref_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(ref_force);
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);
	Role_Id_Set result_set;
	int result_num = 0;
	// 优先在本段位内,战力在本阶层+1~-3阶层之间随机
	result_set.clear();
	for(int i = seg_key; i <= seg_key; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}

		// 0
		{
			int k = ref_force_lv;
			if(k >= 1 && k <= CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}

		// -1
		{
			int k = ref_force_lv-1;
			if(k >= 1 && k <= CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}

		// +1
		{
			int k = ref_force_lv+1;
			if(k >= 1 && k <= CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}

		// - 2
		{
			int k = ref_force_lv-2;
			if(k >= 1 && k <= CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}

		// - 3
		{
			int k = ref_force_lv-3;
			if(k >= 1 && k <= CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}
//		for(int k=ref_force_lv-3; k <= ref_force_lv+1; ++k){
//			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
//				continue;
//			}
//			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
//					arena_seg_map_[i].force_set_map[k].end());
//		}
	}
	result_num = result_set.size();
	if(result_num > NORMAL_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	//如果随机出来不足10人,在本阶层+1~-5阶层之间随机
	result_set.clear();
	for(int i = seg_key; i <= seg_key; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv-5; k <= ref_force_lv+1; ++k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	result_num = result_set.size();
	if(result_num > NORMAL_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 如果人数依然不足,在本段位 扩大到~-1 扩大到-2 扩大到-3低段位内全随机, 战力阶层随机范围不限
	result_set.clear();
	for(int i = seg_key; i >= seg_key-3; --i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
							arena_seg_map_[i].seg_set.end());
		int count = result_set.size();
		if(count > NORMAL_MATCH_MIN_NUM){
			break;
		}
	}
	result_num = result_set.size();
	if(result_num > NORMAL_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}
	//在自身名次-5~+5之间随机匹配(如果跨服,按跨服排名)
	result_set.clear();
	{
		int self_rank = fight_player->local_rank();
		for(int i = self_rank -5; i <= self_rank+5; ++i){
			if(i == self_rank){
				continue;
			}
			Arena_Rank_Player* tmp_rank_info = get_rank_player_from_rank(i);
			if(!tmp_rank_info){
				continue;
			}
			result_set.insert(tmp_rank_info->role_id());
		}
	}
	result_num = result_set.size();
	if(result_num > NORMAL_MATCH_MIN_NUM){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}
	//如果人数再不足,在全服随机
	result_set.clear();
	for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
		result_set.insert(arena_seg_map_[i].seg_set.begin(),
							arena_seg_map_[i].seg_set.end());
	}
	result_num = result_set.size();
	if(result_num > 3){
		int num = 0;
		while(num < match_num){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Arena_Rank_Player* match_player = get_random_player(fight_player);
	if(match_player){
		int remain_num = match_num - match_players.size();
		if(remain_num > 0){
			for(int i = 0; i < remain_num; ++i){
				match_players.push_back(Arena_Match_P(match_player->role_id(), 0));
			}
		}
	}
	return;
}

bool Arena_Manager::match_difficult_hard(Arena_Rank_Player* fight_player,int highest_force, const role_id_t god_role_id, role_id_t& rt_match_id){
	if(fight_player == NULL){
		return false;
	}
	role_id_t fight_id = fight_player->role_id();
	int ref_force = highest_force;
	int ref_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(ref_force);
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);
	Role_Id_Set result_set;
	int result_num = 0;
	// 在高1段位~本段位中,寻找战力范围+1~+3中随机
	result_set.clear();
	for(int i = seg_key; i <= seg_key+1; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv+1; k <= ref_force_lv+3; ++k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	// 在高1段位~本段位中,寻找战力范围+1~-5中随机
	result_num = result_set.size();
	if(result_num < 4){
		for(int i = seg_key; i <= seg_key+1; ++i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			for(int k=ref_force_lv; k >= ref_force_lv-5; --k){
				if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
					continue;
				}
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}
	}
	//在高1段位~本段位中中随机
	result_num = result_set.size();
	if(result_num < 4){
		for(int i = seg_key; i <= seg_key+1; ++i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			for(int k=CONFIG_CACHE_ARENA->get_arena_max_force_lv(); k >= 1; --k){
				if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
					continue;
				}
				if(k >= ref_force_lv-5 && k <= ref_force_lv+3){
					continue;
				}
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}
	}
	//在高1段位~-1段位中中随机
	result_num = result_set.size();
	if(result_num < 4){
		for(int i = seg_key-1; i <= seg_key-1; ++i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			result_set.insert(arena_seg_map_[i].seg_set.begin(), arena_seg_map_[i].seg_set.end());
		}
	}
	//在高1段位~-2段位中中随机
	result_num = result_set.size();
	if(result_num < 4){
		for(int i = seg_key-2; i <= seg_key-2; ++i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			result_set.insert(arena_seg_map_[i].seg_set.begin(), arena_seg_map_[i].seg_set.end());
		}
	}
	//全服随机
	result_num = result_set.size();
	if(result_num < 4){
		result_set.clear();
		for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
			result_set.insert(arena_seg_map_[i].seg_set.begin(), arena_seg_map_[i].seg_set.end());
		}
	}
	if(result_num > 0){
		int num = 0;
		while(num < 1){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id && rd_id != god_role_id){
				rt_match_id = rd_id;
				num += 1;
				return true;
			}
			result_set.erase(it_rd);
			result_num -= 1;
			if(result_num <= 0){
				break;
			}
		}
	}
	return false;
}

bool Arena_Manager::match_difficult_god(Arena_Rank_Player* fight_player,int highest_force, role_id_t& rt_match_id){
	if(fight_player == NULL){
		return false;
	}
	role_id_t fight_id = fight_player->role_id();
	int ref_force = highest_force;
	int ref_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(ref_force);
	int group = fight_player->group();
	int seg = fight_player->seg();
	int seg_key = this->get_arena_seg_key(group, seg);
	Role_Id_Set result_set;
	int result_num = 0;
	//在高1段位~本段位中,寻找战力范围+1~+5中随机
	result_set.clear();
	for(int i = seg_key; i <= seg_key+1; ++i){
		if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
			continue;
		}
		for(int k=ref_force_lv+1; k <= ref_force_lv+5; ++k){
			if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				continue;
			}
			result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
					arena_seg_map_[i].force_set_map[k].end());
		}
	}
	result_num = result_set.size();
	//在高1段位~本段位中,寻找战力范围+1~+-5中随机
	if(result_num <= 2){
		for(int i = seg_key; i <= seg_key+1; ++i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			for(int k=ref_force_lv; k >= ref_force_lv-5; --k){
				if(k<1 || k > CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
					continue;
				}
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}
	}
	//在高1段位~本段位中中随机
	//在高1段位~-1段位中中随机
	//在高1段位~-2段位中中随机
	result_num = result_set.size();
	if(result_num <= 2){
		result_set.clear();
		for(int i = seg_key; i <= seg_key+1; ++i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			result_set.insert(arena_seg_map_[i].seg_set.begin(), arena_seg_map_[i].seg_set.end());
		}
	}
	result_num = result_set.size();
	if(result_num <= 2){
		for(int i = seg_key-1; i >= seg_key-2; --i){
			if(i<MIN_ARENA_SEG_KEY || i > MAX_ARENA_SEG_KEY){
				continue;
			}
			result_set.insert(arena_seg_map_[i].seg_set.begin(), arena_seg_map_[i].seg_set.end());
			if(result_set.size() >= 2){
				break;
			}
		}
	}
	//全服随机
	result_num = result_set.size();
	if(result_num <= 2){
		result_set.clear();
		for(int i = MIN_ARENA_SEG_KEY; i <= MAX_ARENA_SEG_KEY; ++i){
			result_set.insert(arena_seg_map_[i].seg_set.begin(), arena_seg_map_[i].seg_set.end());
		}
	}
	if(result_num > 0){
		int num = 0;
		while(num < 1){
			Time_Value now = Time_Value::gettimeofday();
			srandom(now.sec() + now.usec());
			int match_rd_num = result_num;
			if(match_rd_num > MAX_ARENA_MATCH_RD_NUM){
				match_rd_num = MAX_ARENA_MATCH_RD_NUM;
			}
			int index = random() % match_rd_num;
			Role_Id_Set::iterator it = result_set.begin();
			Role_Id_Set::iterator it_rd = it;
			while(index > 0){
				++it_rd;
				--index;
			}
			role_id_t rd_id = *it_rd;
			if(fight_id != rd_id){
				rt_match_id = rd_id;
				num += 1;
				return true;
			}
			result_set.erase(it_rd);
			result_num -= 1;
			if(result_num <= 0){
				break;
			}
		}
	}
	return false;
}

int Arena_Manager::process_match_complete(role_id_t fight_id, std::vector<Arena_Load_Status> match_status, int8_t refresh, bool push_cache/* = true*/){
	if(match_status.empty()){
		MSG_USER_TRACE("process_match_complete error match_status NULL fight_id:%ld", fight_id);
	}

	Arena_Rank_Player* rank_player = find_rank_player(fight_id);
	if(!rank_player){
		MSG_USER_TRACE("arena process_player_match_complete error fight_player NULL:%ld", fight_id);
		return -1;
	}
	Logic_Player* logic_player = rank_player->logic_player();
	if(!logic_player){
		MSG_USER_TRACE("arena process_player_match_complete error fight_logic_player NULL:%ld", fight_id);
		return -1;
	}

	int match_num = match_status.size();

	MSG_DEBUG("a_m process_match_complete fight_id:%ld, match_count:%d", fight_id, match_num);

	//匹配时间
	bool b_match = false;
	int remain_time = 0;
	if(match_num == 3){
		logic_player->get_arena_match_remain_time(b_match, remain_time);
		if(!b_match){
			remain_time = CONFIG_CACHE_ARENA->get_refresh_match_time();
			Time_Value finish_time = Time_Value::gettimeofday() + Time_Value(remain_time);
			logic_player->set_match_player_info(finish_time, match_status[0].role_id,
					match_status[1].role_id, match_status[2].role_id,
					match_status[0].difficult_lv,match_status[1].difficult_lv, match_status[2].difficult_lv);
		}
	}


	MSG_50100412 msg;
	msg.remain_time = remain_time;
	for(int i = 0; i < match_num; ++i){
		role_id_t match_id = match_status[i].role_id;
		Arena_Match_Player_Data* arena_match_player_data = logic_player->get_arena_match_player_data(match_id);
		if(!arena_match_player_data){
			MSG_USER_TRACE("process_match_complete error match_data NULL fight_id:%ld, match_id:%ld", fight_id, match_id);
			continue;
		}

		if(fight_id == match_status[i].role_id){// 匹配到自己修改名称
			if(i == 0){
				match_status[i].role_name = "Alisa"+random()%10;
			}else if( i == 2){
				match_status[i].role_name = "Danrtsey"+random()%100;
			}else{
				match_status[i].role_name = "ShreK";
			}
		}else{
			match_status[i].role_name = arena_match_player_data->base_detail.role_name;
		}
		Arena_Match_Info info;
		info.server_name = match_status[i].server_name;
		info.role_id = match_status[i].role_id;
		info.difficult_lv = match_status[i].difficult_lv;
		info.role_name = match_status[i].role_name;
		info.level = arena_match_player_data->base_detail.level;
		info.arousal = arena_match_player_data->base_detail.awake_lvl;
		info.title_id = arena_match_player_data->base_detail.title_id;
		info.avatar_vec = arena_match_player_data->base_detail.avatar_vec;
		info.career = arena_match_player_data->base_detail.career;
		info.gender = arena_match_player_data->base_detail.gender;
		info.score = match_status[i].score;
		info.group = match_status[i].group;
		info.seg = match_status[i].seg;
//		// 王者组，需要排名
//		if(info.group == ARENA_MAX_GROUP){
//			info.rank = match_status[i].rank;
//		}
		info.rank = match_status[i].rank;

		info.head_id = arena_match_player_data->base_detail.head_id;
		int t_hero_force = 0;
		// 战斗英雄信息
		int fighter_idx = arena_match_player_data->heroer_detail.fighter_index;
		Heroer_Detail::Hero_Map::const_iterator it_hero = arena_match_player_data->heroer_detail.hero_map.find(fighter_idx);
		if(it_hero != arena_match_player_data->heroer_detail.hero_map.end()){
			info.hero_id = it_hero->second.hero_id;
			info.hero_name = it_hero->second.hero_name;
			info.hero_level = it_hero->second.level;
			t_hero_force = it_hero->second.hero_fighter_detail.force;
		}
		int t_ttl_force = arena_match_player_data->fighter_detail.force + t_hero_force;
		if(rank_player->rank_info_.fight_times < ARENA_LOW_FORCE_FIGHT_TIMES){//前3场，对手减50%攻击
			info.fight = t_ttl_force*0.5;
		}else{
			info.fight = t_ttl_force;
		}
		msg.match_info.push_back(info);
	}
	OBJ_SEND_TO_CLIENT(msg, (*logic_player));

	add_ready_player(fight_id, match_status);

	if(refresh == 1){// 刷新匹配的，返回刷新成功
		MSG_50100414 ref_msg;
		ref_msg.result = 1;
		OBJ_SEND_TO_CLIENT(ref_msg, (*logic_player));
	}
	return 0;
}

void Arena_Manager::add_ready_player(const role_id_t fight_id, std::vector<Arena_Load_Status> match_status){
	Arena_Match_Status status;
	status.match_players = match_status;
	arena_ready_status_map_[fight_id] = status;
}

int Arena_Manager::get_arena_seg_key(int group, int seg){
	switch(group){
		case 1:
		{
			return seg;
			break;
		}
		case 2:
		{
			return 5+seg;
			break;
		}
		case 3:
		{
			return 10+seg;
			break;
		}
		case 4:
		{
			return 15+seg;
			break;
		}
		case 5:
		{
			return 20+seg;
			break;
		}
		case 6:
		{
			return 25+seg;
			break;
		}
		default:{
			return 26;
			break;
		}
	}
	return 1;
}
void Arena_Manager::arena_seg_add(role_id_t role_id, int seg_key, int force){
	int force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(force);
	arena_seg_map_[seg_key].add_player(role_id, force_lv);
}

void Arena_Manager::arena_seg_del(role_id_t role_id, int seg_key, int force){
	int force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(force);
	arena_seg_map_[seg_key].del_player(role_id, force_lv);
}

void Arena_Manager::arena_seg_change(role_id_t role_id, int old_seg_key, int new_seg_key, int old_force, int new_force){
	if(old_seg_key != new_seg_key){
		arena_seg_del(role_id, old_seg_key, old_force);
		arena_seg_add(role_id, new_seg_key, new_force);
	}else{
		int old_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(old_force);
		int new_force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(new_force);
		if(old_force_lv != new_force_lv){
			arena_seg_map_[old_seg_key].force_change(role_id, old_force_lv, new_force_lv);
		}
	}
}

bool Arena_Manager::is_exploit_id(int value){
	if(value == 503003){
		return true;
	}
	return false;
}

bool Arena_Manager::is_exp_id(int value){
	if(value == PT_EXP_CURRENT){
		return true;
	}
	return false;
}

bool Arena_Manager::is_resource_id(int value){
	if(value == COPPER){
		return true;
	}
	return false;
}

bool Arena_Manager::is_on_top_seg_key(const int group, const int seg){
	if(ranking_vec_.empty()){
		return true;
	}
	if(group == ranking_vec_[0]->group() && seg == ranking_vec_[0]->seg()){
		return true;
	}
	return false;
}
