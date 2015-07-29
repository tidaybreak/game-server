/*
 * Teamer.cpp
 *
 *  Created on: 2014年4月15日
 *      Author: Linqiyou
 */

#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Team_Struct.h"
#include "Config_Cache_Team.h"
#include "Teamer.h"
#include "Team_Manager.h"
#include "Logic_Player.h"
#include "Team_Func.h"
#include "Logic_Monitor.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Err_Code.h"
#include "Msg_Teamer_Struct.h"
#include "Msg_Teamer_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_Active_Enum.h"
#include "Scene_Func.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache.h"
#include "helper/Helper_Def.h"
#include "Config_Cache_Role.h"
#include "chater/Chater_Def.h"
#include "task/Task_Struct.h"
#include "reward/Reward_Def.h"
#include "Scene_Def.h"
#include "Config_Cache_Dungeon.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "pack/Pack_Func.h"

Teamer::Teamer(void) {
	reset();
}

Teamer::~Teamer(void) {

}

void Teamer::reset(void) {
	this->teamer_detail_ = 0;
	this->profile_set_ = PROFILE_USE;
	follow_ = 0;
	start_time_ = 0;
}

void Teamer::load_detail(Teamer_Detail &detail) {
	this->teamer_detail_ = &detail;
}

void Teamer::save_detail(Teamer_Detail &detail) {
	detail = *this->teamer_detail_;
}

Teamer_Detail const &Teamer::teamer_detail(void) {
	return *this->teamer_detail_;
}

void Teamer::save_tick(void) {
	if (this->teamer_detail_) {
		this->teamer_detail_->save_tick();
	}
}

int Teamer::create_init(Teamer_Detail &detail) {
	return 0;
}

void Teamer::module_init(void) {
	bool team_exist = false;
	Team* team = this->get_team();
	if (NULL != team) {
		if (!team->is_in_team(this->role_id())) {
			set_no_team();
		} else if (team->type() != Team::TEAM_TYPE_TIMELIMIT) {
			int64_t arg3 = team->get_team_id();
			if (team->type() == Team::TEAM_TYPE_FB
					|| team->type() == Team::TEAM_TYPE_HERO_DREAM
					|| team->type() == Team::TEAM_TYPE_DEFEND) {
				if (0 == team->in_battle()) {
					const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(team->type());
					if (NULL == scene_id_ptr) {
						set_no_team();
					} else {
						player_self()->base_detail_no_const().location.scene_id = *scene_id_ptr;
						const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
						if (sc->type == Team_Scene_Type
								|| sc->type == Task_Scene_Type) {
							if (!sc->revive_point.empty()) {
								player_self()->base_detail_no_const().location.coord = sc->revive_point[0];
							}
						}
					}
				} else {
					const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
					if (sc->type != Team_Scene_Type
							&& sc->type != Task_Scene_Type) {
						arg3 = team->get_team_id_for_scene();
					}
				}
				this->player_self()->set_create_scene_args(0, Time_Value::zero, arg3, 0, 0, 0, true);
			} else {
				this->player_self()->set_create_scene_args(0, Time_Value::zero, arg3, 0, 0, 0, true);
			}
		}
		team_exist = true;
	}
	if (!team_exist) {
		const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
		if (sc && sc->timeLimit
				&& 0 != player_self()->tasker_detail().target_player_id
				&& 0 != player_self()->tasker_detail().task_id) {
			this->create_team_timelimit(player_self()->location().scene_id, 0, Time_Value::zero, player_self()->tasker_detail().target_player_id,
					player_self()->tasker_detail().task_id, 0, 0, true);
			Team* team = this->get_team();
			if (NULL != team) {
				team->player_offline(*player_self());
			}
		}
	}
}

void Teamer::sign_in(void) {
	if (teamer_detail_->team_id != 0) {
		Team* team = this->get_team();
		// 队伍不存在
		if (NULL == team) {
			set_no_team();
		} else {
			// 玩家掉线期间已被踢出队伍
			if (!team->is_in_team_offlinelist(teamer_detail_->role_id)) {
				set_no_team();
			} else {
//				team->player_online(*this->player_self());
			}
		}
	}
}

int Teamer::sign_in_repeat(bool is_trustership) {
	if (teamer_detail_->team_id != 0) {
		Team* team = this->get_team();

		// 队伍不存在
		if (NULL == team) {
			set_no_team();
		} else {
			if (!is_trustership)
				team->player_offline(*this->player_self());

			// 玩家掉线期间已被踢出队伍
			if (!team->is_in_team_offlinelist(teamer_detail_->role_id)) {
				set_no_team();
			}
		}
	}
	return 0;
}

void Teamer::sign_out(void) {
	Team *team = get_team();
	if (NULL != team) {
//		if (team->type() != Team::TEAM_TYPE_TIMELIMIT) {
			team->player_offline(*this->player_self());
//		} else {
//			this->leave_team();
//		}
	}

	TEAM_MANAGER->cancel_listen_team_info(teamer_detail_->role_id);
	TEAM_MANAGER->cancel_wait(*this->player_self(), teamer_detail_->wait_area_type);
}

void Teamer::trusteeship_sign_out(void) {
	Team *team = get_team();
	if (NULL != team) {
//		if (team->type() != Team::TEAM_TYPE_TIMELIMIT) {
			team->player_offline(*this->player_self());
//		} else {
//			this->leave_team();
//		}
	}

	TEAM_MANAGER->cancel_listen_team_info(teamer_detail_->role_id);
	TEAM_MANAGER->cancel_wait(*this->player_self(), teamer_detail_->wait_area_type);
}

int Teamer::client_ready_now(void) {
	Team* team = this->get_team();
	// 队伍不存在
	if (NULL == team) {
		set_no_team();
	} else {
		// 玩家掉线期间已被踢出队伍
		if (!team->is_in_team_offlinelist(teamer_detail_->role_id)) {
			set_no_team();
		} else {
			profile_info();
			team->player_online(*this->player_self());

			int8_t type = team->type();
			const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(type);
			if (NULL == scene_id_ptr) {
				MSG_USER("ERROR scene_type:%d", type);
				return -1;
			}
			int scene_id = *scene_id_ptr;
			if (player_self()->location().scene_id == scene_id) {
				team->has_sub_profile().erase(role_id());
				if (team->last_act_time() != Time_Value::zero) {
					team->set_last_act_time_now();
				}
			}

			Role_Hash_Set role_set;
			team->team_all_member_set(role_set);
			role_set.erase(role_id());
			if (role_set.empty()) {
//				team->set_last_act_time_now();
			} else {
//				bool set_now = true;
//				for (Role_Hash_Set::const_iterator it_mm = role_set.begin(); it_mm != role_set.end(); ++it_mm) {
//					Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(*it_mm);
//					if (pl && pl->location().scene_id != scene_id) {
//						set_now = false;
//					}
//				}
//				if (set_now) {
//					team->set_last_act_time_now();
//				}
			}
		}
	}
	if (TEAM_MANAGER->team_sports_is_open()) {
		MSG_81000408 msg;
		msg.open = 1;
		msg.icon_id = 11;
		msg.active = 1;
		int64_t last = TEAM_MANAGER->team_sports_open_time().sec() - Time_Value::gettimeofday().sec();
		msg.time = static_cast<uint16_t>(last);
		THIS_SEND_TO_CLIENT(msg);
	}

	notice_team_award();
	return 0;
}

int Teamer::move_scene_completed(void) {
	Team *team = this->get_team();
	if (team) {
		const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
		if (!cfg || (Scene_Team != cfg->manager
				&& Scene_Task_Manager != cfg->manager
				&& Scene_Team_Defend != cfg->manager
				&& Scene_Killing != cfg->manager)) {
			this->leave_team();
			return -1;
		}
		profile_info();
		team->refresh_teammer_info(*player_self());
		team->info_all_team_info(*player_self());
		team->lineup_active(role_id());
		TEAM_MANAGER->active_integrate(role_id());
		client_ready_broadcast_ = false;
		if (!team->is_not_first_team_fb()
				&& team->type() == Team::TEAM_TYPE_FB
				&& team->get_all_team_member_nums() == 1) {
			active_is_first_team_fb();
		}
		sync_teamer_detail_to_scene();
	} else {
		bool need_sync = false;
		if (this->teamer_detail_->team_id > 0) {
			need_sync = true;
		}
		set_no_team();
		if (need_sync) {
			sync_teamer_detail_to_scene();
		}
	}
	return 0;
}

int Teamer::trigger_daily_zero(const Time_Value &now) {
	return 0;
}
int Teamer::trigger_daily_six(const Time_Value& now, bool logic_check) {
	this->teamer_detail_->profile_num_fb = CONFIG_CACHE_TEAM->team_config().profile_num_fb;
	this->teamer_detail_->profile_num_sports = CONFIG_CACHE_TEAM->team_config().profile_num_sports;
	this->teamer_detail_->profile_num_hero_dream = CONFIG_CACHE_TEAM->team_config().profile_num_hero_dream;
	this->teamer_detail_->profile_num_defend = CONFIG_CACHE_TEAM->team_config().profile_num_defend;
	this->teamer_detail_->profile_time = now;
	return 0;
}

int Teamer::fetch_team_info(int8_t type) {
	TEAM_MANAGER->active_team_add(type, player_self(), TEAM_MANAGER->team_map());
	TEAM_MANAGER->active_team_waiter_list(type, player_self(), TEAM_MANAGER->wait_area_player_map());
	return 0;
}

int Teamer::create_team(int scene_id, std::string password, int8_t type, int8_t degree) {
	if (player_self()->is_in_wipe_out()) {
		return ERROR_COPY_IS_WIPE_OUT;
	}
	int ret = this->check_create_team_condition(type, scene_id);
	if (0 != ret) {
		if (ERROR_LEVEL_NEED_LIMIT == ret) {
			ret = 0;
		}
		return ret;
	}

	Team* team = TEAM_MANAGER->create_team(scene_id, password, type, degree);
	if (team) {
		ret = team->player_join(*this->player_self());

		if (0 == ret) {
			MSG_50100301 res_msg;
			res_msg.index = team->get_team_index();
			res_msg.password = team->password();
			res_msg.scene_id = team->scene_id();
			THIS_SEND_TO_CLIENT(res_msg);

			Int64_Team_Map teams;
			teams.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
			TEAM_MANAGER->active_team_add(type, player_self(), teams, true);

			this->team_exit_wait_area(type);
		}
		return ret;
	} else {
		return -1;
	}
}

