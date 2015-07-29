/*
 * Auction_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: xiaoliang
 */

#ifndef AUCTION_DEF_H_
#define AUCTION_DEF_H_

enum Auction_Send_Mail_Id {
	AUCTION_SEND_UNDER_GOODS_ISSUER_MAIL = 1101,//商品人为操作下架,卖家(发布人)邮件
	AUCTION_SEND_UNDER_GOODS_AUCTION_MAIL = 1102,//发送商品人为操作下架,竞拍方邮件
	AUCTION_SEND_SUCCESS_AUCTION_GOODS_ISSUE_MAIL = 1103,//成功拍卖商品(时间到或一口价);卖家(发布人)邮件
	AUCTION_SEND_SUCCESS_AUCTION_GOODS_AUCTION_MAIL = 1104,//成功拍卖商品(时间到类型);竞拍方邮件
	AUCTION_SEND_SUCCESS_AUCTION_GOODS_ONER_MAIL = 1104,//成功拍卖商品(一口价类型);一口价方邮件
	AUCTION_SEND_SUCCESS_ONER_GOODS_ACTIONER_MAIL = 1102, //成功拍卖商品(一口价类型);最后竞拍方邮件
	AUCTION_SEND_AUCTION_FAIL_ACTIONER_MAIL = 1102,//竞拍失败(竞拍方)邮件; 有人比自己出价高的时候就发
	AUCTION_SEND_AUCTION_FAIL_ISSUE_MAIL = 1105,//竞拍失败(发布人)邮件(时间到类型); 无人竞拍
};

enum MONGO_DATA_TYPE {
	MONGO_DATA_UPDATE = 1,
	MONGO_DATA_DELETE = 2
};

enum AUCTION_SUCCESS_TYPE {
	AUCTION_SUCCESS_BY_ONES = 1, //一口价类型
	AUCTION_SUCCESS_BY_TIMEOVER = 2 //时间到类型
};

enum AUCTION_COMMON_ENUM {
	AUCTION_COMMON_FREE_RATE = 20,//卖家(发布人)成功拍卖商品手续费汇率
};

enum AUCTION_COMMON_CONFIG_KEY {
	AUCTION_NUM_LIMIT = 1,//总限制寄售数量
};

#endif /* AUCTION_DEF_H_ */
