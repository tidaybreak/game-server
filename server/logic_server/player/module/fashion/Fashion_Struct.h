/*
 * FashionStruct.h
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#ifndef FASHIONSTRUCT_H_
#define FASHIONSTRUCT_H_

#include "Server_Struct.h"

struct Fashion_Part {
	Fashion_Part(void) { reset(); }
	void reset(void) {
		part_id_ = 0;
		lv_ = 0;
		exp_ = 0;
		used_fashion_id_ = 0;
		fashion_id_vec_.clear();
	}

	int part_id_;
	int lv_;
	int exp_;
	int used_fashion_id_;
	Int_Vec fashion_id_vec_;
};
typedef boost::unordered_map<int, Fashion_Part> Fashion_Part_Map;

struct Fashion_Detail : public Detail{
	enum Fashion_Show_State {
		SHOW_OFF	= 0,
		SHOW_ON		= 1,
	};

	Fashion_Detail(void) { reset(); }
	virtual ~Fashion_Detail(void);
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	void set_show_state(int state);
	inline bool get_show_state(void) const;

	int is_show_;
	int lv_;
	int exp_;
	Fashion_Part_Map fashion_part_map_;
};

#endif /* FASHIONSTRUCT_H_ */

inline bool Fashion_Detail::get_show_state(void) const {
	return is_show_ == Fashion_Show_State::SHOW_ON;
}
