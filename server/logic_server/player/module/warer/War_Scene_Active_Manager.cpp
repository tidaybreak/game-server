/*
 * War_Scene_Active_Manager.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include "War_Scene_Active_Manager.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Dungeon.h"
#include "Logic_Player.h"
#include "announcement/Announcement_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"


War_Scene_Active_Manager::War_Scene_Active_Manager(void):first_start_(false){

}

War_Scene_Active_Manager::~War_Scene_Active_Manager(void){

}

int War_Scene_Active_Manager::start(void){
	Time_Value now = Time_Value::gettimeofday();
	cal_active_time(now, active_status_);
	first_start_ = true;
	return 0;
}

int War_Scene_Active_Manager::tick(const Time_Value& now){
	if(first_start_){
		first_start_ = false;
		if(active_status_.status == 3){
			active_start();
		}
		return 0;
	}
	if(active_status_.status != 0 &&
			now > active_status_.next_status_time){
		if(active_status_.status == 1){// 关闭 ----> 准备
			active_status_.status = 2;
			active_status_.next_status_time = active_status_.next_status_time +
					Time_Value(active_status_.ready_time);
		}else if(active_status_.status == 2){// 准备 ----> 开启
			active_status_.status = 3;
			active_status_.next_status_time = active_status_.next_status_time +
					Time_Value(active_status_.act_time);
		}else if(active_status_.status == 3){// 战场开启 ----> 关闭
			cal_active_time(now, active_status_);
		}
		for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
			if(it->second){
				it->second->send_war_scene_active_to_client();
			}
		}
		//activate to scene
		if(active_status_.status == 3){
			active_start();
		}else if(active_status_.status == 1){
			active_finish();
		}
	}
	return 0;
}

void War_Scene_Active_Manager::get_active_info(int& status, int& act_time, int& remain_time){
	Time_Value now = Time_Value::gettimeofday();
	status = active_status_.status;
	act_time = active_status_.act_time;
	remain_time = active_status_.next_status_time.sec() - now.sec();
	if(remain_time > active_status_.act_time){
		remain_time = active_status_.act_time;
	}
}

int War_Scene_Active_Manager::get_enter_level_limit(void){
	return CONFIG_CACHE_DUNGEON->get_war_level_limit();
}

int War_Scene_Active_Manager::active_start(void){
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_WAR_START);
	return 0;
}

int War_Scene_Active_Manager::active_finish(void){
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_WAR_FINISH);
	return 0;
}


void War_Scene_Active_Manager::cal_active_time(const Time_Value& now, Active_Status& active_status){
	tm tm1;
	time_t t1 = now.sec();
	localtime_r(&t1, &tm1);
	int tm_hour = tm1.tm_hour;
	int tm_min = tm1.tm_min;
	int tm_sec = tm1.tm_sec;
	int now_time = tm_hour*3600+tm_min*60+tm_sec;
	int valid_status = 0;
	int valid_time = 0;
	int valid_act_time = 0;
	bool next_day = false;
	std::vector<Int_Int> open_time = CONFIG_CACHE_DUNGEON->get_war_open_time();
	cal_active_open_time(open_time, now_time, valid_status,
			valid_time, valid_act_time, next_day);
	int default_ready_time =  60*5;
	active_status_.status = valid_status;
	active_status_.ready_time = default_ready_time;
	active_status_.act_time = valid_act_time;
	if(active_status_.status == 1){
		int dt_time = valid_time - active_status_.ready_time - now_time;
		if(next_day){
			dt_time += Time_Value::ONE_DAY_IN_SECS;
		}
		active_status_.next_status_time = now + Time_Value(dt_time);
	}else if(active_status_.status == 2){
		int dt_time = valid_time - now_time;
		if(next_day){
			dt_time += Time_Value::ONE_DAY_IN_SECS;
		}
		active_status_.next_status_time = now + Time_Value(dt_time);
	}else if(active_status_.status == 3){
		int dt_time = valid_time + active_status_.act_time - now_time;
		if(next_day){
			dt_time += Time_Value::ONE_DAY_IN_SECS;
		}
		active_status_.next_status_time = now + Time_Value(dt_time);
	}
}

int War_Scene_Active_Manager::cal_active_open_time(const std::vector<Int_Int>& open_time, const int cur_time, int& valid_status, int& valid_time, int& valid_last_time, bool& next_day){
	int count = open_time.size();
	if(count <= 0){
		return -1;
	}
	int default_ready_time =  60*5;
	for(int i = 0; i < count; ++i){
		int cfg_time = open_time[i].val_1;
		int b_time  = cfg_time/100*Time_Value::ONE_HOUR_IN_SECS+cfg_time%100*Time_Value::ONE_MINUTE_IN_SECS;
		int f_time = b_time + open_time[i].val_2;
		if(cur_time  >= f_time){
			continue;
		}
		next_day = false;
		if(cur_time >= b_time){
			valid_status = 3;
		}else if(cur_time >= (b_time - default_ready_time)){
			valid_status = 2;
		}else{
			valid_status = 1;
		}
		valid_time = b_time;
		valid_last_time = open_time[i].val_2;
		return 0;
	}
	next_day = true;
	valid_status = 1;
	int cfg_time = open_time[0].val_1;
	valid_time  = cfg_time/100*Time_Value::ONE_HOUR_IN_SECS+cfg_time%100*Time_Value::ONE_MINUTE_IN_SECS;
	valid_last_time = open_time[0].val_2;
	return 0;
}

void War_Scene_Active_Manager::system_announce(const int chat_type, const int ann_id, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Chat_Content new_content;
	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}

	MSG_20500000 msg;
	msg.district = chat_type;
	msg.announce_id = ann_id;
	msg.contents = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}
