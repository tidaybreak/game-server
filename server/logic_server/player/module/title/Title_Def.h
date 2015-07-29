/*
 * Title_Def.h
 *
 *  Created on: 2014年7月26日
 *      Author: Linqiyou
 */

#ifndef TITLE_DEF_H_
#define TITLE_DEF_H_

enum {
	NULL_TITLE = 0,				//已被删除的空称号
	RANK_TITLE = 1,				//排名称号
	GROW_TITLE = 2,				//成长称号
	TEMP_TITLE = 3,				//临时称号
	INNER_TITLE = 4,			//内部称号
};

enum Title_Type {
	TITLE_ROLE_LEVEL_FIRST 			= 10090001,				//等级排行第1名
	TITLE_ROLE_LEVEL_SECOND 		= 10090002,				//等级排行第2~10名
	TITLE_ROLE_COMBAT_FIRST 		= 10080001,				//战斗力排行榜第1名
	TITLE_ROLE_COMBAT_SECOND 		= 10080002,				//战斗力排行榜第2~10名
	TITLE_ROLE_GLAMOUR_FIRST 		= 10180001,				//魅力值排行榜第1名
	TITLE_ROLE_GLAMOUR_SECOND 		= 10180002,				//魅力值排行榜第2~10名
	TITLE_ROLE_FRIENDLY_FIRST 		= 10190001,				//友情值排行榜第1名
	TITLE_ROLE_FRIENDLY_SECOND 		= 10200001,				//友情值排行榜第2~10名

	TITLE_ARENA_RANK_FIRST			= 10070001,				//竞技场第一名
	TITLE_ARENA_RANK_SECOND			= 10070002,				//竞技场2-10

	TITLE_TEAM_SPORTS_RANK_FIRST	= 10060001,				//组队竞技第一名
	TITLE_TEAM_SPORTS_RANK_SECOND	= 10060002,				//组队竞技2
	TITLE_TEAM_SPORTS_RANK_THIRD	= 10060003,				//组队竞技3
	TITLE_TEAM_SPORTS_RANK_FOUR		= 10060004,				//组队竞技4-10

	TITLE_FRIEND_20				= 91020005,				//呼朋唤友
	TITLE_FRIEND_50				= 91020006,				//高朋满座
	TITLE_FRIEND_200			= 91020007,				//天下谁人不识君
	TITLE_ZHIZUN_VIP			= 91020008,				//至尊vip

	TITLE_GM 					= 91040001,				//游戏管理员
	TITLE_DIRECTOR 				= 91040002,				//新手指导员

	TITLE_GANG_WAR_FIRST	 	= 10120001,				//第一会长
};


#endif /* TITLE_DEF_H_ */
