/*
 * Alchemy_Config.h
 *
 *  Created on: 2014-05-05
 *      Author: xiaoliang
 */

#ifndef ALCHEMY_CONFIG_H_
#define ALCHEMY_CONFIG_H_

#include "Base_Struct.h"

struct Cards_info {
	Cards_info(void) { reset(); }
	void reset(void) {
		heroID = 0;
		hero_name.clear();
		recruit = 0;
		initial_pro_1 = 0;
		initial_pro_3 = 0;
		initial_pro_6 = 0;
		critical_1 = 0;
		critical_3 = 0;
		critical_6 = 0;
		gold_added = 0;
		critical_added_3 = 0;
		critical_added_6 = 0;
		hero_open_lv = 0;
		index = 0;
	}
	uint32_t heroID; //卡牌ID <=> 英雄ID
	std::string hero_name; //英雄名称
	int hero_open_lv; //开放等级
	uint8_t recruit; //是否已招募 0,否 1,是
	int initial_pro_1; //1倍默认概率(%)
	int initial_pro_3; //3倍默认概率(%)
	int initial_pro_6; //6倍默认概率(%)
	int critical_1;    //1倍暴击率(%)
	int critical_3;    //3倍暴击率(%)
	int critical_6;    //6倍暴击率(%)
	int gold_added;    //金币加成(%)
	int critical_added_3; //3倍暴击率加成(%)
	int critical_added_6; //6倍暴击率加成(%)
	int index;      //卡牌序号
};

typedef boost::unordered_map<uint32_t, Cards_info> Cards_Info_Map;

#endif /* ALCHEMY_CONFIG_H_ */
