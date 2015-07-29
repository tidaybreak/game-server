/*
 * World_Boss_Scene.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#include "World_Boss_Scene.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_World_Boss.h"
#include "world_boss/World_Boss_Manager.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"
#include "pack/Pack_Struct.h"
#include "Battle_Scene.h"
#include "announce/Scene_Announce_Manager.h"
#include "chater/Chater_Def.h"
#include "announcement/Announcement_Struct.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Arena.h"
#include "math.h"

const int World_Boss_Revive_Move_Able = 21;
const int world_boss_act_icon_id = 16;

World_Boss_Scene::World_Boss_Scene(void){
	reset();
}

World_Boss_Scene::~World_Boss_Scene(void){

}

void World_Boss_Scene::reset(void){
	Move_Scene::reset();
	logic_uuid_ = 0;
	act_result_ = -1;
	act_time_type_ = 0;
	act_fight_time_ = 0;
	act_end_time_ = Time_Value::zero;
	world_boss_hp_change_ = false;
	hp_refresh_tick_ = 0;
	first_blood_role_id_ = 0;
	act_state_ = 0;
	balance_time_ = Time_Value::zero;
	boss_step_1_value_ = 0;
	boss_step_2_value_ = 0;
	boss_step_3_value_ = 0;
	is_check_boss_step_ = false;
}

void World_Boss_Scene::set_logic_uuid(const int64_t uuid){
	logic_uuid_ = uuid;
}

void World_Boss_Scene::npc_init(int scene_id, int sort, int sub_sort){
	act_fight_time_ = WORLD_BOSS_MANAGER->act_fight_time_;
	act_end_time_ = WORLD_BOSS_MANAGER->act_end_time_;
	act_time_type_ = 0;
	if(act_end_time_.sec() - Time_Value::gettimeofday().sec() <= act_fight_time_){
		act_time_type_ = 1;
	}
	section_init(scene_id, sort, sub_sort);
	const World_Boss_Hp_Info* boss_info = WORLD_BOSS_MANAGER->get_boss_info();
	if(boss_info){
		NPC_Record_Map::iterator it = npc_section_.record_map.begin();
		if(it != npc_section_.record_map.end()){
			if(it->second.config.npc_vec.empty() == false){
				//it->second.config.npc_vec[0].val_1 = boss_info->boss_type_id;
				it->second.config.refresh_times = 1;
			}
		}
	}
	NPC_MANAGER->section_refresh(&npc_section_);
}

int World_Boss_Scene::tick(const Time_Value &now){
	if(this->is_recover_status()){
		if(now >= balance_time_){
			kick_all_player();
			balance_time_ = now + Time_Value(4);
		}
		return 0;
	}
	Move_Scene::tick(now);
	if(this->scene_status_ == SCENE_NORMAL_STATUS){
		if(this->act_end_time_.sec() - now.sec() <= act_fight_time_){
			this->scene_status_ = SCENE_READY_STATUS;
			change_move_value(World_Boss_Revive_Move_Able, true);
			if(act_time_type_ == 0){
				act_time_type_ = 1;
				broadcast_act_time();
			}
		}
		return 0;
	}
	if(world_boss_hp_change_){
		hp_refresh_tick_ += 1;
		if(hp_refresh_tick_ > 20){
			world_boss_hp_change_ = false;
			hp_refresh_tick_ = 0;
			int blood = WORLD_BOSS_MANAGER->boss_info_.hp_cur;
			broadcast_boss_info();
			set_world_boss_blood(blood);
			//
			WORLD_BOSS_MANAGER->refresh_rank();
			broadcast_rank_info();
		}
	}
	switch(act_state_){
		case 0:{
			if(now >= act_end_time_){
				act_result_ = 0;
				act_state_ = 1;
				balance_time_ = now;
			}
			break;
		}
		case 1:{
			if(now >= balance_time_){
				force_end_battle();
				act_state_ = 2;
				balance_time_ = now + Time_Value(4);
			}
			break;
		}
		case 2:{
			if(now >= balance_time_){
				act_over();
				act_state_ = 3;
				balance_time_ = now + Time_Value(4);
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int World_Boss_Scene::enter_scene(Mover *mover){
	Move_Scene::enter_scene(mover);
	if(mover && mover->player_self()){
		if(this->is_recover_status()){
			int dest = 0;
			mover->player_self()->inner_transmit(dest,0,0);
			return 0;
		}
		World_Boss_Player_Info* player_info = WORLD_BOSS_MANAGER->get_player_info(mover->role_id());
		//
		if(player_info){
			Time_Value now = Time_Value::gettimeofday();
			if(now < player_info->revive_time ){
				mover->player_self()->set_revive_info(true, player_info->revive_time);
			}
			for(int i = 0; i < player_info->ability_2.val_1; ++i){
				mover->player_self()->world_boss_add_power_up_buf(world_boss_act_icon_id);
			}
		}
		for(int i = 0; i < WORLD_BOSS_MANAGER->boss_info_.bonus_id; ++i){
			mover->player_self()->world_boss_add_leader_buf(world_boss_act_icon_id);
		}
	}
	return 0;
}

int World_Boss_Scene::exit_scene(Mover *mover, bool offline){
	Move_Scene::exit_scene(mover, offline);
	if(mover && mover->player_self()){
		// 首杀奖励
		if(mover->role_id() == first_blood_role_id_){
			add_first_blood_reward(first_blood_role_id_);
			first_blood_role_id_ = 0;
		}
		mover->player_self()->reset_revive_info();
		mover->player_self()->world_boss_clear_power_up_buf(world_boss_act_icon_id);
		mover->player_self()->world_boss_clear_leader_buf(world_boss_act_icon_id);
	}
	return 0;
}

int World_Boss_Scene::init_scene_normal(int scene_id, int scene_order, int map_id/* = 0*/){
	Move_Scene_Base::init_scene_normal(scene_id, scene_order, map_id);
	if(scene_config()){
		transmit_point = scene_config()->transmit_point;
		if(scene_config()->type == FuBen_Scene_Type &&
				scene_config()->dungeon.first_layer_id != 0){//多层副本
			//非首层
			if(scene_config()->dungeon.id != scene_config()->dungeon.first_layer_id){
				set_pass_status(Move_Scene::SCENE_PASS_STATUS_NO_COMPELTE);
			}
		}
	}
	Time_Value time_now = Time_Value::gettimeofday();
	scene_begin_time_ = time_now;
	nor_tick_time_ = time_now;
	npc_init(scene_id, 0, 0);
	is_check_boss_step_ = CONFIG_CACHE_WORLD_BOSS->get_boss_weak_check(world_boss_act_icon_id);
	boss_step_1_value_ = CONFIG_CACHE_WORLD_BOSS->get_boss_weak_1(world_boss_act_icon_id);
	boss_step_2_value_ = CONFIG_CACHE_WORLD_BOSS->get_boss_weak_2(world_boss_act_icon_id);
	boss_step_3_value_ = CONFIG_CACHE_WORLD_BOSS->get_boss_weak_3(world_boss_act_icon_id);
	return 0;
}

