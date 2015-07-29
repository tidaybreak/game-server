/*  
* Generate by devtool
*/

#ifndef _MSG_REWARD_H_ENUM_
#define _MSG_REWARD_H_ENUM_

enum Msg_Reward_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_RECEIVE_ONLINE_REWARD = 10106300,				//请求获取在线礼包         

		REQ_REWARD_LOGIN_SEVEN_INFO = 10106301,				//请求7日登陆奖励信息         

		REQ_RECEIVE_LOGIN_SEVEN_REWARD = 10106302,				//请求领取7日登陆奖励         

		RES_REWARD_LOGIN_SEVEN_INFO = 50106301,				//请求7日登陆奖励信息(返回)         

		RES_RECEIVE_LOGIN_SEVEN_REWARD = 50106302,				//请求领取7日登陆奖励(返回)         

		REQ_RECEIVE_ACTIVATION_CODE_REWARD = 10106303,				//请求领取激活码         

		RES_RECEIVE_ACTIVATION_CODE_REWARD = 50106303,				//请求领取激活码(返回)         

		REQ_REWARD_BACK_INFO = 10106304,				//请求奖励追回信息         

		RES_REWARD_BACK_INFO = 50106304,				//请求奖励追回信息(返回)         

		REQ_RECEIVE_REWARD_BACK = 10106305,				//请求追回奖励         

		RES_RECEIVE_REWARD_BACK = 50106305,				//请求追回奖励(返回)         

		REQ_HAVE_RECEIVE_KEEP_REWARD = 10106306,				//请求是否领取过收藏有礼         

		RES_HAVE_RECEIVE_KEEP_REWARD = 50106306,				//请求是否领取过收藏有礼(返回)         

		REQ_KEEP_REWARD_SUCCESS = 10106307,				//请求收藏添加成功         

		REQ_RECEIVE_KEEP_REWARD = 10106308,				//请求领取收藏有礼         

		RES_RECEIVE_KEEP_REWARD = 50106308,				//请求领取收藏有礼(返回)         

		REQ_REWARD_TIP_NUM = 10106309,				//请求奖励系统TIP数         

		RES_REWARD_TIP_NUM = 50106309,				//请求奖励系统TIP数(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_REWARD_H_ENUM_ */
