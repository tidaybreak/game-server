/*
 * Http_Manager.cpp
 *
 *  Created on: Sep 6, 2012
 *      Author: xiaolang
 */

#include "Http_Manager.h"
#include "Pool_Manager.h"
#include "Curl_Worker.h"
#include "Http_Struct.h"

Http_Manager::Http_Manager(void) {
}

Http_Manager::~Http_Manager(void) { }


int Http_Manager::init(int work_num) {
	Curl_Worker *worker = 0;
	for (int i = 0; i < work_num; ++i) {
		if ((worker = POOL_MANAGER->pop_curl_worker_pool()) == 0) {
			MSG_USER("db_worker_pool_.pop() return 0.");
			continue;
		}
		curl_worker_vec_.push_back(worker);
	}

	return 0;
}



int Http_Manager::start(void) {
	LOG_DEBUG("start db worker num:%d", curl_worker_vec_.size());
	for (Curl_Worker_Vector::iterator it = curl_worker_vec_.begin(); it != curl_worker_vec_.end(); ++it) {
		(*it)->thr_create();
	}

	return 0;
}

int Http_Manager::fini(void) {
	return 0;
}

int Http_Manager::push_data_block(Http_Info &info, int64_t key) {
	Http_Info *http_info = POOL_MANAGER->pop_http_info_pool();
	if (! http_info) {
		//MSG_USER("block_pool_.pop() return 0.");
		return -1;
	}

	*http_info = info;

	curl_worker_vec_[key % curl_worker_vec_.size()]->push_data_block(http_info);

	return 0;
}


bool Http_Manager::curl_worker_info_empty(void) {
	bool is_empty = true;
	for (Curl_Worker_Vector::iterator it = curl_worker_vec_.begin(); it != curl_worker_vec_.end(); ++it) {
		if (!(*it)->block_list_empty()) {
			is_empty = false;
			break;
		}
	}

	return is_empty;
}
