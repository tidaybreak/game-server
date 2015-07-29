/*
* Generate by devtool
*/

#ifndef _MSG_ALCHEMY_H_
#define _MSG_ALCHEMY_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取炼金面板信息
*/
struct MSG_10152000  {
	uint8_t mtype;
	MSG_10152000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(mtype);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(mtype)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mtype = 0;
	}
};

/*
获取炼金面板信息(返回)
*/
struct MSG_50152000 : public Base_Msg  {
	uint32_t current_hero_critical;
	uint32_t current_hero_gold;
	uint8_t over_free_bout;
	uint8_t over_pay_bout;
	uint8_t next_pay_diamond;
	int32_t init_gold;
	uint8_t last_tabs;
	uint8_t mtype;
	MSG_50152000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(current_hero_critical);
		w.write_uint32(current_hero_gold);
		w.write_uint8(over_free_bout);
		w.write_uint8(over_pay_bout);
		w.write_uint8(next_pay_diamond);
		w.write_int32(init_gold);
		w.write_uint8(last_tabs);
		w.write_uint8(mtype);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(current_hero_critical)  ||  r.read_uint32(current_hero_gold)  ||  r.read_uint8(over_free_bout)  ||  r.read_uint8(over_pay_bout)  ||  r.read_uint8(next_pay_diamond)  ||  r.read_int32(init_gold)  ||  r.read_uint8(last_tabs)  ||  r.read_uint8(mtype)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50152000;
		current_hero_critical = 0;
		current_hero_gold = 0;
		over_free_bout = 0;
		over_pay_bout = 0;
		next_pay_diamond = 0;
		init_gold = 0;
		last_tabs = 0;
		mtype = 0;
	}
};

/*
请求炼金
*/
struct MSG_10152001  {
	uint8_t tabs;
	uint8_t pay_type;
	uint8_t mtype;
	MSG_10152001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(tabs);
		w.write_uint8(pay_type);
		w.write_uint8(mtype);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(tabs)  ||  r.read_uint8(pay_type)  ||  r.read_uint8(mtype)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		tabs = 0;
		pay_type = 0;
		mtype = 0;
	}
};

/*
请求炼金(返回)
*/
struct MSG_50152001 : public Base_Msg  {
	uint8_t over_free_bout;
	uint8_t over_pay_bout;
	uint8_t next_pay_diamond;
	int32_t get_gold;
	uint8_t critical;
	uint8_t mtype;
	MSG_50152001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(over_free_bout);
		w.write_uint8(over_pay_bout);
		w.write_uint8(next_pay_diamond);
		w.write_int32(get_gold);
		w.write_uint8(critical);
		w.write_uint8(mtype);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(over_free_bout)  ||  r.read_uint8(over_pay_bout)  ||  r.read_uint8(next_pay_diamond)  ||  r.read_int32(get_gold)  ||  r.read_uint8(critical)  ||  r.read_uint8(mtype)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50152001;
		over_free_bout = 0;
		over_pay_bout = 0;
		next_pay_diamond = 0;
		get_gold = 0;
		critical = 0;
		mtype = 0;
	}
};



#endif