int Teamer::dismiss_team(void) {
	Team *team = get_team();
	if (NULL != team) {
		team->team_dismiss();
		TEAM_MANAGER->release_team(team);
		set_no_team();
		return 0;
	} else {
		return -1;
	}
	return 0;
}

int Teamer::profile_info(const int8_t type) {
	MSG_50100308 msg;
	msg.type = type;
	Team *team = this->get_team();
	if (team) {
		msg.type = team->type();
		switch(team->type()) {
		case Team::TEAM_TYPE_FB:
			msg.num = this->profile_num_fb_all();
			break;
		case Team::TEAM_TYPE_SPORTS:
			msg.num = (this->profile_num_sports_all() > 0 ? this->profile_num_sports_all() : 0);
			break;
		case Team::TEAM_TYPE_HERO_DREAM:
			msg.num = this->profile_num_hero_dream();
			break;
		case Team::TEAM_TYPE_DEFEND:
			msg.num = this->profile_num_defend_all();
			break;
		default:
			break;
		}
		msg.profile_set = team->has_profile_set(role_id());//this->profile_set_;
	} else {
		switch(type) {
		case Team::TEAM_TYPE_FB:
			msg.num = this->profile_num_fb_all();
			break;
		case Team::TEAM_TYPE_SPORTS:
			msg.num = (this->profile_num_sports_all() > 0 ? this->profile_num_sports_all() : 0);
			break;
		case Team::TEAM_TYPE_HERO_DREAM:
			msg.num = this->profile_num_hero_dream();
			break;
		case Team::TEAM_TYPE_DEFEND:
			msg.num = this->profile_num_defend_all();
			break;
		default:
			break;
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

//int Teamer::team_setting(void) {
//	this->profile_info();
//
//	{
////		MSG_80100318 res_msg;
//		Team* team = this->get_team();
//		if (NULL != team) {
////			team->get_team_basic_info(res_msg.info);
////			team->set_last_act_time_now();
//		}
////		THIS_SEND_TO_CLIENT(res_msg);
//	}
//	return 0;
//}

int Teamer::invite_role(const MSG_10100302 &msg) {
	if (msg.role_id == teamer_detail_->role_id) {
		return -1;
	}

	Team *team = get_team();
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}

	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(msg.role_id);
	if (player == 0) {
		return ERROR_TEAMER_OFFLINE;
	}

//	const Json::Value &scene_config = CONFIG_CACHE->server_scene_conf(player->scene_detail().scene_id);
//	if (scene_config != Json::Value::null) {
//		if (player_self()->scene_detail().scene_id != player->scene_detail().scene_id && scene_config["team_not_be_invite"].asInt()) {
//			return respond_error_result(RES_INVITE_ROLE, ERROR_TEAM_NOT_BE_INVITE);
//		}
//		if (scene_config["scene_forbid_team"].asBool()) {
//			return respond_error_result(RES_INVITE_ROLE, ERROR_SCENE_TEAM_LIMITA);
//		}
//		if (player->base_detail().faction_id != player_self()->base_detail().faction_id
//				&& scene_config["team_must_same_faction"].asBool()) {
//			return respond_error_result(RES_APPLY_TEAM, ERROR_TEAM_SCENE_LIMIT_FACTION);
//		}
//	}

	if (player->has_team()) {
		return ERROR_ROLE_HAVE_TEAM;
	}

	Scene_Type scene_type = get_scene_type(player->location().scene_id);
	if (scene_type != Scene_Type_Null
			&& Main_Scene_Type != scene_type) {
		return ERROR_TEAM_CANNOT_INVITE;
	}

	int result = 0;
//	bool direct_accept = player->teamer_detail().auto_accept_invite || team_dont_need_agree(*this->player_self(), *player);
//	if (direct_accept) {
//		if (!has_team()) {
//			create_team();
//		}
//		Team *team = get_team();
//		if (NULL != team) {
//			result = team->player_join(*player);
//		}
//	} else {
	team->add_monster(msg.monster);
		result = player->add_inviter(*this->player_self());
//	}

	return result;
}

/*
 * is_world
 * true:房间发的；则已经进副本之后不让加入
 * false:副本内号召的
 * */
int Teamer::apply_team(const MSG_10100303 &msg) {
	if (player_self()->is_in_wipe_out()) {
		return ERROR_COPY_IS_WIPE_OUT;
	}
	int ret = this->check_join_team_condition(msg.role_id, msg.index);

	if (0 != ret) {
		if (ERROR_LEVEL_NEED_LIMIT == ret) {
			ret = 0;
		}
		return ret;
	}

	Team *team = 0;
	if (msg.index != -1) {
		team = TEAM_MANAGER->get_team(msg.index);
	} else {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(msg.role_id);
		if (player == 0) {
			return ERROR_TEAM_LEADER_NOT_ONLINE;
		}
		team = player->get_team();
	}
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}

	int scene_id = 0;
	if ((team->type() == Team::TEAM_TYPE_FB
			|| team->type() == Team::TEAM_TYPE_HERO_DREAM
			|| team->type() == Team::TEAM_TYPE_DEFEND ) && !team->in_battle()) {
		const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(team->type());
		if (NULL == scene_id_ptr) {
			MSG_USER("ERROR scene_type:%d", team->type());
			return -1;
		}
		scene_id = *scene_id_ptr;

		for (Role_Hash_Set::const_iterator it = team->team_member_set().begin(); it != team->team_member_set().end(); ++it) {
			Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
			if (player && scene_id != player->location().scene_id) {
				return ERROR_TEAM_NOT_READY;
			}
		}
		Logic_Player *player = team->get_team_leader();
		if (!player) {
			return  ERROR_TEAM_LEADER_NOT_ONLINE;
		}
		if (scene_id != player->location().scene_id) {
			return  ERROR_TEAM_NOT_READY;
		}
	}
	Logic_Player* player = team->get_team_leader();
	if (player == 0) {
		return ERROR_TEAM_LEADER_NOT_ONLINE;
	}
	ret = player->add_applicant(*this->player_self(), msg.password, msg.profile_set);

	return ret;
}

int Teamer::make_leader(role_id_t new_id) {
	if (new_id == teamer_detail_->role_id) {
		return -1;
	}

	if (!is_leader()) {
		return ERROR_NOT_TEAM_LEADER;
	}
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(new_id);
	if (player == 0) {
		return ERROR_TEAMER_OFFLINE;
	}

	Team *team = get_team();
	int result = 0;
	if (NULL != team) {
		if (team->type() == Team::TEAM_TYPE_TIMELIMIT) {
			return ERROR_TEAM_TIMELIMIT_CANNOT_CHANGE_LEADER;
		}
		result = team->player_lead(*player);
//		team->info_all_team_info();
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}

		Int64_Team_Map team_map;
		team_map.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
		TEAM_MANAGER->active_team_update(team->type(), 0, team_map, false);
	}

	return result;
}

int Teamer::kick_role(role_id_t target_role_id) {
	if (!this->is_leader()) {
		return ERROR_NOT_TEAM_LEADER;
	}
	if (target_role_id == player_self()->role_id()) {
		return -1;
	}
	Team *team = get_team();
	int result = 0;
	if (NULL != team) {
		result = team->player_leave(target_role_id, 1);
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}
	} else {
		result = ERROR_NOT_TEAM_LEADER;
	}
	return result;
}

int Teamer::leave_team(void) {
	Team *team = get_team();
	int result = 0;
	if (NULL != team) {
		if (team->type() == Team::TEAM_TYPE_TIMELIMIT
				&& this->is_leader()) {
			team->team_dismiss_without_leader();
			TEAM_MANAGER->release_team(team);
		} else {
			result = team->player_leave(role_id());
			if (team->get_all_team_member_nums() == 0) {
				TEAM_MANAGER->release_team(team);
			} else {
				if (team->last_act_time() != Time_Value::zero) {
					team->set_last_act_time_now();
				}
			}
		}
	} else {
		result = ERROR_ROLE_NOT_HAVE_TEAM;
	}
	return result;
}

int Teamer::change_team_setting(int8_t profile_set) {
	MSG_50100310 res_msg;
	Team *team = this->get_team();
	if (team) {
		if (PROFILE_USE == profile_set) {
				switch(team->type()) {
				case Team::TEAM_TYPE_FB: {
					if (this->profile_num_fb_all() > 0) {
						team->profile_set_add(role_id());//this->profile_set_ = profile_set;
					}
					break;
				}
				case Team::TEAM_TYPE_HERO_DREAM: {
					if (this->profile_num_hero_dream() > 0) {
						team->profile_set_add(role_id());//this->profile_set_ = profile_set;
					}
					break;
				}
				case Team::TEAM_TYPE_SPORTS: {
					if (this->profile_num_sports_all() > 0) {
						team->profile_set_add(role_id());//this->profile_set_ = profile_set;
					}
					break;
				}
				case Team::TEAM_TYPE_TIMELIMIT: {
					break;
				}
				case Team::TEAM_TYPE_DEFEND: {
					if (this->profile_num_defend_all() > 0) {
						team->profile_set_add(role_id());//this->profile_set_ = profile_set;
					}
					break;
				}
				default:
					break;
				}
		} else {
			team->profile_set_del(role_id());//this->profile_set_ = profile_set;
		}
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}
		res_msg.profile_set = team->has_profile_set(role_id());//this->profile_set_;
	}
	sync_teamer_detail_to_scene();
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Teamer::agree_team_invite(const MSG_10100309 &msg) {
	if (player_self()->is_in_wipe_out()) {
		return ERROR_COPY_IS_WIPE_OUT;
	}
	int ret = this->check_join_team_condition(msg.role_id);

	if (0 != ret) {
		if (ERROR_LEVEL_NEED_LIMIT == ret) {
			ret = 0;
		}
		return ret;
	}

	Role_Hash_Set::iterator it = inviter_map_.find(msg.role_id);
	if (it == inviter_map_.end()) { // 无邀请记录
		return ERROR_CLIENT_OPERATE;
	}
	inviter_map_.erase(it);

	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(msg.role_id);
	if (player == 0) {
		return ERROR_TEAM_LEADER_NOT_ONLINE;
	}

	Team *team = player->get_team();
	int result = 0;
	if (NULL != team) {
		result = team->player_join(*this->player_self(), msg.profile_set);
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}
	}
	if (result == 0) {
		MSG_50100309 res_msg;
		res_msg.role_id = msg.role_id;
		THIS_SEND_TO_CLIENT(res_msg);

		{
			// broadcast all
			Int64_Team_Map team_map;
			team_map.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
			TEAM_MANAGER->active_team_update(team->type(), player_self(), team_map, true);

			this->team_exit_wait_area(team->type());
		}
	}

	return result;
}

