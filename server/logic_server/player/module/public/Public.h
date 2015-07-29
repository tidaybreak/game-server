/*
 * Public.h
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 *
 *      杂项/公共
 */

#ifndef PUBLIC_H_
#define PUBLIC_H_

#include "Logic_Player_Base.h"
#include "Public_Def.h"
#include "fighter/Fighter_Def.h"


#define WAR_SCENE_LEVEL_LIMIT 30

class Packer;

struct Public_Detail;
struct Drops_Item;
struct MSG_10120004;
struct Base_Detail;
struct Flowers_Config;
struct String_Info_Change;
struct MSG_81000067;
struct MSG_10110008;
struct Invite_Reward_Config;
struct MSG_10120110;
struct MSG_20300116;

class Public : virtual public Packer {
public:
	typedef boost::unordered_map<int, int> Power_Map;
	Public(void);
	virtual ~Public();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Public_Detail &detail, const Base_Detail &base_detail);
	// 登录时detail赋值
	void load_detail(Public_Detail &detail);
	// 保存时detail赋值
	void save_detail(Public_Detail &data);

	Public_Detail const &public_detail(void) const;

	// 登录时module初始化
	void module_init(void);
	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	//托管中下线调用
	void trusteeship_sign_out(void);

	// 顶号调用
	int sign_in_repeat(void);

	// 每天凌晨0点触发
	int trigger_daily_zero(const Time_Value &now, bool logic_check = false);
	// 每天凌晨6点触发
	int trigger_daily_six(const Time_Value& now, bool logic_check = false);

	int trigger_weekly_zero(const Time_Value &now, bool logic_check = false);

	void equip_tick(void);


	/////////////////////////////////////////////////////////////////////////////////

	int active_string_info_change(std::vector<String_Info_Change> &change_info);
	int active_int_info_change(const MSG_81000067& active_msg);

	/// 防沉迷
	void fcm_event_1h(void);
	void fcm_event_2h(void);
	void fcm_event_255h(void);
	void fcm_event_3h(void);
	int req_fcm_pass(int fcm);

	/// 掉落
	int get_drops_goods(const int scene_type, const int scene_id, role_id_t drops_role_id, std::vector<Drops_Item>& drops_item_vec, int item_index);
	int add_drops_to_package(const int scene_type, const int scene_id, std::vector<Drops_Item>& drops_item_vec);
	int gather_goods(role_id_t material_role_id, std::vector<Drops_Item>& drops_item_vec);
	int add_goods(const int scene_type, const int scene_id, std::vector<Drops_Item>& drops_item_vec);

	/// 任务
	int task_handle_plot_info(const MSG_10110008 &msg);
	int task_clean_plot_info(String_Vec plot_ids);

	/// 开关
	int set_power_switch(const Power_Type module, const int32_t switch_value);
	int get_power_by_power_type(const Power_Type module);

	// 剧情ID
	int set_plot_info(const std::string plot_id, int plot_step = 0);
	int clean_plot_info(void);
	int req_plot_all_add_one(const MSG_10120110 &msg);

	//新手引导
	void guide_init();
	int req_guide(uint16_t guide_id);
	void close_guide(uint16_t guide_id);
	void close_guide_by_type(Guide_Finish_Enum guide_finish_enum, uint64_t param1 = 0, uint64_t param2 = 0);
	int req_guide_add_item_in_plot(uint32_t item_id, uint32_t item_num);
	int req_guide_set(uint16_t guide_id, uint8_t step);
	int req_save_played_plot(uint32_t plot_id);
	void notify_guide_setp_list();
	int req_task_guide(const uint32_t guide_id);

	//请求获取角色信息
	int req_role_info(role_id_t role_id);
	int req_open_role_panel_logic(role_id_t role_id, int8_t open_type);


	// 玩家改名
	int req_rename(const std::string& new_name);

	//体力
	int req_buy_phy_power(int bind_gold_first);
	int fetch_phy_power_info(void);
	// 恢复体力
	int restor_phy_power(const int value);

	//请求小喇叭聊天
	int req_chat_bugle(const std::vector<Chat_Content> &content);

	// 请求进入战场
	int req_war_scene_enter(bool check_open = true);

	int req_war_scene_reset_enter_num(void);

	int war_scene_enter(role_id_t war_scene_id, int side, Coord& coord);

	int req_war_scene_enter_num(void);
	int req_war_scene_ex_result(void);
	int req_war_scene_receive(void);
	void inner_war_scene_result(const int result, const int rank, const int kill_num, const int coe);
	// 请求每月英雄信息
	int req_fetch_month_hero_info(void);

	// 添加公会战荣誉值
	void add_gang_merit_contribute(int add_merit, int add_contribute);
	// 添加战场荣誉值和碎片
	void add_war_merit_piece(int add_merit, int add_piece, int arena_coin);
	// 同步战场惩罚时间
	void sync_war_forbid_time(const Time_Value time);
	// 战场活动图片状态
	void send_war_scene_active_to_client(void);
	// 篝火晚会活动图片状态
	void send_bonfire_active_to_client(void);
	//
	void send_act_scene_to_client(const int act_icon_id);
	//
	void send_gang_boss_active_to_client(const int state, const int remain_time);
	// 侦听等级变化
	void listen_fighter_level(const int new_level, const int src_level);

	// 请求竞技场面板信息
	int req_arena_panel_info(void);

    //请求获取系统设置
	int req_get_sys_cfg();

	//请求更改系统设置
	int req_set_sys_cfg(const MSG_10120004 &msg);

	//int addition_prop_refresh(Addition_Type addi_type, Prop_Setter_Type pst, std::map<int, double> &pro_val_map);

	// 玩家觉醒
	int req_fetch_awake_lvl(void);
	int req_awake_lvl_upgrade(bool from_hero_msg = false);
	//int awake_lvl(void) const;
	static int player_hero_id(const int career);

	// 玩家神器
	int req_fetch_player_artifact_info(void);
	int req_player_artifact_upgrade(const int artifact_id, bool from_hero_msg = false);
	int get_player_artifacts_pro_val(std::map<int, double>& artifacts_pro_val);
	void player_artifacts_prop_refresh(void);

	// 预告系统
	int req_forenotice_receive_gift(const int32_t gift_lvl);
	int req_forenotice_received_gifts(void);
	void listen_task_status_change(uint32_t task_id, uint8_t task_status);
	int req_set_forenotice_played_effect(uint32_t id);

	// 功勋
	int req_exploit_info(void);
	int refresh_exploit_lvl(void);
	int change_exploit_val(int change_val);
	int active_exploit_info(Logic_Player* target_player);
	int title_nobility_announce(const int exploit_lvl);

	//送花系统配置
	int req_handsel_flowers(uint32_t item_id, std::string &role_name, std::string &msg);
	void offline_add_flowers(std::vector<Flowers_Info> &flowers_info_vec);
	int req_look_flowers(void);
	void self_receive_flowers(role_id_t role_id, std::string &role_name, const Flowers_Config &conf, std::string &msg);

	// 神秘商店
	// 请求神秘商店信息
	int req_fetch_secret_shop_info(void);
	// 请求神秘商店购买物品
	int req_secret_shop_buy_item(int index);

	// 篝火晚会
	int req_campfire_enter(void);
	// world boss enter
	int req_act_enter_scene(int active_id);
	// 请求世界boss击杀信息
	int req_world_boss_kill_info(const int8_t icon_id);
	// 请求设置世界boss开启时间
	int req_world_boss_set_time(const int8_t icon_id, const int8_t hour, const int8_t minute);
	//
	int cmd_add_firewood(int add_campfire = 1);
	//
	int activity_result(int act_id, int type, int result, int value1, int value2, int value3);
	int save_campfire_player_info(const MSG_20300116& msg);

	void public_plot_un_broadcast(const bool ub); // 转场景后，不播剧情

	void send_equip_look(void);
	void hook_equip_look_level_up(const int new_level);
	//你提我改
	int send_proposal(uint8_t type, std::string &contents);

	//首充-充值反馈
	void recharge_listen(uint32_t num);
	int req_open_first_recharge(void);
	int req_receive_first_recharge(void);
	void first_over_receive_to_mail(void);
	int req_open_two_recharge(void);
	int req_open_limit_time_recharge(void);
	int req_receive_two_recharge(void);
	int req_receive_limit_time_recharge(void);
	void reset_two_recharge(void);//判断是否重置全民充值
	void reset_limit_time_recharge(void);//判断是否重置限时充值
	int req_open_recharge_rebate(void);
	int req_receive_recharge_rebate(uint32_t id);
	void reset_recharge_rebate_data(void);
	int req_recharge_rebate_status(void);

	//擂台
	int req_send_challenge_to_gang_chat(int64_t role_id, std::string role_name);
	// world_level
	double get_world_level_add_percent(void);
	int get_world_level_add(void);

	//问卷
	int req_questionnaire_info(void);
	int req_submit_questionnaire(uint8_t qid, std::vector<Questionnaire_Answer_Info> &ans);

	//邀请好友奖励
	int req_open_invite_reward_info(void);
	int req_receive_invite_reward_info(uint8_t type);
	int receive_invite_reward(const Invite_Reward_Config &conf);
	void set_inv_time(const Time_Value &time);
	void get_like_reward(void);

	void public_listen_level_up(int32_t level);

	//体验过期提示
	void player_try_time_out(uint32_t type, const Time_Value &over_time = Time_Value::zero);
	void notify_try_time_out_list();
	int req_player_readed_timeout_try_message(const uint8_t type);

	//增加今日获得粉钻数
	void push_add_today_get_bind_gold(uint32_t num);
	void send_today_get_bind_gold_active(void);

	int friendship_accumulate(void);
	int add_friendship_accumulate(const int friendship);

	int get_vip_upgrade_card(void) const;
	void set_vip_upgrade_card(int times);

	int get_arena_worship(void);
	void set_arena_worship(const int value);
protected:

	Public_Detail *public_detail_;

	Power_Map pwoer_switch_;

	bool un_broadcast_; // 重登转场景，不发剧情，并次数+1

private:
	void init_artifacts(void);
	static void init_artifacts(Public_Detail &detail, const Base_Detail &base_detail);
	// 获取战场收益情况
	void get_war_income_info(int& max_income_num, int& remain_income_num, int& income_rate);

	//通知充值返利有领取
	void active_rechare_rebate_status(void);
};

inline Public_Detail const &Public::public_detail(void) const{
	return *public_detail_;
}

#endif /* PUBLIC_H_ */
