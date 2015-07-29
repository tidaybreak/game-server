/*
 * Daemon_Scene.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#include "Daemon_Scene.h"
#include "Scene_Monitor.h"
#include "Daemon_Server.h"

Daemon_Scene::Daemon_Scene(void)
: config_idx_(-1)
{ }

Daemon_Scene::~Daemon_Scene() { }

Daemon_Scene *Daemon_Scene::instance_;

Daemon_Scene *Daemon_Scene::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Scene;
	return instance_;
}

void Daemon_Scene::start_server(int monitor_index) {
	SCENE_MONITOR->init(monitor_index);
	SCENE_MONITOR->start();

	set_server_running();
}

void Daemon_Scene::stop(void) {
	if (is_server_running()) {
		if (SCENE_MONITOR->message_unit()->is_running()) {
			Block_Buffer buf;
			buf.make_message(SERVER_INNER_SELF_CLOSE);
			buf.finish_message();
			SCENE_MONITOR->push_async_buff(buf);
		} else {
			DAEMON_SERVER_INSTANCE->del_server_list(Daemon_Server::MONITOR_SCENE);
		}
	}
}

