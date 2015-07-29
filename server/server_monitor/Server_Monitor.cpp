/*
 * Server_Monitor.cpp
 *
 *  Created on: Jan 9, 2014
 *      Author: ti
 */

#include "Server_Monitor.h"
#include "Message_Unit.h"
#include "Config_Cache.h"
#include "Global_Timer.h"

Server_Monitor::Server_Monitor() :
monitor_idx_(0),
type_(MONITOR_END),
status_(MONITOR_STATUS_PREPARE),
unit_(0),
server_id_(0) {
	// TODO Auto-generated constructor stub

}

Server_Monitor::~Server_Monitor() {
	// TODO Auto-generated destructor stub
}

int Server_Monitor::init(Server_Enum::MONITOR_TYPE type, int idx, Message_Unit *unit) {
	LOG_DEBUG("init monitor:%d idx:%d", type, idx);

	monitor_idx_ = idx;
	type_ = type;
	unit_ = unit;

	server_id_ = CONFIG_CACHE->server_flag();
	start_time_ = Time_Value::gettimeofday();

	unit->set_monitor(this);
	unit->register_inner_message();
	unit->register_client_message();

	Monitor_Mark_Vec monitor_id_order;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(monitor_id_order, Server_Enum::MONITOR_ALL);
	for (Monitor_Mark_Vec::iterator it = monitor_id_order.begin(); it != monitor_id_order.end(); ++it) {
		monitor_connect_cid_map_.bind(*it, -1);
	}


	const Server_Monitor_Vec &monitor_list = CONFIG_CACHE->server_solution_cache().monitor_list;
	const Monitor_Acceptor_Vec &acceptor_list = monitor_list[idx].acceptor_list;
	for (Monitor_Acceptor_Vec::const_iterator accept_it = acceptor_list.begin(); accept_it != acceptor_list.end(); ++accept_it) {
		if ((*accept_it).monitor_type == type) {

			if ((*accept_it).client_port_suffix) {
				Time_Value recv_timeout = (*accept_it).client_recv_timeout;
				service_client_.set_monitor(Unit_Message::TYPE_UNIT_CLIENT, unit, recv_timeout);
				service_client_.set_port((*accept_it).client_port);
				service_client_.init();
			}

			if ((*accept_it).inner_port_suffix) {
				Time_Value recv_timeout = (*accept_it).inner_recv_timeout;
				service_server_.set_monitor(Unit_Message::TYPE_UNIT_ACCEPT_SERVER, unit, recv_timeout);
				service_server_.set_port((*accept_it).inner_port);
				service_server_.init();
			}

			break;
		}
	}


	Time_Value recv_timeout = Time_Value::zero;
	service_connect_.set_monitor(Unit_Message::TYPE_UNIT_CONNECT_SERVER, unit, recv_timeout);
	service_connect_.init();

	return 0;
}

int Server_Monitor::start(void) {
	if (service_client_.get_port()) {
		service_client_.start();
	}

	if (service_server_.get_port()) {
		service_server_.start();
	}

	service_connect_.start();

	start_monitor_timer();

	RECORD_CLIENT->set_monitor(this);

	return 0;
}

int Server_Monitor::monitor_idx(void) {
	return monitor_idx_;
}

MONITOR_TYPE Server_Monitor::type(void) {
	return type_;
}

void Server_Monitor::set_client_svc_id(int cid, int64_t id) {
	service_client_.set_svc_id(cid, id);
}

