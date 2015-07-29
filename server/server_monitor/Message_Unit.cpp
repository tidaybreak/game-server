/*
 * Message_Unit.cpp
 *
 * Created on: 2012-12-31 18:56
 *     Author: Ti
 */

#include "Message_Unit.h"
#include "Pool_Manager.h"
#include "Server_Monitor.h"
#include "Config_Cache.h"
#include "Daemon_Server.h"
#include "Configurator.h"


Message_Unit::Message_Unit(void) :
is_running_(false),
message_unit_max_limit_(INT_MAX),
monitor_(0),
block_group_pool_(10) {
	pause_ts_.tv_sec = 0;
	pause_ts_.tv_nsec = 30 * 1000 * 1000;
	Date_Time date_time(Time_Value::gettimeofday());
	date_time.minute(0);
	date_time.second(0);
	data_pro_tick_ = Time_Value(date_time.time_sec() + 3600);
}

Message_Unit::~Message_Unit(void) {

}

void Message_Unit::set_monitor(Server_Monitor *monitor) {
	monitor_ = monitor;
	data_pro_tick_ += Time_Value(monitor->monitor_idx());
}

void Message_Unit::run_handler(void) {

    this->process_ptr_list();
    this->is_running_ = false;
}

struct Msg_Time_Sort_Times {
	bool operator() (Message_Stat elem1, Message_Stat elem2) const {
		return elem1.times > elem2.times;
	}
};

struct Msg_Time_Sort_Total {
	bool operator() (Message_Stat elem1, Message_Stat elem2) const {
		return elem1.tv > elem2.tv;
	}
};

struct Msg_Time_Sort_Max {
	bool operator() (Message_Stat elem1, Message_Stat elem2) const {
		return elem1.max_tv > elem2.max_tv;
	}
};

struct Msg_Time_Sort_Avg {
	bool operator() (Message_Stat elem1, Message_Stat elem2) const {
		return elem1.avg > elem2.avg;
	}
};

int Message_Unit::register_inner_process(const uint32_t msg_id, Inner_Func callback) {
	Inner_Func_Map::iterator find_it = inner_message_func_.find(msg_id);
	if (find_it != inner_message_func_.end())
		LOG_ABORT("rebind msg_id:%d", msg_id);
	inner_message_func_[msg_id] = callback;
	return 0;
}

int Message_Unit::process_inner_message(const uint32_t msg_id, const int32_t cid, Block_Buffer &buf) {
	Inner_Func_Map::iterator mapi = inner_message_func_.find(msg_id);
	if (mapi == inner_message_func_.end()) {
		MSG_USER("unknow msg:%d", msg_id);
		return -1;
	}
	return mapi->second(cid, buf);
}

void Message_Unit::show_process_time(void) {
	std::vector<Message_Stat> msg_time_vec;
	for (Message_Stat_Map::iterator it = msg_stat_.begin(); it != msg_stat_.end(); ++it) {
		it->second.msg_id = it->first;
		it->second.avg = (it->second.tv.sec() + it->second.tv.usec() / (1000000.0)) / it->second.times;
		msg_time_vec.push_back(it->second);
	}

	LOG_TEXT("msg process total:%d", msg_time_vec.size());

	int i = 0;
	std::vector<Message_Stat> msg_sort_times;
	std::sort(msg_time_vec.begin(), msg_time_vec.end(), Msg_Time_Sort_Times());
	for (std::vector<Message_Stat>::iterator it = msg_time_vec.begin(); it != msg_time_vec.end(); ++it, ++i) {
		msg_sort_times.push_back(*it);
		if (i > 30) break;
	}

	i = 0;
	std::vector<Message_Stat> msg_sort_total;
	std::sort(msg_time_vec.begin(), msg_time_vec.end(), Msg_Time_Sort_Total());
	for (std::vector<Message_Stat>::iterator it = msg_time_vec.begin(); it != msg_time_vec.end(); ++it, ++i) {
		msg_sort_total.push_back(*it);
		if (i > 30) break;
	}

	i = 0;
	std::vector<Message_Stat> msg_time_avg;
	std::sort(msg_time_vec.begin(), msg_time_vec.end(), Msg_Time_Sort_Avg());
	for (std::vector<Message_Stat>::iterator it = msg_time_vec.begin(); it != msg_time_vec.end(); ++it, ++i) {
		msg_time_avg.push_back(*it);
		if (i > 30) break;
	}

	i = 0;
	std::vector<Message_Stat> msg_time_max;
	std::sort(msg_time_vec.begin(), msg_time_vec.end(), Msg_Time_Sort_Max());
	for (std::vector<Message_Stat>::iterator it = msg_time_vec.begin(); it != msg_time_vec.end(); ++it, ++i) {
		msg_time_max.push_back(*it);
		if (i > 30) break;
	}

	i = 0;
	for (std::vector<Message_Stat>::iterator it = msg_sort_times.begin(); it != msg_sort_times.end(); ++it, ++i) {
		double total_time = msg_sort_total[i].tv.sec() + msg_sort_total[i].tv.usec() / (1000 * 1000.0);

		double max_time = msg_time_max[i].max_tv.sec() + msg_time_max[i].max_tv.usec() / (1000 * 1000.0);

		LOG_TEXT("  [msg:%-10d times:%-10d]     [msg:%-10d times:%-10d, spend:%-10f]     [ msg:%-10d, times:%-10d avg:%-10f]     [msg:%-10d max:%-10f]",
				(*it).msg_id, (*it).times,
				msg_sort_total[i].msg_id, msg_sort_total[i].times, total_time,
				msg_time_avg[i].msg_id, msg_time_avg[i].times, msg_time_avg[i].avg,
				msg_time_max[i].msg_id, max_time);
	}
}

