/*
 * Achievement.h
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#ifndef ACHIEVEMENT_H_
#define ACHIEVEMENT_H_

#include "Logic_Player_Base.h"

class Packer;
struct Achievement_Detail;
struct Achievement_Info;
typedef boost::unordered_map<int32_t, Achievement_Info*> Achievement_Info_Listen;

class Achievement : virtual public Packer {
public:
	enum {
		ACHIEVEMENT_ARENA = 1,
		ACHIEVEMENT_REWARD = 2,
		ACHIEVEMENT_WAR_SCENE = 3,
		ACHIEVEMENT_ALCHEMY	= 4,
		ACHIEVEMENT_TEAM_SPORTS = 5,
	};
	Achievement(void);
	~Achievement(void);
	void reset(void);

	static int create_init(Achievement_Detail &detail);
	void load_detail(Achievement_Detail &detail);
	void save_detail(Achievement_Detail &detail);
	Achievement_Detail const &achievement_detail(void) const;
	void module_init(void);
	void sign_in(void);
	void sign_in_repeat(void);
	void sign_out(void);
	void client_ready_now(void);
	// 场景加载完成
	int move_scene_completed(void);
	//托管中下线调用
	void trusteeship_sign_out(void);

	const static int PACK_ACHIEVEMENT_GAP = 14;

	//msg
	int req_achievement_list(void);
	int req_achievement_get(const int8_t type, const int32_t id);
	int req_achievement_medal_onoff(const int32_t source_index, const int32_t dst_index);
	//msg

	int achievement_active_msg(const int32_t id);
	int achievement_listen_level(const int new_level, const int src_level);
	int achievement_listen_pack_grid(void);
	int achievement_listen_gold(void);
	int achievement_listen_suit_purple(const int32_t id);
	int achievement_listen_suit_gold(const int32_t id);
	int achievement_listen_rune(void);
	int achievement_listen_unban(const int32_t lvl);
	int achievement_listen_master(const int32_t lvl);
	int achievement_listen_friendship(void);
	int achievement_listen_force(const int32_t force);
	int achievement_listen_gang_contribute(void);
	int achievement_listen_recruit_hero(void);
	int achievement_listen_war_kill_nums(void); // 战场累计击杀人
	int achievement_listen_arena_win_nums(void); // 竞技场获胜场
	int achievement_listen_gang_war_win_nums(void); // 公会战胜利次
	int achievement_listen_exploit_lvl(void);
	int achievement_listen_first_lvl_role_id(void); // 本服务器第一个达到级的玩家
	int achievement_listen_first_lvl_ck(void); // 本服务器第一个达到级的刺客
	int achievement_listen_first_lvl_zs(void); // 本服务器第一个达到级的战士
	int achievement_listen_first_lvl_yss(void); // 本服务器第一个达到级的元素师
	int achievement_listen_kill_monster_nums(const int amount); // 击杀只怪物
	int achievement_listen_single_dungeon_chapter_complete(const int chap_id); // 通关第章节
	int achievement_listen_team_fb_complete_nums(const int lvl); // 完成级组队副本
	int achievement_listen_horse_get(const int horse_id); // 获得XXX坐骑
	int achievement_listen_recharge_diamond(void);
	int achievement_listen_secret_shop_buy_nums(const int amount); // 神秘商店购买件物品
	int achievement_listen_dragon_wale_rob_times(void); // 掠夺成功次
	int achievement_listen_dragon_wale_step_foot_times(void); // 留下脚印次
	int achievement_listen_dragon_wale_play_dragon_times(void); // 逗龙次
	int achievement_listen_hero_awake_lvl(const int lvl); // 英雄一级觉醒
	int achievement_listen_task_complete_nums(void); // 完成个任务
	int achievement_listen_dragon_wale_gold_mine_lvl(const int lvl); // 金矿等级达到级
	int achievement_listen_dragon_wale_moon_well_lvl(const int lvl); // 英灵图腾等级达到级
	int achievement_listen_dragon_wale_totem_lvl(const int lvl); // 圣龙祭坛等级达到级
	int achievement_listen_dragon_vale_drive_thief_times(void); // 驱逐骚扰怪次
	int achievement_listen_war_light_win_nums(void); // 在光明阵营获胜场
	int achievement_listen_war_dark_win_nums(void); // 在黑暗阵营获胜场
	int achievement_listen_task_limit_time(const int task_limit_time); // 分钟内完成限时击杀任务
	int achievement_listen_kill_boss(const int boss_id); // 击杀bossXXX
	int achievement_listen_dragon_wale_keep_level(const int lvl); // 龙等级
	int achievement_listen_horse_get_nums(void); // 获得x个坐骑
	int achievement_listen_rune_stone_lvl(const int32_t lvl); // 符石达到最高等级
	int achievement_listen_equip_on_body_both_player_and_hero_by_lvl(void); // 主角.英雄身上x件紫装
	int achievement_listen_arena_lvl(const int32_t lvl); // 竞技场达到x段
	int achievement_listen_hero_nums_by_lvl(void); // 拥有x个x级英雄
	int achievement_listen_first_boss_against(void); // 第一个跟boss对战
	int achievement_listen_complete_suit(const int32_t item_id); // 集齐套装

	int achievement_count_suit_purple_nums(void);
	int achievement_count_suit_gold_nums(void);
	int get_equip_on_body_both_player_and_hero_by_lvl(const int lvl);
	int get_hero_nums_by_lvl(const int lvl, const int num);
	int get_equip_nums(Int_Int_Set_Map cond_ext1);

	//medal
	int achievement_refresh_medal_times(const int8_t type);
	int achievement_arena_times(void); // 竞技场次数
	int achievement_reward_times(void); // 悬赏令次数
	int achievement_war_scene_times(void); // 战场收益次数
	int achievement_alchemy_times(void); // 炼金免费次数次数
	int achievement_team_sports_times(void); // 组队竞技收益次数

protected:
	int achievement_refresh_prop(void);
	int achievement_handle_award(const int8_t type, const int32_t id);

	int achievement_construct(void);
	int achievement_level_construct(void);
	int achievement_pack_grid_construct(void);
	int achievement_gold_construct(void);
	int achievement_suit_purple_construct(void);
	int achievement_suit_gold_construct(void);
	int achievement_rune_construct(void);
	int achievement_unban_construct(void);
	int achievement_master_construct(void);
	int achievement_friendship_construct(void);
	int achievement_force_construct(void);
	int achievement_gang_contribute_construct(void);
	int achievement_recruit_hero_construct(void);
	int achievement_war_kill_nums_construct(void); // 战场累计击杀人
	int achievement_arena_win_nums_construct(void); // 竞技场获胜场
	int achievement_gang_war_win_nums_construct(void); // 公会战胜利次
	int achievement_exploit_lvl_construct(void);
	int achievement_first_lvl_role_id_construct(void); // 本服务器第一个达到级的玩家
	int achievement_first_lvl_ck_construct(void); // 本服务器第一个达到级的刺客
	int achievement_first_lvl_zs_construct(void); // 本服务器第一个达到级的战士
	int achievement_first_lvl_yss_construct(void); // 本服务器第一个达到级的元素师
	int achievement_kill_monster_nums_construct(void); // 击杀只怪物
	int achievement_single_dungeon_chapter_complete_construct(void);
	int achievement_team_fb_complete_nums_construct(void); // 完成级组队副本
	int achievement_horse_get_construct(void); // 获得XXX坐骑
	int achievement_recharge_diamond_construct(void);
	int achievement_secret_shop_buy_nums_construct(void); // 神秘商店购买件物品
	int achievement_dragon_wale_rob_times_construct(void); // 掠夺成功次
	int achievement_dragon_wale_step_foot_times_construct(void); // 留下脚印次
	int achievement_dragon_wale_play_dragon_times_construct(void); // 逗龙次
	int achievement_hero_awake_lvl_construct(void); // 英雄一级觉醒
	int achievement_task_complete_nums_construct(void); // 完成个任务
	int achievement_dragon_wale_gold_mine_lvl_construct(void); // 金矿等级达到级
	int achievement_dragon_wale_moon_well_lvl_construct(void); // 英灵图腾等级达到级
	int achievement_dragon_wale_totem_lvl_construct(void); // 圣龙祭坛等级达到级
	int achievement_dragon_wale_drive_thief_times_construct(void); // 驱逐骚扰怪次
	int achievement_war_light_win_nums_construct(void); // 在光明阵营获胜场
	int achievement_war_dark_win_nums_construct(void); // 在黑暗阵营获胜场
	int achievement_task_limit_time_construct(void); // 分钟内完成限时击杀任务
	int achievement_kill_boss_construct(void); // 击杀bossXXX
	int achievement_horse_get_nums_construct(void);
	int achievement_dragon_wale_keep_level_construct(void);
	int achievement_rune_stone_lvl_construct(void);
	int achievement_equip_on_body_both_player_and_hero_by_lvl_construct(void);
	int achievement_arena_lvl_construct(void);
	int achievement_hero_nums_by_lvl_construct(void);
	int achievement_first_boss_against_construct(void);  //第一个跟boss对战
	int achievement_complete_suit_construct(void);  //集齐套装

	int achievement_announce(const int id);

	void save_tick(void);

private:
	Achievement_Detail *achievement_detail_;
	Int_Hash_Set aim_level;
	Int_Hash_Set aim_pack_grid;
	Int_Hash_Set aim_gold; // 金币
	Int_Hash_Set aim_suit_purple;
	Int_Hash_Set aim_suit_gold;
	Int_Hash_Set aim_rune;
	Int_Hash_Set aim_unban;
	Int_Hash_Set aim_master;
	Int_Hash_Set aim_friendship;
	Int_Hash_Set aim_force;
	Int_Hash_Set aim_gang_contribute;
	Int_Hash_Set aim_recruit_hero;
	Int_Hash_Set aim_war_kill_nums; // 战场累计击杀人
	Int_Hash_Set aim_arena_win_nums; // 竞技场获胜场
	Int_Hash_Set aim_gang_war_win_nums; // 公会战胜利次
	Int_Hash_Set aim_exploit_lvl;
	Int_Hash_Set aim_first_lvl_role_id; // 本服务器第一个达到级的玩家
	Int_Hash_Set aim_first_lvl_ck; // 本服务器第一个达到级的刺客
	Int_Hash_Set aim_first_lvl_zs; // 本服务器第一个达到级的战士
	Int_Hash_Set aim_first_lvl_yss; // 本服务器第一个达到级的元素师
	Int_Hash_Set aim_kill_monster_nums; // 击杀只怪物
	Int_Hash_Set aim_single_dungeon_chapter_complete; // 通关第章节
	Int_Hash_Set aim_team_fb_complete_nums; // 完成级组队副本
	Int_Hash_Set aim_horse_get; // 获得XXX坐骑
	Int_Hash_Set aim_recharge_diamond;
	Int_Hash_Set aim_secret_shop_buy_nums; // 神秘商店购买件物品
	Int_Hash_Set aim_dragon_wale_rob_times; // 掠夺成功次
	Int_Hash_Set aim_dragon_wale_step_foot_times; // 留下脚印次
	Int_Hash_Set aim_dragon_wale_play_dragon_times; // 逗龙次
	Int_Hash_Set aim_hero_awake_lvl; // 英雄一级觉醒
	Int_Hash_Set aim_task_complete_nums; // 完成个任务
	Int_Hash_Set aim_dragon_wale_gold_mine_lvl; // 金矿等级达到级
	Int_Hash_Set aim_dragon_wale_moon_well_lvl; // 英灵图腾等级达到级
	Int_Hash_Set aim_dragon_wale_totem_lvl; // 圣龙祭坛等级达到级
	Int_Hash_Set aim_dragon_wale_drive_thief_times; // 驱逐骚扰怪次
	Int_Hash_Set aim_war_light_win_nums; // 在光明阵营获胜场
	Int_Hash_Set aim_war_drak_win_nums; // 在黑暗阵营获胜场
	Int_Hash_Set aim_task_limit_time; // 分钟内完成限时击杀任务
	Int_Hash_Set aim_kill_boss; // 击杀bossXXX
	Int_Hash_Set aim_horse_get_nums;
	Int_Hash_Set aim_dragon_wale_keep_level;
	Int_Hash_Set aim_rune_stone_lvl;
	Int_Hash_Set aim_equip_on_body_both_player_and_hero_by_lvl;
	Int_Hash_Set aim_arena_lvl;
	Int_Hash_Set aim_hero_nums_by_lvl;
	Int_Hash_Set aim_first_boss_against; //第一个跟boss对战
	Int_Hash_Set aim_complete_suit;  //集齐套装

	Achievement_Info_Listen listen_level;
	Achievement_Info_Listen listen_pack_grid;
	Achievement_Info_Listen listen_gold;
	Achievement_Info_Listen listen_suit_purple;
	Achievement_Info_Listen listen_suit_gold;
	Achievement_Info_Listen listen_rune;
	Achievement_Info_Listen listen_unban;
	Achievement_Info_Listen listen_master;
	Achievement_Info_Listen listen_friendship;
	Achievement_Info_Listen listen_force;
	Achievement_Info_Listen listen_gang_contribute;
	Achievement_Info_Listen listen_recruit_hero;
	Achievement_Info_Listen listen_war_kill_nums; // 战场累计击杀人
	Achievement_Info_Listen listen_arena_win_nums; // 竞技场获胜场
	Achievement_Info_Listen listen_gang_war_win_nums; // 公会战胜利次
	Achievement_Info_Listen listen_exploit_lvl; // 功勋等级
	Achievement_Info_Listen listen_first_lvl_role_id; // 本服务器第一个达到级的玩家
	Achievement_Info_Listen listen_first_lvl_ck; // 本服务器第一个达到级的刺客
	Achievement_Info_Listen listen_first_lvl_zs; // 本服务器第一个达到级的战士
	Achievement_Info_Listen listen_first_lvl_yss; // 本服务器第一个达到级的元素师
	Achievement_Info_Listen listen_kill_monster_nums; // 击杀只怪物
	Achievement_Info_Listen listen_single_dungeon_chapter_complete; // 通关第章节
	Achievement_Info_Listen listen_team_fb_complete_nums; // 完成级组队副本
	Achievement_Info_Listen listen_horse_get; // 获得XXX坐骑
	Achievement_Info_Listen listen_recharge_diamond;
	Achievement_Info_Listen listen_secret_shop_buy_nums; // 神秘商店购买件物品
	Achievement_Info_Listen listen_dragon_wale_rob_times; // 掠夺成功次
	Achievement_Info_Listen listen_dragon_wale_step_foot_times; // 留下脚印次
	Achievement_Info_Listen listen_dragon_wale_play_dragon_times; // 逗龙次
	Achievement_Info_Listen listen_hero_awake_lvl; // 英雄一级觉醒
	Achievement_Info_Listen listen_task_complete_nums; // 完成个任务
	Achievement_Info_Listen listen_dragon_wale_gold_mine_lvl; // 金矿等级达到级
	Achievement_Info_Listen listen_dragon_wale_moon_well_lvl; // 英灵图腾等级达到级
	Achievement_Info_Listen listen_dragon_wale_totem_lvl; // 圣龙祭坛等级达到级
	Achievement_Info_Listen listen_dragon_wale_drive_thief_times; // 驱逐骚扰怪次
	Achievement_Info_Listen listen_war_light_win_nums; // 在光明阵营获胜场
	Achievement_Info_Listen listen_war_dark_win_nums; // 在黑暗阵营获胜场
	Achievement_Info_Listen listen_task_limit_time; // 分钟内完成限时击杀任务
	Achievement_Info_Listen listen_kill_boss; // 击杀bossXXX
	Achievement_Info_Listen listen_horse_get_nums; // 获得个坐骑
	Achievement_Info_Listen listen_dragon_wale_keep_level; // 获得个坐骑
	Achievement_Info_Listen listen_rune_stone_lvl;
	Achievement_Info_Listen listen_equip_on_body_both_player_and_hero_by_lvl;
	Achievement_Info_Listen listen_arena_lvl;
	Achievement_Info_Listen listen_hero_nums_by_lvl;
	Achievement_Info_Listen listen_first_boss_against; // 第一个跟boss对战
	Achievement_Info_Listen listen_complete_suit;  // 集齐套装
};

inline Achievement_Detail const &Achievement::achievement_detail(void) const{
	return *achievement_detail_;
}


#endif /* ACHIEVEMENT_H_ */
