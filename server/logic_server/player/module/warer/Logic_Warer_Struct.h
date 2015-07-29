/*
 * Logic_Warer_Struct.h
 *
 *  Created on: 2014年7月8日
 *      Author: root
 */

#ifndef LOGIC_WARER_STRUCT_H_
#define LOGIC_WARER_STRUCT_H_


#include "Server_Struct.h"
#include "Msg_Struct.h"

struct Warer_Detail:public Detail{

	Warer_Detail(void);
	virtual ~Warer_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	int times;
};



#endif /* LOGIC_WARER_STRUCT_H_ */
