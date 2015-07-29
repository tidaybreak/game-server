/*
 * Helper_Def.h
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 */

#ifndef HELPER_DEF_H_
#define HELPER_DEF_H_

enum DAILY_FULFILL_LISTEN {
	DAILY_FULFILL_LISTEN_ARENA = 1001, //竞技场
	DAILY_FULFILL_LISTEN_SINGLE_EXPLORE = 1002, //单人探险
	DAILY_FULFILL_LISTEN_ELITE_EXPLORE = 1003, //精英推图
	DAILY_FULFILL_LISTEN_TEAM_EXPLORE = 1004, //组队推图 英雄之门
	DAILY_FULFILL_LISTEN_HERO_DREAMS = 1005, //英雄梦境
	DAILY_FULFILL_LISTEN_REWARD_TASK = 1006, //悬赏任务
	DAILY_FULFILL_LISTEN_Team_Defend = 1007, // 守卫帝都
//	1007, //签到
	DAILY_FULFILL_LISTEN_ALCHEMY = 1008, //炼金
	DAILY_FULFILL_LISTEN_TEAM_ARENA = 1009,//组队竞技
	DAILY_FULFILL_LISTEN_GANG_LOTTERY = 1010, //公会抽奖
	DAILY_FULFILL_LISTEN_GANG_DEDICATE = 1011,//公会捐献

	DAILY_FULFILL_LISTEN_WORLD_CHAT = 1012,//世界发言
	DAILY_FULFILL_LISTEN_COST_GOLD = 1013,//消费钻石
	DAILY_FULFILL_LISTEN_RUNE_SMELT = 1014,//符石熔炼
	DAILY_FULFILL_LISTEN_DAILY_LOGIN = 1015,//每日登陆
	DAILY_FULFILL_LISTEN_SECRET_SHOP = 1016,//神秘商人
	DAILY_FULFILL_LISTEN_WARER = 1017,//战场
	DAILY_FULFILL_LISTEN_CAMPFIRE = 1018,//篝火晚会
	DAILY_FULFILL_LISTEN_GANG_WAR = 1019,//公会战
	DAILY_FULFILL_LISTEN_DRAGON_VALE_ROB = 1020,//龙谷掠夺
	DAILY_FULFILL_LISTEN_HANDSEL_FLOWER = 1021,//送花

	DAILY_FULFILL_LISTEN_DRAGON_STEP_FOOT = 2021, //龙谷踩脚印
	DAILY_FULFILL_LISTEN_DRAGON_HARVEST = 2022,	//龙谷收获
	DAILY_FULFILL_LISTEN_DRAGON_PLAY_DRAGON = 2023, //喂龙
	DAILY_FULFILL_LISTEN_DRAGON_DRIVE_THIEF = 2024,	//龙谷驱逐
	DAILY_FULFILL_LISTEN_KNIGHT_TRIALS = 2025,	//骑士试炼场
	DAILY_FULFILL_LISTEN_ANSWER = 2026,	//智慧试炼
	DAILY_FULFILL_LISTEN_RING_TASK = 2027,	//环任务
	DAILY_FULFILL_LISTEN_DEFEND_DD = 2028,	//守卫帝都
	DAILY_FULFILL_LISTEN_MALL_BUY_ITEM = 2029,	//商城购买任一道具

	DAILY_FULFILL_LISTEN_WORLD_BOSS = 2030,//世界BOSS


	DAILY_FULFILL_LISTEN_TASK = 2000, //日常任务(大)
};

enum DAILY_TYPE {
	DAILY_TYPE = 1,	//日常
	ACTIVITY_TYPE = 2,//活跃度
};

enum RECEIVE_TYPE {
	TODAY_AWARD = 1, //每日活跃度奖励
	ALL_AWARD = 2, //累计活跃度奖励
};

enum UPGRADE_SCORE_TYPE {
	UPGRADE_SCORE_EQUIPER_BASE = 1,	//装备基础属性
	UPGRADE_SCORE_HERO_LEVEL_UP = 2,//英雄升级
	UPGRADE_SCORE_EQUIPER_UNBAN_LVL = 3,	//装备解封(装备强化)
	UPGRADE_SCORE_RUNE = 4,	//符文属性(宝石)
	UPGRADE_SCORE_GANG_SKILL = 5,	//公会技能
	UPGRADE_SCORE_DRAGON_PROP = 6,	//龙属性
	UPGRADE_SCORE_RUNE_STONE =	7,	//符石属性
	UPGRADE_SCORE_MASTER = 8,	//装备精通
	UPGRADE_SCORE_ENCHANT =	9,	//装备附魔
	UPGRADE_SCORE_MOUNT = 10,	//坐骑属性(骑术升级)坐骑培养
	UPGRADE_SCORE_WING = 11,	//翅膀属性(翅膀强化)翅膀进阶
	UPGRADE_SCORE_FASHION = 12,	//时装属性
	UPGRADE_SCORE_MOUNT_COLLECT = 13, //坐骑收集
	UPGRADE_SCORE_FASHION_COLLECT = 14,	//时装收集
	UPGRADE_SCORE_ARTTIFACT = 15,	//神器属性
	UPGRADE_SCORE_CARD = 16,//卡牌属性
};

enum UPGRADE_GET_TYPE {
	UPGRADE_HEROER_TEAM_FB_H = 4, //英雄之门
	UPGRADE_HEROER_TEAM_FB = 5, //英雄之门
	UPGRADE_GET_ALCHEMY = 18,//炼金
	UPGRADE_GET_ARENA = 19,//竞技
	UPGRADE_GET_ELITE_EXPLORE = 22,	//精英副本
	UPGRADE_GET_REWARD_TASK = 23,	//悬赏任务
	UPGRADE_GET_TEAM_HOOK = 24,	//组队挂机
	UPGRADE_GET_GAIN_PROD = 25,	//资源收获(龙谷)
//	UPGRADE_GET_ARENA = 26,	//神树升级
	UPGRADE_GET_DRAGON_BRING_UP = 27,	//龙培养
	UPGRADE_GET_WORLD_BOSS = 28,	//世界boss
	UPGRADE_GET_WARER = 29,	//战场
	UPGRADE_GET_LEISURE_ACTIVE = 30,	//休闲活动 篝火舞会
	UPGRADE_GET_TEAM_ARENA = 31,	//组队竞技
	UPGRADE_GET_GANG_FIGHT = 32,	//公会战
};

enum UPGRADE_GET_STATUS_TYPE {
	UPGRADE_GET_STATUS_NOT_OPEN = 0,//不可参与
	UPGRADE_GET_STATUS_CAN_OPEN = 1,//可参与
	UPGRADE_GET_STATUS_ACTIVE = 2,//进行中
};

enum FORCE_CHANGE_ACTIVE_OPEN { //战斗力改变通知客户端开启模块表示
	FORCE_CHANGE_ACTIVE_AREA = 1,//竞技场
	FORCE_CHANGE_ACTIVE_ELITE_EXPLORE = 2, //精英推图
};

enum HELPER_COMMON_DEF {
	HELPER_COMMON_ACTIVITY_VIP_ADD = 10, //VIP每天额外得到活跃度数
	HELPER_COMMON_AUTO_CHECK_IN_VIP_LEVEL = 4,//VIP自动签到等级
	//HELPER_COMMON_CHECK_IN_DAILY_LEVEL = 12, //签到限定等级
	HELPER_COMMON_DIALY_ICON_ID = 11401,//日常图标ID
	HELPER_COMMON_SINGLE_EXPLORE_COST_STRENGTH = 5,//每次单人副本消耗的体力
};

#endif /* HELPER_DEF_H_ */
