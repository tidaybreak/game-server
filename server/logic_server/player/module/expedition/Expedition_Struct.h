/*
 * Expedition_Struct.h
 *
 *  Created on: 2014年10月16日
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_STRUCT_H_
#define EXPEDITION_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "NPC/NPC_Struct.h"


struct Gang_Data_Info {
	Gang_Data_Info(void);
	void reset(void);
	uint32_t line;
	Coord coord;
};

struct Expedition_Detail: public Detail {
	Expedition_Detail(void);
	virtual ~Expedition_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	Coord coord;
	Int_Int_Map kill_beast;
	Int_Hash_Set collect_beast;
	Int_Hash_Set collect_chest;

	Time_Value award_time;
	Int_Time_Map box_collect;

	int award_nums;
	boost::unordered_map<int, Drop_Vec> drops_all_vec_;
};

struct Expedition_Manager_Detail {
typedef boost::unordered_map<int64_t, Gang_Data_Info> Int64_Gang_Data_Info_Map;
	Expedition_Manager_Detail(void) {
		reset();
	}
	void reset(void) {
		gang_data_info_map.clear();
		flag_time = Time_Value::zero;
		award.clear();
		award_time = Time_Value::zero;
		first_listen_time = Time_Value::zero;
		broadcast_announce.clear();
	}
	int load(void);
	int save(void);
	int serialize(Block_Buffer &w) const;
	int deserialize(Block_Buffer &r);

	Int64_Gang_Data_Info_Map gang_data_info_map;
	Time_Value flag_time;

	Int64_Item_Info_Vec_Map award;
	Time_Value award_time;
	Time_Value first_listen_time;

	Int_Time_Map broadcast_announce; // no save
};

#endif /* EXPEDITION_STRUCT_H_ */