void Message_Unit::count_process_time(int msg_id) {
	Time_Value t = Time_Value::gettimeofday() - msg_tick_;
	msg_stat_[msg_id].count(t);
}

void Message_Unit::show_server_status(void) {
	//SERVICE_MANAGER->service_svc_pool().push_shrink();
	LOG_TEXT("service_svc_pool msg:free:%d, used:%d", SERVICE_MANAGER->service_svc_pool().free_obj_list_size(), SERVICE_MANAGER->service_svc_pool().used_obj_list_size());

	//unit_msg_pool_.push_shrink();
	LOG_TEXT("data ptr size:%d, unit msg:free:%d, used:%d", data_ptr_list_.size(), unit_msg_pool_.free_obj_list_size(), unit_msg_pool_.used_obj_list_size());

	//block_group_pool_.push_shrink();
	block_group_pool_.dump_info();

	monitor_->server_monitor_status();
	this->show_process_time();
}

int Message_Unit::register_client_message(void) {
	return 0;
}

int Message_Unit::register_inner_message(void) {

	return 0;
}

bool Message_Unit::is_running(void) {
	return is_running_;
}

void Message_Unit::server_status(void) {
	// 当做和日志服心跳
	RECORD_CLIENT->sync_pid(monitor_->type(), monitor_->monitor_idx());

	int uptime = (Time_Value::gettimeofday().sec() - monitor_->start_time().sec()) / 3600;
	LOG_TEXT("↓↓↓ server status start: uptime:%d(hour), type:%d, index:%d ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓", uptime, monitor_->type(), monitor_->monitor_idx());
	show_server_status();

	Date_Time date_time(Time_Value::gettimeofday());
	date_time.minute(0);
	date_time.second(0);
	data_pro_tick_ += Time_Value(3600);
	LOG_TEXT("↑↑↑ server status end: type:%d ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑", monitor_->type());
	LOG_TEXT_FLUSH;
}

bool Message_Unit::message_unit_busy(void) {
	if (data_ptr_list_.size() > message_unit_max_limit_) {
		return true;
	}

	return false;
}

int Message_Unit::process_ptr_list(void) {
	// 连接日志服
	int loop = 0;
	while (true) {
		if (loop++ >= 10)
			LOG_ABORT("sync record error, type:%d, idx:%d", monitor_->type(), monitor_->monitor_idx());
		if (RECORD_CLIENT->sync_pid(monitor_->type(), monitor_->monitor_idx())) {
			Time_Value::sleep(Time_Value(3, 0));
			LOG_DEBUG("sync record error, type:%d, idx:%d", monitor_->type(), monitor_->monitor_idx());
		} else {
			break;
		}
	}

	this->is_running_ = true;

	Unit_Message *ptr_data = 0;

	Time_Value last_t = Time_Value::gettimeofday();
	while (1) {
    	if (!data_ptr_list_.pop(ptr_data)) {
        	switch (ptr_data->unit_type) {
        		case Unit_Message::TYPE_UNIT_CLIENT: {
        			process_client_buffer(*(Block_Buffer *)ptr_data->ptr_data);
        			break;
        		}
        		case Unit_Message::TYPE_UNIT_CONNECT_SERVER:
          		case Unit_Message::TYPE_UNIT_ACCEPT_SERVER: {
        			process_server_buffer(*(Block_Buffer *)ptr_data->ptr_data);
            		break;
            	}
          		case Unit_Message::TYPE_UNIT_TIMER: {
        			process_timer_buffer(*(Block_Buffer *)ptr_data->ptr_data);
            		break;
            	}
          		case Unit_Message::TYPE_UNIT_LOOP: {
        			process_async_data(ptr_data->data_type, ptr_data->ptr_data);
            		break;
            	}
          		default: {
          			break;
          		}
        	}

        	recover_ptr_body(ptr_data);

        	recover_ptr_shell(ptr_data);
    	}
	}

	return 0;
}

