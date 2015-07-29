/*
 * Inner_Arena_Manager.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: root
 */

#include "Inner_Arena_Manager.h"
#include "Config_Cache_Arena.h"
#include "Pool_Manager.h"
#include "Logic_Player_Detail.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Arena_Struct.h"

const int ARENA_MAX_GROUP = 6;

const int MIN_ARENA_SEG_KEY = 1;

const int MAX_ARENA_SEG_KEY = 26;

const int MIN_ARENA_INNER_RANK_SEG_KEY = 16;

const int MAX_ARENA_MATCH_RD_NUM = 100;

const int NORMAL_MATCH_MIN_NUM = 5;

const int RISE_MATCH_MIN_NUM = 3;


Inner_Arena_Manager::Inner_Arena_Manager(void){

}

Inner_Arena_Manager::~Inner_Arena_Manager(void){

}

void Inner_Arena_Manager::reset(void){
	rank_player_map_.clear();
	ranked_map_.clear();
	ranking_vec_.clear();
	rank_muid_set_.clear();
	rank_load_data_map_.clear();
	arena_seg_map_.clear();
}

void Inner_Arena_Manager::sort_rank(bool first_times/* = false*/){
	if(ranking_vec_.empty()){
		return;
	}
	Arena_Ranking_Vec inner_rank_change_vec;
	ranked_map_.clear();
	//group_rank_map_.clear();
	std::sort(ranking_vec_.begin(), ranking_vec_.end(), Inner_Rank_Greater);
	Arena_Ranking_Vec::iterator it;
	int rank = 0;
	for(it = ranking_vec_.begin(); it != ranking_vec_.end(); ++it){
		rank += 1;
		int group_index = (*it)->group()*10 + (*it)->seg();
		if((*it)->inner_rank() != rank){
			(*it)->set_inner_rank(rank);
			// 排名更改玩家
			if(group_index > 41){
				inner_rank_change_vec.push_back(*it);
			}
		}

		ranked_map_.insert(std::make_pair((*it)->inner_rank(), (*it)));
		//group_rank_map_[group_index].push_back((*it));
	}
//	Group_Rank_Map::iterator iter;
//	for(iter = group_rank_map_.begin(); iter != group_rank_map_.end(); ++iter){
//		std::sort(iter->second.begin(), iter->second.end(), Inner_Fight_Greater);
//	}
//	fight_rank_vec_ = ranking_vec_;
//	std::sort(fight_rank_vec_.begin(), fight_rank_vec_.end(), Inner_Fight_Greater);
//	if(!rank_change_){
//		rank_change_ = true;
//		refresh_time_  = Time_Value::gettimeofday() + Time_Value(6);
//	}

	if(inner_rank_change_vec.empty() == false){
		sync_inner_rank_info(inner_rank_change_vec);
	}
}


Inner_Arena_Rank_Player* Inner_Arena_Manager::get_rank_player(role_id_t role_id){
	Arena_Rank_Player_Map::iterator it = rank_player_map_.find(role_id);
	if(it != rank_player_map_.end()){
		return it->second;
	}else{
		Inner_Arena_Rank_Player* rank_player = POOL_MANAGER->pop_inner_arena_player_pool();
		rank_player->set_role_id(role_id);
		rank_player_map_.insert(std::make_pair(role_id, rank_player));
		Arena_Rank_Player_Map::iterator it = rank_player_map_.find(role_id);
		if(it != rank_player_map_.end()){
			return it->second;
		}
	}
	return NULL;
}

Inner_Arena_Rank_Player* Inner_Arena_Manager::find_rank_player(role_id_t role_id){
	Arena_Rank_Player_Map::iterator it = rank_player_map_.find(role_id);
	if(it != rank_player_map_.end()){
		return it->second;
	}
	return NULL;
}

