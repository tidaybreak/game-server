/*
* Generate by devtool
*/

#ifndef _MSG_FRIEND_H_
#define _MSG_FRIEND_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取好友基本信息
*/
struct MSG_10160001  {
	int64_t role_id;
	MSG_10160001(void) { reset(); };

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
获取好友列表
*/
struct MSG_10160000  {
	MSG_10160000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
添加好友
*/
struct MSG_10160004  {
	std::string role_name;
	MSG_10160004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_name.clear();
	}
};

/*
获取好友列表返回
*/
struct MSG_50160000 : public Base_Msg  {
	std::vector<Friend_Info> friend_info_list;
	std::vector<Friend_Info> stranger_info_list;
	std::vector<Friend_Info> black_info_list;
	int32_t head_photo_id;
	MSG_50160000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __friend_info_list_vec_size = friend_info_list.size();
		w.write_uint16(__friend_info_list_vec_size);
		for(uint16_t i = 0; i < __friend_info_list_vec_size; ++i) {
				friend_info_list[i].serialize(w);
		}

		uint16_t __stranger_info_list_vec_size = stranger_info_list.size();
		w.write_uint16(__stranger_info_list_vec_size);
		for(uint16_t i = 0; i < __stranger_info_list_vec_size; ++i) {
				stranger_info_list[i].serialize(w);
		}

		uint16_t __black_info_list_vec_size = black_info_list.size();
		w.write_uint16(__black_info_list_vec_size);
		for(uint16_t i = 0; i < __black_info_list_vec_size; ++i) {
				black_info_list[i].serialize(w);
		}

		w.write_int32(head_photo_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __friend_info_list_vec_size;
		if(r.read_uint16(__friend_info_list_vec_size)  )
				return -1;
		friend_info_list.reserve(__friend_info_list_vec_size);
		for(uint16_t i = 0; i < __friend_info_list_vec_size; ++i) {
				Friend_Info v;
				if(v.deserialize(r))
						return -1;
				friend_info_list.push_back(v);
		}

		uint16_t __stranger_info_list_vec_size;
		if(r.read_uint16(__stranger_info_list_vec_size)  )
				return -1;
		stranger_info_list.reserve(__stranger_info_list_vec_size);
		for(uint16_t i = 0; i < __stranger_info_list_vec_size; ++i) {
				Friend_Info v;
				if(v.deserialize(r))
						return -1;
				stranger_info_list.push_back(v);
		}

		uint16_t __black_info_list_vec_size;
		if(r.read_uint16(__black_info_list_vec_size)  )
				return -1;
		black_info_list.reserve(__black_info_list_vec_size);
		for(uint16_t i = 0; i < __black_info_list_vec_size; ++i) {
				Friend_Info v;
				if(v.deserialize(r))
						return -1;
				black_info_list.push_back(v);
		}

		if( r.read_int32(head_photo_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50160000;
		friend_info_list.clear();
		stranger_info_list.clear();
		black_info_list.clear();
		head_photo_id = 0;
	}
};

/*
获取好友基本信息返回
*/
struct MSG_50160001 : public Base_Msg  {
	Friend_Info friend_info;
	MSG_50160001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		friend_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( friend_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50160001;
		friend_info.reset();
	}
};

/*
获取好友申请列表
*/
struct MSG_10160002  {
	MSG_10160002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取好友申请列表返回
*/
struct MSG_50160002 : public Base_Msg  {
	std::vector<Friend_Apply_Info> apply_lists;
	MSG_50160002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __apply_lists_vec_size = apply_lists.size();
		w.write_uint16(__apply_lists_vec_size);
		for(uint16_t i = 0; i < __apply_lists_vec_size; ++i) {
				apply_lists[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __apply_lists_vec_size;
		if(r.read_uint16(__apply_lists_vec_size)  )
				return -1;
		apply_lists.reserve(__apply_lists_vec_size);
		for(uint16_t i = 0; i < __apply_lists_vec_size; ++i) {
				Friend_Apply_Info v;
				if(v.deserialize(r))
						return -1;
				apply_lists.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50160002;
		apply_lists.clear();
	}
};

/*
添加好友返回
*/
struct MSG_50160004 : public Base_Msg  {
	int8_t status;
	std::string role_name;
	MSG_50160004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(status);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(status)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50160004;
		status = 0;
		role_name.clear();
	}
};

/*
删除好友
*/
struct MSG_10160003  {
	int64_t friend_id;
	int8_t friend_type;
	MSG_10160003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(friend_id);
		w.write_int8(friend_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(friend_id)  ||  r.read_int8(friend_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		friend_id = 0;
		friend_type = 0;
	}
};

/*
删除好友
*/
struct MSG_50160003 : public Base_Msg  {
	int8_t r_value;
	MSG_50160003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(r_value);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(r_value)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50160003;
		r_value = 0;
	}
};

/*
拉进黑名单
*/
struct MSG_10160005  {
	std::string role_name;
	MSG_10160005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_name.clear();
	}
};

/*
拉进黑名单返回
*/
struct MSG_50160005 : public Base_Msg  {
	int8_t status;
	std::string role_name;
	MSG_50160005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(status);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(status)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50160005;
		status = 0;
		role_name.clear();
	}
};

/*
处理好友申请
*/
struct MSG_10160009  {
	int8_t handel_friend_apply;
	std::string role_name;
	MSG_10160009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(handel_friend_apply);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(handel_friend_apply)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		handel_friend_apply = 0;
		role_name.clear();
	}
};

/*
获取角色在线状态
*/
struct MSG_10160010  {
	std::vector<int64_t> stranger_id_list;
	std::vector<int64_t> recent_id_list;
	MSG_10160010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __stranger_id_list_vec_size = stranger_id_list.size();
		w.write_uint16(__stranger_id_list_vec_size);
		for(uint16_t i = 0; i < __stranger_id_list_vec_size; ++i) {
				w.write_int64(stranger_id_list[i]);
		}

