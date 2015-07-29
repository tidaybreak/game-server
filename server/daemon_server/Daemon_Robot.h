/*
 * Daemon_Robot.h
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#ifndef DAEMON_ROBOT_H_
#define DAEMON_ROBOT_H_

#include "Daemon_Base.h"

class Daemon_Robot: public Daemon_Base {
public:
	static Daemon_Robot *instance(void);
	static void destroy(void);

	void start_server(int monitor_index, char *name);
	void stop(void);

private:
	Daemon_Robot(void);
	virtual ~Daemon_Robot(void);
	Daemon_Robot(const Daemon_Robot &);
	const Daemon_Robot &operator=(const Daemon_Robot &);

private:
	static Daemon_Robot *instance_;

};

#define DAEMON_ROBOT_INSTANCE Daemon_Robot::instance()

#endif /* DAEMON_ROBOT_H_ */
