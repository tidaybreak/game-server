/*
 * Ganger.h
 *
 *  Created on: Apr 24, 2014
 *      Author: caijianwei
 */

#ifndef GANGER_H_
#define GANGER_H_

#include "Logic_Player_Base.h"

struct Ganger_Detail;
struct Mail_Info;
struct Ganger_Info;
struct Gang_DB_View;
struct Reward_Info_Gang_War;

class Ganger : virtual public Logic_Player_Base {
public:
	static const int GANG_WAR_SCENE_ID = 80213001;
	Ganger();
	virtual ~Ganger();
	void reset();
	// 创建角色时初始化
	static int create_init(Ganger_Detail &detail);
	// 登录时detail赋值
	void load_detail(Ganger_Detail &detail);
	// 保存时detail赋值
	void save_detail(Ganger_Detail &data);

	Ganger_Detail const &ganger_detail(void) const;
	Ganger_Detail& ganger_detail_no_const();

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);
	int trigger_daily_six(const Time_Value &now, bool logic_check);
	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);
	// 登录网关后后调用,通知客户端模块信息
	void sign_in(void);
	void sign_in_repeat(void);
	// 登出前调用
	void sign_out(void);
	int move_scene_completed(void);

	void approve_join_gang(Gang_DB_View* gang_view);
	void promote_to_leader();
	int promote_to_headship(const int headship, bool sync = true);
	void kicked_out_gang(gang_id_t quit_gang_id);
	void handle_offline_kick();
	void offline_operate_init(const Ganger_Detail& detail);
	void update_offline_ganger_detail(const Ganger_Detail& detail);

	// msg start
	int req_create_gang(int8_t logo, std::string const& name, const int8_t money_type);
	int req_join_gang(int64_t gang_id);
	int req_gang_info(int64_t gang_id);
	int req_ganger_info(role_id_t role_id);
	int req_gang_list();
	int req_ganger_list( int64_t gang_id);
	int req_quit_gang();
	int req_gang_info_listen(int8_t operate);
	int req_gang_leader_resign(role_id_t target);
	int req_kick_out_gang(role_id_t target);
	int req_set_gang_announce(const std::string& announce);
	int req_invite_to_join(Role_Vec targets);
	int req_gang_passed_events();
	int req_gang_simple_passed_events();
	int req_gang_group_mail(Mail_Info& mail_info);
	int req_gang_donate(int8_t type, int money);
	int req_set_join_block(int8_t set);

	int req_gang_invite_list(void);
	int req_gang_apply_list(void);

	int req_gang_list_action(int64_t gang_id, int8_t action);
	int req_gang_invite_action(int64_t gang_id, int8_t action);
	int req_gang_apply_action(int64_t role_id, int8_t action);
	int req_ganger_change_headship_auth(int64_t role_id, int8_t headship, Int8_Vec &add_auth_vec);
	int req_gang_upgrade(void);
	int req_gang_upgrade_send_gift(void);
	int req_gang_upgrade_cd_clear(int8_t cd_type, int32_t building_id);
	int req_gang_world_notice(void);
	int req_gang_logo_modify(int8_t logo_id);
	int req_gang_seize_power(void);
	int req_send_bless_buf(role_id_t target_role_id, int32_t bless_buf_id);
	int req_research_gang_skill(int32_t skill_id);
	int req_learn_ganger_skill(int32_t skill_id);
	int req_gang_draw(void);
	int req_gang_take_draw_item(void);
	int req_gang_building_upgrade(int32_t building_id);
	int req_gang_shop_buy_item(const int item_id, const int amount);

	int req_gang_war_enter(void);
	int req_gang_war_award(void);
	int req_gang_war_award_assign(Reward_Info_Gang_War &rigw);
	int gang_war_add_gang_war_award_get_times_by_day(const uint16_t i);
	int req_gang_war_draw(void);
//	int req_gang_war_get_rank_award(void);
//	int req_gang_war_rank_award_list(void);

	int inner_send_bless_buf(role_id_t sender_role_id, int32_t bless_buf_id, bool has_buf_id);

	int active_ganger_info_update();
	void active_ganger_string_info_change(void);

	void sync_ganger_info_to_chat();
	void sync_ganger_info_to_scene(void);
	void sync_ganger_war_info_to_scene(void);
	int ganger_load_war_detail(Block_Buffer &buf);
//	int gang_war_load_award(Block_Buffer &buf);
	// msg end

	void insert_apply_id(role_id_t gang_id);
	void erase_apply_id(role_id_t gang_id);
	void add_invite_gang_id(role_id_t gang_id);
	void del_invite_gang_id(role_id_t gang_id);
	void ganger_reset_leave_time(void);
	void ganger_refresh_base_detail(bool sync = true);
	void add_receive_bless_times(void);

	int test_research_all_gang_skill(void);

	int ganger_prop_refresh(void);								// ganger模块人物属性加成

	// interfaces
	int get_ganger_skill_pro_val(std::map<int, double> &pro_val_map) const;
	int add_contribute(const int donate_gain);
	int get_gang_level(void) const;
	bool get_is_level_lader(int32_t level);
	bool is_gang_leader(void) const;
	int get_remain_draw_times(void);

	// 公会修行
	int get_phy_power_practice_left(void);
	void listen_recover_phy_power(Time_Value &now);
	int start_phy_power_practice(void);
	void sync_client_practice_info(void);
	int stop_phy_power_practice(bool sync_to_cli = true);
	int speed_up_practice(void);
	bool check_in_gang_scene(void);
	int need_strenght(void);
	void reset_practice(void);

private:
	void make_ganger_info(Ganger_Info& info);
	int check_add_auth(int8_t headship, Int8_Vec &add_auth_vec, int &target_player_auth);
	void refresh_leader_offline_time(void);
	void refresh_can_seize_power(void);
	void notify_last_leader(void);
	void offline_be_seize_power(void);

	void module_repair(void);				// 如果core，公会数据存储比玩家数据要慢，以公会数据为准进行修复
	void module_init_headship_auth(void);

	void create_gang_init_ganger(Gang_DB_View* gang_view);

	int clear_cd_gang_lvl(Gang_DB_View* gang_view);
	int clear_cd_building_lvl(Gang_DB_View* gang_view, int building_id);

	int ganger_skill_prop_refresh(void);						// 公会技能属性加成
	int set_ganger_skill_pro_val(const std::map<int, double> &pro_val_map);

private:
	Ganger_Detail *ganger_detail_;
};

////////////////////////////////////////////////////////////////////////////

inline const Ganger_Detail& Ganger::ganger_detail(void) const {
	return *ganger_detail_;
}

inline Ganger_Detail& Ganger::ganger_detail_no_const() {
	return *ganger_detail_;
}

#endif /* GANGER_H_ */
