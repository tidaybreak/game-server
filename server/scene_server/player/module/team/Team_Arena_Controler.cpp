/*
 * Team_Arena_Controler.cpp
 *
 *  Created on: 2014年6月18日
 *      Author: Linqiyou
 */

#include "Msg_Active_Struct.h"
#include "Scene_Team_Manager.h"
#include "team/Team_Func.h"
#include "Team_Arena_Controler.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"
#include "Pool_Manager.h"
#include "Global_Scene_Manager.h"
#include "Battle_Scene.h"
#include "Config_Cache_Team.h"
#include "Battle_Scene_Struct.h"

Team_Arena_Controler::Team_Arena_Controler(void) {
	already_match_time_ = 1;
	team_map_.clear();
//	team_set_.clear();
	team_deque_.clear();
	team_matched_.clear();
	level_team_ids_map_.clear();
	team_amount_ = 0;
	last_match_time_ = Time_Value::zero;
	sports_activity_switch_ = TEAM_SPORTS_ACTIVITY_CLOSE;
	team_sports_test_ = false;
}

Team_Arena_Controler::~Team_Arena_Controler(void) {
}

void Team_Arena_Controler::tick(Time_Value &now) {
	open_tick();
	if (TEAM_SPORTS_ACTIVITY_OPEN != this->sports_activity_switch_) {
		return ;
	}

	if (now - last_match_time_ > Time_Value(MATCH_GAP)) {
		match_arena_team();
		last_match_time_ = now;
	}

	if (now - last_rank_refresh_time_ > Time_Value(30)) {
		refresh_rank_list();
		last_rank_refresh_time_ = now;
	}


	while (team_deque_.size() > 0) {
		Matched_Teams &matched_teams = team_deque_.front();
		if (now - matched_teams.match_time > Time_Value(ENTER_WAIT)) {
			Map_Team *team1 = SCENE_TEAM_MANAGER->get_team(matched_teams.matched_pair.first);
			Map_Team *team2 = SCENE_TEAM_MANAGER->get_team(matched_teams.matched_pair.second);
			if (team1 && team2) {
				transfer_handle(*team1, *team2);
			} else if (team1) {
			} else if (team2) {
			}
			team_deque_.pop_front();
			team_matched_.erase(matched_teams.matched_pair.first);
			team_matched_.erase(matched_teams.matched_pair.second);
		} else {
			break;
		}
	}
}

int Team_Arena_Controler::open_tick(void) {
	Time_Value now = Time_Value::gettimeofday();
	//TEAM_SPORTS_SWITCH_CLOSE
	Date_Time dt(now);
	if (((dt.hour() >= 12 && dt.hour() < 14)
			|| (dt.hour() >= 18 && dt.hour() <= 19))) {
		if (((dt.hour() == 12 || dt.hour() == 18) && dt.minute() < 30)
				|| ((dt.hour() == 13 || dt.hour() == 19) && dt.minute() >= 30)) {
//		if ((dt.hour() == 12 && dt.minute() < 30)
//				|| (dt.hour() == 19 && dt.minute() >= 30)) {
			close_sports_activity();
		} else {
			open_sports_activity();
		}
	} else {
		close_sports_activity();
	}
	return 0;
}

