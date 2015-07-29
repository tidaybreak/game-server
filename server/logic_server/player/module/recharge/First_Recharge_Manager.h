/*
 * First_Recharge_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef FIRST_RECHARGE_MANAGER_H_
#define FIRST_RECHARGE_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Logic_Player.h"
#include "active/Active_Content_Struct.h"

class First_Recharge_Manager {

public:
	First_Recharge_Manager();
	virtual ~First_Recharge_Manager();
	void reset(void);
	void tick(const Time_Value &now);

	//图标控制相关
	void open_icon(const uint32_t icon_id, const Time_Value &close_time, const uint32_t level = 0);
	void close_icon(const uint32_t icon_id);
	Icon_Time_Info &get_icon_time_status(void);
	uint32_t get_icon_limit_level(void);
	//图标 -- end
	void open(const Time_Value &start, const Time_Value &close);
	void close(const Time_Value &close);

	const Time_Value &get_start_time(void);
	const Time_Value &get_end_time(void);

private:
	//图标 -- start
	Icon_Time_Info icon_time_status;
	uint32_t icon_limit_level;
	//图标 -- end

	Time_Value start_time;
	Time_Value end_time;
};


typedef Singleton<First_Recharge_Manager> First_Recharge_Manager_Single;
#define FIRST_RECHARGE_MANAGER    (First_Recharge_Manager_Single::instance())

#endif /* RECHARGE_REBATE_H_ */
