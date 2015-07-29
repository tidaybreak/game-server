/*
 * Gang_War_Scene.cpp
 *
 *  Created on: 2014年8月1日
 *      Author: Linqiyou
 */

#include "Gang_War_Scene.h"
#include "Scene_Player.h"
#include "Msg_Active_Struct.h"
#include "Msg_Battle_Scene_Struct.h"
#include "Battle_Scene.h"
#include "NPC/monster/Monster.h"
#include "war/Scene_War_Manager.h"
#include "Config_Cache_Dungeon.h"
#include "Err_Code.h"
#include "Config_Cache_NPC.h"
#include "NPC/NPC_Manager.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Gang_Struct.h"
#include "gang/Gang_War_Def.h"
#include "Scene_Monitor.h"
#include "announcement/Annoucement.h"
#include "chater/Chater_Def.h"
#include "Config_Cache_Gang.h"

const int GANG_WAR_LIGHT_MOVE_ABLE = 21;
const int GANG_WAR_DARK_MOVE_ABLE = 22;

bool Gang_War_Rank_Greater(const Gang_War_Rank_Info& player1, const Gang_War_Rank_Info& player2){
	if (player1.kill_num > player2.kill_num) {
		return true;
	} else if (player1.kill_num < player2.kill_num) {
		return false;
	}
	if (player1.hold_spot > player2.hold_spot) {
		return true;
	} else if (player1.hold_spot < player2.hold_spot) {
		return false;
	}
	if (player1.force > player2.force) {
		return true;
	} else if (player1.force < player2.force) {
		return false;
	}
	if (player1.die_num < player2.die_num) {
		return true;
	} else if (player1.die_num > player2.die_num) {
		return false;
	}
	if (player1.role_id < player2.role_id) {
		return true;
	}
	return false;
}


Gang_War_Scene::Gang_War_Scene(void):player_initial_score_(0), red_score_(0), red_num_(0),
	blue_score_(0), blue_num_(0),  war_total_time_(0){
	reset();
}

Gang_War_Scene::~Gang_War_Scene(void){

}

void Gang_War_Scene::reset(void){
	Move_Scene::reset();

	player_initial_score_ = 0;

	red_score_ = 0;
	red_num_ = 0;
	red_tower_num_ = 0;
	gang_id_red_ = 0;
	gang_name_red_.clear();
	blue_score_ = 0;
	blue_num_ = 0;
	blue_tower_num_ = 0;
	gang_id_blue_ = 0;
	gang_name_blue_.clear();
	red_has_broadcast_will_win_ = false;
	blue_has_broadcast_will_win_ = false;

	check_over_ = false;
	war_over_time_ = Time_Value::max;
	offline_mover_map_.clear();

	src_recover_coord_[0].reset();
	src_recover_coord_[1].reset();
	random_coord_.reset();
	war_total_time_ = 0;

	material_complete_map_.clear();
	material_hold_map_.clear();
	material_ready_list_.clear();
	material_ready_set_.clear();
}

int Gang_War_Scene::tick(const Time_Value &now){
	Move_Scene::tick(now);
	if(this->is_recover_status()){
		return 0;
	}
	if(this->scene_status_ == SCENE_NORMAL_STATUS){
		if(this->war_over_time_ - now <= Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - CONFIG_CACHE_GANG->gang_war_setting_cfg().ready_time)){
			this->scene_status_ = SCENE_READY_STATUS;
			change_move_value(GANG_WAR_LIGHT_MOVE_ABLE, true);
			change_move_value(GANG_WAR_DARK_MOVE_ABLE, true);
			send_end_time();
			gang_war_scene_announce(GANG_WAR_ANNOUNCE_WAR_START, 0, "", "");
		}
		return 0;
	}
//	check_recover_mine();
	if(now >= war_over_time_){
		set_war_over_force();
		return 0;
	}
	if(check_over_){
		this->check_over();
	}

	material_tick();
	return 0;
}

int Gang_War_Scene::enter_scene(Mover *mover){
	Move_Scene::enter_scene(mover);
	if(mover->player_self()){
		Scene_Player* player = mover->player_self();
		Gang_War_Rank_Detail* rank_info = find_offline_mover(mover->role_id());
		if(rank_info){
//			player->stop_gather();
			player->gang_war_set_point_id(0);
			MSG_USER("gang_war_scene:enter_scene: exit_: %d", rank_info->exit_);
			if (rank_info->exit_) {
				if(this->scene_status_ == SCENE_READY_STATUS
						|| (this->war_over_time_ - Time_Value::gettimeofday() <= Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - CONFIG_CACHE_GANG->gang_war_setting_cfg().ready_time))) {
					rank_info->wait_time = Time_Value::gettimeofday() + Time_Value(20);
				}
				rank_info->exit_ = false;
			}
			player->set_gang_war_info(*rank_info);
			if(player->gang_war_info().league == CAMP_RED){
				++red_num_;
			}else{
				++blue_num_;
			}
			player->gang_war_info().src_hp = player->blood_current();
			double dif = player->blood_current() - player->gang_war_info().hp;
			if(dif < 0.00001 && dif > -0.00001){
				// nothing
			}else{
				// 恢复血量
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->gang_war_info().hp;
				fight_setter.push_back(setter);
				//player->fight_modify_fight_prop(fight_setter, true);
				player->modify_props_normal(fight_setter, false, 12);
			}
			this->del_offline_mover(mover->role_id());
		}else{
			if(this->scene_status_ == SCENE_READY_STATUS
					|| (this->war_over_time_ - Time_Value::gettimeofday() <= Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - CONFIG_CACHE_GANG->gang_war_setting_cfg().ready_time))) {
				player->gang_war_info().wait_time = Time_Value::gettimeofday() + Time_Value(20);
			}
			const Gang_War_Matched_Gang_Info *it_index = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(player->ganger_detail().gang_id);
			if (it_index) {
				player->gang_war_info().league = it_index->camp;
			}
			if(player->gang_war_info().league == CAMP_RED){
				++red_num_;
			}else{
				++blue_num_;
			}
			player->gang_war_info().src_hp = player->blood_current();
			player->gang_war_info().hp = player->blood_max();
			player->gang_war_info().gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(player->gang_id());
			// 满血
			if(player->current_blood_rate() < 0.99999){
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->blood_max();
				fight_setter.push_back(setter);
				// player->fight_modify_fight_prop(fight_setter, true);
				player->modify_props_normal(fight_setter, false, 30);
			}
			// 初始积分
			player->gang_war_info().score = player_initial_score_;
		}
		this->broadcast_gang_war_info();
//		this->send_end_time(mover);
		mover->player_self()->update_mover_gang_war_info();
	}
	return 0;
}


int Gang_War_Scene::exit_scene(Mover *mover, bool offline){
	Move_Scene::exit_scene(mover, offline);
	if(this->is_recover_status()){
		return 0;
	}
	if(mover->player_self()){
//		MSG_USER("gang_war_scene:exit_scene: exit_: %d", mover->player_self()->gang_war_info().exit_);
		mover->player_self()->gang_war_info().hp = mover->player_self()->blood_current();
		add_offline_mover(mover->role_id(), mover->player_self()->gang_war_info());
		double src_hp = mover->player_self()->gang_war_info().src_hp;
		mover->player_self()->reset_morale_and_hp(-1.0f, src_hp, false);
		int league = mover->player_self()->gang_war_info().league;
		if(league == 1){
			red_num_ -= 1;
		}else{
			blue_num_ -= 1;
		}
		mover->player_self()->reset_gang_war_info();

		mover->player_self()->stop_gather();
		mover->player_self()->gang_war_set_point_id(0);
		{
			Scene_Layer *layer = scene_layer(0);
			if (layer) {
				for (Mover_Map::iterator it = layer->mover_map_.begin();
						it != layer->mover_map_.end(); ++it) {
					if (it->second && it->second->material_self()) {
						it->second->material_self()->stop_gather(mover->player_self()->role_id());
					}
				}
			}
		}

		this->broadcast_gang_war_info();

//		this->send_end_time(mover);
//		check_player_empty();
	}
	return 0;
}


