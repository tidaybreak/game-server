/*
 * Daemon_Record.h
 *
 *  Created on: Aug 29, 2012
 *      Author: Ti
 */

#ifndef DAEMON_RECORD_H_
#define DAEMON_RECORD_H_

#include "Daemon_Base.h"

class Daemon_Record: public Daemon_Base {
public:
	static Daemon_Record *instance();

	void start_server(int monitor_index);
	void stop(void);


private:
	Daemon_Record(void);
	virtual ~Daemon_Record(void);
	Daemon_Record(const Daemon_Record &);
	const Daemon_Record &operator=(const Daemon_Record &);

private:
	static Daemon_Record *instance_;
};

#define DAEMON_RECORD_INSTANCE Daemon_Record::instance()

#endif /* DAEMON_RECORD_H_ */
