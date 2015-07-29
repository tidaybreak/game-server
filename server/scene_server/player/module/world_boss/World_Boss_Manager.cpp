/*
 * World_Boss_Manager.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */


#include "World_Boss_Manager.h"
#include "NPC/NPC_Manager.h"
#include "World_Boss_Scene_Struct.h"
#include "Scene_Player.h"
#include "Config_Cache_World_Boss.h"
#include "Config_Cache_NPC.h"
#include "Scene_Monitor.h"
#include "Msg_Active_Struct.h"
#include "Msg_World_Boss_Struct.h"
#include "gang_boss_scene/Gang_Boss_Scene.h"
#include "world_boss_scene/World_Boss_Scene.h"
#include "Err_Code.h"
#include "Msg_Inner_Enum.h"

const int rank_show_num = 10;

World_Boss_Manager::World_Boss_Manager(void){
	reset();
}

World_Boss_Manager::~World_Boss_Manager(void){

}

void World_Boss_Manager::reset(void){
	icon_id_ = 16;
	act_state_ = 0;
	first_blood_role_id_ = 0;
	act_detail_.reset();
	act_fight_time_ = 0;
	act_end_time_ = Time_Value::zero;

	boss_info_.reset();
	player_info_map_.clear();
	gang_bless_map_.clear();
	rank_info_.clear();
	rank_show_info_.clear();
	rank_show_info_1.clear();
	rank_show_info_2.clear();
	rank_show_info_3.clear();
}

void World_Boss_Manager::set_icon_id(const int8_t icon_id){
	icon_id_ = icon_id;
}

int World_Boss_Manager::req_world_boss_act_guild_bless(Scene_Player* player){
	if(!player){
		return 0;
	}
	int64_t gang_id = player->gang_id();
	if(gang_id == 0){
		return ERROR_WB_BLESS_NO_HEADSHIP;
	}
	int gang_pos = player->ganger_detail().headship;
	if(gang_pos != GANG_HEADSHIP_VICE && gang_pos != GANG_HEADSHIP_LEADER){
		return ERROR_WB_BLESS_NO_HEADSHIP;
	}
	World_Boss_Gang_Bless_Map::iterator it = gang_bless_map_.find(gang_id);
	if(it != gang_bless_map_.end()){
		return ERROR_WB_BLESS_HAS_ACT;
	}
	// cost
	int cost_glod = CONFIG_CACHE_WORLD_BOSS->get_ability0_cost(0, icon_id_);
	if(cost_glod > 0){
		Block_Buffer buf;
		buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
		buf.write_int32(icon_id_);
		buf.write_int8(10);
		buf.write_int32(cost_glod);
		buf.finish_message();
		player->send_to_logic(buf);
	}
	return 0;
}

