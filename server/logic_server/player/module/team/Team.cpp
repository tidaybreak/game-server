/*
 * Team.cpp
 *
 *  Created on: 2014年4月16日
 *      Author: Linqiyou
 */

#include "Msg_Active_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Dungeon_Struct.h"
#include "Err_Code.h"
#include "Msg_Inner_Struct.h"
#include "Team_Struct.h"
#include "Team_Func.h"
#include "Team_Config.h"
#include "Config_Cache_Team.h"
#include "Team.h"
#include "Team_Manager.h"
#include "Logic_Player.h"
#include "Config_Cache.h"
#include "Record_Client.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_NPC.h"
#include "Utility_Func.h"
#include "module/team/Team_Arena_Controler.h"
#include "Config_Cache_Scene.h"
#include "helper/Helper_Def.h"
#include "Config_Cache_Dungeon.h"
#include "reward/Reward_Def.h"
#include "Scene_Func.h"
#include "Logic_Public.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Mail.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "pack/Pack_Func.h"
#include "Logic_Player_Struct.h"

static const int MIN_TEAM_PLAYER = 1;

Team::Team(): team_member_set_(CONFIG_CACHE_TEAM->team_config().max_player), offline_teamer_map_(CONFIG_CACHE_TEAM->team_config().max_player), offline_time_map_(CONFIG_CACHE_TEAM->team_config().max_player) {
	reset();
}

Team::~Team() {}

void Team::reset(void) {
	scene_id_ = 0;
	password_.clear();
	type_ = 0;
	degree_ = 0;
	words_.clear();
	index_list_.clear();
	is_all_offline_ = false;
	all_offline_time_ = Time_Value::zero;
	team_index_ = 0;
	team_id_ = 0;
	team_id_for_scene_ = 0;
	leader_role_id_ = 0;
	team_member_set_.clear();
	offline_teamer_map_.clear();
	offline_time_map_.clear();
	CLEAR_VECTOR(Int_Role_Id_Vec, lineup_);
	in_battle_ = 0;
	last_act_time_ = Time_Value::zero;
	//初始化位置索引，默认两位
	index_list_.push_back(1);
	index_list_.push_back(1);
	Int_Role_Id int_role_id;
	lineup_.push_back(Int_Role_Id_Vec::value_type(int_role_id));
	lineup_.push_back(Int_Role_Id_Vec::value_type(int_role_id));
	lineup_.push_back(Int_Role_Id_Vec::value_type(int_role_id));

	has_notice_card_ = false;
	start_open_card_time_ = Time_Value::zero;
	drops_all_vec_.clear();
	open_card_list_.clear();
	role_reward_map_.clear();

	role_id_non_strength_.clear();
	is_not_first_team_fb_ = false;
	has_sub_profile_.clear();

	used_box_.clear();
	enter_level_ = 0;
	sports_status_ = TEAM_SPORTS_NORMAL;
	point_ = 0;
	fail_times_ = 0;
	match_fail_ = 0;
	match_fail_time_ = Time_Value::zero;
	battle_over_flag_ = BATTLE_OVER_FLAG_SET;

	profile_set_.clear();
	monster_.clear();

	hurt_map_.clear();
	be_hurt_map_.clear();
	cure_map_.clear();
}

int Team::player_online(Logic_Player &player) {
	if (!is_in_team_offlinelist(player.role_id())) {
		MSG_USER("");
		return -1;
	}

	set_team_not_all_offline();

	offline_teamer_map_.erase(player.base_detail().role_id);
	offline_time_map_.erase(player.base_detail().role_id);

	if (leader_role_id_ == 0) {
		leader_role_id_ = player.teamer_detail().role_id;
	} else if (leader_role_id_ != player.teamer_detail().role_id) {
		team_member_set_.insert(player.base_detail().role_id);
	}

	this->cancel_match(); // maybe

	// 通知小队在线玩家，player上线
	MSG_80100302 online_msg;
	online_msg.leader_id = this->leader_role_id_;
	online_msg.type = this->type_;
	online_msg.profile_set = player.profile_set();
	Online_Teamer_Info teamer;
	Logic_Player *leader = this->get_team_leader();
	if (NULL != leader) {// && player.role_id() != this->leader_role_id_) {
		get_online_teamer_info_from_logic(*leader, teamer);
		online_msg.online_teamer.push_back(teamer);
	}
	for (Role_Hash_Set::const_iterator it = this->team_member_set_.begin();
			it != this->team_member_set_.end(); ++it) {
		Logic_Player *member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != member) {// && player.role_id() != *it) {
			get_online_teamer_info_from_logic(*member, teamer);
			online_msg.online_teamer.push_back(teamer);
		}
	}
	for (RoleID_OnlineTeamerInfo_Map::const_iterator it = this->offline_teamer_map_.begin();
			it != this->offline_teamer_map_.end(); ++it) {
		online_msg.online_teamer.push_back(it->second);
	}
	this->get_team_basic_info(online_msg.team_info);

	OBJ_SEND_TO_CLIENT(online_msg, player);

	{
		MSG_80100318 res_msg;
		this->get_team_basic_info(res_msg.info);
		OBJ_SEND_TO_CLIENT(res_msg, player);
	}

	MSG_80100303 res_msg;
	get_online_teamer_info_from_logic(player, res_msg.teamer);
	Block_Buffer buf;
	buf.make_message(res_msg.msg_id);
	res_msg.serialize(buf);
	buf.finish_message();
	info_all_online_player_without_self(ACTIVE_TEAMER_ONLINE, &buf, player.role_id());

	this->lineup_replace(player);
	Int64_Team_Map team_map;
	team_map.insert(Int64_Team_Map::value_type(this->get_team_id(), this));
	TEAM_MANAGER->active_team_update(this->type(), 0, team_map, false);

//	team_status_change(ONLINE);

	this->sync_team_info_to_chat(Team::SYNC_ADD);
	player.sync_teamer_detail_to_chat();
	this->sync_team_info_to_scene(Team::SYNC_ADD);
	player.sync_teamer_detail_to_scene();
	return 0;
}

int Team::player_offline(Logic_Player &player) {
	if (!is_in_team_onlinelist(player.base_detail().role_id)) {
		MSG_USER("");
		return -1;
	}
	team_member_set_.erase(player.teamer_detail().role_id);
	this->cancel_match();

	Logic_Player *leader = this->get_team_leader();
	if (NULL == leader && this->team_member_set_.empty()) {
		set_team_all_offline();
	}
	Online_Teamer_Info online_info;
	get_online_teamer_info_from_logic(player, online_info);
	online_info.online = false;
	offline_teamer_map_.insert(std::make_pair(player.teamer_detail().role_id, online_info));
	offline_time_map_.insert(std::make_pair(player.teamer_detail().role_id, Time_Value::gettimeofday()));

	MSG_80100304 msg;
	msg.role_id = player.base_detail().role_id;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	info_all_online_player(ACTIVE_TEAMER_OFFLINE, &buf);

	this->lineup_clear(player);

	this->sync_team_info_to_chat(Team::SYNC_ADD);
	player.sync_teamer_detail_to_chat();
	this->sync_team_info_to_scene(Team::SYNC_ADD);
	player.sync_teamer_detail_to_scene();

	return 0;
}

