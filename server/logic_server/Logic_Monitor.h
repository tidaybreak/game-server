/*
 * Logic_Monitor.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef LOGIC_MONITOR_H_
#define LOGIC_MONITOR_H_

#include "Server_Monitor.h"
#include "Logic_Message_Unit.h"
#include "Comm_Int_Lock.h"
struct Player_DB_View;
struct DB_View;
class Logic_Player;

struct greater {
    bool operator ()(int &a,int &b) {
        return a>b;//最小值优先
    }
};
typedef std::priority_queue<int,std::vector<int>,greater> Int_Queue;

class Logic_Monitor: public Server_Monitor, Thread {
public:
	typedef Hash_Map<Monitor_Unique_ID, int, NULL_MUTEX> Gate_Amount_Map;
	typedef Hash_Map<Monitor_Unique_ID, int, NULL_MUTEX> Scene_Amount_Map;
	typedef Hash_Map<Scene_Unique_ID, int, NULL_MUTEX> Scene_Process_Idx;
	typedef Hash_Map<Scene_Unique_ID, Monitor_Unique_ID, NULL_MUTEX> Scene_Muid_Map;

	typedef Hash_Map<role_id_t, Logic_Player *, Null_Mutex> Logic_Player_Role_Id_Map;
	typedef Hash_Map<std::string, Logic_Player *, Thread_Mutex> Logic_Player_Name_Map;

	typedef Hash_Map<Monitor_Unique_ID, Time_Value, Thread_Mutex> Monitor_Link_Time;

	Logic_Monitor();
	virtual ~Logic_Monitor();

	int init(int monitor_index);

	int start(void);

	// 心跳可能连接超时，会影响主线程 单独放另一跳线程
	virtual void run_handler(void);

	void tick(Time_Value &now);

	void power_tick(Time_Value &now);

	virtual Logic_Monitor *logic_monitor(void);

	virtual int server_monitor_status(void);

	virtual int process_monitor_close(void);

	DB_View *db_view(void);

	const Logic_Player_Role_Id_Map &player_role_id_map(void);

	virtual int client_link_drop(const int cid);

	virtual int monitor_link_drop(const int cid);

	virtual int process_muid_drop(Monitor_Unique_ID &muid);

	virtual int server_muid_build(Monitor_Unique_ID &muid, int cid = -1, int service_type = 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int send_to_all_monitor(Block_Buffer &buf);

	int send_to_client(int cid, Block_Buffer &buf);

	int send_to_all_client(Block_Buffer &buf);

	int send_to_gate(Monitor_Unique_ID &idx, Block_Buffer &buf);

	int send_to_scene(int scene_id, int scene_order, Block_Buffer &buf);

	int send_to_local_chat(Block_Buffer &buf);

	int send_error_to_client(int cid, int error_id, int source);

	int abnormal_logout(Logic_Player *player, int close_reason);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	bool get_validate_account(std::string &ip);

	int select_gate_link_info(Monitor_Unique_ID &gate_muid);

	int fetch_gate_link_info(int cid, Monitor_Unique_ID &gate_muid, std::string &ip, int &port);

	int find_scene_muid(int scene_id, int scene_order, Monitor_Unique_ID &muid);

	int find_scene_process_idx(int scene_id, int scene_order);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	Logic_Player *find_logic_player_by_role_id(role_id_t role_id);
	Logic_Player *find_logic_player_by_name(std::string &name);

	int bind_logic_player(Logic_Player &player);
	int unbind_logic_player(role_id_t role_id);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int scene_order_free(const Monitor_Unique_ID &scene);
	bool scene_role_limit(const Monitor_Unique_ID &scene);
	int scene_change_info(const Monitor_Unique_ID &scene, int type);
	int scene_change_info(const Monitor_Unique_ID &src, const Monitor_Unique_ID &des);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	role_id_t get_role_id_by_name(std::string &role_name);
	std::string get_role_name_by_id(role_id_t role_id);

	Player_DB_View *find_player_db_view_by_role_id(role_id_t role_id);
	Player_DB_View *find_player_db_view_by_name(std::string &name);

	int process_player_event(void);

	int login_out_timeout(Time_Value &now);

	bool server_tick_check(void);

	void update_inner_tick(Monitor_Unique_ID &muid, Time_Value &time);

	int player_trigger_daily_zero(void);
	int player_trigger_daily_six(void);
	int player_trigger_daily_twenty(void);
	int player_trigger_weekly_zero(void);

	Time_Value& trigger_daily_zero_time(void);
	Time_Value& trigger_daily_six_time(void);
	Time_Value& trigger_weekly_zero_time(void);

	//竞技场赛季结束清空数据
	void arena_season_clear_data(void);

	// 服务器最高战力 -- 发往中心服
	void handle_hi_power_refresh(void);               // 刷新时调用
	void handle_hi_power(void);                       // 服务器启动时调用

private:
	DB_View *db_view_;
	String_Set company_ip_set_; 					// 公司出口IP
	bool verify_pack_onoff_;						// 包验证开关
	Logic_Message_Unit message_unit_;

	Monitor_Link_Time muid_link_time_;				// 保存除跨服外的链接
	Monitor_Link_Time muid_link_timeout_;			// 保存所有超时的链接

	Gate_Amount_Map gate_amount_map_;
	Scene_Amount_Map scene_amount_map_;
	Scene_Muid_Map scene_muid_map_;
	Scene_Process_Idx scene_process_idx_;

	Logic_Player_Role_Id_Map player_role_id_map_; 	/// role_id - Logic_Player map
	Logic_Player_Name_Map player_name_map_; 		/// player name - Logic_Player map

	Comm_Int_Lock highest_power_;                       // 服务器最高战力值
	Comm_Int_Lock highest_power_avg_;                   // 服务器前十战力平均值
	Time_Value refresh_hi_power_;                       // 下次刷新最高战力时间

	Time_Value trigger_daily_zero_;						// 上次每日0点刷新时间
	Time_Value trigger_daily_six_;						// 上次每日6点刷新时间
	Time_Value trigger_daily_twenty_;					// 上次每日20点刷新时间
	Time_Value trigger_weekly_zero_;						// 上次每周0点刷新时间
};

typedef Singleton<Logic_Monitor> Logic_Monitor_Single;
#define LOGIC_MONITOR    (Logic_Monitor_Single::instance())

inline Logic_Monitor *Logic_Monitor::logic_monitor(void) {
	return this;
}

inline DB_View *Logic_Monitor::db_view(void) {
	return db_view_;
}

inline const Logic_Monitor::Logic_Player_Role_Id_Map &Logic_Monitor::player_role_id_map(void) {
	return player_role_id_map_;
}

inline Logic_Player *Logic_Monitor::find_logic_player_by_role_id(role_id_t role_id) {
	Logic_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

inline Logic_Player *Logic_Monitor::find_logic_player_by_name(std::string &name) {
	Logic_Player_Name_Map::iterator it = player_name_map_.find(name);
	if (it != player_name_map_.end())
		return it->second;
	else
		return 0;
}

inline Time_Value& Logic_Monitor::trigger_daily_zero_time(void){
	return trigger_daily_zero_;
}

inline Time_Value& Logic_Monitor::trigger_daily_six_time(void){
	return trigger_daily_six_;
}

inline Time_Value& Logic_Monitor::trigger_weekly_zero_time(void){
	return trigger_weekly_zero_;
}
#endif /* LOGIC_MONITOR_H_ */