int Server_Monitor::start_monitor_timer(void) {
	Time_Value now = Time_Value::gettimeofday();
	{
		Block_Buffer buf;
		buf.make_message(TIMER_GLOBAL_MONITOR_SELF);
		buf.finish_message();
		GLOBAL_TIMER->register_loop_handler(buf, Time_Value(1), unit_);
	}

	{
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_FIRST);
		buf.write_int32(TIMER_TRIGGER_DAILY_ZERO);
		buf.finish_message();
		Time_Value timeout = next_relative_time(00, 00, 00);
		GLOBAL_TIMER->register_once_handler(buf, Time_Value(timeout), unit_);
	}

	{
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_FIRST);
		buf.write_int32(TIMER_TRIGGER_DAILY_SIX);
		buf.finish_message();
		Time_Value timeout = next_relative_time(6, 00, 00);
		GLOBAL_TIMER->register_once_handler(buf, Time_Value(timeout), unit_);
	}

	{
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_FIRST);
		buf.write_int32(TIMER_TRIGGER_DAILY_TWENTY);
		buf.finish_message();
		Time_Value timeout = next_relative_time(22, 00, 00);
		GLOBAL_TIMER->register_once_handler(buf, Time_Value(timeout), unit_);
	}

	{
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_FIRST);
		buf.write_int32(TIMER_TRIGGER_WEEKLY_ZERO);
		buf.finish_message();
		Time_Value timeout = next_relative_time(00, 00, 00, 1);
		GLOBAL_TIMER->register_once_handler(buf, Time_Value(timeout), unit_);
	}

	return 0;
}

int Server_Monitor::process_monitor_close(void) {
	status_ = MONITOR_STATUS_CLOSING;

	return 0;
}

int Server_Monitor::send_to_all_process(Block_Buffer &buf) {
	Monitor_Mark_Vec list;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();

	server_solution_cache.fetch_muid_single_process(list, CONFIG_CACHE->server_maintainer_cache().server_identity);
	for (Monitor_Mark_Vec::iterator it = list.begin(); it != list.end(); ++it) {
		send_to_monitor(*it, buf);
	}

	return 0;
}

int Server_Monitor::send_to_monitor(const Monitor_Unique_ID &muid, Block_Buffer &buf, bool use_swap) {
	return send_to_monitor(muid.type, muid.id, muid.order, buf, muid.uuid, use_swap);
}

int Server_Monitor::send_to_monitor(MONITOR_TYPE type, int id, int order, Block_Buffer &buf, int64_t server_id, bool use_swap) {
	if (!server_id) server_id = server_id_;

	if (!buf.check_pack_len()) {
		MSG_TRACE_ABORT("id", id);
		return -1;
	}

	int cid = 0;
	Monitor_Unique_ID index(server_id, type, id, order);
	Monitor_Cid_Map::iterator find_it = monitor_connect_cid_map_.find(index);
	if (find_it != monitor_connect_cid_map_.end()) {
		cid = find_it->second;
		index = find_it->first;
		if (cid < 2) {
			cid = service_connect_.connect(index.ip.c_str(), index.port);
			if (cid >= 2) {
				// 同类型同muid进程连接都用这个cid
				const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
				Monitor_Mark_Vec muids;
				server_solution_cache.fetch_muid_same_process(muids, index);
				if (muids.empty()) {
					server_muid_build(index, cid, 1);
				} else {
					for (Monitor_Mark_Vec::iterator m_it = muids.begin(); m_it != muids.end(); ++m_it) {
						server_muid_build(*m_it, cid, 1);
					}
				}

				// 告诉对方本进程同类型的muid
				Block_Buffer link_buf;
				link_buf.make_message(SERVER_INNER_LINK_BUILD);
				link_buf.write_int32(type_);
				muids.clear();
				server_solution_cache.fetch_muid_by_process_idx(muids, monitor_idx_, type_);
				link_buf.write_uint16(muids.size());
				for (Monitor_Mark_Vec::iterator m_it = muids.begin(); m_it != muids.end(); ++m_it) {
					(*m_it).serialize(link_buf);
				}
				link_buf.finish_message();
				service_connect_.push_data_block_with_len(cid, link_buf, true);

				service_connect_.push_data_block_with_len(cid, buf, use_swap);

			} else {
				LOG_USER("connect error, muid:[%ld, %d, %d] [%s:%d]", index.uuid, index.id, index.order, index.ip.c_str(), index.port);
				return -1;
			}
		} else {
			find_it->first.monitor->push_data_block_with_len(cid, buf, use_swap);
			//service_connect_.push_data_block_with_len(cid, buf);
		}
	} else {
		LOG_TRACE_ABORT("error find monitor link info type:%d id:%d order:%d", type, id, order);
		return -1;
	}

	return 0;
}