int Team_Arena_Controler::transfer_handle(Map_Team &team1, Map_Team &team2) {
	typedef std::vector<Scene_Player*> SPlayer_Vec;
	Role_Hash_Set role_ids_1, role_ids_2;
	SPlayer_Vec vec_1, vec_2;
	team1.team_all_member_set(role_ids_1);
	team2.team_all_member_set(role_ids_2);
	for (Role_Hash_Set::iterator it = role_ids_1.begin(); it != role_ids_1.end(); ++it) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
		if (player) {
			vec_1.push_back(player);
		}
	}
	for (Role_Hash_Set::iterator it = role_ids_2.begin(); it != role_ids_2.end(); ++it) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
		if (player) {
			vec_2.push_back(player);
		}
	}

	if (!vec_1.empty() && !vec_2.empty()) {
		Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(NULL);
		scene->set_battle_type(Battle_Scene::BATTLE_TYPE_TEAM_ARENA);
		Other_Info ot;
		ot.type = OT_TEAM_ARENA;
		ot.int_v1 = team1.get_team_score();
		ot.int_v2 = team2.get_team_score();
		scene->set_other_info(ot);

		Int_Hash_Set idx_set;
		int idx = Position_Key::LINE_ONE_FRONT;
		for (SPlayer_Vec::iterator it = vec_1.begin(); it != vec_1.end(); ++it) {
			if ((**it).battle_scene()) {
				MSG_USER("Team::role(%ld) in_battle", (**it).role_id());
				continue;
			}
			idx = team1.get_position((**it).role_id());
			int y = get_other_idx(idx);
			if (0 == y) {
				idx = Position_Key::LINE_ONE_FRONT;
				y = Position_Key::LINE_ONE_BEHIND;
			}
			if (0 != idx_set.count(idx) || 0 != idx_set.count(y)) {
				if (0 == idx_set.count(Position_Key::LINE_ONE_FRONT) && 0 == idx_set.count(Position_Key::LINE_ONE_BEHIND)) {
					idx = Position_Key::LINE_ONE_FRONT;
				} else if (0 == idx_set.count(Position_Key::LINE_TWO_FRONT) && 0 == idx_set.count(Position_Key::LINE_TWO_BEHIND)) {
					idx = Position_Key::LINE_TWO_FRONT;
				} else if (0 == idx_set.count(Position_Key::LINE_THREE_FRONT) && 0 == idx_set.count(Position_Key::LINE_THREE_BEHIND)) {
					idx = Position_Key::LINE_THREE_FRONT;
				} else {
					continue;
				}
			}
			if((**it).set_battle_position(0, idx, scene) == 0){
				(**it).enter_battle_scene(scene);
				idx_set.insert(idx);
			}
		}

		idx_set.clear();
		for (SPlayer_Vec::iterator it = vec_2.begin(); it != vec_2.end(); ++it) {
			if ((**it).battle_scene()) {
				MSG_USER("Team::role(%ld) in_battle", (**it).role_id());
				continue;
			}
			idx = team2.get_position((**it).role_id());
			int y = get_other_idx(idx);
			if (0 == y) {
				idx = Position_Key::LINE_ONE_FRONT;
				y = Position_Key::LINE_ONE_BEHIND;
			}
			if (0 != idx_set.count(idx) || 0 != idx_set.count(y)) {
				if (0 == idx_set.count(Position_Key::LINE_ONE_FRONT)
						&& 0 == idx_set.count(Position_Key::LINE_ONE_BEHIND)
						&& 0 == idx_set.count(Position_Key::LINE_ONE_FIRST)) {
					idx = Position_Key::LINE_ONE_BEHIND;
				} else if (0 == idx_set.count(Position_Key::LINE_TWO_FRONT)
						&& 0 == idx_set.count(Position_Key::LINE_TWO_BEHIND)
						&& 0 == idx_set.count(Position_Key::LINE_TWO_FIRST)) {
					idx = Position_Key::LINE_TWO_BEHIND;
				} else if (0 == idx_set.count(Position_Key::LINE_THREE_FRONT)
						&& 0 == idx_set.count(Position_Key::LINE_THREE_BEHIND)
						&& 0 == idx_set.count(Position_Key::LINE_THREE_FIRST)) {
					idx = Position_Key::LINE_THREE_BEHIND;
				} else {
					continue;
				}
			}
			if((**it).set_battle_position(1, idx, scene) == 0){
				(**it).enter_battle_scene(scene);
				idx_set.insert(idx);
			}
		}
	} else if (vec_1.empty() && vec_2.empty()) {
//
	} else if (vec_1.empty()) {
		++team1.fail_times();
	} else if (vec_2.empty()) {
		++team2.fail_times();
	}
	return 0;
}

