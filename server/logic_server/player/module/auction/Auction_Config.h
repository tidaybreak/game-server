/*
 * Auction_Config.h
 *
 *  Created on: 2014-09-12
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef AUCTION_CONFIG_H_
#define AUCTION_CONFIG_H_

#include "Game_Typedef.h"
#include "Time_Value.h"

struct Auction_Goods_Config {
	Auction_Goods_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		item_id = 0;
		num = 0;
		start_time = Time_Value::zero;
		continue_time = 0;
		auction_price = 0;
		ones_price = 0;
	}

	uint32_t id;
	uint32_t item_id;//物品ID
	uint32_t num;//物品数量
	Time_Value start_time;//开始时间
	uint32_t continue_time;//延续时长
	uint32_t auction_price;//竞标价
	uint32_t ones_price;//一口价
};

typedef boost::unordered_map<uint32_t, Auction_Goods_Config> Auction_Goods_Config_Map;

struct Auction_Costs_Config {
	Auction_Costs_Config(void) { reset(); }
	void reset(void) {
		continue_time = 0;
		keep_costs = 0;
		ann_costs = 0;
		ann_id = 0;
	}

	uint32_t continue_time;//保管时长(小时)
	uint32_t keep_costs;//保管费用
	uint32_t ann_costs;//广播费用
	uint32_t ann_id;//广播ID
};
typedef boost::unordered_map<uint32_t, Auction_Costs_Config> Auction_Costs_Config_Map;
#endif /* AUCTION_CONFIG_H_ */