Inner_Arena_Rank_Player* Inner_Arena_Manager::get_random_player(Inner_Arena_Rank_Player* rank_player){
	int count = ranked_map_.size();
	if(count == 1){
		return rank_player;
	}
	Inner_Arena_Rank_Player* match_player;
	int rank;
	rank = rank_player->inner_rank();
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

Inner_Arena_Rank_Player* Inner_Arena_Manager::get_rank_player_from_rank(int rank){
	Arena_Ranked_Map::iterator it = ranked_map_.find(rank);
	if(it != ranked_map_.end()){
		return it->second;
	}
	return NULL;
}

int Inner_Arena_Manager::inner_arena_sync_rank_info(Block_Buffer& buf){
	int64_t uuid = 0;
	Monitor_Unique_ID muid;
	int count = 0;
	buf.read_int64(uuid);
	buf.read_int32(count);
	Player_Rank_Info rank_info;
	for(int i = 0; i < count; ++i){
		rank_info.reset();
		rank_info.deserialize(buf);
		Inner_Arena_Rank_Player* rank_player = this->find_rank_player(rank_info.role_id);
		if(rank_player){
			int old_seg_key = this->get_arena_seg_key(rank_player->group(), rank_player->seg());
			int old_force = rank_player->fight();
			rank_player->rank_info.role_name = rank_info.role_name;
			rank_player->rank_info.server_name = rank_info.server_name;
			rank_player->rank_info.level = rank_info.level;
			rank_player->rank_info.fight = rank_info.fight;
			rank_player->rank_info.score = rank_info.score;
			rank_player->rank_info.group = rank_info.group;
			rank_player->rank_info.seg = rank_info.seg;
			rank_player->rank_info.king_time = rank_info.king_time;
			rank_player->rank_info.fight_times = rank_info.fight_times;
			rank_player->uuid = uuid;
			int new_seg_key = this->get_arena_seg_key(rank_player->group(), rank_player->seg());
			int new_force = rank_player->fight();
			this->arena_seg_change(rank_player->role_id(), old_seg_key, old_force, new_seg_key, new_force);
			//
			if(old_seg_key < MIN_ARENA_INNER_RANK_SEG_KEY && new_seg_key >= MIN_ARENA_INNER_RANK_SEG_KEY){
				this->ranking_vec_.push_back(rank_player);
			}
		}else{
			Inner_Arena_Rank_Player* rank_player = this->get_rank_player(rank_info.role_id);
			if(rank_player){
				rank_player->rank_info.role_name = rank_info.role_name;
				rank_player->rank_info.server_name = rank_info.server_name;
				rank_player->rank_info.level = rank_info.level;
				rank_player->rank_info.fight = rank_info.fight;
				rank_player->rank_info.score = rank_info.score;
				rank_player->rank_info.group = rank_info.group;
				rank_player->rank_info.seg = rank_info.seg;
				rank_player->rank_info.king_time = rank_info.king_time;
				rank_player->uuid = uuid;
				int seg_key = this->get_arena_seg_key(rank_player->group(), rank_player->seg());
				this->arena_seg_add(rank_player->role_id(), seg_key, rank_player->fight());
				if(seg_key >= MIN_ARENA_INNER_RANK_SEG_KEY){
					this->ranking_vec_.push_back(rank_player);
				}
			}
		}
	}
	sort_rank();
	return 0;
}

int Inner_Arena_Manager::inner_arena_match_player(Block_Buffer& buf){
	MSG_DEBUG("i_a_m inner_arena_match_player");
	int8_t refresh = 0;
	role_id_t fight_id = 0;
	int streak_lose = 0;
	int rise_fail = 0;
	int highest_force = 0;
	int8_t match = 0;
	role_id_t match_id_1 = 0;
	role_id_t match_id_2 = 0;
	role_id_t match_id_3 = 0;
	int8_t match_dlv_1 = 0;
	int8_t match_dlv_2 = 0;
	int8_t match_dlv_3 = 0;
	buf.read_int8(refresh);
	buf.read_int64(fight_id);
	buf.read_int32(streak_lose);
	buf.read_int32(rise_fail);
	buf.read_int32(highest_force);
	buf.read_int8(match);
	if(match == 1){
		buf.read_int64(match_id_1);
		buf.read_int64(match_id_2);
		buf.read_int64(match_id_3);
		buf.read_int8(match_dlv_1);
		buf.read_int8(match_dlv_2);
		buf.read_int8(match_dlv_3);
	}

	Inner_Arena_Rank_Player* fight_player = this->find_rank_player(fight_id);
	if(fight_player == NULL){
		MSG_USER_TRACE("inner_arena_match_player error rank_player NULL:%ld", fight_id);
		return -1;
	}
	fight_player->arena_streak_lose = streak_lose;
	fight_player->arena_rise_fail = rise_fail;
	std::vector<Arena_Match_P> match_id_vec;
	if(match == 1){
		match_id_vec.push_back(Arena_Match_P(match_id_1, match_dlv_1));
		match_id_vec.push_back(Arena_Match_P(match_id_2, match_dlv_2));
		match_id_vec.push_back(Arena_Match_P(match_id_3, match_dlv_3));
		// check valid
	}else{
		int group = fight_player->group();
		int seg = fight_player->seg();
		int score = fight_player->score();
		int rise = 0;
		Arena_Base_Config* config = CONFIG_CACHE_ARENA->get_arena_base_conifg(group*10+seg);
		if(config){
			if(group != ARENA_MAX_GROUP && score >= config->arena_promote){
				rise =  1;
			}
		}
		this->match_player(fight_player, highest_force, rise, group, match_id_vec);
	}
	int match_num = match_id_vec.size();
	// check valid
	for(int i = 0; i < match_num; ++i){
		Inner_Arena_Rank_Player* match_player = this->find_rank_player(match_id_vec[i].role_id);
		if(!match_player){
			Block_Buffer inner_buf;
			inner_buf.make_message(INNER_ARENA_MATCH_PLAYER);
			inner_buf.write_int8(0);
			inner_buf.write_int8(match);
			inner_buf.write_int8(refresh);
			inner_buf.write_int64(fight_id);
			inner_buf.write_int8(0);
			inner_buf.finish_message();
			int64_t uuid = fight_player->uuid;
			if(uuid == 0){
				MSG_USER_TRACE("inner_arena_fetch_fight_data error rank_uuid NULL:%ld", fight_id);
				return -1;
			}
			SCENE_MONITOR->send_to_logic(uuid, inner_buf);
			MSG_DEBUG("i_a_m to logic INNER_ARENA_MATCH_PLAYER match_player fail rank:%ld,uuid:%ld, match_num:%d",
					 fight_id, uuid, match_num);
			return -1;
		}
	}
	//return match_result
	{
		Block_Buffer inner_buf;
		inner_buf.make_message(INNER_ARENA_MATCH_PLAYER);
		inner_buf.write_int8(1);
		inner_buf.write_int8(match);
		inner_buf.write_int8(refresh);
		inner_buf.write_int64(fight_id);
		inner_buf.write_int8(match_num);
		for(int i = 0; i < match_num; ++i){
			role_id_t match_id = match_id_vec[i].role_id;
			int8_t match_dlv = match_id_vec[i].difficult_lv;
			Inner_Arena_Rank_Player* match_player = this->find_rank_player(match_id);
			int score = 0;
			int group = 1;
			int seg = 1;
			int rank = 0;
			std::string server_name = "";
			if(match_player){
				score = match_player->score();
				group = match_player->group();
				seg = match_player->seg();
				rank = match_player->inner_rank();
				server_name = match_player->server_name();
			}
			inner_buf.write_int64(match_id);
			inner_buf.write_int8(match_dlv);
			inner_buf.write_string(server_name);
			inner_buf.write_int32(score);
			inner_buf.write_int32(group);
			inner_buf.write_int32(seg);
			inner_buf.write_int32(rank);
		}
		inner_buf.finish_message();
		int64_t uuid = fight_player->uuid;
		if(uuid == 0){
			MSG_USER_TRACE("inner_arena_fetch_fight_data error rank_uuid NULL:%ld", fight_id);
			return -1;
		}
		SCENE_MONITOR->send_to_logic(uuid, inner_buf);
		MSG_DEBUG("i_a_m to logic INNER_ARENA_MATCH_PLAYER rank:%ld,uuid:%ld, match_num:%d",
				 fight_id, uuid, match_num);
	}

	//inner load data
	{
		for(int i = 0; i < match_num; ++i){
			role_id_t match_id = match_id_vec[i].role_id;
			int64_t match_uuid = this->get_rank_player_uuid(match_id);
			if(match_uuid == 0){
				MSG_USER_TRACE("inner_arena_match_player error match_player_uuid NULL:%ld", match_id);
				return -1;
			}
			Block_Buffer inner_buf;
			inner_buf.make_message(INNER_ARENA_FETCH_FIGHT_DATA);
			if(fight_player->uuid != match_uuid){//匹配到跨服的才需要
				MSG_DEBUG("i_a_m add_load_data_player");
				//this->add_load_data_player(match_id, fight_id);
				inner_buf.write_int8(1);
			}else{
				inner_buf.write_int8(0);
			}
			inner_buf.write_int64(fight_id);
			inner_buf.write_int64(match_id);
			inner_buf.finish_message();
			SCENE_MONITOR->send_to_logic(match_uuid, inner_buf);
			MSG_DEBUG("i_a_m inner to logic , rank:%ld, uuid:%ld, match:%ld,uuid:%ld",fight_id,
					fight_player->uuid, match_id, match_uuid);
		}
	}

	return 0;
}

int Inner_Arena_Manager::inner_arena_fetch_fight_data(Block_Buffer& buf){
	MSG_DEBUG("i_a_m inner_arena_fetch_fight_data");
	role_id_t fight_id = 0;
	Logic_Player_Detail l_p_data;
	buf.read_int64(fight_id);
	l_p_data.base_detail.deserialize(buf);
	l_p_data.fighter_detail.deserialize(buf);
	l_p_data.heroer_detail.deserialize_fighter(buf);
	role_id_t match_id = l_p_data.base_detail.role_id;
	Inner_Arena_Rank_Player* match_player = this->find_rank_player(match_id);
	if(match_player == NULL){
		MSG_USER_TRACE("inner_arena_fetch_fight_data error match_player NULL:%ld", match_id);
		return -1;
	}

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ARENA_PLAYER_DATA_COMPLETE);
	inner_buf.write_int64(fight_id);
	l_p_data.base_detail.serialize(inner_buf);
	l_p_data.fighter_detail.serialize(inner_buf);
	l_p_data.heroer_detail.serialize_fighter(inner_buf);
	inner_buf.finish_message();

	int64_t fight_uuid = this->get_rank_player_uuid(fight_id);
	if(fight_uuid == 0){
		MSG_USER_TRACE("inner_arena_fetch_fight_data error fight_muid NULL:%ld", fight_id);
		return -1;
	}
	SCENE_MONITOR->send_to_logic(fight_uuid, inner_buf);
	MSG_DEBUG("i_a_m to logic INNER_ARENA_MATCH_PLAYER rank:%ld,uuid:%ld, match:%ld",
			 fight_id, fight_uuid, match_id);
	return 0;
}

