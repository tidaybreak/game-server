/*  
* Generate by devtool
*/

#ifndef _MSG_EQUIP_H_ENUM_
#define _MSG_EQUIP_H_ENUM_

enum Msg_Equip_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_EQUIP_ON_OFF = 10101000,				//装备穿脱         

		RES_EQUIP_ON_OFF = 50101000,				//装备穿脱(返回)         

		REQ_EQUIP_UNBAN = 10101001,				//装备解封         

		RES_EQUIP_UNBAN = 50101001,				//装备解封(返回)         

		REQ_EQUIP_INCREASE = 10101002,				//装备精通         

		RES_EQUIP_INCREASE = 50101002,				//装备精通(返回)         

		REQ_EQUIP_LEVEL_UP = 10101003,				//装备合成         

		RES_EQUIP_LEVEL_UP = 50101003,				//装备合成(返回)         

		REQ_EQUIP_MASTER_INFO = 10101004,				//获取精通信息         

		RES_EQUIP_MASTER_INFO = 50101004,				//获取精通信息(返回)         

		REQ_EQUIP_ENCHANT = 10101005,				//装备附魔         

		REQ_EQUIP_ENCHANT_REPLACE_PROP_GROUP = 10101006,				//装备附魔属性组替换         

		RES_EQUIP_ENCHANT_REPLACE_PROP_GROUP = 50101006,				//装备附魔属性组替换（返回）         

		RES_EQUIP_ENCHANT = 50101005,				//装备附魔（返回）         

		REQ_EMBED_GEMSTONE = 10101007,				//镶嵌宝石         

		RESP_EMBED_GEMSTONE = 50101007,				//镶嵌宝石(返回)         

		REQ_UMEMBED_GEMSTONE = 10101008,				//卸装宝石         

		RESP_UMEMBED_GEMSTONE = 50101008,				//卸装宝石(返回)         

		REQ_OPEN_GEMSTONE_HOLE = 10101009,				//装备开宝石孔         

		RESP_OPEN_GEMSTONE_HOLE = 50101009,				//装备开宝石孔         

		REQ_GEMSTONE_CONVERT = 10101010,				//宝石转化         

		RESP_GEMSTONE_CONVERT = 50101010,				//宝石转化         

		REQ_GEM_ENABLE_INFO = 10101011,				//宝石信息合成         

		RES_GEM_ENABLE_INFO = 50101011,				//宝石信息合成(返回)         

		REQ_EQUIP_BREAKDOWN = 10101012,				//装备分解         

		RESP_EQUIP_BREAKDOWN = 50101012,				//装备分解(返回)         

		REQ_EQUIP_BREAKDOWN_GET_PROPS = 10101013,				//装备分解获得道具         

		RESP_EQUIP_BREAKDOWN_GET_PROPS = 50101013,				//装备分解获得道具（返回）         

		REQ_GEM_CARVE = 10101014,				//请求宝石雕刻面板         

		RES_GEM_CARVE = 50101014,				//请求宝石雕刻面板（返回）         

		REQ_GEM_CARVE_LVL = 10101015,				//请求宝石雕刻等级         

		RES_GEM_CARVE_LVL = 50101015,				//请求宝石雕刻等级(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_EQUIP_H_ENUM_ */
