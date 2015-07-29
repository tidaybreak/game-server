/*
 * Reward_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef REWARD_DEF_H_
#define REWARD_DEF_H_

enum Activation_Code_Op_Type {
	ACTIVATION_CODE_OP_QUERY = 1, //查询激活码
	ACTIVATION_CODE_OP_UPDATE = 2, //更新激活码状态
	ACTIVATION_CODE_RECEIVE_ID = 3, //记录激活码领取ID
};

enum Reward_Back_Receive_Type {
	REWARD_BACK_RECEIVE_COPPER = 1, //金钱追回
	REWARD_BACK_RECEIVE_GOLD = 2,   //钻石追回
	REWARD_BACK_RECEIVE_ALL_COPPER = 3,//一键金钱追回
	REWARD_BACK_RECEIVE_ALL_GOLD = 4,//至尊追回
};

enum Reward_Back_PRO_TYPE {
	REWARD_BACK_PRO_REWARD_TASK = 1,//	悬赏任务
	REWARD_BACK_PRO_ELITE_EXPLORE = 2,//精英副本 精英副本不使用该枚举; 使用对应副本ID
	REWARD_BACK_PRO_TEAM_EXPLORE = 3,//	组队副本
	REWARD_BACK_PRO_HERO_DREAMS = 4,//	英雄梦境
	REWARD_BACK_PRO_PARTY = 5,//	篝火晚会
	REWARD_BACK_PRO_WORLD_BOSS = 6,//	世界boss
	REWARD_BACK_PRO_TEAM_DEFEND = 7, // 守卫帝都

};

enum Keep_Reward_Status {
	KEEP_REWARD_NO_RECEIVE = 0,//未收藏
	KEEP_REWARD_CAN_RECEIVE = 1,//已收藏,未领奖
	KEEP_REWARD_IS_RECEIVE = 2,//已经领奖
};

#endif /* REWARD_DEF_H_ */
