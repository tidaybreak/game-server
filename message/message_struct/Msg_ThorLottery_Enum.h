/*  
* Generate by devtool
*/

#ifndef _MSG_THORLOTTERY_H_ENUM_
#define _MSG_THORLOTTERY_H_ENUM_

enum Msg_ThorLottery_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_GET_SEASON_ID = 10108001,				//打开面板获取季数         

		RESP_GET_SEASON_ID = 50108001,				//打开面板获取季数(返回)         

		REQ_THOR_LOTTERY = 10108002,				//雷神之锤抽奖         

		RESP_THOR_LOTTERY = 50108002,				//雷神之锤抽奖(返回)         

		REQ_THORLOTTERY_EXCHANGE_ITEM = 10108003,				//雷神之锤兑换奖品         

		RESP_THORLOTTERY_EXCHANGE_ITEM = 50108003,				//雷神之锤兑换奖品(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_THORLOTTERY_H_ENUM_ */
