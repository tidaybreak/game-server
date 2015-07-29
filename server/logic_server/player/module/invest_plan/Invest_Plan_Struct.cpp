/*
 * Invest_Plan_Struct.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */


#include "Invest_Plan_Struct.h"
#include "DB_Operator.h"
#include "Invest_Plan_Def.h"

Invest_Plan_Detail::Invest_Plan_Detail(void) {
	reset();
}

Invest_Plan_Detail::~Invest_Plan_Detail(void) {

}

void Invest_Plan_Detail::reset(void) {
	role_id = 0;
	is_month_card_invest = false;
	left_day = 0;
	month_card_profit_button_map.clear();
	days = 0;

	invested_money = 0;
	invest_button = UPGRADE_INVEST_BUTTON_STATUS_FIRST_INVEST;
	upgrade_invest_profit_button_map.clear();
	can_invest_money_set.clear();
	invested_money_list.clear();
	getted_upgrade_invest_profit_map.clear();
}

int Invest_Plan_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Invest_Plan_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Invest_Plan_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_invest_plan_detail(*this);
	return 0;
}

int Invest_Plan_Detail::load(void) {
	return MONGO_OPERATOR->load_invest_plan_detail(*this);
}
