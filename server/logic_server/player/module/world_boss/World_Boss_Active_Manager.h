/*
 * World_Boss_Active_Manager.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef WORLD_BOSS_ACTIVE_MANAGER_H_
#define WORLD_BOSS_ACTIVE_MANAGER_H_


#include "Game_Typedef.h"
#include "Singleton.h"
#include "world_boss/World_Boss_Struct.h"

class Logic_Player;
struct Money_Add_Info;
struct Item_Detail;

class World_Boss_Active_Manager{
	struct Active_Status{
		Active_Status(void){
			reset();
		}
		void reset(void){
			status = 0;
			ready_time = 0;
			act_time = 0;
			act_finish = false;
			next_status_time = Time_Value::zero;
		}
		int status;							//1 :close 2:ready 3:open
		int ready_time;
		int act_time;
		bool act_finish;
		Time_Value next_status_time;
	};
public:
	World_Boss_Active_Manager(void);
	~World_Boss_Active_Manager(void);
public:
	int start(void);
	int tick(const Time_Value &now);
	void daily_refresh(void);
public:
	int req_enter_scene(Logic_Player* player);
	int get_enter_level_limit(void);
	void get_active_info(int& status, int& remain_time, int &party_id);
	int cmd_sync_active_info(Block_Buffer& buf);
	int active_ready(void);
	int active_start(void);
	int active_finish(void);
	// 开始
	int cmd_start_active(int time);
	// 结束
	int cmd_finish_active(void);
	// gm set boss lv
	int cmd_set_world_boss_lv(const int lv);
	// gm set boss kill num
	int cmd_set_world_boss_kill_num(const int kill_num);
	//
	int get_boss_type_id(void);
public:
	bool is_open(void);
	void listen_open_active(Time_Value before_start_time, Time_Value start_time, Time_Value end_time);
	void listen_close_active(void);
	void listen_open_active_icon(Time_Value before_start_time, Time_Value start_time, Time_Value end_time);
	void listen_close_active_icon(void);
public:
	int inner_sync_info(Logic_Player* player, Block_Buffer& buf);
	int inner_act_end(Block_Buffer& buf);
public:
	int get_gang_bless_cost(void);
private:
	int load_active_info(void);
	void cal_active_time(const Time_Value& now, Active_Status& active_status);
	void system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id = 0, std::string str1 = "", std::string str2 = "", std::string str3 = "");
	void resource_to_item(std::vector<Money_Add_Info> money_list, std::vector<Item_Detail>& item_vec);
private:
	World_Boss_Detail active_info_;
	Active_Status active_status_;
	bool first_start_;
	int act_id_;
	Role_Id_Set enter_player_set_;
	//
	bool act_open_;
	bool icon_open_;
	Time_Value before_start_time_;
	Time_Value start_time_;
	Time_Value end_time_;
};


typedef Singleton<World_Boss_Active_Manager> World_Boss_Active_Manager_Single;
#define WORLD_BOSS_ACTIVE_MANAGER (World_Boss_Active_Manager_Single::instance())




#endif /* WORLD_BOSS_ACTIVE_MANAGER_H_ */
