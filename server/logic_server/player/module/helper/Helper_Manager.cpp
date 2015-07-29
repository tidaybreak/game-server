/*
 * Helper_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Helper_Manager.h"
#include "DB_Operator.h"
#include "Config_Cache_Daily.h"
#include "Config_Cache.h"

Helper_Manager::Helper_Manager() {
	// TODO Auto-generated constructor stub
	reset();
}

Helper_Manager::~Helper_Manager() {
	// TODO Auto-generated destructor stub
}

void Helper_Manager::reset(void) {
	check_in_map.clear();
	group_id = 0;
	refresh_time = Time_Value::zero;
}

int Helper_Manager::start(void) {
	refresh_check_in();
	return 0;
}

void Helper_Manager::tick(const Time_Value &now) {
	if(refresh_time.sec() <= now.sec()) {
		refresh_check_in();
	}
}

const Check_In_Conf* Helper_Manager::get_check_in_conf_by_day(uint32_t day) const {
	Check_In_Conf_Map::const_iterator iter = check_in_map.find(day);
	if(iter != check_in_map.end()) {
		return &iter->second;
	} else {
		return 0;
	}
}

const uint32_t Helper_Manager::get_group_id(void) {
	return group_id;
}

void Helper_Manager::refresh_check_in(void) {
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	//int days = get_time_subdays(server_maintainer.game_open_time, Time_Value::gettimeofday());
	Time_Value zero_time1;
	get_zero_time(server_maintainer.game_open_time, zero_time1);
	Time_Value zero_time2;
	get_zero_time(Time_Value::gettimeofday(), zero_time2);
	int days =  (zero_time2.sec() - zero_time1.sec()) / (Time_Value::ONE_DAY_IN_SECS + 0.0) + 0.8;
	group_id = days/30 + 1;
	check_in_map = CONFIG_CACHE_DAILY->get_check_in_map_by_group_id(group_id);
	//得出下一次刷新时间
	Time_Value game_open_day_zero_time = Time_Value::zero;
	get_zero_time(server_maintainer.game_open_time, game_open_day_zero_time);
	refresh_time.sec(game_open_day_zero_time.sec() + (group_id*30) * Time_Value::ONE_DAY_IN_SECS);
}

const Time_Value& Helper_Manager::get_refresh_time(void) {
	return refresh_time;
}
