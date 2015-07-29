/*  
* Generate by devtool
*/

#ifndef _MSG_GANG_H_ENUM_
#define _MSG_GANG_H_ENUM_

enum Msg_Gang_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_CREATE_GANG = 10170000,				//创建公会         

		RES_CREATE_GANG = 50170000,				//创建公会（返回）         

		REQ_JOIN_GANG = 10170001,				//加入公会         

		RES_JOIN_GANG = 50170001,				//加入公会（返回）         

		REQ_GANG_INFO = 10170002,				//获取公会信息         

		RES_GANG_INFO = 50170002,				//获取公会信息（返回）         

		REQ_GANG_SELF_INFO = 10170003,				//获取个人公会信息         

		RES_GANG_SELF_INFO = 50170003,				//获取个人公会信息（返回）         

		REQ_GANG_LIST = 10170004,				//获取公会列表         

		RES_GANG_LIST = 50170004,				//获取公会列表（返回）         

		REQ_GANGER_LIST = 10170005,				//获取公会成员列表         

		RES_GANGER_LIST = 50170005,				//获取公会成员列表（返回）         

		REQ_GANG_QUIT = 10170006,				//退出公会         

		RES_GANG_QUIT = 50170006,				//退出公会（返回）         

		REQ_GANG_RESIGN = 10170007,				//转让公会         

		RES_GANG_RESIGN = 50170007,				//转让公会（返回）         

		REQ_GANG_INFO_UPDATE = 10170008,				//监听公会信息更新         

		RES_GANG_INFO_UPDATE = 50170008,				//监听公会信息更新（返回）         

		REQ_GANG_KICK_OUT = 10170009,				//踢出公会         

		RES_GANG_KICK_OUT = 50170009,				//踢出公会（返回）         

		REQ_GANG_SET_ANNOUNCE = 10170010,				//修改公会公告         

		RES_GANG_SET_ANNOUNCE = 50170010,				//修改公会公告（返回）         

		REQ_GANG_INVITE_TO_JOIN = 10170011,				//邀请好友加入公会         

		RES_GANG_INVITE_TO_JOIN = 50170011,				//邀请好友加入公会（返回）         

		REQ_GANG_PASSED_EVENTS = 10170012,				//获取公会动态         

		RES_GANG_PASSED_EVENTS = 50170012,				//获取公会动态（返回）         

		REQ_GANG_SIMPLE_PASSED_EVENT = 10170013,				//获取公会简易动态         

		RES_GANG_SIMPLE_PASSED_EVENT = 50170013,				//获取公会简易动态（返回）         

		REQ_GANG_GROUP_MAIL = 10170014,				//发送公会群邮件         

		RES_GANG_GROUP_MAIL = 50170014,				//发送公会群邮件（返回）         

		REQ_GANG_DONATE = 10170015,				//公会捐献         

		RES_GANG_DONATE = 50170015,				//公会捐献（返回）         

		REQ_GANG_SET_JOIN_BLOCK = 10170016,				//公会加入设置         

		RES_GANG_SET_JOIN_BLOCK = 50170016,				//公会加入设置（返回）         

		REQ_GANG_INVITE_LIST = 10170017,				//获取公会邀请列表         

		RES_GANG_INVITE_LIST = 50170017,				//获取公会邀请列表（返回）         

		REQ_GANG_APPLY_LIST = 10170018,				//获取公会申请列表         

		RES_GANG_APPLY_LIST = 50170018,				//获取公会申请列表（返回）         

		REQ_GANG_LIST_ACTION = 10170019,				//公会列表操作[申请,取消]         

		REQ_GANG_INVITE_ACTION = 10170020,				//邀请列表操作[同意,拒绝]         

		REQ_GANG_APPLY_ACTION = 10170021,				//申请列表操作[同意,拒绝]         

		RES_GANG_LIST_ACTION = 50170019,				//公会列表操作[申请,取消]（返回）         

		RES_GANG_INVITE_ACTION = 50170020,				//邀请列表操作[同意,拒绝]（返回）         

		RES_GANG_APPLY_ACTION = 50170021,				//申请列表操作[同意,拒绝]         

		RES_GANGER_CHANGE_HEADSHIP_AUTH = 50170022,				//改变公会成员职位权限（返回）         

		REQ_GANGER_CHANGE_HEADSHIP_AUTH = 10170022,				//公会成员职位权限改变         

		REQ_GANG_UPGRADE = 10170023,				//公会升级         

		RES_GANG_UPGRADE = 50170023,				//公会升级（返回）         

		REQ_GANG_UPGRADE_SEND_GIFT = 10170024,				//公会升级发礼包         

		RES_GANG_UPGRADE_SEND_GIFT = 50170024,				//公会升级发礼包（返回）         

		REQ_GANG_UPGRADE_CD_CLEAR = 10170025,				//公会清升级CD         

		RES_GANG_UPGRADE_CD_CLEAR = 50170025,				//公会清升级CD（返回）         

		REQ_GANG_WORLD_NOTICE = 10170026,				//公会发世界公告         

		RES_GANG_WORLD_NOTICE = 50170026,				//公会发世界公告（返回）         

		REQ_GANG_LOGO_MODIFY = 10170027,				//修改公会LOGO         

		RES_GANG_LOGO_MODIFY = 50170027,				//修改公会LOGO（返回）         

		REQ_GANG_SEIZE_POWER = 10170028,				//公会夺权         

		RES_GANG_SEIZE_POWER = 50170028,				//公会夺权（返回）         

		REQ_SEND_BLESS_BUF = 10170029,				//请求发送公会祝福         

		RES_SEND_BLESS_BUF = 50170029,				//请求发送公会祝福（返回）         

		REQ_RESEARCH_GANG_SKILL = 10170030,				//请求研究公会技能         

		RES_RESEARCH_GANG_SKILL = 50170030,				//请求研究公会技能（返回）         

		REQ_LEARN_GANGER_SKILL = 10170031,				//请求学习公会技能         

		RES_LEARN_GANGER_SKILL = 50170031,				//请求学习公会技能（返回）         

		REQ_GANG_DRAW = 10170032,				//公会抽奖         

		RES_GANG_DRAW = 50170032,				//公会抽奖（返回）         

		REQ_GANG_TAKE_DRAW_ITEM = 10170033,				//公会领取抽奖物品         

		RES_GANG_TAKE_DRAW_ITEM = 50170033,				//公会领取抽奖物品（返回）         

		REQ_GANG_BUILDING_UPGRADE = 10170034,				//公会建筑升级         

		RES_GANG_BUILDING_UPGRADE = 50170034,				//公会建筑升级（返回）         

		REQ_GANG_WAR_ENTER = 10170035,				//进入公会战         

		RES_GANG_WAR_ENTER = 50170035,				//进入公会战(返回)         

		REQ_GANG_WAR_BOARD = 10170036,				//请求公会战面板信息         

		RES_GANG_WAR_BOARD = 50170036,				//请求公会战面板信息(返回)         

		REQ_GANG_WAR_SPOT = 10170037,				//请求公会战面板结点信息         

		RES_GANG_WAR_SPOT = 50170037,				//请求公会战面板结点信息(返回)         

		REQ_GANG_WAR_ENTER_SCENE = 10270000,				//进入公会战         

		REQ_GANG_WAR_HEAL = 10270001,				//公会战请求治疗         

		REQ_GANG_WAR_AWARD = 10170038,				//请求公会战奖励列表         

		RES_GANG_WAR_AWARD = 50170038,				//请求公会战奖励列表(返回)         

		REQ_GANG_WAR_AWARD_ASSIGN = 10170039,				//请求公会战奖励分配         

		RES_GANG_WAR_AWARD_ASSIGN = 50170039,				//请求公会战奖励分配(返回)         

		REQ_GANG_WAR_DUEL = 10270002,				//公会战请求决斗         

		REQ_GANG_WAR_AGREE_DUEL_S = 10270004,				//同意公会战决斗         

		REQ_GANG_SHOP_BUY_ITEM = 10170041,				//公会商店购买道具         

		RES_GANG_SHOP_BUY_ITEM = 50170041,				//公会商店购买道具（返回）         

		REQ_GANG_WAR_DRAW = 10170042,				//公会战抽签         

		RES_GANG_WAR_DRAW = 50170042,				//公会战抽签（返回）         

		REQ_GANG_WAR_GET_RANK_AWARD = 10170043,				//公会战领取排名奖励         

		RES_GANG_WAR_GET_RANK_AWARD = 50170043,				//公会战领取排名奖励（返回）         

		REQ_GANG_WAR_RANK_AWARD_LIST = 10170044,				//公会战排名奖励列表         

		RES_GANG_WAR_RANK_AWARD_LIST = 50170044,				//公会战排名奖励列表（返回）         

		REQ_GANG_WAR_GANG_RANK = 10170045,				//请求公会战公会排名         

		RES_GANG_WAR_GANG_RANK = 50170045,				//请求公会战公会排名(返回)         

		REQ_PHY_POWER_PRACTICE_START = 10170047,				//开始修行         

		REQ_PHY_POWER_PRACTICE_STOP = 10170048,				//结束修行         

		RESP_PHY_POWER_PRACTICE_START = 50170047,				//开始修行(返回)         

		REQ_SPEED_UP_PRACTICE = 10170049,				//加速修行         

		RESP_SPEED_UP_PRACTICE = 50170049,				//加速修行(返回）         

		RESP_PHY_POWER_PRACTICE_STOP = 50170048,				//停止修行(返回）         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_GANG_H_ENUM_ */