int Gang_War_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene::client_scene_complete(fighter);
	if(fighter.player_self()){
		fighter.player_self()->gang_war_info().role_id = fighter.player_self()->role_id();
		fighter.player_self()->gang_war_info().role_name = fighter.player_self()->role_name();
		fighter.player_self()->gang_war_info().force = fighter.fighter_detail().fetch_fight_property(PT_FORCE);
		send_war_info(fighter.player_self());
		send_player_info(fighter.player_self());
		broadcast_tower_info(fighter.player_self());

//		if (this->scene_status_ == SCENE_NORMAL_STATUS
//				&& CONFIG_CACHE_GANG->gang_war_setting_cfg().obstacles == 2 ) {
//			MSG_81000005 acv_msg;
//			if(fighter.player_self()->gang_war_info().league == CAMP_RED){
//				CONFIG_CACHE_GANG->gang_war_setting_cfg().obstacles[0u];
//				acv_msg.npc_info.role_id = 1000000 + CAMP_RED;
//			}else{
//				CONFIG_CACHE_GANG->gang_war_setting_cfg().obstacles[1u];
//				acv_msg.npc_info.role_id = 1000000 + CAMP_BLUE;
//			}
//		}
		if(this->is_recover_status()){
			kick_player(fighter.player_self(), 0);
		}
	}
	return 0;
}

void Gang_War_Scene::npc_init(int scene_id, int sort, int sub_sort){
	section_init(scene_id, sort, sub_sort);
	NPC_MANAGER->section_refresh(&npc_section_);
}
void Gang_War_Scene::section_init(int scene_id, int sort, int sub_sort){
	random_coord_.reset();
//	const War_Random_Coord_Config* rd_coord_cfg = CONFIG_CACHE_DUNGEON->war_random_coord_config();
//	if(rd_coord_cfg){
//		random_coord_.light_revive_ball = rd_coord_cfg->light_revive_ball;
//		random_coord_.dark_revive_ball = rd_coord_cfg->dark_revive_ball;
//		random_coord_.light_mine = rd_coord_cfg->light_mine;
//		random_coord_.dark_mine = rd_coord_cfg->dark_mine;
//	}
//	Monster_Point_Config revive_ball_cfg;
//	Monster_Point_Config nor_mine_cfg;

	npc_section_.scene_ = this;
	npc_section_.is_team = scene_config()->dungeon.can_team > 0 ? true:false;
	Monster_Section_Config* s_config = CONFIG_CACHE_NPC->get_monster_section_config(scene_id);
	if(s_config == NULL){
		MSG_USER("error scene_id %d;" scene_id);
		return ;
	}
	Monster_Point_Config_Map::iterator p_it;
	for(p_it = s_config->point_map.begin(); p_it != s_config->point_map.end(); ++p_it){
//		if(p_it->second.point_type == 1 && p_it->second.npc_vec.empty() == false){
//			const NPC_Config_Cache* m_cfg = CONFIG_CACHE_NPC->find_npc_config_cache(p_it->second.npc_vec[0].val_1);
//			if(m_cfg){
////				if(m_cfg->type == MOVER_T_MATERIAL_GANG_WAR_TOWER){
////					nor_mine_cfg = p_it->second;
////					continue;
////				}else if(m_cfg->type == MOVER_T_MATERIAL_GANG_WAR_REVIVE_BALL){
////					revive_ball_cfg = p_it->second;
////					continue;
////				}
//			}
//		}
		NPC_Record record;
		record.config = p_it->second;
		record.scene = this;
		if(record.config.refresh_con == 1){
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday() +
					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
		}
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(p_it->first, record));
	}
//	int stage = 0;
//	role_id_t war_id = this->create_scene_args_.id;
//	Honour_Field* h_field = SCENE_WAR_MANAGER->get_honour_field(war_id);
//	if(h_field){
//		stage = h_field->zone_id - WAR_SCENE_LEVEL_LIMIT/WAR_SCENE_ZONE_STAGE;
//	}
	Monster_Group_Config_Map::iterator g_it;
	for(g_it = s_config->group_map.begin(); g_it != s_config->group_map.end(); ++g_it){
		NPC_Group group;
		group.config = g_it->second;
		// 卫兵、资源官动态成长 怪物类型id递减1000
		int m_count = group.config.monster.size();
		for(int i = 0; i < m_count; ++i){
			int new_monster_id = group.config.monster[i];
			if(CONFIG_CACHE_NPC->find_npc_config_cache(new_monster_id)){
				group.config.monster[i] = new_monster_id;
			}
		}
		npc_section_.group_map.insert(std::make_pair(g_it->first, group));
	}
	npc_section_.path_guide_vec = s_config->path_guide_vec;
	//
//	nor_mine_init(&nor_mine_cfg);
//	revive_ball_init(&revive_ball_cfg);
}

//void Gang_War_Scene::nor_mine_init(Monster_Point_Config* point_cfg){
//	for(int i = L_MINE_B; i <= L_MINE_E; ++i){
//		NPC_Record record;
//		record.config = *point_cfg;
//		record.config.id = i;
//		pop_random_coord(i, record.config.birth_coord);
//		record.scene = this;
//		record.section = &npc_section_;
//		if(record.config.refresh_con == 1){
//			record.check_refresh = true;
//			record.birth_freq = Time_Value::gettimeofday() +
//					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
//		}
//		npc_section_.record_map.insert(std::make_pair(i, record));
//	}
//	for(int i = D_MINE_B; i <= D_MINE_E; ++i){
//		NPC_Record record;
//		record.config = *point_cfg;
//		record.config.id = i;
//		pop_random_coord(i, record.config.birth_coord);
//		record.scene = this;
//		record.section = &npc_section_;
//		if(record.config.refresh_con == 1){
//			record.check_refresh = true;
//			record.birth_freq = Time_Value::gettimeofday() +
//					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
//		}
//		npc_section_.record_map.insert(std::make_pair(i, record));
//	}
//}
//void Gang_War_Scene::revive_ball_init(Monster_Point_Config* point_cfg){
//	for(int i = GANG_WAR_RED_REVIVE_BALL_B; i <= GANG_WAR_RED_REVIVE_BALL_E; ++i){
//		NPC_Record record;
//		record.config = *point_cfg;
//		record.config.id = i;
//		pop_random_coord(i, record.config.birth_coord);
//		record.scene = this;
//		record.section = &npc_section_;
//		if(record.config.refresh_con == 1){
//			record.check_refresh = true;
//			record.birth_freq = Time_Value::gettimeofday() +
//					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
//		}
//		npc_section_.record_map.insert(std::make_pair(i, record));
//	}
//	for(int i = GANG_WAR_BLUE_REVIVE_BALL_B; i <= GANG_WAR_BLUE_REVIVE_BALL_E; ++i){
//		NPC_Record record;
//		record.config = *point_cfg;
//		record.config.id = i;
//		pop_random_coord(i, record.config.birth_coord);
//		record.scene = this;
//		record.section = &npc_section_;
//		if(record.config.refresh_con == 1){
//			record.check_refresh = true;
//			record.birth_freq = Time_Value::gettimeofday() +
//					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
//		}
//		npc_section_.record_map.insert(std::make_pair(i, record));
//	}
//}

