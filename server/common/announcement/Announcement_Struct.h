/*
 * Announcement_Struct.h
 *
 *  Created on: May 12, 2014
 *      Author: caijianwei
 */

#ifndef ANNOUNCEMENT_STRUCT_H_
#define ANNOUNCEMENT_STRUCT_H_

// 1：世界 2：公会；3:组队；4：系统 5：喇叭 6:私聊
enum ANNOUNCE_DISTRICT_ID {
	ANNOUNCE_DISTRICT_WORLD = 1,
	ANNOUNCE_DISTRICT_GANG	= 2,
	ANNOUNCE_DISTRICT_TEAM	= 3,
	ANNOUNCE_DISTRICT_SYSTEM = 4,
	ANNOUNCE_DISTRICT_TRUMPET = 5,
	ANNOUNCE_DISTRICT_PRIVATE = 6,
	ANNOUNCE_DISTRICT_END
};

enum ANNOUNCE_ID {
	ANNOUNCE_ID_RANK_UPDATED 		= 1001,			// 排行榜信息已更新
	ANNOUNCE_ID_SERVER_MAINTAIN 	= 1101, 			// 服务器维护
	ANNOUNCE_ID_CREATE_GANG 		= 1203,			// 创建了新的公会
	ANNOUNCE_ID_ARENA_RISE  		= 1204,     	// 竞技场晋级黄金段位
	ANNOUNCE_ID_WAR_SCENE_BEGIN 	= 1205,			// 战场公告
	ANNOUNCE_ID_WAR_SCENE_END   	= 1220,			// 战场公告
	ANNOUNCE_ID_RUNE_STONE_SMELT	= 1221,			// 符石熔炼
	ANNOUNCE_ID_BONFIRE_READY	   = 1381,			// 篝火晚会准备开启
	ANNOUNCE_ID_BONFIRE_START		= 1322,			// 篝火晚会开启
	ANNOUNCE_ID_BONFIRE_START_1	= 1382,			// 篝火晚会开启1
	ANNOUNCE_ID_BONFIRE_FINISH		= 1323,			// 篝火晚会结束
	ANNOUNCE_ID_WAR_START			= 1325,			//	战场活动开启
	ANNOUNCE_ID_WAR_FINISH  		= 1326,        // 战场活动结束
	ANNOUNCE_ID_VIP_CLUB_NUM		= 1330,			// VIP等级人数改变到指定数量 开服活动
	ANNOUNCE_ID_ACTIVE_GANG_LEVEL	= 1331,			// 公会等级提升 开服活动
	ANN_ID_EQUIP_ENCHANT				= 1222,			// 装备熔炼
	ANN_ID_GANG_DRAW					= 1315,			// 公会抽奖
	ANN_ID_GANG_WAR					= 1324,			// 公会开始广播
	ANN_ID_TEAM_SPORTS				= 1327,			// 组队竞技开始广播
	ANN_ID_INIT_OPEN_VIP4			= 1333,			// 开通VIP4增加全服广播
	ANNOUNCE_ID_SECRET_SHOP_1   	= 1350,			// 神秘商店1出现
	ANNOUNCE_ID_SECRET_SHOP_2   	= 1351,			// 神秘商店2出现
	ANNOUNCE_ID_HANDSEL_ALL_COVENANT   = 1353,			//handsel精灵盟约
	ANNOUNCE_ID_OPEN_ALL_COVENANT   = 1354,			//全部精灵盟约
	ANNOUNCE_ID_ACTIVE_CONTENT_COUNT = 1352,			//开服活动结算
	ANNOUNCE_ID_BUY_PROMO_GOODS 	= 1355,			//抢购了打折商品
	ANN_ID_TITLE					= 1356,			// 称号
	ANN_ID_ACHIEVEMENT				= 1357,			// 成就
	ANNOUNCE_PROTECT_SELF_DRAGON_VALE = 1360,       //成功守护龙谷
	ANNOUNCE_DRAGON_EVOLUTION_SUCCESS = 1361,       //龙进化
	ANN_ID_RUNE_FILL_ENERGY_1		= 1363,			// 符文充能（宝石升级）
	ANN_ID_RUNE_FILL_ENERGY_2		= 1364,			// 符文充能（宝石升级）
	ANN_ID_RUNE_FILL_ENERGY_3		= 1365,			// 符文充能（宝石升级）
	ANN_ID_ARTIFACT_UPGRADE			= 1366,			// 神器升级
	ANN_ID_MASTER					= 1367,			// 精通

	ANN_ID_THORLOTTERY_ITEM			= 1369, 	//
	ANN_ID_THORLOTTERY_EXCHANGER	= 1370,

	ANN_ID_GANG_JOIN_IN					= 1371,		// 加入公会
	ANN_ID_GANG_HEADSHIP_APPOINT		= 1372,		// 公会职位任命
	ANN_ID_GANG_QUIT					= 1373,		// 退出公会
	ANN_ID_GANG_KICK					= 1374,		// 踢出公会
	ANN_ID_GANG_UPGRADE					= 1375,		// 公会升级
	ANN_ID_GANG_RESIGN_TO				= 1376,		// 公会转让
	ANN_ID_GANG_SEIZE_POWER				= 1377,		// 公会夺权
	ANN_ID_GANG_RANK_CHANGE				= 1378,		// 公会排名变化
	ANN_ID_GANG_FUND_LACK				= 1379,		// 公会维护费用不足
	ANN_ID_GANG_ANNOUNCE_SET			= 1380,		// 公会公告修改
	ANN_ID_DRAGON_VALE_SPECIAL_ONLINE 				= 1383,      //龙谷至尊宝上线
	ANN_ID_WING_ACTIVE								= 1390,		// 激活翅膀
	ANN_ID_WING_UPGRADE								= 1391,		// 升阶翅膀
	ANN_ID_EXPEDITION_DEFEND						= 5001,		// 防守通知
	ANN_ID_EXPEDITION_START							= 5005,		// start
	ANN_ID_EXPEDITION_TIMING_MONSTER_REFRESH		= 5006,		// timing monster refresh
	ANN_ID_EXPEDITION_TIMING_MONSTER_KILLED			= 5007,		// timing monster killed
	ANN_ID_EXPEDITION_BOX_OPEN						= 5008,		// open box

	ANN_ID_CHALLENGE 					= 6001,     //擂台
	ANN_ID_ANSWER_RANK_FIRST					= 1421,     //智慧试炼

	ANN_ID_BATTLE_IN_BONFIRE_SCENE		= 1422,		// 篝火场景战斗
	ANN_ID_VIP_TURNTABLE		= 1424,		// VIP转盘广播
	ANN_ID_TITLE_NOBILITY		= 9101,		// 爵位
	ANN_ID_GEM_FOUR		        = 9012,		// 四级宝石合成广播
	ANN_ID_GEM_FIVE			    = 9013,		// 五级以上宝石合成广播
	ANN_ID_GOLD_SYN			    = 9014,		// 金装合成广播
	ANN_ID_GEM_CARVE_TEN			    = 9015,		// 10级宝石雕刻广播
	ANN_ID_GEM_CARVE_TWENTY             = 9016,		// 20级宝石雕刻广播
	ANN_ID_GEM_CARVE_THIRTY			    = 9017,		// 30级宝石雕刻广播
	ANN_ID_OPEN_LOTTERY			        = 9018,		// 开礼包获取道具
	ANN_ID_CARD_MAKE			        = 9019,		// 卡牌制作转化广播

};

#endif /* ANNOUNCEMENT_STRUCT_H_ */