int Team_Arena_Controler::add_wait_match_team(int64_t team_id) {
	Map_Team *team_old = SCENE_TEAM_MANAGER->get_team(team_id);
	if (NULL == team_old) {
		MSG_DEBUG("not found: %ld", team_id);
		return -1;
	}

	if (Map_Team::TEAM_SPORTS_NORMAL == team_old->sports_status()) {
		if (team_map_.count(team_id)) {
			MSG_DEBUG("team_map_: %ld", team_id);
			return 0;
		}
		if (team_matched_.count(team_id)) {
			MSG_DEBUG("team_matched_: %ld", team_id);
			return 0;
		}
		Map_Team *team = POOL_MANAGER->pop_map_team_pool();
		if (!team) {
			MSG_DEBUG("pop_map_team_pool: %ld", team_id);
			return -1;
		}
		*team = *team_old;
//		this->team_set_.insert(team);
		this->team_map_.insert(std::make_pair(team_id, team));
	} else {
		MSG_DEBUG("sports_status: %d", (int)team_old->sports_status());
	}
	return 0;
}

int Team_Arena_Controler::del_wait_match_team(int64_t team_id) {
	Int64_Map_Team_Map::iterator it = this->team_map_.find(team_id);
	if (it != this->team_map_.end()) {
//		this->team_set_.erase(it->second);
		Map_Team* mt = it->second;
		this->team_map_.erase(it);
		POOL_MANAGER->push_map_team_pool(mt);
	}
	return 0;
}

int Team_Arena_Controler::open_sports_activity(void) {
	this->sports_activity_switch_ = TEAM_SPORTS_ACTIVITY_OPEN;

	return 0;
}

void Team_Arena_Controler::update_role_score(Block_Buffer &buf) {
	uint8_t type = 0;
	buf.read_uint8(type);
	if (type) {

		int role_size = 0;


		std::string server_name;
		buf.read_string(server_name);
		buf.read_int32(role_size);
		for (int i = 0; i < role_size; ++i) {
			role_id_t role_id = 0;
			int score = 0;
			int force = 0;
			std::string name;

			buf.read_int64(role_id);
			buf.read_int32(score);
			buf.read_int32(force);
			buf.read_string(name);

			Team_Role_Score_Map::iterator find_it = team_role_score_map.find(role_id);
			if (find_it != team_role_score_map.end()) {
				find_it->second.score = score;
				find_it->second.change = true;
			} else {
				Team_Role_Score role_score;
				role_score.role_id = role_id;
				role_score.score = score;
				role_score.force = force;
				role_score.name = name;
				role_score.area = server_name;
				team_role_score_map.insert(std::make_pair(role_id, role_score));
			}
		}

		if (level_team_role_score_map.empty())
			refresh_rank_list();
	} else {
		team_role_score_map.clear();
		level_team_role_score_map.clear();
	}

}

int Team_Arena_Controler::close_sports_activity(void) {
	if (!team_sports_test_) {
		this->sports_activity_switch_ = TEAM_SPORTS_ACTIVITY_CLOSE;
	}
	return 0;
}

bool Team_Arena_Controler::is_sports_open(void) {
	return TEAM_SPORTS_ACTIVITY_OPEN == this->sports_activity_switch_;
}
bool &Team_Arena_Controler::team_sports_test(void) {
	return team_sports_test_;
}

int Team_Arena_Controler::get_role_score(role_id_t role_id) {
	Team_Role_Score_Map::iterator find_it = team_role_score_map.find(role_id);
	if (find_it != team_role_score_map.end()) {
		return find_it->second.score;
	} else {
		return 0;
	}
}