int Team::player_join(Logic_Player &player, const bool is_auto_profile_set, const bool is_timelimit) {
	if (get_all_team_member_nums() >= CONFIG_CACHE_TEAM->team_config().max_player || this->get_all_team_member_nums() >= this->get_position_max()) {
		return ERROR_TEAM_ROLE_FULL;
	}
	has_sub_profile().erase(player.role_id());

	role_id_t arg3 = team_id_;
	int scene_id_lead = 0;
	int scene_id = 0;
	Coord coord;
	if (NOT_TIMELIMIT == is_timelimit) { // 分开是为了检测配置文件（这样处理，以防外网时，配置不正确异常）1
		if (type_ != TEAM_TYPE_TIMELIMIT) {
			const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(type_);
			if (NULL == scene_id_ptr) {
				MSG_USER("ERROR scene_type:%d", type_);
				return -1;
			}
			scene_id = *scene_id_ptr;
		} else {
			arg3 = this->leader_role_id_;
		}
	}

	this->cancel_match(); // maybe
	player.set_team_id(this->team_id_);
	if (0 == leader_role_id_) {	// 新创建的队伍，加入的玩家就是队长
		this->leader_role_id_ = player.base_detail().role_id;
		this->set_sports_level();
		if (type_ == TEAM_TYPE_FB) {
			this->is_not_first_team_fb_ = player.is_task_submited(CONFIG_CACHE_TEAM->team_config().robot_task);//player.teamer_detail().is_not_first_team_fb;
		}
	} else {
		MSG_80100305 msg;
		get_online_teamer_info_from_logic(player, msg.teamer);
		Block_Buffer buf;
		buf.make_message(ACTIVE_TEAMER_JOIN);
		msg.serialize(buf);
		buf.finish_message();
		info_all_online_player(ACTIVE_TEAMER_JOIN, &buf);
		team_member_set_.insert(player.base_detail().role_id);
		this->set_sports_level();

		Logic_Player *leader = this->get_team_leader();
		if (NULL != leader) { // 传到队长附近处理
			scene_id_lead = leader->base_detail().location.scene_id;
			coord = leader->base_detail().location.coord;
			const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id_lead);
			if(scene_config == NULL){
//				MSG_USER("cannot find dungeon info, id=%d", scene_id_lead);
				scene_id_lead = 0;
				coord.reset();
			} else {
//				Scene_Type scene_type = get_scene_type(scene_id_lead);
//				if (scene_type == Scene_Type_Null || scene_type == Main_Scene_Type) {
//					scene_id_lead = 0;
//					coord.reset();
//				}
				int map_value = CONFIG_CACHE_SCENE->get_map_value(scene_config->map_id, coord);
				if (map_value <= 0) {
					coord.reset();
				}
				if (Scene_Team != scene_config->manager
						&& (Scene_Task_Manager != scene_config->manager
							&& Scene_Team_Defend != scene_config->manager)
							&& Scene_Killing != scene_config->manager) {
					scene_id_lead = 0;
					coord.reset();
				}
			}
		}
	}
	if (is_auto_profile_set) {
		switch(type_) {
		case TEAM_TYPE_FB: {
			if (player.profile_num_fb_all() > 0) {
				profile_set_add(player.role_id());
			} else {
				profile_set_del(player.role_id());
			}
			break;
		}
		case TEAM_TYPE_SPORTS: {
			if (player.profile_num_sports_all() > 0) {
				profile_set_add(player.role_id());
			} else {
				profile_set_del(player.role_id());
			}
			break;
		}
		case TEAM_TYPE_HERO_DREAM: {
			if (player.profile_num_hero_dream() > 0) {
				profile_set_add(player.role_id());
			} else {
				profile_set_del(player.role_id());
			}
			break;
		}
		case TEAM_TYPE_DEFEND: {
			if (player.profile_num_defend_all() > 0) {
				profile_set_add(player.role_id());
			} else {
				profile_set_del(player.role_id());
			}
			break;
		}
		default:
			break;
		}
	} else {
		profile_set_del(player.role_id());//player.profile_set() = Teamer::PROFILE_UNUSE;
	}
	this->lineup_replace(player);
	TEAM_MANAGER->cancel_wait(player, player.teamer_detail().wait_area_type);
	if (NOT_TIMELIMIT == is_timelimit) { // 限时击杀：将创建队伍、加入队伍合在一起，优化广播数
		info_all_team_info(player);
	}

	this->refresh_teammer_info();

	if (NOT_TIMELIMIT == is_timelimit) {// 分开是为了检测配置文件（这样处理，以防外网时，配置不正确异常）2
//		if (type_ == TEAM_TYPE_FB
//				|| type_ == TEAM_TYPE_HERO_DREAM
//				|| type_ == TEAM_TYPE_DEFEND) {
//			if (0 != scene_id_lead && scene_id_lead != scene_id) {
//				scene_id = scene_id_lead;//
//				arg3 = team_id_for_scene_;
//			}
//		}
		if (!(type_ == TEAM_TYPE_FB
				&& this->in_battle_ == 0)) {
			if (0 != scene_id_lead && scene_id_lead != scene_id) {
				scene_id = scene_id_lead;//
			}
		} else {
			coord.reset();
		}
		const Scene_Config *des_scene_config =
					CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
		if (!des_scene_config) {
			return ERROR_SCENE_NOT_EXIST;
		}
		if (des_scene_config->type != Team_Scene_Type
				&& des_scene_config->type != Task_Scene_Type) {
			arg3 = team_id_for_scene_;
		}
		player.set_create_scene_args(0, Time_Value::zero, arg3, 0, 0, 0, true);
		player.inner_transmit(scene_id, 0, coord, arg3, "");
	}

	player.get_team_match_info();
	this->sync_team_info_to_chat(Team::SYNC_ADD);
	player.sync_teamer_detail_to_chat();
	this->sync_team_info_to_scene(Team::SYNC_ADD);
	player.sync_teamer_detail_to_scene();

	return 0;
}

int Team::player_leave(role_id_t role_id, int8_t is_kick) {
	if (!is_in_team(role_id)) {
		return -1;
	}

	if (leader_role_id_ == role_id) {
		if (!team_member_set_.empty()) {
			//rand
			Role_Hash_Set::iterator begin = this->team_member_set_.begin();
			uint32_t index = std::rand()%this->team_member_set_.size();
			std::advance(begin, index);
			Logic_Player *lvl_hightest = LOGIC_MONITOR->find_logic_player_by_role_id(*begin);
			if (NULL == lvl_hightest) {
				MSG_USER("online: team_member:%d, offline_num:%d, online_num:%d, leader_id:%ld;",
						this->team_member_set_.size(), this->get_offline_team_member_nums(),
						this->get_online_team_member_nums(), this->leader_role_id_);

				if (this->team_member_set_.size() >= 2) {
					for (Role_Hash_Set::iterator it = ++team_member_set_.begin(); it != team_member_set_.end(); ++it) {
						if (it != begin) {
							lvl_hightest = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
							if (NULL == lvl_hightest) {
								MSG_USER("online2: team_member:%d, offline_num:%d, online_num:%d, leader_id:%ld;",
										this->team_member_set_.size(), this->get_offline_team_member_nums(),
										this->get_online_team_member_nums(), this->leader_role_id_);
								return -1;//impossible
							}
							break;//only 2
						}
					}
				}
			}
			if (NULL != lvl_hightest) {
				this->player_lead(*lvl_hightest, true);
			} else {
				MSG_USER("online3: team_member:%d, offline_num:%d, online_num:%d, leader_id:%ld;",
						this->team_member_set_.size(), this->get_offline_team_member_nums(),
						this->get_online_team_member_nums(), this->leader_role_id_);
			}
		} else { // 小队无人在线了，若有招人，取消招人
			Logic_Player *team_leader = get_team_leader();
			if (NULL != team_leader) {
				team_leader->req_set_ready_state(false);
				team_leader->set_no_team();
//				team_leader->sync_teamer_detail_to_map();
			}
			if (this->get_offline_team_member_nums() > 0) {
				if (!this->is_all_offline_) {
					this->set_team_all_offline();
				}
				if (this->is_in_team_offlinelist(role_id) && 1 == this->get_offline_team_member_nums()) {
					team_dismiss();
					return 0;
				}
				bool is_handle = false;
				for (RoleID_OnlineTeamerInfo_Map::const_iterator it = this->offline_teamer_map_.begin();
						it != this->offline_teamer_map_.end(); ++it) {
					if (role_id != it->first) {
						this->player_lead(it->first);
						is_handle = true;
						break;
					}
				}
				if (!is_handle) {
					MSG_USER("offline: team_member:%d, offline_num:%d, online_num:%d, leader_id:%ld;",
						this->team_member_set_.size(), this->get_offline_team_member_nums(),
						this->get_online_team_member_nums(), this->leader_role_id_);
				}
			} else {
				team_dismiss();
				return 0;
			}
		}
	}
	this->cancel_match();

	MSG_80100306 msg;
	msg.role_id = role_id;
	msg.is_kick = is_kick;
	Block_Buffer buf;
	buf.make_message(ACTIVE_TEAMMER_LEAVE);
	msg.serialize(buf);
	buf.finish_message();
	info_all_online_player(ACTIVE_TEAMMER_LEAVE, &buf);

	Role_Hash_Set::iterator it = team_member_set_.find(role_id);
	if (it != team_member_set_.end()) {
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			team_player->set_no_team();
//			team_player->sync_teamer_detail_to_map();
			int scene_id = 0;
			team_player->set_create_scene_args(0, Time_Value::zero, 0, 0, 0, 0, true);
			team_player->inner_transmit(scene_id, 0, 0, "");
			Int64_Team_Map team_map;
			team_map.insert(Int64_Team_Map::value_type(this->get_team_id(), this));
			TEAM_MANAGER->active_team_update(this->type(), team_player, team_map, false);
		}
		team_member_set_.erase(role_id);
	} else {
		RoleID_OnlineTeamerInfo_Map::iterator it_offline = this->offline_teamer_map_.find(role_id);
		if (it_offline != this->offline_teamer_map_.end()) {
			this->offline_teamer_map_.erase(it_offline);
		}
		RoleID_TimeValue_Map::iterator it_time = this->offline_time_map_.find(role_id);
		if (it_time != this->offline_time_map_.end()) {
			this->offline_time_map_.erase(it_time);
		}
	}
	has_sub_profile().erase(role_id);
//	this->info_all_team_info();
	this->refresh_teammer_info();
	if (this->get_all_team_member_nums() == 1) {
		Logic_Player *le = this->get_team_leader();
		if (le) {
			le->active_is_first_team_fb();
		}
	}

	Int64_Team_Map team_map;
	team_map.insert(Int64_Team_Map::value_type(this->get_team_id(), this));
	TEAM_MANAGER->active_team_update(this->type(), 0, team_map, true);

//	team_status_change(LEAVE);

//	update_team_hiring();
	this->sync_team_info_to_chat(Team::SYNC_ADD);
	this->sync_team_info_to_scene(Team::SYNC_ADD);
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (NULL != player) {
		player->set_no_team();
		OBJ_SEND_TO_CLIENT(msg, (*player));
		player->sync_teamer_detail_to_chat();
		player->sync_teamer_detail_to_scene();
	}
	this->lineup_refresh();

	return 0;
}

int Team::start(void) {
	int ret = 0;
	switch(this->type_) {
	case TEAM_TYPE_FB:
		ret = this->enter_fb();
		break;
	case TEAM_TYPE_SPORTS: {
		ret = this->begin_match();
		break;
	}
	case TEAM_TYPE_HERO_DREAM:
		ret = this->enter_fb();
		break;
	case TEAM_TYPE_TIMELIMIT:
		break;
	case TEAM_TYPE_DEFEND: {
		ret = this->enter_fb();
		break;
	}
	default:
		break;
	}
	if (0 == ret) {
		has_sub_profile().clear();
		this->set_last_act_time_zero();
		Int64_Team_Map teams;
		teams.insert(Int64_Team_Map::value_type(this->team_id_, this));
		TEAM_MANAGER->active_team_del(this->type(), 0, teams, true);
		TEAM_MANAGER->recycle_team_index(*this);
		this->sync_team_info_to_chat(Team::SYNC_ADD);
		this->sync_team_info_to_scene(Team::SYNC_ADD);
	}
	return 0;
}

