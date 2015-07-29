// -*- C++ -*-
/*
 * Acceptor.h
 *
 *  Created on: Apr 24, 2012
 *      Author: Root
 */

#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include "Thread.h"

class Acceptor: public Thread {
public:
	Acceptor(void);

	virtual ~Acceptor(void);

	void set(int port);

	int init(void);

	int fini(void);

	void server_listen(void);

	void server_accept(void);

	virtual int accept_svc(int connfd);

	virtual void run_handler(void);

	virtual void exit_handler(void);

	int get_port(void);

private:
	int port_;
	int listenfd_;
};

#endif /* ACCEPTOR_H_ */
