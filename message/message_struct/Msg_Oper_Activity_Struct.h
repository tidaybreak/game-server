/*
* Generate by devtool
*/

#ifndef _MSG_OPER_ACTIVITY_H_
#define _MSG_OPER_ACTIVITY_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求运营活动信息
*/
struct MSG_10117000  {
	MSG_10117000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求运营活动信息(返回)
*/
struct MSG_50117000 : public Base_Msg  {
	std::vector<Oper_Activity_Item_Info> item;
	MSG_50117000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_vec_size = item.size();
		w.write_uint16(__item_vec_size);
		for(uint16_t i = 0; i < __item_vec_size; ++i) {
				item[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_vec_size;
		if(r.read_uint16(__item_vec_size)  )
				return -1;
		item.reserve(__item_vec_size);
		for(uint16_t i = 0; i < __item_vec_size; ++i) {
				Oper_Activity_Item_Info v;
				if(v.deserialize(r))
						return -1;
				item.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50117000;
		item.clear();
	}
};

/*
请求运营活动领取奖励
*/
struct MSG_10117001  {
	uint32_t act_id;
	uint32_t rec_id;
	MSG_10117001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(act_id);
		w.write_uint32(rec_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(act_id)  ||  r.read_uint32(rec_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		act_id = 0;
		rec_id = 0;
	}
};

/*
请求运营活动领取奖励(返回)
*/
struct MSG_50117001 : public Base_Msg  {
	uint32_t act_id;
	uint32_t rec_id;
	MSG_50117001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(act_id);
		w.write_uint32(rec_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(act_id)  ||  r.read_uint32(rec_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50117001;
		act_id = 0;
		rec_id = 0;
	}
};

/*
请求运营活动图标堆叠数
*/
struct MSG_10117002  {
	MSG_10117002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求运营活动图标堆叠数(返回)
*/
struct MSG_50117002 : public Base_Msg  {
	uint32_t num;
	MSG_50117002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50117002;
		num = 0;
	}
};

/*
请求运营活动单标签信息
*/
struct MSG_10117003  {
	uint32_t act_id;
	MSG_10117003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(act_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(act_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		act_id = 0;
	}
};

/*
请求运营活动单标签信息(返回)
*/
struct MSG_50117003 : public Base_Msg  {
	Oper_Activity_Item_Info item;
	MSG_50117003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		item.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( item.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50117003;
		item.reset();
	}
};



#endif