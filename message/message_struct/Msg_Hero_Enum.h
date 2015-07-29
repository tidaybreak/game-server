/*  
* Generate by devtool
*/

#ifndef _MSG_HERO_H_ENUM_
#define _MSG_HERO_H_ENUM_

enum Msg_Hero_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_HEROER_INFO = 10102000,				//获取玩家英雄信息         

		RES_FETCH_HEROER_INFO = 50102000,				//获取玩家英雄信息(返回)         

		REQ_HERO_LEVEL_UP = 10102001,				//英雄升级         

		RES_HEROER_LEVEL_UP = 50102001,				//英雄升级（返回）         

		REQ_HERO_EQUIP_ON_OFF = 10102002,				//英雄装备穿脱         

		RES_HERO_EQUIP_ON_OFF = 50102002,				//英雄装备穿脱(返回)         

		REQ_HERO_OUT = 10102003,				//英雄跟随         

		RES_HERO_OUT = 50102003,				//英雄跟随（返回）         

		REQ_HERO_RECRUIT = 10102004,				//英雄招募         

		RES_HERO_RECRUIT = 50102004,				//英雄招募(返回)         

		REQ_HERO_SET_FORMATION = 10102005,				//英雄设置阵型         

		RES_HERO_SET_FORMATION = 50102005,				//英雄设置阵型（返回）         

		REQ_HERO_SET_FIGHT = 10102006,				//设置战斗英雄         

		RES_HERO_SET_FIGHT = 50102006,				//设置战斗英雄（返回）         

		REQ_HERO_AWAKE = 10102007,				//英雄觉醒         

		RES_HERO_AWAKE = 50102007,				//英雄觉醒（返回）         

		REQ_ARTIFACT_UPGRADE = 10102020,				//英雄神器升级         

		RES_ARTIFACT_UPGRADE = 50102020,				//英雄神器升级（返回）         

		REQ_NEW_HERO_CLICK = 10102021,				//点击新英雄去掉new         

		RES_NEW_HERO_CLICK = 50102021,				//点击新英雄去掉new（返回）         

		REQ_FETCH_CLICKED_NEW_HEROS = 10102022,				//获取新英雄信息         

		RES_FETCH_CLICKED_NEW_HEROS = 50102022,				//获取点击过的新英雄信息（返回）         

		REQ_HERO_ASSIST = 10102032,				//邀请英雄助阵         

		RESP_HERO_ASSIST = 50102032,				//邀请英雄助阵（返回）         

		REQ_HERO_INTERACTIVE = 10102033,				//助阵英雄互动         

		RESP_HERO_INTERACTIVE = 50102033,				//助阵英雄互动（返回）         

		REQ_GIFT_FOR_ASSIST_HERO = 10102034,				//赠送助阵英雄礼品         

		RESP_GIFT_FOR_ASSIST_HERO = 50102034,				//赠送助阵英雄礼品(返回）         

		RESP_UPDATE_ASSIST_HERO_INFO = 50102031,				//更新单个英雄助阵信息(返回）         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_HERO_H_ENUM_ */
