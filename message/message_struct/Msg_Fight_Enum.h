/*  
* Generate by devtool
*/

#ifndef _MSG_FIGHT_H_ENUM_
#define _MSG_FIGHT_H_ENUM_

enum Msg_Fight_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_CLIENT_MOVE = 10200102,				//移动         

		RES_CLIENT_MOVE = 50200102,				//移动(返回)         

		REQ_BATTLE_READY = 10200105,				//战斗准备完成         

		RES_BATTLE_READY = 50200105,				//战斗准备完成(返回)         

		REQ_USER_SKILL = 10200110,				//使用技能         

		RES_USER_SKILL = 50200110,				//使用技能(返回)         

		REQ_PLAYER_DUEL = 10200150,				//邀请决斗         

		REQ_CLEAR_SKILL = 10100165,				//洗点技能         

		REQ_MOVE_SKILL = 10100166,				//技能位置移动         

		REQ_PLAYER_FIGHT = 10200151,				//请求和怪物战斗         

		REQ_LEARN_SKILL = 10100141,				//学习技能         

		RES_LEARN_SKILL = 50100141,				//学习技能(返回)         

		REQ_SKILLS_LIST = 10100140,				//获取技能列表         

		RES_SKILLS_LIST = 50100140,				//获取技能列表（返回)         

		REQ_CLIENT_HERO_MOVE = 10200103,				//英雄移动         

		RES_CLIENT_HERO_MOVE = 50200103,				//英雄移动（返回）         

		REQ_OPEN_TALENT = 10100142,				//开启指定页，指定天赋         

		REQ_OPEN_TALENT_PAGE = 10100143,				//开启指定天赋         

		REQ_USE_TALENT_PAGE = 10100144,				//使用天赋         

		REQ_FIGHT_PAUSE = 10200111,				//新手期战斗引导暂停         

		RES_FIGHT_PAUSE = 50200111,				//新手期战斗引导暂停（返回）         

		RES_BATTLE_TIME_LEFT = 50200112,				//战斗时间通知         

		RES_PLAYER_BE_DUEL = 50200153,				//被邀请决斗(此消息非龙谷消息，是移动战斗类别的消息)         

		REQ_RES_PLAYER_DUEL = 10200155,				//应答邀请决斗         

		RES_PLAYER_DUEL = 50200155,				//是否接受决斗         

		REQ_TO_FIGHT = 10200156,				//发起挑战_普通场景         

		REQ_FIGHT_GIVE_UP = 10200113,				//战斗认输         

		RESP_TRIGGER_ADDITION_SKILL = 50200114,				//触发追加技         

		RESP_BE_SKILL_FIX_CD_TIME = 50200115,				//战斗开始，通知前端被动技能修正CD         

		RESP_USE_TALENT_PAGE = 50100144,				//使用天赋(返回)         

		REQ_PLAYER_FIGHT_MONSTER = 10200149,				//请求跟怪物战斗（不在场景的怪）         

		REQ_MULTI_BATTLE_GIVE_UP = 10200117,				//多人战斗认输         

		RESP_MULTI_BATTLE_GIVE_UP = 50200117,				//多人战斗认输(返回)         

		RESP_TEAM_BATTLE_GIVE_UP = 50200118,				//多人战斗认输成功返回         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_FIGHT_H_ENUM_ */
