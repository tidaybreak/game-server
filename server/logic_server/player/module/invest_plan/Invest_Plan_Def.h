/*
 * Invest_Plan_Def.h
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */

#ifndef INVEST_PLAN_DEF_H_
#define INVEST_PLAN_DEF_H_

#include <string>

//月卡投资领取按钮状态
enum Motnth_Card_Profit_Button_Status {
	MONTH_CARD_PROFIT_BUTTON_STATUS_NO = 0,      //不可领取
	MONTH_CARD_PROFIT_BUTTON_STATUS_CAN_GET = 1, //可领取
	MONTH_CARD_PROFIT_BUTTON_STATUS_GETTED = 2,  //已领取
};


//升级投资领取按钮状态
enum Upgrade_Invest_Profit_Button_Status {
	UPGRADE_INVEST_PROFIT_BUTTON_NO = 0,	   //不可领取
	UPGRADE_INVEST_PROFIT_BUTTON_CAN_GET = 1,  //可领取
	UPGRADE_INVEST_PROFIT_BUTTON_GETTED = 2,   //已领取
};

//升级投资马上投资按钮状态
enum Upgrade_Invest_Button_Status {
	UPGRADE_INVEST_BUTTON_STATUS_FIRST_INVEST = 0,    //马上投资
	UPGRADE_INVEST_BUTTON_STATUS_ADD_INVEST = 1,      //追加投资
	UPGRADE_INVEST_BUTTON_STATUS_GET_ALL_PROFIT = 2,  //领取追加收益
	UPGRADE_INVEST_BUTTON_STATUS_HIDE_BUTTON = 3,     //隐藏按钮
};

//投资计划的类型
enum INVEST_PLAN_TYPE {
	INVEST_PLAN_TYPE_MONTH_CARD_INVEST = 1, //月卡投资
	INVEST_PLAN_TYPE_UPGRADE_INVEST = 2,    //升级投资
};

//投资计划历史记录类型
enum INVEST_PLAN_HISTORY_RECORD_TYPE {
	INVEST_PLAN_HISTORY_RECORD_TYPE_INVEST_MONEY = 1, //存入
	INVEST_PLAN_HISTORY_RECORD_TYPE_GET_PROFIT = 2,   //领取
};

//投资计划历史记录
struct Invest_Plan_History_Record {
	Invest_Plan_History_Record() {
		reset();
	}
	void reset() {
		invest_type = 0;
		history_type = 0;
		money = 0;
		player_name = "";
	}
	uint8_t invest_type;//投资计划的类型
	uint8_t history_type;//投资计划历史记录类型
	uint32_t money;      //存入/领取的钻石/粉钻数目
	std::string player_name;  //玩家姓名
};

typedef std::list<Invest_Plan_History_Record> Invest_Plan_History_Record_List;

#endif /* INVEST_PLAN_DEF_H_ */
