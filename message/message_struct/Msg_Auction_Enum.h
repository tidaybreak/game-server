/*  
* Generate by devtool
*/

#ifndef _MSG_AUCTION_H_ENUM_
#define _MSG_AUCTION_H_ENUM_

enum Msg_Auction_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_AUCTION_GOODS_LIST = 10107000,				//请求拍卖行商品列表         

		RES_AUCTION_GOODS_LIST = 50107000,				//请求拍卖行商品列表(返回)         

		REQ_ADD_AUCTION_GOODS = 10107001,				//请求增加拍卖商品         

		RES_ADD_AUCTION_GOODS = 50107001,				//请求增加拍卖商品(返回)         

		REQ_UNDER_AUCTION_GOODS = 10107002,				//请求下架竞拍商品         

		RES_UNDER_AUCTION_GOODS = 50107002,				//请求下架竞拍商品(返回)         

		REQ_ONES_PRICE_BUY_AUCTION_GOODS = 10107003,				//请求一口价购买商品         

		RES_ONES_PRICE_BUY_AUCTION_GOODS = 50107003,				//请求一口价购买商品(返回)         

		REQ_UPDATE_AUCTION_GOODS = 10107004,				//请求出价竞拍商品         

		RES_UPDATE_AUCTION_GOODS = 50107004,				//请求出价竞拍商品(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_AUCTION_H_ENUM_ */
