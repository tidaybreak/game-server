/*  
* Generate by devtool
*/

#ifndef _MSG_DRAGON_VALE_H_ENUM_
#define _MSG_DRAGON_VALE_H_ENUM_

enum Msg_Dragon_Vale_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_CREATE_NEW_BUILDING = 10180000,				//请求建造新的建筑         

		RES_CREATE_NEW_BUILDING = 50180000,				//请求建造新的建筑(返回)         

		REQ_UPGRADE_BUILDING = 10180001,				//请求升级建筑         

		RES_UPGRADE_BUILDING = 50180001,				//请求升级建筑（返回）         

		REQ_QUICK_FINISH_BUILDING = 10180002,				//请求加速建造或者升级建筑         

		REQ_CANCEL_FINISH_BUILDING = 10180003,				//请求取消建造或者升级建筑         

		RES_CANCEL_FINISH_BUILDING = 50180003,				//请求取消建造或者升级建筑(返回)         

		REQ_GAIN_PRODUCTION = 10180004,				//请求收获资源         

		RES_GAIN_PRODUCTION = 50180004,				//请求收获资源(返回)         

		REQ_STEP_FOOT = 10180005,				//请求踩脚印         

		RES_STEP_FOOT = 50180005,				//请求踩脚印(返回)         

		REQ_ADD_BUILDING_QUEUE = 10180006,				//请求增加龙谷建筑队列         

		RES_ADD_BUILDING_QUEUE = 50180006,				//请求增加建筑队列(返回)         

		REQ_DRAGON_VALE_FIENDLIST = 10180007,				//请求获取有龙谷的好友列表         

		REQ_DRANON_VALE_GANG_MEMBER_LIST = 10180008,				//请求获取有龙谷的同一个公会成员列表         

		RES_DRAGON_VALE_FIENDLIST = 50180007,				//请求获取有龙谷的好友列表(返回)         

		RES_DRANON_VALE_GANG_MEMBER_LIST = 50180008,				//请求获取有龙谷的同一个公会成员列表(返回)         

		REQ_DRAGON_BORN = 10180009,				//请求孵化龙蛋         

		RES_DRAGON_BORN = 50180009,				//请求孵化龙蛋(返回)         

		REQ_PLAY_DRAGON = 10180010,				//请求逗龙         

		RES_PLAY_DRAGON = 50180010,				//请求逗龙(返回)         

		REQ_GET_DRAGON_TOTAL_INFO = 10180011,				//请求获取龙的总信息(培养、进化左侧面板)         

		RES_GET_DRAGON_TOTAL_INFO = 50180011,				//请求获取龙的总信息(返回)         

		REQ_KEEP_DRAGON = 10180012,				//请求培养龙         

		RES_KEEP_DRAGON = 50180012,				//请求培养龙(返回)         

		REQ_ADD_DRAGON_FEELING = 10180013,				//请求增加1点龙心情值         

		RES_ADD_DRAGON_FEELING = 50180013,				//请求增加1点龙心情值(返回)         

		REQ_EVOLUTE_DRAGON = 10180014,				//请求进化龙         

		RES_EVOLUTE_DRAGON = 50180014,				//请求进化龙(返回)         

		REQ_ROB_OTHER_DRAGON_VALE = 10180015,				//请求掠夺别人龙谷         

		RES_ROB_OTHER_DRAGON_VALE = 50180015,				//请求掠夺别人龙谷（返回）         

		REQ_ROB_OTHER_DRAGON_VALE_MATCH = 10180016,				//请求掠夺匹配         

		RES_ROB_OTHER_DRAGON_VALE_MATCH = 50180016,				//请求掠夺别人龙谷匹配(返回)         

		REQ_ATTCK_ROB_MATCH_MEMBER = 10200152,				//请求进攻掠夺龙谷匹配到的人(可能在线、也可能不在线)         

		REQ_ACCEPT_FIGHT_WITH_ROBBER = 10200153,				//龙谷主人同意应战来掠夺的人         

		REQ_GET_NOTICE = 10180017,				//请求获取布告栏         

		RES_GET_NOTICE = 50180017,				//请求获取布告栏(返回)         

		REQ_GAIN_ROB_PRODUCTION_TO_LOGIC = 10180018,				//请求收获掠夺资源         

		RES_GAIN_ROB_PRODUCTION_TO_LOGIC_RES = 50180018,				//请求收获掠夺资源(返回)         

		REQ_DRIVE_DRAGON_VALE_THIEF_SCENE = 10200154,				//请求驱逐沙鼠偷猎者         

		RES_DRIVE_DRAGON_VALE_THIEF_SCENE = 50200154,				//请求驱逐沙鼠偷猎者(返回)         

		REQ_ROB_OTHER_DRAGON_VALE_TASK_LISTEN = 10180019,				//掠夺战斗胜利，但没有掠夺资源，通知逻辑服任务监听         

		REQ_BATCH_EVOLUTE_DRAGON = 10180020,				//请求批量进化龙         

		RES_BATCH_EVOLUTE_DRAGON = 50180020,				//请求批量进化龙(返回)         

		REQ_DRAGON_BUILDING_GAIN_PRODUCTION_LEFT_TIME = 10180021,				//请求获取建筑可收获剩余时间         

		RES_DRAGON_BUILDING_GAIN_PRODUCTION_LEFT_TIME = 50180021,				//请求获取建筑可收获剩余时间（返回）         

		REQ_GET_INVITE_FRIEND_STEP_FOOT_LIST = 10180022,				//获取邀请好友踩脚印的好友列表         

		RES_GET_INVITE_FRIEND_STEP_FOOT_LIST = 50180022,				//获取邀请好友踩脚印的好友列表(返回)         

		REQ_INVITE_FRIEND_STEP_FOOT = 10180023,				// 邀请好友踩脚印         

		REQ_INVITE_ALL_FRIEND_STEP_FOOT = 10180024,				//邀请全部好友踩脚印         

		RES_INVITE_FRIEND_STEP_FOOT = 50180023,				//邀请好友踩脚印(返回)         

		RES_INVITE_ALL_FRIEND_STEP_FOOT = 50180024,				//邀请全部好友踩脚印(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_DRAGON_VALE_H_ENUM_ */
