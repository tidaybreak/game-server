/*
 * Logic_Warer_Struct.cpp
 *
 *  Created on: 2014年7月8日
 *      Author: root
 */

#include "Logic_Warer_Struct.h"
#include "DB_Operator.h"

Warer_Detail::Warer_Detail(void){
	reset();
}

Warer_Detail::~Warer_Detail(void){

}

void Warer_Detail::reset(void){

}

int Warer_Detail::serialize(Block_Buffer &buffer) const{
	return 0;
}
int Warer_Detail::deserialize(Block_Buffer &buffer){
	return 0;
}

int Warer_Detail::load(void){
	//return MONGO_OPERATOR->load_arenaer_detail(*this);
	return 0;
}

int Warer_Detail::save(void){
//	if(role_id != 0 && rank > 0){
//		return MONGO_OPERATOR->save_arenaer_detail(*this);
//	}
	return 0;
}