int Inner_Arena_Manager::inner_arena_fetch_rank_info(Block_Buffer& buf){
	int64_t fetch_uuid = 0;
	role_id_t fetch_role_id = 0;
	buf.read_int64(fetch_uuid);
	buf.read_int64(fetch_role_id);


	MSG_50100400 inner_msg;
	Inner_Arena_Rank_Player* fetch_rank_player = this->find_rank_player(fetch_role_id);
	if(fetch_rank_player){
		inner_msg.rank = fetch_rank_player->inner_rank();
	}
	inner_msg.type = 1;
	int max_rank_num = 20;
	int rank_num = 0;
	int count = ranking_vec_.size();
	for(int i = 0; i < count; ++i){
		int group_index = ranking_vec_[i]->group()*10 + ranking_vec_[i]->seg();
		if(group_index < 41){
			break;
		}
		Arena_Rank_Info info;
		info.fight = ranking_vec_[i]->fight();
		info.group = ranking_vec_[i]->group();
		info.level = ranking_vec_[i]->level();
		info.name = ranking_vec_[i]->role_name();
		info.rank = ranking_vec_[i]->inner_rank();
		info.role_id = ranking_vec_[i]->role_id();
		info.score = ranking_vec_[i]->score();
		info.seg = ranking_vec_[i]->seg();
		info.server_name = ranking_vec_[i]->server_name();
		inner_msg.rank_info_vec.push_back(info);
		rank_num += 1;
		if(rank_num >= max_rank_num){
			break;
		}
	}
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ARENA_FETCH_RANK_INFO);
	inner_buf.write_int64(fetch_role_id);
	inner_msg.serialize(inner_buf);
	inner_buf.finish_message();
	SCENE_MONITOR->send_to_logic(fetch_uuid, inner_buf);
	return 0;
}