int Team_Arena_Controler::get_role_score_list(role_id_t role_id, int level, Block_Buffer &buf) {
	//level = 0;
	//Team_Role_Score_Map::iterator t_find_it = team_role_score_map.begin();
	//if (t_find_it != team_role_score_map.end()) {
	//	level = t_find_it->second.rank;
	//}
	buf.write_int32(level);

	Level_Team_Role_Score_Map::iterator find_it = level_team_role_score_map.find(level);
	if (find_it != level_team_role_score_map.end()) {
		int amount = find_it->second.size() > 50 ? 50 : find_it->second.size();
		buf.write_int32(amount);
		for (Team_Role_Score_Vec::iterator it = find_it->second.begin(); it != find_it->second.end() && amount > 0; ++it, --amount) {
			buf.write_string((*it)->name);
			buf.write_string((*it)->area);
			buf.write_int32((*it)->score);
			buf.write_int32((*it)->force);
		}
	} else {
		buf.write_int32(0);
	}

	return 0;
}

int Team_Arena_Controler::refresh_rank_list(role_id_t role_id) {
	Scene_Player *player = 0;

	if (role_id) {
		Team_Role_Score_Map::iterator find_it = team_role_score_map.find(role_id);
		if (find_it != team_role_score_map.end()) {
			player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
			if (player) {
				Block_Buffer buf;
				buf.make_message(20100312);
				buf.write_int32(find_it->second.rank);
				buf.finish_message();
				player->send_to_logic(buf);
			}
		}
	} else {
		level_team_role_score_map.clear();
		for (Team_Role_Score_Map::iterator it = team_role_score_map.begin(); it != team_role_score_map.end(); ++it) {
			if (it->second.score > 0) {
				int level = CONFIG_CACHE_TEAM->arena_multi_level(it->second.score);
				level_team_role_score_map[level].push_back(&it->second);
				it->second.change = false;
			}
		}


		for (Level_Team_Role_Score_Map::iterator it = level_team_role_score_map.begin(); it != level_team_role_score_map.end(); ++it) {
			std::sort(it->second.begin(), it->second.end(), Team_Role_Score_Greater());

			int i = 1;
			for (Team_Role_Score_Vec::iterator r_it = it->second.begin(); r_it != it->second.end(); ++r_it, ++i) {
				if ((**r_it).rank != i) {
					player = SCENE_MONITOR->find_scene_player_by_role_id((*r_it)->role_id);
					if (player) {
						Block_Buffer buf;
						buf.make_message(20100312);
						buf.write_int32(i);
						buf.finish_message();
						player->send_to_logic(buf);
					}
				}
				(**r_it).rank = i;
			}
		}
	}

	return 0;
}

