/*
 * Chat_Monitor.h
 *
 *  Created on: Nov 25, 2013
 *      Chator: ti
 */

#ifndef CHAT_MONITOR_H_
#define CHAT_MONITOR_H_

#include "Server_Monitor.h"
#include "Chat_Message_Unit.h"

class Chat_Player;

class Chat_Monitor: public Server_Monitor {
public:
	typedef Hash_Map<role_id_t, Chat_Player *, Null_Mutex> Chat_Player_Role_Id_Map;

	Chat_Monitor();
	virtual ~Chat_Monitor();

	int init(int monitor_index);

	int start(void);

	int start_monitor_timer(void);

	virtual Chat_Monitor *chat_monitor(void);

	virtual int server_monitor_status(void);

	virtual int process_monitor_close(void);

	virtual int monitor_link_drop(const int cid);

	virtual int process_muid_drop(Monitor_Unique_ID &muid);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int send_to_all_players(Block_Buffer& buf, int limit_level = 0);

	int send_to_gate(Monitor_Unique_ID &idx, Block_Buffer &buf);
	int send_to_all_gate(Block_Buffer &buf);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	Chat_Player *find_chat_player_by_role_id(role_id_t role_id);

	int bind_chat_player(Chat_Player &player);
	int unbind_chat_player(role_id_t role_id);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int player_trigger_daily_zero(void);

private:
	Chat_Message_Unit message_unit_;

	Chat_Player_Role_Id_Map player_role_id_map_;

	Monitor_Mark_Vec gate_monitor_id_order;
};

typedef Singleton<Chat_Monitor> Chat_Monitor_Single;
#define CHAT_MONITOR    (Chat_Monitor_Single::instance())

inline Chat_Monitor *Chat_Monitor::chat_monitor(void) {
	return this;
}

inline Chat_Player *Chat_Monitor::find_chat_player_by_role_id(role_id_t role_id) {
	Chat_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

#endif /* CHAT_MONITOR_H_ */
