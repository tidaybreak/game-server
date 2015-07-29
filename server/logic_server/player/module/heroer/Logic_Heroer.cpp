/*
 * Logic_Heroer.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Hero.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Role.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "equiper/Equiper_Def.h"
#include "equiper/Equiper.h"
#include "equiper/Equiper_Struct.h"
#include "equiper/Equiper_Func.h"
#include "Logic_Heroer_Def.h"
#include "Logic_Heroer.h"
#include "Logic_Heroer_Struct.h"
#include "Logic_Heroer_Func.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Hero_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Fighter_Detail.h"
#include "NPC/hero/Hero_Struct.h"
#include "task/Task_Struct.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Equip.h"
#include "Msg_Rune_Stone_Struct.h"
#include "rune_stone/Rune_Stone_Def.h"
#include "Config_Cache_Gang.h"
#include "gang/Gang_Struct.h"
#include "rune/Rune_Struct.h"
#include "public/Public_Def.h"
#include "public/Public_Struct.h"
#include <set>
#include "Config_Cache_Buff.h"
#include "status/Logic_Status_Struct.h"
#include "status/Status_Func.h"
#include "Config_Cache_Task.h"

Logic_Heroer::Logic_Heroer(void) {
	reset();
}

Logic_Heroer::~Logic_Heroer() {

}

void Logic_Heroer::reset(void) {
	heroer_detail_ = 0;
}

int Logic_Heroer::create_init(Heroer_Detail &detail) {
	init_artifacts(detail);
	return 0;
}

void Logic_Heroer::load_detail(Heroer_Detail &detail) {
	heroer_detail_ = &detail;
}

void Logic_Heroer::save_detail(Heroer_Detail &detail) {
	if (heroer_detail().is_change) {
		detail = heroer_detail();
		heroer_detail_->is_change = false;
	}
}

void Logic_Heroer::sign_in(void) {

}

void Logic_Heroer::sign_out(void) {

}

void Logic_Heroer::module_init(void) {
	// 神器初始化
	init_artifacts();
	// 刷新全部神器属性
	refresh_artifacts_pro_val();
	// 人物神器加成
	player_self()->player_artifacts_prop_refresh();

	// 刷新英雄属性技能(不发消息)
	for (Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		// 一二级转化率
		const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
		if (hero_config) {
			for (Int_Double_Map::const_iterator it_prop_val = hero_config->hero_prop_conver.begin();
					it_prop_val != hero_config->hero_prop_conver.end(); ++it_prop_val) {
				it_hero->second.hero_fighter_detail.modify_percent_property(Property_Type(it_prop_val->first),
						AT_CONVERSION, O_SET, it_prop_val->second);
			}
		}
		heroer_skill_refresh(it_hero->second.hero_index, false, false);
		heroer_prop_refresh(it_hero->second.hero_index, false, false);
		if (hero_config)
			heroer_assist_skill_refresh(*hero_config, it_hero->second);
	}

	init_panal_tags();

	hero_status_load_init();

	Int_Double_Map props;
	Int_Int_Vec old_skills;
	pos_refresh_hero_assist(props, old_skills);
	for (Int_Int_Vec::iterator it = old_skills.begin(); it != old_skills.end(); ++it) {
		player_self()->init_be_skill_extra(it->val_1, it->val_2);
	}
}

int Logic_Heroer::client_ready_now(void) {
//	if (heroer_detail_->outer_index) {
//		active_hero_info(heroer_detail_->outer_index);
//	}
//	if (heroer_detail_->fighter_index && heroer_detail_->fighter_index != heroer_detail_->outer_index) {
//		active_hero_info(heroer_detail_->fighter_index);
//	}
	return 0;
}

int Logic_Heroer::move_scene_completed(void) {
	return 0;
}

int Logic_Heroer::enter_move_scene(void){
	int fight_index = get_hero_fight_index();
	if (fight_index) {
		req_hero_set_fight(fight_index, 1, false, true);
		active_hero_info(fight_index);
	}else{
		int out_index = get_hero_out_index();
		if (out_index != 0) {
			req_hero_out(out_index, 1, false, true);
		}
	}
	return 0;
}

void Logic_Heroer::hero_time_up(const Time_Value &now) {
	while (! heroer_detail_->out_status_timepri_list.empty()) {
		Status_Info *status_info = heroer_detail_->out_status_timepri_list.front();
		if (status_info->last_end_time > now) break;
		heroer_detail_->out_status_timepri_list.pop_front();

		// sync scene
		this->hero_erase_status_type(status_info->status_id_);
	}
}

int Logic_Heroer::trigger_daily_six(const Time_Value &now) {

	if (0 == heroer_detail_) return 0;
	for (Heroer_Detail::Hero_Map::iterator it = heroer_detail_->hero_map.begin();
			it != heroer_detail_->hero_map.end(); ++it) {
		it->second.community_times = 0;
		it->second.gift_times = 0;
	}
	heroer_detail_->detail_change();
	return 0;
}

int Logic_Heroer::req_hero_amount(void) {
	return heroer_detail_->hero_map.size();
}

int Logic_Heroer::req_fetch_heroer_info(void) {
	MSG_50102000 res_msg;
	Hero_Info hero_info;
	for (Heroer_Detail::Hero_Map::iterator iter = heroer_detail_->hero_map.begin();
			iter != heroer_detail_->hero_map.end(); ++iter) {
		hero_info.reset();
		heroer_detail_->get_hero_info(iter->second.hero_index, hero_info);
		res_msg.hero_info_vec.push_back(hero_info);

		if (!res_msg.outer_index && iter->second.is_outer) {
			res_msg.outer_index = iter->second.hero_index;
		}
		if (!res_msg.fighter_index && iter->second.is_fighter) {
			res_msg.fighter_index = iter->second.hero_index;
		}
	}
	res_msg.show_hero = heroer_detail_->outer_index;

	Int_Int int_val;
	for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
			it != heroer_detail_->hero_assist_map.end(); ++it) {
		int_val.reset();
		int_val.val_1 = it->first;
		int_val.val_2 = it->second;
		res_msg.assist_hero_vec.push_back(int_val);
	}

	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::active_hero_info(const int hero_index, int active_type) {
	MSG_81001001 active_msg;
	heroer_detail_->get_hero_info(hero_index, active_msg.hero_info);
	active_msg.active_type = active_type;
	THIS_SEND_TO_CLIENT(active_msg);
	return 0;
}

int Logic_Heroer::active_panal_hero_info(Logic_Player* target_player) {
	if (! target_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	MSG_82210000 active_msg;
	active_msg.target_role_id = target_player->role_id();
	Hero_Info hero_info;

	for (std::vector<Panal_Hero_Tag>::const_iterator iter = target_player->heroer_detail().panal_hero_tags.begin();
			iter != target_player->heroer_detail().panal_hero_tags.end(); ++iter) {
		hero_info.reset();
		target_player->heroer_detail().get_hero_info(iter->hero_index, hero_info);
		active_msg.hero_info_vec.push_back(hero_info);

		if (!active_msg.outer_index && hero_info.is_outer) {
			active_msg.outer_index = hero_info.hero_index;
		}
		if (!active_msg.fighter_index && hero_info.is_fighter) {
			active_msg.fighter_index = hero_info.hero_index;
		}
	}

//	for (Heroer_Detail::Hero_Map::const_iterator iter = target_player->heroer_detail().hero_map.begin();
//			iter != target_player->heroer_detail().hero_map.end(); ++iter) {
//		hero_info.reset();
//		target_player->heroer_detail().get_hero_info(iter->second.hero_index, hero_info);
//		active_msg.hero_info_vec.push_back(hero_info);
//
//		if (!active_msg.outer_index && iter->second.is_outer) {
//			active_msg.outer_index = iter->second.hero_index;
//		}
//		if (!active_msg.fighter_index && iter->second.is_fighter) {
//			active_msg.fighter_index = iter->second.hero_index;
//		}
//	}

	THIS_SEND_TO_CLIENT(active_msg);
	return 0;
}

int Logic_Heroer::active_panal_hero_tags(Logic_Player* target_player) {
	if (! target_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}
	MSG_82210001 active_msg;
	for (std::vector<Panal_Hero_Tag>::const_iterator it = target_player->heroer_detail().panal_hero_tags.begin();
			it != target_player->heroer_detail().panal_hero_tags.end(); ++it) {
		active_msg.panal_heros_tags.push_back(it->hero_index);
	}
	THIS_SEND_TO_CLIENT(active_msg);
	return 0;
}

void Logic_Heroer::active_hero_int_info_change(void) {
	int outer_awake_lvl = get_outer_hero_awake_lvl();
	if (! outer_awake_lvl) {
		return;
	}
	MSG_81000067 active_msg;
	active_msg.role_id = role_id();
	active_msg.int_type = 32;
	Int32_Info_Change int32_info_change;
	int32_info_change.type = IICT_HERO_AWAKE_LVL;
	int32_info_change.value = outer_awake_lvl;
	active_msg.vec_32_change_info.push_back(int32_info_change);
	player_self()->active_int_info_change(active_msg);
}

int Logic_Heroer::req_hero_equip_on_off(const int32_t index_from, const int32_t index_to) {
	Item_Detail *equip_from = this->pack_get_item(index_from);
	if (! equip_from) {
		return ERROR_ITEM_NOT_EXIST;
	}

	int id_from = equip_from->id_;
	int id_to = 0;
	{
		Item_Detail *equip_to = this->pack_get_item(index_to);
		if (equip_to)
			id_to = equip_to->id_;
	}

	/* 脱下装备 */
	if (pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), index_from)) {
		int result = this->pack_move_item(index_from, Pack::PACK_T_PACKAGE_INDEX);
		if (result != 0) {
			return result;
		}

		heroer_skill_refresh(get_hero_index_by_pack_index(index_from));
		heroer_prop_refresh(get_hero_index_by_pack_index(index_from));

	/* 穿上装备 */
	} else if (pack_is_pack_type(index_from, Pack::PACK_T_PACKAGE_INDEX) == 0 &&
			pack_is_hero_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), index_to)) {
		//int hero_index = get_hero_index_by_pack_index(index_to);
		if (! has_hero_index(get_hero_index_by_pack_index(index_to))) {
			return ERROR_ITEM_NOT_EXIST;
		}

		const Item_Detail_Config *equip_from_cfg = CONFIG_CACHE_ITEM->find_item(equip_from->id_);
		if (! equip_from_cfg) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		// check part
		int part = equip_from_cfg->equip_cfg.part;
		if (! is_equip_part(part)) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (! check_hero_equip_idx_by_part(index_to, part)) {
			return ERROR_CLIENT_PARAM;
		}

		// check career
		if (equip_from_cfg->equip_cfg.career &&
				equip_from_cfg->equip_cfg.career != CAREER_HERO) {
			return ERROR_ITEM_UNUSABLE_CAREER;
		}
		Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(get_hero_index_by_pack_index(index_to));
		if (it_hero == heroer_detail_->hero_map.end()) {
			return ERROR_HERO_NOT_EXIST;
		}
		if (EP_WEAPONS == part && equip_from_cfg->equip_cfg.hero_id &&
				equip_from_cfg->equip_cfg.hero_id != it_hero->second.hero_id) {
			return ERROR_EQUIP_NOT_THIS_HERO;
		}

		// 过期
		if (equip_from->overdue_time > Time_Value::zero && equip_from->overdue_time <= Time_Value::gettimeofday()) {
			return ERROR_ITEM_OVERDUE;
		}

		// player level
		if (player_self()->level() < equip_from_cfg->needLevel) {
			player_self()->send_tip_to_client(ERROR_LEVEL_NEED_LIMIT,equip_from_cfg->needLevel);
			return -1;
		}

		int result = this->pack_move_item(index_from, index_to);
		if (result != 0) {
			return result;
		}

		set_item_bind(equip_from);

		heroer_skill_refresh(get_hero_index_by_pack_index(index_to));
		heroer_prop_refresh(get_hero_index_by_pack_index(index_to));

//		Item_Detail *equip_from_guide = this->pack_get_item(index_from);
		if (equip_from) {
			player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_HERO_HAS_A_EQUIP, equip_from->id_);
			player_self()->task_listen_hero_pull_on_equip(equip_from->id_);
			player_self()->task_listen_equip_jewelry();
			player_self()->task_listen_have_equip_but_no_erase();
			player_self()->task_listen_equip_purple_ring();
		}

		player_self()->achievement_listen_equip_on_body_both_player_and_hero_by_lvl();

	} else {
		return ERROR_CLIENT_OPERATE;
	}

	player_self()->task_listen_hero_equip_n();

	// task_listen_suit
	player_self()->task_listen_role_hero_suit(id_from, 1);
	player_self()->task_listen_role_hero_suit(id_to, 1);

	MSG_50102002 msg;
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Logic_Heroer::req_hero_rune_stone_on_off(const int index_from, const int index_to) {
	if (!index_from || !index_to) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail *item_from = this->pack_get_item(index_from);
	if (! item_from) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != item_from->is_item_type(item_from->id_, Item_Detail::RUNE_STONE)) {
		return ERROR_CLIENT_OPERATE;
	}

	Hero_Pack_Module hpm_from = static_cast<Hero_Pack_Module>(pack_get_hpm(index_from));
	Hero_Pack_Module hpm_to = static_cast<Hero_Pack_Module>(pack_get_hpm(index_to));
	/* 脱下 */
	if (hpm_from == HPM_RUNE_STONE_EQUIP_INDEX &&
			0 == pack_is_pack_type(index_to, PACK_T_RUNE_STONE_PACK_INDEX)) {
	//if (pack_is_pack_type(index_from, Pack::PACK_T_RUNE_STONE_EQUIP_INDEX) == 0) {

		int ret_move = this->pack_move_item(index_from, Pack::PACK_T_RUNE_STONE_PACK_INDEX);
		if (ret_move) {
			return ret_move;
		}

		heroer_prop_refresh(get_hero_index_by_pack_index(index_from));

	/* 穿上 */
	} else if (0 == pack_is_pack_type(index_from, PACK_T_RUNE_STONE_PACK_INDEX) &&
			hpm_to == HPM_RUNE_STONE_EQUIP_INDEX) {
//	//} else if (pack_is_pack_type(index_from, Pack::PACK_T_RUNE_STONE_PACK_INDEX) == 0 &&
//	//		pack_is_pack_type(index_to, Pack::PACK_T_RUNE_STONE_EQUIP_INDEX) == 0) {

		const Item_Detail_Config *item_from_cfg = CONFIG_CACHE_ITEM->find_item(item_from->id_);
		if (!item_from_cfg || !item_from_cfg->rune_stone.rune_type) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		int pack_type = pack_get_hero_pack_type(index_to) ;
		if (player_self()->has_put_on_same_rune_type(pack_type, item_from_cfg->rune_stone.rune_type)) {
			return ERROR_RUNE_STONE_PUT_ON_SAME_TYPE;
		}

		int ret_move = this->pack_move_item(index_from, index_to);
		if (ret_move) {
			return ret_move;
		}

		player_self()->set_item_bind(item_from);

		heroer_prop_refresh(get_hero_index_by_pack_index(index_to));
	} else {
		return ERROR_CLIENT_PARAM;
	}

	MSG_50103209 res_msg;
	res_msg.index_from = index_from;
	res_msg.index_to = index_to;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Logic_Heroer::req_hero_rune_stone_move(const int index_from, const int index_to) {
	if (!index_from || !index_to) {
		return ERROR_CLIENT_PARAM;
	}

	Item_Detail *item_from = this->pack_get_item(index_from);
	if (! item_from) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (0 != item_from->is_item_type(item_from->id_, Item_Detail::RUNE_STONE)) {
		return ERROR_CLIENT_OPERATE;
	}
	if (HPM_RUNE_STONE_EQUIP_INDEX != pack_get_hpm(index_to)) {
		return ERROR_CLIENT_OPERATE;
	}

	int ret_move = this->pack_move_item(index_from, index_to);
	if (ret_move) {
		return ret_move;
	}

	Pack_Type pack_type_to;
	pack_get_pack_type(index_to, pack_type_to);

	MSG_50103206 res_msg;
	res_msg.index_from = index_from;
	res_msg.index_type_to = pack_type_to;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Logic_Heroer::req_new_hero_click(const int hero_id) {
	if (! hero_id) {
		return ERROR_CLIENT_PARAM;
	}
	heroer_detail_->clicked_new_heros.insert(hero_id);
	heroer_detail_->detail_change();

	MSG_50102021 res_msg;
	res_msg.hero_id = hero_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::req_fetch_clicked_new_heros(void) {
	MSG_50102022 res_msg;
	res_msg.clicked_new_heros.insert(res_msg.clicked_new_heros.begin(),
			heroer_detail_->clicked_new_heros.begin(), heroer_detail_->clicked_new_heros.end());
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::hero_swap_equips(const int32_t hero_idx_from, const int32_t hero_idx_to) {
	if (!hero_idx_from || !hero_idx_to || !has_hero_index(hero_idx_to)) {
		return 0;
	}

	const Item_Detail *equip_from = NULL;
	int pack_idx_to = 0;
	int pack_type_from = this->pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), hero_idx_from);
	int pack_type_to = this->pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX), hero_idx_to);

	for (int pack_idx_from = pack_type_from + EQUIP_BEGIN;
			pack_idx_from < pack_type_from + EQUIP_END; ++pack_idx_from) {
		equip_from = this->pack_get_const_item(pack_idx_from);
		if (! equip_from) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(equip_from->id_);
		if (! item_cfg) {
			continue;
		}
		// hero weapons not swap
		if (EP_WEAPONS == item_cfg->equip_cfg.part) {
			int hero_id_to = heroer_get_hero_id(hero_idx_to);
			if (item_cfg->equip_cfg.hero_id && item_cfg->equip_cfg.hero_id != hero_id_to) {
				MSG_81001002 active_msg;
				active_msg.wrong_id = hero_id_to;
				active_msg.right_id = item_cfg->equip_cfg.hero_id;
				active_msg.item_id = equip_from->id_;
				THIS_SEND_TO_CLIENT(active_msg);
			}
			continue;
		}
		pack_idx_to = pack_idx_from - pack_type_from + pack_type_to;
		hero_swap_equip(pack_idx_from, pack_idx_to);
	}

	return 0;
}

