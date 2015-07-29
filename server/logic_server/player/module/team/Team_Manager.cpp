/*
 * Team_Manager.cpp
 *
 *  Created on: 2014年4月15日
 *      Author: Linqiyou
 */

#include "Team_Def.h"
#include "Team_Func.h"
#include "Team_Struct.h"
#include "Team_Manager.h"
#include "DB_Operator.h"
#include "Logic_Player_Struct.h"
#include "Logic_Player.h"
#include "Pool_Manager.h"
#include "Logic_Monitor.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Active_Enum.h"
#include "Err_Code.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache.h"
#include "DB_Def.h"
#include "Config_Cache_Team.h"
#include "mail/Mail_Struct.h"
#include "Config_Cache_Mail.h"
#include "Logic_Public.h"
#include "Msg_Inner_Enum.h"
#include "announce/Logic_Announce.h"

const int Team_Manager::TEAM_SPORTS_SCENE_ID = 80211002;

Team_Manager::Team_Manager(void) {
	team_id_ = 0;
	team_id_unused_.clear();
	team_map_.clear();
	team_index_id_map_.clear();
	team_id_for_scene_ = 0;

	wait_area_player_map_.clear();
	listen_player_map_.clear();
	player_listen_type_map_.clear();
	team_fb_vec_.clear();
	team_sports_vec_.clear();
	integrate_map_.clear();
	integrate_ranking_.clear();
	team_sports_switch_ = TEAM_SPORTS_SWITCH_CLOSE;
//	team_sports_ranking_checkout_switch_ = 0;
	team_sports_start_time_ = Time_Value::zero;
	team_sports_test_ = false;
	team_sports_open_time_ = Time_Value::zero;

	first_lvl_.reset();
	team_sports_open_times_ = 0;
}

Team_Manager::~Team_Manager(void) { }

int Team_Manager::start(void) {
	load_detail();
	return 0;
}
int Team_Manager::load_detail(void) {
	MONGO_OPERATOR->load_team_role(integrate_map_, integrate_ranking_, team_sports_start_time_, first_lvl_, team_sports_open_times_);
	return 0;
}
int Team_Manager::save_detail(void) {
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_TEAM_INFO);

	int idx_befor = buf.get_write_idx();
	buf.write_uint16(0);
	uint16_t amount = 0;
	for (RoleId_TeamSportsRankingDetail_Map::const_iterator it = integrate_map_.begin();
			it != integrate_map_.end(); ++it) {
		if (it->second.integrate_ > 0 || it->second.fail_times || it->second.win_times) {
			it->second.serialize(buf);
			++amount;
		}
	}
	int idx_after = buf.get_write_idx();
	buf.set_write_idx(idx_befor);
	buf.write_uint16(amount);
	buf.set_write_idx(idx_after);

	team_sports_start_time_.serialize(buf);
	first_lvl_.serialize(buf);
	buf.write_int32(team_sports_open_times_);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	return 0;
}

struct Team_PtrLess {
	bool operator() (const Team *team1, const Team *team2) const {
		return compare_less(*team1 , *team2);
	}
};
Team* Team_Manager::create_team(int scene_id, std::string password, int8_t type, int8_t degree) {
	if (type < Team::TEAM_TYPE_START || type > Team::TEAM_TYPE_END) {
		return 0;
	}
	Team *team = POOL_MANAGER->pop_team_pool();
	if (!team) {
		return 0;
	}
	team->set_team_id(this->generate_team_id());
	team->scene_id() = scene_id;
	team->password() = password;
	team->type() = type;
	team->degree() = degree;
	team->set_last_act_time_now();
	team_map_.insert(std::make_pair(team->get_team_id(), team));
	this->set_team_index(team);
	this->set_team_id_for_scene(team);

	this->change_team_type(team, type, scene_id, degree);
	return team;
}

int Team_Manager::set_team_index(Team* team) {
	if (!team) {
		return ERROR_TEAM_NOT_EXIST;
	}
	team->set_team_index(this->generate_team_index());
	team_index_id_map_.insert(std::make_pair(team->get_team_index(), team->get_team_id()));
	return 0;
}

int Team_Manager::set_team_id_for_scene(Team* team) {
	if (!team) {
		return ERROR_TEAM_NOT_EXIST;
	}
	team->set_team_id_for_scene(this->generate_team_id_for_scene());
	return 0;
}

void Team_Manager::release_team(Team *team) {
	if (!team) {
		return ;
	}
	int64_t team_id = team->get_team_id();

	this->team_map_.erase(team_id);
	this->recycle_team_index(*team);
	switch(team->type()) {
	case Team::TEAM_TYPE_FB: {
		const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(team->scene_id());
		if (scene_config) {
			for (std::vector<Team*>::iterator it = team_fb_vec_.begin(); it != team_fb_vec_.end(); ++it) {
				if (team_id == (**it).get_team_id()) {
					team_fb_vec_.erase(it);
					break;
				}
			}
			std::stable_sort(team_fb_vec_.begin(), team_fb_vec_.end(), Team_PtrLess());
		}
		break;
	}
	case Team::TEAM_TYPE_SPORTS: {
		for (std::vector<Team*>::iterator it = team_sports_vec_.begin(); it != team_sports_vec_.end(); ++it) {
			if (team_id == (**it).get_team_id()) {
				team_sports_vec_.erase(it);
				break;
			}
		}
		std::stable_sort(team_sports_vec_.begin(), team_sports_vec_.end(), Team_PtrLess());
		break;
	}
	case Team::TEAM_TYPE_TIMELIMIT: {
//		const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(team->scene_id());
//		if (scene_config) {
//			for (std::vector<Team*>::iterator it = team_fb_vec_.begin(); it != team_fb_vec_.end(); ++it) {
//				if (team_id == (**it).get_team_id()) {
//					team_fb_vec_.erase(it);
//					break;
//				}
//			}
//			std::stable_sort(team_fb_vec_.begin(), team_fb_vec_.end(), Team_PtrLess());
//		}
		break;
	}
	case Team::TEAM_TYPE_DEFEND: {
		break;
	}
	default:
		break;
	}
	POOL_MANAGER->push_team_pool(team);
}

