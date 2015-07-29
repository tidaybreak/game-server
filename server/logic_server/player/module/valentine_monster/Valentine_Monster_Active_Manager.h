/*
 * Valentine_Monster_Active_Manager.h
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */

#ifndef VALENTINE_MONSTER_ACTIVE_MANAGER_H_
#define VALENTINE_MONSTER_ACTIVE_MANAGER_H_


#include "Game_Typedef.h"
#include "Singleton.h"
#include "Monitor_Struct.h"

class Logic_Player;
struct Money_Add_Info;
struct Item_Detail;


class Valentine_Monster_Active_Manager{
	typedef boost::unordered_map<int, Monitor_Unique_ID> Main_Scene_Map;
public:
	Valentine_Monster_Active_Manager(void);
	~Valentine_Monster_Active_Manager(void);
public:
	void reset(void);
	void start(void);
	void init_main_scene(void);
	bool is_open(void);
	void listen_open_active(Time_Value before_start_time, Time_Value start_time, Time_Value end_time);
	void listen_close_active(void);
	void cmd_start_active(int value);
	void cmd_end_active(void);
public:
	int inner_sync_info(Logic_Player* player, Block_Buffer& buf);
	void resource_to_item(std::vector<Money_Add_Info> money_list, std::vector<Item_Detail>& item_vec);
private:
	void system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id = 0, std::string str1 = "", std::string str2 = "", std::string str3 = "");
private:
	Main_Scene_Map main_scene_map_;
	bool act_open_;
	Time_Value before_start_time_;
	Time_Value start_time_;
	Time_Value end_time_;
};

typedef Singleton<Valentine_Monster_Active_Manager> Valentine_Monster_Active_Manager_Single;
#define VALENTINE_MONSTER_ACTIVE_MANAGER (Valentine_Monster_Active_Manager_Single::instance())

#endif /* VALENTINE_MONSTER_ACTIVE_MANAGER_H_ */
