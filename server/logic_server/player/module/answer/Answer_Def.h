/*
 * Answer_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef ANSWER_DEF_H_
#define ANSWER_DEF_H_

const int ANSWER_INNER_SCENE_ID = 80217001;
const int ANSWER_OFFICE_PLAYER_RANK_MAIL_ID = 1107;

enum ANSWER_COMMON_CFG_KEY {
	ANSWER_COMMON_CFG_KEY_QUESTION_NUM = 1,//活动题目数量(题)
	ANSWER_COMMON_CFG_KEY_MAX_FREE = 2,//玩家最大免答次数(次)
	ANSWER_COMMON_CFG_KEY_MAX_DOUBLE = 3,//玩家最大双倍次数(次)
	ANSWER_COMMON_CFG_KEY_READ_TIME = 4,//阅题时间(秒)
	ANSWER_COMMON_CFG_KEY_ANSWER_TIME = 5,//答题时间(秒)
	ANSWER_COMMON_CFG_KEY_ANN_TIME = 6,//公布答案时间(秒)
	ANSWER_COMMON_CFG_KEY_VIP_FREE = 7,//VIP免答次数
	ANSWER_COMMON_CFG_KEY_VIP_DOUBLE = 8,//VIP双倍次数
	ANSWER_COMMON_CFG_KEY_CENTER_X = 9,//X轴地图中心点
};


enum ANSWER_DATA_CHANNLE_TYPE {
	ANSWER_DATA_CHANNLE_TYPE_LS_OPEN_ACTIVE = 1,//通知活动开启
	ANSWER_DATA_CHANNLE_TYPE_LS_CLOSE_ACTIVE = 2,//通知活动关闭
	ANSWER_DATA_CHANNLE_TYPE_SL_GET_INTEGRAL_REWARD = 3,//通知领取积分奖励
	ANSWER_DATA_CHANNLE_TYPE_SL_GET_RANK_REWARD = 4,//通知发放排名奖励
	ANSWER_DATA_CHANNLE_TYPE_SL_SAVE_DATA = 5,//数据发送到逻辑服务器存库
};

enum ANSWER_TYPE {
	ANSWER_TYPE_A_TRUE = 1,
	ANSWER_TYPE_B_TRUE = 2,
	ANSWER_TYPE_ALL_TRUE = 3,
};

enum ANSWER_ACTIVE_TYPE {
	ACTIVE_TYPE_20003 = 20003,	//(双倍积分)选择人数多的一方是正确的一方 t
	ACTIVE_TYPE_20004 = 20004,	//	(双倍积分)选择人数少的一方是正确的一方 t
	ACTIVE_TYPE_20005 = 20005,	//	(双倍积分)活动开启时,排行榜战力第一的玩家是谁? t
	ACTIVE_TYPE_20006 = 20006,	//	(双倍积分)活动开启时,排行榜战力第二的玩家是谁? t
	ACTIVE_TYPE_20007 = 20007,	//	(双倍积分)活动开启时,排行榜战力第三的玩家是谁? t
	ACTIVE_TYPE_20008 = 20008,	//	(双倍积分)活动开启时,竞技场第一名王者是谁? t
	ACTIVE_TYPE_20009 = 20009,	//	(双倍积分)活动开启时,竞技场第二名王者是谁? t
	ACTIVE_TYPE_20010 = 20010,	//	(双倍积分)活动开启时,竞技场第三名王者是谁? t
	ACTIVE_TYPE_20014 = 20014,	//	(双倍积分)当前服务器中排行第一的公会是? t
	ACTIVE_TYPE_20015 = 20015,	//	(双倍积分)当前服务器中排行第二的公会是? t
	ACTIVE_TYPE_20016 = 20016,	//	(双倍积分)男性角色少的一方是正确的一方 t
	ACTIVE_TYPE_20017 = 20017,	//	(双倍积分)男性角色多的一方是正确的一方 t
	ACTIVE_TYPE_20018 = 20018,	//	(双倍积分)女性角色多的一方是正确的一方 t
	ACTIVE_TYPE_20019 = 20019,	//	(双倍积分)女性角色少的一方是正确的一方 t
	ACTIVE_TYPE_10162 = 20020,	//	(双倍积分)参加本次活动的男性角色多,还是女性角色多? t

};

#endif /* ANSWER_DEF_H_ */
