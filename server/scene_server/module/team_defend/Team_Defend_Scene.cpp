/*
 * Team_Defend_Scene.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: linqiyou
 */

#include "Team_Defend_Scene.h"
#include "Msg_Inner_Struct.h"
#include "mover/Mover.h"
#include "fighter/Fighter.h"
#include "Config_Cache_Dungeon.h"
#include "Scene_Player.h"
#include "Msg_Active_Struct.h"
#include "scene_outpost/Scene_Outpost_Manager.h"
#include "Battle_Scene.h"
#include "NPC/Machine.h"
#include "Base_Detail.h"
#include "Msg_Inner_Enum.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_Team.h"
#include "Config_Cache_NPC.h"
#include "pack/Pack_Def.h"
#include "Config_Cache_Gang.h"

const int Team_Defend_Scene::BOSS_WAVE = 4;
Team_Defend_Scene::Team_Defend_Scene() {
	reset();
}

Team_Defend_Scene::~Team_Defend_Scene() {
	// TODO Auto-generated destructor stub
}

void Team_Defend_Scene::reset(void) {
	Move_Scene::reset();

	role_id_ = 0;
	role_vec_.clear();
	monster_poinit_set_.clear();
	has_gain_rewards = false;
	cumulate_award_.clear();
	kill_nums_.clear();
	start_time_ = Time_Value::zero;
	delay_kick_player_.clear();

	end_time_ = Time_Value::zero;
	wave_ = 0;
	point_ids_.clear();
	point_boss_ = 0;
	point_all_.clear();
	point_all_bak_.clear();
	pointid_name_.clear();
	monster_appear_time_ = Time_Value::zero;
	will_appear_point_ = 0;
	hurt_map_.clear();
	be_hurt_map_.clear();
	cure_map_.clear();
}

void Team_Defend_Scene::set_role_id(role_id_t role_id) {
	role_id_ = role_id;
}

int Team_Defend_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	if(mover->mover_type() == MOVER_T_PLAYER && mover->player_self()){
		role_vec_.insert(mover->role_id());
		set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);

		if (scene_config_->type == TEAM_DEFEND_Scene_Type) {
			int id = mover->player_self()->base_detail().rela_scene_id;
			if (0 != id && id == scene_config_->dungeon.id){//多层副本时，取上一层有收益情况
				kill_nums_[mover->role_id()] = mover->player_self()->kill_nums();
				cumulate_award_[mover->role_id()] = mover->player_self()->cumulate_award();
				if (mover->player_self()->start_time() != Time_Value::zero) {
					start_time_ = mover->player_self()->start_time();
				}
			}else{
				int kill_nums = 0;
				Role_Int_Map::const_iterator it_kn = kill_nums_.find(mover->role_id());
				if (it_kn != kill_nums_.end()) {
					kill_nums = it_kn->second;
				}
				Cumulate_Award_Info::const_iterator it_cai = cumulate_award_.find(mover->role_id());
				if (it_cai != cumulate_award_.end()) {
					mover->player_self()->add_cumulate_info(it_cai->second, kill_nums);
				} else {
					mover->player_self()->add_cumulate_info(Int_Int_Map(), kill_nums);
				}
				mover->player_self()->start_time() = start_time_;
			}
		}
		mover->player_self()->cumulate_award().clear();
		mover->player_self()->kill_nums() = 0;
		mover->player_self()->start_time() = Time_Value::zero;
		active_cumulate_award(mover->player_self());
		active_team_def_time(mover->player_self());
		active_team_def_appear_time(mover->player_self());
	}

	return 0;
}

