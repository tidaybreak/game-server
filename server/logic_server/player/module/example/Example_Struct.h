/*
 * Example_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef EXAMPLE_STRUCT_H_
#define EXAMPLE_STRUCT_H_

#include "Server_Struct.h"

struct Example_Detail: public Detail {
	Example_Detail(void);
	virtual ~Example_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

};

#endif /* EXAMPLE_STRUCT_H_ */