int Server_Monitor::monitor_link_drop(const int cid) {
	for (Monitor_Cid_Map::iterator it = monitor_connect_cid_map_.begin(); it != monitor_connect_cid_map_.end(); ++it) {
		if (it->second == cid) {
			it->second = -1;
		}
	}

	return 0;
}

int Server_Monitor::client_link_drop(const int cid) {

	return 0;
}

int Server_Monitor::server_muid_build(Monitor_Unique_ID &muid, int cid, int service_type) {
	if (service_type == 1) {
		muid.monitor = &service_connect_;
	} else if (service_type == 2) {
		muid.monitor = &service_server_;
	}

	Monitor_Cid_Map::iterator find_it = monitor_connect_cid_map_.find(muid);
	if (find_it == monitor_connect_cid_map_.end()) {
		monitor_connect_cid_map_.bind(muid, cid);
		LOG_DEBUG("new muid rebuild, muid:[%ld, %d, %d, %d, %d], cid:%d", muid.uuid, muid.type, muid.order, muid.id, muid.port, cid);
	} else {
		if (find_it->first.ip != muid.ip || find_it->first.port != muid.port) {
			LOG_USER("repeat ip or port, now[%s:%d], muid[%s:%d]", find_it->first.ip.c_str(), find_it->first.port, muid.ip.c_str(), muid.port);
		}

		if ((find_it->second < 0 && cid > 0) || find_it->first.ip != muid.ip || find_it->first.port != muid.port) {
			if (find_it->second > 0 && cid > 0)
				LOG_USER("error muid rebuild, muid:[%ld, %d, %d, %d, %d], old cid:%d, new cid:%d", muid.uuid, muid.type, muid.order, muid.id, muid.port, find_it->second, cid);
			else
				monitor_connect_cid_map_.rebind(muid, cid);
		}
	}

	return 0;
}

int Server_Monitor::server_monitor_status(void) {

	return 0;
}

int Server_Monitor::get_muid_connect_by_cid(int cid, Monitor_Mark_Vec &muid) {
	GUARD_READ(RW_Mutex, mon, monitor_connect_cid_map_.mutex());

	for (Monitor_Cid_Map::iterator it = monitor_connect_cid_map_.begin(); it != monitor_connect_cid_map_.end(); ++it) {
		if (it->second == cid) {
			muid.push_back(it->first);
		}
	}
	return 0;
}

int Server_Monitor::close_client_by_cid(int cid, Time_Value delay, int reason, int64_t value) {
	if (cid < 2) {
		LOG_USER("cid = %d", cid);
		return -1;
	}

	LOG_DEBUG("monitor initiative close(delay) client, cid:%d reason:%d, value:%ld", cid, reason, value);

	// 给客户端发断线提示
	{
		Block_Buffer buf;
		buf.make_message(80000005);
		buf.write_int32(reason);
		buf.finish_message();
		send_to_client_by_cid(cid, buf);
	}

	{
		Block_Buffer buf;
		buf.make_message(SERVER_INNER_CLIENT_CLOSE);
		buf.write_int32(cid);
		buf.write_int32(reason);
		buf.write_int64(value);
		buf.finish_message();
		GLOBAL_TIMER->register_once_handler(buf, delay, unit_);
	}

	return 0;
}

int Server_Monitor::close_link_by_cid(int cid, Link_Close close_info) {
	LOG_DEBUG("monitor closing client, cid:%d reason:%d, value:%ld", cid, close_info.close_reason, close_info.close_value);

	Svc *svc = service_client_.find_svc(cid);
	if (svc) {
		svc->handle_close(close_info);
	} else {
		LOG_DEBUG("may be aleady logout cid:%d", cid);
	}
	return 0;
}

