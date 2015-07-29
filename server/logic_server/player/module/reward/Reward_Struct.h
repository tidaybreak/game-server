/*
 * Reward_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef REWARD_STRUCT_H_
#define REWARD_STRUCT_H_

#include "Server_Struct.h"

//struct Activation_Code_Info {
//	Activation_Code_Info(void) { reset(); }
//	void reset(void) {
//		type = 0;
//		code.clear();
//		status = 0;
//	}
//	void serialize(Block_Buffer & w) const {
//		w.write_uint32(type);
//		w.write_string(code);
//		w.write_uint32(status);
//	}
//
//	int deserialize(Block_Buffer & r) {
//		r.read_uint32(type);
//		r.read_string(code);
//		r.read_uint32(status);
//		return 0;
//	}
//	uint32_t type;
//	std::string code;
//	uint32_t status;
//};

struct Activation_Code_Info {
	Activation_Code_Info(void) { reset(); }
	void reset(void) {
		type = 0;
		id = 0;
		rand = 0;
	}

	uint32_t type;
	uint32_t id;
	uint32_t rand;
};

typedef boost::unordered_map<long long int, UInt_UInt_Map> Timesec_UInt_UInt_Map;
typedef boost::unordered_map<uint32_t, UInt_UInt_Map> UInt_UInt_UInt_Map;
typedef boost::unordered_map<uint32_t, Time_Value> UInt_Time_Map;

struct Reward_Detail: public Detail {
	Reward_Detail(void);
	virtual ~Reward_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	uint32_t online_reward_num;
	Time_Value online_reward_time;
	Time_Value last_retime;//最后一次存库时间;离线时间;最后一次的时间;更精准的用于记录在线奖励
	uint8_t online_reward_over;
	Time_Value last_online_refresh_time;

//	uint32_t login_seven_num; //七日奖励领取天数
//	uint8_t login_seven_is_receive;//七日奖励是否已经领取
	uint32_t login_days; //登陆的天数
	UInt_Set receive_seven_days;//已经领取的天数

	UInt_UInt_Map activation_code_receive_info; //激活码领取信息

	Timesec_UInt_UInt_Map reward_back_all_num;//累计每天奖励找回项目对应次数
	UInt_UInt_UInt_Map reward_back_item_value;//奖励找回项目类型 奖励值
	UInt_Time_Map reward_back_receive_time; //最近一次奖励找回领取时间
	UInt_Time_Map reward_back_first_time; //项目第一次完成时间
	UInt_UInt_Map reward_back_item_record;//项目当次唯一ID,项目过程中获取到的奖励,分批分次发送调用接口
	UInt_UInt_UInt_Map reward_back_item_value_tmp;//奖励找回项目类型 奖励值,分批分次发送调用接口,刷历史最高来获取奖励找回
	uint8_t keep_reward_status;

};

#endif /* REWARD_STRUCT_H_ */