int Inner_Arena_Manager::inner_arena_battle_result(Block_Buffer& buf){
	role_id_t fight_id = 0;
	role_id_t match_id = 0;
	int8_t result = 0;
	Player_Rank_Info rank_info;
	buf.read_int8(result);
	buf.read_int64(match_id);
	rank_info.deserialize(buf);

	fight_id = rank_info.role_id;

	Inner_Arena_Rank_Player* fight_player = this->find_rank_player(fight_id);
	Inner_Arena_Rank_Player* match_player = this->find_rank_player(match_id);
	if(fight_player == NULL){
		return -1;
	}
	int old_seg_key = this->get_arena_seg_key(fight_player->group(), fight_player->seg());
	int old_force = fight_player->fight();
	fight_player->rank_info.role_name = rank_info.role_name;
	fight_player->rank_info.server_name = rank_info.server_name;
	fight_player->rank_info.level = rank_info.level;
	fight_player->rank_info.fight = rank_info.fight;
	fight_player->rank_info.score = rank_info.score;
	fight_player->rank_info.group = rank_info.group;
	fight_player->rank_info.seg = rank_info.seg;
	fight_player->rank_info.king_time = rank_info.king_time;
	fight_player->rank_info.fight_times = rank_info.fight_times;
	int new_seg_key = this->get_arena_seg_key(fight_player->group(), fight_player->seg());
	int new_force = fight_player->fight();
	this->arena_seg_change(fight_player->role_id(), old_seg_key, old_force, new_seg_key, new_force);
	// 参与过竞技场的可以参与排名
	if(old_seg_key < MIN_ARENA_INNER_RANK_SEG_KEY && new_seg_key >= MIN_ARENA_INNER_RANK_SEG_KEY){
		ranking_vec_.push_back(fight_player);
	}
	if(match_player == NULL){
		return -1;
	}
	//
	if(result == 1){//王者组打赢名称高的，互换名次
		if(fight_player->group() == ARENA_MAX_GROUP &&
				match_player->group() == ARENA_MAX_GROUP){
			if(fight_player->rank_info.king_time < match_player->rank_info.king_time){
				int king_time = fight_player->rank_info.king_time;
				fight_player->rank_info.king_time = match_player->rank_info.king_time;
				match_player->rank_info.king_time = king_time;
				{
					MSG_20300039 msg;
					msg.role_id = fight_player->role_id();
					msg.king_time = fight_player->king_time();
					Block_Buffer inner_buf;
					inner_buf.make_message(msg.msg_id);
					msg.serialize(inner_buf);
					inner_buf.finish_message();
					int64_t uuid = fight_player->uuid;
					if(uuid != 0){
						SCENE_MONITOR->send_to_logic(uuid, inner_buf);
					}
				}

				{
					MSG_20300039 msg;
					msg.role_id = match_player->role_id();
					msg.king_time = match_player->king_time();
					Block_Buffer inner_buf;
					inner_buf.make_message(msg.msg_id);
					msg.serialize(inner_buf);
					inner_buf.finish_message();
					int64_t uuid = match_player->uuid;
					if(uuid != 0){
						SCENE_MONITOR->send_to_logic(uuid, inner_buf);
					}
				}
			}
		}
	}
	this->sort_rank();
	return 0;
}

