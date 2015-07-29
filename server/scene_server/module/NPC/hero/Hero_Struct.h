/*
 * Hero_Struct.h
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#ifndef HERO_STRUCT_H_
#define HERO_STRUCT_H_

#include "Base_Struct.h"
#include "Server_Struct.h"

/// 英雄基本信息
struct Hero_Base_Detail {
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	role_id_t master_role_id;		// 主人id
	std::string master_role_name;	// 主人名称
	role_id_t hero_role_id;			// 英雄地图服id
	int hero_index;					// 英雄index
	int hero_id;					// 英雄ID
	std::string hero_name;			// 英雄name
	int hero_facade_id;				// 英雄facade_id
	int hero_formation;				// 英雄阵型
	int hero_level;					// 英雄等级
	int awake_lvl;					// 觉醒等级
	Position_Key pos_key;			// 战斗时阵营位置（进入战斗前确定）

};


#endif /* HERO_STRUCT_H_ */
