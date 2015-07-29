/*
 * Achievement_Config.h
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#ifndef ACHIEVEMENT_CONFIG_H_
#define ACHIEVEMENT_CONFIG_H_

#include "Game_Typedef.h"
#include "Base_Struct.h"

struct Achievement_Cfg_Setting {
	Achievement_Cfg_Setting(void) {
		reset();
	}
	void reset(void) {
		prop.clear();
		bind_diamond = 0;
	}
	Property_Vec prop;

	int32_t bind_diamond;
};

struct Achievement_Cfg_Detail {
	Achievement_Cfg_Detail(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		name.clear();
		num = 0;
		award.clear();
		broadcast = 0;
		des.clear();
		type = 0;
		cond = 0;
		cond_ext = 0;
		cond_ext1.clear();
	}
	int32_t id;
	std::string name;
	int32_t num;
	Item_Info_Vec award;
	int8_t broadcast;
	std::string des;
	int32_t type;
	int32_t cond;
	int32_t cond_ext; // 用于特殊类的条件：章节通关的关数
	Int_Int_Set_Map cond_ext1;
};
typedef boost::unordered_map<int32_t, Achievement_Cfg_Detail> Achievement_Cfg_Detail_Map;

struct Achievement_Cfg_Medal_Detail {
	Achievement_Cfg_Medal_Detail(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		type = 0;
		num = 0;
		profile = 0;
	}

	int32_t id;
	int8_t type;
	int32_t num;
	int32_t profile;
};
typedef boost::unordered_map<int32_t, Achievement_Cfg_Medal_Detail> Achievement_Cfg_Medal_Map;

#endif /* ACHIEVEMENT_CONFIG_H_ */