int World_Boss_Scene::client_scene_complete(Fighter &fighter) {
	if(fighter.player_self() == NULL){
		return 0;
	}
	Move_Scene::client_scene_complete(fighter);
	World_Boss_Player_Info* player_info = WORLD_BOSS_MANAGER->get_player_info(fighter.role_id());
	if(!player_info){
		return 0;
	}
	//场景信息
	MSG_81000600 msg_act_info;
	msg_act_info.icon_id = world_boss_act_icon_id;
	gang_id_t gang_id = fighter.player_self()->gang_id();
	World_Boss_Gang_Bless_Map::const_iterator it = WORLD_BOSS_MANAGER->gang_bless_map_.find(gang_id);
	if(it != WORLD_BOSS_MANAGER->gang_bless_map_.end()){
		msg_act_info.act_guild_bless_role_id = it->second.role_id;
		msg_act_info.act_guild_bless_role_name = it->second.role_name;
		msg_act_info.act_guild_bless_role_pos = it->second.gang_pos;
	}
	int ability_num = 0;
	ability_num = player_info->ability_0.val_2 - player_info->ability_0.val_1;
	if(ability_num < 0){
		ability_num = 0;
	}
	msg_act_info.ability_0_num = ability_num;

   ability_num = player_info->ability_1.val_2 - player_info->ability_1.val_1;
	if(ability_num < 0){
		ability_num = 0;
	}
	msg_act_info.ability_1_num = ability_num;

	ability_num = player_info->ability_2.val_2 - player_info->ability_2.val_1;
	if(ability_num < 0){
		ability_num = 0;
	}
	msg_act_info.ability_2_num = ability_num;

	ability_num = player_info->ability_3.val_2 - player_info->ability_3.val_1;
	if(ability_num < 0){
		ability_num = 0;
	}
	msg_act_info.ability_3_num = ability_num;

	msg_act_info.act_time_type = act_time_type_;
	int remain_time = act_end_time_.sec() - Time_Value::gettimeofday().sec();
	if(act_time_type_ == 0){
		remain_time -= act_fight_time_;
	}
	if(remain_time < 0){
		remain_time = 0;
	}
	msg_act_info.act_time = remain_time;
	msg_act_info.boss_cur_hp = WORLD_BOSS_MANAGER->boss_info_.hp_cur;
	msg_act_info.boss_max_hp = WORLD_BOSS_MANAGER->boss_info_.hp_max;
	msg_act_info.boss_type_id = WORLD_BOSS_MANAGER->boss_info_.boss_type_id;
	msg_act_info.boss_name = WORLD_BOSS_MANAGER->boss_info_.boss_name;
	msg_act_info.boss_buffer_type = WORLD_BOSS_MANAGER->boss_info_.buffer_step;
	if(msg_act_info.boss_buffer_type == 3){
		msg_act_info.boss_buffer_type = 0;
	}
	msg_act_info.boss_bonus_id = WORLD_BOSS_MANAGER->boss_info_.bonus_id;
	msg_act_info.boss_bonus_role_name = WORLD_BOSS_MANAGER->boss_info_.bonus_role_name;
	msg_act_info.boss_kill_num = WORLD_BOSS_MANAGER->act_detail_.kill_num;
	msg_act_info.boss_lv = WORLD_BOSS_MANAGER->act_detail_.boss_lv;
	msg_act_info.revive_time = player_info->get_revive_time();
	OBJ_SEND_TO_CLIENT(msg_act_info, fighter);
	// 排名信息
	MSG_81000604 msg_rank_info;
	msg_rank_info.icon_id = world_boss_act_icon_id;
	msg_rank_info.damage = player_info->rank_info.damage;
	msg_rank_info.damage_rate = player_info->rank_info.damage_rate;
	msg_rank_info.rank = player_info->rank_info.rank;
	msg_rank_info.rank_info = WORLD_BOSS_MANAGER->rank_show_info_;
	if(player_info->rank_info.career == 1){
		msg_rank_info.career_rank = player_info->career_rank;
		msg_rank_info.career_rank_info = WORLD_BOSS_MANAGER->rank_show_info_1;
	}else if(player_info->rank_info.career == 2){
		msg_rank_info.career_rank = player_info->career_rank;
		msg_rank_info.career_rank_info = WORLD_BOSS_MANAGER->rank_show_info_2;
	}else{
		msg_rank_info.career_rank = player_info->career_rank;
		msg_rank_info.career_rank_info = WORLD_BOSS_MANAGER->rank_show_info_3;
	}
	OBJ_SEND_TO_CLIENT(msg_rank_info, fighter);
	// 首杀奖励
	if(fighter.role_id() == first_blood_role_id_){
		add_first_blood_reward(first_blood_role_id_);
		first_blood_role_id_ = 0;
	}
	return 0;
}


