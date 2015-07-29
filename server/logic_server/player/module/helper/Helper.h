/*
 * Helper.h
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 *
 *      日常模块
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "Logic_Player_Base.h"

struct Helper_Detail;

class Helper : virtual public Logic_Player_Base {
public:
	Helper(void);
	virtual ~Helper();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Helper_Detail &detail);

	// 登录时detail赋值
	void load_detail(Helper_Detail &detail);

	// 保存时detail赋值
	void save_detail(Helper_Detail &data);

	Helper_Detail const &daily_detail(void) const;

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

	// 每日6点刷新
	int trigger_daily_six(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	void daily_helper_listen(uint32_t type, uint32_t type_id = 0);

	int req_daily(void);
	int req_activity(void);
	int req_activity_receive(uint8_t type, uint32_t id);
	int req_today_open_active(void);

	int req_upgrade_score();
	int req_upgrade_get_status(UInt_Vec get_type, std::string &tip, int32_t item_id, uint8_t isAuto);

	int req_helper_check_in(void);

	void helper_listen_fighter_force(int new_force);
	void helper_listen_join_arena_force(const int last_join_arena_force);
	void helper_listen_world_chat(void); //监听世界聊天

	void calculate_two_properties(const std::map<int, double> &pro_val_map,  std::map<int, double> &out_pro_val_map);
	//监听等级改变
	//void reward_back_listen_level_up(const int level);

	void helper_listen_icon_open(const uint16_t icon_id);

	void test_set_today_is_check_in(void); //todo 测试命令使用;要删除

private:
	void listen_daily(uint32_t type, uint32_t type_id = 0);
	void listen_activity(uint32_t type, uint32_t type_id = 0);

	void active_to_client(uint32_t type, uint32_t id, uint32_t num);

	int helper_check_in(void);
	void send_check_in_msg(void);

	int get_fashion_strengthen_level(void);//当前穿着的时装强化级别
	int get_fashion_collect_num(void);//当前获得时装件数

private:
	Helper_Detail *helper_detail_;
	Time_Value last_listen_force_time;
};

inline Helper_Detail const &Helper::daily_detail(void) const{
	return *helper_detail_;
}

#endif /* HELPER_H_ */
