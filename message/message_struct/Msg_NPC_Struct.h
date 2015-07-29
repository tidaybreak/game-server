/*
* Generate by devtool
*/

#ifndef _MSG_NPC_H_
#define _MSG_NPC_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取掉落详细信息
*/
struct MSG_10210000  {
	int64_t role_id;
	MSG_10210000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
获取掉落详细信息(返回)
*/
struct MSG_50210000 : public Base_Msg  {
	int64_t role_id;
	std::vector<Drops_Item> item_vec;
	MSG_50210000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size)  )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Drops_Item v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50210000;
		role_id = 0;
		item_vec.clear();
	}
};

/*
领取掉落宝箱物品
*/
struct MSG_10210001  {
	int64_t role_id;
	int8_t index;
	MSG_10210001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		index = 0;
	}
};

/*
领取掉落宝箱物品(返回)
*/
struct MSG_50210001 : public Base_Msg  {
	int64_t role_id;
	int8_t item_index;
	MSG_50210001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(item_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(item_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50210001;
		role_id = 0;
		item_index = 0;
	}
};

/*
开始采集
*/
struct MSG_10210100  {
	int64_t material_role_id;
	MSG_10210100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		material_role_id = 0;
	}
};

/*
开始采集（返回）
*/
struct MSG_50210100 : public Base_Msg  {
	int64_t material_role_id;
	int8_t gather_time;
	int8_t result;
	MSG_50210100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
		w.write_int8(gather_time);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  ||  r.read_int8(gather_time)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50210100;
		material_role_id = 0;
		gather_time = 0;
		result = 0;
	}
};

/*
中断采集
*/
struct MSG_10210101  {
	int64_t material_role_id;
	MSG_10210101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		material_role_id = 0;
	}
};

/*
中断采集（返回）
*/
struct MSG_50210101 : public Base_Msg  {
	int64_t material_role_id;
	MSG_50210101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(material_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(material_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50210101;
		material_role_id = 0;
	}
};

/*
战斗中创建怪物
*/
struct MSG_10210200  {
	int32_t monter_type_id;
	int8_t pos;
	MSG_10210200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(monter_type_id);
		w.write_int8(pos);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(monter_type_id)  ||  r.read_int8(pos)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		monter_type_id = 0;
		pos = 0;
	}
};

/*
开启机关
*/
struct MSG_10210103  {
	int64_t role_id;
	MSG_10210103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
触发机关通知
*/
struct MSG_10210104  {
	uint16_t event_id;
	Coord p_coord;
	Coord m_coord;
	MSG_10210104(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(event_id);
		p_coord.serialize(w);
		m_coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(event_id)  ||  p_coord.deserialize(r)  ||  m_coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		event_id = 0;
		p_coord.reset();
		m_coord.reset();
	}
};

/*
请求拾取物品
*/
struct MSG_10210002  {
	std::vector<int64_t> role_id_vec;
	MSG_10210002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_id_vec_vec_size = role_id_vec.size();
		w.write_uint16(__role_id_vec_vec_size);
		for(uint16_t i = 0; i < __role_id_vec_vec_size; ++i) {
				w.write_int64(role_id_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_id_vec_vec_size;
		if(r.read_uint16(__role_id_vec_vec_size)  )
				return -1;
		role_id_vec.reserve(__role_id_vec_vec_size);
		for(uint16_t i = 0; i < __role_id_vec_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				role_id_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_id_vec.clear();
	}
};

/*
请求拾取物品返回
*/
struct MSG_50210002 : public Base_Msg  {
	uint8_t result;
	MSG_50210002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50210002;
		result = 0;
	}
};

/*
请求在场景刷怪、采集物、宝箱
*/
struct MSG_10210010  {
	int32_t point_id;
	MSG_10210010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
	}
};

/*
请求跟怪点战斗
*/
struct MSG_10210011  {
	int32_t point_id;
	MSG_10210011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
	}
};



#endif