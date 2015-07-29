/*
 * DaemonChat.h
 *
 *  Created on: May 7, 2012
 *      Author: Ti
 */

#ifndef DAEMON_CHAT_H_
#define DAEMON_CHAT_H_

#include "Daemon_Base.h"

class Daemon_Chat: public Daemon_Base {
public:
	static Daemon_Chat * instance();

	void start_server(int monitor_index);
	void stop(void);

private:
	Daemon_Chat(void);
	virtual ~Daemon_Chat(void);
	Daemon_Chat(const Daemon_Chat &);
	const Daemon_Chat &operator=(const Daemon_Chat &);

private:
	int config_idx_;
	static Daemon_Chat *instance_;
};

#define DAEMON_CHAT_INSTANCE Daemon_Chat::instance()

#endif /* DAEMON_CHAT_H_ */