int Inner_Arena_Manager::inner_arena_season_finish(Block_Buffer& buf){
	int64_t uuid = 0;
	buf.read_int64(uuid);
	Arena_Ranking_Vec::iterator it;
	for(it = ranking_vec_.begin(); it != ranking_vec_.end(); ){
		if((*it)->uuid == uuid){
			int seg_key = this->get_arena_seg_key((*it)->group(), (*it)->seg());
			this->arena_seg_del((*it)->role_id(), seg_key, (*it)->fight());
			(*it)->season_reset();
			it = ranking_vec_.erase(it);
		}else{
			++it;
		}
	}
	return 0;
}

int64_t Inner_Arena_Manager::get_rank_player_uuid(role_id_t role_id){
	Inner_Arena_Rank_Player* rank_player = this->find_rank_player(role_id);
	if(rank_player){
		return rank_player->uuid;
	}
	return 0;
}

bool Inner_Arena_Manager::get_load_data_player(const role_id_t match_role_id, std::vector<role_id_t>& rank_role_id){
	Rank_Load_Data_Map::iterator it = rank_load_data_map_.find(match_role_id);
	if(it != rank_load_data_map_.end()){
		rank_role_id = it->second;
		return true;
	}
	return false;
}

void Inner_Arena_Manager::add_load_data_player(const role_id_t match_role_id, const role_id_t rank_role_id){
	Rank_Load_Data_Map::iterator it = rank_load_data_map_.find(match_role_id);
	if(it != rank_load_data_map_.end()){
		rank_load_data_map_[match_role_id].push_back(rank_role_id);
	}else{
		std::vector<role_id_t> vec;
		vec.push_back(rank_role_id);
		rank_load_data_map_[match_role_id] = vec;
	}
}

