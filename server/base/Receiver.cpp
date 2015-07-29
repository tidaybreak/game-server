/*
 * Receiver.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: Root
 */

#include "Receiver.h"

Receiver_Watcher::Receiver_Watcher(Receiver *r, int type, int timeout)
: Epoll_Watcher(type, timeout),
  receiver_(r)
{ }

Receiver_Watcher::~Receiver_Watcher(void) { }

int Receiver_Watcher::inner_cmd(void) {
	receiver_->process_drop();
	return 0;
}

Receiver::Receiver(void)
: heartbeat_timeout_(Time_Value::zero),
  reactor_(0)
{ }

Receiver::~Receiver(void) { }

int Receiver::set(Time_Value *timeout) {
	if (timeout == 0)
		heartbeat_timeout_ = Time_Value::zero;
	else
		heartbeat_timeout_ = *timeout;
	return 0;
}

void Receiver::init(void) {
	if (heartbeat_timeout_ == Time_Value::zero) {
		if ((reactor_ = new Receiver_Watcher(this)) == 0) {
			LOG_ABORT("new Receiver_Watcher errno:%d", errno);
		}
	} else {
		if ((reactor_ = new Receiver_Watcher(this, Epoll_Watcher::WITH_IO_HEARTBEAT, heartbeat_timeout_.sec())) == 0) {
			LOG_ABORT("Receiver new Reactor errno:%d", errno);
		}
	}
}

void Receiver::fini(void) {
	if (reactor_) {
		delete reactor_;
		reactor_ = 0;
	}
}

void Receiver::run_handler(void) {
	//LOG_DEBUG("start receiver");
	reactor_->loop();
}

Epoll_Watcher *Receiver::reactor(void) {
	return reactor_;
}

int Receiver::push_drop(int cid) {
	drop_list_.push_back(cid);
	reactor_->notify();
	return 0;
}

int Receiver::process_drop(void) {
	int cid = 0;
	Svc *svc = 0;

	while (! drop_list_.empty()) {
		cid = drop_list_.pop_front();
		if ((svc = find_svc(cid)) != 0) {
			svc->set_closed(true);
			if (svc->get_reg_recv()) {
				svc->unregister_recv_handler();
				svc->set_reg_recv(false);
			}
		}
		drop_handler(cid);
	}

	return 0;
}

int Receiver::register_svc(Svc *svc) {
	return reactor_->add(svc, Epoll_Watcher::EVENT_INPUT);
}

int Receiver::unregister_svc(Svc *svc) {
	return reactor_->remove(svc);
}

int Receiver::drop_handler(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

Svc *Receiver::find_svc(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}
