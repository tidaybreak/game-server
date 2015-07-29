/*
 * Ranking_Def.h
 *
 *  Created on: 2014-05-22
 *      Author: xiaoliang
 */

#ifndef RANKING_DEF_H_
#define RANKING_DEF_H_

namespace RankingDef {
	enum Ranking_Type {
		LOCAL_PERSONAL_LEVEL_RANKING = 102, 		 //个人排行(个人等级)本服
		LOCAL_PERSONAL_FORCE_RANKING = 101, 		 //个人排行(个人战斗力)本服
		LOCAL_PERSONAL_HEROER_RANKING = 103,		 //个人排行(武将排行)本服
		LOCAL_PERSONAL_CHARM_RANKING = 104,		 //个人排行(魅力排行)本服
		LOCAL_PERSONAL_FRIEND_VALUE_RANKING = 105,		 //个人排行(友情值排行)本服
		LOCAL_PERSONAL_MOUNT_RANKING = 107,//坐骑排行
		LOCAL_PERSONAL_WING_RANKING = 106,//翅膀排行

		LOCAL_EQUIPAGE_ARMS_RANKING = 201,  		 //装备排行(武器排行)本服
		LOCAL_EQUIPAGE_ARMOR_RANKING = 202, 		 //装备排行(防具排行)本服
		LOCAL_EQUIPAGE_ACCESSORIES_RANKING = 203,    //装备排行(饰品排行)本服
		LOCAL_EQUIPAGE_ARTIFACT_RANKING = 204,   	 //装备排行(神器排行)本服

		LOCAL_ARENA_SIGLE_RANKING = 301,           //竞技排行(单人竞技排行)本服   这个榜单客户端直接从竞技场取
		LOCAL_ARENA_TEAM_RANKING = 302,			   //竞技排行(组队竞技排行)本服	这个榜单已经不再需要

		LOCAL_DUNGEON_SIGLE_RANKING = 401,   	  //副本排行(单人副本)本服
		LOCAL_DUNGEON_SG_RANKING = 402,          //副本排行(上古副本)本服
		LOCAL_DUNGEON_SMD_RANKING = 403,          //副本排行(魔神殿)本服

		LOCAL_GANG_RANKING = 500,    	          //公会排行 本服

		LOCAL_ASSET_SIGLE_RANKING = 601,		 //财富排行(个人财富榜)本服
		LOCAL_ASSET_GANG_RANKING = 602,			 //财富排行(公会财富榜)本服
	};
	enum Save_Data_Type {
		SAVE_RANKING	= 10001,
		SAVE_EQUIPMENT_CHANGE	= 10002,
	};

	enum Change_Type {
		CHANGE_UPDATE	= 11,
		CHANGE_DELETE	= 12,
	};
};

#endif /* EXAMPLE_DEF_H_ */
