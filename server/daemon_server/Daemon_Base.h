/*
 * Daemon_Base.h
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#ifndef DAEMON_BASE_H_
#define DAEMON_BASE_H_


class Epoll_Watcher;

class Daemon_Base {
public:
	Daemon_Base(void);
	virtual ~Daemon_Base(void);

	bool is_server_running(void);
	void set_server_running(void);

protected:
	bool is_server_running_;
};

#endif /* DAEMON_BASE_H_ */
