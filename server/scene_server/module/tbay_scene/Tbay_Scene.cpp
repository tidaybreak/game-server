/*
 * Tbay_Scene.cpp
 *
 *  Created on: 2015年4月10日
 *      Author: root
 */



#include "Tbay_Scene.h"
#include "Msg_Inner_Struct.h"
#include "mover/Mover.h"
#include "Scene_Player.h"
#include "Config_Cache_Dungeon.h"
#include "Config_Cache_Public.h"
#include "Config_Cache_NPC.h"
#include "Scene_Monitor.h"
#include "Msg_Active_Struct.h"
#include "Msg_Scene_Outpost_Struct.h"
#include "NPC/Drops.h"
#include "NPC/Machine.h"
#include "Battle_Scene.h"
#include "Scene_Def.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache.h"
#include "Config_Cache_Role.h"
#include "single_scene/Single_Scene.h"
#include "single_scene/Single_Scene_Manager.h"
#include "pack/Pack_Struct.h"

const int TBAY_BEGIN_MPT = 1016;

Tbay_Scene::Tbay_Scene() {
	reset();
}

Tbay_Scene::~Tbay_Scene() {
	// TODO Auto-generated destructor stub
}

void Tbay_Scene::reset(void) {
	Move_Scene::reset();

	role_id_ = 0;
	monster_poinit_set_.clear();
	offline_ = false;
	has_scene_pass_box_ = false;
	has_gain_rewards = false;

	single_muid_.reset();
	tbay_state_ = 0;
	tbay_max_time_ = 0;
	gold_num_ = 0;
	gold_type_ = 0;
	bless_index_ = 0;
	bless_buff_id_ = 0;
	tbay_finish_time_ = Time_Value::zero;
	player_ready_ = false;
	player_normal_exit_ = false;
}

void Tbay_Scene::set_role_id(role_id_t role_id) {
	role_id_ = role_id;
}

int Tbay_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);
	gold_type_ = this->create_scene_args_.type;
	tbay_max_time_ = this->create_scene_args_.int_value;
	change_move_value(TBAY_BEGIN_MPT, true);
	return 0;
}

int Tbay_Scene::release_scene_normal(void){
	//MSG_DEBUG("outpost scene release,scene_id:%d, role:%ld", scene_id(), role_id_);
	Move_Scene::release_scene_normal();
	notice_single_scene(!player_normal_exit_);
	return 0;
}

int Tbay_Scene::tick(const Time_Value &now){
	Move_Scene::tick(now);
	if(this->is_recover_status() || !player_ready_){
		return 0;
	}
	switch(tbay_state_){
		case 0:{
			break;
		}
		case 1:{
			if(now >= tbay_finish_time_){
				tbay_state_ = 2;
				tbay_finish_time_ = now + Time_Value(1);
				set_all_drops_invalid();
			}
			break;
		}
		case 2:{
			if(now >= tbay_finish_time_){
				tbay_state_ = 3;
				kick_all_player();
				notice_single_scene(false);
				set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
				recover_time_ = now + Time_Value(5);

			}
			break;
		}
		case 3:{
			kick_all_player();
			set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
			notice_single_scene(false);
			recover_time_ = now + Time_Value(5);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Tbay_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);
	if(!mover || mover->player_self() == NULL){
		return 0;
	}
	set_scene_status(Move_Scene::SCENE_NORMAL_STATUS);
	if(bless_index_ != 0){
		add_tbay_bless(mover->player_self());
	}
	if(tbay_state_ == 2){
		tbay_finish_time_ = Time_Value::gettimeofday() + Time_Value(5);
	}
//	//offline login
//	if(offline_){
//
//	}
//	if(scene_config()->type == Elite_Scene_Type){// 精英副本是否有通关宝箱
//		if(mover && mover->drops_self()){
//			if(mover->drops_self()->is_scene_pass_box()){
//				has_scene_pass_box_ = true;
//			}
//		}
//	}else if(scene_config()->type == FuBen_Scene_Type){
//		if(mover && mover->player_self()){// 单人副本没有体力进入，不需触发战斗就没有收益
//			if(player_enter_battle_set_.count(mover->player_self()->role_id()) == 0){//
//				if(mover->player_self()->fighter_detail().current_strength() <= 0){
//					set_no_profit_player(mover->player_self()->role_id());
//					mover->player_self()->set_no_profit(true);
//				}
//			}
//		}
//	}
	return 0;
}

int Tbay_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);

	if (!mover ||mover->player_self() == NULL){
		return 0;
	}
	if(bless_index_ != 0){
		del_tbay_bless(mover->player_self());
	}
	this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
	if (offline) {
		MSG_DEBUG("outpost scene recover offline, scene id:%ld", this->scene_id());
		recover_time_ = Time_Value::gettimeofday() + scene_config()->scene_keep_time;
		player_normal_exit_ = false;
	}else{
		MSG_DEBUG("outpost scene recover normal, scene id:%ld", this->scene_id());
		recover_time_ = Time_Value::zero;
		player_normal_exit_ = true;
	}
	offline_ = offline;
	player_ready_ = false;
	return 0;
}