int World_Boss_Scene::update_move_able_info(Mover* mover){
	mover->clear_moveable_value();
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		if((*it).move_value.empty() == false){
			Block_Buffer buf;
			MSG_81000022 msg;
			Move_Value::iterator s_it;
			for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
				if(s_it->val_2 == MOVE_ABLE_FROM){
					if(s_it->val_1 == World_Boss_Revive_Move_Able){
						if(mover->player_self()){
							if(mover->player_self()->is_revive() == false){
								msg.moveable_info_vec.push_back(s_it->val_1);
								uint8_t val1 = s_it->val_1;
								mover->add_moveable_value(val1);
							}
						}
					}else {
						msg.moveable_info_vec.push_back(s_it->val_1);
						uint8_t val1 = s_it->val_1;
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

void World_Boss_Scene::hook_player_enter_battle(Scene_Player* player){
	if(!player){
		return;
	}
	if(player->battle_scene()){
		int blood = world_boss_manager()->boss_info_.hp_cur;
		player->battle_scene()->set_world_boss_blood(blood);
		Fighter* boss = player->battle_scene()->get_boss_fighter();
		if(boss){
			if(world_boss_manager()->boss_info_.buffer_step == 1){
				add_weak_up_1_buf_to_fighter(boss);
			}else if(world_boss_manager()->boss_info_.buffer_step == 2){
				add_weak_up_2_buf_to_fighter(boss);
			}
		}
	}
	bool first_blood = WORLD_BOSS_MANAGER->check_first_blood(player);
	if(first_blood){
		first_blood_role_id_ = player->role_id();
		this->announce_to_all(7001, player->role_id(), player->role_name());
	}
}

void World_Boss_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters){
	if(win_pos == 0){
		int count = win_mover_vec.size();
		for(int i = 0; i < count; ++i){
			World_Boss_Player_Info* player_info = WORLD_BOSS_MANAGER->get_player_info(win_mover_vec[i]->role_id());
			if(player_info){//
				Scene_Player* player = win_mover_vec[i]->player_self();
				if(player){
					Prop_Setter_Vec fight_setter;
					Prop_Setter setter;
					setter.prop_type = PT_BLOOD_CURRENT;
					setter.operate = O_SET;
					setter.addi_type = AT_BASIC;
					setter.basic = player->blood_max();
					fight_setter.push_back(setter);
					player->modify_props_normal(fight_setter, false, 24);
					// 战斗收益
					this->add_fight_reward(player_info->rank_info.role_id, player_info->fight_damage, player_info->ability_3_coe);
					player_info->fight_damage = 0;
				}
			}
		}
	}else{
		int revive_time = CONFIG_CACHE_WORLD_BOSS->get_revive_time(world_boss_act_icon_id);
		Time_Value rv_time = Time_Value::gettimeofday() + Time_Value(revive_time);
		int count = lose_mover_vec.size();
		for(int i = 0; i < count; ++i){
			WORLD_BOSS_MANAGER->set_player_revive_time(lose_mover_vec[i]->role_id(), revive_time);
			Scene_Player* player = lose_mover_vec[i]->player_self();
			if(player){//recover hp
				player->set_revive_info(true, rv_time);
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->blood_max();
				fight_setter.push_back(setter);
				player->modify_props_normal(fight_setter, false, 24);
				player->back_to_revive_point();
				// 战斗收益
				World_Boss_Player_Info* player_info = WORLD_BOSS_MANAGER->get_player_info(player->role_id());
				if(player_info){
					this->add_fight_reward(player_info->rank_info.role_id, player_info->fight_damage, player_info->ability_3_coe);
					player_info->fight_damage = 0;
				}
			}
		}
	}
}

void World_Boss_Scene::be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id){
	if(!battle_scene){
		return;
	}
	if(act_result_ != -1){// 活动结束
		return;
	}
	WORLD_BOSS_MANAGER->boss_info_.hp_cur -= sub_blood;
	check_weak_up_buf();
	int bonus_id = 0;
	role_id_t bonus_role_id = 0;
	std::string bonus_role_name = "";
	role_id_t attacker_master_id = attacker_id;
	Fighter* fighter = battle_scene->find_mover_with_role_id(attacker_id);
	if(fighter){
		if(fighter->monster_self()){
			attacker_master_id = fighter->monster_self()->master_role_id();
		}else if(fighter->hero_self()){
			attacker_master_id = fighter->hero_self()->master_role_id();
		}
	}
	WORLD_BOSS_MANAGER->player_damage(attacker_master_id, sub_blood, bonus_id, bonus_role_id, bonus_role_name);
	if(WORLD_BOSS_MANAGER->boss_info_.hp_cur <= 0){
		WORLD_BOSS_MANAGER->boss_info_.hp_cur = 0;
		set_world_boss_blood(0);
		hp_refresh_tick_ = 40;
		act_result_ = 1;
		Time_Value now = Time_Value::gettimeofday();
		int kill_boss_time = now.sec() - (act_end_time_.sec() - act_fight_time_);
		int rapid_kill_time = CONFIG_CACHE_WORLD_BOSS->get_boss_rapid_kill_time(world_boss_act_icon_id);
		if(kill_boss_time <= rapid_kill_time){
			act_result_ = 2;
		}
		act_state_ = 1;
		balance_time_ = Time_Value::gettimeofday() + Time_Value(5);
		NPC_Record_Map::iterator it;
		for(it = npc_section_.record_map.begin(); it != npc_section_.record_map.end(); ++it){
			it->second.clear_point();
		}
		World_Boss_Player_Info* player_info = WORLD_BOSS_MANAGER->get_player_info(attacker_master_id);
		if(player_info){//
			player_info->achieve_5 = 1;
			this->announce_to_all(7002, player_info->rank_info.role_id, player_info->rank_info.role_name);
			// 击杀boss通知
			{
				Block_Buffer kb_buf;
				kb_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				kb_buf.write_int32(world_boss_act_icon_id);
				kb_buf.write_int8(4);
				kb_buf.write_int64(player_info->rank_info.role_id);
				kb_buf.finish_message();
				SCENE_MONITOR->send_to_logic(logic_uuid_, kb_buf);
			}
		}
	}else{
		if(bonus_id != 0){
			add_leader_buf();
			this->announce_to_all(7003, bonus_role_id, bonus_role_name);
		}
	}
	world_boss_hp_change_ = true;
}

void World_Boss_Scene::set_world_boss_blood(const int blood){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self() &&
					it->second->player_self()->battle_scene()) {
				it->second->player_self()->battle_scene()->set_world_boss_blood(blood);
			}
		}
	}
}

