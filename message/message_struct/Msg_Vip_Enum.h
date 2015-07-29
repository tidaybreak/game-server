/*  
* Generate by devtool
*/

#ifndef _MSG_VIP_H_ENUM_
#define _MSG_VIP_H_ENUM_

enum Msg_Vip_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_OPEN_VIP = 10106200,				//请求开通VIP         

		RES_OPEN_VIP = 50106200,				//请求开通VIP(返回)         

		REQ_VIP_WORLD_TREE_INFO = 10106201,				//请求世界之树信息         

		RES_VIP_WORLD_TREE_INFO = 50106201,				//请求世界之树信息(返回)         

		REQ_VIP_RECEIVE_WORLD_TREE = 10106202,				//请求领取世界之树奖励         

		RES_VIP_RECEIVE_WORLD_TREE = 50106202,				//请求领取世界之树奖励(返回)         

		REQ_VIP_TURNTABLE_OPEN = 10106203,				//请求打开VIP转盘获取物品组         

		RES_VIP_TURNTABLE_OPEN = 50106203,				//请求打开VIP转盘获取物品组(返回)         

		REQ_VIP_TURNTABLE_LOTTERY = 10106204,				//请求VIP转盘抽奖         

		RES_VIP_TURNTABLE_LOTTERY = 50106204,				//请求VIP转盘抽奖(返回)         

		REQ_VIP_TURNTABLE_RECEIVE = 10106205,				//请求获取VIP转盘奖品         

		REQ_RECEIVE_VIP_DAILY_REWARD = 10106206,				//请求领取VIP日礼包         

		RES_RECEIVE_VIP_DAILY_REWARD = 50106206,				//请求领取VIP日礼包(返回)         

		REQ_OPEN_VIP_DAILY_REWARD = 10106207,				//请求打开VIP日礼包         

		RES_OPEN_VIP_DAILY_REWARD = 50106207,				//请求打开VIP日礼包(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_VIP_H_ENUM_ */