int Teamer::listen_team_info(const int8_t & type, const int8_t &listened) {
	if (listened) {
		TEAM_MANAGER->listen_team_info(type, teamer_detail_->role_id);
	} else {
		TEAM_MANAGER->cancel_listen_team_info(type, teamer_detail_->role_id);
	}
	return 0;
}

int Teamer::team_enter_wait_area(role_id_t role_id, int8_t type) {
	if (has_team()) {
		return ERROR_ROLE_HAVE_TEAM;
	}

	int result = 0;
	if (result != 0) {
		return result;
	}

	result = TEAM_MANAGER->team_wait(*this->player_self(), type);
	MSG_50100312 res_msg;
	res_msg.result = 1;
	res_msg.type = type;
	if (0 == result) {
		res_msg.result = 0;
	}
	teamer_detail_->wait_area_type = type;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Teamer::team_exit_wait_area(int8_t type) {
	int result = TEAM_MANAGER->cancel_wait(*this->player_self(), type);

	MSG_50100313 res_msg;
	res_msg.result = 1;
	res_msg.type = type;
	if (0 == result) {
		res_msg.result = 0;
	}
	teamer_detail_->wait_area_type = 0;
	THIS_SEND_TO_CLIENT(res_msg);

	return result;
}

int Teamer::reject_team_invite(role_id_t role_id) {
	if (role_id == 0) {	// 拒绝所有
		for (Role_Hash_Set::iterator it = inviter_map_.begin(); it != inviter_map_.end(); ++it) {
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
			if (NULL != player) {
				player->rejected_invite_by(this->player_self());
			}
		}
		inviter_map_.clear();
	} else {
		Role_Hash_Set::iterator it = inviter_map_.find(role_id);
		if (it == inviter_map_.end()) { // 无邀请记录
			return ERROR_CLIENT_OPERATE;
		}
		inviter_map_.erase(it);
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if (NULL != player) {
			player->rejected_invite_by(this->player_self());
		}
	}

	MSG_50100316 res_msg;
	res_msg.role_id = role_id;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

struct Team_PtrLess {
	bool operator() (const Team *team1, const Team *team2) const {
		return compare_less(*team1 , *team2);
	}
};

int Teamer::req_apply_team_quick(int8_t type) {
	if (player_self()->is_in_wipe_out()) {
		return ERROR_COPY_IS_WIPE_OUT;
	}
	int ret = ERROR_TEAM_EMPTY;
	if (Team::TEAM_TYPE_FB == type
			|| Team::TEAM_TYPE_SPORTS == type
//			|| Team::TEAM_TYPE_DEFEND == type
			) {
		ret = this->join_team_in_rule(type);
	} else {
		for (Int64_Team_Map::const_iterator it = TEAM_MANAGER->team_map().begin();
				it != TEAM_MANAGER->team_map().end(); ++it) {
			if (type == it->second->type()
					&& it->second->get_all_team_member_nums() < it->second->get_position_max()
					&& it->second->password().empty() && 0 != it->second->get_team_index()) {
				ret = this->check_join_team_condition(it->second->leader_role_id());

				if (0 != ret) {
					continue;
				}

				ret = it->second->player_join(*player_self());

				if (0 == ret) {
					// broadcast all
					Int64_Team_Map team_map;
					team_map.insert(Int64_Team_Map::value_type(it->second->get_team_id(), it->second));
					TEAM_MANAGER->active_team_update(it->second->type(), player_self(), team_map, true);

					this->team_exit_wait_area(it->second->type());
				}
				break;
			}
		}
	}
	MSG_50100317 res_msg;
	res_msg.result = (ret != 0) ? 1 : 0;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Teamer::join_team_in_rule(const int8_t type) {
	int ret = 0;
	int32_t role_level = this->level();
	typedef std::map<uint32_t, Team_Vec> Dis_Team_Vec;
	Dis_Team_Vec dis_team_vec; // 可加入的队伍
	uint32_t dis = 0;
	Team_Vec vec;
	switch(type) {
	case Team::TEAM_TYPE_FB:
		vec = TEAM_MANAGER->team_fb_vec();
		break;
	case Team::TEAM_TYPE_SPORTS:
		vec = TEAM_MANAGER->team_sports_vec();
		break;
	case Team::TEAM_TYPE_HERO_DREAM:
		break;
	case Team::TEAM_TYPE_DEFEND: {
		break;
	}
	default:
		break;
	}
	for (Team_Vec::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		if (type == (*it)->type()
				&& (*it)->get_all_team_member_nums() < (*it)->get_position_max()
				&& (*it)->password().empty() && 0 != (*it)->get_team_index()) {
			ret = this->check_join_team_condition((*it)->leader_role_id());

			if (0 != ret) {
				continue;
			}
			dis = abs(role_level - (*it)->enter_level());
			dis_team_vec[dis].push_back(*it);
		}
	}

	bool join_team_success = false;
	if (!dis_team_vec.empty()) {
		Team_Vec team_vec;
		for (Dis_Team_Vec::iterator dis_it = dis_team_vec.begin(); dis_it != dis_team_vec.end(); ++dis_it) {
			team_vec = dis_it->second;
			std::stable_sort(team_vec.begin(), team_vec.end(), Team_PtrLess());
			for (Team_Vec::iterator it = team_vec.begin(); it != team_vec.end(); ++it) {
				ret = (*it)->player_join(*player_self());

				if (0 == ret) {
					// broadcast all
					Int64_Team_Map team_map;
					team_map.insert(Int64_Team_Map::value_type((*it)->get_team_id(), (*it)));
					TEAM_MANAGER->active_team_update((*it)->type(), player_self(), team_map, true);

					this->team_exit_wait_area((*it)->type());
					join_team_success = true;
				}
				break;
			}
			if (join_team_success) {
				break;
			}
		}
	}
	return join_team_success ? 0 : 1;
}

int Teamer::req_set_ready_state(bool ready) {
	if (Teamer_Detail::Ready_Status(ready) < Teamer_Detail::READY_START
			|| Teamer_Detail::Ready_Status(ready) > Teamer_Detail::READY_END) {
		return ERROR_CLIENT_PARAM;
	}

	Team *team = get_team();
	if (team) {
		if (this->is_leader() && !team->in_battle()) {
			if (ready) {
				if (team->last_act_time() != Time_Value::zero) {
					team->set_last_act_time_now();
				}
				int result = team->check_all_member_ready();
				if (team->get_offline_team_member_nums() > 0) {
					return ERROR_TEAM_MEMBER_NO_ONLINE;
				}
				if (0 == result) {	// 若全部玩家都已准备，则通知可匹配 否则报错
					switch(team->type()) {
					case Team::TEAM_TYPE_FB:
						team->in_battle() = 1;
						break;
					case Team::TEAM_TYPE_SPORTS: {
						if (false == TEAM_MANAGER->team_sports_is_open()) {
							return ERROR_TEAM_SPORTS_NOT_OPEN;
						}
						if (team->get_online_team_member_nums() < CONFIG_CACHE_TEAM->team_config().max_player) {
							return ERROR_MEMBER_NOT_ENOUGH;
						}
						team->in_battle() = 0;
						team->battle_over_flag() = Team::BATTLE_OVER_FLAG_UNSET;
						{
							MSG_80100321 msg;
							msg.ready = IN_MATCH;
							Block_Buffer buf;
							buf.make_message(msg.msg_id);
							msg.serialize(buf);
							buf.finish_message();
							team->info_all_online_player(ACTIVE_TEAM_IN_MATCH, &buf);
						}
						break;
					}
					case Team::TEAM_TYPE_HERO_DREAM:
						team->in_battle() = 1;
						break;
					case Team::TEAM_TYPE_DEFEND:
						team->in_battle() = 1;
						break;
					default:
						break;
					}
					// start
					int ret = team->start();
					if (0 != ret) {
						return ret;
					}
					this->teamer_detail_->ready = ready;
					this->sync_teamer_detail_to_scene();
//					team->info_all_team_info();
				} else {
					if (ERROR_TEAMMER_NOT_READY == result) {
						result = 0;
					}
					return result;
				}
			} else {
				if (this->teamer_detail_->ready != ready) {
					team->cancel_match();
					team->in_battle() = 0;
					this->teamer_detail_->ready = ready;
					this->active_other_member();
					this->sync_teamer_detail_to_scene();
					{
						MSG_80100321 msg;
						msg.ready = CANCEL_MATCH;
						Block_Buffer buf;
						buf.make_message(msg.msg_id);
						msg.serialize(buf);
						buf.finish_message();
						team->info_all_online_player(ACTIVE_TEAM_IN_MATCH, &buf);
					}
				}
			}
		} else {
			switch(team->type()) {
			case Team::TEAM_TYPE_FB:
				this->teamer_detail_->ready = ready;
				this->active_other_member();
				break;
			case Team::TEAM_TYPE_SPORTS: {
				if (Teamer_Detail::UNREADY == ready) {
					if (Team::TEAM_SPORTS_SIGNED == team->sports_status()) {
						team->cancel_match();
					}
				} else {
					if (false == TEAM_MANAGER->team_sports_is_open()) {
						return ERROR_TEAM_SPORTS_NOT_OPEN;
					}
				}
				if (this->teamer_detail_->ready != ready) {
					this->teamer_detail_->ready = ready;
					this->active_other_member();
					this->sync_teamer_detail_to_scene();
				}
				{
					MSG_80100321 msg;
					msg.ready = CANCEL_MATCH;
					Block_Buffer buf;
					buf.make_message(msg.msg_id);
					msg.serialize(buf);
					buf.finish_message();
					team->info_all_online_player(ACTIVE_TEAM_IN_MATCH, &buf);
				}
				break;
			}
			case Team::TEAM_TYPE_HERO_DREAM:
				this->teamer_detail_->ready = ready;
				this->active_other_member();
				break;
			case Team::TEAM_TYPE_DEFEND:
				this->teamer_detail_->ready = ready;
				this->active_other_member();
				break;
			default:
				break;
			}
		}
	}

	MSG_50100320 res_msg;
	res_msg.ready = this->teamer_detail_->ready;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Teamer::req_back_room(void) {
	Team* team = this->get_team();
	if (NULL != team) {
		team->has_sub_profile().erase(role_id());
		if (this->profile_set() == PROFILE_USE) {
			switch(team->type()) {
			case Team::TEAM_TYPE_FB: {
				if (this->profile_num_fb_all() == 0) {
					team->profile_set_del(role_id());
				}
				break;
			}
			case Team::TEAM_TYPE_SPORTS: {
				if (this->profile_num_sports_all() <= 0) {
					team->profile_set_del(role_id());
				}
				break;
			}
			case Team::TEAM_TYPE_HERO_DREAM: {
				if (this->profile_num_hero_dream() == 0) {
					team->profile_set_del(role_id());
				}
				break;
			}
			case Team::TEAM_TYPE_DEFEND: {
				if (this->profile_num_defend_all() == 0) {
					team->profile_set_del(role_id());
				}
				break;
			}
			default:
				break;
			}
		}
		int8_t type = team->type();
		const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(type);
		if (NULL == scene_id_ptr) {
			MSG_USER("ERROR scene_type:%d", type);
			return -1;
		}
		int scene_id = *scene_id_ptr;

		if (team->get_all_team_member_nums() > 1) {
			team->set_last_act_time_now();
		} else {
			Role_Hash_Set role_set;
			team->team_all_member_set(role_set);
			role_set.erase(role_id());
			bool set_now = true;
			for (Role_Hash_Set::const_iterator it_mm = role_set.begin(); it_mm != role_set.end(); ++it_mm) {
				Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(*it_mm);
				if (pl && pl->location().scene_id != scene_id) {
					set_now = false;
				}
			}
			if (set_now) {
				team->set_last_act_time_now();
			}
		}

		this->player_self()->set_create_scene_args(0, Time_Value::zero, team->get_team_id(), 0, 0, 0, true);
		this->player_self()->inner_transmit(scene_id, 0, 0, "");
	}
//	this->teamer_detail_->ready = false;
	return 0;
}

int Teamer::active_wait_area(int8_t type) {
	TEAM_MANAGER->active_team_waiter_list(type, player_self(), TEAM_MANAGER->wait_area_player_map());
	return 0;
}

int Teamer::set_password(std::string password_new) {
	Team *team = this->get_team();
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}

//	if (team->password() != password_old) {
//		return ERROR_TEAM_ERROR_PASSWORD;
//	}
	if (team->last_act_time() != Time_Value::zero) {
		team->set_last_act_time_now();
	}

	team->password() = password_new;

	team->refresh_teammer_info();

	MSG_50100318 res_msg;
	res_msg.password = team->password();
	THIS_SEND_TO_CLIENT(res_msg);

	Int64_Team_Map team_map;
	team_map.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
	TEAM_MANAGER->active_team_update(team->type(), player_self(), team_map, true);
	return 0;
}
int Teamer::cancel_password(std::string password) {
	Team *team = this->get_team();
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}

//	if (team->password() != password) {
//		return ERROR_TEAM_ERROR_PASSWORD;
//	}

	if (team->last_act_time() != Time_Value::zero) {
		team->set_last_act_time_now();
	}
	team->password() = "";

	team->refresh_teammer_info();

	MSG_50100319 res_msg;
	res_msg.password = team->password();
	THIS_SEND_TO_CLIENT(res_msg);

	Int64_Team_Map team_map;
	team_map.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
	TEAM_MANAGER->active_team_update(team->type(), player_self(), team_map, true);
	return 0;
}

int Teamer::add_inviter(Logic_Player &inviter) {
	Block_Buffer buf;
	buf.make_message(INNER_BATTLE_STATUS);
	buf.write_int64(inviter.role_id());
	buf.finish_message();
	return this->send_to_scene(buf);
}

int Teamer::req_battle_status(Block_Buffer &buf) {
	role_id_t role_id = 0;
	bool is_in_battle = 0;
	buf.read_int64(role_id);
	buf.read_bool(is_in_battle);

	if (0 == role_id) {
		return 0;
	}
	Logic_Player *inviter = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (!inviter) {
		return 0;
	}
	if (is_in_battle) {
		return inviter->send_err_to_client(ERROR_ADD_INVITER_FAIL, REQ_INVITE_ROLE);
	}

	Team *team = inviter->get_team();
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}

	inviter_map_.insert(role_id);
	if (team->last_act_time() != Time_Value::zero) {
		team->set_last_act_time_now();
	}

	MSG_80100308 msg;
	msg.type = team->type();
	msg.value = team->scene_id();
	msg.scene_degree = team->scene_degree();
	msg.in_battle = team->in_battle();
	msg.nums = get_profile_num_by_type(team->type());
	msg.monster = team->get_monster();
	TEAM_MANAGER->get_role_info_from_logic_player(inviter, msg.inviter);

	THIS_SEND_TO_CLIENT(msg);
	MSG_50100302 res_msg;
	res_msg.result = 1;
	OBJ_SEND_TO_CLIENT(res_msg, (*inviter));
	return 0;
}

int Teamer::add_applicant(Logic_Player &applicant, std::string password, int8_t profile_set) {
	Team *team = this->get_team();
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}
	if (!team->is_not_first_team_fb() && team->type() == Team::TEAM_TYPE_FB) {
		return ERROR_TEAM_FB_CAN_NOT_ADD_APPLICANT;
	}
	if (!team->in_battle() && "" != team->password() && team->password() != password) {
		return ERROR_TEAM_ERROR_PASSWORD;
	}
	int ret = team->player_join(applicant, profile_set);

	if (0 == ret) {
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}
		MSG_80100309 msg;
		TEAM_MANAGER->get_role_info_from_logic_player(&applicant, msg.applicant);
		THIS_SEND_TO_CLIENT(msg);

		{
			// broadcast all
			Int64_Team_Map team_map;
			team_map.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
			TEAM_MANAGER->active_team_update(team->type(), player_self(), team_map, true);

			this->team_exit_wait_area(team->type());
		}
	}
	return ret;
}