//void Gang_War_Scene::pop_random_coord(int point_id, Coord& coord){
//	std::vector<Coord> coord_vec;
//	if(point_id >= GANG_WAR_RED_REVIVE_BALL_B && point_id <= GANG_WAR_RED_REVIVE_BALL_E){
//		coord_vec = random_coord_.light_revive_ball;
//	}else if(point_id >= GANG_WAR_BLUE_REVIVE_BALL_B && point_id <= GANG_WAR_BLUE_REVIVE_BALL_E){
//		coord_vec = random_coord_.dark_revive_ball;
//	}else if(point_id >= L_MINE_B && point_id <= L_MINE_E){
//		coord_vec = random_coord_.light_mine;
//	}else if(point_id >= L_MINE_E && point_id <= D_MINE_E){
//		coord_vec = random_coord_.dark_mine;
//	}
//	int count = coord_vec.size();
//	if(count <= 0){
//		return;
//	}
//	int index = random()%count;
//	coord = coord_vec[index];
//	int tm_index = 0;
//	std::vector<Coord>::iterator it;
//	for(it = coord_vec.begin(); it != coord_vec.end(); ++it){
//		if(index == tm_index){
//			coord_vec.erase(it);
//			break;
//		}
//		++tm_index;
//	}
//	if(point_id >= GANG_WAR_RED_REVIVE_BALL_B && point_id <= GANG_WAR_RED_REVIVE_BALL_E){
//		random_coord_.light_revive_ball = coord_vec;
//	}else if(point_id >= GANG_WAR_BLUE_REVIVE_BALL_B && point_id <= GANG_WAR_BLUE_REVIVE_BALL_E){
//		random_coord_.dark_revive_ball = coord_vec;
//	}else if(point_id >= L_MINE_B && point_id <= L_MINE_E){
//		random_coord_.light_mine = coord_vec;
//	}else if(point_id >= L_MINE_E && point_id <= D_MINE_E){
//		random_coord_.dark_mine = coord_vec;
//	}
//}
//void Gang_War_Scene::push_random_coord(int point_id, const Coord& coord){
//	if(point_id >= GANG_WAR_RED_REVIVE_BALL_B && point_id <= GANG_WAR_RED_REVIVE_BALL_E){
//		random_coord_.light_revive_ball.push_back(coord);
//	}else if(point_id >= GANG_WAR_BLUE_REVIVE_BALL_B && point_id <= GANG_WAR_BLUE_REVIVE_BALL_E){
//		random_coord_.dark_revive_ball.push_back(coord);
//	}else if(point_id >= L_MINE_B && point_id <= L_MINE_E){
//		random_coord_.light_mine.push_back(coord);
//	}else if(point_id >= L_MINE_E && point_id <= D_MINE_E){
//		random_coord_.dark_mine.push_back(coord);
//	}
//}
//
//void Gang_War_Scene::change_random_coord(int point_id){
//	NPC_Record* record = get_npc_record(point_id);
//	if(record){
//		Coord src_coord = record->config.birth_coord;
//		Coord rd_coord;
//		this->pop_random_coord(point_id, rd_coord);
//		record->config.birth_coord = rd_coord;
//		push_random_coord(point_id, src_coord);
//	}
//}

void Gang_War_Scene::material_tick(void) {
	Time_Value now = Time_Value::gettimeofday();
	bool need_broadcast = false;
	for (Camp_IndexGangWarMaterialInfoMap_Map::iterator it = material_complete_map_.begin(); it != material_complete_map_.end(); ++it) {
		for (Index_GangWarMaterialInfo_Map::iterator it_d = it->second.begin(); it_d != it->second.end(); ++it_d) {
			if (it_d->second.t < now) {
				if (it_d->second.index == 7) {
					if (it->first == CAMP_RED) {
						this->red_score_ += CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_score_center;
					} else if (it->first == CAMP_BLUE) {
						this->blue_score_ += CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_score_center;
					} else {
						MSG_DEBUG("create_score:(center) scene_index: %ld, int_value: %d, now: %ld, tower_camp: %d",
								this->create_scene_args_.id, this->create_scene_args_.int_value, it->first);
					}
				} else {
					if (it->first == CAMP_RED) {
						this->red_score_ += CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_score;
					} else if (it->first == CAMP_BLUE) {
						this->blue_score_ += CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_score;
					} else {
						MSG_DEBUG("create_score:(other) scene_index: %ld, int_value: %d, now: %ld, tower_camp: %d",
								this->create_scene_args_.id, this->create_scene_args_.int_value, it->first);
					}
				}
				it_d->second.t += Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_score_tick);
				need_broadcast = true;
				if (red_has_broadcast_will_win_ == false && red_score_ >= 3000) {
					red_has_broadcast_will_win_ = true;
					gang_war_scene_announce(GANG_WAR_WILL_WIN, CAMP_RED, "", gang_name_red_);
				}
				if (blue_has_broadcast_will_win_ == false && blue_score_ >= 3000) {
					blue_has_broadcast_will_win_ = true;
					gang_war_scene_announce(GANG_WAR_WILL_WIN, CAMP_BLUE, "", gang_name_blue_);
				}
			}
		}
	}
	if (need_broadcast) {
		check_over_ = true;
		this->broadcast_gang_war_info();
	}

	while (!material_ready_list_.empty()) {
		Gang_War_Material_Info *material_info = material_ready_list_.front();
		if (material_info->t > now) {
//			MSG_DEBUG("scene_index: %ld, least time: %ld, now: %ld", this->create_scene_args_.id, material_info->t.sec(), now.sec());
			break;
		}
		material_ready_list_.pop_front();
		material_info->t = now + Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_score_tick);
		material_complete_map_[material_info->camp].insert(std::make_pair(material_info->index, *material_info));
		material_ready_set_.erase(material_info->id);
		if (material_info->camp == CAMP_RED) {
			++red_tower_num_;
		} else if (material_info->camp == CAMP_BLUE) {
			++blue_tower_num_;
		} else {
			MSG_DEBUG("scene_index: %ld, int_value: %d, now: %ld", this->create_scene_args_.id, this->create_scene_args_.int_value);
		}
		std::string gang_name;
		Mover *player = this->find_mover_with_layer(0, material_info->role_id);
		if (player && player->player_self()) {
			++player->player_self()->gang_war_info().tower_num;
			gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(player->player_self()->gang_war_info().gang_id);
			send_player_info(player->player_self());
		} else {
			Gang_War_Rank_Detail* rank_info = find_offline_mover(material_info->role_id);
			if(rank_info){
				++rank_info->tower_num;
				gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(rank_info->gang_id);
			}
		}
		for (Role_Id_Set::const_iterator it = material_info->other_role.begin(); it != material_info->other_role.end(); ++it) {
			Mover *player = this->find_mover_with_layer(0, *it);
			if (player && player->player_self()) {
				++player->player_self()->gang_war_info().tower_num;
//				gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(player->gang_war_info().gang_id);
				send_player_info(player->player_self());
			} else {
				Gang_War_Rank_Detail* rank_info = find_offline_mover(*it);
				if(rank_info){
					++rank_info->tower_num;
//					gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(rank_info->gang_id);
				}
			}
		}
		const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(material_info->npc_type_id);
		if (config_cache) {
			gang_war_scene_announce(GANG_WAR_ANNOUNCE_HOLD, material_info->role_id, material_info->role_name,
					config_cache->name, gang_name);
		}

		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->material_self() && it->second->material_self()->npc_record()->config.id == material_info->id) {
					it->second->material_self()->camp() = material_info->camp;
					it->second->material_self()->camp_bak() = 0;
					it->second->material_self()->camp_last() = 0;
					it->second->material_self()->send_appear_addition();
				}
			}
		}

		broadcast_tower_info();
		broadcast_gang_war_info();
	}
}
void Gang_War_Scene::material_insert_sort_list(Gang_War_Material_Info *info) {
	GangWarMaterialInfo_List::iterator it = material_ready_list_.begin();
	for (; it != material_ready_list_.end(); ++it) {
		if ((**it).t > info->t) {
			material_ready_list_.insert(it, info);
			break;
		}
	}
	if (it == material_ready_list_.end()) {
		material_ready_list_.push_back(info);
	}
	material_ready_set_.insert(info->id);
}
void Gang_War_Scene::material_remove_sort_list(Gang_War_Material_Info *info) {
	material_ready_list_.remove(info);
	material_ready_set_.erase(info->id);
}
int8_t Gang_War_Scene::material_get_index_from_point_id(const int point_id) {
	switch (point_id) {
	case Gang_War_Def::RED_ONE: {
		return 1;
	}
	case Gang_War_Def::RED_TWO: {
		return 2;
	}
	case Gang_War_Def::RED_THREE: {
		return 3;
	}
//	case Gang_War_Def::RED_FOUR: {
//		return 4;
//	}
//	case Gang_War_Def::RED_FIVE: {
//		return 5;
//	}
//	case Gang_War_Def::RED_SIX: {
//		return 6;
//	}
	case Gang_War_Def::BLACK_ONE: {
		return 4;
	}
//	case Gang_War_Def::BLUE_ONE: {
//		return 8;
//	}
//	case Gang_War_Def::BLUE_TWO: {
//		return 9;
//	}
//	case Gang_War_Def::BLUE_THREE: {
//		return 10;
//	}
	case Gang_War_Def::BLUE_FOUR: {
		return 5;
	}
	case Gang_War_Def::BLUE_FIVE: {
		return 6;
	}
	case Gang_War_Def::BLUE_SIX: {
		return 7;
	}
	default:
	return -1;
	}
}