int Message_Unit::recover_ptr_body(Unit_Message *ptr_data) {
	if (!ptr_data->ptr_data) return CALL_RETURN;

	if (ptr_data->data_type == Unit_Message::TYPE_DATA_BUFFER) {
		push_block(ptr_data->pool_gid, (Block_Buffer *)ptr_data->ptr_data);
		return CALL_RETURN;
	}

	return CALL_CONTINUE;
}

int Message_Unit::recover_ptr_shell(Unit_Message *ptr_data) {
	return unit_msg_pool_.push(ptr_data);
}

int Message_Unit::process_client_buffer(Block_Buffer &buf) {
	msg_tick_ = Time_Value::gettimeofday();

	int cid = 0;
	uint32_t len = 0;
	uint32_t msg_id = 0;
	uint32_t serial_cipher = 0;
	uint32_t msg_time_cipher = 0;

	if (buf.read_int32(cid) ||
		buf.read_uint32(len) ||
		buf.read_uint32(msg_id) ||
		buf.read_uint32(serial_cipher) ||
		buf.read_uint32(msg_time_cipher)) {

		LOG_USER("deserialize error, cid:%d len:%d msg_id:%d", cid, len, msg_id);
		monitor()->close_client_by_cid(cid, Time_Value(2), 10000101);
		return -1;
	}

	process_client_msg(msg_id, cid, buf, serial_cipher, msg_time_cipher);

	count_process_time(msg_id);

	return 0;
}

int Message_Unit::process_server_buffer(Block_Buffer &buf) {
	msg_tick_ = Time_Value::gettimeofday();

	int cid = 0;
	uint32_t len = 0;
	uint32_t msg_id = 0;

	if (buf.read_int32(cid) ||
		buf.read_uint32(len) ||
		buf.read_uint32(msg_id)) {

		LOG_USER("deserialize error, cid:%d len:%d msg_id:%d", len, msg_id);
		return -1;
	}

	process_server_msg(msg_id, cid, buf);

	count_process_time(msg_id);

	return 0;
}

int Message_Unit::process_timer_buffer(Block_Buffer &buf) {
	msg_tick_ = Time_Value::gettimeofday();

	uint32_t len = 0;
	uint32_t msg_id = 0;

	if (buf.read_uint32(len) ||
		buf.read_uint32(msg_id)) {

		LOG_USER("deserialize error, cid:%d len:%d msg_id:%d", len, msg_id);
		return -1;
	}

	process_timer_msg(msg_id, buf, msg_tick_);

	count_process_time(msg_id);

	return 0;
}

int Message_Unit::process_async_data(int data_type, void *ptr_data) {
	switch (data_type) {
	case Unit_Message::TYPE_DATA_BUFFER: {
		Block_Buffer *buf = (Block_Buffer *)ptr_data;
		uint32_t len = 0;
		uint32_t msg_id = 0;

		if (buf->read_uint32(len) ||
			buf->read_uint32(msg_id)) {
			LOG_USER("deserialize error, cid:%d len:%d msg_id:%d", len, msg_id);
			return -1;
		}

		process_async_buff(msg_id, *buf);
		break;
	}
	default: {
		return CALL_CONTINUE;
		break;
	}
	}

	return CALL_RETURN;
}

int Message_Unit::process_client_msg(uint32_t msg_id, int cid, Block_Buffer &buf, uint32_t serial_cipher, uint32_t msg_time_cipher) {

	return 0;
}

