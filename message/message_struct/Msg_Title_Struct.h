/*
* Generate by devtool
*/

#ifndef _MSG_TITLE_H_
#define _MSG_TITLE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取称号信息
*/
struct MSG_10100900  {
	MSG_10100900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取称号信息(返回)
*/
struct MSG_50100900 : public Base_Msg  {
	std::vector<Int_Int> title_vector;
	int32_t equip_title;
	int8_t task_complete;
	MSG_50100900(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __title_vector_vec_size = title_vector.size();
		w.write_uint16(__title_vector_vec_size);
		for(uint16_t i = 0; i < __title_vector_vec_size; ++i) {
				title_vector[i].serialize(w);
		}

		w.write_int32(equip_title);
		w.write_int8(task_complete);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __title_vector_vec_size;
		if(r.read_uint16(__title_vector_vec_size)  )
				return -1;
		title_vector.reserve(__title_vector_vec_size);
		for(uint16_t i = 0; i < __title_vector_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				title_vector.push_back(v);
		}

		if( r.read_int32(equip_title)  ||  r.read_int8(task_complete)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100900;
		title_vector.clear();
		equip_title = 0;
		task_complete = 0;
	}
};

/*
称号穿/脱
*/
struct MSG_10100901  {
	int32_t title_id;
	int8_t op;
	MSG_10100901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(title_id);
		w.write_int8(op);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(title_id)  ||  r.read_int8(op)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		title_id = 0;
		op = 0;
	}
};

/*
称号穿/脱(返回)
*/
struct MSG_50100901 : public Base_Msg  {
	int32_t title_id;
	int8_t op;
	MSG_50100901(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(title_id);
		w.write_int8(op);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(title_id)  ||  r.read_int8(op)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100901;
		title_id = 0;
		op = 0;
	}
};



#endif