int Gang_War_Scene::init_scene_normal(int scene_id, int scene_order, int map_id/* = 0*/){
	Move_Scene_Base::init_scene_normal(scene_id, scene_order, map_id);
	transmit_point = scene_config()->transmit_point;

	const Gang_War_Matched_Gangs_Pair *it_index = SCENE_GANG_GLOBAL_MANAGER->find_matched_gangs_pair(create_scene_args_.id, create_scene_args_.sub_sort);
	if (it_index) {
		gang_id_red_ = it_index->matched_pair.first;
		gang_id_blue_ = it_index->matched_pair.second;
		gang_name_red_ = SCENE_GANG_GLOBAL_MANAGER->gang_name(it_index->matched_pair.first);
		gang_name_blue_ = SCENE_GANG_GLOBAL_MANAGER->gang_name(it_index->matched_pair.second);
		war_total_time_ = CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - (Time_Value::gettimeofday().sec() - it_index->match_time.sec());
	} else {
		war_total_time_ = CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time;
	}
	scene_begin_time_= Time_Value::gettimeofday();
	npc_init(scene_id, 0, 0);
	war_over_time_ = scene_begin_time_ + Time_Value(war_total_time_);
	player_initial_score_ = 0;
	return 0;
}

int Gang_War_Scene::release_scene_normal(void){
	MSG_DEBUG("battle scene release,scene_id:%d", scene_id());
	Move_Scene::release_scene_normal();

	return 0;
}

int Gang_War_Scene::scene_pass_to_logic(void){
	return 0;
}

void Gang_War_Scene::hook_player_enter_battle(Scene_Player* player){
	return;
}

void Gang_War_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters){
	Scene_Player* win_player = NULL;
	Scene_Player* lose_player = NULL;
	if(battle_type == Battle_Scene::BATTLE_TYPE_PVP){
		const int win_count = win_mover_vec.size();
		const int lose_count = lose_mover_vec.size();
		int lose_count_player = 0;
		for(int i=0; i<lose_count; ++i){
			Scene_Player* player  = lose_mover_vec[i]->player_self();
			if(player){
				lose_player = player;
				player->gang_war_info().die_num += 1;
				// back to
				player->gang_war_die_revive();
				player->update_mover_gang_war_info();

				++lose_count_player;
			}
		}

		int win_merit = CONFIG_CACHE_GANG->gang_war_setting_cfg().mercit;
		int contribute = CONFIG_CACHE_GANG->gang_war_setting_cfg().contribute;
		for(int i=0; i<win_count; ++i){
			Scene_Player* player  = win_mover_vec[i]->player_self();
			if(player){
				win_player = player;

				int add_win_merit = player->gang_war_info().add_merit(win_merit);
				int add_win_contribute = player->gang_war_info().add_contribute(contribute);
				player->gang_war_info().kill_num += lose_count_player;

//				win_player->update_mover_gang_war_info();
				player->add_merit_contribute_to_logic(add_win_merit, add_win_contribute);
				// 飘字通知 击杀信息：获取了xx功勋，xx公会贡献。
				player->gang_war_scene_notice(GANG_WAR_NOTICE_KILL, "", "",
						add_win_merit, add_win_contribute, true);

				if (player->dead_status()) {
					player->gang_war_info().die_num += 1;
					player->gang_war_die_revive_winner();
				}
				player->update_mover_gang_war_info();
				send_player_info(player);
			}
		}
		if(!win_player || !lose_player){
			return;
		}
		broadcast_gang_war_info();
	} else if (battle_type == Battle_Scene::BATTLE_TYPE_GANG_WAR_LEADER) {
		const int win_count = win_mover_vec.size();
		const int lose_count = lose_mover_vec.size();
		int lose_count_player = 0;
		role_id_t role_id = 0;
		std::string role_name;
		std::string gang_name;
		for(int i=0; i<lose_count; ++i){
			Scene_Player* player  = lose_mover_vec[i]->player_self();
			if(player){
				Gang_DB_View* gdv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(player->gang_id());
				if (gdv && gdv->leader == player->role_id()) {
					role_id = player->role_id();
					role_name = player->role_name();
				}
				if (gdv) {
					gang_name = gdv->name;
				}
				lose_player = player;
				player->gang_war_info().die_num += 1;
				// back to
				player->gang_war_die_revive();
				player->update_mover_gang_war_info();

				++lose_count_player;
			}
		}

		int win_merit = CONFIG_CACHE_GANG->gang_war_setting_cfg().mercit;
		int contribute = CONFIG_CACHE_GANG->gang_war_setting_cfg().contribute;
		for(int i=0; i<win_count; ++i){
			Scene_Player* player  = win_mover_vec[i]->player_self();
			if(player){
				win_player = player;

				int add_win_merit = player->gang_war_info().add_merit(win_merit);
				int add_win_contribute = player->gang_war_info().add_contribute(contribute);
				player->gang_war_info().kill_num += lose_count_player;

//				win_player->update_mover_gang_war_info();
				player->add_merit_contribute_to_logic(add_win_merit, add_win_contribute);
				// 飘字通知 击杀信息：获取了xx功勋，xx公会贡献。
				player->gang_war_scene_notice(GANG_WAR_NOTICE_KILL, "", "",
						add_win_merit, add_win_contribute, true);

				if (player->dead_status()) {
					player->gang_war_info().die_num += 1;
					player->gang_war_die_revive();
				} else {
					send_player_info(player);
				}
				player->update_mover_gang_war_info();
			}
		}
		if(!win_player || !lose_player){
			return;
		}
		if (win_player) {
			win_player->gang_war_scene_announce(GANG_WAR_ANNOUNCE_DUEL_WIN, role_id, role_name, gang_name);
		} else if (lose_player) {
			lose_player->gang_war_scene_announce(GANG_WAR_ANNOUNCE_DUEL_WIN, role_id, role_name, gang_name);
		}
	}

	return;
}
void Gang_War_Scene::hook_gather_special_handle(Mover* mover, int point_id, int material_type){
	broadcast_tower_info();
	if (mover && mover->player_self()) {
		mover->player_self()->gang_war_set_point_id(0);
		mover->player_self()->update_mover_gang_war_info();
	}
}
void Gang_War_Scene::hook_gather_start_special(Mover* mover, int point_id, int material_type){
	if(material_type == MOVER_T_MATERIAL_GANG_WAR_TOWER){ // 刚采完1分钟内再被敌方采，重置变成己方时间
		Scene_Player* player = mover->player_self();
		if(!player){
			return;
		}
		player->gang_war_set_point_id(point_id);
		player->update_mover_gang_war_info();
		int8_t index = material_get_index_from_point_id(point_id);
		const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(player->gang_id());
		if (!info) {
			return ;
		}
		Index_GangWarMaterialInfo_Map::iterator it = material_hold_map_.find(index);
		if (it != material_hold_map_.end()) {
			GangWarMaterialInfo_List::iterator it_r = material_ready_list_.begin();
			for (; it_r != material_ready_list_.end(); ++it_r) {
				if ((**it_r).index == index) {
					(**it_r).t = Time_Value::gettimeofday() + Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_change_time);
					break;
				}
			}
		}
	}
}
void Gang_War_Scene::hook_gather_success(Mover* mover, int point_id, int material_type){
	Scene_Player* player = mover->player_self();
	if(!player){
		return;
	}
	player->gang_war_set_point_id(0);
	player->update_mover_gang_war_info();
	//
	if(material_type == MOVER_T_MATERIAL_GANG_WAR_TOWER){
		int8_t index = material_get_index_from_point_id(point_id);
		const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(player->gang_id());
		if (!info) {
			return ;
		}
		bool need_insert = false;
		bool is_attack = false;
		Index_GangWarMaterialInfo_Map::iterator it = material_hold_map_.find(index);
		if (it != material_hold_map_.end()) {
			// clear start
			if (info->camp != it->second.camp) {
				bool need_erase = false;
				Index_GangWarMaterialInfo_Map::iterator it_mcm = material_complete_map_[it->second.camp].find(index);
				if (it_mcm != material_complete_map_[it->second.camp].end()) {
					if (it->second.camp == CAMP_RED) {
						--red_tower_num_;
					} else if (it->second.camp == CAMP_BLUE) {
						--blue_tower_num_;
					} else {
						MSG_DEBUG("erase_tower: scene_index: %ld, int_value: %d, now: %ld, tower_camp: %d",
								this->create_scene_args_.id, this->create_scene_args_.int_value, it->second.camp);
					}
					need_erase = true;
				}
				material_complete_map_[it->second.camp].erase(index);
				if (material_complete_map_[it->second.camp].empty()) {
					material_complete_map_.erase(it->second.camp);
				}
				material_remove_sort_list(&(it->second));
//				if (need_erase) {
//					Scene_Player *other_player = SCENE_MONITOR->find_scene_player_by_role_id(it->second.role_id);
//					if (other_player) {
//						--other_player->gang_war_info().tower_num;
//						send_player_info(other_player);
//					} else {
//						Gang_War_Rank_Detail* rank_info = find_offline_mover(mover->role_id());
//						if(rank_info){
//							--rank_info->tower_num;
//						}
//					}
//				}
				broadcast_gang_war_info();
				need_insert = true;
				is_attack = true;
				material_hold_map_.erase(it);
			} else {
				it->second.other_role.insert(player->role_id());
			}
			// clear end
		} else {
			need_insert = true;
		}
		if (need_insert) {
			// insert start
			Gang_War_Material_Info material_info;
			material_info.index = index;
			material_info.id = point_id;
			material_info.t = Time_Value::gettimeofday() + Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().tower_change_time);
			material_info.camp = info->camp;
			material_info.role_id = player->role_id();
			material_info.role_name = player->role_name();
			NPC_Record* record = this->get_npc_record(point_id);
			if (record && !record->config.npc_vec.empty()) {
				material_info.npc_type_id = record->config.npc_vec[0].val_1;
				RoleID_NPC_Map::iterator it_npc = record->alive_npc_map.begin();
				if (it_npc != record->alive_npc_map.end() && it_npc->second->material_self()) {
//					it_npc->second->material_self();
				}
			}

			bool is_self_last_time = false;// 资源塔最后一次是属于本方的
			NPC_Record* nr = this->get_npc_record(point_id);
			RoleID_NPC_Map::iterator it_nr = nr->alive_npc_map.begin();
			for (; it_nr != nr->alive_npc_map.end(); ++it_nr) {
				if (it_nr->second->npc_type_id() == material_info.npc_type_id && it_nr->second->material_self()) {
					if (it_nr->second->material_self()->camp_last() == 0) {
						if (is_attack && it_nr->second->material_self()->camp() != 0) {
							it_nr->second->material_self()->camp_last() = it_nr->second->material_self()->camp();
						}
					} else if (it_nr->second->material_self()->camp_last() != 0 && it_nr->second->material_self()->camp_last() == info->camp) {
						material_info.t = Time_Value::gettimeofday();
						is_self_last_time = true;
					}
					break;
				}
			}

			material_hold_map_.insert(std::make_pair(index, material_info));
			it = material_hold_map_.find(index);
			if (it != material_hold_map_.end()) {
				material_insert_sort_list(&(it->second));
//				if (info->camp == CAMP_RED) {
//					++red_tower_num_;
//				} else {
//					++blue_tower_num_;
//				}
//				++player->gang_war_info().tower_num;
				broadcast_gang_war_info();
//				send_player_info(player);

				if (is_attack) {
					const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(material_info.npc_type_id);
					if (config_cache) {
						if (!is_self_last_time) {
							gang_war_scene_announce(GANG_WAR_ANNOUNCE_ATTACK, player->role_id(), player->role_name(),
								config_cache->name, SCENE_GANG_GLOBAL_MANAGER->gang_name(player->gang_war_info().gang_id));
						} else {
							gang_war_scene_announce(GANG_WAR_ANN_DEFEND, player->role_id(), player->role_name(),
								config_cache->name, SCENE_GANG_GLOBAL_MANAGER->gang_name(player->gang_war_info().gang_id));
						}
					}
				} else {
					const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(material_info.npc_type_id);
					if (config_cache) {
						gang_war_scene_announce(GANG_WAR_ANN_ATTACK_EMPTY_TOWER, player->role_id(), player->role_name(),
							config_cache->name, SCENE_GANG_GLOBAL_MANAGER->gang_name(player->gang_war_info().gang_id));
					}
				}
			}
			// insert end
		}
	}else if(material_type == MOVER_T_MATERIAL_GANG_WAR_REVIVE_BALL){
		// 采集复活加速球
		player->gang_war_gather_revive_ball();
		// 更改刷新点
//		change_random_coord(point_id);
	}

	return;
}

