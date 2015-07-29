/*
 * Daemon_Gate.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#include "Daemon_Gate.h"
#include "Gate_Monitor.h"
#include "Daemon_Server.h"

Daemon_Gate::Daemon_Gate(void) { }

Daemon_Gate::~Daemon_Gate(void) { }

Daemon_Gate *Daemon_Gate::instance_ = 0;

Daemon_Gate *Daemon_Gate::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Gate;
	return instance_;
}

void Daemon_Gate::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Gate::start_server(int monitor_index) {
	GATE_MONITOR->init(monitor_index);
	GATE_MONITOR->start();

	set_server_running();
}

void Daemon_Gate::stop(void) {
	if (is_server_running()) {
		if (GATE_MONITOR->message_unit()->is_running()) {
			Block_Buffer buf;
			buf.make_message(SERVER_INNER_SELF_CLOSE);
			buf.finish_message();
			GATE_MONITOR->push_async_buff(buf);
		} else {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_GATE);
		}
	}
}


