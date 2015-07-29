/*
 * Task_Def.h
 *
 *  Created on: Jun 9, 2014
 *      Author: gaojianshan
 */

#ifndef TASK_DEF_H_
#define TASK_DEF_H_

struct TaskExecType {
	enum Exec_Type {
		TASK_EXEC_KILL_MONSTER = 111111,           // 杀怪
		TASK_EXEC_FB = 111112,                     // 完成副本任务
		TASK_EXEC_NPC_TALK = 111113,           // NPC对话
		TASK_EXEC_ARRIVE = 111114,                 // 到达目的地
		TASK_EXEC_LVL = 111115,                    // 等级
		TASK_EXEC_COLLECT = 111116,                // 采集物品
		TASK_EXEC_KILL_MONSTER_GROUP = 111117,     // 杀指定场景指定组怪
		TASK_EXEC_PACKAGE =  111118,               // 收集打怪掉落物品
		TASK_EXEC_FB_TYPE = 111119,                // 完成某类型副本任务
		TASK_EXEC_WIN_PLAYER =  111120,            // 击败玩家
		TASK_EXEC_JOIN_GANG = 111121,              //加入公会
		TASK_EXEC_GANGER_LEVEL = 111122,             //公会等级到达N级
		TASK_EXEC_PLAYER_ALL_EQUIP =  111123,      //玩家强化N次(没有装备、部位的限制)
		TASK_EXEC_BODY_EQUIP = 111124,        	   //强化指定部位N次
		TASK_EXEC_BODY_EQUIP_LEVEL = 111125,       //强化指定部位的装备到N级
		TASK_EXEC_EQUIP_LEVEL = 111126,            //强化指定装备到N级(有装备限制)
		TASK_EXEC_STONE_LEVEL = 111127,            //宝石提升到N级
		TASK_EXEC_BUY_GOODS = 111128,              //在商城购买某件物品  param1:物品id  如果param1配0，只要是商城购买的道具都算
		TASK_EXEC_GET_HERO = 111129,               //获得某个英雄
		TASK_EXEC_FIGHT = 111130,                  //挑战NPC(与NPC对话进入战斗)
        TASK_EXEC_COMMIT_CONSUME_ITEM = 111131,	   //任务提交时消耗XX道具（ID）XX个（数量）
        TASK_EXEC_DELIVER_LETTER = 111132,         //送信任务-领取任务时获得特定道具（ID），提交时需要扣除该道具（ID）
        TASK_EXEC_HELP_OTHER_COMPLETE_TEAM_SCENE = 111133, //协助其他玩家完成多人副本N次(对副本无限制，策划只配置次数)
        TASK_EXEC_SELL_ITEM = 111134,                      //出售X件道具（道具无限制，只检测出售个数）
        TASK_EXEC_ARENA = 111135,                    //竞技场战斗胜利
        TASK_EXEC_CHAT_WITH_FRIEND = 111136,         //与X个好友聊天(每个好友只要1次聊天即可)
        TASK_EXEC_SEND_MAIL = 111137,                //发送XXX封邮件(只限制封数，不限制给谁发)
        TASK_EXEC_TEAM_SCENE_OR_PVP = 111138,        //XXX次多人战斗胜利. 即:多人副本或者多人组队竞技胜利  注明： (1)多人副本只要胜利即可, 有收益、无收益均可. (2)多人组队竞技，只要胜利即可)
        TASK_EXEC_BROWSE_OTHER_PLAYER_INFO = 111139, //查看XXX个玩家的信息
        TASK_EXEC_ARENA_LEVEL = 111140,							 //指定组id的竞技场到XX段位(包括以上)
        TASK_EXEC_EQUIP_CRIMSON_LEVEL = 111141,					 //装备符文充能到XX级
        TASK_EXEC_ARENA_ALL = 111142,							 //参与XX次竞技场(不论胜负，单人竞技、组队竞技都可以)
        TASK_EXEC_CREATE_DRAGON_VALE_BUILDING = 111143,			 //指定类型的龙谷建筑建造XX个
        TASK_EXEC_GAIN_DRAGON_VALE_BUILIDING_PRODUCTION = 111144,//收获XX次指定类型的龙谷建筑资源
        TASK_EXEC_WAKE_UP_HERO_TO_QUALITY_LEVEL = 111145,	     //将指定英雄觉醒至指定的觉醒品质(如：蓝色、绿色)
        TASK_EXEC_KEEP_HERO_TO_LEVEL = 111146,					 //将指定英雄培养至指定的等级
        TASK_EXEC_COMPOSE_EQUIP = 111147, 						 //合成指定装备
        TASK_EXEC_HERO_DREAM_SCENE = 111148,					 //通关XX次英雄梦境
        TASK_EXEC_GANGER_SKILL_LEVEL = 111149,					     //指定公会技能提升到XX级(玩家学习的公会技能)
        TASK_EXEC_WAR = 111150,									 //参与并打赢战场
        TASK_EXEC_WAR_ALL = 111151,         //参与战场X次(打赢、打输都可以)
        TASK_EXEC_KILL_ENEMY_IN_WAR = 111152,//在战场中击杀X个敌人
        TASK_EXEC_SINGLE_ARENA_ALL = 111153,//单人竞技参与X次(打赢、打输都可以)
        TASK_EXEC_SINGLE_ARENA_WIN = 111154,//单人竞技参与并打赢X次
        TASK_EXEC_TEAM_ARENA_ALL = 111155,  //组队竞技参与X次(打赢、打输都可以)
        TASK_EXEC_TEAM_ARENA_WIN = 111156,  //组队竞技参与并打赢X次
        TASK_EXEC_GANGER_BLESS_BUF = 111157,//公会抽奖X次
        TASK_EXEC_ROB_OTHER_DRAGON_VALE_ALL = 111158,//龙谷掠夺X次(只要参与掠夺别人的龙谷战斗即可，不管输赢，不管是否掠夺资源)
        TASK_EXEC_DRAGON_VALE_STEP_FOOT_ALL = 111159,//龙谷踩脚印X次(只要踩脚印即可，不管是否提升星级)
        TASK_EXEC_ACTIVENESS_OF_TODAY = 111160,//当天活跃度达到X
        TASK_EXEC_DRAGON_VALE_BUILDING_UPGRADE = 111161,//龙谷建筑升级到X级
        TASK_EXEC_LIMIT_TIME_KILL_BOSS = 111162,//限时击杀boss
        TASK_EXEC_DRAGON_KEEP_ALL = 111163,//龙培养X次(只要花钱或者道具培养了，不管是否培养成功)
        TASK_EXEC_DRAGON_KEEP_SUCCESS = 111164,//龙培养X次(龙培养成功，培养到下一阶才算)
        TASK_EXEC_DONATE_TO_GANG = 111165,//公会捐献x次
        TASK_EXEC_PRESENT_BUF_TO_OTHER = 111166,//公会成员赠送buf给他人x次
        TASK_EXEC_FINISH_REWARD_TASK_NUM = 111167,//完成悬赏任务数量(不记忆历史完成的)
        TASK_EXEC_KEEP_DRAGON_TO_LEVEL = 111168,  //将龙培养到指定阶数
        TASK_EXEC_KNIGHT_TRIAL = 111169, //骑士试炼通关xx关
        TASK_EXEC_SUIT = 111170,         //某个套装的对应的部件全部穿在身上(玩家职业对应的可以穿的全部)  出战英雄的也算
        TASK_EXEC_ROLE_PULL_ON_EQUIP = 111171,//主角穿指定的装备
        TASK_EXEC_HERO_PULL_ON_EQUIP = 111172,//英雄穿指定的装备
        TASK_EXEC_ACTIVATE_WING = 111173,     //激活翅膀
        TASK_EXEC_GET_QUALITY_RUNE = 111174,  //获得x品质的符石xx个
        TASK_EXEC_PASS_KNGIHT_TRIALS = 111175, //通关骑士试炼x次(从第一关打到最后一关，所有关都打完才算通关一次)
        TASK_EXEC_HAVE_HERO = 111176,          //拥有多少个英雄
        TASK_EXEC_HAVE_HERO_QUALITY = 111177,  //拥有多少个x品质的英雄
        TASK_EXEC_FIRST_PASS_SCENE = 111178,   //首通单人副本、精英副本(接任务前通关过、接任务后通关都算) (不限制是第几次通关)
        TASK_EXEC_DRAGON_BORN = 111179,        //龙谷龙蛋孵化
        TASK_EXEC_HORSE_STAR_LEVEL = 111180,   //指定id的激活坐骑升到xx星级
        TASK_EXEC_HAVE_ITEM_BUT_NO_ERASE = 111181, //拥有某个道具(不包括符石)，仅限于人物背包段中，不扣道具
        TASK_EXEC_HAVE_EQUIP_BUT_NO_ERASE = 111182, //拥有某个装备(不包括符石)，主角穿戴的装备、英雄穿戴的装备、人物背包段中的未穿戴的装备，不扣除
        TASK_EXEC_SAME_GANG_CHALLENGE_ALL = 111183, //前往擂台与同公会玩家决斗X次(不论输赢，都算)
        TASK_EXEC_MAIN_SCENE_NPC_BATTLE = 111184,   //主城npc战斗任务
        TASK_EXEC_EQUIP_CARD_ALL = 111185,          //向任意装备镶嵌兵魂
        TASK_EXEC_EQUIP_CARD_BY_POSITION = 111186,  //向指定部位镶嵌兵魂
        TASK_EXEC_CARD_UPGRADE_TO_LEVEL_ALL = 111187,   //将任意兵魂升级到XX级(镶嵌到装备上的才算)
        TASK_EXEC_CARD_UPGRADE_TO_LEVEL_BY_POSITION = 111188, //将某一个部位兵魂升级到XX级(镶嵌到装备上的才算)
        TASK_EXEC_LOTTERY_DRAW = 111189, //博彩N次
        TASK_EXEC_EQUIP_JEWELRY = 111190, //穿戴4件紫色或紫色以上（金色）的首饰(主角、英雄)
        TASK_EXEC_INVITE_FRIEND_STEP_FOOT = 111191, //邀请好友踩脚印N次
        TASK_EXEC_KILL_GROUP_NUM = 111192, //击杀怪物组
        TASK_EXEC_DID_REWARD_TASK_NUM = 111193, //完成悬赏任务数量(记忆历史完成的)
        TASK_EXEC_ALCHEMY_NUM = 111194, //炼金X次
        TASK_EXEC_DID_REWARD_TASK_NUM_TODAY = 111195, //当日完成X次悬赏任务
        TASK_EXEC_REMOTE_SCENE_ANY_PASS = 111196, //通关任意一关上古副本
        TASK_EXEC_KNIGHT_TRIAL_ANY_PASS = 111197, //通关任意一关骑士试炼
        TASK_EXEC_COMPOSE_GEMSTONE_LEVEL = 111198, //合成一颗XX级宝石
        TASK_EXEC_INLAY_POSITION_GEMSTONE = 111199, //镶嵌某个部位一颗宝石
        TASK_EXEC_INLAY_ANY_GEMSTONE = 111200, //镶嵌任意一颗宝石
        TASK_EXEC_WORLD_BOSS = 111201,//参与世界boss
        TASK_EXEC_WORLD_BOSS_IMMEDIATE_REVIVE = 111202,//世界boss立即复活(花钱的才算)
        TASK_EXEC_ENCHANT_ANY_EQUIP = 111203,//附魔任意装备
        TASK_EXEC_INLAY_ID_GEMSTONE = 111204,//镶嵌指定id的宝石
        TASK_EXEC_GANG_WELL_RECOVER_SPIRIT = 111205, //公会体力泉恢复体力1次
        TASK_EXEC_EQUIP_PURPLE_RING = 111206, //穿上紫色及紫色以上的戒指
        TASK_EXEC_INLAY_LEVEL_GEMSTONE = 111207,//镶嵌某个等级及以上的宝石
        TASK_EXEC_UPGRADE_CARD = 111208, //吞噬卡牌n次
        TASK_EXEC_ROLE_EQUIP_4 = 111209, //主角穿戴4件某色或某色以上的装备
        TASK_EXEC_HERO_EQUIP_4 = 111210, //指定英雄穿戴4件某色或某色以上的装备
        TASK_EXEC_DAYLY_SINGLE_ARENA_ALL = 111211, //当天内单人竞技参与X次(打赢、打输都可以，接任务前的次数也算)

