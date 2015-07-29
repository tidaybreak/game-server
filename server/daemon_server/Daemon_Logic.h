/*
 * Daemon_Logic.h
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#ifndef DAEMON_LOGIC_H_
#define DAEMON_LOGIC_H_

#include "Daemon_Base.h"

class Daemon_Logic: public Daemon_Base {
public:
	static Daemon_Logic *instance(void);
	static void destroy(void);

	void start_server(int monitor_index);
	void close_tip(void);
	void stop(void);

private:
	Daemon_Logic(void);
	virtual ~Daemon_Logic(void);
	Daemon_Logic(const Daemon_Logic &);
	const Daemon_Logic &operator=(const Daemon_Logic &);

private:
	static Daemon_Logic *instance_;

};

#define DAEMON_LOGIC_INSTANCE Daemon_Logic::instance()

#endif /* DAEMON_LOGIC_H_ */
