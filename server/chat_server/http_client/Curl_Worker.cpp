/*
 * Curl_Worker.cpp
 *
 *  Created on: Sep 6, 2012
 *      Author: XIAOLIANG
 */

#include "Curl_Worker.h"
#include "Pool_Manager.h"
#include <curl/curl.h>
#include <curl/easy.h>

Curl_Worker::Curl_Worker(void) { }

Curl_Worker::~Curl_Worker(void) { }

void Curl_Worker::run_handler(void) {
	process_list();
}

bool Curl_Worker::block_list_empty(void) {
	return http_queue_.empty();
}

void Curl_Worker::process_list(void) {
	bool all_empty = true;
	timespec pause_ts;
	pause_ts.tv_sec = 1;
	pause_ts.tv_nsec = 0;

	while (1) {
		all_empty = true;

		if (!http_queue_.empty()) {
			Http_Info *info = http_queue_.pop_front();
			if (info) {
				all_empty = false;
				process_block(info);
				process_requests(info);
			}
			POOL_MANAGER->push_http_info_pool(info);
		}

		if(all_empty) {
			::nanosleep (&pause_ts, 0);
		}

	}
}

int Curl_Worker::process_block(Http_Info *info) {
	if (! info) {
//		MSG_USER("info == 0");
		return -1;
	}
	switch(info->op_type) {
		case HTTP_OP_CHAT_MONITOR: {//聊天监控
			break;
		}
	}
	return 0;
}

int Curl_Worker::process_requests(Http_Info *info) {
//	PERF_MON("Curl_Worker::process_requests");
	if(!info) {
		return -1;
	}
	switch(info->type) {
		case HTTP_GET: {
			std::stringstream strUrl;
			strUrl << info->url << "?";
			for(String_String_Map::iterator it = info->uri.begin(); it != info->uri.end(); ++it) {
				strUrl << it->first << "=" << it->second << "&";
			}
			 CURLcode res;
			 CURL* curl = curl_easy_init();
			 if(NULL == curl) {
				 return CURLE_FAILED_INIT;
			 }
			 MSG_DEBUG("%s", strUrl.str().c_str());
			 curl_easy_setopt(curl, CURLOPT_URL, strUrl.str().c_str());
			 curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);//需要回调; 也不关心返回值
			 //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
			     /**
			     * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
			     * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
			     */
			 curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			 curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
			 curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
			 res = curl_easy_perform(curl);
			 curl_easy_cleanup(curl);
			break;
		}
		case HTTP_POST: {//还未实现POST
			break;
		}
	}
	return 0;
}

int Curl_Worker::OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid) {
    return 0;
}
