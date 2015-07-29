/*  
* Generate by devtool
*/

#ifndef _MSG_PHP_H_ENUM_
#define _MSG_PHP_H_ENUM_

enum Msg_Php_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		FORBID_ACCOUNT_ROLE = 30100001,				// 账号和角色封禁         

		FORBID_IP = 30100010,				// IP地址封禁或解除         

		RETURN_TO_PHP_MSG = 30100000,				//PHP通知处理结果(返回)         

		MALL_GOODS_CHANGE = 30100002,				//PHP通知商城信息变动         

		ONLINE_RECHARGE = 30100003,				//PHP通知在线充值         

		BACKSTAGE_SEND_MAIL = 30100004,				//PHP通知发送邮件         

		GAG_ACCOUNT_ROLE = 30100005,				//账号和角色禁言         

		REQ_PHP_GET_ACCESS_COUNT = 30100006,				//PHP请求获取统计数据         

		RES_PHP_GET_ACCESS_COUNT = 30500006,				//PHP请求获取统计数据(返回)         

		REQ_PHP_SEND_PROPOSAL_REPLY = 30100007,				//PHP发送你提我改回复         

		PHP_SEND_GM_ANNOUNCE = 30100008,				//PHP通知GM公告         

		REQ_PHP_ADD_ACTIVATION_KEY = 30100011,				//PHP请求增加激活码解码KEY         

		REQ_PHP_FACEBOOK_SHARE_SUCCESS = 30100012,				//PHP通知Facebook分享成功         

		PHP_INNER_ONLINE_RECHARGE = 30100009,				//PHP通知内部充值         

		REQ_PHP_NOTICE_ACTIVE_OPEN = 30100013,				//PHP后台通知活动开启         

		REQ_PHP_NOTICE_ACTIVE_CLOSE = 30100014,				//PHP后台通知活动结束         

		PHP_SEND_ACTIVITY_EXCHANGE_MALL_GOODS_CHANGE = 30100015,				//PHP通知活动兑换商店道具变动         

		REQ_PHP_NOTICE_MOUNT_FUSE_TIME = 30100016,				//PHP后台通知坐骑融合时间         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_PHP_H_ENUM_ */
