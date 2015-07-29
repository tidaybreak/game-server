/*
 * Monitor_Struct.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ti
 */

#include "Monitor_Struct.h"

void Link_Drop::reset(void) {
	cid = 0;
	service_type = 0;
	close_reason = 0;
	close_value = 0;
}

void Link_Drop::serialize(Block_Buffer & w) const {
	w.write_int32(cid);
	w.write_int32(service_type);
	w.write_int32(close_reason);
	w.write_int64(close_value);
}

int Link_Drop::deserialize(Block_Buffer & r) {
	if( r.read_int32(cid)  ||  r.read_int32(service_type)  ||  r.read_int32(close_reason)  || r.read_int64(close_value)  )
			return -1;
		return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

void Monitor_Link_Info::reset(void) {
	telecom_ip = "";
	unicom_ip = "";
	client_port = 0;
	inner_port = 0;
}

Monitor_Unique_ID::Monitor_Unique_ID(int64_t uuid_, Server_Enum::MONITOR_TYPE type_, int32_t id_, int32_t order_) :
	uuid(uuid_), type(type_), id(id_), order(order_), port(0), monitor(0) {}

void Monitor_Unique_ID::reset(void) {
	uuid = 0;
	type = Server_Enum::MONITOR_END;
	id = 0;
	order = 0;
	ip = "";
	port = 0;
	monitor = 0;
}

void Monitor_Unique_ID::serialize(Block_Buffer & w) const {
	w.write_int64(uuid);
	w.write_int32(type);
	w.write_int32(id);
	w.write_int32(order);
	w.write_string(ip);
	w.write_int32(port);
}

int Monitor_Unique_ID::deserialize(Block_Buffer & r) {
	int32_t type_ = 0;
	if( r.read_int64(uuid)  ||  r.read_int32(type_)  ||  r.read_int32(id)  || r.read_int32(order)  || r.read_string(ip)  || r.read_int32(port)  )
			return -1;
	type = (Server_Enum::MONITOR_TYPE)type_;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

void Player_Monitor_Link::reset(void) {
	gate_muid.reset();
	logic_muid.reset();
	scene_muid.reset();
	local_chat_muid.reset();
	inter_chat_muid.reset();
}

void Player_Monitor_Link::serialize(Block_Buffer & w) const {
	gate_muid.serialize(w);
	logic_muid.serialize(w);
	scene_muid.serialize(w);
	local_chat_muid.serialize(w);
	inter_chat_muid.serialize(w);
}

int Player_Monitor_Link::deserialize(Block_Buffer & r) {
	if( gate_muid.deserialize(r)  ||  logic_muid.deserialize(r)  ||  scene_muid.deserialize(r)  || local_chat_muid.deserialize(r)  || inter_chat_muid.deserialize(r)  )
			return -1;
	return 0;
}

void Scene_Unique_ID::reset(void) {
	scene_id = 0;
	scene_order = 0;
}

//////////////////////////////////////////////////////////////////////////////////////

void Message_Stat::reset(void) {
	msg_id = 0;
	times = 0;
	avg = 0.0;
	tv = Time_Value::zero;
	min_tv = Time_Value(100);
	max_tv = Time_Value::zero;
}

void Message_Stat::count(Time_Value &t) {;
	tv += t;
	times++;
	if (t > max_tv) max_tv = t;

	if (t < min_tv) min_tv = t;
}
