/*
 * Scene_Player_Base.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#include "Scene_Move_Base.h"

Scene_Move_Base::Scene_Move_Base() {
	// TODO Auto-generated constructor stub

}

Scene_Move_Base::~Scene_Move_Base() {
	// TODO Auto-generated destructor stub
}

void Scene_Move_Base::reset(void) {

}

void Scene_Move_Base::make_message(Block_Buffer &buf, int msg_id) {
	buf.write_uint32(0); /// length
	buf.write_uint32(msg_id);
	buf.write_int64(role_id());
}

int Scene_Move_Base::send_err_to_client(int error_id, int source) {

	return 0;
}

int Scene_Move_Base::send_to_gate(Block_Buffer &buf, bool use_swap) {

	return 0;
}

int Scene_Move_Base::send_to_logic(Block_Buffer &buf, bool use_swap) {

	return 0;
}

int Scene_Move_Base::send_to_client(Block_Buffer &buf, bool use_swap) {

	return 0;
}

int Scene_Move_Base::send_to_local_chat(Block_Buffer &buf, bool use_swap) {

	return 0;
}

void Scene_Move_Base::call_broadcast_around(Block_Buffer &buf) {
	return;
}

std::string Scene_Move_Base::role_name(void) {

	return "";
}

int Scene_Move_Base::career(void) {

	return 0;
}

int Scene_Move_Base::vip(void) {

	return 0;
}
