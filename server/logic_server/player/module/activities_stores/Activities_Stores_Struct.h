/*
 * Activities_Stores_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef ACTIVITIES_STORES_STRUCT_H_
#define ACTIVITIES_STORES_STRUCT_H_

#include "Server_Struct.h"

struct Activities_Stores_Detail: public Detail {
	Activities_Stores_Detail(void);
	virtual ~Activities_Stores_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	UInt_UInt_Map id_num_buy;
};

#endif /* ACTIVITIES_STORES_STRUCT_H_ */