int Team::enter_fb(void) {
	Logic_Player *teamer = this->get_team_leader();
	if (NULL == teamer) {
		return ERROR_TEAMMER_NOT_READY;
	}
	int tp = 0;
	if (type_ == TEAM_TYPE_FB) {
		tp = 4;
	} else if (type_ == TEAM_TYPE_HERO_DREAM) {
		tp = 6;
	} else if (type_ == TEAM_TYPE_DEFEND) {
//		tp = 6;
	}
	teamer->set_create_scene_args(0, Time_Value::zero, this->team_id_for_scene_, 0, 0, 0, true);
	teamer->inner_transmit(this->scene_id_, 0, 0, "");
	teamer->team_hook_enter_fb(this->scene_id_, 0);
	if (0 != tp) {
		MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`)" " VALUES (%d,%ld,%ld,%d)",
				tp,  teamer->role_id(), Time_Value::gettimeofday().sec(), teamer->level());
	}
	for (Role_Hash_Set::const_iterator it = this->team_member_set_.begin(); it != this->team_member_set_.end(); ++it) {
		Logic_Player *teamer = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL == teamer) {
			return ERROR_TEAMMER_NOT_READY;
		}
		teamer->set_create_scene_args(0, Time_Value::zero, this->team_id_for_scene_, 0, 0, 0, true);
		teamer->inner_transmit(this->scene_id_, 0, 0, "");
		teamer->team_hook_enter_fb(this->scene_id_, 0);
		teamer->sync_teamer_detail_to_scene();
		if (0 != tp) {
			MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`)" " VALUES (%d,%ld,%ld,%d)",
					tp,  teamer->role_id(), Time_Value::gettimeofday().sec(), teamer->level());
		}
	}

	this->drops_all_vec_.clear();
	this->open_card_list_.clear();
	this->role_reward_map_.clear();
	this->used_box_.clear();
	return 0;
}

int Team::begin_match(void) {
	this->sports_status_ = TEAM_SPORTS_SIGNED;
	return 0;
}

int Team::cancel_match(void) {
	if (TEAM_SPORTS_SIGNED == this->sports_status_) {
		if (0 == this->get_team_index()) {
			TEAM_MANAGER->set_team_index(this);
			Int64_Team_Map teams;
			teams.insert(Int64_Team_Map::value_type(this->get_team_id(), this));
			TEAM_MANAGER->active_team_add(this->type(), 0, teams, true);
		}
		this->sports_status_ = TEAM_SPORTS_NORMAL;
		this->unready_leader();
	}
	return 0;
}

Team::Team_Sports_Status &Team::sports_status(void) {
	return this->sports_status_;
}

int Team::get_out_index(Logic_Player &player) {
	int player_idx = POSITION_FRONT;
	int outer_index = player.heroer_get_outer_hero_formation();
	switch(outer_index) {
	case Position_Key::LINE_TWO_FRONT:
		player_idx = POSITION_BEHIND;
		break;
	case Position_Key::LINE_TWO_BEHIND:
		player_idx = POSITION_FRONT;
		break;
	default:
		player_idx = POSITION_FRONT;
		break;
	}
	return player_idx;
}

int Team::lineup_replace(Logic_Player &player) {
	Int_Role_Id int_role_id;
	int_role_id.role_id = player.role_id();
	int_role_id.int_value = this->get_out_index(player);
	Int_Role_Id_Vec lineup(this->lineup_);
	if (this->is_lineup_empty()) {
		this->lineup_.clear();
		int i = 0;
		for (Int_Role_Id_Vec::iterator it = lineup.begin(); it != lineup.end(); ++it) {
			if(POSITION_2 == i) {// 初始第一个进入阵型的肯定是队长
				this->lineup_.push_back(int_role_id);
			} else {
				this->lineup_.push_back(*it);
			}
			++i;
		}
	} else if (this->is_role_in_lineup(int_role_id.role_id)) {
		this->lineup_.clear();
		for (Int_Role_Id_Vec::iterator it = lineup.begin(); it != lineup.end(); ++it) {
			if (it->role_id == int_role_id.role_id) {
				this->lineup_.push_back(int_role_id);
			} else {
				this->lineup_.push_back(*it);
			}
		}
	}else {
		this->lineup_.clear();
		bool has_insert = false;
		for (Int_Role_Id_Vec::iterator it = lineup.begin(); it != lineup.end(); ++it) {
			if (it->role_id != 0) {
				if (it->role_id == int_role_id.role_id) {
					this->lineup_.push_back(int_role_id);// impossible
					break;
				} else {
					this->lineup_.push_back(*it);
				}
			} else {
				if (has_insert) {
					this->lineup_.push_back(*it);
				} else {
					this->lineup_.push_back(int_role_id);
					has_insert = true;
				}
			}
		}
	}
	this->sync_team_info_to_scene(Team::SYNC_ADD);

	MSG_80100319 act_msg;
	act_msg.info = this->lineup_;
	Block_Buffer buf;
	buf.make_message(act_msg.msg_id);
	act_msg.serialize(buf);
	buf.finish_message();
	this->info_all_online_player(act_msg.msg_id, &buf);
	return 0;
}

int Team::lineup_replace(const Int_Role_Id_Vec &lineup) {
	if (lineup_check(lineup)) {
		this->lineup_.assign(lineup.begin(), lineup.end());
	} else {
		return ERROR_TEAM_LINEUP_INCORRECT;
	}
	return 0;
}

int Team::lineup_clear(Logic_Player &player) {
	Int_Role_Id int_role_id;
	int_role_id.role_id = 0;
	int_role_id.int_value = 0;
	Int_Role_Id_Vec lineup(this->lineup_);
	if (this->is_role_in_lineup(player.role_id())) { // impossile
		this->lineup_.clear();
		for (Int_Role_Id_Vec::iterator it = lineup.begin(); it != lineup.end(); ++it) {
			if (it->role_id == player.role_id()) {
				this->lineup_.push_back(int_role_id);
			} else {
				this->lineup_.push_back(*it);
			}
		}
	}

	this->lineup_active();
	return 0;
}

bool Team::lineup_check(const Int_Role_Id_Vec &lineup) {
	if (lineup.size() != 3) {
		return false;
	}
	for (Int_Role_Id_Vec::const_iterator it = lineup.begin(); it != lineup.end(); ++it) {
		if (it->role_id != 0 && (!this->is_in_team(it->role_id) || it->int_value < POSITION_START || it->int_value > POSITION_END)) {
			return false;
		}
	}
	return true;
}

bool Team::is_role_in_lineup(role_id_t role_id) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (role_id == it->role_id) {
			return true;
		}
	}
	return false;
}

bool Team::is_lineup_empty(void) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (it->role_id) {
			return false;
		}
	}
	return true;
}

Int_Role_Id *Team::get_lineup(role_id_t role_id) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (it->role_id == role_id) {
			return &(*it);
		}
	}
	return 0;
}

int Team::get_position(const role_id_t &role_id) {
	int i = Position_Key::LINE_ONE_FRONT;
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (it->role_id == role_id) {
			if (POSITION_FRONT != it->int_value) {
				i += 3;
			}
			return i;
		}
		++i;
	}
	return 0;
}

int Team::lineup_refresh(void) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (!this->is_in_team(it->role_id)) {
			it->role_id = 0;
			it->int_value = 0;
		}
	}
	this->lineup_active();
	return 0;
}

int Team::lineup_active(void) {
	MSG_80100319 act_msg;
	act_msg.info = this->lineup_;
	Block_Buffer buf;
	buf.make_message(act_msg.msg_id);
	act_msg.serialize(buf);
	buf.finish_message();
	this->info_all_online_player(act_msg.msg_id, &buf);
	return 0;
}

int Team::lineup_active(const role_id_t &role_id) {
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (!player) {
		return ERROR_TEAMER_OFFLINE;
	}
	MSG_80100319 act_msg;
	act_msg.info = this->lineup_;
	OBJ_SEND_TO_CLIENT(act_msg, (*player));
	return 0;
}

int Team::team_dismiss(void) {
	Int64_Team_Map teams;
	teams.insert(Int64_Team_Map::value_type(this->team_id_, this));
	TEAM_MANAGER->active_team_del(this->type(), 0, teams, true);
	MSG_80100307 act_msg;
	Block_Buffer buf;
	buf.make_message(act_msg.msg_id);
	act_msg.serialize(buf);
	buf.finish_message();
	this->info_all_online_player(act_msg.msg_id, &buf);

	const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(this->type_);
	int scene_id = 0;
	if (NULL != scene_id_ptr) {
		scene_id = *scene_id_ptr;
	}

	Logic_Player *team_leader = get_team_leader();
	if (NULL != team_leader) {
		team_leader->set_no_team();
		team_leader->sync_teamer_detail_to_chat();
		team_leader->sync_teamer_detail_to_scene();

		Scene_Type scene_type = get_scene_type(team_leader->location().scene_id);

		if (scene_type == Team_Scene_Type || scene_type == Team_Fb_Scene_Type
				|| scene_type == HERO_DREAM_Scene_Type
				|| scene_type == Task_Scene_Type
				|| scene_type == TEAM_DEFEND_Scene_Type) {
			int scene_bak = 0;
			team_leader->inner_transmit(scene_bak, 0, 0, "");
		}
	}
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			team_player->set_no_team();
			team_player->sync_teamer_detail_to_chat();
			team_player->sync_teamer_detail_to_scene();

			if (0 != scene_id && team_player->location().scene_id == scene_id) {
				int scene_bak = 0;
				team_player->inner_transmit(scene_bak, 0, 0, "");
			}
		}
	}

	this->sync_team_info_to_chat(Team::SYNC_DEL);
	this->sync_team_info_to_scene(Team::SYNC_DEL);
	leader_role_id_ = 0;

	return 0;
}

