/*
* Generate by devtool
*/

#ifndef _MSG_DAILY_H_
#define _MSG_DAILY_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求日常信息
*/
struct MSG_10106000  {
	MSG_10106000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求日常信息(返回)
*/
struct MSG_50106000 : public Base_Msg  {
	std::vector<Uint_Uint> daily_info;
	std::vector<Uint_Uint> daily_max;
	MSG_50106000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __daily_info_vec_size = daily_info.size();
		w.write_uint16(__daily_info_vec_size);
		for(uint16_t i = 0; i < __daily_info_vec_size; ++i) {
				daily_info[i].serialize(w);
		}

		uint16_t __daily_max_vec_size = daily_max.size();
		w.write_uint16(__daily_max_vec_size);
		for(uint16_t i = 0; i < __daily_max_vec_size; ++i) {
				daily_max[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __daily_info_vec_size;
		if(r.read_uint16(__daily_info_vec_size)  )
				return -1;
		daily_info.reserve(__daily_info_vec_size);
		for(uint16_t i = 0; i < __daily_info_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				daily_info.push_back(v);
		}

		uint16_t __daily_max_vec_size;
		if(r.read_uint16(__daily_max_vec_size)  )
				return -1;
		daily_max.reserve(__daily_max_vec_size);
		for(uint16_t i = 0; i < __daily_max_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				daily_max.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106000;
		daily_info.clear();
		daily_max.clear();
	}
};

/*
请求活跃度信息
*/
struct MSG_10106001  {
	MSG_10106001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活跃度信息(返回)
*/
struct MSG_50106001 : public Base_Msg  {
	std::vector<Uint_Uint> activity_info;
	uint32_t today_activity;
	uint32_t all_activity;
	std::vector<Uint_Uint> today_award_receive;
	std::vector<uint32_t> all_award_receive;
	uint8_t receive_vip_activity;
	uint32_t toady_activity_group;
	MSG_50106001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __activity_info_vec_size = activity_info.size();
		w.write_uint16(__activity_info_vec_size);
		for(uint16_t i = 0; i < __activity_info_vec_size; ++i) {
				activity_info[i].serialize(w);
		}

		w.write_uint32(today_activity);
		w.write_uint32(all_activity);
		uint16_t __today_award_receive_vec_size = today_award_receive.size();
		w.write_uint16(__today_award_receive_vec_size);
		for(uint16_t i = 0; i < __today_award_receive_vec_size; ++i) {
				today_award_receive[i].serialize(w);
		}

		uint16_t __all_award_receive_vec_size = all_award_receive.size();
		w.write_uint16(__all_award_receive_vec_size);
		for(uint16_t i = 0; i < __all_award_receive_vec_size; ++i) {
				w.write_uint32(all_award_receive[i]);
		}

		w.write_uint8(receive_vip_activity);
		w.write_uint32(toady_activity_group);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __activity_info_vec_size;
		if(r.read_uint16(__activity_info_vec_size)  )
				return -1;
		activity_info.reserve(__activity_info_vec_size);
		for(uint16_t i = 0; i < __activity_info_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				activity_info.push_back(v);
		}

		if( r.read_uint32(today_activity)  ||  r.read_uint32(all_activity)  )
				return -1;
 		uint16_t __today_award_receive_vec_size;
		if(r.read_uint16(__today_award_receive_vec_size)  )
				return -1;
		today_award_receive.reserve(__today_award_receive_vec_size);
		for(uint16_t i = 0; i < __today_award_receive_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				today_award_receive.push_back(v);
		}

		uint16_t __all_award_receive_vec_size;
		if(r.read_uint16(__all_award_receive_vec_size)  )
				return -1;
		all_award_receive.reserve(__all_award_receive_vec_size);
		for(uint16_t i = 0; i < __all_award_receive_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				all_award_receive.push_back(v);
		}

		if( r.read_uint8(receive_vip_activity)  ||  r.read_uint32(toady_activity_group)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106001;
		activity_info.clear();
		today_activity = 0;
		all_activity = 0;
		today_award_receive.clear();
		all_award_receive.clear();
		receive_vip_activity = 0;
		toady_activity_group = 0;
	}
};

/*
请求领取活跃度礼包
*/
struct MSG_10106002  {
	uint8_t type;
	uint32_t id;
	MSG_10106002(void) { reset(); };

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
请求领取活跃度礼包(返回)
*/
struct MSG_50106002 : public Base_Msg  {
	uint8_t type;
	uint32_t id;
	MSG_50106002(void) { reset(); };

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
		msg_id = 50106002;
		type = 0;
		id = 0;
	}
};

/*
请求当天开启的活动
*/
struct MSG_10106003  {
	MSG_10106003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求当天开启的活动(返回)
*/
struct MSG_50106003 : public Base_Msg  {
	std::vector<uint32_t> open_active;
	MSG_50106003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __open_active_vec_size = open_active.size();
		w.write_uint16(__open_active_vec_size);
		for(uint16_t i = 0; i < __open_active_vec_size; ++i) {
				w.write_uint32(open_active[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __open_active_vec_size;
		if(r.read_uint16(__open_active_vec_size)  )
				return -1;
		open_active.reserve(__open_active_vec_size);
		for(uint16_t i = 0; i < __open_active_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				open_active.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106003;
		open_active.clear();
	}
};

/*
请求实力提升列表分数
*/
struct MSG_10106004  {
	MSG_10106004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求实力提升列表分数(返回)
*/
struct MSG_50106004 : public Base_Msg  {
	std::vector<Uint_Uint> upgrade;
	MSG_50106004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __upgrade_vec_size = upgrade.size();
		w.write_uint16(__upgrade_vec_size);
		for(uint16_t i = 0; i < __upgrade_vec_size; ++i) {
				upgrade[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __upgrade_vec_size;
		if(r.read_uint16(__upgrade_vec_size)  )
				return -1;
		upgrade.reserve(__upgrade_vec_size);
		for(uint16_t i = 0; i < __upgrade_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				upgrade.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106004;
		upgrade.clear();
	}
};

/*
请求资源不足获得方式状态
*/
struct MSG_10106005  {
	std::vector<uint32_t> get_type;
	std::string tip_name;
	int32_t goodsId;
	uint8_t isAuto;
	MSG_10106005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __get_type_vec_size = get_type.size();
		w.write_uint16(__get_type_vec_size);
		for(uint16_t i = 0; i < __get_type_vec_size; ++i) {
				w.write_uint32(get_type[i]);
		}

		w.write_string(tip_name);
		w.write_int32(goodsId);
		w.write_uint8(isAuto);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __get_type_vec_size;
		if(r.read_uint16(__get_type_vec_size)  )
				return -1;
		get_type.reserve(__get_type_vec_size);
		for(uint16_t i = 0; i < __get_type_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				get_type.push_back(v);
		}

		if( r.read_string(tip_name)  ||  r.read_int32(goodsId)  ||  r.read_uint8(isAuto)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		get_type.clear();
		tip_name.clear();
		goodsId = 0;
		isAuto = 0;
	}
};

/*
请求资源不足获得方式状态(返回)
*/
struct MSG_50106005 : public Base_Msg  {
	std::vector<Upgrade_Get_Info> info;
	std::string tip_name;
	int32_t goodsId;
	uint8_t isAuto;
	MSG_50106005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

		w.write_string(tip_name);
		w.write_int32(goodsId);
		w.write_uint8(isAuto);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Upgrade_Get_Info v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		if( r.read_string(tip_name)  ||  r.read_int32(goodsId)  ||  r.read_uint8(isAuto)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106005;
		info.clear();
		tip_name.clear();
		goodsId = 0;
		isAuto = 0;
	}
};

/*
请求签到
*/
struct MSG_10106006  {
	MSG_10106006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求签到(返回)
*/
struct MSG_50106006 : public Base_Msg  {
	MSG_50106006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50106006;
	}
};



#endif