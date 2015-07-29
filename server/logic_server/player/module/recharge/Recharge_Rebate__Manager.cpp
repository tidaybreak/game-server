/*
 * Recharge_Rebate_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Recharge_Rebate_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"

Recharge_Rebate_Manager::Recharge_Rebate_Manager() {
	reset();
}

Recharge_Rebate_Manager::~Recharge_Rebate_Manager() {

}

void Recharge_Rebate_Manager::reset(void) {
	icon_time_status.reset();
	icon_limit_level = 0;
	start_time = Time_Value::zero;
	end_time = Time_Value::zero;
	act_id = 0;
}

void Recharge_Rebate_Manager::open_icon(const uint32_t icon_id,const Time_Value &close_time, const uint32_t level) {
	icon_time_status.icon_id = icon_id;
	icon_time_status.over_time = close_time;
	icon_limit_level = level;
	//效果buf
	MSG_81000408 effec_msg;
	effec_msg.open = 1;
	effec_msg.icon_id = icon_id;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服效果,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(icon_limit_level);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}
void Recharge_Rebate_Manager::close_icon(uint32_t icon_id) {
	icon_time_status.reset();
	//效果buf
	MSG_81000408 effec_msg;
	effec_msg.open = 0;
	effec_msg.icon_id = icon_id;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服效果,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(0);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}

Icon_Time_Info &Recharge_Rebate_Manager::get_icon_time_status(void) {
	return icon_time_status;
}

uint32_t Recharge_Rebate_Manager::get_icon_limit_level(void) {
	return icon_limit_level;
}

void Recharge_Rebate_Manager::open(const Time_Value &start, const Time_Value &close, const uint32_t act_id) {
	start_time = start;
	end_time = close;
	this->act_id = act_id;
}

const uint32_t Recharge_Rebate_Manager::get_act_id(void) {
	return act_id;
}

void Recharge_Rebate_Manager::close(const Time_Value &close) {
	end_time = close;
}

const Time_Value &Recharge_Rebate_Manager::get_start_time(void) {
	return start_time;
}

const Time_Value &Recharge_Rebate_Manager::get_end_time(void) {
	return end_time;
}