int World_Boss_Scene::get_world_boss_act_time_type(void){
	return act_time_type_;
}

World_Boss_Manager* World_Boss_Scene::world_boss_manager(void){
	return WORLD_BOSS_MANAGER;
}

void World_Boss_Scene::kick_all_player(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		std::vector<Scene_Player*> player_vec;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				// 恢复进入战场的血量
				//double src_hp = it->second->player_self()->war_info()->src_hp;
				double src_hp = it->second->player_self()->blood_max();
				it->second->player_self()->restore_hp(src_hp, false);
				if(it->second->is_in_mover_scene()){//
					player_vec.push_back(it->second->player_self());
				}
			}
		}
		int count = player_vec.size();
		for(int i = 0; i < count; ++i){
			int dest = 0;
			player_vec[i]->inner_transmit(dest,0,0);
		}
	}
}

void World_Boss_Scene::act_over(void){
	WORLD_BOSS_MANAGER->refresh_rank();
	//奖励
	MSG_81000602 msg;
	msg.icon_id = world_boss_act_icon_id;
	msg.result = act_result_;
	msg.boss_kill_num = WORLD_BOSS_MANAGER->act_detail_.kill_num;

	int8_t send_mail = 0;
	Block_Buffer reward_buf;
	reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
	reward_buf.write_int32(world_boss_act_icon_id);
	reward_buf.write_int8(1);
	int act_player_num = WORLD_BOSS_MANAGER->player_info_map_.size();
	reward_buf.write_int32(act_player_num);

	Item_Info item_info;
	item_info.bind = Item_Detail::BIND;
	boost::unordered_map<int, int> reward_item_map;
	boost::unordered_map<int, int> other_reward_map;
	boost::unordered_map<int, int> rank_reward_map;
	boost::unordered_map<int, int> first_rank_reward_map;
	int rank_coe = 100;
	if(act_result_ == 0){
		rank_coe = 95;
	}
	double rank_money_coe = 1.0f;
	int cfg_boss_lv = WORLD_BOSS_MANAGER->boss_info_.cfg_boss_lv;
	const World_Boss_Level_Config* cfg_lv = CONFIG_CACHE_WORLD_BOSS->get_level_config_from_lv(cfg_boss_lv,world_boss_act_icon_id);
	if(cfg_lv){
		rank_money_coe = cfg_lv->rank_reward_money/1000.0f;
	}

	World_Boss_Player_Info_Map::const_iterator player_it;
	for(player_it = WORLD_BOSS_MANAGER->player_info_map_.begin(); player_it != WORLD_BOSS_MANAGER->player_info_map_.end(); ++player_it){
		send_mail = 1;
		reward_item_map.clear();
		other_reward_map.clear();
		rank_reward_map.clear();
		role_id_t player_role_id = player_it->second.rank_info.role_id;
		int player_damage = player_it->second.rank_info.damage;
		double player_damage_rate = player_it->second.rank_info.damage_rate;
		int self_rank = player_it->second.career_rank;
		//lv
		if(cfg_lv){
			if(player_it->second.achieve_1 != 0){// 初级勇者奖励
				int count = cfg_lv->low_hero_item.size();
				for(int i = 0; i < count; ++i){
					int amount = cfg_lv->low_hero_item[i].val_2;
					if(amount > 0){
						reward_item_map[cfg_lv->low_hero_item[i].val_1] += amount;
						other_reward_map[cfg_lv->low_hero_item[i].val_1] += amount;
					}

				}
			}
			if(player_it->second.achieve_2 != 0){// 中级勇者奖励
				int count = cfg_lv->mid_hero_item.size();
				for(int i = 0; i < count; ++i){
					int amount = cfg_lv->mid_hero_item[i].val_2;
					if(amount > 0){
						reward_item_map[cfg_lv->mid_hero_item[i].val_1] += amount;
						other_reward_map[cfg_lv->mid_hero_item[i].val_1] += amount;
					}
				}
			}
			if(player_it->second.achieve_3 != 0){// 高级勇者奖励
				int count = cfg_lv->high_hero_item.size();
				for(int i = 0; i < count; ++i){
					int amount = cfg_lv->high_hero_item[i].val_2;
					if(amount > 0){
						reward_item_map[cfg_lv->high_hero_item[i].val_1] += amount;
						other_reward_map[cfg_lv->high_hero_item[i].val_1] += amount;
					}
				}
			}
//			if(player_it->second.achieve_4 != 0){// 首杀奖励
//				int count = cfg_lv->first_blood_reward.size();
//				for(int i = 0; i < count; ++i){
//					reward_item_map[cfg_lv->first_blood_reward[i].val_1] += cfg_lv->first_blood_reward[i].val_2;
//				}
//			}
			if(player_it->second.achieve_5 != 0){// 击杀奖励
				int count = cfg_lv->first_kill_reward.size();
				for(int i = 0; i < count; ++i){
					int amount = cfg_lv->first_kill_reward[i].val_2;
					if(amount){
						reward_item_map[cfg_lv->first_kill_reward[i].val_1] += amount;
						other_reward_map[cfg_lv->first_kill_reward[i].val_1] += amount;
					}

				}
			}
		}
		//rank
		const World_Boss_Reward_Config* cfg_rank = CONFIG_CACHE_WORLD_BOSS->get_reward_config(cfg_boss_lv, self_rank, world_boss_act_icon_id);
		if(cfg_rank){
			{// 资源奖励
				int count = cfg_rank->reward_rank_base_money.size();
				for(int i = 0; i < count; ++i){
					int amount = cfg_rank->reward_rank_base_money[i].val_2*rank_money_coe*rank_coe/100;
					if(amount > 0){
						reward_item_map[cfg_rank->reward_rank_base_money[i].val_1] += amount;
						rank_reward_map[cfg_rank->reward_rank_base_money[i].val_1] += amount;
					}
				}
			}

			{// 道具奖励
				int count = cfg_rank->reward_rank_base_item.size();
				for(int i = 0; i < count; ++i){
					int amount = cfg_rank->reward_rank_base_item[i].val_2*rank_coe/100;
					if(amount > 0){
						reward_item_map[cfg_rank->reward_rank_base_item[i].val_1] += amount;
						rank_reward_map[cfg_rank->reward_rank_base_item[i].val_1] += amount;
					}

				}
			}
		}
		// 客户端界面奖励通知
		Mover* mover = this->find_mover_with_layer(0, player_role_id);
		if(mover){
			Scene_Player* player = mover->player_self();
			if(player){
				msg.damage = player_it->second.rank_info.damage;
				msg.damage_rate = player_it->second.rank_info.damage_rate;
				msg.rank = player_it->second.career_rank;
				msg.achieve_1 = player_it->second.achieve_1;
				msg.achieve_2 = player_it->second.achieve_2;
				msg.achieve_3 = player_it->second.achieve_3;
				msg.achieve_4 = player_it->second.achieve_4;
				msg.achieve_5 = player_it->second.achieve_5;
				msg.items.clear();
				msg.rank_items.clear();
				//
				boost::unordered_map<int,int>::iterator other_it;
				for(other_it = reward_item_map.begin(); other_it != reward_item_map.end(); ++other_it){
					item_info.id = other_it->first;
					item_info.amount = other_it->second;
					msg.items.push_back(item_info);
				}

				boost::unordered_map<int,int>::iterator rank_it;
				for(rank_it = rank_reward_map.begin(); rank_it != rank_reward_map.end(); ++rank_it){
					item_info.id = rank_it->first;
					item_info.amount = rank_it->second;
					msg.rank_items.push_back(item_info);
				}
				OBJ_SEND_TO_CLIENT(msg, (*player));
				send_mail = 0;
			}
		}
		//to logic
		reward_buf.write_int64(player_role_id);
		reward_buf.write_int32(player_damage);
		reward_buf.write_double(player_damage_rate);
		reward_buf.write_int32(self_rank);
		reward_buf.write_int8(send_mail);
		int item_num = reward_item_map.size();
		reward_buf.write_int32(item_num);
		boost::unordered_map<int,int>::iterator it;
		for(it = reward_item_map.begin(); it != reward_item_map.end(); ++it){
			int item_id = it->first;
			int item_amount = it->second;
			reward_buf.write_int32(item_id);
			reward_buf.write_int32(item_amount);
		}
		if(self_rank == 1){
			first_rank_reward_map = rank_reward_map;
		}
	}
	reward_buf.finish_message();
	SCENE_MONITOR->send_to_logic(logic_uuid_, reward_buf);
	{// act_end
		Block_Buffer end_buf;
		end_buf.make_message(INNER_ACT_SCENE_END);
		end_buf.write_int32(world_boss_act_icon_id);
		end_buf.write_int64(0);
		end_buf.write_int32(act_result_);
		end_buf.finish_message();
		SCENE_MONITOR->send_to_logic(logic_uuid_, end_buf);
	}

	// announce act end
	role_id_t first_role_id_1 = 0;
	role_id_t first_role_id_2 = 0;
	role_id_t first_role_id_3 = 0;
	std::string first_role_name_1 = "";
	std::string first_role_name_2 = "";
	std::string first_role_name_3 = "";
	WORLD_BOSS_MANAGER->get_first_rank_info(first_role_id_1, first_role_name_1,
			first_role_id_2, first_role_name_2,
			first_role_id_3, first_role_name_3);
	if(first_role_id_1 != 0 || first_role_id_2 != 0 || first_role_id_3 != 0){
		std::stringstream ostr;
		const World_Boss_Reward_Config* first_rank = CONFIG_CACHE_WORLD_BOSS->get_reward_config(cfg_boss_lv, 1, world_boss_act_icon_id);
		if(first_rank){
			boost::unordered_map<int, int>::iterator first_it;
			for(first_it = first_rank_reward_map.begin(); first_it != first_rank_reward_map.end(); ++first_it){
				const Item_Detail_Config * cfg_item = CONFIG_CACHE_ITEM->find_item(first_it->first);
				if(cfg_item){
					ostr<<cfg_item->name<<"*"<<first_it->second<< " ";
				}else{
					if(first_it->first == COPPER){
						ostr<<CONFIG_CACHE_ARENA->get_text_copper()<<"*"<<first_it->second<< " ";
					}else if(first_it->first == SOULS){
						ostr<<CONFIG_CACHE_ARENA->get_text_soul()<<"*"<<first_it->second<< " ";
					}
				}
			}
		}
		if(first_role_id_1 != 0){
			this->announce_to_all(7004, first_role_id_1, first_role_name_1, ostr.str());
		}
		if(first_role_id_2 != 0){
			this->announce_to_all(7011, first_role_id_2, first_role_name_2, ostr.str());
		}
		if(first_role_id_3 != 0){
			this->announce_to_all(7012, first_role_id_3, first_role_name_3, ostr.str());
		}
	}else{
		this->announce_to_all(7005, 0, "");
	}
	WORLD_BOSS_MANAGER->reset();
	//
	kick_all_player();
	this->set_scene_status(SCENE_RECOVER_STATUS);
	recover_time_ = Time_Value::gettimeofday() + Time_Value(10);
}