int Logic_Heroer::hero_swap_equip(const int32_t pack_idx_from, const int32_t pack_idx_to) {
	int result = this->pack_move_item(pack_idx_from, pack_idx_to);
	if (result != 0) {
		return result;
	}
	return 0;
}

int Logic_Heroer::req_set_hero_out_state(int8_t is_out){
	int hero_index = 0;
	if(is_out == 0){
		hero_index = get_hero_out_index();
	}else{
		if(heroer_detail_->fighter_index != 0){
			hero_index = heroer_detail_->fighter_index;
		}else{
			hero_index = heroer_detail_->last_fighter_idx;
		}
	}
	heroer_detail_->outer_index = is_out;
	heroer_detail_->detail_change();
	if(hero_index != 0){
		return req_hero_out(hero_index, is_out);
	}else{
		MSG_50102003 res_msg;
		res_msg.is_success = 1;
		res_msg.is_alert_res = 0;
		res_msg.is_out = is_out;
		THIS_SEND_TO_CLIENT(res_msg);
	}
	return 0;
}

int Logic_Heroer::req_hero_out(const int32_t hero_index, const int8_t is_out, bool alert_res, bool transfer) {
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}

	// 特殊场景英雄不可跟随
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->scene_id());
	if (scene_config && scene_config->hero_disable) {
		return ERROR_HERO_OUT_SCENE_FORBIT;
	}

	if (is_out) {
		//收回上一次出战的
		MSG_81001001 active_msg;
		for (Heroer_Detail::Hero_Map::iterator it_out = heroer_detail_->hero_map.begin();
				it_out != heroer_detail_->hero_map.end(); ++it_out) {
			if (it_out->second.is_outer) {
				it_out->second.is_outer = 0;
				this->sync_hero_out_to_scene(0);

				active_msg.reset();
				heroer_detail_->get_hero_info(it_out->second.hero_index, active_msg.hero_info);
				if (! transfer) {
					THIS_SEND_TO_CLIENT(active_msg);
				}
			}
		}

		it_hero->second.is_outer = 1;
		heroer_detail_->refresh_hero_base_detail(hero_index, heroer_detail_->role_id, player_self()->role_name());
		// syn_to_map
		this->sync_hero_detail_to_scene(OUTER_HERO_ONLY);
	} else {
		it_hero->second.is_outer = 0;
		heroer_detail_->hero_base_detail_.reset();
	}

	heroer_detail_->detail_change();

	// 发送跟随状态到地图服
	this->sync_hero_out_to_scene(is_out);

	if (! transfer) {
		active_hero_info(it_hero->second.hero_index);
	}

	MSG_50102003 res_msg;
	res_msg.is_success = 1;
	res_msg.is_alert_res = alert_res;
	res_msg.is_out = is_out;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::req_hero_recruit(const int32_t hero_id, const int8_t use_diamond) {
	if (! hero_id) {
		return -1;
	}
	if (has_hero_id(hero_id)) {
		return ERROR_HERO_HAS_EXIST;
	}
	if (use_diamond && player_self()->vip() < 4) {
		return ERROR_HERO_RECRUIT_VIP_LACK;	// TODO
	}

	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(hero_id);
	if (! hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

//	// condition: gong xun
//	if (player_self()->public_detail().exploit_lvl < hero_config->recruit_exploit_lvl) {
//		return ERROR_HERO_RECRUIT_EXPLOIT_LVL_LACK;
//	}

	Id_Amount_Vec cost_items;
	Money_Sub_List money_list;
	Money_Sub_Info sub_soul_info(MONEY_SUB_TYPE_END, 0, MONEY_SUB_MAX);
	Money_Sub_Info sub_diam_info(MONEY_SUB_TYPE_END, 0, MONEY_SUB_MAX);

	if (use_diamond && player_self()->vip() >= 4) {
		// check diamond
		sub_diam_info.type = GOLD_ONLY;
		sub_diam_info.nums = hero_config->vip_recruit_diamond;
		sub_diam_info.dm_info = MONEY_SUB_HERO_RECRUIT;
		int sub_result = player_self()->pack_try_sub_money(sub_diam_info);
		if (sub_result) {
			return sub_result;
		}
	} else {
		// condition: open level
		if (player_self()->level() < hero_config->hero_open_lvl) {
			return ERROR_ROLE_LV_LIM;
		}
		// condition: gong xun
		if (player_self()->public_detail().exploit_lvl < hero_config->recruit_exploit_lvl) {
			return ERROR_HERO_RECRUIT_EXPLOIT_LVL_LACK;
		}
		// check task
		if (! is_fini_recruit_task(hero_id, hero_config)) {
			return ERROR_HERO_RECRUIT_NOT_FINI_TASK;
		}
		// check soul
		if (hero_config->recruit_cost_soul > 0) {
			sub_soul_info.type = SOULS_ONLY;
			sub_soul_info.nums = hero_config->recruit_cost_soul;
			sub_soul_info.dm_info = MONEY_SUB_HERO_RECRUIT;
			int sub_result = player_self()->pack_try_sub_money(sub_soul_info);
			if (sub_result) {
				return sub_result;
			}
		}
		// check material
		cost_items = hero_config->recruit_cost_items;
		if (! cost_items.empty()) {
			Id_Amount_Vec::iterator it;
			for(it = cost_items.begin(); it != cost_items.end(); ){
				if(it->id == COPPER ){
					money_list.push_back(Money_Sub_Info(COPPER_ONLY, it->amount, MONEY_SUB_HERO_RECRUIT));
					it = cost_items.erase(it);
				}else if(it->id == GOLD ){
					money_list.push_back(Money_Sub_Info(GOLD_ONLY, it->amount, MONEY_SUB_HERO_RECRUIT));
					it = cost_items.erase(it);
				}else if(it->id == BIND_GOLD ){
					money_list.push_back(Money_Sub_Info(BIND_GOLD_ONLY, it->amount, MONEY_SUB_HERO_RECRUIT));
					it = cost_items.erase(it);
				}else if(it->id == DRAGON_SOUL ){
					money_list.push_back(Money_Sub_Info(DRAGON_SOUL_ONLY, it->amount, MONEY_SUB_HERO_RECRUIT));
					it = cost_items.erase(it);
				}else if(it->id == SOULS ){
					money_list.push_back(Money_Sub_Info(SOULS_ONLY, it->amount, MONEY_SUB_HERO_RECRUIT));
					it = cost_items.erase(it);
				}else if(it->id == FRIENDSHIP ){
					money_list.push_back(Money_Sub_Info(FRIENDSHIP_ONLY, it->amount, MONEY_SUB_HERO_RECRUIT));
					it = cost_items.erase(it);
				}else{
					++it;
				}
			}
		}

		if(! cost_items.empty()){
			int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, cost_items);
			if (try_res) {
				return try_res;
			}
		}
		if(! money_list.empty()){
			int try_res = player_self()->pack_try_sub_money(money_list);
			if (try_res) {
				return try_res;
			}
		}
	}

	//-------------------------------- all check passed ---------------------------------------------
	MSG_81000102 acv_msg;
	acv_msg.type = 1;
	// Sub material
	if (! cost_items.empty()) {
		 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_items, Lose_Item_DM_Info(ITEM_LOSE_HERO_RECRUIT));
		 Id_Amount_Vec::iterator it;
		 for(it = cost_items.begin(); it != cost_items.end(); ++it){
			 Item_Detail item(it->id, it->amount, Item_Detail::BIND);
			 Item_Basic_Info item_base;
			 item.set_item_basic_info(item_base);
			 acv_msg.item_info.push_back(item_base);
		 }
	}
	// Sub money
	if (sub_soul_info.type != MONEY_SUB_TYPE_END && sub_soul_info.nums > 0) {
		player_self()->pack_sub_money(sub_soul_info);
		acv_msg.property.push_back(Property(SOULS, sub_soul_info.nums));
	}
	if (sub_diam_info.type != MONEY_SUB_TYPE_END && sub_diam_info.nums > 0) {
		player_self()->pack_sub_money(sub_diam_info);
		acv_msg.property.push_back(Property(GOLD, sub_diam_info.nums));
	}
	if(! money_list.empty()){
		player_self()->pack_sub_money(money_list);
		Money_Sub_List::iterator it;
		for(it = money_list.begin(); it != money_list.end(); ++it){
			if(it->type == SOULS_ONLY){
				acv_msg.property.push_back(Property(SOULS, it->nums));
			}else if(it->type == COPPER_ONLY){
				acv_msg.property.push_back(Property(COPPER, it->nums));
			}else if(it->type == DRAGON_SOUL_ONLY){
				acv_msg.property.push_back(Property(DRAGON_SOUL, it->nums));
			}
		}
	}
	if(!acv_msg.item_info.empty() || !acv_msg.property.empty()){
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	int res_add = hero_add(hero_id, use_diamond);
	if (res_add) {
		return res_add;
	}

	// listening
	//成就监听
	player_self()->achievement_listen_recruit_hero();
	player_self()->other_module_notify_icon_to_close_icon(11701);

	MSG_50102004 res_msg;
	res_msg.hero_id = hero_id;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::req_hero_set_formation(const int32_t hero_index, const int32_t hero_formation, bool is_alert) {
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}
	if (hero_formation != 5 && hero_formation != 8) {	// || hero_formation == it_hero->second.formation
		return ERROR_CLIENT_OPERATE;
	}

	it_hero->second.formation = hero_formation;

	heroer_detail_->detail_change();

//	if (heroer_detail_->outer_index == it_hero->second.hero_index) {
//		heroer_detail_->refresh_hero_base_detail(hero_index, heroer_detail_->role_id, player_self()->role_name());
//		this->sync_hero_base_detail_to_scene(OUTER_HERO);
//	}
	if (heroer_detail_->fighter_index == it_hero->second.hero_index) {
		heroer_detail_->refresh_fight_hero_base(hero_index, heroer_detail_->role_id, player_self()->role_name());
		this->sync_hero_base_detail_to_scene(FIGHT_HERO_ONLY);
	}

	player_self()->team_hook_lineup_change(it_hero->second.formation);

	MSG_50102005 res_msg;
	res_msg.hero_index = hero_index;
	res_msg.hero_formation = hero_formation;
	res_msg.is_alert = is_alert;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::req_hero_set_fight(const int32_t hero_index, const int8_t is_fight, bool alert_res, bool transfer) {
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}

	for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
			it != heroer_detail_->hero_assist_map.end(); ++it) {
		if (hero_index == it->second)
			return ERROR_INNER_SERVER;
	}

	if (heroer_detail_->fighter_index) {
		heroer_detail_->last_fighter_idx = heroer_detail_->fighter_index;
	}

	if (is_fight) {
		// 重置上次战斗英雄状态
		MSG_81001001 active_msg;
		for (Heroer_Detail::Hero_Map::iterator it_fight = heroer_detail_->hero_map.begin();
				it_fight != heroer_detail_->hero_map.end(); ++it_fight) {
			if (it_fight->second.is_fighter) {
				it_fight->second.is_fighter = 0;
				this->sync_hero_fight_to_scene(0);

//				active_msg.reset();
//				heroer_detail_->get_hero_info(it_fight->second.hero_index, active_msg.hero_info);
//				if (! transfer) {
//					THIS_SEND_TO_CLIENT(active_msg);
//				}
			}
		}

		it_hero->second.is_fighter = 1;
		it_hero->second.fight_date = Time_Value::gettimeofday().sec();
		heroer_detail_->fighter_index = hero_index;

		// 出战英雄装备交换
		if (heroer_detail_->last_fighter_idx != heroer_detail_->fighter_index) {
			hero_swap_equips(heroer_detail_->last_fighter_idx, heroer_detail_->fighter_index);

			heroer_skill_refresh(heroer_detail_->last_fighter_idx, false, false);
			heroer_prop_refresh(heroer_detail_->last_fighter_idx, false, false);

			heroer_skill_refresh(heroer_detail_->fighter_index,false, false);
			heroer_prop_refresh(heroer_detail_->fighter_index);

			active_hero_info(heroer_detail_->last_fighter_idx);
		}else{
			heroer_skill_refresh(heroer_detail_->fighter_index, false, false);
			heroer_prop_refresh(heroer_detail_->fighter_index, false, true);
		}

		heroer_detail_->refresh_fight_hero_base(hero_index, heroer_detail_->role_id, player_self()->role_name());
		// syn_to_map
		this->sync_hero_detail_to_scene(FIGHT_HERO_ONLY);
		player_self()->achievement_listen_equip_on_body_both_player_and_hero_by_lvl();
	} else {
		it_hero->second.is_fighter = 0;
		heroer_detail_->fighter_index = 0;
		heroer_detail_->fight_hero_base_.reset();
		heroer_skill_refresh(heroer_detail_->last_fighter_idx, false, false);
		heroer_prop_refresh(heroer_detail_->last_fighter_idx, false, true);
	}

	refresh_panal_tags();
	active_panal_hero_tags(player_self());

	heroer_detail_->detail_change();

	// 发送出战状态到地图服，战斗英雄暂不直接放出，仅用于收回战斗英雄
	if (0 == is_fight) {
		this->sync_hero_fight_to_scene(is_fight);
	}

	if (! transfer) {
		active_hero_info(hero_index);

		Block_Buffer buf;
		buf.make_message(INNER_REFRESH_ROLE_PANEL_PROP);
		buf.finish_message();
		this->send_to_scene(buf);

		//task listen
		{
			if(heroer_detail_->fighter_index) {
				Heroer_Detail::Hero_Map::const_iterator fight_hero =
						heroer_detail_->hero_map.find(heroer_detail_->fighter_index);
				if(fight_hero != heroer_detail_->hero_map.end()) {
					for(std::map<int, int>::const_iterator it = fight_hero->second.suit_cnt_map.begin();
							it != fight_hero->second.suit_cnt_map.end();
							++it) {
						  const Suit_ID_Cfg *suit_cfg = CONFIG_CACHE_EQUIP->find_suit(it->first);
						  if(suit_cfg /*&& it->second >= suit_cfg->max_cnt_put_on*/) {
							  player_self()->task_listen_suit(it->first, it->second);
						  }
					}
				}
			}
		}//end task listen

	}

	MSG_50102006 res_msg;
	res_msg.is_success = 1;
	res_msg.is_alert_res = alert_res;
	res_msg.is_fight = is_fight;
	THIS_SEND_TO_CLIENT(res_msg);

	// listening
	player_self()->team_hook_heroer_change(heroer_detail_->fighter_index);

	int hero_id = heroer_get_hero_id(heroer_detail_->fighter_index);
	if(hero_id != 0) {
		player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_HERO_SET_FIGHT, hero_id);
	}
	// 出战英雄改变，展示英雄切换
	if(heroer_detail_->outer_index != 0){
		if(is_fight){
			req_hero_out(hero_index, 1, false);
		}
	}
	return 0;
}

