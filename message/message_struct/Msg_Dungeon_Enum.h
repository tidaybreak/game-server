/*  
* Generate by devtool
*/

#ifndef _MSG_DUNGEON_H_ENUM_
#define _MSG_DUNGEON_H_ENUM_

enum Msg_Dungeon_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_PASTED_SGL_DUNGEON = 10100200,				//请求已通关单人副本         

		RES_PASTED_SGL_DUNGEON = 50100200,				//请求已通关单人副本(返回)         

		RET_SINGLE_DUNGEON_REWARD = 50100203,				//返回副本通关结算         

		REQ_OPEN_CARD = 10100201,				//请求翻牌         

		RES_OPEN_CARD = 50100201,				//请求翻牌(返回)         

		REQ_FETCH_ELITE_DUNGEON_INFO = 10100202,				//请求精英副本信息         

		RES_FETCH_ELITE_DUNGEON_INFO = 50100202,				//请求精英副本信息返回         

		REQ_RESET_ELITE_DUNGEON_TIMES = 10100204,				//请求重置精英副本         

		RES_RESET_ELITE_DUNGEON_TIMES = 50100204,				//请求重置精英副本返回         

		REQ_RECEIVE_SINGLE_DUNGEON_AWARD = 10100205,				//请求领取单人副本章节奖励         

		RES_RECEIVE_SINGLE_DUNGEON_AWARD = 50100205,				//请求领取单人副本章节奖励返回         

		REQ_DUNGEON_PANEL_CLOSE = 10200200,				//副本结算面板关闭通知         

		REQ_WIPE_OUT_SINGLE_DUNGEON = 10100210,				//请求扫荡单人副本         

		RES_WIPE_OUT_SINGLE_DUNGEON = 50100210,				//请求扫荡单人副本返回         

		REQ_INTERRUP_WIPE_OUT_SINGLE_DUNGEON = 10100211,				//请求中断扫荡单人副本         

		RES_INTERRUP_WIPE_OUT_SINGLE_DUNGEON = 50100211,				//请求中断扫荡单人副本返回         

		REQ_COMPLETE_WIPE_OUT_SINGLE_DUNGEON = 10100212,				//请求完成扫荡单人副本         

		RES_COMPLETE_WIPE_OUT_SINGLE_DUNGEON = 50100212,				//请求完成扫荡单人副本返回         

		REQ_OBTAIN_WIPE_DOUBLE_INCOME = 10100213,				//请求获得双倍扫荡收益         

		RES_OBTAIN_WIPE_DOUBLE_INCOME = 50100213,				//请求获得双倍扫荡收益(返回)         

		REQ_SELECT_WIPE_DOUBLE = 10100214,				//请求勾选双倍扫荡         

		RES_SELECT_WIPE_DOUBLE = 50100214,				//请求勾选双倍扫荡(返回)         

		REQ_BUY_ELITE_ADD_NUM = 10100206,				//请求购买上古副本额外次数         

		RES_BUY_ELITE_ADD_NUM = 50100206,				//请求购买上古副本额外次数返回         

		REQ_ASK_ELITE_TRAN = 10100207,				//请求上古副本传送         

		REQ_SINGLE_DUNGEON_REVIVE = 10200215,				//请求单人副本复活         

		RES_SINGLE_DUNGEON_REVIVE = 50200215,				//请求单人副本复活返回         

		REQ_RECEIVE_ELITE_FIRST_PASS = 10100208,				//请求领取上古魔神首通奖励         

		RES_RECEIVE_ELITE_FIRST_PASS = 50100208,				//请求领取上古魔神首通奖励返回         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_DUNGEON_H_ENUM_ */
