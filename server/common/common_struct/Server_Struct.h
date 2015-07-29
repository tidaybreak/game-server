/*
 * Server_Struct.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *      没事不要改这里
 */

#ifndef SERVER_STRUCT_H_
#define SERVER_STRUCT_H_

#include "Monitor_Struct.h"

struct Detail {
	Detail(void) : is_change(false), role_id(0), logout(false) {};
	virtual ~Detail(void) { }
	/// 重置接口
	virtual void reset(void) = 0;

	/// 序列化反序列化接口，方便将该Detail用于传输(此接口实现方法可参考msg下工具生成的代码)
	virtual int serialize(Block_Buffer &buffer) const = 0;
	virtual int deserialize(Block_Buffer &buffer) = 0;

	/// 数据库存取接口
	virtual int load(void) = 0;
	virtual int save(void) = 0;


	void detail_change(void) {
		is_change = true;
	}

	bool is_change;
	role_id_t role_id;
	bool logout;			// 是否登出状态
};

//////////////////////////////////////////////////////////////////////////////

struct Account_Key {
	Account_Key();
	Account_Key(const std::string account_p, const int agent_num_p, const int server_num_p);
	void reset(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	std::string account_name;
	int agent_num;
	int server_num;

	bool operator==(const Account_Key &key) const {
		return account_name == key.account_name && agent_num == key.agent_num && server_num == key.server_num;
	}
};

inline std::size_t hash_value(const Account_Key &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.account_name);
    boost::hash_combine(seed, key.agent_num);
    boost::hash_combine(seed, key.server_num);
    return seed;
}

////////////////////////////////////////////////////////////////////////

struct Position_Key {
	Position_Key();
	Position_Key(const int p, const int idx);
	void reset(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	enum Position_Idx {
		LINE_START 			= 1,
		LINE_ONE_FIRST		= 1,
		LINE_ONE_FRONT 		= 4,
		LINE_ONE_BEHIND 	= 7,
		LINE_TWO_FIRST		= 2,
		LINE_TWO_FRONT 		= 5,
		LINE_TWO_BEHIND 	= 8,
		LINE_THREE_FIRST	= 3,
		LINE_THREE_FRONT 	= 6,
		LINE_THREE_BEHIND 	= 9,
		LINE_END 			= 9,
	};

	int pos;
	int idx;

	bool operator==(const Position_Key &key) const {
		return pos == key.pos && idx == key.idx;
	}
	bool operator!=(const Position_Key &key) const {
		return pos != key.pos || idx != key.idx;
	}
};

inline std::size_t hash_value(const Position_Key &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.pos);
    boost::hash_combine(seed, key.idx);
    return seed;
}

////////////////////////////////////////////////////////////////////////

struct Scene_Key {
	Scene_Key(void);
	Scene_Key(const int scene_id_, const int type_, const int sub_type_);
	void reset(void);

	int scene_id;		// 场景id
	int sort;			// 层/章
	int sub_sort;		// 节

	bool operator==(const Scene_Key &key) const {
		return scene_id == key.scene_id && sort == key.sort && sub_sort == key.sub_sort;
	}
};

////////////////////////////////////////////////////////////////////////

inline std::size_t hash_value(const Scene_Key &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.scene_id);
    boost::hash_combine(seed, key.sort);
    boost::hash_combine(seed, key.sub_sort);
    return seed;
}

///////////////////////////////////////////////////////////////////////

struct Role_Key {
	Role_Key(void);
	Role_Key(const int career_, const int gender_);
	void reset(void);

	int career;
	int gender;

	bool operator==(const Role_Key &key) const{
		return career == key.career && gender == key.gender;
	}
};

inline std::size_t hash_value(const Role_Key &key) {
	std::size_t seed = 0;
	boost::hash_combine(seed, key.career);
	boost::hash_combine(seed, key.gender);
	return seed;
}

///////////////////////////////////////////////////////////////////////

struct Server_Key {
	Server_Key();
	Server_Key(const int agent_num_p, const int server_num_p);
	void reset(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int agent_num;
	int server_num;

	bool operator==(const Server_Key &key) const {
		return agent_num == key.agent_num && server_num == key.server_num;
	}
};
typedef boost::unordered_map<Server_Key, int> Server_List_Map;

inline std::size_t hash_value(const Server_Key &key) {
    std::size_t seed = 0;
    boost::hash_combine(seed, key.agent_num);
    boost::hash_combine(seed, key.server_num);
    return seed;
}


struct Create_Scene_Args {
	enum {

	};