int Logic_Heroer::req_hero_awake(const int8_t is_hero, const int32_t hero_index, const bool without_condition) {
	if (! is_hero) {
		return player_self()->req_awake_lvl_upgrade(true);
	}

	if (! hero_index) {
		return ERROR_CLIENT_OPERATE;
	}
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
	if (! hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (it_hero->second.awake_lvl >= hero_config->max_awake_lvl) {
		//return ERROR_HERO_AWAKE_LVL_MAX;	// 客户端要求不提示
		return -2;
	}

	if (!without_condition) {
		// check material
		size_t size_cfg_cost_items = hero_config->awake_lvl_cost_items.size();
		Id_Amount_Vec awake_cost_items;
		Money_Sub_List money_list;
		if ((uint)it_hero->second.awake_lvl < size_cfg_cost_items) {
			awake_cost_items = hero_config->awake_lvl_cost_items[it_hero->second.awake_lvl];
			Id_Amount_Vec::iterator it;
			for(it = awake_cost_items.begin(); it != awake_cost_items.end(); ){
				if(it->id == COPPER ){
					money_list.push_back(Money_Sub_Info(COPPER_ONLY, it->amount, MONEY_SUB_HERO_AWAKE));
					it = awake_cost_items.erase(it);
				}else if(it->id == GOLD ){
					money_list.push_back(Money_Sub_Info(GOLD_ONLY, it->amount, MONEY_SUB_HERO_AWAKE));
					it = awake_cost_items.erase(it);
				}else if(it->id == BIND_GOLD ){
					money_list.push_back(Money_Sub_Info(BIND_GOLD_ONLY, it->amount, MONEY_SUB_HERO_AWAKE));
					it = awake_cost_items.erase(it);
				}else if(it->id == DRAGON_SOUL ){
					money_list.push_back(Money_Sub_Info(DRAGON_SOUL_ONLY, it->amount, MONEY_SUB_HERO_AWAKE));
					it = awake_cost_items.erase(it);
				}else if(it->id == SOULS ){
					money_list.push_back(Money_Sub_Info(SOULS_ONLY, it->amount, MONEY_SUB_HERO_AWAKE));
					it = awake_cost_items.erase(it);
				}else if(it->id == FRIENDSHIP ){
					money_list.push_back(Money_Sub_Info(FRIENDSHIP_ONLY, it->amount, MONEY_SUB_HERO_AWAKE));
					it = awake_cost_items.erase(it);
				}else{
					++it;
				}
			}
		}
		if (! awake_cost_items.empty()) {
			int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, awake_cost_items);
			if (try_res) {
				return try_res;
			}
		}
		if(! money_list.empty()){
			int try_res = player_self()->pack_try_sub_money(money_list);
			if (try_res) {
				return try_res;
			}
		}
		//-------------------------------- all check passed ---------------------------------------------
		MSG_81000102 acv_msg;
		acv_msg.type = 1;
		// Sub material
		if (! awake_cost_items.empty()) {
			 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, awake_cost_items,
					 Lose_Item_DM_Info(ITEM_LOSE_AWAKE));
			 Id_Amount_Vec::iterator it;
			 for(it = awake_cost_items.begin(); it != awake_cost_items.end(); ++it){
				 Item_Detail item(it->id, it->amount, Item_Detail::BIND);
				 Item_Basic_Info item_base;
				 item.set_item_basic_info(item_base);
				 acv_msg.item_info.push_back(item_base);
			 }
		}
		//
		if(! money_list.empty()){
			player_self()->pack_sub_money(money_list);
			Money_Sub_List::iterator it;
			for(it = money_list.begin(); it != money_list.end(); ++it){
				if(it->type == SOULS_ONLY){
					acv_msg.property.push_back(Property(SOULS, it->nums));
				}else if(it->type == COPPER_ONLY){
					acv_msg.property.push_back(Property(COPPER, it->nums));
				}else if(it->type == DRAGON_SOUL_ONLY){
					acv_msg.property.push_back(Property(DRAGON_SOUL, it->nums));
				}
			}
		}
		if(!acv_msg.item_info.empty() || !acv_msg.property.empty()){
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}

	++it_hero->second.awake_lvl;

	heroer_prop_refresh(it_hero->second.hero_index, true, false);
	heroer_skill_refresh(it_hero->second.hero_index);


	if (heroer_detail_->outer_index != 0 && hero_index == heroer_detail_->fighter_index) {// 展示英雄
		heroer_detail_->refresh_hero_base_detail(hero_index, role_id(), role_name());
		active_hero_int_info_change();
	}
	this->sync_hero_base_detail_to_scene(OUTER_FIGHT_HERO);

	heroer_detail_->detail_change();

	active_hero_info(hero_index, AT_HERO_AWAKE);

	player_self()->task_listen_wake_up_hero_to_quality_level(it_hero->second.hero_id, it_hero->second.awake_lvl + 1);
	player_self()->achievement_listen_hero_awake_lvl(it_hero->second.awake_lvl);
	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_WAKE_UP_HERO_TO_QUALITY_LEVEL,
									  it_hero->second.hero_id,
								      it_hero->second.awake_lvl + 1);

	MSG_50102007 res_msg;
	res_msg.result = 1;
	res_msg.is_hero = is_hero;
	res_msg.hero_index = hero_index;
