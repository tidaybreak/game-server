/*
 * Active_Content_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef ACTIVE_CONTENT_MANAGER_H_
#define ACTIVE_CONTENT_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Active_Content_Struct.h"
#include "Logic_Player.h"

class Active_Content_Manager {

public:
	Active_Content_Manager();
	virtual ~Active_Content_Manager();
	void reset(void);

	void start(void);
	void tick(void);

	void reset_count_flag(void);

	void reset_vip_announce_ed_level(void);
	void fill_role_info(Role_Info &role, role_id_t role_id);

	//图标控制相关 - 紧限与开服活动和开服活动以后的变种活动使用;没法倒计时也没法做任何事情,只能表示开和关
	void open_icon(const uint32_t icon_id, const Time_Value &close_time, const uint32_t level = 0, const uint32_t act_id = 0);
	void close_icon(const uint32_t icon_id, const uint32_t act_id = 0);
	Icon_Time_Info &get_icon_time_status(void);
	uint32_t get_icon_limit_level(void);
	//图标 -- end

	//限时抢购 - start
	void open_limit_buy(const Time_Value &start, const Time_Value &end);
	void close_limit_buy(void);
//	void add_limit_buy_num(uint32_t id, uint32_t num);
//	Limit_Buy_Config* get_limit_buy_info(uint32_t id);
	const Time_Value& get_limit_buy_start_time(void);
	const Time_Value& get_limit_buy_end_time(void);
	//限时抢购 - end

	//VIP俱乐部 - start
	void open_vip_club(const Time_Value &start, const Time_Value &end);
	void close_vip_club(void);
	void count_vip_num_on_start(void);
	void listen_all_player_vip_level_change(uint32_t new_level, uint32_t old_level, Logic_Player *player);
	const Time_Value& get_vip_club_start_time(void);
	const Time_Value& get_vip_club_end_time(void);
	const UInt_UInt_Map& get_vip_club_level_num(void);
	const uint32_t get_vip_club_num_by_vip_level(uint32_t level);
	//VIP俱乐部 - end

	//冲级高手 - start
	void open_level_ace(const Time_Value &start, const Time_Value &end);
	void close_level_ace(void);
	const Time_Value& get_level_ace_start_time(void);
	const Time_Value& get_level_ace_end_time(void);
	//冲级高手 - end

	//翅膀升级 - start
	void open_wing_levelup(const Time_Value &start, const Time_Value &end);
	void close_wing_levelup(void);
	const Time_Value& get_wing_levelup_start_time(void);
	const Time_Value& get_wing_levelup_end_time(void);
	//翅膀升级 - end

	//套装收集 - start
	void open_suit_collect(const Time_Value &start, const Time_Value &end);
	void close_suit_collect(void);
	const Time_Value& get_suit_collect_start_time(void);
	const Time_Value& get_suit_collect_end_time(void);
	//套装收集 - end

	//公会升级 - start
	void open_guild_level(const Time_Value &start, const Time_Value &end);
	void close_guild_level(void);
	const Time_Value& get_guild_level_start_time(void);
	const Time_Value& get_guild_level_end_time(void);
	bool have_announce_guild_of_level(int64_t gang_id, uint32_t level);
	//公会升级 - end

	//等级排名 - start
	void open_level_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time);
	void count_level_rank(void);
	void close_level_rank(void);
	const Time_Value& get_level_rank_start_time(void);
	const Time_Value& get_level_rank_end_time(void);
	const Time_Value& get_level_rank_count_time(void);
	void get_three_level_rank_on_count(std::vector<Role_Info> &role_info_list);
	int get_level_rank_on_count_rank(role_id_t role_id);
	bool is_count_level_rank(void);
	//等级排名 - end

	//竞技场排名 - start
	void open_arena_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time);
	void count_arena_rank(void);
	void close_arena_rank(void);
	const Time_Value& get_arena_rank_start_time(void);
	const Time_Value& get_arena_rank_end_time(void);
	const Time_Value& get_arena_rank_count_time(void);
	void get_three_arena_rank_on_count(std::vector<Role_Info> &role_info_list);
	int get_arena_rank_on_count_rank(role_id_t role_id);
	bool is_count_arena_rank(void);
	//竞技场排名 - end

	//战斗力排名 - start
	void open_force_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time);
	void count_force_rank(void);
	void close_force_rank(void);
	const Time_Value& get_force_rank_start_time(void);
	const Time_Value& get_force_rank_end_time(void);
	const Time_Value& get_force_rank_count_time(void);
	void get_three_force_rank_on_count(std::vector<Role_Info> &role_info_list);
	int get_force_rank_on_count_rank(role_id_t role_id);
	bool is_count_force_rank(void);
	//战斗力排名 - end

	//上古副本排名 - start
	void open_sg_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time);
	void count_sg_rank(void);
	void close_sg_rank(void);
	const Time_Value& get_sg_rank_start_time(void);
	const Time_Value& get_sg_rank_end_time(void);
	const Time_Value& get_sg_rank_count_time(void);
	void get_three_sg_rank_on_count(std::vector<Role_Info> &role_info_list);
	int get_sg_rank_on_count_rank(role_id_t role_id);
	bool is_count_sg_rank(void);
	//上古副本排名 - end

	//Mount排名 - start
	void open_mount_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time);
	void count_mount_rank(void);
	void close_mount_rank(void);
	const Time_Value& get_mount_rank_start_time(void);
	const Time_Value& get_mount_rank_end_time(void);
	const Time_Value& get_mount_rank_count_time(void);
	void get_three_mount_rank_on_count(std::vector<Role_Info> &role_info_list);
	int get_mount_rank_on_count_rank(role_id_t role_id);
	bool is_count_mount_rank(void);
	//Mount排名 - end

	//Wing排名 - start
	void open_wing_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time);
	void count_wing_rank(void);
	void close_wing_rank(void);
	const Time_Value& get_wing_rank_start_time(void);
	const Time_Value& get_wing_rank_end_time(void);
	const Time_Value& get_wing_rank_count_time(void);
	void get_three_wing_rank_on_count(std::vector<Role_Info> &role_info_list);
	int get_wing_rank_on_count_rank(role_id_t role_id);
	bool is_count_wing_rank(void);
	//Wing排名 - end

	//竞技场段位 - start
	void open_arena_dan(const uint32_t id, const Time_Value &start, const Time_Value &end);
	void close_arena_dan(void);
	const Time_Value& get_arena_dan_start_time(void);
	const Time_Value& get_arena_dan_last_start_time(void);
	const uint32_t get_arena_dan_last_act_id(void);
	const Time_Value& get_arena_dan_end_time(void);
	//竞技场段位 - end

	//公用活动时间控制 *********** start
	void open_common(const uint32_t id,const uint32_t type,const Time_Value &start, const Time_Value &end);
	void close_common(const uint32_t id);
	const Time_Value& get_common_start_time(const uint32_t id);
	const Time_Value& get_common_last_start_time(const uint32_t id);
	const Int_Time_Map& get_common_last_start_time_map(void);
	const Time_Value& get_common_end_time(const uint32_t id);
	const uint32_t get_comm_act_type_id(uint32_t type);
	//公用活动时间控制 *********** end

	//公用活动图标控制 ********************start
	void common_open_icon(const uint32_t icon_id, const Time_Value &close_time, const uint32_t level, const Time_Value & dtime, const int8_t active);
	void common_close_icon(const uint32_t icon_id);
	Common_Active_Icon_Info_Map &get_common_icon(void);
	//公用活动图标控制 ********************end


	void active_content_listen_world_boss_end_info(role_id_t role_id , uint32_t damage, uint32_t rank, uint32_t damage_rate);

private:

	bool can_reset_count;

	bool need_save_db;
	//排行榜活动刷新领取奖励,发送邮件
	int receive_count_rank_reward(const Active_Reward_Info_Config &conf, role_id_t role_id, uint32_t rank_type, uint32_t rank);
	//图标 -- start
	Icon_Time_Info icon_time_status;
	uint32_t icon_limit_level;
	//图标 -- end

	//限时抢购 - start
	UInt_UInt_Map limit_buy_num; //ID,对应购买次数
	Time_Value limit_buy_start_time; //开始时间
	Time_Value limit_buy_end_time; //结束时间
	//限时抢购 - end

	//VIP俱乐部 - start
	Time_Value vip_club_start_time;//开始时间
	Time_Value vip_club_end_time;//结束时间
	UInt_UInt_Map vip_club_level_num; //VIP等级;对应的人数;
	//VIP俱乐部 - end

	//冲级高手 - start
	Time_Value level_ace_start_time;//开始时间
	Time_Value level_ace_end_time;//结束时间
	//冲级高手 - end

	//冲级高手 - start
	Time_Value wing_levelup_start_time;//开始时间
	Time_Value wing_levelup_end_time;//结束时间
	//冲级高手 - end

	//套装收集 - start
	Time_Value suit_collect_start_time;//开始时间
	Time_Value suit_collect_end_time;//结束时间
	//套装收集 - end

	//公会升级 - start
	Time_Value guild_level_start_time;//开始时间
	Time_Value guild_level_end_time;//结束时间
//	UInt_Set guild_announce_ed_level;
	//公会升级 - end

	//等级排名 - start
	Time_Value level_rank_start_time;//开始时间
	Time_Value level_rank_count_time;//结算时间
	Time_Value level_rank_end_time;//结束时间
	//等级排名 -end

	//竞技场排名 - start
	Time_Value arena_rank_start_time;//开始时间
	Time_Value arena_rank_count_time;//结算时间
	Time_Value arena_rank_end_time;//结束时间
	//竞技场排名 - end

	//战斗力排名 - start
	Time_Value force_rank_start_time;//开始时间
	Time_Value force_rank_count_time;//结算时间
	Time_Value force_rank_end_time;//结束时间
	//战斗力排名 - end

	//上古副本排名 - start
	Time_Value sg_rank_start_time;//开始时间
	Time_Value sg_rank_count_time;//结算时间
	Time_Value sg_rank_end_time;//结束时间
	//上古副本排名 - end

	//mount排名 - start
	Time_Value mount_rank_start_time;//开始时间
	Time_Value mount_rank_count_time;//结算时间
	Time_Value mount_rank_end_time;//结束时间
	//mount排名 - end

	//wing排名 - start
	Time_Value wing_rank_start_time;//开始时间
	Time_Value wing_rank_count_time;//结算时间
	Time_Value wing_rank_end_time;//结束时间
	//wing排名 - end

	//竞技场段位 - start
	Time_Value arena_dan_start_time;//开始时间
	Time_Value arena_dan_end_time;//结束时间
	//竞技场段位 - end

	//公用活动时间控制 *********** start
	Int_Time_Map common_start_time;
	Int_Time_Map common_type_start_time;
	Int_Time_Map common_end_time;
	UInt_UInt_Map comm_act_type_id;
	//公用活动时间控制 *********** end

	//公用活动图标控制 ********************start
	Common_Active_Icon_Info_Map common_icon_info;
	//公用活动图标控制 ********************end

	Active_Content_Info c;
};


typedef Singleton<Active_Content_Manager> Active_Content_Manager_Single;
#define ACTIVE_CONTENT_MANAGER    (Active_Content_Manager_Single::instance())

#endif /* ACTIVE_CONTENT_H_ */
