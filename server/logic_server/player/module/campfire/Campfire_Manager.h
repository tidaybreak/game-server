/*
 * Campfire_Manager.h
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#ifndef CAMPFIRE_MANAGER_H_
#define CAMPFIRE_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"
#include "campfire/Campfire_Struct.h"
#include "Logic_Player.h"
#include "Msg_Inner_Struct.h"

class Campfire_Manager{
	struct Active_Status{
		Active_Status(void){
			reset();
		}
		void reset(void){
			status = 0;
			ready_time = 0;
			act_time = 0;
			next_status_time = Time_Value::zero;
		}
		int status;							//1 :close 2:ready 3:open
		int ready_time;
		int act_time;
		Time_Value next_status_time;
	};
public:
	Campfire_Manager(void);
	~Campfire_Manager(void);
public:
	int start(void);
	int tick(const Time_Value &now);
	void daily_refresh(void);
public:
	int get_enter_level_limit(void);
	int get_dancing_time(void);
	void get_active_info(int& status, int& remain_time, int &party_id);
	int get_campfire_point_id(gang_id_t gang_id);
	int cmd_sync_campfire_info(Block_Buffer& buf);
	int campfire_ready(void);
	int campfire_start(void);
	int campfire_finish(void);
	// 篝火舞会开始
	int bonfire_act_start(int time);
	// 篝火舞会结束
	int bonfire_act_finish(void);

	int player_get_bonfire_award(Logic_Player *player, const MSG_20300113 &msg);
	int load_player_info(Logic_Player *player);
	int save_player_info(Logic_Player *player, const MSG_20300116 &msg);
	int mail_to_player(Logic_Player *player, const MSG_20300117 &msg);

	FB_Info_Detail* get_fb_info_detail(int scene_id);

	void set_fb_info_detail(int scene_id, role_id_t role_id, std::string role_name, int force);
private:
	int load_campfire(void);
	int load_fb_info(void);

	Campfire_Detail* get_campfire_detail(gang_id_t gang_id);

	void cal_active_time(const Time_Value& now, Active_Status& active_status);
	void system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id = 0, std::string str1 = "", std::string str2 = "", std::string str3 = "");
private:
	Campfire_Detail_Map campfire_info_;
	Active_Status active_status_;
	bool first_start_;
	int act_id;

	FB_Info_Detail_Map fb_info_detail_map_;
};


typedef Singleton<Campfire_Manager> Campfire_Manager_Single;
#define CAMPFIRE_MANAGER (Campfire_Manager_Single::instance())


#endif /* CAMPFIRE_MANAGER_H_ */