void World_Boss_Scene::broadcast_boss_info(void){
	int blood = WORLD_BOSS_MANAGER->boss_info_.hp_cur;
	int damage = blood - WORLD_BOSS_MANAGER->boss_info_.pre_hp_cur;
	WORLD_BOSS_MANAGER->boss_info_.pre_hp_cur = blood;
	MSG_81000603 msg;
	msg.icon_id = world_boss_act_icon_id;
	msg.bonus_id = WORLD_BOSS_MANAGER->boss_info_.bonus_id;
	msg.buffer_type = WORLD_BOSS_MANAGER->boss_info_.buffer_step;
	if(msg.buffer_type == 3){
		msg.buffer_type = 0;
	}
	msg.bonus_role_name = WORLD_BOSS_MANAGER->boss_info_.bonus_role_name;
	msg.cur_hp = blood;
	msg.max_hp = WORLD_BOSS_MANAGER->boss_info_.hp_max;
	msg.damage = damage;
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	this->broadcast_all(0, buf);
}

void World_Boss_Scene::broadcast_rank_info(void){
	MSG_81000604 msg;
	msg.icon_id = world_boss_act_icon_id;
	msg.rank_info = WORLD_BOSS_MANAGER->rank_show_info_;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self() &&
					it->second->player_self()->is_in_mover_scene()) {
				World_Boss_Player_Info* player_info = WORLD_BOSS_MANAGER->get_player_info(it->second->role_id());
				if(player_info){
					msg.damage = player_info->rank_info.damage;
				   msg.damage_rate = player_info->rank_info.damage_rate;
				   msg.rank = player_info->rank_info.rank;
					if(player_info->rank_info.career == 1){
						msg.career_rank = player_info->career_rank;
						msg.career_rank_info = WORLD_BOSS_MANAGER->rank_show_info_1;
					}else if(player_info->rank_info.career == 2){
						msg.career_rank = player_info->career_rank;
						msg.career_rank_info = WORLD_BOSS_MANAGER->rank_show_info_2;
					}else{
						msg.career_rank = player_info->career_rank;
						msg.career_rank_info = WORLD_BOSS_MANAGER->rank_show_info_3;
					}
				   Block_Buffer buf;
				   MAKE_MSG_BUF(msg, buf);
				   it->second->send_to_client(buf);
				}
			}
		}
	}
}

