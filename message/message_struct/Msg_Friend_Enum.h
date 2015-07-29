/*  
* Generate by devtool
*/

#ifndef _MSG_FRIEND_H_ENUM_
#define _MSG_FRIEND_H_ENUM_

enum Msg_Friend_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_FRIEND_INFO = 10160001,				//获取好友基本信息         

		REQ_FETCH_FRIEND_LIST = 10160000,				//获取好友列表         

		REQ_ADD_FRIEND = 10160004,				//添加好友         

		RES_FETCH_FRIEND_LIST = 50160000,				//获取好友列表返回         

		RES_FETCH_FRIEND_INFO = 50160001,				//获取好友基本信息返回         

		REQ_FETCH_FRIEND_APPLY_LIST = 10160002,				//获取好友申请列表         

		RES_FETCH_FRIEND_APPLY_LIST = 50160002,				//获取好友申请列表返回         

		RES_ADD_FRIEND = 50160004,				//添加好友返回         

		REQ_DELET_FRIEND = 10160003,				//删除好友         

		RES_DELETE_FRIEND = 50160003,				//删除好友         

		REQ_PUSH_BLACK = 10160005,				//拉进黑名单         

		RES_PUSH_BLACK = 50160005,				//拉进黑名单返回         

		REQ_HANDEL_FRIEND_APPLY = 10160009,				//处理好友申请         

		REQ_FETCH_ROLE_ONLINE = 10160010,				//获取角色在线状态         

		RES_FETCH_ROLE_ONLINE = 50160010,				//获取角色在线状态(返回)         

		RES_FRIEND_SET_PHOTO = 50160011,				//获取设置头像列表(返回)         

		REQ_FRIEND_SET_PHOTO = 10160011,				//获取设置头像列表         

		REQ_SET_HEAD_PHOTO = 10160012,				//请求设置头像         

		RES_SET_HEAD_PHOTO = 50160012,				//请求设置头像(返回)         

		REQ_MULTI_ADD_FRIEND = 10160013,				//批量添加好友         

		RES_MULTI_ADD_FRIEND = 50160013,				//批量添加好友(返回)         

		REQ_FRIEND_BASE_INFO_LIST = 10160014,				//获取好友基础信息列表         

		RES_FRIEND_BASE_INFO_LIST = 50160014,				//获取好友基础信息列表(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_FRIEND_H_ENUM_ */
