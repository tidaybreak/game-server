/*  
* Generate by devtool
*/

#ifndef _MSG_DAILY_H_ENUM_
#define _MSG_DAILY_H_ENUM_

enum Msg_Daily_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_DAILY_INFO = 10106000,				//请求日常信息         

		RES_DAILY_INFO = 50106000,				//请求日常信息(返回)         

		REQ_ACTIVITY_INFO = 10106001,				//请求活跃度信息         

		RES_ACTIVITY_INFO = 50106001,				//请求活跃度信息(返回)         

		REQ_RECEIVE_ACTIVITY_AWARD = 10106002,				//请求领取活跃度礼包         

		RES_RECEIVE_ACTIVITY_AWARD = 50106002,				//请求领取活跃度礼包(返回)         

		REQ_TODAY_OPEN_ACTIVE = 10106003,				//请求当天开启的活动         

		RES_TODAY_OPEN_ACTIVE = 50106003,				//请求当天开启的活动(返回)         

		REQ_UPGRADE_SCORE = 10106004,				//请求实力提升列表分数         

		RES_UPGRADE_SCORE = 50106004,				//请求实力提升列表分数(返回)         

		REQ_UPGRADE_GET_STATUS = 10106005,				//请求资源不足获得方式状态         

		RES_UPGRADE_GET_STATUS = 50106005,				//请求资源不足获得方式状态(返回)         

		REQ_HELPER_CHECK_IN = 10106006,				//请求签到         

		RES_HELPER_CHECK_IN = 50106006,				//请求签到(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_DAILY_H_ENUM_ */
