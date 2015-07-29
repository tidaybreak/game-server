/*
 * Anci_Artifact_Ext.cpp
 *
 *  Created on: 2014年10月11日
 *      Author: Linqiyou
 */

#include "Msg_Anci_Artifact_Struct.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "achievement/Achievement_Def.h"
#include "Anci_Artifact_Def.h"
#include "Anci_Artifact_Struct.h"
#include "pack/Pack_Struct.h"
#include "pack/Packer.h"
#include "Anci_Artifact.h"
#include "Config_Cache_Anci_Artifact.h"
#include "Config_Cache_Item.h"
#include "gang/Gang_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Logic_Player.h"
#include "Config_Cache_Equip.h"
#include "equiper/Equiper_Func.h"
#include "chater/Chater_Def.h"
#include "announce/Logic_Announce.h"
#include "public/Public_Struct.h"
#include "dungeon/Single_Dungeon_Struct.h"
#include "Config_Cache_NPC.h"
#include "team/Team_Manager.h"
#include "achievement/Achievement_Struct.h"

int Anci_Artifact::anci_artifact_listen_war_kill_nums(void) {
	int32_t nums = player_self()->achievement_detail().war_kill_nums_his;
	for (AA_Info_Listen::iterator it = listen_war_kill_nums.begin();
			it != listen_war_kill_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_war_kill_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_arena_win_nums(void) {
	int32_t nums = player_self()->achievement_detail().arena_win_nums_his;
	for (AA_Info_Listen::iterator it = listen_arena_win_nums.begin();
			it != listen_arena_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_arena_win_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_gang_war_win_nums(void) {
	int32_t nums = player_self()->achievement_detail().gang_war_win_nums_his;
	for (AA_Info_Listen::iterator it = listen_gang_war_win_nums.begin();
			it != listen_gang_war_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_gang_war_win_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_exploit_lvl(void) {
	int32_t nums = player_self()->public_detail().exploit_lvl;
	for (AA_Info_Listen::iterator it = listen_exploit_lvl.begin();
			it != listen_exploit_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_exploit_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_first_lvl_role_id(void) {
	for (AA_Info_Listen::iterator it = listen_first_lvl_role_id.begin();
			it != listen_first_lvl_role_id.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_role_id_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_role_id_his().end()
			|| it_fl->second != role_id()) {
			continue;
		}
		it->second->data = 1;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_first_lvl_role_id_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_first_lvl_ck(void) {
	for (AA_Info_Listen::iterator it = listen_first_lvl_ck.begin();
			it != listen_first_lvl_ck.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_ck_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_ck_his().end()
			|| it_fl->second != role_id()) {
			continue;
		}
		it->second->data = 1;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_first_lvl_ck_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_first_lvl_zs(void) {
	for (AA_Info_Listen::iterator it = listen_first_lvl_zs.begin();
			it != listen_first_lvl_zs.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_zs_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_zs_his().end()
			|| it_fl->second != role_id()) {
			continue;
		}
		it->second->data = 1;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_first_lvl_zs_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_first_lvl_yss(void) {
	for (AA_Info_Listen::iterator it = listen_first_lvl_yss.begin();
			it != listen_first_lvl_yss.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_yss_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_yss_his().end()
			|| it_fl->second != role_id()) {
			continue;
		}
		it->second->data = 1;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_first_lvl_yss_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_kill_monster_nums(void) {
	int32_t nums = player_self()->achievement_detail().kill_monster_nums_his;
	for (AA_Info_Listen::iterator it = listen_kill_monster_nums.begin();
			it != listen_kill_monster_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_kill_monster_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_single_dungeon_chapter_complete(const int chap_id) {
	for (AA_Info_Listen::iterator it = listen_single_dungeon_chapter_complete.begin();
			it != listen_single_dungeon_chapter_complete.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg || chap_id != cfg->cond_ext) {
			continue;
		}
		it->second->data = 1;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_single_dungeon_chapter_complete_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_team_fb_complete_nums(void) {
	for (AA_Info_Listen::iterator it = listen_team_fb_complete_nums.begin();
			it != listen_team_fb_complete_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		int32_t nums = player_self()->achievement_detail().team_fb_complete_nums_his.count(cfg->cond_ext);
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_team_fb_complete_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_horse_get(void) {
	for (AA_Info_Listen::iterator it = listen_horse_get.begin();
			it != listen_horse_get.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		int32_t nums = player_self()->achievement_detail().horse_get_his.count(cfg->cond_ext);
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_horse_get_construct();
//	anci_artifact_listen_horse_get_nums();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_recharge_diamond(void) {
	int32_t nums = static_cast<int32_t>(player_self()->public_detail().recharge_money_his);
	for (AA_Info_Listen::iterator it = listen_recharge_diamond.begin();
			it != listen_recharge_diamond.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_recharge_diamond_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_secret_shop_buy_nums(void) {
	int32_t nums = player_self()->achievement_detail().secret_shop_buy_nums_his;
	for (AA_Info_Listen::iterator it = listen_secret_shop_buy_nums.begin();
			it != listen_secret_shop_buy_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_secret_shop_buy_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_rob_times(void) {
	for (AA_Info_Listen::iterator it = listen_dragon_wale_rob_times.begin();
			it != listen_dragon_wale_rob_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->achievement_detail().dragon_wale_rob_times_his;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_dragon_wale_rob_times_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_step_foot_times(void) {
	for (AA_Info_Listen::iterator it = listen_dragon_wale_step_foot_times.begin();
			it != listen_dragon_wale_step_foot_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->achievement_detail().dragon_wale_step_foot_times_his;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_dragon_wale_step_foot_times_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_play_dragon_times(void) {
	for (AA_Info_Listen::iterator it = listen_dragon_wale_play_dragon_times.begin();
			it != listen_dragon_wale_play_dragon_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->achievement_detail().dragon_wale_play_dragon_times_his;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_dragon_wale_play_dragon_times_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_hero_awake_lvl(void) {
	int32_t nums = player_self()->achievement_detail().hero_awake_lvl_his;
	for (AA_Info_Listen::iterator it = listen_hero_awake_lvl.begin();
			it != listen_hero_awake_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_hero_awake_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_task_complete_nums(void) {
	int32_t nums = player_self()->achievement_detail().task_complete_nums_his;
	for (AA_Info_Listen::iterator it = listen_task_complete_nums.begin();
			it != listen_task_complete_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_task_complete_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_gold_mine_lvl(void) {
	int32_t nums = player_self()->achievement_detail().dragon_wale_gold_mine_lvl_his;
	for (AA_Info_Listen::iterator it = listen_dragon_wale_gold_mine_lvl.begin();
			it != listen_dragon_wale_gold_mine_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_dragon_wale_gold_mine_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_moon_well_lvl(void) {
	int32_t nums = player_self()->achievement_detail().dragon_wale_moon_well_lvl_his;
	for (AA_Info_Listen::iterator it = listen_dragon_wale_moon_well_lvl.begin();
			it != listen_dragon_wale_moon_well_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_dragon_wale_moon_well_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_totem_lvl(void) {
	int32_t nums = player_self()->achievement_detail().dragon_wale_totem_lvl_his;
	for (AA_Info_Listen::iterator it = listen_dragon_wale_totem_lvl.begin();
			it != listen_dragon_wale_totem_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_dragon_wale_totem_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_vale_drive_thief_times(void) {
	int32_t nums = player_self()->achievement_detail().dragon_wale_drive_thief_times_his;
	for (AA_Info_Listen::iterator it = listen_dragon_wale_drive_thief_times.begin();
			it != listen_dragon_wale_drive_thief_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_dragon_wale_drive_thief_times_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_war_light_win_nums(void) {
	int32_t nums = player_self()->achievement_detail().war_light_win_nums_his;
	for (AA_Info_Listen::iterator it = listen_war_light_win_nums.begin();
			it != listen_war_light_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_war_light_win_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_war_dark_win_nums(void) {
	int32_t nums = player_self()->achievement_detail().war_dark_win_nums_his;
	for (AA_Info_Listen::iterator it = listen_war_dark_win_nums.begin();
			it != listen_war_dark_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_war_dark_win_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_task_limit_time(void) {
	for (AA_Info_Listen::iterator it = listen_task_limit_time.begin();
			it != listen_task_limit_time.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		int32_t nums = 0;
		if (player_self()->achievement_detail().task_limit_time_his != -1
				&& player_self()->achievement_detail().task_limit_time_his <= cfg->cond_ext) {
			nums = 1;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_task_limit_time_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_kill_boss(void) {
	for (AA_Info_Listen::iterator it = listen_kill_boss.begin();
			it != listen_kill_boss.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		int32_t nums = player_self()->achievement_detail().kill_boss_his.count(cfg->cond_ext);
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_kill_boss_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_dragon_wale_keep_level(void) {
	int32_t nums = player_self()->achievement_detail().dragon_wale_keep_level_his;
	for (AA_Info_Listen::iterator it = listen_dragon_wale_keep_level.begin();
			it != listen_dragon_wale_keep_level.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_dragon_wale_keep_level_construct();
	return 0;
}

int Anci_Artifact::anci_artifact_listen_horse_get_nums(void) {
	int32_t nums = player_self()->get_mount_num();
	for (AA_Info_Listen::iterator it = listen_horse_get_nums.begin();
			it != listen_horse_get_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
	}
	save_tick();
	aa_horse_get_nums_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_rune_stone_lvl(void) {
	int32_t nums = player_self()->achievement_detail().rune_stone_lvl_his;
	for (AA_Info_Listen::iterator it = listen_rune_stone_lvl.begin();
			it != listen_rune_stone_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_rune_stone_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_equip_on_body_both_player_and_hero_by_lvl(void) {
	for (AA_Info_Listen::iterator it = listen_equip_on_body_both_player_and_hero_by_lvl.begin();
			it != listen_equip_on_body_both_player_and_hero_by_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->get_equip_on_body_both_player_and_hero_by_lvl(cfg->cond_ext);
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_equip_on_body_both_player_and_hero_by_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_arena_lvl(void) {
	int32_t nums = player_self()->achievement_detail().arena_lvl_his;
	for (AA_Info_Listen::iterator it = listen_arena_lvl.begin();
			it != listen_arena_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_arena_lvl_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_hero_nums_by_lvl(void) {
	for (AA_Info_Listen::iterator it = listen_hero_nums_by_lvl.begin();
			it != listen_hero_nums_by_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->get_hero_nums_by_lvl(cfg->cond_ext, cfg->num);
		if (cfg->num <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_hero_nums_by_lvl_construct();
	return 0;
}

int Anci_Artifact::aa_level_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_level.begin();
			it != aa_level.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = level();
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_level.insert(std::make_pair(*it, &it_map->second));
		}
	}
	save_tick();
	return 0;
}

int Anci_Artifact::aa_pack_grid_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_pack_grid.begin();
			it != aa_pack_grid.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_pack_grid.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_gold_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_gold.begin();
			it != aa_gold.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = Packer::pack_get_money().gold;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_gold.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_suit_purple_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_suit_purple.begin();
			it != aa_suit_purple.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		int nums = player_self()->achievement_count_suit_purple_nums();
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = nums;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_suit_purple.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_suit_gold_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_suit_gold.begin();
			it != aa_suit_gold.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().suit_gold_ids.size();
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_suit_gold.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_rune_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_rune.begin();
			it != aa_rune.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->get_gemstone_count(cfg->cond_ext);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_rune.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_unban_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_unban.begin();
			it != aa_unban.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().unban_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_unban.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_master_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_master.begin();
			it != aa_master.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().master_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_master.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_friendship_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_friendship.begin();
			it != aa_friendship.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = Packer::pack_get_money().friendship;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_friendship.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_force_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_force.begin();
			it != aa_force.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().force_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_force.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_gang_contribute_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_gang_contribute.begin();
			it != aa_gang_contribute.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->ganger_detail().total_contribute;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_gang_contribute.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_recruit_hero_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_recruit_hero.begin();
			it != aa_recruit_hero.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->heroer_detail().hero_map.size();
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_recruit_hero.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_war_kill_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_war_kill_nums.begin();
			it != aa_war_kill_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().war_kill_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_war_kill_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_arena_win_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_arena_win_nums.begin();
			it != aa_arena_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().arena_win_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_arena_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_gang_war_win_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_gang_war_win_nums.begin();
			it != aa_gang_war_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().gang_war_win_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_gang_war_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_exploit_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_exploit_lvl.begin();
			it != aa_exploit_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->public_detail().exploit_lvl;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_exploit_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_first_lvl_role_id_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_first_lvl_role_id.begin();
			it != aa_first_lvl_role_id.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = player_self()->achievement_detail().first_lvl_role_id_his.find(cfg->cond_ext);
//		if (it_fl != player_self()->achievement_detail().first_lvl_role_id_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_role_id_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_role_id_his().end()) {
			continue;
		}
		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_first_lvl_role_id.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_first_lvl_ck_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_first_lvl_ck.begin();
			it != aa_first_lvl_ck.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = player_self()->achievement_detail().first_lvl_ck_his.find(cfg->cond_ext);
//		if (it_fl != player_self()->achievement_detail().first_lvl_ck_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_ck_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_ck_his().end()) {
			continue;
		}
		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_first_lvl_ck.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_first_lvl_zs_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_first_lvl_zs.begin();
			it != aa_first_lvl_zs.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = player_self()->achievement_detail().first_lvl_zs_his.find(cfg->cond_ext);
//		if (it_fl != player_self()->achievement_detail().first_lvl_zs_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_zs_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_zs_his().end()) {
			continue;
		}
		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_first_lvl_zs.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_first_lvl_yss_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_first_lvl_yss.begin();
			it != aa_first_lvl_yss.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = player_self()->achievement_detail().first_lvl_yss_his.find(cfg->cond_ext);
//		if (it_fl != player_self()->achievement_detail().first_lvl_yss_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_yss_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_yss_his().end()) {
			continue;
		}
		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_first_lvl_yss.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_kill_monster_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_kill_monster_nums.begin();
			it != aa_kill_monster_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().kill_monster_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_kill_monster_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_single_dungeon_chapter_complete_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_single_dungeon_chapter_complete.begin();
			it != aa_single_dungeon_chapter_complete.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->sgl_dungeon_detail().get_chapter_status(cfg->cond_ext);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_single_dungeon_chapter_complete.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_team_fb_complete_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_team_fb_complete_nums.begin();
			it != aa_team_fb_complete_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().team_fb_complete_nums_his.count(cfg->cond_ext);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_team_fb_complete_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_horse_get_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_horse_get.begin();
			it != aa_horse_get.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().horse_get_his.count(cfg->cond_ext);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_horse_get.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_recharge_diamond_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_recharge_diamond.begin();
			it != aa_recharge_diamond.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->public_detail().recharge_money_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_recharge_diamond.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_secret_shop_buy_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_secret_shop_buy_nums.begin();
			it != aa_secret_shop_buy_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().secret_shop_buy_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_secret_shop_buy_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_rob_times_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_rob_times.begin();
			it != aa_dragon_wale_rob_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_rob_times_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_rob_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_step_foot_times_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_step_foot_times.begin();
			it != aa_dragon_wale_step_foot_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_step_foot_times_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_step_foot_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_play_dragon_times_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_play_dragon_times.begin();
			it != aa_dragon_wale_play_dragon_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_play_dragon_times_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_play_dragon_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_hero_awake_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_hero_awake_lvl.begin();
			it != aa_hero_awake_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().hero_awake_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_hero_awake_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_task_complete_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_task_complete_nums.begin();
			it != aa_task_complete_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().task_complete_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_task_complete_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_gold_mine_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_gold_mine_lvl.begin();
			it != aa_dragon_wale_gold_mine_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_gold_mine_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_gold_mine_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_moon_well_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_moon_well_lvl.begin();
			it != aa_dragon_wale_moon_well_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_moon_well_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_moon_well_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_totem_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_totem_lvl.begin();
			it != aa_dragon_wale_totem_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_totem_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_totem_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_dragon_wale_drive_thief_times_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_drive_thief_times.begin();
			it != aa_dragon_wale_drive_thief_times.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_drive_thief_times_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_drive_thief_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_war_light_win_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_war_light_win_nums.begin();
			it != aa_war_light_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().war_light_win_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_war_light_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_war_dark_win_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_war_drak_win_nums.begin();
			it != aa_war_drak_win_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().war_dark_win_nums_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_war_dark_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_task_limit_time_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_task_limit_time.begin();
			it != aa_task_limit_time.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		int32_t nums = 0;
		if (player_self()->achievement_detail().task_limit_time_his != -1
				&& player_self()->achievement_detail().task_limit_time_his <= cfg->cond_ext) {
			nums = 1;
		}
		info.reset();
		info.id = *it;
		info.data = nums;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_task_limit_time.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_kill_boss_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_kill_boss.begin();
			it != aa_kill_boss.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().kill_boss_his.count(cfg->cond_ext);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_kill_boss.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Anci_Artifact::aa_horse_get_nums_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_horse_get_nums.begin();
			it != aa_horse_get_nums.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().dragon_wale_keep_level_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_horse_get_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_dragon_wale_keep_level_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_dragon_wale_keep_level.begin();
			it != aa_dragon_wale_keep_level.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->get_mount_num();
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_dragon_wale_keep_level.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_rune_stone_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_rune_stone_lvl.begin();
			it != aa_rune_stone_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().rune_stone_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_rune_stone_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_equip_on_body_both_player_and_hero_by_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_equip_on_body_both_player_and_hero_by_lvl.begin();
			it != aa_equip_on_body_both_player_and_hero_by_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->get_equip_on_body_both_player_and_hero_by_lvl(cfg->cond_ext);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_equip_on_body_both_player_and_hero_by_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_arena_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_arena_lvl.begin();
			it != aa_arena_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->achievement_detail().arena_lvl_his;
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_arena_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Anci_Artifact::aa_hero_nums_by_lvl_construct(void) {
	Anci_Artifact_Cond_Info info;
	for (Int_Hash_Set::iterator it = aa_hero_nums_by_lvl.begin();
			it != aa_hero_nums_by_lvl.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it);
		if (!cfg) {
			continue;
		}
		AA_Info_Map::iterator it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()
			|| this->anci_artifact_detail().complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->get_hero_nums_by_lvl(cfg->cond_ext, cfg->num);
		this->anci_artifact_detail_->aa_info_map.insert(std::make_pair(*it, info));
		it_map = this->anci_artifact_detail_->aa_info_map.find(*it);
		if (it_map != this->anci_artifact_detail_->aa_info_map.end()) {
			listen_hero_nums_by_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
