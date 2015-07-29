/*
 * Life_Skill_Struct.h
 *
 *  Created on: 2015年3月10日
 *      Author: zhoujin
 */

#ifndef LIFE_SKILL_STRUCT_H_
#define LIFE_SKILL_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "boost/unordered_map.hpp"


typedef boost::unordered_map<uint8_t,Int_Int> Life_Skill_Map;
struct Life_Skill_Detail: public Detail {

	void reset(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	int load(void);
	int save(void);
	Life_Skill_Detail(void) { reset(); }

    uint16_t energy;
    int remain_time;                  // 还剩多少时间可恢复5点精力,只有在下线的时候计算这个值
    uint8_t inil;                     // 是否初始化，主要是为了针对老号 0、未初始化 1、已初始化
    Life_Skill_Map skill_map;
};

#endif


