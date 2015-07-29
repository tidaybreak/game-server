/*
 * Logic_Status_Struct.h
 *
 *  Created on: 2014年6月12日
 *      Author: Linqiyou
 */

#ifndef LOGIC_STATUS_STRUCT_H_
#define LOGIC_STATUS_STRUCT_H_

#include "Server_Struct.h"
#include "status/Status_Struct.h"

struct Logic_Status_Detail : public Detail {
	typedef boost::unordered_map<int32_t, Status_Info> Status_Map;
	Logic_Status_Detail();
	virtual ~Logic_Status_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	inline void save_tick(void) { is_change = true; };

	role_id_t role_id;
//	Status_Detail status_detail;

	Status_Map status_map;
	Status_Running_Effect running_effect;
};


#endif /* LOGIC_STATUS_STRUCT_H_ */
