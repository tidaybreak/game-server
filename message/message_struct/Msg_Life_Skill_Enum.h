/*  
* Generate by devtool
*/

#ifndef _MSG_LIFE_SKILL_H_ENUM_
#define _MSG_LIFE_SKILL_H_ENUM_

enum Msg_Life_Skill_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_OPEN_LFSKILL_PANEL = 10116000,				//打开生活技能界面         

		RES_OPEN_LFSKILL_PANEL = 50116000,				//打开生活技能界面返回         

		REQ_LEARN_LIFE_SKILL = 10116001,				//学习生活技能         

		RES_LEARN_LIFE_SKILL = 50116001,				//学习生活技能返回         

		REQ_LIFE_SKILL_UPGRADE = 10116002,				//生活技能升级         

		RES_LIFE_SKILL_UPGRADE = 50116002,				//生活技能升级返回         

		REQ_SPAR_REFINE = 10116003,				//晶石精练         

		RES_SPAR_REFINE = 50116003,				//晶石精练返回         

		REQ_CARD_MAKE = 10116004,				//卡牌制作         

		RES_CARD_MAKE = 50116004,				//卡牌制作返回         

		REQ_SPAR_SMELT = 10116005,				//晶石熔炼         

		RES_SPAR_SMELT = 50116005,				//晶石熔炼返回         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_LIFE_SKILL_H_ENUM_ */
