/*
 * Mall.cpp
 *
 *  Created on: 2012-6-15
 *      Author: ChenTeng
 */

#include "Mall_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "public/Public_Def.h"
#include "DB_Def.h"
#include "Config_Cache.h"

Mall_Manager::Mall_Manager() {
	// TODO Auto-generated constructor stub
	save_db = false;
	cycle_day_sum = 0;
}

Mall_Manager::~Mall_Manager() {
	// TODO Auto-generated destructor stub
}

int Mall_Manager::start(void) {
	MONGO_OPERATOR->load_item_sales_num(item_sales_num, tabs_item_sales_num);
	MONGO_OPERATOR->load_goods_list(goods_list_);//初始化商城物品
	MONGO_OPERATOR->load_promo_goods_list(promo_goods_list_);//初始化商城物品
	MONGO_OPERATOR->load_banner_goods_list(banner_goods_list_);//初始化商城物品
	cycle_day_sum = 0;
	refresh_promo_banner_goods(promo_goods_list_, promo_goods_);
	refresh_promo_banner_goods(banner_goods_list_, banner_goods_);
	tick_time_ = Time_Value::gettimeofday() + Time_Value(10);
	save_db = false;
	return 0;
}

Mall_Promo_Info &Mall_Manager::get_promo_goods() {
	return promo_goods_;
}

Mall_Promo_Info &Mall_Manager::get_banner_goods() {
	return banner_goods_;
}

void Mall_Manager::trigger_daily_zero(const Time_Value &now) {
	bool send = false;
	int32_t old_time = promo_goods_.start_time;
	int32_t bold_stime = banner_goods_.start_time;
	refresh_promo_banner_goods(promo_goods_list_, promo_goods_);
	refresh_promo_banner_goods(banner_goods_list_, banner_goods_);
	if(old_time != promo_goods_.start_time) {
		MSG_DEBUG("MALL promo goods time out on zero clean sales_num now:%ld oldtime:%d newtime:%d",now.sec(), old_time, promo_goods_.start_time);
		item_sales_num.clear();
		save_db = true;
		send = true;
	}
	if(bold_stime != banner_goods_.start_time) {
		send = true;
	}
	if(send) {
		send_inner_goods_list_to_client();
	}
}

void Mall_Manager::trigger_daily_six(const Time_Value &now) {
	//item_sales_num.clear();
	//save_db = true;
	tabs_item_sales_num.clear();
	send_inner_goods_list_to_client();
}

void Mall_Manager::send_inner_goods_list_to_client(void) {
	MSG_82151000 msg;
	msg.promo_goods = promo_goods_;
	msg.banner_goods = banner_goods_;
	msg.diff_list = goods_list_;
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if(it->second->get_power_by_power_type(Power_Type(pwoer_mall)) == 1){
			it->second->fite_promo_goods_num(msg.promo_goods.goods_list);//更新商品限购个数
			it->second->fite_tabs_goods_num(msg.diff_list);
			OBJ_SEND_TO_CLIENT(msg, (*it->second));
		}
	}
}

void Mall_Manager::tick(Time_Value &now) {
	if (now > tick_time_) {
		MSG_82151000 msg;
		bool send = false;
		if(promo_goods_.end_time < now.sec()) {
			int32_t old_time = promo_goods_.start_time;
			promo_goods_.reset();
			if(!promo_goods_list_.empty()) {
				refresh_promo_banner_goods(promo_goods_list_, promo_goods_);
				send = true;
			}

			if(old_time != promo_goods_.start_time) {
				MSG_DEBUG("MALL promo goods time out on tick clean sales_num now:%ld oldtime:%d newtime:%d",now.sec(), old_time, promo_goods_.start_time);
				item_sales_num.clear();
				save_db = true;
			}
		}
		if(banner_goods_.end_time < now.sec()) {
			banner_goods_.reset();
			if(!banner_goods_list_.empty()) {
				refresh_promo_banner_goods(banner_goods_list_, banner_goods_);
				send = true;
			}
		}
		if(send) {
			send_inner_goods_list_to_client();
//			const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
//			msg.promo_goods = promo_goods_;
//			msg.banner_goods = banner_goods_;
//			for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
//				if(it->second->get_power_by_power_type(Power_Type(pwoer_mall)) == 1){
//					it->second->fite_promo_goods_num(msg.promo_goods.goods_list);//更新商品限购个数
//					OBJ_SEND_TO_CLIENT(msg, (*it->second));
//				}
//			}
		}
		tick_time_ += Time_Value(10);
	}
	if(save_db) {
		save_db = false;
		Block_Buffer buf;
		buf.make_message(MONGO_SAVE_MALL_BUY_NUM);
		buf.write_uint16(item_sales_num.size());
		for(Item_Role_Set_Map::iterator iter = item_sales_num.begin(); iter != item_sales_num.end(); ++iter) {
			buf.write_int32(iter->first);
			buf.write_uint16(iter->second.size());
			for(Role_Id_MSet::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
				buf.write_int64(*it);
			}
		}
		buf.write_uint16(tabs_item_sales_num.size());
		for(Tabs_Item_Role_Set_Map::iterator it_t = tabs_item_sales_num.begin(); it_t != tabs_item_sales_num.end(); ++it_t) {
			buf.write_int32(it_t->first);
			buf.write_uint16(it_t->second.size());
			for(Item_Role_Set_Map::iterator iter = it_t->second.begin(); iter != it_t->second.end(); ++iter) {
				buf.write_int32(iter->first);
				buf.write_uint16(iter->second.size());
				for(Role_Id_MSet::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
					buf.write_int64(*it);
				}
			}
		}
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
	}
}

