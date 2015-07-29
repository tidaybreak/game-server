/*
 * First_Recharge_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "First_Recharge_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "DB_Def.h"

First_Recharge_Manager::First_Recharge_Manager() {
	reset();
}

First_Recharge_Manager::~First_Recharge_Manager() {

}

void First_Recharge_Manager::reset(void) {
	icon_time_status.reset();
	icon_limit_level = 0;
	start_time = Time_Value::zero;
	end_time = Time_Value::zero;
}

void First_Recharge_Manager::tick(const Time_Value &now) {

}

void First_Recharge_Manager::open_icon(const uint32_t icon_id,const Time_Value &close_time, const uint32_t level) {
//	icon_time_status.icon_id = icon_id;
//	icon_time_status.over_time = close_time;
//	icon_limit_level = level;
//	int time =  close_time.sec() - Time_Value::gettimeofday().sec();
//	if(time < 0) { time = 0; }
//	//效果buf
//	MSG_81000408 effec_msg;
//	effec_msg.open = 1;
//	effec_msg.icon_id = icon_id;
//	effec_msg.active = 0;
//	effec_msg.time = time;
//	Block_Buffer effec_buf;
//	effec_buf.make_message(effec_msg.msg_id);
//	effec_msg.serialize(effec_buf);
//	effec_buf.finish_message();
//	//全服效果,由聊天服转发
//	Block_Buffer buf;
//	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
//	buf.write_int32(icon_limit_level);
//	buf.copy(&effec_buf);
//	buf.finish_message();
//	LOGIC_MONITOR->send_to_local_chat(buf);
}
void First_Recharge_Manager::close_icon(uint32_t icon_id) {
//	icon_time_status.reset();
//	//效果buf
//	MSG_81000408 effec_msg;
//	effec_msg.open = 0;
//	effec_msg.icon_id = icon_id;
//	Block_Buffer effec_buf;
//	effec_buf.make_message(effec_msg.msg_id);
//	effec_msg.serialize(effec_buf);
//	effec_buf.finish_message();
//	//全服效果,由聊天服转发
//	Block_Buffer buf;
//	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
//	buf.write_int32(0);
//	buf.copy(&effec_buf);
//	buf.finish_message();
//	LOGIC_MONITOR->send_to_local_chat(buf);
}

Icon_Time_Info &First_Recharge_Manager::get_icon_time_status(void) {
	return icon_time_status;
}

uint32_t First_Recharge_Manager::get_icon_limit_level(void) {
	return icon_limit_level;
}

void First_Recharge_Manager::open(const Time_Value &start, const Time_Value &close) {
	start_time = start;
	end_time = close;
}

void First_Recharge_Manager::close(const Time_Value &close) {
	end_time = close;
//	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
//	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
//		it->second->first_over_receive_to_mail();
//	}
}

const Time_Value &First_Recharge_Manager::get_start_time(void) {
	return start_time;
}

const Time_Value &First_Recharge_Manager::get_end_time(void) {
	return end_time;
}
