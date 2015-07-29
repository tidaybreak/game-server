/*
 * Helper_Struct.h
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 */

#ifndef HELPER_STRUCT_H_
#define HELPER_STRUCT_H_

#include "Server_Struct.h"
#include "Game_Typedef.h"

struct Helper_Detail: public Detail {
	Helper_Detail(void);
	virtual ~Helper_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	UInt_UInt_Map daily_num_map_;	//日常
	UInt_UInt_Map activity_num_map_; //活跃度

	uint32_t today_activity; //今日活跃度
	uint32_t all_activity;	//所有活跃度
	uint8_t receive_vip_activity; //是否领取VIP活跃度

	UInt_Set today_award_receive;	//今日活跃度已领
	UInt_Vec all_award_receive;		//所有活跃度已领

	int32_t last_join_arena_force;
	bool no_join_areana_tip;
	int32_t last_tip_force;

	uint8_t check_in_day; //累计签到天数
	Time_Value last_check_in_refresh_time; //上一次签到刷新时间
	uint8_t today_is_check_in;
};

#endif /* HELPER_STRUCT_H_ */
