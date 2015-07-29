/*
 * Equiper.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Item.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "Equiper_Def.h"
#include "Equiper_Struct.h"
#include "Equiper.h"
#include "Equiper_Func.h"
#include "Msg_Equip_Struct.h"
#include "Config_Cache_Equip.h"
#include "Config_Cache.h"
#include "Fighter_Detail.h"
#include "ranking/Ranking_Manager.h"
#include "Utility_Func.h"
#include "mail/Mail_Struct.h"
#include "Msg_Equip_Enum.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Config_Cache_Role.h"
#include "Cache_Base.h"
#include "announce/Logic_Announce.h"
#include "fighter/Fighter_Struct.h"
#include "fashion/Fashion.h"
#include "public/Public_Def.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "mall/Mall_Manager.h"
#include "oper_activity/Oper_Activity_Def.h"

Equiper::Equiper(void) {
	reset();
}

Equiper::~Equiper() {

}

void Equiper::reset(void) {
	equiper_detail_ = 0;
	equiper_breakdown_prop.clear();
}

int Equiper::create_init(Equiper_Detail &detail) {

	return 0;
}

void Equiper::load_detail(Equiper_Detail &detail) {
	equiper_detail_ = &detail;
}

void Equiper::save_detail(Equiper_Detail &detail) {
	if (equiper_detail().is_change) {
		detail = equiper_detail();
		equiper_detail_->is_change = false;
	}
}


void Equiper::module_init(void) {
	// 身上
	Int_Vec equip_index;
	get_equip_index(PACK_T_EQUIP_INDEX, equip_index);
	Item_Detail *equip = NULL;
	for (Int_Vec::const_iterator it = equip_index.begin(); it != equip_index.end(); ++it) {
		equip = this->pack_get_item(*it);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (!item_config || !item_config->equip_cfg.part) {
			continue;
		}

		// 过期
		if (equip->overdue_time > Time_Value::zero && equip->overdue_time <= Time_Value::gettimeofday()) {
			equiper_detail_->notified_overdue_idx.insert(equip->index_);
		}

		refresh_equip_force(equip);
	}

	equiper_prop_refresh(10006);

	init_suit_skill();
	init_equips_self_skills(PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN, PACK_T_EQUIP_INDEX + EQUIP_SUIT_END);

}

void Equiper::sign_in(void) {
	//this->create_event(Logic_Enum::EVENT_LOGIC_EQUIPER_TICK, reinterpret_cast<event_fun>(&Logic_Player::equiper_tick), Time_Value(EQUIPER_TICK_SEC));
	//this->create_event(Logic_Enum::EVENT_LOGIC_EQUIPER_TICK, &Equiper::equiper_tick, Time_Value(EQUIPER_TICK_SEC));
}

void Equiper::sign_out(void) {

}

int Equiper::client_ready_now(void) {

	return 0;
}

void Equiper::equiper_tick(void) {
	// 勋章过期
	Item_Detail *equip = this->pack_get_item(EQUIP_IDX_MEDAL);
	if (! equip) {
		return;
	}

	if (!equiper_detail_->notified_overdue_idx.count(equip->index_) &&
			equip->overdue_time > Time_Value::zero && equip->overdue_time <= Time_Value::gettimeofday()) {
		// notify client
		equiper_prop_refresh(10005);
		equiper_detail_->notified_overdue_idx.insert(equip->index_);
	}
	// 英雄勋章
	if(player_self()){
		Heroer_Detail::Hero_Map::const_iterator it;
		for(it = player_self()->heroer_detail().hero_map.begin();
				it != player_self()->heroer_detail().hero_map.end(); ++it){
			int hero_index = it->second.hero_index;
			int hero_medal_index = PACK_T_HERO_EQUIP_INDEX +
					hero_index * ONE_HERO_EQUIP_GAP+EQUIP_SUIT_END;
			Item_Detail *hero_equip = this->pack_get_item(hero_medal_index);
			if (! hero_equip) {
				continue;
			}
			if (!equiper_detail_->notified_overdue_idx.count(hero_equip->index_) &&
					hero_equip->overdue_time > Time_Value::zero && hero_equip->overdue_time <= Time_Value::gettimeofday()) {
				// notify client
				player_self()->heroer_prop_refresh(hero_index);
				equiper_detail_->notified_overdue_idx.insert(hero_equip->index_);
			}
		}

	}
}

int Equiper::req_equip_on_off(const int index_from, const int index_to) {
	Item_Detail *equip_from = this->pack_get_item(index_from);
	if (! equip_from) {
		return ERROR_ITEM_NOT_EXIST;
	}

	int suit_idx_begin = PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN;
	int suit_idx_end = PACK_T_EQUIP_INDEX + EQUIP_SUIT_END;

	int id_from = equip_from->id_;
	int id_to = 0;
	{
		Item_Detail *equip_to = this->pack_get_item(index_to);
		if (equip_to)
			id_to = equip_to->id_;
	}

	Int_Vec suit_skills = statistic_suit_skill(suit_idx_begin, suit_idx_end);
	Int_Vec equip_self_skills = get_equips_self_skills(suit_idx_begin, suit_idx_end);
	Int_Vec all_skills;
	all_skills.insert(all_skills.end(), suit_skills.begin(), suit_skills.end());
	all_skills.insert(all_skills.end(), equip_self_skills.begin(), equip_self_skills.end());

	/* 脱下装备 */
	if (pack_is_pack_type(index_from, Pack::PACK_T_EQUIP_INDEX) == 0) {
		int32_t item_id = equip_from->id_;
		int32_t item_index = equip_from->index_;

		// equip force refresh
		if (refresh_equip_force(equip_from, true, true)) {
			active_item_addit_info_update(equip_from);
		}

		int result = this->pack_move_item(index_from, Pack::PACK_T_PACKAGE_INDEX);
		if (result != 0) {
			refresh_equip_force(equip_from);
			return result;
		}

		if (equiper_detail_->notified_overdue_idx.count(item_index)) {
			equiper_detail_->notified_overdue_idx.erase(item_index);
		}

		// refresh prop
		equiper_prop_refresh(10014);
		// refresh rune prop
//		player_self()->rune_prop_refresh();
		player_self()->task_listen_role_equip_n();

		//排行榜监听
		RANKING_MANAGER->monitor_equipment_off(player_self()->role_id(), item_id, item_index);


	/* 穿上装备 */
	} else if (pack_is_pack_type(index_from, Pack::PACK_T_PACKAGE_INDEX) == 0 &&
			pack_is_pack_type(index_to, Pack::PACK_T_EQUIP_INDEX) == 0) {
		const Item_Detail_Config *equip_from_cfg = CONFIG_CACHE_ITEM->find_item(equip_from->id_);
		if (! equip_from_cfg) {
			MSG_USER("item config error:%d", equip_from->id_);
			return ERROR_CONFIG_NOT_EXIST;
		}

		// check part
		int part = equip_from_cfg->equip_cfg.part;
		if (! is_equip_part(part)) {
			MSG_USER("item config error:%d", part);
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (! check_equip_idx_by_part(index_to, part)) {
			MSG_USER("item config error:%d", part);
			return ERROR_CLIENT_PARAM;
		}

		// check career
		if (equip_from_cfg->equip_cfg.career && equip_from_cfg->equip_cfg.career != player_self()->career()) {
			std::string name = CONFIG_CACHE_ROLE->get_career_name(career());
			player_self()->send_tip_to_client(ERROR_CAREER_NOT_USE,name);
			return -1;
		}

		if (player_self()->level() < equip_from_cfg->needLevel) {
			player_self()->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT,equip_from_cfg->needLevel);
			return -1;
		}

		// 过期
		if (equip_from->overdue_time > Time_Value::zero && equip_from->overdue_time <= Time_Value::gettimeofday()) {
			return ERROR_ITEM_OVERDUE;
		}

		Item_Detail *equip_to = this->pack_get_item(index_to);
		if (equip_to) {
			RANKING_MANAGER->monitor_equipment_off(player_self()->role_id(), equip_to->id_, equip_to->index_);
		}

		int result = this->pack_move_item(index_from, index_to);
		if (result != 0) {
			return result;
		}

		set_item_bind(equip_from);

		// refresh prop
		equiper_prop_refresh(10023);
		// refresh rune prop
//		player_self()->rune_prop_refresh();
		// equip force refresh
		if (refresh_equip_force(equip_from)) {
			active_item_addit_info_update(equip_from);
		}

		//排行榜监听
		RANKING_MANAGER->monitor_equipment_change(player_self()->role_id(), equip_from);

		if(equip_from) {
			player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_ROLE_HAS_A_EQUIP, equip_from->id_);
		}
		player_self()->achievement_listen_equip_on_body_both_player_and_hero_by_lvl();

		if(equip_from) {
			player_self()->task_listen_role_pull_on_equip(equip_from->id_);
			player_self()->title_listen_cumulate_equip();
			player_self()->task_listen_equip_jewelry();
			player_self()->task_listen_have_equip_but_no_erase();
			player_self()->task_listen_equip_purple_ring();
		}
		player_self()->task_listen_role_equip_n();
	} else {
		return ERROR_CLIENT_OPERATE;
	}

	// task_listen_suit
	player_self()->task_listen_role_hero_suit(id_from, 0);
	player_self()->task_listen_role_hero_suit(id_to, 0);

	Skill_Info_Vec skill_vec;
	for (Int_Vec::iterator it = all_skills.begin(); it != all_skills.end(); ++it) {
		Skill_Info info;
		info.skill_id = *it;
		info.skill_lv = 1;
		skill_vec.push_back(info);
	}
	player_self()->remove_be_skills(skill_vec);

	suit_skills.clear();
	equip_self_skills.clear();
	suit_skills = statistic_suit_skill(suit_idx_begin, suit_idx_end);;
	equip_self_skills = get_equips_self_skills(suit_idx_begin, suit_idx_end);

	all_skills.clear();
	all_skills.insert(all_skills.end(), suit_skills.begin(), suit_skills.end());
	all_skills.insert(all_skills.end(), equip_self_skills.begin(), equip_self_skills.end());

	skill_vec.clear();
	for (Int_Vec::iterator it = all_skills.begin(); it != all_skills.end(); ++it) {
		Skill_Info info;
		info.skill_id = *it;
		info.skill_lv = 1;
		skill_vec.push_back(info);
	}
	player_self()->add_be_skills(skill_vec);

	player_self()->refresh_avatar();

	MSG_50101000 msg;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Equiper::req_equip_unban(const uint32_t index, const uint32_t lucky_stone_id, const uint32_t auto_buy) {
	if (! index) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail *equip = this->pack_get_item(index);
	if (! equip) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != equip->is_item_type(equip->id_, Item_Detail::EQUIP)) {
		return ERROR_CLIENT_OPERATE;
	}

	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (! item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (item_config->equip_cfg.part == EP_MEDAL) {
		return ERROR_PERM_LIMIT;
	}
	if (equip->addon.equip.unban_lvl >= item_config->equip_cfg.max_unban_lvl) {
		return ERROR_UNBAN_LVL_MAX;
	}

	const Item_Detail *lucky_stone = NULL;
	Index_Amount lucky_index_amount(0, 1);
	int lucky_stone_rate = 0;
	if (lucky_stone_id) {
		lucky_stone = this->pack_get_item(PACK_T_PACKAGE_INDEX, lucky_stone_id);
		if (! lucky_stone) {
			return ERROR_ITEM_NOT_EXIST;
		}
		lucky_index_amount.index = lucky_stone->index_;

		const Item_Detail_Config *lucky_stone_cfg = CONFIG_CACHE_ITEM->find_item(lucky_stone->id_);
		if (! lucky_stone_cfg) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		lucky_stone_rate = lucky_stone_cfg->special_cfg.lucky_stone_rate;

		int ret_try = pack_try_erase_item(lucky_index_amount);
		if (ret_try) {
			return ret_try;
		}
	}

	const Equip_Unban_Lvl_Cfg *unban_lvl_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_cfg(equip->addon.equip.unban_lvl + 1);
	if (! unban_lvl_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	// copper
	if (unban_lvl_cfg->cost_copper > 0) {
		Money_Sub_Info sub_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
				unban_lvl_cfg->cost_copper, MONEY_SUB_EQUIP_UNBAN));
		int sub_result = player_self()->pack_try_sub_money(sub_info);
		if (sub_result) {
			return sub_result;
		}
		player_self()->pack_sub_money(sub_info);
	}

	if (lucky_index_amount.index) {
		pack_erase_item(lucky_index_amount, Lose_Item_DM_Info(ITEM_LOSE_EQUIP_UNBAN));
	}

	/*
	 * success_rate
	 */
	int success_rate = unban_lvl_cfg->success_rate;
	if (lucky_stone_rate) {
		success_rate += lucky_stone_rate;
	}
	Int_Vec wight;
	success_rate = success_rate > 100  ?  100 : success_rate;
	success_rate = success_rate < 0  ?  0 : success_rate;
	wight.push_back(100 - success_rate);
	wight.push_back(success_rate);
	int wight_index = U_FUNC::get_rand_index(wight);
	bool is_success = wight_index == 1;

	/*
	 * unban equip
	 */
	if (is_success) {
		++equip->addon.equip.unban_lvl;

		refresh_equip_force(equip);

		if (0 == pack_is_pack_type(index, PACK_T_EQUIP_INDEX)) {
			equiper_prop_refresh(10015);
		} else if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), index)) {
			player_self()->heroer_prop_refresh(player_self()->get_hero_index_by_pack_index(index));
		}

		active_item_addit_info_update(equip);

		//监听玩家强化N次的任务
	    player_self()->task_listen_player_all_equip(1);
	    //监听强化指定部位N次的任务
	    player_self()->task_listen_body_equip(get_part_by_id((int)equip->id_), 1);
		//监听指定部位的装备强化等级
		player_self()->task_listen_body_equip_level(get_part_by_id((int)equip->id_), equip->addon.equip.unban_lvl);
		//监听指定装备的强化等级
		player_self()->task_listen_equip_level(equip->id_, equip->addon.equip.unban_lvl);
		//排行榜监听
		RANKING_MANAGER->monitor_equipment_change(player_self()->role_id(), equip);

		//成就系统监听
		player_self()->achievement_listen_unban(equip->addon.equip.unban_lvl);
	}

	if (equip->bind_ != Item_Detail::BIND) {
		equip->bind_ = Item_Detail::BIND;
		MSG_82100000 item_msg;
		Item_Basic_Info item_basic;
		equip->set_item_basic_info(item_basic);
		item_msg.item_info.push_back(item_basic);
		THIS_SEND_TO_CLIENT(item_msg);
	}

	MSG_50101001 res_msg;
	res_msg.index = index;
	res_msg.result = is_success;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Equiper::equiper_prop_refresh(int from) {
	std::map<int, double> pro_val_map;
	Int_Int_Map stone_color_map;
	Item_Detail *equip = NULL;
	int unban_lvl_whole = 50;
	int master_lvl_whole = 100;
	equiper_detail_->suit_cnt_map.clear();

	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			unban_lvl_whole = index < EQUIP_IDX_SUIT_END ?  0 : unban_lvl_whole;
			master_lvl_whole = index < EQUIP_IDX_SUIT_END ?  0 : master_lvl_whole;
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}

		// 是否过期
		if (equip->overdue_time > Time_Value::zero && equip->overdue_time <= Time_Value::gettimeofday()) {
			continue;
		}

		// 获得装备全身等级（8件）
		if (index < EQUIP_IDX_SUIT_END) {
			unban_lvl_whole = equip->addon.equip.unban_lvl < unban_lvl_whole ?
					equip->addon.equip.unban_lvl : unban_lvl_whole;
			Master_Part_Info_Map::const_iterator it_part = this->equiper_detail_->master_part_info.find(index);
			if (it_part != this->equiper_detail_->master_part_info.end()) {
				master_lvl_whole = it_part->second.level < master_lvl_whole ?
						it_part->second.level : master_lvl_whole;
			} else {
				master_lvl_whole = 0;
			}
		}

		// 获得套装件数
		if (index < EQUIP_IDX_SUIT_END && item_config->equip_cfg.suit_id) {
			++equiper_detail_->suit_cnt_map[item_config->equip_cfg.suit_id];
		}

		// 装备基础属性
		base_prop_refresh(item_config, pro_val_map);

		// 精通加成-装备基础属性
		master_base_prop_refresh(item_config, index, pro_val_map);
		// 精通加成-装备解封属性
