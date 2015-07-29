/*  
* Generate by devtool
*/

#ifndef _MSG_CAMPFIRE_H_ENUM_
#define _MSG_CAMPFIRE_H_ENUM_

enum Msg_Campfire_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_CAMPFIRE_ENTER = 10100700,				//请求进入篝火晚会         

		REQ_CAMPFIRE_ADD_FIREWOOD = 10200701,				//请求添加篝火柴火         

		REQ_CAMPFIRE_FIRE = 10200702,				//请求点燃篝火         

		REQ_CAMPFIRE_RANK = 10200703,				//请求篝火排名         

		RES_CAMPFIRE_RANK = 50200703,				//请求篝火排名（返回）         

		REQ_CAMPFIRE_ADDWOOD_AWARD = 10200704,				//请求点燃/添柴奖励         

		RES_CAMPFIRE_ADDWOOD_AWARD = 50200704,				//请求点燃/添柴奖励(返回)         

		REQ_CAMPFIRE_SPOT_AWARD = 10200705,				//请求篝火结算奖励         

		RES_CAMPFIRE_SPOT_AWARD = 50200705,				//请求篝火结算奖励(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_CAMPFIRE_H_ENUM_ */