int Team_Arena_Controler::match_arena_team(void) {
	dispatch();
	//MSG_DEBUG("team_amount_: %d", team_amount_);
	if (team_amount_ > 0) {
		Map_Team *team = NULL;
		for (UInt_UIntVec_Map_Reverse::iterator it = level_team_ids_map_.begin();
				it != level_team_ids_map_.end(); ++it) {

			if (it->second.empty()) continue;

			Team_Id_Score_Vec team_id_score;
			for (Role_Vec::iterator team_it = it->second.begin(); team_it != it->second.end(); ++team_it) {
				int score = 0;
				team = SCENE_TEAM_MANAGER->get_team(*team_it);
				if (team) {
					score = team->get_team_score();
					Team_Id_Score team_score;
					team_score.team_id = team->get_team_id();
					team_score.score = score;
					team_score.match_fail = team->fail_times();
					team_id_score.push_back(team_score);
				}
			}

			if (random()%2)
				std::sort(team_id_score.begin(), team_id_score.end(), Team_Score_Greater());
			else
				std::sort(team_id_score.begin(), team_id_score.end(), Team_Score_Lesser());

			while (team_id_score.size() > 0) {
 				if (team_id_score.size() < 2) {
					team = SCENE_TEAM_MANAGER->get_team(team_id_score.front().team_id);
					if (team) {
						++team->match_fail();
					}
					break;
				}

				int64_t team_id1 = (*team_id_score.begin()).team_id;
				int team1_score = CONFIG_CACHE_TEAM->arena_multi_level((*team_id_score.begin()).score);
				team_id_score.erase(team_id_score.begin());

				int64_t team_id2 = (*team_id_score.begin()).team_id;
				int team2_score = CONFIG_CACHE_TEAM->arena_multi_level((*team_id_score.begin()).score);
				team_id_score.erase(team_id_score.begin());

				Map_Team *team1 = SCENE_TEAM_MANAGER->get_team(team_id1);
				Map_Team *team2 = SCENE_TEAM_MANAGER->get_team(team_id2);
				matched_handle(team_id1, team_id2);
				del_wait_match_team(team_id1);
				del_wait_match_team(team_id2);
				if (team1 && Map_Team::TEAM_SPORTS_SIGNED == team1->sports_status()) {
					team1->sports_status() = Map_Team::TEAM_SPORTS_MATCHED;
					team1->sports_battle_level() = team1_score;
				}
				if (team2 && Map_Team::TEAM_SPORTS_SIGNED == team2->sports_status()) {
					team2->sports_status() = Map_Team::TEAM_SPORTS_MATCHED;
					team2->sports_battle_level() = team2_score;
				}
			}
		}
	}
	return 0;
}

int Team_Arena_Controler::matched_handle(int64_t team_id1, int64_t team_id2) {
	Matched_Teams matched_teams;
	matched_teams.matched_pair.first =team_id1;
	matched_teams.matched_pair.second = team_id2;
	matched_teams.match_time = Time_Value::gettimeofday();
	team_deque_.push_back(matched_teams);
	team_matched_.insert(team_id1);
	team_matched_.insert(team_id2);
	{
		MSG_80100320 msg;
		Map_Team *team1 = SCENE_TEAM_MANAGER->get_team(team_id1);
		if (team1) {
			Role_Hash_Set role_set;
			team1->team_all_member_set(role_set);
			for (Role_Hash_Set::iterator it = role_set.begin(); it != role_set.end(); ++it) {
				Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					msg.roles.push_back(player->teamer_detail().team_match_info);
				}
			}
		}
		if (msg.roles.size() < 3) {
			Team_Match_Info team_match_info;
			for (uint16_t i = msg.roles.size(); i < 3; ++i) {
				msg.roles.push_back(team_match_info);
			}
		}
		Map_Team *team2 = SCENE_TEAM_MANAGER->get_team(team_id2);
		if (team2) {
			Role_Hash_Set role_set;
			team2->team_all_member_set(role_set);
			for (Role_Hash_Set::iterator it = role_set.begin(); it != role_set.end(); ++it) {
				Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					msg.roles.push_back(player->teamer_detail().team_match_info);
				}
			}
		}
		if (msg.roles.size() < 6) {
			Team_Match_Info team_match_info;
			for (uint16_t i = msg.roles.size(); i < 6; ++i) {
				msg.roles.push_back(team_match_info);
			}
		}
		if (team1) {
			Role_Hash_Set role_set;
			team1->team_all_member_set(role_set);
			for (Role_Hash_Set::iterator it = role_set.begin(); it != role_set.end(); ++it) {
				Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
			}
		}
	}
	{
		MSG_80100320 msg;
		Map_Team *team2 = SCENE_TEAM_MANAGER->get_team(team_id2);
		if (team2) {
			Role_Hash_Set role_set;
			team2->team_all_member_set(role_set);
			for (Role_Hash_Set::iterator it = role_set.begin(); it != role_set.end(); ++it) {
				Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					msg.roles.push_back(player->teamer_detail().team_match_info);
				}
			}
		}
		if (msg.roles.size() < 3) {
			Team_Match_Info team_match_info;
			for (uint16_t i = msg.roles.size(); i < 3; ++i) {
				msg.roles.push_back(team_match_info);
			}
		}
		Map_Team *team1 = SCENE_TEAM_MANAGER->get_team(team_id1);
		if (team1) {
			Role_Hash_Set role_set;
			team1->team_all_member_set(role_set);
			for (Role_Hash_Set::iterator it = role_set.begin(); it != role_set.end(); ++it) {
				Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					msg.roles.push_back(player->teamer_detail().team_match_info);
				}
			}
		}
		if (msg.roles.size() < 6) {
			Team_Match_Info team_match_info;
			for (uint16_t i = msg.roles.size(); i < 6; ++i) {
				msg.roles.push_back(team_match_info);
			}
		}
		if (team2) {
			Role_Hash_Set role_set;
			team2->team_all_member_set(role_set);
			for (Role_Hash_Set::iterator it = role_set.begin(); it != role_set.end(); ++it) {
				Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
				if (player) {
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
			}
		}
	}
	return 0;
}

