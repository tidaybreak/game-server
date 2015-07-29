/*
 * custom_define.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef CUSTOM_DEFINE_H_
#define CUSTOM_DEFINE_H_

#include "server_define.h"

namespace Game_Enum {
	/*
	 * Common Enum
	 * */
	enum COMMON_ENUM {
		CLIENT_MESSAGE_BEGIN 		= 10000000,	//客户端消息号起始
		CLIENT_MESSAGE_END			= 20000000,	//客户端消息号结束
		MOUDLE_MAX_MESSAGE_COUNT	= 100000,	//每个消息块最多消息个数

		DEFAULT_HASH_MAP_SIZE		= 2048,		//哈希默认大小
		DEFAULT_TIME_OUT 			= 10,		//默认超时时间
		DEFAULT_MAX_NAME_LENGTH		= 128,		//名称默认最大长度
		DEFAULT_MAX_CONTENT_LEGNTH	= 512,		//内容默认最大长度
		DEFAULT_TOTAL_PAGE			= 1,		//默认总页数
		DEFAULT_MAX_RAND_TIMES		= 100,		//默认最大随机次数
		DEFAULT_MAX_RT_TIMES	 	= 10,		//默认最大随机方向的次数

		MAX_BEAST_NAME_LEN			= 12,		//宠物名字长度
		MAX_MSG_NAME_LEN			= 32,		//消息名最大长度
	};
}

// 服务器内部消息
enum Inner_Msg {
	INN_SERVER_CLOSE 	  	= 700000,				//关闭
	INN_SERVER_EXIT		    = 700110,				//退出

	INN_RECORD_SERVER_CLOSE = 700002,				//日志服关闭连接关闭

	INN_LOGIC_CLIENT_CLOSE  = 700001,				//玩家主动关闭逻辑服连接
	INN_LOGIC_SCENE_CLOSE 	= 700003,				//场景服关闭连接
	INN_LOGIC_CHAT_CLOSE 	= 700004,				//聊天服关闭连接

	INN_SCENE_CLIENT_CLOSE  = 700101,				//玩家主动关闭场景服连接
	INN_SCENE_LOGIC_CLOSE 	= 700103,				//逻辑服关闭连接
	INN_SCENE_CHAT_CLOSE 	= 700104,				//聊天服关闭连接

	INN_CHAT_CLIENT_CLOSE  = 700201,				//玩家主动关闭聊天服连接
	INN_CHAT_LOGIC_CLOSE 	= 700203,				//逻辑服关闭连接
	INN_CHAT_SCENE_CLOSE 	= 700204,				//场景服关闭连接
	INN_CHAT_PLAYER_CLOSE_BY_CID = 702010,			//聊天服主动关闭玩家连接

	INN_SERVER_HEARTBEAT	= 777777,				//内部心跳包
};

// 定时器消息
enum Time_Handle_Msg {
	TIME_HANDLE_SERVER_HEARTBEAT	= 777777,		// 服务器心跳

	TIME_HANDLE_LOGIC_PLAYER_CLOSE_BY_CID = 700010,			//逻辑服主动关闭玩家连接

	TIME_HANDLE_SCENE_PLAYER_CLOSE_BY_CID = 701010,			//场景服主动关闭玩家连接

};



#endif /* CUSTOM_DEFINE_H_ */