void Inner_Arena_Manager::del_load_data_player(const role_id_t match_role_id){
	Rank_Load_Data_Map::iterator it = rank_load_data_map_.find(match_role_id);
	if(it != rank_load_data_map_.end()){
		rank_load_data_map_.erase(it);
	}
}

void Inner_Arena_Manager::sync_inner_rank_info(Arena_Ranking_Vec& rank_vec){
	std::vector<Id_Int_Int> int_role_id_vec;
	boost::unordered_map<int64_t, std::vector<Id_Int_Int>> muid_rank_map;
	int count = rank_vec.size();
	for(int i = 0; i < count; ++i){
		int64_t uuid = rank_vec[i]->uuid;
		if(uuid != 0){
			if(muid_rank_map.find(uuid) == muid_rank_map.end()){
				muid_rank_map[uuid] = int_role_id_vec;
			}
			Id_Int_Int ird;
			ird.id = rank_vec[i]->role_id();
			ird.val1 = rank_vec[i]->inner_rank();
			ird.val2 = rank_vec[i]->king_time();
			muid_rank_map[uuid].push_back(ird);
		}
	}
	boost::unordered_map<int64_t, std::vector<Id_Int_Int>>::iterator it;
	for(it = muid_rank_map.begin(); it != muid_rank_map.end(); ++it){
		int count = it->second.size();
		if(count > 0){
			MSG_20300036 msg;
			msg.rank_info = it->second;
			Block_Buffer buf;
			buf.make_message(INNER_ARENA_SYNC_INNER_RANK);
			msg.serialize(buf);
			buf.finish_message();
			SCENE_MONITOR->send_to_logic(it->first, buf);
		}
	}
}


void Inner_Arena_Manager::match_player(Inner_Arena_Rank_Player* fight_player, int highest_force, const int rise, const int group, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	int match_num = 1;
	if(group == ARENA_MAX_GROUP){
		match_num = 3;
		return match_king_group(fight_player, highest_force, match_num, match_players);
	}else{
		if(rise == 1){
			match_num = 1;
			if(group == 1){
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
			int streak_lose = fight_player->streak_lose();
			bool on_top_seg_key = is_on_top_seg_key(fight_player->group(), fight_player->seg());
			// god
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
			Inner_Arena_Rank_Player* dc_rank;
			std::vector<Inner_Arena_Rank_Player*> dc_rank_vec;
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
			std::sort(dc_rank_vec.begin(), dc_rank_vec.end(), Inner_Fight_Greater);
			int dc_count = dc_rank_vec.size();
			for(int i = 0; i < dc_count; ++i){
				if(i == 0){
//					if(difficult_type == 4){
//						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 0));
//					}else if(difficult_type == 0 || difficult_type == 1){
//						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 2));
//					}else{
//						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
//					}
					if(on_top_seg_key){
						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
					}else{
						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 2));
					}
				}else if(i == 1){
//					if(difficult_type == 0 || difficult_type == 2){
//						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
//					}else{
//						match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 0));
//					}
					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 1));
				}else{
					match_players.push_back(Arena_Match_P(dc_rank_vec[i]->role_id(), 0));
				}
			}
		}
	}

}