int Team_Defend_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);

	if(mover->mover_type() == MOVER_T_PLAYER && mover->player_self()){
		role_vec_.erase(mover->role_id());
		if (role_vec_.empty()) {
			this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
			if (offline) {
				MSG_DEBUG("single scene recover offline, scene id:%ld", this->scene_id());
				recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
			}else{
				MSG_DEBUG("single scene recover normal, scene id:%ld", this->scene_id());
				recover_time_ = Time_Value::zero;
			}
		}
		int kill_nums = 0;
		Role_Int_Map::const_iterator it_kn = kill_nums_.find(mover->role_id());
		if (it_kn != kill_nums_.end()) {
			kill_nums = it_kn->second;
		}
		Cumulate_Award_Info::const_iterator it_cai = cumulate_award_.find(mover->role_id());
		if (it_cai != cumulate_award_.end()) {
			mover->player_self()->add_cumulate_info(it_cai->second, kill_nums);
		} else {
			mover->player_self()->add_cumulate_info(Int_Int_Map(), kill_nums);
		}
	}
	return 0;
}
int Team_Defend_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene::client_scene_complete(fighter);
	if (fighter.player_self()) {
		active_cumulate_award(fighter.player_self());
		active_team_def_appear_time(fighter.player_self());
		active_team_def_time(fighter.player_self());
		bool need_trans = false;
		if (delay_kick_player_.count(fighter.role_id()) && fighter.player_self()) {
			need_trans = true;
		} else if (0 == fighter.player_self()->get_team_id()) {
			need_trans = true;
		}
		if (need_trans) {
			int dest = 0;
			fighter.player_self()->inner_transmit(dest,0,0);
		}
	}
	return 0;
}
int Team_Defend_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);
	start_time_ = Time_Value::gettimeofday();
	npc_init(scene_id, 0, 0);
	return 0;
}

void Team_Defend_Scene::npc_init(int scene_id, int sort, int sub_sort) {
//	NPC_MANAGER->section_refresh(&npc_section_);
	section_init(scene_id, sort, sub_sort);
}
void Team_Defend_Scene::section_init(int scene_id, int sort, int sub_sort) {
	const Team_Defend_Cfg_Detail * tdcd = CONFIG_CACHE_TEAM->find_team_defend(scene_id);
	if (!tdcd) return;
	end_time_ = start_time_ + Time_Value(tdcd->init_time);

	npc_section_.scene_ = this;
	npc_section_.is_team = scene_config()->dungeon.can_team > 0 ? true:false;
	Monster_Section_Config* s_config = CONFIG_CACHE_NPC->get_monster_section_config(scene_id);
	if(s_config == NULL){
		MSG_USER("error scene_id %d;" scene_id);
		return ;
	}
	Monster_Point_Config_Map::iterator p_it;
	Int_Hash_Set point_ids;
	for(p_it = s_config->point_map.begin(); p_it != s_config->point_map.end(); ++p_it){
		for (std::vector<Int_Int>::const_iterator it_npc = p_it->second.npc_vec.begin();
				it_npc != p_it->second.npc_vec.end(); ++it_npc) {
			if (it_npc->val_1 == tdcd->boss_id) {
				point_boss_ = p_it->first;
				const NPC_Config_Cache * monster_cfg = CONFIG_CACHE_NPC->find_npc_config_cache(it_npc->val_1);
				if(!monster_cfg){
					MSG_TRACE_ABORT("Team_Defend::monster not find(%d)", it_npc->val_1);
					continue;
				}
				pointid_name_.insert(std::make_pair(p_it->first, monster_cfg->name));
				break;
			} else {
				bool has_set = false;
				for (Int_Hash_Set::const_iterator it_ot = tdcd->monster_ids.begin();
						it_ot != tdcd->monster_ids.end(); ++it_ot) {
					if (it_npc->val_1 != *it_ot) {
						continue;
					}
					const NPC_Config_Cache * monster_cfg = CONFIG_CACHE_NPC->find_npc_config_cache(it_npc->val_1);
					if(!monster_cfg){
						MSG_TRACE_ABORT("Team_Defend::monster not find(%d)", it_npc->val_1);
						break;
					}
					pointid_name_.insert(std::make_pair(p_it->first, monster_cfg->name));
					has_set = true;
					break;
				}
				if (has_set) {
					break;
				}
			}
		}

		if (point_boss_ == p_it->first) {
			continue;
		}
		point_all_.insert(p_it->first);
		point_all_bak_.insert(p_it->first);
	}
	Int_Vec point_all;
	point_all.assign(point_all_.begin(), point_all_.end());
	if (point_all.empty()) {
		MSG_TRACE_ABORT("Team_Defend::point_map empty(%d)", scene_id);
		return ;
	}

	if (point_boss_ == 0) {
		MSG_TRACE_ABORT("Team_Defend::point_boss_ empty(%d)", scene_id);
		return ;
	}
	int rand_num = random() % point_all.size();
	p_it = s_config->point_map.find(point_all[rand_num]);
	if (p_it != s_config->point_map.end()) {
		NPC_Record record;
		record.config = p_it->second;
		record.scene = this;
		record.config.refresh_con = 1;
		record.check_refresh = true;
		record.birth_freq = Time_Value::gettimeofday() +
				Time_Value(tdcd->first_wave_time);
		monster_appear_time_ = record.birth_freq;
		will_appear_point_ = p_it->first;
		record.config.refresh_times = 1;
		if (!tdcd->birth_coord.empty()) {
			int rand_coord = random() % tdcd->birth_coord.size();
			record.config.birth_coord = tdcd->birth_coord[rand_coord];
		}
		MSG_USER("Team_Defend_Scene:first x(%d), y(%d)", record.config.birth_coord.pixel_coord().x, record.config.birth_coord.pixel_coord().y);
		npc_section_.record_map[p_it->first] = record;
		point_ids_.insert(p_it->first);
		point_all_.erase(p_it->first);
		++wave_;
	}
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
		npc_section_.group_map[g_it->first] = group;
	}
	npc_section_.path_guide_vec = s_config->path_guide_vec;
}