void World_Boss_Scene::broadcast_act_time(void){
	MSG_81000605 msg;
	msg.icon_id = world_boss_act_icon_id;
	msg.act_time_type = act_time_type_;
	msg.act_time = act_end_time_.sec() - Time_Value::gettimeofday().sec();
	if(msg.act_time < 0){
		msg.act_time = 0;
	}
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	this->broadcast_all(0, buf);
}

void World_Boss_Scene::add_leader_buf(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				it->second->player_self()->world_boss_add_leader_buf(world_boss_act_icon_id);
			}
		}
	}
	broadcast_boss_info();
}

void World_Boss_Scene::add_first_blood_reward(const role_id_t role_id){
	int cfg_boss_lv = WORLD_BOSS_MANAGER->boss_info_.cfg_boss_lv;
	const World_Boss_Level_Config* cfg_lv = CONFIG_CACHE_WORLD_BOSS->get_level_config_from_lv(cfg_boss_lv, world_boss_act_icon_id);
	if(cfg_lv){
		Block_Buffer reward_buf;
		reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
		reward_buf.write_int32(world_boss_act_icon_id);
		reward_buf.write_int8(2);
		reward_buf.write_int64(first_blood_role_id_);
		int count = cfg_lv->first_blood_reward.size();
		reward_buf.write_int32(count);
		for(int i = 0; i < count; ++i){
			reward_buf.write_int32(cfg_lv->first_blood_reward[i].val_1);
			reward_buf.write_int32(cfg_lv->first_blood_reward[i].val_2);
		}
		reward_buf.finish_message();
		SCENE_MONITOR->send_to_logic(logic_uuid_, reward_buf);
	}
}

