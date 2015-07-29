/*
 * Daemon_Scene.h
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#ifndef DAEMON_MAP_H_
#define DAEMON_MAP_H_

#include "Daemon_Base.h"

class Daemon_Scene: public Daemon_Base {
public:
	static Daemon_Scene *instance(void);

	void start_server(int monitor_index);
	void stop(void);

private:
	Daemon_Scene(void);
	virtual ~Daemon_Scene(void);
	Daemon_Scene(const Daemon_Scene &);
	const Daemon_Scene &operator=(const Daemon_Scene &);

private:
	int config_idx_;
	static Daemon_Scene *instance_;
};

#define DAEMON_SCENE_INSTANCE Daemon_Scene::instance()

#endif /* DAEMON_MAP_H_ */
