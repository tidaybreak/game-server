/*
 * Honor_Arena_Manager.cpp
 *
 *  Created on: 2015年3月16日
 *      Author: root
 */

#include "Honor_Arena_Manager.h"
#include "Config_Cache_Honor_Arena.h"
#include "honor_arena_scene/Honor_Arena_Scene.h"
#include "NPC/hero/Hero.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Scene_Public.h"
#include "Global_Scene_Manager.h"
#include "scene_battle/Battle_Scene.h"
#include "Msg_Honor_Arena_Struct.h"

const int MAX_RANK_SHOW_NUM = 150;


Honor_Arena_Manager::Honor_Arena_Manager(void){
	reset();
}

Honor_Arena_Manager::~Honor_Arena_Manager(void){

}

void Honor_Arena_Manager::reset(void){
	icon_id_ = 16;
	act_state_ = 0;
	act_fight_time_ = 0;
	act_end_time_ = Time_Value::zero;

	player_info_map_.clear();
	rank_info_.clear();
	match_rank_info_.clear();
	fight_teams_.clear();

	match_tick_num_ = 0;
	check_rank_change_tick_ = 0;
}

void Honor_Arena_Manager::set_icon_id(const int8_t icon_id){
	icon_id_ = icon_id;
}

int Honor_Arena_Manager::tick(const Time_Value &now){
	check_fight_teams(now);

	match_tick_num_ += 1;
	check_rank_change_tick_ += 1;
	if(match_tick_num_ > 30){// 3秒匹配一次
		match_tick_num_ = 0;
		make_match();
	}
	if(check_rank_change_tick_ > 40){
		check_rank_change_tick_ = 0;
		check_rank_change();
	}
	return 0;
}

int Honor_Arena_Manager::inner_player_enter(const role_id_t gang_id, Block_Buffer& buf){
	role_id_t role_id = 0;
	std::string role_name = "";
	int vip_lv = 0;
	int career = 0;
	int force = 0;
	std::string gang_name;
	buf.read_int64(role_id);
	buf.read_string(role_name);
	buf.read_int32(vip_lv);
	buf.read_int32(career);
	buf.read_int32(force);
	buf.read_string(gang_name);
	bool is_new = false;
	Honor_Arena_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it == player_info_map_.end()){
		Honor_Arena_Player_Info player_info;
		player_info.rank_info.role_id = role_id;
		player_info.rank_info.score = CONFIG_CACHE_HONOR_ARENA->get_base_score();
		player_info.streak_win_state = CONFIG_CACHE_HONOR_ARENA->get_streak_show();
		std::pair<Honor_Arena_Player_Info_Map::iterator, bool> result_pair = player_info_map_.insert(std::make_pair(role_id, player_info));
		if(result_pair.second == true){
			rank_info_.push_back(&(result_pair.first->second));
		}
		is_new = true;
	}
	it = player_info_map_.find(role_id);
	if(it != player_info_map_.end()){
		it->second.rank_info.role_name = role_name;
		it->second.rank_info.guild_name = gang_name;
		//it->second.rank_info.career = career;
		it->second.rank_info.force = force;
		it->second.state = 0;								// idle
	}
	if(is_new){
		refresh_rank();
	}
	return 0;
}

