// -*- C++ -*-
/*
 * Receiver.h
 *
 *  Created on: Apr 25, 2012
 *      Author: Root
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Thread.h"
#include "Svc.h"
#include "Epoll_Watcher.h"
#include "Time_Value.h"
#include "List.h"
#include "Lib_Log.h"

class Receiver;

class Receiver_Watcher: public Epoll_Watcher {
public:
	Receiver_Watcher(Receiver *r, int type = 0, int timeout = 0);

	virtual ~Receiver_Watcher(void);

	virtual int inner_cmd(void);

private:
	Receiver *receiver_;
};

class Receiver: public Thread {
	typedef List<int, Thread_Mutex> Drop_List;

public:
	Receiver(void);

	virtual ~Receiver(void);

	int set(Time_Value *timeout = 0);

	void init(void);

	void fini(void);

	virtual void run_handler(void);

	Epoll_Watcher *reactor(void);

	int push_drop(int cid);

	int process_drop(void);

	int register_svc(Svc *svc);

	int unregister_svc(Svc *svc);

	virtual int drop_handler(int cid);

	virtual Svc *find_svc(int cid);

private:
	Drop_List drop_list_;
	Time_Value heartbeat_timeout_;
	Receiver_Watcher *reactor_;
};

#endif /* RECEIVER_H_ */

