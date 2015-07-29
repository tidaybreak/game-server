/*
 * Alchemy.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: XIAOLIANG
 */
#include "Logic_Module_Head.h"
#include "Config_Cache_Active_Content.h"
#include "pack/Packer.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Err_Code.h"
#include "Active_Content_Struct.h"
#include "Active_Content_Manager.h"
#include "Msg_Activity_Struct.h"
#include "Msg_Active_Struct.h"
#include "mall/Mall_Manager.h"
#include "Active_Content_Def.h"
#include "ranking/Ranking_Manager.h"
#include "Logic_Player_Struct.h"
#include "equiper/Equiper_Struct.h"
#include "arena/Arena_Manager.h"
#include "gang/Gang_Struct.h"
#include "Config_Cache_Item.h"
#include "rune/Rune_Struct.h"
#include "wing/Wing_Struct.h"
#include "mount/Mount_Struct.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "Active_Def.h"
#include "Active_Manager.h"
#include "equiper/Equiper_Def.h"
#include "task/Task_Struct.h"
#include "helper/Helper_Struct.h"
#include "Config_Cache_Wing.h"

using namespace Pack;

Active_Content::Active_Content(void) {
	reset();
}

Active_Content::~Active_Content() {

}

void Active_Content::reset(void) {
	active_content_detail_ = 0;
	is_open_icon = false;
}



void Active_Content::load_detail(Active_Content_Detail &detail) {
	active_content_detail_ = &detail;
}

void Active_Content::save_detail(Active_Content_Detail &detail) {
	if (active_content_detail().is_change) {
		detail = active_content_detail();
		active_content_detail_->is_change = false;
	}
}

void Active_Content::module_init(void) {
	clear_active_content();
}

void Active_Content::daily_clear_active_content(void) {

}

void Active_Content::clear_active_content(void) {
	{//战场获胜
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER);
			active_content_detail_->warer_win_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//组队竞技
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA);
			active_content_detail_->team_arena_win_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//悬赏任务额外奖励
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK);
			active_content_detail_->reward_task_done_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//累计附魔x次
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI);
			active_content_detail_->enchanting_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//坐骑进阶X次
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X);
			active_content_detail_->mount_lvup_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//合成X张精英卡牌
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE);
			active_content_detail_->mount_lvup_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//炼金有礼
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT);
			active_content_detail_->alchemy_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{//世界BOSS
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS);
			active_content_detail_->common_receive_id[active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS]].clear();
			active_content_detail_->all_world_boss_hurt = 0;
			active_content_detail_->today_world_boss_hurt = 0;
			active_content_detail_->world_boos_max_rank = 0;
			active_content_detail_->detail_change();
		}
	}
	{//天天跑环 环任务活动
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK);
			active_content_detail_->ring_task_recharge = false;
			active_content_detail_->detail_change();
		}
	}
	{//坐骑升级 升星
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP);
			active_content_detail_->common_receive_id[active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP]].clear();
			active_content_detail_->detail_change();
		}
	}
	{////龙谷掠夺活动
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY);
			active_content_detail_->common_receive_id[active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY]].clear();
			active_content_detail_->dragon_prey_num = 0;
			active_content_detail_->detail_change();
		}
	}
	{////累计X个X品质英雄,
		if(active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER] != ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER)) {
			active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER] = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER);
			active_content_detail_->common_receive_id[active_content_detail_->common_type_act_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER]].clear();
			active_content_detail_->detail_change();
		}
	}

}

int Active_Content::client_ready_now(void) {
	Icon_Time_Info icon = ACTIVE_CONTENT_MANAGER->get_icon_time_status();
	if(icon.over_time.sec() > Time_Value::gettimeofday().sec()
	&& ACTIVE_CONTENT_MANAGER->get_icon_limit_level() <= (uint32_t)player_self()->level()) {
		send_active_content_open_icon(icon.icon_id);
	}
	//公用图标控制
	Common_Active_Icon_Info_Map &comm_icon = ACTIVE_CONTENT_MANAGER->get_common_icon();
	for(Common_Active_Icon_Info_Map::iterator it = comm_icon.begin(); it != comm_icon.end(); ++it) {
		if(it->second.over_time.sec() > Time_Value::gettimeofday().sec()
			&& it->second.level_limit <= (uint32_t)player_self()->level()) {
					int time =   it->second.down_time.sec() - Time_Value::gettimeofday().sec();
					if(time < 0) { time = 0; }
					send_active_content_open_icon(it->second.icon_id, time, it->second.active);
			}
	}
	//竞技场段位活动结束了发邮件
	if(ACTIVE_CONTENT_MANAGER->get_arena_dan_end_time().sec() == 0 || ACTIVE_CONTENT_MANAGER->get_arena_dan_end_time().sec() < Time_Value::gettimeofday().sec()) {
		receive_arena_dan_reward_in_mail();
	}
	//公用活动结束发邮件
	const Int_Time_Map &comm_act_lt_map = ACTIVE_CONTENT_MANAGER->get_common_last_start_time_map();
	for(Int_Time_Map::const_iterator it = comm_act_lt_map.begin(); it != comm_act_lt_map.end(); ++it) {
		if(it->second.sec() != 0 && ACTIVE_CONTENT_MANAGER->get_common_end_time(it->first).sec() < Time_Value::gettimeofday().sec()) {
			receive_common_reward_in_mail(it->first);
		}
	}
	return 0;
}

void Active_Content::send_active_content_open_icon(uint32_t icon_id, uint16_t time, int8_t active) {
	MSG_81000408 msg;
	msg.open = 1;
	msg.active = active;
	msg.time = time;
	msg.icon_id = icon_id;
	THIS_SEND_TO_CLIENT(msg);
	active_content_detail_->is_open_icon_id.insert(icon_id);
}

void Active_Content::send_active_content_close_icon(uint32_t icon_id) {
//	is_open_icon = false;
	MSG_81000408 msg;
	msg.open = 0;
	msg.active = 0;
	msg.time = 0;
	msg.icon_id = icon_id;
	THIS_SEND_TO_CLIENT(msg);
	UInt_Set::iterator it = active_content_detail_->is_open_icon_id.find(icon_id);
	if(it != active_content_detail_->is_open_icon_id.end()) {
		active_content_detail_->is_open_icon_id.erase(it);
	}
}

void Active_Content::active_content_listen_level(uint32_t level) {
	Icon_Time_Info icon = ACTIVE_CONTENT_MANAGER->get_icon_time_status();
	if(icon.over_time.sec() > Time_Value::gettimeofday().sec()
	&& ACTIVE_CONTENT_MANAGER->get_icon_limit_level() <= level) {
		if(!is_open_icon) {
			is_open_icon = true;
			send_active_content_open_icon(icon.icon_id);
		}
	}
	req_active_content_tip_num();
}

int Active_Content::trigger_daily_zero(const Time_Value &now) {
	player_self()->send_to_client_reward_task_list_show_status();
	player_self()->notify_remain_reward_task_num();
	clear_active_content();
	return 0;
}

int Active_Content::trigger_daily_six(const Time_Value &now) {
	//world boss
	active_content_detail_->today_world_boss_hurt = 0;
//	active_content_detail_->world_boos_max_rank = INT_MAX;
	const World_Boss_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_world_boss_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS);
	if(conf && act_id) {
		for(World_Boss_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.type  == 2) {
				UInt_Set::iterator it = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(it != active_content_detail_->common_receive_id[act_id].end()) {
					active_content_detail_->common_receive_id[act_id].erase(it);
				}
			}
		}
	}
	active_content_detail_->detail_change();
	{//通知上古副本加入次数
		int num = ACTIVE_MANAGER->get_active_add_num(ACTION_ACTIVE_CONTENT_SG_WELFARE);
		if(num) {
			player_self()->task_add_elite_challenge_num(num);
		}
	}

	clear_and_send_mail_on_reward_activity_gift();
	return 0;
}

void Active_Content::fill_role_info(Role_Info &role, role_id_t role_id) {
	role.reset();
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		Player_DB_View *player = iter->second;
		role.role_id = player->role_id;
		role.role_name = player->role_name;
		role.career = player->career;
		role.combat = player->force;
		role.gender = player->gender;
		role.head_photo_id = player->friend_.head_photo_id;
		role.level = player->level;
		role.vip_level = player->vip_level;
		role.guild_name = player->gang_name;
	}
}

uint32_t Active_Content::get_active_content_rune_by_level(uint32_t level) {
	uint32_t count = 0;
//	const Rune_Part_Detail_Map &part_detail_map = player_self()->rune_detail().part_detail_map;
//	for(Rune_Part_Detail_Map::const_iterator it = part_detail_map.begin(); it != part_detail_map.end(); ++it) {
//		const Rune_Hole_Detail_Map &hole_detail_map = it->second.hole_detail_map;
//		for(Rune_Hole_Detail_Map::const_iterator iter = hole_detail_map.begin(); iter != hole_detail_map.end(); ++iter) {
//			if(level <= (uint32_t)iter->second.level) {
//				++count;
//			}
//		}
//	}
	int index_start = Pack_Type::PACK_T_EQUIP_INDEX + EQUIP_BEGIN;
	int index_end = Pack_Type::PACK_T_EQUIP_INDEX + EQUIP_END;
	for (int index = index_start; index < index_end; ++index) {
		Item_Detail *equip = player_self()->pack_get_item(index);
		if (0 == equip || equip->type_ != Item_Detail::EQUIP) continue;
		for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
			int gemstone_id = equip->addon.equip.gemstones[i].stone_id;
			if (0 == gemstone_id) continue;
			const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(gemstone_id);
			if (0 == cfg) {
				LOG_TRACE_ABORT("gemstone item cannot be found, id =%d", gemstone_id);
				continue;
			}
			if((uint32_t)cfg->gemstone_cfg.lv >= level) {
				++count;
			}
		}
	}
	//heroer
	for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin();
			it != player_self()->heroer_detail().hero_map.end(); ++it) {
		int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX +
				it->second.hero_index * ONE_HERO_EQUIP_GAP;
		int index_start = hero_pack_idx_gap + EQUIP_BEGIN;
		int index_end = hero_pack_idx_gap + EQUIP_END;
		for (int index = index_start; index < index_end; ++index) {
			Item_Detail *equip = player_self()->pack_get_item(index);
			if (0 == equip || equip->type_ != Item_Detail::EQUIP) continue;
			for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
				int gemstone_id = equip->addon.equip.gemstones[i].stone_id;
				if (0 == gemstone_id) continue;
				const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(gemstone_id);
				if (0 == cfg) {
					LOG_TRACE_ABORT("gemstone item cannot be found, id =%d", gemstone_id);
					continue;
				}
				if((uint32_t)cfg->gemstone_cfg.lv >= level) {
					++count;
				}
			}
		}
	}
	return count;
}

uint32_t Active_Content::get_mount_star_level_by_mount_id(uint32_t id) {
	std::vector<Horse_Info>::const_iterator it;
	if(id) {
		for(it = player_self()->mount_detail().horse_list.begin(); it != player_self()->mount_detail().horse_list.end(); ++it){
			if((uint32_t)it->horse_id == id){
				return it->train_lv;
			}
		}
	} else {
		uint32_t max = 0;
		for(it = player_self()->mount_detail().horse_list.begin(); it != player_self()->mount_detail().horse_list.end(); ++it){
			if((uint32_t)it->train_lv > max){
				max = it->train_lv;
			}
		}
		return max;
	}
	return 0;
}

int Active_Content::receive_active_content_reward_in_mail(const Active_Reward_Info_Config &reward_info, uint32_t mail_id) {
	if(reward_info.item.size()) {
		Item_Vec item_list;
		for(Active_Item_Vec::const_iterator item_it = reward_info.item.begin(); item_it != reward_info.item.end(); ++item_it) {
			if(item_it->career == 0 || item_it->career == (uint32_t)player_self()->career()) {
				Item_Detail item(item_it->item_id, item_it->num, Item_Detail::BIND);
				item_list.push_back(item);
			}
		}
		if(item_list.size()) {
			LOGIC_PUBLIC->send_mail(mail_id, player_self()->role_id(), item_list);
		}
	}
	return 0;
}

void Active_Content::clear_and_send_mail_on_reward_activity_gift(void) {
	const Activity_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_activity_gift_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ACTIVITY_GIFT);
	if(conf && act_id) {
		uint32_t activity_gift_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(activity_gift_end_time + Time_Value::ONE_DAY_IN_SECS < Time_Value::gettimeofday().sec()) {
			return;
		}
		Active_Reward_Info_Config reward_info;
		for(Activity_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit == active_content_detail_->common_receive_id[act_id].end()) {
				//判断是否通关
				if(player_self()->daily_detail().today_activity >= iter->second.num) {
					reward_info.push_back(iter->second.reward_info);
				}
			}
		}
		receive_active_content_reward_in_mail(reward_info, 0);
		active_content_detail_->common_receive_id[act_id].clear();
		active_content_detail_->detail_change();
	}
}

