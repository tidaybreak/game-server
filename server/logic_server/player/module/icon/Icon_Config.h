/*
 * Icon_Config.h
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#ifndef ICON_CONFIG_H_
#define ICON_CONFIG_H_
#include "Base_Struct.h"

struct Task_Open_Condition {
	Task_Open_Condition() : task_id(0), task_status(0) {
		reset();
	}
	void reset() {
		task_id = 0;
		task_status = 0;
	}
	uint32_t task_id;
	uint32_t task_status;
};

typedef std::list<Task_Open_Condition> Task_Open_Condition_List;

struct Icon_Base_Config {
	Icon_Base_Config(void) { reset(); }
	void reset(void) {
		icon_id = 0;
		level = 0;
		item_id = 0;
		is_join_gang = false;
		type = 0;
		is_forever_close = false;
		task_open_condition_list.clear();
		forver_close_level = 0;
	}
	uint16_t icon_id;
	int level;//玩家等级开启条件
	uint32_t item_id; //使用道具开启
	bool is_join_gang;//是否加入公会开启
	uint8_t type;     //图标类型
	bool is_forever_close;//开启后，满足条件，是否需要永久关闭图标
	uint32_t forver_close_level;
	Task_Open_Condition_List task_open_condition_list; //任务状态开启条件列表
};

typedef boost::unordered_map<uint16_t, Icon_Base_Config> Icon_Config_Map;



#endif /* ICON_CONFIG_H_ */