int Team_Defend_Scene::release_scene_normal(void){
	MSG_DEBUG("team scene release,scene_id:%d, role:%ld", scene_id(), role_id_);
	Move_Scene::release_scene_normal();
	SCENE_OUTPOST_MANAGER->clear_vagrant_store(scene_id());
	return 0;
}

int Team_Defend_Scene::add_new_monster_point(const int point, bool is_boss, Coord point_coord) {
	//刷流浪商人
	SCENE_OUTPOST_MANAGER->add_new_monster_point(this, point, is_boss, point_coord);

	monster_poinit_set_.insert(point);
	const Team_Defend_Cfg_Detail * tdcd = CONFIG_CACHE_TEAM->find_team_defend(scene_id_);
	if (!tdcd || !scene_config()){
		return -1;
	}
	if (point == point_boss_) {
		is_scene_pass_ = true;
		set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_RECEIVE);
		hook_scene_pass();
	} else {
		end_time_ += Time_Value(tdcd->award_time);
		int add_time = 0;

		Monster_Section_Config* s_config = CONFIG_CACHE_NPC->get_monster_section_config(scene_id_);
		if(s_config == NULL){
			MSG_USER("error scene_id %d;" scene_id);
			return -1;
		}
		if (wave_ == BOSS_WAVE) {
			Monster_Point_Config_Map::iterator p_it = s_config->point_map.find(point_boss_);
			if (p_it != s_config->point_map.end()) {
				NPC_Record record;
				record.config = p_it->second;
				record.scene = this;
				record.config.refresh_con = 1;
				record.check_refresh = true;
				record.birth_freq = Time_Value::gettimeofday() +
						Time_Value(tdcd->wave_time);
				monster_appear_time_ = record.birth_freq;
				will_appear_point_ = p_it->first;
				record.config.refresh_times = 1;
				if (!tdcd->birth_coord.empty()) {
					int rand_coord = random() % tdcd->birth_coord.size();
					record.config.birth_coord = tdcd->birth_coord[rand_coord];
				}
				++wave_;
				MSG_USER("Team_Defend_Scene:wave(%d) x(%d), y(%d)", wave_, record.config.birth_coord.pixel_coord().x, record.config.birth_coord.pixel_coord().y);
				npc_section_.record_map[p_it->first] = record;
				point_ids_.insert(p_it->first);
			}
		} else {
			if (point_all_.empty()) {
				point_all_ = point_all_bak_;
			}
			Int_Vec point_all;
			point_all.assign(point_all_.begin(), point_all_.end());
			if (point_all.empty()) {
				MSG_TRACE_ABORT("point_map empty(%d)", scene_id_);
				return -1;
			}
			int rand_num = random() % point_all.size();
			Monster_Point_Config_Map::iterator p_it = s_config->point_map.find(point_all[rand_num]);
			if (p_it != s_config->point_map.end()) {
				NPC_Record record;
				record.config = p_it->second;
				record.scene = this;
				record.config.refresh_con = 1;
				record.check_refresh = true;
				record.birth_freq = Time_Value::gettimeofday() +
						Time_Value(tdcd->wave_time);
				monster_appear_time_ = record.birth_freq;
				will_appear_point_ = p_it->first;
				record.config.refresh_times = 1;
				if (!tdcd->birth_coord.empty()) {
					int rand_coord = random() % tdcd->birth_coord.size();
					record.config.birth_coord = tdcd->birth_coord[rand_coord];
				}
				++wave_;
				MSG_USER("Team_Defend_Scene:wave(%d) x(%d), y(%d)", wave_, record.config.birth_coord.pixel_coord().x, record.config.birth_coord.pixel_coord().y);
				npc_section_.record_map[p_it->first] = record;
				point_all_.erase(p_it->first);
				point_ids_.insert(p_it->first);
			}
			add_time = tdcd->wave_time;
		}
		for (Role_Id_Set::const_iterator it_rv = role_vec_.begin();
				it_rv != role_vec_.end(); ++it_rv) {
			Mover* mover = this->find_mover_with_layer(0, *it_rv);
			if (mover && mover->player_self()) {
				active_team_def_time(mover->player_self(), add_time);
				active_team_def_appear_time(mover->player_self());
			}
		}
	}
	for (Role_Id_Set::const_iterator it_rv = role_vec_.begin();
			it_rv != role_vec_.end(); ++it_rv) {
		Mover* mover = this->find_mover_with_layer(0, *it_rv);
		if (mover && mover->player_self()) {
			active_team_def_boss_info(mover->player_self());
		}
	}
	return 0;
}

