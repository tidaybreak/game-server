/*
* Generate by devtool
*/

#ifndef _MSG_GANG_H_
#define _MSG_GANG_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
创建公会
*/
struct MSG_10170000  {
	int64_t role_id;
	int8_t logo_id;
	std::string name;
	int8_t money_type;
	MSG_10170000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(logo_id);
		w.write_string(name);
		w.write_int8(money_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(logo_id)  ||  r.read_string(name)  ||  r.read_int8(money_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		logo_id = 0;
		name.clear();
		money_type = 0;
	}
};

/*
创建公会（返回）
备注：
成功返回公会id
*/
struct MSG_50170000 : public Base_Msg  {
	int64_t gang_id;
	int8_t money_type;
	std::string name;
	MSG_50170000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(money_type);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(money_type)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170000;
		gang_id = 0;
		money_type = 0;
		name.clear();
	}
};

/*
加入公会
*/
struct MSG_10170001  {
	int64_t gang_id;
	MSG_10170001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
	}
};

/*
加入公会（返回）
*/
struct MSG_50170001 : public Base_Msg  {
	int64_t gang_id;
	int8_t result;
	std::string gang_name;
	MSG_50170001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(result);
		w.write_string(gang_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(result)  ||  r.read_string(gang_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170001;
		gang_id = 0;
		result = 0;
		gang_name.clear();
	}
};

/*
获取公会信息
*/
struct MSG_10170002  {
	int64_t gang_id;
	MSG_10170002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
	}
};

/*
获取公会信息（返回）
*/
struct MSG_50170002 : public Base_Msg  {
	Gang_Info gang_info;
	MSG_50170002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		gang_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( gang_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170002;
		gang_info.reset();
	}
};

/*
获取个人公会信息
备注：
加入、创建公会后的个人信息
*/
struct MSG_10170003  {
	int64_t role_id;
	MSG_10170003(void) { reset(); };

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
获取个人公会信息（返回）
*/
struct MSG_50170003 : public Base_Msg  {
	Ganger_Info ganger_info;
	MSG_50170003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		ganger_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( ganger_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170003;
		ganger_info.reset();
	}
};

/*
获取公会列表
*/
struct MSG_10170004  {
	MSG_10170004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取公会列表（返回）
*/
struct MSG_50170004 : public Base_Msg  {
	std::vector<Gang_Row_Info> gang_infos;
	MSG_50170004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gang_infos_vec_size = gang_infos.size();
		w.write_uint16(__gang_infos_vec_size);
		for(uint16_t i = 0; i < __gang_infos_vec_size; ++i) {
				gang_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gang_infos_vec_size;
		if(r.read_uint16(__gang_infos_vec_size)  )
				return -1;
		gang_infos.reserve(__gang_infos_vec_size);
		for(uint16_t i = 0; i < __gang_infos_vec_size; ++i) {
				Gang_Row_Info v;
				if(v.deserialize(r))
						return -1;
				gang_infos.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170004;
		gang_infos.clear();
	}
};

/*
获取公会成员列表
*/
struct MSG_10170005  {
	int64_t gang_id;
	MSG_10170005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
	}
};

/*
获取公会成员列表（返回）
*/
struct MSG_50170005 : public Base_Msg  {
	int64_t gang_id;
	std::vector<Ganger_Row_Info> ganger_infos;
	MSG_50170005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		uint16_t __ganger_infos_vec_size = ganger_infos.size();
		w.write_uint16(__ganger_infos_vec_size);
		for(uint16_t i = 0; i < __ganger_infos_vec_size; ++i) {
				ganger_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  )
				return -1;
 		uint16_t __ganger_infos_vec_size;
		if(r.read_uint16(__ganger_infos_vec_size)  )
				return -1;
		ganger_infos.reserve(__ganger_infos_vec_size);
		for(uint16_t i = 0; i < __ganger_infos_vec_size; ++i) {
				Ganger_Row_Info v;
				if(v.deserialize(r))
						return -1;
				ganger_infos.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170005;
		gang_id = 0;
		ganger_infos.clear();
	}
};

/*
退出公会
*/
struct MSG_10170006  {
	MSG_10170006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
退出公会（返回）
*/
struct MSG_50170006 : public Base_Msg  {
	MSG_50170006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170006;
	}
};

/*
转让公会
*/
struct MSG_10170007  {
	int64_t role_id;
	MSG_10170007(void) { reset(); };

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
转让公会（返回）
*/
struct MSG_50170007 : public Base_Msg  {
	MSG_50170007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170007;
	}
};

/*
监听公会信息更新
*/
struct MSG_10170008  {
	int8_t listen;
	MSG_10170008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(listen);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(listen)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		listen = 0;
	}
};

/*
监听公会信息更新（返回）
*/
struct MSG_50170008 : public Base_Msg  {
	MSG_50170008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170008;
	}
};