int Team_Manager::recycle_team_index(Team &team) {
	uint32_t team_index = team.get_team_index();
	team.set_team_index(0);
	this->team_index_id_map_.erase(team_index);
	this->recycle_team_index(team_index);
	return 0;
}

Team* Team_Manager::get_team(int64_t team_id) {
	Int64_Team_Map::iterator it = team_map_.find(team_id);
	if (it != team_map_.end()) {
		return it->second;
	} else {
		return NULL;
	}
}

int Team_Manager::listen_team_info(const int8_t & type, role_id_t role_id) {
	this->listen_player_map_[type].insert(
			Role_Id_Set::value_type(role_id));
	this->player_listen_type_map_[role_id].insert(
			Role_Id_Set::value_type(type));
	return 0;
}

int Team_Manager::cancel_listen_team_info(const int8_t & type, role_id_t role_id) {
	if (this->listen_player_map_[type].count(role_id)) {
		this->listen_player_map_[type].erase(role_id);
	}
	if (this->player_listen_type_map_[role_id].count(type)) {
		this->player_listen_type_map_[role_id].erase(type);
	}

	if (this->listen_player_map_[type].empty()) {
		this->listen_player_map_.erase(type);
	}
	if (this->player_listen_type_map_[role_id].empty()) {
		this->player_listen_type_map_.erase(role_id);
	}
	return 0;
}

int Team_Manager::cancel_listen_team_info(role_id_t role_id) {
	int8_t type = 0;
	RoleID_Int8Set_Map::iterator it = this->player_listen_type_map_.find(role_id);
	if (it != this->player_listen_type_map_.end()) {
		for (Int8_Set::const_iterator it_type = it->second.begin(); it_type != it->second.end(); ++it_type) {
			type = *it_type;
			if (this->listen_player_map_[type].count(role_id)) {
				this->listen_player_map_[type].erase(role_id);
			}
			if (this->listen_player_map_[type].empty()) {
				this->listen_player_map_.erase(type);
			}
		}
	}
	this->player_listen_type_map_.erase(role_id);
	return 0;
}

int Team_Manager::team_wait(Logic_Player& player, int type) {
	this->wait_area_player_map_[type].insert(
			Role_Id_Set::value_type(player.role_id()));
	UInt_Role_Set_Map role_map;
	role_map[type].insert(Role_Id_Set::value_type(player.role_id()));
	this->active_team_waiter_add(type, &player, role_map, true);
	return 0;
}

int Team_Manager::cancel_wait(Logic_Player &player, int type) {

	if (this->wait_area_player_map_[type].count(player.role_id())) {
		this->wait_area_player_map_[type].erase(player.role_id());
		UInt_Role_Set_Map role_map;
		role_map[type].insert(Role_Id_Set::value_type(player.role_id()));
		this->active_team_waiter_del(type, &player, role_map, true);
	}
	if (this->wait_area_player_map_[type].empty()) {
		this->wait_area_player_map_.erase(type);
	}
	return 0;
}


void Team_Manager::cancel_assign_apply(Logic_Player &player, int scene_id) {
//	if (!is_scene_id_apply_hiring_able(scene_id)) {
//		return;
//	}
//
//	applyhire_map_[scene_id].apply_player_map.erase(player.base_detail().role_id);
//	MSG_802115 msg;
//	msg.role_id = player.base_detail().role_id;
//	msg.scene_id = scene_id;
//	Block_Buffer buf;
//	msg.serialize(buf);
//
//	info_fb_listener(scene_id, ACTIVE_ERASE_APPLY_PLAYER, 0, &buf, player.faction_role_detail().faction_id);
}

int64_t Team_Manager::generate_team_id(void) {
	do {
		if (team_id_ == 0) {
			team_id_ = CONFIG_CACHE->server_flag() * 1000000000L + 1;
		} else {
			++team_id_;
		}
	} while (team_map_.count(team_id_) != 0);

	return team_id_;
}

uint32_t Team_Manager::generate_team_index(void) {
	int team_id = 1;
	if (!this->team_id_unused_.empty()) {
		UInt_Set::iterator it = this->team_id_unused_.begin();
		team_id = *it;
		this->team_id_unused_.erase(it);
	} else {
		if (!this->team_index_id_map_.empty()) {
			team_id = team_index_id_map_.size() + 1;
		}
	}

	return team_id;
}