void Team_Defend_Scene::hook_scene_pass(void) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				it->second->hook_scene_pass(this->scene_id());
			}
		}
	}
}

int Team_Defend_Scene::scene_pass_to_logic(void){
	if(is_scene_pass_ == false){
		if(this->is_scene_complete_){
			this->is_delay_show_ts_point_= true;
			this->reset_delay_show_ts_point_time();
		}
		return -1;
	}
	if(has_gain_rewards){
		return 0;
	}
	has_gain_rewards = true;
	MSG_20200081 msg;
	msg.result = 0;
	if(scene_config() && scene_config()->dungeon.first_layer_id != 0){
		msg.scene_id = scene_config()->dungeon.first_layer_id;
	}else{
		msg.scene_id = scene_id();
	}
	Scene_Layer *layer = scene_layer(0);
	Mover* mover = NULL;
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()) {
				if(is_no_profit(it->second->role_id())){
					msg.no_profit_player_vec.push_back(it->second->role_id());
				}
				mover = it->second;
			}
		}
	}
	if(mover){
		this->is_delay_show_ts_point_= true;
		this->ts_point_delay_time_ = Time_Value::gettimeofday() + Time_Value(3);
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);

		std::vector<RoleID_Double_Map*> map_all;
		map_all.push_back(&hurt_map_);
		map_all.push_back(&be_hurt_map_);
		map_all.push_back(&cure_map_);
		for (std::vector<RoleID_Double_Map*>::const_iterator it = map_all.begin();
				it != map_all.end(); ++it) {
			buf.write_uint16((*it)->size());
			for (RoleID_Double_Map::const_iterator it_rdm = (*it)->begin();
					it_rdm != (*it)->end(); ++it_rdm) {
				buf.write_int64(it_rdm->first);
				buf.write_double(it_rdm->second);
			}
		}
		buf.finish_message();
		(*mover).send_to_logic(buf);
	}
	return 0;
}

