/*
 * Chat_Player.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#include "Chat_Player.h"
#include "Err_Code.h"
#include "Chat_Monitor.h"
#include "Chat_Client_Request.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"

Chat_Player::Chat_Player() {
	// TODO Auto-generated constructor stub
	monitor_ = CHAT_MONITOR;
	//聊天间隔时间检测
//	channel_limit_time.resize(10, Time_Value::zero);
//	channel_limit_time[1] = Time_Value(3,0);			//世界聊天间隔3秒
//	channel_last_chat_time.resize(10, Time_Value::zero);
}

Chat_Player::~Chat_Player() {
	// TODO Auto-generated destructor stub
}

int Chat_Player::send_to_client(Block_Buffer &buf, bool use_swap) {
	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(0, role_id());
		ret = send_to_gate(buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(0, role_id());
		ret = send_to_gate(tmp_buf, true);
	}

	return ret;
}

int Chat_Player::send_to_gate(Block_Buffer &buf, bool use_swap) {
	return monitor_->send_to_monitor(monitor_link_.gate_muid, buf);
}

int Chat_Player::send_to_logic(Block_Buffer &buf, bool use_swap) {
	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.logic_muid, buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.logic_muid, tmp_buf, true);
	}

	return ret;
}

int Chat_Player::send_to_scene(Block_Buffer &buf, bool use_swap) {
	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.scene_muid, buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.scene_muid, tmp_buf, true);
	}

	return ret;
}

int Chat_Player::send_err_to_client(int error_id, int source) {
	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(source);
	buf.write_string("");
	buf.finish_message();
	return send_to_client(buf);
}

int Chat_Player::system_chat(std::vector<Chat_Content *> &contents){
	MSG_83000000 scMsg;
	scMsg.chat_type = chat_system;
	scMsg.system_type = 4;
	scMsg.role_type = player_self()->base_detail().role_type;

	for (std::vector<Chat_Content *>::iterator it = contents.begin(); it != contents.end(); ++it) {
		scMsg.content.push_back(**it);
	}
	Block_Buffer buf;
	buf.make_message(ACTIVE_CHAT_BOARD);
	scMsg.serialize(buf);
	buf.finish_message();
	return monitor()->send_to_all_players(buf);
}

void Chat_Player::sign_in(void) {

}

void Chat_Player::reset(void) {
	base_detail_.reset();
	monitor_link_.reset();
}

role_id_t Chat_Player::role_id(void) {
	return base_detail_.role_id;
}

uint8_t Chat_Player::gender(void) {
	return base_detail_.gender;
}

int Chat_Player::level(void) {
	return base_detail_.level;
}

int Chat_Player::career(void) {
	return base_detail_.career;
}

std::string Chat_Player::role_name(void) {
	return base_detail_.role_name;
}

int Chat_Player::vip(void) {
	Time_Value now = Time_Value::gettimeofday();
	if (base_detail_.vip_over_time <= now)
		return 0;
	return base_detail_.vip;
}

void Chat_Player::set_vip(int32_t over_time, int32_t vip_level) {
	base_detail_.vip_over_time.sec(over_time);
	base_detail_.vip = vip_level;
}

void Chat_Player::set_level(int32_t level) {
	base_detail_.level = level;
}

void Chat_Player::sign_out(void) {
	//todo 登出状态

	//todo 各模块退出处理(退出场景等)

	//todo 同步数据到逻辑

}

void Chat_Player::load_detail(Block_Buffer &buf) {
	monitor_link_.deserialize(buf);
	base_detail_.deserialize(buf);

	this->deserialize_black_list(buf);
}

void Chat_Player::module_init(void) {
	Chater::module_init();
}

int Chat_Player::client_ready_now(void) {
	Chater::client_ready_now();
	return 0;
}

int Chat_Player::set_scene_muid(Monitor_Unique_ID &muid) {
	monitor_link_.scene_muid = muid;
	base_detail_.location.scene_id = muid.id;
	base_detail_.location.scene_order = muid.order;
	return 0;
}

int Chat_Player::set_role_name(const std::string &new_name) {
	base_detail_.role_name = new_name;
	return 0;
}

int Chat_Player::modify_gender(const uint8_t gender) {
	base_detail_.gender = gender;
	return 0;
}

int Chat_Player::broadcast_buf(Block_Buffer &buf) {
	return player_self()->monitor()->send_to_all_players(buf);
}

//bool Chat_Player::check_chat_frequency(uint8_t chat_type) {
//	if(channel_limit_time[chat_type] == Time_Value::zero ||
//			channel_last_chat_time[chat_type] == Time_Value::zero )
//	{
//		return true;
//	}
//	if( Time_Value::gettimeofday() - channel_last_chat_time[chat_type] >=
//			channel_limit_time[chat_type])
//	{
//		return true;
//	}
//	return false;
//}
//
//void Chat_Player::set_chat_time(uint8_t chat_type)
//{
//	if(channel_limit_time[chat_type] != Time_Value::zero)
//	{
//		channel_last_chat_time[chat_type] = Time_Value::gettimeofday();
//	}
//}
