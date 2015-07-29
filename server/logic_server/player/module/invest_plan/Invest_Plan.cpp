/*
 * Invest_Plan.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */


#include "Invest_Plan.h"
#include "Invest_Plan_Struct.h"
#include "Logic_Module_Head.h"
#include "Invest_Plan_Def.h"
#include "Config_Cache_Invest_Plan.h"
#include "Msg_Invest_Plan_Struct.h"
#include "pack/Packer.h"
#include "Logic_Player.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "Invest_Plan_Manager.h"
#include "Base_Detail.h"

Invest_Plan::Invest_Plan(void) {
	reset();
}

Invest_Plan::~Invest_Plan() {
}

void Invest_Plan::reset(void) {
	invest_plan_detail_ = 0;
}

int Invest_Plan::create_init(Invest_Plan_Detail &detail, const Base_Detail &baseDetail) {
	for(uint8_t i = 0; i <= 30; ++i) {
		Month_Card_Profit_Button month_card_profit_button;
		month_card_profit_button.reset();
		month_card_profit_button.day = i;
		month_card_profit_button.status = MONTH_CARD_PROFIT_BUTTON_STATUS_NO;
		detail.month_card_profit_button_map.insert(std::make_pair(i, month_card_profit_button));
	}

	const Upgrade_Invest_Cfg &upgrade_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_upgrade_invest_cfg();
	const Level_Profit_Cfg_Map &level_profit_cfg_map =  CONFIG_CACHE_INVEST_PLAN->get_level_profit_cfg_map();

	if(upgrade_invest_cfg.profit_immediately != 0) {
		Upgrade_Invest_Profit_Button upgrade_invest_profit_button;
		upgrade_invest_profit_button.reset();
		upgrade_invest_profit_button.level = 0;
		upgrade_invest_profit_button.status = UPGRADE_INVEST_PROFIT_BUTTON_NO;
		detail.upgrade_invest_profit_button_map.insert(std::make_pair(0, upgrade_invest_profit_button));
	}

	for(Level_Profit_Cfg_Map::const_iterator it = level_profit_cfg_map.begin(); it != level_profit_cfg_map.end(); ++it) {
		Upgrade_Invest_Profit_Button upgrade_invest_profit_button;
		upgrade_invest_profit_button.reset();
		upgrade_invest_profit_button.level = it->first;
		upgrade_invest_profit_button.status = UPGRADE_INVEST_PROFIT_BUTTON_NO;
		detail.upgrade_invest_profit_button_map.insert(std::make_pair(it->first, upgrade_invest_profit_button));
	}

	detail.can_invest_money_set.insert(upgrade_invest_cfg.min_invest_money);
	detail.can_invest_money_set.insert(upgrade_invest_cfg.max_invest_money);

	if(upgrade_invest_cfg.min_invest_money < 5000 && upgrade_invest_cfg.max_invest_money > 5000) {
		detail.can_invest_money_set.insert(5000);
	}

	if(upgrade_invest_cfg.min_invest_money < 3000 && upgrade_invest_cfg.max_invest_money > 3000) {
		detail.can_invest_money_set.insert(3000);
	}

	if(upgrade_invest_cfg.min_invest_money < 1000 && upgrade_invest_cfg.max_invest_money > 1000) {
		detail.can_invest_money_set.insert(1000);
	}

	return 0;
}

void Invest_Plan::load_detail(Invest_Plan_Detail &detail) {
	invest_plan_detail_ = &detail;
}

void Invest_Plan::save_detail(Invest_Plan_Detail &detail) {
	if (invest_plan_detail().is_change) {
		detail = invest_plan_detail();
		invest_plan_detail_->is_change = false;
	}
}

int Invest_Plan::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Invest_Plan::trigger_daily_six(const Time_Value &now) {
	if(!invest_plan_detail_->is_month_card_invest) {
		return 0;
	}

	invest_plan_detail_->days += 1;

	Month_Card_Profit_Button_Map::iterator find_it =
			invest_plan_detail_->month_card_profit_button_map.find(invest_plan_detail_->days);
	if(find_it == invest_plan_detail_->month_card_profit_button_map.end()) {
		this->save_invest_plan_tick();
		return 0;
	}

	if(MONTH_CARD_PROFIT_BUTTON_STATUS_NO == find_it->second.status) {
		find_it->second.status = MONTH_CARD_PROFIT_BUTTON_STATUS_CAN_GET;
	}

	MSG_82310002 msg;
	msg.day = invest_plan_detail_->days;
	THIS_SEND_TO_CLIENT(msg);

	this->save_invest_plan_tick();

	return 0;
}

int Invest_Plan::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Invest_Plan::move_scene_completed(void) {

	return 0;
}

int Invest_Plan::client_ready_now(void) {
	bool is_open_icon = false;

	//TODO  is_open_icon

	const Month_Card_Invest_Cfg &month_card_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_month_card_invest_cfg();
	uint32_t total_num = 0;
	for(Month_Card_Profit_Button_Map::const_iterator it =  invest_plan_detail_->month_card_profit_button_map.begin();
			it != invest_plan_detail_->month_card_profit_button_map.end();
			++it) {
		if(0 == it->first && month_card_invest_cfg.profit_immediately == 0) {
			continue;
		}

		if(it->second.status == MONTH_CARD_PROFIT_BUTTON_STATUS_CAN_GET) {
			++total_num;
		}
	}

	for(Upgrade_Invest_Profit_Button_Map::const_iterator it =  invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		if(it->second.status == UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET) {
			++total_num;
		}
	}

	//通知客户端所有可领取的按钮数量
	if(is_open_icon)
	{
		MSG_82310004 msg_82310004;
		msg_82310004.total_num = total_num;
		Block_Buffer buf_82310004;
		buf_82310004.make_message(msg_82310004.msg_id);
		msg_82310004.serialize(buf_82310004);
		buf_82310004.finish_message();
		player_self()->send_to_client(buf_82310004);
	}

	return 0;
}

