/*
 * Active_Content.h
 *
 *  Created on: Mar 7, 2014
 *      Author: XIAOLIANG
 */

#ifndef ACTIVE_CONTENT_H_
#define ACTIVE_CONTENT_H_

#include "Logic_Player_Base.h"

struct Active_Content_Detail;
struct Active_Reward_Info_Config;

class Active_Content : virtual public Packer {
public:
	Active_Content(void);
	virtual ~Active_Content();
	void reset(void);

	// 登录时detail赋值
	void load_detail(Active_Content_Detail &detail);
	// 保存时detail赋值
	void save_detail(Active_Content_Detail &data);
	// 登录时module初始化
	void module_init(void);
	void clear_active_content(void);
	void daily_clear_active_content(void);
	// 客户端初始化完成
	int client_ready_now(void);

	void send_active_content_open_icon(uint32_t icon_id, uint16_t time = 0, int8_t active = 0);
	void send_active_content_close_icon(uint32_t icon_id);
	void active_content_listen_level(uint32_t level);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);
	int trigger_daily_six(const Time_Value &now);

	//客户端请求相关 - start
//	int req_limit_buy(uint32_t id, uint32_t num);
	//客户端请求活动信息
	int req_active_content_info(void);
	int req_active_content_tip_num(void);
	/************************/
	int req_receive_vip_club_reward(uint32_t id, uint32_t reward_id);
	int req_receive_level_ace_reward(uint32_t level);
	int req_receive_suit_collect_reward(uint32_t level);
	int req_receive_guild_level_reward(uint32_t id);
	int req_receive_gem_levelup_reward(uint32_t id);
	int req_receive_wing_levelup_reward(uint32_t level);
	int req_receive_mount_levelup_reward(uint32_t id);
	int req_receive_sg_pass_reward(uint32_t id);
	int req_receive_mount_pass_reward(uint32_t id);
	int req_receive_wing_pass_reward(uint32_t id);
	int req_receive_arena_dan_reward(uint32_t id);
	int req_receive_active_content_reward(uint32_t act_type, uint32_t id);
	  /****************/
	int req_active_content_limit_buy(void);
	int req_active_content_vip_club(void);
	int req_active_content_level_ace(void);
	int req_active_content_suit_collect(void);
	int req_active_content_guild_level(void);
	int req_active_content_gem_levelup(void);
	int req_active_content_wing_levelup(void);
	int req_active_content_mount_levelup(void);
	int req_active_content_level_rank(void);
	int req_active_content_arena_rank(void);
	int req_active_content_force_rank(void);
	int req_active_content_sg_rank(void);
	int req_active_content_mount_rank(void);
	int req_active_content_wing_rank(void);
	int req_active_content_arena_dan(void);

	int req_active_content_world_boss(void);
	int req_active_content_suit_collect_two(void);
	int req_active_content_heroes_door(void);
	int req_active_content_team_arena(void);
	int req_active_content_warer(void);
	int req_active_content_reward_task(void);
	int req_active_content_sgfb(void);

	int req_active_content_fm_x_ci(void);
	int req_active_content_x_x_quality_heroer(void);
	int req_active_content_mount_lvup_x(void);
	int req_active_content_fusion_x_elite_card(void);
	int req_active_content_alchemy_gift(void);
	int req_active_content_activity_gift(void);
	int req_active_content_ring_task(void);
	int req_active_content_dragon_prey(void);
	//客户端请求相关 - end

	void active_content_listen_gang_level_up(int64_t gang_id, int32_t level);
	void active_content_listen_gang_change(void);
	void active_content_listen_vip_level_change(int32_t level);
	void active_content_listen_suit_collect(uint32_t item_id);
	void active_content_listen_arena_levelup(int group, int seg);//arena_dan_pass_time
	void active_content_listen_world_boss_end_info(uint32_t hurt, uint32_t rank, uint32_t hurt_rate);
	void active_content_listen_heroes_door(uint32_t type, uint32_t level);
	void active_content_listen_warer_win(void);
	void active_content_listen_team_arena_win(void);
	void active_content_listen_reward_task_done(void);
	void active_content_listen_enchanting(void);
	void active_content_listen_mount_evolve(void);
	void active_content_listen_card_fusion(uint32_t type);
	void active_content_listen_alchemy_num(void);
	void active_content_listen_dragon_prey_num(void);

	Active_Content_Detail const &active_content_detail(void) const;
	Time_Value &get_arena_dan_level_time(uint32_t group, uint32_t seg);
	int receive_arena_dan_reward_in_mail(void);
	int receive_common_reward_in_mail(uint32_t act_id);

	void get_heroer_type_num_map(UInt_UInt_Map &map);

	//common
	void active_listen_level_up(const uint32_t level);
	void active_listen_recharge(void);

private:
	Active_Content_Detail *active_content_detail_;
	bool is_open_icon;

	void clear_and_send_mail_on_reward_activity_gift(void);

	int receive_active_content_reward(const Active_Reward_Info_Config &reward_info, const Gain_Item_DM_Info &item_dm_info, const Money_DM_Info &money_dm_info_);


	int receive_active_content_reward_in_mail(const Active_Reward_Info_Config &reward_info, uint32_t mail_id = 3001);
	void fill_role_info(Role_Info &role, role_id_t role_id);
	uint32_t get_active_content_rune_by_level(uint32_t level);
	uint32_t get_mount_star_level_by_mount_id(uint32_t id);
};

inline Active_Content_Detail const &Active_Content::active_content_detail(void) const{
	return *active_content_detail_;
}

#endif /* ACTIVE_CONTENT_H_ */
