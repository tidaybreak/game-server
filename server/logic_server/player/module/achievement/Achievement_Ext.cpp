/*
 * Achievement_Ext.cpp
 *
 *  Created on: 2014年9月11日
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
#include "Config_Cache_NPC.h"
#include "team/Team_Manager.h"

int Achievement::achievement_listen_war_kill_nums(void) {
	int32_t nums = ++this->achievement_detail_->war_kill_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_war_kill_nums.begin();
			it != listen_war_kill_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
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
	save_tick();
	achievement_war_kill_nums_construct();
	player_self()->anci_artifact_listen_war_kill_nums();
	return 0;
}
int Achievement::achievement_listen_arena_win_nums(void) {
	int32_t nums = ++this->achievement_detail_->arena_win_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_arena_win_nums.begin();
			it != listen_arena_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
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
	save_tick();
	achievement_arena_win_nums_construct();
	player_self()->anci_artifact_listen_arena_win_nums();
	return 0;
}
int Achievement::achievement_listen_gang_war_win_nums(void) {
	int32_t nums = ++this->achievement_detail_->gang_war_win_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_gang_war_win_nums.begin();
			it != listen_gang_war_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_gang_war_win_nums_construct();
	player_self()->anci_artifact_listen_gang_war_win_nums();
	return 0;
}
int Achievement::achievement_listen_exploit_lvl(void) {
	int32_t nums = player_self()->public_detail().exploit_lvl;
	player_self()->title_nobility_announce(nums);
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_exploit_lvl.begin();
			it != listen_exploit_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
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
	save_tick();
	achievement_exploit_lvl_construct();
	player_self()->anci_artifact_listen_exploit_lvl();
	return 0;
}
int Achievement::achievement_listen_first_lvl_role_id(void) {
	achievement_first_lvl_role_id_construct();
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_first_lvl_role_id.begin();
			it != listen_first_lvl_role_id.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_role_id_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_role_id_his().end()) {
			continue;
		}
		int32_t nums = (it_fl->second == role_id()) ? 1 : 0;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	player_self()->anci_artifact_listen_first_lvl_role_id();
	return 0;
}
int Achievement::achievement_listen_first_lvl_ck(void) {
	achievement_first_lvl_ck_construct();
	if (2 != player_self()->base_detail().career) {
		return -1;
	}
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_first_lvl_ck.begin();
			it != listen_first_lvl_ck.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_ck_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_ck_his().end()) {
			continue;
		}
		int32_t nums = (it_fl->second == role_id()) ? 1 : 0;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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

	player_self()->anci_artifact_listen_first_lvl_ck();
	return 0;
}
int Achievement::achievement_listen_first_lvl_zs(void) {
	achievement_first_lvl_zs_construct();
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_first_lvl_zs.begin();
			it != listen_first_lvl_zs.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_zs_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_zs_his().end()) {
			continue;
		}
		int32_t nums = (it_fl->second == role_id()) ? 1 : 0;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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

	player_self()->anci_artifact_listen_first_lvl_zs();
	return 0;
}
int Achievement::achievement_listen_first_lvl_yss(void) {
	achievement_first_lvl_yss_construct();
	if (3 != player_self()->base_detail().career) {
		return -1;
	}
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_first_lvl_yss.begin();
			it != listen_first_lvl_yss.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_yss_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_yss_his().end()) {
			continue;
		}
		int32_t nums = (it_fl->second == role_id()) ? 1 : 0;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	player_self()->anci_artifact_listen_first_lvl_yss();
	return 0;
}
int Achievement::achievement_listen_kill_monster_nums(const int amount) {
	this->achievement_detail_->kill_monster_nums_his += amount;
	int32_t nums = this->achievement_detail_->kill_monster_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_kill_monster_nums.begin();
			it != listen_kill_monster_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_kill_monster_nums_construct();
	player_self()->anci_artifact_listen_kill_monster_nums();
	return 0;
}
int Achievement::achievement_listen_single_dungeon_chapter_complete(const int chap_id) {
//	int32_t nums = ++this->achievement_detail_->kill_monster_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_single_dungeon_chapter_complete.begin();
			it != listen_single_dungeon_chapter_complete.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		int32_t nums = (chap_id == cfg->cond_ext) ? 1 : 0;//player_self()->sgl_dungeon_detail().get_chapter_status(cfg->cond_ext);
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_single_dungeon_chapter_complete_construct();
	player_self()->anci_artifact_listen_single_dungeon_chapter_complete(chap_id);
	return 0;
}
int Achievement::achievement_listen_team_fb_complete_nums(const int lvl) {
	if (0 == this->achievement_detail_->team_fb_complete_nums_his.count(lvl)) {
		this->achievement_detail_->team_fb_complete_nums_his.insert(lvl);
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_team_fb_complete_nums.begin();
				it != listen_team_fb_complete_nums.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			int32_t nums = this->achievement_detail_->team_fb_complete_nums_his.count(cfg->cond_ext);
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_team_fb_complete_nums_construct();
		player_self()->anci_artifact_listen_team_fb_complete_nums();
	}
	return 0;
}
int Achievement::achievement_listen_horse_get(const int horse_id) {
	if (0 == this->achievement_detail_->horse_get_his.count(horse_id)) {
		this->achievement_detail_->horse_get_his.insert(horse_id);
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_horse_get.begin();
				it != listen_horse_get.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			int32_t nums = this->achievement_detail_->horse_get_his.count(cfg->cond_ext);
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_horse_get_construct();
		player_self()->anci_artifact_listen_horse_get();
	}
	achievement_listen_horse_get_nums();
	return 0;
}
int Achievement::achievement_listen_recharge_diamond(void) {
	int32_t nums = static_cast<int32_t>(player_self()->public_detail().recharge_money_his);
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_recharge_diamond.begin();
			it != listen_recharge_diamond.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_recharge_diamond_construct();
	player_self()->anci_artifact_listen_recharge_diamond();
	return 0;
}
int Achievement::achievement_listen_secret_shop_buy_nums(const int amount) {
	if (amount <= 0) {
		return -1;
	}
	this->achievement_detail_->secret_shop_buy_nums_his += amount;
	int32_t nums = this->achievement_detail_->secret_shop_buy_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_secret_shop_buy_nums.begin();
			it != listen_secret_shop_buy_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_secret_shop_buy_nums_construct();
	player_self()->anci_artifact_listen_secret_shop_buy_nums();
	return 0;
}
int Achievement::achievement_listen_dragon_wale_rob_times(void) {
	int32_t nums = ++this->achievement_detail_->dragon_wale_rob_times_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_dragon_wale_rob_times.begin();
			it != listen_dragon_wale_rob_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_dragon_wale_rob_times_construct();
	player_self()->anci_artifact_listen_dragon_wale_rob_times();
	return 0;
}
int Achievement::achievement_listen_dragon_wale_step_foot_times(void) {
	int32_t nums = ++this->achievement_detail_->dragon_wale_step_foot_times_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_dragon_wale_step_foot_times.begin();
			it != listen_dragon_wale_step_foot_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_dragon_wale_step_foot_times_construct();
	player_self()->anci_artifact_listen_dragon_wale_step_foot_times();
	return 0;
}
int Achievement::achievement_listen_dragon_wale_play_dragon_times(void) {
	int32_t nums = ++this->achievement_detail_->dragon_wale_play_dragon_times_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_dragon_wale_play_dragon_times.begin();
			it != listen_dragon_wale_play_dragon_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_dragon_wale_play_dragon_times_construct();
	player_self()->anci_artifact_listen_dragon_wale_play_dragon_times();
	return 0;
}
int Achievement::achievement_listen_hero_awake_lvl(const int lvl) {
	if (this->achievement_detail_->hero_awake_lvl_his < lvl) {
		this->achievement_detail_->hero_awake_lvl_his = lvl;
		int32_t nums = this->achievement_detail_->hero_awake_lvl_his;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_hero_awake_lvl.begin();
				it != listen_hero_awake_lvl.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_hero_awake_lvl_construct();
		player_self()->anci_artifact_listen_hero_awake_lvl();
	}
	achievement_listen_hero_nums_by_lvl();
	return 0;
}
int Achievement::achievement_listen_task_complete_nums(void) {
	int32_t nums = ++this->achievement_detail_->task_complete_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_task_complete_nums.begin();
			it != listen_task_complete_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_task_complete_nums_construct();
	player_self()->anci_artifact_listen_task_complete_nums();
	return 0;
}
int Achievement::achievement_listen_dragon_wale_gold_mine_lvl(const int lvl) {
	if (this->achievement_detail_->dragon_wale_gold_mine_lvl_his < lvl) {
		this->achievement_detail_->dragon_wale_gold_mine_lvl_his = lvl;
		int32_t nums = this->achievement_detail_->dragon_wale_gold_mine_lvl_his;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_dragon_wale_gold_mine_lvl.begin();
				it != listen_dragon_wale_gold_mine_lvl.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_dragon_wale_gold_mine_lvl_construct();
		player_self()->anci_artifact_listen_dragon_wale_gold_mine_lvl();
	}
	return 0;
}
int Achievement::achievement_listen_dragon_wale_moon_well_lvl(const int lvl) {
	if (this->achievement_detail_->dragon_wale_moon_well_lvl_his < lvl) {
		this->achievement_detail_->dragon_wale_moon_well_lvl_his = lvl;
		int32_t nums = this->achievement_detail_->dragon_wale_moon_well_lvl_his;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_dragon_wale_moon_well_lvl.begin();
				it != listen_dragon_wale_moon_well_lvl.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_dragon_wale_moon_well_lvl_construct();
		player_self()->anci_artifact_listen_dragon_wale_moon_well_lvl();
	}
	return 0;
}
int Achievement::achievement_listen_dragon_wale_totem_lvl(const int lvl) {
	if (this->achievement_detail_->dragon_wale_totem_lvl_his < lvl) {
		this->achievement_detail_->dragon_wale_totem_lvl_his = lvl;
		int32_t nums = this->achievement_detail_->dragon_wale_totem_lvl_his;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_dragon_wale_totem_lvl.begin();
				it != listen_dragon_wale_totem_lvl.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_dragon_wale_totem_lvl_construct();
		player_self()->anci_artifact_listen_dragon_wale_totem_lvl();
	}
	return 0;
}
int Achievement::achievement_listen_dragon_vale_drive_thief_times(void) {
	int32_t nums = ++this->achievement_detail_->dragon_wale_drive_thief_times_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_dragon_wale_drive_thief_times.begin();
			it != listen_dragon_wale_drive_thief_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_dragon_wale_drive_thief_times_construct();
	player_self()->anci_artifact_listen_dragon_vale_drive_thief_times();
	return 0;
}
int Achievement::achievement_listen_war_light_win_nums(void) {
	int32_t nums = ++this->achievement_detail_->war_light_win_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_war_light_win_nums.begin();
			it != listen_war_light_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_war_light_win_nums_construct();
	player_self()->anci_artifact_listen_war_light_win_nums();
	return 0;
}
int Achievement::achievement_listen_war_dark_win_nums(void) {
	int32_t nums = ++this->achievement_detail_->war_dark_win_nums_his;
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_war_dark_win_nums.begin();
			it != listen_war_dark_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
	achievement_war_dark_win_nums_construct();
	player_self()->anci_artifact_listen_war_dark_win_nums();
	return 0;
}
int Achievement::achievement_listen_task_limit_time(const int task_limit_time) {
	int task_limit_time_bak = ceil(task_limit_time / 60.0);
	if (-1 == this->achievement_detail_->task_limit_time_his
			|| this->achievement_detail_->task_limit_time_his > task_limit_time_bak) {
		this->achievement_detail_->task_limit_time_his = task_limit_time_bak;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_task_limit_time.begin();
				it != listen_task_limit_time.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			int32_t nums = 0;
			if (this->achievement_detail_->task_limit_time_his != -1
					&& this->achievement_detail_->task_limit_time_his <= cfg->cond_ext) {
				nums = 1;
			}
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_task_limit_time_construct();
		player_self()->anci_artifact_listen_task_limit_time();
	}
	return 0;
}
int Achievement::achievement_listen_kill_boss(const int boss_id) {
	const NPC_Config_Cache * monster_cfg = CONFIG_CACHE_NPC->find_npc_config_cache(boss_id);
	if(!monster_cfg
			|| (monster_cfg->type != MOVER_T_MONSTER_BOSS && monster_cfg->type != MOVER_T_MONSTER_ELITE_BOSS)){
		return -1;
	}
	if (0 == this->achievement_detail_->kill_boss_his.count(boss_id)) {
		this->achievement_detail_->kill_boss_his.insert(boss_id);
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_kill_boss.begin();
				it != listen_kill_boss.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			int32_t nums = this->achievement_detail_->kill_boss_his.count(cfg->cond_ext);
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_kill_boss_construct();
		player_self()->anci_artifact_listen_kill_boss();
	}
	return 0;
}
int Achievement::achievement_listen_dragon_wale_keep_level(const int lvl) {
	if (this->achievement_detail_->dragon_wale_keep_level_his < lvl) {
		this->achievement_detail_->dragon_wale_keep_level_his = lvl;
		int32_t nums = this->achievement_detail_->dragon_wale_keep_level_his;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_dragon_wale_keep_level.begin();
				it != listen_dragon_wale_keep_level.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
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
		achievement_dragon_wale_keep_level_construct();
		player_self()->anci_artifact_listen_dragon_wale_keep_level();
	}
	return 0;
}

int Achievement::achievement_listen_horse_get_nums(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	int32_t nums = player_self()->get_mount_num();
	for (Achievement_Info_Listen::iterator it = listen_horse_get_nums.begin();
			it != listen_horse_get_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = nums;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	save_tick();
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_horse_get_nums_construct();
	player_self()->anci_artifact_listen_horse_get_nums();
	return 0;
}

int Achievement::achievement_listen_rune_stone_lvl(const int32_t lvl) {
	if (this->achievement_detail_->rune_stone_lvl_his < lvl) {
		this->achievement_detail_->rune_stone_lvl_his = lvl;
		save_tick();

		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_rune_stone_lvl.begin(); it != listen_rune_stone_lvl.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != this->achievement_detail_->rune_stone_lvl_his
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = this->achievement_detail_->rune_stone_lvl_his;
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
				info.data = this->achievement_detail_->rune_stone_lvl_his;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_rune_stone_lvl_construct();
		player_self()->anci_artifact_listen_rune_stone_lvl();
	}
	return 0;
}
int Achievement::achievement_listen_equip_on_body_both_player_and_hero_by_lvl(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_equip_on_body_both_player_and_hero_by_lvl.begin();
			it != listen_equip_on_body_both_player_and_hero_by_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		int32_t nums = get_equip_on_body_both_player_and_hero_by_lvl(cfg->cond_ext);
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = nums;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	save_tick();
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_equip_on_body_both_player_and_hero_by_lvl_construct();
	player_self()->anci_artifact_listen_equip_on_body_both_player_and_hero_by_lvl();
	return 0;
}
int Achievement::achievement_listen_arena_lvl(const int32_t lvl) {
	if (this->achievement_detail_->arena_lvl_his < lvl) {
		this->achievement_detail_->arena_lvl_his = lvl;
		MSG_USER("this->achievement_detail_->arena_lvl_his: %d", this->achievement_detail_->arena_lvl_his);
		save_tick();

		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_arena_lvl.begin(); it != listen_arena_lvl.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->data != this->achievement_detail_->arena_lvl_his
				&& it->second->state_count == 0) {
				need_change = true;
			}
			it->second->data = this->achievement_detail_->arena_lvl_his;
			if (cfg->cond <= it->second->data && it->second->state_count == 0) {
				MSG_USER("cond: %d, data: %d", cfg->cond, it->second->data);
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			save_tick();
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = this->achievement_detail_->arena_lvl_his;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_arena_lvl_construct();
		player_self()->anci_artifact_listen_arena_lvl();
	}
	return 0;
}
int Achievement::achievement_listen_hero_nums_by_lvl(void) {
	MSG_81100801 acv_msg;
	acv_msg.op = 1;
	Achievement_Detail_Msg info;
	for (Achievement_Info_Listen::iterator it = listen_hero_nums_by_lvl.begin();
			it != listen_hero_nums_by_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
		if (!cfg) {
			continue;
		}
		bool need_change = false;
		int32_t nums = get_hero_nums_by_lvl(cfg->cond_ext, cfg->cond);
		if (it->second->data != nums
			&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			need_change = true;
		}
		it->second->data = nums;
		if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
			it->second->state_count = Achievement_Def::CAN_APPLY;
			it->second->state_award = Achievement_Def::CAN_APPLY;
			achievement_active_msg(it->first);
			achievement_announce(it->first);
		}
		if (need_change) {
			info.reset();
			info.id = it->first;
			info.data = nums;
			info.state_award = it->second->state_award;
			info.state_count = it->second->state_count;
			acv_msg.list.push_back(info);
		}
	}
	save_tick();
	if (!acv_msg.list.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	achievement_hero_nums_by_lvl_construct();
	player_self()->anci_artifact_listen_hero_nums_by_lvl();
	return 0;
}

int Achievement::achievement_listen_first_boss_against(void) {
	if (0 == this->achievement_detail_->first_boss_against) {
		this->achievement_detail_->first_boss_against = 1;
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_first_boss_against.begin();
				it != listen_first_boss_against.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			if (it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = 1;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = 1;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_first_boss_against_construct();
	}

	return 0;
}

int Achievement::achievement_listen_complete_suit(const int32_t item_id) {
	if (0 == this->achievement_detail_->horse_get_his.count(item_id)) {
		this->achievement_detail_->horse_get_his.insert(item_id);
		MSG_81100801 acv_msg;
		acv_msg.op = 1;
		Achievement_Detail_Msg info;
		for (Achievement_Info_Listen::iterator it = listen_complete_suit.begin();
				it != listen_complete_suit.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(it->first);
			if (!cfg) {
				continue;
			}
			bool need_change = false;
			int32_t nums = this->achievement_detail_->complete_suit.count(cfg->cond_ext);
			if (it->second->data != nums
				&& it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				need_change = true;
			}
			it->second->data = nums;
			if (cfg->cond <= it->second->data && it->second->state_count == Achievement_Def::CANNOT_APPLY) {
				it->second->state_count = Achievement_Def::CAN_APPLY;
				it->second->state_award = Achievement_Def::CAN_APPLY;
				achievement_active_msg(it->first);
				achievement_announce(it->first);
			}
			if (need_change) {
				info.reset();
				info.id = it->first;
				info.data = nums;
				info.state_award = it->second->state_award;
				info.state_count = it->second->state_count;
				acv_msg.list.push_back(info);
			}
		}
		save_tick();
		if (!acv_msg.list.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		achievement_complete_suit_construct();
	}
	return 0;
}

int Achievement::achievement_level_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_level.begin();
			it != aim_level.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
//		if (cfg->cond > level()) {
//			Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
//			if (it_map != this->achievement_detail_->achievement_info_map.end()
//				|| this->achievement_detail_->complete_ids.count(*it) > 0) {
//				break;
//			}
//			info.reset();
//			info.id = *it;
//			info.data = level();
//			this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
//			it_map = this->achievement_detail_->achievement_info_map.find(*it);
//			if (it_map != this->achievement_detail_->achievement_info_map.end()) {
//				listen_level.insert(std::make_pair(*it, &it_map->second));
//			}
//			break;
//		} else {
			Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
			if (it_map != this->achievement_detail_->achievement_info_map.end()
				|| this->achievement_detail_->complete_ids.count(*it) > 0) {
				continue;
			}
			info.reset();
			info.id = *it;
			info.data = level();
			this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
			it_map = this->achievement_detail_->achievement_info_map.find(*it);
			if (it_map != this->achievement_detail_->achievement_info_map.end()) {
				listen_level.insert(std::make_pair(*it, &it_map->second));
			}
//			continue;
//		}
	}
	save_tick();
	return 0;
}

int Achievement::achievement_pack_grid_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_pack_grid.begin();
			it != aim_pack_grid.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = Packer::pack_get_capacity(Pack::Pack_Type::PACK_T_PACKAGE_INDEX);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_pack_grid.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_gold_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_gold.begin();
			it != aim_gold.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = Packer::pack_get_money().gold;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_gold.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_suit_purple_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_suit_purple.begin();
			it != aim_suit_purple.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		int nums = achievement_count_suit_purple_nums();
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = nums;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_suit_purple.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_suit_gold_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_suit_gold.begin();
			it != aim_suit_gold.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->suit_gold_ids.size();
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_suit_gold.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_rune_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_rune.begin();
			it != aim_rune.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->get_gemstone_count(cfg->cond_ext);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_rune.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_unban_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_unban.begin();
			it != aim_unban.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->unban_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_unban.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_master_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_master.begin();
			it != aim_master.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->master_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_master.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_friendship_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_friendship.begin();
			it != aim_friendship.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = Packer::pack_get_money().friendship;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_friendship.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_force_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_force.begin();
			it != aim_force.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->force_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_force.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_gang_contribute_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_gang_contribute.begin();
			it != aim_gang_contribute.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->ganger_detail().achi_contribute;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_gang_contribute.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_recruit_hero_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_recruit_hero.begin();
			it != aim_recruit_hero.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->heroer_detail().hero_map.size();
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_recruit_hero.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_war_kill_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_war_kill_nums.begin();
			it != aim_war_kill_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->war_kill_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_war_kill_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_arena_win_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_arena_win_nums.begin();
			it != aim_arena_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->arena_win_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_arena_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_gang_war_win_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_gang_war_win_nums.begin();
			it != aim_gang_war_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->gang_war_win_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_gang_war_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_exploit_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_exploit_lvl.begin();
			it != aim_exploit_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->public_detail().exploit_lvl;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_exploit_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_first_lvl_role_id_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_first_lvl_role_id.begin();
			it != aim_first_lvl_role_id.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = this->achievement_detail_->first_lvl_role_id_his.find(cfg->cond_ext);
//		if (it_fl != this->achievement_detail_->first_lvl_role_id_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_role_id_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_role_id_his().end()) {
			continue;
		}
//		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_first_lvl_role_id.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_first_lvl_ck_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_first_lvl_ck.begin();
			it != aim_first_lvl_ck.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_ck_his().find(cfg->cond_ext);
//		if (it_fl != TEAM_MANAGER->first_lvl_ck_his().end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_ck_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_ck_his().end()) {
			continue;
		}
//		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_first_lvl_ck.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_first_lvl_zs_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_first_lvl_zs.begin();
			it != aim_first_lvl_zs.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = this->achievement_detail_->first_lvl_zs_his.find(cfg->cond_ext);
//		if (it_fl != this->achievement_detail_->first_lvl_zs_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_zs_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_zs_his().end()) {
			continue;
		}
//		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_first_lvl_zs.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_first_lvl_yss_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_first_lvl_yss.begin();
			it != aim_first_lvl_yss.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
//		Int_Uid::const_iterator it_fl = this->achievement_detail_->first_lvl_yss_his.find(cfg->cond_ext);
//		if (it_fl != this->achievement_detail_->first_lvl_yss_his.end() && it_fl->second == role_id()) {
//			info.data = 1;
//		}
		Int_Uid::const_iterator it_fl = TEAM_MANAGER->first_lvl_yss_his().find(cfg->cond_ext);
		if (it_fl == TEAM_MANAGER->first_lvl_yss_his().end()) {
			continue;
		}
//		info.data = (it_fl->second == role_id()) ? 1 : 0;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_first_lvl_yss.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_kill_monster_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_kill_monster_nums.begin();
			it != aim_kill_monster_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->kill_monster_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_kill_monster_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_single_dungeon_chapter_complete_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_single_dungeon_chapter_complete.begin();
			it != aim_single_dungeon_chapter_complete.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->sgl_dungeon_detail().get_chapter_status(cfg->cond_ext);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_single_dungeon_chapter_complete.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_team_fb_complete_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_team_fb_complete_nums.begin();
			it != aim_team_fb_complete_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->team_fb_complete_nums_his.count(cfg->cond_ext);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_team_fb_complete_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_horse_get_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_horse_get.begin();
			it != aim_horse_get.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->horse_get_his.count(cfg->cond_ext);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_horse_get.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_recharge_diamond_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_recharge_diamond.begin();
			it != aim_recharge_diamond.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->public_detail().recharge_money_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_recharge_diamond.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_secret_shop_buy_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_secret_shop_buy_nums.begin();
			it != aim_secret_shop_buy_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->secret_shop_buy_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_secret_shop_buy_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_rob_times_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_rob_times.begin();
			it != aim_dragon_wale_rob_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_rob_times_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_rob_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_step_foot_times_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_step_foot_times.begin();
			it != aim_dragon_wale_step_foot_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_step_foot_times_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_step_foot_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_play_dragon_times_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_play_dragon_times.begin();
			it != aim_dragon_wale_play_dragon_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_play_dragon_times_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_play_dragon_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_hero_awake_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_hero_awake_lvl.begin();
			it != aim_hero_awake_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->hero_awake_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_hero_awake_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_task_complete_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_task_complete_nums.begin();
			it != aim_task_complete_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->task_complete_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_task_complete_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_gold_mine_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_gold_mine_lvl.begin();
			it != aim_dragon_wale_gold_mine_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_gold_mine_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_gold_mine_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_moon_well_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_moon_well_lvl.begin();
			it != aim_dragon_wale_moon_well_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_moon_well_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_moon_well_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_totem_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_totem_lvl.begin();
			it != aim_dragon_wale_totem_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_totem_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_totem_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_dragon_wale_drive_thief_times_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_drive_thief_times.begin();
			it != aim_dragon_wale_drive_thief_times.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_drive_thief_times_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_drive_thief_times.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_war_light_win_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_war_light_win_nums.begin();
			it != aim_war_light_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->war_light_win_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_war_light_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_war_dark_win_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_war_drak_win_nums.begin();
			it != aim_war_drak_win_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->war_dark_win_nums_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_war_dark_win_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_task_limit_time_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_task_limit_time.begin();
			it != aim_task_limit_time.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		int32_t nums = 0;
		if (this->achievement_detail_->task_limit_time_his != -1
				&& this->achievement_detail_->task_limit_time_his <= cfg->cond_ext) {
			nums = 1;
		}
		info.reset();
		info.id = *it;
		info.data = nums;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_task_limit_time.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_kill_boss_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_kill_boss.begin();
			it != aim_kill_boss.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->kill_boss_his.count(cfg->cond_ext);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_kill_boss.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}
int Achievement::achievement_horse_get_nums_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_horse_get_nums.begin();
			it != aim_horse_get_nums.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = player_self()->get_mount_num();
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_horse_get_nums.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_dragon_wale_keep_level_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_dragon_wale_keep_level.begin();
			it != aim_dragon_wale_keep_level.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->dragon_wale_keep_level_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_dragon_wale_keep_level.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_rune_stone_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_rune_stone_lvl.begin();
			it != aim_rune_stone_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->rune_stone_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_rune_stone_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_equip_on_body_both_player_and_hero_by_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_equip_on_body_both_player_and_hero_by_lvl.begin();
			it != aim_equip_on_body_both_player_and_hero_by_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = get_equip_on_body_both_player_and_hero_by_lvl(cfg->cond_ext);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_equip_on_body_both_player_and_hero_by_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_arena_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_arena_lvl.begin();
			it != aim_arena_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->arena_lvl_his;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_arena_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_hero_nums_by_lvl_construct(void) {
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_hero_nums_by_lvl.begin();
			it != aim_hero_nums_by_lvl.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = get_hero_nums_by_lvl(cfg->cond_ext, cfg->cond);
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_hero_nums_by_lvl.insert(std::make_pair(*it, &it_map->second));
		}
	}
	return 0;
}

int Achievement::achievement_first_boss_against_construct(void){
	Achievement_Info info;
	for (Int_Hash_Set::iterator it = aim_first_boss_against.begin();
			it != aim_first_boss_against.end(); ++it) {
		const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
		if (!cfg) {
			continue;
		}
		Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()
			|| this->achievement_detail_->complete_ids.count(*it) > 0) {
			continue;
		}
		info.reset();
		info.id = *it;
		info.data = this->achievement_detail_->first_boss_against;
		this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
		it_map = this->achievement_detail_->achievement_info_map.find(*it);
		if (it_map != this->achievement_detail_->achievement_info_map.end()) {
			listen_first_boss_against.insert(std::make_pair(*it, &it_map->second));
		}
	}

	return 0;
}
int Achievement::achievement_complete_suit_construct(void){
	Achievement_Info info;
		for (Int_Hash_Set::iterator it = aim_complete_suit.begin();
				it != aim_complete_suit.end(); ++it) {
			const Achievement_Cfg_Detail *cfg = CONFIG_CACHE_ACHIEVEMENT->find_achievement_cfg(*it);
			if (!cfg) {
				continue;
			}
			Achievement_Info_Map::iterator it_map = this->achievement_detail_->achievement_info_map.find(*it);
			if (it_map != this->achievement_detail_->achievement_info_map.end()
				|| this->achievement_detail_->complete_ids.count(*it) > 0) {
				continue;
			}
			info.reset();
			info.id = *it;
			info.data = this->achievement_detail_->complete_suit.count(cfg->cond_ext);
			this->achievement_detail_->achievement_info_map.insert(std::make_pair(*it, info));
			it_map = this->achievement_detail_->achievement_info_map.find(*it);
			if (it_map != this->achievement_detail_->achievement_info_map.end()) {
				listen_complete_suit.insert(std::make_pair(*it, &it_map->second));
			}
		}
	return 0;
}