void Invest_Plan::module_init(void) {
	if(invest_plan_detail_->is_month_card_invest && invest_plan_detail_->left_day ==  0
			&& get_month_card_invest_plan_left_monty() == 0) {
		reset_invest_plan_data();
		req_open_month_card_invest_plan();
	}
}

void Invest_Plan::sign_in(void) {

}

void Invest_Plan::sign_out(void) {

}

void Invest_Plan::trusteeship_sign_out(void) {

}

int Invest_Plan::sign_in_repeat(void) {

	return 0;
}

void Invest_Plan::save_invest_plan_tick() {
	invest_plan_detail_->detail_change();
}

int Invest_Plan::req_open_month_card_invest_plan() {
	if(invest_plan_detail_->is_month_card_invest && invest_plan_detail_->left_day ==  0
			&& get_month_card_invest_plan_left_monty() == 0) {
		reset_invest_plan_data();
	}

	const Month_Card_Invest_Cfg &month_card_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_month_card_invest_cfg();

	MSG_50101200 msg;
	msg.is_invest = invest_plan_detail_->is_month_card_invest ? 1 : 0;
	msg.left_day = invest_plan_detail_->left_day;
	msg.left_money = get_month_card_invest_plan_left_monty();

	for(Month_Card_Profit_Button_Map::const_iterator it = invest_plan_detail_->month_card_profit_button_map.begin();
			it != invest_plan_detail_->month_card_profit_button_map.end();
			++it) {
		if(0 == it->first && month_card_invest_cfg.profit_immediately == 0) {
			continue;
		}
		Month_Card_Invest_Info msg_month_card_info;
		msg_month_card_info.reset();
		msg_month_card_info.day = it->first;
		msg_month_card_info.status = it->second.status;
		msg.button_llist.push_back(msg_month_card_info);
	}
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Invest_Plan::req_get_month_card_profit(uint8_t day) {
	if(day > 30) {
		return ERROR_MONTH_CARD_INVEST_PROFIT_DAY_INVALID;
	}

	Month_Card_Profit_Button_Map::iterator find_it = invest_plan_detail_->month_card_profit_button_map.find(day);
	if(find_it == invest_plan_detail_->month_card_profit_button_map.end()) {
		return ERROR_MONTH_CARD_INVEST_PROFIT_DAY_INVALID;
	}

	if(MONTH_CARD_PROFIT_BUTTON_STATUS_NO == find_it->second.status) {
		return ERROR_CAN_NOT_GET_THIS_DAY_PROFIT;
	}

	if(MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED == find_it->second.status) {
		return ERROR_GETTED_THIS_DAY_PROFIT;
	}

	if(MONTH_CARD_PROFIT_BUTTON_STATUS_CAN_GET != find_it->second.status) {
		return ERROR_CAN_NOT_GET_THIS_DAY_PROFIT;
	}

	const Month_Card_Invest_Cfg &month_card_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_month_card_invest_cfg();

	if(0 == day) {
		if(month_card_invest_cfg.profit_immediately == 0) {
			return ERROR_NO_MONTH_CARD_IMMEDICATE_PROFIT;
		}

		MSG_50101201 msg;
		msg.day = day;

		Month_Card_Profit_Button_Map::const_iterator one_it = invest_plan_detail_->month_card_profit_button_map.find(1);
		if(one_it == invest_plan_detail_->month_card_profit_button_map.end()) {
			MSG_USER("no first day");
			return 0;
		}

		if(one_it->second.status == MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED) {
			invest_plan_detail_->left_day = (invest_plan_detail_->left_day >= 1) ? (invest_plan_detail_->left_day - 1) : 0;
		}

		find_it->second.status = MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED;

		msg.is_invest = invest_plan_detail_->is_month_card_invest ? 1 : 0;
		msg.left_day = invest_plan_detail_->left_day;
		msg.left_money = get_month_card_invest_plan_left_monty();
		THIS_SEND_TO_CLIENT(msg);

		gain_money_in_invest_plan(BIND_GOLD, month_card_invest_cfg.profit_immediately, MONEY_ADD_MONTH_CARD_INVEST_PROFIT);

		this->save_invest_plan_tick();

		Invest_Plan_History_Record invest_plan_history_record;
		invest_plan_history_record.reset();
		invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_MONTH_CARD_INVEST;
		invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_GET_PROFIT;
		invest_plan_history_record.money = month_card_invest_cfg.profit_immediately;
		invest_plan_history_record.player_name = player_self()->role_name();
		INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

		{
			MSG_82310003 msg_82310003;
			msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
			msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
			msg_82310003.history_record.money = invest_plan_history_record.money;
			msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
			THIS_SEND_TO_CLIENT(msg_82310003);
		}

		return 0;
	}

	if(1 == day) {
		MSG_50101201 msg;
		msg.day = day;

		Month_Card_Profit_Button_Map::const_iterator zero_it = invest_plan_detail_->month_card_profit_button_map.find(0);
		if(zero_it == invest_plan_detail_->month_card_profit_button_map.end()) {
			MSG_USER("no zero day");
			return 0;
		}

		if(zero_it->second.status == MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED
				|| month_card_invest_cfg.profit_immediately == 0) {
			invest_plan_detail_->left_day = (invest_plan_detail_->left_day >= 1) ? (invest_plan_detail_->left_day - 1) : 0;
		}

		find_it->second.status = MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED;

		msg.is_invest = invest_plan_detail_->is_month_card_invest ? 1 : 0;
		msg.left_day = invest_plan_detail_->left_day;
		msg.left_money = get_month_card_invest_plan_left_monty();
		THIS_SEND_TO_CLIENT(msg);

		gain_money_in_invest_plan(BIND_GOLD, month_card_invest_cfg.profit_per_day, MONEY_ADD_MONTH_CARD_INVEST_PROFIT);

		this->save_invest_plan_tick();

		Invest_Plan_History_Record invest_plan_history_record;
		invest_plan_history_record.reset();
		invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_MONTH_CARD_INVEST;
		invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_GET_PROFIT;
		invest_plan_history_record.money = month_card_invest_cfg.profit_per_day;
		invest_plan_history_record.player_name = player_self()->role_name();
		INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

		{
			MSG_82310003 msg_82310003;
			msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
			msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
			msg_82310003.history_record.money = invest_plan_history_record.money;
			msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
			THIS_SEND_TO_CLIENT(msg_82310003);
		}
		return 0;
	}

	find_it->second.status = MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED;

	MSG_50101201 msg;
	msg.day = day;
	invest_plan_detail_->left_day = (invest_plan_detail_->left_day >= 1) ? (invest_plan_detail_->left_day - 1) : 0;
	msg.is_invest = invest_plan_detail_->is_month_card_invest ? 1 : 0;
	msg.left_day = invest_plan_detail_->left_day;
	msg.left_money = get_month_card_invest_plan_left_monty();
	THIS_SEND_TO_CLIENT(msg);

	gain_money_in_invest_plan(BIND_GOLD, month_card_invest_cfg.profit_per_day, MONEY_ADD_MONTH_CARD_INVEST_PROFIT);

	this->save_invest_plan_tick();

	Invest_Plan_History_Record invest_plan_history_record;
	invest_plan_history_record.reset();
	invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_MONTH_CARD_INVEST;
	invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_GET_PROFIT;
	invest_plan_history_record.money = month_card_invest_cfg.profit_per_day;
	invest_plan_history_record.player_name = player_self()->role_name();
	INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

	{
		MSG_82310003 msg_82310003;
		msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
		msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
		msg_82310003.history_record.money = invest_plan_history_record.money;
		msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
		THIS_SEND_TO_CLIENT(msg_82310003);
	}

	if(invest_plan_detail_->is_month_card_invest && invest_plan_detail_->left_day ==  0
			&& get_month_card_invest_plan_left_monty() == 0) {
		reset_invest_plan_data();
		req_open_month_card_invest_plan();
	}

	return 0;
}

int Invest_Plan::req_month_card_invest() {
	if(invest_plan_detail_->is_month_card_invest) {
		return ERROR_INVESTED_IN_MONTH_CARD_INVEST_PLAN;
	}

	const Month_Card_Invest_Cfg &month_card_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_month_card_invest_cfg();
	int ret = consume_money_in_invest_plan(GOLD_ONLY, month_card_invest_cfg.invest_money, MONEY_SUB_MONTH_CARD_INVEST);
	if(0 != ret) {
		return ret;
	}

	invest_plan_detail_->is_month_card_invest = true;
	invest_plan_detail_->left_day = 30;
	invest_plan_detail_->days = 1;

	MSG_50101202 msg;
	msg.left_day = invest_plan_detail_->left_day;
	msg.left_money = get_month_card_invest_plan_left_monty();

	for(Month_Card_Profit_Button_Map::iterator it = invest_plan_detail_->month_card_profit_button_map.begin();
			it != invest_plan_detail_->month_card_profit_button_map.end();
			++it) {
		if(0 == it->first && month_card_invest_cfg.profit_immediately == 0) {
			continue;
		}

		if(0 == it->first || 1 == it->first) {
			it->second.status = MONTH_CARD_PROFIT_BUTTON_STATUS_CAN_GET;
		}

		Month_Card_Invest_Info  msg_month_card_invest_info;
		msg_month_card_invest_info.reset();
		msg_month_card_invest_info.day = it->first;
		msg_month_card_invest_info.status = it->second.status;
		msg.button_llist.push_back(msg_month_card_invest_info);
	}

	THIS_SEND_TO_CLIENT(msg);

	this->save_invest_plan_tick();

	Invest_Plan_History_Record invest_plan_history_record;
	invest_plan_history_record.reset();
	invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_MONTH_CARD_INVEST;
	invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_INVEST_MONEY;
	invest_plan_history_record.money = month_card_invest_cfg.invest_money;
	invest_plan_history_record.player_name = player_self()->role_name();
	INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

	{
		MSG_82310003 msg_82310003;
		msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
		msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
		msg_82310003.history_record.money = invest_plan_history_record.money;
		msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
		THIS_SEND_TO_CLIENT(msg_82310003);
	}
	return 0;
}

uint32_t Invest_Plan::get_month_card_invest_plan_left_monty() {
	uint32_t total_left_money = 0;
	const Month_Card_Invest_Cfg &month_card_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_month_card_invest_cfg();

	for(Month_Card_Profit_Button_Map::const_iterator it = invest_plan_detail_->month_card_profit_button_map.begin();
			it != invest_plan_detail_->month_card_profit_button_map.end();
			++it) {
		if(it->second.status != MONTH_CARD_PROFIT_BUTTON_STATUS_CAN_GET && it->second.status != MONTH_CARD_PROFIT_BUTTON_STATUS_NO) {
			continue;
		}

		if(it->first == 0) {
			total_left_money += month_card_invest_cfg.profit_immediately;
		}
		else {
			total_left_money += month_card_invest_cfg.profit_per_day;
		}
	}
	return total_left_money;
}

uint32_t Invest_Plan::get_upgrade_invest_total_invested_money() {
	uint32_t total_money = 0;

	for(Invested_Money_List::const_iterator it = invest_plan_detail_->invested_money_list.begin();
			it != invest_plan_detail_->invested_money_list.end();
			++it) {
		total_money += *it;
	}

	return total_money;
}

uint32_t Invest_Plan::get_upgrade_invest_plan_left_money() {
	uint32_t invested_money = get_upgrade_invest_total_invested_money();
	uint32_t total_profit = 0;

	const Level_Profit_Cfg_Map &level_profit_cfg_map = CONFIG_CACHE_INVEST_PLAN->get_level_profit_cfg_map();
	for(Level_Profit_Cfg_Map::const_iterator it = level_profit_cfg_map.begin(); it != level_profit_cfg_map.end(); ++it) {
//		if(player_self()->level() < (int)it->first) {
//			continue;
//		}

		total_profit += invested_money * it->second / 100;
	}

	uint32_t getted_profit = 0;
	for(Getted_Upgrade_Invest_Profit_Map::const_iterator level_it = invest_plan_detail_->getted_upgrade_invest_profit_map.begin();
			level_it != invest_plan_detail_->getted_upgrade_invest_profit_map.end();
			++level_it) {
		const uint32_t *level_profit_ptr =  CONFIG_CACHE_INVEST_PLAN->find_Level_Profit_Cfg(level_it->first);
		if(!level_profit_ptr) {
			continue;
		}

		uint32_t level_getted_invested_money = 0;
		for(Invested_Money_List::const_iterator money_it = level_it->second.begin();
				money_it != level_it->second.end();
				++money_it) {
			level_getted_invested_money += *money_it;
		}

		getted_profit += level_getted_invested_money * (*level_profit_ptr)/ 100;
	}

	return (total_profit > getted_profit) ? (total_profit - getted_profit) : 0;
}


bool Invest_Plan::is_upgrade_invest_full() {
	const Upgrade_Invest_Cfg &upgrade_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_upgrade_invest_cfg();
	return get_upgrade_invest_total_invested_money() >= upgrade_invest_cfg.max_invest_money;
}

int Invest_Plan::req_open_upgrade_invest() {
	MSG_50101203 msg;
	msg.is_full = is_upgrade_invest_full() ? 1 : 0;
	msg.invested_money = get_upgrade_invest_total_invested_money();
	msg.can_profit = get_upgrade_invest_can_get_lef_money();

	typedef boost::unordered_set<uint32_t> Delete_Level_Set;
	Delete_Level_Set delete_level_set;

	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		const uint32_t *level_cfg = CONFIG_CACHE_INVEST_PLAN->find_Level_Profit_Cfg(it->first);
		if(!level_cfg) {
			delete_level_set.insert(it->first);
		}
	}

	for(Delete_Level_Set::const_iterator it = delete_level_set.begin(); it != delete_level_set.end(); ++it) {
		invest_plan_detail_->upgrade_invest_profit_button_map.erase(*it);
		invest_plan_detail_->getted_upgrade_invest_profit_map.erase(*it);
	}

	if(!delete_level_set.empty()) {
		if(UPGRADE_INVEST_BUTTON_STATUS_GET_ALL_PROFIT == invest_plan_detail_->invest_button
				&& get_upgrade_invest_can_get_lef_money() == 0) {
			if(is_upgrade_invest_full()) {
				invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_HIDE_BUTTON;

			}
			else {
				invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST;
			}
		}
	}

	msg.invest_button = invest_plan_detail_->invest_button;

	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		Upgrade_Invest_Info upgrade_invest_info;
		upgrade_invest_info.reset();
		upgrade_invest_info.level = it->first;
		upgrade_invest_info.status = it->second.status;
		msg.button_list.push_back(upgrade_invest_info);
	}

	for(Invested_Money_Set::const_iterator it = invest_plan_detail_->can_invest_money_set.begin();
			it != invest_plan_detail_->can_invest_money_set.end();
			++it) {
		msg.invest_list.push_back(*it);
	}

	for(Getted_Upgrade_Invest_Profit_Map::const_iterator level_it = invest_plan_detail_->getted_upgrade_invest_profit_map.begin();
			level_it != invest_plan_detail_->getted_upgrade_invest_profit_map.end();
			++level_it) {
		uint32_t getted_level_profit = 0;
		for(Invested_Money_List::const_iterator money_it = level_it->second.begin();
				money_it != level_it->second.end();
				++money_it) {
			getted_level_profit += *money_it;
		}

		Upgrade_Invest_Profit_Info upgrade_invest_profit_info;
		upgrade_invest_profit_info.reset();
		upgrade_invest_profit_info.level = level_it->first;
		upgrade_invest_profit_info.invest_money = getted_level_profit;
		msg.upgrade_invest_profit_list.push_back(upgrade_invest_profit_info);
	}

	THIS_SEND_TO_CLIENT(msg);

	delete_level_set.clear();

	return 0;
}