        //        TASK_EXEC_USE_SKILL = 111190, 					   //使用某个技能XX次（每个职业技能不一样，支持多种技能ID配置）(只限制次数，只要是数组中的任意一个技能即可)
        //        TASK_EXEC_APPEND_SKILL = 111191,                   //释放XX次追加技

	};
};

enum Sync_Item_Type_Operate {
	SYNC_TASK_SOURCE_ADD = 1,
	SYNC_TASK_SOURCE_DEL = 2,
	SYNC_TASK_SOURCE_TRANSMIT = 3,
};

//任务品质
enum Task_Quality {
	TASK_QUALITY_WHITE = 0,    //白色品质
	TASK_QUALITY_GREEN = 1,    //绿色品质
	TASK_QUALITY_BLUE = 2,     //蓝色品质
	TASK_QUALITY_PURPLE = 3,   //紫色品质
	TASK_QUALITY_ORANGE = 4,   //橙色品质
	TASK_QUALITY_END,
};

//任务流水状态
enum Task_Stream_Status {
	TASK_STREAM_ACCEPT = 1,//接受任务
	TASK_STREAM_FINISH = 2,//完成任务
	TASK_STREAM_COMMIT = 3,//提交任务
};

//环任务类型
enum Ring_Task_Type {
	RING_TASK_TYPE_NPC_TALK =  1,//npc对话
	RING_TASK_TYPE_COLLECT = 2,  //采集
	RING_TASK_TYPE_LIMIT_TIME_KILL_BOSS = 3, //限时击杀
	RING_TASK_TYPE_KILL_MONSTER = 4,  //杀怪
	RING_TASK_TYPE_COMMIT_CONSUME_ITEM = 5, //上缴
	RING_TASK_TYPE_DRAGON_VALE_OP = 6, //龙谷操作
};

