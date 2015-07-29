/*
 * Scene_Player_Detail.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *      只在场景服用
 */

#ifndef SCENE_PLAYER_DETAIL_H_
#define SCENE_PLAYER_DETAIL_H_

#include "Base_Detail.h"
#include "Fighter_Detail.h"
#include "mover/Mover_Struct.h"

struct Scene_Player_Detail {
	enum LOAD_STATUS {
		NULL_STUB,
		SUCCESS_LOADED,		/// 加载成功
		ROLE_NOT_EXIST,		/// 角色不存在
	};

	Scene_Player_Detail(void);
	void reset(void);

	role_id_t role_id;
	int status;

	Base_Detail base_detail;
	Fighter_Detail fighter_detail;
	Mover_Detail mover_detail;
};

#endif /* SCENE_PLAYER_DETAIL_H_ */
