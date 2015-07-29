/*  
* Generate by devtool
*/

#ifndef _MSG_CHAT_H_ENUM_
#define _MSG_CHAT_H_ENUM_

enum Msg_Chat_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_LOCAL_CHAT = 10300000,				//本服聊天请求         

		REQ_HISTORY_CHAT = 10300001,				//私聊聊天记录请求         

		RES_HISTORY_CHAT = 50300001,				//聊天记录请求（返回）         

		REQ_HISTORY_GANG_CHAT = 10300002,				//公会聊天记录请求         

		REQ_INPUT_STATUS = 10300003,				//发起输入状态         

		RES_INPUT_STATUS = 50300003,				//推送输入状态(返回)         

		REQ_ON_SCENE_CHAT = 10200011,				//请求同场景聊天         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_CHAT_H_ENUM_ */
