/*  
* Generate by devtool
*/

#ifndef _MSG_TASK_H_ENUM_
#define _MSG_TASK_H_ENUM_

enum Msg_Task_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_TASK_LIST = 10110000,				//获取任务列表         

		RES_FETCH_TASK_LIST = 50110000,				//获取任务列表(返回)         

		REQ_FETCH_NPC_TASK_LIST = 10110001,				//获取NPC任务(历史原因，遗留，未用到，待删除)         

		RES_FETCH_NPC_TASK_LIST = 50110001,				//获取NPC任务(返回)         

		REQ_TASK_ACCETP = 10110002,				//接受任务         

		RES_TASK_ACCETP = 50110002,				//接受任务(返回)         

		REQ_TASK_ABANDON = 10110003,				//放弃任务         

		RES_TASK_ABANDON = 50110003,				//放弃任务(返回)         

		REQ_TASK_COMPLETE = 10110004,				//交付任务         

		RES_TASK_COMPLETE = 50110004,				//交付任务(返回)         

		REQ_TASK_GET_AWARD = 10110005,				//领取任务奖励         

		RES_TASK_GET_AWARD = 50110005,				//领取任务奖励(返回)         

		REQ_TASK_ACCETP_BATCH = 10110006,				//批量接受任务         

		RES_TASK_ACCETP_BATCH = 50110006,				//批量接受任务(返回)         

		REQ_TASK_QUICK_COMPLETE = 10110007,				//快速完成         

		REQ_TASK_PLOT = 10110008,				//剧情消息         

		RES_TASK_MONSTER = 50110008,				//剧情消息(返回)         

		REQ_TASK_NPC_TALK = 10110009,				//NPC对话任务，完成对话，请求完成任务         

		RES_TASK_NPC_TALK = 50110009,				//NPC对话任务，完成对话，请求完成任务(返回)         

		REQ_TASK_CLEAR_PLOTS = 10110010,				//完成副本清除剧情数据         

		RES_TASK_CLEAR_PLOTS = 50110010,				//完成副本清除剧情数据(返回)         

		REQ_TASK_ADD_NPC = 10110011,				//任务随身怪处理         

		RES_TASK_ADD_NPC = 50110011,				//任务随身怪处理(返回)         

		REQ_REWARD_TASK_LIST = 10110012,				//请求悬赏任务列表         

		RES_REWARD_TASK_LIST = 50110012,				//请求悬赏任务列表(返回)         

		REQ_OPEN_REWARD_TASK_LIST = 10110013,				//请求打开悬赏榜         

		RES_OPEN_REWARD_TASK_LIST = 50110013,				//请求打开悬赏榜（返回）         

		TASK_LISTEN_CHAT_WITH_PLAYER = 10110014,				//任务监听打开私聊窗口         

		REQ_COMMIT_ITEM = 10110015,				//请求提交道具         

		REQ_RECOVER_TASK_CHAIN = 10110016,				//请求恢复任务链         

		RES_RECOVER_TASK_CHAIN = 50110016,				//请求恢复任务链(返回)         

		REQ_SET_FORENOTICE_PLAYED_EFFECT = 10110017,				//设置特效已经播放         

		REQ_SHOW_DAY_TASK_LIST = 10110018,				//请求显示日常任务列表         

		RES_SHOW_DAY_TASK_LIST = 50110018,				//请求显示日常任务列表(返回)         

		REQ_PLAYER_READED_TIMEOUT_TRY_MESSAGE = 10110019,				//玩家已读体验过期消息         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_TASK_H_ENUM_ */