uint32_t Invest_Plan::get_level_invested_money_sum(uint32_t level) {
	Getted_Upgrade_Invest_Profit_Map::const_iterator find_it = invest_plan_detail_->getted_upgrade_invest_profit_map.find(level);
	if(find_it == invest_plan_detail_->getted_upgrade_invest_profit_map.end()) {
		return 0;
	}

	uint32_t getted_level_profit = 0;
	for(Invested_Money_List::const_iterator money_it = find_it->second.begin();
			money_it != find_it->second.end();
			++money_it) {
		getted_level_profit += *money_it;
	}
	return getted_level_profit;
}

int Invest_Plan::consume_money_in_invest_plan(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type) {
	//判断是否有足够的货币
	int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(
			                   Money_Sub_Type(money_type),
			                   money_num, Money_DM_Info(lose_money_type)));
	if(return_status != 0) {
		switch(money_type) {
		case COPPER_ONLY:
			return ERROR_COPPER_NOT_ENOUGH;
		case BIND_GOLD_FIRST:
			return ERROR_GOLD_NOT_ENOUGH;
		case GOLD_ONLY:
			return ERROR_GOLD_NOT_ENOUGH;
		case BIND_GOLD_ONLY:
			return ERROR_BIND_GOLD_NOT_ENOUGH;
		case DRAGON_SOUL_ONLY:
			return ERROR_DRAGON_SOUL_NOT_ENOUGH;
		case SOULS_ONLY:
			return ERROR_SOULS_NOT_ENOUGH;
		case FRIENDSHIP_ONLY:
			return ERROR_FRIENDSHIP_NOT_ENOUGH;
		}

		return ERROR_MONEY_TYPE_ILLEGAL;
	}

	//减掉相应的货币
	if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(money_type),
			money_num, Money_DM_Info(lose_money_type))) != 0) {
		return ERROR_SUB_MONEY_FAIL;
	}

	return 0;
}