int64_t Team_Manager::generate_team_id_for_scene(void) {
	if (team_id_for_scene_ == 0) {
		team_id_for_scene_ = CONFIG_CACHE->server_flag() * 1000000000L + 1;
	} else {
		++team_id_for_scene_;
	}

	return team_id_for_scene_;
}

/*
 * 回收team_id
 * 1.当this->team_map_为空，清除this->team_id_unused
 * 2.当this->team_map_不空，判断传入的team_id与this->team_map_的最大team_id_max比较
 *   若大于，则一直清除this->team_id_unused尾部元素，直到小于
 * */
int Team_Manager::recycle_team_index(uint32_t team_index) {
	if (this->team_index_id_map_.empty()) {
		this->team_id_unused_.clear();
	} else {
		if (team_index <= 0) {
			return 0;
		}
		UInt_UInt_Map::iterator it_tm = --this->team_index_id_map_.end();
		uint32_t team_max = it_tm->first;
		if (team_max < team_index) {
			while (!this->team_id_unused_.empty()) {
				UInt_Set::iterator it = --this->team_id_unused_.end();
				if (team_max < (*it)) {
					team_index = *it;
					this->team_id_unused_.erase(it);
				} else {
					break;
				}
			}
		} else { // 不会相等
			this->team_id_unused_.insert(team_index);
		}
	}
	return 0;
}

void Team_Manager::tick(const Time_Value &now) {
	MSG_81000080 ac_msg;
	std::vector<Team*> dismiss_team_vec;
	Block_Buffer buf;
	for (Int64_Team_Map::iterator it = team_map_.begin(); it != team_map_.end(); ++it) {
		Team* team = it->second;
		if (team->start_open_card_time() != Time_Value::zero
				&& (now - team->start_open_card_time() > Time_Value(Team::OPEN_CARD_TIME)))
		{
			ac_msg.reset();
			if (!team->drops_all_vec().empty()) {
				// 翻牌处理
				Team::Reward_Info_Vec reward_info_vec;
				if (!team->role_reward_map().count( team->leader_role_id())) {
					reward_info_vec.clear();
					team->team_card_reward_msg(-1, team->leader_role_id(), ac_msg.list);
				}
				for (Role_Hash_Set::const_iterator it_on = team->team_member_set().begin();
						it_on != team->team_member_set().end(); ++it_on) {
					if (!team->role_reward_map().count(*it_on)) {
						reward_info_vec.clear();
						team->team_card_reward_msg(-1, *it_on, ac_msg.list);
					}
				}
			}
			buf.reset();
			buf.make_message(ac_msg.msg_id);
			ac_msg.serialize(buf);
			buf.finish_message();
			team->info_all_online_player(ac_msg.msg_id, &buf);
			team->start_open_card_time() = Time_Value::zero;
			team->drops_all_vec().clear();
			team->open_card_list().clear();
			team->role_reward_map().clear();
		}

		{
			// 离线超时检查
			if (!team->offline_time_map().empty()
					&& (team->in_battle()
							|| (Team::TEAM_TYPE_SPORTS == team->type() && Team::TEAM_SPORTS_NORMAL == team->sports_status()))) {
				Team::RoleID_TimeValue_Map offline_time_map = team->offline_time_map();
				for (Team::RoleID_TimeValue_Map::iterator it_off = offline_time_map.begin();
						it_off != offline_time_map.end(); ++it_off) {
					if (now - it_off->second > Time_Value(long(Team::DISMISS_TIMEOUT_SEC))) {
						role_id_t role_id = it_off->first;
						if (team->get_all_team_member_nums() > 1) {
							if (team->type() == Team::TEAM_TYPE_TIMELIMIT
									&& team->is_leader(role_id)) {
								dismiss_team_vec.push_back(team);
							} else {
								team->player_leave(role_id);
								team->offline_teamer_map().erase(role_id);
								team->offline_time_map().erase(role_id);
								if (team->last_act_time() != Time_Value::zero) {
									team->set_last_act_time_now();
								}
							}
						} else {
							dismiss_team_vec.push_back(team);
						}
					}
				}
			} else if(0 == team->get_all_team_member_nums()) {
				dismiss_team_vec.push_back(team);
			}
		}
	}

	for (std::vector<Team*>::iterator it = dismiss_team_vec.begin(); it != dismiss_team_vec.end(); ++it) {
		if ((*it)->type() == Team::TEAM_TYPE_TIMELIMIT) {
			(*it)->team_dismiss_without_leader();
		} else {
			(*it)->team_dismiss();
		}
		this->release_team(*it);
	}

	dismiss_team_vec.clear();
	for (Int64_Team_Map::iterator it = team_map_.begin(); it != team_map_.end(); ++it) {
		if (it->second->is_team_offline_over_time(now)
				|| (Time_Value::zero != it->second->last_act_time()
						&& now - it->second->last_act_time() >= CONFIG_CACHE_TEAM->team_config().offline_limit
						&& TEAM_OFFLINE_NUMS == it->second->get_all_team_member_nums())) {
			dismiss_team_vec.push_back(it->second);
		}
	}

	for (std::vector<Team*>::iterator it = dismiss_team_vec.begin(); it != dismiss_team_vec.end(); ++it) {
		(*it)->team_dismiss();
		this->release_team(*it);
	}

	team_sports_tick(now);

	if (now > save_tick_) {
		this->save_detail();
		save_tick_ = now + Time_Value(3600 * 3);
	}
}

