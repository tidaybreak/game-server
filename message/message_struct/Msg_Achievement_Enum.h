/*  
* Generate by devtool
*/

#ifndef _MSG_ACHIEVEMENT_H_ENUM_
#define _MSG_ACHIEVEMENT_H_ENUM_

enum Msg_Achievement_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_ACHIEVEMENT_LIST = 10100800,				//获取成就列表         

		RES_ACHIEVEMENT_LIST = 50100800,				//获取成就列表(返回)         

		REQ_ACHIEVEMENT_GET = 10100801,				//获取成就奖励         

		RES_ACHIEVEMENT_GET = 50100801,				//获取成就奖励(返回)         

		REQ_ACHIEVEMENT_MEDAL_ONOFF = 10100802,				//穿脱奖章         

		RES_ACHIEVEMENT_MEDAL_ONOFF = 50100802,				//穿脱奖章(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ACHIEVEMENT_H_ENUM_ */
