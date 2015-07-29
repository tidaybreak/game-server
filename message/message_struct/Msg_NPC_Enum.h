/*  
* Generate by devtool
*/

#ifndef _MSG_NPC_H_ENUM_
#define _MSG_NPC_H_ENUM_

enum Msg_NPC_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_DROPS_LIST = 10210000,				//获取掉落详细信息         

		RES_FETCH_DROPS_LIST = 50210000,				//获取掉落详细信息(返回)         

		REQ_GET_DROPS_GOODS = 10210001,				//领取掉落宝箱物品         

		RES_GET_DROPS_GOODS = 50210001,				//领取掉落宝箱物品(返回)         

		REQ_START_GATHER = 10210100,				//开始采集         

		RES_START_GATHER = 50210100,				//开始采集（返回）         

		REQ_STOP_GATHER = 10210101,				//中断采集         

		RES_STOP_GATHER = 50210101,				//中断采集（返回）         

		REQ_CREATE_BATTLE_MONSTER = 10210200,				//战斗中创建怪物         

		REQ_OPEN_MACHINE = 10210103,				//开启机关         

		REQ_TRIGGER_MACHINE = 10210104,				//触发机关通知         

		REQ_PICK_UP_GOODS = 10210002,				//请求拾取物品         

		RES_PICK_UP_GOODS = 50210002,				//请求拾取物品返回         

		REQ_REFRESH_SCENE_POINT = 10210010,				//请求在场景刷怪、采集物、宝箱         

		REQ_FIGHT_SCENE_POINT = 10210011,				//请求跟怪点战斗         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_NPC_H_ENUM_ */
