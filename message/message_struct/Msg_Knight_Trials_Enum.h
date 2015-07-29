/*  
* Generate by devtool
*/

#ifndef _MSG_KNIGHT_TRIALS_H_ENUM_
#define _MSG_KNIGHT_TRIALS_H_ENUM_

enum Msg_Knight_Trials_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		RES_OPEN_KNIGHT_TRIALS_PLAN = 50107100,				//请求打开骑士试炼界面(返回)         

		REQ_OPEN_KNIGHT_TRIALS_PLAN = 10107100,				//请求打开骑士试炼界面         

		REQ_ENTER_FIGHTER_KNIGHT = 10107101,				//请求进入战斗         

		RES_KNIGHT_FIGHTER_RESULT = 50107101,				//请求战斗结果(返回)         

		REQ_OPEN_KNIGHT_TRIALS_BOX_REWARD = 10107102,				//请求打开宝箱         

		RES_OPEN_KNIGHT_TRIALS_BOX_REWARD = 50107102,				//请求打开宝箱(返回)         

		REQ_RECEIVE_KNIGHT_TRIALS_BOX_REWARD = 10107103,				//请求领取宝箱奖励         

		RES_RECEIVE_KNIGHT_TRIALS_BOX_REWARD = 50107103,				//请求领取宝箱奖励(返回)         

		REQ_RESET_KNIGHT_TRIALS_PLAYER_LIST = 10107104,				//请求重置骑士试炼挑战         

		RES_RESET_KNIGHT_TRIALS_PLAYER_LIST = 50107104,				//请求重置骑士试炼挑战(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_KNIGHT_TRIALS_H_ENUM_ */
