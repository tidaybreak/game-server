/*
 * Two_Recharge_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef TWO_RECHARGE_MANAGER_H_
#define TWO_RECHARGE_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Logic_Player.h"
#include "active/Active_Content_Struct.h"

class Two_Recharge_Manager {

public:
	Two_Recharge_Manager();
	virtual ~Two_Recharge_Manager();
	void reset(void);
	void tick(const Time_Value &now);

	//图标控制相关 - 紧限与开服活动和开服活动以后的变种活动使用;没法倒计时也没法做任何事情,只能表示开和关
	void open_icon(const uint32_t icon_id, const Time_Value &close_time, const uint32_t level = 0);
	void close_icon(const uint32_t icon_id);
	Icon_Time_Info &get_icon_time_status(void);
	uint32_t get_icon_limit_level(void);
	//图标 -- end
	void open(const Time_Value &start, const Time_Value &close);
	void close(const Time_Value &close);

	const Time_Value &get_start_time(void);
	const Time_Value &get_end_time(void);

	void up_rec_num(void);
	uint32_t get_rec_num(void);
private:
	//图标 -- start
	Icon_Time_Info icon_time_status;
	uint32_t icon_limit_level;
	//图标 -- end

	Time_Value start_time;
	Time_Value end_time;

	Two_Recharge_Info info;
};


typedef Singleton<Two_Recharge_Manager> Two_Recharge_Manager_Single;
#define TWO_RECHARGE_MANAGER    (Two_Recharge_Manager_Single::instance())

#endif /* RECHARGE_REBATE_H_ */
