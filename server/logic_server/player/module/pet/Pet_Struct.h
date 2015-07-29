/*
 * Pet_Struct.h
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#ifndef PET_STRUCT_H_
#define PET_STRUCT_H_

#include "Server_Struct.h"

struct Pet_Detail : public Detail {

	Pet_Detail(void) { reset(); }
	virtual ~Pet_Detail(void);

	void reset(void);

	int load(void);
	int save(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int lv;
	int state;
	int upgrade_val;
	int active_illusion_id;
	Int_Int_Map skill_map;
	Int_Vec illusion_vec;
};

#endif /* PET_STRUCT_H_ */