//	res_msg.new_awake_skill = new_awake_skill;
	THIS_SEND_TO_CLIENT(res_msg);

	player_self()->task_listen_have_hero_quality();

	return 0;
}

int Logic_Heroer::req_artifact_upgrade(const int8_t is_hero, const int32_t artifact_id, const int32_t hero_index) {
	if (! is_hero) {
		// 玩家神器
		return player_self()->req_player_artifact_upgrade(artifact_id, true);
	}

	if (! hero_index) {
		return ERROR_CLIENT_OPERATE;
	}
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
	if (! hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (! hero_config->artifact_ids.count(artifact_id)) {
		return ERROR_CLIENT_OPERATE;
	}

	const Artifact_Cfg_Id *artifact_cfg_id = CONFIG_CACHE_HERO->find_artifact(artifact_id);
	if (! artifact_cfg_id) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	Artifact_Map::iterator it_artifact = it_hero->second.artifacts.find(artifact_id);
	if (it_artifact == it_hero->second.artifacts.end()) {
		return ERROR_INNER_SERVER;
	}
	if (it_artifact->second.artifact_lvl >= artifact_cfg_id->max_lvl) {
		return ERROR_HERO_ARTIFACT_LVL_MAX;
	}

	const Artifact_Cfg_Lvl *artifact_cfg_lvl_next =
			CONFIG_CACHE_HERO->find_artifact_lvl(it_artifact->second.artifact_lvl + 1);
	if (! artifact_cfg_lvl_next) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	// check soul
	int cost_soul = ceil(artifact_cfg_id->cost_soul_param * artifact_cfg_lvl_next->cost_soul);
	Money_Sub_Info money_info(Money_Sub_Info(Money_Sub_Type(SOULS_ONLY), cost_soul, MONEY_SUB_ARTIFACT_UPGRADE));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	// check material
	Id_Amount_Vec cost_items = artifact_cfg_lvl_next->cost_items;
	for (uint8_t i = 0; i < cost_items.size(); ++i) {
		cost_items[i].amount = ceil(cost_items[i].amount * artifact_cfg_id->cost_item_param);
	}
	if (! artifact_cfg_lvl_next->cost_items.empty()) {
		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, cost_items);
		if (try_res) {
			return try_res;
		}
	}

	//---------------- all check passed ------------------
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}
	// Sub material
	if (! artifact_cfg_lvl_next->cost_items.empty()) {
		 player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_items, Lose_Item_DM_Info(ITEM_LOSE_ARTIFACT_UPGRADE));
	}

	int orig_artifact_lvl = it_artifact->second.artifact_lvl;

	++it_artifact->second.artifact_lvl;

	refresh_artifacts_pro_val();

	active_hero_info(hero_index, AT_HERO_ARTIFACT_UPGRADE);	// 前端说先发更新等级配合播动画

	heroer_prop_refresh();

	heroer_detail_->detail_change();

	int orig_artifact_step = orig_artifact_lvl ?  (orig_artifact_lvl - 1) / 10 + 1  :  1;
	int artifact_step = (it_artifact->second.artifact_lvl - 1) / 10 + 1;
	bool is_step_up = artifact_step > orig_artifact_step ?  1 : 0;

	// announce
	if (is_step_up) {
		int show_step = 0, show_level = 0;
		if (it_artifact->second.artifact_lvl % 10 == 0) {
			show_step = it_artifact->second.artifact_lvl / 10;
			show_level = 10;
		} else {
			show_step = it_artifact->second.artifact_lvl / 10 + 1;
			show_level = it_artifact->second.artifact_lvl % 10;
		}
		player_self()->create_ann_artifact_upgrade(hero_config->hero_name, artifact_cfg_id->artifact_name,
				artifact_step, show_level);
	}

	MSG_50102020 res_msg;
	res_msg.result = 1;
	res_msg.is_hero = is_hero;
	res_msg.artifact_id = artifact_id;
	res_msg.hero_index = hero_index;
	res_msg.is_step_up = is_step_up;
	res_msg.artifact_lvl = it_artifact->second.artifact_lvl;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Logic_Heroer::hero_add(const int hero_id, bool use_diamond /*= false*/, const bool send_client) {
	if (! hero_id) {
		return -1;
	}
	if (has_hero_id(hero_id)) {
		return ERROR_HERO_HAS_EXIST;
	}
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(hero_id);
	if (! hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	Hero_Info_Detail hero_info_detail;

	// 生成hero_index
	for (int index = 1; index <= 100; ++index) {
		Heroer_Detail::Hero_Map::iterator iter = heroer_detail_->hero_map.find(index);
		if (iter != heroer_detail_->hero_map.end()) {
			continue;
		} else {
			hero_info_detail.hero_index = index;
			break;
		}
	}
	if (! hero_info_detail.hero_index) {
		return -1;
	}

	hero_info_detail.hero_id = hero_id;
	hero_info_detail.hero_facade_id = hero_id;
	hero_info_detail.hero_name = hero_config->hero_name;
	hero_info_detail.level = hero_config->hero_init_lvl;
	hero_info_detail.awake_lvl = hero_config->hero_initial_awake;
	hero_info_detail.formation = 8;
	init_artifacts(hero_info_detail);

	if (use_diamond) {
		hero_info_detail.level = 20;
		hero_info_detail.awake_lvl = 2;
	}

	heroer_detail_->hero_map.insert(std::make_pair(hero_info_detail.hero_index,	hero_info_detail));

	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_info_detail.hero_index);
	if (it_hero != heroer_detail_->hero_map.end()) {
		// 一二级转化率
		const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
		if (hero_config) {
			for (Int_Double_Map::const_iterator it_prop_val = hero_config->hero_prop_conver.begin();
					it_prop_val != hero_config->hero_prop_conver.end(); ++it_prop_val) {
				it_hero->second.hero_fighter_detail.modify_percent_property(Property_Type(it_prop_val->first),
						AT_CONVERSION, O_SET, it_prop_val->second);
			}
		}
		bool syn_scene = false;
		if(heroer_detail_->outer_index != 0 && heroer_detail_->fighter_index == it_hero->second.hero_index){
			syn_scene = true;
		}
		heroer_prop_refresh(it_hero->second.hero_index, syn_scene, false);
		heroer_skill_refresh(it_hero->second.hero_index, syn_scene, false);

		heroer_assist_skill_refresh(*hero_config, it_hero->second);
	}

	// 琳，自动设置为战斗英雄并出战
//	if (hero_info_detail.hero_id == HERO_ID_LIN) {
//		heroer_detail_->fighter_index = hero_info_detail.hero_index;
//		req_hero_set_fight(heroer_detail_->fighter_index, 1, false);
//	}

	refresh_panal_tags();

	heroer_detail_->detail_change();

	MSG_81001000 active_msg;
	heroer_detail_->get_hero_info(hero_info_detail.hero_index, active_msg.hero_info);
	THIS_SEND_TO_CLIENT(active_msg);

	// listening
	player_self()->refresh_cards();

	//任务监听获得指定英雄
	player_self()->task_listen_get_hero(hero_id);

	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_GET_HERO, hero_id);

	player_self()->task_listen_have_hero(heroer_detail_->hero_map.size());

	player_self()->task_listen_have_hero_quality();

	if (send_client) {
		MSG_50102004 res_msg;
		res_msg.hero_id = hero_id;
		res_msg.result = 1;
		THIS_SEND_TO_CLIENT(res_msg);
	}
	return 0;
}

int Logic_Heroer::hero_erase(const int hero_index) {
	Heroer_Detail::Hero_Map::iterator iter = heroer_detail_->hero_map.find(hero_index);
	if (iter != heroer_detail_->hero_map.end()) {
		if (hero_index == heroer_detail_->fighter_index) {
			if(heroer_detail_->outer_index != 0){
				req_hero_out(hero_index, 0, false);
			}
			req_hero_set_fight(hero_index, 0, false);
		}
		heroer_detail_->hero_map.erase(iter);
	}
	refresh_panal_tags();
	heroer_detail_->detail_change();
	active_panal_hero_info(player_self());

	return 0;
}

int Logic_Heroer::req_hero_level_up(const int32_t hero_index, const bool without_condition) {
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	} else {
		if (it_hero->second.level >= MAX_HERO_LVL) {
			return ERROR_CLIENT_OPERATE;
		}
		if (it_hero->second.level - player_self()->level() >= MAX_HERO_LVL_GTE_ROLE) {
			return ERROR_HERO_LVL_ROLE_LIMIT;
		}

		const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
		if (! hero_config) {
			return ERROR_CONFIG_NOT_EXIST;
		}

		if ((int)hero_config->hero_lvl_awake_limit.size() <= it_hero->second.awake_lvl) {
			return ERROR_HERO_AWAKE_LVL_MAX;
		}
		if (it_hero->second.level >= hero_config->hero_lvl_awake_limit[it_hero->second.awake_lvl]) {
			//return ERROR_HERO_UPGRADE_AWAKE_LVL_LOW; // 客户端要求不提示
			return -2;
		}

		const Hero_Lvl_Misc_Config *lvl_misc_cfg = CONFIG_CACHE_HERO->find_hero_lvl_misc(it_hero->second.level);
		if (! lvl_misc_cfg)
			return ERROR_CONFIG_NOT_EXIST;

		if (!without_condition) {
			if (lvl_misc_cfg->lvl_up_cost_souls > 0) {
				int final_cost_souls = lvl_misc_cfg->lvl_up_cost_souls * hero_config->factor_lvl_up_soul;
				Money_Sub_Info sub_info(Money_Sub_Info(Money_Sub_Type(SOULS_ONLY), final_cost_souls, MONEY_SUB_HERO_LEVEL_UP));
				int sub_result = player_self()->pack_try_sub_money(sub_info);
				if (sub_result) {
					return sub_result;
				}
				player_self()->pack_sub_money(sub_info);
			}
		}

		// 助阵英雄处理
		Int_Double_Map props;
		Int_Int_Vec old_skills;
		bool is_assist_hero = false;
		for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
				it != heroer_detail_->hero_assist_map.end(); ++it) {
			if (it_hero->second.hero_index == it->second) {
				pre_refresh_hero_assist(props, old_skills);
				is_assist_hero = true;
				break;
			}
		}

		++it_hero->second.level;

		heroer_prop_refresh(it_hero->second.hero_index, true, false);
		heroer_skill_refresh(it_hero->second.hero_index, true, false);
//		heroer_assist_skill_refresh(*hero_config, it_hero->second);

		active_hero_info(hero_index, AT_HERO_LEVEL_UP);

		// listening
		player_self()->task_listen_keep_hero_to_level(it_hero->second.hero_id, it_hero->second.level);

		heroer_detail_->detail_change();

		if (is_assist_hero) {
			pos_refresh_hero_assist(props, old_skills);
			fight_heroer_prop_refresh();
		}
	}

	MSG_50102001 res_msg;
	res_msg.result = 1;
	res_msg.hero_level = it_hero->second.level;
	res_msg.hero_index = hero_index;
	THIS_SEND_TO_CLIENT(res_msg);

	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_HERO_TO_LEVEL, it_hero->second.hero_id, it_hero->second.level);

	return 0;
}

