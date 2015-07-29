/*  
* Generate by devtool
*/

#ifndef _MSG_RUNE_H_ENUM_
#define _MSG_RUNE_H_ENUM_

enum Msg_Rune_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_RUNE_INFO = 10103000,				//获取符文信息         

		RES_FETCH_RUNE_INFO = 50103000,				//获取符文信息(返回)         

		REQ_RUNE_ACTIVATE = 10103001,				//符文激活         

		REQ_RUNE_FILL_ENERGY = 10103002,				//符文充能         

		RES_RUNE_ACTIVATE = 50103001,				//符文激活(返回)         

		RES_RUNE_FILL_ENERGY = 50103002,				//符文充能(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_RUNE_H_ENUM_ */