struct Item_Chance {
	int32_t val_1;/**/
	int32_t val_2;/**/
	Item_Chance(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(val_1);
		w.write_int32(val_2);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(val_1)  ||  r.read_int32(val_2)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		val_1 = 0;
		val_2 = 0;
	}
};

inline std::size_t hash_value(const Item_Chance &Item_Chance) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int32_t>(Item_Chance.val_1));
    boost::hash_combine(seed, static_cast<int32_t>(Item_Chance.val_2));
    return seed;
}

inline bool operator==(const Item_Chance &a, const Item_Chance &b) {
	return a.val_1 == b.val_1 && a.val_2 == b.val_2;
}

typedef boost::unordered_set<Item_Chance> Item_Chance_Set;
typedef Hash_Map<int, Item_Chance_Set, NULL_MUTEX> Npc_Item_Chance_Set_Map; //key: npc_id
typedef std::multimap<int, Item_Chance> Npc_Item_Chance_Map;

struct Svr_Scene_Npc {
	Svr_Scene_Npc() {
		reset();
	}
	void reset() {
		scene_id = 0;
		npc_id = 0;
	}
	uint32_t scene_id;
	uint32_t npc_id;
};

inline std::size_t hash_value(const Svr_Scene_Npc &svr_scene_npc) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int32_t>(svr_scene_npc.scene_id));
    boost::hash_combine(seed, static_cast<int32_t>(svr_scene_npc.npc_id));
    return seed;
}

inline bool operator==(const Svr_Scene_Npc &a, const Svr_Scene_Npc &b) {
	return a.scene_id == b.scene_id && a.npc_id == b.npc_id;
}

typedef boost::unordered_set<Svr_Scene_Npc> Svr_Scene_Npc_Set;

#endif /* TASK_DEF_H_ */
