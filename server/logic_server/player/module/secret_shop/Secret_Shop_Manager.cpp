/*
 * Secret_Shop_Manager.cpp
 *
 *  Created on: 2014年7月30日
 *      Author: root
 */

#include "Secret_Shop_Manager.h"
#include "Msg_Other_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Secret_Shop.h"
#include "Logic_Player.h"
#include "Utility_Func.h"
#include "Config_Cache.h"
#include "Logic_Monitor.h"
#include "announcement/Announcement_Struct.h"
#include "Err_Code.h"
#include "Config_Cache_Scene.h"
#include "helper/Helper_Def.h"
#include "Msg_Active_Struct.h"


Secret_Shop_Manager::Secret_Shop_Manager(void):ann_space_(0), ann_index_(0), shop_open_(false),
	scene_id_(0), chapman_(0), month_hero_cfg_id_(0), month_hero_cfg_next_id_(0),
	month_hero_remain_day_(0){

}

Secret_Shop_Manager::~Secret_Shop_Manager(void){

}

int Secret_Shop_Manager::start(void){
	init_main_scene();
	Time_Value now = Time_Value::gettimeofday();
	activate_shop(now);
	//每月英雄
	cal_month_hero_info(now, month_hero_cfg_id_, month_hero_cfg_next_id_, month_hero_remain_day_);
	return 0;
}

int Secret_Shop_Manager::tick(const Time_Value &now){
	if(shop_open_){
		if(now >= close_time_){
			close_shop(now);
			activate_shop(now);
			return 0;
		}
		if(now > ann_tick_){
			announce_opening_shop(now);
		}
	}else{
		if(now >= activate_time_){
			open_shop(now);
			return 0;
		}
	}
	return 0;
}

void Secret_Shop_Manager::init_main_scene(void){
	Monitor_Mark_Vec monitor_id_order;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(monitor_id_order, Server_Enum::MONITOR_SCENE);
	for (Monitor_Mark_Vec::iterator it = monitor_id_order.begin(); it != monitor_id_order.end(); ++it) {
		if(it->id/10000 == 8010){
			main_scene_map_.insert(std::make_pair(it->id, (*it)));
		}
	}
}

void Secret_Shop_Manager::register_main_scene(const Monitor_Unique_ID& muid){

}

void Secret_Shop_Manager::unregister_main_scene(const Monitor_Unique_ID& muid){

}

int Secret_Shop_Manager::req_buy_shop_item(Logic_Player* player, int index){
	if(!player){
		return -1;
	}
	int item_count = this->shop_item_.size();
	if(item_count <= 0 || index < 0 || index >= item_count){
		return -1;
	}
	// 购买数量检测
	if(shop_item_[index].config->num != 0 &&
			shop_item_[index].buy_num >= shop_item_[index].config->num){
		//
		MSG_50120011 msg;
		msg.item_id = -1;
		msg.shop_type = 0;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		//
		req_fetch_shop_info(player);
		return ERROR_SECRET_SHOP_NUM_LIMIT;
	}
	// 限购检测
	if(shop_item_[index].config->limit_num != 0 &&
			shop_item_[index].player_buy_num(player->role_id()) >= shop_item_[index].config->limit_num){
		return ERROR_SECRET_SHOP_LIMIT_NUM_LIMIT;
	}
	int item_id = shop_item_[index].config->item;
	int shop_type = shop_item_[index].config->type;
	// 金钱检测
	int money_type = shop_item_[index].config->money_type;
	int money_cost = shop_item_[index].config->cost;
	int bind_type = shop_item_[index].config->binding;
	Money_Sub_Type m_s_type = MONEY_SUB_TYPE_END;
	switch(money_type){
		case COPPER:{
			m_s_type = COPPER_ONLY;
			break;
		}
		case GOLD:{
			m_s_type = GOLD_ONLY;
			break;
		}
		case BIND_GOLD:{
			m_s_type = BIND_GOLD_FIRST;
			break;
		}
		default:{
			break;
		}
	}
	int buy_result = 0;
	Money_Sub_Info m_s_info(m_s_type, money_cost, MONEY_SUB_STORE_USE);
	if(money_cost > 0){
		buy_result = player->pack_try_sub_money(m_s_info);
	}
	if(buy_result != 0){
		return buy_result;
	}
	// 背包检测
	Item_Detail item = Item_Detail(item_id, 1, (Item_Detail::Bind_Type)bind_type);
	buy_result = player->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX, item);
	if( buy_result != 0){
		return buy_result;
	}
	// 添加到背包
	player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,Gain_Item_DM_Info(Pack::ITEM_GAIN_STORE));
	// 扣钱
	if(money_cost > 0){
		player->pack_sub_money(m_s_info);
	}
	//
	shop_item_[index].buy_item(player->role_id());

	{
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		Item_Basic_Info ibi;
		item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		OBJ_SEND_TO_CLIENT(acv_msg, (*player));
	}
	//
	MSG_50120011 msg;
	msg.item_id = item_id;
	msg.shop_type = shop_type;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	//
	req_fetch_shop_info(player);
	// TOD助手日常和活跃度侦听
	player->daily_helper_listen(DAILY_FULFILL_LISTEN_SECRET_SHOP);
	player->achievement_listen_secret_shop_buy_nums(1);
	return 0;
}