int Gang_War_Scene::scene_status(void) {
	return this->scene_status_;
}

int Gang_War_Scene::req_war_scene_fetch_rank(Scene_Player* player){
	Block_Buffer buf;
	MSG_50170037 msg;
	msg.index = -1;
	std::vector<Gang_War_Rank_Info> info_vec;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				Gang_War_Rank_Info info;
				Gang_War_Rank_Detail &war_info = player->gang_war_info();
				info.role_id = war_info.role_id;
				info.role_name = war_info.role_name;
				info.kill_num = war_info.kill_num;
				info.die_num = war_info.die_num;
				info.hold_spot = war_info.tower_num;
				info.headship = war_info.headship;
				info.force = war_info.force;
				info.gang_name = war_info.gang_name;
				info.camp = war_info.league;
				info_vec.push_back(info);
			}
		}
	}

	for (Offline_Mover_Map::const_iterator it = offline_mover_map_.begin();
			it != offline_mover_map_.end(); ++it) {
		Gang_War_Rank_Info info;
		const Gang_War_Rank_Detail &war_info = it->second;
		info.role_id = war_info.role_id;
		info.role_name = war_info.role_name;
		info.kill_num = war_info.kill_num;
		info.die_num = war_info.die_num;
		info.hold_spot = war_info.tower_num;
		info.headship = war_info.headship;
		info.force = war_info.force;
		info.gang_name = war_info.gang_name;
		info.camp = war_info.league;
		info_vec.push_back(info);
	}
	std::sort(info_vec.begin(), info_vec.end(), Gang_War_Rank_Greater);
//	int info_count = info_vec.size();
//	for(int i=0; i<info_count; ++i){
//		info_vec[i].rank = i+1;
//	}
	msg.rank = info_vec;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

int Gang_War_Scene::req_gang_war_heal(Scene_Player* player){
	if (!player) {
		return -1;
	}
	//check
	const War_Base_Config* base_cfg = CONFIG_CACHE_DUNGEON->war_base_config();
	if(!base_cfg){
		return -1;
	}
	if (player->gang_war_info().skill_cd_time != Time_Value::zero && player->gang_war_info().skill_cd_time > Time_Value::gettimeofday()) {
		return -1;
	}
	// 复活中
	if(player->gang_war_info().revive != 0){
		return ERROR_WAR_SCENE_REVIVE_LIMIT;
	}

	std::vector<Scene_Player*> heal_player;
	Block_Buffer buf;
	MSG_81170006 msg;
	msg.role_id = player->role_id();
	Scene_Player_Set player_set;
	collect_round_fight_player(0, player->grid_coord(), player_set, 200);
	Scene_Player_Set::iterator it;
	for(it = player_set.begin(); it != player_set.end(); ++it){
		if((*it)->role_id() != player->role_id()){
			msg.healed_player_vec.push_back((*it)->role_id());
		}
		heal_player.push_back(*it);
	}
	int heal_count = heal_player.size();
	double heal_rate = 0.3;
	for(int i=0; i<heal_count; ++i){
		heal_player[i]->recover_war_hp(heal_rate);
		heal_player[i]->update_mover_gang_war_info();
	}
	MAKE_MSG_BUF(msg, buf);
	this->broadcast_around(player->mover_detail().layer_id, player->mover_detail().block_index,
			buf);

	player->gang_war_info().skill_cd_time = Time_Value::gettimeofday() + Time_Value(CONFIG_CACHE_GANG->gang_war_setting_cfg().skill_cd);

	send_player_info(player);
	return 0;
}