int Team::team_dismiss_without_leader(void) {
	Int64_Team_Map teams;
	teams.insert(Int64_Team_Map::value_type(this->team_id_, this));
	TEAM_MANAGER->active_team_del(this->type(), 0, teams, true);
	MSG_80100307 act_msg;
	Block_Buffer buf;
	buf.make_message(act_msg.msg_id);
	act_msg.serialize(buf);
	buf.finish_message();
	this->info_all_online_player(act_msg.msg_id, &buf);

	const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(this->type_);
	int scene_id = 0;
	if (NULL != scene_id_ptr) {
		scene_id = *scene_id_ptr;
	}
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			team_player->set_no_team();
			team_player->sync_teamer_detail_to_chat();
			team_player->sync_teamer_detail_to_scene();

			if (0 != scene_id && team_player->location().scene_id == scene_id) {
				int scene_bak = 0;
				team_player->inner_transmit(scene_bak, 0, 0, "");
			}
		}
	}

	this->sync_team_info_to_chat(Team::SYNC_DEL);
	this->sync_team_info_to_scene(Team::SYNC_DEL);
	leader_role_id_ = 0;

	return 0;
}

int Team::player_lead(Logic_Player &player, const bool is_leader_leave) {
	if (!is_in_team_onlinelist(player.base_detail().role_id)) {
		return ERROR_PLAYER_NOT_ONLINE;
	}

	MSG_80100312 msg;
	msg.old_leader = leader_role_id_;
	msg.new_leader = player.role_id();

	team_member_set_.insert(msg.old_leader);
	leader_role_id_ = msg.new_leader;
	team_member_set_.erase(msg.new_leader);

	Block_Buffer buf;
	buf.make_message(ACTIVE_TEAM_LEADER_CHANGE);
	msg.serialize(buf);
	buf.finish_message();

	if (!is_leader_leave) {
		info_all_online_player(ACTIVE_TEAM_LEADER_CHANGE, &buf);
	} else {
		info_all_online_player_without_self(ACTIVE_TEAM_LEADER_CHANGE, &buf, msg.old_leader);
	}
	player.req_set_ready_state(Teamer_Detail::UNREADY);

//	update_team_hiring();

//	team_status_change(LEADER_CHANGE);

	return 0;
}

int Team::player_lead(role_id_t role_id) {
	if( this->is_in_team(role_id) && !this->is_leader(role_id)) {
		Logic_Player * leader = this->get_team_leader();
		if (NULL != leader) {
			leader->set_no_team();
		}
		this->leader_role_id_ = role_id;
	}
	return 0;
}

bool Team::is_team_all_offline(void) {
	if (this->offline_teamer_map().empty() && this->team_member_set().empty() && NULL == this->get_team_leader()) {
		return true;
	}
	return false;
}

int Team::set_team_all_offline(void) {
	is_all_offline_ = true;
	all_offline_time_ = Time_Value::gettimeofday();
	return 0;
}

int Team::set_team_not_all_offline(void) {
	is_all_offline_ = false;
	return 0;
}

void Team::refresh_teammer_info(void) {
	MSG_80100318 res_msg;
	this->get_team_basic_info(res_msg.info);
	Block_Buffer buf;
	buf.make_message(ACTIVE_TEAM_SET);
	res_msg.serialize(buf);
	buf.finish_message();
	this->info_all_online_player(ACTIVE_TEAM_SET, &buf);
}

void Team::refresh_teammer_info(Logic_Player &player) {
	MSG_80100318 res_msg;
	this->get_team_basic_info(res_msg.info);
	OBJ_SEND_TO_CLIENT(res_msg, player);
}

void Team::info_all_team_info(Logic_Player &player) {
	MSG_80100302 team_info_msg;
	this->get_team_basic_info(team_info_msg.team_info);
	Online_Teamer_Info online_teamer_info;
	team_info_msg.leader_id = leader_role_id_;
	team_info_msg.type = this->type_;
	team_info_msg.profile_set = player.profile_set();
	Logic_Player *team_leader = get_team_leader();
	if (NULL != team_leader) {
		get_online_teamer_info_from_logic(*team_leader, online_teamer_info);
	}
	team_info_msg.online_teamer.push_back(online_teamer_info);
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		online_teamer_info.reset();
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			get_online_teamer_info_from_logic(*team_player, online_teamer_info);
		} else {
			MSG_USER("");
		}
		team_info_msg.online_teamer.push_back(online_teamer_info);
	}

	if (!offline_teamer_map_.empty()) {
		for (RoleID_OnlineTeamerInfo_Map::iterator it = offline_teamer_map_.begin(); it != offline_teamer_map_.end();
				++it) {
			team_info_msg.online_teamer.push_back(it->second);
		}
	}

	OBJ_SEND_TO_CLIENT(team_info_msg, player);

}

void Team::info_all_team_info(void) {
	MSG_80100302 team_info_msg;
	this->get_team_basic_info(team_info_msg.team_info);
	Online_Teamer_Info online_teamer_info;
	team_info_msg.leader_id = leader_role_id_;
	team_info_msg.type = type_;
	Logic_Player *team_leader = get_team_leader();
	if (NULL != team_leader) {
		get_online_teamer_info_from_logic(*team_leader, online_teamer_info);
	}
	team_info_msg.online_teamer.push_back(online_teamer_info);
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		online_teamer_info.reset();
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			get_online_teamer_info_from_logic(*team_player, online_teamer_info);
		} else {
			MSG_USER("");
		}
		team_info_msg.online_teamer.push_back(online_teamer_info);
	}

	if (!offline_teamer_map_.empty()) {
		for (RoleID_OnlineTeamerInfo_Map::iterator it = offline_teamer_map_.begin(); it != offline_teamer_map_.end();
				++it) {
			team_info_msg.online_teamer.push_back(it->second);
		}
	}

	Logic_Player *leader = this->get_team_leader();
	if (NULL != leader) {
		team_info_msg.profile_set = leader->profile_set();
		OBJ_SEND_TO_CLIENT(team_info_msg, (*leader));
	}
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		online_teamer_info.reset();
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			team_info_msg.profile_set = team_player->profile_set();
			OBJ_SEND_TO_CLIENT(team_info_msg, (*team_player));
		}
	}

}

//int Team::sub_online_profile_num(void) {
//	Logic_Player *leader = this->get_team_leader();
//	if (leader) {
//		switch(type_) {
//		case TEAM_TYPE_FB:
//			if (leader->profile_num_fb_all() > 0) {
//				leader->profile_num_fb_sub();
//			}
//			break;
//		case TEAM_TYPE_SPORTS:
//			if (leader->profile_num_sports_all() > 0) {
//				--leader->profile_num_sports();
//			}
//			break;
//		default:
//			break;
//		}
//	}
//	for (Role_Hash_Set::const_iterator it = this->team_member_set_.begin();
//			it != this->team_member_set_.end(); ++it) {
//		Logic_Player *member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
//		if (member) {
//			switch(type_) {
//			case TEAM_TYPE_FB:
//				if (member->profile_num_fb_all() > 0) {
//					member->profile_num_fb_sub();
//				}
//				break;
//			case TEAM_TYPE_SPORTS:
//				if (member->profile_num_sports() > 0) {
//					--member->profile_num_sports();
//				}
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	return 0;
//}

void Team::info_leader(int msg_id, Block_Buffer* buf) {
	Logic_Player *team_leader = get_team_leader();
	if (NULL != team_leader) {
		team_leader->send_to_client(*buf);
	}
}

void Team::info_member(int msg_id, Block_Buffer* buf) {
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			team_player->send_to_client(*buf);
		}
	}
}

void Team::info_member_without_self(int msg_id, Block_Buffer* buf, role_id_t role_id) {
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		if (*it == role_id) continue;
		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (NULL != team_player) {
			team_player->send_to_client(*buf);
		}
	}
}

Logic_Player *Team::team_leader(void) const {
	if (0 == leader_role_id_) {
		return NULL;
	} else {
		return LOGIC_MONITOR->find_logic_player_by_role_id(leader_role_id_);
	}
}

//void Team::team_status_change(int type) {
//	Logic_Player *team_leader = get_team_leader();
//	if (NULL != team_leader) {
////		team_leader->sync_teamer_detail_to_map();
//	}
//	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
//		Logic_Player *team_player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
//		if (NULL != team_player) {
////			team_player->sync_teamer_detail_to_map();
//		}
//	}
//
//	if (type != ONLINE) {
////		LOGIC_COMMON_INSTANCE->handle_team_vip_buf_change(this);
//	}
//}

Logic_Player *Team::get_team_leader(void) {
	if (0 != leader_role_id_) {
		return LOGIC_MONITOR->find_logic_player_by_role_id(leader_role_id_);
	} else {
		return NULL;
	}
}

bool Team::is_leader_online(void) const {
	if (0 != leader_role_id_) {
		return (0 != LOGIC_MONITOR->find_logic_player_by_role_id(leader_role_id_));
	} else {
		return false;
	}
}

int Team::check_all_member_ready(void) {
	uint32_t member_min_limit = MIN_TEAM_PLAYER;//CONFIG_INSTANCE->island_config()["min_member"].asUInt();
//	if (member_min_limit < 1) {
//		member_min_limit = 1;
//	}
	if (team_member_set_.size() < member_min_limit - 1) {
		return ERROR_MEMBER_NOT_ENOUGH;
	}

	int scene_id = 0;
	Logic_Player *leader = this->get_team_leader();
	if (leader) {
		scene_id = leader->location().scene_id;
	} else {
		return ERROR_TEAMMER_NOT_READY;
	}

	MSG_80100317 msg;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	for (Role_Hash_Set::const_iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (player) {
			if (!player->team_get_ready_status()) {
				msg.name = player->base_detail().role_name;
				msg.serialize(buf);
				buf.finish_message();
				this->info_all_online_player(ACTIVE_TEAMMER_UNREADY, &buf);
				return ERROR_TEAMMER_NOT_READY;
			} else if (scene_id != player->location().scene_id) {
				return ERROR_TEAMER_NOT_NEARBY;
			}
		} else {
			return ERROR_TEAMMER_NOT_READY;
		}
	}
	if (NULL == this->get_team_leader()) {
		return  ERROR_TEAMMER_NOT_READY;
	}

//	msg.name.clear();
//	msg.serialize(buf);
//	buf.finish_message();
//	this->info_all_online_player(ACTIVE_TEAMMER_UNREADY, 0, &buf);
	return 0;
}

