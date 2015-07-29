/*
 * Record_Monitor.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Record_Monitor.h"
#include "Record_Client.h"
#include "Pool_Manager.h"
#include "Config_Cache.h"
#include "Daemon_Server.h"
#include "db_mysql/Mysql_Operator.h"
#include "Global_Timer.h"

Record_Monitor::Record_Monitor() {
	// TODO Auto-generated constructor stub
}

Record_Monitor::~Record_Monitor() {
	// TODO Auto-generated destructor stub
}

int Record_Monitor::init(int monitor_index) {
	Server_Monitor::init(Server_Enum::MONITOR_RECORD, monitor_index, &message_unit_);


	return 0;
}

int Record_Monitor::start(void) {
	Server_Monitor::start();

	//日志流水MYSQL
	MYSQL_OPERATOR->init();

	message_unit_.thr_create();

	return 0;
}

int Record_Monitor::start_monitor_timer(void) {
	Block_Buffer buf;
	buf.make_message(TIMER_GLOBAL_MONITOR_SELF);
	buf.finish_message();
	GLOBAL_TIMER->register_loop_handler(buf, Time_Value(Time_Value::ONE_MINUTE_IN_SECS, 0), unit_);
	return 0;
}

int Record_Monitor::process_monitor_close(void) {
	Server_Monitor::process_monitor_close();
	LOG_DEBUG("process_monitor_close record");


	boost::unordered_set<Monitor_Unique_ID> monitor_id_order;
	const Server_Monitor_Vec &monitor_list = CONFIG_CACHE->server_solution_cache().monitor_list;
	const Monitor_Acceptor_Vec &acceptor_list = monitor_list[monitor_idx_].acceptor_list;
	for (Monitor_Acceptor_Vec::const_iterator accept_it = acceptor_list.begin(); accept_it != acceptor_list.end(); ++accept_it) {
		for (Monitor_Mark_Vec::const_iterator it = (*accept_it).monitor_id_order_vec_.begin(); it != (*accept_it).monitor_id_order_vec_.end(); ++it) {
			monitor_id_order.insert(*it);
		}
	}

	for (Monitor_Cid_Map::iterator it = monitor_connect_cid_map_.begin(); it != monitor_connect_cid_map_.end(); ++it) {
		if (it->second > 0 && it->first.type != MONITOR_RECORD) {
			// 只有当不是本进程连接时
			if (!monitor_id_order.count(it->first)) {
				LOG_DEBUG("not same process %d  %d", it->first.type, it->first.id);
				return 0;
			}
		}
	}

	MYSQL_OPERATOR->commit();

	DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_RECORD);

	return 0;
}

int Record_Monitor::monitor_link_drop(const int cid) {
	Server_Monitor::monitor_link_drop(cid);

	LOG_DEBUG("record monitor link drop cid:%d status:%d", cid, status_);

	if (status_ != MONITOR_STATUS_CLOSING)
		return 0;

	for (Monitor_Cid_Map::iterator it = monitor_connect_cid_map_.begin(); it != monitor_connect_cid_map_.end(); ++it) {
		if (it->second > 0 && it->first.type != MONITOR_RECORD) {
			LOG_DEBUG("record monitor link drop cid:%d status:%d   %d  %d", it->second, status_, it->first.type, it->first.id);

			return 0;
		}
	}

	MYSQL_OPERATOR->commit();

	DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_RECORD);

	return 0;
}
