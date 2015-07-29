/*
 * Achievement.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: Linqiyou
 */

#include "Msg_Achievement_Struct.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "Achievement_Def.h"
#include "Achievement_Struct.h"
#include "pack/Pack_Struct.h"
#include "pack/Packer.h"
#include "Achievement.h"
#include "Config_Cache_Achievement.h"
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

Achievement::Achievement(void) {
	reset();
}
Achievement::~Achievement(void) {}
void Achievement::reset(void) {
	achievement_detail_ = 0;
	aim_level.clear();
	aim_pack_grid.clear();
	aim_gold.clear();
	aim_suit_purple.clear();
	aim_suit_gold.clear();
	aim_rune.clear();
	aim_unban.clear();
	aim_master.clear();
	aim_friendship.clear();
	aim_force.clear();
	aim_gang_contribute.clear();
	aim_recruit_hero.clear();
	aim_war_kill_nums.clear(); // 战场累计击杀人
	aim_arena_win_nums.clear(); // 竞技场获胜场
	aim_gang_war_win_nums.clear(); // 公会战胜利次
	aim_exploit_lvl.clear();
	aim_first_lvl_role_id.clear(); // 本服务器第一个达到级的玩家
	aim_first_lvl_ck.clear(); // 本服务器第一个达到级的刺客
	aim_first_lvl_zs.clear(); // 本服务器第一个达到级的战士
	aim_first_lvl_yss.clear(); // 本服务器第一个达到级的元素师
	aim_kill_monster_nums.clear(); // 击杀只怪物
	aim_single_dungeon_chapter_complete.clear(); // 通关第章节
	aim_team_fb_complete_nums.clear(); // 完成级组队副本
	aim_horse_get.clear(); // 获得XXX坐骑
	aim_recharge_diamond.clear();
	aim_secret_shop_buy_nums.clear(); // 神秘商店购买件物品
	aim_dragon_wale_rob_times.clear(); // 掠夺成功次
	aim_dragon_wale_step_foot_times.clear(); // 留下脚印次
	aim_dragon_wale_play_dragon_times.clear(); // 逗龙次
	aim_hero_awake_lvl.clear(); // 英雄一级觉醒
	aim_task_complete_nums.clear(); // 完成个任务
	aim_dragon_wale_gold_mine_lvl.clear(); // 金矿等级达到级
	aim_dragon_wale_moon_well_lvl.clear(); // 英灵图腾等级达到级
	aim_dragon_wale_totem_lvl.clear(); // 圣龙祭坛等级达到级
	aim_dragon_wale_drive_thief_times.clear(); // 驱逐骚扰怪次
	aim_war_light_win_nums.clear(); // 在光明阵营获胜场
	aim_war_drak_win_nums.clear(); // 在黑暗阵营获胜场
	aim_task_limit_time.clear(); // 分钟内完成限时击杀任务
	aim_kill_boss.clear(); // 击杀bossXXX
	aim_horse_get_nums.clear();
	aim_dragon_wale_keep_level.clear();
	aim_rune_stone_lvl.clear();
	aim_equip_on_body_both_player_and_hero_by_lvl.clear();
	aim_arena_lvl.clear();
	aim_hero_nums_by_lvl.clear();
	aim_first_boss_against.clear();
	aim_complete_suit.clear();

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
	listen_equip_on_body_both_player_and_hero_by_lvl.clear();
	listen_arena_lvl.clear();
	listen_hero_nums_by_lvl.clear();
	listen_first_boss_against.clear();
	listen_complete_suit.clear(); //集齐套装
}

