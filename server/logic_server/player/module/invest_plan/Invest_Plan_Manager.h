/*
 * Invest_Plan_Manager.h
 *
 *  Created on: Oct 17, 2014
 *      Author: gaojianshan
 */

#ifndef INVEST_PLAN_MANAGER_H_
#define INVEST_PLAN_MANAGER_H_

#include "Singleton.h"
#include <list>
#include "Invest_Plan_Def.h"
#include "Time_Value.h"
#include "active/Active_Content_Struct.h"

class Invest_Plan_Manager {
public:
	Invest_Plan_Manager();
	void reset();
	void clear_history_record();
	void add_history_record(const Invest_Plan_History_Record &record);
	inline const Invest_Plan_History_Record_List& get_record_list() const;
	void save_invest_plan_history_record_list();
	void load_invest_plan_history_record_list(const Invest_Plan_History_Record_List &history_list);
	void start();

private:
	Invest_Plan_History_Record_List invest_plan_history_record_list;
};

const Invest_Plan_History_Record_List& Invest_Plan_Manager::get_record_list() const {
	return invest_plan_history_record_list;
}

typedef Singleton<Invest_Plan_Manager> Invest_Plan_Manager_Single;
#define INVEST_PLAN_MANAGER   (Invest_Plan_Manager_Single::instance())

#endif /* INVEST_PLAN_MANAGER_H_ */
