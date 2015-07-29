/*
 * Core_Struct.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ti
 */

#include "Server_Struct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////

Account_Key::Account_Key() : account_name(""), agent_num(0), server_num(0) {}
Account_Key::Account_Key(const std::string account_p, const int agent_num_p, const int server_num_p) {
	account_name = account_p;
	agent_num = agent_num_p;
	server_num = server_num_p;
}

void Account_Key::reset(void) {
	account_name = "";
	agent_num = 0;
	server_num = 0;
}

int Account_Key::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account_name);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	return 0;
}

int Account_Key::deserialize(Block_Buffer &buffer) {
	buffer.read_string(account_name);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

Server_Key::Server_Key() : agent_num(0), server_num(0) {}
Server_Key::Server_Key(const int agent_num_p, const int server_num_p) {
	agent_num = agent_num_p;
	server_num = server_num_p;
}

void Server_Key::reset(void) {
	agent_num = 0;
	server_num = 0;
}

int Server_Key::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	return 0;
}

int Server_Key::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

Position_Key::Position_Key() : pos(0), idx(0) {}
Position_Key::Position_Key(const int p_p, const int idx_p) {
	pos = p_p;
	idx = idx_p;
}

void Position_Key::reset(void) {
	pos = 0;
	idx = 0;
}

int Position_Key::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(pos);
	buffer.write_int32(idx);
	return 0;
}

int Position_Key::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(pos);
	buffer.read_int32(idx);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

Scene_Key::Scene_Key() {
	reset();
}

Scene_Key::Scene_Key(const int scene_id_, const int type_, const int sub_type_) :
		scene_id(scene_id_), sort(type_), sub_sort(sub_type_) {}

void Scene_Key::reset(void) {
	scene_id = 0;
	sort = 0;
	sub_sort = 0;
}

//////////////////////////////////////////////////////////////////////////////////////

Role_Key::Role_Key() {
	reset();
}

Role_Key::Role_Key(const int career_, const int gender_) {
	career = career_;
	gender = gender_;
}

void Role_Key::reset(void) {
	career = 1;
	gender = 1;
}

//////////////////////////////////////////////////////////////////////////////////////


Create_Scene_Args::Create_Scene_Args(void) {
	reset();
}

Create_Scene_Args::Create_Scene_Args(int type_, role_id_t id_val1, int i_val1, int i_val2, int i_val3, bool i_val4) {
	type = type_;
	id = id_val1;
	sort = i_val1;
	sub_sort = i_val2;
	int_value = i_val3;
	slave = i_val4;
}

void Create_Scene_Args::reset(void) {
	time = Time_Value::zero;
	type = 0;
	id = 0;
	sort = 0;
	sub_sort = 0;
	int_value = 0;
	slave = false;
}

int Create_Scene_Args::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(type);
	buffer.write_int64(id);
	buffer.write_int32(sort);
	buffer.write_int32(sub_sort);
	buffer.write_int32(int_value);
	buffer.write_bool(slave);
	return 0;
}

int Create_Scene_Args::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(type);
	buffer.read_int64(id);
	buffer.read_int32(sort);
	buffer.read_int32(sub_sort);
	buffer.read_int32(int_value);
	buffer.read_bool(slave);
	return 0;
}


void IP_Info::reset(void) {
	telecom_ip.clear();
	unicom_ip.clear();
}

//////////////////////////////////////////////////////////////////////////////////////

void Server_Maintainer_Cache::reset(void) {
	game_open_time = Time_Value::zero;
	game_merge_time = Time_Value::zero;
	server_identity = 0;
	game_server_flag = "";
	platform_des = "";
	server_close_delay = 0;
	game_solution_id = 0;
	game_inter_flag = 0;
	game_port_prefix = 0;
	game_ver_type = 0;
	db_work_num = 0;
	record_id = 0;
	record_output_type = 0;
	local_log_level = 0;
	record_log_level = 0;
	package_frequency = 90;
	md5_key = "";
	verify_msg_time = false;
	config_not_update = true;
	inner_tick_time_ = Time_Value(10);
	inner_time_out_ = Time_Value(0, 50000);
	command_mode = 0;
	command.clear();
	valgrind_allow = false;

	chat_monitor_url.clear();
	chat_monitor_key.clear();
	pt_server_id.clear();

	mongo_ip = "";
	mongo_port = 0;

	mysql_ip = "";
	mysql_port = 0;
	mysql_user = "";
	mysql_pw = "";
	mysql_dbname.clear();

	merge_level_lt_ = 0;
	merge_login_day_ = 0;

	default_solution_id = 0;
	center_url = "";
}

void Mysql_Conf::reset(void) {
	mysql_ip = "";
	mysql_port = 0;
	mysql_user = "";
	mysql_pw = "";
	mysql_dbname = "";
}

