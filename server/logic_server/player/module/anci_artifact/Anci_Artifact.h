/*
 * Anci_Artifact.h
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#ifndef ANCI_ARTIFACT_H_
#define ANCI_ARTIFACT_H_

#include "Logic_Player_Base.h"

struct Anci_Artifact_Detail;
struct Anci_Artifact_Cond_Info;
typedef boost::unordered_map<int32_t, Anci_Artifact_Cond_Info*> AA_Info_Listen;

class Anci_Artifact : virtual public Packer {
public:
	Anci_Artifact(void);
	virtual ~Anci_Artifact(void);
	void reset(void);

	static int create_init(Anci_Artifact_Detail &detail);

	void module_init(void);

	void load_detail(Anci_Artifact_Detail &detail);
	void save_detail(Anci_Artifact_Detail &detail);

	const Anci_Artifact_Detail &anci_artifact_detail(void) const;

	void sign_in(void);
	int sign_in_repeat(void);

	int client_ready_now(void);
	void sign_out(void);
	int move_scene_completed(void);

//	// 登录或转场景同步逻辑信息到场景
//	int sync_scene_module_info(Block_Buffer &buf);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

//	// 每周0点刷新
//	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

	// msg
	int req_anci_artifact_list(void);
	int req_anci_artifact_unban(const int id);
	int req_anci_artifact_active(const int id);
	int req_anci_artifact_box_get(const int id);
	// msg

	// 监听神器解封条件
	int anci_artifact_listen_unban(const int fb_id);
	// 激活神器条件后，检查宝石状态、宝箱状态
	void anci_artifact_refresh_state(const int cond_id, const bool need_refresh_hero);

	int anci_artifact_active_msg(const int32_t id);
	int anci_artifact_listen_level(const int new_level, const int src_level);
	int anci_artifact_listen_pack_grid(void);
	int anci_artifact_listen_gold(void);
	int anci_artifact_listen_suit_purple(void);
//	int anci_artifact_listen_suit_gold(const int32_t id);
	int anci_artifact_listen_rune(void);
	int anci_artifact_listen_unban(void);
	int anci_artifact_listen_master(void);
	int anci_artifact_listen_friendship(void);
	int anci_artifact_listen_force(void);
	int anci_artifact_listen_gang_contribute(void);
	int anci_artifact_listen_recruit_hero(void);
	int anci_artifact_listen_war_kill_nums(void); // 战场累计击杀人
	int anci_artifact_listen_arena_win_nums(void); // 竞技场获胜场
	int anci_artifact_listen_gang_war_win_nums(void); // 公会战胜利次
	int anci_artifact_listen_exploit_lvl(void);
	int anci_artifact_listen_first_lvl_role_id(void); // 本服务器第一个达到级的玩家
	int anci_artifact_listen_first_lvl_ck(void); // 本服务器第一个达到级的刺客
	int anci_artifact_listen_first_lvl_zs(void); // 本服务器第一个达到级的战士
	int anci_artifact_listen_first_lvl_yss(void); // 本服务器第一个达到级的元素师
	int anci_artifact_listen_kill_monster_nums(void); // 击杀只怪物
	int anci_artifact_listen_single_dungeon_chapter_complete(const int chap_id); // 通关第章节
	int anci_artifact_listen_team_fb_complete_nums(void); // 完成级组队副本
	int anci_artifact_listen_horse_get(void); // 获得XXX坐骑
	int anci_artifact_listen_recharge_diamond(void);
	int anci_artifact_listen_secret_shop_buy_nums(void); // 神秘商店购买件物品
	int anci_artifact_listen_dragon_wale_rob_times(void); // 掠夺成功次
	int anci_artifact_listen_dragon_wale_step_foot_times(void); // 留下脚印次
	int anci_artifact_listen_dragon_wale_play_dragon_times(void); // 逗龙次
	int anci_artifact_listen_hero_awake_lvl(void); // 英雄一级觉醒
	int anci_artifact_listen_task_complete_nums(void); // 完成个任务
	int anci_artifact_listen_dragon_wale_gold_mine_lvl(void); // 金矿等级达到级
	int anci_artifact_listen_dragon_wale_moon_well_lvl(void); // 英灵图腾等级达到级
	int anci_artifact_listen_dragon_wale_totem_lvl(void); // 圣龙祭坛等级达到级
	int anci_artifact_listen_dragon_vale_drive_thief_times(void); // 驱逐骚扰怪次
	int anci_artifact_listen_war_light_win_nums(void); // 在光明阵营获胜场
	int anci_artifact_listen_war_dark_win_nums(void); // 在黑暗阵营获胜场
	int anci_artifact_listen_task_limit_time(void); // 分钟内完成限时击杀任务
	int anci_artifact_listen_kill_boss(void); // 击杀bossXXX
	int anci_artifact_listen_dragon_wale_keep_level(void);
	int anci_artifact_listen_horse_get_nums(void);
	int anci_artifact_listen_rune_stone_lvl(void);
	int anci_artifact_listen_equip_on_body_both_player_and_hero_by_lvl(void);
	int anci_artifact_listen_arena_lvl(void);
	int anci_artifact_listen_hero_nums_by_lvl(void);

//	int anci_artifact_count_suit_purple_nums(void);
//	int anci_artifact_count_suit_gold_nums(void);
	void aa_refresh_prop_hero(std::map<int, double> &pro_val_map);

protected:
	int aa_refresh_prop(void);
//	int aa_handle_award(const int8_t type, const int32_t id);

	int aa_construct(void);
	int aa_level_construct(void);
	int aa_pack_grid_construct(void);
	int aa_gold_construct(void);
	int aa_suit_purple_construct(void);
	int aa_suit_gold_construct(void);
	int aa_rune_construct(void);
	int aa_unban_construct(void);
	int aa_master_construct(void);
	int aa_friendship_construct(void);
	int aa_force_construct(void);
	int aa_gang_contribute_construct(void);
	int aa_recruit_hero_construct(void);
	int aa_war_kill_nums_construct(void); // 战场累计击杀人
	int aa_arena_win_nums_construct(void); // 竞技场获胜场
	int aa_gang_war_win_nums_construct(void); // 公会战胜利次
	int aa_exploit_lvl_construct(void);
	int aa_first_lvl_role_id_construct(void); // 本服务器第一个达到级的玩家
	int aa_first_lvl_ck_construct(void); // 本服务器第一个达到级的刺客
	int aa_first_lvl_zs_construct(void); // 本服务器第一个达到级的战士
	int aa_first_lvl_yss_construct(void); // 本服务器第一个达到级的元素师
	int aa_kill_monster_nums_construct(void); // 击杀只怪物
	int aa_single_dungeon_chapter_complete_construct(void);
	int aa_team_fb_complete_nums_construct(void); // 完成级组队副本
	int aa_horse_get_construct(void); // 获得XXX坐骑
	int aa_recharge_diamond_construct(void);
	int aa_secret_shop_buy_nums_construct(void); // 神秘商店购买件物品
	int aa_dragon_wale_rob_times_construct(void); // 掠夺成功次
	int aa_dragon_wale_step_foot_times_construct(void); // 留下脚印次
	int aa_dragon_wale_play_dragon_times_construct(void); // 逗龙次
	int aa_hero_awake_lvl_construct(void); // 英雄一级觉醒
	int aa_task_complete_nums_construct(void); // 完成个任务
	int aa_dragon_wale_gold_mine_lvl_construct(void); // 金矿等级达到级
	int aa_dragon_wale_moon_well_lvl_construct(void); // 英灵图腾等级达到级
	int aa_dragon_wale_totem_lvl_construct(void); // 圣龙祭坛等级达到级
	int aa_dragon_wale_drive_thief_times_construct(void); // 驱逐骚扰怪次
	int aa_war_light_win_nums_construct(void); // 在光明阵营获胜场
	int aa_war_dark_win_nums_construct(void); // 在黑暗阵营获胜场
	int aa_task_limit_time_construct(void); // 分钟内完成限时击杀任务
	int aa_kill_boss_construct(void); // 击杀bossXXX
	int aa_horse_get_nums_construct(void);
	int aa_dragon_wale_keep_level_construct(void);
	int aa_rune_stone_lvl_construct(void);
	int aa_equip_on_body_both_player_and_hero_by_lvl_construct(void);
	int aa_arena_lvl_construct(void);
	int aa_hero_nums_by_lvl_construct(void);

//	int aa_announce(const int id);

	void save_tick(void);

private:
	Anci_Artifact_Detail *anci_artifact_detail_;
	Int_Hash_Set aa_level;
	Int_Hash_Set aa_pack_grid;
	Int_Hash_Set aa_gold; // 金币
	Int_Hash_Set aa_suit_purple;
	Int_Hash_Set aa_suit_gold;
	Int_Hash_Set aa_rune;
	Int_Hash_Set aa_unban;
	Int_Hash_Set aa_master;
	Int_Hash_Set aa_friendship;
	Int_Hash_Set aa_force;
	Int_Hash_Set aa_gang_contribute;
	Int_Hash_Set aa_recruit_hero;
	Int_Hash_Set aa_war_kill_nums; // 战场累计击杀人
	Int_Hash_Set aa_arena_win_nums; // 竞技场获胜场
	Int_Hash_Set aa_gang_war_win_nums; // 公会战胜利次
	Int_Hash_Set aa_exploit_lvl;
	Int_Hash_Set aa_first_lvl_role_id; // 本服务器第一个达到级的玩家
	Int_Hash_Set aa_first_lvl_ck; // 本服务器第一个达到级的刺客
	Int_Hash_Set aa_first_lvl_zs; // 本服务器第一个达到级的战士
	Int_Hash_Set aa_first_lvl_yss; // 本服务器第一个达到级的元素师
	Int_Hash_Set aa_kill_monster_nums; // 击杀只怪物
	Int_Hash_Set aa_single_dungeon_chapter_complete; // 通关第章节
	Int_Hash_Set aa_team_fb_complete_nums; // 完成级组队副本
	Int_Hash_Set aa_horse_get; // 获得XXX坐骑
	Int_Hash_Set aa_recharge_diamond;
	Int_Hash_Set aa_secret_shop_buy_nums; // 神秘商店购买件物品
	Int_Hash_Set aa_dragon_wale_rob_times; // 掠夺成功次
	Int_Hash_Set aa_dragon_wale_step_foot_times; // 留下脚印次
	Int_Hash_Set aa_dragon_wale_play_dragon_times; // 逗龙次
	Int_Hash_Set aa_hero_awake_lvl; // 英雄一级觉醒
	Int_Hash_Set aa_task_complete_nums; // 完成个任务
	Int_Hash_Set aa_dragon_wale_gold_mine_lvl; // 金矿等级达到级
	Int_Hash_Set aa_dragon_wale_moon_well_lvl; // 英灵图腾等级达到级
	Int_Hash_Set aa_dragon_wale_totem_lvl; // 圣龙祭坛等级达到级
	Int_Hash_Set aa_dragon_wale_drive_thief_times; // 驱逐骚扰怪次
	Int_Hash_Set aa_war_light_win_nums; // 在光明阵营获胜场
	Int_Hash_Set aa_war_drak_win_nums; // 在黑暗阵营获胜场
	Int_Hash_Set aa_task_limit_time; // 分钟内完成限时击杀任务
	Int_Hash_Set aa_kill_boss; // 击杀bossXXX
	Int_Hash_Set aa_horse_get_nums;
	Int_Hash_Set aa_dragon_wale_keep_level;
	Int_Hash_Set aa_rune_stone_lvl;
	Int_Hash_Set aa_equip_on_body_both_player_and_hero_by_lvl;
	Int_Hash_Set aa_arena_lvl;
	Int_Hash_Set aa_hero_nums_by_lvl;
	Int_Hash_Set aa_first_boss_against;
	Int_Hash_Set aa_complete_suit; //集齐套装

	AA_Info_Listen listen_level;
	AA_Info_Listen listen_pack_grid;
	AA_Info_Listen listen_gold;
	AA_Info_Listen listen_suit_purple;
	AA_Info_Listen listen_suit_gold;
	AA_Info_Listen listen_rune;
	AA_Info_Listen listen_unban;
	AA_Info_Listen listen_master;
	AA_Info_Listen listen_friendship;
	AA_Info_Listen listen_force;
	AA_Info_Listen listen_gang_contribute;
	AA_Info_Listen listen_recruit_hero;
	AA_Info_Listen listen_war_kill_nums; // 战场累计击杀人
	AA_Info_Listen listen_arena_win_nums; // 竞技场获胜场
	AA_Info_Listen listen_gang_war_win_nums; // 公会战胜利次
	AA_Info_Listen listen_exploit_lvl; // 功勋等级
	AA_Info_Listen listen_first_lvl_role_id; // 本服务器第一个达到级的玩家
	AA_Info_Listen listen_first_lvl_ck; // 本服务器第一个达到级的刺客
	AA_Info_Listen listen_first_lvl_zs; // 本服务器第一个达到级的战士
	AA_Info_Listen listen_first_lvl_yss; // 本服务器第一个达到级的元素师
	AA_Info_Listen listen_kill_monster_nums; // 击杀只怪物
	AA_Info_Listen listen_single_dungeon_chapter_complete; // 通关第章节
	AA_Info_Listen listen_team_fb_complete_nums; // 完成级组队副本
	AA_Info_Listen listen_horse_get; // 获得XXX坐骑
	AA_Info_Listen listen_recharge_diamond;
	AA_Info_Listen listen_secret_shop_buy_nums; // 神秘商店购买件物品
	AA_Info_Listen listen_dragon_wale_rob_times; // 掠夺成功次
	AA_Info_Listen listen_dragon_wale_step_foot_times; // 留下脚印次
	AA_Info_Listen listen_dragon_wale_play_dragon_times; // 逗龙次
	AA_Info_Listen listen_hero_awake_lvl; // 英雄一级觉醒
	AA_Info_Listen listen_task_complete_nums; // 完成个任务
	AA_Info_Listen listen_dragon_wale_gold_mine_lvl; // 金矿等级达到级
	AA_Info_Listen listen_dragon_wale_moon_well_lvl; // 英灵图腾等级达到级
	AA_Info_Listen listen_dragon_wale_totem_lvl; // 圣龙祭坛等级达到级
	AA_Info_Listen listen_dragon_wale_drive_thief_times; // 驱逐骚扰怪次
	AA_Info_Listen listen_war_light_win_nums; // 在光明阵营获胜场
	AA_Info_Listen listen_war_dark_win_nums; // 在黑暗阵营获胜场
	AA_Info_Listen listen_task_limit_time; // 分钟内完成限时击杀任务
	AA_Info_Listen listen_kill_boss; // 击杀bossXXX
	AA_Info_Listen listen_horse_get_nums; // 获得个坐骑
	AA_Info_Listen listen_dragon_wale_keep_level; // 获得个坐骑
	AA_Info_Listen listen_rune_stone_lvl; // 符石等级
	AA_Info_Listen listen_equip_on_body_both_player_and_hero_by_lvl; // 主角、英雄身上x件紫装
	AA_Info_Listen listen_arena_lvl; // 竞技场达到白银段
	AA_Info_Listen listen_hero_nums_by_lvl; // 拥有x个x级英雄
	AA_Info_Listen listen_first_boss_against; // 第一个跟boss对战
	AA_Info_Listen listen_complete_suit; // 集齐套装
};

inline const Anci_Artifact_Detail &Anci_Artifact::anci_artifact_detail(void) const {
	return *anci_artifact_detail_;
}

#endif /* ANCI_ARTIFACT_H_ */
