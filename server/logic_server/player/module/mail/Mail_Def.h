/*
 * Mail_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef MAIL_DEF_H_
#define MAIL_DEF_H_

enum MAIL_STATE{
	ALL_MAIL = 0,
	HAVE_READ = 1 ,
	HAVE_NOT_READ = 2,

	MAIL_TYPE_SYS = 0,				// 系统邮件
	MAIL_TYPE_USER = 1,				// 用户邮件
	MAIL_TYPE_PHP = 2,				// PHP邮件

	MAIL_TITLE_MAX_LEN = 128,		// 标题最大长度
	MAIL_CONTENT_MAX_LEN = 1024,		// 内容最大长度

	MAIL_ITEM_MAX_NUM = 280, //邮件道具最大数量
};


#endif /* MAIL_DEF_H_ */
