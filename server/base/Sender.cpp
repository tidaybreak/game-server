/*
 * Sender.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: Root
 */

#include "Sender.h"
#include "Svc.h"
#include "Lib_Log.h"
#include "comm_func.h"

Sender::Sender(void)
: svc_map_(max_fd()),
  is_register_self_(false){
    pthread_mutex_init(&this->_mutex,NULL);
    pthread_cond_init(&this->_cond,NULL);
}

Sender::~Sender(void) { }

void Sender::init(void) {

}

void Sender::fini(void) {

}

void Sender::run_handler(void) {
	//LOG_DEBUG("start sender");
	register_self_timer();

	while(true) {
	    pthread_mutex_lock(&this->_mutex);

	    while(this->append_list_.empty() && this->drop_list_.empty()) {
	        pthread_cond_wait(&this->_cond,&this->_mutex);
	    }

		pthread_mutex_unlock(&this->_mutex);

		append_send_block();
		process_drop();

		GUARD(Svc_Map_Lock, mon, svc_map_lock_);
		for (Svc_Map::iterator it = svc_map_.begin(); it != svc_map_.end(); ++it) {
			if (it->second->is_closed())
				continue;
			it->second->send_data();
		}


	}


	return ;
}

/// 获取、释放一个buf
Block_Buffer *Sender::pop_block(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Sender::push_block(int cid, Block_Buffer *buf) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Sender::push_data_block_with_len(int cid, Block_Buffer &rbuf, bool use_swap) {
	if (rbuf.readable_bytes() <= 0) {
		LOG_USER_TRACE("block readable_bytes = %ul.", rbuf.readable_bytes());
		return -1;
	}

	Block_Buffer *buf = pop_block(cid);
	if (! buf) {
		LOG_USER_TRACE("buf == 0");
		return -1;
	}

	if (use_swap) {
		buf->swap(rbuf);
		int w_idx = buf->get_write_idx();
		int r_idx = buf->get_read_idx() - 4;
		if (r_idx < 0) {
			LOG_TRACE_ABORT("r_idx:%d", r_idx);
			return -1;
		}
		buf->set_read_idx(r_idx);
		buf->set_write_idx(r_idx);
		buf->write_int32(cid);
		buf->set_write_idx(w_idx);
	} else {
		buf->write_int32(cid);
		buf->copy(&rbuf);
	}

	append_list_.push_back(buf);
    pthread_cond_signal(&this->_cond);
	return 0;
}


int Sender::append_send_block(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;
	Svc *svc = 0;

	while ((buf = append_list_.pop_front()) != 0) {
		buf->read_int32(cid);
		if ((svc = find_svc(cid)) != 0) {
			if ((svc->push_send_block(buf)) != 0) {
				push_block(cid, buf);
			}
		} else {
			push_block(cid, buf);
		}
	}
	return 0;
}

int Sender::push_drop(int cid) {
	drop_list_.push_back(cid);
    pthread_cond_signal(&this->_cond);
	return 0;
}

int Sender::process_drop(void) {
	int cid = 0;
	Svc *svc = 0;

	while (! drop_list_.empty()) {
		cid = drop_list_.pop_front();
		if ((svc = find_svc(cid)) != 0) {
			if (svc->get_reg_send()) {
				svc->unregister_send_handler();
				svc->set_reg_send(false);
			}
			drop_handler(cid);
		}
	}
	return 0;
}

int Sender::register_svc(Svc *svc) {
	GUARD(Svc_Map_Lock, mon, svc_map_lock_);
	svc_map_.insert(std::make_pair(svc->get_cid(), svc));
	return 0;
}

int Sender::unregister_svc(Svc *svc) {
	GUARD(Svc_Map_Lock, mon, svc_map_lock_);
	svc_map_.erase(svc->get_cid());
	return 0;
}


int Sender::register_self_timer(void) {
	if (! is_register_self_) {
		is_register_self_ = true;
	}

	return 0;
}

int Sender::drop_handler(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

Svc *Sender::find_svc(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}
