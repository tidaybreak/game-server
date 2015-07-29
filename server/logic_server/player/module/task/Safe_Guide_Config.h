/*
 * Safe_Guide_Config.h
 *
 *  Created on: Sep 12, 2014
 *      Author: gaojianshan
 */

#ifndef SAFE_GUIDE_CONFIG_H_
#define SAFE_GUIDE_CONFIG_H_

#include <map>

//引导保底配置
struct Safe_Guide_Config {
	Safe_Guide_Config() {
		reset();
	}
	void reset() {
		safe_type = 0;
		safe_id = 0;
		safe_level = 0;
		param1 = 0;
	}
	uint32_t safe_task;
	uint16_t safe_type;//引导保底类型
	uint64_t safe_id;//保底id
	uint32_t safe_level;//最低保底等级(如果最低保底等级为0， 没有等级限制)
	uint32_t param1;//参数1
};

//key:task_id
typedef std::multimap<uint64_t, Safe_Guide_Config> Safe_Guide_Config_Map;

enum Safe_Guide_Type_Enum {
	SAFE_GUIDE_TYPE_LEARN_ROLE_SKILL = 1,//学习主角技能
	SAFE_GUIDE_TYPE_HERO_TO_LEVEL = 2,//英雄等级提升
	SAFE_GUIDE_TYPE_GET_HERO = 3,//获得英雄
	SAFE_GUIDE_TYPE_GET_ITEM = 4,//获取道具
	SAFE_GUIDE_TYPE_OPEN_NEW_FUNCTION_ICON = 5,//新功能图标开启
	SAFE_GUIDE_TYPE_HERO_SET_FIGHT = 6,//英雄出战
	SAFE_GUIDE_TYPE_WAPE_UP_HERO_TO_QUALITY = 7, //英雄觉醒到某个品质
};

#endif /* SAFE_GUIDE_CONFIG_H_ */
