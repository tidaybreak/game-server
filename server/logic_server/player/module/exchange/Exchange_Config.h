/*
 * Exchange_Config.h
 *
 *  Created on: 2014年6月25日
 *      Author: Linqiyou
 */

#ifndef EXCHANGE_CONFIG_H_
#define EXCHANGE_CONFIG_H_

struct Exchange_Config_Detail {
	Exchange_Config_Detail(void) {
		reset();
	}
	void reset(void) {
		idx = 0;
		type = 0;
		id = 0;
		bind = 0;
		level = 0;
		exchange_type = 0;
		amount = 0;
		reputation = 0;
		limit = 0;
		stack = 0;
	}

	int idx;
	int16_t type;
	int id;
	int8_t bind; // 绑定状态：对应Item_Detail的绑定状态
	int level;
	int exchange_type; // 兑换物品需要的物品
	int amount; // 兑换物品需要的物品数量
	int reputation; // 功勋等级
	int limit;
	int stack; //兑换基数

};
typedef boost::unordered_map<int, Exchange_Config_Detail> Exchange_Config;

struct Int_Greater {
	bool operator()(const int &ui1, const int &ui2) const {
		return ui1 > ui2;
	}
};
typedef std::set<int, Int_Greater> Int_Set_Reverse;
typedef std::map<int, Int_Set> Int_IntSet_Map;
typedef std::map<int, Int_Set> Int_IntSet_Map_Reverse;
typedef std::map<int, Int_Set_Reverse, Int_Greater> Int_IntSetReverse_Map_Reverse;

struct Skill_Exchange_Item {
	Skill_Exchange_Item(void) { reset(); }
	void reset(void) {
		id = 0;
		career = 0;
		skill_series = 0;
		skill_lv = 0;
		type_cost = 0;
		num_cost = 0;
		reputation_need = 0;
	}
	int id;
	int career;
	int skill_series;
	int skill_lv;
	int type_cost;
	int num_cost;
	int reputation_need;
};
typedef boost::unordered_map<int, Skill_Exchange_Item> Skill_Exchange_Map;

#endif /* EXCHANGE_CONFIG_H_ */
