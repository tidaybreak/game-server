/*  
* Generate by devtool
*/

#ifndef _MSG_MALL_H_ENUM_
#define _MSG_MALL_H_ENUM_

enum Msg_Mall_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_MALL_LIST = 10151000,				//获取商品列表         

		RES_FETCH_MALL_LIST = 50151000,				//返回商品列表         

		REQ_BUY_GOODS_IN_MALL = 10151001,				//购买商品         

		RES_BUY_GOODS_IN_MALL = 50151001,				//购买商品(返回)         

		REQ_ONE_KEY_BUY_GOODS_IN_MALL = 10151002,				//一键购买商品         

		RES_ONE_KEY_BUY_GOODS_IN_MALL = 50151002,				//一键购买商品(返回)         

		REQ_FAST_BUY_ITEM = 10151003,				//请求快速购买物品         

		RES_FAST_BUY_ITEM = 50151003,				//请求快速购买物品(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_MALL_H_ENUM_ */