bool Logic_Heroer::has_hero_index(const int hero_index) {
	return heroer_detail_->hero_map.count(hero_index);
}

bool Logic_Heroer::has_hero_id(const int hero_id) {
	for (Heroer_Detail::Hero_Map::iterator it = heroer_detail_->hero_map.begin();
			it != heroer_detail_->hero_map.end(); ++it) {
		if (it->second.hero_id == hero_id) {
			return true;
		}
	}
	return false;
}

int Logic_Heroer::update_hero_sync_info(Hero_Info_Detail &hero_info_detail) {
	// 基本信息
	heroer_detail_->refresh_hero_base_detail(hero_info_detail.hero_index, heroer_detail_->role_id, player_self()->role_name());
//	heroer_detail_->hero_base_detail_.master_role_id = heroer_detail_->role_id;
//	heroer_detail_->hero_base_detail_.master_role_name = player_self()->role_name();
//	//heroer_detail_->hero_base_detail_.hero_role_id = ;
//	heroer_detail_->hero_base_detail_.hero_index = hero_info.hero_index;
//	heroer_detail_->hero_base_detail_.hero_id = hero_info.hero_id;
//	heroer_detail_->hero_base_detail_.hero_name = hero_info.hero_name;
//	heroer_detail_->hero_base_detail_.hero_facade_id = hero_info.hero_facade_id;
//	heroer_detail_->hero_base_detail_.hero_formation = hero_info.formation;

	//heroer_detail_->hero_base_detail_.coord = player_self()->base_detail().location.coord;

	// 战斗信息
//	hero_info_detail.hero_fighter_detail.role_id = heroer_detail_->role_id;
//	heroer_detail_->hero_fighter_detail_.reset();
//	heroer_detail_->hero_fighter_detail_.level = hero_info.level;
//	heroer_detail_->hero_fighter_detail_.experience = hero_info.exp;
//
//	if (! hero_info.prop_value.empty()) {
//		for (size_t i = 0; i < hero_info.prop_value.size(); ++i) {
//			if (hero_info.prop_value[i].id > 0 && hero_info.prop_value[i].value > 0.0) {
//				heroer_detail_->hero_fighter_detail_.modify_fight_property((Property_Type)hero_info.prop_value[i].id, AT_BASIC, O_ADD,
//						hero_info.prop_value[i].value, 0.0, 0.0);
//			}
//		}
//	}

	return 0;
}

int Logic_Heroer::sync_hero_base_detail_to_scene(const int detail_type) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_HERO_BASE_DETAIL);
	switch (detail_type) {
	case OUTER_HERO_ONLY:
		buf.write_int8(OUTER_HERO_ONLY);
		heroer_detail_->hero_base_detail_.serialize(buf);
		break;
	case FIGHT_HERO_ONLY:
		buf.write_int8(FIGHT_HERO_ONLY);
		heroer_detail_->fight_hero_base_.serialize(buf);
		break;
	case OUTER_FIGHT_HERO:
		buf.write_int8(OUTER_FIGHT_HERO);
		heroer_detail_->hero_base_detail_.serialize(buf);
		//heroer_detail_->fight_hero_base_.serialize(buf);
		break;
	default:
		return -1;
	}
	buf.finish_message();
	return this->send_to_scene(buf);
}

int Logic_Heroer::sync_hero_fighter_detail_to_scene(const int detail_type, const Fighter_Detail &hero_fighter_detail) {
	if (detail_type != OUTER_HERO_ONLY && detail_type != FIGHT_HERO_ONLY && detail_type != OUTER_FIGHT_HERO) {
		return -1;
	}
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_HERO_FIGHTER_DETAIL);
	buf.write_int8(detail_type);
	hero_fighter_detail.serialize(buf);
	buf.write_int64(heroer_detail_->role_id);
	heroer_detail_->logic_status_detail_.serialize(buf);
	buf.finish_message();
	return this->send_to_scene(buf);
}

int Logic_Heroer::sync_hero_out_to_scene(int is_out) {
	MSG_20400003 msg;
	msg.reset();
	msg.master_role_id = heroer_detail_->role_id;
	msg.is_out = is_out;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

int Logic_Heroer::sync_hero_fight_to_scene(int is_fight) {
	MSG_20400007 msg;
	msg.reset();
	msg.master_role_id = heroer_detail_->role_id;
	msg.is_fight = is_fight;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

int Logic_Heroer::sync_hero_detail_to_scene(const int detail_type) {
	int hero_idx = 0;
	if(detail_type == OUTER_HERO_ONLY){
		if(heroer_detail_->outer_index != 0){
			if(heroer_detail_->fighter_index != 0){
				hero_idx = heroer_detail_->fighter_index;
			}else{
				hero_idx = heroer_detail_->last_fighter_idx;
			}
		}
	}else{
		hero_idx = heroer_detail_->fighter_index;
	}
	Heroer_Detail::Hero_Map::iterator iter = heroer_detail_->hero_map.find(hero_idx);
	if (iter != heroer_detail_->hero_map.end()) {
		this->update_hero_sync_info(iter->second);
		this->sync_hero_base_detail_to_scene(detail_type);
		this->sync_hero_fighter_detail_to_scene(detail_type, iter->second.hero_fighter_detail);
	}

	return 0;
}

int Logic_Heroer::refresh_heroer_base_detail(void) {
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(heroer_detail_->fighter_index);
	if (it_hero != heroer_detail_->hero_map.end()) {
		heroer_detail_->refresh_hero_base_detail(it_hero->second.hero_index, heroer_detail_->role_id,
				player_self()->role_name());
	}
	return 0;
}

int Logic_Heroer::refresh_heroer_fight_detail() {
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(heroer_detail_->fighter_index);
	if (it_hero != heroer_detail_->hero_map.end()) {
		heroer_detail_->refresh_fight_hero_base(it_hero->second.hero_index, heroer_detail_->role_id,
				player_self()->role_name());
	}
	return 0;
}

int Logic_Heroer::sync_heroer_module_info(Block_Buffer &buf) {
	for (Heroer_Detail::Hero_Map::iterator iter = heroer_detail_->hero_map.begin();
			iter != heroer_detail_->hero_map.end(); ++iter) {
		if (iter->second.is_outer) {
			//this->convert_hero_fight_property(iter->second);
			//this->add_hero_equip_property(iter->second);
			//this->update_hero_sync_info(iter->second);
			heroer_detail_->hero_base_detail_.serialize(buf);
			iter->second.hero_fighter_detail.serialize(buf);
			break;
		}
	}

	return 0;
}

int Logic_Heroer::heroer_get_outer_hero_formation(void) {
	if (heroer_detail_->fighter_index) {
		Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(heroer_detail_->fighter_index);
		if (it_hero != heroer_detail_->hero_map.end()) {
			return it_hero->second.formation;
		}
	}
	return 0;
}

int Logic_Heroer::heroer_get_hero_index(const int hero_id) {
	for (Heroer_Detail::Hero_Map::iterator it = heroer_detail_->hero_map.begin();
			it != heroer_detail_->hero_map.end(); ++it) {
		if (it->second.hero_id == hero_id) {
			return it->second.hero_index;
		}
	}
	return 0;
}

int Logic_Heroer::heroer_get_hero_id(const int hero_index) {
	for (Heroer_Detail::Hero_Map::iterator it = heroer_detail_->hero_map.begin();
			it != heroer_detail_->hero_map.end(); ++it) {
		if (it->second.hero_index == hero_index) {
			return it->second.hero_id;
		}
	}
	return 0;
}

int Logic_Heroer::get_fight_hero_lvl_limit(const int hero_index) {
	Heroer_Detail::Hero_Map::const_iterator find_it = heroer_detail_->hero_map.find(hero_index);
	if (find_it != heroer_detail_->hero_map.end()) {
		return get_level_limit(find_it->second);
	}
	return 0;
}

int Logic_Heroer::get_fight_hero_lvl(const int hero_index) {
	Heroer_Detail::Hero_Map::const_iterator find_it = heroer_detail_->hero_map.find(hero_index);
	if (find_it != heroer_detail_->hero_map.end()) {
		return find_it->second.level;
	}
	return 0;
}

int Logic_Heroer::get_heros_lvl(Int_Int_Map &id_lvl_map) {
	id_lvl_map.clear();
	for (Heroer_Detail::Hero_Map::const_iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		id_lvl_map.insert(std::make_pair(it_hero->second.hero_id, it_hero->second.level));
	}
	return 0;
}

int Logic_Heroer::get_heros_awake_lvl(Int_Int_Map &id_lvl_map) {
	id_lvl_map.clear();
	for (Heroer_Detail::Hero_Map::const_iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		id_lvl_map.insert(std::make_pair(it_hero->second.hero_id, it_hero->second.awake_lvl));
	}
	return 0;
}

int Logic_Heroer::get_fight_hero_force(void) {
	if (heroer_detail_->fighter_index) {
		Heroer_Detail::Hero_Map::const_iterator it_hero = heroer_detail_->hero_map.find(heroer_detail_->fighter_index);
		if (it_hero != heroer_detail_->hero_map.end()) {
			return it_hero->second.hero_fighter_detail.fetch_fight_property(PT_FORCE);
		}
	}
	return 0;
}

int Logic_Heroer::get_outer_hero_awake_lvl(void) {
	int out_index = get_hero_out_index();
	if (out_index != 0) {
		Heroer_Detail::Hero_Map::const_iterator it_hero = heroer_detail_->hero_map.find(out_index);
		if (it_hero != heroer_detail_->hero_map.end()) {
			return it_hero->second.awake_lvl;
		}
	}
	return 0;
}

int Logic_Heroer::get_hero_rune_pro_val(std::map<int, double> &pro_val_map) const {
	pro_val_map.clear();
	pro_val_map = heroer_detail_->hero_rune_pro_val;
	return 0;
}

int Logic_Heroer::set_hero_rune_pro_val(const std::map<int, double> &pro_val_map) {
	heroer_detail_->hero_rune_pro_val.clear();
	heroer_detail_->hero_rune_pro_val = pro_val_map;
	return 0;
}

int Logic_Heroer::master_whole_prop_refresh(Hero_Info_Detail &hero_info_detail, const int master_whole_lvl,
		std::map<int, double> &pro_val_map) {
	if (! master_whole_lvl) {
		return 0;
	}
	UInt_UInt_Map::const_iterator lvl_whole_cfg = CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().find(master_whole_lvl);
	if (lvl_whole_cfg == CONFIG_CACHE_EQUIP->incr_lvl_whole_prop_map().end()
			|| lvl_whole_cfg->second <= 0) {
		return -1;
	}

	Item_Detail *equip = NULL;
	const Item_Detail_Config *item_config = NULL;
	int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + hero_info_detail.hero_index * ONE_HERO_EQUIP_GAP;
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

		// 精通加成-装备基础属性
		master_whole_base_prop_refresh(item_config, master_whole_lvl, pro_val_map);
		// 精通加成-装备解封属性
		master_whole_unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, master_whole_lvl, pro_val_map);
	}

	return 0;
}

bool Logic_Heroer::is_fini_recruit_task(const int32_t hero_id, const Hero_Config *hero_config) {
	if (! hero_config) {
		return false;
	}

	bool is_fini = true;
	for (size_t i = 0; i < hero_config->recruit_tasks.size(); ++i) {
		int task_id = hero_config->recruit_tasks[i];
		if (player_self()->task_state(task_id) < TaskInfo::TASK_STATUS_FINISH) {
			is_fini = false;
			break;
		}
	}

	return is_fini;
}

bool Logic_Heroer::is_fini_tasks(const int32_t hero_id, const Int_Vec &tasks) {
	if (!hero_id || tasks.empty()) {
		return false;
	}

	bool is_fini = true;
	for (size_t i = 0; i < tasks.size(); ++i) {
		int task_id = tasks[i];
		if (player_self()->task_state(task_id) < TaskInfo::TASK_STATUS_FINISH) {
			is_fini = false;
			break;
		}
	}

	return is_fini;
}

bool Logic_Heroer::is_awake_limit(const Hero_Info_Detail &hero_info_detail) {
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(hero_info_detail.hero_id);
	if (!hero_config || hero_config->hero_lvl_awake_limit.empty() ||
			(int)hero_config->hero_lvl_awake_limit.size() <= hero_info_detail.awake_lvl) {
		return true;
	}
	return hero_info_detail.level >= hero_config->hero_lvl_awake_limit[hero_info_detail.awake_lvl];

//	switch (hero_info_detail.awake_lvl) {
//	case 0:
//		return hero_info_detail.level >= 10;
//	case 1:
//		return hero_info_detail.level >= 20;
//	case 2:
//		return hero_info_detail.level >= 30;
//	case 3:
//		return hero_info_detail.level >= 40;
//	case 4:
//		return hero_info_detail.level >= 50;
//	default:
//		return true;
//	}
}

