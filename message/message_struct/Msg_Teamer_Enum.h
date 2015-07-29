/*  
* Generate by devtool
*/

#ifndef _MSG_TEAMER_H_ENUM_
#define _MSG_TEAMER_H_ENUM_

enum Msg_Teamer_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_TEAM_PLATFORM = 10100300,				//请求大厅信息         

		RES_TEAM_PLATFORM = 50100300,				//请求大厅信息(返回)         

		REQ_CREATE_TEAM = 10100301,				//创建房间         

		RES_CREATE_TEAM = 50100301,				//创建房间(返回)         

		REQ_INVITE_ROLE = 10100302,				//邀请加入房间         

		RES_INVITE_ROLE = 50100302,				//邀请加入房间(返回)         

		REQ_APPLY_TEAM = 10100303,				//申请加入房间         

		RES_APPLY_TEAM = 50100303,				//申请加入房间(返回)         

		REQ_MAKE_LEADER = 10100304,				//设为队长         

		RES_MAKE_LEADER = 50100304,				//设为队长         

		REQ_KICK_ROLE = 10100305,				//踢出队伍         

		RES_KICK_ROLE = 50100305,				//踢出队伍(返回)         

		REQ_LEAVE_TEAM = 10100306,				//离开队伍         

		RES_LEAVE_TEAM = 50100306,				//离开队伍(返回)         

		REQ_DISMISS_TEAM = 10100307,				//解散队伍         

		RES_DISMISS_TEAM = 50100307,				//解散队伍(返回)         

		REQ_AGREE_TEAM_INVITE = 10100309,				//同意组队邀请         

		RES_AGREE_TEAM_INVITE = 50100309,				//同意组队邀请(返回)         

		REQ_TEAM_HIRING = 10100311,				//请求队伍招人         

		RES_TEAM_HIRING = 50100311,				//请求队伍招人(返回)         

		REQ_TEAM_BE_WAIT = 10100312,				//玩家进入待战区         

		RES_TEAM_BE_WAIT = 50100312,				//玩家进入待战区(返回)         

		RES_TEAM_CANCEL_WAIT = 50100313,				//玩家退出待战区(返回)         

		REQ_TEAM_CANCEL_WAIT = 10100313,				//玩家退出待战区         

		REQ_LISTEN_TEAM_INFO = 10100315,				//监听组队信息         

		RES_LISTEN_TEAM_INFO = 50100315,				//监听组队信息(返回)         

		REQ_REJECT_TEAM_INVITE = 10100316,				//拒绝组队邀请         

		RES_REJECT_TEAM_INVITE = 50100316,				//拒绝组队邀请(返回)         

		REQ_TEAM_SETTING = 10100308,				//请求队伍设置         

		RES_TEAM_SETTING = 50100308,				//请求队伍设置(返回)         

		REQ_CHANGE_TEAM_SETTING = 10100310,				//修改组队设置-个人         

		RES_CHANGE_TEAM_SETTING = 50100310,				//修改组队设置-个人(返回)         

		REQ_APPLY_TEAM_QUICK = 10100317,				//快速加入         

		RES_APPLY_TEAM_QUICK = 50100317,				//快速加入(返回)         

		REQ_TEAM_SET_PASSWORD = 10100318,				//设置房间密码         

		RES_TEAM_SET_PASSWORD = 50100318,				//设置房间密码(返回)         

		RES_TEAM_CANCEL_PASSWORD = 50100319,				//取消房间密码(返回)         

		REQ_TEAM_CANCEL_PASSWORD = 10100319,				//取消房间密码         

		REQ_TEAM_READY = 10100320,				//玩家准备         

		RES_TEAM_READY = 50100320,				//玩家准备         

		REQ_TEAM_WAIT_AREA = 10100321,				//请求等待区信息         

		REQ_TEAM_SET_POSITION_OPEN_OR_CLOSE = 10100322,				//设置队伍位置开启关闭         

		RES_TEAM_SET_POSITION_OPEN_OR_CLOSE = 50100322,				//设置队伍位置开启关闭(返回)         

		REQ_TEAM_SET_INFO = 10100323,				//修改队伍设置         

		RES_TEAM_SET_INFO = 50100323,				//修改队伍设置(返回)         

		REQ_TEAM_LINEUP_INFO = 10100324,				//组队布阵信息         

		RES_TEAM_LINEUP_INFO = 50100324,				//组队布阵信息(返回)         

		REQ_TEAM_LINEUP_SET = 10100325,				//组队布阵设置         

		RES_TEAM_LINEUP_SET = 50100325,				//组队布阵设置(返回)         

		REQ_TEAM_BACK_ROOM = 10100326,				//返回房间         

		RES_TEAM_BACK_ROOM = 50100326,				//返回房间(返回)         

		REQ_TEAM_HERO_DREAM = 10100327,				//请求英雄梦境列表         

		RES_TEAM_HERO_DREAM = 50100327,				//请求英雄梦境列表（返回）         

		REQ_TEAM_FOLLOW = 10100328,				//组队跟随设置         

		RES_TEAM_FOLLOW = 50100328,				//组队跟随设置（返回）         

		REQ_TEAM_SPORTS_TEST_CONNECT = 10100329,				//组队竞技测试         

		RES_TEAM_SPORTS_TEST_CONNECT = 50100329,				//组队竞技测试(返回)         

		REQ_TEAM_INFO = 10100330,				//请求组队信息         

		RES_TEAM_INFO = 50100330,				//请求组队信息(返回)         

		REQ_TEAM_QUICK_INVITE = 10100331,				//快速邀请         

		RES_TEAM_QUICK_INVITE = 50100331,				//快速邀请(返回)         

		REQ_TEAM_RANK_LIST = 10100332,				//获取积分排名         

		RES_TEAM_RANK_LIST = 50100332,				//获取积分排名(返回)         

		REQ_TEAM_SCORE_AWARD = 10100333,				//请求领取组队积分奖励         

		RES_TEAM_SCORE_AWARD = 50100333,				//请求领取组队积分奖励(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_TEAMER_H_ENUM_ */