void Monitor_Acceptor::reset(void) {
	monitor_type = Server_Enum::MONITOR_END;
	client_port_suffix = 0;
	client_port = 0;
	inner_port_suffix = 0;
	inner_port = 0;
	client_recv_timeout  = Time_Value(60,    0);		// 超时断开时间
	inner_recv_timeout 	 = Time_Value::zero;			// 内部不超时

}

void Monitor_Info::reset(void) {
	command = "";
	name = "";
	run_on = 0;
	index = 0;
	check = "null";
	acceptor_list.clear();
}

void Server_Solution_Cache::reset(void) {
	solution_id = 0;
	monitor_list.clear();
	inter_server.clear();
}

bool Server_Solution_Cache::exist_muid(Server_Enum::MONITOR_TYPE type, int id, int order, int run_on) const {
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		if ((*monitor_it).run_on != run_on) continue;

		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (type != (*accept_it).monitor_type) continue;

			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				if ((*it_order_it).id == id && (*it_order_it).order == order) return true;
			}
		}
	}
	return false;
}

void Server_Solution_Cache::fetch_muid_by_type(Monitor_Mark_Vec &list, Server_Enum::MONITOR_TYPE type) const {
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (type != Server_Enum::MONITOR_ALL && type != (*accept_it).monitor_type) continue;

			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				list.push_back(*it_order_it);
			}
		}
	}
}

int Server_Solution_Cache::fetch_logic_client_port(void) const {
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (Server_Enum::MONITOR_LOGIC == (*accept_it).monitor_type)
				return (*accept_it).client_port;
		}
	}

	return 0;
}

void Server_Solution_Cache::fetch_muid_single_monitor(Monitor_Mark_Vec &list, Server_Enum::MONITOR_TYPE type) const {
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (type != Server_Enum::MONITOR_ALL && type != (*accept_it).monitor_type) continue;

			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				list.push_back(*it_order_it);
				break;
			}
		}
	}
}

void Server_Solution_Cache::fetch_muid_same_process(Monitor_Mark_Vec &list, Monitor_Unique_ID &muid) const {
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				if ((*it_order_it) == muid) {
					list = (*accept_it).monitor_id_order_vec_;
					return ;
				}
			}
		}
	}

}

void Server_Solution_Cache::fetch_muid_single_process(Monitor_Mark_Vec &list, int run_on) const {
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		if (run_on && run_on != (*monitor_it).run_on) continue;

		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if ((*accept_it).monitor_id_order_vec_.size()) {
				list.push_back((*accept_it).monitor_id_order_vec_.front());
				break;
			}
		}
	}
}

void Server_Solution_Cache::fetch_muid_by_process_idx(Monitor_Mark_Vec &list, uint process_idx, const Server_Enum::MONITOR_TYPE type) const {
	if (process_idx >= monitor_list.size()) {
		LOG_ABORT("process_idx:%d, monitor_list:%d", process_idx, monitor_list.size());
	}

	const Monitor_Acceptor_Vec &acceptor_list = monitor_list[process_idx].acceptor_list;
	for (Monitor_Acceptor_Vec::const_iterator accept_it = acceptor_list.begin(); accept_it != acceptor_list.end(); ++accept_it) {
		for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
			if (type == Server_Enum::MONITOR_TYPE::MONITOR_ALL || (*it_order_it).type == type)
				list.push_back(*it_order_it);
		}
	}

}

int Server_Solution_Cache::fetch_scene_process_idx(int scene_id, int scene_order) const {
	int idx = 0;
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it, ++idx) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (Server_Enum::MONITOR_SCENE != (*accept_it).monitor_type) continue;

			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				if ((*it_order_it).id == scene_id && (*it_order_it).order == scene_order) return idx;
			}
		}
	}

	return -1;
}

int Server_Solution_Cache::fetch_gate_process_idx(int id, int order) const {
	int idx = 0;
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it, ++idx) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if (Server_Enum::MONITOR_GATE != (*accept_it).monitor_type) continue;

			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				if ((*it_order_it).id == id && (*it_order_it).order == order) return idx;
			}
		}
	}

	return -1;
}

int Server_Solution_Cache::fetch_order_total(const Server_Enum::MONITOR_TYPE type, const int id) const {
	int order_total = 0;
	for (Server_Monitor_Vec::const_iterator monitor_it = monitor_list.begin(); monitor_it != monitor_list.end(); ++monitor_it) {
		for (Monitor_Acceptor_Vec::const_iterator accept_it = (*monitor_it).acceptor_list.begin(); accept_it != (*monitor_it).acceptor_list.end(); ++accept_it) {
			if ((*accept_it).monitor_type != type) continue;

			for (Monitor_Mark_Vec::const_iterator it_order_it = (*accept_it).monitor_id_order_vec_.begin(); it_order_it != (*accept_it).monitor_id_order_vec_.end(); ++it_order_it) {
				if ((*it_order_it).id == id)
					order_total++;
			}
		}
	}

	return order_total;
}