int Logic_Heroer::get_level_limit(const Hero_Info_Detail &hero_info_detail) {
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(hero_info_detail.hero_id);
	if (!hero_config || (int)hero_config->hero_lvl_awake_limit.size() <= hero_info_detail.awake_lvl) {
		return -1;
	}
	return std::min(hero_config->hero_lvl_awake_limit[hero_info_detail.awake_lvl],
			player_self()->level() + MAX_HERO_LVL_GTE_ROLE);
}

int Logic_Heroer::get_detail_type(Hero_Info_Detail &hero_info_detail) {
	if (hero_info_detail.hero_index == heroer_detail_->fighter_index) {
		if(heroer_detail_->outer_index != 0){
			return OUTER_FIGHT_HERO;
		}else{
			return FIGHT_HERO_ONLY;
		}
	}
	return 0;
}

int Logic_Heroer::fight_heroer_prop_refresh(bool sync_scene/* = true*/, bool sync_client/* = true*/){
	return heroer_prop_refresh(heroer_detail_->fighter_index, sync_scene, sync_client);
}

int Logic_Heroer::fight_heroer_skill_refresh(bool sync_scene/* = true*/, bool sync_client/* = true*/){
	return heroer_skill_refresh(heroer_detail_->fighter_index, sync_scene, sync_client);
}

int Logic_Heroer::heroer_prop_refresh(const int hero_index, bool sync_scene, bool sync_client) {
	if (! hero_index)
		return ERROR_PERM_LIMIT;

	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end())
		return ERROR_HERO_NOT_EXIST;

	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
	if (! hero_config)
		return ERROR_CONFIG_NOT_EXIST;

	for (int addition_type = AT_BASIC; addition_type < AT_END; ++addition_type) {
		if (addition_type == AT_CONVERSION || addition_type == AT_PASSIVE_SKILL) {
			continue;
		}
		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			it_hero->second.hero_fighter_detail.modify_fight_property((Property_Type)(*it_prop), (Addition_Type)addition_type, O_SET, 0.0, 0.0, 0.0);
			if (PT_BLOOD_MAX == *it_prop) {
				it_hero->second.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, (Addition_Type)addition_type, O_SET, 0.0, 0.0, 0.0);
			}
		}
	}
	it_hero->second.hero_fighter_detail.extra_skill_vec.clear();

	// 英雄等级属性
	level_prop_refresh(it_hero->second, hero_config);

	// 觉醒属性
	awake_prop_refresh(it_hero->second, hero_config);

	// 装备属性
	equip_prop_refresh(it_hero->second);

	// 符石属性
	rune_stone_prop_refresh(it_hero->second);

	// 符文属性
//	if (it_hero->first == heroer_detail_->fighter_index) {
//		hero_rune_prop_refresh(it_hero->second);
//	}

	if(hero_index == heroer_detail_->fighter_index){

		// 神器属性
		artifacts_prop_refresh(it_hero->second);

		// 龙谷属性
		dragon_vale_prop_refresh(it_hero->second);

		// 公会技能属性
		gang_skill_hero_prop_refresh(it_hero->second);

		// 刷新翅膀
		wing_prop_refresh(it_hero->second);

		// refresh pet attr & be skills
		pet_prop_refresh(it_hero->second);

		// 刷新出战英雄坐骑属性
		mount_prop_refresh(it_hero->second);

		// status(战斗精灵)属性加成
		status_prop_refresh(it_hero->second);

		// 上古神器
		anci_artifact_prop_refresh(it_hero->second);

		// 助阵英雄加属性
		refresh_outer_fight_hero_assist_prop(it_hero->second);
	}


	// ---------------------- 属性加成 END -------------------------
	int force_before = get_fight_hero_force();

	// 更新：更新英雄战力
	it_hero->second.hero_fighter_detail.update_force();

	// 刷新当前血量
	double blood_max = it_hero->second.hero_fighter_detail.fetch_fight_property(PT_BLOOD_MAX);
	it_hero->second.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, blood_max, 0.0, 0.0);

	if (sync_scene) {
		int detail_type = get_detail_type(it_hero->second);
		this->sync_hero_detail_to_scene(detail_type);
	}
	if (sync_client) {
		// 更新玩家总战力
		if (hero_index == heroer_detail_->fighter_index) {
			player_self()->update_ttl_force();
		}
		active_hero_info(hero_index);
	}

	int force_now = get_fight_hero_force();
	if (force_before > force_now) {
		MSG_DEBUG("hero force change role:%ld, %d   %d   %d", role_id(), player_self()->fighter_detail().force, force_before, force_now);
	}

	return 0;
}

int Logic_Heroer::heroer_prop_refresh(void) {
	// hero
	for (Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		heroer_prop_refresh(it_hero->second.hero_index);
	}
	// player
	player_self()->player_artifacts_prop_refresh();

	return 0;
}

int Logic_Heroer::level_prop_refresh(Hero_Info_Detail &hero_info_detail, const Hero_Config *hero_config) {
	std::map<int, double> pro_val_map;
	const Int_Double_Map *hero_lvl_prop = CONFIG_CACHE_HERO->find_hero_lvl_prop(hero_info_detail.level);
	for (Int_Double_Map::const_iterator it_lvl_pro = hero_lvl_prop->begin();
			it_lvl_pro != hero_lvl_prop->end(); ++it_lvl_pro) {
		int prop = it_lvl_pro->first;
		double value = it_lvl_pro->second;
		if (it_lvl_pro->first > 0 && it_lvl_pro->second > 0.0) {
			pro_val_map[prop] += value;
		}
	}

	// 属性修正（改为只修正英雄等级属性）
	correct_prop_refresh(hero_info_detail, pro_val_map, hero_config);

	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_BASIC, PST_BASIC, pro_val_map);

	return 0;
}

int Logic_Heroer::awake_prop_refresh(Hero_Info_Detail &hero_info_detail, const Hero_Config *hero_config) {
	if (! hero_config) {
		return -1;
	}
	Awake_Lvl_Cfg_Map::const_iterator it_awake_lvl_cfg = hero_config->awake_lvl_cfg_map.find(hero_info_detail.awake_lvl);
	if (it_awake_lvl_cfg == hero_config->awake_lvl_cfg_map.end()) {
		return 0;
	}
	if (! it_awake_lvl_cfg->second.prop_val_map.empty()) {
		std::map<int, double> pro_val_map;
		for (Int_Double_Map::const_iterator it_cfg_prop_map = it_awake_lvl_cfg->second.prop_val_map.begin();
				it_cfg_prop_map != it_awake_lvl_cfg->second.prop_val_map.end(); ++it_cfg_prop_map) {
			if (FIGHT_PROP_SET.count(it_cfg_prop_map->first)) {
				pro_val_map.insert(std::make_pair(it_cfg_prop_map->first, it_cfg_prop_map->second));
			}
		}
		hero_modify_fight_property(hero_info_detail, AT_AWAKE, PST_FIXED, pro_val_map);
	}

	return 0;
}

int Logic_Heroer::equip_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	std::map<int, double> pro_val_map;
	Int_Int_Map stone_color_map;
	Item_Detail *equip = NULL;
	hero_info_detail.suit_cnt_map.clear();
	int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + hero_info_detail.hero_index * ONE_HERO_EQUIP_GAP;
	int unban_lvl_whole = 50;
	int master_lvl_whole = 100;
	int hero_equip_suit_end = hero_pack_idx_gap + EQUIP_SUIT_END;

	for (int index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
		equip = this->pack_get_item(index);
		if (! equip) {
			unban_lvl_whole = index < hero_equip_suit_end ?  0 : unban_lvl_whole;
			master_lvl_whole = index < hero_equip_suit_end ?  0 : master_lvl_whole;
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
		if (index < hero_equip_suit_end) {
			unban_lvl_whole = equip->addon.equip.unban_lvl < unban_lvl_whole ?
					equip->addon.equip.unban_lvl : unban_lvl_whole;

			{ // 精通处理
				int index_old = PACK_T_HERO_EQUIP_INDEX + ONE_HERO_EQUIP_GAP + index % 100;
				Master_Part_Info_Map::const_iterator it_part = this->equiper_detail().master_part_info.find(index_old);
				if (it_part != this->equiper_detail().master_part_info.end()) {
					master_lvl_whole = it_part->second.level < master_lvl_whole ?
							it_part->second.level : master_lvl_whole;
				} else {
					master_lvl_whole = 0;
				}
			}
		}
		// 获得套装件数
		if (index < hero_equip_suit_end && item_config->equip_cfg.suit_id) {
			++hero_info_detail.suit_cnt_map[item_config->equip_cfg.suit_id];
		}

		// 装备基础属性
		base_prop_refresh(item_config, pro_val_map);
		// 装备解封属性
		unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, pro_val_map);

		// 精通加成-装备基础属性
		master_base_prop_refresh(item_config, equip->index_, pro_val_map);
		// 精通加成-装备解封属性
		master_unban_prop_refresh(item_config, equip->addon.equip.unban_lvl, equip->index_, pro_val_map);

		// 装备附魔属性
		enchant_prop_refresh(equip, pro_val_map);

		// 卡牌属性
		player_self()->card_prop_refresh(*equip, pro_val_map);

		// 宝石属性
		player_self()->gemstone_prop_refresh(*equip, pro_val_map);
		player_self()->calc_genstone(*equip, stone_color_map);
	}

	// 宝石套装属性
	player_self()->gemstone_suit_prop_refresh(stone_color_map, pro_val_map);

	// 全身解封等级属性
	unban_whole_prop_refresh(unban_lvl_whole, pro_val_map);
	// 精通-全身精通达到X级（X可配）将获得装备共鸣
	master_whole_prop_refresh(hero_info_detail, master_lvl_whole, pro_val_map);

	// 套装属性、技能
	suit_prop_refresh(hero_info_detail.suit_cnt_map, pro_val_map);

	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_EQUIP, PST_BASIC, pro_val_map);

	return 0;
}

int Logic_Heroer::rune_stone_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	std::map<int, double> pro_val_map;
	const Item_Detail *item = NULL;
	std::map<int, int> combine_cnt_map;
	int pack_type = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + hero_info_detail.hero_index * ONE_HERO_EQUIP_GAP;
	uint cap = pack_is_hero_type(pack_type) ?  pack_get_hero_capacity(pack_type) :  pack_get_capacity((Pack_Type)pack_type);
	int idx_start = pack_type + RS_INNER_IDX_BEGIN;
	int idx_end = pack_type + cap + 1;

	for (int idx = idx_start; idx < idx_end; ++idx) {
		item = this->pack_get_const_item(idx);
		if (! item) {
			continue;
		}
		const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item->id_);
		if (! item_cfg) {
			continue;
		}
		// 获得组合件数
		if (item_cfg->rune_stone.combine_id) {
			++combine_cnt_map[item_cfg->rune_stone.combine_id];
		}
		// exp_lvl prop
		player_self()->exp_lvl_prop_refresh(item_cfg, item->addon.rune_stone.exp_lvl, pro_val_map);
	}

	// 组合属性
	player_self()->combine_prop_refresh(combine_cnt_map, pro_val_map);

	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_RUNE_STONE, PST_FIXED, pro_val_map);

	return 0;
}

int Logic_Heroer::artifacts_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	std::map<int, double> pro_val_map;
	for (std::map<int, double>::iterator it_art_pro = heroer_detail_->artifacts_pro_val.begin();
			it_art_pro != heroer_detail_->artifacts_pro_val.end(); ++it_art_pro) {
		if (it_art_pro->first && it_art_pro->second > 0.0) {
			pro_val_map[it_art_pro->first] += it_art_pro->second;
		}
	}

	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_ARTIFACTS, PST_FIXED, pro_val_map);

	return 0;
}

int Logic_Heroer::status_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_STATUS, PST_FIXED, this->heroer_detail_->status_pro_val);
	hero_modify_fight_property(hero_info_detail, AT_STATUS, PST_PERCENT, this->heroer_detail_->status_pro_val_per);

	return 0;
}

