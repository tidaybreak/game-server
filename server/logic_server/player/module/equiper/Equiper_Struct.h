/*
 * Equiper_Struct.h
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#ifndef EQUIPER_STRUCT_H_
#define EQUIPER_STRUCT_H_

#include "Server_Struct.h"

struct Master_Part_Info_Detail {
	int32_t index;/*部位索引主角10001-10008 英雄1-8*/
	int32_t level;/*等级*/
	int32_t luck_value;/*当前幸运值*/
	int32_t fail_cnt;//失败次数
	Master_Part_Info_Detail(void) { reset(); };

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
typedef boost::unordered_map<int32_t, Master_Part_Info_Detail> Master_Part_Info_Map;

struct Equiper_Detail: public Detail {
	enum Part_Index {
		HERO_START	= 1001001,
		HERO_END	= 1001008,

		ROLE_START	= 10001,
		ROLE_END	= 10008,
	};
	Equiper_Detail(void);
	virtual ~Equiper_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	std::map<int, int> suit_cnt_map;
	Int_Set notified_overdue_idx;						// 已通知客户端的过期index set
	role_id_t role_id;
	Master_Part_Info_Map master_part_info;
	Int_Hash_Set gem_data;
	uint32_t carve_lvl;
	uint32_t gem_progress;
};

struct Equip_Force_Info {
	Equip_Force_Info(void) { reset(); };
	virtual ~Equip_Force_Info() { };
	void reset(void);
	double calculate_force(void);

	std::map<int, double> pt_val_map;
};

#endif /* EQUIPER_STRUCT_H_ */
