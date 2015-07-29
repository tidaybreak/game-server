/*
 * Pack_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef PACK_DEF_H_
#define PACK_DEF_H_

namespace Pack {

const int PACK_INDEX_GAP = 10000;
const int ALL_HERO_EQUIP_GAP = 1000000;
const int ONE_HERO_EQUIP_GAP = 1000;
const int PACK_SAVE_TIME_INTERVAL = 300;

// 指定包裹类型
enum Pack_Type {
	PACK_T_EQUIP_INDEX 				= 10000,	//人物装备
	PACK_T_PACKAGE_INDEX 			= 20000,	//人物背包
	PACK_T_STORAGE_INDEX 			= 30000,	//人物仓库
	PACK_T_ACHIEVEMENT_INDEX 		= 40000,	//成就勋章身上位
	PACK_T_THOR_LOTTERY_PACK_INDEX	= 50000,	//雷神之锤

	PACK_T_RUNE_STONE_SMELT_INDEX 	= 100000,	// 符石熔炉
	PACK_T_RUNE_STONE_PACK_INDEX 	= 110000,	// 符石背包
	PACK_T_RUNE_STONE_STORE_INDEX 	= 120000,	// 符石仓库
	PACK_T_RUNE_STONE_EQUIP_INDEX 	= 130000,	// 符石装备
	PACK_T_RUNE_STONE_BOX_INDEX 	= 140000,	// 符石魔盒

	PACK_T_CARD_BOX_INDEX			= 150000, 	// 卡牌仓库

	// ------- index百万以上：英雄携带物品段，每段占领一个百万index位，预留100个段 -----------
	PACK_T_HERO_EQUIP_INDEX 				= 1000000,	// 英雄身上装备 1000000-1999999
	PACK_T_HERO_EQUIP_INDEX_END 			= 1999999,	// 英雄身上装备 1000000-1999999

	PACK_T_HERO_RUNE_STONE_EQUIP_INDEX 		= 2000000,	// 英雄身上符石 2000000-2999999
	PACK_T_HERO_RUNE_STONE_EQUIP_INDEX_END	= 2999999,	// 英雄身上符石 2000000-2999999

	PACK_T_HERO_INDEX_END,

	PACK_T_ALL_INDEX = 0
};

enum Update_From_Type{
	FROM_NULL	= 0,
	FROM_MOVE	= 1,
	FROM_ERASE	= 2,
	FROM_INSERT	= 3,
};

enum Hero_Pack_Module {		// 用作接口的参数类型，要与Pack_Type的英雄部分保存一致
	HPM_EQUIP_INDEX 				= PACK_T_HERO_EQUIP_INDEX,	// 英雄身上装备 1000000-1999999
	HPM_RUNE_STONE_EQUIP_INDEX 		= PACK_T_HERO_RUNE_STONE_EQUIP_INDEX,	// 英雄身上符石 2000000-2999999
	HPM_END							 = 0
};

enum MERGE_WAY {
	MERGE_WAY_EQUAL,
	MERGE_WAY_SIMILAR
};

// money增加的方式 请保证增加的类型不要超过10000，后台流水需要
enum Money_Gain_Type {
    MONEY_ADD_SELL_GOODS = 1,                    //出售物品
    MONEY_ADD_TASK_AWARD = 2,                    //任务奖励
    MONEY_ADD_MAIL_PICKUP = 3,                   //邮件附件
    MONEY_ADD_RECHARGE = 5,                      //充值
    MONEY_ADD_USE_ITEM = 6,                      //使用道具
    MONEY_ADD_TRADE_GAIN = 7,                   //交易获得
    MONEY_ADD_VIP_RIGHT = 8,                    //VIP赠送
    MONEY_ADD_DAILY_AWARD = 9,                  //日常奖励
    MONEY_ADD_DROPS = 10,                        //打怪拾取
    MONEY_ADD_FACTION_WELFARE = 11,              //帮派福利
    MONEY_ADD_FENGCE = 12,                       //封测获得
    MONEY_ADD_UNLEARN = 13,                      //重修技能获得(归还之前的消耗)
    MONEY_ADD_HUODONG = 14,                      //活动奖励
    MONEY_ADD_ACHIEVEMENT_GAME = 15,             //成就游戏
    MONEY_ADD_FB_AWARD = 16,                     //副本奖励
    MONEY_ADD_MATERIAL = 17,                     //采集获得
    MONEY_ADD_ALCHEMY = 18,				         	//炼金获得
    MONEY_ADD_TEAM_SPORTS = 19,					 	//组队竞技
    MONEY_ADD_REWARD_TASK = 20,                  	//悬赏任务奖励
    MONEY_ADD_TEAM_HERO_DREAM = 21,				 	//英雄梦境
    MONEY_CANCEL_CREATE_DRAGON_VALE_BUILDING = 22,	//取消建造龙谷建筑返还
    MONEY_GAIN_PRODUCTION_IN_DRAGON_VALE = 23,		//在龙谷内收获资源获得
    MONEY_GAIN_DRAONG_SOUL_BY_PLAY_DRAGON = 24,		//逗龙奖励龙魂
    MONEY_GAIN_ACHIEVEMENT = 25,					//成就获得
    MONEY_ADD_VIP_WORLD_TREE = 26,					//VIP世界之树获取
    MONEY_ADD_TEAM_FB_AWARD = 27,					//组队推图
    MONEY_ADD_RUNE_STONE_SMELT = 28,				//符石熔炼
    MONEY_ADD_REWARD_BACK	   = 29,				//奖励找回
    MONEY_ADD_RUNE_STONE_SELL = 30,					//符石出售
    MONEY_ADD_ROB_DRAGON_VALE_PRODUCTION = 31,      //掠夺收获跨服龙谷资源
    MONEY_ADD_LEVEL_ACE = 32,						//冲级高手活动
    MONEY_ADD_SUIT_COLLECT = 33,					//套装收集活动
    MONEY_ADD_DRIVTE_THIEF_GET_DRAGON_SOUL = 34,    //龙谷驱逐获得龙魂奖励
    MONEY_ADD_GUILD_LEVEL = 35, 					//工会等级活动
    MONEY_ADD_STEP_FOOT_GET_DRAGON_SOUL = 36,       //龙谷踩脚印获得龙魂奖励
    MONEY_ADD_ROB_MATCH_FAIL_BACK_COIN = 37,        //龙谷掠夺匹配失败返回扣除的金币
    MONEY_ADD_ONLINE_REWARD = 38,					//在线奖励获取
    MONEY_ADD_KEEP_REWARD = 39,						//收藏有奖
    MONEY_ADD_TEAM_FB = 40, 						// 组队推图获得
    MONEY_ADD_FIRST_RECHARGE = 41, 					// 首充礼包获得
    MONEY_ADD_DAY_TASK = 42,						//日常任务奖励
    MONEY_ADD_WEEK_TASK = 43,						//周常任务奖励
    MONEY_CANCEL_UPGRADE_DRAGON_VALE_BUILDING = 44, //取消升级龙谷建筑返还
    MONEY_ADD_DAILY_CHECK_IN = 45, //签到获得
    MONEY_ADD_FORENOTICE_GIFTS = 46,				// 预告礼包
    MONEY_ADD_KNIGHT_TRIALS = 47,				// 骑士试炼开启宝箱
    MONEY_ADD_GEM_LEVELUP = 48,				//宝石等级活动
    MONEY_ADD_ANCI_ARTIFACT = 49,			// 上古神器
    MONEY_ADD_MONTH_CARD_INVEST_PROFIT = 50,         //领取月卡投资收益
    MONEY_ADD_UPGRADE_INVEST_LEVEL_PROFIT = 51,      //领取升级投资等级收益
    MONEY_ADD_UPGRADE_INVEST_GET_ALL_PROFIT = 52,    //一键领取升级投资所有收益
    MONEY_ADD_WING_LEVELUP = 53,  					 //开服翅膀等级活动
    MONEY_ADD_MOUNT_LEVELUP = 54,  					 //开服坐骑升级活动
    MONEY_ADD_RING_TASK = 55,                        //环任务奖励
    MONEY_ADD_ANSWER = 56,                        //智慧试炼
    MONEY_GAIN_COIN_BY_PLAY_DRAGON = 57,		//逗龙奖励金币
    MONEY_ADD_DRIVTE_THIEF_GET_COIN = 58,    //龙谷驱逐获得金币奖励
    MONEY_ADD_STEP_FOOT_GET_COIN = 59,       //龙谷踩脚印获得金币奖励
    MONEY_ADD_ROB_ATTCK_FAIL_BACK_COIN = 60,        //掠夺进攻无效返回扣除的金币
    MONEY_ADD_SG_PASS = 61,        //开服活动,上古副本通关奖励
    MONEY_ADD_TIMELIMIT	= 62,	// 限时击杀/决战之颠
    MONEY_ADD_ARENA_FIHGT		= 63,			// 竞技场胜负奖励
    MONEY_ADD_ARENA_RANK		= 64,			// 竞技场每日排名奖励
    MONEY_ADD_ARENA_SEASON		= 65,			// 竞技场赛季奖励
	MONEY_ADD_QUESTIONNAIRE = 66, 			//调查问卷获得
	MONEY_ADD_INVITE_REWARD = 67, 			//邀请奖励获得
	MONEY_ADD_VIP_DAILY = 68, 			//VIP每日登陆领取
	MONEY_ADD_37VIP_REWARD = 69, 			//37VIP等级领取
	MONEY_ADD_WAR_AWARD = 70, 				// 战场获得
	MONEY_ADD_INNER_RECHARGE = 71, 				//内部充值
	MONEY_ADD_ARENA_DAN = 72, 				//竞技场段位-开服活动

	MONEY_ADD_SELL_CARD	= 73,				//出售卡牌
	MONEY_ADD_WORLD_BOSS	= 74,				// 世界boss结算
	MONEY_ADD_WORLD_BOSS_FB	= 75,				// 世界boss首杀
	MONEY_ADD_WORLD_BOSS_FT	= 76,				// 世界boss战斗


	MONEY_ADD_ACTIVE_CONTENT_TEAM_ARENA = 77,             // 组队竞技(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_WARER = 78,             // 战场(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_REWARD_TASK = 79,             // 悬赏(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_SGFB = 80,             // 上古副本(开服活动)

	MONEY_ADD_EXPEDITION_CHEST = 81, // 远征宝箱掉落
	MONEY_ADD_EXPEDITION_BEAST = 82, // 远征野兽区掉落
	MONEY_ADD_EXPEDITION_SETTLE_AWARD = 83, // 远征每日奖励
	MONEY_ADD_EXPEDITION_SETTLE_AWARD_BIG = 84, // 远征每日奖励大包
	MONEY_ADD_ACTIVITY_EXCHANGE_MALL = 85, //活动兑换商店
	MONEY_ADD_EXPEDITION_DEMAGE_AWARD = 86, // 远征：伤害奖励
	MONEY_ADD_VALENTIEN_MONSTER_KILL_BOSS 	= 87, 	// 情人节杀死boss奖励

	MONEY_ADD_ACTIVE_CONTENT_WORLD_BOSS = 88,             // 世界boss(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_SUIT_CONLLECT_TWO = 89,             // 套装收集二段
	MONEY_ADD_ACTIVE_CONTENT_HEROES_DOOR = 90,             // 英雄之门(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_FM_X_CI = 91,//累计附魔X次(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_X_X_QUALITY_HEROER = 92,//累计X个X品质英雄(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_MOUNT_LVUP_X = 93,//坐骑进阶X次(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_FUSION_X_ELITE_CARD = 94,//合成X张精英卡牌(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_ALCHEMY_GIFT = 95, //炼金有礼(开服活动)
	MONEY_ADD_EQUIP_BREAKDOWN_GOLD = 96,                  	//装备分解获得金币

    MONEY_ADD_SOUL_BY_PLAY_DRAGON = 96,		//逗龙奖励英灵
    MONEY_ADD_SOUL_BY_DRIVTE_THIEF = 97,    //龙谷驱逐获得英灵奖励
    MONEY_ADD_SOUL_BY_STEP_FOOT = 98,       //龙谷踩脚印获得英灵奖励
    MONEY_ADD_FRIENDSHIP_BY_PLAY_DRAGON = 99,		//逗龙奖励友情值
    MONEY_ADD_FRIENDSHIP_BY_DRIVTE_THIEF = 100,    //龙谷驱逐获得友情值奖励
    MONEY_ADD_FRIENDSHIP_BY_STEP_FOOT = 101,       //龙谷踩脚印获得友情值奖励
	MONEY_ADD_ACTIVE_CONTENT_ACTIVITY_GIFT = 102, //炼金有礼(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_RING_TASK = 103, //天天跑环(开服活动)
	MONEY_ADD_ACTIVE_CONTENT_DRAGON_PREY = 104, //龙谷掠夺活动(开服活动)
	MONEY_ADD_LIMIT_TIME_RECHARGE = 105, //限时充值活动
	MONEY_ADD_HONOR_ARENA_SHOW			= 106,				// 荣誉竞技场界面展示奖励（首胜、十战、连胜）
	MONEY_ADD_HONOR_ARENA_RANK			= 107,				// 荣誉竞技场排名奖励
	MONEY_ADD_HONOR_ARENA_FIGHT		= 108,				// 荣誉竞技场战斗奖励
	MONEY_ADD_OPER_ACTIVITY	= 109,				//运营活动
	MONEY_ADD_SINGLE_CHAPTER 						= 110,         //单人副本章节奖励
	MONEY_ADD_ACTIVITY_CONTENT         =111,//开服活动

    MONEY_ADD_GM = 9999,							//GM获取
    MONEY_ADD_MAX = 10000,							//无
};

enum Money_Lose_Type {
    MONEY_SUB_MEND_EQUIP = 10001,                //装备修复
    MONEY_SUB_MEND_ALL_EQUIP = 10002,            //修复所有装备
    MONEY_SUB_MAIL_MONEY = 10003,                //邮件附件
    MONEY_SUB_MAIL_POSTAGE = 10004,              //邮费
    MONEY_SUB_SHOP_USE = 10005,                  //商店购买
    MONEY_SUB_MALL_USE = 10006,                  //商城购买
    MONEY_SUB_SHOP_BUYBACK = 10007,              //商店购回
    MONEY_SUB_MOUNT_EVOLVE = 10008,              //坐骑进阶花费
    MONEY_SUB_MOUNT_TRAIN = 10009,               //坐骑培养消耗
    MONEY_SUB_EQUIP_INCREASE = 10010,            //装备精通
    MONEY_SUB_HERO_LEVEL_UP = 10011,             //英雄升级
    MONEY_SUB_TRADE_USE = 10012,                 //交易失去
    MONEY_SUB_ADD_PACKAGE_CAP = 10013,           //扩充背包
    MONEY_SUB_OPEN_VIP = 10014,                  //开通VIP
    MONEY_SUB_UPDATE_VIP = 10015,                //升级VIP
    MONEY_SUB_USE_HORN = 10016,                  //使用喇叭
    MONEY_SUB_LEARN_SKILL = 10017,               //学习技能
    MONEY_SUB_FAST_TASK_CONSUME = 10018,         //快速完成任务消耗
    MONEY_SUB_WELFARE_CONSUME = 10019,           //购买离线经验
    MONEY_SUB_RELIVE = 10020,                    //复活
    MONEY_SUB_TRANSFER = 10021,                  //传送
    MONEY_SUB_GEMSTONE_COMBINE = 10022,          //宝石合成
//    MONEY_SUB_GEMSTONE_CONVERT = 10023,          //宝石属性转换
    MONEY_SUB_GEMSTONE_FILL = 10024,             //宝石镶嵌
    MONEY_SUB_GEMSTONE_ERASE = 10025,            //宝石摘除
    MONEY_SUB_FACTION_CREATE = 10026,            //帮派创建
    MONEY_SUB_EQUIP_ADDITION = 10027,            //装备洗练
    MONEY_SUB_EQUIP_LEVEL_UP = 10028,            //装备锻造
    MONEY_SUB_AUTO_BUY = 10029,                  //自动购买
    MONEY_SUB_ADD_STORAGE_CAP = 10030,           //仓库扩充
    MONEY_SUB_MARKET_CHARGE = 10031,             //市场求购手续费
    MONEY_SUB_MARKET_PLEDGE = 10032,             //市场求购押金
    MONEY_SUB_MARKET_STORAGE = 10033,            //市场寄售保管费
    MONEY_SUB_MARKET_BUY = 10034,                //市场购买
    MONEY_SUB_TASK_COMPLETE = 10035,             //任务快速完成
    MONEY_SUB_TASK_RESET = 10036,                //任务重置
    MONEY_SUB_TASK_REFRESH = 10037,              //任务刷新
    MONEY_SUB_PET_SKILL_TAKEOFF = 10038,         //宠物技能封印
    MONEY_SUB_PET_SKILL_STONE = 10049,           //宠物兽魂石刷新
    MONEY_SUB_CONTINUE_VIP = 10050,              //续费vip
    MONEY_SUB_EQUIP_INHERIT = 10051,             //装备传承
    MONEY_SUB_FACTION_DONATE = 10052,            //帮派捐献
    MONEY_SUB_PET_RECRUIT = 10053,               //宠物招募
    MONEY_SUB_FB_CHOUJIANG = 10054,              //副本抽奖消耗
    MONEY_SUB_FB_RESET = 10055,                  //副本重置消耗
    MONEY_SUB_FB_MAKEUP = 10056,                 //副本经验补偿
    MONEY_SUB_PET_CHANGE_NAME = 10057,           //宠物改名
    MONEY_SUB_PET_CHANGE_FACADE = 10058,         //宠物幻化
    MONEY_SUB_SEND_COORD = 10059,                //发送坐标
    MONEY_SUB_GEM_REFINE = 10060,                //宝石精炼失去
    MONEY_SUB_MOUNT_EQUIP_ENHANCE = 10061,       //坐骑装备强化
    MONEY_SUB_FACTION_INVITE_FRIEND = 10062,	 //帮派好友邀请
    MONEY_SUB_ROLE_ERASE_SKILL = 10063,			 //角色洗点技能
    MONEY_SUB_OPEN_TALENT_PAGE = 10064,			 //开天赋页
    MONEY_SUB_OPEN_TALENT = 10065,				 //开天赋
    MONEY_SUB_ALCHEMY = 10066,				     //炼金
    MONEY_SUB_GANG_UPGRADE_GIFT = 10067,         //发公会升级礼包消耗
    MONEY_SUB_EXCHANGE = 10068,					 //物品兑换
    MONEY_SUB_MONEY_REFRESH_REWARD_TASK_LIST = 10069, //立即刷新悬赏榜
    MONEY_SUB_MONEY_BUY_COVENANT = 10070,		 //开通或续费精灵盟约
    MONEY_SUB_MONEY_CREATE_DRAGON_VALE_BUILDING = 10071,  //建造龙谷建筑消耗
    MONEY_SUB_MONEY_UPGRADE_DRAGON_VALE_BUILDING = 10072, //升级建筑消耗
    MONEY_SUB_MONEY_QUICK_FINISH_VALE_BUILDING = 10073,   //加速建造或者升级建筑消耗
    MONEY_SUB_MONEY_COIN_BY_KEEP_DRAGON = 10074,		  //培养龙消耗金币
    MONEY_SUB_MONEY_DRAGON_SOUL_BY_KEEP_DRAGON = 10075,   //培养龙消耗龙魂
    MONEY_SUB_MONEY_ADD_DRAGON_FEELING = 10076,           //增加龙心情值消耗
    MONEY_SUB_MONEY_DRAGON_EVOLUTION = 10077,             //进化龙消耗
    MONEY_SUB_MONEY_REWARD_BACK_COST = 10078,			 //奖励找回
    MONEY_SUB_MONEY_ROB_DRAGON_VALE = 10079,             //龙谷资源被入侵者掠夺
    MONEY_SUB_MONEY_ROB_MATCH = 10080,                   //龙谷掠夺匹配
    MONEY_SUB_MONEY_BATCH_EVOLUTION_DRAGON = 10081,      //批量龙进化
    MONEY_SUB_MONEY_FASHION_INHERIT	= 10082,			 //时装传承
    MONEY_SUB_MONEY_ADD_AUCTION_GOODS	= 10083,			 //寄售拍卖商品
    MONEY_SUB_ONES_BUY_AUCTION_GOODS	= 10084,			 //一口价购买竞拍商品
    MONEY_SUB_UPDATE_AUCTION_GOODS	= 10085,			 //出价竞拍商品
    MONEY_SUB_MONEY_FASHION_DRAW_CARD = 10086,			 //时装抽卡牌
    MONEY_SUB_QUICK_BUY = 10087,			 //快捷购买
    MONEY_SUB_USE_ITEM = 10089,                      //使用道具
    MONEY_SUB_UPGRADE_WING = 10090,					 //升阶翅膀
    MONEY_SUB_MONTH_CARD_INVEST = 10091,             //月卡投资
    MONEY_SUB_MONEY_UPGRADE_INVEST_FIRST = 10092,    //升级投资的马上投资
    MONEY_SUB_MONEY_UPGRADE_INVEST_ADD_MONEY = 10093,//升级投资的追加投资
    MONEY_SUB_MONEY_RECOVER_TASK_CHAIN_BY_FRIENDSHIP = 10094,//友情值恢复任务链
    MONEY_SUB_MONEY_RECOVER_TASK_CHAIN_BY_DIAMOND = 10095,//钻石恢复任务链
    MONEY_SUB_ARENA_BUY_PAY_TIMES = 10096,					  // 购买单人竞技场次数
    MONEY_SUB_EQUIP_ENCHANT = 10097,                 // 装备附魔
    MONEY_SUB_GANG_CLEAR_CD_BUILDING_LVL = 10098,    // 清公会建筑CD
    MONEY_SUB_GANG_CLEAR_CD_GANG_LVL = 10099,        // 清公会等级CD
    MONEY_SUB_GANG_SEND_BLESS_BUF = 10100,           // 发公会祝福
    MONEY_SUB_GANG_SEND_GROUP_MAIL = 10101,          // 发公会群邮件
    MONEY_SUB_GANG_SEIZE_POWER = 10102,              // 公会夺权
    MONEY_SUB_GANG_WORLD_NOTICE = 10103,             // 公会世界公告
    MONEY_SUB_ARTIFACT_UPGRADE = 10104,              // 神器升级
    MONEY_SUB_RUNE_STONE_SMELT = 10105,              // 符石熔炼
    MONEY_SUB_RUNE_FILL_ENERGY = 10106,              // 符文充能
    MONEY_SUB_EQUIP_UNBAN = 10107,                   // 装备强化
    MONEY_SUb_THOR_LOTTERY = 10108,				 		  // 雷神之锤
    MONEY_SUB_ARENA_REFRESH_CD 		= 10109,			  // 单人竞技场刷新cd
    MONEY_SUB_ARENA_REFRESH_MATCH 	= 10110,			  // 单人竞技场刷新匹配
    MONEY_SUB_FB_COMPLETE_WIPE 	   = 10111,			  // 副本加速扫荡
    MONEY_SUB_FB_BUY_PHY			   = 10112,			  // 单人副本购买体力
    MONEY_SUB_STORE_USE				   = 10113,			  // 神秘商人购买
    MONEY_SUB_CARD_UPGRADE	= 10114,						  // 卡牌
    MONEY_SUB_MOUNT_TRAIN_AUTO = 10115,              // 坐骑自动培养消耗
    MONEY_SUB_PHY_POWER_PRACTICE = 10116, 			  // 公会修行

    MONEY_SUB_GEMSTONE_UNEMBED = 10117, 			// 宝石拆装
    MONEY_SUB_GEMSTONE_CONVERT	= 10118,			// 宝石转化
    MONEY_SUB_WORLD_BOSS_BLESS		= 10119,				// 世界boss公会祝福
    MONEY_SUB_WORLD_BOSS_REBORN		= 10120,				// 世界boss浴火重生
    MONEY_SUB_WORLD_BOSS_POWER_UP	= 10121,				// 世界boss激发潜能

    MONEY_SUB_ELITE_ADD_NUM			= 10122,				// 上古副本增加额外挑战次数
    MONEY_SUB_CARD_MERGE = 10123,							// 卡牌合成
    MONEY_SUB_ACTIVITY_EXCHANGE_MALL = 10124,							// 活动兑换商店
    MONEY_SUB_GEMS = 10125,                                                  //合成宝石消耗
    MONEY_SUB_HERO_AWAKE = 10126,             			//英雄觉醒
    MONEY_SUB_HERO_RECRUIT = 10127,             		//英雄招募
    MONEY_SUB_FB_REVIVE = 10128,             			//单人副本付费复活

    MONEY_SUB_FASHION_PART_UPGRADE = 10129, //时装部位升级
    MONEY_SUB_LIFE_SKILL_UP = 10130,                    // 生活技能升级
    MONEY_SUB_SPAR_REFINE = 10131,                      // 晶石精练
    MONEY_SUB_CARD_MAKE = 10132,                      // 卡牌制作
    MONEY_SUB_SPAR_SMELT = 10133,                      // 晶石熔炼
    MONEY_SUB_LEARN_LIFE_SKILL = 10134,                      // 生活技能学习
    MONEY_SUB_MOUNT_FUSE = 10135,                      // 坐骑融合

    MONEY_SBU_CARD_POTEN_UPGRADE = 10136, // 卡牌升金
    MONEY_SUB_VAGRANT_STORE				   = 10137,			  // 流浪商人购买
    MONEY_SUB_TBAY_BLESS				  		= 10138,			  // 宝藏海湾神秘祝福
    MONEY_SUB_ARENA_WORSHIP                        = 10139,    // 竞技场被膜拜

    MONEY_SUB_GM = 19999,                        //GM消耗
    MONEY_SUB_MAX = 20000,

};

enum Item_Gain_Type {
    ITEM_GAIN_TEST = 1,                          //测试获取
    ITEM_GAIN_GEMSTONE_ERASE = 2,                //宝石摘除
    ITEM_GAIN_GEMSTONE_COMBINE = 3,              //宝石合成
//    ITEM_GAIN_GEMSTONE_CONVERT = 4,              //宝石属性转换
    ITEM_GAIN_MAIL = 5,                          //邮件附件
    ITEM_GAIN_COMBINE = 6,                       //炼炉合成
    ITEM_GAIN_GIFT = 7,                          //礼包
    ITEM_GAIN_CON_LOGIN = 8,                    //每日登陆奖励
    ITEM_GAIN_ONLINE_AWARD = 9,                 //在线奖励
    ITEM_GAIN_VIP = 10,                          //VIP赠送
    ITEM_GAIN_TRADE = 11,                        //交易获得
    ITEM_GAIN_MALL = 12,                         //商城购买
    ITEM_GAIN_TASK = 13,                         //任务
    ITEM_GAIN_DAILY = 14,                        //活跃度奖励
    ITEM_GAIN_FB = 15,                           //副本奖励
    ITEM_GAIN_RANK = 16,                         //排行榜奖励
    ITEM_GAIN_MARKET = 17,                       //市场获得
    ITEM_GAIN_EQUIP_INCREASE = 18,               //装备强化失败补偿
    ITEM_GAIN_EQUIP_OFF = 19,                    //脱下装备
    ITEM_GAIN_EXCHANGE = 20,                     //物品兑换获得
    ITEM_GAIN_DROPS_GET = 21,                    //打怪拾取
    ITEM_GAIN_EQUIP_LEVEL_UP = 22,               //装备锻造
    ITEM_GAIN_FACTION_AWAED = 23,                //帮派奖励
    ITEM_GAIN_PET_SKILL_STONE = 24,              //宠物兽魂石刷新
    ITEM_GAIN_PET_SKILL_TAKEOFF = 25,            //宠物技能封印获得
    ITEM_GAIN_COLLECTION = 26,                   //采集获得
    ITEM_GAIN_EQUIP_INHERIT = 27,                //装备传承
    ITEM_GAIN_PET_RESOLVE = 28,                  //宠物分解
    ITEM_GAIN_UNLEARN = 29,                      //技能重新
    ITEM_GAIN_ACHIEVEMENT = 30,                  //成就奖励
    ITEM_GAIN_FACTION_STORAGE = 31,              //帮派仓库取出
    ITEM_GAIN_PET_RECRUIT = 32,                  //宠物招募
    ITEM_GAIN_DAILY_SIGN = 33,                   //日常签到
    ITEM_GAIN_SELECT_ITEM = 34,                  //选择指定道具
    ITEM_GAIN_PET_COMBINE = 35,                  //宠物融合
    ITEM_GAIN_MOUNT_EQUIP = 36,                  //坐骑装备
    ITEM_GAIN_SHOP = 37,                        //商店购买
    ITEM_GAIN_BUYBACK = 38,                     //商店购回
    ITEM_GAIN_TREASURE = 39,                    //使用宝箱
    ITEM_GAIN_AUTO_BUY = 40,                    //自动购买
    ITEM_GAIN_HUODONG = 41,                     //活动奖励获取
    ITEM_GAIN_CHIEVEMENT_GAME = 42,             //成就获得
    ITEM_GAIN_STORE = 43,                       //神秘商店
    ITEM_GAIN_CONSUME_HD = 44,                  //消费活动获取
    ITEM_GAIN_RECHARGE_HD = 45,                 //充值活动获取
    ITEM_GAIN_TEAM_SPORTS = 46,					//组队竞技
    ITEM_GAIN_ACCEPT_REWARD_TASK = 47,          //领取悬赏任务获得
    ITEM_GAIN_VIP_TURNTABLE = 48,				//vip转盘获得
    ITEM_GAIN_TEAM_FB = 49,                      //组队副本奖励
    ITEM_GAIN_TEAM_SPORTS_CHECKOUT = 50,		//组队竞技结算
    ITEM_GAIN_REWARD_BACK = 51,					//奖励找回
    ITEM_GAIN_LIMIT_BUY = 52,					//限时抢购
    ITEM_GAIN_VIP_CLUB = 53,					//VIP俱乐部活动
    ITEM_GAIN_LEVEL_ACE = 54,					//冲级高手活动
    ITEM_GAIN_SUIT_COLLECT = 55,				//套装收集活动
    ITEM_GAIN_GANG_WAR = 56,				//公会战奖励
    ITEM_GAIN_STEP_FOOT = 57,                   //给别人龙谷踩脚印获得
    ITEM_GAIN_OTHER_STEP_FOOT = 58,             //别人来龙谷踩脚印，自己获得
    ITEM_GAIN_KEEP_REWARD = 59,             //收藏有奖
    ITEM_GAIN_FIRST_RECHARGE = 60,             //首充
    ITEM_GAIN_PLAY_SELF_DRAGON = 61,           //逗自己的龙获得
    ITEM_GAIN_PLAY_OTHER_DRAGON = 62,          //逗别人的龙获得
    ITEM_GAIN_SAFE_GUIDE = 63,                //引导保底获得
    ITEM_FASHION_DRAW_CARD = 64,			   //时装抽卡牌
    ITEM_GAIN_KNIGHT_TRIALS = 65,			   //骑士试炼开启宝箱
    ITEM_GAIN_GUIDE_ADD_IN_PLOT = 66,          //新手引导剧情中采集道具
    ITEM_GAIN_GEM_LEVELUP = 67, 				//宝石等级活动
    ITEM_GAIN_WING = 68,						//翅膀
    ITEM_GAIN_GUILD_LEVEL = 69, 				//公会等级活动
    ITEM_GAIN_WING_LEVELUP = 70, 				//翅膀活动
    ITEM_GAIN_MOUNT_LEVELUP = 71, 				//坐骑升级活动
    ITEM_GAIN_RING_TASK = 72,                   //环任务奖励
    ITEM_GAIN_GANG_SHOP = 73,                   // 公会商店购买
    ITEM_GAIN_GANG_DRAW = 74,                   // 公会抽奖
    ITEM_GAIN_FORENOTICE_GIFT = 76,             // 预告礼包
    ITEM_GAIN_RUNE_STONE_EXCHANGE = 77,         // 符石兑换
    ITEM_GAIN_RUNE_STONE_SMELT = 78,            // 符石熔炼
    ITEM_GAIN_THOR_LOTTERY = 79,				//雷神之锤
    ITEM_GAIN_ANCI_ARTIFACT = 80,				// 上古神器
    ITEM_GAIN_ANSWER = 81,				//智慧试炼
    ITEM_GAIN_SG_PASS = 82,				//开服活动,上古副本通关奖励
    ITEM_GAIN_ARENA_FIGHT 	= 83,				//单人竞技胜负奖励
    ITEM_GAIN_ARENA_RANK 	= 84,				//单人竞技每日排名奖励
    ITEM_GAIN_ARENA_SEASON = 85,				//单人竞技赛季奖励
    ITEM_GAIN_WAR_SCENE = 86,				   //战场获得奖励
	ITEM_GAIN_QUESTIONNAIRE = 87, 			//调查问卷获得
	ITEM_GAIN_INVITE_REWARD = 88, 			//邀请奖励获得
	ITEM_GAIN_VIP_DAILY = 89, 			//VIP每日登陆可以领取
	ITEM_GAIN_37VIP_REWARD = 90, 			//37VIP等级领取
	ITEM_GAIN_WING_RET_REWARD = 91,		// 翅膀返还奖励

	ITEM_GAIN_GEMSTONE_UNEMBED = 93,	// 宝石拆下
	ITEM_GAIN_GEMSTONE_CONVERT = 94,		// 宝石转化
	ITEM_GAIN_ARENA_DAN = 95,		//竞技场段位-开服活动
	ITEM_GAIN_WORLD_BOSS = 96,             // 世界boss
	ITEM_GAIN_WORLD_BOSS_FB = 97,             // 世界boss首杀
	ITEM_GAIN_WORLD_BOSS_FT = 98,             // 世界boss战斗

	ITEM_GAIN_ACTIVE_CONTENT_HEROES_DOOR = 99,             // 英雄之门(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_TEAM_ARENA = 100,             // 组队竞技(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_WARER = 101,             // 战场(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_REWARD_TASK = 102,             // 悬赏(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_SGFB = 103,             // 上古副本(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_WORLD_BOSS = 104,             // 世界boss(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_SUIT_CONLLECT_TWO = 105,             // 套装收集二段

	ITEM_GAIN_EXPEDITION_CHEST = 106, // 远征宝箱掉落
	ITEM_GAIN_EXPEDITION_BEAST = 107, // 远征野兽区掉落
	ITEM_GAIN_EXPEDITION_SETTLE_AWARD = 108, // 远征每日奖励
	ITEM_GAIN_EXPEDITION_SETTLE_AWARD_BIG = 109, // 远征每日奖励大包
	ITEM_GAIN_ACTIVITY_EXCHANGE_MALL = 110, //活动兑换商店
	ITEM_GAIN_VALENTINE_MONSTER_KILL_BOSS	 = 111, 		// 情人节杀死boss奖励

	ITEM_GAIN_ACTIVE_CONTENT_FM_X_CI	 = 112, 		// 累计附魔X次(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_X_X_QUALITY_HEROER	 = 113, 		// 累计X个X品质英雄(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_MOUNT_LVUP_X	 = 114, 		// 坐骑进阶X次(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_FUSION_X_ELITE_CARD	 = 115, 		// 合成X张精英卡牌(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_ALCHEMY_GIFT	 = 116, 		// 炼金有礼(开服活动)
	ITEM_GAIN_USE_ITEM = 117,                           // 使用道具
	ITEM_GAIN_EQUIP_BREAKDOWN_ENCHANT = 118,                        //装备分解获得附魔券
	ITEM_GAIN_EQUIP_BREAKDOWN_PROP = 119,                        //装备分解获得道具
	ITEM_GAIN_ACTIVE_CONTENT_ACTIVITY_GIFT	 = 120, 		// 活跃度好礼(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_RING_TASK	 = 121, 		// 天天跑环(开服活动)
	ITEM_GAIN_ACTIVE_CONTENT_DRAGON_PREY	 = 122, 		// 龙谷掠夺活动(开服活动)
	ITEM_GAIN_LIMIT_TIME_RECHARGE = 123, //限时充值活动
	ITEM_GAIN_SPAR_REFINE = 124,                        // 晶石精练
	ITEM_GAIN_CARD_MAKE = 125,                          // 卡牌制作
	ITEM_GAIN_SPAR_SMELT = 126,                         // 晶石熔炼
	ITEM_GAIN_HONOR_ARENA_SHOW			 = 127, 								// 荣誉竞技场界面展示奖励（首胜、十战、连胜）
	ITEM_GAIN_HONOR_ARENA_RANK			 = 128, 								// 荣誉竞技场排名奖励
	ITEM_GAIN_EXPEDITION_OCCUPY_CASTLE_AWARD	= 129, // 攻城榜领取
	ITEM_GAIN_OPER_ACTIVITY                =130,    //运营活动
	ITEM_GAIN_SINGLE_CHAPTER 						= 131,                           //单人副本章节奖励
	ITEM_GAIN_VAGRANT_STORE = 132,                       //流浪商人商店
	ITEM_GAIN_ACTIVITY_CONTENT                =133,    //开服活动

	ITEM_GAIN_CAMPFIRE_LIT				  = 135, //点燃篝火的奖励
	ITEM_GAIN_CAMPFIRE_ADD_WOOD = 136, //篝火添柴奖励
	ITEM_GAIN_CAMPFIRE_CONSOLATION = 137, //篝火安慰奖
	ITEM_GAIN_CAMPFIRE_RANK				= 138, //篝火排名奖励

    ITEM_GAIN_MAX = 10000,                      //无
};

enum Item_Lose_Type {
    ITEM_LOSE_TEST = 10001,                      //测试失去
    ITEM_LOSE_GEMSTONE_FILL = 10002,             //宝石镶嵌
    ITEM_LOSE_GEMSTONE_COMBINE = 10003,          //宝石合成
//    ITEM_LOSE_GEMSTONE_CONVERT = 10004,          //宝石属性转换
    ITEM_LOSE_MAIL = 10005,                      //邮件附件
    ITEM_LOSE_COMBINE = 10006,                   //炼炉合成
    ITEM_LOSE_PET_GROWTH = 10007,                //宠物成长提升
    ITEM_LOSE_PET_SKILL_LERAN = 10008,           //宠物技能学习
    ITEM_LOSE_TRADE = 10009,                     //交易失去
    ITEM_LOSE_TASK = 10010,                      //任务
    ITEM_LOSE_MARKET = 10011,                    //市场
    ITEM_LOSE_RELIVE = 10012,                    //复活
    ITEM_LOSE_EQUIP_UNBAN = 10013,               //装备强化
    ITEM_LOSE_EQUIP_ON = 10014,                  //穿上装备
    ITEM_LOSE_EQUIP_LEVEL_UP = 10015,            //装备锻造
    ITEM_LOSE_EQUIP_ENCHANT = 10016,             //装备附魔
    ITEM_LOSE_MOUNT_EVOLVE = 10017,              //坐骑进阶
    ITEM_LOSE_MOUNT_TRAIN = 10018,               //坐骑培养
    ITEM_LOSE_EXCHANGE = 10019,                  //物品兑换失去
    ITEM_LOSE_EQUIP_INHERIT = 10020,             //装备传承
    ITEM_LOSE_FACTION_STORAGE_DONATE = 10021,    //帮派物品捐献
    ITEM_LOSE_PET_GENIUS = 10022,                //宠物天赋强化
    ITEM_LOSE_MOUNT_EQUIP_ENHANCE = 10023,		 //坐骑装备强化
    ITEM_LOSE_USE = 10024,                       //物品使用
    ITEM_LOSE_SELL_SHOP = 10025,                 //出售
    ITEM_LOSE_SELL = 10026,                      //批量出售
    ITEM_LOSE_LEARN_SKILL = 10027,               //技能学习
    ITEM_LOSE_DISCARD = 10028,                   //丢弃
    ITEM_LOSE_TRANSFER = 10029,                  //传送
    ITEM_LOSE_MOUNT_FEED = 10030,                //坐骑喂养
    ITEM_LOSE_FACTION_CREATE = 10031,            //帮派创建
    ITEM_LOSE_UNLEARN_SKILL = 10032,             //重修技能消耗
    ITEM_LOSE_PACKAGE_CAPACITY = 10033,          //增加仓库容量
    ITEM_LOSE_STORAGE_CAPACITY = 10034,          //增加仓库容量
    ITEM_LOSE_EQUIP_QUENCH = 10035,              //装备淬炼
    ITEM_LOST_ENTER_FB = 10036,                  //副本进入扣取道具
    ITEM_LOST_GATHER = 10037,                    //采集扣取
    ITEM_LOSE_GEM_REFINE = 10038,                //宝石精炼失去
    ITEM_LOSE_OPEN_BOX = 10039,					 //开宝箱失去
    ITEM_LOSE_OPEN_TALENT = 10040,				 //开天赋
    ITEM_LOSE_OVERDUE_TIME = 10041,				 //时效过期
    ITEM_LOST_CHAT	= 10042,					 //聊天
    ITEM_LOST_FAST_FINISH = 10043,               //快速完成任务消耗
    ITEM_LOST_ACCEPT_ORANGE_REWARD_TASK = 10044, //领取橙色品质悬赏任务，扣悬赏令
    ITEM_LOSE_REWARD_TASK = 10045,               //放弃、提交悬赏任务以及限时悬赏任务失败，扣除领取悬赏任务时获得的道具
    ITEM_LOSE_DRAGON_EVOLUTION = 10046,          //进化龙消耗
    ITEM_LOSE_VIP_TURNTABLE = 10047,			 //vip转盘失去
    ITEM_LOSE_HANDSEL_FLOWERS = 10048,			 //送花失去
    ITEM_LOSE_BATCH_EVOLUTION_DRAGON = 10049,    //批量进化龙
    ITEM_LOSE_MERGE_FASHION	= 10050,			 //合成时装
    ITEM_LOSE_ADD_AUCTION_GOODS	= 10051, 		 //寄售拍卖商品
    ITEM_LOSE_DRAW_CARD = 10052,				 //时装抽卡牌
    ITEM_LOSE_USE_ITEM = 10053,				 	 //道具使用
    ITEM_LOSE_ACTIVE_WING = 10054,				 //激活翅膀
    ITEM_LOSE_UPGRADE_WING = 10055,				 //升阶翅膀
    ITEM_LOSE_EQUIP_MASTER = 10056,		         //装备master
    ITEM_LOSE_ARTIFACT_UPGRADE = 10057,		     // 神器升级
    ITEM_LOSE_AWAKE = 10058,		             // 觉醒消耗
    ITEM_LOSE_HERO_RECRUIT = 10059,		         // 英雄招募
    ITEM_LOSE_RENAME = 10060,		             // 改名消耗
    ITEM_LOSE_RUNE_STONE_MERGE = 10061,		     // 符石合并
    ITEM_LOSE_RUNE_STONE_SELL = 10062,		     // 符石出售
    ITEM_LOSE_RUNE_FILL_ENERGY = 10063,		     // 符文充能
    ITEM_LOSE_THOR_LOTTERY = 10064,				 //雷神之锤
    ITEM_LOSE_WIPE_DOUBLE = 10065,		         // 扫荡双倍
    ITEM_LOSE_TEAM_WORLD_INVITE = 10066,		// 组队世界邀请
    ITEM_LOSE_CARD_UPGRADE = 10067,				// 卡牌吞噬
    ITEM_LOSE_MOUNT_ACTIVATE = 10068,				// 坐骑激活
    ITEM_LOSE_MOUNT_LEARN_SKILL = 10069,			// 坐骑学习技能
    ITEM_LOSE_ELITE_RESET 		  = 10070,			// 上古副本重置
    ITEM_LOSE_MOUNT_AUTO_TRAIN = 10071,               //坐骑一键培养

    ITEM_LOSE_GEMSTONE_EMBED = 10072,					// 宝石镶嵌
    ITEM_LOSE_GEMSTONE_CONVERT = 10073,				// 宝石转化
    ITEM_LOSE_GEMSTONE_OPEN_HOLE = 10074, 			// 装备开宝石孔

    ITEM_LOSE_SELL_CARD	= 10075,								// 出售卡牌

    ITEM_LOSE_ELITE_ADD_NUM = 10076, 					// 上古副本增加额外挑战次数

    ITEM_LOSE_CARD_OPEN_HOLE = 10077,					// 卡牌开孔
    ITEM_LOSE_CARD_MERGE = 10078,							// 卡牌合成
    ITEM_LOSE_ACTIVITY_EXCHANGE_MALL = 10079,							//活动兑换商店

    ITEM_LOSE_INTIMACY_HERO = 10080,					// 英雄互动
    ITEM_LOSE_BREAKDOWN = 10081,					// 装备分解
    ITEM_LOSE_USE_FASHION_ITEM = 10082,	   //时装吸收
    ITEM_LOSE_FASHION_PART_UPGRADE = 10083, // 时装部位升级
    ITEM_LOSE_SPAR_REFINE = 10084,              // 晶石精练
    ITEM_LOSE_CARD_MAKE = 10085,                // 卡牌制作
    ITEM_LOSE_SPAR_SMELT = 10086,              // 晶石熔炼

    ITEM_LOSE_RUNE_STONE_ESSENCE_EXCHANGE = 10087, // 符石精华兑换
    ITEM_LOSE_GEMSTONE_CARVE = 10088, // 宝石雕刻
    ITEM_LOSE_MOUNT_FUSE = 10089,				// 坐骑融合

	ITEM_LOSE_OPER_ACTIVITY = 10090,//运营活动
	ITEM_LOSE_CARD_POTEN_UPGRADE = 10091, //卡牌升金

    ITEM_LOSE_MAX = 20000,                       //无
};

enum Item_Move_Type {
    ITEM_MOVE_TEST = 20001,                      //测试移动
    ITEM_MOVE_PACK_STORE = 20002,                //从人物背包到人物仓库
    ITEM_MOVE_STORE_PACK = 20003,                //从人物仓库到人物背包
    ITEM_MOVE_PACK_EQUIP = 20004,                //从人物背包到人物装备 -- 穿
    ITEM_MOVE_EQUIP_PACK = 20005,                //从人物装备到人物背包 -- 脱
    ITEM_MOVE_PACK_HERO = 20006,                 //从人物背包到英雄装备 -- 穿
    ITEM_MOVE_HERO_PACK = 20007,                 //从英雄装备到人物背包 -- 脱
    ITEM_MOVE_PACK_RUNE_STONE = 20008,           //从符石背包到人物符石装备 -- 穿
    ITEM_MOVE_RUNE_STONE_PACK = 20009,           //从人物符石装备到符石背包 -- 脱
    ITEM_MOVE_PACK_HERO_RUNE_STONE = 20010,      //从符石背包到英雄符石装备 -- 穿
    ITEM_MOVE_HERO_RUNE_STONE_PACK = 20011,      //从英雄符石装备到符石背包 -- 脱
    ITEM_MOVE_PACK_MEDAL = 20012,                //从人物背包到勋章 -- 穿
    ITEM_MOVE_MEDAL_PACK = 20013,                //从勋章到人物背包 -- 脱

    ITEM_MOVE_SAME_PACK = 20020,                 //同类型背包内移动
};

enum Item_Addit_Update_Type {
	ITEM_ADDIT_UPDATE_SPIRIT = 30001,            //元神附加信息更新
};

enum Auto_Buy_Type {
    AUTO_BUY_TRANSFER = 1,                       //传送
    AUTO_BUY_HORN = 2,                           //喇叭使用
    AUTO_BUY_OPEN_VIP = 3,                       //开通vip
    AUTO_BUY_UPDATE_VIP = 4,                     //升级vip
    AUTO_BUY_CONTINUE_VIP = 5,                   //续费vip
    AUTO_BUY_EQUIP_INCREASE = 6,                 //装备强化
    AUTO_BUY_EQUIP_ADDITION = 7,                 //装备洗炼
    AUTO_BUY_MOUNT_TRAIN = 8,                    //坐骑培养
    AUTO_BUY_PET_MOOD = 9,                       //宠物心情增加
    AUTO_BUY_PET_GROWTH = 10,                    //宠物成长
    AUTO_BUY_PET_PURIFY = 11,                    //宠物洗髓
    AUTO_BUY_MOUNT_EVOLVE = 12,                  //坐骑进阶
    AUTO_BUY_RELIVE = 13,                        //自动复活
    AUTO_BUY_INHERIT = 14,                       //装备传承
};

// 增加money bind_copper/copper/...
enum Money_Type {
	COPPER = 501502,    		//金币
	GOLD = 501602,      		//钻石
	BIND_GOLD = 501702, 		//粉钻
	DRAGON_SOUL = 502803, 	// 龙魂
	SOULS = 502903,     		//英灵
	FRIENDSHIP = 503403,		//友情值
	REPUTATION = 503603, 	//声望
	MONEY_TYPE_END
};

// 使用money的方式
enum Money_Sub_Type {
	COPPER_ONLY = 1,
	BIND_GOLD_FIRST = 2,
	GOLD_ONLY = 3,
	BIND_GOLD_ONLY = 4,
	SOULS_ONLY = 5,
	FRIENDSHIP_ONLY = 6,
	DRAGON_SOUL_ONLY = 7,
	REPUTATION_ONLY = 8,
	MONEY_SUB_TYPE_END
};

// 若已经调用了TRY的接口了，则不需要在内部重复调用
enum Pack_Try {
	WITH_TRY = 0,
	WITHOUT_TRY = 1
};

enum Money_Opt_Type {
	MONEY_OPT_TRY,
	MONEY_OPT_REAL,
};

enum Money_Tips {
	MONEY_TIPS_YES,
	MONEY_TIPS_NO
};

enum {
	MAX_LOCK_SEC = 5
};

// 由于存在交易的情况，不能依据SEQ是否为0来判断是否需要生成SEQ，故用枚举指定是否生成SEQ
enum Seq_Type {
	GENERATE_SEQ,
	DONT_GENERATE_SEQ
};

// 一行有7个背包格子
const int PACKAGE_GRID_PER_LINE = 7;
const int STORAGE_GRID_PER_LINE = 8;
const int PACKAGE_ITEM_MAX_AMOUNT = 500;

} // namespace pack


// 定义系统提示的类型
enum TIPS_TYPE {
	/*预留10个位置放置使用金钱相关 不要随意改变顺序（需要跟背包内金钱类型顺序一致）*/
	TIP_VALUE 				= 1,		/// 提示值
	USE_BIND_COPPER 		= 121,		/// 使用绑定铜钱
	USE_COPPER 				= 122,		/// 使用铜钱
	USE_COUPON 				= 123,		/// 使用礼券
	USE_GOLD 				= 124,		/// 使用元宝
	USE_GOLD_BOND 			= 125,		/// 使用金券
	USE_MALL_SCORE 			= 126,		/// 使用商城积分
	USE_MILITARY_EXPLOIT	= 127,		/// 使用战功
	USE_XIANJU_MONEY		= 128,		/// 使用积分

	/*预留10个位置放置获得金钱相关*/
	GET_BIND_COPPER 		= 131,		/// 获得绑定铜钱
	GET_COPPER 				= 132,		/// 获得铜钱
	GET_COUPON 				= 133,		/// 获得礼券
	GET_GOLD 				= 134,		/// 获得元宝
	GET_GOLD_BOND 			= 135,		/// 获得金券
	GET_MALL_SCORE 			= 136,		/// 获得商城积分
	GET_MILITARY_EXPLOIT	= 137,		/// 获得战功
	GET_XIANJU_MONEY		= 138,		/// 获得积分

	USE_ITEM 				= 141,		/// 使用物品
	GET_ITEM 				= 142,		/// 获得物品
	GET_EXP 				= 143,		/// 获得经验
	LEVEL_UP 				= 144,		/// 升级
	GET_BLOOD 				= 145,		/// 补充血量
	GET_MAGIC				= 146,		/// 补充法力
	GET_HURT				= 147,		/// 受到伤害
	OUT_HURT				= 148,		/// 造成伤害
	OUT_HIT					= 149,		/// 造成暴击伤害

	LOSE_ITEM				= 150,		/// 失去物品
	SELL_ITEM				= 151,		/// 卖出物品
	DISCARD_ITEM			= 152,		/// 丢弃物品

	USE_CONTRIBUTION		= 170,		/// 使用帮派贡献
	GET_CONTRIBUTION		= 171,		/// 获得帮派贡献
	GET_COMPREHENSION		= 172,		/// 获得悟性
	USE_COMPREHENSION		= 173,		/// 使用悟性
	GET_CARD_SOUL			= 174,		/// 获得卡牌魂魄
	GET_FACTION_GOLD		= 175,		/// 获得荒金
	GET_PET_FAME			= 176,		/// 获得声望
	GET_FACTION_RESOURCES	= 177,		/// 获得帮派资源
	GET_FACTION_MONEY		= 178,		/// 获得帮派资金

	GET_QINGYIZHI			= 200,		/// 获得情义值
	GET_BLUE_SOUL			= 201,		/// 获得蓝魂值
	GET_PURPLE_SOUL			= 202,		/// 获得紫魂值

	GET_FRAGMENT			= 203,		/// 获得神器碎片
	LOSE_FRAGMENT			= 204,		/// 扣去神器碎片

	GET_CHENGJIU			= 210,		/// 获得成就

	GET_WXYL_LINGLI			= 220,		/// 补充五行元灵灵力
	LOSE_EQUIP_BREAKDOWN    = 221,		/// 装备分解
	LOSE_RUNE_STONE_ESSENCE_EXCHANGE   = 222,   ///符石精华兑换

	TIPS_NONE,
};