void Teamer::rejected_invite_by(Logic_Player *player) {
	MSG_80100310 msg;
	TEAM_MANAGER->get_role_info_from_logic_player(player, msg.rejecter);
	THIS_SEND_TO_CLIENT(msg);
	return ;
}

int Teamer::req_team_set_position(const int8_t type, const int8_t position) {
	if (Team::Position_Type(position) < Team::POSITION_START
			|| Team::Position_Type(position) > Team::POSITION_END) {
		return ERROR_CLIENT_PARAM;
	}

	if (!this->is_leader()) {
		return ERROR_NOT_TEAM_LEADER;
	}
	Team *team = this->get_team();
	if (NULL == team) {
		return ERROR_ROLE_NOT_HAVE_TEAM;
	}
	if (team->last_act_time() != Time_Value::zero) {
		team->set_last_act_time_now();
	}
	int8_t index = 0;
	for (Int8_Vec::iterator it = team->index_list().begin(); it != team->index_list().end(); ++it) {
		if (index == position) {
			(*it) = type;
		}
		++index;
	}
//	if (type) {
//		team->index_list().erase(position);
//	} else {
//		team->index_list().insert(position);
//	}

	{
		MSG_50100322 res_msg;
		res_msg.index_list.assign(team->index_list().begin(), team->index_list().end());
		Block_Buffer buf;
		buf.make_message(RES_TEAM_SET_POSITION_OPEN_OR_CLOSE);
		res_msg.serialize(buf);
		buf.finish_message();
		team->info_all_online_player(RES_TEAM_SET_POSITION_OPEN_OR_CLOSE, &buf);
//		THIS_SEND_TO_CLIENT(res_msg);
	}

	{
		// broadcast all
		team->refresh_teammer_info();
		Int64_Team_Map team_map;
		team_map.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
		TEAM_MANAGER->active_team_update(team->type(), player_self(), team_map, true);
	}
	return 0;
}