int Team_Defend_Scene::tick(const Time_Value &now){
	Move_Scene::tick(now);
	check_team_transmit();

	if (end_time_ <= now && (pass_status_ == SCENE_PASS_STATUS_NO_START || pass_status_ == SCENE_PASS_STATUS_NO_COMPELTE)) {
		recover_time_ = Time_Value::gettimeofday() + Time_Value(300);
		this->set_scene_status(SCENE_RECOVER_STATUS);
		kick_all_player();
	}
	return 0;
}

void Team_Defend_Scene::send_path_guide(int point, Scene_Player* player){
	if(!player){
		return ;
	}
	if(point == 0){
		int count = transmit_point.size();
		if(count > 0){
			MSG_81000016 msg;
			msg.type = 4;
			msg.pos_x = transmit_point[0].coord.pixel_coord().x;
			msg.pos_y = transmit_point[0].coord.pixel_coord().y;
			msg.point = 0;
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
		return;
	}
	NPC_Record* npc_record = this->get_npc_record(point);
	if(npc_record == NULL){
		return;
	}
	{
	MSG_81000016 msg;
	msg.type = npc_record->config.point_type;
	msg.pos_x = npc_record->config.birth_coord.pixel_coord().x;
	msg.pos_y = npc_record->config.birth_coord.pixel_coord().y;
	msg.point = point;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	}

	return;
}

void Team_Defend_Scene::hook_player_enter_battle(Scene_Player* player){
	std::pair<Role_Id_Set::iterator, bool> ret;
	ret = this->player_enter_battle_set_.insert(player->role_id());
	if(ret.second == true){//第一场战斗开始
		//消耗体力
		Scene_Config::Dungeon_Config_Detail::CostMap::const_iterator it;
		int cost_type = Scene_Config::Dungeon_Config_Detail::COST_PHY_POWER;
		it = this->scene_config_->dungeon.cost_map.find(cost_type);
		if(it != this->scene_config_->dungeon.cost_map.end()){
			int cost_phy_power = it->second;
			if(cost_phy_power > 0){
				if(player->cost_phy_power(cost_phy_power) == 0){// 消耗体力
					// 副本消耗通知
					MSG_81000064 msg;
					msg.scene_id = scene_id_;
					msg.phy_power = cost_phy_power;
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}else{// 没有收益
					set_no_profit_player(player->role_id());
					player->set_no_profit(true);
				}
			}
		}
		// 打副本通知
		if(scene_config_ && scene_config_->dungeon.first_layer_id == 0){
			MSG_20200201 msg;
			msg.act_id = scene_type();
			msg.type = 1;
			Block_Buffer buf;
			buf.make_message(msg.msg_id);
			msg.serialize(buf);
			buf.finish_message();
			player->send_to_logic(buf);
			// 副本通关状态
			this->set_pass_status(Move_Scene::SCENE_PASS_STATUS_NO_COMPELTE, true);
		}
	}
}

void Team_Defend_Scene::hook_battle_finish(int battle_type, int win_pos,
		std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters){
	Scene_Layer *layer = scene_layer(0);
	if(battle_type == Battle_Scene::BATTLE_TYPE_TEAM && win_pos == 1){
		if (layer) {
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second->machine_self() && it->second->machine_self()->is_lift()) {
					it->second->machine_self()->lift_reset();
				}
			}
		}
	}
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second->player_self()) {
				if (0 == it->second->player_self()->get_team_id()) {
					delay_kick_player_.insert(it->second->player_self()->role_id());
				} else {
					RoleID_Double_Map::iterator it_o = hurt_map_.find(it->second->player_self()->role_id());
					if (it_o != hurt_map_.end()) {
						it_o->second += it->second->player_self()->get_ttl_damage_in_battle();
					} else {
						hurt_map_[it->second->player_self()->role_id()] = it->second->player_self()->get_ttl_damage_in_battle();
					}
					it_o = be_hurt_map_.find(it->second->player_self()->role_id());
					if (it_o != be_hurt_map_.end()) {
						it_o->second += it->second->player_self()->get_ttl_be_damanged_in_battle();
					} else {
						be_hurt_map_[it->second->player_self()->role_id()] = it->second->player_self()->get_ttl_be_damanged_in_battle();
					}
					it_o = cure_map_.find(it->second->player_self()->role_id());
					if (it_o != cure_map_.end()) {
						it_o->second += it->second->player_self()->get_ttl_cure_in_battle();
					} else {
						cure_map_[it->second->player_self()->role_id()] = it->second->player_self()->get_ttl_cure_in_battle();
					}
				}
			}
		}
	}
}
void Team_Defend_Scene::hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums) {
	if (!player || (prop.empty() && kill_monster_nums == 0)) {
		return;
	}
	Cumulate_Award_Info::iterator it = cumulate_award_.find(player->role_id());
	if (it != cumulate_award_.end()) {
		for (Int_Int_Map::const_iterator it_prop = prop.begin(); it_prop != prop.end(); ++it_prop) {
			it->second[it_prop->first] += it_prop->second;
		}
	} else {
		if (!cumulate_award_.insert(std::make_pair(player->role_id(), prop)).second) {
			return;
		}
	}
	{
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_TEAM_CUMULATE_AWARD);
		//
//		Team_Fb_Scene_Type   	= 80204, 	   /// 组队推图副本
//		HERO_DREAM_Scene_Type   = 80205, 	   /// 英雄梦境副本
		buf.write_int32(scene_config_->type);
		buf.write_int64(start_time_.sec());
//		start_time_.serialize(buf);
		buf.write_uint16(prop.size());
		for (Int_Int_Map::const_iterator it = prop.begin(); it != prop.end(); ++it) {
			buf.write_int32(it->first);
			buf.write_int32(it->second);
		}
		buf.finish_message();
		player->send_to_logic(buf);
	}
	Role_Int_Map::iterator it_kn = kill_nums_.find(player->role_id());
	if (it_kn != kill_nums_.end()) {
		it_kn->second += kill_monster_nums;
	} else {
		kill_nums_.insert(std::make_pair(player->role_id(), kill_monster_nums));
	}
	active_cumulate_award(player);
	return;
}
void Team_Defend_Scene::active_cumulate_award(Scene_Player* player) {
	if (!player) {
		return ;
	}
	active_team_def_boss_info(player);
//	MSG_80100325 acv_msg;
//	if (scene_config_->type == Team_Fb_Scene_Type) {
//		acv_msg.type = 0;
//	} else if (scene_config_->type == HERO_DREAM_Scene_Type) {
//		acv_msg.type = 1;
//		Role_Int_Map::const_iterator it_kn = kill_nums_.find(player->role_id());
//		if (it_kn != kill_nums_.end()) {
//			acv_msg.kill_nums = it_kn->second;
//		}
//	} else if(scene_config_->type == TEAM_DEFEND_Scene_Type) {
//		return ;
//	} else {
//		return;
//	}
//	Cumulate_Award_Info::iterator it = cumulate_award_.find(player->role_id());
//	if (it != cumulate_award_.end()) {
//		Reward_Info ri;
//		for (Int_Int_Map::const_iterator it_prop = it->second.begin(); it_prop != it->second.end(); ++it_prop) {
//			if (it_prop->first < 1000000
//					&& (it_prop->first != Pack::COPPER
//							&& it_prop->first != Pack::SOULS)) {
//				continue;
//			}
//			ri.reset();
//			ri.reward_id = it_prop->first;
//			ri.quantity = it_prop->second;
//			acv_msg.award.push_back(ri);
//		}
//	}
//	OBJ_SEND_TO_CLIENT(acv_msg, (*player));
	return;
}

