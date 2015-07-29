/*
 * Auction_Struct.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: xiaoliang
 */

#include "Auction_Struct.h"
#include "DB_Operator.h"
#include "Auction_Config.h"
#include "Logic_Monitor.h"
#include "Msg_Struct.h"

Auction_Goods_Data::Auction_Goods_Data(void) {
	reset();
}
Auction_Goods_Data::Auction_Goods_Data(uint32_t id,uint32_t item_id, uint32_t num,
		uint32_t continue_time, uint32_t auction_price, uint32_t ones_price, role_id_t issue_role_id):
			id(id), item_id(item_id), num(num), auction_price(auction_price), ones_price(ones_price), continue_time(continue_time),
			  issue_role_id(issue_role_id) {
	start_time = Time_Value::gettimeofday();
	over_time.sec(start_time.sec() + continue_time * Time_Value::ONE_HOUR_IN_SECS);
	auction_role_id = 0;
	l_auction_price = 0;
	auction_fail_role_id.clear();
}

Auction_Goods_Data::Auction_Goods_Data(const Auction_Goods_Config &conf) {
	id = conf.id;
	item_id = conf.item_id;
	num = conf.num;
	auction_price = conf.auction_price;
	ones_price = conf.ones_price;
	start_time = conf.start_time;
	continue_time = conf.continue_time;
	over_time.sec(start_time.sec() + continue_time + Time_Value::ONE_HOUR_IN_SECS);
	issue_role_id = 0;
	auction_role_id = 0;//最后一次竞拍人
	l_auction_price = 0;//最后一次竞拍价
	auction_fail_role_id.clear();
}

void Auction_Goods_Data::update_of_config(const Auction_Goods_Config &conf) {
	id = conf.id;
	item_id = conf.item_id;
	num = conf.num;
	auction_price = conf.auction_price;
	ones_price = conf.ones_price;
	start_time = conf.start_time;
	continue_time = conf.continue_time;
	over_time.sec(start_time.sec() + continue_time + Time_Value::ONE_HOUR_IN_SECS);
	issue_role_id = 0;
}

void Auction_Goods_Data::make_auction_goods_info(Auction_Goods_Info &agi) const {
	agi.reset();
	agi.id = id;
	agi.item_id = item_id;
	agi.num = num;
	agi.auction_price = auction_price;
	agi.ones_price = ones_price;
	agi.lave_time = over_time.sec() - Time_Value::gettimeofday().sec();
	agi.start_time = start_time.sec();
	if(issue_role_id) {
		agi.auction_role_id = issue_role_id;
		agi.auction_role_name = LOGIC_MONITOR->get_role_name_by_id(issue_role_id);
	}
	if(auction_role_id) {
		agi.last_auction_role_id = auction_role_id;
		agi.last_auction_role_name = LOGIC_MONITOR->get_role_name_by_id(auction_role_id);
	}
	agi.last_auction_price = l_auction_price;
	for(Role_Id_Set::iterator it = auction_fail_role_id.begin(); it != auction_fail_role_id.end(); ++it) {
		agi.auction_fail_role_id.push_back(*it);
	}

}

void Auction_Goods_Data::reset(void) {
	id = 0;//id
	item_id = 0;//道具ID
	num = 0;//数量
	auction_price = 0;//默认竞拍价
	ones_price = 0;//默认一口价
	over_time = Time_Value::zero;//过期时间
	start_time = Time_Value::zero;//开始时间
	continue_time = 0;//延续时长(小时)
	issue_role_id = 0;//拍卖商品发布人
	auction_role_id = 0;//最后一次竞拍人
	l_auction_price = 0;//最后一次竞拍价
	auction_fail_role_id.clear();
}

int Auction_Goods_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_uint32(id);
	buffer.write_uint32(item_id);
	buffer.write_uint32(num);
	buffer.write_uint32(auction_price);
	buffer.write_uint32(ones_price);
	buffer.write_int64(over_time.sec());
	buffer.write_int64(start_time.sec());
	buffer.write_uint32(continue_time);
	buffer.write_int64(issue_role_id);
	buffer.write_int64(auction_role_id);
	buffer.write_uint32(l_auction_price);
	buffer.write_uint8(auction_fail_role_id.size());
	for(Role_Id_Set::const_iterator it = auction_fail_role_id.begin(); it != auction_fail_role_id.end(); ++it) {
		buffer.write_int64(*it);
	}
	return 0;
}

int Auction_Goods_Data::deserialize(Block_Buffer &buffer) {
	buffer.read_uint32(id);
	buffer.read_uint32(item_id);
	buffer.read_uint32(num);
	buffer.read_uint32(auction_price);
	buffer.read_uint32(ones_price);

	int64_t time = 0;
	buffer.read_int64(time);
	over_time.sec(time);

	time = 0;
	buffer.read_int64(time);
	start_time.sec(time);

	buffer.read_uint32(continue_time);
	buffer.read_int64(issue_role_id);
	buffer.read_int64(auction_role_id);
	buffer.read_uint32(l_auction_price);

	auction_fail_role_id.clear();
	uint8_t size = 0;
	buffer.read_uint8(size);
	for(;size > 0; --size) {
		int64_t role_id = 0;
		buffer.read_int64(role_id);
		auction_fail_role_id.insert(role_id);
	}
	return 0;
}

Auction_Detail::Auction_Detail(void) {
	reset();
}

Auction_Detail::~Auction_Detail(void) {

}

void Auction_Detail::reset(void) {

}

int Auction_Detail::serialize(Block_Buffer &buffer) const {
	return 0;
}

int Auction_Detail::deserialize(Block_Buffer &buffer) {
	return 0;
}

int Auction_Detail::save(void) {
	return 0;
}

int Auction_Detail::load(void) {
	return 0;
}
