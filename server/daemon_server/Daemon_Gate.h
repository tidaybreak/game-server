/*
 * Daemon_Gate.h
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#ifndef DAEMON_GATE_H_
#define DAEMON_GATE_H_

#include "Daemon_Base.h"

class Daemon_Gate: public Daemon_Base {
public:
	static Daemon_Gate *instance(void);
	static void destroy(void);

	void start_server(int monitor_index);
	void stop(void);

private:
	Daemon_Gate(void);
	virtual ~Daemon_Gate(void);
	Daemon_Gate(const Daemon_Gate &);
	const Daemon_Gate &operator=(const Daemon_Gate &);

private:
	static Daemon_Gate *instance_;

};

#define DAEMON_GATE_INSTANCE Daemon_Gate::instance()

#endif /* DAEMON_GATE_H_ */
