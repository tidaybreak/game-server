/*
 * Logic_Timer_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Logic_Timer_Request.h"
#include "Logic_Monitor.h"
#include "Config_Cache.h"
#include "Game_Manager.h"
#include "DB_Manager.h"
#include "Pool_Manager.h"
#include "Logic_Player.h"
#include "Utility_Func.h"
#include "Logic_Public.h"
#include "Msg_Active_Struct.h"
#include "Global_Timer.h"
struct Chat_Content;

Logic_Timer_Request::Logic_Timer_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = LOGIC_MONITOR;
}

Logic_Timer_Request::~Logic_Timer_Request() {
	// TODO Auto-generated destructor stub
}

int Logic_Timer_Request::monitor_tick(void) {
	Time_Value now = Time_Value::gettimeofday();

	monitor()->tick(now);

	return 0;
}

int Logic_Timer_Request::reboot_server(Block_Buffer &buf) {
	if (LOGIC_PUBLIC->reboot_time() == Time_Value::zero) return 0;

	Time_Value now = Time_Value::gettimeofday();
	if (now > LOGIC_PUBLIC->reboot_time()) {
		Block_Buffer new_buf;
		new_buf.make_message(SERVER_INNER_SELF_CLOSE);
		new_buf.finish_message();
		LOGIC_MONITOR->send_to_all_process(new_buf);
		LOGIC_PUBLIC->set_reboot_time(Time_Value::zero);
	} else {
		std::string cont;
		buf.read_string(cont);
		std::stringstream str_stream;
		str_stream << cont;
		str_stream << LOGIC_PUBLIC->reboot_time().sec() - now.sec();

		MSG_83000000 msg;
		msg.chat_type = 100;

		Chat_Content content;
		content.text = str_stream.str();
		msg.content.push_back(content);

		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);

		monitor_->send_to_all_client(buf);
	}

	return 0;
}

int Logic_Timer_Request::player_trigger_first(int32_t type){
	if(type == TIMER_TRIGGER_DAILY_ZERO){
		player_trigger_daily_zero();
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_DAILY_ZERO);
		buf.finish_message();
		GLOBAL_TIMER->register_loop_handler(buf, Time_Value(Time_Value::ONE_DAY_IN_SECS), monitor()->message_unit());
	}else if(type == TIMER_TRIGGER_DAILY_SIX){
		player_trigger_daily_six();
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_DAILY_SIX);
		buf.finish_message();
		GLOBAL_TIMER->register_loop_handler(buf, Time_Value(Time_Value::ONE_DAY_IN_SECS), monitor()->message_unit());
	}else if(type == TIMER_TRIGGER_DAILY_TWENTY){
		player_trigger_daily_twenty();
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_DAILY_TWENTY);
		buf.finish_message();
		GLOBAL_TIMER->register_loop_handler(buf, Time_Value(Time_Value::ONE_DAY_IN_SECS), monitor()->message_unit());
	}else if(type == TIMER_TRIGGER_WEEKLY_ZERO) {
		player_trigger_weekly_zero();
		Block_Buffer buf;
		buf.make_message(TIMER_TRIGGER_WEEKLY_ZERO);
		buf.finish_message();
		GLOBAL_TIMER->register_loop_handler(buf, Time_Value(Time_Value::ONE_WEEK_IN_SECS), monitor()->message_unit());
	}
	return 0;
}

int Logic_Timer_Request::player_trigger_daily_zero(void) {
	monitor()->player_trigger_daily_zero();
	return 0;
}

int Logic_Timer_Request::player_trigger_daily_six(void) {
	monitor()->player_trigger_daily_six();
	return 0;
}

int Logic_Timer_Request::player_trigger_daily_twenty(void) {
	monitor()->player_trigger_daily_twenty();
	return 0;
}

int Logic_Timer_Request::player_trigger_weekly_zero(void) {
	monitor()->player_trigger_weekly_zero();
	return 0;
}
