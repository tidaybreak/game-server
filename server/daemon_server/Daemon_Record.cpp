/*
 * Daemon_Log.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: Ti
 */

#include "Daemon_Record.h"
#include "Record_Monitor.h"

Daemon_Record::Daemon_Record(void) { }

Daemon_Record::~Daemon_Record(void) { }

Daemon_Record *Daemon_Record::instance_;

Daemon_Record *Daemon_Record::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Record;
	return instance_;
}

void Daemon_Record::start_server(int monitor_index) {
	RECORD_MONITOR->init(monitor_index);
	RECORD_MONITOR->start();

	set_server_running();
}

void Daemon_Record::stop(void) {
	if (is_server_running()) {
		Block_Buffer buf;
		buf.make_message(SERVER_INNER_SELF_CLOSE);
		buf.finish_message();
		RECORD_MONITOR->push_async_buff(buf);
	}
}