Goods_Info_Vec &Mall_Manager::get_goods_info(){
	return goods_list_;
}

int32_t Mall_Manager::find_goods_price_by_tabs_and_id(int32_t tabs, int32_t id) {
	for(Goods_Info_Vec::iterator it = goods_list_.begin(); it != goods_list_.end(); ++it) {
		if(it->tabs == tabs && it->id == id) {
			return it->price;
		}
	}
	return 0;
}

void Mall_Manager::update_tabs_item_sales_num(int32_t tabs, int32_t item_id,role_id_t role_id) {
	tabs_item_sales_num[tabs][item_id].insert(role_id);
}

void Mall_Manager::update_item_sales_num(int32_t item_id,role_id_t role_id) {
	Item_Role_Set_Map::iterator iter = item_sales_num.find(item_id);
	if(iter != item_sales_num.end()) {
		iter->second.insert(role_id);
	} else {
		Role_Id_MSet role_set;
		role_set.clear();
		role_set.insert(role_id);
		item_sales_num.insert(std::make_pair(item_id, role_set));
	}
	save_db = true;
}

uint32_t Mall_Manager::get_num_by_item(int32_t item_id) {
	Item_Role_Set_Map::iterator iter = item_sales_num.find(item_id);
	if(iter != item_sales_num.end()) {
		return iter->second.size();
	}
	return 0;
}

uint32_t Mall_Manager::get_num_by_role_id(int32_t item_id, role_id_t role_id) {
	Item_Role_Set_Map::iterator iter = item_sales_num.find(item_id);
	if(iter != item_sales_num.end()) {
		return iter->second.count(role_id);
	}
	return 0;
}

uint32_t Mall_Manager::get_tabs_num_by_item(int32_t tabs, int32_t item_id) {
	Tabs_Item_Role_Set_Map::iterator tabs_iter = tabs_item_sales_num.find(tabs);
	if(tabs_iter != tabs_item_sales_num.end()) {
		Item_Role_Set_Map::iterator iter = tabs_iter->second.find(item_id);
		if(iter != tabs_iter->second.end()) {
			return iter->second.size();
		}
	}
	return 0;
}

uint32_t Mall_Manager::get_tabs_num_by_role_id(int32_t tabs, int32_t item_id, role_id_t role_id) {
	Tabs_Item_Role_Set_Map::iterator tabs_iter = tabs_item_sales_num.find(tabs);
	if(tabs_iter != tabs_item_sales_num.end()) {
		Item_Role_Set_Map::iterator iter = tabs_iter->second.find(item_id);
		if(iter != tabs_iter->second.end()) {
			return iter->second.count(role_id);
		}
	}
	return 0;
}

int Mall_Manager::goods_changed(Goods_Info_Vec &goods_list, Int_Vec &diff_tabs, Promo_Goods_Info_Vec &promo_goods_list, Promo_Goods_Info_Vec &banner_goods_list){
	MSG_DEBUG("PHP SEND GOODS_CHANGED");
	Time_Value time = Time_Value::gettimeofday();
	MSG_82151000 msg;
	goods_list_=goods_list;
	promo_goods_list_=promo_goods_list;
	//记录改变前的开始时间
	int32_t old_stime = promo_goods_.start_time;
	promo_goods_.reset();
	refresh_promo_banner_goods(promo_goods_list_, promo_goods_);
	if(old_stime != promo_goods_.start_time) {//开始时间改变了;就需要重设购买限制
		MSG_DEBUG("MALL promo goods time out on php send goods_changed now:%ld oldtime:%d newtime:%d",time.sec(), old_stime, promo_goods_.start_time);
		item_sales_num.clear();
		save_db = true;
	}
	msg.promo_goods = promo_goods_;
	//banner
	banner_goods_list_ = banner_goods_list;
	refresh_promo_banner_goods(banner_goods_list_, banner_goods_);
	msg.banner_goods = banner_goods_;
	//goods
	msg.diff_list = goods_list;
//	for ( Int_Vec::iterator tit = diff_tabs.begin(); tit != diff_tabs.end(); tit++) {
//		for ( Goods_Info_Vec::iterator it = goods_list.begin(); it != goods_list.end(); it++ ) {
//			if ((*tit) == (*it).tabs){
//				msg.diff_list.push_back((*it));
//			}
//		}
//	}
	msg.diff_tabs=diff_tabs;
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if(it->second->get_power_by_power_type(Power_Type(pwoer_mall)) == 1){
			it->second->fite_promo_goods_num(msg.promo_goods.goods_list);//更新商品限购个数
			it->second->fite_tabs_goods_num(msg.diff_list);
			OBJ_SEND_TO_CLIENT(msg, (*it->second));
		}
	}
	return 0;
}