	int type;
	int sort;
	int sub_sort;
	Time_Value time;
	role_id_t id;
	int int_value;
	bool slave; // true被动传送,非客户端请求
	Create_Scene_Args(void);
	Create_Scene_Args(int type_, role_id_t id_val1 = 0, int i_val1_ = 0, int i_val2_ = 0, int i_val3_ = 0, bool i_val4 = false);
	void reset(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct IP_Info {
	IP_Info(void) { reset(); };
	void reset(void);

	std::string telecom_ip;
	std::string unicom_ip;
};
typedef boost::unordered_map<int, IP_Info> Server_Id_IP_Map;


struct Server_Maintainer_Cache {
	Server_Maintainer_Cache(void) { reset(); };
	void reset(void);

	Time_Value game_open_time; 		// 开服时间
 	Time_Value game_merge_time;		// 合服时间(大于开服时间,小于当前时间才有效)

	int server_identity;
	int game_inter_flag;
	std::string game_server_flag;
	std::string platform_des;
	int server_close_delay;
	int game_solution_id;
	int game_port_prefix;
	int game_ver_type;				// 区别国内版本.0国内，1国外
	int db_work_num;
	int record_id;
	int record_output_type;
	int local_log_level;
	int record_log_level;
	Time_Value inner_tick_time_;
	Time_Value inner_time_out_;
	uint32_t package_frequency;
	std::string md5_key;		// md5验证串
	bool verify_msg_time;		// 验证包时间
	bool config_not_update;		// 配制是否热更
	bool valgrind_allow;

	int command_mode;
	String_Set command;

	std::string chat_monitor_url;
	std::string chat_monitor_key;
	std::string pt_server_id;

	std::string mongo_ip;
	int mongo_port;

	std::string mysql_ip;
	int mysql_port;
	std::string mysql_user;
	std::string mysql_pw;
	std::string mysql_dbname;


	int merge_level_lt_;
	int merge_login_day_;

	int default_solution_id;
	std::string center_url;
};

//////////////////////////////////////////////////

struct Mysql_Conf {
	Mysql_Conf(void) { reset(); };
	void reset(void);

	std::string mysql_ip;
	int mysql_port;
	std::string mysql_user;
	std::string mysql_pw;
	std::string mysql_dbname;
};
typedef std::vector<Mysql_Conf> Mysql_Conf_Vec;

struct Monitor_Acceptor {
	Monitor_Acceptor(void) { reset(); };
	void reset(void);

	Server_Enum::MONITOR_TYPE monitor_type;
	int client_port_suffix;
	int client_port;
	int inner_port_suffix;
	int inner_port;
	Time_Value client_recv_timeout;
	Time_Value inner_recv_timeout;
	Monitor_Mark_Vec monitor_id_order_vec_;
};
typedef std::vector<Monitor_Acceptor> Monitor_Acceptor_Vec;

struct Monitor_Info {
	Monitor_Info(void) { reset(); };
	void reset(void);

	std::string command;
	std::string name;
	int run_on;
	int index;
	std::string check;
	Monitor_Acceptor_Vec acceptor_list;
};
typedef std::vector<Monitor_Info> Server_Monitor_Vec;

struct Server_Solution_Cache {
	Server_Solution_Cache(void) { reset(); };
	void reset(void);

	bool exist_muid(Server_Enum::MONITOR_TYPE type, int id, int order, int run_on) const;

	void fetch_muid_by_type(Monitor_Mark_Vec &list, Server_Enum::MONITOR_TYPE type = Server_Enum::MONITOR_ALL) const;

	void fetch_muid_single_monitor(Monitor_Mark_Vec &list, Server_Enum::MONITOR_TYPE type = Server_Enum::MONITOR_ALL) const;

	void fetch_muid_same_process(Monitor_Mark_Vec &list, Monitor_Unique_ID &muid) const;

	// 所有进程取单个monitor
	void fetch_muid_single_process(Monitor_Mark_Vec &list, int run_on = 0) const;

	// muids
	void fetch_muid_by_process_idx(Monitor_Mark_Vec &list, uint process_idx, const Server_Enum::MONITOR_TYPE type = Server_Enum::MONITOR_TYPE::MONITOR_ALL) const;

	// 场景进程id
	int fetch_scene_process_idx(int scene_id, int scene_order) const;

	// 网关里程id
	int fetch_gate_process_idx(int id, int order) const;

	int fetch_order_total(const Server_Enum::MONITOR_TYPE type, const int id) const;

	int fetch_logic_client_port(void) const;

	int solution_id;
	Server_Monitor_Vec monitor_list;
	Int_Set inter_server;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* BASE_STRUCT_H_ */
