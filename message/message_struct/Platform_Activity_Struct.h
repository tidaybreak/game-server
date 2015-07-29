/*
* Generate by devtool
*/

#ifndef _PLATFORM_ACTIVITY_H_
#define _PLATFORM_ACTIVITY_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求领取37会员等级奖励
*/
struct MSG_10112000  {
	uint32_t id;
	MSG_10112000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取37会员等级奖励(返回)
*/
struct MSG_50112000 : public Base_Msg  {
	uint32_t id;
	MSG_50112000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50112000;
		id = 0;
	}
};

/*
请求打开37会员等级奖励
*/
struct MSG_10112001  {
	int32_t pt_vip;
	MSG_10112001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(pt_vip);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(pt_vip)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pt_vip = 0;
	}
};

/*
请求打开37会员等级奖励(返回)
*/
struct MSG_50112001 : public Base_Msg  {
	std::vector<Uint_Uint> status;
	MSG_50112001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __status_vec_size = status.size();
		w.write_uint16(__status_vec_size);
		for(uint16_t i = 0; i < __status_vec_size; ++i) {
				status[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __status_vec_size;
		if(r.read_uint16(__status_vec_size)  )
				return -1;
		status.reserve(__status_vec_size);
		for(uint16_t i = 0; i < __status_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				status.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50112001;
		status.clear();
	}
};



#endif