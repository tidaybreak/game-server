/*
 * Example.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "Example.h"
#include "Example_Struct.h"
#include "Logic_Module_Head.h"

Example::Example(void) {
	reset();
}

Example::~Example() {
	// TODO Auto-generated destructor stub
}

void Example::reset(void) {
	example_detail_ = 0;
}

int Example::create_init(Example_Detail &detail) {

	return 0;
}

void Example::load_detail(Example_Detail &detail) {
	example_detail_ = &detail;
}

void Example::save_detail(Example_Detail &detail) {
	if (example_detail().is_change) {
		detail = example_detail();
		example_detail_->is_change = false;
	}
}

int Example::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Example::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Example::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Example::move_scene_completed(void) {

	return 0;
}

int Example::client_ready_now(void) {

	return 0;
}

void Example::module_init(void) {

}

void Example::sign_in(void) {

}

void Example::sign_out(void) {

}

void Example::trusteeship_sign_out(void) {

}

int Example::sign_in_repeat(void) {

	return 0;
}