int Honor_Arena_Manager::inner_sync_info(Scene_Player* player, Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 99:{
			role_id_t role_id = 0;
			std::string role_name = "";
			uint8_t gender = 0;
			buf.read_int64(role_id);
			buf.read_string(role_name);
			buf.read_uint8(gender);
			Honor_Arena_Player_Info_Map::iterator it = player_info_map_.find(role_id);
			if(it != player_info_map_.end()){
				it->second.rank_info.role_name = role_name;
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Honor_Arena_Manager::inner_act_start(const role_id_t gang_id, Block_Buffer& buf){
	if(act_state_ != ACT_STATE_START){//
		act_state_ = ACT_STATE_START;
		int icon_id = icon_id_;
		reset();
		icon_id_ = icon_id;
	}
	int remain_time = 0;
	int fight_time = 0;
	buf.read_int32(fight_time);
	buf.read_int32(remain_time);
	act_fight_time_ = fight_time;
	act_end_time_ = Time_Value::gettimeofday() + Time_Value(remain_time);
	return 0;
}

int Honor_Arena_Manager::inner_act_end(const role_id_t gang_id, Block_Buffer& buf){
	if(act_state_ != ACT_STATE_START){//
		return 0;
	}
	int remain_time = 0;
	buf.read_int32(remain_time);
	act_end_time_ = Time_Value::gettimeofday() + Time_Value(remain_time);
	return 0;
}

int Honor_Arena_Manager::req_match(Scene_Player* player){
	if(!player){
		return 0;
	}
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return -1;
	}
	if(player_info->state != 0){
		return -1;
	}
	player_info->state = 1;
	player_info->match_time = Time_Value::gettimeofday().sec();
	return 0;
}
int Honor_Arena_Manager::req_receive(Scene_Player* player, const int type){
	if(!player){
		return 0;
	}
	if(player->move_scene() == NULL){
		return 0;
	}
	Honor_Arena_Scene* scene = dynamic_cast<Honor_Arena_Scene*>(player->move_scene());
	if(!scene){
		return 0;
	}
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return -1;
	}
	if(type == HONOR_ARENA_FIRST_WIN){// 首胜
		if(player_info->first_win == 0){// 未达成
			return -1;
		}else if(player_info->first_win == 2){
			return -1;
		}
		player_info->first_win = 2;
	}else if(type == HONOR_ARENA_TEN_FIGHT){// 十战
		if(player_info->ten_fight == 0){// 未达成
			return -1;
		}else if(player_info->ten_fight == 2){// 已领取
			return -1;
		}
		player_info->ten_fight = 2;
	}else{// 连胜
		int streak_index = -1;
		int count = player_info->streak_win_state.size();
		for(int i = 0; i < count; ++i){
			if(player_info->streak_win_state[i].val_1 == type){
				streak_index = i;
				break;
			}
		}
		if(streak_index == -1){
			return -1;
		}
		if(player_info->streak_win_state[streak_index].val_2 == 0){// 未达成
			return -1;
		}else if(player_info->streak_win_state[streak_index].val_2 == 2){// 已领取
			return -1;
		}
		player_info->streak_win_state[streak_index].val_2 = 2;
	}
	scene->add_show_reward(player->role_id(), type);
	MSG_50211202 msg;
	msg.type = type;
	msg.result = 1;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

int Honor_Arena_Manager::req_rank(Scene_Player* player){
	if(!player){
		return 0;
	}
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return 0;
	}
	MSG_50211205 msg;
	msg.self_rank = player_info->rank_info.rank;
	int count = rank_info_.size();
	for(int i = 0; i < count; ++i){
		if(i >= MAX_RANK_SHOW_NUM){
			break;
		}
		msg.rank_infos.push_back(rank_info_[i]->rank_info);
	}
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

int Honor_Arena_Manager::req_panel_info(Scene_Player* player){
	if(!player){
		return 0;
	}
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return 0;
	}
	player_info->rank_change = false;
	MSG_50211207 msg;
	Int_Int show_state;
	show_state.val_1 = HONOR_ARENA_FIRST_WIN;
	show_state.val_2 = player_info->first_win;
	msg.achievements.push_back(show_state);

	show_state.val_1 = HONOR_ARENA_TEN_FIGHT;
	show_state.val_2 = player_info->ten_fight;
	msg.achievements.push_back(show_state);

	int count = player_info->streak_win_state.size();
	for(int i = 0; i < count; ++i){
		show_state.val_1 = player_info->streak_win_state[i].val_1;
		show_state.val_2 = player_info->streak_win_state[i].val_2;
		msg.achievements.push_back(show_state);
	}

	msg.items = player_info->gain_items;
	msg.score = player_info->rank_info.score;
	msg.fight_num = player_info->fight_num;
	msg.win_num = player_info->win_num;
	msg.auto_match = player_info->auto_match;
	OBJ_SEND_TO_CLIENT(msg, (*player));

	{
		MSG_50211206 near_msg;// 显示前2后3
		int rank_num = rank_info_.size();
		int rank_begin = 0;
		int rank_end = 0;
		int last_num = rank_num - player_info->rank_info.rank;
		if(last_num < 3){
			rank_begin = player_info->rank_info.rank - 2 - (3 - last_num);
		}else{
			rank_begin = player_info->rank_info.rank - 2;
		}
		if(rank_begin < 1){
			rank_begin = 1;
		}
		rank_end = rank_begin+5;
		for(int i = 0; i < rank_num; ++i){
			if(i < 3){
				near_msg.top_info.push_back(rank_info_[i]->role_info);
			}
			if(i >= (rank_begin - 1) && i <= (rank_end - 1)){
				near_msg.near_rank.push_back(rank_info_[i]->rank_info);
			}
		}

		OBJ_SEND_TO_CLIENT(near_msg, (*player));
	}

	return 0;
}

int Honor_Arena_Manager::req_set_auto_match(Scene_Player* player, const int8_t is_auto){
	if(!player){
		return 0;
	}
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return 0;
	}
	player_info->auto_match = is_auto;
	MSG_50211208 msg;
	msg.is_auto = is_auto;
	msg.result = 1;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

Honor_Arena_Player_Info* Honor_Arena_Manager::get_player_info(const role_id_t role_id){
	Honor_Arena_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it != player_info_map_.end()){
		return &(it->second);
	}
	return NULL;
}