bool Mall_Manager::refresh_promo_banner_goods(Promo_Goods_Info_Vec &goods_list, Mall_Promo_Info &goods)  {
	Time_Value time = Time_Value::gettimeofday();
	for ( Promo_Goods_Info_Vec::iterator pit = goods_list.begin(); pit != goods_list.end();++pit) {
		long long int start_time = 0;
		long long int end_time = 0;
		if(pit->start_time && pit->end_time) {
			start_time = get_time_by_int32(pit->start_time);//得到开始时间
			end_time = get_time_by_int32(pit->end_time);//得到结束时间
		}
		if(start_time <= time.sec() && end_time > time.sec()){
			goods = *pit;
			goods.start_time = start_time;
			goods.end_time = end_time;
			return true;
		}
	}
	//--------------------------
	//得到开服时间
	const Server_Maintainer_Cache &server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	//开服时间0点
	long long int start_server_time = get_day_zero_time(server_maintainer.game_open_time);
	uint32_t sum_day = 0;
	for ( Promo_Goods_Info_Vec::iterator pit = goods_list.begin(); pit != goods_list.end();++pit) {
		long long int start_time = 0;
		long long int end_time = 0;
		if(pit->start_time && pit->end_time) {
			start_time = get_time_by_int32(pit->start_time);//得到开始时间
			end_time = get_time_by_int32(pit->end_time);//得到结束时间
		} else if(pit->day) {
			uint32_t slep_day = 0;
			if(cycle_day_sum) {
				slep_day = ((time.sec() - start_server_time)/(cycle_day_sum*Time_Value::ONE_DAY_IN_SECS)) * cycle_day_sum;
			}
			start_time = start_server_time + Time_Value::ONE_DAY_IN_SECS * (slep_day+sum_day);
			end_time = start_time + Time_Value::ONE_DAY_IN_SECS * (pit->day);
		}
		sum_day += pit->day;
		if(start_time <= time.sec() && end_time > time.sec()){
			goods = *pit;
			goods.start_time = start_time;
			goods.end_time = end_time;
			return true;
		}// else if(pit->day == 0 && end_time <= time.sec()){
			//pit = goods_list.erase(pit);
//		} else {
//			pit++;
//		}
	}
	//第二次补充
	cycle_day_sum = sum_day;sum_day = 0;
	for ( Promo_Goods_Info_Vec::iterator pit = goods_list.begin(); pit != goods_list.end(); pit++) {
		if(pit->day) {
			long long int start_time = 0;
			long long int end_time = 0;
			uint32_t slep_day = 0;
			if(cycle_day_sum) {
				slep_day = ((time.sec() - start_server_time)/(cycle_day_sum*Time_Value::ONE_DAY_IN_SECS))*cycle_day_sum;
			}
			start_time = start_server_time + Time_Value::ONE_DAY_IN_SECS * (slep_day+sum_day);
			end_time = start_time + Time_Value::ONE_DAY_IN_SECS * (pit->day);
			sum_day += pit->day;
			if(start_time <= time.sec() && end_time > time.sec()){
				goods = *pit;
				goods.start_time = start_time;
				goods.end_time = end_time;
				return true;
			}
		}
	}
	return false;
}

long long int Mall_Manager::get_day_zero_time(const Time_Value &time) {
	Date_Time date_(time);
	date_.hour(0);
	date_.minute(0);
	date_.second(0);
	return (date_.time_sec());
}

long long int Mall_Manager::get_time_by_int32(const int32_t hm) {
	Date_Time date_(Time_Value::gettimeofday());
	date_.month(hm/100);
	date_.day(hm%100);
	date_.hour(0);
	date_.minute(0);
	date_.second(0);
	return date_.time_sec();
}
