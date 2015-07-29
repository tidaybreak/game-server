/*
 * ThorLottery_Config.cpp
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#include "ThorLottery_Config.h"

void ThorLottery_Item_Config::reset(void) {

	id					= 0;
	weight				= 0;
	num					= 0;
	bind_state			= 0;
	is_broad			= 0;
	exchange_cost		= 0;
}

void ThorLottery_Order_Config::reset(void) {
	order				= 0;
	weight				= 0;
	min_lottery			= 0;
	max_lottery			= 0;
	server_limit		= 0;
	thorlottery_item_config_map.clear();
}

void ThorLottery_Config::reset(void) {

	season_id		= 0;
	start_weeks		= 0;
	end_weeks		= 0;
	thorlottery_order_config_map.clear();
}
