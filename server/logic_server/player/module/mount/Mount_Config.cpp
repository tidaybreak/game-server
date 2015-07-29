/*
 * Mount_Config.cpp
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */


#include "Mount_Config.h"
#include "status/Status_Struct.h"

Horse_Group_Config::Horse_Group_Config(void){
	reset();
}

void Horse_Group_Config::reset(void){
	group_id = 0;
	group.clear();
	skill = 0;
}

/////////////////////////////////////////////////////////////////////


int Horse_Base_Config::get_mod_id(const int train_lv){
	int model_id = 0;
	std::vector<Int_Int>::iterator it;
	for(it = mod_id.begin(); it != mod_id.end(); ++it){
		if(train_lv < it->val_1){
			break;
		}
		model_id = it->val_2;
	}
	return model_id;
}
