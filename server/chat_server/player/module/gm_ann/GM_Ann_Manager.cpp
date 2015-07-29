/*
 * GM_Ann_Manager.cpp
 *
 *  Created on: 2014-06-19
 *      Author: xiaoliang
 */

#include "GM_Ann_Manager.h"
#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "DB_Manager.h"
#include "DB_Operator.h"
#include "Chat_Monitor.h"
#include "GM_Ann_Def.h"

GM_Ann_Manager::GM_Ann_Manager() {
	reset();
}

GM_Ann_Manager::~GM_Ann_Manager() { }

void GM_Ann_Manager::reset() {
	ann_info_map.clear();
}

void GM_Ann_Manager::start(void) {
	MONGO_OPERATOR->load_ann_info(ann_info_map);
	send_ann_loop(Time_Value::gettimeofday());
}

void GM_Ann_Manager::tick(const Time_Value &now) {
	send_ann_loop(now);
}

void GM_Ann_Manager::send_ann_loop(const Time_Value &now) {
	Int_Set over_list;
	for(GM_Ann_Info_Map::iterator it = ann_info_map.begin(); it != ann_info_map.end(); ++it) {
		GM_Ann_Info &info = it->second;
		if(info.etime.sec() >= now.sec()) {
			if(info.stime.sec() <= now.sec() && info.step) {
				if(info.send_time == Time_Value::zero) {
					info.send_time.sec(now.sec() + info.step);
				} else if(info.send_time.sec() <= now.sec()) {
					send_ann(info.contents);
					info.send_time.sec(now.sec() + info.step);
				}
			}
		} else {
			over_list.insert(info.id);
		}
	}
	for(Int_Set::iterator iter = over_list.begin(); iter != over_list.end(); ++iter) {
		GM_Ann_Info_Map::iterator it = ann_info_map.find(*iter);
		if(it != ann_info_map.end()) {
			ann_info_map.erase(it);
		}
	}
}

void GM_Ann_Manager::send_ann(const std::string &contents) {
	MSG_83000003 msg;
	msg.contents = contents;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	CHAT_MONITOR->send_to_all_players(buf);
}

void GM_Ann_Manager::set_gm_announce(int32_t id, int8_t type, int32_t stime, int32_t etime, int32_t step, std::string &contents) {
	switch(type) {
		case GM_ANN_SEND_ONE: {//即时发送
			send_ann(contents);
			break;
		}
		case GM_ANN_SET_STEP: {//时间间隔发送
			Time_Value now = Time_Value::gettimeofday();
			GM_Ann_Info_Map::iterator it = ann_info_map.find(id);
			if(it != ann_info_map.end()) {
				it->second.stime.sec(stime);
				it->second.etime.sec(etime);
				it->second.step = step;
				it->second.contents = contents;
				it->second.send_time.sec(now.sec() + it->second.step);
			} else {
				GM_Ann_Info info;
				info.reset();
				info.id = id;
				info.stime.sec(stime);
				info.etime.sec(etime);
				info.step = step;
				info.contents = contents;
				info.send_time.sec(now.sec() + info.step);
				ann_info_map.insert(std::make_pair(id, info));
			}
			break;
		}
		case GM_ANN_DELETE: {//删除或使公告失效
			GM_Ann_Info_Map::iterator it = ann_info_map.find(id);
			if(it != ann_info_map.end()) {
				ann_info_map.erase(it);
			}
		}
	}
}