void Inner_Arena_Manager::match_king_group(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
	if(fight_player == NULL){
		return;
	}
	int8_t difficult_lv = 0;
	int match_count = 0;
	std::vector<int> match_result;
	int count = this->ranking_vec_.size();
	int self_pos = fight_player->inner_rank();
	//排名高一位的玩家
	int high_pos = self_pos -1;
	if(high_pos > 0){
		Inner_Arena_Rank_Player* high_rank = this->get_rank_player_from_rank(high_pos);
		if(high_rank){
			match_players.push_back(Arena_Match_P(high_rank->role_id(), difficult_lv));
			match_count += 1;
		}
	}else{
		high_pos = self_pos;
	}

	for(int i = high_pos -3; i < high_pos; ++i){
		if(i > 0){
			match_result.push_back(i);
		}
	}
	for(int i = self_pos+1; i <= self_pos+5; ++i){
		if(i < count){
			match_result.push_back(i);
		}
	}
	//
	std::random_shuffle(match_result.begin(), match_result.end());
	int rn_count = match_result.size();
	for(int i = 0; i < rn_count; ++i){
		int rk = match_result[i];
		Inner_Arena_Rank_Player* rank_info = this->get_rank_player_from_rank(rk);
		if(rank_info){
			match_players.push_back(Arena_Match_P(rank_info->role_id(), difficult_lv));
			match_count += 1;
		}
		if(match_count >= 3){
			break;
		}
	}
	if(match_count >= 3){
		return;
	}
	//人数不足3个
	int ex_count = 3 - match_count;
	role_id_t role_id_dif_1 = 0;
	role_id_t role_id_dif_2 = 0;
	if(match_players.size() >= 1){
		role_id_dif_1 = match_players[0].role_id;
	}
	if(match_players.size() >= 2){
		role_id_dif_2 = match_players[1].role_id;
	}
	this->match_low_fight(fight_player, highest_force, ex_count, match_players, role_id_dif_1, role_id_dif_2);
}

