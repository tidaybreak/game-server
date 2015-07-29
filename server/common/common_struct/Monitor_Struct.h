/*
 * Monitor_Struct.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *      没事不要改这里
 */

#ifndef MONITOR_STRUCT_H_
#define MONITOR_STRUCT_H_

#include "Game_Typedef.h"
#include "server_define.h"
#include "Record_Client.h"

class Service_Monitor;

////////////////////////////////////////////////////////////////////////////////////////

struct Link_Drop {
	Link_Drop(void) { reset(); };
	Link_Drop(int cid_, int service_type_, int close_reason_, int close_value_) :
		cid(cid_), service_type(service_type_), close_reason(close_reason_), close_value(close_value_) { };
	void reset(void);
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);

	int cid;
	int service_type;
	int close_reason;
	int64_t close_value;
};

////////////////////////////////////////////////////////////////////////////////////////

struct Monitor_Link_Info {
	Monitor_Link_Info(void) { reset(); };
	void reset(void);

	std::string telecom_ip;
	std::string unicom_ip;
	int client_port;
	int inner_port;
};

struct Monitor_Unique_ID {
	Monitor_Unique_ID(void) { reset(); };
	Monitor_Unique_ID(int64_t uuid_, Server_Enum::MONITOR_TYPE type_, int32_t id_, int32_t order_);
	void reset(void);
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);

	int64_t uuid;					// unique id(role id or type id)
	Server_Enum::MONITOR_TYPE type;
	int32_t id;						// order one(scene id or gate id or)
	int32_t order;					// order two(scene order)
	std::string ip;
	int port;
	Service_Monitor *monitor;
};
typedef std::vector<Monitor_Unique_ID> Monitor_Mark_Vec;
typedef boost::unordered_map<Monitor_Unique_ID, Monitor_Link_Info> Monitor_Link_Map;

inline std::size_t hash_value(const Monitor_Unique_ID &map_id) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int>(map_id.uuid));
    boost::hash_combine(seed, static_cast<int>(map_id.type));
    boost::hash_combine(seed, static_cast<int>(map_id.id));
    boost::hash_combine(seed, static_cast<int>(map_id.order));
    return seed;
}

inline bool operator==(const Monitor_Unique_ID &a, const Monitor_Unique_ID &b) {
	return a.uuid == b.uuid && a.type == b.type && a.id == b.id && a.order == b.order;
}

struct Player_Monitor_Link {
	Player_Monitor_Link(void) { reset(); };
	void reset(void);
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);

	Monitor_Unique_ID gate_muid;
	Monitor_Unique_ID logic_muid;
	Monitor_Unique_ID scene_muid;
	Monitor_Unique_ID local_chat_muid;	// 本服聊天服
	Monitor_Unique_ID inter_chat_muid;	// 跨服聊天服
};

struct Scene_Unique_ID {
	Scene_Unique_ID(int id, int order) : scene_id(id), scene_order(order) {};
	Scene_Unique_ID(void) { reset(); };
	void reset(void);
	int scene_id;
	int scene_order;
};

inline std::size_t hash_value(const Scene_Unique_ID &scene) {
    std::size_t seed = 0;
    boost::hash_combine(seed, static_cast<int>(scene.scene_id));
    boost::hash_combine(seed, static_cast<int>(scene.scene_order));
    return seed;
}

inline bool operator==(const Scene_Unique_ID &a, const Scene_Unique_ID &b) {
	return a.scene_id == b.scene_id && a.scene_order == b.scene_order;
}

////////////////////////////////////////////////////////////////////////////////////////

struct Message_Stat {
	int msg_id;
	int times;
	double avg;
	Time_Value tv;
	Time_Value min_tv;
	Time_Value max_tv;

	Message_Stat(void) { reset(); }
	void reset(void);
	void count(Time_Value &t);
};
typedef boost::unordered_map<long, Message_Stat> Message_Stat_Map;

////////////////////////////////////////////////////////////////////////////////////////

#endif /* BASE_STRUCT_H_ */
