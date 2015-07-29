/*
 * Active_Config.h
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef ACTIVE_CONFIG_H_
#define ACTIVE_CONFIG_H_

#include "Game_Typedef.h"
struct Event {
	uint32_t relatively;
	uint32_t action;
	uint32_t rel_back;
	UInt_Vec argv;
	Event(void) { reset(); }
	void reset(void) {
		relatively = 0;
		action = 0;
		argv.clear();
		rel_back = 0;
	}
};
typedef std::vector<Event> Event_Vec;
struct Active_Config {
	Active_Config(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		type = 0;
		icon = 0;
		open_type = 0;
		interval_open_date.clear();
		loop_open_date = 0;
		time.clear();
		blank_type = 0;
		blank_days = 0;
		loop = 0;
		over_time = 0;
		continued = 0;
		CLEAR_VECTOR(Event_Vec, event_action);
		before_time = 0;
		openLevel = 0;
		after_id = 0;
	}
	uint32_t id;
	uint32_t type;
	uint32_t icon;
	uint32_t open_type; //开启类型
	UInt_Vec interval_open_date; //开启期限(数组)
	uint32_t loop_open_date;//开启期限(数字,循环)
	std::string assign_open_date; //开启期限(指定日期,字符)
	uint32_t openLevel;//开启等级
	uint32_t over_time;
	uint32_t after_id;//后置于活动结束

	UInt_Vec time;	//相对时间

	uint32_t before_time; //动作提前(分)

	uint32_t blank_type; //启始类型
	uint32_t blank_days; //启始值(天) 间隔
	std::string assign_blank_time; //指定启始日期,字符

	uint32_t loop; //总执行次数
	uint32_t continued;//活动持续时间(分)

	Event_Vec event_action;
};

typedef boost::unordered_map<uint32_t, Active_Config> Active_Conf_Map;

#endif /* ACTIVE_CONFIG_H_ */
