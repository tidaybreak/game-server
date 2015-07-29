/*
 * Chater_Def.h
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#ifndef CHATER_DEF_H_
#define CHATER_DEF_H_

#include "Time_Value.h"

enum Chat_Type{
	chat_world   		= 1,
	chat_guild  		= 2,
	chat_team    		= 3,
	chat_system 		= 4,
	chat_horn			= 5,
	chat_player			= 6,
	chat_scene			= 7,
};

enum Read_Chat_Record_Type{
	READ_OFFLINE		= 1,
	READ_HISTORY		= 2
};
const Time_Value Chat_CD_in_World(3);


enum Chat_Check_Comm_Cfg_Type {
	CHAT_CHECK_COMM_CONTENTS_LEN = 10001,//检测内容缓冲大小
	CHAT_CHECK_COMM_GAP_COUNT = 10002,//缓冲大小中,大于多少被列入封禁
	CHAT_CHECK_COMM_GAP_TIME = 10003,//检测单用户时间内聊天频率(秒)单位时间
	CHAT_CHECK_COMM_GAP_NUM = 10004,//单位时间内聊天大于此数量,列为黑名单用户
	CHAT_CHECK_COMM_CHR_LEN = 10005,//检测聊天内容最大起始长度(含等于)
};

#endif /* CHATER_DEF_H_ */
