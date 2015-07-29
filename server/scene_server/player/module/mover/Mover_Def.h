/*
 * Mover_Def.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef MOVER_DEF_H_
#define MOVER_DEF_H_

enum Mover_Type {
	MOVER_T_PLAYER	           				= 0,   		 	//  *player

	MOVER_T_MONSTER		     				= 300,   		// 小怪
	MOVER_T_MONSTER_BOSS		  			= 301,			// 大boss
	MOVER_T_MONSTER_ELITE_BOSS		  		= 302,			// 精英boss
	MOVER_T_MONSTER_WAR_LIGHT_SRC			= 310,			// 战场光明回收官
	MOVER_T_MONSTER_WAR_LIGHT_GUARD			= 311,			// 战场光明守卫
	MOVER_T_MONSTER_WAR_LIGHT_REVIVE 		= 312,			// 战场光明复活官
	MOVER_T_MONSTER_WAR_DARK_SRC			= 320,			// 战场光黑暗收官
	MOVER_T_MONSTER_WAR_DARK_GUARD			= 321,			// 战场光黑暗守卫
	MOVER_T_MONSTER_WAR_DARK_REVIVE			= 322,			// 战场光黑暗复活官
	MOVER_T_MONSTER_CAMPFIRE_MATCHMAN		= 323,			//篝火晚会火柴人
	MOVER_T_MONSTER_CAMPFIRE_GODDESS		=	324,			//篝火晚会女神

	MOVER_T_MONSTER_END						= 399,			// 怪物分界点

	MOVER_T_DROP			    			= 400,   		// 掉落物

	MOVER_T_MATERIAL			  					= 500,			// 采集物
	MOVER_T_MATERIAL_WAR_MINE			  		= 501,			// 战场普通矿
	MOVER_T_MATERIAL_WAR_SPE_MINE				= 502,			// 战场紫晶矿
	MOVER_T_MATERIAL_WAR_REVIVE_BALL    	= 503,         // 战场复活加速球
	MOVER_T_MATERIAL_GANG_WAR_TOWER			= 504,			//公会战资源塔
	MOVER_T_MATERIAL_GANG_WAR_REVIVE_BALL	= 505,			// 公会战复活加速球
	MOVER_T_MATERIAL_CAMPFIRE_MEAT			= 506,			// 篝火晚会烤肉
	MOVER_T_MATERIAL_CAMPFIRE_FIRE			= 507,			// 篝火晚会篝火

	MOVER_T_HERO				  			= 600,			// 英雄

	MOVER_T_MACHINE			  				= 700,			// 机关

	MOVER_T_PLAYER_MONSTER    				= 1000,			// 竞技场镜像
};

enum Mover_Appear_Addition_Type {
	MOVER_APPEAR_ADDITION_WAR						= 100, 			// 战场附加出现信息
	MOVER_APPEAR_ADDITION_GANG_WAR				= 300, 			// 公会战附加出现信息
	MOVER_APPEAR_ADDITION_CAMPFIRE				= 400, 			// 篝火晚会篝火附加信息
	MOVER_APPEAR_ADDITION_CAMPFIRE_DANCING		= 401, 			// 篝火晚会人物附加信息
	MOVER_APPEAR_ADDITION_EXPEDITION			= 500,			// 远征血条
	MOVER_APPEAR_ADD_EXPE_DROPS_OR_MATERIAL		= 510,			// 远征drops or material
};


#endif /* MOVER_DEF_H_ */
