/*  
* Generate by devtool
*/

#ifndef _MSG_PACKER_H_ENUM_
#define _MSG_PACKER_H_ENUM_

enum Msg_Packer_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_PACKAGE_INFO = 10100100,				//获取背包信息         

		RES_FETCH_PACKAGE_INFO = 50100100,				//获取背包信息(返回)         

		REQ_ADD_CAPACITY = 10100101,				//增加容量         

		RES_ADD_CAPACITY = 50100101,				//增加容量(返回)         

		REQ_DISCARD_ITEM = 10100102,				//丢弃物品         

		RES_DISCARD_ITEM = 50100102,				//丢弃物品(返回)         

		REQ_MOVE_ITEM = 10100103,				//移动物品         

		RES_MOVE_ITEM = 50100103,				//移动物品(返回)         

		REQ_SPLIT_ITEM = 10100104,				//拆分物品         

		RES_SPLIT_ITEM = 50100104,				//拆分物品(返回)         

		REQ_SORT_ITEM = 10100105,				//整理物品         

		RES_SORT_ITEM = 50100105,				//整理物品(返回)         

		REQ_USE_SPECINDEX_ITEM = 10100106,				//使用指定位置的物品         

		RES_USE_SPECINDEX_ITEM = 50100106,				//使用指定位置的物品(返回)         

		REQ_USE_SPECIAL_GIFT = 10100107,				//使用指定型礼包         

		RES_USE_SPECIAL_GIFT = 50100107,				//使用指定型礼包(返回)         

		REQ_MERGE_ITEM = 10100108,				//请求合并物品         

		RES_MERGE_ITEM = 50100108,				//请求合并物品(返回)         

		REQ_SELL_ITEM = 10100109,				//请求出售物品         

		RES_SELL_ITEM = 50100109,				//请求出售物品(返回)         

		REQ_MOVE_ITEM_FROM_TOWER = 10100110,				//取出爬塔副本仓库         

		RES_MOVE_ITEM_FROM_TOWER = 50100110,				//取出爬塔副本仓库(返回)         

		REQ_MOVE_ITEM_FROM_PACKAGE = 10100111,				//从任意仓库取出物品         

		RES_MOVE_ITEM_FROM_PACKAGE = 50100111,				//从任意仓库取出物品(返回)         

		REQ_FETCH_PACKAGE_INFO_OTHER = 10100112,				//获取指定角色背包信息         

		RES_FETCH_PACKAGE_INFO_OTHER = 50100112,				//获取指定角色背包信息(返回)         

		REQ_BUY_AND_USE = 10100113,				//购买并使用         

		RES_BUY_AND_USE = 50100113,				//购买并使用(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_PACKER_H_ENUM_ */