Honor_Arena_Player_Info* Honor_Arena_Manager::get_player_info_from_rank(const int rank){
	int count = rank_info_.size();
	if(rank <= 0 || rank > count){
		return NULL;
	}
	return rank_info_[rank-1];
}

bool Honor_Arena_Manager::find_player(const role_id_t role_id){
	Honor_Arena_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it != player_info_map_.end()){
		return true;
	}
	return false;
}

void Honor_Arena_Manager::refresh_rank(void){
	std::sort(rank_info_.begin(), rank_info_.end(), Honor_Arena_Rank_Greater);
	int count = rank_info_.size();
	for(int i = 0; i < count; ++i){
		int src_rank = rank_info_[i]->rank_info.rank;
		rank_info_[i]->rank_info.rank = i + 1;
		if(src_rank != rank_info_[i]->rank_info.rank){
			rank_info_[i]->rank_change = true;
		}
	}
}

void Honor_Arena_Manager::player_enter_scene(Scene_Player* player){
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return;
	}
	player_info->role_info.base_info.role_id = player->base_detail().role_id;
	player_info->role_info.base_info.role_name = player->base_detail().role_name;
	player_info->role_info.base_info.head_photo_id = player->base_detail().head_id;
	player_info->role_info.base_info.gender = player->base_detail().gender;
	player_info->role_info.base_info.guild_name = player->base_detail().gang_name;
	player_info->role_info.base_info.career = player->base_detail().career;
	player_info->role_info.base_info.level = player->base_detail().level;
	player_info->role_info.base_info.vip_level = player->base_detail().vip;
	player_info->role_info.base_info.combat = player_info->rank_info.force;
	player_info->role_info.base_info.title_id = player->base_detail().title_id;

	player_info->role_info.avatar = player->base_detail().avatar_vec;
	player_info->state = 0;
}

