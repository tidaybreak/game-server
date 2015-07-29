/*
 * logic_player_def.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef LOGIC_PLAYER_DEF_H_
#define LOGIC_PLAYER_DEF_H_

namespace Logic_Enum {
	// monitor type
	enum PLAYER_EVENT {
		EVENT_LOGIC_SAVE_DETAIL	= 1000000,				// 保存玩家数据
		EVENT_PUBLIC_FCM_OPERATING_1h = 1000001,      	//防沉迷操作1h
		EVENT_PUBLIC_FCM_OPERATING_2h = 1000002,      	//防沉迷操作2h
		EVENT_PUBLIC_FCM_OPERATING_255h = 1000003,      //防沉迷操作2.55h
		EVENT_PUBLIC_FCM_OPERATING_3h = 1000004,     	//防沉迷操作3h
		EVENT_LOGIC_TITLE_TIME = 1000005,				// 称号模块（称号过期）
		EVENT_LOGIC_EQUIPER_TICK = 1000006, 			// 装备定时
		EVENT_LOGIC_ENERGY_TICK = 1000007,              // 累计在线30分钟定时恢复精力
		EVENT_LOGIC_ENERGY_COMFIRE_TICK = 1000008,      // 在篝火舞会定时恢复精力
	};

};

#endif /* LOGIC_PLAYER_DEF_H_ */
