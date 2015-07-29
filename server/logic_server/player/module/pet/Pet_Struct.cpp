/*
 * Pet_Struct.cpp
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#include "Pet_Struct.h"
#include "DB_Operator.h"

Pet_Detail::~Pet_Detail(void) {

}

void Pet_Detail::reset(void) {

	role_id = 0;
	lv = 0;
	state = 0;
	upgrade_val = 0;
	active_illusion_id = 0;
	skill_map.clear();
	illusion_vec.clear();
}

int Pet_Detail::load(void) {

	return MONGO_OPERATOR->load_pet_data(*this);
}

int Pet_Detail::save(void) {

	return MONGO_OPERATOR->save_pet_data(*this);
}

int Pet_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}
int Pet_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}