int Teamer::req_team_set_info(const int8_t type, const int32_t scene_id, const int32_t degree) {
	Team *team = this->get_team();
	if (NULL == team) {
		return ERROR_ROLE_NOT_HAVE_TEAM;
	}
	if (!this->is_leader()) {
		return ERROR_NOT_TEAM_LEADER;
	}
	if (team->offline_teamer_map().size() > 0) {
		return ERROR_TEAM_MEMBER_OFFLINE;
	}
	if (team->last_act_time() != Time_Value::zero) {
		team->set_last_act_time_now();
	}
	if (type < Team::TEAM_TYPE_START || type > Team::TEAM_TYPE_END) {
		return -1;
	}
	if (team->type() != type || team->scene_id() != scene_id || team->degree() != degree) {
		int ret = 0;
		if (team->type() != type) {
//			if (!team->is_not_first_team_fb() && team->type() == Team::TEAM_TYPE_FB) {
//				return ERROR_TEAM_TIMELIMIT_CAN_NOT_CHANGE_TYPE;
//			}
			std::vector<Team_Basic_Info> del_teams;
			int8_t type_old = team->type();
			{
				Team_Basic_Info team_info;
				team->get_team_basic_info(team_info);
				del_teams.push_back(team_info);
			}

			Role_Hash_Set all_member;
			team->team_all_member_set(all_member);
			for (Role_Hash_Set::const_iterator it_m = all_member.begin();
					it_m != all_member.end(); ++it_m) {
				Logic_Player *mem = LOGIC_MONITOR->find_logic_player_by_role_id(*it_m);
				if (mem) {
					int ret = mem->check_team_condition(type, scene_id);
					if (0 != ret && ERROR_LEVEL_NEED_LIMIT != ret && ERROR_TEAM_SPORTS_NOT_OPEN != ret) {
						std::string str1 = mem->role_name();
						return send_tip_to_client(ERROR_TEAM_CHANGE_TYPE, str1);
					}
					if (ERROR_LEVEL_NEED_LIMIT == ret) { // 组队副本的等级限制条件：另外发了
						return -1;
					} else if (0 != ret) {
						return ret;
					}
				} else {
					return ERROR_TEAM_MEMBER_OFFLINE;
				}
			}
			ret = TEAM_MANAGER->change_team_type(team, type, scene_id, degree);
			if (0 == ret) {
				if (type == Team::TEAM_TYPE_FB) {
					team->get_team_leader();
					team->is_not_first_team_fb() = player_self()->is_task_submited(CONFIG_CACHE_TEAM->team_config().robot_task);//this->teamer_detail_->is_not_first_team_fb;
					if (!team->is_not_first_team_fb()
							&& team->get_all_team_member_nums() == 1) {
						this->active_is_first_team_fb();
					}
					team->sync_team_info_to_scene(Team::SYNC_ADD);
				} else if (!team->is_not_first_team_fb()) {
					team->is_not_first_team_fb() = true;
					this->active_is_first_team_fb();
				}
				TEAM_MANAGER->active_listen_player(type_old, del_teams);
				Int64_Team_Map teams;
				teams.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
				TEAM_MANAGER->active_team_add(type, 0, teams, true);

				int scene_id = 0;
				if (type != Team::TEAM_TYPE_TIMELIMIT) {
					const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(type);
					if (NULL == scene_id_ptr) {
						MSG_USER("ERROR scene_type:%d", type);
						return -1;
					}
					scene_id = *scene_id_ptr;
				}
				if (scene_id != player_self()->location().scene_id) {
					player_self()->set_create_scene_args(0, Time_Value::zero, team->get_team_id(), 0, 0, 0, true);
					player_self()->inner_transmit(scene_id, 0, Coord(0, 0), 0, "");
					for (Role_Hash_Set::const_iterator it_m = all_member.begin();
							it_m != all_member.end(); ++it_m) {
						Logic_Player *mem = LOGIC_MONITOR->find_logic_player_by_role_id(*it_m);
						if (mem) {
							mem->set_create_scene_args(0, Time_Value::zero, team->get_team_id(), 0, 0, 0, true);
							mem->inner_transmit(scene_id, 0, Coord(0, 0), 0, "");
						}
					}
				}
			}
		} else if((type == Team::TEAM_TYPE_FB || type == Team::TEAM_TYPE_HERO_DREAM || type == Team::TEAM_TYPE_TIMELIMIT
				 || type == Team::TEAM_TYPE_DEFEND)
				&& (scene_id != team->scene_id() || degree != team->degree())) {
			Role_Hash_Set all_member;
			team->team_all_member_set(all_member);
			for (Role_Hash_Set::const_iterator it_m = all_member.begin();
					it_m != all_member.end(); ++it_m) {
				Logic_Player *mem = LOGIC_MONITOR->find_logic_player_by_role_id(*it_m);
				if (mem) {
					int ret = mem->check_team_condition(type, scene_id);
					if (0 != ret && ERROR_LEVEL_NEED_LIMIT != ret) {
						std::string str1 = mem->role_name();
						return send_tip_to_client(ERROR_TEAM_CHANGE_TYPE, str1);
					}
					if (ERROR_LEVEL_NEED_LIMIT == ret) { // 组队副本的等级限制条件：另外发了
						return -1;
					}
				} else {
					return ERROR_TEAM_MEMBER_OFFLINE;
				}
			}

			team->type() = type;
			team->scene_id() = scene_id;
			team->degree() = degree;
			TEAM_MANAGER->set_fb_level(team);

			Int64_Team_Map teams;
			teams.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
			TEAM_MANAGER->active_team_update(type, 0, teams, true);

		}
		if (0 == ret) {
			team->sync_team_info_to_chat(Team::SYNC_ADD);
			team->sync_team_info_to_scene(Team::SYNC_ADD);
			team->unready_all();
			team->refresh_teammer_info();
		}
	}
	MSG_50100323 res_msg;
	res_msg.scene_id = team->scene_id();
	res_msg.type = team->type();
	res_msg.degree = team->degree();
	THIS_SEND_TO_CLIENT(res_msg);
	this->profile_info();
	return 0;
}

int Teamer::req_team_lineup_info(void) {
	MSG_50100324 res_msg;
	Team *team = this->get_team();
	if (NULL != team) {
//		team->info_all_team_info();
		res_msg.info = team->lineup();
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}
	} else {
		Int_Role_Id int_role_id;
		res_msg.info.push_back(Int_Role_Id_Vec::value_type(int_role_id));
		res_msg.info.push_back(Int_Role_Id_Vec::value_type(int_role_id));
		res_msg.info.push_back(Int_Role_Id_Vec::value_type(int_role_id));
	}
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Teamer::req_team_lineup_set(const Int_Role_Id_Vec &info) {
	MSG_50100325 res_msg;
	int ret = 0;
//	bool is_change = false;
	Team *team = this->get_team();
	if (NULL != team) {
		if (team->last_act_time() != Time_Value::zero) {
			team->set_last_act_time_now();
		}
		if (!team->is_not_first_team_fb() && team->type() == Team::TEAM_TYPE_FB) {
			return ERROR_TEAM_TIMELIMIT_CAN_NOT_CHANGE_TYPE;
		}
		// 处理英雄位置
		Int_Role_Id *lineup = team->get_lineup(this->role_id());
		if (0 != lineup) {
			for (Int_Role_Id_Vec::const_iterator it = info.begin(); it != info.end(); ++it) {
				if (this->player_self()->role_id() == it->role_id && lineup->int_value != it->int_value) {
					int index = Position_Key::LINE_TWO_FRONT;
					switch(it->int_value) {
					case Team::POSITION_FRONT:
						index = Position_Key::LINE_TWO_BEHIND;
						break;
					case Team::POSITION_BEHIND:
						index = Position_Key::LINE_TWO_FRONT;
						break;
					default:
						index = Position_Key::LINE_TWO_BEHIND;
						break;
					}
					lineup->int_value = it->int_value;
					if (this->player_self()->heroer_detail().fighter_index) {
						this->player_self()->req_hero_set_formation(this->player_self()->heroer_detail().fighter_index, index);
					}
//						is_change = true;
					break;
				}
			}
		}

		if (this->is_leader()) {
			ret = team->lineup_replace(info);
		}
//		team->info_all_team_info();
		this->active_member_lineup();
		team->sync_team_info_to_scene(Team::SYNC_ADD);
	} else {
		return ERROR_TEAM_NOT_EXIST;
	}

	res_msg.info = team->lineup();
	THIS_SEND_TO_CLIENT(res_msg);
//	if (is_change) {
//		this->active_member_lineup();
//	}

	return ret;
}

int Teamer::req_open_card(int index) {
	Team* team = Teamer::get_team();
	if (NULL != team) {
//		team->set_last_act_time_now();
		return team->req_open_card(index, *player_self());
	}
	return ERROR_TEAM_NOT_EXIST;
}

int Teamer::req_team_hero_dream(void) {
	MSG_50100327 acv_msg;
	Int_Int_Set_Map task_ids;
	for (Scene_Config_Map_Cache::const_iterator it = CONFIG_CACHE_SCENE->scene_config_cache_map().begin();
			it != CONFIG_CACHE_SCENE->scene_config_cache_map().end(); ++it) {
		if (HERO_DREAM_Scene_Type == it->second.type) {
			Scene_Config::Dungeon_Config_Detail::OpenConMap::const_iterator it_ocm = it->second.dungeon.open_con_map.find(4);
			if (it_ocm != it->second.dungeon.open_con_map.end()) {
				task_ids[it_ocm->second].insert(it->second.scene_id);
			}
		}
	}
	for (Int_Int_Set_Map::const_iterator it_task = task_ids.begin(); it_task != task_ids.end(); ++it_task) {
		if (this->player_self()->is_task_submited(it_task->first)) {
			for (Int_Set::const_iterator it_s = it_task->second.begin(); it_s != it_task->second.end(); ++it_s) {
				acv_msg.scene_ids.push_back(*it_s);
			}
		}
	}
	THIS_SEND_TO_CLIENT(acv_msg);
	return 0;
}
int Teamer::req_team_follow(const int8_t follow) {
	if (0 != follow && 1 != follow) {
		return -1;
	}
	this->follow_ = follow;
	sync_teamer_detail_to_scene();
	return 0;
}
int Teamer::req_team_info(const MSG_10100330 &msg) {
	MSG_50100330 res_msg;
	Team* team = TEAM_MANAGER->get_team(msg.team_id);
	if (team) {
		res_msg.team_id = team->get_team_id();
		res_msg.exist = 1;
		res_msg.type = team->type();
		res_msg.password = team->password();
		res_msg.num = get_profile_num_by_type(res_msg.type);
		res_msg.in_battle = team->in_battle();
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Teamer::req_team_quick_invite(const MSG_10100331 &msg) {
	MSG_50100331 res_msg;
	Team* team = get_team();
	if (team) {
		Id_Amount_Vec iav;
		if (!CONFIG_CACHE_TEAM->team_config().world_invite_cost.empty()) {
			for (Item_Info_Vec::const_iterator it = CONFIG_CACHE_TEAM->team_config().world_invite_cost.begin();
					it != CONFIG_CACHE_TEAM->team_config().world_invite_cost.end(); ++it) {
				iav.push_back(Id_Amount(it->id, it->amount, Bind_Verify(it->bind)));
			}
		}
		if (0 == player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, iav)) {
			player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, iav, Lose_Item_DM_Info(ITEM_LOSE_TEAM_WORLD_INVITE));
			res_msg.result = 1;
		}
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Teamer::team_hook_enter_fb(const int scene_id, const int order) {
	Team *team = this->get_team();
	if (!team) {
		return -1;
	}
	switch(team->type()) {
	case Team::TEAM_TYPE_FB:
		break;
	case Team::TEAM_TYPE_SPORTS: {
		break;
	}
	case Team::TEAM_TYPE_HERO_DREAM:
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_HERO_DREAMS);
		break;
	case Team::TEAM_TYPE_TIMELIMIT:
		break;
	case Team::TEAM_TYPE_DEFEND:
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_DEFEND_DD);
		break;
	default:
		break;
	}
	return 0;
}
int Teamer::team_hook_finish_fb(const int scene_id, const int order) {
	player_self()->task_listen_team_scene_or_pvp();
	if (PROFILE_UNUSE == profile_set()) {
		this->team_hook_finish_fb_non_profile_set(scene_id, order);
	}
	if (PROFILE_USE == profile_set()) {
		this->team_hook_finish_fb_profile_set(scene_id, order);
	}
//	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_TEAM_EXPLORE);

	return 0;
}

