/*
 * Activity_Config.h
 *
 *  Created on: Apr 16, 2014
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#include "active/Active_Content_Config.h"

#ifndef ACTIVITY_CONFIG_H_
#define ACTIVITY_CONFIG_H_

struct Vip37_Config {
	Vip37_Config(void) { reset(); }
	void reset(void) {
		id = 0;
		vip_lv = 0;
		reward.reset();
	}
	uint32_t id;
	uint32_t vip_lv;
	Active_Reward_Info_Config reward;
};

typedef boost::unordered_map<uint32_t, Vip37_Config> Vip37_Conf_Map;

#endif /* ACTIVITY_37_CONFIG_H_ */