//		master_unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, index, pro_val_map);

		// 装备解封属性
		unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, pro_val_map);

		// 装备附魔属性
		enchant_prop_refresh(equip, pro_val_map);

		// 卡牌属性
		player_self()->card_prop_refresh(*equip, pro_val_map);

		// 装备宝石
		gemstone_prop_refresh(*equip, pro_val_map);

		calc_genstone(*equip, stone_color_map);
	}

	// 宝石套装
	gemstone_suit_prop_refresh(stone_color_map, pro_val_map);

	// 全身解封等级属性
	unban_whole_prop_refresh(unban_lvl_whole, pro_val_map);

	// 精通-全身精通达到X级（X可配）将获得装备共鸣
	master_whole_prop_refresh(master_lvl_whole, pro_val_map);

	// 套装属性、技能
	suit_prop_refresh(equiper_detail_->suit_cnt_map, pro_val_map);

	player_self()->refresh_fashion_prop_total(pro_val_map);

	// 人物属性加成
	player_self()->addition_prop_refresh(AT_EQUIP, PST_BASIC, pro_val_map, from);

	return 0;
}

int Equiper::master_base_prop_refresh(const Item_Detail_Config *item_config, const int32_t index, std::map<int, double> &pro_val_map) {
	if (! item_config) {
		MSG_USER("role:%ld", role_id());
		return -1;
	}
	int32_t index_old = index;
	if (index_old > PACK_T_HERO_EQUIP_INDEX) {
		index_old = PACK_T_HERO_EQUIP_INDEX + 1 * ONE_HERO_EQUIP_GAP + index % 100;
	}

	Master_Part_Info_Map::const_iterator it_part = this->equiper_detail_->master_part_info.find(index_old);
	if (it_part == this->equiper_detail_->master_part_info.end()) {
		//MSG_USER("role:%ld %d", role_id(), index_old);
		return -1;
	}
	UInt_UInt_Map::const_iterator it = CONFIG_CACHE_EQUIP->incr_lvl_prop_map().find(it_part->second.level);
	if (it == CONFIG_CACHE_EQUIP->incr_lvl_prop_map().end()
			|| it->second <= 0) {
		MSG_USER("role:%ld %d", role_id(), it_part->second.level);
		return -1;
	}

	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			pro_val_map[cfg_prop_val_vec[i].id] += cfg_prop_val_vec[i].value * it->second * 0.01;
		}
	}

	return 0;
}

int Equiper::master_unban_prop_refresh(const Item_Detail_Config *item_config, const int unban_lvl, const int32_t index, std::map<int, double> &pro_val_map) {
	if (!item_config || !unban_lvl) {
		//MSG_USER("role:%ld %d", role_id(), unban_lvl);
		return -1;
	}
	int32_t index_old = index;
	if (index_old > PACK_T_HERO_EQUIP_INDEX) {
		index_old = PACK_T_HERO_EQUIP_INDEX + 1 * ONE_HERO_EQUIP_GAP + index % 100;
	}

	Master_Part_Info_Map::const_iterator it_part = this->equiper_detail_->master_part_info.find(index_old);
	if (it_part == this->equiper_detail_->master_part_info.end()) {
		//MSG_USER("role:%ld %d", role_id(), index_old);
		return -1;
	}
	const Equip_Unban_Lvl_Cfg *unban_lvl_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_cfg(unban_lvl);
	if (!unban_lvl_cfg || unban_lvl_cfg->add_prop_rate <= 0) {
		MSG_USER("role:%ld %d", role_id(), unban_lvl);
		return -1;
	}
	UInt_UInt_Map::const_iterator it = CONFIG_CACHE_EQUIP->incr_lvl_prop_map().find(it_part->second.level);
	if (it == CONFIG_CACHE_EQUIP->incr_lvl_prop_map().end()
			|| it->second <= 0) {
		MSG_USER("role:%ld %d", role_id(), it_part->second.level);
		return -1;
	}
	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			int prop = cfg_prop_val_vec[i].id;
			double base_val = cfg_prop_val_vec[i].value;
			double add_val = base_val * unban_lvl_cfg->add_prop_rate * 0.01 * it->second * 0.01;
			pro_val_map[prop] += add_val;
		}
	}
	return 0;
}

int Equiper::master_whole_prop_refresh(const int master_whole_lvl, std::map<int, double> &pro_val_map) {
	if (! master_whole_lvl) {
		//MSG_USER("role:%ld %d", role_id(), master_whole_lvl);
		return 0;
	}
	UInt_UInt_Map::const_iterator lvl_whole_cfg = CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().find(master_whole_lvl);
	if (lvl_whole_cfg == CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().end()
			|| lvl_whole_cfg->second <= 0) {
		MSG_USER("role:%ld %d", role_id(), master_whole_lvl);
		return -1;
	}

	Item_Detail *equip = NULL;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		if (index >= EQUIP_IDX_SUIT_END) {
			continue;
		}
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			MSG_USER("role:%ld %d", role_id(), equip->id_);
			continue;
		}

		// 精通加成-装备基础属性
		master_whole_base_prop_refresh(item_config, master_whole_lvl, pro_val_map);
		// 精通加成-装备解封属性
		master_whole_unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, master_whole_lvl, pro_val_map);

		refresh_equip_force(equip);
	}

	return 0;
}

int Equiper::master_whole_base_prop_refresh(const Item_Detail_Config *item_config, const int32_t master_level, std::map<int, double> &pro_val_map) {
	if (! item_config)
		return -1;
	UInt_UInt_Map::const_iterator it = CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().find(master_level);
	if (it == CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().end()
			|| it->second <= 0) {
		return -1;
	}

	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			pro_val_map[cfg_prop_val_vec[i].id] += cfg_prop_val_vec[i].value * it->second * 0.01;
		}
	}

	return 0;
}

int Equiper::master_whole_unban_prop_refresh(const Item_Detail_Config *item_config, const int unban_lvl, const int32_t master_level, std::map<int, double> &pro_val_map) {
	if (!item_config || !unban_lvl) {
		return -1;
	}
	const Equip_Unban_Lvl_Cfg *unban_lvl_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_cfg(unban_lvl);
	if (!unban_lvl_cfg || unban_lvl_cfg->add_prop_rate <= 0) {
		return -1;
	}
	UInt_UInt_Map::const_iterator it = CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().find(master_level);
	if (it == CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().end()
			|| it->second <= 0) {
		return -1;
	}
	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			int prop = cfg_prop_val_vec[i].id;
			double base_val = cfg_prop_val_vec[i].value;
			double add_val = base_val * unban_lvl_cfg->add_prop_rate * 0.01 * it->second * 0.01;
			pro_val_map[prop] += add_val;
		}
	}
	return 0;
}

int Equiper::base_prop_refresh(const Item_Detail_Config *item_config, std::map<int, double> &pro_val_map) {
	if (! item_config) {
		MSG_USER("role:%ld", role_id());
		return -1;
	}

	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			pro_val_map[cfg_prop_val_vec[i].id] += cfg_prop_val_vec[i].value;
		}
	}

	return 0;
}

int Equiper::unban_prop_refresh(const Item_Detail_Config *item_config, const int unban_lvl, std::map<int, double> &pro_val_map) {
	if (!item_config || !unban_lvl) {
		//MSG_USER("role:%ld %d", role_id(), unban_lvl);
		return -1;
	}
	const Equip_Unban_Lvl_Cfg *unban_lvl_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_cfg(unban_lvl);
	if (!unban_lvl_cfg || unban_lvl_cfg->add_prop_rate <= 0) {
		MSG_USER("role:%ld %d", role_id(), unban_lvl);
		return -1;
	}
	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			int prop = cfg_prop_val_vec[i].id;
			double base_val = cfg_prop_val_vec[i].value;
			double add_val = base_val * unban_lvl_cfg->add_prop_rate * 0.01;
			pro_val_map[prop] += add_val;
		}
	}
	return 0;
}

int Equiper::unban_whole_prop_refresh(const int unban_whole_lvl, std::map<int, double> &pro_val_map) {

	const Equip_Unban_Lvl_Whole_Cfg_Map &whole_map = CONFIG_CACHE_EQUIP->get_unban_lvl_whole_cfg_map();
	for (Equip_Unban_Lvl_Whole_Cfg_Map::const_iterator it = whole_map.begin(); it != whole_map.end(); ++it) {
		if (it->first > unban_whole_lvl) continue;
		for (Int_Double_Map::const_iterator iter = it->second.prop_val_map.begin(); iter != it->second.prop_val_map.end(); ++iter) {
			const int &id = iter->first;
			const double &value = iter->second;
			pro_val_map[id] += value;
		}
	}
//	if (! unban_whole_lvl) {
//		return 0;
//	}
//	const Equip_Unban_Lvl_Whole_Cfg *lvl_whole_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_whole_cfg(unban_whole_lvl);
//	if (!lvl_whole_cfg || lvl_whole_cfg->prop_val_map.empty()) {
//		return -1;
//	}
//
//	for (Int_Double_Map::const_iterator it_prop_val = lvl_whole_cfg->prop_val_map.begin();
//			it_prop_val != lvl_whole_cfg->prop_val_map.end(); ++it_prop_val) {
//		int prop = it_prop_val->first;
//		double val = it_prop_val->second;
//		pro_val_map[prop] += val;
//	}
	return 0;
}

void Equiper::active_item_addit_info_update(const Item_Detail *item) {
	if (! item)
		return;
	MSG_82100004 active_msg;
	active_msg.reset();
	active_msg.type = PACK_TYPE;
	Item_Equip_Info equip_info;
	// 获取卡牌信息
	item->addon.equip.make_item_equip_info(equip_info, *this);
	active_msg.equip_addit.push_back(equip_info);
	active_msg.equip_addit.rbegin()->index = item->index_;
	THIS_SEND_TO_CLIENT(active_msg);
}

void Equiper::set_item_bind(Item_Detail *item, bool notify) {
	if (!item || item->bind_ == Item_Detail::BIND) {
		return;
	}
	item->bind_ = Item_Detail::BIND;

	if (notify) {
		MSG_82100000 item_msg;
		Item_Basic_Info item_basic;
		item->set_item_basic_info(item_basic);
		item_msg.item_info.push_back(item_basic);
		THIS_SEND_TO_CLIENT(item_msg);
	}
}