int Tbay_Scene::client_scene_complete(Fighter &fighter){
	Move_Scene::client_scene_complete(fighter);
	player_ready_ = true;
	return 0;
}

//int Tbay_Scene::scene_pass_to_logic(void){
//	if(is_scene_pass_ == false){
//		if(is_scene_complete_){
//			this->is_delay_show_ts_point_= true;
//			this->reset_delay_show_ts_point_time();
//		}
//		return -1;
//	}
//	if(has_gain_rewards){
//		return 0;
//	}
//	has_gain_rewards = true;
//	MSG_20200080 msg;
//	if(scene_config() && scene_config()->dungeon.first_layer_id != 0){
//		msg.scene_id = scene_config()->dungeon.first_layer_id;
//	}else{
//		msg.scene_id = scene_id();
//	}
//	Mover *mover = this->find_mover_with_layer(0, role_id_);
//	if (mover) {
//		if(is_no_profit(mover->role_id())){// 没收益
//			msg.result = 0;
//			this->is_delay_show_ts_point_= true;
//			this->reset_delay_show_ts_point_time();
//		}else{// 有收益
//			msg.result = 1;
//			this->is_delay_show_ts_point_= true;
//			if(has_scene_pass_box_ || scene_config()->type == FuBen_Scene_Type){
//				this->ts_point_delay_time_ = Time_Value::gettimeofday() + Time_Value(12);
//			}else{
//				this->reset_delay_show_ts_point_time();
//			}
//
//		}
//		OBJ_SEND_TO_LOGIC(msg, (*mover));
//	}
//	return 0;
//}
//
//void Tbay_Scene::send_path_guide(int point, Scene_Player* player){
//	if(!player){
//		return ;
//	}
//	if(point == 0){
//		int count = transmit_point.size();
//		if(count > 0){
//			MSG_81000016 msg;
//			msg.type = 4;
//			msg.pos_x = transmit_point[0].coord.pixel_coord().x;
//			msg.pos_y = transmit_point[0].coord.pixel_coord().y;
//			msg.point = 0;
//			OBJ_SEND_TO_CLIENT(msg, (*player));
//		}
//		return;
//	}
//	NPC_Record* npc_record = this->get_npc_record(point);
//	if(npc_record == NULL){
//		return;
//	}
//	{
//	MSG_81000016 msg;
//	msg.type = npc_record->config.point_type;
//	msg.pos_x = npc_record->config.birth_coord.pixel_coord().x;
//	msg.pos_y = npc_record->config.birth_coord.pixel_coord().y;
//	msg.point = point;
//	OBJ_SEND_TO_CLIENT(msg, (*player));
//	}
//	return;
//}
//
//void Tbay_Scene::hook_scene_pass(void){
//	Scene_Layer *layer = scene_layer(0);
//	if (layer) {
//		for (Mover_Map::iterator it = layer->mover_map_.begin();
//				it != layer->mover_map_.end(); ++it) {
//			if (it->second) {
//				it->second->hook_scene_pass(this->scene_id());
//			}
//		}
//	}
//}
//
//void Tbay_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters){
//	if(battle_type == Battle_Scene::BATTLE_TYPE_PVE && win_pos == 1){
//		Scene_Layer *layer = scene_layer(0);
//		if (layer) {
//			for (Mover_Map::iterator it = layer->mover_map_.begin();
//					it != layer->mover_map_.end(); ++it) {
//				if (it->second->machine_self() && it->second->machine_self()->is_lift()) {
//					it->second->machine_self()->lift_reset();
//				}
//			}
//		}
//	}
//}

