/*
 * Vip_Struct.h
 *
 *  Created on: 2014年7月21日
 *      Author: xiaoliang
 */

#ifndef VIP_STRUCT_H_
#define VIP_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "Vip_Config.h"
#include "pack/Pack_Struct.h"

typedef boost::unordered_map<uint32_t, World_Tree_Flock> World_Tree_Flock_Map;

struct Vip_Detail: public Detail {
	Vip_Detail(void);
	virtual ~Vip_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	uint32_t grow_val;   //当前成长值
	Time_Value last_settle_grow;    // 上一次结算成长值时间,初次开通会被赋予开通时间
	uint32_t cost_money; //消费累计额度(计算消费成长值)
	bool expired_prompt; //过期提示
	Time_Value exp_over_time; //体验过期时间

	World_Tree_Flock_Map world_tree_val;

	Turntable_Config turntable_data;
	uint32_t turntable_cost;
	Item_Vec turntable_reward;

	bool has_receive_daily_reward;

};

#endif /* VIP_STRUCT_H_ */