int Team::unready_all(void) {
	Logic_Player *leader = this->get_team_leader();
	if (leader) {
		leader->req_set_ready_state(false);
	}
	for (Role_Hash_Set::const_iterator it = this->team_member_set_.begin();
			it != this->team_member_set_.end(); ++it) {
		Logic_Player *member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (member && member->team_get_ready_status()) {
			member->req_set_ready_state(false);
		}
	}
	return 0;
}

int Team::unready_leader(void) {
	Logic_Player *leader = this->get_team_leader();
	if (leader) {
		leader->req_set_ready_state(false);
	}
	return 0;
}

int &Team::scene_id(void) {
	return this->scene_id_;
}

int8_t &Team::scene_degree(void) {
	return this->degree_;
}

std::string &Team::password(void) {
	return this->password_;
}

int8_t &Team::type(void) {
	return this->type_;
}

std::string &Team::words(void) {
	return this->words_;
}

int8_t &Team::degree(void) {
	return this->degree_;
}

Int8_Vec &Team::index_list(void) {
	return this->index_list_;
}

int8_t &Team::in_battle(void) {
	return this->in_battle_;
}

Time_Value &Team::last_act_time(void) {
	return this->last_act_time_;
}

const role_id_t &Team::leader_role_id(void) {
	return this->leader_role_id_;
}

const int &Team::enter_level(void) const{
	return this->enter_level_;
}

int Team::set_enter_level(const int lvl) {
	this->enter_level_ = lvl;
	return 0;
}

int64_t Team::get_team_id_for_scene(void) {
	return team_id_for_scene_;
}

int Team::set_team_id_for_scene(const int64_t id) {
	team_id_for_scene_ = id;
	return 0;
}

void Team::info_all_online_player(int msg_id, Block_Buffer* buf) {
	info_leader(msg_id, buf);
	info_member(msg_id, buf);
}

void Team::info_all_online_player_without_self(int msg_id, Block_Buffer* buf, role_id_t role_id) {
	if (!this->is_leader(role_id)) {
		info_leader(msg_id, buf);
	}
	info_member_without_self(msg_id, buf, role_id);
}

void Team::get_team_basic_info(Team_Basic_Info &team_info) {
	team_info.reset();
	team_info.index = this->team_index_;
	team_info.team_id = this->team_id_;
	team_info.type = this->type_;
	team_info.password = this->password_;
	team_info.scene_id = this->scene_id_;
	team_info.teamer_nums = this->get_all_team_member_nums();
	if (team_info.teamer_nums > CONFIG_CACHE_TEAM->team_config().max_player) {
		team_info.teamer_nums = CONFIG_CACHE_TEAM->team_config().max_player;
	}
	team_info.scene_degree = this->degree_;
	team_info.teamer_nums_max = this->get_position_max();
	if (team_info.teamer_nums_max > CONFIG_CACHE_TEAM->team_config().max_player) {
		team_info.teamer_nums_max = CONFIG_CACHE_TEAM->team_config().max_player;
	}
	team_info.words = this->words_;
	team_info.leader.role_id = this->leader_role_id_;
	team_info.battle = this->in_battle_;
	team_info.level = this->enter_level_;
	Logic_Player *player = this->get_team_leader();
	if (NULL != player) {
		TEAM_MANAGER->get_role_info_from_logic_player(player, team_info.leader);
	} else {
		RoleID_OnlineTeamerInfo_Map::const_iterator it = this->offline_teamer_map().find(this->leader_role_id_);
		if (it != this->offline_teamer_map().end()) {
			team_info.leader = it->second.role;
		}
	}
	team_info.index_list = this->index_list_;//.assign(this->index_list_.begin(), this->index_list_.end());
	if (this->type_ == TEAM_TYPE_FB && !is_not_first_team_fb_) {
		team_info.teamer_nums = team_info.teamer_nums_max;
	}
}

Team::RoleID_TimeValue_Map &Team::offline_time_map(void) {
	return this->offline_time_map_;
}

Team::RoleID_OnlineTeamerInfo_Map &Team::offline_teamer_map(void) {
	return this->offline_teamer_map_;
}

Int_Role_Id_Vec &Team::lineup(void) {
	return this->lineup_;
}

bool &Team::has_notice_card(void) {
	return has_notice_card_;
}
Time_Value &Team::start_open_card_time(void) {
	return this->start_open_card_time_;
}

Int_Uid &Team::open_card_list(void) {
	return this->open_card_list_;
}

boost::unordered_map<int, Drop_Vec> &Team::drops_all_vec(void) {
	return this->drops_all_vec_;
}

Team::Role_RewardInfoVec_Map &Team::role_reward_map() {
	return this->role_reward_map_;
}

int Team::req_open_card(int index, Logic_Player& player) {
	switch(type_) {
	case TEAM_TYPE_FB:
		if (!has_sub_profile().count(player.role_id())) {
//		if (Teamer::PROFILE_USE == player.profile_set() && player.profile_num_fb_all() > 0) {
//			player.profile_num_fb_sub();
//			player.profile_info();
//		} else {
			if (Teamer::PROFILE_USE == player.profile_set()) {
				return ERROR_TEAM_NO_BATTLE;
			}
			return ERROR_PROFILE_NOT_USE;
		}
		break;
	case TEAM_TYPE_SPORTS:
		break;
	default:
		break;
	}
	if (index < OPEN_CARD_INDEX_START || index > OPEN_CARD_INDEX_END) {
		return ERROR_CLIENT_OPERATE;
	}
	if (this->open_card_list_.count(index)) {
		return ERROR_TEAM_CARD_INDEX_HAD_USE;
	}
	if ((int)this->open_card_list_.size() == CONFIG_CACHE_TEAM->team_config().max_player) {
		return ERROR_CLIENT_OPERATE;
	}
	if (this->drops_all_vec_.empty()) {
		return ERROR_FB_ALREADY_OPEN_CARD;
	}
	for (Int_Uid::const_iterator it = this->open_card_list_.begin();
			it != this->open_card_list_.end(); ++it) {
		if (it->second == player.role_id()) {
			return ERROR_FB_ALREADY_OPEN_CARD;
		}
	}
//	this->open_card_list_.insert(std::make_pair(index, player.role_id()));
//	this->role_reward_map_.insert(std::make_pair(player.role_id(), Reward_Info_Vec()));
	// 翻牌奖励

	MSG_81000080 ac_msg;
	int ret = this->team_card_reward_msg(index, player.role_id(), ac_msg.list, true);
	if (ret != 0) {
		return ret;
	}
	Block_Buffer buf;
	buf.make_message(ac_msg.msg_id);
	ac_msg.serialize(buf);
	buf.finish_message();

	this->info_all_online_player_without_self(ac_msg.msg_id, &buf, player.role_id());

	MSG_50100201 res_msg;
	std::vector<Reward_Info_List>::const_iterator it = ac_msg.list.begin();
	if (it != ac_msg.list.end()) {
		res_msg.award.assign((*it).award.begin(), (*it).award.end());
	}

	OBJ_SEND_TO_CLIENT(res_msg, player);
	return 0;
}

int Team::team_card_unused_index(void) {
	for (int i = OPEN_CARD_INDEX_START; i < OPEN_CARD_INDEX_END; ++i) {
		if (!this->open_card_list_.count(i)) {
			return i;
		}
	}
	return -1;
}

int Team::team_card_reward_vec(int index, Reward_Info_Vec &reward_list, role_id_t role_id) {
	reward_list.clear();
	int dungeon_id = this->scene_id_;
	const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(dungeon_id);
	if(scene_config == NULL){
		MSG_USER("cannot find dungeon info, id=%d", dungeon_id);
		return -1;
	}
//	const Team_Award_Config* award_cfg = CONFIG_CACHE_DUNGEON->find_team_award_config(dungeon_id);
//	if(award_cfg == NULL){
//		MSG_USER("cannot find team dungeon award, id=%d", dungeon_id);
//		return -1;
//	}
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	Int_Int_Map prop;

	Item_Vec pack_item_vec;
	Reward_Info reward;
//	MSG_50100203 msg;
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);

	this->team_card_reward_from_drop(index, role_id, reward_list, pack_item_vec);

	//normal reward
//	for (Scene_Config::Dungeon_Config_Detail::Goods_Drop_Vec::const_iterator it = config_detail.goods_drop_vec.begin();
//			it != config_detail.goods_drop_vec.end(); ++it) {
//		if((*it).rate >random()%1000){
//			reward.reward_id = (*it).goods_id;
//			reward.quantity = (*it).quantity;
//			reward_list.push_back(reward);
//			//package item
//			Item_Detail item(reward.reward_id, reward.quantity,1);
//			pack_item_vec.push_back(item);
//		}
//	}
	//pack
	//exp