int Invest_Plan::gain_money_in_invest_plan(uint32_t money_type, uint32_t money_num, uint32_t gain_money_type) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	enum Money_Type add_money_type;
	switch(money_type) {
		case COPPER:
			add_money_type = COPPER;
			break;
		case BIND_GOLD:
			add_money_type = BIND_GOLD;
			break;
		case GOLD:
			add_money_type = GOLD;
			break;
		case SOULS:
			add_money_type = SOULS;
			break;
		case FRIENDSHIP:
			add_money_type = FRIENDSHIP;
			break;
		case DRAGON_SOUL:
			add_money_type = DRAGON_SOUL;
			break;
		default:
			add_money_type = MONEY_TYPE_END;
			break;
	}

	if (add_money_type != MONEY_TYPE_END && money_num > 0) {
		acv_msg.property.push_back(Property(add_money_type, money_num));

		Money_Add_Info money_add_info(add_money_type,
				                      money_num,
				                      Money_DM_Info(gain_money_type));
		player_self()->pack_add_money(money_add_info, Pack::MONEY_OPT_REAL);
	}

	if (!acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	return 0;
}

void Invest_Plan::getted_level_profit_to_getted_map(uint32_t level, uint32_t invest_money) {
	Getted_Upgrade_Invest_Profit_Map::iterator level_it = invest_plan_detail_->getted_upgrade_invest_profit_map.find(level);
	if(level_it == invest_plan_detail_->getted_upgrade_invest_profit_map.end()) {
		Invested_Money_List invested_money_list;
		invested_money_list.push_back(invest_money);
		invest_plan_detail_->getted_upgrade_invest_profit_map.insert(std::make_pair(level, invested_money_list));
	}
	else {
		level_it->second.push_back(invest_money);
	}
}

