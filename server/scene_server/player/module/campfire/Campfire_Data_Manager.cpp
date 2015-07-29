/*
 * Campfire_Data_Manager.cpp
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */


#include "Campfire_Data_Manager.h"
#include "campfire/Campfire_Struct.h"
#include "campfire/Scene_Campfire_Struct.h"
#include "campfire_scene/Campfire_Scene_Manager.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"

Campfire_Data_Manager::Campfire_Data_Manager(void) : max_rank_(0){

}

Campfire_Data_Manager::~Campfire_Data_Manager(void){

}

void Campfire_Data_Manager::load_campfire(Block_Buffer& buf){
	campfire_info_.clear();
	int count = 0;
	buf.read_int32(count);
	for(int i = 0; i < count; ++i){
		Campfire_Detail detail;
		detail.deserialize(buf);
		campfire_info_.insert(std::make_pair(detail.gang_id, detail));
	}
}

int Campfire_Data_Manager::cmd_campfire_activate(Block_Buffer& buf){
//	player_campfire_map_.clear();
	max_rank_ = 0;
	int remain_time = 0;
	buf.read_int32(remain_time);
	load_campfire(buf);
	campfire_over_time_ = Time_Value::gettimeofday() + Time_Value(remain_time);
	players_came_in.clear();
	return 0;
}

int Campfire_Data_Manager::cmd_campfire_finish(void){
	Player_Campfire_Map::iterator iter;
	for (iter = player_campfire_map_.begin(); iter != player_campfire_map_.end(); ++iter) {
		iter->second.rank_rev_stat = 1;
		iter->second.killed_match_man.clear();
		iter->second.add_firewood = 0;
		save_campfire_player_info(iter->second.role_id);
	}

	//给参加过的人发结算通知
	boost::unordered_set<role_id_t>::const_iterator iter1;
	for (iter1 = players_came_in.begin(); iter1 != players_came_in.end(); ++iter1) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*iter1);
			if (player)
				player->send_campfire_spot_info();
	}

	campfire_info_.clear();
	campfire_over_time_ = Time_Value::zero;
	players_came_in.clear();

	return 0;
}

void Campfire_Data_Manager::max_rank_plus_plus() {
	++max_rank_;
}

uint32_t Campfire_Data_Manager::max_rank() {
	return max_rank_;
}

int Campfire_Data_Manager::get_campfire_remain_time(void){
	Time_Value now = Time_Value::gettimeofday();
	int remain_time = campfire_over_time_.sec() - now.sec();
	if(remain_time < 0){
		remain_time = 0;
	}
	return remain_time;
}

Campfire_Detail* Campfire_Data_Manager::get_campfire_detail_from_point(const int point_id){
	Campfire_Detail_Map::iterator it;
	for(it = campfire_info_.begin(); it != campfire_info_.end(); ++it){
		if(it->second.id == point_id){
			return &(it->second);
		}
	}
	return NULL;
}

Campfire_Detail* Campfire_Data_Manager::get_campfire_detail_from_gang(const int64_t gang_id){
	Campfire_Detail_Map::iterator it = campfire_info_.find(gang_id);
	if(it != campfire_info_.end()){
		return &(it->second);
	}
	return NULL;
}

int Campfire_Data_Manager::add_campfire_detail(Campfire_Detail& detail){
	campfire_info_.insert(std::make_pair(detail.gang_id, detail));
	return 0;
}

Player_Campfire_Map Campfire_Data_Manager::get_campfire_finished_player(void) {
	Player_Campfire_Map temp;
	for (Player_Campfire_Map::const_iterator iter =
			player_campfire_map_.begin(); iter != player_campfire_map_.end();
			++iter) {
		if (iter->second.add_firewood != 0) // 0代表未完成
			temp.insert(*iter);
	}
	return temp;
}

Scene_Campfire_Info* Campfire_Data_Manager::get_player_campfire_info(role_id_t role_id){
	Player_Campfire_Map::iterator it = player_campfire_map_.find(role_id);
	if(it != player_campfire_map_.end()){
		return &(it->second);
	}
	return NULL;
}

int Campfire_Data_Manager::save_campfire_player_info(role_id_t player_id) {
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(player_id);
	if (!player)
		return 0;
	return player->save_campfire_player_info();
}

void Campfire_Data_Manager::add_player(const Scene_Campfire_Info& info) {
	player_campfire_map_[info.role_id] = info;
}

void Campfire_Data_Manager::check_if_came_in(Scene_Player* player) {
	if (!player)
		return;
	if (players_came_in.find(player->role_id()) == players_came_in.end()) {
		player->clear_campfire_award_record();
		players_came_in.insert(player->role_id());
	}
}

void Campfire_Data_Manager::max_rank_minus_one() {
	--max_rank_;
}
