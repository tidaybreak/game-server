/*
 * Invest_Plan_Config.h
 *
 *  Created on: Oct 14, 2014
 *      Author: gaojianshan
 */

#include <map>

//月卡投资配置
struct Month_Card_Invest_Cfg {
	Month_Card_Invest_Cfg() {
		reset();
	}
	void reset() {
		invest_money = 0;
		profit_immediately = 0;
		profit_per_day = 0;
	}
	uint32_t invest_money;      //月卡投资钻石数目
	uint32_t profit_immediately;//月卡投资后，立即返还粉钻数目
	uint32_t profit_per_day;    //每日领取粉钻数目
};


//升级投资配置
struct Upgrade_Invest_Cfg {
	Upgrade_Invest_Cfg() {
		reset();
	}
	void reset() {
		min_invest_money = 0;
		max_invest_money = 0;
		profit_immediately = 0;
		max_invest_level = 0;
	}
	uint32_t min_invest_money;   //等级投资钻石最小额度
	uint32_t max_invest_money;   //等级投资钻石最大额度
	uint32_t profit_immediately; //等级投资后，立即返还粉钻百分比(如：20% 应该配20，只配整数)
	uint32_t max_invest_level;//等级投资最大等级限制
};

//玩家等级对应升级投资收益百分比配置
//key:玩家等级  value:达到等级，返回粉钻为投资钻石额度的百分比(如：20% 应该配20， 只配整数)
typedef std::map<uint32_t, uint32_t> Level_Profit_Cfg_Map;

//在首次投资前,升级投资默认显示额度配置
//key:vip等级   value:默认显示投资额度
typedef std::map<uint32_t, uint32_t> Vip_Defualt_Invest_Cfg_Map;

