/*
 * Server_Monitor.h
 *
 *  Created on: Jan 9, 2014
 *      Author: ti
 */

#ifndef SERVER_MONITOR_H_
#define SERVER_MONITOR_H_

#include "Monitor_Struct.h"
#include "Service_Monitor.h"

using namespace Server_Enum;

class Message_Unit;
class Record_Monitor;
class Logic_Monitor;
class Gate_Monitor;
class Robot_Monitor;
class Scene_Monitor;
class Chat_Monitor;

class Server_Monitor {
public:
	typedef Hash_Map<Monitor_Unique_ID, int, RW_MUTEX> Monitor_Cid_Map;
	typedef Hash_Map<int, Monitor_Unique_ID, RW_MUTEX> Cid_Monitor_Map;

	Server_Monitor();
	virtual ~Server_Monitor();

	int init(MONITOR_TYPE type, int idx, Message_Unit *unit);

	int start(void);

	int monitor_idx(void);

	MONITOR_TYPE type(void);

	Service_Monitor &service_monitor(void);

	Message_Unit *message_unit(void);

	Time_Value &start_time(void);

	void set_client_svc_id(int cid, int64_t id);

	virtual int start_monitor_timer(void);

	virtual int server_monitor_status(void);

	virtual int process_monitor_close(void);

	virtual Logic_Monitor *logic_monitor(void) { return 0; };

	virtual Gate_Monitor *gate_monitor(void) { return 0; };

	virtual Robot_Monitor *robot_monitor(void) { return 0; };

	virtual Scene_Monitor *scene_monitor(void) { return 0; };

	virtual Chat_Monitor *chat_monitor(void) { return 0; };

	virtual Record_Monitor *record_monitor(void) { return 0; };

	inline int send_to_client_by_cid(int cid, Block_Buffer &buf, bool use_swap = false);

	inline int send_to_server_by_cid(int cid, Block_Buffer &buf);

	int send_to_all_process(Block_Buffer &buf);

	int send_to_monitor(const Monitor_Unique_ID &muid, Block_Buffer &buf, bool use_swap = false);

	int send_to_monitor(MONITOR_TYPE type, int id, int order, Block_Buffer &buf, int64_t server_id = 0, bool use_swap = false);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	virtual int client_link_drop(const int cid);

	virtual int monitor_link_drop(const int cid);

	virtual int server_muid_build(Monitor_Unique_ID &muid, int cid = -1, int service_type = 0);

	int close_client_by_cid(int cid, Time_Value delay, int reason, int64_t value = 0);

	int close_link_by_cid(int cid, Link_Close close_info);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	int push_async_buff(Block_Buffer &buf);

	int get_client_peer_addr(int cid, std::string &ip);

	int get_client_local_addr(int cid, std::string &ip);

	int get_muid_connect_by_cid(int cid, Monitor_Mark_Vec &muid);

	const Monitor_Unique_ID* get_monitor_unique_id_by_uuid(int64_t uuid, Server_Enum::MONITOR_TYPE type);

	void logic_server_uuid_to_vector(std::vector<int64_t> &uuid_vector);

	void send_to_all_scene(Block_Buffer &buf);

	const Monitor_Cid_Map &get_monitor_connect_cid_map(void) const;

protected:
	int monitor_idx_;
	MONITOR_TYPE type_;
	MONITOR_STATUS status_;
	Message_Unit *unit_;
	int64_t server_id_;
	Time_Value start_time_;

	Service_Accept service_client_;
	Service_Accept service_server_;
	Service_Connect service_connect_;

	Monitor_Cid_Map monitor_connect_cid_map_;		// 所有平台&服muid信息
};

inline Service_Monitor &Server_Monitor::service_monitor(void) {
	return service_client_;
}

inline Message_Unit *Server_Monitor::message_unit(void) {
	return unit_;
}

inline Time_Value &Server_Monitor::start_time(void) {
	return start_time_;
}

inline int Server_Monitor::send_to_client_by_cid(int cid, Block_Buffer &buf, bool use_swap) {
	if (cid < 2) {
		LOG_USER("cid = %d", cid);
		return -1;
	}
	return service_client_.push_data_block_with_len(cid, buf, use_swap);
}

inline int Server_Monitor::send_to_server_by_cid(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		LOG_USER("cid = %d", cid);
		return -1;
	}
	return service_server_.push_data_block_with_len(cid, buf);
}

#endif /* SERVER_MONITOR_H_ */
