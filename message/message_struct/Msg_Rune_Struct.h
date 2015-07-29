/*
* Generate by devtool
*/

#ifndef _MSG_RUNE_H_
#define _MSG_RUNE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取符文信息
*/
struct MSG_10103000  {
	MSG_10103000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取符文信息(返回)
*/
struct MSG_50103000 : public Base_Msg  {
	std::vector<Rune_Part_Info> part_infos;
	MSG_50103000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __part_infos_vec_size = part_infos.size();
		w.write_uint16(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				part_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __part_infos_vec_size;
		if(r.read_uint16(__part_infos_vec_size)  )
				return -1;
		part_infos.reserve(__part_infos_vec_size);
		for(uint16_t i = 0; i < __part_infos_vec_size; ++i) {
				Rune_Part_Info v;
				if(v.deserialize(r))
						return -1;
				part_infos.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50103000;
		part_infos.clear();
	}
};

/*
符文激活
*/
struct MSG_10103001  {
	int32_t part;
	int32_t hole_index;
	int8_t auto_buy;
	MSG_10103001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(part);
		w.write_int32(hole_index);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(part)  ||  r.read_int32(hole_index)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		part = 0;
		hole_index = 0;
		auto_buy = 0;
	}
};

/*
符文充能
*/
struct MSG_10103002  {
	int32_t part;
	int32_t hole_index;
	int8_t is_bat;
	int8_t auto_buy;
	MSG_10103002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(part);
		w.write_int32(hole_index);
		w.write_int8(is_bat);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(part)  ||  r.read_int32(hole_index)  ||  r.read_int8(is_bat)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		part = 0;
		hole_index = 0;
		is_bat = 0;
		auto_buy = 0;
	}
};

/*
符文激活(返回)
*/
struct MSG_50103001 : public Base_Msg  {
	int8_t is_activate;
	int8_t auto_buy;
	MSG_50103001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_activate);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_activate)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103001;
		is_activate = 0;
		auto_buy = 0;
	}
};

/*
符文充能(返回)
*/
struct MSG_50103002 : public Base_Msg  {
	int8_t is_success;
	int8_t is_upgrade;
	int8_t auto_buy;
	MSG_50103002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_success);
		w.write_int8(is_upgrade);
		w.write_int8(auto_buy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_success)  ||  r.read_int8(is_upgrade)  ||  r.read_int8(auto_buy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50103002;
		is_success = 0;
		is_upgrade = 0;
		auto_buy = 0;
	}
};



#endif