int Equiper::active_equip_master_info(Logic_Player* target_player) {
	if (! target_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	MSG_82210003 active_msg;
	active_msg.target_role_id = target_player->role_id();
	Master_Part_Info info;
	for (Master_Part_Info_Map::const_iterator it = target_player->equiper_detail().master_part_info.begin();
			it != target_player->equiper_detail().master_part_info.end(); ++it) {
		info.reset();
		info.index = it->second.index;
		info.level = it->second.level;
		info.luck_value = it->second.luck_value;
		UInt_UInt_Map::const_iterator it_fail_add_rate = CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().find(it->second.level);
		if (it_fail_add_rate != CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().end()) {
			info.fail_add_rate = it_fail_add_rate->second * it->second.fail_cnt;
		}
		active_msg.part_infos.push_back(info);
	}
	THIS_SEND_TO_CLIENT(active_msg);
	return 0;
}

int Equiper::req_equip_master(const uint32_t index, const uint8_t auto_buy) {
	if ((auto_buy == 0 || auto_buy ==1) == false)
		return ERROR_CLIENT_PARAM;

	if (!((index >= Equiper_Detail::ROLE_START && index <= Equiper_Detail::ROLE_END)
			|| (index >= Equiper_Detail::HERO_START && index <= Equiper_Detail::HERO_END))) {
		return ERROR_CLIENT_PARAM;
	}
	uint32_t index_old = index;

	MSG_50101002 res_msg;
	res_msg.reset();

	int is_success = 0;
	Master_Part_Info_Map::iterator master_it = this->equiper_detail_->master_part_info.find(index);
	if (master_it == this->equiper_detail_->master_part_info.end()) {
		Master_Part_Info_Detail info;
		info.index = index;
		std::pair<Master_Part_Info_Map::iterator, bool> pr = this->equiper_detail_->master_part_info.insert(std::make_pair(index, info));
		if (!pr.second) {
			return ERROR_INNER_SERVER;
		}
		master_it = pr.first;
	}

	if (static_cast<uint32_t>(master_it->second.level) > CONFIG_CACHE_EQUIP->incr_max_lvl())
		return ERROR_EQUIP_MASTER_MAX_LVL;

	int role_lv_limit = CONFIG_CACHE_EQUIP->get_role_lv_limit(master_it->second.level);
	if (level() < role_lv_limit)  {
		this->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT, role_lv_limit);
		return 0;
	}

	int result = equip_master(master_it->second, auto_buy, is_success, res_msg.copper, res_msg.item_vec);
	if (result != 0) return result;


	res_msg.item_vec.clear();
	res_msg.copper = 0;


	equiper_detail_->detail_change();

	if (0 == is_success) {
		if (index >= Equiper_Detail::HERO_START && index <= Equiper_Detail::HERO_END) {
			index_old = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP + index % 100;
		}
		Item_Detail *equip = this->pack_get_item(index_old);
		if (equip) {
			refresh_equip_force(equip);

			if (0 == pack_is_pack_type(index_old, PACK_T_EQUIP_INDEX)) {
				equiper_prop_refresh(10013);
			} else if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), index_old)) {
				player_self()->heroer_prop_refresh(player_self()->get_hero_index_by_pack_index(index_old));
			}

			active_item_addit_info_update(equip);
		}

		//成就监听
		player_self()->achievement_listen_master(master_it->second.level);
		if (CONFIG_CACHE_EQUIP->need_broadcast_set().count(master_it->second.level) > 0) {
			player_self()->create_ann_equip_master(player_self(), index, master_it->second.level);
		}
	}

	res_msg.result = (0 == is_success) ? 1 : 0;
	res_msg.level = master_it->second.level;
	res_msg.luck_value = master_it->second.luck_value;
	UInt_UInt_Map::const_iterator it_fail_add_rate = CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().find(master_it->second.level);
	if (it_fail_add_rate != CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().end()) {
		res_msg.fail_add_rate = it_fail_add_rate->second * master_it->second.fail_cnt;
	}
	res_msg.index = index;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Equiper::req_equip_compose(const int32_t level_up_id, const int amount, const int8_t use_diamond) {
	if (! level_up_id) {
		return ERROR_CLIENT_PARAM;
	}
	int amount_bak = amount;
	if (amount_bak <= 0) {
		amount_bak = 1;
	}

	const Item_Detail_Config *lvl_up_cfg = CONFIG_CACHE_ITEM->find_item(level_up_id);
	if (! lvl_up_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Equip_Compose_Id_Cost::const_iterator it_cost = CONFIG_CACHE_EQUIP->level_up_id_cost_map().find(level_up_id);
	if (it_cost == CONFIG_CACHE_EQUIP->level_up_id_cost_map().end()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	Equip_Compose_Id_Cost::const_iterator it_cost_equip = CONFIG_CACHE_EQUIP->level_up_id_cost_equip_map().find(level_up_id);
	Id_Amount equip_all;
	if (it_cost_equip != CONFIG_CACHE_EQUIP->level_up_id_cost_equip_map().end()) {
		for (Item_Info_Vec::const_iterator it = it_cost_equip->second.item_vec.begin(); it != it_cost_equip->second.item_vec.end(); ++it) {
			equip_all.id = (*it).id;
			equip_all.amount = (*it).amount;
			break;
		}
	}
	Item_Detail *equip = 0;
	bool need_broadcase = false;
	if (equip_all.id > 0) {
		if (equip_all.id && amount_bak > 1) { // 策划说只做道具的
			return ERROR_CLIENT_OPERATE;
		}
		equip = this->pack_get_item(PACK_T_EQUIP_INDEX, equip_all.id);
		if (!equip) {
			for (Heroer_Detail::Hero_Map::const_iterator it = player_self()->heroer_detail().hero_map.begin(); it != player_self()->heroer_detail().hero_map.end(); ++it) {
				equip = this->pack_get_item_hero(PACK_T_HERO_EQUIP_INDEX + it->first * ONE_HERO_EQUIP_GAP, equip_all.id);
				if (equip) {
					if (it->first == player_self()->heroer_detail().fighter_index) {
						need_broadcase = true;
					}
					break;
				}
			}
		} else {
			need_broadcase = true;
		}
		if (!equip) {
			equip = this->pack_get_item(PACK_T_PACKAGE_INDEX, equip_all.id);
		}
		if (!equip) {
			MSG_82100008 acv_msg;
			int ret = ERROR_ITEM_NOT_ENOUGH;
			acv_msg.item_ids.push_back(equip_all.id);
			THIS_SEND_TO_CLIENT(acv_msg);
			if (0 != ret) {
				return ERROR_ITEM_NOT_ENOUGH;
			}
		}
	}

	int need_money = 0;
	int ret = 0;
	Id_Amount_Vec cost_all;
	if (!it_cost->second.item_vec.empty()) {
		Id_Amount it_amount;
		for (Item_Info_Vec::const_iterator it = it_cost->second.item_vec.begin(); it != it_cost->second.item_vec.end(); ++it) {
			it_amount.id = (*it).id;
			it_amount.amount = (*it).amount * amount_bak;
			cost_all.push_back(it_amount);
		}

		Id_Amount_Vec id_amount_need;
		if(use_diamond > 0) {
			Id_Amount it_amount;
			for (Id_Amount_Vec::iterator it = cost_all.begin(); it != cost_all.end();) {
				int amount = pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, (*it).id);
				if((*it).amount > amount) {
					it_amount.amount = (*it).amount - amount;
					it_amount.id = (*it).id;
					id_amount_need.push_back(it_amount);

					(*it).amount = amount;
				}

				if (amount == 0) {
					it = cost_all.erase(it);
				} else {
					it++;
				}
			}

			if (!id_amount_need.empty()) {
				int sum_material = 0;
				Equip_Compose_Id_Cost::const_iterator it_cost = CONFIG_CACHE_EQUIP->level_up_id_cost_map().find(level_up_id);
				if (it_cost != CONFIG_CACHE_EQUIP->level_up_id_cost_map().end()) {
					sum_material =  it_cost->second.item_id_amount.amount;
				}
				int price = MALL_MANAGER->find_goods_price_by_tabs_and_id(2, it_cost->second.item_id_amount.id);
				need_money = sum_material * price * it_amount.amount;

				int ret = pack_try_sub_money(Money_Sub_Info(BIND_GOLD_FIRST, need_money, Pack::MONEY_SUB_GEMS));
				if (0 != ret) {
					return ret;
				}
			}
		} else {
			ret = pack_try_erase_item(PACK_T_PACKAGE_INDEX, cost_all);
			if (0 != ret) {
				return -1;
			}
		}
	}

	if (it_cost->second.copper > 0) {
		int return_status = pack_try_sub_money(Money_Sub_Info(COPPER_ONLY, it_cost->second.copper * amount_bak, MONEY_SUB_EQUIP_LEVEL_UP));
		if(return_status != 0) {
			return return_status;
		}
	}

	//  扣除金钱花费
	int cost_copper_i = it_cost->second.copper * amount_bak;
	if (cost_copper_i > 0) {
		Money_Sub_List money_sub_list;
		Money_Sub_Info copper_sub_info(COPPER_ONLY, cost_copper_i, Money_DM_Info(Pack::MONEY_SUB_EQUIP_LEVEL_UP, 0 , 0, level_up_id));
		money_sub_list.push_back(copper_sub_info);

		int sub_result = this->pack_sub_money(money_sub_list);
		if (sub_result != 0) return sub_result;
	}

	if (need_money > 0) {
		Money_Sub_Info money_sub_info(Pack::BIND_GOLD_FIRST, need_money, Money_DM_Info(Pack::MONEY_SUB_GEMS));
		player_self()->pack_sub_money(money_sub_info, Pack::MONEY_OPT_REAL);
	}


	if (equip_all.id > 0 && !equip) {
		return -1;
	}

	if (!cost_all.empty()) {
		ret = Packer::pack_erase_item(PACK_T_PACKAGE_INDEX,
				cost_all, Lose_Item_DM_Info(ITEM_LOSE_EQUIP_LEVEL_UP));
		if (0 != ret) {
			return -1;
		}
	}

	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	bool has_send_tip = false;
	bool has_send_mail = false;
	if (equip) { // 身上装备
		// 检查是否装备，重设卡片数据
		if (Item_Detail::EQUIP == equip->type_) {
			player_self()->reset_card_location(*equip);
		}
		uint32_t old_equip_id = equip->id_;
		equip->id_ = it_cost->second.level_up_id;
		Equip_Addon_Detail ead = equip->addon.equip;
		memset(&equip->addon, 0x00, sizeof(equip->addon));
		if (Item_Detail::EQUIP == equip->type_) {
			equip->addon.equip.unban_lvl = ead.unban_lvl;
			const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(equip_all.id);
			if (ic->color == 4 && lvl_up_cfg->color == 5) {
				equip->addon.equip.cnt_enchant_group = ead.cnt_enchant_group;
				for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP; ++i) {
					equip->addon.equip.enchant_groups[i] = ead.enchant_groups[i];
				}
				for (uint16_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
					equip->addon.equip.enchant_lock_prop_fail[i] = ead.enchant_lock_prop_fail[i];
				}
			}
			for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
				equip->addon.equip.card_hole_array_[i] = ead.card_hole_array_[i];
				equip->addon.equip.card_hole_array_[i].card_index = 0;
			}
			for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
				equip->addon.equip.gemstones[i] = ead.gemstones[i];
			}
		}
		if (need_broadcase) {
			refresh_equip_force(equip);
		}

		Pack_Type pt = PACK_T_EQUIP_INDEX;
		if (0 == pack_is_pack_type(equip->index_, PACK_T_EQUIP_INDEX)) {
			int part = lvl_up_cfg->equip_cfg.part;
			if (! is_equip_part(part)
					|| ! check_equip_idx_by_part(equip->index_, part)
					|| player_self()->level() < lvl_up_cfg->needLevel) {
				///////
				int result = this->pack_move_item(equip->index_, Pack::PACK_T_PACKAGE_INDEX);
				if (result != 0) {
					if (0 == this->pack_try_erase_item(equip->index_)) {
						Item_Vec item_list;
						item_list.push_back(*equip);
						this->pack_erase_item(equip->index_,  Lose_Item_DM_Info(ITEM_LOSE_EQUIP_LEVEL_UP));
						const Item_Gain_Source_Detail *cfg = CONFIG_CACHE_ITEM->find_item_gain_source(ITEM_GAIN_EQUIP_LEVEL_UP);
						if (!cfg) {
							LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
						} else {
							const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
							Mail_Config_Map::const_iterator mailc = mail_config.find(PACK_MAIL_ID_NEW);
							if(mailc == mail_config.end()) {
								LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
								return -1;
							}
							std::string title;
							std::string contents;
							{
								const int size_buf = mailc->second.mail_title.size() + \
										sizeof(cfg->name) * 2 + 10;
								char content_buf[size_buf];
								memset(content_buf, 0, size_buf);
								snprintf(content_buf, size_buf, mailc->second.mail_title.c_str(), cfg->name.c_str());
								title.assign(content_buf);
							}

							{
								const int size_buf = mailc->second.mail_content.size() + \
										sizeof(cfg->name) * 2 + 10;
								char content_buf[size_buf];
								memset(content_buf, 0, size_buf);
								snprintf(content_buf, size_buf, mailc->second.mail_content.c_str(), cfg->name.c_str());
								contents.assign(content_buf);
							}
							if (0 != LOGIC_PUBLIC->send_mail(PACK_MAIL_ID_NEW, role_id(), item_list, title, contents)) {
								LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
							}
						}
						this->send_err_to_client(ERROR_PACK_FULL_BUT_MAIL, REQ_EQUIP_LEVEL_UP);
					}
					has_send_mail = true;
				} else {
					pt = PACK_T_PACKAGE_INDEX;
				}
			} else {
				equiper_prop_refresh(10009);
				//排行榜监听
				RANKING_MANAGER->monitor_equipment_off(player_self()->role_id(), equip_all.id, equip->index_);
				//排行榜监听
				RANKING_MANAGER->monitor_equipment_change(player_self()->role_id(), equip);
				this->send_err_to_client(ERROR_EQUIP_COMPOSE_SUCCESS_ON_BODY, REQ_EQUIP_LEVEL_UP);
				has_send_tip = true;
			}
		} else if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), equip->index_)) {
			int index = equip->index_;
			if (player_self()->level() < lvl_up_cfg->needLevel) {
				///////
				int result = this->pack_move_item(equip->index_, Pack::PACK_T_PACKAGE_INDEX);
				if (result != 0) {
					if (0 == this->pack_try_erase_item(equip->index_)) {
						Item_Vec item_list;
						item_list.push_back(*equip);
						this->pack_erase_item(equip->index_,  Lose_Item_DM_Info(ITEM_LOSE_EQUIP_LEVEL_UP));
						const Item_Gain_Source_Detail *cfg = CONFIG_CACHE_ITEM->find_item_gain_source(ITEM_GAIN_EQUIP_LEVEL_UP);
						if (!cfg) {
							LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
						} else {
							Mail_Config_Map mail_config = CONFIG_CACHE_MAIL->mail_config();
							Mail_Config_Map::iterator mailc = mail_config.find(PACK_MAIL_ID_NEW);
							if(mailc == mail_config.end()) {
								LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
								return -1;
							}
							std::string title;
							std::string contents;
							{
								const int size_buf = mailc->second.mail_title.size() + \
										sizeof(cfg->name) * 2 + 10;
								char content_buf[size_buf];
								memset(content_buf, 0, size_buf);
								snprintf(content_buf, size_buf, mailc->second.mail_title.c_str(), cfg->name.c_str());
								title.assign(content_buf);
							}

							{
								const int size_buf = mailc->second.mail_content.size() + \
										sizeof(cfg->name) * 2 + 10;
								char content_buf[size_buf];
								memset(content_buf, 0, size_buf);
								snprintf(content_buf, size_buf, mailc->second.mail_content.c_str(), cfg->name.c_str());
								contents.assign(content_buf);
							}
							if (0 != LOGIC_PUBLIC->send_mail(PACK_MAIL_ID_NEW, role_id(), item_list, title, contents)) {
								LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
							}
						}
						this->send_err_to_client(ERROR_PACK_FULL_BUT_MAIL, REQ_EQUIP_LEVEL_UP);
					}
					has_send_mail = true;
				} else {
					pt = PACK_T_PACKAGE_INDEX;
				}
			} else {
				this->send_err_to_client(ERROR_EQUIP_COMPOSE_SUCCESS_ON_BODY, REQ_EQUIP_LEVEL_UP);
			}
			player_self()->heroer_prop_refresh(player_self()->get_hero_index_by_pack_index(index));
			has_send_tip = true;
		} else {
			pt = PACK_T_PACKAGE_INDEX;
		}

		if (!has_send_mail) {
			equip->bind_ = Item_Detail::BIND;
			MSG_82100000 item_msg;
			Item_Basic_Info item_basic;
			equip->set_item_basic_info(item_basic);
			item_msg.item_info.push_back(item_basic);
			acv_msg.item_info.push_back(item_basic);
			THIS_SEND_TO_CLIENT(item_msg);
			active_item_addit_info_update(equip);
			write_item_lose_stream(Lose_Item_DM_Info(ITEM_LOSE_EQUIP_LEVEL_UP), old_equip_id, equip->amount_, equip->bind_, pt);
			write_item_gain_stream(Gain_Item_DM_Info(ITEM_GAIN_EQUIP_LEVEL_UP), equip->id_, equip->amount_, equip->bind_, pt);
		}
	} else {
		Item_Detail level_up(it_cost->second.level_up_id, amount_bak, Item_Detail::BIND);
		this->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, level_up, Gain_Item_DM_Info(ITEM_GAIN_EQUIP_LEVEL_UP));
		Item_Basic_Info item_basic;
		level_up.set_item_basic_info(item_basic);
		acv_msg.item_info.push_back(item_basic);
	}

	if (!acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	MSG_50101003 res_msg;
	res_msg.result = (has_send_tip ? 0 : 1);
	THIS_SEND_TO_CLIENT(res_msg);

	////任务监听合成指定装备  equip_id:装备id
	player_self()->task_listen_compose_equip(it_cost->second.level_up_id);

	///开服活动监听合成  equip_id:装备id
	player_self()->active_content_listen_suit_collect(it_cost->second.level_up_id);

	const Item_Detail_Config* compose_item =
			CONFIG_CACHE_ITEM->find_item(it_cost->second.level_up_id);
	bool is_gemstone_type = compose_item &&
			Item_Detail::TOOL_GEMSTONE == get_item_type_second(compose_item->item_id);
	if (is_gemstone_type) {
		player_self()->task_listen_compose_gemstone_level(compose_item->gemstone_cfg.lv, amount);
	}
	player_self()->gem_compose_announce(level_up_id);
	return 0;
}

