/*
 * Dragon_Vale.h
 *
 *  Created on: 2014年7月11日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_H_
#define DRAGON_VALE_H_

#include <map>
#include "Logic_Player_Base.h"
#include "boost/unordered_map.hpp"
#include "Dragon_Vale_Def.h"

struct Dragon_Vale_Detail;
struct Base_Detail;
struct Dragon_Evolution_Cfg;

class Dragon_Vale : virtual public Logic_Player_Base {
public:
	Dragon_Vale(void);
	virtual ~Dragon_Vale();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Dragon_Vale_Detail &dragonValeDetail, const Base_Detail &baseDetail);

	// 登录时detail赋值
	void load_detail(Dragon_Vale_Detail &detail);

	// 保存时detail赋值
	void save_detail(Dragon_Vale_Detail &detail);

	inline const Dragon_Vale_Detail& dragon_vale_detail(void) const;
	inline Dragon_Vale_Detail& dragon_vale_detail_no_const();

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

	//每天凌晨6点触发
	int trigger_daily_six(const Time_Value &now);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	//龙谷信息改变，同步龙谷信息到PlayerDBView
	void sync_dragon_vale_to_db_view();
	void sync_dragon_vale_create_args_to_db_view();

	//龙谷信息改变,保存数据库
	void save_dragon_vale_tick();

	//进自己龙谷
	void enter_self_dragon_vale();

	//进别人龙谷
	void enter_other_dragon_vale(role_id_t role_id);
	void enter_other_offline_player_dragon_vale(role_id_t role_id);

	// 等级改变监听
	void dragon_vale_listen_level_up(int new_level);

	//请求建造新的建筑
	int req_add_new_building(uint32_t building_id, uint8_t building_type, uint8_t consume_type);

	//请求升级建筑
	int req_upgrade_building(uint32_t building_id, uint8_t building_type);

	//在龙谷内消耗货币
	int consume_money_in_dragon_vale(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type);

	//在龙谷内获得货币
	int gain_money_in_dragon_vale(uint8_t money_type, uint32_t money_num, uint32_t gain_money_type, bool is_record = true);

	//请求加速建造或者升级建筑
	int req_quick_finish_building(uint32_t building_id, uint8_t building_type);

	//请求取消建造或者升级建筑
	int req_cancel_finish_building(uint32_t building_id, uint8_t building_type);

	//请求收获资源
	int req_gain_production(uint32_t building_id, uint8_t building_type);

	//根据建筑类型获取收获资源类型   注意：返回的值是Pack::Money_Sub_Type类型
	uint8_t get_production_type(uint8_t building_type);

	//1分钟监听一次建筑资源产出
	void listen_building_output_production_by_one_minute(const Time_Value &now);

	//监听龙谷建筑建造或者升级完成
	void listen_building_finish(const Time_Value &now);

	//请求获取有龙谷的好友列表
	void req_get_dragon_vale_friend_list();

	//请求获取有龙谷的同一个公会成员列表
	void req_get_dranon_vale_gang_member_list();

	//获取各种建筑类型所有建筑的最大资源存储上限及当前产出量
	void get_building_max_storage(uint32_t &gold_mine_max_storage, uint32_t &moon_weel_max_storage,
			uint32_t &totem_max_storage, uint32_t &gold_mine_production, uint32_t &moon_weel_production,
			uint32_t &totem_production, bool is_rob = false);

	//请求孵化龙蛋
	int req_dragon_born();

	//监听龙蛋孵化完成
	void listen_dragon_born_finish(const Time_Value &now);

	//请求逗龙
	int req_play_dragon(role_id_t role_id);

	//监听逗龙冷却时间
	void listen_play_dragon_interval_time(const Time_Value &now);

	//监听龙的心情值每隔多长时间下降1点
	void listen_dragon_feeling_down_time(const Time_Value &now);

	//请求获取龙的总信息
	int req_get_dragon_total_info(role_id_t role_id);

	//请求培养龙  type:培养类型
	int req_keep_dragon(uint8_t type);

	//请求增加1点龙心情值
	int req_add_dragon_feeling();

	//请求进化龙
	int req_evolute_dragon(uint8_t type);

	//从龙谷获得主角战斗属性加成
	void get_add_fight_property(std::map<int, double> &add_property_map);

	//龙谷监听vip等级变化，自动开启/关闭vip等级相关的建筑队列
	void dragon_vale_listen_vip_level_chance(uint32_t new_vip_level);

	//主角斗力属性加成
	uint32_t get_add_power_from_dragon_vale();

	//主角护甲属性加成
	uint32_t get_add_armor_from_dragon_vale();

	//主角耐力属性加成
	uint32_t get_add_stamina_from_dragon_vale();

	//英雄斗力属性加成
	uint32_t get_add_hero_power_from_dragon_vale();

	//英雄护甲属性加成
	uint32_t get_add_hero_armor_from_dragon_vale();

	//英雄耐力属性加成
	uint32_t get_add_hero_stamina_from_dragon_vale();

	uint32_t get_add_hero_crit_from_dragon_vale();
	uint32_t get_add_hero_prevent_from_dragon_vale();

	int req_rob_other_dragon_vale();
	int req_rob_other_dragon_vale_match();
	int req_gain_rob_dragon_vale_production(int64_t to_uuid, int64_t to_role_id,
										    int64_t uuid, int64_t role_id,
										    uint32_t building_id, uint8_t building_type);
	void listen_refresh_dragon_vale_monster(const Time_Value &now);
	void listen_dragon_rob_protect_time(const Time_Value &now);
	void sync_special_to_db_view();
	void sync_dragon_vale_reset_to_db_view();
	bool is_dragon_vale_special();
	void hook_dragon_vale_special_change();
	int req_step_foot(int64_t role_id);
	int req_get_dragon_vale_notice_board();
	int rob_dragon_vale_task_listen();
	int get_dragon_vale_building_type_level_map(boost::unordered_multimap<uint8_t, uint16_t> &building_map);
	void dragon_born_right_now_test();
	int req_batch_evolution_dragon(const uint8_t type, const uint16_t batch_num, const uint16_t ten_times);
	int req_batch_evolution_dragon_type_0(const Dragon_Evolution_Cfg &dragon_evolution_cfg);
	void dragon_vale_sync_player_level_change_to_db_view(int new_level);
	void process_robber_win_in_rob_fight();
	int req_dragon_building_gain_production_left_time(uint32_t id, uint8_t type);
	void process_robber_gain_total_production(uint32_t gold_mine, uint32_t moon_well, uint32_t totem);
	void process_robber_begin_attck();

	void dragon_vale_listen_icon_open(const uint16_t icon_id);
	int req_get_invite_friend_step_foot_list();
	int req_invite_friend_step_foot(int64_t role_id);
	int req_invite_all_friend_step_foot();

	// inter
	int listen_lvl_when_module_init(void);

private:
	//逗自己的龙
	int play_self_dragon();

	//逗别人的龙
	int play_other_dragon(role_id_t role_id);

	//获取自己的龙总信息
	int get_self_dragon_total_info();

	//获取别人的龙的总信息
	int get_other_dragon_total_info(role_id_t role_id);

	//判断货币是否足够，不足返回对应的不足的错误码
	int check_money_is_engough(uint8_t money_type, uint32_t money_num, uint32_t lose_money_type);

	//处理龙的进化
	int process_evolution_dragon(const Dragon_Evolution_Cfg &dragon_evolution_cfg);

	//判断当前是否有正在建造或者升级中的建筑
	bool is_has_building_upgrading();

	void process_login_building_output_production_by_one_minute();
	void process_login_building_finish();
	void process_login_dragon_born_finish();
	void process_login_play_dragon_interval_time();
	void process_login_dragon_feeling_down_time();
	void process_login_dragon_rob_protect_time();
	void process_login_refresh_dragon_vale_monster();
	int process_step_offline_player_foot(int64_t role_id);
	int process_batch_evolution_dragon(const Dragon_Evolution_Cfg &dragon_evolution_cfg,
					const uint16_t batch_num, const uint16_t ten_times);
	uint16_t play_dragon_get_heart_num_by_chance();
	uint16_t get_dragon_building_total_num();
	uint16_t get_dragon_building_num_by_type(uint8_t type);
	void notify_has_can_create_dragon_building_num();
	void refresh_dragon_vale_op_by_step_foot(const int64_t target_id);
	void correct_building_queue();
	void correct_dragon_evolution_20150129();

private:
	Dragon_Vale_Detail *dragon_vale_detail_;
	Building_Notice_Map building_notice_map;
};


const Dragon_Vale_Detail& Dragon_Vale::dragon_vale_detail(void) const {
	return *dragon_vale_detail_;
}

Dragon_Vale_Detail& Dragon_Vale::dragon_vale_detail_no_const() {
	return *dragon_vale_detail_;
}

#endif /* DRAGON_VALE_H_ */
