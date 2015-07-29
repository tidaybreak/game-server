/*
 * Item_User_Struct.h
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#ifndef ITEM_USER_STRUCT_H_
#define ITEM_USER_STRUCT_H_

#include "Server_Struct.h"
#include "Item_User_Def.h"

struct Item_User_Detail : public Detail {
	typedef std::vector<Time_Value> Item_Use_Time;

	Item_User_Detail();
	virtual ~Item_User_Detail();

	virtual int serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);
	virtual int load(void);
	virtual int save(void);
	virtual void reset(void);

	inline void save_tick(void) { is_change = true; };

	role_id_t role_id;
	Item_Use_Time can_use_time_;							// 物品可使用的时间
	Int_Double_Map extra_prop_;								// 物品附加属性
	std::map<role_id_t, int> item_had_used_times_;		// 物品使用次数,专指卡牌福袋，第一次特殊处理
};

#endif /* ITEM_USER_STRUCT_H_ */