int Invest_Plan::req_get_upgrade_invest_profit(uint32_t level) {
	const uint32_t *level_profit_ptr = 0;
	if(level != 0) {
		level_profit_ptr = CONFIG_CACHE_INVEST_PLAN->find_Level_Profit_Cfg(level);
		if(!level_profit_ptr) {
			return ERROR_UPGRADE_INVEST_LEVEL_PROFIT_CFG_NOT_EXIST;
		}
	}

	Upgrade_Invest_Profit_Button_Map::iterator find_it = invest_plan_detail_->upgrade_invest_profit_button_map.find(level);
	if(find_it == invest_plan_detail_->upgrade_invest_profit_button_map.end()) {
		return ERROR_UPGRADE_INVEST_GET_LEVEL_INVALID;
	}

	if(UPGRADE_INVEST_PROFIT_BUTTON_NO  == find_it->second.status) {
		return ERROR_CAN_NOT_GET_THIS_LEVEL_PROFIT;
	}

	if(UPGRADE_INVEST_PROFIT_BUTTON_GETTED == find_it->second.status) {
		return ERROR_GETTED_THIS_LEVEL_PROFIT;
	}

	if(UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET != find_it->second.status) {
		return ERROR_CAN_NOT_GET_THIS_LEVEL_PROFIT;
	}

	if(level != 0 && player_self()->level() < (int)level) {
		return ERROR_LEVEL_LOWER_GET_UPGRADE_INVEST_PROFIT_LEVEL;
	}

	uint32_t getted_level_money = get_level_invested_money_sum(level);
	uint32_t total_invested_money = get_upgrade_invest_total_invested_money();

	const Upgrade_Invest_Cfg &upgrade_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_upgrade_invest_cfg();

	uint32_t add_money = 0;
	if(level != 0) {
		add_money = (total_invested_money > getted_level_money) ?
				(total_invested_money - getted_level_money) *  (*level_profit_ptr) / 100 : 0;
	}
	else {
		add_money = (total_invested_money > getted_level_money) ?
				(total_invested_money - getted_level_money) * upgrade_invest_cfg.profit_immediately / 100 : 0;
	}

	gain_money_in_invest_plan(BIND_GOLD, add_money, MONEY_ADD_UPGRADE_INVEST_LEVEL_PROFIT);

	find_it->second.status = UPGRADE_INVEST_PROFIT_BUTTON_GETTED;

	if(total_invested_money > getted_level_money) {
		getted_level_profit_to_getted_map(level, total_invested_money - getted_level_money);
	}

	if(UPGRADE_INVEST_BUTTON_STATUS_GET_ALL_PROFIT == invest_plan_detail_->invest_button
			&& get_upgrade_invest_can_get_lef_money() == 0) {
		if(is_upgrade_invest_full()) {
			invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_HIDE_BUTTON;
			MSG_82310000  msg_82310000;
			msg_82310000.invest_button = invest_plan_detail_->invest_button;
			THIS_SEND_TO_CLIENT(msg_82310000);
		}
		else {
			invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST;
			MSG_82310000  msg_82310000;
			msg_82310000.invest_button = invest_plan_detail_->invest_button;
			THIS_SEND_TO_CLIENT(msg_82310000);
		}
	}

	MSG_50101204 msg_50101204;
	msg_50101204.level = level;
	msg_50101204.can_profit = get_upgrade_invest_can_get_lef_money();

	for(Getted_Upgrade_Invest_Profit_Map::const_iterator level_it = invest_plan_detail_->getted_upgrade_invest_profit_map.begin();
			level_it != invest_plan_detail_->getted_upgrade_invest_profit_map.end();
			++level_it) {
		uint32_t getted_level_profit = 0;
		for(Invested_Money_List::const_iterator money_it = level_it->second.begin();
				money_it != level_it->second.end();
				++money_it) {
			getted_level_profit += *money_it;
		}

		Upgrade_Invest_Profit_Info upgrade_invest_profit_info;
		upgrade_invest_profit_info.reset();
		upgrade_invest_profit_info.level = level_it->first;
		upgrade_invest_profit_info.invest_money = getted_level_profit;
		msg_50101204.upgrade_invest_profit_list.push_back(upgrade_invest_profit_info);
	}

	THIS_SEND_TO_CLIENT(msg_50101204);

	this->save_invest_plan_tick();

	Invest_Plan_History_Record invest_plan_history_record;
	invest_plan_history_record.reset();
	invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_UPGRADE_INVEST;
	invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_GET_PROFIT;
	invest_plan_history_record.money = add_money;
	invest_plan_history_record.player_name = player_self()->role_name();
	INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

	{
		MSG_82310003 msg_82310003;
		msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
		msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
		msg_82310003.history_record.money = invest_plan_history_record.money;
		msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
		THIS_SEND_TO_CLIENT(msg_82310003);
	}

	return 0;
}

