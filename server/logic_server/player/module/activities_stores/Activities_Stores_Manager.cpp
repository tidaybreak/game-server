/*
 * Activities_Stores.cpp
 *
 *  Created on: 2012-6-15
 *      Author: ChenTeng
 */

#include <activities_stores/Activities_Stores_Manager.h>
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "public/Public_Def.h"
#include "DB_Def.h"
#include "Config_Cache.h"

Activities_Stores_Manager::Activities_Stores_Manager() {
	// TODO Auto-generated constructor stub
}

Activities_Stores_Manager::~Activities_Stores_Manager() {
	// TODO Auto-generated destructor stub
}

int Activities_Stores_Manager::start(void) {
	MONGO_OPERATOR->load_activities_stores_goods(goods_map);
	return 0;
}


void Activities_Stores_Manager::trigger_daily_zero(const Time_Value &now) {

}

void Activities_Stores_Manager::trigger_daily_six(const Time_Value &now) {

}

void Activities_Stores_Manager::open(const Time_Value start, const Time_Value end) {
	start_time = start;
	end_time = end;
}

void Activities_Stores_Manager::end(const Time_Value end) {
	end_time = Time_Value::gettimeofday();
}

bool Activities_Stores_Manager::is_open(void) {
	Time_Value now = Time_Value::gettimeofday();
	return (now.sec() >= start_time.sec()) && (now.sec() <= end_time.sec());
}

Activity_Exchange_Mall_Goods_Info_Map &Activities_Stores_Manager::get_goods_map(void) {
	return goods_map;
}

Activity_Exchange_Mall_Goods_Info *Activities_Stores_Manager::get_goods_by_id(uint32_t id) {
	Activity_Exchange_Mall_Goods_Info_Map::iterator it = goods_map.find(id);
	if(it != goods_map.end()) {
		return &it->second;
	}
	return 0;
}

void Activities_Stores_Manager::php_send_activity_exchange_mall_goods_change(Activity_Exchange_Mall_Goods_Info_Vec &goods) {
	MSG_DEBUG('php send activity exchange mall goods chage, size:%d', goods.size());
	copy_vec_to_map(goods);
}

void Activities_Stores_Manager::copy_vec_to_map(Activity_Exchange_Mall_Goods_Info_Vec &goods) {
	//goods_map.clear();
	UInt_Vec del_;
	int act_id = get_goods_act_id(goods);
	if(act_id == 0) return;
//	for(Activity_Exchange_Mall_Goods_Info_Map::iterator itera = goods_map.begin(); itera != goods_map.end();++itera) {
//		if(itera->second.act_id == act_id) {
//			del_.push_back(itera->second.id);
//		}
//	}
//	for(UInt_Vec::iterator it = del_.begin(); it != del_.end(); ++it) {
//		Activity_Exchange_Mall_Goods_Info_Map::iterator del_it = goods_map.find(*it);
//		if(del_it != goods_map.end()) {
//			goods_map.erase(del_it);
//		}
//	}
	for(Activity_Exchange_Mall_Goods_Info_Map::iterator itera = goods_map.begin(); itera != goods_map.end(); )
	{
		if(itera->second.act_id == (uint32_t)act_id)
		{
			itera = goods_map.erase(itera);
		}else{
			++itera;
		}
	}
	for(Activity_Exchange_Mall_Goods_Info_Vec::iterator it = goods.begin(); it != goods.end(); ++it) {
		Activity_Exchange_Mall_Goods_Info_Map::iterator itera = goods_map.find(it->id);
		if(itera == goods_map.end()) {
			goods_map.insert(std::make_pair(it->id, *it));
		} else {
			itera->second = *it;
		}
	}
}

int Activities_Stores_Manager::get_goods_act_id(Activity_Exchange_Mall_Goods_Info_Vec &goods)
{
	Activity_Exchange_Mall_Goods_Info_Vec::iterator it = goods.begin();
	if(it != goods.end()) {
		return it->act_id;
	}
	return 0;
}
