/*
 * Global_Timer.cpp
 *
 *  Created on: May 30, 2012
 *      Author: Ti
 */

#include "Global_Timer.h"
#include "Pool_Manager.h"
#include "Message_Unit.h"


Global_Timer_Handler::Global_Timer_Handler(void) : msg_buf_(0), unit_(0), loop_times_(0) {

}

Global_Timer_Handler::~Global_Timer_Handler(void) { }

void Global_Timer_Handler::reset(void) {
	Event_Handler::reset();
	unit_ = 0;
	msg_buf_.reset();
	loop_times_ = 0;
}

void Global_Timer_Handler::set_msg_unit(Message_Unit *unit) {
	unit_ = unit;
}

int Global_Timer_Handler::set_msg_buf(Block_Buffer &buf, int times) {
	loop_times_ = times;

	msg_buf_.reset();
	msg_buf_.copy(&buf);

	return 0;
}

int Global_Timer_Handler::handle_remove(void) {
	POOL_MANAGER->push_global_timer_handler_pool(this);
	return 0;
}

int Global_Timer_Handler::handle_timeout(const Time_Value &tv) {
	if (loop_times_ != INT_MAX)
		loop_times_--;

	Block_Buffer *buf = unit_->pop_block(0);
	if (!buf) {
		LOG_USER("error pop buf");
		return -1;
	}


	buf->copy(&msg_buf_);

	unit_->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, Unit_Message::TYPE_UNIT_TIMER);

	if (!loop_times_) {
		set_timer_flag(Epoll_Watcher::EVENT_ONCE_TIMER);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////

Global_Timer::Global_Timer(void) { }

Global_Timer::~Global_Timer(void) { }

Epoll_Watcher &Global_Timer::watcher(void) {
	return watcher_;
}

void Global_Timer::register_once_handler(Block_Buffer &buf, Time_Value delay, Message_Unit *unit) {
	if (!unit) {
		MSG_USER("error unit");
		return ;
	}
	if (delay <= Time_Value::zero) {
		delay = Time_Value(0, 1000);
	}
	Global_Timer_Handler *handler = POOL_MANAGER->pop_global_timer_handler_pool();

	if (!handler) {
		MSG_USER("pop_logic_timer_handler_pool");
		return ;
	}

	handler->set_msg_unit(unit);
	if (handler->set_msg_buf(buf, 1)) {
		MSG_USER("Global_Timer::set_msg_buf");
		return ;
	}

	watcher_.add(handler, Epoll_Watcher::EVENT_TIMEOUT | Epoll_Watcher::EVENT_ONCE_TIMER, &delay);
}

void Global_Timer::register_loop_handler(Block_Buffer &buf, Time_Value loop_time, Message_Unit *unit, int loop_times) {
	if (loop_time <= Time_Value::zero) {
		loop_time = Time_Value(0, 1000);
	}
	Global_Timer_Handler *handler = POOL_MANAGER->pop_global_timer_handler_pool();

	handler->set_msg_unit(unit);
	handler->set_msg_buf(buf, loop_times);
	watcher_.add(handler, Epoll_Watcher::EVENT_TIMEOUT, &loop_time);
}

void Global_Timer::run_handler(void) {
	watcher_.loop();
}

