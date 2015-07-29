/*  
* Generate by devtool
*/

#ifndef _MSG_MAIL_H_ENUM_
#define _MSG_MAIL_H_ENUM_

enum Msg_Mail_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_MAIL_LIST = 10150000,				//获取邮件列表         

		REQ_MAIL_CHECK = 10150001,				//查看邮件         

		REQ_MAIL_SEND = 10150002,				//发送邮件         

		RES_FETCH_MAIL_LIST = 50150000,				//获取邮件列表返回         

		RES_MAIL_CHECK = 50150001,				//查看邮件返回         

		RES_MAIL_SEND = 50150002,				//发送邮件返回         

		REQ_MUTIMAIL_DELET = 10150005,				//批量删除邮件         

		RES_MUTIMAIL_DELET = 50150005,				//批量删除邮件列表返回         

		REQ_PICKUP_ITEM = 10150003,				//请求收取附件         

		RES_PICKUP_ITEM = 50150003,				//请求收取附件返回         

		REQ_PICKUP_MAIL_ALL_ITEM = 10150004,				//请求一键收取附件         

		RES_PICKUP_MAIL_ALL_ITEM = 50150004,				//请求一键收取附件(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_MAIL_H_ENUM_ */
