/*
 * Daemon_Robot.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#include "Daemon_Robot.h"
#include "Robot_Monitor.h"

Daemon_Robot::Daemon_Robot(void) { }

Daemon_Robot::~Daemon_Robot(void) { }

Daemon_Robot *Daemon_Robot::instance_ = 0;

Daemon_Robot *Daemon_Robot::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Robot;
	return instance_;
}

void Daemon_Robot::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Robot::start_server(int monitor_index, char *name) {
	ROBOT_MONITOR->init(monitor_index);
	ROBOT_MONITOR->start(name);

	set_server_running();
}

void Daemon_Robot::stop(void) {
	if (is_server_running()) {
		Block_Buffer buf;
		buf.make_message(SERVER_INNER_SELF_CLOSE);
		buf.finish_message();
		ROBOT_MONITOR->push_async_buff(buf);
	}
}


