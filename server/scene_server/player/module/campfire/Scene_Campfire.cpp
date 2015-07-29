/*
 * Scene_Campfire.cpp
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#include "Scene_Campfire.h"
#include "NPC/NPC_Manager.h"
#include "campfire_scene/Campfire_Scene.h"
#include "Scene_Player.h"
#include "Campfire_Data_Manager.h"
#include "campfire_scene/Campfire_Scene_Manager.h"
#include "Config_Cache_Secret_Shop.h"
#include "Msg_Inner_Struct.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"

Scene_Campfire::Scene_Campfire(void){

}

Scene_Campfire::~Scene_Campfire(void){

}

void Scene_Campfire::reset(void){

}

void Scene_Campfire::sign_in(void){
}

void Scene_Campfire::sign_out(void){

}

int Scene_Campfire::time_up(const Time_Value& now){
	return 0;
}

Scene_Campfire_Info* Scene_Campfire::campfire_info(void){
	return &campfire_info_;
}

void Scene_Campfire::set_campfire_info(Scene_Campfire_Info* info){
	if(info){
		campfire_info_ = *info;
	}
}

void Scene_Campfire::listen_gang_info_sync(void){
	if(player_self()){
		campfire_info_.gang_id = player_self()->base_detail().gang_id;
		campfire_info_.gang_name = player_self()->base_detail().gang_name;
		campfire_info_.headship = player_self()->ganger_detail().headship;
		Campfire_Scene* scene = dynamic_cast<Campfire_Scene* >(move_scene());
		if(scene){
			return scene->listen_gang_info_sync(player_self());
		}
	}
}

int Scene_Campfire::req_campfire_add_firewood(role_id_t fire_role_id){
	if(move_scene() == NULL || player_self() == NULL){
		return -1;
	}
	Material* material = NPC_MANAGER->find_material_npc(fire_role_id);
	if(!material){
		return -1;
	}
	Campfire_Scene* scene = dynamic_cast<Campfire_Scene* >(move_scene());
	if(!scene){
		return -1;
	}
	return scene->req_campfire_add_firewood(player_self(), material);
}

int Scene_Campfire::req_campfire_fire(role_id_t fire_role_id){
	if(move_scene() == NULL || player_self() == NULL){
		return -1;
	}
	Material* material = NPC_MANAGER->find_material_npc(fire_role_id);
	if(!material){
		return -1;
	}
	Campfire_Scene* scene = dynamic_cast<Campfire_Scene* >(move_scene());
	if(!scene){
		return -1;
	}
	return scene->req_campfire_fire(player_self(), material);
}

int Scene_Campfire::req_campfire_rank_list() {
	if(move_scene() == NULL || player_self() == NULL){
		return -1;
	}
	Campfire_Scene* scene = dynamic_cast<Campfire_Scene* >(move_scene());
	if(!scene){
		return -1;
	}
	scene->req_rank_list_info(player_self());
	return 0;
}

int Scene_Campfire::send_campfire_spot_info() {
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(role_id());
		if (player_info == 0)
			return -1;

		MSG_81000701 msg;
		msg.rank = player_info->rank;
		msg.rank_award = player_info->rank_rev_stat;

		if (player_info->is_lighter)
			msg.campfire_award_type = 0;
		else
			msg.campfire_award_type = 1;
		msg.campfire_award = player_info->recieve_stat;

		Block_Buffer buffer;
		buffer.make_message(msg.msg_id);
		msg.serialize(buffer);
		buffer.finish_message();

		player_self()->send_to_client(buffer);
		return 0;
}

int Scene_Campfire::send_campfire_personal_info() {
	//要读火柴人数据，放Manager做算了
	return CAMPFIRE_SCENE_MANAGER->send_campfire_personal_info(this->player_self());
}

int Scene_Campfire::save_campfire_player_info() {
	Scene_Campfire_Info* info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player_self()->role_id());
	if (!info)
		return -1;

	MSG_20300116 msg;
	msg.role_id = info->role_id;
	msg.finish_time = info->finish_time.sec();
	msg.rank = info->rank;
	boost::unordered_set<int32_t>::const_iterator iter;
	for (iter = info->killed_match_man.begin();
			iter != info->killed_match_man.end(); ++iter) {
		msg.killed_match_man.push_back(*iter);
	}
	msg.is_lighter = info->is_lighter;
	msg.recieve_stat = info->recieve_stat;
	msg.rank_rev_stat = info->rank_rev_stat;
	msg.add_firewood = info->add_firewood;

	Block_Buffer buff;
	buff.make_message(msg.msg_id);
	msg.serialize(buff);
	buff.finish_message();

	player_self()->send_to_logic(buff);

	set_campfire_info(info);
	return 0;
}

int Scene_Campfire::get_campfire_award() {
	if (player_self()->base_detail().gang_id == 0) {
		return ERROR_BONFIRE_AWARD_NO_GANG;
	}
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player_self()->role_id());
	if (player_info == 0)
		return -1;

	if (player_info->recieve_stat == 0)
		return ERROR_BONFIRE_AWARD_NOT_FINISHED;
	if (player_info->recieve_stat == 2)
		return ERROR_BONFIRE_AWARD_HAS_RECEIVED;

	MSG_20300113 msg;

	int type;
	if (player_info->is_lighter)
		type = Campfire_Award_Config::CAMPFIRE_AWARD_LIT;
	else
		type = Campfire_Award_Config::CAMPFIRE_AWARD_ADD_WOOD;
	msg.type = type;

	Campfire_Award_Item_Vec awards = CONFIG_CACHE_SECRET_SHOP->campfire_award_config()->award_configs.find(type)->second;
	Campfire_Award_Item_Vec::const_iterator iter1;
	for (iter1 = awards.begin(); iter1 != awards.end(); ++iter1) {
		Item_Info info;
		info.id = (*iter1).item_id;
		info.amount = (*iter1).item_count;
		info.bind = (*iter1).bind_type;
		msg.items.push_back(info);
	}

	Block_Buffer buffer;
	buffer.make_message(msg.msg_id);
	msg.serialize(buffer);
	buffer.finish_message();

	player_self()->send_to_logic(buffer);
	return 0;
}

int Scene_Campfire::get_campfire_rank_award() {
//	if (player_self()->base_detail().gang_id == 0) {
//		return ERROR_BONFIRE_AWARD_NO_GANG;
//	}
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player_self()->role_id());
	if (player_info == 0)
		return -1;

	if (player_info->rank_rev_stat == 2) {
		return ERROR_BONFIRE_AWARD_HAS_RECEIVED;
	}

	int type;
	uint32_t rank = player_info->rank;
	Campfire_Award_Item_Vec awards;
	if (rank > 0) {
		Rank_Award_Config_Vec vec = CONFIG_CACHE_SECRET_SHOP->campfire_award_config()->rank_award_configs;
		Rank_Award_Config_Vec::const_iterator award_iter;
		for (award_iter = vec.begin(); award_iter != vec.end(); ++award_iter) {
			if (rank >= (*award_iter).rank_start && rank <= (*award_iter).rank_stop) {
				type = Campfire_Award_Config::CAMPFIRE_AWARD_RANK;
				awards = (*award_iter).awards;
				break;
			}
		}
	}

	//安慰奖
	if (awards.empty()) {
		type = Campfire_Award_Config::CAMPFIRE_AWARD_CONSOLATION;
		awards = CONFIG_CACHE_SECRET_SHOP->campfire_award_config()->award_configs.find(Campfire_Award_Config::CAMPFIRE_AWARD_CONSOLATION)->second;
	}

	MSG_20300113 msg;
	msg.type = type;

	Campfire_Award_Item_Vec::const_iterator iter1;
	for (iter1 = awards.begin(); iter1 != awards.end(); ++iter1) {
		Item_Info info;
		info.id = (*iter1).item_id;
		info.amount = (*iter1).item_count;
		info.bind = (*iter1).bind_type;
		msg.items.push_back(info);
	}

	Block_Buffer buffer;
	buffer.make_message(msg.msg_id);
	msg.serialize(buffer);
	buffer.finish_message();

	player_self()->send_to_logic(buffer);
	return 0;
}

int Scene_Campfire::get_campfire_award_result(const MSG_20300114& msg) {
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player_self()->role_id());
	if (player_info == 0)
		return -1;

	if (msg.type == Campfire_Award_Config::CAMPFIRE_AWARD_LIT || msg.type == Campfire_Award_Config::CAMPFIRE_AWARD_ADD_WOOD)
	{
		player_info->recieve_stat = 2;

		send_campfire_personal_info();

		MSG_50200704 msg_res;
		if (msg.result == 0)
			msg_res.result = 1;
		else
			msg_res.result = 0;

		save_campfire_player_info();

		Block_Buffer buffer;
		buffer.make_message(msg_res.msg_id);
		msg_res.serialize(buffer);
		buffer.finish_message();
		player_self()->send_to_client(buffer);
	}
	else if (msg.type == Campfire_Award_Config::CAMPFIRE_AWARD_CONSOLATION || msg.type == Campfire_Award_Config::CAMPFIRE_AWARD_RANK)
	{
		player_info->rank_rev_stat = 2;

		MSG_50200705 msg_res;
		if (msg.result == 0)
			msg_res.result = 1;
		else
			msg_res.result = 0;

		save_campfire_player_info();

		Block_Buffer buffer;
		buffer.make_message(msg_res.msg_id);
		msg_res.serialize(buffer);
		buffer.finish_message();
		player_self()->send_to_client(buffer);
	}

	send_campfire_spot_info();

	return 0;
}

int Scene_Campfire::load_campfire_player_info(const MSG_20300116& msg) {
	Scene_Campfire_Info info;
	info.role_id = role_id();
	info.role_name = player_self()->role_name();
	info.finish_time.set(msg.finish_time, 0);
	info.rank = msg.rank;
	Scene_Campfire_Info* src_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(info.role_id);
	if (src_info) {
		boost::unordered_set<int32_t>::const_iterator iter;
		for (iter = src_info->killed_match_man.begin();
				iter != src_info->killed_match_man.end(); ++iter) {
			info.killed_match_man.insert(*iter);
		}
		info.add_firewood = src_info->add_firewood;
	}
	info.is_lighter = msg.is_lighter;
	info.recieve_stat = msg.recieve_stat;
	info.rank_rev_stat = msg.rank_rev_stat;
	info.gang_id = player_self()->base_detail().gang_id;
	info.gang_name = player_self()->base_detail().gang_name;
	info.headship = player_self()->ganger_detail().headship;
	CAMPFIRE_DATA_MANAGER->add_player(info);

	send_campfire_spot_info();

	return 0;
}

void Scene_Campfire::req_campfire_player_info() {
	MSG_20300115 msg;
	msg.role_id = player_self()->role_id();
	Block_Buffer buff;
	buff.make_message(msg.msg_id);
	msg.serialize(buff);
	buff.finish_message();
	player_self()->send_to_logic(buff);
}

void Scene_Campfire::clear_campfire_award_record() {
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player_self()->role_id());
	if (player_info == 0)
		return ;

	Campfire_Award_Item_Vec awards;
	if (player_info->recieve_stat == 1) {
		int type;
		if (player_info->is_lighter)
			type = Campfire_Award_Config::CAMPFIRE_AWARD_LIT;
		else
			type = Campfire_Award_Config::CAMPFIRE_AWARD_ADD_WOOD;

		Campfire_Award_Item_Vec award1 = CONFIG_CACHE_SECRET_SHOP->campfire_award_config()->award_configs.find(type)->second;
		Campfire_Award_Item_Vec::const_iterator iter;
		for (iter = award1.begin(); iter != award1.end(); ++iter) {
			awards.push_back(*iter);
		}
	}
	if (player_info->rank_rev_stat == 1) {
		Campfire_Award_Item_Vec award2;
		if (player_info->rank > 0) {
			Rank_Award_Config_Vec vec = CONFIG_CACHE_SECRET_SHOP->campfire_award_config()->rank_award_configs;
			Rank_Award_Config_Vec::const_iterator award_iter;
			for (award_iter = vec.begin(); award_iter != vec.end(); ++award_iter) {
				if (player_info->rank >= (*award_iter).rank_start && player_info->rank <= (*award_iter).rank_stop) {
					award2 = (*award_iter).awards;
					break;
				}
			}
		}

		//安慰奖
		if (awards.empty()) {
			award2 = CONFIG_CACHE_SECRET_SHOP->campfire_award_config()->award_configs.find(Campfire_Award_Config::CAMPFIRE_AWARD_CONSOLATION)->second;
		}
		Campfire_Award_Item_Vec::const_iterator iter;
		for (iter = award2.begin(); iter != award2.end(); ++iter) {
			awards.push_back(*iter);
		}
	}

	std::vector<Item_Basic_Info> vec;
	Item_Basic_Info one_info;
	Campfire_Award_Item_Vec::const_iterator iter;
	for (iter = awards.begin(); iter != awards.end(); ++iter) {
		one_info.id = (*iter).item_id;
		one_info.amount = (*iter).item_count;
		one_info.bind = (*iter).bind_type;
		vec.push_back(one_info);
	}

	if (!awards.empty()) {
		MSG_20300117 msg;
		msg.mail_id = AWARD_MAIL_ID;
		msg.items = vec;

		Block_Buffer buff;
		buff.make_message(msg.msg_id);
		msg.serialize(buff);
		buff.finish_message();
		player_self()->send_to_logic(buff);
	}

	player_info->add_firewood = 0;
	player_info->finish_time.set(0, 0);
	player_info->rank = 0;
	player_info->recieve_stat = 0;
	player_info->rank_rev_stat = 0;
	player_info->is_lighter = 0;
	player_info->killed_match_man.clear();

	player_self()->save_campfire_player_info();
	player_self()->send_campfire_spot_info();
}