int Equiper::req_equip_master_info(void) {
	MSG_50101004 res_msg;
	Master_Part_Info info;
	for (Master_Part_Info_Map::const_iterator it = this->equiper_detail_->master_part_info.begin();
			it != this->equiper_detail_->master_part_info.end(); ++it) {
		info.reset();
		info.index = it->second.index;
		info.level = it->second.level;
		info.luck_value = it->second.luck_value;
		UInt_UInt_Map::const_iterator it_fail_add_rate = CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().find(it->second.level);
		if (it_fail_add_rate != CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().end()) {
			info.fail_add_rate = it_fail_add_rate->second * it->second.fail_cnt;
		}
		res_msg.part_infos.push_back(info);
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Equiper::req_equip_breakdown(const Int_Vec &equip_index) {
	MSG_50101012 msg;
	THIS_SEND_TO_CLIENT(msg);

	if (equip_index.size() > 5) {
		return ERROR_CLIENT_PARAM;
	}

	int8_t star_sum = 0;
	int32_t gold_sum = 0;
	int32_t enchant_num = 0;
	Id_Amount_Vec breakdown_items;
	Id_Amount id_amount;
	for (Int_Vec::const_iterator it = equip_index.begin(); it != equip_index.end(); ++it) {
		const Item_Detail *equip = pack_get_const_item(*it);
		if (NULL == equip) {
			return ERROR_CLIENT_OPERATE;
		}

		if (Item_Detail::is_item_type(equip->id_, Item_Detail::EQUIP)) {
			return ERROR_CLIENT_OPERATE;
		}

		const int32_t equip_breakdown_gold = CONFIG_CACHE_EQUIP->find_equip_breakdown_gold(equip->addon.equip.unban_lvl);
		gold_sum += equip_breakdown_gold;
		const Item_Detail_Config *item_price = CONFIG_CACHE_ITEM->find_item(equip->id_);
		int sellPrice = item_price->sellPrice;
		gold_sum += sellPrice;

		int idx = equip->addon.equip.cnt_enchant_group - 1;
		if(idx >= 0) {
			for(int i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
				star_sum += equip->addon.equip.enchant_groups[idx].enchant_props[i].star;
			}
		}
		const int32_t equip_breakdown_enchant_num = CONFIG_CACHE_EQUIP->find_equip_breakdown_enchant(star_sum);
		enchant_num += equip_breakdown_enchant_num;

		id_amount.id = equip->id_;
		id_amount.amount = 1;
		breakdown_items.push_back(id_amount);
	}

	Money_Add_Info money_add_info(Pack::COPPER, gold_sum, Money_DM_Info(Pack::MONEY_ADD_EQUIP_BREAKDOWN_GOLD));
	player_self()->pack_add_money(money_add_info, Pack::MONEY_OPT_REAL);

	Item_Detail item(20511301, enchant_num, Item_Detail::BIND);
	player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(Pack::ITEM_GAIN_EQUIP_BREAKDOWN_ENCHANT));

	player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, equiper_breakdown_prop, Gain_Item_DM_Info(Pack::ITEM_GAIN_EQUIP_BREAKDOWN_PROP));

	for (Int_Vec::const_iterator it = equip_index.begin(); it != equip_index.end(); ++it) {
		pack_erase_item(Index_Amount(*it, 1), Lose_Item_DM_Info(ITEM_LOSE_BREAKDOWN), LOSE_EQUIP_BREAKDOWN, WITHOUT_TRY);
	}

	MSG_81000102 item_msg;
	Item_Basic_Info item_basic;
	Id_Amount id_amount_prop;
	Id_Amount id_amount_enchant;
	Id_Amount id_amount_gold;
	Id_Amount_Vec id_amount_vec_item;
	Item_Vec::const_iterator item_vec_prop;
	Id_Amount_Vec::const_iterator id_amount_iter;
	for (item_vec_prop = equiper_breakdown_prop.begin(); item_vec_prop != equiper_breakdown_prop.end(); ++item_vec_prop) {
		id_amount_prop.id = (*item_vec_prop).id_;
		id_amount_prop.amount = (*item_vec_prop).amount_;
		id_amount_vec_item.push_back(id_amount_prop);
	}
	if(enchant_num != 0){
		id_amount_enchant.id = 20511301;
		id_amount_enchant.amount = enchant_num;
		id_amount_vec_item.push_back(id_amount_enchant);
	}
	for (id_amount_iter = id_amount_vec_item.begin(); id_amount_iter != id_amount_vec_item.end(); ++id_amount_iter) {
		item_basic.id = (*id_amount_iter).id;
		item_basic.amount = (*id_amount_iter).amount;
		item_msg.item_info.push_back(item_basic);
	}
	item_msg.property.push_back(Property(Pack::COPPER, gold_sum));
	if (!item_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(item_msg);
	}
	return 0;
}

int Equiper::req_equip_breakdown_prop(const Int_Vec &equip_index) {
	if (equip_index.size() > 5) {
		return ERROR_CLIENT_PARAM;
	}
	MSG_50101013 res_msg;
	Id_Amount id_amount;
	Item_Id_Amount item_id_amount;
	std::vector<Item_Id_Amount> breakdown_prop;
	equiper_breakdown_prop.clear();
	Int_Set st;
	for (Int_Vec::const_iterator it = equip_index.begin();
		it != equip_index.end(); ++it) {
		const Item_Detail *equip = pack_get_const_item(*it);
		if (NULL == equip) {
			return ERROR_CLIENT_OPERATE;
		}
		if (Item_Detail::is_item_type(equip->id_, Item_Detail::EQUIP)) {
			return ERROR_CLIENT_OPERATE;
		}
		const Item_Detail_Config *item = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (NULL == item) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		const Equip_Breakdown_Prop *equip_breakdown_prop = CONFIG_CACHE_EQUIP->find_equip_breakdown_prop(item->needLevel);
		if (NULL == equip_breakdown_prop) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		if(!equip_breakdown_prop->resolve_prop.empty()) {
			switch(item->color) {
				case 1: {
					int idx = Utility_Func::lottery_one(equip_breakdown_prop->chance_1);
					id_amount = equip_breakdown_prop->resolve_prop[idx];
					break;
				}
				case 2: {
					int idx = Utility_Func::lottery_one(equip_breakdown_prop->chance_2);
					id_amount = equip_breakdown_prop->resolve_prop[idx];
					break;
				}
				case 3: {
					int idx = Utility_Func::lottery_one(equip_breakdown_prop->chance_3);
					id_amount = equip_breakdown_prop->resolve_prop[idx];
					break;
				}
				case 4: {
					int idx = Utility_Func::lottery_one(equip_breakdown_prop->chance_4);
					id_amount = equip_breakdown_prop->resolve_prop[idx];
					break;
				}
				case 5: {
					int idx = Utility_Func::lottery_one(equip_breakdown_prop->chance_5);
					id_amount = equip_breakdown_prop->resolve_prop[idx];
					break;
				}
				default: {
					return ERROR_CONFIG_NOT_EXIST;
				}
			}
		}

		item_id_amount.id = id_amount.id;
		item_id_amount.amount = id_amount.amount;
		breakdown_prop.push_back(item_id_amount);
		st.insert(id_amount.id);

		const int32_t equip_breakdown_gold = CONFIG_CACHE_EQUIP->find_equip_breakdown_gold(equip->addon.equip.unban_lvl);
		res_msg.gold += equip_breakdown_gold;

		int8_t star_sum = 0;
		int idx = equip->addon.equip.cnt_enchant_group - 1;
		if(idx >= 0) {
			for(int i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
				star_sum += equip->addon.equip.enchant_groups[idx].enchant_props[i].star;
			}
		}
		const int32_t equip_breakdown_enchant_num = CONFIG_CACHE_EQUIP->find_equip_breakdown_enchant(star_sum);
		res_msg.enchant_num += equip_breakdown_enchant_num;
	}

	std::vector<Item_Id_Amount>::const_iterator prop_vec;
	for (prop_vec = breakdown_prop.begin(); prop_vec != breakdown_prop.end(); ++prop_vec) {
		Item_Detail item_id((*prop_vec).id, (*prop_vec).amount, 0);
		equiper_breakdown_prop.push_back(item_id);
	}

	for(std::set<int>::iterator it = st.begin(); it != st.end(); ++it) {
		item_id_amount.id = *it;
		item_id_amount.amount = 1;
		res_msg.prop.push_back(item_id_amount);
	}

	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Equiper::get_part_by_index(const int equip_index) {
	//if (pack_is_pack_type(equip_index, Pack::PACK_T_EQUIP_INDEX) == 0) {
		const Item_Detail *equip = pack_get_const_item(equip_index);
		if (equip) {
			const Item_Detail_Config *equip_from_cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (equip_from_cfg) {
				return equip_from_cfg->equip_cfg.part;
			}
		}
	//}
	return 0;
}

int Equiper::get_part_by_id(const int equip_id) {
	const Item_Detail_Config *equip_from_cfg = CONFIG_CACHE_ITEM->find_item(equip_id);
	if (equip_from_cfg) {
		return equip_from_cfg->equip_cfg.part;
	}
	return 0;
}

int Equiper::equip_master(Master_Part_Info_Detail& info, const int auto_buy, int &is_success, int32_t &res_copper, std::vector<Item_Basic_Info> &res_item) {
	uint32_t increase_level = info.level;
	uint32_t increase_level_old = info.level;
	uint32_t luck_value = info.luck_value;

	UInt_UInt_Map::const_iterator it_max_luck_value = CONFIG_CACHE_EQUIP->incr_lvl_max_luck_value_map().find(increase_level);
	if (it_max_luck_value == CONFIG_CACHE_EQUIP->incr_lvl_max_luck_value_map().end()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Equip_Master_Cost::const_iterator it_cost = CONFIG_CACHE_EQUIP->incr_lvl_cost_map().find(increase_level);
	if (it_cost == CONFIG_CACHE_EQUIP->incr_lvl_cost_map().end()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	uint need_stone_id = it_cost->second.id;
	int need_stone_amount = it_cost->second.amount;
	int pack_stone_amount = this->pack_calc_item(Pack_Type::PACK_T_PACKAGE_INDEX, need_stone_id);
	int money_type = 0, money_amount = 0, item_amount = 0;
	if (pack_stone_amount < need_stone_amount) {
		if (auto_buy == 0) {
			MSG_82100008 acv_msg;
			acv_msg.item_ids.push_back(need_stone_id);
			THIS_SEND_TO_CLIENT(acv_msg);
			return -1;
		}
//		//try自动购买材料
		item_amount = need_stone_amount - pack_stone_amount;
		int ret = player_self()->req_fast_buy_item_in_mall_tabs(need_stone_id, item_amount, 1);
		if (0 != ret) {
//			MSG_82100008 acv_msg;
//			acv_msg.item_ids.push_back(need_stone_id);
//			THIS_SEND_TO_CLIENT(acv_msg);
			return ret;
		}
		pack_stone_amount = this->pack_calc_item(Pack_Type::PACK_T_PACKAGE_INDEX, need_stone_id);
		if (pack_stone_amount < need_stone_amount) {
			MSG_82100008 acv_msg;
			acv_msg.item_ids.push_back(need_stone_id);
			THIS_SEND_TO_CLIENT(acv_msg);
			return -1;
		}
//		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(need_stone_id);
//		if (!item_cfg) {
//			return ERROR_CONFIG_NOT_EXIST;
//		}
//		money_type = item_cfg->sys_sell_type;
//		money_amount = item_cfg->sys_sell * item_amount;
//		if (money_type <= 0 || money_amount<= 0) {
//			MSG_82100008 acv_msg;
//			acv_msg.item_ids.push_back(need_stone_id);
//			THIS_SEND_TO_CLIENT(acv_msg);
//			return -1;
//		}
	}

	int return_status = pack_try_sub_money(Money_Sub_Info(COPPER_ONLY, it_cost->second.copper, MONEY_SUB_EQUIP_INCREASE));
	if(return_status != 0) {
		return return_status;
	}

	//  扣除金钱花费
	int cost_copper_i = it_cost->second.copper;
	if (cost_copper_i > 0) {
		Money_Sub_List money_sub_list;
		Money_Sub_Info copper_sub_info(COPPER_ONLY, cost_copper_i, Money_DM_Info(Pack::MONEY_SUB_EQUIP_INCREASE, 0 , 0, info.index));
		money_sub_list.push_back(copper_sub_info);

		if (money_type > 0) {
			money_sub_list.push_back( Money_Sub_Info(static_cast<Money_Sub_Type>(money_type), money_amount, Money_DM_Info(Pack::MONEY_SUB_EQUIP_INCREASE, 0 , 0, info.index)) );
		}
		int sub_result = this->pack_sub_money(money_sub_list);
		if (sub_result != 0) return sub_result;
		res_copper += cost_copper_i;
	}


	{	///  扣除强化石
		int erase_stone_amount = pack_stone_amount >= need_stone_amount ? need_stone_amount : pack_stone_amount;
		if (erase_stone_amount >= 0) {
			Id_Amount id_amount(need_stone_id, erase_stone_amount);
			this->pack_erase_item(PACK_T_PACKAGE_INDEX, id_amount, Lose_Item_DM_Info(ITEM_LOSE_EQUIP_MASTER));
		}
	}

	{	///  统计强化石消耗
		bool cost_item_exist = false;
		for (std::vector<Item_Basic_Info>::iterator iter = res_item.begin(); iter != res_item.end(); ++iter) {
			if (iter->id == need_stone_id) {
				iter->amount += need_stone_amount;
				cost_item_exist = true;
				break;
			}
		}
		if (!cost_item_exist) {
			Item_Basic_Info item_info;
			item_info.reset();
			item_info.id = need_stone_id;
			item_info.amount = need_stone_amount;
			res_item.push_back(item_info);
		}
	}

	if (luck_value < it_max_luck_value->second) {
		//原始配置的成功率
		uint32_t success_rate = 0;
		UInt_UInt_Map::const_iterator it_success_rate = CONFIG_CACHE_EQUIP->incr_lvl_success_rate_map().find(increase_level_old);
		if (it_success_rate != CONFIG_CACHE_EQUIP->incr_lvl_success_rate_map().end()) {
			success_rate = it_success_rate->second;
		}

		success_rate = (success_rate < 0) ? 0 : success_rate;
		uint32_t fail_add_rate = 0;
		UInt_UInt_Map::const_iterator it = CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().find(info.level);
		if (it != CONFIG_CACHE_EQUIP->incr_lvl_fail_add_rate_map().end()) {
			fail_add_rate = it->second;
		}
		success_rate += fail_add_rate * info.fail_cnt;
		success_rate = (success_rate > 1000) ? 1000 : success_rate;

		std::vector<int> wight;
		wight.push_back(success_rate);
		wight.push_back(1000 - success_rate);
		is_success = Utility_Func::lottery_one(wight);
	}

	/// 强化成功
	if (is_success == 0) {
		//强级处理
		info.level++;
		info.luck_value = 0;
		info.fail_cnt = 0;

	/// 强化失败
	} else {
		info.fail_cnt++;

		//幸运值处理
		UInt_UInt_Map::const_iterator it_add_luck = CONFIG_CACHE_EQUIP->incr_lvl_fail_add_luck_value_map().find(increase_level_old);
		if (it_add_luck != CONFIG_CACHE_EQUIP->incr_lvl_fail_add_luck_value_map().end()) {
			info.luck_value += it_add_luck->second;
			if (info.luck_value > static_cast<int32_t>(it_max_luck_value->second)) {
				info.luck_value = it_max_luck_value->second;
			}
		} else {
			info.luck_value++;
		}
	}

	return 0;
}

int Equiper::calculate_equip_force(const std::map<int, double> &pro_val_map) {
	if (! pro_val_map.empty()) {
		Equip_Force_Info force_info;
		for (std::map<int, double>::const_iterator it_pro_val_map = pro_val_map.begin();
				it_pro_val_map != pro_val_map.end(); ++it_pro_val_map) {
			if (it_pro_val_map->first && it_pro_val_map->second > 0.0) {
				std::map<int, double>::iterator it_force = force_info.pt_val_map.find(it_pro_val_map->first);
				if (it_force != force_info.pt_val_map.end()) {
					it_force->second += it_pro_val_map->second;
				}
			}
		}
		return ceil(force_info.calculate_force());
	}
	return 0;
}

int Equiper::refresh_equip_force(Item_Detail *equip, bool filter_rune,  bool filter_master) {
	if (! equip) {
		return 0;
	}
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (! item_config) {
		return 0;
	}

	std::map<int, double> pro_val_map;

	// 装备基础属性
	base_prop_refresh(item_config, pro_val_map);
	// 装备解封属性
	unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, pro_val_map);

	if (!filter_master) {
		// 精通加成-装备基础属性
		master_base_prop_refresh(item_config, equip->index_, pro_val_map);

		// 精通加成-装备解封属性
//		master_unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, equip->index_, pro_val_map);
	}

	// 装备附魔属性
	enchant_prop_refresh(equip, pro_val_map);

	// 符石系统加成
//	if (!filter_rune) {
//		this->player_self()->rune_prop_refresh(equip->index_, equip->index_, pro_val_map);
//	}

	// 计算装备战斗力
	int32_t force = calculate_equip_force(pro_val_map);
	if (force == equip->addon.equip.force)
		return 0;
	else {
		equip->addon.equip.force = calculate_equip_force(pro_val_map);
		return equip->addon.equip.force;
	}

	return 0;
}

int Equiper::calculate_equip_force(const uint equip_id) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip_id);
	if (! item_config) {
		return 0;
	}

	std::map<int, double> pro_val_map;
	base_prop_refresh(item_config, pro_val_map);

	return calculate_equip_force(pro_val_map);
}