int Teamer::team_hook_finish_fb_non_profile_set(const int scene_id, const int order) {
	player_self()->task_listen_help_other_complete_team_scene(1);
	return 0;
}

int Teamer::team_hook_finish_fb_profile_set(const int scene_id, const int order) {
	// do something
	return 0;
}

void Teamer::hook_fighter_force(const int new_force) {
	this->active_other_member();
}

int Teamer::create_team_timelimit(const int32_t &scene_id, int type, Time_Value time, role_id_t id,
								  int v1, int v2, int v3, bool v4 /*= false*/) {
	int ret = 0;//this->check_create_team_condition(Team::TEAM_TYPE_FB, scene_id);
//	if (0 != ret) {
//		if (ERROR_LEVEL_NEED_LIMIT == ret) {
//			ret = 0;
//		}
//		return ret;
//	}

	Team* team = TEAM_MANAGER->create_team(scene_id, "", Team::TEAM_TYPE_TIMELIMIT, 0);
	if (team) {
		// start
		team->in_battle() = 1;
		ret = team->player_join(*this->player_self(), true, true);

		if (0 == ret) {
			MSG_50100301 res_msg;
			res_msg.index = team->get_team_index();
			res_msg.password = team->password();
			res_msg.scene_id = team->scene_id();
			THIS_SEND_TO_CLIENT(res_msg);

//			Int64_Team_Map teams;
//			teams.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
//			TEAM_MANAGER->active_team_add(Team::TEAM_TYPE_FB, player_self(), teams, true);

			this->team_exit_wait_area(Team::TEAM_TYPE_TIMELIMIT);

//			Logic_Player *teamer = team->get_team_leader();
//			if (NULL == teamer) {
//				return ERROR_TEAMMER_NOT_READY;
//			}
			this->player_self()->set_create_scene_args(type, time, id, v1, v2, v3, v4);
//			this->player_self()->set_create_scene_args(0, Time_Value::zero, role_id(), 0, 0, 0, true);
//			this->player_self()->inner_transmit(team->scene_id(), 0, 0, "");
			this->player_self()->team_hook_enter_fb(team->scene_id(), 0);
//			for (Role_Hash_Set::const_iterator it = team->team_member_set().begin(); it != team->team_member_set().end(); ++it) {
//				Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
//				if (NULL == teamer) {
//					return ERROR_TEAMMER_NOT_READY;
//				}
//				this->player_self()->set_create_scene_args(0, Time_Value::zero, team->get_team_id(), 0, 0, 0, true);
//				this->player_self()->inner_transmit(team->scene_id(), 0, 0, "");
//				this->player_self()->team_hook_enter_fb(team->scene_id(), 0);
//			}

//			team->drops_all_vec().clear();
//			team->open_card_list().clear();
//			team->role_reward_map().clear();
			team->set_last_act_time_zero();
//			Int64_Team_Map teams;
//			teams.insert(Int64_Team_Map::value_type(team->get_team_id(), team));
//			TEAM_MANAGER->active_team_del(team->type(), 0, teams, true);
			TEAM_MANAGER->recycle_team_index(*team);

			this->teamer_detail_->ready = Teamer_Detail::READY;
			team->battle_over_flag() = Team::BATTLE_OVER_FLAG_UNSET;
			this->sync_teamer_detail_to_scene();
			team->info_all_team_info();
		}
		return ret;
	} else {
		return -1;
	}
	return 0;
}

int Teamer::check_create_team_condition(const int8_t type, const int32_t scene_id) {
	if (get_scene_type(this->player_self()->base_detail().location.scene_id) != Main_Scene_Type &&
			get_scene_type(this->player_self()->base_detail().location.scene_id) != Arena_Panel_Scene_Type) {
		return ERROR_SCENE_TEAM_LIMITA;
	}
	if (has_team()) {
		return ERROR_ROLE_HAVE_TEAM;
	}

	return check_team_condition(type, scene_id);
}

int Teamer::check_team_condition(const int8_t type, const int32_t scene_id) {
	switch(type) {
	case Team::TEAM_TYPE_FB:
		return this->check_fb_condition(scene_id);
		break;
	case Team::TEAM_TYPE_SPORTS:
		return this->check_sports_condition();
	case Team::TEAM_TYPE_HERO_DREAM:
		return this->check_hero_dream_condition(scene_id);
		break;
	case Team::TEAM_TYPE_TIMELIMIT:
		break;
	case Team::TEAM_TYPE_DEFEND:
		return this->check_defend_condition(scene_id);
	default:
		break;
	}

	return 0;
}

int Teamer::check_fb_condition(const int32_t scene_id) {
	const Scene_Config * scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (!scene_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Scene_Config::Dungeon_Config_Detail dungeon = scene_config->dungeon;
	Scene_Config::Dungeon_Config_Detail::OpenConMap::const_iterator open_con_it = dungeon.open_con_map.find(2);
	if (open_con_it != dungeon.open_con_map.end()) {
		if (level() < open_con_it->second) {
			this->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT, open_con_it->second);
			return -1;
		}
	}

	if (!scene_config->dungeon.can_team) {
		return ERROR_INNER_SERVER;
	}

	return 0;
}

int Teamer::check_sports_condition(void) {
	if (false == TEAM_MANAGER->team_sports_is_open()) {
		return ERROR_TEAM_SPORTS_NOT_OPEN;
	}
	const int level_limit = 20;
	if (level() < level_limit) {
		this->send_tip_to_client(ERROR_TEAM_SPORTS_LEVEL_LIMIT, level_limit);
		return -1;
	}
	{
		int8_t type = Team::TEAM_TYPE_SPORTS;
		int scene_id = 0;
		const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(type);
		if (NULL == scene_id_ptr) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		scene_id = *scene_id_ptr;
		Block_Buffer buf;
		buf.make_message(REQ_TEAM_SPORTS_TEST_CONNECT);
		buf.finish_message();
		if (0 != LOGIC_MONITOR->send_to_scene(scene_id, 0, buf)) {
			return ERROR_TEAM_SPORTS_CANNOT_CONNECT;
		}
	}
	return 0;
}

int Teamer::check_hero_dream_condition(const int32_t scene_id) {
	const Scene_Config * scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (!scene_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Scene_Config::Dungeon_Config_Detail dungeon = scene_config->dungeon;
	Scene_Config::Dungeon_Config_Detail::OpenConMap::const_iterator open_con_it = dungeon.open_con_map.find(2);
	if (open_con_it != dungeon.open_con_map.end()) {
		if (level() < open_con_it->second) {
			this->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT, open_con_it->second);
			return -1;
		}
	}
	open_con_it = dungeon.open_con_map.find(4);
	if (open_con_it != dungeon.open_con_map.end()) {
		if (!player_self()->is_task_submited(open_con_it->second)) {
			return ERROR_FB_NOT_OPEN;
		}
	}

	if (!scene_config->dungeon.can_team) {
		return ERROR_INNER_SERVER;
	}
	return 0;
}

int Teamer::check_defend_condition(const int scene_id) {
	const Scene_Config * scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (!scene_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Scene_Config::Dungeon_Config_Detail dungeon = scene_config->dungeon;
	Scene_Config::Dungeon_Config_Detail::OpenConMap::const_iterator open_con_it = dungeon.open_con_map.find(2);
	if (open_con_it != dungeon.open_con_map.end()) {
		if (level() < open_con_it->second) {
			this->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT, open_con_it->second);
			return -1;
		}
	}

	if (!scene_config->dungeon.can_team) {
		return ERROR_INNER_SERVER;
	}
	return 0;
}

int Teamer::check_join_team_condition(const role_id_t &role_id, const int64_t index, const std::string &password, const int8_t &is_world) {
	if (get_scene_type(this->player_self()->base_detail().location.scene_id) != Main_Scene_Type) {
		return ERROR_SCENE_TEAM_LIMITA;
	}

//	if (role_id == teamer_detail_->role_id) {
//		return ERROR_CLIENT_OPERATE;
//	}

	if (has_team()) {
		return ERROR_ROLE_HAVE_TEAM;
	}

	Team *team = 0;
	if (index != -1) {
		team = TEAM_MANAGER->get_team(index);
	} else {
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if (player == 0) {
			return ERROR_TEAM_LEADER_NOT_ONLINE;
		}
		team = player->get_team();
	}
	if (NULL == team) {
		return ERROR_TEAM_NOT_EXIST;
	}

//	if (is_world && (0 == team->get_team_index() || team->in_battle())) {
//		return ERROR_TEAM_NOT_EXIST;
//	}

//	if (team->in_battle()) {
//		switch(team->type()) {
//		case Team::TEAM_TYPE_FB:
//			if (this->teamer_detail_->profile_num_fb <= 0) {
//				return ERROR_TEAM_PROFILE_NUM_LIMIT;
//			}
//			break;
//		case Team::TEAM_TYPE_SPORTS:
//			if (this->profile_num_sports_all() <= 0) {
//				return ERROR_TEAM_PROFILE_NUM_LIMIT;
//			}
//			break;
//		case Team::TEAM_TYPE_HERO_DREAM:
//			if (this->teamer_detail_->profile_num_hero_dream <= 0) {
//				return ERROR_TEAM_PROFILE_NUM_LIMIT;
//			}
//			break;
//		default:
//			break;
//		}
//	}

	switch(team->type()) {
	case Team::TEAM_TYPE_FB:{
		Logic_Player *pl = team->get_team_leader();
		if (pl) {
			if (!team->is_not_first_team_fb()) {
				return ERROR_TEAM_FB_CAN_NOT_ADD_APPLICANT;
			}
		} else {
			return ERROR_TEAM_LEADER_NOT_ONLINE;
		}
		return this->check_fb_condition(team->scene_id());
	}
	case Team::TEAM_TYPE_SPORTS:
		return this->check_sports_condition();
	case Team::TEAM_TYPE_HERO_DREAM:
		return this->check_hero_dream_condition(team->scene_id());
	case Team::TEAM_TYPE_DEFEND:
		return this->check_defend_condition(team->scene_id());
	default:
		break;
	}

	return 0;
}

