/*
 * Gate_Player_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef GATE_PLAYER_STRUCT_H_
#define GATE_PLAYER_STRUCT_H_

#include "Game_Typedef.h"

struct Msg_Process_Info {
	int32_t msg;/**/
	int32_t msg_time;/**/
	int32_t server_time;/**/

	Msg_Process_Info(int32_t msg_, int32_t msg_time_, int32_t server_time_) {
		msg = msg_;
		msg_time = msg_time_;
		server_time = server_time_;
	};

	void reset(void) {
		msg = 0;
		msg_time = 0;
		server_time = 0;
	}
};
typedef std::list<Msg_Process_Info> Msg_Process_Info_Vec;

struct MSG_Detail {
	bool is_inited;
	int cid;
	unsigned int hash_key;				/// 用于加解密的hash key
	uint32_t msg_serial;				/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;		/// 操作频率统计
	Time_Value msg_interval_timestamp;
	Msg_Process_Info_Vec msg_vec;

	void reset(void) {
		is_inited = false;
		cid = -1;
		hash_key = 0;
		msg_serial = 0;
		msg_timestamp = Time_Value::zero;
		msg_interval_count_ = 0;
		msg_interval_timestamp = Time_Value::zero;
		msg_vec.clear();
	}
};

#endif /* GATE_PLAYER_STRUCT_H_ */
