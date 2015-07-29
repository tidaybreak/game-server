/*
 * Invest_Plan_Struct.h
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */

#ifndef INVEST_PLAN_STRUCT_H_
#define INVEST_PLAN_STRUCT_H_

#include "Server_Struct.h"
#include <map>
#include <set>
#include <list>

//月卡投资领取按钮信息
struct Month_Card_Profit_Button {
	Month_Card_Profit_Button() {
		reset();
	}
	void reset() {
		day = 0;
		status = 0;
	}
	uint8_t day;    //第几天 day=0，表示投资后，立即领取的按钮
	uint8_t status; //按钮状态 0:不可领取，1：可领取，2：已领取
};

//key:第几天
typedef std::map<uint8_t, Month_Card_Profit_Button> Month_Card_Profit_Button_Map;

//升级投资领取按钮信息
struct Upgrade_Invest_Profit_Button {
	Upgrade_Invest_Profit_Button() {
		reset();
	}
	void reset() {
		level = 0;
		status = 0;
	}
	uint32_t level; //等级  level=0，表示升级投资后，立即领取
	uint8_t status; //按钮状态 0:不可领取，1：可领取，2：已领取
};

//key: level
typedef std::map<uint32_t, Upgrade_Invest_Profit_Button> Upgrade_Invest_Profit_Button_Map;

//key:投资额度
typedef std::set<uint32_t> Invested_Money_Set;

//投资额度
typedef std::list<uint32_t> Invested_Money_List;

//key:level value:已经领取升级投资收益的投资额度列表
typedef std::map<uint32_t, Invested_Money_List> Getted_Upgrade_Invest_Profit_Map;

struct Invest_Plan_Detail: public Detail {
	Invest_Plan_Detail(void);
	virtual ~Invest_Plan_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	bool is_month_card_invest;//是否已月卡投资
	uint8_t left_day;         //还有多少天可领
	Month_Card_Profit_Button_Map month_card_profit_button_map;//月卡投资领取按钮列表
	uint8_t days;            //月卡投资后，当前登录的是第几天

	uint32_t invested_money;  //已投资钻石数目
	uint8_t invest_button;    //马上投资按钮状态 0:马上投资 1:追加投资 2:领取追加收益 3:隐藏按钮
	Upgrade_Invest_Profit_Button_Map upgrade_invest_profit_button_map;//升级投资领取按钮列表
	Invested_Money_Set can_invest_money_set;						  //可以投资的额度列表
	Invested_Money_List invested_money_list;                          //已经投资的额度列表
	Getted_Upgrade_Invest_Profit_Map getted_upgrade_invest_profit_map;//已经领取收益的等级对应的投资额度列表
};



#endif /* INVEST_PLAN_STRUCT_H_ */