int Equiper::suit_prop_refresh(const std::map<int, int> &suit_cnt_map, std::map<int, double> &pro_val_map) {
	if (suit_cnt_map.empty()) {
		return -1;
	}

	Int_Set skills;
	for (std::map<int, int>::const_iterator it = suit_cnt_map.begin(); it != suit_cnt_map.end(); ++it) {
		int suit_id = it->first;
		int suit_cnt = it->second;
		const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(suit_id);
		if (! suit_cfg) {
			MSG_USER("role:%ld %d", role_id(), suit_id);
			continue;
		}

		// 从小到大叠加套装数对应属性，并非只找最大
		for (Suit_Count_Cfg_Map::const_iterator it_cfg_cnt = suit_cfg->count_cfg_map_.begin();
				it_cfg_cnt != suit_cfg->count_cfg_map_.end(); ++it_cfg_cnt) {
			if (suit_cnt < it_cfg_cnt->first) {
				break;
			}
			// prop
			for (Int_Double_Map::const_iterator it_cfg_prop = it_cfg_cnt->second.prop_val_map.begin();
					it_cfg_prop != it_cfg_cnt->second.prop_val_map.end(); ++it_cfg_prop) {
				int prop = it_cfg_prop->first;
				double val = it_cfg_prop->second;
				pro_val_map[prop] += val;
			}
			//skill
			skills.insert(it_cfg_cnt->second.skills.begin(), it_cfg_cnt->second.skills.end());
		}
	}

	//player_self()->insert_passive_skills(skills);

	return 0;
}

int Equiper::test_get_and_enhance_equips(const int suit_id, const int unban_lvl) {
	const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(suit_id);
	if (!suit_cfg || suit_cfg->item_ids.empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Item_Vec item_vec;
	for (uint8_t i = 0; i < suit_cfg->item_ids.size(); ++i) {
		Item_Detail item_equip(suit_cfg->item_ids[i], 1, Item_Detail::BIND);
		Item_Detail *equip = &item_equip;
		if (! equip) {
			return ERROR_ITEM_NOT_EXIST;
		}
		if (0 != equip->is_item_type(equip->id_, Item_Detail::EQUIP)) {
			return ERROR_CLIENT_OPERATE;
		}

		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		int set_unban_lvl = unban_lvl;
		if (unban_lvl >= item_config->equip_cfg.max_unban_lvl) {
			set_unban_lvl = item_config->equip_cfg.max_unban_lvl;
		}

		equip->addon.equip.unban_lvl = set_unban_lvl;

		refresh_equip_force(equip);

		item_vec.push_back(item_equip);
	}

	int res = this->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_vec, Gain_Item_DM_Info(ITEM_GAIN_TEST));
	if (res) {
		return res;
	}

	return 0;
}

int Equiper::test_unban_equips(const int unban_lvl) {
	if (! unban_lvl) {
		return -1;
	}
	Item_Detail *equip = NULL;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}

		int set_unban_lvl = unban_lvl;
		if (unban_lvl >= item_config->equip_cfg.max_unban_lvl) {
			set_unban_lvl = item_config->equip_cfg.max_unban_lvl;
		}

		equip->addon.equip.unban_lvl = set_unban_lvl;

		refresh_equip_force(equip);
		active_item_addit_info_update(equip);
	}

	equiper_prop_refresh(10017);


	// fight hero
	if (player_self()->heroer_detail().fighter_index) {
		uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP;
		int suit_idx_begin = hero_pack_idx_gap + EQUIP_SUIT_BEGIN;
		int suit_idx_end = hero_pack_idx_gap + EQUIP_SUIT_END;

		for (int index = suit_idx_begin; index < suit_idx_end; ++index) {
			equip = this->pack_get_item(index);
			if (! equip) {
				continue;
			}
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (! item_config) {
				continue;
			}

			int set_unban_lvl = unban_lvl;
			if (unban_lvl >= item_config->equip_cfg.max_unban_lvl) {
				set_unban_lvl = item_config->equip_cfg.max_unban_lvl;
			}

			equip->addon.equip.unban_lvl = set_unban_lvl;

			refresh_equip_force(equip);
			active_item_addit_info_update(equip);

			player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index);
		}
	}

	return 0;
}

int Equiper::test_master(const int master_lvl) {
	int lvl_max = static_cast<int>(CONFIG_CACHE_EQUIP->incr_max_lvl());
	int lvl = master_lvl > lvl_max ? lvl_max - 1 : master_lvl;
	for (int index = Equiper_Detail::ROLE_START; index <= Equiper_Detail::ROLE_END; ++index) {
		Master_Part_Info_Map::iterator master_it = this->equiper_detail_->master_part_info.find(index);
		if (master_it == this->equiper_detail_->master_part_info.end()) {
			Master_Part_Info_Detail info;
			info.index = index;
			std::pair<Master_Part_Info_Map::iterator, bool> pr = this->equiper_detail_->master_part_info.insert(std::make_pair(index, info));
			if (!pr.second) {
				continue;
			}
			master_it = pr.first;
			if (static_cast<uint32_t>(master_it->second.level) > CONFIG_CACHE_EQUIP->incr_max_lvl())
				continue;
		}
		master_it->second.level = lvl;
		master_it->second.luck_value = 0;
		master_it->second.fail_cnt = 0;
		Item_Detail *equip = this->pack_get_item(index);
		if (equip) {
			refresh_equip_force(equip);
			equiper_prop_refresh(10016);
		}
	}
	for (int index = Equiper_Detail::HERO_START; index <= Equiper_Detail::HERO_END; ++index) {
		Master_Part_Info_Map::iterator master_it = this->equiper_detail_->master_part_info.find(index);
		if (master_it == this->equiper_detail_->master_part_info.end()) {
			Master_Part_Info_Detail info;
			info.index = index;
			std::pair<Master_Part_Info_Map::iterator, bool> pr = this->equiper_detail_->master_part_info.insert(std::make_pair(index, info));
			if (!pr.second) {
				continue;
			}
			master_it = pr.first;
			if (static_cast<uint32_t>(master_it->second.level) > CONFIG_CACHE_EQUIP->incr_max_lvl())
				continue;
		}
		master_it->second.level = lvl;
		master_it->second.luck_value = 0;
		master_it->second.fail_cnt = 0;
		int index_old = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP + index % 100;
		Item_Detail *equip = this->pack_get_item(index_old);
		if (equip) {
			refresh_equip_force(equip);
			player_self()->heroer_prop_refresh(player_self()->get_hero_index_by_pack_index(index_old));
		}
	}

	Packer::set_change();
	//成就监听
	player_self()->achievement_listen_master(lvl);

	return 0;
}

int Equiper::get_suit_ids(void) {

	return 0;
}

int Equiper::get_equips_enchant_star(void) {
	int sum_star = 0;
	Item_Detail *equip = NULL;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_SUIT_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}
		if (equip->addon.equip.cnt_enchant_group >= 1) {
			for (uint8_t idx_enchant_props = 0;
					idx_enchant_props < equip->addon.equip.enchant_groups[0].cnt_enchant_prop &&
					idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
					++idx_enchant_props) {
				sum_star += equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].star;
			}
		}
	}
	int heroer_index = player_self()->heroer_detail().fighter_index;
	if(heroer_index){
		uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + heroer_index * ONE_HERO_EQUIP_GAP;
		for(uint32_t index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
			equip = this->pack_get_item(index);
			if (! equip) {
				continue;
			}
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (! item_config) {
				continue;
			}
			if (equip->addon.equip.cnt_enchant_group >= 1) {
				for (uint8_t idx_enchant_props = 0;
						idx_enchant_props < equip->addon.equip.enchant_groups[0].cnt_enchant_prop &&
						idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
						++idx_enchant_props) {
					sum_star += equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].star;
				}
			}
		}
	}
	return sum_star;
}

int Equiper::get_equips_max_possible_enchant_star(void) {
	int sum_star = 0;
	Item_Detail *equip = NULL;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_SUIT_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}
		sum_star += cnt_enchant_prop(item_config->color) * 10;
	}
	int heroer_index = player_self()->heroer_detail().fighter_index;
	if(heroer_index){
		uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + heroer_index * ONE_HERO_EQUIP_GAP;
		for(uint32_t index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
			equip = this->pack_get_item(index);
			if (! equip) {
				continue;
			}
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (! item_config) {
				continue;
			}
			sum_star += cnt_enchant_prop(item_config->color) * 10;
		}
	}
	return sum_star;
}

int Equiper::get_master_effect_lvl_sum_on_player(void) {
	int lvl_all = 0;
	Item_Detail *equip = NULL;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_SUIT_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}
		if (equip->overdue_time > Time_Value::zero && equip->overdue_time <= Time_Value::gettimeofday()) {
			continue;
		}

		// 获得装备全身等级（8件）
		if (index < EQUIP_IDX_SUIT_END) {
			Master_Part_Info_Map::const_iterator it_part = this->equiper_detail_->master_part_info.find(index);
			if (it_part != this->equiper_detail_->master_part_info.end()) {
				lvl_all += it_part->second.level;
			}
		}
	}
	return lvl_all;
}
int Equiper::get_master_effect_lvl_sum_on_heroer(void) {
	if (0 == player_self() || 0 == player_self()->heroer_detail().fighter_index) {
		return 0;
	}
	int lvl_all = 0;
	Item_Detail *equip = NULL;

	int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP;
	int hero_equip_idx_suit_end = hero_pack_idx_gap + EQUIP_IDX_SUIT_END;

	for (int index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_config) {
			continue;
		}
		if (equip->overdue_time > Time_Value::zero && equip->overdue_time <= Time_Value::gettimeofday()) {
			continue;
		}

		// 获得装备全身等级（8件）
		if (index < hero_equip_idx_suit_end) {
			int index_old = PACK_T_HERO_EQUIP_INDEX + ONE_HERO_EQUIP_GAP + index % 100;
			Master_Part_Info_Map::const_iterator it_part = this->equiper_detail().master_part_info.find(index_old);
			if (it_part != this->equiper_detail().master_part_info.end()) {
				lvl_all += it_part->second.level;
			}
		}
	}
	return lvl_all;
}

int Equiper::enchant_make_saved_lock_prop_cnt_map(const Item_Detail *equip, std::map<int, int> &saved_lock_prop_cnt_map) {
	if (! equip) {
		return ERROR_INNER_SERVER;
	}
	// 已保存的锁定属性条数map
	saved_lock_prop_cnt_map.clear();
	for (uint8_t idx_enchant_props = 0;
			idx_enchant_props < equip->addon.equip.enchant_groups[0].cnt_enchant_prop &&
			idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
			++idx_enchant_props) {
		if (equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].is_lock &&
				equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type) {
			++saved_lock_prop_cnt_map[equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type];
		}
	}
	return 0;
}

int Equiper::cal_enchant_prop_weights(const Item_Detail *equip, const Enchant_Part_Cfg* enchant_part_cfg, const Enchant_Mix_Cfg& enchant_mix_cfg,
		 const std::map<int, int> &saved_lock_prop_cnt_map, const int idx_enchant_groups, Int_Vec &weights, int enchant_weight_rate) {
	if (!equip || !enchant_part_cfg ||	// idx_enchant_groups >= equip->addon.equip.cnt_enchant_group ||
			idx_enchant_groups >= Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP) {
		return ERROR_INNER_SERVER;
	}
	weights.clear();

	// 已保存组里的锁定属性条数map
	//std::map<int, int> saved_lock_prop_cnt_map;

	// 本组里的没锁属性条数map
	std::map<int, int> notlock_prop_cnt_map;
	for (uint8_t idx_enchant_props = 0;
			idx_enchant_props < equip->addon.equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop &&
			idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
			++idx_enchant_props) {
		if (!equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].is_lock	&&
				equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_type) {
			++notlock_prop_cnt_map[equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_type];
		}
	}

	// 属性衰减条数map (merge map)
	std::map<int, int> prop_subcnt_map = notlock_prop_cnt_map;
//	for (std::map<int, int>::iterator it_subcnt_map = prop_subcnt_map.begin(); it_subcnt_map != prop_subcnt_map.end(); ++it_subcnt_map) {
//		if (it_subcnt_map->first) {
//			for (std::map<int, int>::const_iterator it_saved_map = saved_lock_prop_cnt_map.begin();
//					it_saved_map != saved_lock_prop_cnt_map.end(); ++it_saved_map) {
//				if (it_saved_map->first) {
//					prop_subcnt_map[it_saved_map->first] += it_saved_map->second;
//				}
//			}
//		}
//	}

	for (std::map<int, int>::const_iterator it_saved_map = saved_lock_prop_cnt_map.begin();
			it_saved_map != saved_lock_prop_cnt_map.end(); ++it_saved_map) {
		if (it_saved_map->first) {
			prop_subcnt_map[it_saved_map->first] += it_saved_map->second;
		}
	}

	// 属性衰减权重map
	std::map<int, int> prop_subweight_map;
	for (std::map<int, int>::const_iterator it_subcnt_map = prop_subcnt_map.begin();
			it_subcnt_map != prop_subcnt_map.end(); ++it_subcnt_map) {
		if (it_subcnt_map->first && it_subcnt_map->second && enchant_mix_cfg.sub_weight_once) {
			prop_subweight_map.insert(std::make_pair(it_subcnt_map->first,
					it_subcnt_map->second * enchant_mix_cfg.sub_weight_once));
		}
	}

	// 生成 weights
	for (std::map<int, int>::const_iterator it_cfg_weight = enchant_part_cfg->prop_weight_map.begin();
			it_cfg_weight != enchant_part_cfg->prop_weight_map.end(); ++it_cfg_weight) {

		int weight = it_cfg_weight->second;
		Int_Int_Map::const_iterator find_it = enchant_part_cfg->props_ratio.find(it_cfg_weight->first);
		if (enchant_part_cfg->props_ratio.end() != find_it) {
			weight *= enchant_weight_rate;
		}
		std::map<int, int>::const_iterator find_it_subweight_map = prop_subweight_map.find(it_cfg_weight->first);
		if (find_it_subweight_map != prop_subweight_map.end()){
			weight -= find_it_subweight_map->second;
		}
		weight = weight < 0 ? 0 : weight;
		weights.push_back(weight);
	}

	return 0;
}