//	{
//		Exp_All exp_all = player->modify_experience_inter(award_cfg->exp, true, true, true);
//		if (0 == exp_all.result) {
//			reward.reward_id = PT_EXP_CURRENT;
//			reward.quantity = exp_all.all;
//			reward.vip_add = exp_all.vip_exp;
//			reward.world_level = player->get_world_level_add();
//			reward_list.push_back(reward);
//			acv_msg.property.push_back(Property(502503, exp_all.all));
//			acv_msg.vip_exp = exp_all.vip_exp;
//			acv_msg.world_level = player->get_world_level_add();
//			prop.insert(std::make_pair(reward.reward_id, award_cfg->exp));
//		}
//	}
//	//gold
//	if(award_cfg->copper > 0){
//		int gold = award_cfg->copper;
//		int ret = -1;
//		if(player){
//			ret = player->pack_add_money(Money_Add_Info(COPPER, gold, Money_DM_Info(MONEY_ADD_TEAM_FB_AWARD)), MONEY_OPT_REAL);
//		}
//		if (0 == ret) {
//			reward.reward_id = COPPER;
//			reward.quantity = gold;
//			reward_list.push_back(reward);
//			acv_msg.property.push_back(Property(COPPER, gold));
//			prop.insert(std::make_pair(reward.reward_id, reward.quantity));
//		}
//	}
//	//soul
//	if(award_cfg->soul > 0){
//		int soul = award_cfg->soul;
//		int ret = -1;
//		if(player){
//			ret = player->pack_add_money(Money_Add_Info(SOULS, soul, Money_DM_Info(MONEY_ADD_TEAM_FB_AWARD)), MONEY_OPT_REAL);
//		}
//		if (0 == ret) {
//			reward.reward_id = SOULS;
//			reward.quantity = soul;
//			reward_list.push_back(reward);
//			acv_msg.property.push_back(Property(SOULS, soul));
//			prop.insert(std::make_pair(reward.reward_id, reward.quantity));
//		}
//	}

	// to packege
	if(pack_item_vec.size() > 0){
		Item_Vec piv;
		Money_Add_List mal;
		for (Item_Vec::const_iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
			if (it->id_ < 1000000) {
				mal.push_back(Money_Add_Info(Money_Type(it->id_), it->amount_, Money_DM_Info(MONEY_ADD_TEAM_FB)));
				acv_msg.property.push_back(Property(it->id_, it->amount_));
				prop.insert(std::make_pair(it->id_, it->amount_));
			} else {
				piv.push_back(*it);
				Item_Basic_Info ibi;
				it->set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				Card_Info card_info;
				if (it->set_item_card_info(card_info)) {
					acv_msg.card_info_vec.push_back(card_info);
				}
				prop.insert(std::make_pair(ibi.id, ibi.amount));
			}
		}
		if (player) {
			if (!mal.empty()) {
				player->pack_add_money(mal);
			}
			if (!piv.empty()) {
				player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
					piv, Gain_Item_DM_Info(Pack::ITEM_GAIN_TEAM_FB));
			}
		} else {
			const int PACK_MAIL_ID = 1019;
			const int PACK_MAIL_ID_NEW = 1106;
			const Item_Gain_Source_Detail *cfg = CONFIG_CACHE_ITEM->find_item_gain_source(Pack::ITEM_GAIN_TEAM_FB);
			if (!cfg) {
				LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id, piv);
			} else {
				const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
				const Mail_Config_Map::const_iterator mailc = mail_config.find(PACK_MAIL_ID_NEW);
				if(mailc == mail_config.end()) {
					LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id, piv);
					return -1;
				}
				std::string title;
				std::string contents;
				{
					const int size_buf = mailc->second.mail_title.size() + \
							sizeof(cfg->name) * 2 + 10;
					char content_buf[size_buf];
					memset(content_buf, 0, size_buf);
					snprintf(content_buf, size_buf, mailc->second.mail_title.c_str(), cfg->name.c_str());
					title.assign(content_buf);
				}

				{
					const int size_buf = mailc->second.mail_content.size() + \
							sizeof(cfg->name) * 2 + 10;
					char content_buf[size_buf];
					memset(content_buf, 0, size_buf);
					snprintf(content_buf, size_buf, mailc->second.mail_content.c_str(), cfg->name.c_str());
					contents.assign(content_buf);
				}
				if (0 != LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id, piv, title, contents)) {
					LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id, piv);
				}
			}
		}
	}

	if (player && (!acv_msg.item_info.empty() || !acv_msg.property.empty())) {
		OBJ_SEND_TO_CLIENT(acv_msg, (*player));
		player->reward_back_listen_accumulated_rewards(REWARD_BACK_PRO_TEAM_EXPLORE, prop, player->team_start_time());
	}
	return 0;
}

int Team::team_card_reward_msg(const int &index, const role_id_t &role_id, std::vector<Reward_Info_List> &list, bool from_client) {
//	for (Role_Vec::const_iterator it = this->role_id_non_strength_.begin(); it != this->role_id_non_strength_.end(); ++it) {
//		if (role_id == *it) {
//			return ERROR_STRENGTH_NOT_ENOUGH;
//		}
//	}
	if (!from_client) {
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if (player) {
			switch(type_) {
			case TEAM_TYPE_FB:
				if (!has_sub_profile().count(player->role_id())) {
//				if (Teamer::PROFILE_USE == player->profile_set() && player->profile_num_fb_all() > 0) {
////					player->profile_num_fb_sub();
////					player->profile_info();
//				} else {
					if (Teamer::PROFILE_USE == player->profile_set()) {
						return ERROR_TEAM_NO_BATTLE;
					}
					return ERROR_PROFILE_NOT_USE;
				}
				break;
			case TEAM_TYPE_SPORTS:
				break;
			default:
				break;
			}
		} else {
			return ERROR_PERM_LIMIT;
		}
	}
	int index_bak = index;
	if (index < OPEN_CARD_INDEX_START || index > OPEN_CARD_INDEX_END) {
		index_bak = this->team_card_unused_index();
	}
	if (index_bak != -1) {
		Reward_Info_List info;
		info.index = index_bak;
		if (this->is_in_team_offlinelist(role_id)) {
			Team::RoleID_OnlineTeamerInfo_Map::const_iterator it_off = this->offline_teamer_map().find(role_id);
			if (it_off != this->offline_teamer_map().end()) {
				info.role_info = it_off->second.role;
			}
		} else {
			Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
			if (NULL != player) {
				get_team_role_info_from_logic(player, info.role_info);
			}
		}
		this->team_card_reward_vec(info.index, info.award, (role_id));
		list.push_back(info);
	} else {
		MSG_USER("impossible");
	}
	return 0;
}

int Team::team_card_reward_from_drop(int index, const role_id_t &role_id, Reward_Info_Vec &reward_vec, Item_Vec &pack_item_vec) {
	if (index < OPEN_CARD_INDEX_START || index > OPEN_CARD_INDEX_END) {
		return ERROR_CLIENT_OPERATE;
	}
	boost::unordered_map<int, Drop_Vec>::const_iterator it_drops_all = this->drops_all_vec_.find(index);
	if (it_drops_all != this->drops_all_vec_.end()) {
		Reward_Info info;
//		Item_Detail item;
		for (Drop_Vec::const_iterator it = it_drops_all->second.begin();
				it != it_drops_all->second.end(); ++it) {
			make_item_vec(pack_item_vec, (*it).item, (*it).amount, (*it).bind);
		}
		for (Item_Vec::const_iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
			info.reset();
			info.reward_id = (*it).id_;
			info.quantity = (*it).amount_;
			info.bind = (*it).bind_;
			reward_vec.push_back(info);
		}
		this->open_card_list_.insert(std::make_pair(index, role_id));
		this->role_reward_map_.insert(std::make_pair(role_id, reward_vec));
	}
	return 0;
}

int Team::team_card_handle_drops(void) {
	int dungeon_id = this->scene_id_;
	const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(dungeon_id);
	if(scene_config == NULL){
		MSG_USER("cannot find dungeon info, id=%d", dungeon_id);
		return -1;
	}
	const Team_Award_Config* award_cfg = CONFIG_CACHE_DUNGEON->find_team_award_config(dungeon_id);
	if(award_cfg == NULL){
		MSG_USER("cannot find team dungeon award, id=%d", dungeon_id);
		return -1;
	}
	if (award_cfg->drops.empty()) {
		Drop_Vec drop_all;
		for (int i = 0; i < MAX_DROPS; ++i) {
			this->drops_all_vec_.insert(std::make_pair(i, drop_all));
		}
		return -1;
	}
	double mult_coe = award_cfg->drop_mult_coe;
	double lv_coe = award_cfg->drop_lv_coe;
	for (Int_Vec::const_iterator it_drops = award_cfg->drops.begin();
			it_drops != award_cfg->drops.end(); ++it_drops) {
		const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(*it_drops);
		if (drop_config == NULL) continue;
		int player_count = MAX_DROPS;
		Drop_Vec drops_all;
		for(int i=0; i<player_count; ++i) {
			drops_all.clear();
			team_card_process_drop_mode(0, drops_all, drop_config, mult_coe, lv_coe);
			boost::unordered_map<int, Drop_Vec>::iterator it = this->drops_all_vec_.find(i);
			if (it != this->drops_all_vec_.end()) {
				for (Drop_Vec::const_iterator it_drops_all = drops_all.begin();
						it_drops_all != drops_all.end(); ++it_drops_all) {
					it->second.push_back(*it_drops_all);
				}
			} else {
				this->drops_all_vec_.insert(std::make_pair(i, drops_all));
			}
		}
	}

	int size = MAX_DROPS - this->drops_all_vec_.size();
	Drop_Vec drop_all;
	for (int i = 0; i < size; ++i) {
		this->drops_all_vec_.insert(std::make_pair(i, drop_all));
	}

	return 0;
}

Role_Vec &Team::team_fb_role_id_non_strength(void) {
	return this->role_id_non_strength_;
}
bool &Team::is_not_first_team_fb(void) {
	return is_not_first_team_fb_;
}
Role_Id_Set &Team::has_sub_profile(void){
	return has_sub_profile_;
}

int Team::set_sports_level(bool is_strict) {
	if (is_strict && TEAM_TYPE_SPORTS != type_) {
		return 0;
	}
	int lvl = 0; // 取队内最高等级
	Logic_Player *leader = this->get_team_leader();
	if (leader) {
		lvl = leader->level();
	}
	for (Role_Hash_Set::iterator it = this->team_member_set_.begin();
			it != this->team_member_set_.end(); ++it) {
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (player && lvl < player->level()) {
			lvl = player->level();
		}
	}
	for (Team::RoleID_OnlineTeamerInfo_Map::iterator it = this->offline_teamer_map_.begin();
			it != this->offline_teamer_map_.end(); ++it) {
		if (lvl < it->second.role.level) {
			lvl = it->second.role.level;
		}
	}
	this->set_enter_level(lvl);
	return 0;
}