void Team_Manager::team_sports_tick(const Time_Value &now) {
	//TEAM_SPORTS_SWITCH_CLOSE
	Date_Time dt(now);
	if (((dt.hour() >= 12 && dt.hour() < 14)
			|| (dt.hour() >= 18 && dt.hour() <= 19))) {
		if (((dt.hour() == 12 || dt.hour() == 18) && dt.minute() < 30)
				|| ((dt.hour() == 13 || dt.hour() == 19) && dt.minute() >= 30)) {
			close_team_sports();
		} else {
			open_team_sports();
		}
	} else {
		if (team_sports_test_ == false
				|| (team_sports_open_time_ != Time_Value::zero && team_sports_open_time_ < now)) {
			close_team_sports();
		}
	}

	if (team_sports_start_time_ != Time_Value::zero) {
		if (team_sports_start_time_ < now) {

			handle_ranking_checkout();
		}
	}
}

void Team_Manager::change_team_score(role_id_t role_id, int score) {
	RoleId_TeamSportsRankingDetail_Map::iterator it = this->integrate_map_.find(role_id);
	if (it != this->integrate_map_.end()) {
		it->second.integrate_ = score;

		Block_Buffer buf;
		buf.make_message(INNER_SYNC_TEAM_INTEGRATE);
		buf.write_uint8(1);
		buf.write_string(CONFIG_CACHE->server_maintainer_cache().platform_des);
		buf.write_int32(1);
		buf.write_int64(role_id);
		buf.write_int32(it->second.integrate_);
		buf.write_int32(it->second.force);
		buf.write_string(it->second.name_);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, buf);
	}
}

const Time_Value &Team_Manager::team_sports_open_time(void) {
	return team_sports_open_time_;
}

int Team_Manager::get_role_team_level(role_id_t role_id) {
	RoleId_TeamSportsRankingDetail_Map::iterator find_it = integrate_map_.find(role_id);
	if (find_it != integrate_map_.end()) {
		const Arena_Multi_Checkout_Detail *cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_checkout(find_it->second.integrate_);
		if (cfg) {
			return cfg->index;
		}
	}
	return 0;
}

void Team_Manager::free_pool_cache(void) {
//	team_pool_.shrink_all();
}

void Team_Manager::handle_ranking_checkout(void) {
	Date_Time dt_ck(Time_Value::gettimeofday() + Time_Value(Time_Value::ONE_DAY_IN_SECS * 3));
	dt_ck.hour(0);
	dt_ck.minute(0);
	dt_ck.second(0);
	dt_ck.microsec(0);
	team_sports_start_time_.sec(dt_ck.time_sec());

	for (Team_Sports_Ranking_Info::const_iterator it = integrate_ranking_.begin();
			it != integrate_ranking_.end(); ++it) {
		const Arena_Multi_Checkout_Detail *cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_checkout((**it).integrate_);
		if (!cfg) {
			continue;
		}
		if (cfg->item_vec.empty()) {
			continue;
		}

		Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id((**it).role_id_);
		if (teamer) {
			teamer->notice_team_award(CONFIG_CACHE_TEAM->arena_multi_level((**it).integrate_));
		} else {
			Logic_Player::offline_team_award((**it).role_id_, CONFIG_CACHE_TEAM->arena_multi_level((**it).integrate_));
		}
	}

	if (team_sports_open_times_%2 == 1) {
		integrate_ranking_.clear();
		integrate_map_.clear();
	}

	team_sports_open_times_++;

	save_detail();

	// 同步组队竞技积分
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_TEAM_INTEGRATE);
	buf.write_uint8(0);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, buf);
}

const Int64_Team_Map &Team_Manager::team_map(void) {
	return team_map_;
}

const Team_Vec &Team_Manager::team_fb_vec(void) {
	return team_fb_vec_;
}

const Team_Vec &Team_Manager::team_sports_vec(void) {
	return team_sports_vec_;
}

int Team_Manager::change_team_type(Team *team, int8_t type, const int32_t scene_id, const int32_t degree) {
	int ret = 0;
	switch(type) {
		case Team::TEAM_TYPE_FB: {
			ret = this->insert_fb_vec(team, scene_id);
			erase_team_type(team, type);
			if (0 == ret) {
				team->type() = type;
				team->scene_id() = scene_id;
				team->degree() = degree;
			}
			break;
		}
		case Team::TEAM_TYPE_SPORTS: {
			ret = this->insert_sports_vec(team);
			erase_team_type(team, type);
			if (0 == ret) {
				team->type() = type;
				team->scene_id() = scene_id;
				team->degree() = degree;
			}
			//this->active_integrate(*team);
			break;
		}
		case Team::TEAM_TYPE_HERO_DREAM:
			erase_team_type(team, type);
			if (0 == ret) {
				team->type() = type;
				team->scene_id() = scene_id;
				team->degree() = degree;
			}
			break;
		case Team::TEAM_TYPE_TIMELIMIT: {
			break;
		}
		case Team::TEAM_TYPE_DEFEND: {
//			ret = this->insert_fb_vec(team, scene_id);
			erase_team_type(team, type);
			if (0 == ret) {
				team->type() = type;
				team->scene_id() = scene_id;
				team->degree() = degree;
			}
			break;
		}
		default:
			ret = -1;
			break;
	}
	return ret;
}

