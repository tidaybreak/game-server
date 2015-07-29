/*
* Generate by devtool
*/

#ifndef _MSG_COVENANT_H_
#define _MSG_COVENANT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求开通或续费精灵盟约
*/
struct MSG_10106100  {
	uint8_t type;
	uint32_t id;
	MSG_10106100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		id = 0;
	}
};

/*
请求开通或续费精灵盟约(返回)
*/
struct MSG_50106100 : public Base_Msg  {
	std::vector<uint32_t> id;
	MSG_50106100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_size = id.size();
		w.write_uint16(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				w.write_uint32(id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_size;
		if(r.read_uint16(__id_vec_size)  )
				return -1;
		id.reserve(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106100;
		id.clear();
	}
};

/*
请求赠送灵精盟约
*/
struct MSG_10106101  {
	uint8_t type;
	uint32_t id;
	std::string role_name;
	MSG_10106101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_uint32(id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_uint32(id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		id = 0;
		role_name.clear();
	}
};

/*
请求赠送灵精盟约(返回)
*/
struct MSG_50106101 : public Base_Msg  {
	std::vector<uint32_t> id;
	MSG_50106101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_size = id.size();
		w.write_uint16(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				w.write_uint32(id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_size;
		if(r.read_uint16(__id_vec_size)  )
				return -1;
		id.reserve(__id_vec_size);
		for(uint16_t i = 0; i < __id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106101;
		id.clear();
	}
};

/*
请求开始每天倒计时盟约
*/
struct MSG_10106102  {
	uint32_t id;
	MSG_10106102(void) { reset(); };

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
请求开始每天倒计时盟约(返回)
*/
struct MSG_50106102 : public Base_Msg  {
	uint32_t id;
	uint32_t today_become_time;
	MSG_50106102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(today_become_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(today_become_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106102;
		id = 0;
		today_become_time = 0;
	}
};



#endif