int Equiper::cal_cnt_enchant(const int is_bat, const int8_t auto_buy_stone, const int8_t auto_buy_lock, const Int8_Vec &locks, const Enchant_Mix_Cfg& enchant_mix_cfg,
		int &cnt_enchant, int &need_copper, int &need_gold, int &erase_stone_amount, int &erase_lock_amount) {
	int max_cnt_enchant = is_bat ? Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP - 2  : 1;
	cnt_enchant = max_cnt_enchant;

	int stone_id = enchant_mix_cfg.material_id;
	//int stone_num = enchant_mix_cfg.material_num;
	int lock_id = enchant_mix_cfg.lock_id;
	int msg_lock_amount = std::count_if(locks.begin(), locks.end(), is_true);

	const Item_Detail_Config *stone_cfg = CONFIG_CACHE_ITEM->find_item(stone_id);
	const Item_Detail_Config *lock_cfg = CONFIG_CACHE_ITEM->find_item(lock_id);
	if (!stone_cfg || !lock_id) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	int pack_stone_amount = this->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, stone_id);
	int pack_lock_amount = this->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, lock_id);
	int pack_stone_tmp = pack_stone_amount;
	int pack_lock_tmp = pack_lock_amount;
	int buy_stone_once = 0;		// 单次购买石头数
	int buy_lock_once = 0;		// 单次购买锁数
	int gold_cost_once = 0;
	int64_t gold_all_cost = 0;


	const Money_Detail &money = player_self()->pack_get_money();
	int64_t money_gold_total = static_cast<int64_t>(money.bind_diamond) + static_cast<int64_t>(money.diamond);

	/*
	 * 单算gold支持的次数
	 */
	int cnt_enchant_gold = 0;
	for(int8_t i = 1; i <= max_cnt_enchant && cnt_enchant_gold <= max_cnt_enchant; ++i) {
		buy_stone_once = 0;
		buy_lock_once = 0;
		gold_cost_once = 0;
		//石单次花费:
		if (pack_stone_tmp >= 1) {
			buy_stone_once = 0;
			--pack_stone_tmp;
		} else {
			//pack_stone_tmp = 0;
			if (auto_buy_stone) {
				buy_stone_once = 1;
			} else {
				//
			}
		}
		//锁单次花费:
		if (pack_lock_tmp >= msg_lock_amount) {
			buy_lock_once = 0;
			pack_lock_tmp -= msg_lock_amount;
		} else {
			pack_lock_tmp = 0;
			if (auto_buy_lock) {
				buy_lock_once = msg_lock_amount - pack_lock_tmp;
			} else {
				//
			}
		}
		//两材料合起来单次花费，可能是变动值，即单次不完全一样:
		if (buy_stone_once) {
			gold_cost_once += stone_cfg->sys_sell * buy_stone_once;
		}
		if (buy_lock_once) {
			gold_cost_once += lock_cfg->sys_sell * buy_lock_once;
		}
		//总花费
		gold_all_cost += gold_cost_once;
		if (money_gold_total < gold_all_cost) {
			break;
		}
		++cnt_enchant_gold;
	}
	// 放自动购买材料处提示金钱不足
//	if (cnt_enchant_gold < 1 && auto_buy) {
//		return ERROR_GOLD_NOT_ENOUGH;
//
//	}

	// 单算无自动购买,背包石头支持次数
	int cnt_enchant_stone = cnt_enchant_gold;
	if (! auto_buy_stone) {
		int pack_stone_times = pack_stone_amount;	// 无自动购买,石头支持次数 = 背包石头个数 / 每次耗石头数
		if (pack_stone_times < 1) {
			MSG_82100008 active_msg;
			active_msg.item_ids.push_back(stone_id);
			THIS_SEND_TO_CLIENT(active_msg);
			return ERROR_ITEM_NOT_ENOUGH;
		}
		cnt_enchant_stone = pack_stone_times;
	} else {
		if (cnt_enchant_gold < 1) {
			return ERROR_GOLD_NOT_ENOUGH;
		}
	}

	// 单算无自动购买,背包锁支持次数
	int cnt_enchant_lock = cnt_enchant_gold;
	if (! auto_buy_lock) {
		if (msg_lock_amount > 0) {
			int pack_lock_times = U_FUNC::div(pack_lock_amount, msg_lock_amount) ;	//无自动购买,背包锁支持次数 = 背包锁个数/每条属性耗锁数
			if (pack_lock_times < 1) {
//				MSG_82100008 active_msg;
//				active_msg.item_ids.push_back(lock_id);
//				THIS_SEND_TO_CLIENT(active_msg);
				return ERROR_EQUIP_ENCHANT_LOCK_LACK;
			}
			cnt_enchant_lock = pack_lock_times;
			//cnt_enchant = std::min(max_cnt_enchant, std::min(cnt_enchant_copper, std::min(pack_stone_amount, pack_lock_times)));
		} else {
			//cnt_enchant = std::min(max_cnt_enchant, std::min(cnt_enchant_copper, pack_stone_amount));
		}
	} else {
		if (cnt_enchant_gold < 1 && msg_lock_amount > 0) {
			return ERROR_GOLD_NOT_ENOUGH;
		}
	}

	// 单算copper支持的次数
	int cnt_enchant_copper = U_FUNC::div(money.gold, enchant_mix_cfg.cost_copper);
	if (cnt_enchant_copper < 1) {
		return ERROR_COPPER_NOT_ENOUGH;
	}

	// 合算
	cnt_enchant = std::min(cnt_enchant, cnt_enchant_gold);
	cnt_enchant = std::min(cnt_enchant, cnt_enchant_stone);
	cnt_enchant = std::min(cnt_enchant, cnt_enchant_lock);
	cnt_enchant = std::min(cnt_enchant, cnt_enchant_copper);
	//cnt_enchant = std::min(cnt_enchant, max_cnt_enchant);

	if (cnt_enchant < 1) {
		return ERROR_EQUIP_ENCHANT_CNT_ZERO;
	}

	// 实际购买石头数
	int buy_stone_amount = 0;
	if (pack_stone_amount < 1) {
		buy_stone_amount = cnt_enchant;
	} else if (pack_stone_amount >= 1 && pack_stone_amount < cnt_enchant) {
		buy_stone_amount = cnt_enchant - pack_stone_amount;
	} else {
		buy_stone_amount = 0;
	}
	// 实际购买锁数
	int buy_lock_amount = 0;
	if (pack_lock_amount < 1) {
		buy_lock_amount = cnt_enchant * msg_lock_amount;
	} else if (pack_lock_amount >= 1 && pack_lock_amount < cnt_enchant * msg_lock_amount) {
		buy_lock_amount =  cnt_enchant * msg_lock_amount - pack_lock_amount;
	} else {
		buy_lock_amount = 0;
	}

	need_copper = enchant_mix_cfg.cost_copper * cnt_enchant;
	need_gold = stone_cfg->sys_sell * buy_stone_amount + lock_cfg->sys_sell * buy_lock_amount;

	erase_stone_amount = pack_stone_amount > cnt_enchant ? cnt_enchant : pack_stone_amount;
	erase_lock_amount = cnt_enchant * msg_lock_amount - buy_lock_amount;

	return 0;
}

int Equiper::init_enchant_lock_prop_fail(Item_Detail *equip, const Int8_Vec &locks, uint8_t cnt_enchant_prop, std::map<int, int>& fail_prop_cnt_map) {
	if (! equip) {
		return ERROR_INNER_SERVER;
	}
	if (equip->addon.equip.enchant_groups[0].enchant_props[0].prop_type) {	// had_enchanted

//		for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
//			if (equip->addon.equip.enchant_lock_prop_fail[i].prop) {
//				fail_prop_cnt_map.insert(std::make_pair(equip->addon.equip.enchant_lock_prop_fail[i].prop,
//						equip->addon.equip.enchant_lock_prop_fail[i].fail_cnt));
//			}
//		}

		fail_prop_cnt_map.clear();

		for (uint8_t i = 0;	i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {	// loop enchant_props[0] if lock or not
			if (! locks[i]) {
				// if not lock, reset
				//equip->addon.equip.enchant_lock_prop_fail[i].reset();
			} else {
				// if lock, init
				fail_prop_cnt_map.insert(std::make_pair(equip->addon.equip.enchant_groups[0].enchant_props[i].prop_type, 0));

				// add cnt in fail_prop_cnt_map
				for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
					if (equip->addon.equip.enchant_lock_prop_fail[i].prop) {
						std::map<int, int>::iterator find_it_fail_map = fail_prop_cnt_map.find(equip->addon.equip.enchant_lock_prop_fail[i].prop);
						if (find_it_fail_map != fail_prop_cnt_map.end()) {
							find_it_fail_map->second = equip->addon.equip.enchant_lock_prop_fail[i].fail_cnt;
						}
					}
				}

//				if (! equip->addon.equip.enchant_lock_prop_fail[idx_enchant_props].prop) {
//					equip->addon.equip.enchant_lock_prop_fail[idx_enchant_props].prop =
//							equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type;
//				}
				// 因为map好操作，洗的过程用fail_prop_cnt_map记录，统一洗完再给数组enchant_lock_prop_fail赋值

//				for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
//					if (equip->addon.equip.enchant_lock_prop_fail[i].prop == enchant_prop) {
//						return true;
//					}
//				}
//
//				if (equip->addon.equip.enchant_groups[0].enchant_props[i].prop_type) {
//					++fail_prop_cnt_map[equip->addon.equip.enchant_groups[0].enchant_props[i].prop_type];
//				}

			}
		}
	}
	return 0;
}

bool Equiper::in_array_enchant_fail(const int enchant_prop, const Item_Detail *equip) {
	if (! equip) {
		return false;
	}
	for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
		if (equip->addon.equip.enchant_lock_prop_fail[i].prop == enchant_prop) {
			return true;
		}
	}
	return false;
}

int Equiper::enchant_one_prop(Item_Detail *equip, const Enchant_Part_Cfg* enchant_part_cfg, const Enchant_Color_Cfg* enchant_color_cfg,
		const Equip_Lvl_Cfg* equip_lvl_cfg,	const Enchant_Mix_Cfg& enchant_mix_cfg,	const std::map<int, int>& saved_lock_prop_cnt_map,
		const Int8_Vec& locks, std::map<int, int>& fail_prop_cnt_map, uint8_t cnt_enchant_prop, uint8_t idx_enchant_groups,
		uint8_t idx_enchant_props, Int_Set& output_props, int enchant_weight_rate) {

	if (!equip || !enchant_part_cfg || !enchant_color_cfg || !equip_lvl_cfg ||
			idx_enchant_props >= Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP ||
			idx_enchant_groups >= Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP) {
		return ERROR_INNER_SERVER;
	}

	equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].is_lock = locks[idx_enchant_props];

	if (! locks[idx_enchant_props]) {
		int output_prop = 0;

		/*
		 * 保底: 失败次数
		 */
		int compensatory_prop = 0;
		if (! saved_lock_prop_cnt_map.empty()) {
			for (std::map<int, int>::iterator it_fail_map = fail_prop_cnt_map.begin();
					it_fail_map != fail_prop_cnt_map.end(); ++it_fail_map) {
				std::map<int, int>::const_iterator find_it_saved_map = saved_lock_prop_cnt_map.begin();
				if (find_it_saved_map == saved_lock_prop_cnt_map.end()) {
					continue;
				}
				std::map<int, int>::const_iterator find_it_cfg_cntfail = enchant_mix_cfg.cntlock_cntfail_map.find(find_it_saved_map->second);
				if (find_it_cfg_cntfail == enchant_mix_cfg.cntlock_cntfail_map.end()) {
					continue;
				}
				if (it_fail_map->second >= find_it_cfg_cntfail->second) {
					compensatory_prop = it_fail_map->first;
					//it_fail_map->second = 0;
					break;
				}
			}
		}
		// 失败到一定次数，直接给锁定属性中的一个
		if (compensatory_prop) {
//		if (equip->addon.equip.enchant_lock_prop_fail[idx_enchant_props].fail_cnt >= 8) {
//			Int_Vec lock_prop_vec;
//			Int_Vec lock_prop_weight_vec;
//			for (std::map<int, int>::const_iterator it_saved_lock = saved_lock_prop_cnt_map.begin();
//					it_saved_lock != saved_lock_prop_cnt_map.end(); ++it_saved_lock) {
//				lock_prop_vec.push_back(it_saved_lock->first);
//				lock_prop_weight_vec.push_back(1);
//			}
//			if (! lock_prop_vec.empty()) {
//				uint wight_idx = U_FUNC::get_rand_index(lock_prop_weight_vec);
//				if (wight_idx >= lock_prop_vec.size()) {
//					return ERROR_INNER_SERVER;
//				}
//				output_prop = lock_prop_vec[wight_idx];
//			}

			output_prop = compensatory_prop;

		} else {
			/*
			 * 随机属性
			 */
			//uint wight_idx_part = U_FUNC::get_rand_index(enchant_part_cfg->weights);
			//int test_prop = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type;
			Int_Vec weights;
			cal_enchant_prop_weights(equip, enchant_part_cfg, enchant_mix_cfg, saved_lock_prop_cnt_map,	idx_enchant_groups, weights, enchant_weight_rate);
			uint wight_idx_part = U_FUNC::get_rand_index(weights);
			if (wight_idx_part >= enchant_part_cfg->props.size()) {
				return ERROR_INNER_SERVER;
			}
			output_prop = enchant_part_cfg->props[wight_idx_part];

			// 如果随出的属性在已锁的失败属性里，因该条失败属性已成功随出，要在失败属性里reset掉
//			for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
//				if (equip->addon.equip.enchant_lock_prop_fail[i].prop == output_prop) {
//					equip->addon.equip.enchant_lock_prop_fail[i].reset();
//				}
//			}
		}

//		// 属性已成功产出，要在失败属性里reset掉该类属性
//		for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
//			if (equip->addon.equip.enchant_lock_prop_fail[i].prop == output_prop) {
//				equip->addon.equip.enchant_lock_prop_fail[i].reset();
//			}
//		}

		std::map<int, int>::iterator find_it_fail_map = fail_prop_cnt_map.find(output_prop);
		if (find_it_fail_map != fail_prop_cnt_map.end()) {
			find_it_fail_map->second = 0;
		}

		const Enchant_Prop_Cfg* enchant_prop_cfg = CONFIG_CACHE_EQUIP->find_enchant_prop(output_prop);
		if (! enchant_prop_cfg) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		output_props.insert(output_prop);

		/*
		 * 随机星数
		 */
		uint wight_idx_color = U_FUNC::get_rand_index(enchant_color_cfg->weights);
		if (wight_idx_color >= enchant_color_cfg->stars.size()) {
			return ERROR_INNER_SERVER;
		}
		int rand_star = enchant_color_cfg->stars[wight_idx_color];
		const Enchant_Star_Cfg* enchant_star_cfg = CONFIG_CACHE_EQUIP->find_enchant_star(rand_star);
		if (! enchant_star_cfg) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		/*
		 * 计算属性值
		 */
		double val = 0.0;
		if (M_RACE_FIGHT_PROPS.count(output_prop)) {
			val = enchant_prop_cfg->value * enchant_part_cfg->race_factor *
					equip_lvl_cfg->race_prop_factor * enchant_star_cfg->race_prop_factor;
		} else {
			val = enchant_prop_cfg->value * enchant_part_cfg->factor *
					equip_lvl_cfg->base_prop_factor * enchant_star_cfg->base_prop_factor;
		}

		equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_type = output_prop;
		equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].star = rand_star;
		equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_val = val;
		equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].is_lock = locks[idx_enchant_props];

		equip->addon.equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop = cnt_enchant_prop;
	} else {
		equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props] = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props];
	}

	return 0;
}

