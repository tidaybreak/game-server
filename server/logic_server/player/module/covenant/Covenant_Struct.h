/*
 * Covenant_Struct.h
 *
 *  Created on: 2014年7月15日
 *      Author: xiaoliang
 */

#ifndef COVENANT_STRUCT_H_
#define COVENANT_STRUCT_H_

#include "Server_Struct.h"
struct Covenant_Data {
	uint32_t id;/*盟约ID*/
	uint8_t init_status;/*初始状态,0未开通过, 1开通过*/
	Time_Value over_time;/*过期时间*/
	Time_Value today_become_time;/*特殊精灵,在当天有持续时间, 表示当天开启后持续时间过期时间*/
	Covenant_Data(void) { reset(); };
	void reset(void);
};
typedef boost::unordered_map<uint32_t, Covenant_Data> Covenant_Data_Map;
struct Covenant_Detail: public Detail {
	Covenant_Detail(void);
	virtual ~Covenant_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	Covenant_Data_Map covenant_data_map;
};

#endif /* COVENANT_STRUCT_H_ */