int Logic_Heroer::anci_artifact_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	std::map<int, double> pro_val_map;
	player_self()->aa_refresh_prop_hero(pro_val_map);

	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_ANCI_ARTIFACT, PST_FIXED, pro_val_map);

	return 0;
}

int Logic_Heroer::wing_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	Prop_Setter_Vec vec;
	player_self()->get_wing_props_for_hero(vec);
	if (vec.empty()) return 0;

	for (Prop_Setter_Vec::iterator it = vec.begin(); it != vec.end(); ++it) {
		hero_info_detail.hero_fighter_detail.modify_fight_property(it->prop_type,
				it->addi_type, it->operate, it->basic, it->fixed, it->percent);
	}

	return 0;
}

int Logic_Heroer::pet_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	Prop_Setter_Vec vec;
	Int_Vec skill_vec;
	player_self()->refresh_pet_attr_for_hero(vec, skill_vec);
	if (vec.empty() && skill_vec.empty()) return 0;

	for (Prop_Setter_Vec::iterator it = vec.begin(); it != vec.end(); ++it) {

		hero_info_detail.hero_fighter_detail.modify_fight_property(it->prop_type,
				it->addi_type, it->operate, it->basic, it->fixed, it->percent);
	}

	Skill_DB_Info skill_info;
	for (Int_Vec::iterator it = skill_vec.begin(); it != skill_vec.end(); ++it) {

		skill_info.reset();
		skill_info.skill_id = *it;
		skill_info.skill_level = 1;
		hero_info_detail.hero_fighter_detail.extra_skill_vec.push_back(skill_info);
	}

	return 0;
}

int Logic_Heroer::gang_skill_hero_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	std::map<int, double> pro_val_map;
	const Ganger_Detail& ganger_detail = player_self()->ganger_detail();
	if(! ganger_detail.gang_id) {
		return ERROR_GANG_NOT_IN_GANG;
	}
	if (ganger_detail.skill_map.empty()) {
		return ERROR_NO_SUCH_SKILL;
	}

	std::map<int, double> gang_skill_pv_map;
	player_self()->get_ganger_skill_pro_val(gang_skill_pv_map);

	for (std::map<int, double>::const_iterator it_skill_pv_map = gang_skill_pv_map.begin();
			it_skill_pv_map != gang_skill_pv_map.end(); ++it_skill_pv_map) {
		if (it_skill_pv_map->first && it_skill_pv_map->second > 0.0) {
			pro_val_map[it_skill_pv_map->first] += it_skill_pv_map->second;
		}
	}

	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_GANG, PST_FIXED, pro_val_map);

	return 0;
}

int Logic_Heroer::hero_rune_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	/*
	 * 符文系统属性以部位有装备为前提
	 */
	std::map<int, double> pro_val_map;
	int hole_lvl_whole = 1000;
	int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + hero_info_detail.hero_index * ONE_HERO_EQUIP_GAP;
	int hero_equip_suit_end = hero_pack_idx_gap + EQUIP_SUIT_END;

	for (Rune_Part_Detail_Map::const_iterator it_part = player_self()->rune_detail().part_detail_map.begin();
			it_part != player_self()->rune_detail().part_detail_map.end(); ++it_part) {
		if (it_part->first < PACK_T_HERO_EQUIP_INDEX) {
			continue;
		}

		// check equip
		uint rune_index = it_part->first;
		uint item_index = rune_index - ONE_HERO_EQUIP_GAP + hero_info_detail.hero_index * ONE_HERO_EQUIP_GAP;
		if (player_self()->is_item_exist(item_index)) {
			player_self()->rune_prop_refresh(rune_index, item_index, pro_val_map);

			if ((int)rune_index < hero_equip_suit_end) {
				if (hole_lvl_whole) {
					int part_min_hole_lvl = player_self()->get_part_min_hole_lvl(it_part->second);
					hole_lvl_whole = part_min_hole_lvl < hole_lvl_whole ? part_min_hole_lvl : hole_lvl_whole;
				}
			}

		} else {
			if ((int)rune_index < hero_equip_suit_end) {
				hole_lvl_whole = 0;
			}
		}
	}

	// 全身符文属性
	if (hole_lvl_whole && hole_lvl_whole != 1000) {
		player_self()->hole_lvl_whole_prop_refresh(hole_lvl_whole, pro_val_map);
	}

	// 属性加成
//	hero_modify_fight_property(hero_info_detail, AT_RUNE, PST_FIXED, pro_val_map);

	set_hero_rune_pro_val(pro_val_map);

	return 0;
}

int Logic_Heroer::correct_prop_refresh(Hero_Info_Detail &hero_info_detail, std::map<int, double> &pro_val_map,
		const Hero_Config *hero_config) {
	if (! hero_config) {
		return -1;
	}

	if (! pro_val_map.empty()) {
		for (std::map<int, double>::iterator it_pro_val_map = pro_val_map.begin();
				it_pro_val_map != pro_val_map.end(); ++it_pro_val_map) {
			if (it_pro_val_map->first && it_pro_val_map->second > 0.0) {
				Int_Double_Map::const_iterator it_correct_val = hero_config->hero_prop_correct.find(it_pro_val_map->first);
				if (it_correct_val != hero_config->hero_prop_correct.end()) {
					//double test1 = it_pro_val_map->second;
					//double test2 = it_correct_val->second;
					it_pro_val_map->second *= it_correct_val->second;
				} else {
					it_pro_val_map->second = 0;
				}
			}
		}
	}

	return 0;
}

int Logic_Heroer::hero_modify_fight_property(Hero_Info_Detail &hero_info_detail, Addition_Type addi_type, Prop_Setter_Type pst, const std::map<int, double> &pro_val_map) {
	bool pst_basic = false, pst_fixed = false, pst_percent = false;
	switch (pst) {
	case PST_BASIC:
		pst_basic = true;
		break;
	case PST_FIXED:
		pst_fixed = true;
		break;
	case PST_PERCENT:
		pst_percent = true;
		break;
	default:
		return 0;
	}

	for (std::map<int, double>::const_iterator iter = pro_val_map.begin(); iter != pro_val_map.end(); ++iter) {
		if (iter->first) {
			double basic = pst_basic ? iter->second : 0.0;
			double fixed = pst_fixed ? iter->second : 0.0;
			double percent = pst_percent ? iter->second : 0.0;
			hero_info_detail.hero_fighter_detail.modify_fight_property((Property_Type)iter->first, addi_type, O_SET, basic, fixed, percent);
		}
	}

	return 0;
}

int Logic_Heroer::heroer_skill_refresh(const int hero_index, bool sync_scene, bool sync_client, bool init_be_skill) {
	if (! hero_index)
		return ERROR_PERM_LIMIT;

	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end())
		return ERROR_HERO_NOT_EXIST;

	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
	if (! hero_config)
		return ERROR_CONFIG_NOT_EXIST;


	// -----------Reset hero skills START --------------------------------------------
	it_hero->second.hero_fighter_detail.talent_map.clear();
	it_hero->second.hero_fighter_detail.cur_used_talent_ = 0;
	it_hero->second.hero_fighter_detail.default_skill = 0;

	// reset skill prop
	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		it_hero->second.hero_fighter_detail.modify_fight_property((Property_Type)(*it_prop), AT_PASSIVE_SKILL, O_SET, 0.0, 0.0, 0.0);
		if (PT_BLOOD_MAX == *it_prop) {
			it_hero->second.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, AT_PASSIVE_SKILL, O_SET, 0.0, 0.0, 0.0);
		}
	}

	// -----------Reset hero skills END ----------------------------------------------

	//int player_level = player_self()->level();
	int skill_loc = 6;
	int default_skill = 0;

	Talent talent;
	talent.talent_id = 1;
//	Talent_Page page;
//	page.reset();
//	page.page_id = 1;

	Skill_DB_Info skill_db_info;

	// 英雄等级技能
	for (uint16_t i_skill_id_vec = 0; i_skill_id_vec < hero_config->skill_id_vec.size(); ++i_skill_id_vec) {
		int skill_id = hero_config->skill_id_vec[i_skill_id_vec];
		const Skill_Config *skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
		if (! skill_config) {
			continue;
		}

		int skill_level = 0;
		for (Skill_Detail_Map::const_iterator it_skill_lvl_map = skill_config->skill_level_map.begin();
				it_skill_lvl_map != skill_config->skill_level_map.end(); ++it_skill_lvl_map) {
			if (it_skill_lvl_map->second.skill_level > skill_level &&
					it_hero->second.level >= it_skill_lvl_map->second.lv_lim) {
				skill_level = it_skill_lvl_map->second.skill_level;
			}
		}
		if (! skill_level) {
			continue;
		}

		default_skill = !default_skill ? skill_id : default_skill;

		skill_db_info.reset();
		skill_db_info.skill_id = skill_id;
		skill_db_info.skill_level = skill_level;

		if (skill_config->skill_type == TYPE_ADD_SKILL) {
			skill_db_info.skill_loc = 9;	// 追加技能放最后位置
		} else {
			skill_db_info.skill_loc = (skill_id == default_skill) ? 0 : skill_loc++;
		}

//		page.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
		talent.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
	}

	// 英雄觉醒技能
	uint16_t awake_start_index = 0;
	if(it_hero->second.awake_lvl > 4){
		awake_start_index = it_hero->second.awake_lvl - 4;
	}
	for (uint16_t i_skill_id_vec = 0;
			i_skill_id_vec < it_hero->second.awake_lvl && i_skill_id_vec < hero_config->awake_skill_id_vec.size();
			++i_skill_id_vec) {
		if(i_skill_id_vec < awake_start_index){
			continue;
		}
		int skill_id = hero_config->awake_skill_id_vec[i_skill_id_vec];
		const Skill_Config *skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
		if (! skill_config) {
			continue;
		}

		int skill_level = 0;
		for (Skill_Detail_Map::const_iterator it_skill_lvl_map = skill_config->skill_level_map.begin();
				it_skill_lvl_map != skill_config->skill_level_map.end(); ++it_skill_lvl_map) {
			if (it_skill_lvl_map->second.skill_level > skill_level &&
					it_hero->second.level >= it_skill_lvl_map->second.lv_lim) {
				skill_level = it_skill_lvl_map->second.skill_level;
			}
		}
		if (! skill_level) {
			continue;
		}

		default_skill = !default_skill ? skill_id : default_skill;

		skill_db_info.reset();
		skill_db_info.skill_id = skill_id;
		skill_db_info.skill_level = skill_level;
		skill_db_info.skill_loc = 0;

		talent.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
	}

//	it_hero->second.hero_fighter_detail.talent_page_map[page.page_id] = page;
//	it_hero->second.hero_fighter_detail.cur_used_talent_page = page.page_id;
	it_hero->second.hero_fighter_detail.talent_map[talent.talent_id] = talent;
	it_hero->second.hero_fighter_detail.cur_used_talent_ = talent.talent_id;
	it_hero->second.hero_fighter_detail.default_skill = default_skill;

	// add not show skills
	it_hero->second.hero_fighter_detail.extra_skill_vec.clear();
	hero_equip_skill_refresh(it_hero->second);
	// 刷新出战英雄坐骑上阵技能
	if(hero_index == heroer_detail_->fighter_index){
		mount_skill_refresh(it_hero->second);
	}

	if (init_be_skill) {	//竞技场，龙谷，不初始化被动技能属性
		it_hero->second.hero_fighter_detail.set_be_skill_attr();
	}

	// 更新：更新英雄战力
	it_hero->second.hero_fighter_detail.update_force();

	if (sync_scene) {
		int detail_type = get_detail_type(it_hero->second);
		this->sync_hero_detail_to_scene(detail_type);
	}

	if (sync_client) {
		active_hero_info(hero_index);
	}
	return 0;
}

