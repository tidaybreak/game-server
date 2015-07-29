/*
 * Day_Week_Task_Config.h
 *
 *  Created on: Aug 19, 2014
 *      Author: gaojianshan
 */

#ifndef DAY_WEEK_TASK_CONFIG_H_
#define DAY_WEEK_TASK_CONFIG_H_

#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"

//group number
typedef boost::unordered_set<uint32_t> Day_Group_Set;
//key:time
typedef boost::unordered_map<uint8_t, Day_Group_Set> Day_Group_Map;

//group number
typedef boost::unordered_set<uint32_t> Week_Group_Set;

//key:  (uint16_t)(((uint32_t)wday << 8) | (uint32_t)time)
typedef boost::unordered_map<uint16_t, Week_Group_Set> Week_Group_Map;

//note: wday => 0 - 6  周日到周五
inline uint16_t create_key_of_week_group_map(uint8_t wday, uint8_t time) {
	return (uint16_t)(((uint32_t)wday << 8) | (uint32_t)time);
}

//日常周常奖励
struct Day_Week_Task_Award_Cfg {
	Day_Week_Task_Award_Cfg() {
		reset();
	}
	void reset() {
		exp = 0;
		coin = 0;
		dragon_soul = 0;
		soul = 0;

		item1_id = 0;
		item1_num = 0;
		is_item1_bind = false;

		item2_id = 0;
		item2_num = 0;
		is_item2_bind = false;

		item3_id = 0;
		item3_num = 0;
		is_item3_bind = false;

		item4_id = 0;
		item4_num = 0;
		is_item4_bind = false;
	}
	uint32_t exp;
	uint32_t coin;
	uint32_t dragon_soul;
	uint32_t soul;

	uint32_t item1_id;
	uint32_t item1_num;
	bool is_item1_bind;

	uint32_t item2_id;
	uint32_t item2_num;
	bool is_item2_bind;

	uint32_t item3_id;
	uint32_t item3_num;
	bool is_item3_bind;

	uint32_t item4_id;
	uint32_t item4_num;
	bool is_item4_bind;
};

typedef boost::unordered_map<uint32_t, Day_Week_Task_Award_Cfg> Day_Week_Task_Award_Cfg_Map;

struct Day_Week_Task_Config {
	Day_Week_Task_Config() {
		reset();
	}
	void reset(){
		day_group_map.clear();
		week_group_map.clear();
		day_task_award_cfg_map.clear();
		week_task_award_cfg_map.clear();
	}
	Day_Group_Map day_group_map;
	Week_Group_Map week_group_map;
	Day_Week_Task_Award_Cfg_Map day_task_award_cfg_map;
	Day_Week_Task_Award_Cfg_Map week_task_award_cfg_map;
};

#endif /* DAY_WEEK_TASK_CONFIG_H_ */