int Secret_Shop_Manager::req_fetch_shop_info(Logic_Player* player){

	MSG_50120010 msg;

	Time_Value now = Time_Value::gettimeofday();
	int remain_time = this->close_time_.sec() - now.sec();
	if(remain_time < 0){
		remain_time = 0;
	}
	msg.remain_time = remain_time;

	int count = shop_item_.size();
	for(int i = 0; i < count; ++i){
		Secret_Item_Info info;
		info.shop_type = shop_item_[i].config->type;
		info.item_id = shop_item_[i].config->item;
		info.remain_num = shop_item_[i].config->num - shop_item_[i].buy_num;
		info.buy_num = shop_item_[i].player_buy_num(player->role_id());
		msg.items.push_back(info);
	}

	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

void Secret_Shop_Manager::trigger_daily_six(const Time_Value &now){
	int cur_id = 0;
	int next_id = 0;
	int remain_day = 0;
	//每月英雄
	cal_month_hero_info(now, cur_id, next_id, remain_day);
	bool notice = false;
	if(month_hero_cfg_id_ != cur_id || month_hero_cfg_next_id_ != next_id){
		notice = true;
	}
	month_hero_cfg_id_ = cur_id;
	month_hero_cfg_next_id_ = next_id;
	month_hero_remain_day_ = remain_day;
	if(month_hero_cfg_id_  != 0 && notice){
		MSG_50120070 msg;
		msg.cfg_index= month_hero_cfg_id_;
		msg.remain_day = month_hero_remain_day_;
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		LOGIC_MONITOR->send_to_all_client(buf);
	}
}

void Secret_Shop_Manager::get_month_hero_info(int& cfg_id, int& remain_day){
	cfg_id = this->month_hero_cfg_id_;
	remain_day = this->month_hero_remain_day_;
}

int Secret_Shop_Manager::open_shop(const Time_Value &now){
	if(shop_open_){
		return -1;
	}
	shop_open_ = true;
	open_time_ = now;
	int duration_time = 600;
	const Secret_Base_Config* base_cfg = CONFIG_CACHE_SECRET_SHOP->base_config();
	if(base_cfg){
		duration_time = Utility_Func::get_rand_min_max(base_cfg->duration_time.val_1,
				base_cfg->duration_time.val_2);
	}
	close_time_ = now + Time_Value(duration_time);
	//to scene
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_SECRET_SHOP);
	Main_Scene_Map::iterator it = main_scene_map_.find(scene_id_);
	if(it != main_scene_map_.end()){
		it->second.serialize(buf);
	}
	ref_point_.serialize(buf);
	buf.write_int32(chapman_);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(scene_id_, 0, buf);
	announce_open_shop();
	ann_tick_ = now + Time_Value(ann_space_);
	ann_index_ = 0;
	return 0;
}

int Secret_Shop_Manager::close_shop(const Time_Value &now){
	// to scene
	if(shop_open_ == false){
		return -1;
	}
	shop_open_ = false;
	shop_item_.clear();
	//to scene
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_SECRET_SHOP);
	Main_Scene_Map::iterator it = main_scene_map_.find(scene_id_);
	if(it != main_scene_map_.end()){
		it->second.serialize(buf);
	}
	ref_point_.serialize(buf);
	buf.write_int32(0);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(scene_id_, 0, buf);

	announce_close_shop();
	return 0;
}

