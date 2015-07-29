/*
 * Robot_Monitor.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef ROBOT_MONITOR_H_
#define ROBOT_MONITOR_H_

#include "Server_Monitor.h"
#include "Robot_Message_Unit.h"
#include "Server_Struct.h"

class Robot_Player;
struct NPC_Appear_Info;

class Robot_Monitor: public Server_Monitor {
public:
	typedef Hash_Map<int, Robot_Player *, Null_Mutex> Robot_Player_Cid_Map;
	typedef Hash_Map<role_id_t, Robot_Player *, Null_Mutex> Robot_Player_Role_Id_Map;
	typedef Hash_Map<Account_Key, int, Null_Mutex> Account_Cid_Map;
	typedef Hash_Map<int,  Account_Key, Null_Mutex> Cid_Account_Map;
	typedef std::vector<Account_Key> Account_Vec;


	Robot_Monitor();
	virtual ~Robot_Monitor();

	int init(int monitor_index);

	int start(char *name);

	virtual Robot_Monitor *robot_monitor(void);

	virtual int process_monitor_close(void);

	virtual int client_link_drop(const int cid);

	virtual int server_link_drop(const int cid);

	virtual int monitor_link_drop(const int cid);

	int connect_to_gate(std::string &ip, int port);

	int send_to_gate(int cid, Block_Buffer &buf);

	int connect_to_logic(Account_Key &account);

	int create_send_to_logic(int cid, std::string &account, Block_Buffer &buf);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	Robot_Player *find_player_by_cid(int cid);
	Robot_Player *find_player_by_role_id(role_id_t role_id);

	int bind_rid_player(Robot_Player &player);
	int unbind_rid_player(role_id_t role_id);

	int bind_cid_player(Robot_Player &player);
	int unbind_cid_player(int cid);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int login_logic_server(void);

	int drop_link_to_logic(Account_Key &account);
	int drop_link_to_logic(const int logic_cid);

	int tick(void);

	int rand_move(Time_Value &now);
	int rand_move(Robot_Player *player);
	int tick_doing(Time_Value &now);
	int do_wayfind(Time_Value &now, Robot_Player *player);
	int run_way_move(Time_Value &now, Robot_Player *player);
	int turn_scenes(Time_Value &now, Robot_Player *player);

	int do_task(Time_Value &now, Robot_Player *player);

	int get_account_by_cid(int cid, Account_Key &account);
	int get_cid_by_account(Account_Key &account);

	//test//////////////////////////////////////////////////////////////////////////////////////////////////
	int set_fetch_role(Account_Key &account);
	int unset_fetch_role(Account_Key &account);
	int set_loging(std::string account);
	int unset_loging(std::string account);
	int set_loging_gate(std::string account);
	int unset_loging_gate(std::string account);
	int set_loginoff(std::string account);

private:
	Robot_Message_Unit message_unit_;
	Account_Vec account_vec;
	uint robot_login_idx_;
	int cnt_try_create_role_online_;

	Robot_Player_Cid_Map robot_player_cid_map_;
	Robot_Player_Role_Id_Map player_player_id_map_;

	Account_Cid_Map logic_logining_account_cid_map_;
	Cid_Account_Map logic_logining_cid_account_map_;

	boost::unordered_map<Account_Key, int> logiccid_loop_map_;

	String_Set robot_loginoff_set_;

	std::string server_name;

	boost::unordered_set<Account_Key> fetch_role_ing_set;
	String_Set login_ing_set;
	String_Set login_gate_ing_set;

	Time_Value login_logic_tick_;

};

typedef Singleton<Robot_Monitor> Robot_Monitor_Single;
#define ROBOT_MONITOR    (Robot_Monitor_Single::instance())

inline Robot_Monitor *Robot_Monitor::robot_monitor(void) {
	return this;
}

inline Robot_Player *Robot_Monitor::find_player_by_cid(int cid) {
	Robot_Player_Cid_Map::iterator it = robot_player_cid_map_.find(cid);
	if (it != robot_player_cid_map_.end())
		return it->second;
	else
		return 0;
}

inline Robot_Player *Robot_Monitor::find_player_by_role_id(role_id_t role_id) {
	Robot_Player_Role_Id_Map::iterator it = player_player_id_map_.find(role_id);
	if (it != player_player_id_map_.end())
		return it->second;
	else
		return 0;
}

#endif /* ROBOT_MONITOR_H_ */