int Gang_War_Scene::update_move_able_info(Mover* mover){
	mover->clear_moveable_value();
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		if((*it).move_value.empty() == false){
			MSG_81000022 msg;
			Move_Value::iterator s_it;
			for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
				if(s_it->val_2 == MOVE_ABLE_FROM){
					if(s_it->val_1 == GANG_WAR_LIGHT_MOVE_ABLE){
						if(mover->player_self()
								&& mover->player_self()->gang_war_info().league == 1
								&& mover->player_self()->gang_war_info().revive == 0
								&& mover->player_self()->gang_war_info().wait_time < Time_Value::gettimeofday()
										){
							msg.moveable_info_vec.push_back(s_it->val_1);
							int16_t val1 = s_it->val_1;
							mover->add_moveable_value(val1);
						}
					}else if(s_it->val_1 == GANG_WAR_DARK_MOVE_ABLE){
						if(mover->player_self()
								&& mover->player_self()->gang_war_info().league == 2
								&& mover->player_self()->gang_war_info().revive == 0
								&& mover->player_self()->gang_war_info().wait_time < Time_Value::gettimeofday()){
							msg.moveable_info_vec.push_back(s_it->val_1);
							int16_t val1 = s_it->val_1;
							mover->add_moveable_value(val1);
						}
					}else {
						msg.moveable_info_vec.push_back(s_it->val_1);
						int16_t val1 = s_it->val_1;
						mover->add_moveable_value(val1);
					}
				}
			}
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}else{
			MSG_81000022 msg;
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}
		break;
	}
	return 0;
}
int Gang_War_Scene::change_move_value(int16_t mp_value, bool moveable){
	int val_2 = UNMOVE_ABLE_VAL;
	if(moveable){
		val_2 = MOVE_ABLE_FROM;
	}
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		bool exist = false;
		for(Move_Value::iterator s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
			if((*s_it).val_1 == mp_value){
				exist = true;
				(*s_it).val_2 = val_2;
				break;
			}
		}
		if(exist == false){
			Int_Int d_int;
			d_int.val_1 = mp_value;
			d_int.val_2 = val_2;
			(*it).move_value.push_back(d_int);
		}
		//
		for(Mover_Map::iterator player_it = (*it).mover_map_.begin();
				player_it != (*it).mover_map_.end(); ++player_it){
			if(player_it->second->player_self()){
				MSG_81000022 msg;
				Move_Value::iterator s_it;
				for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
					if(s_it->val_2 == MOVE_ABLE_FROM && player_it->second->player_self()){
						if(s_it->val_1 == GANG_WAR_LIGHT_MOVE_ABLE){
							if(player_it->second->player_self()->gang_war_info().league == 1 &&
									player_it->second->player_self()->gang_war_info().revive == 0
									&& player_it->second->player_self()->gang_war_info().wait_time < Time_Value::gettimeofday()
											){
								msg.moveable_info_vec.push_back(s_it->val_1);
								int16_t val1 = s_it->val_1;
								player_it->second->player_self()->add_moveable_value(val1);
							}
						}else if(s_it->val_1 == GANG_WAR_DARK_MOVE_ABLE){
							if(player_it->second->player_self()->gang_war_info().league == 2 &&
									player_it->second->player_self()->gang_war_info().revive == 0
									&& player_it->second->player_self()->gang_war_info().wait_time < Time_Value::gettimeofday()){
								msg.moveable_info_vec.push_back(s_it->val_1);
								int16_t val1 = s_it->val_1;
								player_it->second->player_self()->add_moveable_value(val1);
							}
						}else {
							msg.moveable_info_vec.push_back(s_it->val_1);
							int16_t val1 = s_it->val_1;
							player_it->second->player_self()->add_moveable_value(val1);
						}
					}
				}
				OBJ_SEND_TO_CLIENT(msg, (*player_it->second));
			} else {
				if(moveable){
					player_it->second->add_moveable_value(mp_value);
				}else{
					player_it->second->del_moveable_value(mp_value);
				}
			}
		}
	}

	return 0;
}

void Gang_War_Scene::send_war_info(Mover* mover){
	MSG_81170002 msg;
	Gang_War_Total_Info info;
	info.tower_num = red_tower_num_;
	info.camp = Gang_War_Camp::CAMP_RED;
	info.member_num = red_num_;
	info.resource_num = red_score_;
	info.gang_name = gang_name_red_;

	msg.info.push_back(info);
	info.reset();
	info.tower_num = blue_tower_num_;
	info.camp = Gang_War_Camp::CAMP_BLUE;
	info.member_num = blue_num_;
	info.resource_num = blue_score_;
	info.gang_name = gang_name_blue_;
	msg.info.push_back(info);
	OBJ_SEND_TO_CLIENT(msg, (*mover));
	send_end_time(mover);
}

void Gang_War_Scene::send_player_info(Scene_Player* player){
	player->send_gang_war_player_info();
}

void Gang_War_Scene::broadcast_gang_war_info(void){
	Block_Buffer buf;
	MSG_81170002 msg;
	Gang_War_Total_Info info;
	info.tower_num = red_tower_num_;
	info.camp = Gang_War_Camp::CAMP_RED;
	info.member_num = red_num_;
	info.resource_num = red_score_;
	info.gang_name = gang_name_red_;
	msg.info.push_back(info);
	info.reset();
	info.tower_num = blue_tower_num_;
	info.camp = Gang_War_Camp::CAMP_BLUE;
	info.member_num = blue_num_;
	info.resource_num = blue_score_;
	info.gang_name = gang_name_blue_;
	msg.info.push_back(info);
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	this->broadcast_all_within_battle(0, buf);
//	send_end_time();
}

void Gang_War_Scene::send_end_time(Mover* mover) {
	Block_Buffer buf;
	Time_Value now = Time_Value::gettimeofday();
	MSG_81170000 msg;
	msg.type = 0;
	msg.end_time = war_over_time_.sec() - now.sec() - (CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - CONFIG_CACHE_GANG->gang_war_setting_cfg().ready_time);
	if (msg.end_time <= 0) {
		msg.type = 1;
		msg.end_time = war_over_time_.sec() - now.sec();
	}

	if (mover) {
		OBJ_SEND_TO_CLIENT(msg, (*mover));
	} else {
		MAKE_MSG_BUF(msg, buf);
		this->broadcast_all_within_battle(0, buf);
	}
}

Gang_War_Rank_Detail* Gang_War_Scene::find_offline_mover(role_id_t role_id){
	Offline_Mover_Map::iterator it = offline_mover_map_.find(role_id);
	if(it != offline_mover_map_.end()){
		return &(it->second);
	}
	return NULL;
}

void Gang_War_Scene::add_offline_mover(role_id_t role_id, Gang_War_Rank_Detail& info){
//	info->offline_kick_time = Time_Value::gettimeofday() + Time_Value(OFFLINE_KICK_TIME);
	offline_mover_map_.insert(std::make_pair(role_id, info));
}
void Gang_War_Scene::del_offline_mover(role_id_t role_id){
	offline_mover_map_.erase(role_id);
}

//void Gang_War_Scene::check_recover_mine(void){
//	for(int i=0; i<2; ++i){
//		if(src_recover_coord_[i] != Coord()){
//			Mover_Set mover_set;
//			this->collect_round_mover(0, src_recover_coord_[i], mover_set, SRC_RECOVER_DISTANCE);
//			recover_mine(i+1, mover_set);
//		}
//	}
//}

void Gang_War_Scene::check_over(void){
	check_over_ = false;
	int win_league = 0;
	if(this->red_score_ >= CONFIG_CACHE_GANG->gang_war_setting_cfg().win_score){
		win_league = CAMP_RED;
	}
	if(this->blue_score_ >= CONFIG_CACHE_GANG->gang_war_setting_cfg().win_score){
		win_league = CAMP_BLUE;
	}
	if(win_league != 0){
		war_over(win_league);
	}
}

//bool Gang_War_Scene::check_player_empty(void){
//	bool empty = true;
//	Scene_Layer *layer = scene_layer(0);
//	if (layer) {
//		for (Mover_Map::iterator it = layer->mover_map_.begin();
//				it != layer->mover_map_.end(); ++it) {
//			if (it->second && it->second->player_self()) {
//				empty = false;
//				break;
//			}
//		}
//	}
//	if(empty){
//		recover_time_ = Time_Value::gettimeofday() + Time_Value(300);
//		this->set_scene_status(SCENE_RECOVER_STATUS);
//		SCENE_WAR_MANAGER->honour_field_finish(create_scene_args_.id);
//		return true;
//	}
//	return false;
//}