int Team_Arena_Controler::dispatch(void) {
	level_team_ids_map_.clear();
	Role_Vec role_vec;
	for (int i = 0; i < 20; i++) {
		level_team_ids_map_[i * 5] = role_vec;
	}
	team_amount_ = 0;
	uint32_t lvl = 0; // 等级段
	for (Int64_Map_Team_Map::iterator it = team_map_.begin(); it != team_map_.end(); ) {
		if (!SCENE_TEAM_MANAGER->get_team(it->first)) {
			POOL_MANAGER->push_map_team_pool(it->second);
			team_map_.erase(it++);
			continue;
		}

		lvl = it->second->enter_level() % 10;
		if (lvl < 5) {
			lvl = it->second->enter_level() - lvl;
		} else {
			lvl = it->second->enter_level() - lvl + 5;
		}
		if (it->second->fail_times() > Map_Team::FAIL_TIMES && lvl >= 25) {
			//lvl -= 5;
		}

		level_team_ids_map_[lvl].push_back(it->first);
		++team_amount_;

		++it;
	}
	merge_dispatch();
	return 0;
}

int Team_Arena_Controler::merge_dispatch(void) {
	UInt_UIntVec_Map_Reverse level_team_ids_map_bak = level_team_ids_map_;
	level_team_ids_map_.clear();
	for (UInt_UIntVec_Map_Reverse::iterator it = level_team_ids_map_bak.begin(); it != level_team_ids_map_bak.end(); ++it) {
		int32_t now_amount = it->second.size();
		std::vector<int64_t> vec_to = it->second;
		uint32_t lvl = it->first;
		if (0 == now_amount) {
			continue;
		}

		// 当小于等于2支队伍时
		if (now_amount <= 2) {
			UInt_UIntVec_Map_Reverse::iterator it_tmp = it;
			++it_tmp;
			if (it_tmp != level_team_ids_map_bak.end()) {
				if (now_amount + it_tmp->second.size() >= 2) {
					// 和前一段合并后大于等于2支队伍
					merge_int_vec(it_tmp->second, vec_to);
					level_team_ids_map_[it_tmp->first] = vec_to;
					it = it_tmp;
					continue;
				} else if (now_amount + it_tmp->second.size() == 1) {
					UInt_UIntVec_Map_Reverse::iterator it_tmp_tmp = it_tmp;
					++it_tmp_tmp;
					if (it_tmp_tmp != level_team_ids_map_bak.end()) {
						if (now_amount + it_tmp->second.size() + it_tmp_tmp->second.size() >= 2) {
							// 和前前一段合并后大于等于2支队伍
							merge_int_vec(it_tmp_tmp->second, vec_to);
							level_team_ids_map_[it_tmp_tmp->first] = vec_to;
							it = it_tmp_tmp;
							continue;
						}
					}
				}
			}
		}

		level_team_ids_map_[lvl] = vec_to;
	}

	return 0;
}
