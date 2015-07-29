/*
 * Single_Scene.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#include "Single_Scene.h"
#include "Msg_Inner_Struct.h"
#include "mover/Mover.h"
#include "Scene_Player.h"
#include "Config_Cache_Dungeon.h"
#include "Config_Cache_Public.h"
#include "Scene_Monitor.h"
#include "Msg_Active_Struct.h"
#include "scene_outpost/Scene_Outpost_Manager.h"
#include "NPC/Drops.h"
#include "NPC/Machine.h"
#include "Battle_Scene.h"
#include "Scene_Def.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache.h"
#include "Config_Cache_Role.h"

Single_Scene::Single_Scene() {
	reset();
}

Single_Scene::~Single_Scene() {
	// TODO Auto-generated destructor stub
}

void Single_Scene::reset(void) {
	Move_Scene::reset();

	role_id_ = 0;
	offline_ = false;
	plot_id_ = "";
	plot_step_ = PLOT_NULL;
	plot_record_id_ = 0;
	has_scene_pass_box_ = false;
	has_gain_rewards = false;
	is_double_income_ = false;

	outpost_role_id_ = 0;
}

void Single_Scene::set_role_id(role_id_t role_id) {
	role_id_ = role_id;
}

int Single_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);
	if(scene_type() == Elite_Scene_Type){
		if(this->create_scene_args_.type == 1){
			is_double_income_ = true;
		}else{
			is_double_income_ = false;
		}
	}else{
		scene_param_1_ = 0;
		scene_param_2_ = CONFIG_CACHE_PUBLIC->get_public_default_value(22);		// 单人副本免费复活次数
		scene_param_3_ = CONFIG_CACHE_PUBLIC->get_public_default_value(23);		// 单人副本付费复活消耗钻石
	}
	return 0;
}

int Single_Scene::release_scene_normal(void){
	//MSG_DEBUG("single scene release,scene_id:%d, role:%ld", scene_id(), role_id_);

	Move_Scene::release_scene_normal();

	SCENE_OUTPOST_MANAGER->clear_vagrant_store(scene_id());
	return 0;
}

int Single_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);
	if(mover->player_self() && scene_param_1_ == 1){
		mover->player_self()->set_revive_info(true, Time_Value::max);
	}
	//offline login
	if(offline_){
		if(mover->player_self()){
			mover->player_self()->set_battle_plot_data(plot_id_, plot_step_, plot_record_id_);
		}
	}
	if(scene_config()->type == Elite_Scene_Type){// 精英副本是否有通关宝箱
		if(mover && mover->drops_self()){
			if(mover->drops_self()->is_scene_pass_box()){
				has_scene_pass_box_ = true;
			}
		}
	}else if(scene_config()->type == FuBen_Scene_Type){
		if(mover && mover->player_self()){// 单人副本没有体力进入，不需触发战斗就没有收益
			if(player_enter_battle_set_.count(mover->player_self()->role_id()) == 0){//
				if(mover->player_self()->fighter_detail().current_strength() <= 0){
					set_no_profit_player(mover->player_self()->role_id());
					mover->player_self()->set_no_profit(true);
				}
			}
		}
	}
	return 0;
}

int Single_Scene::exit_scene(Mover *mover, bool offline) {
	int target_scene_id = 0;
	if(mover && mover->player_self()){
		target_scene_id = mover->player_self()->base_detail().rela_scene_id;
	}
	Move_Scene::exit_scene(mover, offline);

	if (mover->player_self() == NULL){
		return 0;
	}
	mover->player_self()->reset_revive_info();
	//
	if(target_scene_id/1000 == Tbay_Scene_Type){
		this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
		recover_time_ = Time_Value::gettimeofday() + Time_Value(60*60);
	}else{
		this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
		if (offline) {
			MSG_DEBUG("single scene recover offline, scene id:%ld", this->scene_id());
			recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
		}else{
			MSG_DEBUG("single scene recover normal, scene id:%ld", this->scene_id());
			recover_time_ = Time_Value::zero;
			// 上古副本通关没有领取宝箱的，自动领取飘字提示
			// 3：通关未领宝箱
			if(scene_type() == Elite_Scene_Type &&
					get_pass_status() == 3){
				if(has_scene_pass_box_){
					Scene_Layer *layer = scene_layer(0);
					if (layer) {
						Mover_Map::iterator it;
						for(it = layer->mover_map_.begin(); it != layer->mover_map_.end(); ++it){
							if(it->second && it->second->drops_self()){
								if(it->second->drops_self()->is_scene_pass_box()){
									it->second->drops_self()->drops_detail().set_getting_status();
									//send to logic
									MSG_20300000 logic_msg;
									logic_msg.drops_role_id = mover->role_id();
									logic_msg.item_index = -1;
									it->second->drops_self()->get_drops_goods_by_index(mover->player_self(), -1, logic_msg.item_vec);
									logic_msg.scene_type = scene_type();
									logic_msg.scene_id = scene_id();

									Block_Buffer logic_buf;
									MAKE_MSG_BUF(logic_msg, logic_buf);
									mover->send_to_logic(logic_buf);
									break;
								}
							}
						}
					}
				}
			}
		}
		offline_ = offline;
	}
	return 0;
}

int Single_Scene::add_new_monster_point(const int point, bool is_boss, Coord point_coord) {
	Move_Scene::add_new_monster_point(point, is_boss, point_coord);
	//刷流浪商人
	SCENE_OUTPOST_MANAGER->add_new_monster_point(this, point, is_boss, point_coord);

	if(scene_config()){
		// 国内任务特殊处理
		if(CONFIG_CACHE->server_maintainer_cache().game_ver_type == 0){
			if(scene_config()->scene_id == CONFIG_CACHE_ROLE->get_single_first_id() &&
					this->create_scene_args_.type == 0){// 特定副本首通加
				Mover* mover = this->find_mover_with_layer(0, role_id_);
				if(mover && mover->player_self()){
					if(point == 1){// 临时英雄出战
						Block_Buffer buf;
						buf.make_message(INNER_HERO_TMP_OUT_FIGHT);
						buf.write_int8(0);
						buf.write_int32(60210021);
						buf.finish_message();
						mover->player_self()->send_to_logic(buf);
					}else if(point == 2){// 删除临时英雄
						Block_Buffer buf;
						buf.make_message(INNER_HERO_TMP_OUT_FIGHT);
						buf.write_int8(1);
						buf.write_int32(60210021);
						buf.finish_message();
						mover->player_self()->send_to_logic(buf);
					}
				}
			}
		}else{
//			if(scene_config()->scene_id == 80201002
//					&& this->create_scene_args_.type == 0){// 特定副本首通加
//				Mover* mover = this->find_mover_with_layer(0, role_id_);
//				if(mover && mover->player_self()){
//					if(point == 2){// 临时英雄出战
//						Block_Buffer buf;
//						buf.make_message(INNER_HERO_TMP_OUT_FIGHT);
//						buf.write_int8(0);
//						buf.write_int32(60210021);
//						buf.finish_message();
//						mover->player_self()->send_to_logic(buf);
//					}else if(point == 3){// 删除临时英雄
//						Block_Buffer buf;
//						buf.make_message(INNER_HERO_TMP_OUT_FIGHT);
//						buf.write_int8(1);
//						buf.write_int32(60210021);
//						buf.finish_message();
//						mover->player_self()->send_to_logic(buf);
//					}
//				}
//			}
		}

		if(scene_config()->dungeon.pass_con == SCENE_PASS_CON_KILL_ALL){
			if (is_monster_point_all_clear()) {
				is_scene_complete_ = true;
				if(scene_config()->dungeon.relation == 0 ||
						scene_config()->dungeon.relation == -1){
					is_scene_pass_ = true;
					// 副本通关状态
					if(has_scene_pass_box_){// 精英的为没有领取宝箱的状态
						set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_NO_RECEIVE);
					}else{
						set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_RECEIVE);
					}
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
					if(has_scene_pass_box_){// 精英的为没有领取宝箱的状态
						set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_NO_RECEIVE);
					}else{
						set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_RECEIVE);
					}
					hook_scene_pass();
				}
			}
		}
	}
	return 0;
}

int Single_Scene::scene_pass_to_logic(void){
	if(is_scene_pass_ == false){
		if(is_scene_complete_){
			this->is_delay_show_ts_point_= true;
			this->reset_delay_show_ts_point_time();
		}
		return -1;
	}
	if(has_gain_rewards){
		return 0;
	}
	has_gain_rewards = true;
	MSG_20200080 msg;
	if(scene_config() && scene_config()->dungeon.first_layer_id != 0){
		msg.scene_id = scene_config()->dungeon.first_layer_id;
	}else{
		msg.scene_id = scene_id();
	}
	Mover *mover = this->find_mover_with_layer(0, role_id_);
	if (mover) {
		if(is_no_profit(mover->role_id())){// 没收益
			msg.result = 0;
			this->is_delay_show_ts_point_= true;
			this->reset_delay_show_ts_point_time();
		}else{// 有收益
			msg.result = 1;
			this->is_delay_show_ts_point_= true;
			if(has_scene_pass_box_ || scene_config()->type == FuBen_Scene_Type){
				this->ts_point_delay_time_ = Time_Value::gettimeofday() + Time_Value(12);
			}else{
				this->reset_delay_show_ts_point_time();
			}

		}
		OBJ_SEND_TO_LOGIC(msg, (*mover));
	}
	return 0;
}

void Single_Scene::send_path_guide(int point, Scene_Player* player){
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

void Single_Scene::save_battle_plot_date(std::string plot_id, int plot_step, int record_id){
	if(plot_step == PLOT_START_REQ){
		this->plot_id_ = plot_id;
		this->plot_record_id_ = record_id;
		this->plot_step_ = plot_step;
		return;
	}
	if(plot_step == PLOT_BEGIN){
		this->plot_id_ = plot_id;
		this->plot_step_ = plot_step;
		return;
	}
	//reset;
	this->plot_id_ = "";
	this->plot_record_id_ = 0;
	this->plot_step_ = PLOT_NULL;
}

void Single_Scene::hook_player_enter_battle(Scene_Player* player){
	if(!player){
		return;
	}
	std::pair<Role_Id_Set::iterator, bool> ret;
	ret = this->player_enter_battle_set_.insert(player->role_id());
	if(ret.second == true){//第一场战斗开始
		Scene_Config::Dungeon_Config_Detail::CostMap::const_iterator it;
		int cost_type = Scene_Config::Dungeon_Config_Detail::COST_PHY_POWER;
		it = this->scene_config_->dungeon.cost_map.find(cost_type);
		if(it != this->scene_config_->dungeon.cost_map.end()){//消耗体力
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
			this->set_pass_status(Move_Scene::SCENE_PASS_STATUS_NO_COMPELTE);
		}
	}
}
void Single_Scene::hook_scene_pass(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second) {
				it->second->hook_scene_pass(this->scene_id());
			}
		}
	}
}

void Single_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters){
	if(battle_type == Battle_Scene::BATTLE_TYPE_PVE && win_pos == 1){
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second->machine_self() && it->second->machine_self()->is_lift()) {
					it->second->machine_self()->lift_reset();
				}
			}
		}
	}
}

bool Single_Scene::is_double_income(void){
	return is_double_income_;
}

void Single_Scene::set_double_income(const bool value){
	is_double_income_ = value;
}

void Single_Scene::hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums){
	if(!player){
		return;
	}
	this->hook_player_enter_battle(player);
}

void Single_Scene::outpost_notice_exit(const int64_t monster_role_id, bool clear_single){
	if(monster_role_id != 0){
		Mover* mover = this->find_mover_with_layer(0, monster_role_id);
		if(mover && mover->monster_self()){
			mover->exit_move_scene();
		}
	}
	if(clear_single){
		this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
		MSG_DEBUG("single scene recover outpost notice, scene id:%ld", this->scene_id());
		recover_time_ = Time_Value::zero;
	}
}