int Team_Manager::insert_fb_vec(Team *team,const int32_t scene_id) {
	if (!team) {
		return -1;
	}
	for (Team_Vec::const_iterator it = this->team_fb_vec_.begin(); it != this->team_fb_vec_.end(); ++it) {
		if (team->get_team_id() == (*it)->get_team_id()) {
			return -1;
		}
	}
	if (0 == set_fb_level(team, scene_id)) {
		team_fb_vec_.push_back(team);
		std::stable_sort(team_fb_vec_.begin(), team_fb_vec_.end(), Team_PtrLess());
		return 0;
	}
	return -1;
}

int Team_Manager::insert_sports_vec(Team *team) {
	for (Team_Vec::const_iterator it = this->team_sports_vec_.begin(); it != this->team_sports_vec_.end(); ++it) {
		if (team->get_team_id() == (*it)->get_team_id()) {
			return -1;
		}
	}
	this->set_sports_level(team);
	this->team_sports_vec_.push_back(team);
	std::stable_sort(this->team_sports_vec_.begin(), this->team_sports_vec_.end(), Team_PtrLess());
	return 0;
}

int erase_team_vec(Team *team, Team_Vec &team_vec) {
	if (!team || team_vec.empty()) {
		return 0;
	}
	for (Team_Vec::iterator it = team_vec.begin(); it != team_vec.end(); ++it) {
		if ((*it)->get_team_id() == team->get_team_id()) {
			team_vec.erase(it);
			break;
		}
	}
	return 0;
}

int Team_Manager::erase_team_type(Team *team, int8_t type) {
	if (team->type() == type) {
		return 0;
	}
	switch(team->type()) {
	case Team::TEAM_TYPE_FB: {
		erase_team_vec(team, this->team_fb_vec_);
		break;
	}
	case Team::TEAM_TYPE_SPORTS: {
		erase_team_vec(team, this->team_sports_vec_);
		break;
	}
	case Team::TEAM_TYPE_HERO_DREAM: {
		break;
	}
	case Team::TEAM_TYPE_TIMELIMIT: {
		break;
	}
	case Team::TEAM_TYPE_DEFEND: {
		break;
	}
	default:
		break;
	}
	return 0;
}

int Team_Manager::set_sports_level(Team *team) {
	if (!team) {
		return 0;
	}
	team->set_sports_level(false);
	return 0;
}

int Team_Manager::set_fb_level(Team *team, const int scene_id) {
	int scene_id_bak = scene_id;
	if (-1 == scene_id_bak) {
		scene_id_bak = team->scene_id();
	}
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id_bak);
	if (scene_config) {
		Scene_Config::Dungeon_Config_Detail dungeon = scene_config->dungeon;
		Scene_Config::Dungeon_Config_Detail::OpenConMap::const_iterator open_con_it = dungeon.open_con_map.find(2);
		int lvl = 0;
		if (open_con_it != dungeon.open_con_map.end()) {
			lvl = open_con_it->second;
		}
		team->set_enter_level(lvl);
		return 0;
	}
	return -1;
}

int Team_Manager::active_team_add(int8_t type, Logic_Player *player, const Int64_Team_Map &team_map, bool to_all) {
	if (team_map.empty()) {
		return 0;
	}
	MSG_80100300 res_msg;
	res_msg.operate = 1;
	res_msg.type = type;
	Team_Basic_Info team_info;
	for (Int64_Team_Map::const_iterator it =  team_map.begin(); it != team_map.end(); ++it) {
		team_info.reset();
		it->second->get_team_basic_info(team_info);
		if (0 == team_info.index || type != team_info.type) {
			continue;//
		}
		res_msg.teams.push_back(team_info);
	}

	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}
int Team_Manager::active_team_del(int8_t type, Logic_Player *player, const Int64_Team_Map &team_map, bool to_all){
	if (team_map.empty()) {
		return 0;
	}
	MSG_80100300 res_msg;
	res_msg.operate = 2;
	res_msg.type = type;
	Team_Basic_Info team_info;
	for (Int64_Team_Map::const_iterator it =  team_map.begin(); it != team_map.end(); ++it) {
		if (type != it->second->type()) {
			continue;
		}
		it->second->get_team_basic_info(team_info);
		res_msg.teams.push_back(team_info);
	}
	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}
int Team_Manager::active_team_update(int8_t type, Logic_Player *player, const Int64_Team_Map &team_map, bool to_all){
	if (team_map.empty()) {
		return 0;
	}
	MSG_80100300 res_msg;
	res_msg.operate = 3;
	res_msg.type = type;
	Team_Basic_Info team_info;
	for (Int64_Team_Map::const_iterator it =  team_map.begin(); it != team_map.end(); ++it) {
		team_info.reset();
		it->second->get_team_basic_info(team_info);
		if (0 == team_info.index || type != team_info.type) {
			continue;//
		}
		res_msg.teams.push_back(team_info);
	}

	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}