void Honor_Arena_Manager::player_exit_scene(Scene_Player* player){
	if(!player){
		return;
	}
	Honor_Arena_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return;
	}
	if(player_info->state == 2){//wait fight
		std::vector<Honor_Arena_Match_Team>::iterator it;
		for(it = fight_teams_.begin(); it != fight_teams_.end();++it){
			if(it->role_id_1 == player->role_id()){
				{
					MSG_50211201 msg;
					msg.result = 0;
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
				Honor_Arena_Player_Info* match_info = get_player_info(it->role_id_2);
				if(match_info){
					match_info->state = 0;
					Mover* match_mover = player->find_mover_in_move_scene(it->role_id_2);
					if(match_mover){
						MSG_50211201 msg;
						msg.result = 0;
						OBJ_SEND_TO_CLIENT(msg, (*match_mover));
					}
				}
				fight_teams_.erase(it);
				break;
			}
			if(it->role_id_2 == player->role_id()){
				{
					MSG_50211201 msg;
					msg.result = 0;
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
				Honor_Arena_Player_Info* match_info = get_player_info(it->role_id_1);
				if(match_info){
					match_info->state = 0;
					Mover* match_mover = player->find_mover_in_move_scene(it->role_id_1);
					if(match_mover){
						MSG_50211201 msg;
						msg.result = 0;
						OBJ_SEND_TO_CLIENT(msg, (*match_mover));
					}
				}
				fight_teams_.erase(it);
				break;
			}
		}
	}
	player_info->state = -1;		// out state
}

void Honor_Arena_Manager::make_match(void){
	int now_sec = Time_Value::gettimeofday().sec();
	int fight_sec = now_sec + 3;
	match_rank_info_.clear();
	int wait_index = 0;
	std::vector<Honor_Arena_Player_Info*> wait_match_info;
	boost::unordered_map<role_id_t, int> wait_index_map;

	for(std::vector<Honor_Arena_Player_Info*>::iterator it = rank_info_.begin();
			it != rank_info_.end(); ++it){
		if((*it)->state == 1){
			(*it)->match_lv = (now_sec - (*it)->match_time)/10;
			match_rank_info_.push_back(*it);
			wait_match_info.push_back(*it);
			wait_index_map[(*it)->rank_info.role_id] = wait_index;
			wait_index += 1;
		}
	}
	int wait_count = wait_match_info.size();
	if(wait_count <= 1){
		return;
	}
	std::sort(match_rank_info_.begin(), match_rank_info_.end(), Honor_Arena_Match_Greater);
	for(std::vector<Honor_Arena_Player_Info*>::iterator it = match_rank_info_.begin();
			it != match_rank_info_.end(); ++it){
		Honor_Arena_Player_Info* match_info = *it;
		Scene_Player* match_player = SCENE_MONITOR->find_scene_player_by_role_id(match_info->rank_info.role_id);
		if(!match_player){
			continue;
		}
		if(match_info->state != 1){
			continue;
		}
		int score_spread = CONFIG_CACHE_HONOR_ARENA->get_match_score_spread(match_info->match_lv);
		int score_min = match_info->rank_info.score - score_spread;
		if(score_min < 0){
			score_min = 0;
		}
		int score_max = match_info->rank_info.score + score_spread;
		int index = wait_index_map[match_info->rank_info.role_id];
		if(index < 0 || index >= wait_count){
			continue;
		}
		bool is_success = false;
		// 匹配积分低的
		for(int i = index + 1; i < wait_count; ++i){
			if(wait_match_info[i]->rank_info.score < score_min){
				break;
			}
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(wait_match_info[i]->rank_info.role_id);
			if(!player){
				continue;
			}
			if(wait_match_info[i]->state != 1){
				continue;
			}
			// 匹配成功
			wait_match_info[i]->state = 2;
			match_info->state = 2;
			Honor_Arena_Match_Team team;
			team.fight_time = fight_sec;
			team.role_id_1 = wait_match_info[i]->rank_info.role_id;
			team.role_id_2 = match_info->rank_info.role_id;
			fight_teams_.push_back(team);
			{
				MSG_50211201 msg;
				msg.result = 1;
				msg.role_info = wait_match_info[i]->role_info.base_info;
				Hero *hero = SCENE_PUBLIC->find_fight_hero(wait_match_info[i]->rank_info.role_id);
				if(hero){
					msg.hero_id = hero->hero_base_detail().hero_id;
				}
				OBJ_SEND_TO_CLIENT(msg, (*match_player));
			}
			{
				MSG_50211201 msg;
				msg.result = 1;
				msg.role_info = match_info->role_info.base_info;
				Hero *hero = SCENE_PUBLIC->find_fight_hero(match_info->rank_info.role_id);
				if(hero){
					msg.hero_id = hero->hero_base_detail().hero_id;
				}
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}
			is_success = true;
			break;
		}
		if(is_success){
			continue;
		}
		// 匹配积分高的
		for(int i = index - 1; i >= 0; --i){
			if(wait_match_info[i]->rank_info.score > score_max){
				break;
			}
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(wait_match_info[i]->rank_info.role_id);
			if(!player){
				continue;
			}
			if(wait_match_info[i]->state != 1){
				continue;
			}
			// 匹配成功
			wait_match_info[i]->state = 2;
			match_info->state = 2;
			Honor_Arena_Match_Team team;
			team.fight_time = fight_sec;
			team.role_id_1 = wait_match_info[i]->rank_info.role_id;
			team.role_id_2 = match_info->rank_info.role_id;
			fight_teams_.push_back(team);
			{
				MSG_50211201 msg;
				msg.result = 1;
				msg.role_info = wait_match_info[i]->role_info.base_info;
				Hero *hero = SCENE_PUBLIC->find_fight_hero(wait_match_info[i]->rank_info.role_id);
				if(hero){
					msg.hero_id = hero->hero_base_detail().hero_id;
				}
				OBJ_SEND_TO_CLIENT(msg, (*match_player));
			}
			{
				MSG_50211201 msg;
				msg.result = 1;
				msg.role_info = match_info->role_info.base_info;
				Hero *hero = SCENE_PUBLIC->find_fight_hero(match_info->rank_info.role_id);
				if(hero){
					msg.hero_id = hero->hero_base_detail().hero_id;
				}
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}
			is_success = true;
			break;
		}
	}
}

void Honor_Arena_Manager::check_rank_change(void){
	Honor_Arena_Player_Info_Map::iterator it;
	for(it = player_info_map_.begin(); it != player_info_map_.end(); ++it){
		if(it->second.rank_change == false){
			continue;
		}
		if(it->second.state == 0 || it->second.state == 1){
			send_rank_change(&(it->second));
		}
	}
}

void Honor_Arena_Manager::check_fight_teams(const Time_Value& now){
	int now_sec = now.sec();
	std::vector<Honor_Arena_Match_Team>::iterator it;
	for(it = fight_teams_.begin(); it != fight_teams_.end();){
		if(now_sec > it->fight_time){
			if(start_fight(it->role_id_1, it->role_id_2) == 0){

			}else{

			}
			it = fight_teams_.erase(it);
		}else{
			++it;
		}
	}
}

int Honor_Arena_Manager::start_fight(const role_id_t role_id_1, const role_id_t role_id_2){
	Scene_Player* player1 = SCENE_MONITOR->find_scene_player_by_role_id(role_id_1);
	Scene_Player* player2 = SCENE_MONITOR->find_scene_player_by_role_id(role_id_2);
	if(!player1 || !player2){
		return -1;
	}
	Honor_Arena_Scene* scene1 = dynamic_cast<Honor_Arena_Scene*>(player1->move_scene());
	Honor_Arena_Scene* scene2 = dynamic_cast<Honor_Arena_Scene*>(player2->move_scene());
	if(!scene1 || !scene2){
		return -1;
	}
	if(player1->is_in_battle() || player2->is_in_battle()){
		return -1;
	}
	Battle_Scene *battle_scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player1->move_scene());
	if(!battle_scene){
		return -1;
	}
	battle_scene->set_battle_type(Battle_Scene::BATTLE_TYPE_HONOR_ARENA);
	Position_Key player_pos_0(0, Position_Key::LINE_TWO_FRONT);
	if(player1->set_battle_position(player_pos_0.pos, player_pos_0.idx, battle_scene) == 0){
		player1->enter_battle_scene(battle_scene);
	}

	Position_Key player_pos_1(1, Position_Key::LINE_TWO_FRONT);
	if(player2->set_battle_position(player_pos_1.pos, player_pos_1.idx, battle_scene) == 0){
		player2->player_self()->enter_battle_scene(battle_scene);
	}
	return 0;
}

void Honor_Arena_Manager::send_rank_change(Honor_Arena_Player_Info* player_info){
	if(!player_info){
		return;
	}
	Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(player_info->rank_info.role_id);
	if(!player){
		return;
	}
	MSG_50211206 near_msg;// 显示前2后3
	int rank_num = rank_info_.size();
	int rank_begin = 0;
	int rank_end = 0;
	int last_num = rank_num - player_info->rank_info.rank;
	if(last_num < 3){
		rank_begin = player_info->rank_info.rank - 2 - (3 - last_num);
	}else{
		rank_begin = player_info->rank_info.rank - 2;
	}
	if(rank_begin < 1){
		rank_begin = 1;
	}
	rank_end = rank_begin+5;
	for(int i = 0; i < rank_num; ++i){
		if(i < 3){
			near_msg.top_info.push_back(rank_info_[i]->role_info);
		}
		if(i >= (rank_begin - 1) && i <= (rank_end - 1)){
			near_msg.near_rank.push_back(rank_info_[i]->rank_info);
		}
	}
	OBJ_SEND_TO_CLIENT(near_msg, (*player));
}

