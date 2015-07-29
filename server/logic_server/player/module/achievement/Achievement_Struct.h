/*
 * Achievement_Struct.h
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#ifndef ACHIEVEMENT_STRUCT_H_
#define ACHIEVEMENT_STRUCT_H_

#include "Server_Struct.h"

struct Achievement_Info {
	Achievement_Info(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		state_count = 0;
		state_award = 0;
		data = 0;
	}
	int32_t id;
	int8_t state_count;
	int8_t state_award;
	int32_t data;
};
typedef boost::unordered_map<int32_t, Achievement_Info> Achievement_Info_Map;
typedef boost::unordered_map<int32_t, Achievement_Info*> Achievement_Info_Listen;
struct Achievement_Detail : public Detail {
	Achievement_Detail(void);
	~Achievement_Detail(void);
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	void detail_change(void) {
		is_change = true;
	}

	role_id_t role_id;
	int64_t achievement_count; // 成就点
	Achievement_Info_Map achievement_info_map;
	Int_Hash_Set complete_ids;

	Int_Hash_Set suit_purple_ids;
	Int_Hash_Set suit_gold_ids;
	int32_t rune_lvl_his;
	int32_t unban_lvl_his;
	int32_t master_lvl_his;
	int32_t force_lvl_his;
	int32_t war_kill_nums_his; // 战场累计击杀人
	int32_t arena_win_nums_his; // 竞技场获胜场
	int32_t gang_war_win_nums_his; // 公会战胜利次
//	Int_Uid first_lvl_role_id_his; // 本服务器第一个达到级的玩家
//	Int_Uid first_lvl_ck_his; // 本服务器第一个达到级的刺客
//	Int_Uid first_lvl_zs_his; // 本服务器第一个达到级的战士
//	Int_Uid first_lvl_yss_his; // 本服务器第一个达到级的元素师
	int32_t kill_monster_nums_his; // 击杀只怪物
	Int_Hash_Set team_fb_complete_nums_his; // 完成级组队副本
	Int_Hash_Set horse_get_his; // 获得XXX坐骑
	int32_t secret_shop_buy_nums_his; // 神秘商店购买件物品
	int32_t dragon_wale_rob_times_his; // 掠夺成功次
	int32_t dragon_wale_step_foot_times_his; // 留下脚印次
	int32_t dragon_wale_play_dragon_times_his; // 逗龙次
	int32_t hero_awake_lvl_his; // 英雄一级觉醒
	int32_t task_complete_nums_his; // 完成个任务
	int32_t dragon_wale_gold_mine_lvl_his; // 金矿等级达到级
	int32_t dragon_wale_moon_well_lvl_his; // 英灵图腾等级达到级
	int32_t dragon_wale_totem_lvl_his; // 圣龙祭坛等级达到级
	int32_t dragon_wale_drive_thief_times_his; // 驱逐骚扰怪次
	int32_t war_light_win_nums_his; // 在光明阵营获胜场
	int32_t war_dark_win_nums_his; // 在黑暗阵营获胜场
	int32_t task_limit_time_his; // 分钟内完成限时击杀任务
	Int_Hash_Set kill_boss_his; // 击杀bossXXX
	int32_t dragon_wale_keep_level_his;
	int rune_stone_lvl_his; // 符石最高等级
	int arena_lvl_his; // 竞技场达到x段
	int32_t first_boss_against; // 第一个跟boss对战
	Int_Hash_Set complete_suit; // 集齐套装
};


#endif /* ACHIEVEMENT_STRUCT_H_ */
