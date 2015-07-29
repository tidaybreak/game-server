/*
 * Anci_Artifact.cpp
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#include "Logic_Module_Head.h"
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
#include "achievement/Achievement_Struct.h"

Anci_Artifact::Anci_Artifact(void) {
	reset();
}
Anci_Artifact::~Anci_Artifact(void) {}

void Anci_Artifact::reset(void) {
	anci_artifact_detail_ = 0;
	aa_level.clear();
	aa_pack_grid.clear();
	aa_gold.clear();
	aa_suit_purple.clear();
	aa_suit_gold.clear();
	aa_rune.clear();
	aa_unban.clear();
	aa_master.clear();
	aa_friendship.clear();
	aa_force.clear();
	aa_gang_contribute.clear();
	aa_recruit_hero.clear();
	aa_war_kill_nums.clear(); // 战场累计击杀人
	aa_arena_win_nums.clear(); // 竞技场获胜场
	aa_gang_war_win_nums.clear(); // 公会战胜利次
	aa_exploit_lvl.clear();
	aa_first_lvl_role_id.clear(); // 本服务器第一个达到级的玩家
	aa_first_lvl_ck.clear(); // 本服务器第一个达到级的刺客
	aa_first_lvl_zs.clear(); // 本服务器第一个达到级的战士
	aa_first_lvl_yss.clear(); // 本服务器第一个达到级的元素师
	aa_kill_monster_nums.clear(); // 击杀只怪物
	aa_single_dungeon_chapter_complete.clear(); // 通关第章节
	aa_team_fb_complete_nums.clear(); // 完成级组队副本
	aa_horse_get.clear(); // 获得XXX坐骑
	aa_recharge_diamond.clear();
	aa_secret_shop_buy_nums.clear(); // 神秘商店购买件物品
	aa_dragon_wale_rob_times.clear(); // 掠夺成功次
	aa_dragon_wale_step_foot_times.clear(); // 留下脚印次
	aa_dragon_wale_play_dragon_times.clear(); // 逗龙次
	aa_hero_awake_lvl.clear(); // 英雄一级觉醒
	aa_task_complete_nums.clear(); // 完成个任务
	aa_dragon_wale_gold_mine_lvl.clear(); // 金矿等级达到级
	aa_dragon_wale_moon_well_lvl.clear(); // 英灵图腾等级达到级
	aa_dragon_wale_totem_lvl.clear(); // 圣龙祭坛等级达到级
	aa_dragon_wale_drive_thief_times.clear(); // 驱逐骚扰怪次
	aa_war_light_win_nums.clear(); // 在光明阵营获胜场
	aa_war_drak_win_nums.clear(); // 在黑暗阵营获胜场
	aa_task_limit_time.clear(); // 分钟内完成限时击杀任务
	aa_kill_boss.clear(); // 击杀bossXXX
	aa_horse_get_nums.clear();
	aa_dragon_wale_keep_level.clear();
	aa_rune_stone_lvl.clear();
	aa_equip_on_body_both_player_and_hero_by_lvl.clear();
	aa_arena_lvl.clear();
	aa_hero_nums_by_lvl.clear();
	aa_first_boss_against.clear();
	aa_complete_suit.clear();

	listen_level.clear();
	listen_pack_grid.clear();
	listen_gold.clear();
	listen_suit_purple.clear();
	listen_suit_gold.clear();
	listen_rune.clear();
	listen_unban.clear();
	listen_master.clear();
	listen_friendship.clear();
	listen_force.clear();
	listen_gang_contribute.clear();
	listen_recruit_hero.clear();
	listen_war_kill_nums.clear(); // 战场累计击杀人
	listen_arena_win_nums.clear(); // 竞技场获胜场
	listen_gang_war_win_nums.clear(); // 公会战胜利次
	listen_exploit_lvl.clear(); // 功勋等级
	listen_first_lvl_role_id.clear(); // 本服务器第一个达到级的玩家
	listen_first_lvl_ck.clear(); // 本服务器第一个达到级的刺客
	listen_first_lvl_zs.clear(); // 本服务器第一个达到级的战士
	listen_first_lvl_yss.clear(); // 本服务器第一个达到级的元素师
	listen_kill_monster_nums.clear(); // 击杀只怪物
	listen_single_dungeon_chapter_complete.clear(); // 通关第章节
	listen_team_fb_complete_nums.clear(); // 完成级组队副本
	listen_horse_get.clear(); // 获得XXX坐骑
	listen_recharge_diamond.clear();
	listen_secret_shop_buy_nums.clear(); // 神秘商店购买件物品
	listen_dragon_wale_rob_times.clear(); // 掠夺成功次
	listen_dragon_wale_step_foot_times.clear(); // 留下脚印次
	listen_dragon_wale_play_dragon_times.clear(); // 逗龙次
	listen_hero_awake_lvl.clear(); // 英雄一级觉醒
	listen_task_complete_nums.clear(); // 完成个任务
	listen_dragon_wale_gold_mine_lvl.clear(); // 金矿等级达到级
	listen_dragon_wale_moon_well_lvl.clear(); // 英灵图腾等级达到级
	listen_dragon_wale_totem_lvl.clear(); // 圣龙祭坛等级达到级
	listen_dragon_wale_drive_thief_times.clear(); // 驱逐骚扰怪次
	listen_war_light_win_nums.clear(); // 在光明阵营获胜场
	listen_war_dark_win_nums.clear(); // 在黑暗阵营获胜场
	listen_task_limit_time.clear(); // 分钟内完成限时击杀任务
	listen_kill_boss.clear(); // 击杀bossXXX
	listen_horse_get_nums.clear();
	listen_dragon_wale_keep_level.clear();
	listen_rune_stone_lvl.clear();
	listen_equip_on_body_both_player_and_hero_by_lvl.clear(); // 主角、英雄身上x件紫装
	listen_arena_lvl.clear(); // 竞技场达到白银段
	listen_hero_nums_by_lvl.clear(); // 拥有x个x级英雄
	listen_first_boss_against.clear();
	listen_complete_suit.clear();
}

int Anci_Artifact::create_init(Anci_Artifact_Detail &detail) {
	return 0;
}

void Anci_Artifact::module_init(void) {
	aa_refresh_prop();

	Int_Vec erase_vec;
	for (AA_Info_Map::iterator it = this->anci_artifact_detail_->aa_info_map.begin();
			it != this->anci_artifact_detail_->aa_info_map.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond((*it).second.id);
		if (!cfg) {
			erase_vec.push_back((*it).second.id);
			continue;
		}
		switch(cfg->type) {
		case Achievement_Def::AT_LEVEL: {
			it->second.data = level();
			listen_level.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_PACK_GRID: {
			it->second.data = Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX);
			listen_pack_grid.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_GOLD: {
			it->second.data = Packer::pack_get_money().gold;
			listen_gold.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_SUIT_PURPLE: {
			it->second.data = player_self()->achievement_count_suit_purple_nums();
			listen_suit_purple.insert(std::make_pair(it->second.id, &it->second));
//			it->second.data = achievement_count_suit_gold_nums();
//			listen_suit_gold.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_RUNE_LVL: {
			it->second.data = player_self()->get_gemstone_count(cfg->cond_ext);
			listen_rune.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_UNBAN_LVL: {
			it->second.data = player_self()->achievement_detail().unban_lvl_his;
			listen_unban.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_MASTER_LVL: {
			it->second.data = player_self()->achievement_detail().master_lvl_his;
			listen_master.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FRIENDSHIP: {
			it->second.data = Packer::pack_get_money().friendship;
			listen_friendship.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FORCE: {
			it->second.data = player_self()->fighter_detail().force;
			listen_force.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_GANG_CONTRIBUTE: {
			it->second.data = player_self()->ganger_detail().total_contribute;
			listen_gang_contribute.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HERO_NUMS: {
			it->second.data = player_self()->heroer_detail().hero_map.size();
			listen_recruit_hero.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_WAR_KILL_NUMS: {
			it->second.data = player_self()->achievement_detail().war_kill_nums_his;
			listen_war_kill_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_ARENA_WIN_NUMS: {
			it->second.data = player_self()->achievement_detail().arena_win_nums_his;
			listen_arena_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_GANG_WAR_WIN_NUMS: {
			it->second.data = player_self()->achievement_detail().gang_war_win_nums_his;
			listen_gang_war_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_EXPLOIT_LVL: {
			it->second.data = player_self()->public_detail().exploit_lvl;
			listen_exploit_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_ROLE_ID: {
//			it->second.data = player_self()->achievement_detail().first_lvl_role_id_his;
			listen_first_lvl_role_id.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_CK: {
//			it->second.data = player_self()->achievement_detail().first_lvl_ck_his;
			listen_first_lvl_ck.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_ZS: {
//			it->second.data = player_self()->achievement_detail().first_lvl_zs_his;
			listen_first_lvl_zs.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_YSS: {
//			it->second.data = player_self()->achievement_detail().first_lvl_yss_his.count(role_id());
			listen_first_lvl_yss.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_KILL_MONSTER_NUMS: {
			it->second.data = player_self()->achievement_detail().kill_monster_nums_his;
			listen_kill_monster_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_SINGLE_DUNGEON_COMPLETE: {
			it->second.data = player_self()->sgl_dungeon_detail().get_chapter_status(cfg->num);
			listen_single_dungeon_chapter_complete.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_TEAM_FB_COMPLETE_NUMS: {
			it->second.data = player_self()->achievement_detail().team_fb_complete_nums_his.count(cfg->num);
			listen_team_fb_complete_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HORSE_GET: {
			it->second.data = player_self()->achievement_detail().horse_get_his.count(cfg->num);
			listen_horse_get.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_RECHARGE_DIAMOND: {
			it->second.data = static_cast<int32_t>(player_self()->public_detail().recharge_money_his);
			listen_recharge_diamond.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_SECRET_SHOP_BUY_NUMS: {
			it->second.data = player_self()->achievement_detail().secret_shop_buy_nums_his;
			listen_secret_shop_buy_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_ROB_SUCESS_TIMES: {
			it->second.data = player_self()->achievement_detail().dragon_wale_rob_times_his;
			listen_dragon_wale_rob_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_STEP_FOOT_TIMES: {
			it->second.data = player_self()->achievement_detail().dragon_wale_step_foot_times_his;
			listen_dragon_wale_step_foot_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_PLAY_DRAGON_TIMES: {
			it->second.data = player_self()->achievement_detail().dragon_wale_play_dragon_times_his;
			listen_dragon_wale_play_dragon_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HERO_AWAKE_LVL: {
			it->second.data = player_self()->achievement_detail().hero_awake_lvl_his;
			listen_hero_awake_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_TASK_COMPLETE_NUMS: {
			it->second.data = player_self()->achievement_detail().task_complete_nums_his;
			listen_task_complete_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_GOLD_MINE_LVL: {
			it->second.data = player_self()->achievement_detail().dragon_wale_gold_mine_lvl_his;
			listen_dragon_wale_gold_mine_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_MOON_WELL_LVL: {
			it->second.data = player_self()->achievement_detail().dragon_wale_moon_well_lvl_his;
			listen_dragon_wale_moon_well_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_TOTEM_LVL: {
			it->second.data = player_self()->achievement_detail().dragon_wale_totem_lvl_his;
			listen_dragon_wale_totem_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_DRIVE_THIEF_TIMES: {
			it->second.data = player_self()->achievement_detail().dragon_wale_drive_thief_times_his;
			listen_dragon_wale_drive_thief_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_LIGHT_WIN_NUMS: {
			it->second.data = player_self()->achievement_detail().war_light_win_nums_his;
			listen_war_light_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_DRAK_WIN_NUMS: {
			it->second.data = player_self()->achievement_detail().war_dark_win_nums_his;
			listen_war_dark_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_TASK_LIMIT_TIME: {
			if (player_self()->achievement_detail().task_limit_time_his != -1
					&& player_self()->achievement_detail().task_limit_time_his <= cfg->cond_ext) {
				it->second.data = 1;
			}
			listen_task_limit_time.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_KILL_BOSS: {
			it->second.data = player_self()->achievement_detail().kill_boss_his.count(cfg->num);
			listen_kill_boss.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HORSE_GET_NUMS: {
			it->second.data = player_self()->get_mount_num();
			listen_horse_get_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DRAGON_WALE_KEEP_LEVEL: {
			it->second.data = player_self()->achievement_detail().dragon_wale_keep_level_his;
			listen_dragon_wale_keep_level.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_RUNE_STONE_LEVEL: {
			it->second.data = player_self()->achievement_detail().rune_stone_lvl_his;
			listen_rune_stone_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_EQUIP_ON_LVL: {
			it->second.data = player_self()->get_equip_on_body_both_player_and_hero_by_lvl(cfg->cond_ext);
			listen_equip_on_body_both_player_and_hero_by_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_ARENA_LVL: {
			it->second.data = player_self()->achievement_detail().arena_lvl_his;
			listen_arena_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HERO_NUMS_BY_LVL: {
			it->second.data = player_self()->get_hero_nums_by_lvl(cfg->cond_ext, cfg->num);
			listen_hero_nums_by_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_BOSS_AGAINST: {
			it->second.data = player_self()->achievement_detail().first_boss_against;
			listen_first_boss_against.insert(std::make_pair(it->second.id, &it->second));
			break;
			}
		case Achievement_Def::AT_COMPLETE_SUIT: {
			it->second.data = player_self()->achievement_detail().first_boss_against;
			listen_complete_suit.insert(std::make_pair(it->second.id, &it->second));
			break;
			}
		default:
			break;
		}
	}
	for (Int_Vec::const_iterator it = erase_vec.begin(); it != erase_vec.end(); ++it) {
		this->anci_artifact_detail_->aa_info_map.erase(*it);
	}
	aa_construct();
}

void Anci_Artifact::load_detail(Anci_Artifact_Detail &detail) {
	anci_artifact_detail_ = &detail;
}
void Anci_Artifact::save_detail(Anci_Artifact_Detail &detail) {
	if(anci_artifact_detail().is_change) {
		detail = anci_artifact_detail();
		anci_artifact_detail_->is_change = false;
	}
	return ;
}

void Anci_Artifact::sign_in(void) {
	anci_artifact_listen_arena_lvl();
	anci_artifact_listen_unban();
}
int Anci_Artifact::sign_in_repeat(void) {
	return 0;
}

int Anci_Artifact::client_ready_now(void) {
	return 0;
}
void Anci_Artifact::sign_out(void) {
}
int Anci_Artifact::move_scene_completed(void) {
	return 0;
}

//	// 登录或转场景同步逻辑信息到场景
//	int Anci_Artifact::sync_scene_module_info(Block_Buffer &buf);

// 每日0点刷新
int Anci_Artifact::trigger_daily_zero(const Time_Value &now) {
	return 0;
}

//	// 每周0点刷新
//	int Anci_Artifact::trigger_weekly_zero(const Time_Value &now);

//托管中下线调用
void Anci_Artifact::trusteeship_sign_out(void) {
}

int Anci_Artifact::req_anci_artifact_list(void) {
	MSG_50101100 res_msg;
	Anci_Artifact_Info aa_info;
	const AA_Unban_Cond_Cfg &aa_uc_cfg = CONFIG_CACHE_ANCI_ARTIFACT->aa_unban_cond_cfg();
	for (AA_Unban_Cond_Cfg::const_iterator it_unban_cond = aa_uc_cfg.begin();
			it_unban_cond != aa_uc_cfg.end(); ++it_unban_cond) {
		const AA_Unban_Cond &aa_uc = it_unban_cond->second;
		if (!player_self()->is_single_dungeon_complete(aa_uc.num)) {
			continue;
		}
		aa_info.reset();
		aa_info.id = aa_uc.id;
		AA_Unban_Info::const_iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(aa_uc.id);
		if (it_aa_unban_info == this->anci_artifact_detail_->aa_unban_info.end()) {
			Anci_Artifact_Unban_Info aa_ui;
			aa_ui.id = aa_uc.id;
			aa_ui.state = Achievement_Def::CAN_APPLY;
//			aa_ui.box_state = Achievement_Def::CAN_APPLY;
			this->anci_artifact_detail_->aa_unban_info.insert(std::make_pair(aa_ui.id, aa_ui));
			aa_info.state = aa_ui.state;
			aa_info.box_state = aa_ui.box_state;
		} else {
			aa_info.state = it_aa_unban_info->second.state;
			aa_info.box_state = it_aa_unban_info->second.box_state;
		}

		Anci_Artifact_Gem gem_info;
		for (Int_Hash_Set::const_iterator it_gem = aa_uc.gem.begin();
				it_gem != aa_uc.gem.end(); ++it_gem) {

			const AA_Gem *aa_gem = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_gem(*it_gem);
			if (!aa_gem) {
				continue;
			}
			gem_info.reset();
			Int_Int cond;
			bool aa_cond_all_pass = true;
			for (Int_Hash_Set::const_iterator it_cond = aa_gem->cond.begin();
					it_cond != aa_gem->cond.end(); ++it_cond) {
				const AA_Cond *aa_cond = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it_cond);
				if (!aa_cond) {
					aa_cond_all_pass = false;
					continue;
				}
				if (this->anci_artifact_detail_->complete_ids.count(*it_cond) > 0) {
					cond.reset();
					cond.val_1 = *it_cond;
					cond.val_2 = Achievement_Def::ALREADY_APPLY;
					gem_info.cond_list.push_back(cond);
					aa_info.perfect_degree += aa_cond->perfectPoint;
					continue;
				}
				aa_cond_all_pass = false;
				AA_Info_Map::iterator it_aa_info_map = this->anci_artifact_detail_->aa_info_map.find(*it_cond);
				if (it_aa_info_map == this->anci_artifact_detail_->aa_info_map.end()
						|| it_aa_info_map->second.state_count == Achievement_Def::CANNOT_APPLY) {
					continue;
				}
				cond.reset();
				cond.val_1 = *it_cond;
				cond.val_2 = Achievement_Def::CAN_APPLY;
				gem_info.cond_list.push_back(cond);
//				aa_info.perfect_degree += aa_cond->perfectPoint;
			}
			if (!gem_info.cond_list.empty()) {
				gem_info.id = *it_gem;
				if (aa_cond_all_pass) {
					gem_info.state = Achievement_Def::CAN_APPLY;
					aa_info.perfect_degree += aa_gem->perfectPoint;
				}
				aa_info.gem_list.push_back(gem_info);
			}
		}

		res_msg.info.push_back(aa_info);
	}

	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Anci_Artifact::req_anci_artifact_unban(const int id) {
	MSG_50101101 res_msg;
	res_msg.id = id;
	const AA_Unban_Cond *aa_unban_cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_unban_cond(id);
	if (!aa_unban_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	AA_Unban_Info::iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(id);
	if (it_aa_unban_info != this->anci_artifact_detail_->aa_unban_info.end()
			&& it_aa_unban_info->second.state == Achievement_Def::CAN_APPLY) {
		it_aa_unban_info->second.state = Achievement_Def::ALREADY_APPLY;
		res_msg.result = 1;

		if (!aa_unban_cfg->property.empty()) {
			Prop_Setter_Vec setter_vec;
			Prop_Setter setter;
			for (Int_Double_Map::const_iterator it_prop = aa_unban_cfg->property.begin();
					it_prop != aa_unban_cfg->property.end(); ++it_prop) {
				setter.reset();
				setter.prop_type = Property_Type((*it_prop).first);
				setter.operate = O_ADD;
				setter.addi_type = AT_ANCI_ARTIFACT;
				setter.basic = 0.0;
				setter.fixed = (*it_prop).second;
				setter.percent = 0.0;
				setter_vec.push_back(setter);
			}
			player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index);
			player_self()->fight_prop_modify(setter_vec, 206);
		}
		save_tick();
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Anci_Artifact::req_anci_artifact_active(const int id) {
	MSG_50101102 res_msg;
	res_msg.id = id;
	const AA_Cond *aa_cond = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(id);
	if (!aa_cond) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const AA_Unban_Cond *aa_unban_cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_unban_cond(aa_cond->unban_cond_id);
	const AA_Gem *aa_gem = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_gem(aa_cond->gem_id);
	if (!aa_unban_cfg || !aa_gem) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	if (this->anci_artifact_detail_->complete_ids.count(id) > 0) {
		return ERROR_CLIENT_OPERATE;
	}
	AA_Info_Map::iterator it_aa_info_map = this->anci_artifact_detail_->aa_info_map.find(id);
	if (it_aa_info_map == this->anci_artifact_detail_->aa_info_map.end()
			|| it_aa_info_map->second.state_count != Achievement_Def::CAN_APPLY) {
		return ERROR_CLIENT_OPERATE;
	}

	bool need_refresh_hero = false;
	if (!aa_unban_cfg->property.empty()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		for (Int_Double_Map::const_iterator it_prop = aa_unban_cfg->property.begin();
				it_prop != aa_unban_cfg->property.end(); ++it_prop) {
			setter.reset();
			setter.prop_type = Property_Type((*it_prop).first);
			setter.operate = O_ADD;
			setter.addi_type = AT_ANCI_ARTIFACT;
			setter.basic = 0.0;
			setter.fixed = (*it_prop).second * aa_cond->perfectPoint * 0.01;
			setter.percent = 0.0;
			setter_vec.push_back(setter);
		}
		player_self()->fight_prop_modify(setter_vec, 205);
		need_refresh_hero = true;
	}

	this->listen_level.erase(id);
	this->listen_pack_grid.erase(id);
	this->listen_gold.erase(id);
	this->listen_suit_purple.erase(id);
	this->listen_suit_gold.erase(id);
	this->listen_rune.erase(id);
	this->listen_unban.erase(id);
	this->listen_master.erase(id);
	this->listen_friendship.erase(id);
	this->listen_force.erase(id);
	this->listen_gang_contribute.erase(id);
	this->listen_recruit_hero.erase(id);
	this->listen_war_kill_nums.erase(id); // 战场累计击杀人
	this->listen_arena_win_nums.erase(id); // 竞技场获胜场
	this->listen_gang_war_win_nums.erase(id); // 公会战胜利次
	this->listen_exploit_lvl.erase(id);
	this->listen_first_lvl_role_id.erase(id); // 本服务器第一个达到级的玩家
	this->listen_first_lvl_ck.erase(id); // 本服务器第一个达到级的刺客
	this->listen_first_lvl_zs.erase(id); // 本服务器第一个达到级的战士
	this->listen_first_lvl_yss.erase(id); // 本服务器第一个达到级的元素师
	this->listen_kill_monster_nums.erase(id); // 击杀只怪物
	this->listen_single_dungeon_chapter_complete.erase(id); // 通关第章节
	this->listen_team_fb_complete_nums.erase(id); // 完成级组队副本
	this->listen_horse_get.erase(id); // 获得XXX坐骑
	this->listen_recharge_diamond.erase(id);
	this->listen_secret_shop_buy_nums.erase(id); // 神秘商店购买件物品
	this->listen_dragon_wale_rob_times.erase(id); // 掠夺成功次
	this->listen_dragon_wale_step_foot_times.erase(id); // 留下脚印次
	this->listen_dragon_wale_play_dragon_times.erase(id); // 逗龙次
	this->listen_hero_awake_lvl.erase(id); // 英雄一级觉醒
	this->listen_task_complete_nums.erase(id); // 完成个任务
	this->listen_dragon_wale_gold_mine_lvl.erase(id); // 金矿等级达到级
	this->listen_dragon_wale_moon_well_lvl.erase(id); // 英灵图腾等级达到级
	this->listen_dragon_wale_totem_lvl.erase(id); // 圣龙祭坛等级达到级
	this->listen_dragon_wale_drive_thief_times.erase(id); // 驱逐骚扰怪次
	this->listen_war_light_win_nums.erase(id); // 在光明阵营获胜场
	this->listen_war_dark_win_nums.erase(id); // 在黑暗阵营获胜场
	this->listen_task_limit_time.erase(id); // 分钟内完成限时击杀任务
	this->listen_kill_boss.erase(id); // 击杀bossXXX
	this->listen_horse_get_nums.erase(id);
	this->listen_dragon_wale_keep_level.erase(id);
	this->listen_rune_stone_lvl.erase(id);
	this->listen_equip_on_body_both_player_and_hero_by_lvl.erase(id);
	this->listen_arena_lvl.erase(id);
	this->listen_hero_nums_by_lvl.erase(id);
	this->anci_artifact_detail_->aa_info_map.erase(id);
	this->anci_artifact_detail_->complete_ids.insert(id);

	save_tick();
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	anci_artifact_refresh_state(id, need_refresh_hero);

	return 0;
}
int Anci_Artifact::req_anci_artifact_box_get(const int id) {
	MSG_50101103 res_msg;
	res_msg.id = id;
	const AA_Unban_Cond *aa_unban_cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_unban_cond(id);
	if (!aa_unban_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	AA_Unban_Info::iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(id);
	if (it_aa_unban_info != this->anci_artifact_detail_->aa_unban_info.end()
			&& it_aa_unban_info->second.box_state == Achievement_Def::CAN_APPLY) {
		it_aa_unban_info->second.box_state = Achievement_Def::ALREADY_APPLY;
		res_msg.result = 1;

		if (!aa_unban_cfg->award.empty()) {
			MSG_81000102 acv_msg;
			acv_msg.type = 0;
			Money_Add_List mal;
			for (Item_Info_Vec::const_iterator it = aa_unban_cfg->award.begin();
					it != aa_unban_cfg->award.end(); ++it) {
				if ((*it).id < 1000000) {
					if ((*it).amount > 0) {
						mal.push_back(Money_Add_Info(Pack::Money_Type((*it).id), (*it).amount, Money_DM_Info(MONEY_ADD_ANCI_ARTIFACT)));
						acv_msg.property.push_back(Property((*it).id, (*it).amount));
					}
				} else {
					Item_Detail item((*it).id, (*it).amount, Item_Detail::BIND);
					player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
							Gain_Item_DM_Info(Pack::ITEM_GAIN_ANCI_ARTIFACT));
					Item_Basic_Info item_basic;
					item.set_item_basic_info(item_basic);
					acv_msg.item_info.push_back(item_basic);
				}
			}
			if (!mal.empty()) {
				Packer::pack_add_money(mal);
			}
		    if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		    	THIS_SEND_TO_CLIENT(acv_msg);
		    }
		}
		save_tick();
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Anci_Artifact::anci_artifact_listen_unban(const int fb_id) {
	const AA_Unban_Cond_Cfg &aa_uc_cfg = CONFIG_CACHE_ANCI_ARTIFACT->aa_unban_cond_cfg();
	for (AA_Unban_Cond_Cfg::const_iterator it_unban_cond = aa_uc_cfg.begin();
			it_unban_cond != aa_uc_cfg.end(); ++it_unban_cond) {
		const AA_Unban_Cond &aa_uc = it_unban_cond->second;
		if (fb_id != aa_uc.num || !player_self()->is_single_dungeon_complete(aa_uc.num)) {
			continue;
		}
		AA_Unban_Info::iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(aa_uc.id);
		if (it_aa_unban_info == this->anci_artifact_detail_->aa_unban_info.end()) {
			Anci_Artifact_Unban_Info aa_ui;
			aa_ui.id = aa_uc.id;
			aa_ui.state = Achievement_Def::CAN_APPLY;
			this->anci_artifact_detail_->aa_unban_info.insert(std::make_pair(aa_ui.id, aa_ui));
		} else if (it_aa_unban_info->second.state == Achievement_Def::CANNOT_APPLY) {
			it_aa_unban_info->second.state = Achievement_Def::CAN_APPLY;
		}
		req_anci_artifact_list();
		save_tick();
	}
	return 0;
}

void Anci_Artifact::anci_artifact_refresh_state(const int cond_id, const bool need_refresh_hero) {
	const AA_Cond *aa_cond = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(cond_id);
	if (!aa_cond) {
		return ;
	}
	const AA_Unban_Cond *aa_unban_cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_unban_cond(aa_cond->unban_cond_id);
	const AA_Gem *aa_gem_cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_gem(aa_cond->gem_id);
	if (!aa_unban_cfg || !aa_gem_cfg) {
		return ;
	}

	bool all_pass_gem = true;
	for (Int_Hash_Set::const_iterator it_cond = aa_gem_cfg->cond.begin();
			it_cond != aa_gem_cfg->cond.end(); ++it_cond) {
		if (this->anci_artifact_detail_->complete_ids.count(*it_cond) <= 0) {
			all_pass_gem = false;
			break;
		}
	}
	Prop_Setter_Vec setter_vec;
	if (all_pass_gem) {
		if (!aa_unban_cfg->property.empty() && aa_gem_cfg->perfectPoint > 0) {
			Prop_Setter setter;
			for (Int_Double_Map::const_iterator it_prop = aa_unban_cfg->property.begin();
					it_prop != aa_unban_cfg->property.end(); ++it_prop) {
				setter.reset();
				setter.prop_type = Property_Type((*it_prop).first);
				setter.operate = O_ADD;
				setter.addi_type = AT_ANCI_ARTIFACT;
				setter.basic = 0.0;
				setter.fixed = (*it_prop).second * aa_gem_cfg->perfectPoint * 0.01;
				setter.percent = 0.0;
				setter_vec.push_back(setter);
			}
		}
	}

	bool all_pass = true;
	for (Int_Hash_Set::const_iterator it_gem = aa_unban_cfg->gem.begin();
			it_gem != aa_unban_cfg->gem.end(); ++it_gem) {
		const AA_Gem *aa_gem = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_gem(*it_gem);
		if (!aa_gem) {
			all_pass = false;
			break;
		}
		for (Int_Hash_Set::const_iterator it_cond = aa_gem->cond.begin();
				it_cond != aa_gem->cond.end(); ++it_cond) {
			if (this->anci_artifact_detail_->complete_ids.count(*it_cond) <= 0) {
				all_pass = false;
				break;
			}
		}
		if (!all_pass) {
			break;
		}
	}

	if (all_pass) {
		bool need_change = false;
		AA_Unban_Info::iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(aa_cond->unban_cond_id);
		if (it_aa_unban_info == this->anci_artifact_detail_->aa_unban_info.end()) {
			Anci_Artifact_Unban_Info aa_ui;
			aa_ui.id = aa_cond->unban_cond_id;
			aa_ui.box_state = Achievement_Def::CAN_APPLY;
			this->anci_artifact_detail_->aa_unban_info.insert(std::make_pair(aa_ui.id, aa_ui));
			need_change = true;
		} else if (it_aa_unban_info->second.box_state == Achievement_Def::CANNOT_APPLY) {
			it_aa_unban_info->second.box_state = Achievement_Def::CAN_APPLY;
			need_change = true;
		}

		if (need_change && !aa_unban_cfg->extraProperty.empty()) {
			Prop_Setter setter;
			for (Int_Double_Map::const_iterator it_prop = aa_unban_cfg->extraProperty.begin();
					it_prop != aa_unban_cfg->extraProperty.end(); ++it_prop) {
				setter.reset();
				setter.prop_type = Property_Type((*it_prop).first);
				setter.operate = O_ADD;
				setter.addi_type = AT_ANCI_ARTIFACT;
				setter.basic = 0.0;
				setter.fixed = (*it_prop).second;
				setter.percent = 0.0;
				setter_vec.push_back(setter);
			}
		}
	}
	save_tick();
	if (!setter_vec.empty()) {
		player_self()->fight_prop_modify(setter_vec, 204);
		player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index);
	} else if (need_refresh_hero) {
		player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index);
	}
	req_anci_artifact_list();
}

int Anci_Artifact::aa_refresh_prop(void) {
	Addition_Type addi_type = AT_ANCI_ARTIFACT;
	E_FUNC::reset_fight_prop(*player_self(), addi_type);

	Prop_Setter_Vec setter_vec;
	Prop_Setter setter;
	const AA_Unban_Cond_Cfg &aa_uc_cfg = CONFIG_CACHE_ANCI_ARTIFACT->aa_unban_cond_cfg();
	for (AA_Unban_Cond_Cfg::const_iterator it_unban_cond = aa_uc_cfg.begin();
			it_unban_cond != aa_uc_cfg.end(); ++it_unban_cond) {
		const AA_Unban_Cond &aa_uc = it_unban_cond->second;
		if (aa_uc.property.empty()) {
			continue;
		}
		AA_Unban_Info::const_iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(aa_uc.id);
		if (it_aa_unban_info == this->anci_artifact_detail_->aa_unban_info.end()
				|| it_aa_unban_info->second.state != Achievement_Def::ALREADY_APPLY) {
			continue;
		}
		int perfect_degree = 100;

		bool aa_unban_all_pass = true;
		for (Int_Hash_Set::const_iterator it_gem = aa_uc.gem.begin();
				it_gem != aa_uc.gem.end(); ++it_gem) {

			const AA_Gem *aa_gem = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_gem(*it_gem);
			if (!aa_gem) {
				aa_unban_all_pass = false;
				continue;
			}
			bool aa_gem_all_pass = true;
			for (Int_Hash_Set::const_iterator it_cond = aa_gem->cond.begin();
					it_cond != aa_gem->cond.end(); ++it_cond) {
				const AA_Cond *aa_cond = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it_cond);
				if (!aa_cond) {
					aa_gem_all_pass = false;
					continue;
				}
				if (this->anci_artifact_detail_->complete_ids.count(*it_cond) <= 0) {
					aa_gem_all_pass = false;
					continue;
				}
				perfect_degree += aa_cond->perfectPoint;
			}
			if (aa_gem_all_pass) {
				perfect_degree += aa_gem->perfectPoint;
			} else {
				aa_unban_all_pass = false;
			}
		}

		if (!aa_uc.property.empty()) {
			for (Int_Double_Map::const_iterator it_prop = aa_uc.property.begin();
					it_prop != aa_uc.property.end(); ++it_prop) {
				setter.reset();
				setter.prop_type = Property_Type((*it_prop).first);
				setter.operate = O_ADD;
				setter.addi_type = addi_type;
				setter.basic = 0.0;
				setter.fixed = (*it_prop).second * perfect_degree * 0.01;
				setter.percent = 0.0;
				setter_vec.push_back(setter);
			}
		}
		if (aa_unban_all_pass && !aa_uc.extraProperty.empty()) {
			for (Int_Double_Map::const_iterator it_prop = aa_uc.extraProperty.begin();
					it_prop != aa_uc.extraProperty.end(); ++it_prop) {
				setter.reset();
				setter.prop_type = Property_Type((*it_prop).first);
				setter.operate = O_ADD;
				setter.addi_type = addi_type;
				setter.basic = 0.0;
				setter.fixed = (*it_prop).second;
				setter.percent = 0.0;
				setter_vec.push_back(setter);
			}
		}
	}
	if (! setter_vec.empty()) {
		player_self()->fight_prop_modify(setter_vec, 203);
	}
	return 0;
}
void Anci_Artifact::aa_refresh_prop_hero(std::map<int, double> &pro_val_map) {
	Prop_Setter_Vec setter_vec;
	Prop_Setter setter;
	const AA_Unban_Cond_Cfg &aa_uc_cfg = CONFIG_CACHE_ANCI_ARTIFACT->aa_unban_cond_cfg();
	for (AA_Unban_Cond_Cfg::const_iterator it_unban_cond = aa_uc_cfg.begin();
			it_unban_cond != aa_uc_cfg.end(); ++it_unban_cond) {
		const AA_Unban_Cond &aa_uc = it_unban_cond->second;
		if (aa_uc.property.empty()) {
			continue;
		}
		AA_Unban_Info::const_iterator it_aa_unban_info = this->anci_artifact_detail_->aa_unban_info.find(aa_uc.id);
		if (it_aa_unban_info == this->anci_artifact_detail_->aa_unban_info.end()
				|| it_aa_unban_info->second.state != Achievement_Def::ALREADY_APPLY) {
			continue;
		}
		int perfect_degree = 100;

		bool aa_unban_all_pass = true;
		for (Int_Hash_Set::const_iterator it_gem = aa_uc.gem.begin();
				it_gem != aa_uc.gem.end(); ++it_gem) {

			const AA_Gem *aa_gem = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_gem(*it_gem);
			if (!aa_gem) {
				aa_unban_all_pass = false;
				continue;
			}
			bool aa_gem_all_pass = true;
			for (Int_Hash_Set::const_iterator it_cond = aa_gem->cond.begin();
					it_cond != aa_gem->cond.end(); ++it_cond) {
				const AA_Cond *aa_cond = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(*it_cond);
				if (!aa_cond) {
					aa_gem_all_pass = false;
					continue;
				}
				if (this->anci_artifact_detail_->complete_ids.count(*it_cond) <= 0) {
					aa_gem_all_pass = false;
					continue;
				}
				perfect_degree += aa_cond->perfectPoint;
			}
			if (aa_gem_all_pass) {
				perfect_degree += aa_gem->perfectPoint;
			} else {
				aa_unban_all_pass = false;
			}
		}

		if (!aa_uc.property.empty()) {
			for (Int_Double_Map::const_iterator it_prop = aa_uc.property.begin();
					it_prop != aa_uc.property.end(); ++it_prop) {
				pro_val_map[(*it_prop).first] += (*it_prop).second * perfect_degree * 0.01;
			}
		}
		if (aa_unban_all_pass && !aa_uc.extraProperty.empty()) {
			for (Int_Double_Map::const_iterator it_prop = aa_uc.extraProperty.begin();
					it_prop != aa_uc.extraProperty.end(); ++it_prop) {
				pro_val_map[(*it_prop).first] += (*it_prop).second;
			}
		}
	}

}
//int Anci_Artifact::aa_handle_award(const int8_t type, const int32_t id) {
//	MSG_81000102 acv_msg;
//	acv_msg.type = 0;
//	const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(id);
//	if (cfg) {
////		switch(type) {
////		case 0: {
////			if (cfg->num <= 0) {
////				return -1;
////			}
////			if (CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().bind_diamond > 0) {
////				if (0 != Packer::pack_try_add_money(Money_Add_Info(Pack::BIND_GOLD,
////						cfg->num * CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().bind_diamond, MONEY_GAIN_ACHIEVEMENT))) {
////					return -1;
////				}
////				Packer::pack_add_money(Money_Add_Info(Pack::BIND_GOLD,
////						cfg->num * CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().bind_diamond, MONEY_GAIN_ACHIEVEMENT));
////				acv_msg.property.push_back(Property(Pack::BIND_GOLD, cfg->num * CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().bind_diamond));
////			}
////			if (!CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().prop.empty()) {
////				Prop_Setter_Vec setter_vec;
////				Prop_Setter setter;
////				for (Property_Vec::const_iterator it_prop = CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().prop.begin();
////						it_prop != CONFIG_CACHE_ANCI_ARTIFACT->anci_artifact_cfg_setting().prop.end(); ++it_prop) {
////					setter.reset();
////					setter.prop_type = Property_Type((*it_prop).type);
////					setter.operate = O_ADD;
////					setter.addi_type = AT_ACHIEVEMENT;
////					setter.basic = 0.0;
////					setter.fixed = cfg->num * (*it_prop).value;
////					setter.percent = 0.0;
////					setter_vec.push_back(setter);
////				}
////				if (! setter_vec.empty()) {
////					player_self()->fight_prop_modify(setter_vec);
////				}
////			}
////			player_self()->achievement_detail().anci_artifact_count += cfg->num;
////			save_tick();
////			break;
////		}
////		case 1: {
////			if (cfg->award.empty()) {
////				return -1;
////			}
////			for (Item_Info_Vec::const_iterator it = cfg->award.begin();
////					it != cfg->award.end(); ++it) {
////
////				Item_Detail item((*it).id, (*it).amount, static_cast<Item_Detail::Bind_Type>((*it).bind));
////				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
////						Gain_Item_DM_Info(Pack::ITEM_GAIN_CHIEVEMENT_GAME));
////				Item_Basic_Info item_basic;
////				item.set_item_basic_info(item_basic);
////				acv_msg.item_info.push_back(item_basic);
////			}
////			break;
////		}
////		default:
////			break;
////		}
//	    if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
//	    	THIS_SEND_TO_CLIENT(acv_msg);
//	    }
//		return 0;
//	}
//	return -1;
//}
int Anci_Artifact::aa_construct(void) {
	for (AA_Cond_Cfg::const_iterator cfg = CONFIG_CACHE_ANCI_ARTIFACT->aa_cond_cfg().begin();
			cfg != CONFIG_CACHE_ANCI_ARTIFACT->aa_cond_cfg().end(); ++cfg) {
		switch(cfg->second.type) {
			case Achievement_Def::AT_LEVEL: {
				aa_level.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_PACK_GRID: {
				aa_pack_grid.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_GOLD: {
				aa_gold.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_SUIT_PURPLE: {
				aa_suit_purple.insert(cfg->first);
	//			aa_suit_gold.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_RUNE_LVL: {
				aa_rune.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_UNBAN_LVL: {
				aa_unban.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_MASTER_LVL: {
				aa_master.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FRIENDSHIP: {
				aa_friendship.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FORCE: {
				aa_force.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_GANG_CONTRIBUTE: {
				aa_gang_contribute.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HERO_NUMS: {
				aa_recruit_hero.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_WAR_KILL_NUMS: {
				aa_war_kill_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_ARENA_WIN_NUMS: {
				aa_arena_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_GANG_WAR_WIN_NUMS: {
				aa_gang_war_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_EXPLOIT_LVL: {
				aa_exploit_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_ROLE_ID: {
				aa_first_lvl_role_id.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_CK: {
				aa_first_lvl_ck.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_ZS: {
				aa_first_lvl_zs.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_YSS: {
				aa_first_lvl_yss.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_KILL_MONSTER_NUMS: {
				aa_kill_monster_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_SINGLE_DUNGEON_COMPLETE: {
				aa_single_dungeon_chapter_complete.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_TEAM_FB_COMPLETE_NUMS: {
				aa_team_fb_complete_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HORSE_GET: {
				aa_horse_get.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_RECHARGE_DIAMOND: {
				aa_recharge_diamond.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_SECRET_SHOP_BUY_NUMS: {
				aa_secret_shop_buy_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_ROB_SUCESS_TIMES: {
				aa_dragon_wale_rob_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_STEP_FOOT_TIMES: {
				aa_dragon_wale_step_foot_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_PLAY_DRAGON_TIMES: {
				aa_dragon_wale_play_dragon_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HERO_AWAKE_LVL: {
				aa_hero_awake_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_TASK_COMPLETE_NUMS: {
				aa_task_complete_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_GOLD_MINE_LVL: {
				aa_dragon_wale_gold_mine_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_MOON_WELL_LVL: {
				aa_dragon_wale_moon_well_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_TOTEM_LVL: {
				aa_dragon_wale_totem_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_DRIVE_THIEF_TIMES: {
				aa_dragon_wale_drive_thief_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_LIGHT_WIN_NUMS: {
				aa_war_light_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_DRAK_WIN_NUMS: {
				aa_war_drak_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_TASK_LIMIT_TIME: {
				aa_task_limit_time.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_KILL_BOSS: {
				aa_kill_boss.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HORSE_GET_NUMS: {
				aa_horse_get_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DRAGON_WALE_KEEP_LEVEL: {
				aa_dragon_wale_keep_level.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_RUNE_STONE_LEVEL: {
				aa_rune_stone_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_EQUIP_ON_LVL: {
				aa_equip_on_body_both_player_and_hero_by_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_ARENA_LVL: {
				aa_arena_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HERO_NUMS_BY_LVL: {
				aa_hero_nums_by_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_BOSS_AGAINST: {
				aa_first_boss_against.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_COMPLETE_SUIT: {
				aa_complete_suit.insert(cfg->first);
				break;
			}
		default:
			break;
		}
	}
	aa_level_construct();
	aa_pack_grid_construct();
	aa_gold_construct();
	aa_suit_purple_construct();
	aa_suit_gold_construct();
	aa_rune_construct();
	aa_unban_construct();
	aa_master_construct();
	aa_friendship_construct();
	aa_force_construct();
	aa_gang_contribute_construct();
	aa_recruit_hero_construct();
	aa_war_kill_nums_construct(); // 战场累计击杀人
	aa_arena_win_nums_construct(); // 竞技场获胜场
	aa_gang_war_win_nums_construct(); // 公会战胜利次
	aa_exploit_lvl_construct();
	aa_first_lvl_role_id_construct(); // 本服务器第一个达到级的玩家
	aa_first_lvl_ck_construct(); // 本服务器第一个达到级的刺客
	aa_first_lvl_zs_construct(); // 本服务器第一个达到级的战士
	aa_first_lvl_yss_construct(); // 本服务器第一个达到级的元素师
	aa_kill_monster_nums_construct(); // 击杀只怪物
	aa_single_dungeon_chapter_complete_construct();
	aa_team_fb_complete_nums_construct(); // 完成级组队副本
	aa_horse_get_construct(); // 获得XXX坐骑
	aa_recharge_diamond_construct();
	aa_secret_shop_buy_nums_construct(); // 神秘商店购买件物品
	aa_dragon_wale_rob_times_construct(); // 掠夺成功次
	aa_dragon_wale_step_foot_times_construct(); // 留下脚印次
	aa_dragon_wale_play_dragon_times_construct(); // 逗龙次
	aa_hero_awake_lvl_construct(); // 英雄一级觉醒
	aa_task_complete_nums_construct(); // 完成个任务
	aa_dragon_wale_gold_mine_lvl_construct(); // 金矿等级达到级
	aa_dragon_wale_moon_well_lvl_construct(); // 英灵图腾等级达到级
	aa_dragon_wale_totem_lvl_construct(); // 圣龙祭坛等级达到级
	aa_dragon_wale_drive_thief_times_construct(); // 驱逐骚扰怪次
	aa_war_light_win_nums_construct(); // 在光明阵营获胜场
	aa_war_dark_win_nums_construct(); // 在黑暗阵营获胜场
	aa_task_limit_time_construct(); // 分钟内完成限时击杀任务
	aa_kill_boss_construct(); // 击杀bossXXX
	aa_horse_get_nums_construct();
	aa_dragon_wale_keep_level_construct();
	aa_rune_stone_lvl_construct();
	aa_equip_on_body_both_player_and_hero_by_lvl_construct();
	aa_arena_lvl_construct();
	aa_hero_nums_by_lvl_construct();
	return 0;
}

//int Anci_Artifact::aa_announce(const int id) {
//	const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(id);
//	if (!cfg || 0 == cfg->broadcast) {
//		return -1;
//	}
//	player_self()->create_ann_achievement(player_self(), cfg->des, cfg->name,cfg->id);
//	return 0;
//}

int Anci_Artifact::anci_artifact_active_msg(const int32_t id) {
	MSG_81101100 acv_msg;
	AA_Info_Map::const_iterator it = this->anci_artifact_detail_->aa_info_map.find(id);
	if (it != this->anci_artifact_detail_->aa_info_map.end()) {
		Anci_Artifact_Cond info;
		info.id = id;
		info.data = it->second.data;
		info.state = it->second.state_count;
		acv_msg.list.push_back(info);
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}
int Anci_Artifact::anci_artifact_listen_level(const int new_level, const int src_level) {
//	MSG_81100801 acv_msg;
//	acv_msg.op = 1;
//	Achievement_Detail_Msg info;
	for (AA_Info_Listen::iterator it = listen_level.begin(); it != listen_level.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
//		bool need_change = false;
//		if (it->second->data != new_level
//			&& it->second->state_count == 0) {
//			need_change = true;
//		}
		it->second->data = new_level;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->data = cfg->num;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
//		if (need_change) {
//			info.reset();
//			info.id = it->first;
//			info.data = new_level;
//			info.state_award = it->second->state_award;
//			info.state_count = it->second->state_count;
//			acv_msg.list.push_back(info);
//		}
	}
//	if (!acv_msg.list.empty()) {
//		THIS_SEND_TO_CLIENT(acv_msg);
//	}
	aa_level_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_pack_grid(void) {
	for (AA_Info_Listen::iterator it = listen_pack_grid.begin(); it != listen_pack_grid.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX);
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_pack_grid_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_gold(void) {
	for (AA_Info_Listen::iterator it = listen_gold.begin(); it != listen_gold.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = Packer::pack_get_money().gold;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_gold_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_suit_purple(void) {
	int nums = player_self()->achievement_count_suit_purple_nums();
	for (AA_Info_Listen::iterator it = listen_suit_purple.begin(); it != listen_suit_purple.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = nums;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_suit_purple_construct();
	return 0;
}
//int Anci_Artifact::anci_artifact_listen_suit_gold(const int32_t id) {
//	if (!-CONFIG_CACHE_EQUIP->is_suit_part(id)) {
//		return -1;
//	}
//	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(id);
//	if (!cfg || cfg->color < 5) {
//		return -1;
//	}
//	if (player_self()->achievement_detail().suit_gold_ids.count(id) == 0) {
//		player_self()->achievement_detail().suit_gold_ids.insert(id);
//		save_tick();
//
//		int nums = anci_artifact_count_suit_gold_nums();
//		MSG_81100801 acv_msg;
//		acv_msg.op = 1;
//		Achievement_Detail_Msg info;
//		for (AA_Info_Listen::iterator it = listen_suit_gold.begin(); it != listen_suit_gold.end(); ++it) {
//			const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
//			if (!cfg) {
//				continue;
//			}
//			bool need_change = false;
//			if (it->second->data != nums
//				&& it->second->state_count == 0) {
//				need_change = true;
//			}
//			it->second->data = nums;
//			if (cfg->num <= it->second->data && it->second->state_count == 0) {
//				it->second->state_count = Achievement_Def::CAN_APPLY;
//				it->second->state_award = Achievement_Def::CAN_APPLY;
//				anci_artifact_active_msg(it->first);
//			}
//			save_tick();
//			if (need_change) {
//				info.reset();
//				info.id = it->first;
//				info.data = nums;
//				info.state_award = it->second->state_award;
//				info.state_count = it->second->state_count;
//				acv_msg.list.push_back(info);
//			}
//		}
//		if (!acv_msg.list.empty()) {
//			THIS_SEND_TO_CLIENT(acv_msg);
//		}
//	}
//	aa_suit_gold_construct();
//	return 0;
//}
int Anci_Artifact::anci_artifact_listen_rune(void) {
	for (AA_Info_Listen::iterator it = listen_rune.begin(); it != listen_rune.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->get_gemstone_count(cfg->cond_ext);
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_rune_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_unban(void) {
	for (AA_Info_Listen::iterator it = listen_unban.begin(); it != listen_unban.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->achievement_detail().unban_lvl_his;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_unban_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_master(void) {
	for (AA_Info_Listen::iterator it = listen_master.begin(); it != listen_master.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->achievement_detail().master_lvl_his;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_master_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_friendship(void) {
	for (AA_Info_Listen::iterator it = listen_friendship.begin(); it != listen_friendship.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = Packer::pack_get_money().friendship;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_friendship_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_force(void) {
	for (AA_Info_Listen::iterator it = listen_force.begin(); it != listen_force.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->achievement_detail().force_lvl_his;
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_force_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_gang_contribute(void) {
	for (AA_Info_Listen::iterator it = listen_gang_contribute.begin(); it != listen_gang_contribute.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->ganger_detail().achi_contribute;
//		MSG_USER("data: %d, state_count: %d", it->second->data, it->second->state_count);
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_gang_contribute_construct();
	return 0;
}
int Anci_Artifact::anci_artifact_listen_recruit_hero(void) {
	for (AA_Info_Listen::iterator it = listen_recruit_hero.begin(); it != listen_recruit_hero.end(); ++it) {
		const AA_Cond *cfg = CONFIG_CACHE_ANCI_ARTIFACT->find_aa_cond(it->first);
		if (!cfg) {
			continue;
		}
		it->second->data = player_self()->heroer_detail().hero_map.size();
		if (cfg->num <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			anci_artifact_active_msg(it->first);
		}
		save_tick();
	}
	aa_recruit_hero_construct();
	return 0;
}

void Anci_Artifact::save_tick(void) {
	this->anci_artifact_detail_->is_change = true;
}