void World_Boss_Scene::add_fight_reward(const role_id_t role_id, int damage, const int add_effect){
	if(damage <= 0){
		damage = 1;
	}
	int hurt_reward_coe = WORLD_BOSS_MANAGER->boss_info_.hurt_reward_coe;
	int copper = sqrt(damage)*hurt_reward_coe;
	if(copper <= 0){
		copper = 1;
	}
	if(add_effect > 0){
		copper += copper*add_effect/100;
	}
	Block_Buffer reward_buf;
	reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
	reward_buf.write_int32(world_boss_act_icon_id);
	reward_buf.write_int8(3);
	reward_buf.write_int64(role_id);
	reward_buf.write_int32(copper);
	reward_buf.finish_message();
	SCENE_MONITOR->send_to_logic(logic_uuid_, reward_buf);
}

void World_Boss_Scene::force_end_battle(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				if(player->battle_scene()){
					player->force_finish_battle_scene();
				}
			}
		}
	}
}

void World_Boss_Scene::check_weak_up_buf(void){
	if(!is_check_boss_step_){
		return;
	}
	double max_hp = world_boss_manager()->boss_info_.hp_max;
	if(max_hp < 0){
		max_hp = 1.0;
	}
	int step_value = world_boss_manager()->boss_info_.hp_cur/max_hp*1000;
	if(step_value < boss_step_3_value_){// 40%
		if(world_boss_manager()->boss_info_.buffer_step == 0){
			world_boss_manager()->boss_info_.buffer_step = 3;
		}else if(world_boss_manager()->boss_info_.buffer_step == 1){// HP低于80%时，BOSS物免降低20%
			clear_weak_up_1_buf();
			world_boss_manager()->boss_info_.buffer_step = 3;
			announce_to_all(7010, 0, "");
			broadcast_boss_info();
		}else if(world_boss_manager()->boss_info_.buffer_step == 2){//HP低于60%时，BOSS魔免降低20%
			clear_weak_up_2_buf();
			world_boss_manager()->boss_info_.buffer_step = 3;
			announce_to_all(7010, 0, "");
			broadcast_boss_info();
		}
	}else if(step_value < boss_step_2_value_){// 60%
		if(world_boss_manager()->boss_info_.buffer_step == 0){
			add_weak_up_2_buf();
			world_boss_manager()->boss_info_.buffer_step = 2;
			announce_to_all(7009, 0, "");
			broadcast_boss_info();
		}else if(world_boss_manager()->boss_info_.buffer_step == 1){// HP低于80%时，BOSS物免降低20%
			clear_weak_up_1_buf();
			add_weak_up_2_buf();
			world_boss_manager()->boss_info_.buffer_step = 2;
			announce_to_all(7009, 0, "");
			broadcast_boss_info();
		}else if(world_boss_manager()->boss_info_.buffer_step == 2){//HP低于60%时，BOSS魔免降低20%

		}else if(world_boss_manager()->boss_info_.buffer_step == 3){//hp 40%

		}
	}else if(step_value < boss_step_1_value_){// 80%
		if(world_boss_manager()->boss_info_.buffer_step == 0){
			add_weak_up_1_buf();
			world_boss_manager()->boss_info_.buffer_step = 1;
			announce_to_all(7008, 0, "");
			broadcast_boss_info();
		}else if(world_boss_manager()->boss_info_.buffer_step == 1){// HP低于80%时，BOSS物免降低20%

		}else if(world_boss_manager()->boss_info_.buffer_step == 2){//HP低于60%时，BOSS魔免降低20%

		}else if(world_boss_manager()->boss_info_.buffer_step == 3){//hp 40%

		}
	}
}

void World_Boss_Scene::add_weak_up_1_buf(void){
	const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(world_boss_act_icon_id);
	if(!cfg_base){
		return;
	}
	Status_Effect_Argv argv;
	argv = cfg_base->boss_weak_buff1;
	argv.last_time_ms = -1;

	Scene_Layer *layer = scene_layer(0);
	if (!layer) {
		return;
	}
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second == NULL){
			continue;
		}
		if (it->second->player_self()) {
			if(it->second->battle_scene()){
				Fighter* boss = it->second->battle_scene()->get_boss_fighter();
				if(boss){
					argv.caster_id = boss->role_id();
					boss->insert_status(argv);
				}
			}
		}
//		else if(it->second->monster_self()){
//			if(it->second->is_boss()){
//				argv.caster_id = it->second->monster_self()->role_id();
//				it->second->monster_self()->insert_status(argv);
//			}
//		}
	}
}

