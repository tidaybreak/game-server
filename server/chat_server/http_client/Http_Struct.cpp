/*
 * Http_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: xiaoliang
 */

#include "Http_Struct.h"


Http_Info::Http_Info(void) {
	reset();
}

void Http_Info::reset(void) {
	uri.clear();
	type = HTTP_GET;
	op_type = 0;
	url.clear();
}