const int32_t &Team::point(void) const {
	return this->point_;
}

int Team::add_point(const int32_t point) {
	if (point > 0) {
		this->point_ += point;
	}
	return 0;
}

int16_t &Team::fail_times(void) {
	return this->fail_times_;
}

int16_t &Team::match_fail(void) {
	return this->match_fail_;
}

Time_Value &Team::match_fail_time(void) {
	return this->match_fail_time_;
}

bool &Team::battle_over_flag(void) {
	return this->battle_over_flag_;
}

int Team::team_hero_dream_award(void) {
	const Scene_Config * scene_config = CONFIG_CACHE_SCENE->scene_config_cache(this->scene_id_);
	if (!scene_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Team_Award_Config* award_cfg = CONFIG_CACHE_DUNGEON->find_team_award_config(this->scene_id_);
	if(award_cfg == NULL){
		MSG_USER("cannot find team dungeon award, id=%d", this->scene_id_);
		return -1;
	}

	MSG_81000500 acv_msg;
	MSG_81000102 awd_msg;
	acv_msg.type = type_;
	acv_msg.scene_id = scene_id_;
	Int_String is;
	std::vector<RoleID_Double_Map*> all_map;
	all_map.push_back(&hurt_map_);
	all_map.push_back(&be_hurt_map_);
	all_map.push_back(&cure_map_);
	std::vector< std::vector<Int_String>* > all_map_msg;
	all_map_msg.push_back(&acv_msg.hurt_vec);
	all_map_msg.push_back(&acv_msg.shield_vec);
	all_map_msg.push_back(&acv_msg.cure_vec);
	for (size_t i = 0; i < all_map.size(); ++i) {
		for (RoleID_Double_Map::const_iterator it = all_map[i]->begin(); it != all_map[i]->end(); ) {
			if (is_in_team(it->first)) {
				++it;
				continue;
			}
			it = all_map[i]->erase(it);
		}
	}
	for (size_t i = 0; i < all_map.size(); ++i) {
		for (RoleID_Double_Map::const_iterator it = all_map[i]->begin(); it != all_map[i]->end(); ++it) {
			is.reset();
			is.int_value = static_cast<int>(it->second);
			Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(it->first);
			if (pl) {
				is.str_value = pl->role_name();
				all_map_msg[i]->push_back(is);
			} else {
				DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->first);
				if (iter !=  LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
					is.str_value = iter->second->role_name;
					all_map_msg[i]->push_back(is);
				} else {
					MSG_TRACE_ABORT("role_id(%ld) can not find.", it->first);
				}
			}
		}
	}

	Int_Int_Map prop;
	//Scene_Config::Dungeon_Config_Detail dungeon = scene_config->dungeon;
	Logic_Player *leader = this->get_team_leader();
	if (leader) {
		if (has_sub_profile().count(this->leader_role_id())) {
//			acv_msg.type = 1;
			Reward_Info info;
			{
				Exp_All exp_all = leader->modify_experience_inter(award_cfg->exp, true, true, true);
				if (0 == exp_all.result) {
					info.reset();
					info.reward_id = PT_EXP_CURRENT;
					info.quantity = exp_all.all_without_world_level;
					info.vip_add = exp_all.vip_exp;
					info.world_level = leader->get_world_level_add();
					acv_msg.reward_list.push_back(info);
					awd_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
					prop.insert(std::make_pair(info.reward_id, info.quantity));
					{
						for (int i = DR_EXP_START; i < DR_EXP_END; ++i) {
							info.reset();
							info.reward_id = i;
							switch(i) {
							case BASE_EXP: {
								info.quantity = exp_all.exp;
								break;
							}
							case VIP_EXP: {
								info.quantity = exp_all.vip_exp;
								break;
							}
							case TOOL_EXP: {
								info.quantity = exp_all.buff_exp;
								break;
							}
							case FRIEND_EXP: {
								info.quantity = 0;
								break;
							}
							default:
								continue;
								break;
							}
							acv_msg.reward_list.push_back(info);
						}
					}
				}
			}
			Money_Add_List money_add_list;
			if (award_cfg->copper > 0) {
				info.reset();
				info.reward_id = Pack::COPPER;
				info.quantity = award_cfg->copper;
				acv_msg.reward_list.push_back(info);
				awd_msg.property.push_back(Property(COPPER, award_cfg->copper));
				prop.insert(std::make_pair(info.reward_id, info.quantity));
				money_add_list.push_back(
					Money_Add_Info(Pack::COPPER, award_cfg->copper, Money_DM_Info(Pack::MONEY_ADD_TEAM_HERO_DREAM)));
			}
			if (award_cfg->soul > 0) {
				info.reset();
				info.reward_id = Pack::SOULS;
				info.quantity = award_cfg->soul;
				acv_msg.reward_list.push_back(info);
				awd_msg.property.push_back(Property(SOULS, award_cfg->soul));
				prop.insert(std::make_pair(info.reward_id, info.quantity));
				money_add_list.push_back(
					Money_Add_Info(Pack::SOULS, award_cfg->soul, Money_DM_Info(Pack::MONEY_ADD_TEAM_HERO_DREAM)));
			}
			if (!money_add_list.empty()) {
				leader->pack_add_money(money_add_list, MONEY_OPT_REAL);
			}
		} else if (!has_sub_profile().empty()) {
//			acv_msg.type = 0;
			Reward_Info info;
			info.reward_id = Pack::FRIENDSHIP;
			info.quantity = award_cfg->friendship;
			acv_msg.reward_list.push_back(info);
			prop.insert(std::make_pair(info.reward_id, info.quantity));
			Money_Add_List money_add_list;
			if (award_cfg->friendship > 0) {
				awd_msg.property.push_back(Property(FRIENDSHIP, award_cfg->friendship));
				money_add_list.push_back(
					Money_Add_Info(Pack::FRIENDSHIP, award_cfg->friendship, Money_DM_Info(Pack::MONEY_ADD_TEAM_HERO_DREAM)));
			}
			if (!money_add_list.empty()) {
				leader->pack_add_money(money_add_list, MONEY_OPT_REAL);
			}
		}
		{
			OBJ_SEND_TO_CLIENT(acv_msg, (*leader));
		}
		if (!awd_msg.property.empty()) {
			OBJ_SEND_TO_CLIENT(awd_msg, (*leader));
		}
		leader->reward_back_listen_accumulated_rewards(REWARD_BACK_PRO_HERO_DREAMS, prop, leader->team_start_time());
	}
	for (Role_Hash_Set::const_iterator it = this->team_member_set().begin();
			it != this->team_member_set().end(); ++it) {
		Logic_Player * member = LOGIC_MONITOR->find_logic_player_by_role_id(*it);;
		if (NULL != member) {
			acv_msg.reset();
			awd_msg.reset();
			prop.clear();
			acv_msg.type = type_;
			acv_msg.scene_id = scene_id_;
			for (size_t i = 0; i < all_map.size(); ++i) {
				for (RoleID_Double_Map::const_iterator it = all_map[i]->begin(); it != all_map[i]->end(); ++it) {
					is.reset();
					is.int_value = static_cast<int>(it->second);
					Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(it->first);
					if (pl) {
						is.str_value = pl->role_name();
						all_map_msg[i]->push_back(is);
					} else {
						DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->first);
						if (iter !=  LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
							is.str_value = iter->second->role_name;
							all_map_msg[i]->push_back(is);
						} else {
							MSG_TRACE_ABORT("role_id(%ld) can not find.", it->first);
						}
					}
				}
			}
			if (has_sub_profile().count(*it) ) {
//				acv_msg.type = 1;
				Reward_Info info;
				Exp_All exp_all = member->modify_experience_inter(award_cfg->exp, true, true, true);
				if (0 == exp_all.result) {
					info.reset();
					info.reward_id = PT_EXP_CURRENT;
					info.quantity = exp_all.all;
					info.vip_add = exp_all.vip_exp;
					info.world_level = member->get_world_level_add();
					acv_msg.reward_list.push_back(info);
					awd_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
					prop.insert(std::make_pair(info.reward_id, info.quantity));
					{
						for (int i = DR_EXP_START; i < DR_EXP_END; ++i) {
							info.reset();
							info.reward_id = i;
							switch(i) {
							case BASE_EXP: {
								info.quantity = exp_all.exp;
								break;
							}
							case VIP_EXP: {
								info.quantity = exp_all.vip_exp;
								break;
							}
							case TOOL_EXP: {
								info.quantity = exp_all.buff_exp;
								break;
							}
							case FRIEND_EXP: {
								info.quantity = 0;
								break;
							}
							default:
								continue;
								break;
							}
							acv_msg.reward_list.push_back(info);
						}
					}
				}
				Money_Add_List money_add_list;
				if (award_cfg->copper > 0) {
					info.reset();
					info.reward_id = Pack::COPPER;
					info.quantity = award_cfg->copper;
					acv_msg.reward_list.push_back(info);
					awd_msg.property.push_back(Property(COPPER, award_cfg->copper));
					prop.insert(std::make_pair(info.reward_id, info.quantity));
					money_add_list.push_back(
						Money_Add_Info(Pack::COPPER, award_cfg->copper, Money_DM_Info(Pack::MONEY_ADD_TEAM_HERO_DREAM)));
				}
				if (award_cfg->soul > 0) {
					info.reset();
					info.reward_id = Pack::SOULS;
					info.quantity = award_cfg->soul;
					acv_msg.reward_list.push_back(info);
					awd_msg.property.push_back(Property(SOULS, award_cfg->soul));
					prop.insert(std::make_pair(info.reward_id, info.quantity));
					money_add_list.push_back(
						Money_Add_Info(Pack::SOULS, award_cfg->soul, Money_DM_Info(Pack::MONEY_ADD_TEAM_HERO_DREAM)));
				}
				if (!money_add_list.empty()) {
					member->pack_add_money(money_add_list, MONEY_OPT_REAL);
				}
			} else if (!has_sub_profile().empty()) {
//				acv_msg.type = 0;
				Reward_Info info;
				info.reward_id = Pack::FRIENDSHIP;
				info.quantity = award_cfg->friendship;
				acv_msg.reward_list.push_back(info);
				prop.insert(std::make_pair(info.reward_id, info.quantity));
				Money_Add_List money_add_list;
				if (award_cfg->friendship > 0) {
					awd_msg.property.push_back(Property(FRIENDSHIP, award_cfg->friendship));
					money_add_list.push_back(
						Money_Add_Info(Pack::FRIENDSHIP, award_cfg->friendship, Money_DM_Info(Pack::MONEY_ADD_TEAM_HERO_DREAM)));
				}
				if (!money_add_list.empty()) {
					member->pack_add_money(money_add_list, MONEY_OPT_REAL);
				}
			}
			{
				OBJ_SEND_TO_CLIENT(acv_msg, (*member));
			}
			if (!awd_msg.property.empty()) {
				OBJ_SEND_TO_CLIENT(awd_msg, (*member));
			}
			member->reward_back_listen_accumulated_rewards(REWARD_BACK_PRO_HERO_DREAMS, prop, member->team_start_time());
		}
	}

	return 0;
}

