/*  
* Generate by devtool
*/

#ifndef _MSG_EXCHANGE_H_ENUM_
#define _MSG_EXCHANGE_H_ENUM_

enum Msg_Exchange_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_ITEM_EXCHANGE = 10100500,				//物品兑换         

		REQ_FETCH_EXCHANGE_ITEM_INFO = 10100501,				//获取兑换物品信息         

		RES_FETCH_EXCHANGE_ITEM_INFO = 50100501,				//获取兑换物品信息(返回)         

		RES_ITEM_EXCHANGE = 50100500,				//物品兑换(返回)         

		REQ_EXCHANGE_SKILL_ITEM = 10100502,				//技能消耗品兑换         

		RESP_EXCHANGE_SKILL_ITEM = 50100502,				//技能消耗品兑换(返回）         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_EXCHANGE_H_ENUM_ */
