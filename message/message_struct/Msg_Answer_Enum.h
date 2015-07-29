/*  
* Generate by devtool
*/

#ifndef _MSG_ANSWER_H_ENUM_
#define _MSG_ANSWER_H_ENUM_

enum Msg_Answer_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_USE_ANSWER_FREE = 10209001,				//请求使用免答         

		REQ_USE_ANSWER_DOUBLE = 10209002,				//请求使用双倍         

		RES_USE_ANSWER_FREE = 50209001,				//请求使用免答(返回)         

		RES_USE_ANSWER_DOUBLE = 50209002,				//请求使用双倍(返回)         

		RES_GET_ANSWER_FREE_DOUBLE_NUM = 50209003,				//请求获取免答和双倍次数(返回)         

		REQ_GET_ANSWER_FREE_DOUBLE_NUM = 10209003,				//请求获取免答和双倍次数         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ANSWER_H_ENUM_ */
