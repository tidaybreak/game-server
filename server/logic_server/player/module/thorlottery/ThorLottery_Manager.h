/*
 * ThorLotteryManager.h
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#ifndef THORLOTTERY_MANAGER_H_
#define THORLOTTERY_MANAGER_H_

#include "ThorLottery_Struct.h"

class ThorLottery_Manager {
public:
	enum ThorLotteryState {
		STATE_OFF	= 0,
		STATE_ON	= 1,
	};
public:
	void reset(void);
	void start(void);
	void tick(const Time_Value& now);
	void trigger_daily_six(const Time_Value& now);
	void activity_start(const Time_Value& start_time, const Time_Value& end_time);
	void activity_end(const Time_Value& now);

	const ThorLottery_Manager_Detail& get_thorlottery_manager_detail(void)const;
	int get_thorlottery_state(void) const;
	int get_thorlottery_season_id(void) const;
	int get_thorlottery_lv_limit(void) const;
	int get_thorlottery_end_time(const Time_Value &now) const;
	void add_thorlottery_item(int order);
	void del_thorlottery_item(int order);
	void add_thorlottery_log(role_id_t role_id, const std::string& name, int item_id, int amount, int state);
	void set_detail_change(bool change);

private:
	Time_Value active_end_time_;
	Time_Value save_db_tick_time_;
	ThorLottery_Manager_Detail thorlottery_manager_detail_;
};

typedef Singleton<ThorLottery_Manager> ThorLottery_Manager_Single;
#define THORLOTTERY_MANAGER (ThorLottery_Manager_Single::instance())

#endif /* THORLOTTERY_MANAGER_H_ */
