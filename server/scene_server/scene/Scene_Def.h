/*
 * Scene_Def.h
 *
 *  Created on: Mar 18, 2014
 *      Author: ti
 */

#ifndef SCENE_DEF_H_
#define SCENE_DEF_H_

#include "Record_Client.h"

enum Scene_Type {
	Scene_Type_Null 				= 0,		   	///
	Main_Scene_Type 				= 80100,	   	/// 普通场景
	FuBen_Scene_Type 				= 80201,	   	/// 单人副本
	Elite_Scene_Type        	= 80202, 	   /// 精英副本
	Team_Fb_Scene_Type   		= 80204, 	   /// 组队推图副本
	HERO_DREAM_Scene_Type  		= 80205, 	   /// 英雄梦境副本
	Gang_Manor_Snene_Type		= 80206,	   	/// 公会领地
	War_Scene_Type					= 80207,	   	/// 战场
	TEAM_DEFEND_Scene_Type		= 80208,			/// 守卫帝都
	World_Boss_Scene_Type 		= 80209,   		/// 世界BOSS
	Gang_Boss_Scene_Type  		= 80210, 		/// 公会BOSS
	Team_Scene_Type 				= 80211,	   	/// 组队场景\限时击杀
	Dragon_Vale_Type        	= 80212,       /// 龙谷
	GANG_WAR_TYPE					= 80213,	   	/// 公会战
	Campfire_Scene_Type			= 80214,	   	/// 篝火
	Task_Scene_Type 				= 80215,       /// 任务场景
	Expedition_Type 				= 80216,       /// 远征
	Answer_Type 					= 80217,       /// 答题
	Tbay_Scene_Type 				= 80218,			/// 宝藏海湾
	Honor_Arena_Scene_Type 		= 80219,			/// 荣誉竞技场
	Arena_Panel_Scene_Type          = 80220,       // 竞技大界面
	Killing_Scene_Type 		   = 80221,			/// 杀绵羊
	Scene_Type_MAX 				= 88888,		   ///

};

enum Scene_Manager_Enum {
	Scene_Multi						= 1,				/// 多人场景
	Scene_Single					= 2,				/// 单人场景
	Scene_Team						= 3,				/// 组队场景
	Scene_Gang_Manor				= 4,				/// 公会领地
	Scene_War						= 5,				/// 战场
	Scene_Dragon_Vale       	= 6,        	/// 龙谷场景
	Scene_Gang_War					= 7,				/// 公会战
	Scene_Campfire					= 8,				/// 篝火
	Scene_Task_Manager         = 9,		    	/// 任务场景
	Scene_Expedition_Manager   = 10,    		/// 远征
	Scene_Answer_Manager 	   = 11,    		/// 智慧试炼
	Scene_Team_Defend 	    	= 12,    		/// 守卫帝都
	Scene_World_Boss	 	    	= 13,    		/// 世界boss
	Scene_Gang_Boss	 	    	= 14,    		/// 公会boss
	Scene_Honor_Arena	 	    	= 15,    		/// 荣誉竞技场
	Scene_Tbay	 	    			= 16,    		/// 宝藏海湾
	Scene_Arena	 	    			= 17,    		/// 竞技场
	Scene_Killing              = 18, 			/// 杀绵羊
	Scene_Unknow,
};


#endif /* SCENE_DEF_H_ */
