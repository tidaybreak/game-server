/*
 * Ranking_Config.h
 *
 *  Created on: 2014-05-23
 *      Author: xiaoliang
 */

#ifndef RANKING_CONFIG_H_
#define RANKING_CONFIG_H_

#include "Base_Struct.h"

struct Ranking_Config {
	Ranking_Config(void) { reset(); }
	void reset(void) {
		type = 0;
		num = 0;
		time = 0;
		day = -1;
		week = 0;
	}
	uint32_t type; // 排行榜类型
	uint32_t num;  // 显示条数
	uint32_t time; // 定时执行,相隔(秒)
	int32_t day; //每天刷新
	uint32_t week; //每周刷新
};

typedef boost::unordered_map<uint32_t, Ranking_Config> Ranking_Config_Map;

#endif /* RANKING_CONFIG_H_ */
