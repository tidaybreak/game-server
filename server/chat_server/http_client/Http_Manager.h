/*
 * Http_Manager.h
 *
 *  Created on: Sep 6, 2012
 *      Author: xiaoliang
 */

#ifndef HTTP_MANAGER_H_
#define HTTP_MANAGER_H_

#include "Game_Typedef.h"

class Curl_Worker;
struct Http_Info;

class Http_Manager {
public:
	typedef std::vector<Curl_Worker *> Curl_Worker_Vector;

	Http_Manager(void);
	virtual ~Http_Manager(void);
	Http_Manager(const Http_Manager &);

	int init(int work_num);

	int start(void);

	int fini(void);

	/// 插入HTTP操作消息
	int push_data_block(Http_Info &info, int64_t key = 0);


	bool curl_worker_info_empty(void);

private:

//	const Http_Manager &operator=(const Http_Manager &);

private:
	Curl_Worker_Vector curl_worker_vec_;
};
typedef Singleton<Http_Manager> Http_Manager_Single;
#define HTTP_MANAGER    (Http_Manager_Single::instance())

////////////////////////////////////////////////////////////////////////////////


#endif /* HTTP_MANAGER_H_ */