//void Gang_War_Scene::check_offline_player(const Time_Value& now){
//	Offline_Mover_Map::iterator it;
//	for(it = offline_mover_map_.begin(); it != offline_mover_map_.end(); ++it){
//		if(now >= it->second.offline_kick_time){
//			int league = it->second.league;
//			if(league == 1){
//				light_num_ -= 1;
//			}else{
//				dark_num_ -= 1;
//			}
//			SCENE_WAR_MANAGER->req_leave_war_scene(it->second.role_id);
//			this->broadcast_war_info();
//			offline_mover_map_.erase(it);
//			return;
//		}
//	}
//}

//void Gang_War_Scene::check_inactive_player(const Time_Value& now){
//	Scene_Layer *layer = scene_layer(0);
//	if (layer) {
//		for (Mover_Map::iterator it = layer->mover_map_.begin();
//				it != layer->mover_map_.end(); ++it) {
//			if (it->second && it->second->player_self()) {
//				Scene_Player* player = it->second->player_self();
//				if(now >= player->war_info()->inactive_kikc_time){
//					kick_player(player, 0);
//					return;
//				}
//			}
//		}
//	}
//}

void Gang_War_Scene::war_over(const int win_league){
	Scene_Player* ann_player = NULL;
	MSG_50170037 msg;
	msg.index = -1;
	std::vector<Gang_War_Rank_Info> info_vec;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				// battle
				if(player->battle_scene()){
					player->force_finish_battle_scene();
				}
				Gang_War_Rank_Info info;
				Gang_War_Rank_Detail &war_info = player->gang_war_info();
				info.gang_id = war_info.gang_id;
				info.role_id = war_info.role_id;
				info.role_name = war_info.role_name;
				info.kill_num = war_info.kill_num;
				info.die_num = war_info.die_num;
				info.hold_spot = war_info.tower_num;
				info.headship = war_info.headship;
				info.force = war_info.force;
				info.gang_name = war_info.gang_name;
				info.camp = war_info.league;
				info_vec.push_back(info);
				ann_player = player;
			}
		}
	}
	msg.rank = info_vec;

	for (Offline_Mover_Map::const_iterator it = offline_mover_map_.begin();
			it != offline_mover_map_.end(); ++it) {
		Gang_War_Rank_Info info;
		const Gang_War_Rank_Detail &war_info = it->second;
		info.gang_id = war_info.gang_id;
		info.role_id = war_info.role_id;
		info.role_name = war_info.role_name;
		info.kill_num = war_info.kill_num;
		info.die_num = war_info.die_num;
		info.hold_spot = war_info.tower_num;
		info.headship = war_info.headship;
		info.force = war_info.force;
		info.gang_name = war_info.gang_name;
		info.camp = war_info.league;
		msg.rank.push_back(info);
	}
	std::sort(msg.rank.begin(), msg.rank.end(), Gang_War_Rank_Greater);

	if(ann_player){
		// xx公会获得了此次公会战的胜利！
		std::string gang_name = "";
		if (win_league == CAMP_RED) {
			gang_name = gang_name_red_;
		} else {
			gang_name = gang_name_blue_;
		}
		ann_player->gang_war_scene_announce(GANG_WAR_ANNOUNCE_WAR_WIN, 0,
				gang_name, "",  win_league);
	}

	MSG_81170004 award_win;
	MSG_81170004 award_lose;
	award_win.result = 1;
	award_lose.result = 0;
	Block_Buffer buf_win;
	Block_Buffer buf_lose;
	role_id_t gang_id = 0;
	role_id_t gang_id_lose = 0;
	if (win_league == CAMP_RED) {
		gang_id = gang_id_red_;
		gang_id_lose = gang_id_blue_;
	} else {
		gang_id = gang_id_blue_;
		gang_id_lose = gang_id_red_;
	}

	buf_win.make_message(INNER_SYNC_GANG_WAR_REWARD);
	buf_win.write_int64(gang_id);
	buf_lose.make_message(INNER_SYNC_GANG_WAR_REWARD);
	buf_lose.write_int64(gang_id_lose);
	const Gang_War_Rank_Detail_Cfg *cfg = CONFIG_CACHE_GANG->find_gang_war_rank(SCENE_GANG_GLOBAL_MANAGER->stage(gang_id_red_));
	if (cfg) {
		Reward_Info_Gang_War rigw;
		buf_win.write_uint16(cfg->list.size());
		for (Item_Info_Vec::const_iterator it = cfg->list.begin(); it != cfg->list.end(); ++it) {
			rigw.reset();
			rigw.award.reward_id = it->id;
			rigw.award.quantity = it->amount;
			rigw.award.bind = it->bind;
			award_win.gang.push_back(rigw);
			buf_win.write_int32(it->id);
			buf_win.write_int32(it->amount);
		}

		buf_lose.write_uint16(cfg->lose_list.size());
		for (Item_Info_Vec::const_iterator it = cfg->lose_list.begin(); it != cfg->lose_list.end(); ++it) {
			rigw.reset();
			rigw.award.reward_id = it->id;
			rigw.award.quantity = it->amount;
			rigw.award.bind = it->bind;
			award_lose.gang.push_back(rigw);
			buf_lose.write_int32(it->id);
			buf_lose.write_int32(it->amount);
		}

		Reward_Info award;
		for (Item_Info_Vec::const_iterator it = cfg->self_list.begin(); it != cfg->self_list.end(); ++it) {
			award.reset();
			award.reward_id = it->id;
			award.quantity = it->amount;
			award.bind = it->bind;
			award_win.self.push_back(award);
		}
		for (Item_Info_Vec::const_iterator it = cfg->lose_self_list.begin(); it != cfg->lose_self_list.end(); ++it) {
			award.reset();
			award.reward_id = it->id;
			award.quantity = it->amount;
			award.bind = it->bind;
			award_lose.self.push_back(award);
		}

	} else {
		buf_win.write_uint16(0);
		buf_lose.write_uint16(0);
	}
	buf_win.finish_message();
	buf_lose.finish_message();

	SCENE_GANG_GLOBAL_MANAGER->sync_to_logic(buf_win, gang_id);
	SCENE_GANG_GLOBAL_MANAGER->sync_to_logic(buf_lose, gang_id_lose);

	// 胜负奖励
	Block_Buffer buf_bak;
	int b_count = info_vec.size();
	for(int i=0; i<b_count; ++i){
		Mover* mover = this->find_mover_with_layer(0, info_vec[i].role_id);
		if(mover){
			Scene_Player* player = mover->player_self();
			if(player){
				if(player->gang_war_info().league == win_league){
					OBJ_SEND_TO_CLIENT(award_win, (*player));
				}else{
					OBJ_SEND_TO_CLIENT(award_lose, (*player));
				}
				{
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
			}
		}
	}
	sync_gang_war_result(win_league);
	sync_rank_info(msg.rank, win_league, gang_id, gang_id_lose);
	recover_time_ = Time_Value::gettimeofday() + Time_Value(100);
	this->set_scene_status(SCENE_RECOVER_STATUS);
	kick_all_player();
}

void Gang_War_Scene::kick_all_player(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				player->gang_war_set_point_id(0);
				player->update_mover_gang_war_info();
				player->reset_gang_war_info();
				int dest = 0;
				player->inner_transmit(dest,0,0);
			}
		}
	}
}

int Gang_War_Scene::set_war_over_force(void) {
	int win_league = CAMP_RED;
	if(red_score_ < blue_score_){
		win_league = CAMP_BLUE;
	} else if(red_score_ == blue_score_) {
		int force_red = 0;
		int force_blue = 0;
		Gang_DB_View* gdv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(gang_id_red_);
		if (gdv) {
			force_red = gdv->force_total;
		}
		gdv = SCENE_GANG_GLOBAL_MANAGER->gang_view_by_id(gang_id_blue_);
		if (gdv) {
			force_blue = gdv->force_total;
		}
		if (force_red < force_blue) {
			win_league = CAMP_BLUE;
		}
	}
	war_over(win_league);
	return 0;
}

void Gang_War_Scene::kick_player(Scene_Player* player, int reason){
	int dest = 0;
	player->inner_transmit(dest,0,0);
}

void Gang_War_Scene::war_battle_finish(int win_camp){
	int win_league = win_camp;
	if(win_camp == 0){
		if(this->red_score_ >= this->blue_score_){
			win_league = 1;
		}else{
			win_league = 2;
		}
	}else if(win_camp == 1){
		win_league = 1;
	}else{
		win_league = 2;
	}
	war_over(win_league);
}