int Invest_Plan::req_upgrade_invest_money(uint32_t money, uint8_t type) {
	const Upgrade_Invest_Cfg &upgrade_invest_cfg = CONFIG_CACHE_INVEST_PLAN->get_upgrade_invest_cfg();
	if(player_self()->level() > (int)upgrade_invest_cfg.max_invest_level) {
		return ERROR_INVEST_MAX_LEVEL_LIMIT;
	}

	if(0 == type) {
		return process_upgrade_invest_money_first(money);
	}

	if(1 == type) {
		return req_upgrade_invest_money_add_money(money);
	}

	return ERROR_UPGRADE_INVEST_TYPE_INVALID;
}

int Invest_Plan::process_upgrade_invest_money_first(uint32_t money) {
	if(is_upgrade_invest_full()) {
		return ERROR_UPGRADE_INVEST_MONEY_IS_FULL;
	}

	if(UPGRADE_INVEST_BUTTON_STATUS_FIRST_INVEST != invest_plan_detail_->invest_button) {
		return ERROR_NOT_INVEST_MONEY_FIRST;
	}

	if(invest_plan_detail_->can_invest_money_set.find(money) == invest_plan_detail_->can_invest_money_set.end()) {
		return ERROR_NOT_CAN_INVEST_MONEY;
	}

	const Upgrade_Invest_Cfg &upgrade_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_upgrade_invest_cfg();
	int ret = consume_money_in_invest_plan(GOLD_ONLY, money, MONEY_SUB_MONEY_UPGRADE_INVEST_FIRST);
	if(0 != ret) {
		return ret;
	}

	invest_plan_detail_->invested_money += money;
	invest_plan_detail_->invested_money_list.push_back(money);

	if(is_upgrade_invest_full()) {
		if(get_upgrade_invest_can_get_lef_money() > 0) {
			invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_GET_ALL_PROFIT;
		}
		else {
			invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_HIDE_BUTTON;
		}
	}
	else {
		invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST;
	}

	for(Upgrade_Invest_Profit_Button_Map::iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		if(0 == it->first) {
			it->second.status = UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET;
			return 0;
		}

		if(player_self()->level() < (int)it->first) {
			continue;
		}

		it->second.status = UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET;
	}

	uint32_t invested_money = get_upgrade_invest_total_invested_money();
	uint32_t left_to_invest_money = (upgrade_invest_cfg.max_invest_money > invested_money) ?
					(upgrade_invest_cfg.max_invest_money - invested_money) : 0;

	if(is_upgrade_invest_full() || left_to_invest_money == 0 ||  left_to_invest_money < upgrade_invest_cfg.min_invest_money) {
		invest_plan_detail_->can_invest_money_set.clear();
	}
	else {
		invest_plan_detail_->can_invest_money_set.clear();

		invest_plan_detail_->can_invest_money_set.insert(upgrade_invest_cfg.min_invest_money);
		invest_plan_detail_->can_invest_money_set.insert(left_to_invest_money);

		if(upgrade_invest_cfg.min_invest_money < 5000 && left_to_invest_money > 5000) {
			invest_plan_detail_->can_invest_money_set.insert(5000);
		}

		if(upgrade_invest_cfg.min_invest_money < 3000 && left_to_invest_money > 3000) {
			invest_plan_detail_->can_invest_money_set.insert(3000);
		}

		if(upgrade_invest_cfg.min_invest_money < 1000 && left_to_invest_money > 1000) {
			invest_plan_detail_->can_invest_money_set.insert(1000);
		}
	}

	MSG_50101205 msg;
	msg.is_full = is_upgrade_invest_full() ? 1 : 0;
	msg.invested_money = invest_plan_detail_->invested_money;
	msg.can_profit = get_upgrade_invest_can_get_lef_money();
	msg.invest_button = invest_plan_detail_->invest_button;

	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		Upgrade_Invest_Info upgrade_invest_info;
		upgrade_invest_info.reset();
		upgrade_invest_info.level = it->first;
		upgrade_invest_info.status = it->second.status;
		msg.button_llist.push_back(upgrade_invest_info);
	}

	for(Invested_Money_Set::const_iterator it = invest_plan_detail_->can_invest_money_set.begin();
			it != invest_plan_detail_->can_invest_money_set.end();
			++it) {
		msg.invest_list.push_back(*it);
	}

	THIS_SEND_TO_CLIENT(msg);

	this->save_invest_plan_tick();

	Invest_Plan_History_Record invest_plan_history_record;
	invest_plan_history_record.reset();
	invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_UPGRADE_INVEST;
	invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_INVEST_MONEY;
	invest_plan_history_record.money = money;
	invest_plan_history_record.player_name = player_self()->role_name();
	INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

	{
		MSG_82310003 msg_82310003;
		msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
		msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
		msg_82310003.history_record.money = invest_plan_history_record.money;
		msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
		THIS_SEND_TO_CLIENT(msg_82310003);
	}

	return 0;
}

