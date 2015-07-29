/*  
* Generate by devtool
*/

#ifndef _MSG_ARENA_H_ENUM_
#define _MSG_ARENA_H_ENUM_

enum Msg_Arena_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_LOCAL_ARENA_RANK_INFO = 10100400,				//获取竞技场本服排行         

		RES_FETCH_LOCAL_ARENA_RANK_INFO = 50100400,				//获取竞技场本服排行返回         

		REQ_FETCH_ARENA_INFO = 10100405,				//获取竞技场信息         

		RES_FETCH_ARENA_INFO = 50100405,				//获取竞技场信息返回         

		REQ_CLOSE_ARENA_PANEL = 10100406,				//关闭竞技场面板         

		REQ_MATCH_ARENA_BATTLE = 10100410,				//请求匹配竞技场战斗         

		RES_MATCH_ARENA_BATTLE = 50100410,				//请求匹配竞技场战斗返回         

		REQ_ARENA_BUY_PAY_TIMES = 10100420,				//请求竞技场购买付费挑战次数         

		RES_ARENA_BUY_PAY_TIMES = 50100420,				//请求购买竞技场付费挑战次数返回         

		REQ_CANCEL_MATCH_ARENA_BATTLE = 10100411,				//取消匹配竞技场战斗         

		RES_CANCEL_MATCH_ARENA_BATTLE = 50100411,				//取消匹配竞技场战斗返回         

		REQ_ARENA_RECEIVE_REWARD = 10100401,				//领取竞技场3天或者7天奖励         

		RES_ARENA_RECEIVE_REWARD = 50100401,				//领取竞技场3天或者7天奖励返回         

		REQ_FETCH_ARENA_AWARD_INFO = 10100402,				//获取竞技场玩家奖励信息         

		RES_FETCH_ARENA_AWARD_INFO = 50100402,				//获取竞技场玩家奖励信息返回         

		REQ_RESET_ARENA_CD = 10100407,				//请求重置竞技场cd         

		RES_RESET_ARENA_CD = 50100407,				//请求重置竞技场cd返回         

		REQ_ARENA_MATCH = 10100412,				//请求竞技场匹配         

		RES_ARENA_MATCH = 50100412,				//请求竞技场匹配返回         

		REQ_ARENA_FIGHT = 10100413,				//请求竞技场战斗         

		RES_ARENA_FIGHT = 50100413,				//请求竞技场战斗返回         

		REQ_ARENA_MATCH_REFRESH = 10100414,				//请求竞技场刷新匹配         

		RES_ARENA_MATCH_REFRESH = 50100414,				//请求竞技场刷新匹配返回         

		REQ_OPEN_ARENA_ROLE_PANEL = 10100415,				//竞技场查看人物面板         

		RES_OPEN_ARENA_ROLE_PANEL = 50100415,				//竞技场查看人物面板返回         

		RES_ARENA_PANEL_INFO = 50100408,				//请求竞技场面板信息返回         

		REQ_ARENA_PANEL_INFO = 10100408,				//请求竞技场面板信息         

		REQ_ARENA_WORSHIP = 10100416,				//竞技场膜拜         

		RES_ARENA_WORSHIP = 50100416,				//竞技场膜拜返回         

		REQ_REFRESH_ARENA_PANEL = 10100417,				//竞技场打开面板后刷新         

		RES_REFRESH_ARENA_PANEL = 50100417,				//竞技场打开面板后刷新返回         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ARENA_H_ENUM_ */