int Achievement::create_init(Achievement_Detail &detail) {
	return 0;
}
void Achievement::load_detail(Achievement_Detail &detail) {
	achievement_detail_ = &detail;
}
void Achievement::save_detail(Achievement_Detail &detail) {
	if(achievement_detail().is_change) {
		detail = achievement_detail();
		achievement_detail_->is_change = false;
	}
	return ;
}
void Achievement::module_init(void) {
	achievement_refresh_prop();

	Int_Vec erase_vec;
	for (Achievement_Info_Map::iterator it = this->achievement_detail_->achievement_info_map.begin();
			it != this->achievement_detail_->achievement_info_map.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg((*it).second.id);
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
			it->second.data = achievement_count_suit_purple_nums();
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
			it->second.data = this->achievement_detail_->unban_lvl_his;
			listen_unban.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_MASTER_LVL: {
			it->second.data = this->achievement_detail_->master_lvl_his;
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
			it->second.data = player_self()->ganger_detail().achi_contribute;
			listen_gang_contribute.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HERO_NUMS: {
			it->second.data = player_self()->heroer_detail().hero_map.size();
			listen_recruit_hero.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_WAR_KILL_NUMS: {
			it->second.data = this->achievement_detail_->war_kill_nums_his;
			listen_war_kill_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_ARENA_WIN_NUMS: {
			it->second.data = this->achievement_detail_->arena_win_nums_his;
			listen_arena_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_GANG_WAR_WIN_NUMS: {
			it->second.data = this->achievement_detail_->gang_war_win_nums_his;
			listen_gang_war_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_EXPLOIT_LVL: {
			it->second.data = player_self()->public_detail().exploit_lvl;
			listen_exploit_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_ROLE_ID: {
//			it->second.data = this->achievement_detail_->first_lvl_role_id_his;
			listen_first_lvl_role_id.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_CK: {
//			it->second.data = this->achievement_detail_->first_lvl_ck_his;
			listen_first_lvl_ck.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_ZS: {
//			it->second.data = this->achievement_detail_->first_lvl_zs_his;
			listen_first_lvl_zs.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_LVL_YSS: {
//			it->second.data = this->achievement_detail_->first_lvl_yss_his.count(role_id());
			listen_first_lvl_yss.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_KILL_MONSTER_NUMS: {
			it->second.data = this->achievement_detail_->kill_monster_nums_his;
			listen_kill_monster_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_SINGLE_DUNGEON_COMPLETE: {
			it->second.data = player_self()->sgl_dungeon_detail().get_chapter_status(cfg->cond_ext);
			listen_single_dungeon_chapter_complete.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_TEAM_FB_COMPLETE_NUMS: {
			it->second.data = this->achievement_detail_->team_fb_complete_nums_his.count(cfg->cond_ext);
			listen_team_fb_complete_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HORSE_GET: {
			it->second.data = this->achievement_detail_->horse_get_his.count(cfg->cond_ext);
			listen_horse_get.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_RECHARGE_DIAMOND: {
			it->second.data = static_cast<int32_t>(player_self()->public_detail().recharge_money_his);
			listen_recharge_diamond.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_SECRET_SHOP_BUY_NUMS: {
			it->second.data = this->achievement_detail_->secret_shop_buy_nums_his;
			listen_secret_shop_buy_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_ROB_SUCESS_TIMES: {
			it->second.data = this->achievement_detail_->dragon_wale_rob_times_his;
			listen_dragon_wale_rob_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_STEP_FOOT_TIMES: {
			it->second.data = this->achievement_detail_->dragon_wale_step_foot_times_his;
			listen_dragon_wale_step_foot_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_PLAY_DRAGON_TIMES: {
			it->second.data = this->achievement_detail_->dragon_wale_play_dragon_times_his;
			listen_dragon_wale_play_dragon_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HERO_AWAKE_LVL: {
			it->second.data = this->achievement_detail_->hero_awake_lvl_his;
			listen_hero_awake_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_TASK_COMPLETE_NUMS: {
			it->second.data = this->achievement_detail_->task_complete_nums_his;
			listen_task_complete_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_GOLD_MINE_LVL: {
			it->second.data = this->achievement_detail_->dragon_wale_gold_mine_lvl_his;
			listen_dragon_wale_gold_mine_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_MOON_WELL_LVL: {
			it->second.data = this->achievement_detail_->dragon_wale_moon_well_lvl_his;
			listen_dragon_wale_moon_well_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_TOTEM_LVL: {
			it->second.data = this->achievement_detail_->dragon_wale_totem_lvl_his;
			listen_dragon_wale_totem_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_DRIVE_THIEF_TIMES: {
			it->second.data = this->achievement_detail_->dragon_wale_drive_thief_times_his;
			listen_dragon_wale_drive_thief_times.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_LIGHT_WIN_NUMS: {
			it->second.data = this->achievement_detail_->war_light_win_nums_his;
			listen_war_light_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DW_DRAK_WIN_NUMS: {
			it->second.data = this->achievement_detail_->war_dark_win_nums_his;
			listen_war_dark_win_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_TASK_LIMIT_TIME: {
			if (this->achievement_detail_->task_limit_time_his != -1
					&& this->achievement_detail_->task_limit_time_his <= cfg->cond_ext) {
				it->second.data = 1;
			}
			listen_task_limit_time.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_KILL_BOSS: {
			it->second.data = this->achievement_detail_->kill_boss_his.count(cfg->cond_ext);
			listen_kill_boss.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HORSE_GET_NUMS: {
			it->second.data = player_self()->get_mount_num();
			listen_horse_get_nums.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_DRAGON_WALE_KEEP_LEVEL: {
			it->second.data = this->achievement_detail_->dragon_wale_keep_level_his;
			listen_dragon_wale_keep_level.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_RUNE_STONE_LEVEL: {
			it->second.data = this->achievement_detail_->rune_stone_lvl_his;
			listen_rune_stone_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_EQUIP_ON_LVL: {
			it->second.data = get_equip_on_body_both_player_and_hero_by_lvl(cfg->cond_ext);
			listen_equip_on_body_both_player_and_hero_by_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_ARENA_LVL: {
			it->second.data = this->achievement_detail_->arena_lvl_his;
			listen_arena_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_HERO_NUMS_BY_LVL: {
			it->second.data = get_hero_nums_by_lvl(cfg->cond_ext, cfg->cond);
			listen_hero_nums_by_lvl.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_FIRST_BOSS_AGAINST: {
			it->second.data = this->achievement_detail_->first_boss_against;
			listen_first_boss_against.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		case Achievement_Def::AT_COMPLETE_SUIT: {
			it->second.data = get_equip_nums(cfg->cond_ext1);;
			listen_complete_suit.insert(std::make_pair(it->second.id, &it->second));
			break;
		}
		default:
			break;
		}
	}
	for (Int_Vec::const_iterator it = erase_vec.begin(); it != erase_vec.end(); ++it) {
		this->achievement_detail_->achievement_info_map.erase(*it);
	}
	this->achievement_construct();
	player_self()->achievement_listen_exploit_lvl();
}
void Achievement::sign_in(void) {
}
void Achievement::sign_in_repeat(void) {
}
void Achievement::sign_out(void) {
}
void Achievement::client_ready_now(void) {
}
// 场景加载完成
int Achievement::move_scene_completed(void) {
	return 0;
}
//托管中下线调用
void Achievement::trusteeship_sign_out(void) {
}

//msg
int Achievement::req_achievement_list(void) {
	MSG_50100800 res_msg;
	Achievement_Detail_Msg info;
	for (Int_Hash_Set::const_iterator it = this->achievement_detail_->complete_ids.begin();
			it != this->achievement_detail_->complete_ids.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (cfg) {
			info.reset();
			info.id = *it;
			info.state_award = 2;
			info.state_count = 2;
			info.data = cfg->cond;
			res_msg.list.push_back(info);
		}
	}
	for (Achievement_Info_Map::const_iterator it = this->achievement_detail_->achievement_info_map.begin();
			it != this->achievement_detail_->achievement_info_map.end(); ++it) {
		if (0 == (*it).second.data) {
			continue;
		}
		info.reset();
		info.id = (*it).second.id;
		info.state_count = (*it).second.state_count;
		info.state_award = (*it).second.state_award;
		if (it->second.state_count == Achievement_Def::CANNOT_APPLY) {
			info.data = (*it).second.data;
		} else {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (cfg) {
				info.data = cfg->cond;
			} else {
				info.data = (*it).second.data;
			}
		}
		res_msg.list.push_back(info);
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Achievement::req_achievement_get(const int8_t type, const int32_t id) {
	MSG_50100801 res_msg;
	res_msg.type = type;
	res_msg.id = id;
	Achievement_Info_Map::iterator it = this->achievement_detail_->achievement_info_map.find(id);
	if (it != this->achievement_detail_->achievement_info_map.end()
			&& this->achievement_detail_->complete_ids.count(id) == 0) {
		if ((it->second.state_award != Achievement_Def::CAN_APPLY && type == 1)
				|| (it->second.state_count != Achievement_Def::CAN_APPLY && type == 0)) {
			return ERROR_ACHIEVEMENT_HAS_GET;
		}
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(id);
		if (cfg) {
//			if ((*it).second.data >= cfg->cond) {
				if (0 == achievement_handle_award(type, id)) {
					if (type == 1) {
						it->second.state_award = Achievement_Def::ALREADY_APPLY;
					} else if (type == 0) {
						it->second.state_count = Achievement_Def::ALREADY_APPLY;
					}
					if (cfg->award.empty()) {
						it->second.state_award = Achievement_Def::ALREADY_APPLY;
					}
					if (it->second.state_count == Achievement_Def::ALREADY_APPLY
							&& it->second.state_award == Achievement_Def::ALREADY_APPLY) {
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
						this->listen_first_boss_against.erase(id);
						this->listen_complete_suit.erase(id); // 集齐套装
						this->achievement_detail_->complete_ids.insert(id);
						this->achievement_detail_->achievement_info_map.erase(it);
					}
					save_tick();
					res_msg.result = 1;
				}
//			}
		}
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Achievement::req_achievement_medal_onoff(const int32_t source_index, const int32_t dst_index) {
	Item_Detail *medal_from = this->pack_get_item(source_index);
	if (! medal_from) {
		return ERROR_ITEM_NOT_EXIST;
	}
	const Achievement_Cfg_Medal_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_medal(medal_from->id_);
	if (!cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	MSG_50100802 res_msg;
	/* 脱下装备 */
	if (0 == dst_index) {//pack_is_pack_type(dst_index, Pack::PACK_T_PACKAGE_INDEX) == 0) {
		int result = this->pack_move_item(source_index, Pack::PACK_T_PACKAGE_INDEX);
		if (result != 0) {
			return result;
		}
		//排行榜监听
//		RANKING_MANAGER->monitor_equipment_off(player_self()->role_id(), item_id, item_index);
	/* 穿上装备 */
	} else if (//pack_is_pack_type(source_index, Pack::PACK_T_PACKAGE_INDEX) == 0 &&
			pack_is_pack_type(dst_index, Pack::PACK_T_ACHIEVEMENT_INDEX) == 0) {
		const Item_Detail_Config *equip_from_cfg = CONFIG_CACHE_ITEM->find_item(medal_from->id_);
		if (! equip_from_cfg) {
			MSG_USER("item config error:%d", medal_from->id_);
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (pack_calc_item_by_same_type(Pack::PACK_T_ACHIEVEMENT_INDEX, medal_from->id_) > 0) {
			return ERROR_ACHIEVEMENT_ON_SAME;
		}

		int result = this->pack_move_item(source_index, dst_index);
		if (result != 0) {
			return result;
		}
		res_msg.onoff = 1;
		medal_from->bind_ = Item_Detail::BIND;

//		if (equip_from->bind_ != Item_Detail::BIND) {
//			equip_from->bind_ = Item_Detail::BIND;
//			MSG_82100000 item_msg;
//			Item_Basic_Info item_basic;
//			equip_from->set_item_basic_info(item_basic);
//			item_msg.item_info.push_back(item_basic);
//			THIS_SEND_TO_CLIENT(item_msg);
//		}
//		//排行榜监听
//		RANKING_MANAGER->monitor_equipment_change(player_self()->role_id(), equip_from);
	} else {
		return ERROR_CLIENT_OPERATE;
	}
	achievement_refresh_medal_times(cfg->type);

	// refresh prop
//	equiper_prop_refresh();

	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}
int Achievement::achievement_refresh_prop(void) {
	Addition_Type addi_type = AT_ACHIEVEMENT;
	E_FUNC::reset_fight_prop(*player_self(), addi_type);

	if (!CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().prop.empty()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		for (Property_Vec::const_iterator it_prop = CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().prop.begin();
				it_prop != CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().prop.end(); ++it_prop) {
			setter.reset();
			setter.prop_type = Property_Type((*it_prop).type);
			setter.operate = O_ADD;
			setter.addi_type = AT_ACHIEVEMENT;
			setter.basic = 0.0;
			setter.fixed = this->achievement_detail_->achievement_count * (*it_prop).value;
			setter.percent = 0.0;
			setter_vec.push_back(setter);
		}
		if (! setter_vec.empty()) {
			player_self()->fight_prop_modify(setter_vec, 202);
		}
	}
//	Prop_Setter_Vec setter_vec;
//	Prop_Setter setter;
//	setter.reset();
//	setter.prop_type = PT_STAMINA;
//	setter.operate = O_SET;
//	setter.addi_type = AT_ACHIEVEMENT;
//	setter.basic = 0;
//	setter.fixed = 0.0;
//	setter.percent = 0.0;
//	if (CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().armor > 0) {
//		setter.fixed = this->achievement_detail_->achievement_count * CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().armor;
//	}
//	setter_vec.push_back(setter);
//	if (! setter_vec.empty()) {
//		player_self()->fight_prop_modify(setter_vec);
//	}
	return 0;
}
int Achievement::achievement_handle_award(const int8_t type, const int32_t id) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(id);
	if (cfg) {
		switch(type) {
		case 0: {
			if (cfg->num <= 0) {
				return -1;
			}
			if (CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().bind_diamond > 0) {
				if (0 != Packer::pack_try_add_money(Money_Add_Info(Pack::BIND_GOLD,
						cfg->num * CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().bind_diamond, MONEY_GAIN_ACHIEVEMENT))) {
					return -1;
				}
				Packer::pack_add_money(Money_Add_Info(Pack::BIND_GOLD,
						cfg->num * CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().bind_diamond, MONEY_GAIN_ACHIEVEMENT));
				acv_msg.property.push_back(Property(Pack::BIND_GOLD, cfg->num * CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().bind_diamond));
			}
			if (!CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().prop.empty()) {
				Prop_Setter_Vec setter_vec;
				Prop_Setter setter;
				for (Property_Vec::const_iterator it_prop = CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().prop.begin();
						it_prop != CONFIG_CACHE_ACHIEVEMENT->achievement_cfg_setting().prop.end(); ++it_prop) {
					setter.reset();
					setter.prop_type = Property_Type((*it_prop).type);
					setter.operate = O_ADD;
					setter.addi_type = AT_ACHIEVEMENT;
					setter.basic = 0.0;
					setter.fixed = cfg->num * (*it_prop).value;
					setter.percent = 0.0;
					setter_vec.push_back(setter);
				}
				if (! setter_vec.empty()) {
					player_self()->fight_prop_modify(setter_vec, 201);
				}
			}
			this->achievement_detail_->achievement_count += cfg->num;
			save_tick();
			break;
		}
		case 1: {
			if (cfg->award.empty()) {
				return -1;
			}
			for (Item_Info_Vec::const_iterator it = cfg->award.begin();
					it != cfg->award.end(); ++it) {

				Item_Detail item((*it).id, (*it).amount, static_cast<Item_Detail::Bind_Type>((*it).bind));
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
						Gain_Item_DM_Info(Pack::ITEM_GAIN_CHIEVEMENT_GAME));
				Item_Basic_Info item_basic;
				item.set_item_basic_info(item_basic);
				acv_msg.item_info.push_back(item_basic);
			}
			break;
		}
		default:
			break;
		}
	    if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
	    	THIS_SEND_TO_CLIENT(acv_msg);
	    }
		return 0;
	}
	return -1;
}
int Achievement::achievement_construct(void) {
	for (Achievement_Cfg_Detail_Map::const_iterator cfg = CONFIG_CACHE_ACHIEVEMENT->achievement_cfg().begin();
			cfg != CONFIG_CACHE_ACHIEVEMENT->achievement_cfg().end(); ++cfg) {
//		Achievement_Info_Map::iterator it = this->achievement_detail_->achievement_info_map.find(cfg->first);
//		if (it != this->achievement_detail_->achievement_info_map.end()
//				|| this->achievement_detail_->complete_ids.count(cfg->first) > 0) {
//			continue;
//		}
		switch(cfg->second.type) {
			case Achievement_Def::AT_LEVEL: {
				aim_level.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_PACK_GRID: {
				aim_pack_grid.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_GOLD: {
				aim_gold.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_SUIT_PURPLE: {
				aim_suit_purple.insert(cfg->first);
	//			aim_suit_gold.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_RUNE_LVL: {
				aim_rune.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_UNBAN_LVL: {
				aim_unban.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_MASTER_LVL: {
				aim_master.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FRIENDSHIP: {
				aim_friendship.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FORCE: {
				aim_force.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_GANG_CONTRIBUTE: {
				aim_gang_contribute.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HERO_NUMS: {
				aim_recruit_hero.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_WAR_KILL_NUMS: {
				aim_war_kill_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_ARENA_WIN_NUMS: {
				aim_arena_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_GANG_WAR_WIN_NUMS: {
				aim_gang_war_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_EXPLOIT_LVL: {
				aim_exploit_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_ROLE_ID: {
				aim_first_lvl_role_id.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_CK: {
				aim_first_lvl_ck.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_ZS: {
				aim_first_lvl_zs.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_LVL_YSS: {
				aim_first_lvl_yss.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_KILL_MONSTER_NUMS: {
				aim_kill_monster_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_SINGLE_DUNGEON_COMPLETE: {
				aim_single_dungeon_chapter_complete.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_TEAM_FB_COMPLETE_NUMS: {
				aim_team_fb_complete_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HORSE_GET: {
				aim_horse_get.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_RECHARGE_DIAMOND: {
				aim_recharge_diamond.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_SECRET_SHOP_BUY_NUMS: {
				aim_secret_shop_buy_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_ROB_SUCESS_TIMES: {
				aim_dragon_wale_rob_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_STEP_FOOT_TIMES: {
				aim_dragon_wale_step_foot_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_PLAY_DRAGON_TIMES: {
				aim_dragon_wale_play_dragon_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HERO_AWAKE_LVL: {
				aim_hero_awake_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_TASK_COMPLETE_NUMS: {
				aim_task_complete_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_GOLD_MINE_LVL: {
				aim_dragon_wale_gold_mine_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_MOON_WELL_LVL: {
				aim_dragon_wale_moon_well_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_TOTEM_LVL: {
				aim_dragon_wale_totem_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_DRIVE_THIEF_TIMES: {
				aim_dragon_wale_drive_thief_times.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_LIGHT_WIN_NUMS: {
				aim_war_light_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DW_DRAK_WIN_NUMS: {
				aim_war_drak_win_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_TASK_LIMIT_TIME: {
				aim_task_limit_time.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_KILL_BOSS: {
				aim_kill_boss.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HORSE_GET_NUMS: {
				aim_horse_get_nums.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_DRAGON_WALE_KEEP_LEVEL: {
				aim_dragon_wale_keep_level.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_RUNE_STONE_LEVEL: {
				aim_rune_stone_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_EQUIP_ON_LVL: {
				aim_equip_on_body_both_player_and_hero_by_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_ARENA_LVL: {
				aim_arena_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_HERO_NUMS_BY_LVL: {
				aim_hero_nums_by_lvl.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_FIRST_BOSS_AGAINST: {
				aim_first_boss_against.insert(cfg->first);
				break;
			}
			case Achievement_Def::AT_COMPLETE_SUIT: {
				aim_complete_suit.insert(cfg->first);
				break;
			}
		default:
			break;
		}
	}
	achievement_level_construct();
	achievement_pack_grid_construct();
	achievement_gold_construct();
	achievement_suit_purple_construct();
	achievement_suit_gold_construct();
	achievement_rune_construct();
	achievement_unban_construct();
	achievement_master_construct();
	achievement_friendship_construct();
	achievement_force_construct();
	achievement_gang_contribute_construct();
	achievement_recruit_hero_construct();
	achievement_war_kill_nums_construct(); // 战场累计击杀人
	achievement_arena_win_nums_construct(); // 竞技场获胜场
	achievement_gang_war_win_nums_construct(); // 公会战胜利次
	achievement_exploit_lvl_construct();
	achievement_first_lvl_role_id_construct(); // 本服务器第一个达到级的玩家
	achievement_first_lvl_ck_construct(); // 本服务器第一个达到级的刺客
	achievement_first_lvl_zs_construct(); // 本服务器第一个达到级的战士
	achievement_first_lvl_yss_construct(); // 本服务器第一个达到级的元素师
	achievement_kill_monster_nums_construct(); // 击杀只怪物
	achievement_single_dungeon_chapter_complete_construct();
	achievement_team_fb_complete_nums_construct(); // 完成级组队副本
	achievement_horse_get_construct(); // 获得XXX坐骑
	achievement_recharge_diamond_construct();
	achievement_secret_shop_buy_nums_construct(); // 神秘商店购买件物品
	achievement_dragon_wale_rob_times_construct(); // 掠夺成功次
	achievement_dragon_wale_step_foot_times_construct(); // 留下脚印次
	achievement_dragon_wale_play_dragon_times_construct(); // 逗龙次
	achievement_hero_awake_lvl_construct(); // 英雄一级觉醒
	achievement_task_complete_nums_construct(); // 完成个任务
	achievement_dragon_wale_gold_mine_lvl_construct(); // 金矿等级达到级
	achievement_dragon_wale_moon_well_lvl_construct(); // 英灵图腾等级达到级
	achievement_dragon_wale_totem_lvl_construct(); // 圣龙祭坛等级达到级
	achievement_dragon_wale_drive_thief_times_construct(); // 驱逐骚扰怪次
	achievement_war_light_win_nums_construct(); // 在光明阵营获胜场
	achievement_war_dark_win_nums_construct(); // 在黑暗阵营获胜场
	achievement_task_limit_time_construct(); // 分钟内完成限时击杀任务
	achievement_kill_boss_construct(); // 击杀bossXXX
	achievement_horse_get_nums_construct();
	achievement_dragon_wale_keep_level_construct();
	achievement_rune_stone_lvl_construct();
	achievement_equip_on_body_both_player_and_hero_by_lvl_construct();
	achievement_arena_lvl_construct();
	achievement_hero_nums_by_lvl_construct();
	achievement_first_boss_against_construct();
	achievement_complete_suit_construct(); //集齐套装
	return 0;
}

int Achievement::achievement_announce(const int id) {
	const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(id);
	if (!cfg || 0 == cfg->broadcast) {
		return -1;
	}
	player_self()->create_ann_achievement(player_self(), cfg->des, cfg->name, id);
	return 0;
}

int Achievement::achievement_active_msg(const int32_t id) {
	MSG_81100800 acv_msg;
	Achievement_Info_Map::const_iterator it = this->achievement_detail_->achievement_info_map.find(id);
	if (it != this->achievement_detail_->achievement_info_map.end()) {
		Achievement_Detail_Msg info;
		info.id = id;
		info.data = it->second.data;
		info.state_count = it->second.state_count;
		info.state_award = it->second.state_award;
		acv_msg.list.push_back(info);
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}
int Achievement::achievement_listen_level(const int new_level, const int src_level) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_level.begin(); it != listen_level.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != new_level
			&& it->second->state_count == 0) {
			need_change = true;
		}
		it->second->data = new_level;
		if (cfg->cond <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			it->second->data = cfg->cond;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		save_tick();
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = new_level;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_level_construct();
	player_self()->anci_artifact_listen_level(new_level, src_level);
	return 0;
}
int Achievement::achievement_listen_pack_grid(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_pack_grid.begin(); it != listen_pack_grid.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX)
			&& it->second->state_count == 0) {
			need_change = true;
		}
		it->second->data = Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX);
		if (cfg->cond <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		save_tick();
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX);
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_pack_grid_construct();
	player_self()->anci_artifact_listen_pack_grid();
	return 0;
}
int Achievement::achievement_listen_gold(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_gold.begin(); it != listen_gold.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != Packer::pack_get_money().gold
			&& it->second->state_count == 0) {
			need_change = true;
		}
		it->second->data = Packer::pack_get_money().gold;
		if (cfg->cond <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		save_tick();
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = Packer::pack_get_money().gold;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_gold_construct();
	player_self()->anci_artifact_listen_gold();
	return 0;
}
int Achievement::achievement_listen_suit_purple(const int32_t id) {
	if (!CONFIG_CACHE_EQUIP->is_suit_part(id)) {
		return -1;
	}
	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(id);
	if (!cfg || cfg->color < 4) {
		return -1;
	}
	if (this->achievement_detail_->suit_purple_ids.count(id) == 0) {
		this->achievement_detail_->suit_purple_ids.insert(id);
		save_tick();

		int nums = achievement_count_suit_purple_nums();
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_suit_purple.begin(); it != listen_suit_purple.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		player_self()->anci_artifact_listen_suit_purple();
	}
	achievement_suit_purple_construct();
	return 0;
}
int Achievement::achievement_listen_suit_gold(const int32_t id) {
	if (!CONFIG_CACHE_EQUIP->is_suit_part(id)) {
		return -1;
	}
	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(id);
	if (!cfg || cfg->color < 5) {
		return -1;
	}
	if (this->achievement_detail_->suit_gold_ids.count(id) == 0) {
		this->achievement_detail_->suit_gold_ids.insert(id);
		save_tick();

		int nums = achievement_count_suit_gold_nums();
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_suit_gold.begin(); it != listen_suit_gold.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
//		player_self()->anci_artifact_listen_suit_gold();
	}
	achievement_suit_gold_construct();
	return 0;
}
int Achievement::achievement_listen_rune(void) {
//	if (this->achievement_detail_->rune_lvl_his < lvl) {
//		this->achievement_detail_->rune_lvl_his = lvl;
//		save_tick();

		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_rune.begin(); it != listen_rune.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			int nums = player_self()->get_gemstone_count(cfg->cond_ext);
			if (it->second->data != nums
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = it->second->data;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_rune_construct();
		player_self()->anci_artifact_listen_rune();
//	}
	return 0;
}
int Achievement::achievement_listen_unban(const int32_t lvl) {
	if (this->achievement_detail_->unban_lvl_his < lvl) {
		this->achievement_detail_->unban_lvl_his = lvl;
		save_tick();

		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_unban.begin(); it != listen_unban.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != this->achievement_detail_->unban_lvl_his
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = this->achievement_detail_->unban_lvl_his;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = this->achievement_detail_->unban_lvl_his;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_unban_construct();
		player_self()->anci_artifact_listen_unban();
	}
	return 0;
}
int Achievement::achievement_listen_master(const int32_t lvl) {
	if (this->achievement_detail_->master_lvl_his < lvl) {
		this->achievement_detail_->master_lvl_his = lvl;
		save_tick();

		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_master.begin(); it != listen_master.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != this->achievement_detail_->master_lvl_his
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = this->achievement_detail_->master_lvl_his;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = this->achievement_detail_->master_lvl_his;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_master_construct();
		player_self()->anci_artifact_listen_master();
	}
	return 0;
}
int Achievement::achievement_listen_friendship(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_friendship.begin(); it != listen_friendship.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != Packer::pack_get_money().friendship
			&& it->second->state_count == 0) {
			need_change = true;
		}
		it->second->data = Packer::pack_get_money().friendship;
		if (cfg->cond <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		save_tick();
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = Packer::pack_get_money().friendship;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_friendship_construct();
	player_self()->anci_artifact_listen_friendship();
	return 0;
}
int Achievement::achievement_listen_force(const int32_t force) {
	if (this->achievement_detail_->force_lvl_his < force) {
		this->achievement_detail_->force_lvl_his = force;
		save_tick();

		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_force.begin(); it != listen_force.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != this->achievement_detail_->force_lvl_his
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = this->achievement_detail_->force_lvl_his;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = this->achievement_detail_->force_lvl_his;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_force_construct();
		player_self()->anci_artifact_listen_force();
	}
	return 0;
}
int Achievement::achievement_listen_gang_contribute(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_gang_contribute.begin(); it != listen_gang_contribute.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (static_cast<uint32_t>(it->second->data) != player_self()->ganger_detail().achi_contribute
			&& it->second->state_count == 0) {
			need_change = true;
		}
		it->second->data = player_self()->ganger_detail().achi_contribute;
		if (cfg->cond <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		save_tick();
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = player_self()->ganger_detail().achi_contribute;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_gang_contribute_construct();
	player_self()->anci_artifact_listen_gang_contribute();
	return 0;
}
int Achievement::achievement_listen_recruit_hero(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_recruit_hero.begin(); it != listen_recruit_hero.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (static_cast<uint32_t>(it->second->data) != player_self()->heroer_detail().hero_map.size()
			&& it->second->state_count == 0) {
			need_change = true;
		}
		it->second->data = player_self()->heroer_detail().hero_map.size();
		if (cfg->cond <= it->second->data && it->second->state_count == 0) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		save_tick();
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = player_self()->heroer_detail().hero_map.size();
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_recruit_hero_construct();
	player_self()->anci_artifact_listen_recruit_hero();
	achievement_listen_hero_nums_by_lvl();
	return 0;
}

int Achievement::achievement_count_suit_purple_nums(void) {
	int nums = 0;
	for (Suit_ID_Cfg_Vec::const_iterator it = CONFIG_CACHE_EQUIP->suit_id_cfg_vec().begin();
			it != CONFIG_CACHE_EQUIP->suit_id_cfg_vec().end(); ++it) {
		bool complete = true;
		const Suit_ID_Cfg &cfg = *it;
		if (cfg.item_ids.empty()) {
			continue;
		}
		for( Int_Vec::const_iterator it_item = cfg.item_ids.begin();
				it_item != cfg.item_ids.end(); ++it_item) {
			if (0 == this->achievement_detail_->suit_purple_ids.count(*it_item)) {
				complete = false;
				break;
			}
		}
		if (complete) {
			++nums;
		}
	}
	return nums;
}
int Achievement::achievement_count_suit_gold_nums(void) {
	int nums = 0;
	for (Suit_ID_Cfg_Vec::const_iterator it = CONFIG_CACHE_EQUIP->suit_id_cfg_vec().begin();
			it != CONFIG_CACHE_EQUIP->suit_id_cfg_vec().end(); ++it) {
		bool complete = true;
		const Suit_ID_Cfg &cfg = *it;
		if (cfg.item_ids.empty()) {
			continue;
		}
		for( Int_Vec::const_iterator it_item = cfg.item_ids.begin();
				it_item != cfg.item_ids.end(); ++it_item) {
			if (0 == this->achievement_detail_->suit_gold_ids.count(*it_item)) {
				complete = false;
				break;
			}
		}
		if (complete) {
			++nums;
		}
	}
	return nums;
}

int Achievement::get_equip_on_body_both_player_and_hero_by_lvl(const int lvl) {
	int nums = 0;

	Item_Detail *equip = NULL;
	const Item_Detail_Config *item_config = NULL;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		if (index >= EQUIP_IDX_SUIT_END) {
			continue;
		}
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}

		if (item_config->color >= lvl) {
			++nums;
		}
	}

	if (player_self()->heroer_detail().fighter_index > 0) {
		int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP;
		int hero_equip_idx_suit_end = hero_pack_idx_gap + EQUIP_IDX_SUIT_END;
		for (int index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
			if (index >= hero_equip_idx_suit_end) {
				continue;
			}
			equip = this->pack_get_item(index);
			if (! equip) {
				continue;
			}
			item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (! item_config) {
				continue;
			}
			if (item_config->color >= lvl) {
				++nums;
			}
		}
	}
	return nums;
}
int Achievement::get_hero_nums_by_lvl(const int lvl, const int num) {
	int nums = 0;
	for (std::map<int, Hero_Info_Detail>::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end(); ++it) {
		if (it->second.awake_lvl >= lvl) {
			++nums;
		}
		if (nums >= num) {
			break;
		}
	}
	return nums;
}

int Achievement::get_equip_nums(Int_Int_Set_Map cond_ext1) {
	int nums = 0;
//	for (std::map<int, Hero_Info_Detail>::const_iterator it = player_self()->heroer_detail().hero_map.begin();
//			it != player_self()->heroer_detail().hero_map.end(); ++it) {
//		if (it->second.awake_lvl >= lvl) {
//			++nums;
//		}
//		if (nums >= num) {
//			break;
//		}
//	}
	return nums;
}

int Achievement::achievement_refresh_medal_times(const int8_t type) {
	switch(type) {
	case ACHIEVEMENT_ARENA: {
		this->player_self()->fetch_arena_info();
		break;
	}
	case ACHIEVEMENT_REWARD: {
		this->player_self()->send_to_client_reward_task_list_show_status();
		player_self()->notify_remain_reward_task_num();
		break;
	}
	case ACHIEVEMENT_WAR_SCENE: {
		this->player_self()->req_war_scene_enter_num();
		break;
	}
	case ACHIEVEMENT_ALCHEMY: {
		this->player_self()->req_alchemy_info();
		break;
	}
	case ACHIEVEMENT_TEAM_SPORTS: {
		this->player_self()->profile_info();
		break;
	}
	default:
		break;
	}

	return 0;
}

int Achievement::achievement_arena_times(void) {
	int times = 0;
	for (uint32_t i = Pack::PACK_T_ACHIEVEMENT_INDEX + 1; i <= Pack::PACK_T_ACHIEVEMENT_INDEX + PACK_ACHIEVEMENT_GAP; ++i) {
		Item_Detail * item = this->pack_get_item(i);
		if (!item) {
			continue;
		}
		const Achievement_Cfg_Medal_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_medal(item->id_);
		if (!cfg || cfg->type != ACHIEVEMENT_ARENA) {
			continue;
		}
		times += cfg->num;
	}
	return times;
}
int Achievement::achievement_reward_times(void) {
	int times = 0;
	for (uint32_t i = Pack::PACK_T_ACHIEVEMENT_INDEX + 1; i <= Pack::PACK_T_ACHIEVEMENT_INDEX + PACK_ACHIEVEMENT_GAP; ++i) {
		Item_Detail * item = this->pack_get_item(i);
		if (!item) {
			continue;
		}
		const Achievement_Cfg_Medal_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_medal(item->id_);
		if (!cfg || cfg->type != ACHIEVEMENT_REWARD) {
			continue;
		}
		times += cfg->num;
	}
	return times;
}
int Achievement::achievement_war_scene_times(void) {
	int times = 0;
	for (uint32_t i = Pack::PACK_T_ACHIEVEMENT_INDEX + 1; i <= Pack::PACK_T_ACHIEVEMENT_INDEX + PACK_ACHIEVEMENT_GAP; ++i) {
		Item_Detail * item = this->pack_get_item(i);
		if (!item) {
			continue;
		}
		const Achievement_Cfg_Medal_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_medal(item->id_);
		if (!cfg || cfg->type != ACHIEVEMENT_WAR_SCENE) {
			continue;
		}
		times += cfg->num;
	}
	return times;
}
int Achievement::achievement_alchemy_times(void) {
	int times = 0;
	for (uint32_t i = Pack::PACK_T_ACHIEVEMENT_INDEX + 1; i <= Pack::PACK_T_ACHIEVEMENT_INDEX + PACK_ACHIEVEMENT_GAP; ++i) {
		Item_Detail * item = this->pack_get_item(i);
		if (!item) {
			continue;
		}
		const Achievement_Cfg_Medal_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_medal(item->id_);
		if (!cfg || cfg->type != ACHIEVEMENT_ALCHEMY) {
			continue;
		}
		times += cfg->num;
	}
	return times;
}
int Achievement::achievement_team_sports_times(void) {
	int times = 0;
	for (uint32_t i = Pack::PACK_T_ACHIEVEMENT_INDEX + 1; i <= Pack::PACK_T_ACHIEVEMENT_INDEX + PACK_ACHIEVEMENT_GAP; ++i) {
		Item_Detail * item = this->pack_get_item(i);
		if (!item) {
			continue;
		}
		const Achievement_Cfg_Medal_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_medal(item->id_);
		if (!cfg || cfg->type != ACHIEVEMENT_TEAM_SPORTS) {
			continue;
		}
		times += cfg->num;
	}
	return times;
}

void Achievement::save_tick(void) {
	this->achievement_detail_->is_change = true;
}
