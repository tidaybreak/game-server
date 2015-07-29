/*
 * Activity_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef ACTIVITY_STRUCT_H_
#define ACTIVITY_STRUCT_H_

#include "Server_Struct.h"

struct Activity_Detail: public Detail {
	Activity_Detail(void);
	virtual ~Activity_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	UInt_Set vip37_receive_id;
	bool is_receive_37phone;
	int32_t vip37_level;

	bool is_rec_r2_gift;
};

struct Activity_Info {

};

#endif /* ACTIVITY_STRUCT_H_ */