int Server_Monitor::get_client_peer_addr(int cid, std::string &ip) {
	Svc *svc = service_client_.find_svc(cid);
	if (! svc) {
		LOG_USER_TRACE("cid = %d, svc == 0", cid);
		return -1;
	}
	int port;
	return svc->get_peer_addr(ip, port);
}

int Server_Monitor::get_client_local_addr(int cid, std::string &ip) {
	Svc *svc = service_client_.find_svc(cid);
	if (! svc) {
		LOG_USER_TRACE("svc == 0");
		return -1;
	}
	int port;
	return  svc->get_local_addr(ip, port);
}

int Server_Monitor::push_async_buff(Block_Buffer &buf) {
	Block_Buffer *buff = unit_->pop_block(0);
	buff->copy(&buf);
	unit_->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buff, Unit_Message::TYPE_UNIT_LOOP);

	return 0;
}

const Monitor_Unique_ID* Server_Monitor::get_monitor_unique_id_by_uuid(int64_t uuid, Server_Enum::MONITOR_TYPE type) {
	for(Monitor_Cid_Map::iterator it = monitor_connect_cid_map_.begin();
			it != monitor_connect_cid_map_.end();
			++it) {
		if(uuid == it->first.uuid && type == it->first.type) {
			return &(it->first);
		}
	}
	return 0;
}

void Server_Monitor::logic_server_uuid_to_vector(std::vector<int64_t> &uuid_vector) {
	for(Monitor_Cid_Map::const_iterator it = monitor_connect_cid_map_.begin();
			it != monitor_connect_cid_map_.end();
			++it) {
		if(Server_Enum::MONITOR_LOGIC == it->first.type && it->second > 0) {
			uuid_vector.push_back(it->first.uuid);
		}
	}
}

void Server_Monitor::send_to_all_scene(Block_Buffer &buf) {
	for(Monitor_Cid_Map::const_iterator find_it = monitor_connect_cid_map_.begin();
			find_it != monitor_connect_cid_map_.end(); ++find_it) {
		if(Server_Enum::MONITOR_SCENE == find_it->first.type && find_it->second > 0) {
			Block_Buffer tmp_buf;
			tmp_buf.copy(&buf);
			tmp_buf.make_head(buf.get_msg_id(), 0);

			int cid = 0;
			cid = find_it->second;
			Monitor_Unique_ID index = find_it->first;
			if (cid < 2) {
				cid = service_connect_.connect(index.ip.c_str(), index.port);
				if (cid >= 2) {
					// 同类型同muid进程连接都用这个cid
					const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
					Monitor_Mark_Vec muids;
					server_solution_cache.fetch_muid_same_process(muids, index);
					if (muids.empty()) {
						server_muid_build(index, cid, 1);
					} else {
						for (Monitor_Mark_Vec::iterator m_it = muids.begin(); m_it != muids.end(); ++m_it) {
							server_muid_build(*m_it, cid, 1);
						}
					}

					// 告诉对方本进程同类型的muid
					Block_Buffer link_buf;
					link_buf.make_message(SERVER_INNER_LINK_BUILD);
					link_buf.write_int32(type_);
					muids.clear();
					server_solution_cache.fetch_muid_by_process_idx(muids, monitor_idx_, type_);
					link_buf.write_uint16(muids.size());
					for (Monitor_Mark_Vec::iterator m_it = muids.begin(); m_it != muids.end(); ++m_it) {
						(*m_it).serialize(link_buf);
					}
					link_buf.finish_message();
					service_connect_.push_data_block_with_len(cid, link_buf, true);

					service_connect_.push_data_block_with_len(cid, tmp_buf, 0);

				}
			} else {
				find_it->first.monitor->push_data_block_with_len(cid, tmp_buf, 0);
			}

		}
	}
}

const Server_Monitor::Monitor_Cid_Map &Server_Monitor::get_monitor_connect_cid_map(void) const {
	return monitor_connect_cid_map_;
}
