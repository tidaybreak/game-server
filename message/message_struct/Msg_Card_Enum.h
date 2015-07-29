/*  
* Generate by devtool
*/

#ifndef _MSG_CARD_H_ENUM_
#define _MSG_CARD_H_ENUM_

enum Msg_Card_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_EMBED_CARD = 10109002,				//卡牌镶嵌         

		RESP_EMBED_CARD = 50109002,				//卡牌镶嵌(返回）         

		REQ_SWALLOW_CARD = 10109003,				//卡牌升级         

		RESP_SWALLOW_CARD = 50109003,				//卡牌升级（返回）         

		RESP_UPDATE_CARD_INFO = 50109004,				//更新卡牌信息         

		REQ_SELL_CARD = 10109005,				//出售卡牌         

		RESP_SELL_CARD = 50109005,				//出售卡牌(返回）         

		REQ_EQUIP_OPEN_CARD_HOLE = 10109006,				//卡牌打孔         

		RESP_EQUIP_OPEN_CARD_HOLE = 50109006,				//卡牌打孔(返回)         

		REQ_MERGE_CARD = 10109007,				//合成卡牌         

		RESP_MERGE_CARD = 50109007,				//合成卡牌(返回)         

		REQ_GET_VIP_UPGRADE_TIMES = 10109008,				//获取vip升级卡牌次数         

		RESP_GET_VIP_UPGRADE_TIMES = 50109008,				//获取vip升级卡牌次数(返回）         

		REQ_GET_VIP_UPGRADE_CARD = 10109009,				//vip升级卡牌         

		RESP_GET_VIP_UPGRADE_CARD = 50109009,				//vip升级卡牌(返回）         

		REQ_CARD_POTEN_UPGRADE = 10109010,				//卡牌升金         

		RESP_CARD_POTEN_UPGRADE = 50109010,				//卡牌升金(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_CARD_H_ENUM_ */
