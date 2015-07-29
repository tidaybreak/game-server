/*
 * Honor_Arena_Scene.cpp
 *
 *  Created on: 2015年3月16日
 *      Author: lys
 */

#include "Honor_Arena_Scene.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Honor_Arena_Struct.h"
#include "NPC/NPC_Manager.h"
#include "honor_arena/Honor_Arena_Manager.h"
#include "Config_Cache_Honor_Arena.h"
#include "Scene_Monitor.h"
#include "Scene_Player.h"
#include "pack/Pack_Struct.h"
#include "Battle_Scene.h"
#include "announce/Scene_Announce_Manager.h"
#include "chater/Chater_Def.h"
#include "announcement/Announcement_Struct.h"
#include "Config_Cache_Item.h"
#include "math.h"

const int honor_arena_act_icon_id = 22;
const int MERIT_ID = 503003;
const int SCORE_ID = 1;

Honor_Arena_Scene::Honor_Arena_Scene(void){
	reset();
}

Honor_Arena_Scene::~Honor_Arena_Scene(void){

}

void Honor_Arena_Scene::reset(void){
	Move_Scene::reset();
	logic_uuid_ = 0;
	act_state_ = 0;
	act_result_ = -1;
	act_end_time_ = Time_Value::zero;
	balance_time_ = Time_Value::zero;
	cure_reduce_buff_id_ = 0;
}

void Honor_Arena_Scene::set_logic_uuid(const int64_t uuid){
	logic_uuid_ = uuid;
}

