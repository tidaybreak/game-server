/*
* Generate by devtool
*/

#ifndef _MSG_MOUNT_H_
#define _MSG_MOUNT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求坐骑信息
*/
struct MSG_10100600  {
	MSG_10100600(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求坐骑信息返回
*/
struct MSG_50100600 : public Base_Msg  {
	int32_t cur_mount;
	std::vector<Mount_Info> mount_list;
	std::vector<Int_Int> attr_list;
	std::vector<int32_t> base_attr;
	std::vector<int32_t> train_attr;
	std::vector<Mount_Group_Info> mount_group_list;
	int8_t ride;
	MSG_50100600(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(cur_mount);
		uint16_t __mount_list_vec_size = mount_list.size();
		w.write_uint16(__mount_list_vec_size);
		for(uint16_t i = 0; i < __mount_list_vec_size; ++i) {
				mount_list[i].serialize(w);
		}

		uint16_t __attr_list_vec_size = attr_list.size();
		w.write_uint16(__attr_list_vec_size);
		for(uint16_t i = 0; i < __attr_list_vec_size; ++i) {
				attr_list[i].serialize(w);
		}

		uint16_t __base_attr_vec_size = base_attr.size();
		w.write_uint16(__base_attr_vec_size);
		for(uint16_t i = 0; i < __base_attr_vec_size; ++i) {
				w.write_int32(base_attr[i]);
		}

		uint16_t __train_attr_vec_size = train_attr.size();
		w.write_uint16(__train_attr_vec_size);
		for(uint16_t i = 0; i < __train_attr_vec_size; ++i) {
				w.write_int32(train_attr[i]);
		}

		uint16_t __mount_group_list_vec_size = mount_group_list.size();
		w.write_uint16(__mount_group_list_vec_size);
		for(uint16_t i = 0; i < __mount_group_list_vec_size; ++i) {
				mount_group_list[i].serialize(w);
		}

		w.write_int8(ride);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(cur_mount)  )
				return -1;
 		uint16_t __mount_list_vec_size;
		if(r.read_uint16(__mount_list_vec_size)  )
				return -1;
		mount_list.reserve(__mount_list_vec_size);
		for(uint16_t i = 0; i < __mount_list_vec_size; ++i) {
				Mount_Info v;
				if(v.deserialize(r))
						return -1;
				mount_list.push_back(v);
		}

		uint16_t __attr_list_vec_size;
		if(r.read_uint16(__attr_list_vec_size)  )
				return -1;
		attr_list.reserve(__attr_list_vec_size);
		for(uint16_t i = 0; i < __attr_list_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				attr_list.push_back(v);
		}

		uint16_t __base_attr_vec_size;
		if(r.read_uint16(__base_attr_vec_size)  )
				return -1;
		base_attr.reserve(__base_attr_vec_size);
		for(uint16_t i = 0; i < __base_attr_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				base_attr.push_back(v);
		}

		uint16_t __train_attr_vec_size;
		if(r.read_uint16(__train_attr_vec_size)  )
				return -1;
		train_attr.reserve(__train_attr_vec_size);
		for(uint16_t i = 0; i < __train_attr_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				train_attr.push_back(v);
		}

		uint16_t __mount_group_list_vec_size;
		if(r.read_uint16(__mount_group_list_vec_size)  )
				return -1;
		mount_group_list.reserve(__mount_group_list_vec_size);
		for(uint16_t i = 0; i < __mount_group_list_vec_size; ++i) {
				Mount_Group_Info v;
				if(v.deserialize(r))
						return -1;
				mount_group_list.push_back(v);
		}

		if( r.read_int8(ride)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100600;
		cur_mount = 0;
		mount_list.clear();
		attr_list.clear();
		base_attr.clear();
		train_attr.clear();
		mount_group_list.clear();
		ride = 0;
	}
};

/*
请求坐骑马厩信息
*/
struct MSG_10100601  {
	MSG_10100601(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求坐骑马厩信息返回
*/
struct MSG_50100601 : public Base_Msg  {
	std::vector<Int_Int> com_status_list;
	std::vector<Mount_Info> mount_info_list;
	MSG_50100601(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __com_status_list_vec_size = com_status_list.size();
		w.write_uint16(__com_status_list_vec_size);
		for(uint16_t i = 0; i < __com_status_list_vec_size; ++i) {
				com_status_list[i].serialize(w);
		}

		uint16_t __mount_info_list_vec_size = mount_info_list.size();
		w.write_uint16(__mount_info_list_vec_size);
		for(uint16_t i = 0; i < __mount_info_list_vec_size; ++i) {
				mount_info_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __com_status_list_vec_size;
		if(r.read_uint16(__com_status_list_vec_size)  )
				return -1;
		com_status_list.reserve(__com_status_list_vec_size);
		for(uint16_t i = 0; i < __com_status_list_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				com_status_list.push_back(v);
		}

		uint16_t __mount_info_list_vec_size;
		if(r.read_uint16(__mount_info_list_vec_size)  )
				return -1;
		mount_info_list.reserve(__mount_info_list_vec_size);
		for(uint16_t i = 0; i < __mount_info_list_vec_size; ++i) {
				Mount_Info v;
				if(v.deserialize(r))
						return -1;
				mount_info_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100601;
		com_status_list.clear();
		mount_info_list.clear();
	}
};

/*
请求坐骑骑乘
*/
struct MSG_10100602  {
	int8_t ride;
	int32_t mount_id;
	MSG_10100602(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(ride);
		w.write_int32(mount_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(ride)  ||  r.read_int32(mount_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		ride = 0;
		mount_id = 0;
	}
};

/*
请求坐骑骑乘返回
*/
struct MSG_50100602 : public Base_Msg  {
	int8_t result;
	int8_t ride;
	MSG_50100602(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(ride);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(ride)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100602;
		result = 0;
		ride = 0;
	}
};

/*
请求坐骑激活
*/
struct MSG_10100603  {
	int32_t mount_id;
	uint8_t type;
	MSG_10100603(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mount_id = 0;
		type = 0;
	}
};

/*
请求坐骑激活返回
*/
struct MSG_50100603 : public Base_Msg  {
	int8_t result;
	uint8_t type;
	int32_t mount_id;
	MSG_50100603(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_uint8(type);
		w.write_int32(mount_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_uint8(type)  ||  r.read_int32(mount_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100603;
		result = 0;
		type = 0;
		mount_id = 0;
	}
};

/*
请求点击坐骑新组合
*/
struct MSG_10100604  {
	int32_t group_id;
	MSG_10100604(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(group_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(group_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		group_id = 0;
	}
};

/*
请求点击坐骑新组合返回
*/
struct MSG_50100604 : public Base_Msg  {
	int32_t group_id;
	MSG_50100604(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(group_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(group_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100604;
		group_id = 0;
	}
};

/*
请求坐骑培养
*/
struct MSG_10100605  {
	int32_t mount_id;
	int8_t type;
	MSG_10100605(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mount_id = 0;
		type = 0;
	}
};

/*
请求坐骑培养返回
*/
struct MSG_50100605 : public Base_Msg  {
	int32_t mount_id;
	int8_t type;
	int8_t result;
	int8_t rise;
	int8_t train_lv;
	std::vector<int32_t> attr_cur;
	std::vector<int32_t> attr_max;
	std::vector<int32_t> train_attr;
	uint16_t luck_value;
	int8_t evolve;
	int8_t train_rate;
	int8_t lv;
	MSG_50100605(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int8(type);
		w.write_int8(result);
		w.write_int8(rise);
		w.write_int8(train_lv);
		uint16_t __attr_cur_vec_size = attr_cur.size();
		w.write_uint16(__attr_cur_vec_size);
		for(uint16_t i = 0; i < __attr_cur_vec_size; ++i) {
				w.write_int32(attr_cur[i]);
		}

		uint16_t __attr_max_vec_size = attr_max.size();
		w.write_uint16(__attr_max_vec_size);
		for(uint16_t i = 0; i < __attr_max_vec_size; ++i) {
				w.write_int32(attr_max[i]);
		}

		uint16_t __train_attr_vec_size = train_attr.size();
		w.write_uint16(__train_attr_vec_size);
		for(uint16_t i = 0; i < __train_attr_vec_size; ++i) {
				w.write_int32(train_attr[i]);
		}

		w.write_uint16(luck_value);
		w.write_int8(evolve);
		w.write_int8(train_rate);
		w.write_int8(lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int8(type)  ||  r.read_int8(result)  ||  r.read_int8(rise)  ||  r.read_int8(train_lv)  )
				return -1;
 		uint16_t __attr_cur_vec_size;
		if(r.read_uint16(__attr_cur_vec_size)  )
				return -1;
		attr_cur.reserve(__attr_cur_vec_size);
		for(uint16_t i = 0; i < __attr_cur_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				attr_cur.push_back(v);
		}

		uint16_t __attr_max_vec_size;
		if(r.read_uint16(__attr_max_vec_size)  )
				return -1;
		attr_max.reserve(__attr_max_vec_size);
		for(uint16_t i = 0; i < __attr_max_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				attr_max.push_back(v);
		}

		uint16_t __train_attr_vec_size;
		if(r.read_uint16(__train_attr_vec_size)  )
				return -1;
		train_attr.reserve(__train_attr_vec_size);
		for(uint16_t i = 0; i < __train_attr_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				train_attr.push_back(v);
		}

		if( r.read_uint16(luck_value)  ||  r.read_int8(evolve)  ||  r.read_int8(train_rate)  ||  r.read_int8(lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100605;
		mount_id = 0;
		type = 0;
		result = 0;
		rise = 0;
		train_lv = 0;
		attr_cur.clear();
		attr_max.clear();
		train_attr.clear();
		luck_value = 0;
		evolve = 0;
		train_rate = 0;
		lv = 0;
	}
};

/*
请求坐骑上阵
*/
struct MSG_10100606  {
	int32_t mount_id;
	int8_t deploy_pos;
	MSG_10100606(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int8(deploy_pos);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int8(deploy_pos)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mount_id = 0;
		deploy_pos = 0;
	}
};

/*
请求坐骑上阵返回
*/
struct MSG_50100606 : public Base_Msg  {
	int32_t mount_id_1;
	int8_t deploy_pos_1;
	int32_t mount_id_2;
	int8_t deploy_pos_2;
	int8_t result;
	MSG_50100606(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id_1);
		w.write_int8(deploy_pos_1);
		w.write_int32(mount_id_2);
		w.write_int8(deploy_pos_2);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id_1)  ||  r.read_int8(deploy_pos_1)  ||  r.read_int32(mount_id_2)  ||  r.read_int8(deploy_pos_2)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100606;
		mount_id_1 = 0;
		deploy_pos_1 = 0;
		mount_id_2 = 0;
		deploy_pos_2 = 0;
		result = 0;
	}
};

/*
请求坐骑学习技能
*/
struct MSG_10100608  {
	int32_t mount_id;
	int32_t prop_id;
	MSG_10100608(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int32(prop_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int32(prop_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mount_id = 0;
		prop_id = 0;
	}
};

/*
请求坐骑学习技能返回
*/
struct MSG_50100608 : public Base_Msg  {
	int32_t mount_id;
	int32_t skill_id;
	int8_t result;
	int32_t deploy_skill_1;
	int32_t deploy_skill_2;
	int32_t deploy_skill_3;
	MSG_50100608(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int32(skill_id);
		w.write_int8(result);
		w.write_int32(deploy_skill_1);
		w.write_int32(deploy_skill_2);
		w.write_int32(deploy_skill_3);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int32(skill_id)  ||  r.read_int8(result)  ||  r.read_int32(deploy_skill_1)  ||  r.read_int32(deploy_skill_2)  ||  r.read_int32(deploy_skill_3)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100608;
		mount_id = 0;
		skill_id = 0;
		result = 0;
		deploy_skill_1 = 0;
		deploy_skill_2 = 0;
		deploy_skill_3 = 0;
	}
};

/*
请求坐骑融合生效时间
*/
struct MSG_10100609  {
	MSG_10100609(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求坐骑融合生效时间返回
*/
struct MSG_50100609 : public Base_Msg  {
	std::vector<Int_Int_Int> times;
	MSG_50100609(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __times_vec_size = times.size();
		w.write_uint16(__times_vec_size);
		for(uint16_t i = 0; i < __times_vec_size; ++i) {
				times[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __times_vec_size;
		if(r.read_uint16(__times_vec_size)  )
				return -1;
		times.reserve(__times_vec_size);
		for(uint16_t i = 0; i < __times_vec_size; ++i) {
				Int_Int_Int v;
				if(v.deserialize(r))
						return -1;
				times.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100609;
		times.clear();
	}
};



#endif