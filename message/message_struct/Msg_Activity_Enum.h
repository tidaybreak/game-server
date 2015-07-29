/*  
* Generate by devtool
*/

#ifndef _MSG_ACTIVITY_H_ENUM_
#define _MSG_ACTIVITY_H_ENUM_

enum Msg_Activity_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_ACTIVE_CONTENT_INFO = 10106400,				//请求活动信息         

		RES_ACTIVE_CONTENT_INFO = 50106400,				//请求活动信息(返回)         

		REQ_RECEIVE_VIP_CLUB_REWARD = 10106401,				//请求领取VIP俱乐部奖励         

		RES_RECEIVE_VIP_CLUB_REWARD = 50106401,				//请求领取VIP俱乐部奖励(返回)         

		REQ_RECEIVE_LEVEL_ACE_REWARD = 10106402,				//请求领取冲级高手奖励         

		RES_RECEIVE_LEVEL_ACE_REWARD = 50106402,				//请求领取冲级高手奖励(返回)         

		REQ_RECEIVE_SUIT_COLLECT_REWARD = 10106403,				// 请求领取套装收集奖励         

		RES_RECEIVE_SUIT_COLLECT_REWARD = 50106403,				// 请求领取套装收集奖励(返回)         

		REQ_RECEIVE_GUILD_LEVEL_REWARD = 10106404,				//请求领取公会升级奖励         

		RES_RECEIVE_GUILD_LEVEL_REWARD = 50106404,				//请求领取公会升级奖励(返回)         

		REQ_ACTIVE_CONTENT_TIP_NUM = 10106405,				//请求活动信息可领取TIP数         

		RES_ACTIVE_CONTENT_TIP_NUM = 50106405,				//请求活动信息可领取TIP数(返回)         

		REQ_ACTIVE_CONTENT_LIMIT_BUY_INFO = 10106420,				//请求活动信息-限时购买         

		REQ_ACTIVE_CONTENT_VIP_CLUB_INFO = 10106421,				//请求活动信息-vip俱乐部         

		REQ_ACTIVE_CONTENT_LEVEL_ACE_INFO = 10106422,				//请求活动信息-冲级高手         

		REQ_ACTIVE_CONTENT_SUIT_COLLECT_INFO = 10106423,				//请求活动信息-套装收集         

		REQ_ACTIVE_CONTENT_GUILD_LEVEL_INFO = 10106424,				//请求活动信息-公会升级          

		REQ_ACTIVE_CONTENT_LEVEL_RANK_INFO = 10106425,				//请求活动信息-等级排名         

		REQ_ACTIVE_CONTENT_ARENA_RANK_INFO = 10106426,				//请求活动信息-竞技场排名         

		REQ_ACTIVE_CONTENT_FORCE_RANK_INFO = 10106427,				//请求活动信息-战斗力排名         

		RES_ACTIVE_CONTENT_LIMIT_BUY_INFO = 50106420,				//请求活动信息-限时购买(返回)         

		RES_ACTIVE_CONTENT_VIP_CLUB_INFO = 50106421,				//请求活动信息-vip俱乐部(返回)         

		RES_ACTIVE_CONTENT_LEVEL_ACE_INFO = 50106422,				//请求活动信息-冲级高手(返回)         

		RES_ACTIVE_CONTENT_SUIT_COLLECT_INFO = 50106423,				//请求活动信息-套装收集(返回)         

		RES_ACTIVE_CONTENT_GUILD_LEVEL_INFO = 50106424,				//请求活动信息-公会升级(返回)         

		RES_ACTIVE_CONTENT_LEVEL_RANK_INFO = 50106425,				//请求活动信息-等级排名(返回)         

		RES_ACTIVE_CONTENT_ARENA_RANK_INFO = 50106426,				//请求活动信息-竞技场排名(返回)         

		RES_ACTIVE_CONTENT_FORCE_RANK_INFO = 50106427,				//请求活动信息-战斗力排名(返回)         

		REQ_ACTIVE_CONTENT_GEM_LEVELUP_INFO = 10106428,				//请求活动信息-宝石升级         

		RES_ACTIVE_CONTENT_GEM_LEVELUP_INFO = 50106428,				//请求活动信息-宝石升级(返回)         

		REQ_RECEIVE_GEM_LEVELUP_REWARD = 10106406,				//请求领取宝石升级奖励         

		RES_RECEIVE_GEM_LEVELUP_REWARD = 50106406,				//请求领取宝石升级奖励(返回)         

		REQ_RECEIVE_WING_LEVELUP_REWARD = 10106407,				//请求领取翅膀升级奖励         

		RES_RECEIVE_WING_LEVELUP_REWARD = 50106407,				//请求领取翅膀升级奖励(返回)         

		REQ_ACTIVE_CONTENT_WING_LEVELUP_INFO = 10106429,				//请求活动信息-翅膀升级         

		RES_ACTIVE_CONTENT_WING_LEVELUP_INFO = 50106429,				//请求活动信息-翅膀升级(返回)         

		REQ_ACTIVE_CONTENT_MOUNT_LEVELUP_INFO = 10106430,				//请求活动信息-坐骑升级         

		RES_ACTIVE_CONTENT_MOUNT_LEVELUP_INFO = 50106430,				//请求活动信息-坐骑升级(返回)         

		REQ_RECEIVE_MOUNT_LEVELUP_REWARD = 10106408,				//请求领取坐骑升级奖励         

		RES_RECEIVE_MOUNT_LEVELUP_REWARD = 50106408,				//请求领取坐骑升级奖励(返回)         

		REQ_ACTIVE_CONTENT_SG_RANK_INFO = 10106431,				//请求活动信息-上古副本         

		RES_ACTIVE_CONTENT_SG_RANK_INFO = 50106431,				//请求活动信息-上古副本(返回)         

		REQ_RECEIVE_SG_PASS_REWARD = 10106409,				//请求领取上古排行通关奖励         

		RES_RECEIVE_SG_PASS_REWARD = 50106409,				//请求领取上古排行通关奖励(返回)         

		REQ_ACTIVE_CONTENT_ARENA_DAN_INFO = 10106432,				//请求活动信息-竞技场段位         

		RES_ACTIVE_CONTENT_ARENA_DAN_INFO = 50106432,				//请求活动信息-竞技场段位(返回)         

		REQ_RECEIVE_ARENA_DAN_REWARD = 10106410,				//请求领取竞技场段位奖励         

		RES_RECEIVE_ARENA_DAN_REWARD = 50106410,				//请求领取竞技场段位奖励(返回)         

		REQ_ACTIVE_CONTENT_WORLD_BOSS_INFO = 10106433,				//请求活动信息-世界boss         

		REQ_ACTIVE_CONTENT_SUIT_COLLECT_TWO_INFO = 10106434,				//请求活动信息-套装收集二段         

		REQ_ACTIVE_CONTENT_HEROES_DOOR_INFO = 10106435,				//请求活动信息-英雄之门         

		REQ_ACTIVE_CONTENT_TEAM_ARENA_INFO = 10106436,				//请求活动信息-组队竞技         

		REQ_ACTIVE_CONTENT_WARER_INFO = 10106437,				//请求活动信息-战场         

		REQ_ACTIVE_CONTENT_REWARD_INFO = 10106438,				//请求活动信息-悬赏         

		REQ_ACTIVE_CONTENT_SGFB_INFO = 10106439,				//请求活动信息-上古副本         

		RES_ACTIVE_CONTENT_WORLD_BOSS_INFO = 50106433,				//请求活动信息-世界boss(返回)         

		RES_ACTIVE_CONTENT_SUIT_COLLECT_TWO_INFO = 50106434,				//请求活动信息-套装收集二段(返回)         

		RES_ACTIVE_CONTENT_HEROES_DOOR_INFO = 50106435,				//请求活动信息-英雄之门(返回)         

		RES_ACTIVE_CONTENT_TEAM_ARENA_INFO = 50106436,				//请求活动信息-组队竞技(返回)         

		RES_ACTIVE_CONTENT_WARER_INFO = 50106437,				//请求活动信息-战场(返回)         

		RES_ACTIVE_CONTENT_REWARD_TASK_INFO = 50106438,				//请求活动信息-悬赏(返回)         

		RES_ACTIVE_CONTENT_SGFB_INFO = 50106439,				//请求活动信息-上古副本(返回)         

		REQ_RECEIVE_ACTIVE_CONTENT_REWARD = 10106411,				//请求领取活动奖励(公用1)         

		RES_RECEIVE_ACTIVE_CONTENT_REWARD = 50106411,				//请求领取活动奖励(公用1)(返回)         

		REQ_ACTIVE_CONTENT_FM_X_CI = 10106440,				//请求活动信息-累计附魔X次         

		RES_ACTIVE_CONTENT_FM_X_CI = 50106440,				//请求活动信息-累计附魔X次(返回)         

		REQ_ACTIVE_CONTENT_X_X_QUALITY_HEROER_INFO = 10106441,				//请求活动信息-累计X个X品质英雄         

		RES_ACTIVE_CONTENT_X_X_QUALITY_HEROER_INFO = 50106441,				//请求活动信息-累计X个X品质英雄(返回)         

		REQ_ACTIVE_CONTENT_MOUNT_LVUP_X = 10106442,				//请求活动信息-坐骑进阶X次         

		RES_ACTIVE_CONTENT_MOUNT_LVUP_X = 50106442,				//请求活动信息-坐骑进阶X次(返回)         

		REQ_ACTIVE_CONTENT_FUSION_X_ELITE_CARD = 10106443,				//请求活动信息-合成X张精英卡牌         

		RES_ACTIVE_CONTENT_FUSION_X_ELITE_CARD = 50106443,				//请求活动信息-合成X张精英卡牌(返回)         

		REQ_ACTIVE_CONTENT_ALCHEMY_GIFT = 10106444,				//请求活动信息-炼金有礼         

		RES_ACTIVE_CONTENT_ALCHEMY_GIFT = 50106444,				//请求活动信息-炼金有礼(返回)         

		REQ_ACTIVE_CONTENT_ACTIVITY_GIFT = 10106445,				//请求活动信息-活跃度好礼         

		REQ_ACTIVE_CONTENT_RING_TASK = 10106446,				//请求活动信息-环任务活动         

		REQ_ACTIVE_CONTENT_DRAGON_PREY = 10106447,				//请求活动信息-龙谷掠夺活动         

		RES_ACTIVE_CONTENT_DRAGON_PREY = 50106447,				//请求活动信息-龙谷掠夺活动(返回)         

		RES_ACTIVE_CONTENT_RING_TASK = 50106446,				//请求活动信息-环任务活动(返回)         

		RES_ACTIVE_CONTENT_ACTIVITY_GIFT = 50106445,				//请求活动信息-活跃度好礼(返回)         

		REQ_ACTIVE_CONTENT_MOUNT_RANK_INFO = 10106448,				//请求活动信息-坐骑排名         

		REQ_ACTIVE_CONTENT_WING_RANK_INFO = 10106449,				//请求活动信息-翅膀排名         

		RES_ACTIVE_CONTENT_WING_RANK_INFO = 50106449,				//请求活动信息-翅膀排名(返回)         

		RES_ACTIVE_CONTENT_MOUNT_RANK_INFO = 50106448,				//请求活动信息-坐骑排名(返回)         

		REQ_RECEIVE_MOUNT_PASS_REWARD = 10106412,				//请求领取坐骑排行通关奖励         

		REQ_RECEIVE_WING_PASS_REWARD = 10106413,				//请求领取翅膀排行通关奖励         

		RES_RECEIVE_WING_PASS_REWARD = 50106413,				//请求领取翅膀排行通关奖励(返回)         

		RES_RECEIVE_MOUNT_PASS_REWARD = 50106412,				//请求领取坐骑排行通关奖励(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ACTIVITY_H_ENUM_ */