int Team_Manager::active_listen_player(const int8_t type, std::vector<Team_Basic_Info> &del_teams) {
	MSG_80100300 del_msg;
	del_msg.operate = 2;
	del_msg.type = type;
	del_msg.teams = del_teams;

	UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
	if (it != this->listen_player_map_.end()) {
		for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
			Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
			if (0 != player) {
				OBJ_SEND_TO_CLIENT(del_msg, (*player));
			}
		}
	}
	return 0;
}

int Team_Manager::active_team_waiter_add(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all) {
	if (role_map.empty()) {
		return 0;
	}
	MSG_80100301 res_msg;
	res_msg.operate = 1;
	res_msg.type = type;
	Role_Team_Info online_teamer_info;
	UInt_Role_Set_Map::const_iterator it = role_map.find(type);
	if (it != role_map.end()) {
		for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
			Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
			if (teamer) {
				online_teamer_info.reset();
				this->get_role_info_from_logic_player(teamer, online_teamer_info);
				res_msg.roles.push_back(online_teamer_info);
			}
		}
	}

	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}
int Team_Manager::active_team_waiter_del(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all){
	if (role_map.empty()) {
		return 0;
	}
	MSG_80100301 res_msg;
	res_msg.operate = 2;
	res_msg.type = type;
	Role_Team_Info online_teamer_info;
	UInt_Role_Set_Map::const_iterator it = role_map.find(type);
	if (it != role_map.end()) {
		for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
			Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
			if (teamer) {
				online_teamer_info.reset();
				this->get_role_info_from_logic_player(teamer, online_teamer_info);
				res_msg.roles.push_back(online_teamer_info);
			}
		}
	}


	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}
int Team_Manager::active_team_waiter_update(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all){
	if (role_map.empty()) {
		return 0;
	}
	MSG_80100301 res_msg;
	res_msg.operate = 3;
	res_msg.type = type;
	Role_Team_Info online_teamer_info;
	UInt_Role_Set_Map::const_iterator it = role_map.find(type);
	if (it != role_map.end()) {
		for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
			Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
			if (teamer) {
				online_teamer_info.reset();
				this->get_role_info_from_logic_player(teamer, online_teamer_info);
				res_msg.roles.push_back(online_teamer_info);
			}
		}
	}


	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}

int Team_Manager::active_team_waiter_list(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all) {
	if (role_map.empty()) {
		return 0;
	}
	MSG_80100301 res_msg;
	res_msg.operate = 4;
	res_msg.type = type;
	Role_Team_Info online_teamer_info;
	UInt_Role_Set_Map::const_iterator it = role_map.find(type);
	if (it != role_map.end()) {
		for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
			Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
			if (teamer) {
				online_teamer_info.reset();
				this->get_role_info_from_logic_player(teamer, online_teamer_info);
				res_msg.roles.push_back(online_teamer_info);
			}
		}
	}


	role_id_t role_id = 0;
	if (0 != player) {
		OBJ_SEND_TO_CLIENT(res_msg, (*player));
		role_id = player->role_id();
	}

	if (to_all) {
		UInt_Role_Set_Map::const_iterator it = this->listen_player_map_.find(type);
		if (it != this->listen_player_map_.end()) {
			for (Role_Id_Set::const_iterator it_role = it->second.begin(); it_role != it->second.end(); ++it_role) {
				if (role_id != *it_role) {
					Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it_role);
					if (0 != player) {
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}
				}
			}
		}
	}
	return 0;
}

int Team_Manager::get_role_info_from_logic_player(Logic_Player *player, Role_Team_Info &role) {
	role.reset();
	get_team_role_info_from_logic(player, role);
	return 0;
}

UInt_Role_Set_Map &Team_Manager::wait_area_player_map(void) {
	return this->wait_area_player_map_;
}

