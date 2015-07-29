/*  
* Generate by devtool
*/

#ifndef _MSG_INVEST_PLAN_H_ENUM_
#define _MSG_INVEST_PLAN_H_ENUM_

enum Msg_Invest_Plan_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_OPEN_INVEST_PLAN = 10101200,				//请求打开月卡投资页面         

		RES_OPEN_INVEST_PLAN = 50101200,				//请求打开月卡投资页面(返回)         

		REQ_GET_MONTH_CARD_PROFIT = 10101201,				//请求领取月卡投资的收益         

		RES_GET_MONTH_CARD_PROFIT = 50101201,				//请求领取月卡投资的收益(返回)         

		REQ_MONTH_CARD_INVEST = 10101202,				//请求月卡投资         

		RES_MONTH_CARD_INVEST = 50101202,				//请求月卡投资(返回)         

		REQ_OPEN_UPGRADE_INVEST = 10101203,				//请求打开升级投资页面         

		RES_OPEN_UPGRADE_INVEST = 50101203,				//请求打开升级投资页面(返回)         

		REQ_GET_UPGRADE_INVEST_PROFIT = 10101204,				//请求领取升级投资的收益         

		RES_GET_UPGRADE_INVEST_PROFIT = 50101204,				//请求领取升级投资的收益(返回)         

		REQ_UPGRADE_INVEST_MONEY = 10101205,				//请求升级投资         

		RES_UPGRADE_INVEST_MONEY = 50101205,				//请求升级投资(返回)         

		REQ_GET_ALL_UPGRADE_INVEST_PROFIT = 10101206,				//一键领取所有升级投资收益         

		RES_GET_ALL_UPGRADE_INVEST_PROFIT = 50101206,				//一键领取所有升级投资收益(返回)         

		REQ_GET_INVEST_PLAN_HISTORY = 10101207,				//请求获取投资计划的历史记录         

		RES_GET_INVEST_PLAN_HISTORY = 50101207,				//请求获取投资计划的历史记录(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_INVEST_PLAN_H_ENUM_ */