void Inner_Arena_Manager::match_first_group_rise(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
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

	Inner_Arena_Rank_Player* match_player = get_random_player(fight_player);
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

void Inner_Arena_Manager::match_nor_group_rise(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
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

	Inner_Arena_Rank_Player* match_player = get_random_player(fight_player);
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


void Inner_Arena_Manager::match_nor_group_rise1(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
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

	Inner_Arena_Rank_Player* match_player = get_random_player(fight_player);
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


void Inner_Arena_Manager::match_nor_group_rise2(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players){
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

	Inner_Arena_Rank_Player* match_player = get_random_player(fight_player);
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

void Inner_Arena_Manager::match_low_group_fight(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
		const role_id_t df_role_id_1/* = 0*/, const role_id_t df_role_id_2/* = 0*/){
	if(fight_player == NULL){
		return;
	}
	role_id_t fight_id = fight_player->role_id();
	int ref_force = highest_force*0.8;
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
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
		}
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Inner_Arena_Rank_Player* match_player = get_random_player(fight_player);
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

void Inner_Arena_Manager::match_low_fight(Inner_Arena_Rank_Player* fight_player, int highest_force, const int match_num, std::vector<Arena_Match_P> &match_players,
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
		// - 1
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
		// -2
		{
			int k = ref_force_lv-2;
			if(k >= 1 && k <= CONFIG_CACHE_ARENA->get_arena_max_force_lv()){
				result_set.insert(arena_seg_map_[i].force_set_map[k].begin(),
						arena_seg_map_[i].force_set_map[k].end());
			}
		}
		// -3
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	// 如果人数依然不足,在本段位 扩大到~-1 扩大到-2 扩大到-3低段位内全随机
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
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
		int self_rank = fight_player->inner_rank();
		for(int i = self_rank -5; i <= self_rank+5; ++i){
			if(i == self_rank){
				continue;
			}
			Inner_Arena_Rank_Player* tmp_rank_info = get_rank_player_from_rank(i);
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
			if(rd_id != fight_id && rd_id != df_role_id_1 && rd_id != df_role_id_2 ){
				match_players.push_back(Arena_Match_P(rd_id, 0));
				num += 1;
			}
			result_set.erase(it_rd);
			result_num -= 1;
		}
		return;
	}

	Inner_Arena_Rank_Player* match_player = get_random_player(fight_player);
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

bool Inner_Arena_Manager::match_difficult_hard(Inner_Arena_Rank_Player* fight_player,int highest_force, const role_id_t god_role_id, role_id_t& rt_match_id){
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

bool Inner_Arena_Manager::match_difficult_god(Inner_Arena_Rank_Player* fight_player,int highest_force, role_id_t& rt_match_id){
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


int Inner_Arena_Manager::get_arena_seg_key(int group, int seg){
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
void Inner_Arena_Manager::arena_seg_add(role_id_t role_id, int seg_key, int force){
	int force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(force);
	arena_seg_map_[seg_key].add_player(role_id, force_lv);
}

void Inner_Arena_Manager::arena_seg_del(role_id_t role_id, int seg_key, int force){
	int force_lv = CONFIG_CACHE_ARENA->get_arena_force_level(force);
	arena_seg_map_[seg_key].del_player(role_id, force_lv);
}

void Inner_Arena_Manager::arena_seg_change(role_id_t role_id, int old_seg_key, int old_force, int new_seg_key, int new_force){
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

bool Inner_Arena_Manager::is_on_top_seg_key(const int group, const int seg){
	if(ranking_vec_.empty()){
		return true;
	}
	if(group == ranking_vec_[0]->group() && seg == ranking_vec_[0]->seg()){
		return true;
	}
	return false;
}

///////////////////
//
//
///////////////////

Inner_Arena_Area_Manger::Inner_Arena_Area_Manger(void){

}

Inner_Arena_Area_Manger::~Inner_Arena_Area_Manger(void){

}

int Inner_Arena_Area_Manger::inner_arena_sync_rank_info(Block_Buffer& buf){
	int inner_area = 0;
	buf.read_int32(inner_area);
	Inner_Arena_Manager* inner_manager = get_inner_arena_manager(inner_area);
	if(inner_manager){
		return inner_manager->inner_arena_sync_rank_info(buf);
	}
	return 0;
}

int Inner_Arena_Area_Manger::inner_arena_match_player(Block_Buffer& buf){
	int inner_area = 0;
	buf.read_int32(inner_area);
	Inner_Arena_Manager* inner_manager = get_inner_arena_manager(inner_area);
	if(inner_manager){
		return inner_manager->inner_arena_match_player(buf);
	}
	return 0;
}

int Inner_Arena_Area_Manger::inner_arena_fetch_fight_data(Block_Buffer& buf){
	int inner_area = 0;
	buf.read_int32(inner_area);
	Inner_Arena_Manager* inner_manager = get_inner_arena_manager(inner_area);
	if(inner_manager){
		return inner_manager->inner_arena_fetch_fight_data(buf);
	}
	return 0;
}

int Inner_Arena_Area_Manger::inner_arena_fetch_rank_info(Block_Buffer& buf){
	int inner_area = 0;
	buf.read_int32(inner_area);
	Inner_Arena_Manager* inner_manager = get_inner_arena_manager(inner_area);
	if(inner_manager){
		return inner_manager->inner_arena_fetch_rank_info(buf);
	}
	return 0;
}

int Inner_Arena_Area_Manger::inner_arena_battle_result(Block_Buffer& buf){
	int inner_area = 0;
	buf.read_int32(inner_area);
	Inner_Arena_Manager* inner_manager = get_inner_arena_manager(inner_area);
	if(inner_manager){
		return inner_manager->inner_arena_battle_result(buf);
	}
	return 0;
}

int Inner_Arena_Area_Manger::inner_arena_season_finish(Block_Buffer& buf){
	int inner_area = 0;
	buf.read_int32(inner_area);
	Inner_Arena_Manager* inner_manager = get_inner_arena_manager(inner_area);
	if(inner_manager){
		return inner_manager->inner_arena_season_finish(buf);
	}
	return 0;
}

Inner_Arena_Manager* Inner_Arena_Area_Manger::get_inner_arena_manager(int inner_area){
	Inner_Arena_Manager_Map::iterator it = inner_arena_manager_map_.find(inner_area);
	if(it != inner_arena_manager_map_.end()){
		return it->second;
	}else{
		Inner_Arena_Manager* inner_manager = POOL_MANAGER->pop_inner_arena_manager();
		if(inner_manager){
			inner_arena_manager_map_[inner_area] = inner_manager;
			return inner_manager;
		}
	}
	return NULL;
}