RoleID_Double_Map &Team::hurt_map(void) {
	return hurt_map_;
}
RoleID_Double_Map &Team::be_hurt_map(void) {
	return be_hurt_map_;
}
RoleID_Double_Map &Team::cure_map(void) {
	return cure_map_;
}

bool Team::has_profile_set(role_id_t role_id) {
	return (profile_set_.count(role_id) > 0) || has_sub_profile_.count(role_id) > 0;
}

int Team::profile_set_add(role_id_t role_id) {
	profile_set_.insert(role_id);
	return 0;
}
int Team::profile_set_del(role_id_t role_id) {
	profile_set_.erase(role_id);
	return 0;
}
int Team::add_monster(const std::string &monster) {
	monster_ = monster;
	return 0;
}
int Team::del_monster(void) {
	monster_.clear();
	return 0;
}
std::string Team::get_monster(void) {
	return monster_;
}
bool Team::has_profile_nums(const int8_t type) {
	if (!this->has_sub_profile_.empty()) {
		return true;
	}
	int8_t type_bak = type;
	if (type_bak == -1) {
		type_bak = type_;
	}
	std::vector<Logic_Player *> pl_vec;
	if (leader_role_id_) {
		Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(leader_role_id_);
		if (pl) {
			pl_vec.push_back(pl);
		}
	}
	for (Role_Hash_Set::const_iterator it = team_member_set_.begin();
			it != team_member_set_.end(); ++it) {
		Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(*it);
		if (pl) {
			pl_vec.push_back(pl);
		}
	}
	switch(type_) {
//	case Team::TEAM_TYPE_FB: {
//		for (std::vector<Logic_Player *>::const_iterator it = pl_vec.begin();
//				it != pl_vec.end(); ++it) {
//
//		}
//		break;
//	}
	case Team::TEAM_TYPE_SPORTS: {
		for (std::vector<Logic_Player *>::const_iterator it = pl_vec.begin();
				it != pl_vec.end(); ++it) {
			if ((*it)->profile_num_sports_all() > 0 && Teamer::PROFILE_USE == (*it)->profile_set()) {
				return true;
			}
		}
		break;
	}
//	case Team::TEAM_TYPE_HERO_DREAM:
//		for (std::vector<Logic_Player *>::const_iterator it = pl_vec.begin();
//				it != pl_vec.end(); ++it) {
//
//		}
//		break;
//	case Team::TEAM_TYPE_TIMELIMIT: {
//		for (std::vector<Logic_Player *>::const_iterator it = pl_vec.begin();
//				it != pl_vec.end(); ++it) {
//
//		}
//		break;
//	}
//	case Team::TEAM_TYPE_DEFEND: {
//		for (std::vector<Logic_Player *>::const_iterator it = pl_vec.begin();
//				it != pl_vec.end(); ++it) {
//
//		}
//		break;
//	}
	default:
		break;
	}
	return false;
}

void Team::sync_team_info_to_chat(int8_t operate) {
	MSG_20100302 inner_msg;
	Block_Buffer buf;
	buf.make_message(inner_msg.msg_id);
	buf.write_int8(operate);
	buf.write_int64(this->team_id_);
	if (SYNC_ADD == operate) {
		this->serialize(buf);
	}
	buf.finish_message();

	LOGIC_MONITOR->send_to_local_chat(buf);
}

void Team::sync_team_info_to_scene(int8_t operate) {
	MSG_20100304 inner_msg;
	Block_Buffer buf, buf_back;
	buf.make_message(inner_msg.msg_id);
	buf.write_int8(operate);
	buf.write_int64(this->team_id_);
	if (Team::SYNC_ADD == operate) {
		this->serialize(buf);
	}
	buf.finish_message();
	const Scene_Config_Map_Cache &scene_config_map = CONFIG_CACHE_SCENE->scene_config_cache_map();
	for (Scene_Config_Map_Cache::const_iterator it = scene_config_map.begin(); it != scene_config_map.end(); ++it) {
		if (Scene_Team == it->second.manager
				|| Scene_Task_Manager == it->second.manager
				|| Scene_Team_Defend == it->second.manager
				|| (Scene_Killing == it->second.manager && it->second.dungeon.can_team != 0)) {
			LOGIC_MONITOR->send_to_scene(it->first, 0, buf);
		}
	}
}

void Team::serialize(Block_Buffer &buf) {
	buf.write_int8(type_);
	buf.write_string(words_);
	buf.write_int32(scene_id_);
	buf.write_int8(degree_);
	buf.write_uint32(team_index_);
	buf.write_int64(team_id_);
	buf.write_int64(team_id_for_scene_);
	buf.write_int64(leader_role_id_);
	uint16_t len = 0;
	len = team_member_set_.size();
	buf.write_uint16(len);
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		buf.write_int64(*it);
	}
	len = offline_teamer_map_.size();
	buf.write_uint16(len);
	for (RoleID_OnlineTeamerInfo_Map::iterator it = offline_teamer_map_.begin(); it != offline_teamer_map_.end(); ++it) {
		buf.write_int64(it->first);
	}
	buf.write_int8(in_battle_);
	len = lineup_.size();
	buf.write_uint16(len);
	for (Int_Role_Id_Vec::iterator it = lineup_.begin(); it != lineup_.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_int32(enter_level_);
	buf.write_int16(fail_times_);
	buf.write_int16(match_fail_);
	buf.write_bool(is_not_first_team_fb_);
	buf.write_uint16(has_sub_profile_.size());
	for (Role_Id_Set::const_iterator it = has_sub_profile_.begin(); it != has_sub_profile_.end(); ++it) {
		buf.write_int64(*it);
	}
}

void Team::deserialize(Block_Buffer &buf) {
	buf.read_int8(type_);
	buf.read_string(words_);
	buf.read_int32(scene_id_);
	buf.read_int8(degree_);
	buf.read_uint32(team_index_);
	buf.read_int64(team_id_);
	buf.read_int64(team_id_for_scene_);
	buf.read_int64(leader_role_id_);
	uint16_t len = 0;
	buf.read_uint16(len);
	int64_t role_id = 0;
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(role_id);
		team_member_set_.insert(role_id);
	}
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(role_id);
		offline_teamer_map_.insert(std::make_pair(role_id, Online_Teamer_Info()));
	}
	buf.read_int8(in_battle_);
	buf.read_uint16(len);
	Int_Role_Id int_role_id;
	Int_Role_Id_Vec lineup;
	for (uint16_t i = 0; i < len; ++i) {
		int_role_id.deserialize(buf);
		lineup.push_back(int_role_id);
	}
	this->lineup_replace(lineup);
	buf.read_int32(enter_level_);
	buf.read_int16(fail_times_);
	buf.read_int16(match_fail_);
	buf.read_bool(is_not_first_team_fb_);
	buf.read_uint16(len);
	role_id_t id = 0;
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(id);
		has_sub_profile_.insert(id);
	}
}

void Team::set_last_act_time_now(void) {
	this->last_act_time_ = Time_Value::gettimeofday();
}

void Team::set_last_act_time_zero(void) {
	this->last_act_time_ = Time_Value::zero;
}

bool compare_less(const Team &team1, const Team &team2) {
//	if ((int8_t)Team::TEAM_TYPE_FB != team1.type || (int8_t)Team::TEAM_TYPE_FB != team2.type) {
//		return false;
//	}
//	const Scene_Config *scene_config1 = CONFIG_CACHE_SCENE->scene_config_cache(team1.scene_id_);
//	const Scene_Config *scene_config2 = CONFIG_CACHE_SCENE->scene_config_cache(team2.scene_id_);
//	if (!scene_config1 || !scene_config2) {
//		return false;
//	}
	if (team1.enter_level() < team2.enter_level()) {
		return true;
	} else if (team1.enter_level() == team2.enter_level()
			&& team1.get_unused_position() < team2.get_unused_position()) {
		return true;
	} else if (team1.enter_level() == team2.enter_level()
			&& team1.get_unused_position() == team2.get_unused_position()
			&& team1.get_team_index() < team2.get_team_index()) {
		return true;
	}
	return false;
}

bool Team_PtrGreater::operator ()(const Team* team1, const Team* team2) const {
	if (team1->point() > team2->point()) {
		return true;
	} else if (team1->point() < team2->point()) {
		return false;
	}

	return team1->get_team_id() > team2->get_team_id();
}