		uint16_t __recent_id_list_vec_size = recent_id_list.size();
		w.write_uint16(__recent_id_list_vec_size);
		for(uint16_t i = 0; i < __recent_id_list_vec_size; ++i) {
				w.write_int64(recent_id_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __stranger_id_list_vec_size;
		if(r.read_uint16(__stranger_id_list_vec_size)  )
				return -1;
		stranger_id_list.reserve(__stranger_id_list_vec_size);
		for(uint16_t i = 0; i < __stranger_id_list_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				stranger_id_list.push_back(v);
		}

		uint16_t __recent_id_list_vec_size;
		if(r.read_uint16(__recent_id_list_vec_size)  )
				return -1;
		recent_id_list.reserve(__recent_id_list_vec_size);
		for(uint16_t i = 0; i < __recent_id_list_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				recent_id_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		stranger_id_list.clear();
		recent_id_list.clear();
	}
};

/*
获取角色在线状态(返回)
*/
struct MSG_50160010 : public Base_Msg  {
	std::vector<Int_Role_Id> stranger_online;
	std::vector<Int_Role_Id> recent_online;
	MSG_50160010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __stranger_online_vec_size = stranger_online.size();
		w.write_uint16(__stranger_online_vec_size);
		for(uint16_t i = 0; i < __stranger_online_vec_size; ++i) {
				stranger_online[i].serialize(w);
		}

		uint16_t __recent_online_vec_size = recent_online.size();
		w.write_uint16(__recent_online_vec_size);
		for(uint16_t i = 0; i < __recent_online_vec_size; ++i) {
				recent_online[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __stranger_online_vec_size;
		if(r.read_uint16(__stranger_online_vec_size)  )
				return -1;
		stranger_online.reserve(__stranger_online_vec_size);
		for(uint16_t i = 0; i < __stranger_online_vec_size; ++i) {
				Int_Role_Id v;
				if(v.deserialize(r))
						return -1;
				stranger_online.push_back(v);
		}

		uint16_t __recent_online_vec_size;
		if(r.read_uint16(__recent_online_vec_size)  )
				return -1;
		recent_online.reserve(__recent_online_vec_size);
		for(uint16_t i = 0; i < __recent_online_vec_size; ++i) {
				Int_Role_Id v;
				if(v.deserialize(r))
						return -1;
				recent_online.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50160010;
		stranger_online.clear();
		recent_online.clear();
	}
};

/*
获取设置头像列表(返回)
*/
struct MSG_50160011 : public Base_Msg  {
	std::vector<Int_Int> photo_list;
	MSG_50160011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __photo_list_vec_size = photo_list.size();
		w.write_uint16(__photo_list_vec_size);
		for(uint16_t i = 0; i < __photo_list_vec_size; ++i) {
				photo_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __photo_list_vec_size;
		if(r.read_uint16(__photo_list_vec_size)  )
				return -1;
		photo_list.reserve(__photo_list_vec_size);
		for(uint16_t i = 0; i < __photo_list_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				photo_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50160011;
		photo_list.clear();
	}
};

/*
获取设置头像列表
*/
struct MSG_10160011  {
	MSG_10160011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求设置头像
*/
struct MSG_10160012  {
	int32_t photo_id;
	MSG_10160012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(photo_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(photo_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		photo_id = 0;
	}
};

/*
请求设置头像(返回)
*/
struct MSG_50160012 : public Base_Msg  {
	int32_t photo_id;
	MSG_50160012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(photo_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(photo_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50160012;
		photo_id = 0;
	}
};

/*
批量添加好友
*/
struct MSG_10160013  {
	std::vector<std::string> role_name_vec;
	MSG_10160013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_name_vec_vec_size = role_name_vec.size();
		w.write_uint16(__role_name_vec_vec_size);
		for(uint16_t i = 0; i < __role_name_vec_vec_size; ++i) {
				w.write_string(role_name_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_name_vec_vec_size;
		if(r.read_uint16(__role_name_vec_vec_size)  )
				return -1;
		role_name_vec.reserve(__role_name_vec_vec_size);
		for(uint16_t i = 0; i < __role_name_vec_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				role_name_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_name_vec.clear();
	}
};

/*
批量添加好友(返回)
*/
struct MSG_50160013 : public Base_Msg  {
	std::vector<Int_String> role_name_status;
	MSG_50160013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_name_status_vec_size = role_name_status.size();
		w.write_uint16(__role_name_status_vec_size);
		for(uint16_t i = 0; i < __role_name_status_vec_size; ++i) {
				role_name_status[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_name_status_vec_size;
		if(r.read_uint16(__role_name_status_vec_size)  )
				return -1;
		role_name_status.reserve(__role_name_status_vec_size);
		for(uint16_t i = 0; i < __role_name_status_vec_size; ++i) {
				Int_String v;
				if(v.deserialize(r))
						return -1;
				role_name_status.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50160013;
		role_name_status.clear();
	}
};

/*
获取好友基础信息列表
*/
struct MSG_10160014  {
	MSG_10160014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取好友基础信息列表(返回)
*/
struct MSG_50160014 : public Base_Msg  {
	std::vector<Friend_Base_Info> friend_list;
	MSG_50160014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __friend_list_vec_size = friend_list.size();
		w.write_uint16(__friend_list_vec_size);
		for(uint16_t i = 0; i < __friend_list_vec_size; ++i) {
				friend_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __friend_list_vec_size;
		if(r.read_uint16(__friend_list_vec_size)  )
				return -1;
		friend_list.reserve(__friend_list_vec_size);
		for(uint16_t i = 0; i < __friend_list_vec_size; ++i) {
				Friend_Base_Info v;
				if(v.deserialize(r))
						return -1;
				friend_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50160014;
		friend_list.clear();
	}
};



#endif