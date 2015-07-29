/*
 * ThorLottery_Config.h
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#ifndef THORLOTTERY_CONFIG_H_
#define THORLOTTERY_CONFIG_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"

struct ThorLottery_Item_Config {
	ThorLottery_Item_Config() { reset(); }
	void reset(void);

	int order;
	int id;
	int weight;
	int num;
	int bind_state;
	int is_broad;
	int exchange_cost;
};
typedef boost::unordered_map<int, ThorLottery_Item_Config> ThorLottery_Item_Config_Map;

struct ThorLottery_Order_Config {
	ThorLottery_Order_Config() { reset(); }
	void reset(void);

	int order;
	int weight;
	int min_lottery;
	int max_lottery;
	int server_limit;
	ThorLottery_Item_Config_Map thorlottery_item_config_map;
};
typedef boost::unordered_map<int, ThorLottery_Order_Config> ThorLottery_Order_Config_Map;

struct ThorLottery_Config {
	ThorLottery_Config(void) { reset(); }
	void reset(void);

	int season_id;
	int start_weeks;
	int end_weeks;
	ThorLottery_Order_Config_Map thorlottery_order_config_map;
};
typedef boost::unordered_map<int, ThorLottery_Config> ThorLottery_Map;

#endif /* THORLOTTERY_CONFIG_H_ */
