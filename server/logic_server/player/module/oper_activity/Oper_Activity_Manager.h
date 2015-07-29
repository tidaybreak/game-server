/*
 * Oper_Activity_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef OPER_ACTIVE_MANAGER_H_
#define OPER_ACTIVE_MANAGER_H_

#include <oper_activity/Oper_Activity_Struct.h>
#include "Singleton.h"
#include "Game_Typedef.h"

class Oper_Activity_Manager {

public:
	Oper_Activity_Manager();
	virtual ~Oper_Activity_Manager();
	void reset(void);

	int start(void);

	void tick(Time_Value &now);

	void open(const Time_Value &start, const Time_Value &end);
	void close(const Time_Value &end);

	void open_item(const uint32_t act_id, const Time_Value &start, const Time_Value &end);
	void close_item(const uint32_t act_id, const Time_Value &end);

	bool item_is_open(const uint32_t act_id);

	const Time_Value &get_item_start_time(const uint32_t act_id);
	const Time_Value &get_item_end_time(const uint32_t act_id);

private:
//BIG
	Time_Value act_start;
	Time_Value act_end;
//
	UInt_Time_Map start_time;
	UInt_Time_Map end_time;


	void try_close_append_effect(const uint32_t act_id);
	void try_append_effect(const uint32_t act_id);

};


typedef Singleton<Oper_Activity_Manager> Oper_Activity_Manager_Single;
#define OPER_ACTIVE_MANAGER    (Oper_Activity_Manager_Single::instance())

#endif /* OPER_ACTIVE_H_ */
