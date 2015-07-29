/*
* Generate by devtool
*/

#ifndef _MSG_ANCI_ARTIFACT_H_
#define _MSG_ANCI_ARTIFACT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取神器信息
*/
struct MSG_10101100  {
	MSG_10101100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取神器信息（返回）
*/
struct MSG_50101100 : public Base_Msg  {
	std::vector<Anci_Artifact_Info> info;
	MSG_50101100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Anci_Artifact_Info v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101100;
		info.clear();
	}
};

/*
上古神器解封
*/
struct MSG_10101101  {
	int32_t id;
	MSG_10101101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
上古神器解封(返回)
*/
struct MSG_50101101 : public Base_Msg  {
	int8_t result;
	int32_t id;
	MSG_50101101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101101;
		result = 0;
		id = 0;
	}
};

/*
上古神器条件激活
*/
struct MSG_10101102  {
	int32_t id;
	MSG_10101102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
上古神器条件激活（返回）
*/
struct MSG_50101102 : public Base_Msg  {
	int8_t result;
	int32_t id;
	MSG_50101102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101102;
		result = 0;
		id = 0;
	}
};

/*
上古神器完美宝箱领取
*/
struct MSG_10101103  {
	int32_t id;
	MSG_10101103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
上古神器完美宝箱领取（返回）
*/
struct MSG_50101103 : public Base_Msg  {
	int8_t result;
	int32_t id;
	MSG_50101103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101103;
		result = 0;
		id = 0;
	}
};



#endif