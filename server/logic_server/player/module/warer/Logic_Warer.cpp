/*
 * Logic_Warer.cpp
 *
 *  Created on: 2014年7月8日
 *      Author: root
 */

#include "Logic_Warer.h"
#include "Logic_Warer_Struct.h"


Logic_Warer::Logic_Warer(void):warer_detail_(0){

}

Logic_Warer::~Logic_Warer(void){

}

void Logic_Warer::reset(void){
	warer_detail_ = 0;
}

void Logic_Warer::load_detail(Warer_Detail &detail){
	warer_detail_ = &detail;
}
void Logic_Warer::save_detail(Warer_Detail &detail){
	if(warer_detail_->is_change){
		detail = *warer_detail_;
		warer_detail_->is_change = false;
	}
}

void Logic_Warer::daily_refresh(void){

}

int Logic_Warer::trigger_daily_zero(const Time_Value &now){
	return 0;
}

int Logic_Warer::trigger_daily_six(const Time_Value &now){
	daily_refresh();
	return 0;
}

void Logic_Warer::module_init(void){

}

int Logic_Warer::create_init(Warer_Detail& detail){
	return 0;
}

void Logic_Warer::sign_in(void){

}

void Logic_Warer::sign_out(void){

}
