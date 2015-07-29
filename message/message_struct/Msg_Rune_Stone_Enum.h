/*  
* Generate by devtool
*/

#ifndef _MSG_RUNE_STONE_H_ENUM_
#define _MSG_RUNE_STONE_H_ENUM_

enum Msg_Rune_Stone_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_RUNE_STONE_SYST_INFO = 10103200,				//请求符石系统信息         

		RES_RUNE_STONE_SYST_INFO = 50103200,				//请求符石系统信息（返回）         

		REQ_RUNE_STONE_SMELT = 10103201,				//符石熔炼         

		RES_RUNE_STONE_SMELT = 50103201,				//符石熔炼（返回）         

		REQ_RUNE_STONE_SELL = 10103202,				//符石出售         

		RES_RUNE_STONE_SELL = 50103202,				//符石出售（返回）         

		REQ_RUNE_STONE_PICK = 10103203,				//符石拾取         

		RES_RUNE_STONE_PICK = 50103203,				//符石拾取（返回）         

		REQ_RUNE_STONE_LOCK = 10103204,				//符石锁定         

		RES_RUNE_STONE_LOCK = 50103204,				//符石锁定（返回）         

		REQ_RUNE_STONE_MERGE = 10103205,				//符石合并         

		RES_RUNE_STONE_MERGE = 50103205,				//符石合并（返回）         

		REQ_RUNE_STONE_MOVE = 10103206,				//符石移动(背包、仓库间移动，不包括拾取、穿脱)         

		RES_RUNE_STONE_MOVE = 50103206,				//符石移动(身上、背包、仓库间移动，不包括拾取)（返回）         

		REQ_RUNE_STONE_EXCHANGE = 10103207,				//符石兑换         

		RES_RUNE_STONE_EXCHANGE = 50103207,				//符石兑换(返回)         

		REQ_POINTS = 10103208,				//请求积分         

		REQ_RUNE_STONE_ON_OFF = 10103209,				//符石穿脱         

		RES_RUNE_STONE_ON_OFF = 50103209,				//符石穿脱(返回)         

		REQ_ESSENCE = 10103210,				//请求精华         

		REQ_RUNE_STONE_ESSENCE_EXCHANGE = 10103211,				//符石精华兑换         

		RES_RUNE_STONE_ESSENCE_EXCHANGE = 50103211,				//符石精华兑换(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_RUNE_STONE_H_ENUM_ */
