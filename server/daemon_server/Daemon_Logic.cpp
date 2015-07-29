/*
 * Daemon_Logic.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#include "Daemon_Logic.h"
#include "Logic_Monitor.h"
#include "Daemon_Server.h"

Daemon_Logic::Daemon_Logic(void) { }

Daemon_Logic::~Daemon_Logic(void) { }

Daemon_Logic *Daemon_Logic::instance_ = 0;

Daemon_Logic *Daemon_Logic::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Logic;
	return instance_;
}

void Daemon_Logic::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Logic::start_server(int monitor_index) {
	LOGIC_MONITOR->init(monitor_index);
	LOGIC_MONITOR->start();
	set_server_running();
}


void Daemon_Logic::stop(void) {
	if (is_server_running()) {
		if (LOGIC_MONITOR->message_unit()->is_running()) {
			Block_Buffer buf;
			buf.make_message(SERVER_INNER_SELF_CLOSE);
			buf.finish_message();
			LOGIC_MONITOR->push_async_buff(buf);
		} else {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_LOGIC);
		}
	}
}

void Daemon_Logic::close_tip(void) {
	if (is_server_running()) {
		if (LOGIC_MONITOR->message_unit()->is_running()) {
			Block_Buffer buf;
			buf.make_message(SERVER_INNER_CLOSE_TIP);
			buf.finish_message();
			LOGIC_MONITOR->push_async_buff(buf);
		}
	}
}
