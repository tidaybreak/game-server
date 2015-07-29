/*
 * DaemonChat.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#include "Daemon_Chat.h"
#include "Chat_Monitor.h"
#include "Daemon_Server.h"

Daemon_Chat::Daemon_Chat(void)
: config_idx_(0)
{ }

Daemon_Chat::~Daemon_Chat(void) { }

Daemon_Chat *Daemon_Chat::instance_;

Daemon_Chat *Daemon_Chat::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Chat;
	return instance_;
}

void Daemon_Chat::start_server(int monitor_index) {
	CHAT_MONITOR->init(monitor_index);
	CHAT_MONITOR->start();

	set_server_running();
}

void Daemon_Chat::stop(void) {
	if (is_server_running()) {
		if (CHAT_MONITOR->message_unit()->is_running()) {
			Block_Buffer buf;
			buf.make_message(SERVER_INNER_SELF_CLOSE);
			buf.finish_message();
			CHAT_MONITOR->push_async_buff(buf);
		} else {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_CHAT);
		}
	}
}