void Tbay_Scene::hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums){
	if(!player){
		return;
	}
	bool notice = false;
	for(Int_Int_Map::const_iterator it = prop.begin(); it != prop.end(); ++it){
		if(it->first == COPPER || it->first == SOULS || it->first == DRAGON_SOUL){
			gold_num_ += it->second;
			notice = true;
		}
	}
	if(notice){
		MSG_50211313 msg;
		msg.gold_num = gold_num_;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
}

void Tbay_Scene::hook_player_move(Scene_Player* player){
	if(tbay_state_ != 0){
		return;
	}
	if(!player){
		return;
	}
	Coord coord = player->grid_coord();
	int mpt_value = mpt_value_by_coord(coord.x, coord.y);
	if(mpt_value != TBAY_BEGIN_MPT){
		tbay_state_ = 1;
		tbay_finish_time_ = Time_Value::gettimeofday() + Time_Value(tbay_max_time_);
		notice_tbay_info(player);
	}
}

void Tbay_Scene::set_single_scene_info(Monitor_Unique_ID& muid){
	single_muid_ = muid;
}

void Tbay_Scene::notice_single_scene(bool clear_single){
	if(role_id_ == 0){
		return;
	}
	Create_Scene_Args tmp_args;
	Move_Scene* scene = SINGLE_SCENE_MANAGER->find_scene_simple(single_muid_, role_id_, tmp_args);
	if(!scene){
		return;
	}
	Single_Scene* single_scene = dynamic_cast<Single_Scene*>(scene);
	if(!single_scene){
		return;
	}
	single_scene->outpost_notice_exit(this->create_scene_args_.id, clear_single);
}

void Tbay_Scene::notice_tbay_info(Scene_Player* player){
	if(!player){
		return;
	}
	MSG_50211310 msg;
	if(tbay_state_ == 0 || tbay_state_ == 1){
		msg.state = tbay_state_;
	}else{
		msg.state = 2;
	}

	if(tbay_state_ == 0){
		msg.remain_time = tbay_max_time_;
	}else if(tbay_state_ == 1){
		msg.remain_time = tbay_finish_time_.sec() - Time_Value::gettimeofday().sec();
		if(msg.remain_time < 0){
			msg.remain_time = 0;
		}
	}else{
		msg.remain_time = 0;
	}
	msg.gold_num = gold_num_;
	msg.gold_type = gold_type_;
	msg.bless_index = bless_index_;
	OBJ_SEND_TO_CLIENT(msg, (*player));
}

int Tbay_Scene::req_tbay_bless(Scene_Player* player, const int8_t bless_index){
	if(bless_index_ != 0){
		return -1;
	}
	Tbay_Gold_Npc_Config* gold_npc_cfg = CONFIG_CACHE_NPC->get_tbay_gold_npc_config(bless_index);
	if(!gold_npc_cfg){
		return -1;
	}

	{
		Block_Buffer buf;
		buf.make_message(INNER_SYNC_SCENE_OUTPOST);
		buf.write_int8(20);
		buf.write_int8(bless_index);
		buf.write_int32(gold_npc_cfg->sys_sell_type);
		buf.write_int32(gold_npc_cfg->cost);
		buf.finish_message();
		player->send_to_logic(buf);
	}
	return 0;
}

int Tbay_Scene::req_fetch_tbay_info(Scene_Player* player){
	notice_tbay_info(player);
	return 0;
}

int Tbay_Scene::inner_tbay_bless(Scene_Player* player, const int8_t bless_index){
	bless_index_ = bless_index;
	add_tbay_bless(player);
	notice_tbay_info(player);
	return 0;
}

void Tbay_Scene::kick_all_player(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		std::vector<Scene_Player*> player_vec;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				player_vec.push_back(it->second->player_self());
			}
		}
		int count = player_vec.size();
		int dest = 0;
		Coord coord;
		Create_Scene_Args tmp_args;
		Move_Scene* scene = SINGLE_SCENE_MANAGER->find_scene_simple(single_muid_, role_id_, tmp_args);
		if(scene){
			dest = single_muid_.id;
			Mover* mover = scene->find_mover_with_layer(0, create_scene_args_.id);
			if(mover){
				coord = mover->grid_coord();
			}
		}

		for(int i = 0; i < count; ++i){
			player_vec[i]->inner_transmit(dest,0,0, "",coord);
			notice_tbay_info(player_vec[i]);
		}
	}
}

void Tbay_Scene::add_tbay_bless(Scene_Player* player){
	if(!player){
		return;
	}
	Tbay_Gold_Npc_Config* cfg = CONFIG_CACHE_NPC->get_tbay_gold_npc_config(bless_index_);
	if(!cfg){
		return;
	}
	Status_Effect_Argv argv;
	argv = cfg->buff_argv;
	argv.caster_id = player->role_id();
	argv.last_time_ms = -1;
	player->insert_status_out_fight(argv);
	bless_buff_id_ = argv.status_id;
	{
		MSG_81003000 msg;
		msg.text = cfg->name;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
	return;
}

void Tbay_Scene::del_tbay_bless(Scene_Player* player){
	if(bless_buff_id_ != 0){
		int result = player->erase_status_type(bless_buff_id_);
		if(result == 0){
			Status_Info_Msg msg_info;
			msg_info.status_id = bless_buff_id_;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = player->role_id();
			MSG_81000071 msg;
			msg.buff_detail.push_back(msg_info);
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
}

void Tbay_Scene::set_all_drops_invalid(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		std::vector<Scene_Player*> player_vec;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->drops_self()) {
				it->second->drops_self()->drops_detail().set_getting_status();
			}
		}
	}

}
