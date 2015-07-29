/*  
* Generate by devtool
*/

#ifndef _MSG_COVENANT_H_ENUM_
#define _MSG_COVENANT_H_ENUM_

enum Msg_Covenant_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_BUY_COVENANT = 10106100,				//请求开通或续费精灵盟约         

		RES_BUY_COVENANT = 50106100,				//请求开通或续费精灵盟约(返回)         

		REQ_HANDSEL_COVENANT = 10106101,				//请求赠送灵精盟约         

		RES_HANDSEL_COVENANT = 50106101,				//请求赠送灵精盟约(返回)         

		REQ_START_COVENANT_BECOME_TIME = 10106102,				//请求开始每天倒计时盟约         

		RES_START_COVENANT_BECOME_TIME = 50106102,				//请求开始每天倒计时盟约(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_COVENANT_H_ENUM_ */
