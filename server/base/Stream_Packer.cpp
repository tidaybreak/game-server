/*
 * Packer.h
 *
 *  Created on: Oct 26, 2012
 *      Author: Root
 */

#include "Stream_Packer.h"

Stream_Packer::Stream_Packer(void) {
    pthread_mutex_init(&this->_mutex,NULL);
    pthread_cond_init(&this->_cond,NULL);
}

Stream_Packer::~Stream_Packer(void) { }

int Stream_Packer::push_packing_cid(int cid) {
	packing_list_.push_back(cid);
    pthread_cond_signal(&this->_cond);
	return 0;
}

Svc *Stream_Packer::find_svc(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

Block_Buffer *Stream_Packer::pop_block(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Stream_Packer::push_block(int cid, Block_Buffer *block) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Stream_Packer::packed_data_handler(int cid, Block_Vector &block_vec) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Stream_Packer::drop_handler(int cid) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Stream_Packer::push_drop(int cid) {
	drop_list_.push_back(cid);
    pthread_cond_signal(&this->_cond);
	return 0;
}

int Stream_Packer::process_drop(void) {
	int cid = 0;
	while (! drop_list_.empty()) {
		cid = drop_list_.pop_front();
		drop_handler(cid);
	}
	return 0;
}

void Stream_Packer::run_handler(void) {
	//LOG_DEBUG("start packer");
	process();
}

int Stream_Packer::process(void) {

	while (true) {
	    pthread_mutex_lock(&this->_mutex);

	    while(this->packing_list_.empty() && this->drop_list_.empty()) {
	        pthread_cond_wait(&this->_cond,&this->_mutex);
	    }

		pthread_mutex_unlock(&this->_mutex);

		process_packing_list();
		process_drop();

	}

	return 0;
}

int Stream_Packer::process_packing_list(void) {
	Svc *svc = 0;
	int cid = 0;
	Block_Vector block_vec;

	while (! packing_list_.empty()) {
		cid = packing_list_.pop_front();
		if ((svc = find_svc(cid)) == 0) {
			LOG_USER_TRACE("cannot find svc cid = %d.", cid);
			continue;
		}
		block_vec.clear();
		svc->pack_recv_data(block_vec);

		if (block_vec.size()) {
			svc->recv_pack_size(block_vec.size());
			packed_data_handler(cid, block_vec);
		}
	}
	return 0;
}
