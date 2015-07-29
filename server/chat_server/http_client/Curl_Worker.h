/*
 * Curl_Worker.h
 *
 *  Created on: Sep 6, 2012
 *      Author: XIAOLIANG
 */

#ifndef CURL_WORKER_H_
#define CURL_WORKER_H_

#include "Thread.h"
#include "List.h"
#include "Http_Struct.h"
#include "Pool_Manager.h"

class Curl_Worker: public Thread {
public:
	typedef List<Http_Info*, Thread_Mutex> HttpQueue;

	Curl_Worker(void);
	virtual ~Curl_Worker(void);

	void run_handler(void);

	bool block_list_empty(void);

	void process_list(void);

	/// 通用Block消息
	int push_data_block(Http_Info *info);


	int process_block(Http_Info *info);
	int process_requests(Http_Info *info);
	int OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid);

private:
	HttpQueue http_queue_;
};

////////////////////////////////////////////////////////////////////////////////

inline int Curl_Worker::push_data_block(Http_Info *info) {
	if (! info) {
//		MSG_USER("info == 0");
		return -1;
	}
	if(http_queue_.size() >= 1000) {
		POOL_MANAGER->push_http_info_pool(info);
		return -1;
	}
	http_queue_.push_back(info);
	return 0;
}

#endif /* CURL_WORKER_H_ */
