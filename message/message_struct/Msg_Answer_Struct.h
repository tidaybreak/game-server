/*
* Generate by devtool
*/

#ifndef _MSG_ANSWER_H_
#define _MSG_ANSWER_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求使用免答
*/
struct MSG_10209001  {
	MSG_10209001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求使用双倍
*/
struct MSG_10209002  {
	MSG_10209002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求使用免答(返回)
*/
struct MSG_50209001 : public Base_Msg  {
	int32_t num;
	MSG_50209001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50209001;
		num = 0;
	}
};

/*
请求使用双倍(返回)
*/
struct MSG_50209002 : public Base_Msg  {
	int32_t num;
	MSG_50209002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50209002;
		num = 0;
	}
};

/*
请求获取免答和双倍次数(返回)
*/
struct MSG_50209003 : public Base_Msg  {
	uint32_t free;
	uint32_t double_num;
	MSG_50209003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(free);
		w.write_uint32(double_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(free)  ||  r.read_uint32(double_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50209003;
		free = 0;
		double_num = 0;
	}
};

/*
请求获取免答和双倍次数
*/
struct MSG_10209003  {
	MSG_10209003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};



#endif