int Honor_Arena_Scene::tick(const Time_Value &now){
	if(this->is_recover_status()){
		if(now >= balance_time_){
			kick_all_player();
			balance_time_ = now + Time_Value(4);
		}
		return 0;
	}
	Move_Scene::tick(now);
	HONOR_ARENA_MANAGER->tick(now);
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

int Honor_Arena_Scene::enter_scene(Mover *mover){
	Move_Scene::enter_scene(mover);
	if(mover && mover->player_self()){
		if(this->is_recover_status()){
			int dest = 0;
			mover->player_self()->inner_transmit(dest,0,0);
			return 0;
		}
		HONOR_ARENA_MANAGER->player_enter_scene(mover->player_self());
		add_cure_reduce_buffer(mover->player_self());
	}
	return 0;
}

int Honor_Arena_Scene::exit_scene(Mover *mover, bool offline){
	Move_Scene::exit_scene(mover, offline);
	if(mover && mover->player_self()){
		HONOR_ARENA_MANAGER->player_exit_scene(mover->player_self());
		del_cure_reduce_buffer(mover->player_self());
	}
	return 0;
}

int Honor_Arena_Scene::init_scene_normal(int scene_id, int scene_order, int map_id/* = 0*/){
	Move_Scene::init_scene_normal(scene_id, scene_order, map_id);
	act_end_time_ = HONOR_ARENA_MANAGER->act_end_time_;
	scene_status_ = SCENE_READY_STATUS;
	return 0;
}
int Honor_Arena_Scene::client_scene_complete(Fighter &fighter) {
	if(fighter.player_self() == NULL){
		return 0;
	}
	Move_Scene::client_scene_complete(fighter);
	//场景信息
	return 0;
}

void Honor_Arena_Scene::hook_player_enter_battle(Scene_Player* player){
	if(!player){
		return;
	}
}

void Honor_Arena_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters){
	Scene_Player* win_player = NULL;
	Scene_Player* lose_player = NULL;
	Honor_Arena_Player_Info* win_info = NULL;
	Honor_Arena_Player_Info* lose_info = NULL;

	{
		int count = win_mover_vec.size();
		for(int i = 0; i < count; ++i){
			Scene_Player* player  = win_mover_vec[i]->player_self();
			if(player){
				win_player = player;
				break;
			}
		}
	}
	{
		int count = lose_mover_vec.size();
		for(int i = 0; i < count; ++i){
			Scene_Player* player  = lose_mover_vec[i]->player_self();
			if(player){
				lose_player = player;
				break;
			}
		}
	}
	if(!win_player || !lose_player){
		return;
	}
	win_info = HONOR_ARENA_MANAGER->get_player_info(win_player->role_id());
	lose_info = HONOR_ARENA_MANAGER->get_player_info(lose_player->role_id());
	if(!win_info || !lose_player){
		return;
	}
	int finish_streak_win = lose_info->streak_win;
	win_info->fight_win();
	lose_info->fight_lose();
	// 胜负奖励
	Block_Buffer reward_buf;
	reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
	reward_buf.write_int32(honor_arena_act_icon_id);
	reward_buf.write_int8(3);
	int win_src_score = win_info->rank_info.score;
	int lose_src_score = lose_info->rank_info.score;
	int win_src_rank = win_info->rank_info.rank;
	int lose_src_rank = lose_info->rank_info.rank;
	int win_score = 0;
	int win_copper = 0;
	int win_merit = 0;
	int lose_score = 0;
	int lose_copper = 0;
	int lose_merit = 0;
	std::vector<Item_Info> report_items;
	Item_Info report_item;

	CONFIG_CACHE_HONOR_ARENA->get_fight_lose_reward(lose_score, lose_copper, lose_merit);
	lose_info->add_score(lose_score);
	lose_info->add_reward_to_gain_items(SCORE_ID, lose_score);
	lose_info->add_reward_to_gain_items(COPPER, lose_copper);
	lose_info->add_reward_to_gain_items(MERIT_ID, lose_merit);
	reward_buf.write_int64(lose_player->role_id());
	reward_buf.write_int32(lose_copper);
	reward_buf.write_int32(lose_merit);

	CONFIG_CACHE_HONOR_ARENA->get_fight_win_reward(win_score, win_copper, win_merit);
	// 战报
	//0 XXX战胜了XXX，获得
	report_items.clear();
	report_item.id = SCORE_ID;
	report_item.amount = win_score;
	report_items.push_back(report_item);
	report_item.id = COPPER;
	report_item.amount = win_copper;
	report_items.push_back(report_item);
	report_item.id = MERIT_ID;
	report_item.amount = win_merit;
	report_items.push_back(report_item);
	this->broadcast_fight_report(win_info->role_info.base_info.role_id, win_info->role_info.base_info.role_name,
			lose_info->role_info.base_info.role_id, lose_info->role_info.base_info.role_name, 0, 0, report_items);

	int add_score = 0;
	int add_merit = 0;

	//  连胜奖励
	CONFIG_CACHE_HONOR_ARENA->get_streak_win_reward(win_info->streak_win, add_score, add_merit);
	if(add_score > 0 || add_merit > 0){
		win_score += add_score;
		win_merit += add_merit;
		// 战报
		//1 XXX战胜了XXX，赢得了X连胜，获得
		report_items.clear();
		report_item.id = SCORE_ID;
		report_item.amount = add_score;
		report_items.push_back(report_item);
		report_item.id = MERIT_ID;
		report_item.amount = add_merit;
		report_items.push_back(report_item);
		this->broadcast_fight_report(win_info->role_info.base_info.role_id, win_info->role_info.base_info.role_name,
				lose_info->role_info.base_info.role_id, lose_info->role_info.base_info.role_name, 1, win_info->streak_win, report_items);
	}

	// 终结连胜奖励
	add_score = 0;
	add_merit = 0;
	CONFIG_CACHE_HONOR_ARENA->get_finish_streak_win_reward(finish_streak_win, add_score, add_merit);
	if(add_score > 0 || add_merit > 0){
		win_score += add_score;
		win_merit += add_merit;
		// 战报
		//2 XXX终结了XXX的X连胜，获得
		report_items.clear();
		report_item.id = SCORE_ID;
		report_item.amount = add_score;
		report_items.push_back(report_item);
		report_item.id = MERIT_ID;
		report_item.amount = add_merit;
		report_items.push_back(report_item);
		this->broadcast_fight_report(win_info->role_info.base_info.role_id, win_info->role_info.base_info.role_name,
				lose_info->role_info.base_info.role_id, lose_info->role_info.base_info.role_name, 2, finish_streak_win, report_items);
	}

	win_info->add_score(win_score);
	win_info->add_reward_to_gain_items(SCORE_ID, win_score);
	win_info->add_reward_to_gain_items(COPPER, win_copper);
	win_info->add_reward_to_gain_items(MERIT_ID, win_merit);
	reward_buf.write_int64(win_player->role_id());
	reward_buf.write_int32(win_copper);
	reward_buf.write_int32(win_merit);

	reward_buf.finish_message();
	SCENE_MONITOR->send_to_logic(logic_uuid_, reward_buf);
	// 排名变化公告
	Honor_Arena_Player_Info* first_info = HONOR_ARENA_MANAGER->get_player_info_from_rank(1);
	// 刷新排名
	HONOR_ARENA_MANAGER->refresh_rank();
	handle_rank_ann(first_info, lose_info, lose_src_rank);
	handle_rank_ann(first_info, win_info, win_src_rank);
	// 战斗结果通知客户端
	{
		MSG_50211203 msg;
		msg.result = 1;
		msg.rank = win_info->rank_info.rank;
		msg.rank_up = win_src_rank - msg.rank;
		msg.score = win_info->rank_info.score;
		msg.score_up = msg.score - win_src_score;
		Item_Info item;
		item.id = COPPER;
		item.amount = win_copper;
		msg.items.push_back(item);
		item.id = MERIT_ID;
		item.amount = win_merit;
		msg.items.push_back(item);
		OBJ_SEND_TO_CLIENT(msg, (*win_player));
	}
	// 战斗结果通知客户端
	{
		MSG_50211203 msg;
		msg.result = 0;
		msg.rank = lose_info->rank_info.rank;
		msg.rank_up = lose_src_rank - msg.rank;
		msg.score = lose_info->rank_info.score;
		msg.score_up = msg.score - lose_src_score;
		Item_Info item;
		item.id = COPPER;
		item.amount = lose_copper;
		msg.items.push_back(item);
		item.id = MERIT_ID;
		item.amount = lose_merit;
		msg.items.push_back(item);
		OBJ_SEND_TO_CLIENT(msg, (*lose_player));
	}
	win_info->state = 0;
	lose_info->state = 0;
	//
	//recover hp
	{
		Prop_Setter_Vec fight_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_CURRENT;
		setter.operate = O_SET;
		setter.addi_type = AT_BASIC;
		setter.basic = win_player->blood_max();
		fight_setter.push_back(setter);
		win_player->modify_props_normal(fight_setter, true, 21);
	}
	{
		Prop_Setter_Vec fight_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_CURRENT;
		setter.operate = O_SET;
		setter.addi_type = AT_BASIC;
		setter.basic = lose_player->blood_max();
		fight_setter.push_back(setter);
		lose_player->modify_props_normal(fight_setter, true, 21);
	}
}