int Secret_Shop_Manager::activate_shop(const Time_Value &now){
	if(shop_open_){
		return -1;
	}
	const Secret_Base_Config* base_cfg = CONFIG_CACHE_SECRET_SHOP->base_config();
	if(base_cfg){
		int refresh_time = Utility_Func::get_rand_min_max(base_cfg->refresh_time.val_1,
				base_cfg->refresh_time.val_2);
		activate_time_ = now + Time_Value(refresh_time);
		tm tm1;
		time_t t1 = activate_time_.sec();
		localtime_r(&t1, &tm1);
		int cur_time_value = tm1.tm_hour*100 + tm1.tm_min;
		int valid_time_value = 0;
		if(CONFIG_CACHE_SECRET_SHOP->is_valid_activate_time(cur_time_value, valid_time_value) == false){
			Time_Value valid_time = activate_time_;
			if(cur_time_value > valid_time_value){// 第二天
				valid_time += Time_Value(Time_Value::ONE_DAY_IN_SECS);
			}
			Date_Time date_now(valid_time);
			Date_Time date_des(date_now);
			date_des.hour(valid_time_value/100);
			date_des.minute(valid_time_value%100);
			date_des.second(0);
			activate_time_ = Time_Value(date_des.time_sec()+refresh_time);
		}

		scene_id_ = 80100001;
		std::vector<int> scene_vec;
		Main_Scene_Map::iterator it;
		for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
			if(CONFIG_CACHE_SECRET_SHOP->is_secret_point(it->first)){
				scene_vec.push_back(it->first);
			}
		}
		int scene_count = scene_vec.size();
		if(scene_count > 0){
			int index = random()%scene_count;
			scene_id_ = scene_vec[index];
		}
		CONFIG_CACHE_SECRET_SHOP->get_secret_point(scene_id_, ref_point_, ann_id_vec_, ann_space_);
		chapman_ = random()%2+1;
		CONFIG_CACHE_SECRET_SHOP->get_secret_shop_item(chapman_, shop_item_);
	}
	return 0;
}

int Secret_Shop_Manager::refresh_shop(void){
	Time_Value now = Time_Value::gettimeofday();
	close_shop(now);
	activate_shop(now);
	open_shop(now);
	return 0;
}

int Secret_Shop_Manager::announce_open_shop(void){
	if(!shop_open_){
		return -1;
	}
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id_);
	if(!scene_cfg){
		return -1;
	}
	std::vector<Chat_Content> ann_contents;
	Chat_Content new_content;
	std::stringstream s_stream1;
	s_stream1<<scene_cfg->scene_name;
//	s_stream1<<"("<<ref_point_.pixel_coord().x<<","<<ref_point_.pixel_coord().y<<")";
	new_content.reset();
	new_content.type = ANN_CTTYPE_TEXT;
	new_content.text = s_stream1.str();
	ann_contents.push_back(new_content);
	MSG_20500000 msg;
	msg.district = ANNOUNCE_DISTRICT_SYSTEM;

	if(chapman_ == 1){// 普通
		msg.announce_id = ANNOUNCE_ID_SECRET_SHOP_1;
	}else{// 珍稀
		msg.announce_id = ANNOUNCE_ID_SECRET_SHOP_2;
	}

	msg.contents = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
	return 0;
}

int Secret_Shop_Manager::announce_opening_shop(const Time_Value &now){
	if(!shop_open_){
		return -1;
	}
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id_);
	if(!scene_cfg){
		return -1;
	}
	if(ann_id_vec_.empty()){
		return -1;
	}
	std::vector<Chat_Content> ann_contents;
//	Chat_Content new_content;
//	std::stringstream s_stream1;
//	s_stream1<<scene_cfg->scene_name;
//	new_content.reset();
//	new_content.type = ANN_CTTYPE_TEXT;
//	new_content.text = s_stream1.str();
//	ann_contents.push_back(new_content);
	MSG_20500000 msg;
	msg.district = ANNOUNCE_DISTRICT_SYSTEM;

	int ann_num = ann_id_vec_.size();
	if(ann_index_ < 0 || ann_index_ >= ann_num){
		ann_index_ = 0;
	}
	msg.announce_id = ann_id_vec_[ann_index_];
	++ann_index_;

	msg.contents = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);

	ann_tick_ = now + Time_Value(ann_space_);
	return 0;
}

int Secret_Shop_Manager::announce_close_shop(void){
	std::vector<Chat_Content> ann_contents;
	MSG_20500000 msg;
	msg.district = ANNOUNCE_DISTRICT_SYSTEM;
	msg.announce_id = 1407;
	msg.contents = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
	return 0;
}

void Secret_Shop_Manager::cal_month_hero_info(const Time_Value& now, int& cfg_id, int& cfg_next_id, int& cfg_remain_day){
	Time_Value game_open_time = CONFIG_CACHE->server_maintainer_cache().game_open_time;
	int game_open_day = (now.sec() - game_open_time.sec())/Time_Value::ONE_DAY_IN_SECS;
	int open_day_r = (now.sec() - game_open_time.sec())%Time_Value::ONE_DAY_IN_SECS;
	if(open_day_r > 0){
		game_open_day += 1;
	}
	if(game_open_day <= 0){
		game_open_day = 1;
	}
	int cur_day = 0;
	int next_day = 0;
	CONFIG_CACHE_SECRET_SHOP->get_month_hero_day(game_open_day, cur_day, next_day);
	int remain_day = next_day - game_open_day;
	if(remain_day < 0){
		remain_day = 0;
	}
	cfg_id = cur_day;
	cfg_next_id = next_day;
	cfg_remain_day = remain_day;
}


