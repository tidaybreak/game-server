/*  
* Generate by devtool
*/

#ifndef _MSG_WORLD_BOSS_H_ENUM_
#define _MSG_WORLD_BOSS_H_ENUM_

enum Msg_World_Boss_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_WORLD_BOSS_USE_ABILITY = 10211100,				//请求使用特殊技能         

		RES_WORLD_BOSS_USE_ABILITY = 50211100,				//请求使用特殊技能返回         

		REQ_WORLD_BOSS_ACT_GUILD_BLESS = 10211101,				//请求世界boss公会祝福         

		RES_WORLD_BOSS_ACT_GUILD_BLESS = 50211101,				//请求世界boss公会祝福返回         

		REQ_WORLD_BOSS_KILL_INFO = 10111102,				//请求世界boss击杀信息         

		RES_WORLD_BOSS_KILL_INFO = 50111102,				//请求世界boss击杀信息返回         

		REQ_WORLD_BOSS_SET_TIME = 10111103,				//请求设置世界boss开启时间         

		RES_WORLD_BOSS_SET_TIME = 50111103,				//请求设置世界boss开启时间         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_WORLD_BOSS_H_ENUM_ */
