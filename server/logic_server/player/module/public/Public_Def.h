/*
 * Public_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef PUBLIC_DEF_H_
#define PUBLIC_DEF_H_


enum Power_Type{
	pwoer_mall = 1
};

enum Fcm_Time{
	FCM_STEP_TIME = 3600,
	FCM_RESET_TIME = 5 * FCM_STEP_TIME, //DEFAULT 5
	FCM_LOCK_TIME = 3 * FCM_STEP_TIME,
	FCM_LAST_TIP_TIME = 300,
	FCM_STEP_SHOW_TIME = 3600,  //TEST
	FCM_LAST_SHOW_TIME = 300 //TEST
};

enum Consume_Item {
	CHAT_BUGLE = 20506401,
};

enum Player_Hero_Id {
	//PLAYER_HERO_ID = 60200000,
	PHI_SOLDIER = 60200001,
	PHI_ASSASSIN = 60200002,
	PHI_MAGE = 60200003,
	PHI_END
};

enum Career_Type {
	CAREER_UNDEF = 0,
	CAREER_SOLDIER = 1,
	CAREER_ASSASSIN = 2,
	CAREER_MAGE = 3,
	CAREER_HERO = 4,
};

enum String_Info_Change_Type {
	SICT_GANG = 1,				// 公会名
	SICT_ROLE_NAME = 2,			// 角色名
	SICT_END
};

enum Int_Info_Change_Type {
	IICT_HERO_AWAKE_LVL = 1,
	IICT_END
};

enum First_Recharge_Status {
	First_Recharge_No = 0,
	First_Recharge_Has = 1,
	First_Recharge_Receive =2,
};

enum Recharge_Rebate_Status {
	Recharge_Rebate_No = 0, //不可以领取
	Recharge_Rebate_Can = 1, //可以领取
	Recharge_Rebate_Receive = 2, //已经领取
};

enum Public_Mail_Type {
	FIRST_RECHARGE_MAIL_ID = 1037,
	OLD_PLAYER_GIFT_MAIL_ID = 1044,
};

enum Guide_Finish_Enum {
	GUIDE_FINISH_TYPE_DRAGON_EGG_BORN = 1,//龙蛋已经开始孵化或完成
	GUIDE_FINISH_TYPE_DRAGON_VALE_BUILDING_CREATE = 2,//龙谷建筑有一个已开始建造或者完成
	GUIDE_FINISH_TYPE_LEARN_ROLE_SKILL = 3,//主角技能已学习
	GUIDE_FINISH_TYPE_LEARN_HERO_SKILL = 4,//英雄技能已学习
	GUIDE_FINISH_TYPE_GET_HERO = 5,//已经获得某个英雄
	GUIDE_FINISH_TYPE_HERO_TO_LEVEL = 6,//某个英雄已经提升至指定等级
	GUIDE_FINISH_TYPE_GET_ITEM = 7,//获得某个道具
	GUIDE_FINISH_TYPE_ROLE_HAS_A_EQUIP = 8,//主角身上有某id的装备
	GUIDE_FINISH_TYPE_HERO_HAS_A_EQUIP = 9,//英雄身上有某id的装备
	GUIDE_FINISH_TYPE_HORSE_STAR_LEVEL = 10,//将XX坐骑培养到XX星
	GUIDE_FINISH_TYPE_WAKE_UP_HERO_TO_QUALITY_LEVEL = 11,//指定ID英雄觉醒到XX品质
	GUIDE_FINISH_TYPE_HERO_SET_FIGHT = 12,//指定ID的英雄出战
};

enum Invite_Reward_Type {
	INVITE_REWARD_TYPE_CAN_NOT_RECEIVE = 0,//.不可领
	INVITE_REWARD_TYPE_CAN_RECEIVE = 1,//.可领
	INVITE_REWARD_TYPE_HAS_RECEIVE = 2,//.已领
};

enum Try_Timeout_Type {
	TRY_VIP_TIMEOUT = 1,
	TRY_MAGIC_WING_TIMEOUT = 2,
	TRY_GENIUS_TIMEOUT = 3,
};

enum Recharge_Common_Config_Type {
	RCCT_FIRST_RECHRAGE_START_LEVEL = 1,
	RCCT_FIRST_RECHARGE_TIME = 2,
};

#endif /* PUBLIC_DEF_H_ */
