/*
 * War_Scene_Active_Manager.h
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#ifndef WAR_SCENE_ACTIVE_MANAGER_H_
#define WAR_SCENE_ACTIVE_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"


class War_Scene_Active_Manager{
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
		int status;							// 1 :close 2:ready 3:open
		int ready_time;					// 活动开启前准备时间
		int act_time;						// 活动持续时间
		Time_Value next_status_time;
	};
public:
	War_Scene_Active_Manager(void);
	~War_Scene_Active_Manager(void);
public:
	int start(void);
	int tick(const Time_Value &now);
	void get_active_info(int& status, int& act_time, int& remain_time);
	int get_enter_level_limit(void);
private:
	void cal_active_time(const Time_Value& now, Active_Status& active_status);
	int cal_active_open_time(const std::vector<Int_Int>& open_time, const int cur_time, int& valid_status, int& valid_time, int& valid_last_time, bool& next_day);
	void system_announce(const int chat_type, const int ann_id, std::string str1 = "", std::string str2 = "", std::string str3 = "");
	int active_start(void);
	int active_finish(void);

private:
	Active_Status active_status_;
	bool first_start_;
};


typedef Singleton<War_Scene_Active_Manager> War_Scene_Active_Manager_Single;
#define WAR_SCENE_ACTIVE_MANAGER (War_Scene_Active_Manager_Single::instance())


#endif /* WAR_SCENE_ACTIVE_MANAGER_H_ */
