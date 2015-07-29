/*
 * Gate_Player.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#include "Gate_Player.h"
#include "Gate_Monitor.h"
#include "Config_Cache.h"

Gate_Player::Gate_Player() {
	cid_ = 0;
	monitor_ = GATE_MONITOR;
	recv_pace_off_ = false;
	battle_status_ = false;
	player_sign_out_ = false;
}

Gate_Player::~Gate_Player() {
	// TODO Auto-generated destructor stub
}

void Gate_Player::reset(void) {
	cid_ = 0;
	base_detail_.reset();
	muid_.reset();
	monitor_link_.reset();
	msg_detail_.reset();
	recv_pace_off_ = false;
	battle_status_ = false;

	player_sign_out_ = false;
	session_id_ = "";
}

int Gate_Player::send_to_scene(Block_Buffer &buf) {
	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.scene_muid, buf);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.scene_muid, tmp_buf);
	}

	return ret;
}

int Gate_Player::send_to_local_chat(Block_Buffer &buf) {
	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.local_chat_muid, buf);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.local_chat_muid, tmp_buf);
	}

	return ret;
}

int Gate_Player::abnormal_logout(void) {
	{
		Block_Buffer buf;
		buf.make_message(SERVER_INNER_ABNORMAL);
		buf.write_int32(1);
		buf.write_int64(role_id());
		buf.finish_message();
		this->send_to_logic(buf);
	}

	return 0;
}

int Gate_Player::send_to_inter_chat(Block_Buffer &buf) {
	return monitor_->send_to_monitor(monitor_link_.inter_chat_muid, buf);
}

int Gate_Player::send_to_client(Block_Buffer &buf) {
	return monitor_->send_to_client_by_cid(cid_, buf);
}

int Gate_Player::send_to_logic(Block_Buffer &buf) {
	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.logic_muid, buf);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.logic_muid, tmp_buf);
	}

	return ret;
}

int Gate_Player::echo_test_result(std::string result, role_id_t value) {


	return 0;
}

int Gate_Player::test_command(std::string cmd_string) {
	std::vector<std::string> cmd_tok;

	std::istringstream cmd_stream(cmd_string);
	std::copy(std::istream_iterator<std::string>(cmd_stream),
			std::istream_iterator<std::string>(), std::back_inserter(cmd_tok));

	std::cout << "cmd_string = [" << (cmd_string) << "]" << std::endl;
	if (cmd_tok.empty())
		return 0;

	// 0:关闭 1:全开 2:开放部分命令
	int command_mode = CONFIG_CACHE->server_maintainer_cache().command_mode;
	if (command_mode == 1) {
	} else if (command_mode == 2) {
		if (!CONFIG_CACHE->server_maintainer_cache().command.count(cmd_tok[0]))
			return echo_test_result("this command not able");
	} else {
		return echo_test_result("test command not able");
	}

	if(cmd_tok[0] == "g_server_status") {
		monitor()->message_unit()->server_status();
		return 0;
	}

	return 0;
}

int Gate_Player::load_detail(Block_Buffer &buf) {
	monitor_link_.deserialize(buf);
	base_detail_.deserialize(buf);

	return 0;
}

void Gate_Player::set_cid(int cid) {
	cid_ = cid;
}

void Gate_Player::set_recv_pack_off(bool value) {
	recv_pace_off_ = value;
}

void Gate_Player::set_battle_status(bool status) {
	battle_status_ = status;
}

int Gate_Player::set_scene_muid(Monitor_Unique_ID &muid) {
	monitor_link_.scene_muid = muid;
	base_detail_.location.scene_id = muid.id;
	base_detail_.location.scene_order = muid.order;
	return 0;
}

void Gate_Player::reset_msg_detail(void) {
	msg_detail_.reset();
}

Gate_Player *Gate_Player::player_self(void) {
	return this;
}

std::string Gate_Player::role_name(void) {
	return base_detail_.role_name;
}

std::string &Gate_Player::login_session(void) {
	return session_id_;
}

void Gate_Player::set_login_session(std::string &session) {
	session_id_ = session;
}

MSG_Detail &Gate_Player::msg_detail(void) {
	return msg_detail_;
}

role_id_t Gate_Player::role_id(void) {
	return base_detail_.role_id;
}

int Gate_Player::level(void) {
	return 0;
}

int Gate_Player::career(void) {
	return 0;
}

int Gate_Player::notice_logic_sign_out(int out_reason, int abnormal_time) {
	if (player_sign_out_) {
		MSG_DEBUG("has in sign out, role:%ld, reason:%d", role_id(), out_reason);
		return 0;
	}

	player_sign_out_ = true;

	set_recv_pack_off(true);

	Block_Buffer buf;
	buf.make_message(GATE_LOGIC_CLIENT_DROP);
	buf.write_int32(out_reason);
	buf.write_int32(abnormal_time);
	buf.finish_message();
	send_to_logic(buf);
	return 0;
}

int Gate_Player::verify_msg_detail(uint32_t msg_id, uint32_t serial_cipher, uint32_t msg_time_cipher) {
	if (! msg_detail_.is_inited) {
		//msg_detail_.hash_key = ELFHash(base_detail_.account_info.account_name.c_str(), base_detail_.account_info.account_name.length());
	}
	uint32_t serial = serial_cipher;// ^ msg_detail_.hash_key;
	uint32_t msg_time = msg_time_cipher;// ^ serial_cipher;

	Time_Value now = Time_Value::gettimeofday();
	if (! msg_detail_.is_inited) {
		msg_detail_.is_inited = true;
		msg_detail_.msg_serial = serial; /// 第一条消息序号
		msg_detail_.msg_timestamp = now; /// 第一条消息时间戳
		msg_detail_.msg_interval_count_ = 1;
		msg_detail_.msg_interval_timestamp = now;
		return 0;
	}

	/// 操作频率统计
	if (now - msg_detail_.msg_interval_timestamp < Time_Value(3, 0)) {
		if (msg_detail_.msg_interval_count_ > CONFIG_CACHE->server_maintainer_cache().package_frequency) {
			MSG_USER("role_id:%ld, msg_interval_count:%d", role_id(), msg_detail_.msg_interval_count_);
			for (Msg_Process_Info_Vec::iterator it = msg_detail_.msg_vec.begin(); it != msg_detail_.msg_vec.end(); ++it) {
				MSG_USER("msg = %d, msg_time = %d, server_time = %d", (*it).msg, (*it).msg_time, (*it).server_time);
			}
			msg_detail_.msg_vec.clear();
			return 10000116;//ERROR_CLOSE_CLIENT_TOO_OFTEN;
		}
	} else {
		msg_detail_.msg_interval_count_ = 0;
		msg_detail_.msg_interval_timestamp = now;
	}

	/// 判断包时间戳
	Time_Value msg_time_tv(msg_time, 0);
	int time_deviation = msg_time - now.sec();
	if (CONFIG_CACHE->server_maintainer_cache().verify_msg_time && (time_deviation > 5 || time_deviation < -25)) {
		if (time_deviation < -25) {
			MSG_USER("time error 1 role:%ld, msg_time = %d, msg_timestamp = %d, msg:%d", role_id(), msg_time, now.sec(), msg_id);
		} else {
			MSG_USER("time error 2 role:%ld, msg_time = %d, msg_timestamp = %d, msg:%d", role_id(), msg_time, now.sec(), msg_id);
		}
		for (Msg_Process_Info_Vec::iterator it = msg_detail_.msg_vec.begin(); it != msg_detail_.msg_vec.end(); ++it) {
			MSG_USER("msg = %d, msg_time = %d, server_time = %d", (*it).msg, (*it).msg_time, (*it).server_time);
		}
		msg_detail_.msg_vec.clear();
		return 10000118;//ERROR_CLOSE_CLIENT_MSG_TIME;
	}

	/// 判断包序号
	if (serial <= msg_detail_.msg_serial || serial - msg_detail_.msg_serial > 10) {
		MSG_USER("role_id:%ld, serial = %d, msg_detail_.msg_serial = %d", role_id(), serial, msg_detail_.msg_serial);
		return 10000117;//ERROR_CLOSE_CLIENT_MSG_SERIAL;
	}


	msg_detail_.msg_serial = serial;
	msg_detail_.msg_timestamp = msg_time_tv;
	++msg_detail_.msg_interval_count_;

	if (msg_detail_.msg_vec.size() > 30) {
		msg_detail_.msg_vec.pop_front();
	}
	msg_detail_.msg_vec.push_back(Msg_Process_Info(msg_id, msg_time, now.sec()));

	return 0;
}
