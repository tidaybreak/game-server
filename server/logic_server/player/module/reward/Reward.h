/*
 * Reward.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      新模块例子
 */

#ifndef REWARD_H_
#define REWARD_H_

#include "Logic_Player_Base.h"

struct Reward_Detail;
struct Activation_Code_Info;
struct Reward_Back_Config;
struct Reward_Back_Info;

class Reward : virtual public Logic_Player_Base {
public:
	Reward(void);
	virtual ~Reward();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Reward_Detail &detail);

	// 登录时detail赋值
	void load_detail(Reward_Detail &detail);

	// 保存时detail赋值
	void save_detail(Reward_Detail &data);

	Reward_Detail const &reward_detail(void) const;

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);

	// 登录网关后后调用(只会的正常登录调用一次,顶号不会调)
	void sign_in(void);

	// 顶号时调用
	int sign_in_repeat(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 登出前调用
	void sign_out(void);

	// 场景加载完成
	int move_scene_completed(void);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	// 每天凌晨6点触发
	int trigger_daily_six(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	//客户端请求
	int req_reward_tip_num(void);
	int req_receive_online_reward(void);
	int req_reward_login_seven_info(void);
	int req_receive_login_seven(uint32_t days);
	int req_receive_activation_code_reward(std::string &code);
	int req_reward_back_receive(uint32_t type, uint32_t item_id);
	int req_reward_back_info(void);
	int req_have_receive_keep_reward(void);
	int req_keep_reward_success(void);
	int req_receive_keep_reward(void);

	//激活码处理数据库回调信息
//	int process_activation_code_data(Activation_Code_Info &code_info);

	//监听接口
	int reward_back_listen_fulfill(uint32_t item_id);
	int reward_back_listen_fulfill(uint32_t item_id, UInt_UInt_Map &reward_item);
	int reward_back_listen_accumulated_rewards(uint32_t id, const Int_Int_Map &reward_item,const uint32_t uniqid);


	//监听等级改变
	void reward_back_listen_level_up(const int level);

	//todo 测试指令
	void test_set_seven_is_receive(void);
	void test_all_reward_back_sub_day(void);

private:
	Reward_Detail *reward_detail_;
	bool is_update_online_reward;

	void send_online_reward_msg(void);
	void send_reward_back_msg(void);
	int receive_online_reward(void);
	int receive_login_seven(uint32_t days);

	void update_online_reward(void);

	uint32_t get_reward_back_fulfill_num_by_id(uint32_t id, uint32_t pid = 0);
	uint32_t get_reward_back_can_reveice_days(uint32_t id, uint32_t pid = 0);

	void get_reward_back_info_by_conf(const Reward_Back_Config& conf, Reward_Back_Info &rbi);

	void update_reward_back_reveice_time(uint32_t id);

	void get_six_time(const Time_Value &tv1, Time_Value &tv2);
	int get_six_time_subdays(const Time_Value &tv1, const Time_Value &tv2) ;
};

inline Reward_Detail const &Reward::reward_detail(void) const{
	return *reward_detail_;
}

inline void Reward::get_six_time(const Time_Value &tv1, Time_Value &tv2) {
	Date_Time date_(tv1);
	long sub_sec = 0;
	if(date_.hour() < 6) {
		sub_sec = Time_Value::ONE_DAY_IN_SECS;
	}
	date_.hour(6);
	date_.minute(0);
	date_.second(0);
	tv2.sec(date_.time_sec() - sub_sec);
}

/// tv2和tv1相隔天数
inline int Reward::get_six_time_subdays(const Time_Value &tv1, const Time_Value &tv2) {
	Time_Value zero_time1;
	get_six_time(tv1, zero_time1);
	Time_Value zero_time2;
	get_six_time(tv2, zero_time2);
	return (zero_time2.sec() - zero_time1.sec()) / Time_Value::ONE_DAY_IN_SECS;
}
#endif /* REWARD_H_ */
