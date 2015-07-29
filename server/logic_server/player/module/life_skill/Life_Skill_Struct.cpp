/*
 * Life_Skill_Struct.cpp
 *
 *  Created on: 2015年3月10日
 *      Author: zhoujin
 */

#include "Life_Skill_Struct.h"
#include "DB_Operator.h"


int Life_Skill_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Life_Skill_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Life_Skill_Detail::save(void) {
    if ( is_change )
    	return MONGO_OPERATOR->save_life_skill_data(*this);
	return 0;
}

int Life_Skill_Detail::load(void) {

	return MONGO_OPERATOR->load_life_skill_data(*this);
}

void Life_Skill_Detail::reset(void) {
	is_change = false;
	energy = 0;
	remain_time = 0;
	inil = false;
	skill_map.clear();
}