int Honor_Arena_Scene::add_show_reward(const role_id_t role_id, const int type){
	const Honor_Arena_Show_Config* cfg = CONFIG_CACHE_HONOR_ARENA->get_show_config(type);
	if(cfg){
		Block_Buffer reward_buf;
		reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
		reward_buf.write_int32(honor_arena_act_icon_id);
		reward_buf.write_int8(2);
		reward_buf.write_int64(role_id);
		reward_buf.write_int32(type);
		int count = cfg->continuity_award.size();
		reward_buf.write_int32(count);
		for(int i = 0; i < count; ++i){
			reward_buf.write_int32(cfg->continuity_award[i].val_1);
			reward_buf.write_int32(cfg->continuity_award[i].val_2);
		}
		reward_buf.finish_message();
		SCENE_MONITOR->send_to_logic(logic_uuid_, reward_buf);
	}
	return 0;
}

void Honor_Arena_Scene::kick_all_player(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		std::vector<Scene_Player*> player_vec;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				// 恢复进入的血量
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

void Honor_Arena_Scene::act_over(void){
	HONOR_ARENA_MANAGER->refresh_rank();
//	//奖励
//	MSG_81000602 msg;
//	msg.icon_id = world_boss_act_icon_id;
//	msg.result = act_result_;
//	msg.boss_kill_num = WORLD_BOSS_MANAGER->act_detail_.kill_num;

	int8_t send_mail = 0;
	Block_Buffer reward_buf;
	reward_buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
	reward_buf.write_int32(honor_arena_act_icon_id);
	reward_buf.write_int8(1);
	int act_player_num = HONOR_ARENA_MANAGER->player_info_map_.size();
	reward_buf.write_int32(act_player_num);

	Item_Info item_info;
	item_info.bind = Item_Detail::BIND;

	Honor_Arena_Player_Info_Map::const_iterator player_it;
	for(player_it = HONOR_ARENA_MANAGER->player_info_map_.begin(); player_it != HONOR_ARENA_MANAGER->player_info_map_.end(); ++player_it){
		send_mail = 1;
		role_id_t player_role_id = player_it->second.rank_info.role_id;
		//rank reward
		int self_rank = player_it->second.rank_info.rank;
		Int_Int_Vec rank_items = CONFIG_CACHE_HONOR_ARENA->get_rank_reward(self_rank);
		int item_num = rank_items.size();
		//to logic
		reward_buf.write_int64(player_role_id);
		reward_buf.write_int32(self_rank);
		reward_buf.write_int8(send_mail);
		reward_buf.write_int32(item_num);
		Int_Int_Vec::iterator it;
		for(it = rank_items.begin(); it != rank_items.end(); ++it){
			reward_buf.write_int32(it->val_1);
			reward_buf.write_int32(it->val_2);
		}
	}
	reward_buf.finish_message();
	SCENE_MONITOR->send_to_logic(logic_uuid_, reward_buf);

//	{// act_end
//		Block_Buffer end_buf;
//		end_buf.make_message(INNER_ACT_SCENE_END);
//		end_buf.write_int32(world_boss_act_icon_id);
//		end_buf.write_int64(0);
//		end_buf.write_int32(act_result_);
//		end_buf.finish_message();
//		SCENE_MONITOR->send_to_logic(logic_uuid_, end_buf);
//	}

//	// announce act end
//	role_id_t first_role_id_1 = 0;
//	role_id_t first_role_id_2 = 0;
//	role_id_t first_role_id_3 = 0;
//	std::string first_role_name_1 = "";
//	std::string first_role_name_2 = "";
//	std::string first_role_name_3 = "";
//	WORLD_BOSS_MANAGER->get_first_rank_info(first_role_id_1, first_role_name_1,
//			first_role_id_2, first_role_name_2,
//			first_role_id_3, first_role_name_3);
//	if(first_role_id_1 != 0 || first_role_id_2 != 0 || first_role_id_3 != 0){
//		std::stringstream ostr;
//		const World_Boss_Reward_Config* first_rank = CONFIG_CACHE_WORLD_BOSS->get_reward_config(cfg_boss_lv, 1, world_boss_act_icon_id);
//		if(first_rank){
//			boost::unordered_map<int, int>::iterator first_it;
//			for(first_it = first_rank_reward_map.begin(); first_it != first_rank_reward_map.end(); ++first_it){
//				const Item_Detail_Config * cfg_item = CONFIG_CACHE_ITEM->find_item(first_it->first);
//				if(cfg_item){
//					ostr<<cfg_item->name<<"*"<<first_it->second<< " ";
//				}else{
//					if(first_it->first == COPPER){
//						ostr<<CONFIG_CACHE_ARENA->get_text_copper()<<"*"<<first_it->second<< " ";
//					}else if(first_it->first == SOULS){
//						ostr<<CONFIG_CACHE_ARENA->get_text_soul()<<"*"<<first_it->second<< " ";
//					}
//				}
//			}
//		}
//		if(first_role_id_1 != 0){
//			this->announce_to_all(7004, first_role_id_1, first_role_name_1, ostr.str());
//		}
//		if(first_role_id_2 != 0){
//			this->announce_to_all(7011, first_role_id_2, first_role_name_2, ostr.str());
//		}
//		if(first_role_id_3 != 0){
//			this->announce_to_all(7012, first_role_id_3, first_role_name_3, ostr.str());
//		}
//	}else{
//		this->announce_to_all(7005, 0, "");
//	}
	Honor_Arena_Player_Info* f_info = HONOR_ARENA_MANAGER->get_player_info_from_rank(1);
	Honor_Arena_Player_Info* s_info = HONOR_ARENA_MANAGER->get_player_info_from_rank(2);
	Honor_Arena_Player_Info* t_info = HONOR_ARENA_MANAGER->get_player_info_from_rank(3);
	//xx取得了今日的荣誉竞技场第一名
	if(f_info){
		this->announce_to_all(9003, f_info->role_info.base_info.role_id, f_info->role_info.base_info.role_name, 0, "");
	}
	//今日荣誉竞技场已经结束，恭喜以下玩家获得前三名：
	this->announce_to_all(9004, 0, "", 0, "");
	//第1名：xxx [guild_name]
	if(f_info){
		std::string guild_name = "";
		if(f_info->role_info.base_info.guild_name != ""){
			guild_name = "  [" + f_info->role_info.base_info.guild_name + "]";
		}
		this->announce_to_all(9005, f_info->role_info.base_info.role_id, f_info->role_info.base_info.role_name, 0, "",
				guild_name);
	}
	//第2名：xxx [guild_name]
	if(s_info){
		std::string guild_name = "";
		if(s_info->role_info.base_info.guild_name != ""){
			guild_name = "  [" + s_info->role_info.base_info.guild_name + "]";
		}
		this->announce_to_all(9006, s_info->role_info.base_info.role_id, s_info->role_info.base_info.role_name, 0, "",
				guild_name);
	}
	//第3名：xxx guild_name
	if(t_info){
		std::string guild_name = "";
		if(t_info->role_info.base_info.guild_name != ""){
			guild_name = "  [" + t_info->role_info.base_info.guild_name + "]";
		}
		this->announce_to_all(9007, t_info->role_info.base_info.role_id, t_info->role_info.base_info.role_name, 0, "",
				guild_name);
	}
	HONOR_ARENA_MANAGER->reset();
	//
	kick_all_player();
	this->set_scene_status(SCENE_RECOVER_STATUS);
	recover_time_ = Time_Value::gettimeofday() + Time_Value(10);
}

void Honor_Arena_Scene::force_end_battle(void){
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

void Honor_Arena_Scene::handle_rank_ann(Honor_Arena_Player_Info* first_info, Honor_Arena_Player_Info* player_info, const int src_rank){
	if(!player_info){
		return;
	}
	int report_type;
	std::vector<Item_Info> items;
	// 杀入前100 50 10
	int rank_ann_pos = 0;
	if(src_rank > 100){
		if(player_info->rank_info.rank <= 10){
			rank_ann_pos = 10;
			report_type = 5;
		}else if(player_info->rank_info.rank <= 50){
			rank_ann_pos = 50;
			report_type = 4;
		}else if(player_info->rank_info.rank <= 100){
			rank_ann_pos = 100;
			report_type = 3;
		}
	}else if(src_rank > 50){
		if(player_info->rank_info.rank <= 10){
			rank_ann_pos = 10;
			report_type = 5;
		}else if(player_info->rank_info.rank <= 50){
			rank_ann_pos = 50;
			report_type = 4;
		}
	}else if(src_rank > 10){
		if(player_info->rank_info.rank <= 10){
			rank_ann_pos = 10;
			report_type = 5;
		}
	}
	if(rank_ann_pos != 0){
		std::stringstream str_stream;
		str_stream << rank_ann_pos;
		std::string str_text = str_stream.str();
		this->announce_to_all(9001, player_info->role_info.base_info.role_id,
				player_info->role_info.base_info.role_name, 0, "", str_text);
		this->broadcast_fight_report(player_info->role_info.base_info.role_id,
				player_info->role_info.base_info.role_name, 0, "", report_type, 0, items);
	}

	// 取代了xx，成为荣誉竞技场第一名
	if(first_info && player_info->rank_info.rank == 1 && src_rank != 1){
		this->announce_to_all(9002, player_info->role_info.base_info.role_id,
				player_info->role_info.base_info.role_name, first_info->role_info.base_info.role_id,
				first_info->role_info.base_info.role_name, "");
		report_type = 6;
		this->broadcast_fight_report(player_info->role_info.base_info.role_id,
				player_info->role_info.base_info.role_name, first_info->role_info.base_info.role_id,
				first_info->role_info.base_info.role_name, report_type, 0, items);
	}
}

void Honor_Arena_Scene::broadcast_fight_report(const role_id_t role1, std::string name1, const role_id_t role2, std::string name2,
		const int type, const int value, std::vector<Item_Info> items){
	//0		XXX战胜了XXX，获得
	//1		XXX战胜了XXX，赢得了X连胜，获得
	//2		XXX终结了XXX的X连胜，获得
	//3		XXX过关斩将，杀入前100名，大家都说好厉害！
	//4		XXX过关斩将，杀入前50名，杀神降临！
	//5		XXX过关斩将，杀入前10名，已经无人可挡！
	//6		XXX取代了XXX，成为新晋的第一名！！勇者无畏！
	MSG_50211204 msg;
	msg.report.win_role_id = role1;
	msg.report.win_role_name = name1;
	msg.report.lose_role_id = role2;
	msg.report.lose_role_name = name2;
	msg.report.type.val_1 = type;
	msg.report.type.val_2 = value;
	msg.report.items = items;
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	broadcast_all_within_battle(0, buf);
}

void Honor_Arena_Scene::add_cure_reduce_buffer(Scene_Player* player){
	if(!player){
		return;
	}
	const Honor_Arena_Base_Config* cfg = CONFIG_CACHE_HONOR_ARENA->get_base_config();
	if(!cfg){
		return;
	}
	if(cfg->buff_argv.status_id == 0){
		return;
	}
	Status_Effect_Argv argv;
	argv = cfg->buff_argv;
	argv.caster_id = player->role_id();
	argv.last_time_ms = -1;
	player->insert_status_out_fight(argv);
	cure_reduce_buff_id_ = argv.status_id;
	return;
}

void Honor_Arena_Scene::del_cure_reduce_buffer(Scene_Player* player){
	if(!player){
		return;
	}
	if(cure_reduce_buff_id_ != 0){
		int result = player->erase_status_type(cure_reduce_buff_id_);
		if(result == 0){
			Status_Info_Msg msg_info;
			msg_info.status_id = cure_reduce_buff_id_;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = player->role_id();
			MSG_81000071 msg;
			msg.buff_detail.push_back(msg_info);
			OBJ_SEND_TO_CLIENT(msg, (*player));
		}
	}
}

void Honor_Arena_Scene::announce_to_all(const int ann_id, const role_id_t role_id1, std::string role_name1, const role_id_t role_id2, std::string role_name2,
		std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/, std::string str4/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if(role_id1 != 0){
		role_info.role_id = role_id1;
		role_info.role_name = role_name1;
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
	}
	if(role_id2 != 0){
		role_info.role_id = role_id2;
		role_info.role_name = role_name2;
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

void Honor_Arena_Scene::announce_to_scene(const int ann_id, const role_id_t role_id, std::string role_name,
		std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/, std::string str4/* = ""*/){
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


void Honor_Arena_Scene::announce_to_guild(const int64_t gang_id, const int ann_id, const role_id_t role_id, std::string role_name, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/, std::string str4/* = ""*/){
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


