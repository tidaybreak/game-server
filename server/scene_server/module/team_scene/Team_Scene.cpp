/*
 * Team_Scene.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#include "Team_Scene.h"
#include "Msg_Inner_Struct.h"
#include "mover/Mover.h"
#include "fighter/Fighter.h"
#include "Config_Cache_Dungeon.h"
#include "scene_outpost/Scene_Outpost_Manager.h"
#include "Scene_Player.h"
#include "Msg_Active_Struct.h"
#include "Battle_Scene.h"
#include "NPC/Machine.h"
#include "Base_Detail.h"
#include "Msg_Inner_Enum.h"
#include "team/Team_Def.h"

Team_Scene::Team_Scene() {
	reset();
}

Team_Scene::~Team_Scene() {
	// TODO Auto-generated destructor stub
}

void Team_Scene::reset(void) {
	Move_Scene::reset();

	role_id_ = 0;
	role_vec_.clear();
	monster_poinit_set_.clear();
	has_gain_rewards = false;
	cumulate_award_.clear();
	kill_nums_.clear();
	start_time_ = Time_Value::zero;
	delay_kick_player_.clear();
}

void Team_Scene::set_role_id(role_id_t role_id) {
	role_id_ = role_id;
}

int Team_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	if(mover->mover_type() == MOVER_T_PLAYER && mover->player_self()){
		role_vec_.insert(mover->role_id());
		set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);

		if (scene_config_->type == Team_Fb_Scene_Type
				|| scene_config_->type == HERO_DREAM_Scene_Type
				|| scene_config_->type == TEAM_DEFEND_Scene_Type) {
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
	}

	return 0;
}

int Team_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);

	if(mover->mover_type() == MOVER_T_PLAYER && mover->player_self()){
		role_vec_.erase(mover->role_id());
		if (role_vec_.empty()) {
			this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
//			if (offline) {
//				MSG_DEBUG("single scene recover offline, scene id:%ld", this->scene_id());
				recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
//			}else{
//				MSG_DEBUG("single scene recover normal, scene id:%ld", this->scene_id());
//				recover_time_ = Time_Value::zero;
//			}
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
int Team_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene::client_scene_complete(fighter);
	if (fighter.player_self()) {
		active_cumulate_award(fighter.player_self());
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

		if (scene_id() == TEAM_SPORTS_SCENE_ID)
			fighter.player_self()->notice_team_rank();
	}
	return 0;
}
int Team_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);
	start_time_ = Time_Value::gettimeofday();
	return 0;
}

int Team_Scene::release_scene_normal(void){
	MSG_DEBUG("team scene release,scene_id:%d, role:%ld", scene_id(), role_id_);
	Move_Scene::release_scene_normal();
	SCENE_OUTPOST_MANAGER->clear_vagrant_store(scene_id());
	return 0;
}

int Team_Scene::add_new_monster_point(const int point, bool is_boss, Coord point_coord) {
	//刷流浪商人
	SCENE_OUTPOST_MANAGER->add_new_monster_point(this, point, is_boss, point_coord);

	monster_poinit_set_.insert(point);
	if(scene_config()){
		if(scene_config()->dungeon.pass_con == SCENE_PASS_CON_KILL_ALL){
			int count = monster_poinit_set_.size();
			if (count == npc_section_.get_monster_point_count()) {
				is_scene_complete_ = true;
				if(scene_config()->dungeon.relation == 0 ||
						scene_config()->dungeon.relation == -1){
					is_scene_pass_ = true;
					// 副本通关状态
					set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_RECEIVE);
					hook_scene_pass();
				}
			}
		}else if(scene_config()->dungeon.pass_con == SCENE_PASS_CON_KILL_BOSS){
			if(is_boss){
				is_scene_complete_ = true;
				if(scene_config()->dungeon.relation == 0 ||
						scene_config()->dungeon.relation == -1){
					is_scene_pass_ = true;
					// 副本通关状态
					set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_RECEIVE);
					hook_scene_pass();
				}
			}
		}
	}

	return 0;
}

void Team_Scene::hook_scene_pass(void) {
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

int Team_Scene::scene_pass_to_logic(void){
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
//				if (it->second->player_self()->exp_sum() > 0) {
//					role_id_double.insert(std::make_pair(it->second->player_self()->role_id(),
//							it->second->player_self()->exp_sum()));
//				}
				MSG_USER("LOG: Team_FB complete by role(%ld).", it->second->player_self()->role_id());
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
//		OBJ_SEND_TO_LOGIC(msg, (*mover));
	}
	return 0;
}

int Team_Scene::tick(const Time_Value &now){
	Move_Scene::tick(now);
	check_team_transmit();
	return 0;
}

void Team_Scene::send_path_guide(int point, Scene_Player* player){
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

void Team_Scene::hook_player_enter_battle(Scene_Player* player){
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

void Team_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters){
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
void Team_Scene::hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums) {
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
void Team_Scene::active_cumulate_award(Scene_Player* player) {
	if (!player) {
		return ;
	}
	MSG_80100325 acv_msg;
	if (scene_config_->type == Team_Fb_Scene_Type) {
		acv_msg.type = 0;
	} else if (scene_config_->type == HERO_DREAM_Scene_Type) {
		acv_msg.type = 1;
		Role_Int_Map::const_iterator it_kn = kill_nums_.find(player->role_id());
		if (it_kn != kill_nums_.end()) {
			acv_msg.kill_nums = it_kn->second;
		}
	} else if(scene_config_->type == TEAM_DEFEND_Scene_Type) {
		return ;
	} else {
		return;
	}
	Cumulate_Award_Info::iterator it = cumulate_award_.find(player->role_id());
	if (it != cumulate_award_.end()) {
		Reward_Info ri;
		for (Int_Int_Map::const_iterator it_prop = it->second.begin(); it_prop != it->second.end(); ++it_prop) {
			ri.reset();
			ri.reward_id = it_prop->first;
			ri.quantity = it_prop->second;
			acv_msg.award.push_back(ri);
		}
	}
	OBJ_SEND_TO_CLIENT(acv_msg, (*player));
	return;
}

void Team_Scene::check_team_transmit(void){
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

void Team_Scene::hook_monster_point_clear(const int point_id){
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
