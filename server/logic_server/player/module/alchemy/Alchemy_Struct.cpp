/*
 * Alchemy._Struct.cpp
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#include "Alchemy_Struct.h"
#include "Alchemy_Def.h"
#include "DB_Operator.h"
#include "Alchemy_Config.h"

Alchemy_Detail::Alchemy_Detail() {
    reset();
}

Alchemy_Detail::~Alchemy_Detail() {

}

void Alchemy_Detail::reset() {
	role_id = 0;
	current_hero_critical.reset();
	current_hero_gold.reset();
	used_free_bout = 0;
	used_pay_bout = 0;
	last_tabs = 0;
	six_critical_num = 0;

	current_souls_hero_critical.reset();
	current_souls_hero_gold.reset();
	used_souls_free_bout = 0;
	used_souls_pay_bout = 0;
	last_souls_tabs = 0;
	six_critical_souls_num = 0;
}

int Alchemy_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Alchemy_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Alchemy_Detail::save(void) {
	if (this->role_id)
		MONGO_OPERATOR->save_alchemy_detail(*this);
	return 0;
}

int Alchemy_Detail::load(void) {
	return MONGO_OPERATOR->load_alchemy_detail(*this);
}