//-- 背包容量信息
enum {
	PACK_MAX_CAPACITY = 280,
	STORAGE_MAX_CAPACITY = 280,
	PACK_INIT_CAPACITY = 42,
	STORAGE_INIT_CAPACITY = 35,
	HERO_EQUIP_INIT_CAPACITY = 20,

	RUNE_STONE_EQUIP_MAX_CAP = 8,
	RUNE_STONE_EQUIP_INIT_CAP = 1,

	RUNE_STONE_PACK_MAX_CAP = 200,
	RUNE_STONE_PACK_INIT_CAP = 12,

	SPIRIT_EQUIP_INIT_CAPACITY = 2,
	SPIRIT_PACKAGE_INIT_CAPACITY = 16,

	THORLOTTERY_INIT_CAPACITY = 800,
	THORLOTTERY_MAX_CAPACITY = 800,

	CARD_BOX_INIT_CAPACITY = 1200,
	CARD_BOX_MAX_CAPACITY = 1200,
};

enum Bind_Verify {
	BIND_NONE = 0,
	BIND_ONLY = 1,
	UNBIND_ONLY = 2
};

enum Hole_State {
	STATE_NOT_OPEN = 0,
	STATE_OPEN,
};

enum Item_Vertify_By_Condition {
	CON_START			= 0,
	CON_OVERDUE			= 1,
};

#endif /* PACK_DEF_H_ */
