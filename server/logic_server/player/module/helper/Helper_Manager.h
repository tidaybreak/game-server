/*
 * Helper_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef HELPER_MANAGER_H_
#define HELPER_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Helper_Config.h"

class Helper_Manager {

public:
	Helper_Manager();
	virtual ~Helper_Manager();
	void reset(void);

	int start(void);

	void tick(const Time_Value &now);

	const Check_In_Conf* get_check_in_conf_by_day(uint32_t day) const;
	const uint32_t get_group_id(void);
	const Time_Value& get_refresh_time(void);

private:
	Check_In_Conf_Map check_in_map;
	uint32_t group_id;
	Time_Value refresh_time;

	void refresh_check_in(void);
};


typedef Singleton<Helper_Manager> Helper_Manager_Single;
#define HELPER_MANAGER    (Helper_Manager_Single::instance())

#endif /* HELPER_MANAGER_H_ */