/*
踢出公会
*/
struct MSG_10170009  {
	int64_t role_id;
	MSG_10170009(void) { reset(); };

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
踢出公会（返回）
*/
struct MSG_50170009 : public Base_Msg  {
	Role_Info kicker_info;
	std::string be_kicked_name;
	MSG_50170009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		kicker_info.serialize(w);
		w.write_string(be_kicked_name);
	}

	int deserialize(Block_Buffer & r) {
		if( kicker_info.deserialize(r)  ||  r.read_string(be_kicked_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170009;
		kicker_info.reset();
		be_kicked_name.clear();
	}
};

/*
修改公会公告
*/
struct MSG_10170010  {
	std::string announce;
	MSG_10170010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(announce);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(announce)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		announce.clear();
	}
};

/*
修改公会公告（返回）
*/
struct MSG_50170010 : public Base_Msg  {
	MSG_50170010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170010;
	}
};

/*
邀请好友加入公会
*/
struct MSG_10170011  {
	std::vector<int64_t> role_ids;
	MSG_10170011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_ids_vec_size = role_ids.size();
		w.write_uint16(__role_ids_vec_size);
		for(uint16_t i = 0; i < __role_ids_vec_size; ++i) {
				w.write_int64(role_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_ids_vec_size;
		if(r.read_uint16(__role_ids_vec_size)  )
				return -1;
		role_ids.reserve(__role_ids_vec_size);
		for(uint16_t i = 0; i < __role_ids_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				role_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_ids.clear();
	}
};

/*
邀请好友加入公会（返回）
*/
struct MSG_50170011 : public Base_Msg  {
	int64_t inviter;
	int64_t gang_id;
	std::vector<int64_t> invitees;
	std::string inviter_name;
	std::string gang_name;
	MSG_50170011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(inviter);
		w.write_int64(gang_id);
		uint16_t __invitees_vec_size = invitees.size();
		w.write_uint16(__invitees_vec_size);
		for(uint16_t i = 0; i < __invitees_vec_size; ++i) {
				w.write_int64(invitees[i]);
		}

		w.write_string(inviter_name);
		w.write_string(gang_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(inviter)  ||  r.read_int64(gang_id)  )
				return -1;
 		uint16_t __invitees_vec_size;
		if(r.read_uint16(__invitees_vec_size)  )
				return -1;
		invitees.reserve(__invitees_vec_size);
		for(uint16_t i = 0; i < __invitees_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				invitees.push_back(v);
		}

		if( r.read_string(inviter_name)  ||  r.read_string(gang_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170011;
		inviter = 0;
		gang_id = 0;
		invitees.clear();
		inviter_name.clear();
		gang_name.clear();
	}
};

/*
获取公会动态
*/
struct MSG_10170012  {
	MSG_10170012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取公会动态（返回）
*/
struct MSG_50170012 : public Base_Msg  {
	std::vector<Gang_Passed_Event> passed_events;
	MSG_50170012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __passed_events_vec_size = passed_events.size();
		w.write_uint16(__passed_events_vec_size);
		for(uint16_t i = 0; i < __passed_events_vec_size; ++i) {
				passed_events[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __passed_events_vec_size;
		if(r.read_uint16(__passed_events_vec_size)  )
				return -1;
		passed_events.reserve(__passed_events_vec_size);
		for(uint16_t i = 0; i < __passed_events_vec_size; ++i) {
				Gang_Passed_Event v;
				if(v.deserialize(r))
						return -1;
				passed_events.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170012;
		passed_events.clear();
	}
};

/*
获取公会简易动态
*/
struct MSG_10170013  {
	MSG_10170013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取公会简易动态（返回）
*/
struct MSG_50170013 : public Base_Msg  {
	std::vector<Int_String> simple_events;
	MSG_50170013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __simple_events_vec_size = simple_events.size();
		w.write_uint16(__simple_events_vec_size);
		for(uint16_t i = 0; i < __simple_events_vec_size; ++i) {
				simple_events[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __simple_events_vec_size;
		if(r.read_uint16(__simple_events_vec_size)  )
				return -1;
		simple_events.reserve(__simple_events_vec_size);
		for(uint16_t i = 0; i < __simple_events_vec_size; ++i) {
				Int_String v;
				if(v.deserialize(r))
						return -1;
				simple_events.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170013;
		simple_events.clear();
	}
};

/*
发送公会群邮件
*/
struct MSG_10170014  {
	Mail_Info mail_info;
	MSG_10170014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		mail_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( mail_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mail_info.reset();
	}
};

/*
发送公会群邮件（返回）
*/
struct MSG_50170014 : public Base_Msg  {
	MSG_50170014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170014;
	}
};

/*
公会捐献
*/
struct MSG_10170015  {
	int8_t type;
	uint32_t money;
	MSG_10170015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_uint32(money);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_uint32(money)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		money = 0;
	}
};

/*
公会捐献（返回）
*/
struct MSG_50170015 : public Base_Msg  {
	int8_t type;
	uint32_t money;
	int32_t get_contrib;
	MSG_50170015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_uint32(money);
		w.write_int32(get_contrib);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_uint32(money)  ||  r.read_int32(get_contrib)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170015;
		type = 0;
		money = 0;
		get_contrib = 0;
	}
};

/*
公会加入设置
*/
struct MSG_10170016  {
	int8_t join_block;
	MSG_10170016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(join_block);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(join_block)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		join_block = 0;
	}
};

/*
公会加入设置（返回）
*/
struct MSG_50170016 : public Base_Msg  {
	MSG_50170016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170016;
	}
};

/*
获取公会邀请列表
*/
struct MSG_10170017  {
	MSG_10170017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取公会邀请列表（返回）
*/
struct MSG_50170017 : public Base_Msg  {
	std::vector<Gang_Invite_Info> invite_list;
	MSG_50170017(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __invite_list_vec_size = invite_list.size();
		w.write_uint16(__invite_list_vec_size);
		for(uint16_t i = 0; i < __invite_list_vec_size; ++i) {
				invite_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __invite_list_vec_size;
		if(r.read_uint16(__invite_list_vec_size)  )
				return -1;
		invite_list.reserve(__invite_list_vec_size);
		for(uint16_t i = 0; i < __invite_list_vec_size; ++i) {
				Gang_Invite_Info v;
				if(v.deserialize(r))
						return -1;
				invite_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170017;
		invite_list.clear();
	}
};

/*
获取公会申请列表
*/
struct MSG_10170018  {
	MSG_10170018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取公会申请列表（返回）
*/
struct MSG_50170018 : public Base_Msg  {
	std::vector<Gang_Apply_Info> apply_list;
	MSG_50170018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __apply_list_vec_size = apply_list.size();
		w.write_uint16(__apply_list_vec_size);
		for(uint16_t i = 0; i < __apply_list_vec_size; ++i) {
				apply_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __apply_list_vec_size;
		if(r.read_uint16(__apply_list_vec_size)  )
				return -1;
		apply_list.reserve(__apply_list_vec_size);
		for(uint16_t i = 0; i < __apply_list_vec_size; ++i) {
				Gang_Apply_Info v;
				if(v.deserialize(r))
						return -1;
				apply_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170018;
		apply_list.clear();
	}
};

/*
公会列表操作[申请,取消]
*/
struct MSG_10170019  {
	int64_t gang_id;
	int8_t action;
	MSG_10170019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		action = 0;
	}
};

/*
邀请列表操作[同意,拒绝]
*/
struct MSG_10170020  {
	int64_t gang_id;
	int8_t action;
	MSG_10170020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		action = 0;
	}
};

/*
申请列表操作[同意,拒绝]
*/
struct MSG_10170021  {
	int64_t role_id;
	int8_t action;
	MSG_10170021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		action = 0;
	}
};

/*
公会列表操作[申请,取消]（返回）
*/
struct MSG_50170019 : public Base_Msg  {
	int64_t gang_id;
	int8_t action;
	MSG_50170019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(action);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(action)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170019;
		gang_id = 0;
		action = 0;
	}
};

/*
邀请列表操作[同意,拒绝]（返回）
*/
struct MSG_50170020 : public Base_Msg  {
	int64_t gang_id;
	int8_t action;
	int8_t result;
	MSG_50170020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int8(action);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int8(action)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170020;
		gang_id = 0;
		action = 0;
		result = 0;
	}
};

/*
申请列表操作[同意,拒绝]
*/
struct MSG_50170021 : public Base_Msg  {
	std::vector<int64_t> done_roles;
	int64_t role_id;
	int8_t action;
	int8_t result;
	MSG_50170021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __done_roles_vec_size = done_roles.size();
		w.write_uint16(__done_roles_vec_size);
		for(uint16_t i = 0; i < __done_roles_vec_size; ++i) {
				w.write_int64(done_roles[i]);
		}

		w.write_int64(role_id);
		w.write_int8(action);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __done_roles_vec_size;
		if(r.read_uint16(__done_roles_vec_size)  )
				return -1;
		done_roles.reserve(__done_roles_vec_size);
		for(uint16_t i = 0; i < __done_roles_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				done_roles.push_back(v);
		}

		if( r.read_int64(role_id)  ||  r.read_int8(action)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170021;
		done_roles.clear();
		role_id = 0;
		action = 0;
		result = 0;
	}
};

/*
改变公会成员职位权限（返回）
*/
struct MSG_50170022 : public Base_Msg  {
	std::string member_role_name;
	int8_t headship;
	int8_t result;
	MSG_50170022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(member_role_name);
		w.write_int8(headship);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(member_role_name)  ||  r.read_int8(headship)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170022;
		member_role_name.clear();
		headship = 0;
		result = 0;
	}
};

/*
公会成员职位权限改变
*/
struct MSG_10170022  {
	int64_t member_role_id;
	int8_t headship;
	std::vector<int8_t> add_auth_vec;
	MSG_10170022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(member_role_id);
		w.write_int8(headship);
		uint16_t __add_auth_vec_vec_size = add_auth_vec.size();
		w.write_uint16(__add_auth_vec_vec_size);
		for(uint16_t i = 0; i < __add_auth_vec_vec_size; ++i) {
				w.write_int8(add_auth_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(member_role_id)  ||  r.read_int8(headship)  )
				return -1;
 		uint16_t __add_auth_vec_vec_size;
		if(r.read_uint16(__add_auth_vec_vec_size)  )
				return -1;
		add_auth_vec.reserve(__add_auth_vec_vec_size);
		for(uint16_t i = 0; i < __add_auth_vec_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				add_auth_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		member_role_id = 0;
		headship = 0;
		add_auth_vec.clear();
	}
};

/*
公会升级
*/
struct MSG_10170023  {
	MSG_10170023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会升级（返回）
*/
struct MSG_50170023 : public Base_Msg  {
	int8_t result;
	int32_t gang_level;
	MSG_50170023(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(gang_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(gang_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170023;
		result = 0;
		gang_level = 0;
	}
};

/*
公会升级发礼包
*/
struct MSG_10170024  {
	MSG_10170024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会升级发礼包（返回）
*/
struct MSG_50170024 : public Base_Msg  {
	int8_t result;
	MSG_50170024(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170024;
		result = 0;
	}
};

/*
公会清升级CD
*/
struct MSG_10170025  {
	int8_t cd_type;
	int32_t building_id;
	MSG_10170025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(cd_type);
		w.write_int32(building_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(cd_type)  ||  r.read_int32(building_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		cd_type = 0;
		building_id = 0;
	}
};

/*
公会清升级CD（返回）
*/
struct MSG_50170025 : public Base_Msg  {
	int8_t result;
	int8_t cd_type;
	int32_t building_id;
	MSG_50170025(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(cd_type);
		w.write_int32(building_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(cd_type)  ||  r.read_int32(building_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170025;
		result = 0;
		cd_type = 0;
		building_id = 0;
	}
};

/*
公会发世界公告
*/
struct MSG_10170026  {
	MSG_10170026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会发世界公告（返回）
*/
struct MSG_50170026 : public Base_Msg  {
	int8_t result;
	MSG_50170026(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170026;
		result = 0;
	}
};

/*
修改公会LOGO
*/
struct MSG_10170027  {
	int8_t logo_id;
	MSG_10170027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(logo_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(logo_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		logo_id = 0;
	}
};

/*
修改公会LOGO（返回）
*/
struct MSG_50170027 : public Base_Msg  {
	int8_t logo_id;
	int8_t result;
	MSG_50170027(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(logo_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(logo_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170027;
		logo_id = 0;
		result = 0;
	}
};

/*
公会夺权
*/
struct MSG_10170028  {
	MSG_10170028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会夺权（返回）
*/
struct MSG_50170028 : public Base_Msg  {
	int8_t result;
	MSG_50170028(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170028;
		result = 0;
	}
};

/*
请求发送公会祝福
*/
struct MSG_10170029  {
	int32_t bless_buf_id;
	int64_t target_role_id;
	MSG_10170029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(bless_buf_id);
		w.write_int64(target_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(bless_buf_id)  ||  r.read_int64(target_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		bless_buf_id = 0;
		target_role_id = 0;
	}
};

/*
请求发送公会祝福（返回）
*/
struct MSG_50170029 : public Base_Msg  {
	int32_t bless_buf_id;
	int8_t result;
	int64_t target_role_id;
	MSG_50170029(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(bless_buf_id);
		w.write_int8(result);
		w.write_int64(target_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(bless_buf_id)  ||  r.read_int8(result)  ||  r.read_int64(target_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170029;
		bless_buf_id = 0;
		result = 0;
		target_role_id = 0;
	}
};

/*
请求研究公会技能
*/
struct MSG_10170030  {
	int32_t skill_id;
	MSG_10170030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
请求研究公会技能（返回）
*/
struct MSG_50170030 : public Base_Msg  {
	int32_t skill_id;
	int8_t result;
	MSG_50170030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170030;
		skill_id = 0;
		result = 0;
	}
};

/*
请求学习公会技能
*/
struct MSG_10170031  {
	int32_t skill_id;
	MSG_10170031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
请求学习公会技能（返回）
*/
struct MSG_50170031 : public Base_Msg  {
	int32_t skill_id;
	int8_t result;
	MSG_50170031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170031;
		skill_id = 0;
		result = 0;
	}
};

/*
公会抽奖
*/
struct MSG_10170032  {
	MSG_10170032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会抽奖（返回）
*/
struct MSG_50170032 : public Base_Msg  {
	int32_t draw_card_id;
	MSG_50170032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(draw_card_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(draw_card_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170032;
		draw_card_id = 0;
	}
};

/*
公会领取抽奖物品
*/
struct MSG_10170033  {
	MSG_10170033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会领取抽奖物品（返回）
*/
struct MSG_50170033 : public Base_Msg  {
	int8_t result;
	MSG_50170033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170033;
		result = 0;
	}
};

/*
公会建筑升级
*/
struct MSG_10170034  {
	int32_t building_id;
	MSG_10170034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(building_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(building_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		building_id = 0;
	}
};

/*
公会建筑升级（返回）
*/
struct MSG_50170034 : public Base_Msg  {
	int32_t building_id;
	int8_t result;
	MSG_50170034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(building_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(building_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170034;
		building_id = 0;
		result = 0;
	}
};

/*
进入公会战
*/
struct MSG_10170035  {
	MSG_10170035(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
进入公会战(返回)
*/
struct MSG_50170035 : public Base_Msg  {
	MSG_50170035(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50170035;
	}
};

/*
请求公会战面板信息
*/
struct MSG_10170036  {
	MSG_10170036(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求公会战面板信息(返回)
*/
struct MSG_50170036 : public Base_Msg  {
	std::vector<Gang_War_Spot> info;
	int8_t icon;
	MSG_50170036(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

		w.write_int8(icon);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				Gang_War_Spot v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		if( r.read_int8(icon)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170036;
		info.clear();
		icon = 0;
	}
};

/*
请求公会战面板结点信息
index:16进八（0-7），8进4（8-11），4进2（12-13），2进1（14）
*/
struct MSG_10170037  {
	int8_t index;
	MSG_10170037(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
	}
};

/*
请求公会战面板结点信息(返回)
index:16进八（0-7），8进4（8-11），4进2（12-13），2进1（14）
*/
struct MSG_50170037 : public Base_Msg  {
	int8_t index;
	std::vector<Gang_War_Rank_Info> rank;
	MSG_50170037(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
		uint16_t __rank_vec_size = rank.size();
		w.write_uint16(__rank_vec_size);
		for(uint16_t i = 0; i < __rank_vec_size; ++i) {
				rank[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		uint16_t __rank_vec_size;
		if(r.read_uint16(__rank_vec_size)  )
				return -1;
		rank.reserve(__rank_vec_size);
		for(uint16_t i = 0; i < __rank_vec_size; ++i) {
				Gang_War_Rank_Info v;
				if(v.deserialize(r))
						return -1;
				rank.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170037;
		index = 0;
		rank.clear();
	}
};

/*
进入公会战
*/
struct MSG_10270000  {
	MSG_10270000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会战请求治疗
*/
struct MSG_10270001  {
	MSG_10270001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求公会战奖励列表
*/
struct MSG_10170038  {
	MSG_10170038(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求公会战奖励列表(返回)
*/
struct MSG_50170038 : public Base_Msg  {
	std::vector<Reward_Info_Gang_War> list;
	MSG_50170038(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Reward_Info_Gang_War v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170038;
		list.clear();
	}
};

/*
请求公会战奖励分配
*/
struct MSG_10170039  {
	Reward_Info_Gang_War award;
	MSG_10170039(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		award.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( award.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		award.reset();
	}
};

/*
请求公会战奖励分配(返回)
*/
struct MSG_50170039 : public Base_Msg  {
	Reward_Info_Gang_War award;
	int8_t can_dispath;
	MSG_50170039(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		award.serialize(w);
		w.write_int8(can_dispath);
	}

	int deserialize(Block_Buffer & r) {
		if( award.deserialize(r)  ||  r.read_int8(can_dispath)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170039;
		award.reset();
		can_dispath = 0;
	}
};

/*
公会战请求决斗
*/
struct MSG_10270002  {
	int64_t role_id;
	MSG_10270002(void) { reset(); };

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
同意公会战决斗
*/
struct MSG_10270004  {
	int8_t type;
	int64_t role_id;
	MSG_10270004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		role_id = 0;
	}
};

/*
公会商店购买道具
*/
struct MSG_10170041  {
	int32_t item_id;
	int32_t amount;
	MSG_10170041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(amount);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(amount)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		amount = 0;
	}
};

/*
公会商店购买道具（返回）
*/
struct MSG_50170041 : public Base_Msg  {
	int32_t item_id;
	int32_t amount;
	int32_t result;
	MSG_50170041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(amount);
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(amount)  ||  r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170041;
		item_id = 0;
		amount = 0;
		result = 0;
	}
};

/*
公会战抽签
*/
struct MSG_10170042  {
	MSG_10170042(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会战抽签（返回）
*/
struct MSG_50170042 : public Base_Msg  {
	int8_t result;
	MSG_50170042(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170042;
		result = 0;
	}
};

/*
公会战领取排名奖励
*/
struct MSG_10170043  {
	MSG_10170043(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会战领取排名奖励（返回）
*/
struct MSG_50170043 : public Base_Msg  {
	int8_t result;
	MSG_50170043(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170043;
		result = 0;
	}
};

/*
公会战排名奖励列表
*/
struct MSG_10170044  {
	MSG_10170044(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
公会战排名奖励列表（返回）
*/
struct MSG_50170044 : public Base_Msg  {
	std::vector<Reward_Info_Gang_War_Rank_Award> list;
	MSG_50170044(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Reward_Info_Gang_War_Rank_Award v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170044;
		list.clear();
	}
};

/*
请求公会战公会排名
*/
struct MSG_10170045  {
	MSG_10170045(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求公会战公会排名(返回)
*/
struct MSG_50170045 : public Base_Msg  {
	std::vector<Gang_War_Gang_Rank> list;
	MSG_50170045(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __list_vec_size = list.size();
		w.write_uint16(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __list_vec_size;
		if(r.read_uint16(__list_vec_size)  )
				return -1;
		list.reserve(__list_vec_size);
		for(uint16_t i = 0; i < __list_vec_size; ++i) {
				Gang_War_Gang_Rank v;
				if(v.deserialize(r))
						return -1;
				list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50170045;
		list.clear();
	}
};

/*
开始修行
*/
struct MSG_10170047  {
	MSG_10170047(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
结束修行
*/
struct MSG_10170048  {
	MSG_10170048(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
开始修行(返回)
*/
struct MSG_50170047 : public Base_Msg  {
	int32_t start_time_secs;
	int32_t phy_power_ttl;
	MSG_50170047(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(start_time_secs);
		w.write_int32(phy_power_ttl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(start_time_secs)  ||  r.read_int32(phy_power_ttl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170047;
		start_time_secs = 0;
		phy_power_ttl = 0;
	}
};

/*
加速修行
*/
struct MSG_10170049  {
	MSG_10170049(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
加速修行(返回）
*/
struct MSG_50170049 : public Base_Msg  {
	int32_t result;
	MSG_50170049(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170049;
		result = 0;
	}
};

/*
停止修行(返回）
*/
struct MSG_50170048 : public Base_Msg  {
	int32_t result;
	MSG_50170048(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50170048;
		result = 0;
	}
};



#endif