void Team_Manager::battle_result(role_id_t role_id, int result, int score, Item_Vec &pack_item_vec) {
	RoleId_TeamSportsRankingDetail_Map::iterator it = this->integrate_map_.find(role_id);
	if (it != this->integrate_map_.end()) {
		if (result) {
			it->second.win_times++;
		} else {
			it->second.fail_times++;
		}

		MSG_80100331 msg;
		msg.score = it->second.integrate_;
		const Arena_Multi_Checkout_Detail *cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_checkout(it->second.integrate_);
		if (cfg) {
			msg.level = cfg->index;
			msg.score_max = cfg->rank.high;
		}
		msg.score_get = score;
		for (Item_Vec::iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
			Item_Basic_Info item;
			item.id = (*it).id_;
			item.amount = (*it).amount_;
			item.bind = (*it).bind_;
			msg.item.push_back(item);
		}
		msg.result = result;

		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if (player) {
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
}

int Team_Manager::integrage_change(role_id_t role_id, int32_t integrate) {
	int ret = 0;
	RoleId_TeamSportsRankingDetail_Map::iterator it = this->integrate_map_.find(role_id);
	if (it != this->integrate_map_.end()) {
		int b_level = CONFIG_CACHE_TEAM->arena_multi_level(it->second.integrate_);
		it->second.integrate_ += integrate;
		int a_level = CONFIG_CACHE_TEAM->arena_multi_level(it->second.integrate_);
		if (b_level != a_level)
			ret = 1;

		//Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		//if (player) {
		//	it->second.name_ = player->role_name();
		//}
	} else {
		Team_Sports_Ranking_Detail detail;
		detail.role_id_ = role_id;
		detail.integrate_ += integrate;
		Player_DB_View *target = LOGIC_MONITOR->find_player_db_view_by_role_id(role_id);
		if (target) {
			detail.name_ = target->role_name;
		} else {
				MSG_DEBUG("ERROR: role_id: %ld", role_id);
		}
		this->integrate_map_.insert(std::make_pair(role_id, detail));
		it = this->integrate_map_.find(role_id);
		if (it != this->integrate_map_.end()) {
			this->integrate_ranking_.push_back(&it->second);
		}
	}

	if (it->second.integrate_ < 0) {
		MSG_USER("role:%ld, integrate:%d", role_id, it->second.integrate_);
		it->second.integrate_ = 0;
	}

	Block_Buffer buf;
	buf.make_message(INNER_SYNC_TEAM_INTEGRATE);
	buf.write_uint8(1);
	buf.write_string(CONFIG_CACHE->server_maintainer_cache().platform_des);
	buf.write_int32(1);
	buf.write_int64(role_id);
	buf.write_int32(it->second.integrate_);
	buf.write_int32(it->second.force);
	buf.write_string(it->second.name_);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, buf);

	return ret;
}

int Team_Manager::active_integrate(const int64_t &role_id, int rank) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (!player) {
		return ERROR_TEAMER_OFFLINE;
	}
	Int64_Team_Map::iterator it = this->team_map_.find(player->teamer_detail().team_id);
	if (it == this->team_map_.end()) {
		return ERROR_TEAM_NOT_EXIST;
	}
	if (!it->second->is_in_team(role_id)) {
		return ERROR_TEAM_NOT_EXIST;
	}

	MSG_80100322 msg;
 	RoleId_TeamSportsRankingDetail_Map::iterator find_it = integrate_map_.find(role_id);
	if (find_it != integrate_map_.end()) {
		const Arena_Multi_Checkout_Detail *cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_checkout(find_it->second.integrate_);
		if (cfg) {
			msg.level = cfg->index;
			msg.score_max = cfg->rank.high;
		}
		msg.score = find_it->second.integrate_;
		msg.win = find_it->second.win_times;
		msg.fail = find_it->second.fail_times;
		msg.rank = find_it->second.rank = rank >= 0 ? rank : 0;
	} else {
		const Arena_Multi_Checkout_Detail *cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_checkout(0);
		if (cfg) {
			msg.score_max = cfg->rank.high;
		}

		Team_Sports_Ranking_Detail detail;
		detail.role_id_ = role_id;
		detail.name_ = player->role_name();
		detail.force = player->get_player_force();
		this->integrate_map_.insert(std::make_pair(role_id, detail));
		RoleId_TeamSportsRankingDetail_Map::iterator it = this->integrate_map_.find(role_id);
		if (it != this->integrate_map_.end()) {
			this->integrate_ranking_.push_back(&it->second);
		}
	}

	msg.times = player->profile_num_sports();

	OBJ_SEND_TO_CLIENT(msg, (*player));

	return 0;
}

int Team_Manager::active_integrate(void) {
	MSG_80100322 msg;
	Team_Sports_Integrate info;
	int32_t i = 1;
	for (Team_Sports_Ranking_Info::iterator it = this->integrate_ranking_.begin(); it != this->integrate_ranking_.end(); ++it) {
		if (i > Team_Def::TEAM_SPORTS_INTEGRATE_RANKING) {
			break;
		}
		info.index = i;
		info.integrate = (*it)->integrate_;
		info.name = (*it)->name_;
		info.role_id = (*it)->role_id_;
		msg.ranking.push_back(info);
		++i;
	}
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	for (Team_Vec::iterator it = this->team_sports_vec_.begin(); it != this->team_sports_vec_.end(); ++it) {
		(**it).info_all_online_player(msg.msg_id, &buf);
	}
	return 0;
}

int Team_Manager::active_integrate(Team &team) {
	MSG_80100322 msg;
	Team_Sports_Integrate info;
	int32_t i = 1;
	for (Team_Sports_Ranking_Info::iterator it = this->integrate_ranking_.begin(); it != this->integrate_ranking_.end(); ++it) {
		if (i > Team_Def::TEAM_SPORTS_INTEGRATE_RANKING) {
			break;
		}
		info.index = i;
		info.integrate = (*it)->integrate_;
		info.name = (*it)->name_;
		info.role_id = (*it)->role_id_;
		msg.ranking.push_back(info);
		++i;
	}
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	team.info_all_online_player(msg.msg_id, &buf);
	return 0;
}