int World_Boss_Manager::req_world_boss_use_ability(Scene_Player* player, int type){
	if(!player){
		return 0;
	}
	World_Boss_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return 0;
	}
	if(player->is_in_battle()){
		return 0;
	}
	switch(type){
		case 0:{// 公会鼓舞
			if(player->is_revive() == false){
				return ERROR_WB_USE_ABILITY_ONLY_DEAD;
			}
			if(player_info->ability_0.val_1 >= player_info->ability_0.val_2){
				return ERROR_WB_ABILITY_EMPTY;
			}
			player_info->ability_0.val_1 += 1;
			player_info->set_revive_time(0);
			int monster_id = boss_info_.boss_type_id;
			NPC_MANAGER->create_battle_with_monster(player, monster_id, 0);
			int remain_num = player_info->ability_0.val_2 - player_info->ability_0.val_1;
			MSG_50211100 msg;
			msg.icon_id = icon_id_;
			msg.type = type;
			msg.result = 1;
			msg.remain_num = remain_num;
			OBJ_SEND_TO_CLIENT(msg, (*player));
			{
				MSG_81000026 msg;
				msg.role_id = player->role_id();
				msg.state = 0;
				Block_Buffer buf;
				MAKE_MSG_BUF(msg, buf);
				player->send_to_client(buf);
				// update move_able
				if(player->move_scene()){
					player->move_scene()->update_move_able_info(player);
				}
				player->scene_broad_around(buf, Mover::SCENE_MOVE);
			}
			break;
		}
		case 1:{// 浴火重生
			if(player->is_revive() == false){
				return ERROR_WB_USE_ABILITY_ONLY_DEAD;
			}
			if(player_info->ability_1.val_1 >= player_info->ability_1.val_2){
				return ERROR_WB_ABILITY_EMPTY;
			}
			//cost
			int cost_glod = CONFIG_CACHE_WORLD_BOSS->get_ability1_cost(player_info->ability_1.val_1, icon_id_);
			if(cost_glod > 0){
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(icon_id_);
				buf.write_int8(11);
				buf.write_int32(cost_glod);
				buf.finish_message();
				player->send_to_logic(buf);
			}
			break;
		}
		case 2:{// 激发潜能
			if(player_info->ability_2.val_1 >= player_info->ability_2.val_2){
				return ERROR_WB_ABILITY_EMPTY;
			}
			//cost
			int cost_glod = CONFIG_CACHE_WORLD_BOSS->get_ability2_cost(player_info->ability_2.val_1, icon_id_);
			if(cost_glod > 0){
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(icon_id_);
				buf.write_int8(12);
				buf.write_int32(cost_glod);
				buf.finish_message();
				player->send_to_logic(buf);
			}
			break;
		}
		case 3:{// 财神祝福
			if(player_info->ability_3.val_1 >= player_info->ability_3.val_2){
				return ERROR_WB_ABILITY_EMPTY;
			}
			//cost
			int cost_glod = CONFIG_CACHE_WORLD_BOSS->get_ability3_cost(player_info->ability_3.val_1, icon_id_);
			if(cost_glod > 0){
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(icon_id_);
				buf.write_int8(13);
				buf.write_int32(cost_glod);
				buf.finish_message();
				player->send_to_logic(buf);
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int World_Boss_Manager::req_fight_world_boss(Scene_Player* player){
	if(!player){
		return 0;
	}
	World_Boss_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return 0;
	}
	// 复活检测
	if(boss_info_.hp_cur <= 0){
		return 0;
	}
	//
	NPC_MANAGER->create_battle_with_monster(player, boss_info_.boss_type_id, 0);
	return 0;
}

int World_Boss_Manager::inner_player_enter(const role_id_t gang_id, Block_Buffer& buf){
	role_id_t role_id = 0;
	std::string role_name = "";
	int vip_lv = 0;
	int career = 0;
	buf.read_int64(role_id);
	buf.read_string(role_name);
	buf.read_int32(vip_lv);
	buf.read_int32(career);
	World_Boss_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it == player_info_map_.end()){
		World_Boss_Player_Info player_info;
		player_info.rank_info.role_id = role_id;
		player_info.rank_info.role_name = role_name;
		player_info.rank_info.career = career;
		player_info.gang_id = gang_id;
		std::pair<World_Boss_Player_Info_Map::iterator, bool> result_pair = player_info_map_.insert(std::make_pair(role_id, player_info));
		if(result_pair.second == true){
			rank_info_.push_back(&(result_pair.first->second));
		}
	}
	it = player_info_map_.find(role_id);
	if(it != player_info_map_.end()){
		//ability0 公会鼓舞
		if(gang_id != 0){
			World_Boss_Gang_Bless_Map::iterator gang_it = gang_bless_map_.find(gang_id);
			if(gang_it != gang_bless_map_.end()){
				it->second.ability_0.val_2 = gang_it->second.bless_num;
			}
		}
		// ability1 浴火重生
		it->second.ability_1.val_2 = CONFIG_CACHE_WORLD_BOSS->get_ability1_num(vip_lv, icon_id_);
		// ability2 激发潜能
		it->second.ability_2.val_2 = CONFIG_CACHE_WORLD_BOSS->get_ability2_num(vip_lv, icon_id_);
		// 财神祝福
		it->second.ability_3.val_2 = CONFIG_CACHE_WORLD_BOSS->get_ability3_num(vip_lv, icon_id_);
	}
	return 0;
}

int World_Boss_Manager::inner_sync_info(Scene_Player* player, Block_Buffer& buf){
	if(!player){
		return 0;
	}
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 50:{// 公会祝福扣钱返回
			int sub_result = 0;
			buf.read_int32(sub_result);
			if(sub_result == 0){
				player_act_guild_bless(player);
				MSG_50211101 msg;
				msg.icon_id = icon_id_;
				msg.result = 1;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}else{
				MSG_50211101 msg;
				msg.icon_id = icon_id_;
				msg.result = 0;
				OBJ_SEND_TO_CLIENT(msg, (*player));
				player->send_err_to_client(sub_result, 0);
			}
			break;
		}
		case 51:{// 浴火重生扣钱返回
			World_Boss_Player_Info* player_info = get_player_info(player->role_id());
			if(!player_info){
				return 0;
			}
			int sub_result = 0;
			buf.read_int32(sub_result);
			if(sub_result == 0){
				{
					MSG_81000026 msg;
					msg.role_id = player->role_id();
					msg.state = 0;
					Block_Buffer buf;
					MAKE_MSG_BUF(msg, buf);
					player->send_to_client(buf);
					// update move_able
					if(player->move_scene()){
						player->move_scene()->update_move_able_info(player);
					}
					player->scene_broad_around(buf, Mover::SCENE_MOVE);
				}
				player->set_revive_info(false, Time_Value::zero);
				player_info->set_revive_time(0);
				player_info->ability_1.val_1 += 1;
				int monster_id = boss_info_.boss_type_id;
				NPC_MANAGER->create_battle_with_monster(player, monster_id, 0);
				int remain_num = player_info->ability_1.val_2 - player_info->ability_1.val_1;
				MSG_50211100 msg;
				msg.icon_id = icon_id_;
				msg.type = 1;
				msg.result = 1;
				msg.remain_num = remain_num;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}else{
				int remain_num = player_info->ability_1.val_2 - player_info->ability_1.val_1;
				MSG_50211100 msg;
				msg.icon_id = icon_id_;
				msg.type = 1;
				msg.result = 0;
				msg.remain_num = remain_num;
				OBJ_SEND_TO_CLIENT(msg, (*player));
				player->send_err_to_client(sub_result, 0);
			}
			break;
		}
		case 52:{// 激发潜能扣钱返回
			World_Boss_Player_Info* player_info = get_player_info(player->role_id());
			if(!player_info){
				return 0;
			}
			int sub_result = 0;
			buf.read_int32(sub_result);
			if(sub_result == 0){
				player_info->ability_2.val_1 += 1;
				player->world_boss_add_power_up_buf(icon_id_);
				int remain_num = player_info->ability_2.val_2 - player_info->ability_2.val_1;
				MSG_50211100 msg;
				msg.icon_id = icon_id_;
				msg.type = 2;
				msg.result = 1;
				msg.remain_num = remain_num;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}else{
				int remain_num = player_info->ability_2.val_2 - player_info->ability_2.val_1;
				MSG_50211100 msg;
				msg.icon_id = icon_id_;
				msg.type = 2;
				msg.result = 0;
				msg.remain_num = remain_num;
				OBJ_SEND_TO_CLIENT(msg, (*player));
				player->send_err_to_client(sub_result, 0);
			}
			break;
		}
		case 53:{// 财神祝福扣钱返回
			World_Boss_Player_Info* player_info = get_player_info(player->role_id());
			if(!player_info){
				return 0;
			}
			int sub_result = 0;
			buf.read_int32(sub_result);
			if(sub_result == 0){
				player_info->ability_3_coe += CONFIG_CACHE_WORLD_BOSS->get_ability3_coe(player_info->ability_3.val_1, icon_id_);
				player_info->ability_3.val_1 += 1;
				int remain_num = player_info->ability_3.val_2 - player_info->ability_3.val_1;
				MSG_50211100 msg;
				msg.icon_id = icon_id_;
				msg.type = 3;
				msg.result = 1;
				msg.remain_num = remain_num;
				OBJ_SEND_TO_CLIENT(msg, (*player));
			}else{
				int remain_num = player_info->ability_3.val_2 - player_info->ability_3.val_1;
				MSG_50211100 msg;
				msg.icon_id = icon_id_;
				msg.type = 3;
				msg.result = 0;
				msg.remain_num = remain_num;
				OBJ_SEND_TO_CLIENT(msg, (*player));
				player->send_err_to_client(sub_result, 0);
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int World_Boss_Manager::inner_act_start(const role_id_t gang_id, Block_Buffer& buf){
	if(act_state_ != ACT_STATE_START){//
		act_state_ = ACT_STATE_START;
		int icon_id = icon_id_;
		reset();
		icon_id_ = icon_id;
	}
	int remain_time = 0;
	int fight_time = 0;
	buf.read_int32(fight_time);
	buf.read_int32(remain_time);
	act_detail_.deserialize(buf);
	act_fight_time_ = fight_time;
	act_end_time_ = Time_Value::gettimeofday() + Time_Value(remain_time);
	int cfg_boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(act_detail_.kill_num, icon_id_);
	const World_Boss_Level_Config* cfg_lv = CONFIG_CACHE_WORLD_BOSS->get_level_config_from_lv(cfg_boss_lv, icon_id_);
	if(!cfg_lv){
		return 0;
	}
	const NPC_Config_Cache* cfg_npc = CONFIG_CACHE_NPC->find_npc_config_cache(cfg_lv->monster_id);
	if(!cfg_npc){
		return 0;
	}
	boss_info_.boss_name = cfg_npc->name;
	boss_info_.boss_lv = cfg_npc->level;
	boss_info_.cfg_boss_lv = cfg_boss_lv;
	boss_info_.hurt_reward_coe = cfg_lv->hurt_reward;
	boss_info_.boss_type_id = cfg_lv->monster_id;
	int count = cfg_npc->prop_value_vec.size();
	for(int i = 0; i < count; ++i){
		if(cfg_npc->prop_value_vec[i].id == 506205){
			boss_info_.hp_cur = cfg_npc->prop_value_vec[i].value;
			boss_info_.hp_max = cfg_npc->prop_value_vec[i].value;
			boss_info_.pre_hp_cur = cfg_npc->prop_value_vec[i].value;
			break;
		}
	}
	return 0;
}

int World_Boss_Manager::inner_act_end(const role_id_t gang_id, Block_Buffer& buf){
	if(act_state_ != ACT_STATE_START){//
		return 0;
	}
	int remain_time = 0;
	buf.read_int32(remain_time);
	act_end_time_ = Time_Value::gettimeofday() + Time_Value(remain_time);
	return 0;
}

World_Boss_Hp_Info& World_Boss_Manager::boss_info(void){
	return this->boss_info_;
}

World_Boss_Player_Info* World_Boss_Manager::get_player_info(const role_id_t role_id){
	World_Boss_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it != player_info_map_.end()){
		return &(it->second);
	}
	return NULL;
}

bool World_Boss_Manager::find_player(const role_id_t role_id){
	World_Boss_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it != player_info_map_.end()){
		return true;
	}
	return false;
}

const World_Boss_Hp_Info* World_Boss_Manager::get_boss_info(void){
	return &boss_info_;
}

bool World_Boss_Manager::check_first_blood(Scene_Player* player){
	if(!player){
		return false;
	}
	if(first_blood_role_id_ != 0){
		return false;
	}
	first_blood_role_id_ = player->role_id();
	World_Boss_Player_Info* player_info = get_player_info(player->role_id());
	if(player_info){
		player_info->achieve_4 = 1;
		return true;
	}
	return false;
}

void World_Boss_Manager::refresh_rank(void){
	int count = rank_info_.size();
	if(count <= 0){
		return;
	}
	int career_1_num = 0;
	int career_2_num = 0;
	int career_3_num = 0;
	rank_show_info_.clear();
	rank_show_info_1.clear();
	rank_show_info_2.clear();
	rank_show_info_3.clear();
	std::sort(rank_info_.begin(), rank_info_.end(), World_Boss_Rank_Greater);
	for(int i = 0; i < count; ++i){
		if(rank_info_[i]->rank_info.career == 1){
			++career_1_num;
			rank_info_[i]->career_rank = career_1_num;
			if(career_1_num <= rank_show_num){
				rank_info_[i]->rank_info.rank = career_1_num;
				rank_show_info_1.push_back(rank_info_[i]->rank_info);
			}

		}else if(rank_info_[i]->rank_info.career == 2){
			++career_2_num;
			rank_info_[i]->career_rank = career_2_num;
			if(career_2_num <= rank_show_num){
				rank_info_[i]->rank_info.rank = career_2_num;
				rank_show_info_2.push_back(rank_info_[i]->rank_info);
			}
		}else{
			++career_3_num;
			rank_info_[i]->career_rank = career_3_num;
			if(career_3_num <= rank_show_num){
				rank_info_[i]->rank_info.rank = career_3_num;
				rank_show_info_3.push_back(rank_info_[i]->rank_info);
			}
		}
		rank_info_[i]->rank_info.rank = i+1;
		if(i < rank_show_num){
			rank_show_info_.push_back(rank_info_[i]->rank_info);
		}
	}

}

void World_Boss_Manager::set_player_revive_time(const role_id_t role_id, const int time){
	World_Boss_Player_Info* player_info = get_player_info(role_id);
	if(player_info){
		player_info->set_revive_time(time);
	}
}

bool World_Boss_Manager::player_is_revive(role_id_t role_id){
	return false;
}

void World_Boss_Manager::player_act_guild_bless(Scene_Player* player){
	if(!player){
		return;
	}
	if(!player->move_scene()){
		return;
	}
	Move_Scene* tg_scene = NULL;
	switch(icon_id_){
		case 16:{
			if(player->move_scene()->scene_type() == World_Boss_Scene_Type){
				tg_scene = player->move_scene();
			}
			break;
		}
		case 19:{
			if(player->move_scene()->scene_type() == Gang_Boss_Scene_Type){
				tg_scene = player->move_scene();
			}
			break;
		}
		default:{
			break;
		}
	}

	int bless_num = CONFIG_CACHE_WORLD_BOSS->get_ability0_num(0, icon_id_);
	role_id_t guild_id = player->gang_id();
	int gang_pos = player->ganger_detail().headship;
	World_Boss_Gang_Bless gang_bless;
	gang_bless.role_id = player->role_id();
	gang_bless.role_name = player->role_name();
	gang_bless.bless_num = bless_num;
	gang_bless_map_[guild_id] = gang_bless;

	MSG_81000601 msg;
	msg.icon_id = icon_id_;
	msg.bless_num = bless_num;
	msg.role_id = player->role_id();
	msg.role_name = player->role_name();
	msg.role_pos = gang_pos;
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	World_Boss_Player_Info_Map::iterator it;
	for(it = player_info_map_.begin(); it != player_info_map_.end(); ++it){
		if(it->second.gang_id == guild_id){
			it->second.ability_0.val_2 = bless_num;
			if(!tg_scene){
				continue;
			}
			Mover* mover = tg_scene->find_mover_with_layer(0, it->second.rank_info.role_id);
			if(mover){
				mover->send_to_client(buf);
			}
		}
	}
	switch(icon_id_){
		case 16:{
			World_Boss_Scene* wb_scene = dynamic_cast<World_Boss_Scene*>(player->move_scene());
			if(wb_scene){
				wb_scene->announce_to_guild(guild_id, 7006, player->role_id(), player->role_name());
			}
			break;
		}
		case 19:{
			Gang_Boss_Scene* wb_scene = dynamic_cast<Gang_Boss_Scene*>(player->move_scene());
			if(wb_scene){
				wb_scene->announce_to_guild(guild_id, 7106, player->role_id(), player->role_name());
			}
			break;
		}
		default:{
			break;
		}
	}
}
void World_Boss_Manager::player_damage(const role_id_t role_id, const int damage, int& bonus_id, role_id_t& bonus_role_id, std::string& bonus_role_name){

	World_Boss_Player_Info* player_info = get_player_info(role_id);
	if(!player_info){
		return;
	}
	player_info->rank_info.damage += damage;
	player_info->fight_damage += damage;
	double max_hp = boss_info_.hp_max;
	player_info->rank_info.damage_rate = (double)player_info->rank_info.damage/max_hp;
	// boss领袖加成
	if(boss_info_.bonus_id == 0){
		double leader_hurt_coe = CONFIG_CACHE_WORLD_BOSS->leader_hurt_coe(icon_id_);
		if(player_info->rank_info.damage_rate >= leader_hurt_coe){
			boss_info_.bonus_id = 1;
			boss_info_.bonus_role_id = role_id;
			boss_info_.bonus_role_name = player_info->rank_info.role_name;
			bonus_id = 1;
			bonus_role_id = player_info->rank_info.role_id;
			bonus_role_name = player_info->rank_info.role_name;
		}
	}
	// 初级勇者指标
	if(player_info->achieve_1 == 0){
		int value = CONFIG_CACHE_WORLD_BOSS->achieve_1_coe(boss_info_.cfg_boss_lv, icon_id_);
		if(player_info->rank_info.damage >= value){
			player_info->achieve_1 = 1;
		}
	}
	// 中级勇者指标
	if(player_info->achieve_2 == 0){
		int value = CONFIG_CACHE_WORLD_BOSS->achieve_2_coe(boss_info_.cfg_boss_lv, icon_id_);
		if(player_info->rank_info.damage >= value){
			player_info->achieve_2 = 1;
		}
	}
	// 高级勇者指标
	if(player_info->achieve_3 == 0){
		int value = CONFIG_CACHE_WORLD_BOSS->achieve_3_coe(boss_info_.cfg_boss_lv, icon_id_);
		if(player_info->rank_info.damage >= value){
			player_info->achieve_3 = 1;
		}
	}
}

void World_Boss_Manager::get_first_rank_info(role_id_t& role_id_1, std::string& role_name_1, role_id_t& role_id_2, std::string& role_name_2, role_id_t& role_id_3, std::string& role_name_3){
	if(rank_show_info_1.empty() == false){
		role_id_1 = rank_show_info_1[0].role_id;
		role_name_1 = rank_show_info_1[0].role_name;
	}
	if(rank_show_info_2.empty() == false){
		role_id_2 = rank_show_info_2[0].role_id;
		role_name_2 = rank_show_info_2[0].role_name;
	}
	if(rank_show_info_3.empty() == false){
		role_id_3 = rank_show_info_3[0].role_id;
		role_name_3 = rank_show_info_3[0].role_name;
	}
}

void World_Boss_Manager::listen_gang_info_sync(const gang_id_t src_gang_id, Scene_Player* player){
	if(!player){
		return;
	}
	if(!player->move_scene()){
		return;
	}
	switch(icon_id_){
		case 16:{
			if(dynamic_cast<World_Boss_Scene*>(player->move_scene()) == NULL){
				return;
			}
			break;
		}
		case 19:{
			if(dynamic_cast<Gang_Boss_Scene*>(player->move_scene()) == NULL){
				return;
			}
			break;
		}
		default:{
			return;
			break;
		}
	}

	World_Boss_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return;
	}
	bool notice_to_client = false;
	//ability0 公会鼓舞
	int64_t gang_id = player->gang_id();
	player_info->gang_id = gang_id;
	if(gang_id == 0){
		if(icon_id_ == 19){// 公会boss，退出公会踢出场景
			del_player_info(player->role_id());
			Gang_Boss_Scene* gang_scene = dynamic_cast<Gang_Boss_Scene*>(player->move_scene());
			if(gang_scene){
				gang_scene->kick_player(player->role_id());
			}
			return;
		}
		if(player_info->ability_0.val_2 != 0){
			player_info->ability_0.val_2 = 0;
			notice_to_client = true;
		}
	}else{
		int src_bless_max_num = player_info->ability_0.val_2;
		World_Boss_Gang_Bless_Map::iterator it = gang_bless_map_.find(gang_id);
		if(it != gang_bless_map_.end()){
			player_info->ability_0.val_2 = it->second.bless_num;
		}
		if(src_bless_max_num != player_info->ability_0.val_2){
			notice_to_client = true;
		}
	}
	if(notice_to_client){
		//场景信息
		MSG_81000600 msg_act_info;
		msg_act_info.icon_id = icon_id_;
		World_Boss_Gang_Bless_Map::const_iterator it = gang_bless_map_.find(gang_id);
		if(it != gang_bless_map_.end()){
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

		int act_time_type = 0;
		switch(icon_id_){
			case 16:{
				if(dynamic_cast<World_Boss_Scene*>(player->move_scene())){
					act_time_type = dynamic_cast<World_Boss_Scene*>(player->move_scene())->get_world_boss_act_time_type();
				}
				break;
			}
			case 19:{
				if(dynamic_cast<Gang_Boss_Scene*>(player->move_scene())){
					act_time_type = dynamic_cast<Gang_Boss_Scene*>(player->move_scene())->get_world_boss_act_time_type();
				}
				break;
			}
			default:{
				break;
			}
		}
		msg_act_info.act_time_type = act_time_type;
		int remain_time = act_end_time_.sec() - Time_Value::gettimeofday().sec();
		if(act_time_type == 0){
			remain_time -= act_fight_time_;
		}
		if(remain_time < 0){
			remain_time = 0;
		}
		msg_act_info.act_time = remain_time;
		msg_act_info.boss_cur_hp = boss_info_.hp_cur;
		msg_act_info.boss_max_hp = boss_info_.hp_max;
		msg_act_info.boss_type_id = boss_info_.boss_type_id;
		msg_act_info.boss_name = boss_info_.boss_name;
		msg_act_info.boss_buffer_type = boss_info_.buffer_step;
		if(msg_act_info.boss_buffer_type == 3){
			msg_act_info.boss_buffer_type = 0;
		}
		msg_act_info.boss_bonus_id = boss_info_.bonus_id;
		msg_act_info.boss_bonus_role_name = boss_info_.bonus_role_name;
		msg_act_info.boss_kill_num = act_detail_.kill_num;
		msg_act_info.boss_lv = act_detail_.boss_lv;
		msg_act_info.revive_time = player_info->get_revive_time();
		OBJ_SEND_TO_CLIENT(msg_act_info, (*player));
	}
}

void World_Boss_Manager::listen_vip_info_change(Scene_Player* player){
	if(!player){
		return;
	}
	if(!player->move_scene()){
		return;
	}
	switch(icon_id_){
		case 16:{
			if(dynamic_cast<World_Boss_Scene*>(player->move_scene()) == NULL){
				return;
			}
			break;
		}
		case 19:{
			if(dynamic_cast<Gang_Boss_Scene*>(player->move_scene()) == NULL){
				return;
			}
			break;
		}
		default:{
			return;
			break;
		}
	}

	World_Boss_Player_Info* player_info = get_player_info(player->role_id());
	if(!player_info){
		return;
	}
	int vip_lv = player->vip();
	int64_t gang_id = player->gang_id();
	player_info->ability_1.val_2 = CONFIG_CACHE_WORLD_BOSS->get_ability1_num(vip_lv, icon_id_);
	// ability2 激发潜能
	player_info->ability_2.val_2 = CONFIG_CACHE_WORLD_BOSS->get_ability2_num(vip_lv, icon_id_);
	// 财神祝福
	player_info->ability_3.val_2 = CONFIG_CACHE_WORLD_BOSS->get_ability3_num(vip_lv, icon_id_);

	{
		//场景信息
		MSG_81000600 msg_act_info;
		msg_act_info.icon_id = icon_id_;
		World_Boss_Gang_Bless_Map::const_iterator it = gang_bless_map_.find(gang_id);
		if(it != gang_bless_map_.end()){
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

		int act_time_type = 0;
		switch(icon_id_){
			case 16:{
				if(dynamic_cast<World_Boss_Scene*>(player->move_scene())){
					act_time_type = dynamic_cast<World_Boss_Scene*>(player->move_scene())->get_world_boss_act_time_type();
				}
				break;
			}
			case 19:{
				if(dynamic_cast<Gang_Boss_Scene*>(player->move_scene())){
					act_time_type = dynamic_cast<Gang_Boss_Scene*>(player->move_scene())->get_world_boss_act_time_type();
				}
				break;
			}
			default:{
				break;
			}
		}
		msg_act_info.act_time_type = act_time_type;
		int remain_time = act_end_time_.sec() - Time_Value::gettimeofday().sec();
		if(act_time_type == 0){
			remain_time -= act_fight_time_;
		}
		if(remain_time < 0){
			remain_time = 0;
		}
		msg_act_info.act_time = remain_time;
		msg_act_info.boss_cur_hp = boss_info_.hp_cur;
		msg_act_info.boss_max_hp = boss_info_.hp_max;
		msg_act_info.boss_type_id = boss_info_.boss_type_id;
		msg_act_info.boss_name = boss_info_.boss_name;
		msg_act_info.boss_buffer_type = boss_info_.buffer_step;
		if(msg_act_info.boss_buffer_type == 3){
			msg_act_info.boss_buffer_type = 0;
		}
		msg_act_info.boss_bonus_id = boss_info_.bonus_id;
		msg_act_info.boss_bonus_role_name = boss_info_.bonus_role_name;
		msg_act_info.boss_kill_num = act_detail_.kill_num;
		msg_act_info.boss_lv = act_detail_.boss_lv;
		msg_act_info.revive_time = player_info->get_revive_time();
		OBJ_SEND_TO_CLIENT(msg_act_info, (*player));

	}
}

void World_Boss_Manager::del_player_info(const role_id_t role_id){
	World_Boss_Player_Info_Map::iterator it = player_info_map_.find(role_id);
	if(it == player_info_map_.end()){
		return;
	}
	World_Boss_Player_Info* player_info = &(it->second);
	std::vector<World_Boss_Player_Info*>::iterator s_it;
	for(s_it = rank_info_.begin(); s_it != rank_info_.end(); ++s_it){
		if(player_info == (*s_it)){
			rank_info_.erase(s_it);
			break;
		}
	}
	player_info_map_.erase(it);
	rank_show_info_.clear();
	rank_show_info_1.clear();
	rank_show_info_2.clear();
	rank_show_info_3.clear();
	this->refresh_rank();
}
