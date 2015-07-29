/*
 * Logic_Heroer_Def.h
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#ifndef LOGIC_HEROER_DEF_H_
#define LOGIC_HEROER_DEF_H_

enum {
	HERO_INDEX_BEGIN = 1001,
	HERO_INDEX_END = 2000,

	MAX_HERO_LVL = 100,
	MAX_PANAL_HERO_TAG = 7,			// 角色面板英雄标签最大数
	HERO_ID_LIN = 60210021,			// 琳的配置ID
	MAX_HERO_LVL_GTE_ROLE = 5,		// 英雄最多超过主角X级
};

enum Hero_Detail_Type {
	OUTER_HERO_ONLY = 1,	// 只是跟随英雄
	FIGHT_HERO_ONLY = 2,	// 只是战斗英雄
	OUTER_FIGHT_HERO = 3,	// 既是跟随英雄，又是战斗英雄
};

enum Active_Type {
	AT_HERO_LEVEL_UP = 1,
	AT_HERO_AWAKE = 2,
	AT_HERO_ARTIFACT_UPGRADE = 3,
};

enum {
	MAX_ASSIST_HERO = 2,
};

#endif /* LOGIC_HEROER_DEF_H_ */
