/*  
* Generate by devtool
*/

#ifndef _MSG_OTHER_H_ENUM_
#define _MSG_OTHER_H_ENUM_

enum Msg_Other_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_BUY_PHY_POWER = 10120000,				//购买体力         

		RES_BUY_PHY_POWER = 50120000,				//购买体力返回         

		REQ_FETCH_PHY_POWER_INFO = 10120001,				//请求体力信息         

		RES_FETCH_PHY_POWER_INFO = 50120001,				//请求体力信息返回         

		REQ_CHAT_BUGLE = 10120002,				//请求小喇叭聊天         

		REQ_GET_SYS_CFG = 10120003,				//请求获取系统设置         

		RES_GET_SYS_CFG = 50120003,				//请求获取系统设置(返回)         

		REQ_SET_SYS_CFG = 10120004,				//请求更改系统设置         

		REQ_FETCH_AWAKE_LVL = 10120020,				//获取玩家觉醒等级         

		RES_FETCH_AWAKE_LVL = 50120020,				//获取玩家觉醒等级（返回）         

		REQ_AWAKE_LVL_UPGRADE = 10120021,				//玩家觉醒等级升级         

		RES_AWAKE_LVL_UPGRADE = 50120021,				//玩家觉醒等级升级（返回）         

		REQ_FETCH_PLAYER_ARTIFACT_INFO = 10120030,				//获取玩家神器信息         

		RES_FETCH_PLAYER_ARTIFACT_INFO = 50120030,				//获取玩家神器信息（返回）         

		REQ_FORENOTICE_RECEIVE_GIFT = 10120040,				//请求领取等级礼包         

		RES_FORENOTICE_RECEIVE_GIFT = 50120040,				//请求领取等级礼包（返回）         

		REQ_FORENOTICE_RECEIVED_GIFTS = 10120041,				//请求已领过的等级礼包         

		RES_FORENOTICE_RECEIVED_GIFTS = 50120041,				//请求已领过的等级礼包（返回）         

		REQ_HANDSEL_FLOWERS = 10120050,				//请求送花         

		REQ_FETCH_SECRET_SHOP_INFO = 10120010,				//请求神秘商店信息         

		RES_FETCH_SECRET_SHOP_INFO = 50120010,				//请求神秘商店信息返回         

		REQ_SECRET_SHOP_BUY_ITEM = 10120011,				//请求神秘商店购买物品         

		RES_SECRET_SHOP_BUY_ITEM = 50120011,				//请求神秘商店购买物品返回         

		REQ_LOOK_FLOWERS = 10120051,				//请求查看鲜花         

		RES_HANDSEL_FLOWERS = 50120050,				//请求送花(返回)         

		REQ_SEND_PROPOSAL = 10120052,				//请求发送你提我改         

		RES_SEND_PROPOSAL = 50120052,				//请求发送你提我改(返回)         

		REQ_EXPLOIT_INFO = 10120080,				//获取玩家功勋信息         

		RES_EXPLOIT_INFO = 50120080,				//获取玩家功勋信息（返回）         

		REQ_OPEN_FIRST_RECHARGE = 10120090,				//请求打开首充面板         

		RES_OPEN_FIRST_RECHARGE = 50120090,				//请求打开首充面板(返回)         

		REQ_RECEIVE_FIRST_RECHARGE = 10120091,				//请求领取首充礼包         

		RES_RECEIVE_FIRST_RECHARGE = 50120091,				//请求领取首充礼包(返回)         

		REQ_OPEN_RECHARGE_REBATE = 10120092,				//请求打开充值返利面板         

		RES_OPEN_RECHARGE_REBATE = 50120092,				//请求打开充值返利面板(返回)         

		REQ_RECEIVE_RECHARGE_REBATE = 10120093,				//请求领取充值返利奖励         

		RES_RECEIVE_RECHARGE_REBATE = 50120093,				//请求领取充值返利奖励(返回)         

		REQ_RECEIVE_REBATE_RECHARGE_STATUS = 10120094,				//请求充值返利是否可领取         

		REQ_GUIDE_ADD_ITEM_IN_PLOT = 10120095,				//新手引导，剧情中采集道具         

		REQ_SEND_CHALLENGE_TO_GANG_CHAT = 10120096,				//请求发送擂台结果到公会聊天         

		RES_SEND_CHALLENGE_TO_GANG_CHAT = 50120096,				//请求发送擂台结果到公会聊天(返回)         

		REQ_QUESTIONNAIRE_INFO = 10120097,				//请求调查问卷信息         

		RES_QUESTIONNAIRE_INFO = 50120097,				//请求调查问卷信息(返回)         

		REQ_SBUMIT_QUESTIONNAIRE = 10120098,				//请求提交调查问卷信息         

		REQ_OPEN_INVITE_REWARD_INFO = 10120060,				//请求打开邀请奖励信息         

		RES_OPEN_INVITE_REWARD_INFO = 50120060,				//请求打开邀请奖励信息(返回)         

		RES_RECEIVE_INVITE_REWARD_INFO = 50120061,				//请求领取邀请奖励信息(返回)         

		REQ_RECEIVE_INVITE_REWARD_INFO = 10120061,				//请求领取邀请奖励信息         

		REQ_FETCH_MONTH_HERO_INFO = 10120070,				//请求每月英雄信息         

		RES_FETCH_MONTH_HERO_INFO = 50120070,				//请求每月英雄信息返回         

		REQ_OPEN_TWO_RECHARGE = 10120099,				//请求打开二次(全民)充值面板         

		RES_OPEN_TWO_RECHARGE = 50120099,				//请求打开二次(全民)充值面板(返回)         

		REQ_RECEIVE_TWO_RECHARGE = 10120100,				//请求领取二次(全民)充值面板         

		RES_RECEIVE_TWO_RECHARGE = 50120100,				//请求领取二次(全民)充值面板(返回)         

		REQ_OPEN_LIMIT_TIME_RECHARGE = 10120101,				//请求打开限时充值面板         

		RES_OPEN_LIMIT_TIME_RECHARGE = 50120101,				//请求打开限时充值面板(返回)         

		REQ_RECEIVE_LIMIT_TIME_RECHARGE = 10120102,				//请求领取限时充值面板         

		RES_RECEIVE_LIMIT_TIME_RECHARGE = 50120102,				//请求领取限时充值面板(返回)         

		REQ_PLOT_ALL_ADD_ONE = 10120110,				//剧情数群加1         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_OTHER_H_ENUM_ */
