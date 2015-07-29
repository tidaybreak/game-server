/*
 * Global_Timer.h
 *
 *  Created on: May 30, 2012
 *      Author: Ti
 */

#ifndef GLOBAL_TIMER_H_
#define GLOBAL_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Block_Buffer.h"
#include "Singleton.h"

class Message_Unit;

class Global_Timer_Handler: public Event_Handler {
public:
	Global_Timer_Handler(void);
	virtual ~Global_Timer_Handler(void);

	void reset(void);

	void set_msg_unit(Message_Unit *unit);
	int set_msg_buf(Block_Buffer &buf, int times);

	virtual int handle_remove(void);
	virtual int handle_timeout(const Time_Value &tv);

private:
	Block_Buffer msg_buf_;
	Message_Unit *unit_;
	int loop_times_;
};

class Global_Timer: public Thread {
public:
	Global_Timer(void);
	virtual ~Global_Timer(void);

	virtual void run_handler(void);

	Epoll_Watcher &watcher(void);

	/// 只触发一次
	void register_once_handler(Block_Buffer &buf, Time_Value delay, Message_Unit *unit);

	/// 触发指定次数,默认INT_MAX 次数无限
	void register_loop_handler(Block_Buffer &buf, Time_Value loop_time, Message_Unit *unit, int loop_times = INT_MAX);

private:
	Global_Timer(const Global_Timer &);
	const Global_Timer &operator=(const Global_Timer &);

private:
	Epoll_Watcher watcher_;
};

typedef Singleton<Global_Timer> Global_Timer_Single;
#define GLOBAL_TIMER    (Global_Timer_Single::instance())

#endif /* GLOBAL_TIMER_H_ */