const RoleId_TeamSportsRankingDetail_Map &Team_Manager::integrate_map(void) {
	return this->integrate_map_;
}
const Team_Sports_Ranking_Info &Team_Manager::integrate_ranking(void) {
	return this->integrate_ranking_;
}
const bool Team_Manager::team_sports_is_open(void) {
	return this->team_sports_switch_ == TEAM_SPORTS_SWITCH_OPEN;
}
int Team_Manager::open_team_sports(void) {
	if (team_sports_switch_ == TEAM_SPORTS_SWITCH_CLOSE) {
		team_sports_switch_ = TEAM_SPORTS_SWITCH_OPEN;
		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		if (dt.hour() >= 12 && dt.hour() <= 14) {
			dt.hour(14);
			dt.minute(0);
			dt.second(0);
			dt.microsec(0);
			team_sports_open_time_.sec(dt.time_sec());
		} else if (dt.hour() >= 18 && dt.hour() <= 19) {
			dt.hour(19);
			dt.minute(30);
			dt.second(0);
			dt.microsec(0);
			team_sports_open_time_.sec(dt.time_sec());
		} else {
			team_sports_open_time_ = now + Time_Value(Time_Value::ONE_HOUR_IN_SECS + Time_Value::ONE_MINUTE_IN_SECS * 30);
		}

		if (team_sports_start_time_ == Time_Value::zero) {
			Time_Value now = Time_Value::gettimeofday();
			Date_Time dt(now + Time_Value(Time_Value::ONE_DAY_IN_SECS * 3));
			dt.hour(0);
			dt.minute(0);
			dt.second(0);
			dt.microsec(0);
			team_sports_start_time_.sec(dt.time_sec());
			save_detail();
		}

		{
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_TEAM_SPORTS);
		buf.write_int8(team_sports_switch_);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, buf);
		}

		{
		// 同步组队竞技积分
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_TEAM_INTEGRATE);
		buf.write_uint8(1);
		buf.write_string(CONFIG_CACHE->server_maintainer_cache().platform_des);
		buf.write_int32(integrate_map_.size());
		for (RoleId_TeamSportsRankingDetail_Map::iterator it = integrate_map_.begin(); it != integrate_map_.end(); ++it) {
			buf.write_int64(it->first);
			buf.write_int32(it->second.integrate_);
			buf.write_int32(it->second.force);
			buf.write_string(it->second.name_);
		}
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, buf);
		}


		const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
		for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
//			if(it->second->level() >= 20){
				it->second->create_ann_team_sports();
				break;
//			}
		}

		MSG_81000408 msg;
		msg.open = 1;
		msg.active = 1;
		msg.icon_id = 11;
		msg.time = team_sports_open_time_.sec() - now.sec(); //Time_Value::ONE_HOUR_IN_SECS + Time_Value::ONE_MINUTE_IN_SECS * 30;
		for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
			if(it->second){
				OBJ_SEND_TO_CLIENT(msg, (*it->second));
			}
		}
	}
	return 0;
}
int Team_Manager::close_team_sports(void) {
	if (team_sports_switch_ == TEAM_SPORTS_SWITCH_OPEN) {
		team_sports_switch_ = TEAM_SPORTS_SWITCH_CLOSE;
		team_sports_open_time_ = Time_Value::zero;

		Block_Buffer buf;
		buf.make_message(INNER_SYNC_TEAM_SPORTS);
		buf.write_int8(team_sports_switch_);
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(TEAM_SPORTS_SCENE_ID, 0, buf);

		for (Team_Vec::iterator it = this->team_sports_vec_.begin(); it != this->team_sports_vec_.end(); ++it) {
			(**it).unready_all();
		}
		MSG_81000408 msg;
		msg.open = 0;
		msg.icon_id = 11;
		for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
			if(it->second){
				OBJ_SEND_TO_CLIENT(msg, (*it->second));
			}
		}
	}
	return 0;
}
int Team_Manager::team_sports_test(void) {
	team_sports_test_ = true;
	return 0;
}
int Team_Manager::team_sports_test_close(void) {
	team_sports_test_ = false;
	return 0;
}
void Team_Manager::hook_level_change(const int new_level, Logic_Player &player) {
	bool is_change = false;
	if (first_lvl_.first_lvl_role_id_his.count(new_level) == 0) {
		first_lvl_.first_lvl_role_id_his.insert(std::make_pair(new_level, player.role_id()));
		player.achievement_listen_first_lvl_role_id();
		is_change = true;
	}
	switch(player.career()) {
	case 1: {
		if (first_lvl_.first_lvl_zs_his.count(new_level) == 0) {
			first_lvl_.first_lvl_zs_his.insert(std::make_pair(new_level, player.role_id()));
			player.achievement_listen_first_lvl_zs();
			player.title_listen_level();
			is_change = true;
		}
		break;
	}
	case 2: {
		if (first_lvl_.first_lvl_ck_his.count(new_level) == 0) {
			first_lvl_.first_lvl_ck_his.insert(std::make_pair(new_level, player.role_id()));
			player.achievement_listen_first_lvl_ck();
			player.title_listen_level();
			is_change = true;
		}
		break;
	}
	case 3: {
		if (first_lvl_.first_lvl_yss_his.count(new_level) == 0) {
			first_lvl_.first_lvl_yss_his.insert(std::make_pair(new_level, player.role_id()));
			player.achievement_listen_first_lvl_yss();
			player.title_listen_level();
			is_change = true;
		}
		break;
	}
	default:
		break;
	}
}

Int_Uid &Team_Manager::first_lvl_role_id_his(void) {
	return first_lvl_.first_lvl_role_id_his;
}
Int_Uid &Team_Manager::first_lvl_ck_his(void) {
	return first_lvl_.first_lvl_ck_his;
}
Int_Uid &Team_Manager::first_lvl_zs_his(void) {
	return first_lvl_.first_lvl_zs_his;
}
Int_Uid &Team_Manager::first_lvl_yss_his(void) {
	return first_lvl_.first_lvl_yss_his;
}