int Teamer::team_hook_lineup_change(const int32_t pos) {
	Team *team = this->get_team();
	if (NULL != team) {
		team->lineup_replace(*player_self());
		this->active_member_lineup();
	}

	return 0;
}

int Teamer::team_hook_heroer_change(const int32_t fight_index) {
	Team* team = this->get_team();
	if (NULL != team) {
		team->lineup_replace(*player_self());
		this->active_other_member();
		this->get_team_match_info();
//		this->sync_teamer_detail_to_chat();
		this->sync_teamer_detail_to_scene();
	}
	return 0;
}
void Teamer::hook_fighter_level(const int new_level, const int src_level) {
	Team* team = this->get_team();
	if (NULL != team) {
		this->active_other_member();
		this->get_team_match_info();
		this->sync_teamer_detail_to_chat();
		this->sync_teamer_detail_to_scene();
	}
}

int Teamer::active_other_member(void) {
	Team* team = this->get_team();
	if (NULL == team) {
		return ERROR_ROLE_NOT_HAVE_TEAM;
	}
	MSG_80100313 res_msg;
	get_online_teamer_info_from_logic(*player_self(), res_msg.teamer);
	Block_Buffer buf;
	buf.make_message(ACTIVE_TEAMER_STATUS_CHANGE);
	res_msg.serialize(buf);
	buf.finish_message();
	team->info_all_online_player(ACTIVE_TEAMER_STATUS_CHANGE, &buf);
	return 0;
}

int Teamer::active_member_lineup(void) {
	Team* team = this->get_team();
	if (NULL == team) {
		return ERROR_ROLE_NOT_HAVE_TEAM;
	}
	MSG_80100319 act_msg;
	act_msg.info = team->lineup();
	Block_Buffer buf;
	buf.make_message(act_msg.msg_id);
	act_msg.serialize(buf);
	buf.finish_message();
	team->info_all_online_player(act_msg.msg_id, &buf);
	return 0;
}

int Teamer::active_is_first_team_fb(void) {
	Team* team = this->get_team();
	if (NULL == team) {
		return ERROR_ROLE_NOT_HAVE_TEAM;
	}
	MSG_80100324 act_msg;
	act_msg.state = !player_self()->is_task_submited(CONFIG_CACHE_TEAM->team_config().robot_task);//!this->teamer_detail_->is_not_first_team_fb;
	if (team->type() != Team::TEAM_TYPE_FB) {
		act_msg.state = 0;
	}
	THIS_SEND_TO_CLIENT(act_msg);
	return 0;
}

inline bool Teamer::has_team(void) {
	if (teamer_detail_->team_id == 0) {
		return false;
	} else {
		return (NULL != TEAM_MANAGER->get_team(teamer_detail_->team_id));
	}
}

bool Teamer::is_leader(void) {
	Team *team = get_team();
	if (NULL != team) {
		return team->is_leader(teamer_detail_->role_id);
	} else {
		return false;
	}
}

Team *Teamer::get_team(void) {
	if (teamer_detail_->team_id != 0) {
		return TEAM_MANAGER->get_team(teamer_detail_->team_id);
	} else {
		return NULL;
	}
}

void Teamer::set_team_id(int64_t team_id) {
	teamer_detail_->team_id = team_id;
	teamer_detail_->ready = false;
	teamer_detail_->save_tick();
}

role_id_t Teamer::leader_id(void) {
	Team* team = this->get_team();
	if (team) {
		return team->leader_role_id();
	}
	return 0;
}

void Teamer::sync_teamer_detail_to_chat(void) {
	MSG_20100301 inner_msg;
	Block_Buffer buf;
	buf.make_message(inner_msg.msg_id);
	this->teamer_detail_->serialize(buf);
	buf.finish_message();
	this->send_to_local_chat(buf);
}

void Teamer::sync_teamer_detail_to_scene(void) {
	MSG_20100303 inner_msg;
	Block_Buffer buf;
	buf.make_message(inner_msg.msg_id);
	this->teamer_detail_->serialize(buf);
	buf.write_int8(this->follow_);
	buf.write_int32(profile_set());
	buf.finish_message();
	this->send_to_scene(buf);
}

void Teamer::sync_teamer_detail_to_scene(Block_Buffer &buf) {
	this->teamer_detail_->serialize(buf);
	buf.write_int8(this->follow_);
	buf.write_int32(profile_set());
}

void Teamer::get_team_match_info(void) {
	this->teamer_detail_->team_match_info.role_id = role_id();
	this->teamer_detail_->team_match_info.role_name = role_name();
	this->teamer_detail_->team_match_info.level = level();
	this->teamer_detail_->team_match_info.head_photo_id = this->player_self()->friend_detail().head_photo_id;
	this->teamer_detail_->team_match_info.zone = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	Hero_Info hero;
	this->player_self()->heroer_detail().get_hero_info(this->player_self()->heroer_detail().fighter_index, hero);
	this->teamer_detail_->team_match_info.hero_photo_id = hero.hero_facade_id;
}

int Teamer::team_load_info(Block_Buffer &buf) {
	int64_t team_id = 0;
	int16_t fail_times = 0;
	int16_t match_fail = 0;
	Time_Value match_fail_time;
	buf.read_int64(team_id);
	buf.read_int16(fail_times);
	buf.read_int16(match_fail);
	match_fail_time.deserialize(buf);
	Team *team = TEAM_MANAGER->get_team(team_id);
	if (team && match_fail_time != team->match_fail_time()) {
		team->match_fail_time() = match_fail_time;
		++team->match_fail();
	}
	return 0;
}

int Teamer::team_load_cumulate_award(Block_Buffer &buf) {
	/*
//		Team_Fb_Scene_Type   	= 80204, 	   /// 组队推图副本
//		HERO_DREAM_Scene_Type   = 80205, 	   /// 英雄梦境副本*/
	int scene_type = 0;
	Int_Int_Map prop;
	uint16_t len = 0;
	int key = 0;
	int value = 0;

//	Time_Value now = Time_Value::gettimeofday();
//	Date_Time dt(now);
//	dt.hour(0);
//	dt.minute(0);
//	dt.second(0);
//	dt.microsec(0);
//	now.sec(dt.time_sec());
	double exp_pr = player_self()->exp_sum() + (player_self()->get_vip_addition(1) / 100.0);

	buf.read_int32(scene_type);
	buf.read_int64(start_time_);
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int32(key);
		buf.read_int32(value);
		if (key == PT_EXP_CURRENT) {
			value = value / (1.0 + exp_pr);
		}
		if (value < 0.00000001) {
			continue;
		}
		prop.insert(std::make_pair(key, value));
	}
	uint32_t id = 0;
	switch(scene_type) {
	case Team_Fb_Scene_Type: {
		id = REWARD_BACK_PRO_TEAM_EXPLORE;
		break;
	}
	case TEAM_DEFEND_Scene_Type: {
		id = REWARD_BACK_PRO_TEAM_DEFEND;
		break;
	}
	case HERO_DREAM_Scene_Type: {
		id = REWARD_BACK_PRO_HERO_DREAMS;
		break;
	}
	case Campfire_Scene_Type: {
		id = REWARD_BACK_PRO_PARTY;
		break;
	}
	default:
		return -1;
		break;
	}
	player_self()->reward_back_listen_accumulated_rewards(id, prop, start_time_);
	return 0;
}

bool &Teamer::team_get_ready_status(void) {
	return this->teamer_detail_->ready;
}

int Teamer::team_sub_profile_num(void) {
	if (PROFILE_USE == this->profile_set()) {
		Team *team = this->get_team();
		if (team) {
			switch(team->type()) {
			case Team::TEAM_TYPE_FB: {
				if (team->has_sub_profile().count(role_id())) {
					return 0;
				}
				if (this->profile_num_fb_all() > 0) {
					this->profile_num_fb_sub();
					team->has_sub_profile().insert(role_id());
					profile_info();
					if (this->profile_num_fb_all() == 0) {
						team->profile_set_del(role_id());
						MSG_50100310 res_msg;
						res_msg.profile_set = this->profile_set();
						THIS_SEND_TO_CLIENT(res_msg);
					}
					return 0;
				}
				break;
			}
			case Team::TEAM_TYPE_SPORTS: {
				if (team->has_sub_profile().count(role_id())) {
					return 0;
				}
				if (this->profile_num_sports_all() > 0) {
					--this->profile_num_sports();
					team->has_sub_profile().insert(role_id());
					profile_info();
					if (this->profile_num_sports_all() <= 0) {
						team->profile_set_del(role_id());
						MSG_50100310 res_msg;
						res_msg.profile_set = this->profile_set();
						THIS_SEND_TO_CLIENT(res_msg);
					}
					return 0;
				}
				break;
			}
			case Team::TEAM_TYPE_HERO_DREAM: {
				if (team->has_sub_profile().count(role_id())) {
					return 0;
				}
				if (this->profile_num_hero_dream() > 0) {
					--this->profile_num_hero_dream();
					team->has_sub_profile().insert(role_id());
					profile_info();
					if (this->profile_num_hero_dream() == 0) {
						team->profile_set_del(role_id());
						MSG_50100310 res_msg;
						res_msg.profile_set = this->profile_set();
						THIS_SEND_TO_CLIENT(res_msg);
					}
					return 0;
				}
				break;
			}
			case Team::TEAM_TYPE_DEFEND: {
				if (team->has_sub_profile().count(role_id())) {
					return 0;
				}
				if (this->profile_num_defend_all() > 0) {
					this->profile_num_defend_sub();
					team->has_sub_profile().insert(role_id());
					profile_info();
					if (this->profile_num_defend_all() == 0) {
						team->profile_set_del(role_id());
						MSG_50100310 res_msg;
						res_msg.profile_set = this->profile_set();
						THIS_SEND_TO_CLIENT(res_msg);
					}
					return 0;
				}
				break;
			}
			default:
				break;
			}
		}
	}
	return -1;
}
int Teamer::get_profile_num_by_type(const int8_t type) {
	switch(type) {
	case Team::TEAM_TYPE_FB: {
		return this->profile_num_fb_all();
	}
	case Team::TEAM_TYPE_SPORTS: {
		return this->profile_num_sports_all();
	}
	case Team::TEAM_TYPE_HERO_DREAM: {
		return this->profile_num_hero_dream();
	}
	case Team::TEAM_TYPE_DEFEND: {
		return this->profile_num_defend_all();
	}
	default:
		break;
	}
	return 0;
}

