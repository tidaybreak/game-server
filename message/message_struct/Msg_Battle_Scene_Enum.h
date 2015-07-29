/*  
* Generate by devtool
*/

#ifndef _MSG_BATTLE_SCENE_H_ENUM_
#define _MSG_BATTLE_SCENE_H_ENUM_

enum Msg_Battle_Scene_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_WAR_SCENE_FETCH_RANK = 10211001,				//请求战场排名信息         

		RES_WAR_SCENE_FETCH_RANK = 50211001,				//请求战场排名信息返回         

		REQ_WAR_SCENE_HEAL = 10211002,				//请求战场治疗         

		REQ_WAR_SCENE_ENTER = 10111000,				//请求加入战场         

		REQ_WAR_SCENE_ENTER_NUM = 10111001,				//请求战场进入次数         

		RES_WAR_SCENE_ENTER_NUM = 50111001,				//请求战场进入次数返回         

		RES_WAR_SCENE_RECEIVE = 50111003,				//请求战场领取结算奖励         

		REQ_WAR_SCENE_RECEIVE = 10111003,				//请求领取战场结算奖励         

		RES_WAR_SCENE_EX_RESULT = 50111004,				//领取战场领取额外奖励通知         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_BATTLE_SCENE_H_ENUM_ */