int Logic_Heroer::hero_equip_skill_refresh(Hero_Info_Detail &hero_info_detail) {


	uint32_t hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + hero_info_detail.hero_index * ONE_HERO_EQUIP_GAP;
	int suit_idx_begin = hero_pack_idx_gap + EQUIP_SUIT_BEGIN;
	int suit_idx_end = hero_pack_idx_gap + EQUIP_SUIT_END;

	Int_Vec suit_skills = statistic_suit_skill(suit_idx_begin, suit_idx_end);
	Int_Vec equip_self_skills = get_equips_self_skills(suit_idx_begin, suit_idx_end);
	Int_Vec all_skills;
	all_skills.insert(all_skills.end(), suit_skills.begin(), suit_skills.end());
	all_skills.insert(all_skills.end(), equip_self_skills.begin(), equip_self_skills.end());

	Skill_DB_Info skill_db_info;
	for (Int_Vec::iterator it_skills = all_skills.begin(); it_skills != all_skills.end(); ++it_skills) {
		skill_db_info.reset();
		skill_db_info.skill_id = *it_skills;
		skill_db_info.skill_level = 1;
		skill_db_info.skill_loc = 0;
//		hero_info_detail.hero_fighter_detail.extra_skill_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
		hero_info_detail.hero_fighter_detail.extra_skill_vec.push_back(skill_db_info);
	}

	return 0;
}

int Logic_Heroer::test_awake_lvl_up(const int8_t is_hero, const int32_t hero_id) {
	if (! is_hero) {
		//player_self()->player_awake();
	}

	if (!hero_id) {
		return ERROR_CLIENT_OPERATE;
	}

	int hero_index = heroer_get_hero_index(hero_id);
	if (!hero_index) {
		return ERROR_CLIENT_OPERATE;
	}

	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(it_hero->second.hero_id);
	if (! hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (it_hero->second.awake_lvl >= hero_config->max_awake_lvl) {
		return ERROR_HERO_AWAKE_LVL_MAX;
	}

	++it_hero->second.awake_lvl;

	heroer_skill_refresh(it_hero->second.hero_index);

	heroer_detail_->detail_change();

	active_hero_info(hero_index);

	MSG_50102007 res_msg;
	res_msg.result = 1;
	res_msg.is_hero = is_hero;
	res_msg.hero_index = hero_index;
//	res_msg.new_awake_skill = new_awake_skill;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

void Logic_Heroer::init_artifacts(void) {
	for (Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		init_artifacts(it_hero->second);
	}
}

void Logic_Heroer::init_artifacts(Heroer_Detail &detail) {
	for (Heroer_Detail::Hero_Map::iterator it_hero = detail.hero_map.begin();
			it_hero != detail.hero_map.end(); ++it_hero) {
		init_artifacts(it_hero->second);
	}
}

void Logic_Heroer::init_artifacts(Hero_Info_Detail &hero_info_detail) {
	const Hero_Config* hero_config = CONFIG_CACHE_HERO->find_hero(hero_info_detail.hero_id);
	if (! hero_config) {
		return;
	}
	//if (hero_info_detail.artifacts.size() < hero_config->artifact_ids.size()) {
		Artifact_Detail artifact_detail;
		for (Int_Set::const_iterator it_artifact_ids = hero_config->artifact_ids.begin();
				it_artifact_ids != hero_config->artifact_ids.end(); ++it_artifact_ids) {
			if ((*it_artifact_ids) <= PHI_END) {
				continue;
			}
			if (! hero_info_detail.artifacts.count(*it_artifact_ids)) {
				artifact_detail.reset();
				artifact_detail.artifact_id = *it_artifact_ids;
				hero_info_detail.artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
			}
		}
	//}
}

void Logic_Heroer::refresh_artifacts_pro_val(Hero_Info_Detail &hero_info_detail) {
	for (Artifact_Map::iterator it_artifact = hero_info_detail.artifacts.begin();
			it_artifact != hero_info_detail.artifacts.end(); ++it_artifact) {
		if (it_artifact->second.artifact_lvl) {
			const Artifact_Cfg_Lvl* artifact_cfg_lvl = CONFIG_CACHE_HERO->find_artifact_lvl(it_artifact->second.artifact_lvl);
			if (! artifact_cfg_lvl->pro_val_map.empty()) {
				// 资质
				const Artifact_Cfg_Id* artifact_cfg_id = CONFIG_CACHE_HERO->find_artifact(it_artifact->first);
				if (!artifact_cfg_id || artifact_cfg_id->aptitude_map.empty()) {
					continue;
				}

				for (std::map<int, double>::const_iterator it_cfg_pro = artifact_cfg_lvl->pro_val_map.begin();
						it_cfg_pro != artifact_cfg_lvl->pro_val_map.end(); ++it_cfg_pro) {
					int pro = it_cfg_pro->first;
					double val = it_cfg_pro->second;
					std::map<int, double>::const_iterator find_it_aptitude = artifact_cfg_id->aptitude_map.find(pro);
					if (find_it_aptitude != artifact_cfg_id->aptitude_map.end()) {
						heroer_detail_->artifacts_pro_val[pro] += val * find_it_aptitude->second;
					}
				}
			}
		}
	}
}

void Logic_Heroer::refresh_artifacts_pro_val(void) {
	heroer_detail_->artifacts_pro_val.clear();

	// 玩家神器属性
	std::map<int, double> player_artifacts_pro_val;
	player_self()->get_player_artifacts_pro_val(player_artifacts_pro_val);
	heroer_detail_->artifacts_pro_val = player_artifacts_pro_val;

	// 英雄神器属性
	for (Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		refresh_artifacts_pro_val(it_hero->second);
	}
}

const std::map<int, double>& Logic_Heroer::artifacts_pro_val(void) {
	return heroer_detail_->artifacts_pro_val;
}

void Logic_Heroer::init_panal_tags(void) {
	if (! heroer_detail_->hero_map.empty()) {
		refresh_panal_tags();
	}
}

void Logic_Heroer::refresh_panal_tags(void) {
	heroer_detail_->panal_hero_tags.clear();
	Panal_Hero_Tag panal_hero_tag;
	for (Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.begin();
			it_hero != heroer_detail_->hero_map.end(); ++it_hero) {
		panal_hero_tag.reset();
		panal_hero_tag.is_fight = it_hero->second.hero_index == heroer_detail_->fighter_index;
		panal_hero_tag.is_last_fight = it_hero->second.hero_index == heroer_detail_->last_fighter_idx;
		panal_hero_tag.hero_level = it_hero->second.level;
		panal_hero_tag.hero_index = it_hero->second.hero_index;
		panal_hero_tag.hero_id = it_hero->second.hero_id;
		heroer_detail_->panal_hero_tags.push_back(panal_hero_tag);
	}
	std::sort(heroer_detail_->panal_hero_tags.begin(), heroer_detail_->panal_hero_tags.end());
	std::reverse(heroer_detail_->panal_hero_tags.begin(), heroer_detail_->panal_hero_tags.end());
	if (heroer_detail_->panal_hero_tags.size() > MAX_PANAL_HERO_TAG) {
		heroer_detail_->panal_hero_tags.resize(MAX_PANAL_HERO_TAG);
	}
}

int Logic_Heroer::test_prop(const int hero_index, const int prop_type, const double prop_val) {
	if (!hero_index || !prop_type || prop_val == 0.0) {
		return ERROR_CLIENT_PARAM;
	}
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(hero_index);
	if (it_hero == heroer_detail_->hero_map.end()) {
		return ERROR_HERO_NOT_EXIST;
	}
	if (! FIGHT_PROP_SET.count(prop_type)) {
		return ERROR_PERM_LIMIT;
	}

	Operate_Type operate_type = prop_val > 0.0  ? O_ADD : O_SUB;
	double abs_prop_val = abs(prop_val);

	it_hero->second.hero_fighter_detail.modify_fight_property((Property_Type)prop_type, AT_BASIC, operate_type, abs_prop_val, 0.0, 0.0);
	double blood_max = it_hero->second.hero_fighter_detail.fetch_fight_property(PT_BLOOD_MAX);
	it_hero->second.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, blood_max, 0.0, 0.0);

	bool sync_scene = true;
	bool sync_client = true;
	if (sync_scene) {
		int detail_type = get_detail_type(it_hero->second);
		this->sync_hero_detail_to_scene(detail_type);
	}
	if (sync_client) {
		active_hero_info(hero_index);
	}

	return 0;
}

int Logic_Heroer::dragon_vale_prop_refresh(Hero_Info_Detail &hero_info_detail) {
	std::map<int, double> pro_val_map;
	pro_val_map[PT_POWER] += player_self()->get_add_hero_power_from_dragon_vale();
	pro_val_map[PT_ARMOR] += player_self()->get_add_hero_armor_from_dragon_vale();
	pro_val_map[PT_STAMINA] += player_self()->get_add_hero_stamina_from_dragon_vale();
	pro_val_map[PT_CRIT] += player_self()->get_add_hero_crit_from_dragon_vale();
	pro_val_map[PT_CRIT_DEF] += player_self()->get_add_hero_prevent_from_dragon_vale();


	// 属性加成
	hero_modify_fight_property(hero_info_detail, AT_DRAGON_VALE, PST_FIXED,pro_val_map);

	return 0;
}

int Logic_Heroer::hero_fight_prop_modify(const std::vector<Prop_Setter> &setter_vec){
	if(heroer_detail_ == NULL){
		return -1;
	}
	int fight_hero_index = heroer_detail_->fighter_index;
	Heroer_Detail::Hero_Map::iterator it_hero = heroer_detail_->hero_map.find(fight_hero_index);
	if(it_hero != heroer_detail_->hero_map.end()) {
		int p_count = setter_vec.size();
		for(int i = 0; i < p_count; ++i){
			it_hero->second.hero_fighter_detail.modify_fight_property(setter_vec[i].prop_type, setter_vec[i].addi_type,
								setter_vec[i].operate, setter_vec[i].basic, setter_vec[i].fixed, setter_vec[i].percent);
		}
	}
	return 0;
}

void Logic_Heroer::get_hero_quality_num_map(std::map<int32_t, uint32_t> &hero_quality_num_map) {
	hero_quality_num_map.clear();
	for(Heroer_Detail::Hero_Map::const_iterator it = heroer_detail_->hero_map.begin();
			it != heroer_detail_->hero_map.end();
			++it) {
		std::map<int32_t, uint32_t>::iterator find_it = hero_quality_num_map.find(it->second.awake_lvl);
		if(find_it == hero_quality_num_map.end()) {
			hero_quality_num_map.insert(std::make_pair(it->second.awake_lvl, 1));
		}
		else {
			find_it->second += 1;
		}
	}
}

int Logic_Heroer::mount_skill_refresh(Hero_Info_Detail &hero_info_detail){
	Int_Vec all_skills;
	player_self()->get_mount_skill(all_skills);
	Skill_DB_Info skill_db_info;
	for (Int_Vec::iterator it_skills = all_skills.begin(); it_skills != all_skills.end(); ++it_skills) {
		skill_db_info.skill_id = *it_skills;
		skill_db_info.skill_level = 1;
//		hero_info_detail.hero_fighter_detail.extra_skill_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
		hero_info_detail.hero_fighter_detail.extra_skill_vec.push_back(skill_db_info);
	}
	return 0;
}

int Logic_Heroer::mount_prop_refresh(Hero_Info_Detail &hero_info_detail){
	Prop_Setter_Vec vec;
	player_self()->get_mount_props_for_hero(vec);
	for (Prop_Setter_Vec::iterator it = vec.begin(); it != vec.end(); ++it) {
		hero_info_detail.hero_fighter_detail.modify_fight_property(it->prop_type,
				it->addi_type, it->operate, it->basic, it->fixed, it->percent);
	}
	return 0;
}

int Logic_Heroer::req_inner_add_and_fight(const int hero_id) {
	int add_result = this->hero_add(hero_id);
	if(add_result != 0){
		return -1;
	}
	int hero_index = heroer_get_hero_index(hero_id);
	if (! hero_index) {
		return -1;
	}
	req_hero_set_fight(hero_index, 1, false, true);
	active_hero_info(hero_index);
	return 0;
}

int Logic_Heroer::req_inner_erase_recycle(const int hero_id) {
	int hero_index = heroer_get_hero_index(hero_id);
	if (! hero_index) {
		return -1;
	}
	hero_erase(hero_index);
	return 0;
}

int Logic_Heroer::get_hero_out_index(void){
	if(heroer_detail_){
		if(heroer_detail_->outer_index != 0){
			if(heroer_detail_->fighter_index != 0){
				return heroer_detail_->fighter_index;
			}else{
				return heroer_detail_->last_fighter_idx;
			}

		}
	}
	return 0;
}

int Logic_Heroer::get_hero_fight_index(void){
	if(heroer_detail_){
		return heroer_detail_->fighter_index;
	}
	return 0;
}

void Logic_Heroer::set_hero_out_state(int state){
	if(heroer_detail_){
		heroer_detail_->outer_index = state;
		heroer_detail_->detail_change();
	}
}
