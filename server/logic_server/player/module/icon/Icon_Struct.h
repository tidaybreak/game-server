/*
 * Icon_Struct.h
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#ifndef Icon_STRUCT_H_
#define Icon_STRUCT_H_

#include "Server_Struct.h"

struct Svr_Icon_Info {
	Svr_Icon_Info() {
		reset();
	}
	void reset() {
		id = 0;
		status = 0;
		is_ever_opened = false;
		is_used_item = false;
	}
	uint16_t id;//图标id
	uint8_t status;//开启状态。1-开启，0-未开启
	bool is_ever_opened;//是否开启过图标
	bool is_used_item;  //是否使用过道具
};


typedef boost::unordered_map<icon_id_t, Svr_Icon_Info> Icon_Info_Map;

struct Icon_Detail: public Detail {
	Icon_Detail(void);
	virtual ~Icon_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	Icon_Info_Map icon_map;

};

#endif /* Icon_STRUCT_H_ */