int Invest_Plan::req_upgrade_invest_money_add_money(uint32_t money) {
	if(is_upgrade_invest_full()) {
		return ERROR_UPGRADE_INVEST_MONEY_IS_FULL;
	}

	if(UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST !=  invest_plan_detail_->invest_button) {
		return ERROR_NOT_INVEST_MONEY_ADD;
	}

	if(invest_plan_detail_->can_invest_money_set.find(money) == invest_plan_detail_->can_invest_money_set.end()) {
		return ERROR_NOT_CAN_INVEST_MONEY;
	}

	const Upgrade_Invest_Cfg &upgrade_invest_cfg =  CONFIG_CACHE_INVEST_PLAN->get_upgrade_invest_cfg();
	int ret = consume_money_in_invest_plan(GOLD_ONLY, money, MONEY_SUB_MONEY_UPGRADE_INVEST_ADD_MONEY);
	if(0 != ret) {
		return ret;
	}

	invest_plan_detail_->invested_money += money;
	invest_plan_detail_->invested_money_list.push_back(money);

	if(get_upgrade_invest_can_get_lef_money() > 0) {
		invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_GET_ALL_PROFIT;
	}
	else {
		if(is_upgrade_invest_full()) {
			invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_HIDE_BUTTON;
		}
		else {
			invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST;
		}
	}

	for(Upgrade_Invest_Profit_Button_Map::iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		if(0 == it->first) {
			continue;
		}

		if(player_self()->level() < (int)it->first) {
			continue;
		}

		it->second.status = UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET;
	}

	uint32_t invested_money = get_upgrade_invest_total_invested_money();
	uint32_t left_to_invest_money = (upgrade_invest_cfg.max_invest_money > invested_money) ?
					(upgrade_invest_cfg.max_invest_money - invested_money) : 0;

	if(is_upgrade_invest_full() || left_to_invest_money == 0 ||  left_to_invest_money < upgrade_invest_cfg.min_invest_money) {
		invest_plan_detail_->can_invest_money_set.clear();
	}
	else {
		invest_plan_detail_->can_invest_money_set.clear();

		invest_plan_detail_->can_invest_money_set.insert(upgrade_invest_cfg.min_invest_money);
		invest_plan_detail_->can_invest_money_set.insert(left_to_invest_money);

		if(upgrade_invest_cfg.min_invest_money < 5000 && left_to_invest_money > 5000) {
			invest_plan_detail_->can_invest_money_set.insert(5000);
		}

		if(upgrade_invest_cfg.min_invest_money < 3000 && left_to_invest_money > 3000) {
			invest_plan_detail_->can_invest_money_set.insert(3000);
		}

		if(upgrade_invest_cfg.min_invest_money < 1000 && left_to_invest_money > 1000) {
			invest_plan_detail_->can_invest_money_set.insert(1000);
		}
	}

	MSG_50101205 msg;
	msg.is_full = is_upgrade_invest_full() ? 1 : 0;
	msg.invested_money = invest_plan_detail_->invested_money;
	msg.can_profit = get_upgrade_invest_can_get_lef_money();
	msg.invest_button = invest_plan_detail_->invest_button;

	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		Upgrade_Invest_Info upgrade_invest_info;
		upgrade_invest_info.reset();
		upgrade_invest_info.level = it->first;
		upgrade_invest_info.status = it->second.status;
		msg.button_llist.push_back(upgrade_invest_info);
	}

	for(Invested_Money_Set::const_iterator it = invest_plan_detail_->can_invest_money_set.begin();
			it != invest_plan_detail_->can_invest_money_set.end();
			++it) {
		msg.invest_list.push_back(*it);
	}

	THIS_SEND_TO_CLIENT(msg);

	this->save_invest_plan_tick();

	Invest_Plan_History_Record invest_plan_history_record;
	invest_plan_history_record.reset();
	invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_UPGRADE_INVEST;
	invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_INVEST_MONEY;
	invest_plan_history_record.money = money;
	invest_plan_history_record.player_name = player_self()->role_name();
	INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

	{
		MSG_82310003 msg_82310003;
		msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
		msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
		msg_82310003.history_record.money = invest_plan_history_record.money;
		msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
		THIS_SEND_TO_CLIENT(msg_82310003);
	}

	return 0;
}

int Invest_Plan::req_get_all_upgrade_invest_profit() {
	if(UPGRADE_INVEST_BUTTON_STATUS_GET_ALL_PROFIT != invest_plan_detail_->invest_button) {
		return ERROR_NOT_GET_ALL_UPGRADE_INVEST_PROFIT;
	}

	if(get_upgrade_invest_can_get_lef_money() == 0) {
		return ERROR_NO_GET_PROFIT_CANNOT_GET_ALL_PFOFIT;
	}

	uint32_t add_money = get_upgrade_invest_can_get_lef_money();

	gain_money_in_invest_plan(BIND_GOLD, add_money, MONEY_ADD_UPGRADE_INVEST_GET_ALL_PROFIT);

	invest_plan_detail_->getted_upgrade_invest_profit_map.clear();
	uint32_t invested_money = get_upgrade_invest_total_invested_money();

	for(Upgrade_Invest_Profit_Button_Map::iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		if(player_self()->level() < (int)it->second.level) {
			continue;
		}
		getted_level_profit_to_getted_map(it->first, invested_money);
		it->second.status = UPGRADE_INVEST_PROFIT_BUTTON_GETTED;
	}


	if(is_upgrade_invest_full()) {
		invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_HIDE_BUTTON;
	}
	else {
		invest_plan_detail_->invest_button = UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST;
	}

	MSG_50101206 msg;
	msg.can_profit = get_upgrade_invest_can_get_lef_money();
	msg.invest_button = invest_plan_detail_->invest_button;

	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		Upgrade_Invest_Info upgrade_invest_info;
		upgrade_invest_info.reset();
		upgrade_invest_info.level = it->first;
		upgrade_invest_info.status = it->second.status;
		msg.button_llist.push_back(upgrade_invest_info);
	}

	for(Getted_Upgrade_Invest_Profit_Map::const_iterator level_it = invest_plan_detail_->getted_upgrade_invest_profit_map.begin();
			level_it != invest_plan_detail_->getted_upgrade_invest_profit_map.end();
			++level_it) {
		uint32_t getted_level_profit = 0;
		for(Invested_Money_List::const_iterator money_it = level_it->second.begin();
				money_it != level_it->second.end();
				++money_it) {
			getted_level_profit += *money_it;
		}

		Upgrade_Invest_Profit_Info upgrade_invest_profit_info;
		upgrade_invest_profit_info.reset();
		upgrade_invest_profit_info.level = level_it->first;
		upgrade_invest_profit_info.invest_money = getted_level_profit;
		msg.upgrade_invest_profit_list.push_back(upgrade_invest_profit_info);
	}

	THIS_SEND_TO_CLIENT(msg);

	this->save_invest_plan_tick();

	Invest_Plan_History_Record invest_plan_history_record;
	invest_plan_history_record.reset();
	invest_plan_history_record.invest_type  = INVEST_PLAN_TYPE_UPGRADE_INVEST;
	invest_plan_history_record.history_type = INVEST_PLAN_HISTORY_RECORD_TYPE_GET_PROFIT;
	invest_plan_history_record.money = add_money;
	invest_plan_history_record.player_name = player_self()->role_name();
	INVEST_PLAN_MANAGER->add_history_record(invest_plan_history_record);

	{
		MSG_82310003 msg_82310003;
		msg_82310003.history_record.invest_type = invest_plan_history_record.invest_type;
		msg_82310003.history_record.history_type = invest_plan_history_record.history_type;
		msg_82310003.history_record.money = invest_plan_history_record.money;
		msg_82310003.history_record.player_name = invest_plan_history_record.player_name;
		THIS_SEND_TO_CLIENT(msg_82310003);
	}

	return 0;
}

