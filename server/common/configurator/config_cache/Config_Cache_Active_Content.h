/*
 * Config_Cache_Active_Content.h
 *
 *  Created on: 2014-06-25
 *      Author: "xiaoliang"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ACTIVE_CONTENT_H_
#define CONFIG_CACHE_ACTIVE_CONTENT_H_

#include "Cache_Base.h"
#include "active/Active_Content_Config.h"

typedef boost::unordered_map<uint32_t, UInt_UInt_Map> UInt_UInt_UInt_Map;

class Config_Cache_Active_Content : public Cache_Base {
public:
//	const Limit_Buy_Conf_Map* get_limit_buy_conf_map(void) const;
//	const Limit_Buy_Config* get_limit_buy_conf_by_id(uint32_t id) const ;

	const Vip_Club_Conf_Map* get_vip_club_conf_map(void) const;
	const Vip_Club_Config* get_vip_club_conf_by_id(uint32_t id) const;

	const Level_Ace_Conf_Map* get_level_ace_conf_map(void) const;
	const Level_Ace_Config* get_level_ace_conf_by_level(uint32_t level) const;

	const Sg_Pass_Conf_Map* get_sg_pass_conf_map(void) const;
	const Sg_Pass_Config* get_sg_pass_conf_by_id(uint32_t id) const;

	const Mount_Levelup_Conf_Map* get_mount_pass_conf_map(void) const;
	const Mount_Levelup_Config* get_mount_pass_conf_by_id(uint32_t id) const;

	const Wing_Levelup_Conf_Map* get_wing_pass_conf_map(void) const;
	const Wing_Levelup_Config* get_wing_pass_conf_by_id(uint32_t id) const;

	const Suit_Collect_Conf_Map* get_suit_collect_conf_map(uint32_t career);
	const Suit_Collect_Config* get_suit_collect_conf_by_level(uint32_t level, uint32_t career);

	const Suit_Collect_Two_Conf_Map* get_suit_collect_two_conf_map(uint32_t career);
	const Suit_Collect_Two_Config* get_suit_collect_two_conf_by_level(uint32_t level, uint32_t career);

	const Guild_Level_Conf_Map* get_guild_level_conf_map(void) const;
	const Guild_Level_Config* get_guild_level_conf_by_id(uint32_t id) const;

	const Gem_Levelup_Conf_Map* get_gem_levelup_conf_map(void) const;
	const Gem_Levelup_Config* get_gem_levelup_conf_by_id(uint32_t id) const;

	const Wing_Levelup_Conf_Map* get_wing_levelup_conf_map(void) const;
	const Wing_Levelup_Config* get_wing_levelup_conf_by_level(uint32_t level) const;

	const Mount_Levelup_Conf_Map* get_mount_levelup_conf_map(void) const;
	const Mount_Levelup_Config* get_mount_levelup_conf_by_id(uint32_t id) const;

	const Arena_Dan_Conf_Map* get_arena_dan_conf_map(void) const;
	const Arena_Dan_Config* get_arena_dan_conf_by_id(uint32_t id) const;

	const World_Boss_Conf_Map* get_world_boss_conf_map(void) const;
	const World_Boss_Config* get_world_boss_conf_by_id(uint32_t id) const;

	const Heroes_Door_Conf_Map* get_heroes_door_conf_map(void) const;
	const Heroes_Door_Config* get_heroes_door_conf_by_id(uint32_t id) const;

	const Team_Arena_Conf_Map* get_team_arena_conf_map(void) const;
	const Team_Arena_Config* get_team_arena_conf_by_id(uint32_t id) const;

	const Warer_Conf_Map* get_warer_conf_map(void) const;
	const Warer_Config* get_warer_conf_by_id(uint32_t id) const;

	const Reward_Task_Conf_Map* get_reward_task_conf_map(void) const;
	const Active_Content_Reward_Task_Config* get_reward_task_conf_by_id(uint32_t id) const;

	const Sg_Pass_Conf_Map* get_sg_fb_conf_map(void) const;
	const Sg_Pass_Config* get_sg_fb_conf_by_id(uint32_t id) const;

	const Fm_X_Ci_Conf_Map* get_fm_x_ci_conf_map(void) const;
	const Fm_X_Ci_Config* get_fm_x_ci_conf_by_id(uint32_t id) const;

	const X_X_Quality_Heroer_Map* get_x_x_quality_heroer_conf_map(void) const;
	const X_X_Quality_Heroer_Config* get_x_x_quality_heroer_conf_by_id(uint32_t id) const;

	const Mount_Lvup_X_Conf_Map* get_mount_lvup_x_conf_map(void) const;
	const Mount_Lvup_X_Config* get_mount_lvup_x_conf_by_id(uint32_t id) const;

	const Fusion_X_Elite_Card_Conf_Map* get_fusion_x_elite_card_conf_map(void) const;
	const Fusion_X_Elite_Card_Config* get_fusion_x_elite_card_conf_by_id(uint32_t id) const;

	const Alchemy_Gift_Conf_Map* get_alchemy_gift_conf_map(void) const;
	const Alchemy_Gift_Config* get_alchemy_gift_conf_by_id(uint32_t id) const;

	const Activity_Gift_Conf_Map* get_activity_gift_conf_map(void) const;
	const Activity_Gift_Config* get_activity_gift_conf_by_id(uint32_t id) const;

	const Active_Ring_Task_Conf_Map* get_ring_task_conf_map(void) const;
	const Active_Ring_Task_Config* get_ring_task_conf_by_id(uint32_t id) const;

	const Dragon_Prey_Conf_Map* get_dragon_prey_conf_map(void) const;
	const Dragon_Prey_Config* get_dragon_prey_conf_by_id(uint32_t id) const;

	const Rank_Count_Config* get_level_rank_count_conf_by_rank(uint32_t rank) const;
	const Rank_Count_Config* get_arena_rank_count_conf_by_rank(uint32_t rank) const;
	const Rank_Count_Config* get_force_rank_count_conf_by_rank(uint32_t rank) const;
	const Rank_Count_Config* get_sg_rank_count_conf_by_rank(uint32_t rank) const;
	const Rank_Count_Config* get_mount_rank_count_conf_by_rank(uint32_t rank) const;
	const Rank_Count_Config* get_wing_rank_count_conf_by_rank(uint32_t rank) const;

	void refresh_config_cache(void);
//	void refresh_limit_buy_config_cache(void);
	void refresh_vip_club_config_cache(void);
	void refresh_level_ace_config_cache(void);
	void refresh_sg_pass_config_cache(void);
	void refresh_mount_pass_config_cache(void);
	void refresh_wing_pass_config_cache(void);
	void refresh_suit_collect_config_cache(void);
	void refresh_guild_level_config_cache(void);
	void refresh_level_rank_config_cache(void);
	void refresh_arena_rank_config_cache(void);
	void refresh_force_rank_config_cache(void);
	void refresh_sg_rank_config_cache(void);
	void refresh_mount_rank_config_cache(void);
	void refresh_wing_rank_config_cache(void);
	void refresh_gem_levelup_config_cache(void);
	void refresh_wing_levelup_config_cache(void);
	void refresh_mount_levelup_config_cache(void);
	void refresh_arena_dan_config_cache(void);

	void refresh_world_boss_config_cache(void);
	void refresh_suit_collect_two_config_cache(void);
	void refresh_heroes_door_config_cache(void);
	void refresh_team_arena_config_cache(void);
	void refresh_warer_config_cache(void);
	void refresh_reward_task_config_cache(void);
	void refresh_sg_fb_config_cache(void);

	void refresh_fm_x_ci_config_cache(void);
	void refresh_x_x_quality_heroe_config_cache(void);
	void refresh_mount_lvup_x_config_cache(void);
	void refresh_funsion_x_elite_card_config_cache(void);
	void refresh_alchemy_gift_config_cache(void);

	void refresh_activity_gift_config_cache(void);
	void refresh_ring_task_config_cache(void);
	void refresh_dragon_prey_config_cache(void);

	uint32_t get_act_add_num_config_by_act_id_day(uint32_t act_id, uint32_t day);
	void refresh_act_add_num_config_cache(void);

	void refresh_active_reward_info_config(const Json::Value& item_conf, Active_Reward_Info_Config &info);

private:

//	Limit_Buy_Conf_Map limit_buy_conf_map_;
	Vip_Club_Conf_Map vip_club_conf_map_;
	Level_Ace_Conf_Map level_ace_conf_map_;
	Sg_Pass_Conf_Map sg_pass_conf_map_;
	Suit_Collect_Conf_Map suit_collect_conf_map;
	Guild_Level_Conf_Map guild_level_conf_map_;
	Rank_Count_Conf_Map level_rank_conf_map_;
	UInt_UInt_Map level_rank_id_map;
	Rank_Count_Conf_Map arena_rank_conf_map_;
	UInt_UInt_Map arena_rank_id_map;
	Rank_Count_Conf_Map force_rank_conf_map_;
	UInt_UInt_Map force_rank_id_map;
	Rank_Count_Conf_Map sg_rank_conf_map_;
	UInt_UInt_Map sg_rank_id_map;
	Rank_Count_Conf_Map mount_rank_conf_map_;
	UInt_UInt_Map mount_rank_id_map;
	Mount_Levelup_Conf_Map mount_pass_conf_map_;
	Rank_Count_Conf_Map wing_rank_conf_map_;
	UInt_UInt_Map wing_rank_id_map;
	Wing_Levelup_Conf_Map wing_pass_conf_map_;
	Gem_Levelup_Conf_Map gem_levelup_conf_map;
	Wing_Levelup_Conf_Map wing_levelup_conf_map;
	Mount_Levelup_Conf_Map mount_levelup_conf_map;
	Arena_Dan_Conf_Map arena_dan_conf_map;

	World_Boss_Conf_Map world_boss_conf_map;
	Suit_Collect_Two_Conf_Map suit_collect_two_conf_map;
	Heroes_Door_Conf_Map heroes_door_conf_map;
	Team_Arena_Conf_Map team_arena_conf_map;
	Warer_Conf_Map warer_conf_map;
	Reward_Task_Conf_Map reward_task_conf_map;
	Sg_Pass_Conf_Map sg_fb_conf_map;
	Fm_X_Ci_Conf_Map fm_x_ci_conf_map;
	X_X_Quality_Heroer_Map x_x_quality_heroer_map;
	Mount_Lvup_X_Conf_Map mount_lvup_x_conf_map;
	Fusion_X_Elite_Card_Conf_Map fusion_x_elite_card_conf_map;
	Alchemy_Gift_Conf_Map alchemy_gift_conf_map;

	Activity_Gift_Conf_Map activity_gift_conf_map;
	Active_Ring_Task_Conf_Map ring_task_conf_map;
	Dragon_Prey_Conf_Map dragon_prey_conf_map;

	UInt_UInt_UInt_Map act_add_num_conf;
};
typedef Singleton<Config_Cache_Active_Content> Config_Cache_Active_Content_Single;
#define CONFIG_CACHE_ACTIVE_CONTENT    (Config_Cache_Active_Content_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ACTIVE_CONTENT_H_ */