int Message_Unit::process_server_msg(uint32_t msg_id, int cid, Block_Buffer &buf) {
	switch (msg_id) {
	case SERVER_INNER_LINK_BUILD: {
		int type = 0;
		buf.read_int32(type);

		uint16_t muid_size = 0;
		if (!buf.read_uint16(muid_size)) {
			Monitor_Mark_Vec muids;
			Monitor_Unique_ID muid;
			for (int i = 0; i < muid_size; ++i) {
				muid.reset();
				muid.deserialize(buf);
				monitor()->server_muid_build(muid, cid, 2);
			}
		} else {
			LOG_USER("server link build error!");
		}
		break;
	}
	case SERVER_INNER_SELF_CLOSE: {
		DAEMON_SERVER_INSTANCE->set_server_list(Daemon_Server::MONITOR_SHUTDOWN);
		break;
	}
	case SERVER_INNER_CONFIG_UPDATE: {
		Time_Value now = Time_Value::gettimeofday();

		std::string module;
		buf.read_string(module);
		String_Vec files;
		uint16_t f_size = 0;
		buf.read_uint16(f_size);
		for (int i = 0; i < f_size; ++i) {
			std::string file_path;
			buf.read_string(file_path);
			files.push_back(file_path);
		}
		LOG_DEBUG("config update! module:%s, file size:%d, type:%d, idx:%d", module.c_str(), f_size, monitor_->type(), monitor_->monitor_idx());


		int cross_id = CONFIG_CACHE->server_maintainer_cache().game_inter_flag;
		if(module == "server_global_conf") {
			//加载主配制
			CONFIG_INSTANCE->load_global_config();
			CONFIG_CACHE->refresh_maintainer_cache();
			RECORD_CLIENT->set_switcher(CONFIG_CACHE->server_maintainer_cache().record_log_level);
			Lib_Log::instance()->set_switcher(CONFIG_CACHE->server_maintainer_cache().local_log_level);
		} else {
			CONFIG_INSTANCE->update_config(files, module);
		}

		// 重启条件:跨服id改变且还没开服
		bool restart = false;
		if (cross_id != CONFIG_CACHE->server_maintainer_cache().game_inter_flag) {
			LOG_DEBUG("inter change, new:%d old:%d, open_time:%ld", CONFIG_CACHE->server_maintainer_cache().game_inter_flag, cross_id,
					CONFIG_CACHE->server_maintainer_cache().game_open_time.sec());
			if (CONFIG_CACHE->server_maintainer_cache().game_open_time > Time_Value::gettimeofday()) {
				restart = true;
			} else {
				CONFIG_CACHE->set_cross_info(cross_id);
			}
		}

		Time_Value cost_t = Time_Value::gettimeofday() - now;
		process_config_hook(module, restart);

		break;
	}
	case SERVER_INNER_HEARTBEAT: {
		Block_Buffer inner_buf;
		inner_buf.make_message(SERVER_INNER_HEARTBEAT_BACK);
		inner_buf.copy(&buf);
		inner_buf.finish_message();
		inner_buf.make_head(inner_buf.get_msg_id(), 0);
		monitor_->send_to_server_by_cid(cid, inner_buf);
		break;
	}
	case SERVER_INNER_MSG_TEST: {
		break;
	}
	default : {
		return CALL_CONTINUE;
		break;
	}
	}

	return CALL_RETURN;
}

int Message_Unit::process_config_hook(std::string &module, bool restart) {
	return 0;
}

int Message_Unit::process_async_buff(uint32_t msg_id, Block_Buffer &buf) {
	switch (msg_id) {
	case SERVER_INNER_LINK_CLOSE: {
		Link_Drop link_drop;
		link_drop.deserialize(buf);
		process_link_close(link_drop);
		break;
	}
	case SERVER_INNER_SELF_CLOSE: {
		monitor()->process_monitor_close();
		break;
	}
	default : {
		return CALL_CONTINUE;
		break;
	}
	}

	return CALL_RETURN;
}

int Message_Unit::process_timer_msg(uint32_t msg_id, Block_Buffer &buf, Time_Value &now) {
	if (data_pro_tick_ < Time_Value::gettimeofday()) {
		server_status();
	}


	switch (msg_id) {
	case SERVER_INNER_CLIENT_CLOSE: {
		int cid = 0, close_type = 0;
		int64_t value = 0;
		buf.read_int32(cid);
		buf.read_int32(close_type);
		buf.read_int64(value);
		monitor()->close_link_by_cid(cid, Link_Close(close_type, value));

		break;
	}
	default : {
		return CALL_CONTINUE;
		break;
	}
	}

	return CALL_RETURN;
}

void Message_Unit::process_link_close(Link_Drop &link_drop) {\
	if (link_drop.service_type != Unit_Message::TYPE_UNIT_CLIENT) {
		LOG_DEBUG("link close drop_cid:%d, service_type:%d close_reason:%d close_value:%ld",
				link_drop.cid, link_drop.service_type, link_drop.close_reason, link_drop.close_value);
	}

	switch(link_drop.service_type) {
	case Unit_Message::TYPE_UNIT_CLIENT: {
		monitor()->client_link_drop(link_drop.cid);
		break;
	}
	case Unit_Message::TYPE_UNIT_ACCEPT_SERVER: {
		monitor()->monitor_link_drop(link_drop.cid);
		break;
	}
	case Unit_Message::TYPE_UNIT_CONNECT_SERVER: {
		monitor()->monitor_link_drop(link_drop.cid);
		break;
	}
	default : {
		break;
	}
	}
}
