/*
 * Helper_Config.h
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef HELPER_CONFIG_H_
#define HELPER_CONFIG_H_

#include "Game_Typedef.h"


struct Daily_Config {
	uint32_t id;
	uint32_t fulfil_num;
	uint32_t task_type;
	int32_t type_id;
	uint8_t open_type;
	Int_Int_Map time;

	Daily_Config() {
		reset();
	}
	void reset(void) {
		id = 0;
		fulfil_num = 0;
		task_type = 0;
		type_id = 0;
		open_type = 0;
		time.clear();
	}
};

struct Activity_Config {
	uint32_t id;//活跃度ID
	uint32_t fulfil_num;//需要完成次数
	uint32_t task_type;//项目类型
	int32_t type_id;//项目类型对应ID
	uint32_t activity; //活跃度
	Activity_Config() {
		reset();
	}
	void reset(void) {
		id = 0;
		fulfil_num = 0;
		task_type = 0;
		type_id = 0;
		activity = 0;
	}
};

struct Config_Key {
	uint32_t task_type;
	int32_t type_id;
	Config_Key() {
		reset();
	}
	void reset(void) {
		task_type = 0;
		type_id = 0;
	}
	bool operator<(const Config_Key &key) const  {
		if (task_type == key.task_type)
			return type_id < key.type_id;
		return task_type < key.task_type;
	}
};

typedef std::map<Config_Key, Daily_Config> Daily_Conf_Map;
typedef std::map<Config_Key, Activity_Config> Activity_Conf_Map;
typedef std::map<uint32_t, Config_Key> Conf_Key_Map;

struct Activity_Award_Config { //活跃度奖励
	uint32_t id;
	uint32_t act;
	uint32_t award_id;
	uint32_t num;
	Activity_Award_Config() {
		reset();
	}
	void reset(void) {
		id = 0;
		act = 0;
		award_id = 0;
		num = 0;
	}
};
typedef std::map<uint32_t, Activity_Award_Config, std::less<uint32_t>> Activity_Award_Map;
typedef boost::unordered_map<uint32_t, Activity_Award_Map> Activity_Award_Group_Conf_Map;

struct Activity_Group_Config {
	uint32_t group;
	uint32_t start;
	uint32_t end;
	Activity_Group_Config() {
		reset();
	}
	void reset(void) {
		group = 0;
		start = 0;
		end = 0;
	}
};
typedef boost::unordered_map<uint32_t, Activity_Group_Config> Activity_Group_Conf_Map;


struct Upgrade_Conf {
	Upgrade_Conf(void) { reset(); }
	void reset(void) {
		level_expect.clear();
	}

	uint32_t id;
	Int_Int_Map level_expect;
};

typedef boost::unordered_map<uint32_t, Upgrade_Conf> Upgrade_Conf_Map;


struct Check_In_Conf {
	Check_In_Conf(void) { reset(); }
	void reset(void) {
		day =0;
		item_id = 0;
		num = 0;
		vip_double = 0;
		money_id = 0;
		exp = 0;
	}
	uint32_t day;
	uint32_t item_id;
	uint32_t money_id;
	uint32_t exp;
	uint32_t num;
	uint32_t vip_double;
};
typedef boost::unordered_map<uint32_t, Check_In_Conf> Check_In_Conf_Map;
typedef std::map<uint32_t, Check_In_Conf_Map, std::less<uint32_t>> Check_In_Group_Conf_Map;

#endif /* HELPER_CONFIG_H_ */