void Gang_War_Scene::sync_rank_info(const std::vector<Gang_War_Rank_Info> &vec, const int win_league,
		int64_t gang_id,
		int64_t gang_id_lose) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_RANK_INFO);
	buf.write_int32(create_scene_args_.int_value);
	buf.write_int64(gang_id);
	buf.write_int64(gang_id_lose);
	buf.write_int32(win_league);
	buf.write_uint16(vec.size());
	for (std::vector<Gang_War_Rank_Info>::const_iterator it = vec.begin();
			it != vec.end(); ++it) {
		it->serialize(buf);
	}
	SCENE_GANG_GLOBAL_MANAGER->sync_to_logic_by_area(buf, this->create_scene_args_.sub_sort);
}

void Gang_War_Scene::broadcast_tower_info(Scene_Player *player) {
	MSG_81170005 acv_msg;
	std::map<int8_t, Gang_War_Tower_Detail> tower_map;
	Gang_War_Tower_Detail tower;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->material_self()) {
				int8_t index = material_get_index_from_point_id(it->second->material_self()->npc_record()->config.id);
				if (index > 0) {
					tower.reset();
					tower.color = it->second->material_self()->camp();
					tower.state = it->second->material_self()->is_gathering_camp();

					if (tower.color == CAMP_RED) {
						tower.gang_name_color = gang_name_red_;
					} else if (tower.color == CAMP_BLUE) {
						tower.gang_name_color = gang_name_blue_;
					}
					if (tower.state == CAMP_RED) {
						tower.gang_name_state = gang_name_red_;
					} else if (tower.state == CAMP_BLUE)  {
						tower.gang_name_state = gang_name_blue_;
					}

					tower_map.insert(std::make_pair(index, tower));
				}
			}
		}
	}
	for (int8_t i = 1; i <= 13; ++i) {
		std::map<int8_t, Gang_War_Tower_Detail>::iterator it = tower_map.find(i);
		if (it != tower_map.end()) {
			acv_msg.tower_vec.push_back(it->second);
		} else {
			tower.reset();
			acv_msg.tower_vec.push_back(tower);
		}
	}
	if (player) {
		OBJ_SEND_TO_CLIENT(acv_msg, (*player));
	} else if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				OBJ_SEND_TO_CLIENT(acv_msg, (*it->second->player_self()));
			}
		}
	}
}
void Gang_War_Scene::sync_gang_war_result(const int win_league) {
	const Gang_War_Matched_Gangs_Pair *it_index = SCENE_GANG_GLOBAL_MANAGER->find_matched_gangs_pair(create_scene_args_.id, create_scene_args_.sub_sort);
	if (it_index) {
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_GANG_WAR_RESULT);
//		buf.write_int8(SCENE_GANG_GLOBAL_MANAGER->stage());
		buf.write_uint16(1);
		Gang_War_Spot spot;
		spot.result = static_cast<int8_t>(win_league);
		Gang_Basic_Detail gbd;
		gbd.index = 0;
		spot.index = it_index->matched_index;
		gbd.gang_id = it_index->matched_pair.first;
		gbd.gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(it_index->matched_pair.first);
		{
			const Gang_War_Matched_Gang_Info *gwmgi = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(it_index->matched_pair.first);
			if (gwmgi) {
				gbd.zone = gwmgi->zone;
			}
		}
		spot.gang_vec.push_back(gbd);
		gbd.reset();
		gbd.index = 1;
		gbd.gang_id = it_index->matched_pair.second;
		gbd.gang_name = SCENE_GANG_GLOBAL_MANAGER->gang_name(it_index->matched_pair.second);
		{
			const Gang_War_Matched_Gang_Info *gwmgi = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(it_index->matched_pair.second);
			if (gwmgi) {
				gbd.zone = gwmgi->zone;
			}
		}
		spot.gang_vec.push_back(gbd);
		spot.serialize(buf);
		buf.finish_message();
		SCENE_GANG_GLOBAL_MANAGER->sync_to_logic_by_area(buf, create_scene_args_.sub_sort);
	}
}
int Gang_War_Scene::gang_war_scene_announce(int type, role_id_t role_id, std::string role_name,
		std::string name, std::string gang_name, int value1, int value2, int value3){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	switch(type){
	case GANG_WAR_ANNOUNCE_WAR_WIN:{
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);
		break;
	}
	case GANG_WAR_ANNOUNCE_WAR_START: {
		break;
	}
	case GANG_WAR_ANNOUNCE_ATTACK: {
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			MSG_83000000 msg;
			msg.chat_type = chat_scene;
			msg.system_type = type;
			msg.content = ann_contents;
			role_id_t gang_id = 0;
			Mover *player = this->find_mover_with_layer(0, role_id);
			if (player && player->player_self()) {
				gang_id = player->player_self()->gang_id();
			} else {
				Offline_Mover_Map::iterator it = this->offline_mover_map_.find(role_id);
				if (it != this->offline_mover_map_.end()) {
					gang_id = it->second.gang_id;
				}
			}
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() != gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}

			msg.reset();
			msg.chat_type = chat_scene;
			msg.system_type = GANG_WAR_ANNOUNCE_BE_ATTACK;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() == gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}
		}
		return 0;
		break;
	}
	case GANG_WAR_ANNOUNCE_HOLD: {
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = gang_name;
		ann_contents.push_back(new_content);

		new_content.reset();
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);

		role_id_t gang_id = 0;
		Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
		if (player) {
			gang_id = player->gang_id();
		} else {
			Offline_Mover_Map::iterator it = this->offline_mover_map_.find(role_id);
			if (it != this->offline_mover_map_.end()) {
				gang_id = it->second.gang_id;
			}
		}
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			MSG_83000000 msg;
			msg.chat_type = chat_scene;
			msg.system_type = GANG_WAR_ANNOUNCE_HOLD;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() != gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}
			msg.reset();
			msg.chat_type = chat_scene;
			msg.system_type = GANG_WAR_ANNOUNCE_HOLD_FRIEND;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() == gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}
		}
		return 0;
		break;
	}
	case GANG_WAR_ANN_ATTACK_EMPTY_TOWER: {
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			MSG_83000000 msg;
			msg.chat_type = chat_scene;
			msg.system_type = type;
			msg.content = ann_contents;
			role_id_t gang_id = 0;
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
			if (player) {
				gang_id = player->gang_id();
			} else {
				Offline_Mover_Map::iterator it = this->offline_mover_map_.find(role_id);
				if (it != this->offline_mover_map_.end()) {
					gang_id = it->second.gang_id;
				}
			}
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() != gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}

			msg.reset();
			msg.chat_type = chat_scene;
			msg.system_type = GANG_WAR_ANN_ATTACK_EMPTY_TOWER_SELF;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() == gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}
		}
		return 0;
		break;
	}
	case GANG_WAR_ANN_DEFEND: {
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);

		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			MSG_83000000 msg;
			msg.chat_type = chat_scene;
			msg.system_type = type;
			msg.content = ann_contents;
			role_id_t gang_id = 0;
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
			if (player) {
				gang_id = player->gang_id();
			} else {
				Offline_Mover_Map::iterator it = this->offline_mover_map_.find(role_id);
				if (it != this->offline_mover_map_.end()) {
					gang_id = it->second.gang_id;
				}
			}
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() != gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}

			msg.reset();
			msg.chat_type = chat_scene;
			msg.system_type = GANG_WAR_ANN_DEFEND_SELF;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_id() == gang_id) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}
		}
		return 0;
		break;
	}
	case GANG_WAR_WILL_WIN: {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = name;
		ann_contents.push_back(new_content);

		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			MSG_83000000 msg;
			msg.chat_type = chat_scene;
			msg.system_type = type;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_war_info().league != static_cast<int>(role_id)) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}

			msg.reset();
			msg.chat_type = chat_scene;
			msg.system_type = GANG_WAR_WILL_WIN_SELF;
			msg.content = ann_contents;
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self() && it->second->player_self()->gang_war_info().league == static_cast<int>(role_id)) {
					OBJ_SEND_TO_CLIENT(msg, (*it->second->player_self()));
				}
			}
		}
		return 0;
		break;
	}
	default:{
		break;
	}
	}

	MSG_83000000 msg;
	msg.chat_type = chat_scene;
	msg.system_type = type;
	msg.content = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	broadcast_all_within_battle(0, buf);
	return 0;
}
