/*
 * Http_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: xiaoliang
 */

#ifndef HTTP_STRUCT_H_
#define HTTP_STRUCT_H_

#include "Server_Struct.h"

enum HTTP_REQ_TYPE {
	HTTP_GET = 0,
	HTTP_POST = 1,
};

enum HTTP_OP_TYPE {
	HTTP_OP_CHAT_MONITOR = 1,//聊天监控
};

struct Http_Info {
	Http_Info(void);
	void reset(void);
	std::string url;//请求地址
	String_String_Map uri;//HTTP请求URI,或请求体内容
	uint8_t type; //POST or GET, GET:0 / POST:1
	uint32_t op_type;//操作类型
	//需要HTTP请求头或COOKIES以后扩展
};


#endif /* HTTP_STRUCT_H_ */