int Active_Content::receive_active_content_reward(const Active_Reward_Info_Config &reward_info, const Gain_Item_DM_Info &item_dm_info, const Money_DM_Info &money_dm_info_) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if(reward_info.item.size()) {
		Item_Vec item_list;
		for(Active_Item_Vec::const_iterator item_it = reward_info.item.begin(); item_it != reward_info.item.end(); ++item_it) {
			if(item_it->career == 0 || item_it->career == (uint32_t)player_self()->career()) {
				Item_Detail item(item_it->item_id, item_it->num, Item_Detail::BIND);
				item_list.push_back(item);

				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				acv_msg.item_info.push_back(item_base);
				Card_Info card_info;
				if (item.set_item_card_info(card_info)) {
					acv_msg.card_info_vec.push_back(card_info);
				}
			}
		}
		if(item_list.size()) {
			int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, item_dm_info);
			if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if(reward_info.money.size()) {
		Money_Add_List money;
		money.clear();
		for(Active_Money_Vec::const_iterator money_it = reward_info.money.begin(); money_it != reward_info.money.end(); ++money_it) {
			if(money_it->career == 0 || money_it->career == (uint32_t)player_self()->career()) {
				money.push_back(Money_Add_Info(Money_Type(money_it->money_id), money_it->num, money_dm_info_));
				acv_msg.property.push_back(Property(money_it->money_id, money_it->num));
			}
		}
		if(money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if(reward_info.exp && (reward_info.career == 0 || reward_info.career == (uint32_t)player_self()->career())) {
		Exp_All exp_all = player_self()->modify_experience_inter(reward_info.exp, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	THIS_SEND_TO_CLIENT(acv_msg);
	return 0;
}


void Active_Content::active_content_listen_gang_level_up(int64_t gang_id, int32_t level) {
	if(ACTIVE_CONTENT_MANAGER->have_announce_guild_of_level(gang_id, level)) {
		player_self()->create_ann_active_content_guild_level_up(level);
	}
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_gang_change(void) {
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_vip_level_change(int32_t level) {
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_arena_levelup(int group, int seg) {//arena_dan_pass_time
	active_content_detail_->arena_dan_pass_time[group][seg]=Time_Value::gettimeofday();
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_world_boss_end_info(uint32_t hurt, uint32_t rank, uint32_t hurt_rate) {
	active_content_detail_->all_world_boss_hurt += hurt;
	active_content_detail_->today_world_boss_hurt += hurt;
	if(active_content_detail_->world_boos_max_rank == 0 || active_content_detail_->world_boos_max_rank > rank) {
		active_content_detail_->world_boos_max_rank = rank;
	}
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_heroes_door(uint32_t type, uint32_t level) {
	active_content_detail_->heroes_door_type_level[type].insert(level);
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_warer_win(void) {
	++active_content_detail_->warer_win_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_team_arena_win(void) {
	++active_content_detail_->team_arena_win_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_reward_task_done(void) {
	++active_content_detail_->reward_task_done_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_enchanting(void) {
	++active_content_detail_->enchanting_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_mount_evolve(void) {
	++active_content_detail_->mount_lvup_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_card_fusion(uint32_t type) {
	if(type == 40) {
		++active_content_detail_->fusion_x_elite_card_num;
		active_content_detail_->detail_change();
		req_active_content_tip_num();
	}
}

void Active_Content::active_content_listen_alchemy_num(void) {
	++active_content_detail_->alchemy_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

void Active_Content::active_content_listen_dragon_prey_num(void) {
	++active_content_detail_->dragon_prey_num;
	active_content_detail_->detail_change();
	req_active_content_tip_num();
}

Time_Value &Active_Content::get_arena_dan_level_time(uint32_t group, uint32_t seg) {
	return active_content_detail_->arena_dan_pass_time[group][seg];
}

void Active_Content::active_content_listen_suit_collect(uint32_t item_id) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if (! item_config) {
		return;
	}
	// 获得套装件数
	if (item_config->equip_cfg.suit_id) {
		active_content_detail_->suit_cnt_map[item_id] = 1;
		active_content_detail_->detail_change();
		req_active_content_tip_num();
	}
}

int Active_Content::req_receive_guild_level_reward(uint32_t id) {
	const Guild_Level_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_guild_level_conf_by_id(id);
	if(!conf) {
		return 0;
	}
	//判断是否已经领取
	UInt_Set::iterator rit = active_content_detail_->guild_level_receive_id.find(id);
	if(rit != active_content_detail_->guild_level_receive_id.end()) {
		return ERROR_REWARD_ALREADY_RECEIVE;
	}
	//判断是否可领
	if((uint32_t)player_self()->get_gang_level() < conf->level) {
		return ERROR_CLIENT_PARAM;
	}
	if(conf->type == 1 && !player_self()->get_is_level_lader(conf->level)) {
		return ERROR_CLIENT_PARAM;
	}
	//领取奖励
	int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_GUILD_LEVEL), Money_DM_Info(MONEY_ADD_GUILD_LEVEL));
	if(result) {
		return result;
	}
	//添加已领项
	active_content_detail_->guild_level_receive_id.insert(conf->id);
	active_content_detail_->detail_change();
	//回应客户端
	MSG_50106404 msg;
	msg.reset();
	msg.id = id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_receive_gem_levelup_reward(uint32_t id) {
	const Gem_Levelup_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_gem_levelup_conf_by_id(id);
	if(!conf) {
		return 0;
	}
	//判断是否已经领取
	UInt_Set::iterator rit = active_content_detail_->gem_levelup_receive_id.find(id);
	if(rit != active_content_detail_->gem_levelup_receive_id.end()) {
		return ERROR_REWARD_ALREADY_RECEIVE;
	}
	//判断是否可领
	if((uint32_t)player_self()->get_active_content_rune_by_level(conf->level) < conf->count) {
		return ERROR_CLIENT_PARAM;
	}
	//领取奖励
	int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_GEM_LEVELUP), Money_DM_Info(MONEY_ADD_GEM_LEVELUP));
	if(result) {
		return result;
	}
	//添加已领项
	active_content_detail_->gem_levelup_receive_id.insert(conf->id);
	active_content_detail_->detail_change();
	//回应客户端
	MSG_50106406 msg;
	msg.reset();
	msg.id = id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_receive_suit_collect_reward(uint32_t level) {
	const Suit_Collect_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_conf_by_level(level, player_self()->career());
	if(!conf) {
		return 0;
	}
	//判断是否已经领取
	UInt_Set::iterator rit = active_content_detail_->suit_collect_receive_level.find(level);
	if(rit != active_content_detail_->suit_collect_receive_level.end()) {
		return ERROR_REWARD_ALREADY_RECEIVE;
	}
	//判断是否收集完
	bool receive = true;
	for(UInt_Set::const_iterator nit = conf->suit_collect_item.begin(); nit != conf->suit_collect_item.end(); ++nit) {
		//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
		//曾经拥有过就算
		const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
		if(suit.find(*nit) == suit.end()) {
			receive = false;
			break;
		}
	}
	if(!receive) {
		return ERROR_CLIENT_PARAM;
	}
	//领取奖励
	int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_SUIT_COLLECT), Money_DM_Info(MONEY_ADD_SUIT_COLLECT));
	if(result) {
		return result;
	}
	//添加已领项
	active_content_detail_->suit_collect_receive_level.insert(conf->level);
	active_content_detail_->detail_change();
	//回应客户端
	MSG_50106403 msg;
	msg.reset();
	msg.level = level;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_receive_level_ace_reward(uint32_t level) {
	const Level_Ace_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_level_ace_conf_by_level(level);
	if(conf) {
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->level_ace_receive_level.find(level);
		if(rit != active_content_detail_->level_ace_receive_level.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断等级
		if((uint32_t)player_self()->level() < conf->level) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_LEVEL_ACE), Money_DM_Info(MONEY_ADD_LEVEL_ACE));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->level_ace_receive_level.insert(conf->level);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106402 msg;
		msg.reset();
		msg.level = level;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_receive_wing_levelup_reward(uint32_t level) {
	const Wing_Levelup_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_levelup_conf_by_level(level);
	if(conf) {
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->wing_levelup_receive_level.find(level);
		if(rit != active_content_detail_->wing_levelup_receive_level.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断等级
		if((uint32_t)player_self()->wing_detail().lv < conf->level) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_WING_LEVELUP), Money_DM_Info(MONEY_ADD_WING_LEVELUP));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->wing_levelup_receive_level.insert(conf->level);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106407 msg;
		msg.reset();
		msg.level = level;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_receive_mount_levelup_reward(uint32_t id) {
	const Mount_Levelup_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_levelup_conf_by_id(id);
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP);
	if(conf && act_id) {
		if(conf->act_id && conf->act_id != act_id) {
			return ERROR_CLIENT_PARAM;
		}
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->mount_levelup_receive_id.find(id);
		if(rit != active_content_detail_->mount_levelup_receive_id.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断等级
		if(player_self()->get_mount_star_level_by_mount_id(conf->mount_id) < conf->level) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_MOUNT_LEVELUP), Money_DM_Info(MONEY_ADD_MOUNT_LEVELUP));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->mount_levelup_receive_id.insert(conf->id);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106408 msg;
		msg.reset();
		msg.id = id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::receive_common_reward_in_mail(uint32_t act_id) {
//	if(active_content_detail_->on_send_mail_act_id.find(act_id) != active_content_detail_->on_send_mail_act_id.end()) {
//		return 0;
//	}
//	active_content_detail_->on_send_mail_act_id.insert(act_id);
//	active_content_detail_->detail_change();
//	switch(act_id) {
//		case ACTION_ACTIVE_CONTENT_WORLD_BOSS: { //世界BOSS(开服活动)
//			const World_Boss_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_world_boss_conf_map();
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(World_Boss_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否通关
//					switch(it->second.type) {
//						case 1: {
//							if(it->second.val > active_content_detail_->all_world_boss_hurt) {
//								continue;
//							}
//							break;
//						}
//						case 2: {
//							if(it->second.val > active_content_detail_->today_world_boss_hurt) {
//								continue;
//							}
//							break;
//						}
//						case 3: {
//							if(it->second.val < active_content_detail_->world_boos_max_rank || active_content_detail_->world_boos_max_rank == 0) {
//								continue;
//							}
//							break;
//						}
//						default: {
//							continue;
//							break;
//						}
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_WORLD_BOSS);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_CONTENT_SUIT_COLLECT_TWO: {//套装收集2
//			const Suit_Collect_Two_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_two_conf_map(player_self()->career());
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Suit_Collect_Two_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否收集完
//					bool receive = true;
//					for(UInt_Set::const_iterator nit = it->second.suit_collect_item.begin(); nit != it->second.suit_collect_item.end(); ++nit) {
//						//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
//						//曾经拥有过就算
//						const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
//						if(suit.find(*nit) == suit.end()) {
//							receive = false;
//							break;
//						}
//					}
//					if(!receive) {
//						continue;
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_SUIT_COLLECT_REWARD);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_CONTENT_HEROES_DOOR: {//英雄之门
//			const Heroes_Door_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_heroes_door_conf_map();
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Heroes_Door_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否通关
//					if(active_content_detail_->heroes_door_type_level[it->second.type].find(it->second.level) == active_content_detail_->heroes_door_type_level[it->second.type].end()) {
//						continue;
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_HEROES_DOOR);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_CONTENT_TEAM_ARENA2://组队竞技2
//		case ACTION_ACTIVE_CONTENT_TEAM_ARENA: { //组队竞技
//			const Team_Arena_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_team_arena_conf_map();
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Team_Arena_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					if(it->second.act_id && it->second.act_id != act_id) {
//						continue;
//					}
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否通关
//					if(it->second.num > active_content_detail_->team_arena_win_num) {
//						continue;
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info,ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_TEAM_ARENA);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_CONTENT_WARER : {//战场
//			const Warer_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_warer_conf_map();
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Warer_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否通关
//					if(it->second.num > active_content_detail_->warer_win_num) {
//						continue;
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_WARER);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_CONTENT_REWARD_TASK: { //悬赏
//			const Reward_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_reward_task_conf_map();
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Reward_Task_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否通关
//					if(it->second.num > active_content_detail_->reward_task_done_num) {
//						continue;
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_REWARD_TASK);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_CONTENT_SG_FB : {//：上古副本
//			const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_fb_conf_map();
//			if(conf) {
//				if(active_content_detail_->common_receive_id[act_id].size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Sg_Pass_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
//					//判断是否已经领取
//					UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(it->second.id);
//					if(rit != active_content_detail_->common_receive_id[act_id].end()) {
//						continue;
//					}
//					//判断是否通关
//					if(!player_self()->is_elite_dungeon_complete(it->second.sg_id)) {
//						continue;
//					}
//					//领取奖励
//					receive_active_content_reward_in_mail(it->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_SGFB);
//					//添加已领项
//					active_content_detail_->common_receive_id[act_id].insert(it->second.id);
//					active_content_detail_->detail_change();
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_VIP_CLUB : {	//VIP俱乐部
//			const Vip_Club_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_vip_club_conf_map();
//			if(conf) {
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				Active_Reward_Info_Config reward_info;
//				for(Vip_Club_Conf_Map::const_iterator citer = conf->begin(); citer != conf->end(); ++citer) {
//					for(Vip_Club_Item_Conf_Map::const_iterator iiter = citer->second.reward.begin();
//							iiter != citer->second.reward.end(); ++iiter) {
//						//判断是否有领取过
//						UInt_Set::iterator tit = active_content_detail_->vip_club_receive_id.find(iiter->second.reward_id);
//						if(tit == active_content_detail_->vip_club_receive_id.end()) {
//							//判断次数
//							const uint32_t num = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(citer->second.vip_level);
//							if(num >= citer->second.needs_num && player_self()->vip() >= iiter->second.limit_vip_level) {
//								Active_Item_Config item;
//								reward_info.item.push_back(item);
//								active_content_detail_->vip_club_receive_id.insert(iiter->second.reward_id);
//								active_content_detail_->detail_change();
//							}
//						}
//					}
//				}
//				//领取奖励
//				receive_active_content_reward_in_mail(reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_VIP_CLUB);
//			}
//
//			break;
//		}
//		case ACTION_ACTIVE_LEVEL_ACE: {//冲级活动
//			const Level_Ace_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_level_ace_conf_map();
//			if(conf) {
//				if(active_content_detail_->level_ace_receive_level.size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Level_Ace_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
//					//判断是否有领取过
//					UInt_Set::iterator tit = active_content_detail_->level_ace_receive_level.find(iter->second.level);
//					if(tit == active_content_detail_->level_ace_receive_level.end()) {
//						//判断等级
//						if((uint32_t)player_self()->level() >= iter->second.level) {
//							//领取奖励
//							receive_active_content_reward_in_mail(iter->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_LEVEL_ACE);
//							//添加已领项
//							active_content_detail_->level_ace_receive_level.insert(iter->second.level);
//							active_content_detail_->detail_change();
//						}
//					}
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_SUIT_COLLECT: {	//套装收集
//			const Suit_Collect_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_conf_map(player_self()->career());
//			if(conf) {
//				if(active_content_detail_->suit_collect_receive_level.size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Suit_Collect_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
//					//判断是否可领
//					UInt_Set::iterator tit = active_content_detail_->suit_collect_receive_level.find(cit->second.level);
//					if(tit == active_content_detail_->suit_collect_receive_level.end()) {
//						bool can = true;
//						for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
//							//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
//							//曾经拥有过就算
//							const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
//							if(suit.find(*nit) == suit.end()) {
//								can = false;
//								break;
//							}
//						}
//						if(can) {
//							//领取奖励
//							receive_active_content_reward_in_mail(cit->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_SUIT_COLLECT_REWARD);
//							//添加已领项
//							active_content_detail_->suit_collect_receive_level.insert(cit->second.level);
//							active_content_detail_->detail_change();
//						}
//					}
//				}
//			}
//			break;
//		}
//		case ACTION_ACTIVE_GUILD_LEVEL: {	//公会升级
//			const Guild_Level_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_guild_level_conf_map();
//			if(conf) {
//				if(active_content_detail_->guild_level_receive_id.size() >= conf->size()) {
//					return 0;
//				}
//				const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_common_last_start_time(act_id);
//				if(lstime.sec() == 0) {
//					return 0;//没有开启过活动
//				}
//				for(Guild_Level_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
//					Uint_Uint uu;
//					uu.val_1 = cit->second.id;
//					//判断是否可领
//					UInt_Set::iterator tit = active_content_detail_->guild_level_receive_id.find(cit->second.id);
//					if(tit == active_content_detail_->guild_level_receive_id.end()) {
//						if((uint32_t)player_self()->get_gang_level() >= cit->second.level) {
//							if(cit->second.type == 2 || player_self()->get_is_level_lader(cit->second.level)) {
//								//领取奖励
//								receive_active_content_reward_in_mail(cit->second.reward_info, ACTIVE_CONTENT_CLOSE_RECEIVE_MAIL_GUILD_LEVEL);
//								//添加已领项
//								active_content_detail_->guild_level_receive_id.insert(cit->second.level);
//								active_content_detail_->detail_change();
//							}
//						}
//					}
//				}
//			}
//			break;
//		}
//	}
	return 0;
}

int Active_Content::receive_arena_dan_reward_in_mail(void) {
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_arena_dan_last_act_id();
	if(act_id == 0) {
		return 0;
	}
	if(active_content_detail_->on_send_mail_act_id.find(act_id) != active_content_detail_->on_send_mail_act_id.end()) {
		return 0;
	}
	active_content_detail_->on_send_mail_act_id.insert(act_id);
	active_content_detail_->detail_change();
	const Arena_Dan_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_arena_dan_conf_map();
	if(conf) {
		if(active_content_detail_->arena_dan_receive_id.size() >= conf->size()) {
			return 0;
		}
		const Time_Value &lstime = ACTIVE_CONTENT_MANAGER->get_arena_dan_last_start_time();
		if(lstime.sec() == 0) {
			return 0;//没有开启过活动
		}
		for(Arena_Dan_Conf_Map::const_iterator it = conf->begin(); it != conf->end(); ++it) {
			//判断是否已经领取
			UInt_Set::iterator rit = active_content_detail_->arena_dan_receive_id.find(it->second.id);
			if(rit != active_content_detail_->arena_dan_receive_id.end()) {
				continue;
			}
			//判断该项是否当时活动项目
			if(it->second.act_id && it->second.act_id != act_id) {
				continue;
			}
			Time_Value time = player_self()->get_arena_dan_level_time(it->second.group, it->second.seg);
			long long int overtime = lstime.sec() + Time_Value::ONE_DAY_IN_SECS * it->second.time;
			//判断是否通关
			if(!time.sec() || time.sec() > overtime) {
				continue;
			}
			//领取奖励
			receive_active_content_reward_in_mail(it->second.reward_info);
			//添加已领项
			active_content_detail_->arena_dan_receive_id.insert(it->second.id);
			active_content_detail_->detail_change();
		}
	}
	return 0;
}

int Active_Content::req_receive_arena_dan_reward(uint32_t id) {
	const Arena_Dan_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_arena_dan_conf_by_id(id);
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_arena_dan_last_act_id();
	if(conf && act_id) {
		//判断该项是否当时活动项目
		if(conf->act_id && conf->act_id != act_id) {
			return 0;
		}
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->arena_dan_receive_id.find(id);
		if(rit != active_content_detail_->arena_dan_receive_id.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		Time_Value time = player_self()->get_arena_dan_level_time(conf->group, conf->seg);
		long long int overtime = ACTIVE_CONTENT_MANAGER->get_arena_dan_start_time().sec() + Time_Value::ONE_DAY_IN_SECS * conf->time;
		//判断是否通关
		if(!time.sec() || time.sec() > overtime) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ARENA_DAN), Money_DM_Info(MONEY_ADD_ARENA_DAN));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->arena_dan_receive_id.insert(conf->id);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106410 msg;
		msg.reset();
		msg.id = id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_receive_sg_pass_reward(uint32_t id) {
	const Sg_Pass_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_pass_conf_by_id(id);
	if(conf) {
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->sg_pass_receive_id.find(id);
		if(rit != active_content_detail_->sg_pass_receive_id.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断是否通关
		if(!player_self()->is_elite_dungeon_complete(conf->sg_id)) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_SG_PASS), Money_DM_Info(MONEY_ADD_SG_PASS));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->sg_pass_receive_id.insert(conf->id);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106409 msg;
		msg.reset();
		msg.id = id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_receive_mount_pass_reward(uint32_t id) {
	const Mount_Levelup_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_pass_conf_by_id(id);
	if(conf) {
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->mount_pass_receive_id.find(id);
		if(rit != active_content_detail_->mount_pass_receive_id.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断是否通关
		uint32_t level = player_self()->get_mount_star_level_by_mount_id(conf->mount_id);
		if(level < conf->level) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVITY_CONTENT, ACTION_ACTIVE_MOUNT_RANK), Money_DM_Info(MONEY_ADD_ACTIVITY_CONTENT, ACTION_ACTIVE_MOUNT_RANK));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->mount_pass_receive_id.insert(conf->id);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106412 msg;
		msg.reset();
		msg.id = id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_receive_wing_pass_reward(uint32_t id) {
	const Wing_Levelup_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_pass_conf_by_id(id);
	if(conf) {
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->wing_pass_receive_id.find(id);
		if(rit != active_content_detail_->wing_pass_receive_id.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断是否通关
		int lv = player_self()->wing_detail().lv;
		const Wing_Config *win_conf = CONFIG_CACHE_WING->get_wing_config(lv);
		if(win_conf && (uint32_t)win_conf->show_stage < conf->level) {
			return ERROR_CLIENT_PARAM;
		}
		//领取奖励
		int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVITY_CONTENT, ACTION_ACTIVE_WING_RANK), Money_DM_Info(MONEY_ADD_ACTIVITY_CONTENT, ACTION_ACTIVE_WING_RANK));
		if(result) {
			return result;
		}
		//添加已领项
		active_content_detail_->wing_pass_receive_id.insert(id);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106413 msg;
		msg.reset();
		msg.id = id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_receive_vip_club_reward(uint32_t id, uint32_t reward_id) {
	const Vip_Club_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_vip_club_conf_by_id(id);
	if(conf) {
		//判断是否已经领取
		UInt_Set::iterator rit = active_content_detail_->vip_club_receive_id.find(reward_id);
		if(rit != active_content_detail_->vip_club_receive_id.end()) {
			return ERROR_REWARD_ALREADY_RECEIVE;
		}
		//判断次数
		const uint32_t num = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(conf->vip_level);
		if(conf->needs_num > num) {
			return ERROR_CLIENT_PARAM;
		}
		//判断是否可领
		Vip_Club_Item_Conf_Map::const_iterator fiter = conf->reward.find(reward_id);
		if(fiter == conf->reward.end() || fiter->second.limit_vip_level > (uint32_t)player_self()->level()) {
			return ERROR_CLIENT_PARAM;
		}
		//添加到背包
		Item_Detail item = Item_Detail(fiter->second.item_id, fiter->second.num, Item_Detail::BIND);
		int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(Pack::ITEM_GAIN_VIP_CLUB));
		if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
			return result;
		}
		{
			MSG_81000102 acv_msg;
			acv_msg.type = 0;
			Item_Basic_Info ibi;
			item.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		//添加已领项
		active_content_detail_->vip_club_receive_id.insert(reward_id);
		active_content_detail_->detail_change();
		//回应客户端
		MSG_50106401 msg;
		msg.reset();
		msg.id = id;
		msg.reward_id = reward_id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_active_content_tip_num() {
	clear_active_content();
	if(ACTIVE_CONTENT_MANAGER->get_icon_limit_level() > (uint32_t)player_self()->level()) { return 0; }
	MSG_50106405 msg;
	msg.reset();
	Time_Value now = Time_Value::gettimeofday();
	{//vip俱乐部-奖励ID对应状态, 1.已经领取; 2.可以领取; 3.不能领取
		const Vip_Club_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_vip_club_conf_map();
		if(conf && ACTIVE_CONTENT_MANAGER->get_vip_club_end_time().sec() > now.sec()) {
			for(Vip_Club_Conf_Map::const_iterator citer = conf->begin(); citer != conf->end(); ++citer) {
				for(Vip_Club_Item_Conf_Map::const_iterator iiter = citer->second.reward.begin();
						iiter != citer->second.reward.end(); ++iiter) {
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->vip_club_receive_id.find(iiter->second.reward_id);
					if(tit == active_content_detail_->vip_club_receive_id.end()) {
						//判断次数
						const uint32_t num = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(citer->second.vip_level);
						if(num >= citer->second.needs_num && player_self()->vip() >= iiter->second.limit_vip_level) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//冲级高手
		const Level_Ace_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_level_ace_conf_map();
		if(conf && ACTIVE_CONTENT_MANAGER->get_level_ace_end_time().sec() > now.sec()) {
			for(Level_Ace_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->level_ace_receive_level.find(iter->second.level);
				if(tit == active_content_detail_->level_ace_receive_level.end()) {
					//判断等级
					if((uint32_t)player_self()->level() >= iter->second.level) {
						++msg.num;
					}
				}
			}
		}
	}
	{//套装收集
		const Suit_Collect_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_conf_map(player_self()->career());
		if(conf && ACTIVE_CONTENT_MANAGER->get_suit_collect_end_time().sec() > now.sec()) {
			const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
			for(Suit_Collect_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->suit_collect_receive_level.find(cit->second.level);
				if(tit == active_content_detail_->suit_collect_receive_level.end()) {
					bool can = true;
					for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
						std::map<int, int>::const_iterator hit = suit.find(*nit);
						if(hit == suit.end()) {
							can = false;
							break;
						}
					}
					if(can) {
						++msg.num;
					}
				}
			}
		}
	}
	{//工会等级
		const Guild_Level_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_guild_level_conf_map();
		if(conf && ACTIVE_CONTENT_MANAGER->get_guild_level_end_time().sec() > now.sec()) {
			for(Guild_Level_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->guild_level_receive_id.find(cit->second.id);
				if(tit == active_content_detail_->guild_level_receive_id.end()) {
					if((uint32_t)player_self()->get_gang_level() >= cit->second.level) {
						if(cit->second.type != 1 || player_self()->get_is_level_lader(cit->second.level)) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//宝石升级
		const Gem_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_gem_levelup_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_GEM_UPGRADE);
		if(conf && ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec() > now.sec()) {
			for(Gem_Levelup_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				if(cit->second.act_id && cit->second.act_id != act_id) {
					continue;
				}
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->gem_levelup_receive_id.find(cit->second.id);
				if(tit == active_content_detail_->gem_levelup_receive_id.end()) {
					//(uint32_t)player_self()->get_active_content_rune_by_level(conf->level) < conf->count
					if((uint32_t)player_self()->get_active_content_rune_by_level(cit->second.level) >= cit->second.count) {
						++msg.num;
					}
				}
			}
		}
	}
	{//翅膀升级
		const Wing_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_levelup_conf_map();
		if(conf && ACTIVE_CONTENT_MANAGER->get_wing_levelup_end_time().sec() > now.sec()) {
			for(Wing_Levelup_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->wing_levelup_receive_level.find(cit->second.level);
				if(tit == active_content_detail_->wing_levelup_receive_level.end()) {
					if((uint32_t)player_self()->wing_detail().lv >= cit->second.level) {
						++msg.num;
					}
				}
			}
		}
	}
	{//坐骑升级
		const Mount_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_levelup_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP);
		if(conf && act_id && ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec() > now.sec()) {
			for(Mount_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uint32_t level = player_self()->get_mount_star_level_by_mount_id(iter->second.mount_id);
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->mount_levelup_receive_id.find(iter->second.id);
				if(tit == active_content_detail_->mount_levelup_receive_id.end()) {
					if(level >= iter->second.level) {
						++msg.num;
					}
				}
			}
		}
	}
	{//上古副本
		const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_pass_conf_map();
		if(conf) {
			Uint_Uint uu;
			for(Sg_Pass_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->sg_pass_receive_id.find(iter->second.id);
				if(tit == active_content_detail_->sg_pass_receive_id.end()) {
					//判断是否通关
					if(player_self()->is_elite_dungeon_complete(iter->second.sg_id)) {
						++msg.num;
					}
				}
			}//end for
		}// end if conf
	}
	{//坐骑排名
		uint32_t end_time = ACTIVE_CONTENT_MANAGER->get_mount_rank_end_time().sec();
		if(end_time > now.sec()) {
			const Mount_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_pass_conf_map();
			if(conf) {
				for(Mount_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->mount_pass_receive_id.find(iter->second.id);
					if(tit != active_content_detail_->mount_pass_receive_id.end()) {
						//uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						uint32_t level = player_self()->get_mount_star_level_by_mount_id(iter->second.mount_id);
						//判断是否通关
						if(level >= iter->second.level) {
							++msg.num;
						} else {
							//uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
				}
			}
		}
	}
	{//翅膀排名
		uint32_t end_time = ACTIVE_CONTENT_MANAGER->get_wing_rank_end_time().sec();
		if(end_time > now.sec()) {
			const Wing_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_pass_conf_map();
			if(conf) {
				for(Wing_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->wing_pass_receive_id.find(iter->first);
					if(tit != active_content_detail_->wing_pass_receive_id.end()) {
						//uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						//判断是否通关
						int lv = player_self()->wing_detail().lv;
						const Wing_Config *win_conf = CONFIG_CACHE_WING->get_wing_config(lv);
						if(win_conf && (uint32_t)win_conf->show_stage >= iter->second.level) {
							++msg.num;
						} else {
							//uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
				}
			}
		}
	}
	{//竞技场段位
		const Arena_Dan_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_arena_dan_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_arena_dan_last_act_id();
		if(conf &&  ACTIVE_CONTENT_MANAGER->get_arena_dan_end_time().sec() > Time_Value::gettimeofday().sec() && act_id) {
			for(Arena_Dan_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				//判断该项是否当时活动项目
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				Time_Value time = player_self()->get_arena_dan_level_time(iter->second.group, iter->second.seg);
				long long int overtime = ACTIVE_CONTENT_MANAGER->get_arena_dan_start_time().sec() + Time_Value::ONE_DAY_IN_SECS * iter->second.time;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->arena_dan_receive_id.find(iter->second.id);
				if(tit == active_content_detail_->arena_dan_receive_id.end() && time.sec() && time.sec() <= overtime) {
					++msg.num;
				}
			}
		}
	}
	{//世界boss
		const World_Boss_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_world_boss_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS);
		if(conf && act_id) {
			uint32_t world_boss_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(world_boss_end_time) {
				for(World_Boss_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						switch(iter->second.type) {
							case 1: {
								if(iter->second.val <= active_content_detail_->all_world_boss_hurt) {
									++msg.num;
								}
								break;
							}
							case 2: {
								if(iter->second.val <= active_content_detail_->today_world_boss_hurt) {
									++msg.num;
								}
								break;
							}
							case 3: {
								if(iter->second.sval < active_content_detail_->world_boos_max_rank && iter->second.val >= active_content_detail_->world_boos_max_rank && active_content_detail_->world_boos_max_rank) {
									++msg.num;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	{//套装收集二段
		const Suit_Collect_Two_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_two_conf_map(player_self()->career());
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_SUIT_COLLECT_REWARD);
		if(conf && act_id) {
			uint32_t suit_collect_two_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(suit_collect_two_end_time > now.sec()) {
				const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
				for(Suit_Collect_Two_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
					if(cit->second.act_id && cit->second.act_id != act_id) {
						continue;
					}
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(cit->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						bool can = true;
						for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
							std::map<int, int>::const_iterator hit = suit.find(*nit);
							if(hit == suit.end()) {
								can = false;
								break;
							}
						}
						if(can) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//英雄之门
		const Heroes_Door_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_heroes_door_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_HEROES_DOOR);
		if(conf && act_id) {
			uint32_t heroes_door_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(heroes_door_end_time > now.sec()) {
				for(Heroes_Door_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						if(active_content_detail_->heroes_door_type_level[iter->second.type].find(iter->second.level) != active_content_detail_->heroes_door_type_level[iter->second.type].end()) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//组队竞技
		const Team_Arena_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_team_arena_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA);
		if(conf && act_id) {
			uint32_t team_arena_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(team_arena_end_time > now.sec()) {
				for(Team_Arena_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						if(iter->second.num <= active_content_detail_->team_arena_win_num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//战场
		const Warer_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_warer_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER);
		if(conf && act_id) {
			uint32_t warer_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(warer_end_time > now.sec()) {
				for(Warer_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						if(iter->second.num <= active_content_detail_->warer_win_num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//悬赏
		const Reward_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_reward_task_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK);
		if(conf && act_id) {
			uint32_t reward_task_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(reward_task_end_time > now.sec()) {
				for(Reward_Task_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						if(iter->second.num <= active_content_detail_->reward_task_done_num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//上古副本
		const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_fb_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_SGFB);
		if(conf && act_id) {
			uint32_t sg_fb_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(sg_fb_end_time > now.sec()) {
				for(Sg_Pass_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						//判断是否通关
						if(player_self()->is_elite_dungeon_complete(iter->second.sg_id)) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//累计附魔x次
		const Fm_X_Ci_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fm_x_ci_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI);
		if(conf && act_id) {
			uint32_t fm_x_ci_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(fm_x_ci_end_time > now.sec()) {
				for(Fm_X_Ci_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						//判断是否通关
						if(player_self()->active_content_detail().enchanting_num >= iter->second.num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//累计X个X品质英雄
		const X_X_Quality_Heroer_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_x_x_quality_heroer_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER);
		if(conf && act_id) {
			uint32_t x_x_quality_heroer_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(x_x_quality_heroer_end_time > now.sec()) {
				UInt_UInt_Map type_num_map;
				player_self()->get_heroer_type_num_map(type_num_map);
				for(X_X_Quality_Heroer_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						//判断是否通关
						if(type_num_map[iter->second.type] >= iter->second.num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//坐骑进阶X次
		const Mount_Lvup_X_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_lvup_x_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X);
		if(conf && act_id) {
			uint32_t mount_lvup_x_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(mount_lvup_x_end_time > now.sec()) {
				for(Mount_Lvup_X_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						//判断是否通关
						if(player_self()->active_content_detail().mount_lvup_num >= iter->second.num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//合成X张精英卡牌
		const Fusion_X_Elite_Card_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fusion_x_elite_card_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE);
		if(conf && act_id) {
			uint32_t fusion_x_elite_card_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(fusion_x_elite_card_end_time > now.sec()) {
				for(Fusion_X_Elite_Card_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						//判断是否通关
						if(player_self()->active_content_detail().fusion_x_elite_card_num >= iter->second.num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//炼金有礼
		const Alchemy_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_alchemy_gift_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT);
		if(conf && act_id) {
			uint32_t time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(time > now.sec()) {
				for(Alchemy_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit == active_content_detail_->common_receive_id[act_id].end()) {
						//判断是否通关
						if(player_self()->active_content_detail().alchemy_num >= iter->second.num) {
							++msg.num;
						}
					}
				}
			}
		}
	}
	{//活跃度好礼
		const Activity_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_activity_gift_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ACTIVITY_GIFT);
		if(conf && act_id) {
			for(Activity_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit == active_content_detail_->common_receive_id[act_id].end()) {
					//判断是否通关
					if(player_self()->daily_detail().today_activity >= iter->second.num) {
						++msg.num;
					}
				}
			}
		}
	}
	{//环任务活动
		const Active_Ring_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_ring_task_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK);
		if(conf && act_id) {
			for(Active_Ring_Task_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit == active_content_detail_->common_receive_id[act_id].end()) {
					//判断是否通关
					if(player_self()->tasker_detail().ring_num >= iter->second.num && (iter->second.recharge == 0 || active_content_detail_->ring_task_recharge)) {
						++msg.num;
					}
				}
			}
		}
	}
	{//龙谷掠夺活动
		const Dragon_Prey_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_dragon_prey_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY);
		if(conf && act_id) {
			for(Dragon_Prey_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit == active_content_detail_->common_receive_id[act_id].end()) {
					//判断是否通关
					if(active_content_detail_->dragon_prey_num >= iter->second.num) {
						++msg.num;
					}
				}
			}
		}
	}
	if(msg.num) {
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Active_Content::req_active_content_limit_buy(void) {
	MSG_50106420 msg;
	msg.reset();
	msg.limit_buy_end_time = ACTIVE_CONTENT_MANAGER->get_limit_buy_end_time().sec();
	// 后面新加了数量限制
	std::vector<Goods_Info> &goods_list = MALL_MANAGER->get_promo_goods().goods_list;
	for(std::vector<Goods_Info>::iterator it = goods_list.begin(); it != goods_list.end(); ++it) {
		int32_t sa_num =  MALL_MANAGER->get_num_by_item(it->id);
		int32_t sp_num = MALL_MANAGER->get_num_by_role_id(it->id, player_self()->role_id());
		if(it->all_num == 0 || it->all_num > sa_num) {
			Goods_Info gi = *it;
			if(it->all_num) {
				gi.all_num = it->all_num -sa_num;
			}
			if(it->per_num) {
				gi.per_num = it->per_num -sp_num;
			}
			msg.limit_buy_info.push_back(gi);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_vip_club(void) {
	MSG_50106421 msg;
	msg.reset();
	msg.vip_club_end_time = ACTIVE_CONTENT_MANAGER->get_vip_club_end_time().sec();
	const Vip_Club_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_vip_club_conf_map();
	{//VIP俱乐部-vip等级对应进度
		if(conf) {
			Uint_Uint uu;
			for(Vip_Club_Conf_Map::const_iterator dit = conf->begin(); dit != conf->end(); ++dit) {
				uu.reset();
				uu.val_1 = dit->second.vip_level;
				uu.val_2 = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(dit->second.vip_level);
				msg.vip_club_progress.push_back(uu);
			}
		}
	}
	{//vip俱乐部-奖励ID对应状态, 1.已经领取; 2.可以领取; 3.不能领取
		if(conf) {
			Uint_Uint uu;
			for(Vip_Club_Conf_Map::const_iterator citer = conf->begin(); citer != conf->end(); ++citer) {
				for(Vip_Club_Item_Conf_Map::const_iterator iiter = citer->second.reward.begin();
						iiter != citer->second.reward.end(); ++iiter) {
					uu.reset();
					uu.val_1 = iiter->second.reward_id;
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->vip_club_receive_id.find(iiter->second.reward_id);
					if(tit != active_content_detail_->vip_club_receive_id.end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						//判断次数
						const uint32_t num = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(citer->second.vip_level);
						if(num >= citer->second.needs_num && player_self()->vip() >= iiter->second.limit_vip_level) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.vip_club_reward.push_back(uu);
				}
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_level_ace(void) {
	MSG_50106422 msg;
	msg.reset();
	const Level_Ace_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_level_ace_conf_map();
	if(conf) {
		msg.level_ace_end_time = ACTIVE_CONTENT_MANAGER->get_level_ace_end_time().sec();
		Uint_Uint uu;
		for(Level_Ace_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			uu.reset();
			uu.val_1 = iter->second.level;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->level_ace_receive_level.find(iter->second.level);
			if(tit != active_content_detail_->level_ace_receive_level.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断等级
				if((uint32_t)player_self()->level() >= iter->second.level) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.leve_ace_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_suit_collect(void) {
	MSG_50106423 msg;
	msg.reset();
	const Suit_Collect_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_conf_map(player_self()->career());
	if(conf) {
		msg.suit_collect_end_time = ACTIVE_CONTENT_MANAGER->get_suit_collect_end_time().sec();
		for(Suit_Collect_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
			Uint_Uint uu;
			uu.val_1 = cit->second.level;
			//判断是否可领
			UInt_Set::iterator tit = active_content_detail_->suit_collect_receive_level.find(cit->second.level);
			if(tit != active_content_detail_->suit_collect_receive_level.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
					//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
					//曾经拥有过就算
					const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
					if(suit.find(*nit) == suit.end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						break;
					}
				}
			}
			msg.suit_collect_reward.push_back(uu);
			for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
				//if(player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) || player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
				//曾经拥有过就算
				const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
				if(suit.find(*nit) != suit.end()) {
					msg.suit_collect_id.push_back(*nit);
				}
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_guild_level(void) {
	MSG_50106424 msg;
	msg.reset();
	const Guild_Level_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_guild_level_conf_map();
	if(conf) {
		msg.guild_level_end_time = ACTIVE_CONTENT_MANAGER->get_guild_level_end_time().sec();
		for(Guild_Level_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
			Uint_Uint uu;
			uu.val_1 = cit->second.id;
			//判断是否可领
			UInt_Set::iterator tit = active_content_detail_->guild_level_receive_id.find(cit->second.id);
			if(tit != active_content_detail_->guild_level_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				if((uint32_t)player_self()->get_gang_level() >= cit->second.level) {
					if(cit->second.type == 1 && !player_self()->get_is_level_lader(cit->second.level)) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					}
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.guild_level_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_gem_levelup(void) {
	MSG_50106428 msg;
	const Gem_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_gem_levelup_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_GEM_UPGRADE);
	if(conf && act_id) {
		msg.gem_levelup_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		for(Gem_Levelup_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
			if(cit->second.act_id && cit->second.act_id != act_id) {
				continue;
			}
			Uint_Uint uu;
			uu.val_1 = cit->second.id;
			uint32_t count = player_self()->get_active_content_rune_by_level(cit->second.level);
			//判断是否可领
			UInt_Set::iterator tit = active_content_detail_->gem_levelup_receive_id.find(cit->second.id);
			if(tit != active_content_detail_->gem_levelup_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				if(count >= cit->second.count) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.gem_levelup_reward.push_back(uu);
			uu.val_2 = count;
			msg.gem_levelup_progress.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_wing_levelup(void) {
	MSG_50106429 msg;
	const Wing_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_levelup_conf_map();
	if(conf) {
		msg.wing_levelup_end_time = ACTIVE_CONTENT_MANAGER->get_wing_levelup_end_time().sec();
		for(Wing_Levelup_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
			Uint_Uint uu;
			uu.val_1 = cit->second.level;
			//判断是否可领
			UInt_Set::iterator tit = active_content_detail_->wing_levelup_receive_level.find(cit->second.level);
			if(tit != active_content_detail_->wing_levelup_receive_level.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				if((uint32_t)player_self()->wing_detail().lv >= cit->second.level) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.wing_levelup_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_mount_levelup(void) {
	MSG_50106430 msg;
	const Mount_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_levelup_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP);
	if(conf && act_id) {
		msg.mount_levelup_end_time =  ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		Uint_Uint uu;
		for(Mount_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			uint32_t level = player_self()->get_mount_star_level_by_mount_id(iter->second.mount_id);
			//判断是否可领
			UInt_Set::iterator tit = active_content_detail_->mount_levelup_receive_id.find(iter->second.id);
			if(tit != active_content_detail_->mount_levelup_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				if(level >= iter->second.level) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.mount_levelup_reward.push_back(uu);
			uu.val_2 = level;
			msg.mount_levelup_progress.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_level_rank(void) {
	MSG_50106425 msg;
	msg.reset();
	msg.level_rank_end_time = ACTIVE_CONTENT_MANAGER->get_level_rank_end_time().sec();
	msg.level_rank_count_time = ACTIVE_CONTENT_MANAGER->get_level_rank_count_time().sec();

	if(ACTIVE_CONTENT_MANAGER->is_count_level_rank()) {
		int rank = ACTIVE_CONTENT_MANAGER->get_level_rank_on_count_rank(player_self()->role_id());
		if(rank != -1)
			msg.level_rank = rank;
		ACTIVE_CONTENT_MANAGER->get_three_level_rank_on_count(msg.level_rank_role_info);
	} else {
		int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING, player_self()->role_id());
		if(rank != -1)
			msg.level_rank = rank;
		Role_Vec role_id_set;
		RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING, role_id_set);
		Role_Info role;
		for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
			fill_role_info(role, *rit);
			msg.level_rank_role_info.push_back(role);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_arena_rank(void) {
	MSG_50106426 msg;
	msg.reset();
	msg.arena_rank_end_time = ACTIVE_CONTENT_MANAGER->get_arena_rank_end_time().sec();
	msg.arena_rank_count_time = ACTIVE_CONTENT_MANAGER->get_arena_rank_count_time().sec();
	if(ACTIVE_CONTENT_MANAGER->is_count_arena_rank()) {
		int rank = ACTIVE_CONTENT_MANAGER->get_arena_rank_on_count_rank(player_self()->role_id());
		if(rank != -1)
			msg.arena_rank = rank;
		ACTIVE_CONTENT_MANAGER->get_three_arena_rank_on_count(msg.arena_rank_role_info);
	} else {
		msg.arena_rank = player_self()->local_rank();
		Arena_Manager::Arena_Ranking_Vec &arena_rank = ARENA_MANAGER->arena_ranking_vec();
		Role_Info role;
		int num = 1;
		for(Arena_Manager::Arena_Ranking_Vec::const_iterator rit = arena_rank.begin(); rit != arena_rank.end() && num <= 3; ++rit) {
			fill_role_info(role, (*rit)->role_id());
			msg.arena_rank_role_info.push_back(role);
			++num;
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_force_rank(void) {
	MSG_50106427 msg;
	msg.reset();
	msg.force_rank_end_time = ACTIVE_CONTENT_MANAGER->get_force_rank_end_time().sec();
	msg.force_rank_count_time = ACTIVE_CONTENT_MANAGER->get_force_rank_count_time().sec();
	if(ACTIVE_CONTENT_MANAGER->is_count_force_rank()) {
		int rank = ACTIVE_CONTENT_MANAGER->get_force_rank_on_count_rank(player_self()->role_id());
		if(rank != -1)
			msg.force_rank = rank;
		ACTIVE_CONTENT_MANAGER->get_three_force_rank_on_count(msg.force_rank_role_info);
	} else {
		int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_FORCE_RANKING, player_self()->role_id());
		if(rank != -1)
			msg.force_rank = rank;
		Role_Vec role_id_set;
		RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_FORCE_RANKING, role_id_set);
		Role_Info role;
		for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
			fill_role_info(role, *rit);
			msg.force_rank_role_info.push_back(role);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_arena_dan(void) {
	MSG_50106432 msg;
	const Arena_Dan_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_arena_dan_conf_map();
	if(conf) {
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_arena_dan_last_act_id();
		if(act_id == 0) {
			return 0;
		}
		msg.arena_dan_end_time = ACTIVE_CONTENT_MANAGER->get_arena_dan_end_time().sec();
		Uint_Uint uu;
		for(Arena_Dan_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			//判断该项是否当时活动项目
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			Time_Value time = player_self()->get_arena_dan_level_time(iter->second.group, iter->second.seg);
			long long int overtime = ACTIVE_CONTENT_MANAGER->get_arena_dan_start_time().sec() + Time_Value::ONE_DAY_IN_SECS * iter->second.time;
			//判断是否可领
			UInt_Set::iterator tit = active_content_detail_->arena_dan_receive_id.find(iter->second.id);
			if(tit != active_content_detail_->arena_dan_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				if(time.sec() && time.sec() <= overtime) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.arena_dan_reward.push_back(uu);
			uu.val_2 = overtime;
			msg.arena_dan_progress.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_sg_rank(void) {
	MSG_50106431 msg;
	msg.reset();
	msg.sg_rank_end_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_end_time().sec();
	msg.sg_rank_count_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_count_time().sec();
	if(ACTIVE_CONTENT_MANAGER->is_count_sg_rank()) {
		int rank = ACTIVE_CONTENT_MANAGER->get_sg_rank_on_count_rank(player_self()->role_id());
		if(rank != -1)
			msg.sg_rank = rank;
		ACTIVE_CONTENT_MANAGER->get_three_sg_rank_on_count(msg.sg_rank_role_info);
	} else {
		int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_DUNGEON_SG_RANKING, player_self()->role_id());
		if(rank != -1)
			msg.sg_rank = rank;
		Role_Vec role_id_set;
		RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_DUNGEON_SG_RANKING, role_id_set);
		Role_Info role;
		for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
			fill_role_info(role, *rit);
			msg.sg_rank_role_info.push_back(role);
		}
	}

	const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_pass_conf_map();
	if(conf) {
		Uint_Uint uu;
		for(Sg_Pass_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->sg_pass_receive_id.find(iter->second.id);
			if(tit != active_content_detail_->sg_pass_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->is_elite_dungeon_complete(iter->second.sg_id)) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.sg_rank_reward.push_back(uu);
		}
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_mount_rank(void) {
	MSG_50106448 msg;
	msg.reset();
	msg.mount_rank_end_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_end_time().sec();
	msg.mount_rank_count_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_count_time().sec();
	if(ACTIVE_CONTENT_MANAGER->is_count_sg_rank()) {
		int rank = ACTIVE_CONTENT_MANAGER->get_sg_rank_on_count_rank(player_self()->role_id());
		if(rank != -1)
			msg.mount_rank = rank;
		ACTIVE_CONTENT_MANAGER->get_three_mount_rank_on_count(msg.mount_rank_role_info);
	} else {
		int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_MOUNT_RANKING, player_self()->role_id());
		if(rank != -1)
			msg.mount_rank = rank;
		Role_Vec role_id_set;
		RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_MOUNT_RANKING, role_id_set);
		Role_Info role;
		for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
			fill_role_info(role, *rit);
			msg.mount_rank_role_info.push_back(role);
		}
	}

	const Mount_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_pass_conf_map();
	if(conf) {
		Uint_Uint uu;
		for(Mount_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->mount_pass_receive_id.find(iter->second.id);
			if(tit != active_content_detail_->mount_pass_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				uint32_t level = player_self()->get_mount_star_level_by_mount_id(iter->second.mount_id);
				//判断是否通关
				if(level >= iter->second.level) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.mount_rank_reward.push_back(uu);
		}
	}


	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_wing_rank(void) {
	MSG_50106449 msg;
	msg.reset();
	msg.wing_rank_end_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_end_time().sec();
	msg.wing_rank_count_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_count_time().sec();
	if(ACTIVE_CONTENT_MANAGER->is_count_sg_rank()) {
		int rank = ACTIVE_CONTENT_MANAGER->get_sg_rank_on_count_rank(player_self()->role_id());
		if(rank != -1)
			msg.wing_rank = rank;
		ACTIVE_CONTENT_MANAGER->get_three_wing_rank_on_count(msg.wing_rank_role_info);
	} else {
		int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_WING_RANKING, player_self()->role_id());
		if(rank != -1)
			msg.wing_rank = rank;
		Role_Vec role_id_set;
		RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_WING_RANKING, role_id_set);
		Role_Info role;
		for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
			fill_role_info(role, *rit);
			msg.wing_rank_role_info.push_back(role);
		}
	}

	const Wing_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_pass_conf_map();
	if(conf) {
		Uint_Uint uu;
		for(Wing_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			uu.reset();
			uu.val_1 = iter->first;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->wing_pass_receive_id.find(iter->first);
			if(tit != active_content_detail_->wing_pass_receive_id.end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				int lv = player_self()->wing_detail().lv;
				const Wing_Config *win_conf = CONFIG_CACHE_WING->get_wing_config(lv);
				if(win_conf && (uint32_t)win_conf->show_stage >= iter->second.level) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.wing_rank_reward.push_back(uu);
		}
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}


int Active_Content::req_active_content_info() {
	clear_active_content();
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106400 msg;
	msg.reset();
	msg.limit_buy_end_time = ACTIVE_CONTENT_MANAGER->get_limit_buy_end_time().sec();
	// 后面新加了数量限制
	if(msg.limit_buy_end_time > now.sec()) {
		std::vector<Goods_Info> &goods_list = MALL_MANAGER->get_promo_goods().goods_list;
		for(std::vector<Goods_Info>::iterator it = goods_list.begin(); it != goods_list.end(); ++it) {
			int32_t sa_num =  MALL_MANAGER->get_num_by_item(it->id);
			int32_t sp_num = MALL_MANAGER->get_num_by_role_id(it->id, player_self()->role_id());
			if(it->all_num == 0 || it->all_num > sa_num) {
				Goods_Info gi = *it;
				if(it->all_num) {
					gi.all_num = it->all_num -sa_num;
				}
				if(it->per_num) {
					gi.per_num = it->per_num -sp_num;
				}
				msg.limit_buy_info.push_back(gi);
			}
		}
	}
	msg.vip_club_end_time = ACTIVE_CONTENT_MANAGER->get_vip_club_end_time().sec();
	if(msg.vip_club_end_time > now.sec()) {
		const Vip_Club_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_vip_club_conf_map();
		{//VIP俱乐部-vip等级对应进度
			if(conf) {
				Uint_Uint uu;
				for(Vip_Club_Conf_Map::const_iterator dit = conf->begin(); dit != conf->end(); ++dit) {
					uu.reset();
					uu.val_1 = dit->second.vip_level;
					uu.val_2 = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(dit->second.vip_level);
					msg.vip_club_progress.push_back(uu);
				}
			}
		}

		{//vip俱乐部-奖励ID对应状态, 1.已经领取; 2.可以领取; 3.不能领取
			if(conf) {
				Uint_Uint uu;
				for(Vip_Club_Conf_Map::const_iterator citer = conf->begin(); citer != conf->end(); ++citer) {
					for(Vip_Club_Item_Conf_Map::const_iterator iiter = citer->second.reward.begin();
							iiter != citer->second.reward.end(); ++iiter) {
						uu.reset();
						uu.val_1 = iiter->second.reward_id;
						//判断是否有领取过
						UInt_Set::iterator tit = active_content_detail_->vip_club_receive_id.find(iiter->second.reward_id);
						if(tit != active_content_detail_->vip_club_receive_id.end()) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
						} else {
							//判断次数
							const uint32_t num = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(citer->second.vip_level);
							if(num >= citer->second.needs_num && player_self()->vip() >= iiter->second.limit_vip_level) {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
							} else {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							}
						}
						msg.vip_club_reward.push_back(uu);
					}
				}
			}
		}
	}
	{//冲级高手
		const Level_Ace_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_level_ace_conf_map();
		if(conf) {
			msg.level_ace_end_time = ACTIVE_CONTENT_MANAGER->get_level_ace_end_time().sec();
			if(msg.level_ace_end_time > now.sec()) {
				Uint_Uint uu;
				for(Level_Ace_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					uu.reset();
					uu.val_1 = iter->second.level;
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->level_ace_receive_level.find(iter->second.level);
					if(tit != active_content_detail_->level_ace_receive_level.end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						//判断等级
						if((uint32_t)player_self()->level() >= iter->second.level) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.leve_ace_reward.push_back(uu);
				}
			}
		}
	}
	{//套装收集
		const Suit_Collect_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_conf_map(player_self()->career());
		if(conf) {
			msg.suit_collect_end_time = ACTIVE_CONTENT_MANAGER->get_suit_collect_end_time().sec();
			if(msg.suit_collect_end_time > now.sec()) {
			for(Suit_Collect_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				Uint_Uint uu;
				uu.val_1 = cit->second.level;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->suit_collect_receive_level.find(cit->second.level);
				if(tit != active_content_detail_->suit_collect_receive_level.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
						//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
						//曾经拥有过就算
						const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
						if(suit.find(*nit) == suit.end()) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							break;
						}
					}
				}
				msg.suit_collect_reward.push_back(uu);
				for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
					//if(player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) || player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
					//曾经拥有过就算
					const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
					if(suit.find(*nit) != suit.end()) {
						msg.suit_collect_id.push_back(*nit);
					}
				}
			}
		}
		}
	}
	{//工会等级
		const Guild_Level_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_guild_level_conf_map();
		if(conf) {
			msg.guild_level_end_time = ACTIVE_CONTENT_MANAGER->get_guild_level_end_time().sec();
			if(msg.guild_level_end_time > now.sec()) {
			for(Guild_Level_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				Uint_Uint uu;
				uu.val_1 = cit->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->guild_level_receive_id.find(cit->second.id);
				if(tit != active_content_detail_->guild_level_receive_id.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if((uint32_t)player_self()->get_gang_level() >= cit->second.level) {
						if(cit->second.type == 1 && !player_self()->get_is_level_lader(cit->second.level)) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						}
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.guild_level_reward.push_back(uu);
			}
		}
		}
	}
	{//宝石升级
		const Gem_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_gem_levelup_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_GEM_UPGRADE);
		if(conf && act_id) {
			msg.gem_levelup_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.gem_levelup_end_time > now.sec()) {
			for(Gem_Levelup_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				if(cit->second.act_id && cit->second.act_id != act_id) {
					continue;
				}
				Uint_Uint uu;
				uu.val_1 = cit->second.id;
				uint32_t count = player_self()->get_active_content_rune_by_level(cit->second.level);
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->gem_levelup_receive_id.find(cit->second.id);
				if(tit != active_content_detail_->gem_levelup_receive_id.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(count >= cit->second.count) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.gem_levelup_reward.push_back(uu);
				uu.val_2 = count;
				msg.gem_levelup_progress.push_back(uu);
			}
		}
		}
	}
	{//等级排行
		msg.level_rank_end_time = ACTIVE_CONTENT_MANAGER->get_level_rank_end_time().sec();
		if(msg.level_rank_end_time > now.sec()) {
		msg.level_rank_count_time = ACTIVE_CONTENT_MANAGER->get_level_rank_count_time().sec();
		if(ACTIVE_CONTENT_MANAGER->is_count_level_rank()) {
			int rank = ACTIVE_CONTENT_MANAGER->get_level_rank_on_count_rank(player_self()->role_id());
			if(rank != -1)
				msg.level_rank = rank;
			ACTIVE_CONTENT_MANAGER->get_three_level_rank_on_count(msg.level_rank_role_info);
		} else {
			int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING, player_self()->role_id());
			if(rank != -1)
				msg.level_rank = rank;
			Role_Vec role_id_set;
			RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING, role_id_set);
			Role_Info role;
			for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
				fill_role_info(role, *rit);
				msg.level_rank_role_info.push_back(role);
			}
		}
		}
	}
	{//竞技场排名
		msg.arena_rank_end_time = ACTIVE_CONTENT_MANAGER->get_arena_rank_end_time().sec();
		if(msg.arena_rank_end_time > now.sec()) {
		msg.arena_rank_count_time = ACTIVE_CONTENT_MANAGER->get_arena_rank_count_time().sec();
		if(ACTIVE_CONTENT_MANAGER->is_count_arena_rank()) {
			int rank = ACTIVE_CONTENT_MANAGER->get_arena_rank_on_count_rank(player_self()->role_id());
			if(rank != -1)
				msg.arena_rank = rank;
			ACTIVE_CONTENT_MANAGER->get_three_arena_rank_on_count(msg.arena_rank_role_info);
		} else {
			msg.arena_rank = player_self()->local_rank();
			Arena_Manager::Arena_Ranking_Vec &arena_rank = ARENA_MANAGER->arena_ranking_vec();
			Role_Info role;
			int num = 1;
			for(Arena_Manager::Arena_Ranking_Vec::const_iterator rit = arena_rank.begin(); rit != arena_rank.end() && num <= 3; ++rit) {
				fill_role_info(role, (*rit)->role_id());
				msg.arena_rank_role_info.push_back(role);
				++num;
			}
		}
		}
	}
	{//战斗力排名
		msg.force_rank_end_time = ACTIVE_CONTENT_MANAGER->get_force_rank_end_time().sec();
		if(msg.force_rank_end_time > now.sec()) {
		msg.force_rank_count_time = ACTIVE_CONTENT_MANAGER->get_force_rank_count_time().sec();
		if(ACTIVE_CONTENT_MANAGER->is_count_force_rank()) {
			int rank = ACTIVE_CONTENT_MANAGER->get_force_rank_on_count_rank(player_self()->role_id());
			if(rank != -1)
				msg.force_rank = rank;
			ACTIVE_CONTENT_MANAGER->get_three_force_rank_on_count(msg.force_rank_role_info);
		} else {
			int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_FORCE_RANKING, player_self()->role_id());
			if(rank != -1)
				msg.force_rank = rank;
			Role_Vec role_id_set;
			RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_FORCE_RANKING, role_id_set);
			Role_Info role;
			for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
				fill_role_info(role, *rit);
				msg.force_rank_role_info.push_back(role);
			}
		}
		}
	}
	{//上古副本排名
		msg.sg_rank_end_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_end_time().sec();
		if(msg.sg_rank_end_time > now.sec()) {
		msg.sg_rank_count_time = ACTIVE_CONTENT_MANAGER->get_sg_rank_count_time().sec();
		if(ACTIVE_CONTENT_MANAGER->is_count_sg_rank()) {
			int rank = ACTIVE_CONTENT_MANAGER->get_sg_rank_on_count_rank(player_self()->role_id());
			if(rank != -1)
				msg.sg_rank = rank;
			ACTIVE_CONTENT_MANAGER->get_three_sg_rank_on_count(msg.sg_rank_role_info);
		} else {
			int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_DUNGEON_SG_RANKING, player_self()->role_id());
			if(rank != -1)
				msg.sg_rank = rank;
			Role_Vec role_id_set;
			RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_DUNGEON_SG_RANKING, role_id_set);
			Role_Info role;
			for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
				fill_role_info(role, *rit);
				msg.sg_rank_role_info.push_back(role);
			}
		}

		const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_pass_conf_map();
		if(conf) {
			Uint_Uint uu;
			for(Sg_Pass_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->sg_pass_receive_id.find(iter->second.id);
				if(tit != active_content_detail_->sg_pass_receive_id.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->is_elite_dungeon_complete(iter->second.sg_id)) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.sg_rank_reward.push_back(uu);
			}
		}

		}
	}
	{//坐骑排名
		msg.mount_rank_end_time = ACTIVE_CONTENT_MANAGER->get_mount_rank_end_time().sec();
		if(msg.mount_rank_end_time > now.sec()) {
			msg.mount_rank_count_time = ACTIVE_CONTENT_MANAGER->get_mount_rank_count_time().sec();
			if(ACTIVE_CONTENT_MANAGER->is_count_mount_rank()) {
				int rank = ACTIVE_CONTENT_MANAGER->get_mount_rank_on_count_rank(player_self()->role_id());
				if(rank != -1)
					msg.mount_rank = rank;
				ACTIVE_CONTENT_MANAGER->get_three_mount_rank_on_count(msg.mount_rank_role_info);
			} else {
				int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_MOUNT_RANKING, player_self()->role_id());
				if(rank != -1)
					msg.mount_rank = rank;
				Role_Vec role_id_set;
				RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_MOUNT_RANKING, role_id_set);
				Role_Info role;
				for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
					fill_role_info(role, *rit);
					msg.mount_rank_role_info.push_back(role);
				}
			}

			const Mount_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_pass_conf_map();
			if(conf) {
				Uint_Uint uu;
				for(Mount_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					uu.reset();
					uu.val_1 = iter->second.id;
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->mount_pass_receive_id.find(iter->second.id);
					if(tit != active_content_detail_->mount_pass_receive_id.end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						uint32_t level = player_self()->get_mount_star_level_by_mount_id(iter->second.mount_id);
						//判断是否通关
						if(level >= iter->second.level) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.mount_rank_reward.push_back(uu);
				}
			}

		}
	}
	{//翅膀排名
		msg.wing_rank_end_time = ACTIVE_CONTENT_MANAGER->get_wing_rank_end_time().sec();
		if(msg.wing_rank_end_time > now.sec()) {
			msg.wing_rank_count_time = ACTIVE_CONTENT_MANAGER->get_wing_rank_count_time().sec();
			if(ACTIVE_CONTENT_MANAGER->is_count_wing_rank()) {
				int rank = ACTIVE_CONTENT_MANAGER->get_wing_rank_on_count_rank(player_self()->role_id());
				if(rank != -1)
					msg.wing_rank = rank;
				ACTIVE_CONTENT_MANAGER->get_three_wing_rank_on_count(msg.wing_rank_role_info);
			} else {
				int rank = RANKING_MANAGER->find_rank(RankingDef::LOCAL_PERSONAL_WING_RANKING, player_self()->role_id());
				if(rank != -1)
					msg.wing_rank = rank;
				Role_Vec role_id_set;
				RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_WING_RANKING, role_id_set);
				Role_Info role;
				for(Role_Vec::iterator rit = role_id_set.begin(); rit != role_id_set.end(); ++rit) {
					fill_role_info(role, *rit);
					msg.wing_rank_role_info.push_back(role);
				}
			}

			const Wing_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_pass_conf_map();
			if(conf) {
				Uint_Uint uu;
				for(Wing_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					uu.reset();
					uu.val_1 = iter->first;
					//判断是否有领取过
					UInt_Set::iterator tit = active_content_detail_->wing_pass_receive_id.find(iter->first);
					if(tit != active_content_detail_->wing_pass_receive_id.end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						//判断是否通关
						int lv = player_self()->wing_detail().lv;
						const Wing_Config *win_conf = CONFIG_CACHE_WING->get_wing_config(lv);
						if(win_conf && (uint32_t)win_conf->show_stage >= iter->second.level) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.wing_rank_reward.push_back(uu);
				}
			}

		}
	}
	{//翅膀升级
		const Wing_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_levelup_conf_map();
		if(conf) {
			msg.wing_levelup_end_time = ACTIVE_CONTENT_MANAGER->get_wing_levelup_end_time().sec();
			if(msg.wing_levelup_end_time > now.sec()) {
			Uint_Uint uu;
			for(Wing_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				uu.reset();
				uu.val_1 = iter->second.level;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->wing_levelup_receive_level.find(iter->second.level);
				if(tit != active_content_detail_->wing_levelup_receive_level.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断等级
					if((uint32_t)player_self()->wing_detail().lv >= iter->second.level) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.wing_levelup_reward.push_back(uu);
			}
		}
		}
	}
	{//坐骑升级
		const Mount_Levelup_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_levelup_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LEVELUP);
		if(conf && act_id) {
			msg.mount_levelup_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.mount_levelup_end_time > now.sec()) {
			Uint_Uint uu;
			for(Mount_Levelup_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				uint32_t level = player_self()->get_mount_star_level_by_mount_id(iter->second.mount_id);
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->mount_levelup_receive_id.find(iter->second.id);
				if(tit != active_content_detail_->mount_levelup_receive_id.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(level >= iter->second.level) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.mount_levelup_reward.push_back(uu);
				uu.val_2 = level;
				msg.mount_levelup_progress.push_back(uu);
			}
		}
		}
	}
	{//竞技场段位
		const Arena_Dan_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_arena_dan_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_arena_dan_last_act_id();
		if(conf && act_id) {
			msg.arena_dan_end_time = ACTIVE_CONTENT_MANAGER->get_arena_dan_end_time().sec();
			if(msg.arena_dan_end_time > now.sec()) {
			Uint_Uint uu;
			for(Arena_Dan_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				//判断该项是否当时活动项目
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				Time_Value time = player_self()->get_arena_dan_level_time(iter->second.group, iter->second.seg);
				long long int overtime = ACTIVE_CONTENT_MANAGER->get_arena_dan_start_time().sec() + Time_Value::ONE_DAY_IN_SECS * iter->second.time;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->arena_dan_receive_id.find(iter->second.id);
				if(tit != active_content_detail_->arena_dan_receive_id.end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(time.sec() && time.sec() <= overtime) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.arena_dan_reward.push_back(uu);
				uu.val_2 = overtime;
				msg.arena_dan_progress.push_back(uu);
			}
		}
		}
	}
	{//世界boss
		const World_Boss_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_world_boss_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS);
		if(conf && act_id) {
			msg.world_boss_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.world_boss_end_time > now.sec()) {
				msg.world_boss_all_hurt = active_content_detail_->all_world_boss_hurt;
				msg.world_boss_today_hurt = active_content_detail_->today_world_boss_hurt;
				msg.world_boss_rank = active_content_detail_->world_boos_max_rank?active_content_detail_->world_boos_max_rank:INT_MAX;
				Uint_Uint uu;
				for(World_Boss_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					uu.reset();
					uu.val_1 = iter->second.id;
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit != active_content_detail_->common_receive_id[act_id].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						switch(iter->second.type) {
							case 1: {
								if(iter->second.val <= active_content_detail_->all_world_boss_hurt) {
									uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
								} else {
									uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
								}
								break;
							}
							case 2: {
								if(iter->second.val <= active_content_detail_->today_world_boss_hurt) {
									uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
								} else {
									uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
								}
								break;
							}
							case 3: {
								if(iter->second.sval < active_content_detail_->world_boos_max_rank && iter->second.val >= active_content_detail_->world_boos_max_rank && active_content_detail_->world_boos_max_rank) {
									uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
								} else {
									uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
								}
								break;
							}
							default: {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
								break;
							}
						}
					}
					msg.world_boss_reward.push_back(uu);
				}
			}
		}
	}
	{//套装收集二段
		const Suit_Collect_Two_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_two_conf_map(player_self()->career());
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_SUIT_COLLECT_REWARD);
		if(conf && act_id) {
			msg.suit_collect_two_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.suit_collect_two_end_time > now.sec()) {
				for(Suit_Collect_Two_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
					if(cit->second.act_id && cit->second.act_id != act_id) {
						continue;
					}
					Uint_Uint uu;
					uu.val_1 = cit->second.id;
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(cit->second.id);
					if(tit != active_content_detail_->common_receive_id[act_id].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
							//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
							//曾经拥有过就算
							const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
							if(suit.find(*nit) == suit.end()) {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
								break;
							}
						}
					}
					msg.suit_collect_two_reward.push_back(uu);
					for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
						//if(player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) || player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
						//曾经拥有过就算
						const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
						if(suit.find(*nit) != suit.end()) {
							msg.suit_collect_two_id.push_back(*nit);
						}
					}
				}
			}
		}
	}
	{//英雄之门
		const Heroes_Door_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_heroes_door_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_HEROES_DOOR);
		if(conf && act_id) {
			msg.heroes_door_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.heroes_door_end_time > now.sec()) {
				Uint_Uint uu;
				for(Heroes_Door_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					uu.reset();
					uu.val_1 = iter->second.id;
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit != active_content_detail_->common_receive_id[act_id].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						if(active_content_detail_->heroes_door_type_level[iter->second.type].find(iter->second.level) != active_content_detail_->heroes_door_type_level[iter->second.type].end()) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.heroes_door_reward.push_back(uu);
				}
			}
		}
	}
	{//组队竞技
		const Team_Arena_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_team_arena_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA);
		if(conf && act_id) {
			msg.team_arena_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.team_arena_end_time > now.sec()) {
				msg.team_arena_win_num = active_content_detail_->team_arena_win_num;
				Uint_Uint uu;
				for(Team_Arena_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					uu.reset();
					uu.val_1 = iter->second.id;
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit != active_content_detail_->common_receive_id[act_id].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						if(iter->second.num <= active_content_detail_->team_arena_win_num) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.team_arena_reward.push_back(uu);
				}
			}
		}
	}
	{//战场
		const Warer_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_warer_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER);
		if(conf && act_id) {
			msg.warer_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.warer_end_time > now.sec()) {
				msg.warer_win_num = active_content_detail_->warer_win_num;
				Uint_Uint uu;
				for(Warer_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					uu.reset();
					uu.val_1 = iter->second.id;
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit != active_content_detail_->common_receive_id[act_id].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						if(iter->second.num <= active_content_detail_->warer_win_num) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.warer_reward.push_back(uu);
				}
			}
		}
	}
	{//悬赏
		const Reward_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_reward_task_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK);
		if(conf && act_id) {
			msg.reward_task_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			if(msg.reward_task_end_time > now.sec()) {
				msg.reward_task_done_num = active_content_detail_->reward_task_done_num;
				Uint_Uint uu;
				for(Reward_Task_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					if(iter->second.act_id && iter->second.act_id != act_id) {
						continue;
					}
					uu.reset();
					uu.val_1 = iter->second.id;
					//判断是否可领
					UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
					if(tit != active_content_detail_->common_receive_id[act_id].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
					} else {
						if(iter->second.num <= active_content_detail_->reward_task_done_num) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
						} else {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
						}
					}
					msg.reward_task_reward.push_back(uu);
				}
			}
		}
	}
	{//上古副本
		const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_fb_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_SGFB);
		if(conf && act_id) {
			msg.sg_fb_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			Uint_Uint uu;
			for(Sg_Pass_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->is_elite_dungeon_complete(iter->second.sg_id)) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.sg_fb_reward.push_back(uu);
			}
		}
	}
	{//累计附魔x次
		const Fm_X_Ci_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fm_x_ci_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI);
		if(conf && act_id) {
			msg.fm_x_ci_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.fm_x_ci_num = player_self()->active_content_detail().enchanting_num;
			Uint_Uint uu;
			for(Fm_X_Ci_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->active_content_detail().enchanting_num >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.fm_x_ci_reward.push_back(uu);
			}
		}
	}
	{//累计X个X品质英雄
		const X_X_Quality_Heroer_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_x_x_quality_heroer_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER);
		if(conf && act_id) {
			msg.x_x_quality_heroer_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		//	msg.fm_x_ci_num = player_self()->active_content_detail().enchanting_num;
			UInt_UInt_Map type_num_map;
			player_self()->get_heroer_type_num_map(type_num_map);
			Uint_Uint uu;
			for(X_X_Quality_Heroer_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(type_num_map[iter->second.type] >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.x_x_quality_heroer_reward.push_back(uu);
			}
		}
	}
	{//坐骑进阶X次
		const Mount_Lvup_X_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_lvup_x_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X);
		if(conf && act_id) {
			msg.mount_lvup_x_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.mount_lvup_x_num = player_self()->active_content_detail().mount_lvup_num;
			Uint_Uint uu;
			for(Mount_Lvup_X_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->active_content_detail().mount_lvup_num >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.mount_lvup_x_reward.push_back(uu);
			}
		}
	}
	{//合成X张精英卡牌
		const Fusion_X_Elite_Card_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fusion_x_elite_card_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE);
		if(conf && act_id) {
			msg.fusion_x_elite_card_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.fusion_x_elite_card_num = player_self()->active_content_detail().fusion_x_elite_card_num;
			Uint_Uint uu;
			for(Fusion_X_Elite_Card_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->active_content_detail().fusion_x_elite_card_num >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.fusion_x_elite_card_reward.push_back(uu);
			}
		}
	}
	{//炼金有礼
		const Alchemy_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_alchemy_gift_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT);
		if(conf && act_id) {
			msg.alchemy_gift_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.alchemy_gift_num = player_self()->active_content_detail().alchemy_num;
			Uint_Uint uu;
			for(Alchemy_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->active_content_detail().alchemy_num >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.alchemy_gift_reward.push_back(uu);
			}
		}
	}
	{//活跃度好礼
		const Activity_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_activity_gift_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ACTIVITY_GIFT);
		if(conf && act_id) {
			msg.activity_gift_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.activity_gift_num = player_self()->daily_detail().today_activity;
			Uint_Uint uu;
			for(Activity_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->daily_detail().today_activity >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.activity_gift_reward.push_back(uu);
			}
		}
	}
	{//环任务活动
		const Active_Ring_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_ring_task_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK);
		if(conf && act_id) {
			msg.ring_task_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.ring_task_num = player_self()->tasker_detail().ring_num;
			Uint_Uint uu;
			for(Active_Ring_Task_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(player_self()->tasker_detail().ring_num >= iter->second.num && (iter->second.recharge == 0 || active_content_detail_->ring_task_recharge)) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.ring_task_reward.push_back(uu);
			}
		}
	}
	{//龙谷掠夺活动
		const Dragon_Prey_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_dragon_prey_conf_map();
		const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY);
		if(conf && act_id) {
			msg.dragon_prey_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
			msg.dragon_prey_num = active_content_detail_->dragon_prey_num;
			Uint_Uint uu;
			for(Dragon_Prey_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否有领取过
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					//判断是否通关
					if(active_content_detail_->dragon_prey_num >= iter->second.num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.dragon_prey_reward.push_back(uu);
			}
		}
	}
	{//悬赏大放送-结束时间   && 上古福利-结束时间 && 在线礼包
		msg.reward_task_fs_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(ACTION_ACTIVE_CONTENT_REWARD_TASK_FS).sec();
		msg.sg_welfare_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(ACTION_ACTIVE_CONTENT_SG_WELFARE).sec();
		msg.online_reward_gift_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(ACTION_ACTIVE_CONTENT_ONLINE_REWARD_GIFT).sec();
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_world_boss(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106433 msg;
	const World_Boss_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_world_boss_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS);
	if(conf && act_id) {
		msg.world_boss_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(msg.world_boss_end_time > now.sec()) {
			msg.world_boss_all_hurt = active_content_detail_->all_world_boss_hurt;
			msg.world_boss_today_hurt = active_content_detail_->today_world_boss_hurt;
			msg.world_boss_rank = active_content_detail_->world_boos_max_rank;
			Uint_Uint uu;
			for(World_Boss_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					switch(iter->second.type) {
						case 1: {
							if(iter->second.val <= active_content_detail_->all_world_boss_hurt) {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
							} else {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							}
							break;
						}
						case 2: {
							if(iter->second.val <= active_content_detail_->today_world_boss_hurt) {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
							} else {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							}
							break;
						}
						case 3: {
							if(iter->second.sval < active_content_detail_->world_boos_max_rank && iter->second.val >= active_content_detail_->world_boos_max_rank && active_content_detail_->world_boos_max_rank) {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
							} else {
								uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							}
							break;
						}
						default: {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							break;
						}
					}
				}
				msg.world_boss_reward.push_back(uu);
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_suit_collect_two(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106434 msg;
	const Suit_Collect_Two_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_two_conf_map(player_self()->career());
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_SUIT_COLLECT_REWARD);
	if(conf && act_id) {
		msg.suit_collect_two_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(msg.suit_collect_two_end_time > now.sec()) {
			for(Suit_Collect_Two_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				if(cit->second.act_id && cit->second.act_id != act_id) {
					continue;
				}
				Uint_Uint uu;
				uu.val_1 = cit->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(cit->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
						//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
						//曾经拥有过就算
						const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
						if(suit.find(*nit) == suit.end()) {
							uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
							break;
						}
					}
				}
				msg.suit_collect_two_reward.push_back(uu);
				for(UInt_Set::const_iterator nit = cit->second.suit_collect_item.begin(); nit != cit->second.suit_collect_item.end(); ++nit) {
					//if(player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) || player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
					//曾经拥有过就算
					const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
					if(suit.find(*nit) != suit.end()) {
						msg.suit_collect_two_id.push_back(*nit);
					}
				}
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_heroes_door(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106435 msg;
	const Heroes_Door_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_heroes_door_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_HEROES_DOOR);
	if(conf && act_id) {
		msg.heroes_door_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(msg.heroes_door_end_time > now.sec()) {
			Uint_Uint uu;
			for(Heroes_Door_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(active_content_detail_->heroes_door_type_level[iter->second.type].find(iter->second.level) != active_content_detail_->heroes_door_type_level[iter->second.type].end()) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.heroes_door_reward.push_back(uu);
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_team_arena(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106436 msg;
	const Team_Arena_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_team_arena_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA);
	if(conf && act_id) {
		msg.team_arena_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(msg.team_arena_end_time > now.sec()) {
			msg.team_arena_win_num = active_content_detail_->team_arena_win_num;
			Uint_Uint uu;
			for(Team_Arena_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(iter->second.num <= active_content_detail_->team_arena_win_num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.team_arena_reward.push_back(uu);
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_warer(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106437 msg;
	const Warer_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_warer_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER);
	if(conf && act_id) {
		msg.warer_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(msg.warer_end_time > now.sec()) {
			msg.warer_win_num = active_content_detail_->warer_win_num;
			Uint_Uint uu;
			for(Warer_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(iter->second.num <= active_content_detail_->warer_win_num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.warer_reward.push_back(uu);
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_reward_task(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106438 msg;
	const Reward_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_reward_task_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK);
	if(conf && act_id) {
		msg.reward_task_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		if(msg.reward_task_end_time > now.sec()) {
			msg.reward_task_done_num = active_content_detail_->reward_task_done_num;
			Uint_Uint uu;
			for(Reward_Task_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				if(iter->second.act_id && iter->second.act_id != act_id) {
					continue;
				}
				uu.reset();
				uu.val_1 = iter->second.id;
				//判断是否可领
				UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
				if(tit != active_content_detail_->common_receive_id[act_id].end()) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
				} else {
					if(iter->second.num <= active_content_detail_->reward_task_done_num) {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
					} else {
						uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
					}
				}
				msg.reward_task_reward.push_back(uu);
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_sgfb(void) {
	Time_Value now = Time_Value::gettimeofday();
	MSG_50106439 msg;
	const Sg_Pass_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_fb_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_SGFB);
	if(conf && act_id) {
		msg.sg_fb_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		Uint_Uint uu;
		for(Sg_Pass_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->is_elite_dungeon_complete(iter->second.sg_id)) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.sg_fb_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_fm_x_ci(void) {
	MSG_50106440 msg;
	const Fm_X_Ci_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fm_x_ci_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI);
	if(conf && act_id) {
		msg.fm_x_ci_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.fm_x_ci_num = player_self()->active_content_detail().enchanting_num;
		Uint_Uint uu;
		for(Fm_X_Ci_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->active_content_detail().enchanting_num >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.fm_x_ci_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_x_x_quality_heroer(void) {
	MSG_50106441 msg;
	const X_X_Quality_Heroer_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_x_x_quality_heroer_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER);
	if(conf && act_id) {
		msg.x_x_quality_heroer_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
	//	msg.fm_x_ci_num = player_self()->active_content_detail().enchanting_num;
		UInt_UInt_Map type_num_map;
		player_self()->get_heroer_type_num_map(type_num_map);
		Uint_Uint uu;
		for(X_X_Quality_Heroer_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(type_num_map[iter->second.type] >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.x_x_quality_heroer_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_mount_lvup_x(void) {
	MSG_50106442 msg;
	const Mount_Lvup_X_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_lvup_x_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X);
	if(conf && act_id) {
		msg.mount_lvup_x_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.mount_lvup_x_num = player_self()->active_content_detail().mount_lvup_num;
		Uint_Uint uu;
		for(Mount_Lvup_X_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->active_content_detail().mount_lvup_num >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.mount_lvup_x_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_fusion_x_elite_card(void) {
	MSG_50106443 msg;
	const Fusion_X_Elite_Card_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fusion_x_elite_card_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE);
	if(conf && act_id) {
		msg.fusion_x_elite_card_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.fusion_x_elite_card_num = player_self()->active_content_detail().fusion_x_elite_card_num;
		Uint_Uint uu;
		for(Fusion_X_Elite_Card_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->active_content_detail().fusion_x_elite_card_num >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.fusion_x_elite_card_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_alchemy_gift(void) {
	MSG_50106444 msg;
	const Alchemy_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_alchemy_gift_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT);
	if(conf && act_id) {
		msg.alchemy_gift_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.alchemy_gift_num = player_self()->active_content_detail().alchemy_num;
		Uint_Uint uu;
		for(Alchemy_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->active_content_detail().alchemy_num >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.alchemy_gift_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_activity_gift(void) {
	MSG_50106445 msg;
	const Activity_Gift_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_activity_gift_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_ACTIVITY_GIFT);
	if(conf && act_id) {
		msg.activity_gift_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.activity_gift_num = player_self()->daily_detail().today_activity;
		Uint_Uint uu;
		for(Activity_Gift_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->daily_detail().today_activity >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.activity_gift_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_ring_task(void) {
	MSG_50106446 msg;
	const Active_Ring_Task_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_ring_task_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK);
	if(conf && act_id) {
		msg.ring_task_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.ring_task_num = player_self()->tasker_detail().ring_num;
		Uint_Uint uu;
		for(Active_Ring_Task_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(player_self()->tasker_detail().ring_num >= iter->second.num && (iter->second.recharge == 0 || active_content_detail_->ring_task_recharge)) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.ring_task_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_active_content_dragon_prey(void) {
	MSG_50106447 msg;
	const Dragon_Prey_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_dragon_prey_conf_map();
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY);
	if(conf && act_id) {
		msg.dragon_prey_end_time = ACTIVE_CONTENT_MANAGER->get_common_end_time(act_id).sec();
		msg.dragon_prey_num = active_content_detail_->dragon_prey_num;
		Uint_Uint uu;
		for(Dragon_Prey_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
			if(iter->second.act_id && iter->second.act_id != act_id) {
				continue;
			}
			uu.reset();
			uu.val_1 = iter->second.id;
			//判断是否有领取过
			UInt_Set::iterator tit = active_content_detail_->common_receive_id[act_id].find(iter->second.id);
			if(tit != active_content_detail_->common_receive_id[act_id].end()) {
				uu.val_2 = ACTIVE_CONTENT_RECEIVE_ED;
			} else {
				//判断是否通关
				if(active_content_detail_->dragon_prey_num >= iter->second.num) {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_CAN;
				} else {
					uu.val_2 = ACTIVE_CONTENT_RECEIVE_NO;
				}
			}
			msg.dragon_prey_reward.push_back(uu);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Active_Content::req_receive_active_content_reward(uint32_t act_type, uint32_t id) {
	const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(act_type);
	if(!act_id) { return 0; }
	switch(act_type) {
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_WORLD_BOSS: {//世界BOSS
			const World_Boss_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_world_boss_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				switch(conf->type) {
					case 1: {
						if(conf->val > active_content_detail_->all_world_boss_hurt) {
							return ERROR_CLIENT_PARAM;
						}
						break;
					}
					case 2: {
						if(conf->val > active_content_detail_->today_world_boss_hurt) {
							return ERROR_CLIENT_PARAM;
						}
						break;
					}
					case 3: {
						if(conf->sval >= active_content_detail_->world_boos_max_rank || conf->val < active_content_detail_->world_boos_max_rank || active_content_detail_->world_boos_max_rank == 0) {
							return ERROR_CLIENT_PARAM;
						}
						break;
					}
					default: {
						return 0;
						break;
					}
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_WORLD_BOSS), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_WORLD_BOSS));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_SUIT_COLLECT_REWARD: {//.套装收集2段,
			const Suit_Collect_Two_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_suit_collect_two_conf_by_level(id, player_self()->career());
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否收集完
				bool receive = true;
				for(UInt_Set::const_iterator nit = conf->suit_collect_item.begin(); nit != conf->suit_collect_item.end(); ++nit) {
					//if(!player_self()->pack_calc_item(Pack::PACK_T_EQUIP_INDEX, *nit) && !player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, *nit)) {
					//曾经拥有过就算
					const std::map<int, int> &suit = player_self()->active_content_detail().suit_cnt_map;
					if(suit.find(*nit) == suit.end()) {
						receive = false;
						break;
					}
				}
				if(!receive) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_SUIT_CONLLECT_TWO), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_SUIT_CONLLECT_TWO));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_HEROES_DOOR: {//.英雄之门,
			const Heroes_Door_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_heroes_door_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(active_content_detail_->heroes_door_type_level[conf->type].find(conf->level) == active_content_detail_->heroes_door_type_level[conf->type].end()) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_HEROES_DOOR), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_HEROES_DOOR));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA: { //.组队竞技,
			const Team_Arena_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_team_arena_conf_by_id(id);
			if(conf) {
				if(conf->act_id && act_id != conf->act_id) {
					return 0;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(conf->num > active_content_detail_->team_arena_win_num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_TEAM_ARENA), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_TEAM_ARENA));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_WARER: { //.战场,
			const Warer_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_warer_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(conf->num > active_content_detail_->warer_win_num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_WARER), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_WARER));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_REWARD_TASK: {//悬赏,
			const Active_Content_Reward_Task_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_reward_task_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(conf->num > active_content_detail_->reward_task_done_num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_REWARD_TASK), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_REWARD_TASK));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_SGFB: {//上古副本
			const Sg_Pass_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_fb_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(!player_self()->is_elite_dungeon_complete(conf->sg_id)) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_SGFB), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_SGFB));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_FM_X_CI: {
			const Fm_X_Ci_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fm_x_ci_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(player_self()->active_content_detail().enchanting_num < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_FM_X_CI), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_FM_X_CI));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_X_X_QUALITY_HEROER: {
			const X_X_Quality_Heroer_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_x_x_quality_heroer_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				UInt_UInt_Map type_num_map;
				player_self()->get_heroer_type_num_map(type_num_map);
				//判断是否通关
				if(type_num_map[conf->type] < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_X_X_QUALITY_HEROER), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_X_X_QUALITY_HEROER));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X: {//10.坐骑进阶X次,
			const Mount_Lvup_X_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_lvup_x_conf_by_id(id);
			const uint32_t act_id = ACTIVE_CONTENT_MANAGER->get_comm_act_type_id(ACTIVE_CONTENT_REWARD_ACT_TYPE_MOUNT_LVUP_X);
			if(conf && act_id) {
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(player_self()->active_content_detail().mount_lvup_num < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_MOUNT_LVUP_X), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_MOUNT_LVUP_X));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_FUSION_X_ELITE: {//合成X张精英卡牌
			const Fusion_X_Elite_Card_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_fusion_x_elite_card_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(player_self()->active_content_detail().fusion_x_elite_card_num < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_FUSION_X_ELITE_CARD), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_FUSION_X_ELITE_CARD));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_ALCHEMY_GIFT: {//炼金有礼
			const Alchemy_Gift_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_alchemy_gift_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(player_self()->active_content_detail().alchemy_num < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_ALCHEMY_GIFT), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_ALCHEMY_GIFT));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_ACTIVITY_GIFT: {//活跃度好礼
			const Activity_Gift_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_activity_gift_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(player_self()->daily_detail().today_activity < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_ACTIVITY_GIFT), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_ACTIVITY_GIFT));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_RING_TASK: {//环任务奖励活动
			const Active_Ring_Task_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_ring_task_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(player_self()->tasker_detail().ring_num < conf->num || (conf->recharge && !active_content_detail_->ring_task_recharge)) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_RING_TASK), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_RING_TASK));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		case ACTIVE_CONTENT_REWARD_ACT_TYPE_DRAGON_PREY: {//龙谷掠夺活动
			const Dragon_Prey_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_dragon_prey_conf_by_id(id);
			if(conf) {
				if(conf->act_id && conf->act_id != act_id) {
					return ERROR_CLIENT_PARAM;
				}
				//判断是否已经领取
				UInt_Set::iterator rit = active_content_detail_->common_receive_id[act_id].find(id);
				if(rit != active_content_detail_->common_receive_id[act_id].end()) {
					return ERROR_REWARD_ALREADY_RECEIVE;
				}
				//判断是否通关
				if(active_content_detail_->dragon_prey_num < conf->num) {
					return ERROR_CLIENT_PARAM;
				}
				//领取奖励
				int result = receive_active_content_reward(conf->reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_ACTIVE_CONTENT_DRAGON_PREY), Money_DM_Info(MONEY_ADD_ACTIVE_CONTENT_DRAGON_PREY));
				if(result) {
					return result;
				}
				//添加已领项
				active_content_detail_->common_receive_id[act_id].insert(conf->id);
				active_content_detail_->detail_change();
			}
			break;
		}
		default: {
			return 0;
			break;
		}
	}
	MSG_50106411 msg;
	msg.act_type = act_type;
	msg.id = id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Active_Content::active_listen_level_up(const uint32_t level) {
	//开服活动
//	Icon_Time_Info icon = ACTIVE_CONTENT_MANAGER->get_icon_time_status();
//	if(active_content_detail_->is_open_icon_id.find(icon.icon_id) == active_content_detail_->is_open_icon_id.end()) {
//		if(icon.over_time.sec() > Time_Value::gettimeofday().sec()
//		&& ACTIVE_CONTENT_MANAGER->get_icon_limit_level() <= level) {
//			send_active_content_open_icon(icon.icon_id);
//		}
//	}
	//公用图标控制
	Common_Active_Icon_Info_Map &comm_icon = ACTIVE_CONTENT_MANAGER->get_common_icon();
	for(Common_Active_Icon_Info_Map::iterator it = comm_icon.begin(); it != comm_icon.end(); ++it) {
		if(active_content_detail_->is_open_icon_id.find(it->second.icon_id) == active_content_detail_->is_open_icon_id.end()) {
			if(it->second.over_time.sec() > Time_Value::gettimeofday().sec()
				&& it->second.level_limit <= level) {
					int time =   it->second.down_time.sec() - Time_Value::gettimeofday().sec();
					if(time < 0) { time = 0; }
					send_active_content_open_icon(it->second.icon_id, time, it->second.active);
				}
		}
	}
}

void Active_Content::get_heroer_type_num_map(UInt_UInt_Map &map) {
	map.clear();
	for(Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin(); it != player_self()->heroer_detail().hero_map.end(); ++it) {
		++map[it->second.awake_lvl];
		if(it->second.awake_lvl) {
			for(int i = it->second.awake_lvl - 1; i >= 0; --i) {
				++map[i];
			}
		}
	}
}

void Active_Content::active_listen_recharge(void) {
	active_content_detail_->ring_task_recharge = true;
}

//int Active_Content::req_limit_buy(uint32_t id, uint32_t num) {
//	const Limit_Buy_Config *info = ACTIVE_CONTENT_MANAGER->get_limit_buy_info(id);
//	if(info) {
//		//判断时间
//		{
//			Time_Value now = Time_Value::gettimeofday();
//			Time_Value start_time = ACTIVE_CONTENT_MANAGER->get_limit_buy_start_time();
//			//Time_Value end_time = ACTIVE_CONTENT_MANAGER->get_limit_buy_end_time();
//			if(now.sec() < (start_time.sec() + info->start_time * Time_Value::ONE_MINUTE_IN_SECS)
//					|| now.sec() > (start_time.sec() + info->start_time * Time_Value::ONE_MINUTE_IN_SECS + info->continue_time * Time_Value::ONE_MINUTE_IN_SECS)) {
//				return ERROR_CLIENT_PARAM;
//			}
//		}
//		MSG_50106401 msg;
//		msg.reset();
//		//判断次数
//		if(info < num) {
//			msg.info = *info;
//			THIS_SEND_TO_CLIENT(msg);
//			return ERROR_LIMIT_BUY_OVER_NUM;
//		}
//		//添加物品到背包
//		Item_Detail item =  Item_Detail(info->item_id, info->num, Item_Detail::BIND);
//		player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(ITEM_GAIN_LIMIT_BUY));
//
//	}
//	return 0;
//}

