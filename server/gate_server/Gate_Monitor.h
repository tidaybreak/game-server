/*
 * Gate_Monitor.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef GATE_MONITOR_H_
#define GATE_MONITOR_H_

#include "Server_Monitor.h"
#include "Gate_Message_Unit.h"

class Gate_Player;

class Gate_Monitor: public Server_Monitor {
public:
	typedef Hash_Map<int, Gate_Player *, Null_Mutex> Gate_Player_Cid_Map;
	typedef Hash_Map<role_id_t, Gate_Player *, Null_Mutex> Gate_Player_Role_Id_Map;


	Gate_Monitor();
	virtual ~Gate_Monitor();

	int init(int monitor_index);

	int start(void);

	virtual Gate_Monitor *gate_monitor(void);

	virtual int server_monitor_status(void);

	virtual int process_monitor_close(void);

	virtual int start_monitor_timer(void);

	virtual int client_link_drop(const int cid);

	virtual int monitor_link_drop(const int cid);

	virtual int process_muid_drop(Monitor_Unique_ID &muid);

	int abnormal_logout(Gate_Player *player, int close_reason);

	int send_error_to_client(int cid, int error_id, int source);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	Gate_Player *find_player_by_cid(int cid);
	Gate_Player *find_player_by_role_id(role_id_t role_id);

	int send_to_all_player(Block_Buffer &buf);

	int bind_rid_player(Gate_Player &player);
	int unbind_rid_player(role_id_t role_id);

	int bind_cid_player(Gate_Player &player);
	int unbind_cid_player(int cid);

	void tran_player_add(role_id_t role_id, int delay_sec = 8);
	void tran_player_del(role_id_t role_id);

	int tran_timeout(Time_Value &now);

private:
	Gate_Message_Unit message_unit_;

	Gate_Player_Cid_Map gate_player_cid_map_;
	Gate_Player_Role_Id_Map player_role_id_map_;
	Role_Id_Time_Map tran_player_;		// 正在传送的玩家，core之类会影响下线流程，这里做下预防

};

typedef Singleton<Gate_Monitor> Gate_Monitor_Single;
#define GATE_MONITOR    (Gate_Monitor_Single::instance())

inline Gate_Monitor *Gate_Monitor::gate_monitor(void) {
	return this;
}

inline Gate_Player *Gate_Monitor::find_player_by_cid(int cid) {
	Gate_Player_Cid_Map::iterator it = gate_player_cid_map_.find(cid);
	if (it != gate_player_cid_map_.end())
		return it->second;
	else
		return 0;
}

inline Gate_Player *Gate_Monitor::find_player_by_role_id(role_id_t role_id) {
	Gate_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

#endif /* GATE_MONITOR_H_ */