int Team_Defend_Scene::active_team_def_time(Scene_Player *pl, const int add_time) {
	if (!pl) {
		return -1;
	}
	MSG_80100327 res_msg;
	res_msg.time = end_time_.sec();
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Team_Defend_Scene::active_team_def_boss_info(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}
	MSG_80100326 res_msg;
	Int_String is;
	for (Int_String_Map::const_iterator it = pointid_name_.begin();
			it != pointid_name_.end(); ++it) {
		is.reset();
		if (!point_ids_.count(it->first)) {
			is.str_value = CONFIG_CACHE_GANG->team_defend_boss_name();
		} else {
			is.str_value = it->second;
		}
		is.int_value = (monster_poinit_set_.count(it->first) > 0);
		res_msg.info.push_back(is);
	}

	Cumulate_Award_Info::iterator it = cumulate_award_.find(pl->role_id());
	if (it != cumulate_award_.end()) {
		Reward_Info ri;
		for (Int_Int_Map::const_iterator it_prop = it->second.begin(); it_prop != it->second.end(); ++it_prop) {
			if (it_prop->first < 1000000
					&& (it_prop->first != Pack::COPPER
							&& it_prop->first != Pack::SOULS)) {
				continue;
			}
			ri.reset();
			ri.reward_id = it_prop->first;
			ri.quantity = it_prop->second;
			res_msg.reward.push_back(ri);
		}
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

int Team_Defend_Scene::active_team_def_appear_time(Scene_Player *pl) {
	if (!pl) {
		return -1;
	}
	MSG_80100328 res_msg;
	Int_String_Map::const_iterator it = pointid_name_.find(will_appear_point_);
	if (it != pointid_name_.end()) {
		res_msg.name = it->second;
	}
	res_msg.the_time = monster_appear_time_.sec() - Time_Value::gettimeofday().sec();
	if (res_msg.the_time <= 0) {
		res_msg.the_time = 0;
	}
	OBJ_SEND_TO_CLIENT(res_msg, (*pl));
	return 0;
}

void Team_Defend_Scene::kick_all_player(void) {
	MSG_80100329 acv_msg;
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
				{
					OBJ_SEND_TO_CLIENT(acv_msg, (*player));
				}
				int dest = 0;
				player->inner_transmit(dest,0,0);
			}
		}
	}
}
void Team_Defend_Scene::check_team_transmit(void){
	if(scene_config_ && (this->scene_config_->dungeon.relation != 0 && this->scene_config_->dungeon.relation != -1)&&
			is_scene_complete_){
		int count = transmit_point.size();
		if(count > 0){
			Coord coord = transmit_point[0].coord;
			int dest_scene = transmit_point[0].des_scene_id;
			std::vector<Scene_Player*> player_vec;
			Scene_Layer *layer = scene_layer(0);
			if (layer) {
				for (Mover_Map::iterator it = layer->mover_map_.begin();
						it != layer->mover_map_.end(); ++it) {
					if (it->second->player_self()) {
						player_vec.push_back(it->second->player_self());
					}
				}
			}
			int p_count = player_vec.size();
			if(p_count >= 2){
				bool team_is_near = true;
				for(int i = 0; i < p_count; ++i){
					if (is_in_range(coord.grid_coord(), player_vec[i]->grid_coord(), 150) == false) {
						team_is_near = false;
						break;
					}
				}
				if(team_is_near){
					bool one_is_in = false;
					for(int i = 0; i < p_count; ++i){
						if (is_in_range(coord.grid_coord(), player_vec[i]->grid_coord(), 80) == true) {
							one_is_in = true;
							break;
						}
					}
					if(one_is_in){
						for(int i = 0; i < p_count; ++i){
							player_vec[i]->inner_transmit(dest_scene, 0, 0, "");
						}
					}
				}
			}
		}
	}
	return;
}

void Team_Defend_Scene::hook_monster_point_clear(const int point_id){
	//发送指引路径点
	if (this->npc_section_.path_guide_vec.empty() == false) {
		int point = this->npc_section_.get_next_path_guide();
		if (point != -1) {
			Scene_Layer *layer = scene_layer(0);
			if (layer) {
				for (Mover_Map::iterator it = layer->mover_map_.begin();
						it != layer->mover_map_.end(); ++it) {
					if (it->second->player_self()) {
						send_path_guide(point, it->second->player_self());
					}
				}
			}
		}
	}
}
