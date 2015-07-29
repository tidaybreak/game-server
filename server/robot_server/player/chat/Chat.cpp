/*
 * Chat.cpp
 *
 *  Created on: 2014-06-07
 *      Author: xiaoliang
 */

#include "Chat.h"
#include "Config_Cache.h"
#include "Config_Cache_Robot.h"
#include "Robot_Player.h"
#include "Msg_Chat_Struct.h"

Chat::Chat() {
	reset();
}

void Chat::reset(void) {
	say_interval = CONFIG_CACHE_ROBOT->robot_config().chat_time;
	last_say_time = Time_Value::zero;
}

Chat::~Chat() {
	// TODO Auto-generated destructor stub
}

void Chat::say(Time_Value &now, Location &location) {
	if(now >= (last_say_time + say_interval)) {
		last_say_time = now;
		Robot_Player *player = (Robot_Player*)this;
		MSG_10300000 msg;
		msg.chat_type = 1;
		Chat_Content cc;
		//人物信息
		cc.reset();
		cc.type = 2;
		cc.role_Info.role_name = player->base_detail().role_name;
		cc.role_Info.career = player->base_detail().career;
		cc.role_Info.gender = player->base_detail().gender;
		cc.role_Info.role_id = player->base_detail().role_id;
		msg.content.push_back(cc);
		//聊天内容
		cc.reset();
		cc.type = 0;
		std::ostringstream text;
		text << "各位你好!我目前的场景是:" << location.scene_id << ",坐标(x=" << location.coord.x << ",y=" << location.coord.y << "), 过来找我吧~{a" << ((rand()%39)+1) << "}";
		cc.text = text.str();
		msg.content.push_back(cc);
		//send
		Block_Buffer buf;
		buf.make_client_message(10300000, player->msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		msg.serialize(buf);
		buf.finish_message();
		player->send_to_server(buf);
	}
}

