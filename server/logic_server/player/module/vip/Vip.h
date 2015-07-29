/*
 * Vip.h
 *
 *  Created on: 2014-07-21
 *      Author: xiaoliang
 *
 *      新模块例子
 */

#ifndef VIP_H_
#define VIP_H_

#include "Logic_Player_Base.h"

struct Vip_Detail;
struct Turntable_Config;

class Vip : virtual public Logic_Player_Base {
public:
	Vip(void);
	virtual ~Vip();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Vip_Detail &detail);

	// 登录时detail赋值
	void load_detail(Vip_Detail &detail);

	// 保存时detail赋值
	void save_detail(Vip_Detail &data);

	Vip_Detail const &vip_detail(void) const;

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

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	// 每天凌晨6点触发
	int trigger_daily_six(const Time_Value &now);
	int test_trigger_daily_six(const Time_Value &now); //todo delete
	void test_set_last_settle_grow(uint32_t days); //todo delete

	//托管中下线调用
	void trusteeship_sign_out(void);

	//定时监听是否已经过期
	void listen_vip_expired(const Time_Value &now);

	//开通VIP接口
	void open_vip(uint32_t days, uint32_t init_level = 0);
	void open_vip_hours(uint32_t hours, uint32_t init_level = 0);

	//设置时间接口
	void set_vip_exp_overtime(const Time_Value &time);

	//尝试升级VIP
	void try_vip_level_up(void);

	//求出VIP剩余天数
	uint32_t get_vip_days(void);

	//客户端请求
	int req_open_vip(uint32_t id);//请求开通VIP
	int req_vip_world_tree_info(void);//打开世界之树面板;获取信息
	int req_vip_receive_world_tree(uint32_t id);//请求领取世界之树奖励
	int req_vip_turntable_open();//请求打开VIP转盘获取物品组
	int req_vip_turntable_lottery(uint8_t type);//请求VIP转盘抽奖
	int req_vip_turntable_receive();//请求获取VIP转盘奖品
	int req_vip_daily_receive();//请求获取VIP每日登陆可以领取VIP日礼包
	int req_open_vip_daily_receive();//请求openVIP每日登陆可以领取VIP日礼包

	//VIP系统监听消费累加接口
	void listen_vip_cost_money(const int32_t sub_num, const uint16_t type);
	//VIP系统监听专属加成（世界之树）
	void listen_vip_reward(const uint32_t type, const uint32_t money_type, const uint32_t num);

	//VIP等级改变接口
	void vip_level_change(uint32_t new_vip_level, uint32_t old_vip_level);
	int get_vip_addition(uint32_t type);
	void vip_expired_listen(const Time_Value &over_time, const Time_Value &now);

	void vip_listen_level_up(const int level);

	void send_vip_info_client(uint8_t type); //向客户端发送主动广播VIP最新信息

private:
	Vip_Detail *vip_detail_;

	void sync_vip_info_to_chat(void);

	//获取世界之树是否有奖励可以领取
	int8_t get_vip_world_tree_have_receive(void);
	int make_vip_turntable_data(Turntable_Config &data, std::vector<uint32_t> &id_list);
	void get_grow(const Time_Value &start_time, const Time_Value &end_time);
	void lost_grow(const Time_Value &start_time, const Time_Value &end_time);
	void get_cost_grow(const Time_Value &now);
	void send_open_vip_mail(uint32_t days=0);
	void send_contiune_vip_mail(void);
	void send_expired_vip_mail(void);

	int vip_turntable_lottery(int32_t &id, uint8_t type);//VIP转盘抽奖

	void send_world_tree_icon_status(int8_t open);
};

inline Vip_Detail const &Vip::vip_detail(void) const{
	return *vip_detail_;
}

#endif /* VIP_H_ */