int Teamer::profile_set(void) {
	Team *team = get_team();
	if (team) {
		return team->has_profile_set(role_id());
	}
	return PROFILE_UNUSE;
//	return this->profile_set_;
}

int8_t &Teamer::follow(void) {
	return this->follow_;
}

//int &Teamer::profile_num_fb(void) {
//	return this->teamer_detail_->profile_num_fb;
//}
int Teamer::profile_num_fb_all_cfg(void) {
	return CONFIG_CACHE_TEAM->team_config().profile_num_fb + this->teamer_detail_->profile_num_fb_pre;
}
int Teamer::profile_num_fb_all(void) {
	return this->teamer_detail_->profile_num_fb + this->teamer_detail_->profile_num_fb_pre;
}
int Teamer::profile_num_defend_all(void) {
	return this->teamer_detail_->profile_num_defend;
}
int Teamer::profile_num_fb_sub(const int num) {
	if (this->teamer_detail_->profile_num_fb_pre > 0) {
		--this->teamer_detail_->profile_num_fb_pre;
	} else if (this->teamer_detail_->profile_num_fb > 0) {
		--this->teamer_detail_->profile_num_fb;
	} else {
		return -1;
	}
	save_tick();
	return 0;
}
int Teamer::profile_num_defend_sub(const int num) {
	--this->teamer_detail_->profile_num_defend;
	save_tick();
	return 0;
}
int Teamer::profile_num_fb_pre_add(const int num) {
	this->teamer_detail_->profile_num_fb_pre += num;
	save_tick();
	int profile_set = 0;
	Team *team = this->get_team();
	if (team && team->has_profile_set(role_id())) {
		profile_set = 1;
	}
	change_team_setting(profile_set);
	profile_info();
	return 0;
}

int &Teamer::profile_num_sports(void) {
	return this->teamer_detail_->profile_num_sports;
}

int &Teamer::profile_num_hero_dream(void) {
	return this->teamer_detail_->profile_num_hero_dream;
}

int Teamer::profile_num_sports_all(void) {
	return this->teamer_detail_->profile_num_sports + player_self()->achievement_team_sports_times();
}
int64_t Teamer::team_start_time(void) {
	return start_time_;
}

int Teamer::team_sports_get_award(const uint8_t  result, const int team_sports_battle_level, const int score_gap) {
//	MSG_DEBUG("team_sports_battle_level: %ld, %d", role_id(), team_sports_battle_level);
	Arena_Multi_Detail *cfg = 0;
	if (result) {
		cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_win(team_sports_battle_level);
	} else {
		cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_fail(team_sports_battle_level);
	}
	if (!cfg) {
		MSG_DEBUG("error team_sports_battle_level: %d", team_sports_battle_level);
		return -1;
	}

	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	Item_Vec pack_item_vec;
	int exp = 0;
	int gold = 0;
	int friendship = 0;
	int integrate = 0;
	int merit = 0;
	//exp
	exp = cfg->exp_;
	//gold
	gold = cfg->gold_;
	//gold
	friendship = cfg->friendship_;
	integrate = cfg->integrate_ * CONFIG_CACHE_TEAM->get_correct_by_gap_and_victory(score_gap, result) /  100;
	merit = cfg->merit_;

	int ret = 0;
	Team* team = this->get_team();
	if (!team) {
		return -1;
	}
	if (Teamer::PROFILE_USE == this->profile_set() && this->profile_num_sports_all() > 0) {
		--this->profile_num_sports();
		if (this->profile_num_sports_all() <= 0) {
			team->profile_set_del(role_id());
		}
		this->profile_info();

		for (Item_Info_Vec::const_iterator it = cfg->item_vec_.begin();
				it != cfg->item_vec_.end(); ++it) {
			//package item
			Item_Vec pack_item_vec_tmp;
			make_item_vec(pack_item_vec_tmp, (*it).id, (*it).amount, (*it).bind);
			for (Item_Vec::const_iterator item_it = pack_item_vec_tmp.begin();
					item_it != pack_item_vec_tmp.end(); ++item_it) {
				pack_item_vec.push_back(*item_it);
				Item_Basic_Info ibi;
				(*item_it).set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				Card_Info card_info;
				if((*item_it).set_item_card_info(card_info)) {
					acv_msg.card_info_vec.push_back(card_info);
				}
			}

		}
		{
			Exp_All exp_all = player_self()->modify_experience_inter(exp, true, true);
			if (0 == exp_all.result) {
				acv_msg.property.push_back(Property(502503, exp_all.all));
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}
		if(gold > 0){
			int ret = player_self()->pack_add_money(Money_Add_Info(COPPER, gold, Money_DM_Info(MONEY_ADD_TEAM_SPORTS)), MONEY_OPT_REAL);
			if (0 == ret) {
				acv_msg.property.push_back(Property(COPPER, gold));
			}
		}
		if (merit > 0) {
			if (0 == player_self()->change_exploit_val(merit)) {
				acv_msg.property.push_back(Property(503003, merit));
			}
		}

		// to packege
		if(pack_item_vec.size() > 0){
			player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
						pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_TEAM_SPORTS));
		}
		team->has_sub_profile().insert(role_id());
	} else if (team->has_profile_nums()) {
		if(friendship > 0){
			int ret = player_self()->pack_add_money(Money_Add_Info(FRIENDSHIP, friendship, Money_DM_Info(MONEY_ADD_TEAM_SPORTS)), MONEY_OPT_REAL);
			if (0 == ret) {
				acv_msg.property.push_back(Property(FRIENDSHIP, friendship));
			}
		}
	}

	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	if (integrate) {
		int level_change = TEAM_MANAGER->integrage_change(role_id(), integrate);
		if (level_change) {
			team->info_all_team_info();
		}
	}

	TEAM_MANAGER->battle_result(role_id(), result, integrate, pack_item_vec);

	return ret;
}

int Teamer::get_team_award(int level) {
	bool exist = false;
	for (Int_Vec::iterator it = teamer_detail_->team_award.begin(); it != teamer_detail_->team_award.end(); ++it) {
		if (*it == level) {
			exist = true;
			break;
		}
	}

	if (!exist) {
		return ERROR_CLIENT_PARAM;
	}

	const Arena_Multi_Checkout_Detail *cfg = CONFIG_CACHE_TEAM->arena_multi_cfg_level(level);
	if (!cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	if (cfg->item_vec.empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	MSG_81000102 acv_msg;

	Item_Vec item_vec;
	for (Item_Info_Vec::const_iterator item_it = cfg->item_vec.begin();
		item_it != cfg->item_vec.end(); ++item_it) {
		item_vec.push_back(Item_Detail(item_it->id, item_it->amount, item_it->bind));
		Item_Basic_Info item_base;
		item_base.id = item_it->id;
		item_base.amount = item_it->amount;
		item_base.bind = item_it->bind;
		acv_msg.item_info.push_back(item_base);
	}

	player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_TEAM_SPORTS_CHECKOUT));

	for (Int_Vec::iterator it = teamer_detail_->team_award.begin(); it != teamer_detail_->team_award.end(); ++it) {
		if (*it == level) {
			teamer_detail_->team_award.erase(it);
			break;
		}
	}

	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	MSG_50100333 msg;
	msg.level = teamer_detail_->team_award;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

void Teamer::send_score_list_to_client(Block_Buffer &buf) {
	MSG_50100332 msg;
	buf.read_int32(msg.level);
	int list_size = 0;
	buf.read_int32(list_size);
	Team_Score_Info score_info;
	for (int i = 0; i < list_size; ++i) {
		score_info.reset();
		score_info.rank = i + 1;
		buf.read_string(score_info.role_name);
		buf.read_string(score_info.role_area);
		buf.read_int32(score_info.score);
		buf.read_int32(score_info.force);
		msg.team_score_list.push_back(score_info);
	}
	msg.award_time = TEAM_MANAGER->team_sports_start_time().sec();
	THIS_SEND_TO_CLIENT(msg);
}

void Teamer::notice_team_award(int level_id, bool notice) {
	if (level_id > 0 && teamer_detail_->team_award.size() < 10) {
		teamer_detail_->team_award.push_back(level_id);
	}

	if (level_id > 0 && notice == true) {
		MSG_80100332 msg;
		Int_Int id_level;
		id_level.val_1 = level_id;
		id_level.val_2 = level_id;
		msg.award_info.push_back(id_level);
		THIS_SEND_TO_CLIENT(msg);
	} else {
		if (notice && !teamer_detail_->team_award.empty()) {
			MSG_80100332 msg;
			int i = 0;
			for (Int_Vec::iterator l_it = teamer_detail_->team_award.begin(); l_it != teamer_detail_->team_award.end(); ++l_it, ++i) {
				Int_Int id_level;
				id_level.val_1 = *l_it;
				id_level.val_2 = *l_it;
				msg.award_info.push_back(id_level);
			}
			THIS_SEND_TO_CLIENT(msg);
		}
	}

}

void Teamer::set_no_team(void) {
	teamer_detail_->team_id = 0;
	teamer_detail_->ready = false;
	teamer_detail_->save_tick();
}

void Teamer::set_is_not_first_team_fb(const bool is_not_first_team_fb) {
	this->teamer_detail_->is_not_first_team_fb = is_not_first_team_fb;
	teamer_detail_->save_tick();
}

//int Teamer::team_client_ready_broadcast(const bool &flag) {
//	this->client_ready_broadcast_ = flag;
//	return 0;
//}