void World_Boss_Scene::clear_weak_up_1_buf(void){
	const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(world_boss_act_icon_id);
	if(!cfg_base){
		return;
	}
	int buf_id = cfg_base->boss_weak_buff1.status_id;
	Scene_Layer *layer = scene_layer(0);
	if (!layer) {
		return;
	}
	Status_Info_Msg msg_info;
	msg_info.status_id = buf_id;
	msg_info.overlay = 0;
	msg_info.last_time_ms = -1;
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if (it->second && it->second->player_self()) {
			if(it->second == NULL){
				continue;
			}
			if (it->second->player_self()) {
				if(it->second->battle_scene()){
					Fighter* boss = it->second->battle_scene()->get_boss_fighter();
					if(boss){
						int result = boss->erase_status_type(buf_id);
						if(result == 0){
							msg_info.status_owner = boss->role_id();
							MSG_81000071 msg;
							msg.type = SFT_FIGHT;
							msg.buff_detail.push_back(msg_info);
							Block_Buffer buf;
							MAKE_MSG_BUF(msg, buf);
							it->second->player_self()->send_to_client(buf);
						}
					}
				}
			}
//			else if(it->second->monster_self()){
//				if(it->second->is_boss()){
//					it->second->monster_self()->erase_status_type(buf_id);
//				}
//			}
		}
	}
}

void World_Boss_Scene::add_weak_up_2_buf(void){
	const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(world_boss_act_icon_id);
	if(!cfg_base){
		return;
	}
	Status_Effect_Argv argv;
	argv = cfg_base->boss_weak_buff2;
	argv.last_time_ms = -1;

	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if(it->second == NULL){
				continue;
			}
			if (it->second->player_self()) {
				if(it->second->battle_scene()){
					Fighter* boss = it->second->battle_scene()->get_boss_fighter();
					if(boss){
						argv.caster_id = boss->role_id();
						boss->insert_status(argv);
					}
				}
			}
//			else if(it->second->monster_self()){
//				if(it->second->is_boss()){
//					argv.caster_id = it->second->monster_self()->role_id();
//					it->second->monster_self()->insert_status(argv);
//				}
//			}
		}
	}
}

void World_Boss_Scene::clear_weak_up_2_buf(void){
	const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(world_boss_act_icon_id);
	if(!cfg_base){
		return;
	}
	int buf_id = cfg_base->boss_weak_buff2.status_id;
	Scene_Layer *layer = scene_layer(0);
	if (!layer) {
		return;
	}
	Status_Info_Msg msg_info;
	msg_info.status_id = buf_id;
	msg_info.overlay = 0;
	msg_info.last_time_ms = -1;
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second == NULL){
			continue;
		}
		if (it->second->player_self()) {
			if(it->second->battle_scene()){
				Fighter* boss = it->second->battle_scene()->get_boss_fighter();
				if(boss){
					int result = boss->erase_status_type(buf_id);
					if(result == 0){
						msg_info.status_owner = boss->role_id();
						MSG_81000071 msg;
						msg.type = SFT_FIGHT;
						msg.buff_detail.push_back(msg_info);
						Block_Buffer buf;
						MAKE_MSG_BUF(msg, buf);
						it->second->player_self()->send_to_client(buf);
					}
				}
			}
		}
//		else if(it->second->monster_self()){
//			if(it->second->is_boss()){
//				it->second->monster_self()->erase_status_type(buf_id);
//			}
//		}
	}
}

void World_Boss_Scene::add_weak_up_1_buf_to_fighter(Fighter* fighter){
	if(!fighter){
		return;
	}
	const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(world_boss_act_icon_id);
	if(!cfg_base){
		return;
	}
	Status_Effect_Argv argv;
	argv = cfg_base->boss_weak_buff1;
	argv.last_time_ms = -1;
	argv.caster_id = fighter->role_id();
	fighter->insert_status(argv);
}

void World_Boss_Scene::add_weak_up_2_buf_to_fighter(Fighter* fighter){
	if(!fighter){
		return;
	}
	const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(world_boss_act_icon_id);
	if(!cfg_base){
		return;
	}
	Status_Effect_Argv argv;
	argv = cfg_base->boss_weak_buff2;
	argv.last_time_ms = -1;
	argv.caster_id = fighter->role_id();
	fighter->insert_status(argv);
}

void World_Boss_Scene::announce_to_all(const int ann_id, const role_id_t role_id, std::string role_name, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/, std::string str4/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if(role_id != 0){
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
	}
	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	if(str4 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	SCENE_ANNOUNCE_MANAGER->announce_send_to_all(ANNOUNCE_DISTRICT_SYSTEM, ann_id, ann_contents);
}

void World_Boss_Scene::announce_to_scene(const int ann_id, const role_id_t role_id, std::string role_name, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/, std::string str4/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if(role_id != 0){
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
	}
	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	if(str4 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	MSG_83000000 msg;
	msg.chat_type = chat_scene;
	msg.system_type = ann_id;
	msg.content = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	broadcast_all(0, buf);
}


void World_Boss_Scene::announce_to_guild(const int64_t gang_id, const int ann_id, const role_id_t role_id, std::string role_name, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/, std::string str4/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if(role_id != 0){
		role_info.role_id = role_id;
		role_info.role_name = role_name;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
	}
	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	if(str4 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	SCENE_ANNOUNCE_MANAGER->announce_send_to_gang(gang_id, ann_id, ann_contents);
}

