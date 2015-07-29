/*
 * Knight_Trials.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      新模块例子
 */

#ifndef KNIGHT_TRIALS_H_
#define KNIGHT_TRIALS_H_

#include "Logic_Player_Base.h"

struct Knight_Trials_Detail;
struct Knight_Trials_Player_Detail;
struct Knight_Trials_Player;
struct Uint_Uint;
struct Knight_Trials_Role_Info;

class Knight_Trials : virtual public Logic_Player_Base {
public:
	Knight_Trials(void);
	virtual ~Knight_Trials();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Knight_Trials_Detail &detail);

	// 登录时detail赋值
	void load_detail(Knight_Trials_Detail &detail);
	void load_player_detail(Knight_Trials_Player_Detail &detail);

	// 保存时detail赋值
	void save_detail(Knight_Trials_Detail &data);
	void save_player_detail(Knight_Trials_Player_Detail &data);

	Knight_Trials_Detail const &knight_trials_detail(void) const;
	Knight_Trials_Player_Detail const &knight_trials_player_detail(void) const;

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

	//托管中下线调用
	void trusteeship_sign_out(void);

	//本地匹配往战斗玩家数据插入数据
	void insert_knight_trials_player_fighter_info_vec(Knight_Trials_Player &player);
	//跨服返回的匹配往战斗玩家数据插入数据;这个和本地不一样;本地是插入排序插入,跨服返回的需要替换临近的玩家战力的那个人
	void insert_knight_trials_player_fighter_info_vec_callback(Knight_Trials_Player &player);

	int local_callback_knight_trals_enter_fighter(Knight_Trials_Player &kplayer);

	//客户端请求
	int req_open_knight_trials_plan(void);
	int req_knight_trials_enter_fighter(void);
	int req_up_hide_knight_trials_palyer(void);
	int req_open_knight_trials_reward_box(void);
	int req_receive_knight_trials_reward_box(void);

	//监听
	void listen_knight_trials_win_or_lost(uint8_t battle_result);//监听战斗胜利

	void knight_trials_listen_icon_open(const uint16_t icon_id);

	//临时交换数据标识字段设置接口 - 数据库
//	void set_to_uuid(int64_t uuid);
//	void set_to_role_id(role_id_t role_id);

private:

	//奖励宝箱相关的
	void refres_box_reward_on_rand(uint8_t points);
	void update_add_up_reward(Uint_Uint &uu);

	//匹配相关的
	void refres_knight_trials_on_local(void);
	void refres_knight_trials_player_on_local(bool clear = false);

	void knight_trials_player_to_role_info(Knight_Trials_Player &player, Knight_Trials_Role_Info &value);

	Knight_Trials_Detail *knight_trials_detail_;
	Knight_Trials_Player_Detail *knight_trials_player_detail_;
	int sign_force;
};

inline Knight_Trials_Detail const &Knight_Trials::knight_trials_detail(void) const{
	return *knight_trials_detail_;
}

inline Knight_Trials_Player_Detail const &Knight_Trials::knight_trials_player_detail(void) const{
	return *knight_trials_player_detail_;
}

#endif /* KNIGHT_TRIALS_H_ */
