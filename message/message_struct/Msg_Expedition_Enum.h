/*  
* Generate by devtool
*/

#ifndef _MSG_EXPEDITION_H_ENUM_
#define _MSG_EXPEDITION_H_ENUM_

enum Msg_Expedition_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_EXPEDITION_DAILY_AWARD_GET = 10201300,				//请求获取每日奖励         

		REQ_EXPEDITION_DAILY_AWARD_INFO = 10201301,				//请求每日奖励信息         

		RES_EXPEDITION_DAILY_AWARD_INFO = 50201301,				//请求每日奖励信息(返回)         

		REQ_EXPEDITION_OTHER_BUILD_INFO = 10201302,				//请求建筑信息(非公会基地)         

		RES_EXPEDITION_OTHER_BUILD_INFO = 50201302,				//请求建筑信息(非公会基地)(返回)         

		REQ_EXPEDITION_GANG_RANK = 10201303,				//请求公会排名         

		RES_EXPEDITION_GANG_RANK = 50201303,				//请求公会排名(返回)         

		REQ_EXPEDITION_BATTLE_INFO = 10201304,				//请求对战信息         

		RES_EXPEDITION_BATTLE_INFO = 50201304,				//请求对战信息(返回)         

		REQ_EXPEDITION_GANG_BASE_BUILD_INFO = 10201305,				//请求公会基地信息         

		RES_EXPEDITION_GANG_BASE_BUILD_INFO = 50201305,				//请求公会基地信息(返回)         

		REQ_EXPEDITION_FIGHTING = 10201306,				//请求进出战斗队列         

		RES_EXPEDITION_FIGHTING = 50201306,				//请求进出战斗队列(返回)         

		REQ_EXPEDITION_HELP_INFO = 10201307,				//请求援兵信息         

		RES_EXPEDITION_HELP_INFO = 50201307,				//请求援兵信息(返回)         

		REQ_EXPEDITION_CHANGE_LINE = 10201308,				//远征换线         

		RES_EXPEDITION_CHANGE_LINE = 50201308,				//远征换线(返回)         

		REQ_EXPEDITION_CHEST_COLLECT = 10101301,				//远征请求宝箱点领取         

		RES_EXPEDITION_CHEST_COLLECT = 50101301,				//远征请求宝箱点领取(返回)         

		RES_EXPEDITION_DAILY_AWARD_GET = 50201300,				//请求获取每日奖励(返回)         

		REQ_EXPEDITION_OPEN_CHEST = 10101300,				//远征打开宝箱区         

		RES_EXPEDITION_OPEN_CHEST = 50101300,				//远征打开宝箱区(返回)         

		RES_EXPEDITION_LOOKING = 50201310,				//远征进入待战列表(返回)         

		REQ_EXPEDITION_LOOKING = 10201310,				//远征进入待战列表         

		REQ_EXPEDITION_DEMAGE_RANK = 10201309,				//远征伤害排名         

		RES_EXPEDITION_DEMAGE_RANK = 50201309,				//远征伤害排名(返回)         

		REQ_EXPEDITION_OCCUPY_CASTLE = 10201311,				//请求远征攻城榜         

		RES_EXPEDITION_OCCUPY_CASTLE = 50201311,				//请求远征攻城榜(返回)         

		REQ_EXPEDITION_OCCUPY_CASTLE_AWARD_GET = 10201312,				//领取攻城奖励         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_EXPEDITION_H_ENUM_ */
