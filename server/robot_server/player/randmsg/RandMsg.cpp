/*
 * RandMsg.cpp
 *
 *  Created on: 2014-06-07
 *      Author: xiaoliang
 */

#include "RandMsg.h"
#include "Config_Cache.h"
#include "Config_Cache_Robot.h"
#include "Robot_Player.h"
#include "Robot_Msg_Struct.h"

RandMsg::RandMsg() {
	reset();
}

void RandMsg::reset(void) {
	send_interval = CONFIG_CACHE_ROBOT->robot_config().send_msg_time;
	last_send_time = Time_Value::zero;
	msg_vec = get_robot_msg_map();
}

RandMsg::~RandMsg() {
	// TODO Auto-generated destructor stub
}

void RandMsg::send(Time_Value &now) {
	if(now >= (last_send_time + send_interval)) {
		last_send_time = now;
		if(CONFIG_CACHE_ROBOT->robot_config().send_mode) {

		} else {
			rand_send();
		}
	}
}
void RandMsg::rand_send(void) {
	Robot_Player *player = (Robot_Player*)this;
	int num = rand()%msg_vec->size();
	Robot_Base_Msg *msg = (*msg_vec)[num];
	msg->set_rand();
	Block_Buffer buf;
	buf.make_client_message(msg->msg_id, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg->serialize(buf);
	buf.finish_message();
	player->send_to_server(buf);
	//MSG_USER("%s >< 发送随机消息号:%d", player->base_detail().account.c_str(), msg->msg_id);
}

