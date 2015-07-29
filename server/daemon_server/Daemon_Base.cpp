/*
 * Daemon_Base.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#include "Daemon_Base.h"

Daemon_Base::Daemon_Base(void) : is_server_running_(false) {
}

Daemon_Base::~Daemon_Base(void) { }

bool Daemon_Base::is_server_running(void) {
	return is_server_running_;
}

void Daemon_Base::set_server_running(void) {
	is_server_running_ = true;
}
