/*
 * Exchange_Struct.h
 *
 *  Created on: 2014年6月25日
 *      Author: Linqiyou
 */

#ifndef EXCHANGE_STRUCT_H_
#define EXCHANGE_STRUCT_H_

#include "Server_Struct.h"

struct Exchange_Detail : Detail{
	Exchange_Detail(void);
	virtual ~Exchange_Detail(void);
	void reset(void);

	int serialize(Block_Buffer &buf) const;
	int deserialize(Block_Buffer &buf);

	int load(void);
	int save(void);

	role_id_t role_id;
	Int_Int_Map ex_data;
	Int_Int_Map ex_data_every_day;
};

#endif /* EXCHANGE_STRUCT_H_ */