void Equiper::set_enchant_lock_prop_fail(Item_Detail *equip, const std::map<int, int>& fail_prop_cnt_map) {
	if (! equip) {
		return;
	}
	uint8_t idx_lock_prop_fail = 0;
	for (std::map<int, int>::const_iterator it_fail_map = fail_prop_cnt_map.begin();
			it_fail_map != fail_prop_cnt_map.end() &&
			idx_lock_prop_fail < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
			++it_fail_map) {
		if (it_fail_map->first) {
			equip->addon.equip.enchant_lock_prop_fail[idx_lock_prop_fail].prop = it_fail_map->first;
			equip->addon.equip.enchant_lock_prop_fail[idx_lock_prop_fail].fail_cnt = it_fail_map->second;
			++idx_lock_prop_fail;
		}

	}
}

int Equiper::req_equip_enchant(const int equip_index, const int8_t is_bat, const int8_t auto_buy_stone, const int8_t auto_buy_lock, const Int8_Vec &locks) {
	if (!equip_index) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail *equip = this->pack_get_item(equip_index);
	if (! equip) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != equip->is_item_type(equip->id_, Item_Detail::EQUIP)) {
		return ERROR_CLIENT_OPERATE;
	}

	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (! item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	// 勋章无法附魔
	if (item_config->equip_cfg.part == EP_MEDAL) {
		return ERROR_PERM_LIMIT;
	}

	// 根据品质（颜色）判断附魔数量
	// 如果判断锁定数据，如果全锁定则直接返回
	uint8_t cnt_enchant_prop = this->cnt_enchant_prop(item_config->color);
	if (locks.size() < cnt_enchant_prop) {
		return ERROR_CLIENT_PARAM;
	}
	if (cnt_enchant_prop > Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	const Enchant_Part_Cfg* enchant_part_cfg = CONFIG_CACHE_EQUIP->find_enchant_part(item_config->equip_cfg.part);
	if (!enchant_part_cfg || enchant_part_cfg->props.empty() || enchant_part_cfg->prop_weight_map.empty() ||
			enchant_part_cfg->props.size() != enchant_part_cfg->prop_weight_map.size()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Enchant_Color_Cfg* enchant_color_cfg = CONFIG_CACHE_EQUIP->find_enchant_color(item_config->color);
	if (!enchant_color_cfg ||
			enchant_color_cfg->stars.empty() || enchant_color_cfg->weights.empty() ||
			enchant_color_cfg->stars.size() != enchant_color_cfg->weights.size()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Equip_Lvl_Cfg* equip_lvl_cfg = CONFIG_CACHE_EQUIP->find_equip_lvl(item_config->equip_cfg.equip_lvl);
	if (! equip_lvl_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	const Enchant_Weight_Ratio_Vec& enchant_weight_ratio_vec = CONFIG_CACHE_EQUIP->enchant_weight_ratio_vec();

	const Enchant_Mix_Cfg& enchant_mix_cfg = CONFIG_CACHE_EQUIP->enchant_mix();

	/*
	 * 计算附魔次数、金钱、材料、锁消耗数
	 */
	int cnt_enchant = 0;				// 附魔实际次数
	int need_copper = 0;
	int need_gold = 0;
	int erase_stone_amount = 0;
	int erase_lock_amount = 0;
	int ret_cal = cal_cnt_enchant(is_bat, auto_buy_stone, auto_buy_lock, locks, enchant_mix_cfg,
			cnt_enchant, need_copper, need_gold, erase_stone_amount, erase_lock_amount);
	if (ret_cal) {
		return ret_cal;
	}

	/*
	 * Check money and items
	 */
	// check money
	Money_Sub_Info money_info_copper(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), need_copper, MONEY_SUB_EQUIP_ENCHANT));
	if (money_info_copper.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info_copper);
		if (try_res) {
			return try_res;
		}
	}
	Money_Sub_Info money_info_gold(Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST), need_gold, MONEY_SUB_EQUIP_ENCHANT));
	if (money_info_gold.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info_gold);
		if (try_res) {
			return try_res;
		}
	}
	// check material
	Id_Amount cost_stone(enchant_mix_cfg.material_id, erase_stone_amount);
	if (cost_stone.id && cost_stone.amount) {
		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, cost_stone);
		if (try_res) {
			return try_res;
		}
	}
	// check lock
	Id_Amount cost_lock(enchant_mix_cfg.lock_id, erase_lock_amount);
	if (cost_lock.id && cost_lock.amount) {
		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, cost_lock);
		if (try_res) {
			return try_res;
		}
	}

	//-------------------------- all check passed line ------------------------------------------------

	/*
	 * Sub money and items
	 */
	if (money_info_copper.nums > 0) {
		player_self()->pack_sub_money(money_info_copper);
	}
	if (money_info_gold.nums > 0) {
		player_self()->pack_sub_money(money_info_gold);
	}
	if (cost_stone.id && cost_stone.amount) {
		 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_stone, Lose_Item_DM_Info(ITEM_LOSE_EQUIP_ENCHANT));
	}
	if (cost_lock.id && cost_lock.amount) {
		 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_lock, Lose_Item_DM_Info(ITEM_LOSE_EQUIP_ENCHANT));
	}

	/*
	 * 附魔装备
	 */
	//uint8_t cnt_enchant = 6;
	bool had_enchanted = equip->addon.equip.enchant_groups[0].enchant_props[0].prop_type;

	if (had_enchanted) {
		for (uint8_t i = 0; i < locks.size() && i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
			equip->addon.equip.enchant_groups[0].enchant_props[i].is_lock = locks[i];
		}
	}

	// 生成 已保存的锁定属性条数map
	std::map<int, int> saved_lock_prop_cnt_map;
	enchant_make_saved_lock_prop_cnt_map(equip, saved_lock_prop_cnt_map);

	/* 单次 */
	if (! is_bat) {
		uint8_t idx_enchant_groups = 0;
		if (! had_enchanted) {
			equip->addon.equip.cnt_enchant_group = 1;
		} else {
			idx_enchant_groups = 1;
			equip->addon.equip.cnt_enchant_group = 2;
		}

		Int_Set output_props;
		std::map<int, int> fail_prop_cnt_map;

		init_enchant_lock_prop_fail(equip, locks, cnt_enchant_prop, fail_prop_cnt_map);

		equip->addon.equip.enchant_groups[idx_enchant_groups].reset();

		int enchant_weight_ratio = get_enchant_weight_ratio(enchant_weight_ratio_vec, *equip);
		for (uint8_t idx_enchant_props = 0;
				idx_enchant_props < cnt_enchant_prop && idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
				++idx_enchant_props) {

			enchant_one_prop(equip, enchant_part_cfg, enchant_color_cfg, equip_lvl_cfg, enchant_mix_cfg, saved_lock_prop_cnt_map,
					locks, fail_prop_cnt_map, cnt_enchant_prop, idx_enchant_groups,	idx_enchant_props, output_props, enchant_weight_ratio);
//
//			Int_Set rand_props;
//
//			equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].is_lock = locks[idx_enchant_props];
//
//			if (! locks[idx_enchant_props]) {
//				int output_prop = 0;
//
//				// 失败到一定次数，直接给锁定属性中的一个
//				if (equip->addon.equip.enchant_lock_prop_fail[idx_enchant_props].fail_cnt >= 8) {
//					Int_Vec lock_prop_vec;
//					Int_Vec lock_prop_weight_vec;
//					for (std::map<int, int>::const_iterator it_saved_lock = saved_lock_prop_cnt_map.begin();
//							it_saved_lock != saved_lock_prop_cnt_map.end(); ++it_saved_lock) {
//						lock_prop_vec.push_back(it_saved_lock->first);
//						lock_prop_weight_vec.push_back(1);
//					}
//					if (! lock_prop_vec.empty()) {
//						uint wight_idx = U_FUNC::get_rand_index(lock_prop_weight_vec);
//						if (wight_idx >= lock_prop_vec.size()) {
//							return ERROR_INNER_SERVER;
//						}
//						output_prop = lock_prop_vec[wight_idx];
//					}
//
//				} else {
//					// 随机属性
//					//uint wight_idx_part = U_FUNC::get_rand_index(enchant_part_cfg->weights);
//					int test_prop = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type;
//					Int_Vec weights;
//					cal_enchant_prop_weights(equip, enchant_part_cfg, enchant_mix_cfg, saved_lock_prop_cnt_map,	idx_enchant_groups, weights);
//					uint wight_idx_part = U_FUNC::get_rand_index(weights);
//					if (wight_idx_part >= enchant_part_cfg->props.size()) {
//						return ERROR_INNER_SERVER;
//					}
//					output_prop = enchant_part_cfg->props[wight_idx_part];
//				}
//
//				const Enchant_Prop_Cfg* enchant_prop_cfg = CONFIG_CACHE_EQUIP->find_enchant_prop(output_prop);
//				if (! enchant_prop_cfg) {
//					return ERROR_CONFIG_NOT_EXIST;
//				}
//
//				// 随机星数
//				uint wight_idx_color = U_FUNC::get_rand_index(enchant_color_cfg->weights);
//				if (wight_idx_color >= enchant_color_cfg->stars.size()) {
//					return ERROR_INNER_SERVER;
//				}
//				int rand_star = enchant_color_cfg->stars[wight_idx_color];
//				const Enchant_Star_Cfg* enchant_star_cfg = CONFIG_CACHE_EQUIP->find_enchant_star(rand_star);
//				if (! enchant_star_cfg) {
//					return ERROR_CONFIG_NOT_EXIST;
//				}
//
//				// 计算属性值
//				double val = 0.0;
//				if (M_RACE_FIGHT_PROPS.count(output_prop)) {
//					val = enchant_prop_cfg->value * enchant_part_cfg->factor *
//							equip_lvl_cfg->race_prop_factor * enchant_star_cfg->race_prop_factor;
//				} else {
//					val = enchant_prop_cfg->value * enchant_part_cfg->factor *
//							equip_lvl_cfg->base_prop_factor * enchant_star_cfg->base_prop_factor;
//				}
//
//				equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_type = output_prop;
//				equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].star = rand_star;
//				equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_val = val;
//				equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].is_lock = locks[idx_enchant_props];
//
//				equip->addon.equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop = cnt_enchant_prop;
//			} else {
//				equip->addon.equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props] = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props];
//			}
		}

		// 本组所有属性产出完，对未洗出的已锁属性进行增加次数
		for (std::map<int, int>::iterator it_fail_map = fail_prop_cnt_map.begin();
				it_fail_map != fail_prop_cnt_map.end(); ++it_fail_map) {
			if (! output_props.count(it_fail_map->first)) {
				++it_fail_map->second;
			}
		}

		// 洗完用fail_prop_cnt_map给数组enchant_lock_prop_fail赋值
		set_enchant_lock_prop_fail(equip, fail_prop_cnt_map);
		++equip->addon.equip.enchant_times;

	/* bat */
	} else {
//		if (cnt_enchant > Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP - 2) {
//			return ERROR_INNER_SERVER;
//		}

		Int_Set output_props;
		std::map<int, int> fail_prop_cnt_map;


		for (uint8_t idx_enchant_groups = 2;
				idx_enchant_groups < cnt_enchant + 2 && idx_enchant_groups < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP;
				++idx_enchant_groups) {

			output_props.clear();
			init_enchant_lock_prop_fail(equip, locks, cnt_enchant_prop, fail_prop_cnt_map);

			//init_enchant_lock_prop_fail(equip, locks);

			equip->addon.equip.enchant_groups[idx_enchant_groups].reset();

			int enchant_weight_ratio = get_enchant_weight_ratio(enchant_weight_ratio_vec, *equip);
			for (uint8_t idx_enchant_props = 0;
					idx_enchant_props < cnt_enchant_prop && idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
					++idx_enchant_props) {

				enchant_one_prop(equip, enchant_part_cfg, enchant_color_cfg, equip_lvl_cfg, enchant_mix_cfg, saved_lock_prop_cnt_map,
						locks, fail_prop_cnt_map, cnt_enchant_prop, idx_enchant_groups,	idx_enchant_props, output_props, enchant_weight_ratio);
//
//				equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].is_lock = locks[idx_enchant_props];
//
//				if (! locks[idx_enchant_props]) {
//					// 随机属性
//					//uint wight_idx_part = U_FUNC::get_rand_index(enchant_part_cfg->weights);
//					Int_Vec weights;
//					cal_enchant_prop_weights(equip, enchant_part_cfg, enchant_mix_cfg, saved_lock_prop_cnt_map,	idx_enchant_groups, weights);
//					uint wight_idx_part = U_FUNC::get_rand_index(weights);
//					if (wight_idx_part >= enchant_part_cfg->props.size()) {
//						return ERROR_INNER_SERVER;
//					}
//					int rand_prop = enchant_part_cfg->props[wight_idx_part];
//					const Enchant_Prop_Cfg* enchant_prop_cfg = CONFIG_CACHE_EQUIP->find_enchant_prop(rand_prop);
//					if (! enchant_prop_cfg) {
//						return ERROR_CONFIG_NOT_EXIST;
//					}
//
//					// 随机星数
//					uint wight_idx_color = U_FUNC::get_rand_index(enchant_color_cfg->weights);
//					if (wight_idx_color >= enchant_color_cfg->stars.size()) {
//						return ERROR_INNER_SERVER;
//					}
//					int rand_star = enchant_color_cfg->stars[wight_idx_color];
//					const Enchant_Star_Cfg* enchant_star_cfg = CONFIG_CACHE_EQUIP->find_enchant_star(rand_star);
//					if (! enchant_star_cfg) {
//						return ERROR_CONFIG_NOT_EXIST;
//					}
//
//					// 计算属性值
//					double val = 0.0;
//					if (M_RACE_FIGHT_PROPS.count(rand_prop)) {
//						val = enchant_prop_cfg->value * enchant_part_cfg->factor *
//								equip_lvl_cfg->race_prop_factor * enchant_star_cfg->race_prop_factor;
//					} else {
//						val = enchant_prop_cfg->value * enchant_part_cfg->factor *
//								equip_lvl_cfg->base_prop_factor * enchant_star_cfg->base_prop_factor;
//					}
//
//					equip->addon.equip.enchant_groups[idx_enchant_groups + 2].enchant_props[idx_enchant_props].prop_type = rand_prop;
//					equip->addon.equip.enchant_groups[idx_enchant_groups + 2].enchant_props[idx_enchant_props].star = rand_star;
//					equip->addon.equip.enchant_groups[idx_enchant_groups + 2].enchant_props[idx_enchant_props].prop_val = val;
//					equip->addon.equip.enchant_groups[idx_enchant_groups + 2].enchant_props[idx_enchant_props].is_lock = locks[idx_enchant_props];
//
//					equip->addon.equip.enchant_groups[idx_enchant_groups + 2].cnt_enchant_prop = cnt_enchant_prop;
//				} else {
//					equip->addon.equip.enchant_groups[idx_enchant_groups + 2].enchant_props[idx_enchant_props] = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props];
//				}
			}

			// 本组所有属性产出完，对未洗出的已锁属性进行增加次数
//			for (uint8_t idx_lock_prop_fail = 0;
//					idx_lock_prop_fail < cnt_enchant_prop &&
//					idx_lock_prop_fail < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
//					++idx_lock_prop_fail) {
//				if (locks[idx_lock_prop_fail] &&
//						!output_props.count(equip->addon.equip.enchant_lock_prop_fail[idx_lock_prop_fail].prop)) {
//					++equip->addon.equip.enchant_lock_prop_fail[idx_lock_prop_fail].fail_cnt;
//				}
//			}
			// 本组所有属性产出完，对未洗出的已锁属性进行增加次数
			for (std::map<int, int>::iterator it_fail_map = fail_prop_cnt_map.begin();
					it_fail_map != fail_prop_cnt_map.end(); ++it_fail_map) {
				if (! output_props.count(it_fail_map->first)) {
					++it_fail_map->second;
				}
			}

			// 洗完用fail_prop_cnt_map给数组enchant_lock_prop_fail赋值
			set_enchant_lock_prop_fail(equip, fail_prop_cnt_map);

			++equip->addon.equip.enchant_times;
		}

		equip->addon.equip.cnt_enchant_group = cnt_enchant + 2;
	}

	refresh_equip_force(equip);

	if (!had_enchanted && !is_bat) {
		if (0 == pack_is_pack_type(equip_index, PACK_T_EQUIP_INDEX)) {
			equiper_prop_refresh(10012);
		} else if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), equip_index)) {
			player_self()->heroer_prop_refresh(player_self()->get_hero_index_by_pack_index(equip_index));
		}
	}

	active_item_addit_info_update(equip);

	if (equip->bind_ != Item_Detail::BIND) {
		equip->bind_ = Item_Detail::BIND;
		MSG_82100000 item_msg;
		Item_Basic_Info item_basic;
		equip->set_item_basic_info(item_basic);
		item_msg.item_info.push_back(item_basic);
		THIS_SEND_TO_CLIENT(item_msg);
	}
	player_self()->active_content_listen_enchanting();//开服活动监听
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_ENCHANTING_NUM);
	MSG_50101005 res_msg;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	player_self()->task_listen_enchant_any_equip();

	return 0;
}