int Invest_Plan::req_get_invest_plan_history() {
	MSG_50101207 msg;
	const Invest_Plan_History_Record_List &record_list =  INVEST_PLAN_MANAGER->get_record_list();
	for(Invest_Plan_History_Record_List::const_reverse_iterator it = record_list.rbegin(); it != record_list.rend(); ++it) {
		INVEST_PLAN_HISTORY_RECORD msg_invest_plan_history_record;
		msg_invest_plan_history_record.reset();
		msg_invest_plan_history_record.invest_type = it->invest_type;
		msg_invest_plan_history_record.history_type = it->history_type;
		msg_invest_plan_history_record.money = it->money;
		msg_invest_plan_history_record.player_name = it->player_name;
		msg.history_list.push_back(msg_invest_plan_history_record);
	}
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

void Invest_Plan::invest_plan_listen_level_up(const int new_level) {
	//data
	if(get_upgrade_invest_total_invested_money() == 0 || get_upgrade_invest_plan_left_money() == 0) {
		return;
	}

	for(Upgrade_Invest_Profit_Button_Map::iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		if(UPGRADE_INVEST_PROFIT_BUTTON_GETTED == it->second.status) {
			continue;
		}

		if(new_level < (int)it->first) {
			continue;
		}

		const uint32_t *level_profit_cfg_ptr = CONFIG_CACHE_INVEST_PLAN->find_Level_Profit_Cfg(it->first);
		if(!level_profit_cfg_ptr) {
			continue;
		}

		it->second.status = UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET;
	}

	this->save_invest_plan_tick();

	MSG_82310001 msg;
	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		Upgrade_Invest_Info upgrade_invest_info;
		upgrade_invest_info.reset();
		upgrade_invest_info.level = it->first;
		upgrade_invest_info.status = it->second.status;
		msg.button_list.push_back(upgrade_invest_info);
	}
	THIS_SEND_TO_CLIENT(msg);
}

uint32_t Invest_Plan::get_upgrade_invest_can_get_lef_money() {
	uint32_t invested_money = get_upgrade_invest_total_invested_money();
	uint32_t total_profit = 0;

	const Level_Profit_Cfg_Map &level_profit_cfg_map = CONFIG_CACHE_INVEST_PLAN->get_level_profit_cfg_map();
	for(Level_Profit_Cfg_Map::const_iterator it = level_profit_cfg_map.begin(); it != level_profit_cfg_map.end(); ++it) {
		if(player_self()->level() < (int)it->first) {
			continue;
		}

		total_profit += invested_money * it->second / 100;
	}

	uint32_t getted_profit = 0;
	for(Getted_Upgrade_Invest_Profit_Map::const_iterator level_it = invest_plan_detail_->getted_upgrade_invest_profit_map.begin();
			level_it != invest_plan_detail_->getted_upgrade_invest_profit_map.end();
			++level_it) {
		const uint32_t *level_profit_ptr =  CONFIG_CACHE_INVEST_PLAN->find_Level_Profit_Cfg(level_it->first);
		if(!level_profit_ptr) {
			continue;
		}

		uint32_t level_getted_invested_money = 0;
		for(Invested_Money_List::const_iterator money_it = level_it->second.begin();
				money_it != level_it->second.end();
				++money_it) {
			level_getted_invested_money += *money_it;
		}

		getted_profit += level_getted_invested_money * (*level_profit_ptr)/ 100;
	}

	return (total_profit > getted_profit) ? (total_profit - getted_profit) : 0;
}

bool Invest_Plan::is_exist_little_level_button() {
	bool flag = false;
	for(Upgrade_Invest_Profit_Button_Map::const_iterator it = invest_plan_detail_->upgrade_invest_profit_button_map.begin();
			it != invest_plan_detail_->upgrade_invest_profit_button_map.end();
			++it) {
		if(player_self()->level() >= (int)it->second.level) {
			flag = true;
		}
	}
	return flag;
}

void Invest_Plan::reset_invest_plan_data() {
	//clear
	invest_plan_detail_->is_month_card_invest = false;
	invest_plan_detail_->left_day = 0;
	invest_plan_detail_->month_card_profit_button_map.clear();
	invest_plan_detail_->days = 0;

	for(uint8_t i = 0; i <= 30; ++i) {
		Month_Card_Profit_Button month_card_profit_button;
		month_card_profit_button.reset();
		month_card_profit_button.day = i;
		month_card_profit_button.status = MONTH_CARD_PROFIT_BUTTON_STATUS_NO;
		invest_plan_detail_->month_card_profit_button_map.insert(std::make_pair(i, month_card_profit_button));
	}

	this->save_invest_plan_tick();
}
