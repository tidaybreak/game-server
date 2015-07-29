/*
 * Invest_Plan_Manager.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: gaojianshan
 */

#include "Invest_Plan_Manager.h"
#include "DB_Manager.h"
#include "DB_Operator.h"
#include "DB_Def.h"
#include "Logic_Monitor.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"

Invest_Plan_Manager::Invest_Plan_Manager() {
	reset();
}

void Invest_Plan_Manager::reset() {
	invest_plan_history_record_list.clear();
}

void Invest_Plan_Manager::add_history_record(const Invest_Plan_History_Record &record) {
	while(invest_plan_history_record_list.size() >= 50) {
		invest_plan_history_record_list.pop_front();
	}

	invest_plan_history_record_list.push_back(record);

	save_invest_plan_history_record_list();
}

void Invest_Plan_Manager::save_invest_plan_history_record_list() {
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_INVEST_PLAN_HISTORY_RECORD);

	buf.write_uint8((uint8_t)invest_plan_history_record_list.size());

	for(Invest_Plan_History_Record_List::const_iterator it = invest_plan_history_record_list.begin();
			it != invest_plan_history_record_list.end();
			++it) {
		buf.write_uint8(it->invest_type);
		buf.write_uint8(it->history_type);
		buf.write_uint32(it->money);
		buf.write_string(it->player_name);
	}

	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

void Invest_Plan_Manager::load_invest_plan_history_record_list(const Invest_Plan_History_Record_List &history_list) {
	invest_plan_history_record_list.clear();
	invest_plan_history_record_list.assign(history_list.begin(), history_list.end());
}

void Invest_Plan_Manager::start() {
	Block_Buffer buf;
	buf.make_message(MONGO_LOAD_INVEST_PLAN_HISTORY_RECORD);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

void Invest_Plan_Manager::clear_history_record() {
	invest_plan_history_record_list.clear();
	save_invest_plan_history_record_list();
}