int Equiper::get_enchant_weight_ratio(const Enchant_Weight_Ratio_Vec& vec,
		const Item_Detail &equip) {

	int enchant_times = equip.addon.equip.enchant_times;
	for (Enchant_Weight_Ratio_Vec::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		if (enchant_times < it->times_min || enchant_times > it->times_max) continue;
		return it->ratio;
	}

	return 1;
}

int Equiper::req_equip_enchant_replace_prop_group(const int equip_index, const int8_t group_index) {
	if (!equip_index || group_index >= Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail *equip = this->pack_get_item(equip_index);
	if (! equip) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != equip->is_item_type(equip->id_, Item_Detail::EQUIP)) {
		return ERROR_CLIENT_OPERATE;
	}

	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (! item_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (item_config->equip_cfg.part == EP_MEDAL) {
		return ERROR_PERM_LIMIT;
	}

	if (group_index >= equip->addon.equip.cnt_enchant_group) {
		return ERROR_CLIENT_PARAM;
	}
	if (! equip->addon.equip.enchant_groups[group_index].enchant_props[0].prop_type) {
		return ERROR_CLIENT_OPERATE;
	}

//	for (uint16_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP && i < equip->addon.equip.enchant_groups[group_index].cnt_enchant_prop; ++i) {
//		equip->addon.equip.enchant_groups[0].enchant_props[i] = equip->addon.equip.enchant_groups[group_index].enchant_props[i];
//	}
	equip->addon.equip.enchant_groups[0] = equip->addon.equip.enchant_groups[group_index];

	equip->addon.equip.cnt_enchant_group = 1;


	refresh_equip_force(equip);

	if (0 == pack_is_pack_type(equip_index, PACK_T_EQUIP_INDEX)) {
		equiper_prop_refresh(10010);
	} else if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), equip_index)) {
		player_self()->heroer_prop_refresh(player_self()->get_hero_index_by_pack_index(equip_index));
	}

	active_item_addit_info_update(equip);

	// announce
	enchant_broadcast(equip);

	MSG_50101006 res_msg;
	res_msg.equip_index = equip_index;
	res_msg.group_index = group_index;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Equiper::enchant_prop_refresh(const Item_Detail *equip, std::map<int, double> &pro_val_map) {
	if (equip->addon.equip.cnt_enchant_group >= 1) {
		for (uint8_t idx_enchant_props = 0;
				idx_enchant_props < equip->addon.equip.enchant_groups[0].cnt_enchant_prop &&
				idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
				++idx_enchant_props) {
			if (equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type > 0) {
				int prop_type = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type;
				double prop_val = equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_val;
				if (M_RACE_FIGHT_PROPS.count(prop_type)) {
					prop_val *= 0.01;
				}
				pro_val_map[prop_type] += prop_val;
			}
		}
	}
	return 0;
}

void Equiper::refresh_over_due(Item_Detail *equip) {
	if (! equip) {
		return;
	}
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
	if (! item_config) {
		return;
	}
}

void Equiper::enchant_broadcast(const Item_Detail *equip) {
	if (! equip) {
		return;
	}
	if (equip->addon.equip.cnt_enchant_group >= 1) {
		const Enchant_Mix_Cfg& enchant_mix_cfg = CONFIG_CACHE_EQUIP->enchant_mix();
		for (uint8_t idx_enchant_props = 0;
				idx_enchant_props < equip->addon.equip.enchant_groups[0].cnt_enchant_prop &&
				idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
				++idx_enchant_props) {
			if (equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].is_lock == 0 &&
					enchant_mix_cfg.ann_enchant_prop.count(equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type) &&
					equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].star >= enchant_mix_cfg.ann_enchant_star) {
				player_self()->create_ann_equip_enchant(equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_type,
						equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].prop_val,
						equip->addon.equip.enchant_groups[0].enchant_props[idx_enchant_props].star);
			}
		}
	}
}

void Equiper::init_suit_skill() {

	Int_Vec ids = statistic_suit_skill(PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN,
			PACK_T_EQUIP_INDEX + EQUIP_SUIT_END);

	Skill_Info_Vec skill_vec;
	for (Int_Vec::iterator it= ids.begin(); it != ids.end(); ++it) {
		player_self()->init_be_skill_extra(*it, 1);
//		Skill_Info info;
//		info.skill_id = *it;
//		info.skill_lv = 1;
//		skill_vec.push_back(info);
	}

//	player_self()->module_init_add_be_skills(skill_vec);
}

Int_Vec Equiper::statistic_suit_skill(const int suit_idx_begin, const int suit_idx_end) {
	Int_Vec ids;
	if (0 == equiper_detail_ || !this->packer_detail_pointer()) {
		return ids;
	}
	Item_Detail *equip = 0;
	Int_Int_Map suit_map;

	for (int idx = suit_idx_begin; idx < suit_idx_end; ++idx) {
		equip = 0;
		equip = pack_get_item(idx);
		if (0 == equip) continue;
		const Item_Detail_Config* cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (0 == cfg) continue;
		if (idx < suit_idx_end && 0 != cfg->equip_cfg.suit_id) {
			++suit_map[cfg->equip_cfg.suit_id];
		}
	}

	if (suit_map.empty()) return ids;

	Int_Int_Map skills;
	for (Int_Int_Map::iterator it = suit_map.begin(); it != suit_map.end(); ++it) {
		int id = it->first;
		int count = it->second;

		const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(id);
		if (0 == suit_cfg) continue;
		for (Suit_Count_Cfg_Map::const_iterator cnt_it = suit_cfg->count_cfg_map_.begin();
				cnt_it != suit_cfg->count_cfg_map_.end(); ++cnt_it) {
			if (count < cnt_it->first) continue;
			for (Int_Set::iterator s_it = cnt_it->second.skills.begin();
					s_it != cnt_it->second.skills.end(); ++s_it) {
				++skills[*s_it];
			}
		}
	}

	for (Int_Int_Map::iterator it = skills.begin(); it != skills.end(); ++it) {
		ids.push_back(it->first);
	}

	return ids;
}

Int_Vec Equiper::get_equips_self_skills(const int idx_begin, const int idx_end) {
	Int_Vec skills;
	Item_Detail *equip = NULL;
	if (!equiper_detail_ || !this->packer_detail_pointer()) {
		return skills;
	}
	for (int idx = idx_begin; idx < idx_end; ++idx) {
		equip = pack_get_item(idx);
		if (! equip) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (! item_cfg) {
			continue;
		}
		if (! item_cfg->equip_cfg.skills.empty()) {
			skills.insert(skills.end(), item_cfg->equip_cfg.skills.begin(), item_cfg->equip_cfg.skills.end());
		}
	}
	return skills;
}

void Equiper::init_equips_self_skills(const int idx_begin, const int idx_end) {
	Int_Vec skills = get_equips_self_skills(idx_begin, idx_end);
	Skill_Info_Vec skill_info_vec;
	for (Int_Vec::iterator it= skills.begin(); it != skills.end(); ++it) {
		Skill_Info info;
//		info.skill_id = *it;
//		info.skill_lv = 1;
//		skill_info_vec.push_back(info);
		player_self()->init_be_skill_extra(*it, 1);
	}
//	player_self()->module_init_add_be_skills(skill_info_vec);
}

bool Equiper::has_put_on_suit_fully(const int suit_id, const int hero_index) {
	const std::map<int, int>* ptr_suit_cnt_map = NULL;
	if (hero_index) {
		if (! player_self()->has_hero_index(hero_index)) {
			return false;
		}
		Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.find(hero_index);
		if (it_hero != player_self()->heroer_detail().hero_map.end()) {
			ptr_suit_cnt_map = &it_hero->second.suit_cnt_map;
		}
	} else {
		ptr_suit_cnt_map = &equiper_detail_->suit_cnt_map;
	}
	if (!ptr_suit_cnt_map || ptr_suit_cnt_map->empty()) {
		return false;
	}

	std::map<int, int>::const_iterator it_suit_map = ptr_suit_cnt_map->find(suit_id);
	if (it_suit_map != ptr_suit_cnt_map->end()) {
		int suit_id = it_suit_map->first;
		int suit_cnt = it_suit_map->second;
		const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(suit_id);
		if (! suit_cfg) {
			return false;
		}
		if (suit_cnt >= suit_cfg->max_cnt_put_on) {
			return true;
		}
	}

	return false;
}

int Equiper::get_put_on_suit_count(const int suit_id,
		const int hero_index) {
	const std::map<int, int>* ptr_suit_cnt_map = NULL;
	if (hero_index) {
		if (! player_self()->has_hero_index(hero_index)) {
			return 0;
		}
		Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.find(hero_index);
		if (it_hero != player_self()->heroer_detail().hero_map.end()) {
			ptr_suit_cnt_map = &it_hero->second.suit_cnt_map;
		}
	} else {
		ptr_suit_cnt_map = &equiper_detail_->suit_cnt_map;
	}
	if (!ptr_suit_cnt_map || ptr_suit_cnt_map->empty()) {
		return 0;
	}

	std::map<int, int>::const_iterator it_suit_map = ptr_suit_cnt_map->find(suit_id);
	if (it_suit_map != ptr_suit_cnt_map->end()) {
		int suit_id = it_suit_map->first;
		int suit_cnt = it_suit_map->second;
		const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(suit_id);
		if (! suit_cfg) {
			return 0;
		}
		return suit_cnt;
	}

	return 0;
}

void Equiper::get_jewelry_color_num(std::map<int32_t, int32_t>  &jewelry_map) {
	jewelry_map.clear();

	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		const Item_Detail *equip = player_self()->pack_get_const_item(index);
		if (! equip) {
			continue;
		}

		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (!item_config) {
			continue;
		}

		int position = player_self()->get_part_by_id((int)equip->id_);
		if(position != EP_JEWELRY) {
			continue;
		}

		std::map<int32_t, int32_t>::iterator find_it = jewelry_map.find((int32_t)item_config->color);
		if(find_it == jewelry_map.end()) {
			jewelry_map.insert(std::make_pair((int32_t)item_config->color, equip->amount_));
		}
		else {
			find_it->second += equip->amount_;
		}
	}
}

void Equiper::get_equip_color_num(int equip_part, std::map<int32_t, int32_t>  &equip_map)
{
	equip_map.clear();
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
			const Item_Detail *equip = player_self()->pack_get_const_item(index);
			if (! equip) {
				continue;
			}

			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
			if (!item_config) {
				continue;
			}

			int position = player_self()->get_part_by_id((int)equip->id_);
			if(position != equip_part) {
				continue;
			}

			std::map<int32_t, int32_t>::iterator find_it = equip_map.find((int32_t)item_config->color);
			if(find_it == equip_map.end()) {
				equip_map.insert(std::make_pair((int32_t)item_config->color, equip->amount_));
			}
			else {
				find_it->second += equip->amount_;
			}
		}
}

void Equiper::refresh_equip_avatar(Int_Int_Map &avatar_map) {
	Item_Detail *equip = 0;
	for (int index = PACK_T_EQUIP_INDEX + EQUIP_BEGIN; index < PACK_T_EQUIP_INDEX + EQUIP_END; ++index) {
		equip = this->pack_get_item(index);
		if (0 == equip) continue;

		// 是否过期
		if (equip->overdue_time > Time_Value::zero
				&& equip->overdue_time <= Time_Value::gettimeofday()) continue;

		const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (0 == cfg) continue;

		if (cfg->equip_cfg.avatar_map.empty()) continue;

		Equip_Avatar_Map::const_iterator find_it_1 = cfg->equip_cfg.avatar_map.find(player_self()->career());
		if (find_it_1 == cfg->equip_cfg.avatar_map.end()) continue;
		Int_Int_Map::const_iterator find_it_2 = find_it_1->second.find(player_self()->gender());
		if (find_it_2 == find_it_1->second.end()) continue;

//		player_self()->set_fashion_id(find_it_2->second);
		avatar_map[cfg->equip_cfg.part] = find_it_2->second;
	}
}
