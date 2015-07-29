/*
 * Single_Dungeon.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: root
 */

#include "Logic_Module_Head.h"
#include "Err_Code.h"
#include "Single_Dungeon.h"
#include "Single_Dungeon_Struct.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_NPC.h"
#include "Config_Cache_Role.h"
#include "Msg_Dungeon_Struct.h"
#include "Logic_Player.h"
#include "pack/Packer.h"
#include "pack/Pack_Struct.h"
#include "team/Team.h"
#include "Msg_Active_Struct.h"
#include "Config_Cache_Dungeon.h"
#include "Logic_Player.h"
#include "Msg_Inner_Struct.h"
#include "vip/Vip_Def.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Mail.h"
#include "mail/Mail.h"
#include "mail/Mail_Struct.h"
#include "covenant/Covenant.h"
#include "covenant/Covenant_Def.h"
#include "helper/Helper_Def.h"
#include "campfire/Campfire_Manager.h"
#include "Config_Cache.h"
#include "ranking/Ranking_Manager.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "pack/Pack_Func.h"

const int FIRST_SINGLE_DUNGEON_ID = 80201001;
const int FIRST_NOR_ELITE_DUNGEON_ID = 80202001;
const int FIRST_HOR_ELITE_DUNGEON_ID = 80202101;
const int FIRST_MAIN_SCENE_ID = 80100001;
const int ITEM_ID_TOWER_SOAL = 20515401;				// 塔魂道具ID

const int ITEM_ID_ELITE_COIN = 21001401;							// 上古硬币
const int ITEM_ID_LORD_COIN = 21001402;							// 魔神硬币

const int WIPE_COST_STR = 10;									// 扫荡扣取体力

Single_Dungeon::Single_Dungeon(){
	reset();
}

Single_Dungeon::~Single_Dungeon() {
}

int Single_Dungeon::create_init(Sgl_Dungeon_Detail &detail) {
	// 单人副本第一个激活副本
	int first_single = CONFIG_CACHE_ROLE->get_single_first_id();
	int first_nor_elite = CONFIG_CACHE_ROLE->get_elite_first_id(1);
	int first_hor_elite = CONFIG_CACHE_ROLE->get_elite_first_id(2);
	a_new_single_dungeon(detail, first_single);
	a_new_elite_dungeon(detail, first_nor_elite, 1);
	a_new_elite_dungeon(detail, first_hor_elite, 2);

	// 单人副本章节奖励
	std::vector<int> chapter_vec;
	int chapter_count = CONFIG_CACHE_DUNGEON->get_single_dungeon_chapter_count(
			chapter_vec);
	for (int i = 0; i < chapter_count; ++i) {
		Int_Int chapter_award;
		chapter_award.val_1 = chapter_vec[i];
		detail.chapter_award_vec.push_back(chapter_award);
	}
	detail.elite_reset_times = 1;
	detail.detail_change();
	return 0;
}

int Single_Dungeon::a_new_single_dungeon(Sgl_Dungeon_Detail &detail,
		int dungeon_id) {
	if (dungeon_id == 0 || dungeon_id == -1) {
		return -1;
	}
	Sgl_Dungeon_Data data;
	data.dungeon_id_ = dungeon_id;
	data.daily_counter_ = 0;
	data.progress_ = Sgl_Dungeon_Data::NOT_COMPLETED;
	data.ttl_counter_ = 0;
	detail.sgl_dungeon_map_[data.dungeon_id_] = data;
	detail.detail_change();
	return 0;
}

int Single_Dungeon::a_new_elite_dungeon(Sgl_Dungeon_Detail &detail,
		int dungeon_id, int difficulty) {
	if (dungeon_id == 0 || dungeon_id == -1) {
		return -1;
	}
	Sgl_Dungeon_Data data;
	data.dungeon_id_ = dungeon_id;
	data.daily_counter_ = 0;
	bool b_sgl_complete = false;
	if (difficulty == 1) {
		int single_id = 0;
		const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(
				dungeon_id);
		if (cfg) {
			single_id = cfg->dungeon.elite;
		}
		if(single_id == 0){
			b_sgl_complete = true;
		}else{
			Sgl_Dungeon_Map::iterator single_it = detail.sgl_dungeon_map_.find(single_id);
			if(single_it != detail.sgl_dungeon_map_.end()){
				if(single_it->second.progress_ == Sgl_Dungeon_Data::COMPLETED){
					b_sgl_complete = true;
				}
			}
		}
		if (b_sgl_complete) {
			data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
		} else {
			data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
		}
		detail.nor_elite_map_[data.dungeon_id_] = data;
	} else {
		bool b_sgl_complete = false;
		if(dungeon_id == CONFIG_CACHE_ROLE->get_elite_first_id(difficulty)){// 魔神殿首个副本激活，查看相对应的上古副本是否通关
			int lord_open_id = CONFIG_CACHE_DUNGEON->get_lord_open_id();
			Sgl_Dungeon_Map::iterator elite_it = detail.nor_elite_map_.find(lord_open_id);
			if(elite_it != detail.nor_elite_map_.end()){
				if(elite_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
						elite_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){
					b_sgl_complete = true;
				}
			}
		}else{
			b_sgl_complete = true;
		}
		if (b_sgl_complete) {
			data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
		} else {
			data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
		}
		detail.hor_elite_map_[data.dungeon_id_] = data;
	}
	detail.detail_change();
	return 0;
}

void Single_Dungeon::load_detail(Sgl_Dungeon_Detail &detail) {
	sgl_dungeon_detail_ = &detail;
}

void Single_Dungeon::save_detail(Sgl_Dungeon_Detail &detail) {
	if (sgl_dungeon_detail().is_change) {
		detail = sgl_dungeon_detail();
		this->sgl_dungeon_detail_->is_change = false;
	}
}

//initial module when role login
void Single_Dungeon::module_init(void) {
	check_elite_wipe_open();
	init_wipe_out();
}

void Single_Dungeon::sign_in(void) {
}

void Single_Dungeon::sign_out(void) {
	// 没有翻牌的
	if(is_opening_card_ == 1){
		is_opening_card_ = 0;
		const Wipe_Award_Config* award_cfg =
				CONFIG_CACHE_DUNGEON->find_wipe_award_config(opening_card_scene_id_);
		if(award_cfg){
			int8_t player_career = this->career();
			int card_id = 0;
			int card_amount = 0;
			award_cfg->get_card_reward(player_career, card_id, card_amount);
			int base_exp = 0;
			Money_Add_List money_add_list;
			Item_Vec pack_item_vec;
			MSG_81000102 acv_msg;
			acv_msg.type = 0;

			if (card_id == PT_EXP_CURRENT) {
				base_exp += card_amount;
			} else if (card_id == COPPER || card_id == GOLD
							|| card_id == BIND_GOLD || card_id == SOULS
							|| card_id == DRAGON_SOUL || card_id == FRIENDSHIP) {
				Money_Type my_type = (Money_Type) card_id;
				int my_num = card_amount;
				money_add_list.push_back(
						Money_Add_Info(my_type, my_num,
								Money_DM_Info(MONEY_ADD_FB_AWARD)));
				acv_msg.property.push_back(Property(my_type, my_num));
			} else {
				Item_Vec acv_item_vec;
				make_item_vec(acv_item_vec, card_id, card_amount,
						Item_Detail::BIND);
				for (Item_Vec::iterator it = acv_item_vec.begin();
						it != acv_item_vec.end(); ++it) {
					Item_Basic_Info item_base;
					(*it).set_item_basic_info(item_base);
					acv_msg.item_info.push_back(item_base);
					Card_Info card_info;
					if ((*it).set_item_card_info(card_info)) {
						acv_msg.card_info_vec.push_back(card_info);
					}
					pack_item_vec.push_back(*it);
				}
			}

			if(base_exp > 0){
				Exp_All exp_all = player_self()->modify_experience_inter(base_exp,
						true, true, false, true, 1, opening_card_scene_id_);
				if (0 == exp_all.result) {
					acv_msg.property.push_back(
							Property(PT_EXP_CURRENT, exp_all.all));
					acv_msg.vip_exp = exp_all.vip_exp;
					acv_msg.world_level = player_self()->get_world_level_add();
				}
			}
			if (!money_add_list.empty()) {
				this->player_self()->pack_add_money(money_add_list, MONEY_OPT_REAL);
			}
			if (!pack_item_vec.empty()) {
				this->player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
						pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_FB));
			}

			if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}
	}
	// 没有领取上古首通
	if(is_sg_first_pass_){
		req_receive_elite_first_pass(false);
	}
}

int Single_Dungeon::move_scene_completed(void) {
	int scene_id = player_self()->scene_id();
	const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (cfg) {
		if (cfg) {
			if (cfg->type == Elite_Scene_Type) {
				int diff = cfg->dungeon.difficulty;
				send_elite_trail_info_to_client(diff);
			}
		}
	}
	return 0;
}

void Single_Dungeon::daily_refresh(void) {
	// 上古副本
	for (Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->nor_elite_map_.begin();
			it != sgl_dungeon_detail_->nor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
			it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
		}
	}
	// 魔神殿
	for (Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->hor_elite_map_.begin();
			it != sgl_dungeon_detail_->hor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
			it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
		}
	}

	//
	if (player_self()) {
		int vip_lv = player_self()->vip();
		int lv_limit = CONFIG_CACHE_DUNGEON->get_elite_reset_vip_lv();
		if (vip_lv >= lv_limit) {
			add_elite_reset_times();
		}
	}
//	//
//	for (it = sgl_dungeon_detail_->nor_elite_map_.begin();
//			it != sgl_dungeon_detail_->nor_elite_map_.end(); ++it) {
//		it->second.add_gold_num = 0;
//		it->second.add_prop_num = 0;
//		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
//			it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
//		}
//	}
//	sgl_dungeon_detail_->elite_ch_num = 0;
//	sgl_dungeon_detail_->elite_add_buy_num = 0;
//	sgl_dungeon_detail_->elite_task_add_num = 0;
//	sgl_dungeon_detail_->elite_add_num = 0;
	// 副本追踪
	sgl_dungeon_detail_->elite_trail_info.reset();
	sgl_dungeon_detail_->lord_trail_info.reset();

	sgl_dungeon_detail_->wipe_elite_double = false;
	sgl_dungeon_detail_->wipe_elite_double_not_selected = false;

	sgl_dungeon_detail_->detail_change();
}

int Single_Dungeon::trigger_daily_zero(const Time_Value &now,
		bool logic_check/* = false*/) {
	return 0;
}

int Single_Dungeon::trigger_daily_six(const Time_Value &now,
		bool logic_check/* = false*/) {
	daily_refresh();
	return 0;
}

void Single_Dungeon::reset(void) {
	sgl_dungeon_detail_ = 0;
	offline_wipe_num_ = 0;
	is_opening_card_ = 0;
	opening_card_scene_id_ = 0;
	is_sg_first_pass_ = false;
	sg_first_pass_scene_id_ = 0;
}

int Single_Dungeon::client_ready_now(void) {
	if (sgl_dungeon_detail_ == NULL) {
		return -1;
	}
	if (sgl_dungeon_detail_->wipe_type != 0) {
		Time_Value now = Time_Value::gettimeofday();
		int remain_time = sgl_dungeon_detail_->wipe_finish_time.sec()
				- now.sec();
		if (sgl_dungeon_detail_->wipe_status != 0 || remain_time < 0) {
			remain_time = 0;
		}
		int complete_num = sgl_dungeon_detail_->wipe_infos.size();
		int remain_num = sgl_dungeon_detail_->wipe_num - complete_num;
		if (remain_num < 0) {
			remain_num = 0;
		}
		//
		{
			MSG_81000409 msg;
			msg.wipe_type = sgl_dungeon_detail_->wipe_type;
			int wc_count = sgl_dungeon_detail_->wipe_infos.size();
			for (int i = 0; i < wc_count; ++i) {
				msg.wipe_scenes.push_back(
						sgl_dungeon_detail_->wipe_infos[i].scene_id);
			}
			int wn_count = sgl_dungeon_detail_->wipe_scenes.size();
			for (int i = 0; i < wn_count; ++i) {
				msg.wipe_scenes.push_back(sgl_dungeon_detail_->wipe_scenes[i]);
			}
			THIS_SEND_TO_CLIENT(msg);
		}
		{
			MSG_50100210 msg;
			msg.wipe_type = sgl_dungeon_detail_->wipe_type;
			msg.remain_num = remain_num;
			msg.remain_time = remain_time;
			msg.wipe_info = sgl_dungeon_detail_->wipe_infos;
			THIS_SEND_TO_CLIENT(msg);
		}

		if (sgl_dungeon_detail_->wipe_status != 0) {
			sgl_dungeon_detail_->reset_wipe_info();
		}
	}
	return 0;
}

int Single_Dungeon::check_enter_dungeon(int dungeon_id, int role_lv) {
//	const Dungeon_Config config = CONFIG_CACHE->dungeon_config();
//	typedef Dungeon_Config::DungeonConfigMap DgnCfgMap;
//	for (DgnCfgMap::const_iterator it = config.dungeon_config_map_.begin();
//			it != config.dungeon_config_map_.end(); ++it) {
//		if (it->first == dungeon_id && (role_lv > it->second.lv_)) {
//			return 0;
//		} else if (it->first == dungeon_id) {
//			return ERROR_LV_LIMIT;
//		}
//	}
//	return ERROR_DUNGEON_NOT_EXIST;
	return 0;
}

int Single_Dungeon::check_exit_dungeon(const Location& loc) {
	return 0;
}

void Single_Dungeon::reset_clear_monster_counter(int dungeon_id) {
	this->sgl_dungeon_detail_->reset_clear_monster_counter(dungeon_id);
}

void Single_Dungeon::clear_single_dungeon(const Scene_Config* scene_config,
		bool is_no_profit/* = false*/) {
	int dungeon_id = scene_config->scene_id;
	Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->sgl_dungeon_map_.find(
			dungeon_id);
	if (it == sgl_dungeon_detail_->sgl_dungeon_map_.end()) {
		MSG_USER("cannot locate dungeon info, dungeon_id=%d, role_id=%ld",
				dungeon_id, this->role_id());
		return;
	}
	settle_reward(it->second, dungeon_id, is_no_profit);
}

int Single_Dungeon::settle_reward(Sgl_Dungeon_Data &dungeon_data, int layer_id,
		bool is_no_profit/* = false*/) {
	if (player_self() == NULL) {
		return -1;
	}
	int dungeon_id = layer_id;
	const Scene_Config* scene_config = CONFIG_CACHE_SCENE->scene_config_cache(
			dungeon_id);
	if (scene_config == NULL) {
		MSG_USER("cannot find dungeon info, id=~d", dungeon_id);
		return -1;
	}
	const Wipe_Award_Config* award_cfg =
			CONFIG_CACHE_DUNGEON->find_wipe_award_config(dungeon_id);
	if (award_cfg == NULL) {
		MSG_USER("cannot find dungeon award, id=~d", dungeon_id);
		return -1;
	}

	dungeon_data.clear_monster_count_ += 1;
	int8_t player_career = player_self()->career();

	int total_exp = 0;
	Item_Vec pack_item_vec;
	Money_Add_List money_add_list;
	Reward_Info reward;
	MSG_50100203 res_msg;
	res_msg.scene_id = dungeon_id;
	int base_exp = 0;
	int first_exp = 0;
	MSG_81000102 first_acv_msg;
	first_acv_msg.type = 0;
	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	// first reward
	if (dungeon_data.clear_monster_count_ == 1) {
		//first_goods
		int goods_count = award_cfg->first_reward(player_career).size();
		for (int i = 0; i < goods_count; ++i) {
			reward.reward_id = award_cfg->first_reward(player_career)[i].val_1;
			reward.quantity = award_cfg->first_reward(player_career)[i].val_2;
			if (reward.reward_id == PT_EXP_CURRENT) {				//首通经验不做任何加成
				first_exp += reward.quantity;
				//res_msg.first_reward_list.push_back(reward);
			} else if (reward.reward_id == COPPER || reward.reward_id == GOLD
					|| reward.reward_id == BIND_GOLD
					|| reward.reward_id == SOULS
					|| reward.reward_id == DRAGON_SOUL
					|| reward.reward_id == FRIENDSHIP) {
				res_msg.first_reward_list.push_back(reward);
				Money_Type my_type = (Money_Type) reward.reward_id;
				int my_num = reward.quantity;
				money_add_list.push_back(
						Money_Add_Info(my_type, my_num,
								Money_DM_Info(MONEY_ADD_FB_AWARD)));
				first_acv_msg.property.push_back(Property(my_type, my_num));
			} else {
				res_msg.first_reward_list.push_back(reward);
//				Item_Detail item(reward.reward_id, reward.quantity,Item_Detail::BIND);
//				pack_item_vec.push_back(item);
				make_item_vec(pack_item_vec, reward.reward_id, reward.quantity,
						Item_Detail::BIND);
				for (Item_Vec::const_iterator it = pack_item_vec.begin();
						it != pack_item_vec.end(); ++it) {
					Item_Basic_Info item_base;
//					item.set_item_basic_info(item_base);
					(*it).set_item_basic_info(item_base);
					first_acv_msg.item_info.push_back(item_base);
					Card_Info card_info;
					if ((*it).set_item_card_info(card_info)) {
						first_acv_msg.card_info_vec.push_back(card_info);
					}
				}
			}
		}
	}
	//copy reward
	if (is_no_profit == false) {
		// 可以翻牌
		is_opening_card_ = 1;
		opening_card_scene_id_ = dungeon_id;

		int goods_count = award_cfg->copy_reward(player_career).size();
		for (int i = 0; i < goods_count; ++i) {
			reward.reward_id = award_cfg->copy_reward(player_career)[i].val_1;
			reward.quantity = award_cfg->copy_reward(player_career)[i].val_2;
			if (reward.reward_id == PT_EXP_CURRENT) {
				base_exp += reward.quantity;
			} else if (reward.reward_id == COPPER || reward.reward_id == GOLD
					|| reward.reward_id == BIND_GOLD
					|| reward.reward_id == SOULS
					|| reward.reward_id == DRAGON_SOUL
					|| reward.reward_id == FRIENDSHIP) {
				res_msg.reward_list.push_back(reward);
				Money_Type my_type = (Money_Type) reward.reward_id;
				int my_num = reward.quantity;
				money_add_list.push_back(
						Money_Add_Info(my_type, my_num,
								Money_DM_Info(MONEY_ADD_FB_AWARD)));
				acv_msg.property.push_back(Property(my_type, my_num));
			} else {
				res_msg.reward_list.push_back(reward);
//				Item_Detail item(reward.reward_id, reward.quantity,Item_Detail::BIND);
//				pack_item_vec.push_back(item);
				Item_Vec acv_item_vec;
				make_item_vec(acv_item_vec, reward.reward_id, reward.quantity,
						Item_Detail::BIND);
				for (Item_Vec::iterator it = acv_item_vec.begin();
						it != acv_item_vec.end(); ++it) {
					Item_Basic_Info item_base;
					(*it).set_item_basic_info(item_base);
					acv_msg.item_info.push_back(item_base);
					Card_Info card_info;
					if ((*it).set_item_card_info(card_info)) {
						acv_msg.card_info_vec.push_back(card_info);
					}
					pack_item_vec.push_back(*it);
				}
			}
		}
		{
			Exp_All exp_all = player_self()->modify_experience_inter(base_exp,
					true, true, false, true, 1, dungeon_id);
			if (0 == exp_all.result) {
				total_exp += exp_all.all;
				//  1 通关经验
				reward.reward_id = 1;
				reward.quantity = base_exp;
				reward.vip_add = 0;
				reward.world_level = player_self()->get_world_level_add();
				res_msg.reward_list.push_back(reward);
				// 2 vip加成
				reward.reward_id = 2;
				reward.quantity = exp_all.vip_exp;
				reward.vip_add = 0;
				reward.world_level = player_self()->get_world_level_add();
				res_msg.reward_list.push_back(reward);
				//  3 道具加成
				reward.reward_id = 3;
				reward.quantity = exp_all.buff_exp;
				reward.vip_add = 0;
				reward.world_level = player_self()->get_world_level_add();
				res_msg.reward_list.push_back(reward);

				acv_msg.property.push_back(
						Property(PT_EXP_CURRENT, exp_all.all));
				acv_msg.vip_exp = exp_all.vip_exp;
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}
	} else {
		is_opening_card_ = 2;
		opening_card_scene_id_ = dungeon_id;
		reward.reward_id = PT_EXP_CURRENT;
		reward.quantity = 0;
		res_msg.reward_list.push_back(reward);
		reward.reward_id = COPPER;
		reward.quantity = 0;
		res_msg.reward_list.push_back(reward);
		reward.reward_id = SOULS;
		reward.quantity = 0;
		res_msg.reward_list.push_back(reward);
	}

	// to packege
	if (first_exp > 0) {
		Exp_All exp_all = player_self()->modify_experience_inter(first_exp,
				false, false, false);
		if (0 == exp_all.result) {
			total_exp += exp_all.all;
			first_acv_msg.property.push_back(
					Property(PT_EXP_CURRENT, exp_all.all));
			first_acv_msg.vip_exp = exp_all.vip_exp;
			first_acv_msg.world_level = player_self()->get_world_level_add();
			// 4 首通经验
			reward.reward_id = 4;
			reward.quantity = exp_all.all;
			reward.vip_add = 0;
			reward.world_level = player_self()->get_world_level_add();
			res_msg.reward_list.push_back(reward);
			// 首通获得界面显示
			Reward_Info reward_f_exp;
			reward_f_exp.reward_id = PT_EXP_CURRENT;
			reward_f_exp.quantity = exp_all.all;
			reward_f_exp.vip_add = 0;
			reward_f_exp.world_level = player_self()->get_world_level_add();
			res_msg.first_reward_list.push_back(reward_f_exp);
		}
	}
	// 总经验
	reward.reward_id = PT_EXP_CURRENT;
	reward.quantity = total_exp;
	reward.vip_add = 0;
	reward.world_level = player_self()->get_world_level_add();
	res_msg.reward_list.push_back(reward);

	if (CONFIG_CACHE->server_maintainer_cache().game_ver_type == 1) {// 国外版本特殊处理
		if (dungeon_data.dungeon_id_ == CONFIG_CACHE_ROLE->get_single_first_id() &&
				dungeon_data.clear_monster_count_ == 1) {
			res_msg.first_reward_list.clear();
			res_msg.reward_list.clear();
		}
	}

	THIS_SEND_TO_CLIENT(res_msg);

	if (!money_add_list.empty()) {
		this->player_self()->pack_add_money(money_add_list, MONEY_OPT_REAL);
	}
	if (!pack_item_vec.empty()) {
		this->player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
				pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_FB));
	}

	if (!first_acv_msg.item_info.empty() || !first_acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(first_acv_msg);
	}
	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	//新通关的，开启下个副本 lys
	if (dungeon_data.progress_ == Sgl_Dungeon_Data::NOT_COMPLETED) {
		dungeon_data.progress_ = Sgl_Dungeon_Data::COMPLETED;
		sgl_dungeon_detail_->single_complete_num =
				this->get_single_complete_num();
		if (scene_config->dungeon.next_id != 0) {
			a_new_single_dungeon(*this->sgl_dungeon_detail_,
					scene_config->dungeon.next_id);
		}
		//
		hook_single_dungeon_first_complete(dungeon_data.dungeon_id_);
	}
	if (sgl_dungeon_detail_) {
		sgl_dungeon_detail_->detail_change();
	}
	req_fetch_single_dungeon_info();
	return 0;
}

int Single_Dungeon::req_fetch_single_dungeon_info(void) {
	MSG_50100200 response;
	Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_single_dungeon_id();
	for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
		Sgl_Dungeon_Data* data = this->get_single_dungeon_data(*it);
		if (!data) {
			break;
		}
		Single_Dungeon_Info info;
		info.dungeon_id = data->dungeon_id_;
		info.is_passed = data->progress_;
		info.daily_counter = data->ttl_counter_;
		response.sgl_dungeon_info_array.push_back(info);
		if (data->progress_ == Sgl_Dungeon_Data::NOT_COMPLETED) {
			break;
		}
	}
	// 如果没有重新激活
	if (response.sgl_dungeon_info_array.empty()) {
		int scene_id = CONFIG_CACHE_ROLE->get_single_first_id();
		a_new_single_dungeon(*sgl_dungeon_detail_, scene_id);
		for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
			Sgl_Dungeon_Data* data = this->get_single_dungeon_data(*it);
			if (!data) {
				break;
			}
			Single_Dungeon_Info info;
			info.dungeon_id = data->dungeon_id_;
			info.is_passed = data->progress_;
			info.daily_counter = data->ttl_counter_;
			response.sgl_dungeon_info_array.push_back(info);
			if (data->progress_ == Sgl_Dungeon_Data::NOT_COMPLETED) {
				break;
			}
		}
	}

	int count = sgl_dungeon_detail_->chapter_award_vec.size();
	for (int i = 0; i < count; ++i) {
		int status = sgl_dungeon_detail_->chapter_award_vec[i].val_2;
		response.chapter_award_vec.push_back(status);
	}
	THIS_SEND_TO_CLIENT(response);
	return 0;
}

int Single_Dungeon::req_fetch_elite_dungeon_info(const int8_t type) {
	MSG_50100202 msg;
	msg.type = type;
	int vip_lv = player_self()->vip();
	int vip_lv_limit = CONFIG_CACHE_DUNGEON->get_elite_reset_vip_lv();
	if (vip_lv >= vip_lv_limit) {
		msg.reset_times = this->sgl_dungeon_detail_->elite_reset_times;
	}
	int last_id = 0;
	int last_status = 0;
	Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(type);
	for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
		int scene_id = *it;
		Sgl_Dungeon_Data* data = this->get_elite_dungeon_data(scene_id, type);
		if (!data) {
			break;
		}
		Elite_Dungeon_Info info;
		info.id = data->dungeon_id_;
		info.status = data->progress_;
//		if(data->add_gold_num > 0 || data->add_prop_num > 0){
//			info.add_num = 1;
//		}
		FB_Info_Detail* fb_detail = CAMPFIRE_MANAGER->get_fb_info_detail(
				scene_id);
		if (fb_detail) {
			info.best_force = fb_detail->best_force;
			info.best_role_id = fb_detail->best_role_id;
			info.best_role_name = fb_detail->best_role_name;
		}
		msg.elite_nor_vec.push_back(info);
		last_id = data->dungeon_id_;
		last_status = data->progress_;
	}
	// 没有副本，添加第一个
	if(msg.elite_nor_vec.empty()){
		int first_scene_id = CONFIG_CACHE_ROLE->get_elite_first_id(type);
		a_new_elite_dungeon(*sgl_dungeon_detail_, first_scene_id, type);
		for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
			int scene_id = *it;
			Sgl_Dungeon_Data* data = this->get_elite_dungeon_data(scene_id, type);
			if (!data) {
				break;
			}
			Elite_Dungeon_Info info;
			info.id = data->dungeon_id_;
			info.status = data->progress_;
	//		if(data->add_gold_num > 0 || data->add_prop_num > 0){
	//			info.add_num = 1;
	//		}
			FB_Info_Detail* fb_detail = CAMPFIRE_MANAGER->get_fb_info_detail(
					scene_id);
			if (fb_detail) {
				info.best_force = fb_detail->best_force;
				info.best_role_id = fb_detail->best_role_id;
				info.best_role_name = fb_detail->best_role_name;
			}
			msg.elite_nor_vec.push_back(info);
			last_id = data->dungeon_id_;
			last_status = data->progress_;
		}
	}
	// 如果最后一个是激活状态，在添加一个
	if (last_id != 0 && last_status == Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE) {
		const Scene_Config* cfg_cur = CONFIG_CACHE_SCENE->scene_config_cache(
				last_id);
		if (cfg_cur) {
			int next_id = cfg_cur->dungeon.next_id;
			const Scene_Config* cfg_next =
					CONFIG_CACHE_SCENE->scene_config_cache(next_id);
			if (cfg_next) {
				Elite_Dungeon_Info info;
				info.id = next_id;
				// 检测相应的单人副本通关状态，打通就是未激活，没有打通就是未解锁状态
				int single_id = cfg_next->dungeon.elite;
				bool b_sgl_complete = this->check_relation_single_dungeon(single_id);
				if (b_sgl_complete) {
					info.status = Sgl_Dungeon_Data::ELITE_DUNGEON_INACTIVE;
				} else {
					info.status = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
				}
				msg.elite_nor_vec.push_back(info);
			}
		}
	}

//	int remain_add_num = sgl_dungeon_detail_->elite_add_buy_num - sgl_dungeon_detail_->elite_add_num;
//	if(remain_add_num < 0){
//		remain_add_num = 0;
//	}
//	int max_free_num = CONFIG_CACHE_DUNGEON->get_elite_ch_num()+sgl_dungeon_detail_->elite_task_add_num;
//	int remain_free_num = max_free_num - sgl_dungeon_detail_->elite_ch_num;
//	if(remain_free_num < 0){
//		remain_free_num = 0;
//	}
//	msg.challenge_num = remain_add_num + remain_free_num;
//	msg.challenge_num_max = max_free_num;
//	msg.add_num = sgl_dungeon_detail_->elite_add_num;
//	msg.add_buy_num = sgl_dungeon_detail_->elite_add_buy_num;
//	msg.add_num_max = CONFIG_CACHE_DUNGEON->get_elite_add_num();
	msg.nor_wipe_activate = sgl_dungeon_detail_->get_act_elite_wipe(type);
	msg.wipe_elite_double_state = sgl_dungeon_detail_->wipe_elite_double;
	msg.wipe_elite_double_ui_selected =
			!sgl_dungeon_detail_->wipe_elite_double_not_selected;
	// 上古副本每个职业的第一名
	RankingDef::Ranking_Type rank_type = RankingDef::LOCAL_DUNGEON_SG_RANKING;
	if(type != 1){
		rank_type = RankingDef::LOCAL_DUNGEON_SMD_RANKING;
	}
	RANKING_MANAGER->find_rank_role_id_name_by_career(rank_type, msg.role_id_1,
			msg.role_name_1, 1, 1);
	RANKING_MANAGER->find_rank_role_id_name_by_career(rank_type, msg.role_id_2,
			msg.role_name_2, 2, 1);
	RANKING_MANAGER->find_rank_role_id_name_by_career(rank_type, msg.role_id_3,
			msg.role_name_3, 3, 1);
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Single_Dungeon::req_reset_elite_dungeon_times(int bind_gold_first) {
	if (player_self() == NULL) {
		return -1;
	}
	if (player_self()->vip()
			< (uint32_t) CONFIG_CACHE_DUNGEON->get_elite_reset_vip_lv()) {
		return -1;
	}
	if (sgl_dungeon_detail_->elite_reset_times == 0) {
		return -1;
	}
	int item_id = CONFIG_CACHE_DUNGEON->get_elite_reset_cost();
	int ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,
			Id_Amount(item_id, 1));
	if (ret != 0) {
		return ret;
	}
	int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
			Id_Amount(item_id, 1), Lose_Item_DM_Info(ITEM_LOSE_ELITE_RESET));
	if (result != 0) {
		return result;
	}
	Sgl_Dungeon_Map::iterator it;
	for (it = sgl_dungeon_detail_->nor_elite_map_.begin();
			it != sgl_dungeon_detail_->nor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
			it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
		}
	}

	for (it = sgl_dungeon_detail_->hor_elite_map_.begin();
			it != sgl_dungeon_detail_->hor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
			it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
		}
	}

	sgl_dungeon_detail_->elite_reset_times = 0;
	sgl_dungeon_detail_->detail_change();

	MSG_50100204 msg;
	msg.result = 1;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Single_Dungeon::req_receive_chapter_award(int chapter_id) {
	if (player_self() == NULL) {
		return -1;
	}
	int count = sgl_dungeon_detail_->chapter_award_vec.size();
	if (chapter_id <= 0 || chapter_id > count) {
		return -1;
	}
	int rec_status = sgl_dungeon_detail_->get_chapter_status(chapter_id);
	if (rec_status != 1) {
		return -1;
	}
	const Chapter_Award_Config* award_config =
			CONFIG_CACHE_DUNGEON->find_chapter_award_config(chapter_id);
	if (!award_config) {
		return -1;
	}
	Money_Add_List money_add_list;
	Item_Vec pack_item_vec;
	Reward_Info reward;
	MSG_81000102 acv_msg;
	int goods_count = award_config->award_vec.size();
	for (int i = 0; i < goods_count; ++i) {
		reward.reward_id = award_config->award_vec[i].val_1;
		reward.quantity = award_config->award_vec[i].val_2;
		if (reward.reward_id == COPPER || reward.reward_id == GOLD
				|| reward.reward_id == BIND_GOLD
				|| reward.reward_id == DRAGON_SOUL || reward.reward_id == SOULS
				|| reward.reward_id == FRIENDSHIP || reward.reward_id == REPUTATION) {
			Money_Type my_type = (Money_Type) reward.reward_id;
			int my_num = reward.quantity;
			money_add_list.push_back(
					Money_Add_Info(my_type, my_num,Money_DM_Info(MONEY_ADD_SINGLE_CHAPTER)));
		} else {
//			Item_Detail item(reward.reward_id, reward.quantity,Item_Detail::BIND);
//			pack_item_vec.push_back(item);
			make_item_vec(pack_item_vec, reward.reward_id, reward.quantity,
					Item_Detail::BIND);
		}

	}
	// 物品类
	if (pack_item_vec.size() > 0) {
		int ret = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
				pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_SINGLE_CHAPTER));
		if (ret) {
			return ret;
		}
		for (Item_Vec::iterator item_it = pack_item_vec.begin();
				item_it != pack_item_vec.end(); ++item_it) {
			Item_Basic_Info item_base;
			(*item_it).set_item_basic_info(item_base);
			acv_msg.item_info.push_back(item_base);
			Card_Info card_info;
			if ((*item_it).set_item_card_info(card_info)) {
				acv_msg.card_info_vec.push_back(card_info);
			}
		}
	}
	// 资源类
	if (money_add_list.empty() == false) {
		int ret = player_self()->pack_add_money(money_add_list);
		if (ret) {
			return ret;
		}
		int count = money_add_list.size();
		for (int i = 0; i < count; ++i) {
			Property prop;
			prop.type = money_add_list[i].type;
			prop.value = money_add_list[i].nums;
			acv_msg.property.push_back(prop);
		}
	}

	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	// 领取成功
	sgl_dungeon_detail_->set_chapter_status(chapter_id, 2);
	MSG_50100205 msg;
	msg.chapter = chapter_id;
	msg.result = 1;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Single_Dungeon::req_elite_buy_add_num(const int scene_id){
	if(!sgl_dungeon_detail_){
		return 0;
	}
	Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id, 1);
	if(!elite_data){
		return ERROR_FB_NO_SCENE;
	}
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(elite_data->dungeon_id_);
	if(!scene_cfg){
		return ERROR_SCENE_NOT_EXIST;
	}
	if(scene_cfg->dungeon.is_cost_times() == false){
		return ERROR_FB_HAS_CH_NUM;
	}
	if(elite_data->progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){
		int max_ch_num = CONFIG_CACHE_DUNGEON->get_elite_ch_num()+sgl_dungeon_detail_->elite_task_add_num;
		if(sgl_dungeon_detail_->elite_ch_num < max_ch_num){
			req_fetch_elite_dungeon_info(1);
			return ERROR_FB_HAS_CH_NUM;
		}
	}

	if(elite_data->add_gold_num > 0){
		return ERROR_FB_HAS_ELITE_ADD_NUM;
	}
	if(elite_data->add_prop_num > 0){
		return ERROR_FB_HAS_ELITE_ADD_NUM;
	}

	int prop_id = CONFIG_CACHE_DUNGEON->get_elite_add_num_prop_id();
	int prop_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, prop_id);
	if(prop_id != 0 && prop_count > 0){// 道具购买
    	//扣物品
		int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				 Id_Amount(prop_id, 1),Lose_Item_DM_Info(ITEM_LOSE_ELITE_ADD_NUM));
		if (0 != result) {
			return ERROR_SUB_ITEM_FAIL;
		}
		elite_data->add_prop_num = 1;
		sgl_dungeon_detail_->detail_change();
		MSG_50100206 msg;
		msg.add_num = sgl_dungeon_detail_->elite_add_buy_num;
		msg.scene_id = scene_id;
		msg.result = 2;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}else{//钻石购买
		int max_add_num = CONFIG_CACHE_DUNGEON->get_elite_add_num();
		if(sgl_dungeon_detail_->elite_add_buy_num >= max_add_num){// 购买次数达到上限
			return ERROR_FB_ELITE_ADD_NUM_LIMIT;
		}
		int gold_cost = CONFIG_CACHE_DUNGEON->get_elite_add_num_cost();
		Money_Sub_Type cost_type = BIND_GOLD_FIRST;
		if(CONFIG_CACHE_DUNGEON->get_elite_add_num_type() == 2){
			cost_type = GOLD_ONLY;
		}
		int sub_money_result = player_self()->pack_sub_money(Money_Sub_Info(cost_type, gold_cost, MONEY_SUB_ELITE_ADD_NUM));
		if(sub_money_result != 0){
			return sub_money_result;
		}
		elite_data->add_gold_num = 1;
		sgl_dungeon_detail_->elite_add_buy_num += 1;
		sgl_dungeon_detail_->detail_change();
		MSG_50100206 msg;
		msg.add_num = sgl_dungeon_detail_->elite_add_buy_num;
		msg.scene_id = scene_id;
		msg.result = 1;
		THIS_SEND_TO_CLIENT(msg);
		return 0;
	}
	return 0;
}

int Single_Dungeon::req_receive_elite_first_pass(bool to_client/* = true*/, bool hook_gain/* = true*/){
	// 没有领取上古首通
	if(is_sg_first_pass_){
		is_sg_first_pass_ = false;
		const Wipe_Award_Config* award_cfg =
				CONFIG_CACHE_DUNGEON->find_wipe_award_config(sg_first_pass_scene_id_);
		if(award_cfg){
			int base_exp = 0;
			Money_Add_List money_add_list;
			Item_Vec pack_item_vec;
			MSG_81000102 acv_msg;
			acv_msg.type = 0;
			int8_t player_career = this->career();
			int goods_count = award_cfg->first_reward(player_career).size();
			for (int i = 0; i < goods_count; ++i) {
				int item_id = award_cfg->first_reward(player_career)[i].val_1;;
				int item_amount = award_cfg->first_reward(player_career)[i].val_2;
				if (item_id == PT_EXP_CURRENT) {
					base_exp += item_amount;
				} else if (item_id == COPPER || item_id == GOLD
								|| item_id == BIND_GOLD || item_id == SOULS
								|| item_id == DRAGON_SOUL || item_id == FRIENDSHIP || item_id == REPUTATION) {
					Money_Type my_type = (Money_Type) item_id;
					int my_num = item_amount;
					money_add_list.push_back(
							Money_Add_Info(my_type, my_num, Money_DM_Info(MONEY_ADD_FB_AWARD)));
					acv_msg.property.push_back(Property(my_type, my_num));
				} else {
					Item_Vec acv_item_vec;
					make_item_vec(acv_item_vec, item_id, item_amount, Item_Detail::BIND);
					for (Item_Vec::iterator it = acv_item_vec.begin(); it != acv_item_vec.end(); ++it) {
						Item_Basic_Info item_base;
						(*it).set_item_basic_info(item_base);
						acv_msg.item_info.push_back(item_base);
						Card_Info card_info;
						if ((*it).set_item_card_info(card_info)) {
							acv_msg.card_info_vec.push_back(card_info);
						}
						pack_item_vec.push_back(*it);
					}
				}
			}
			if(base_exp > 0){
				Exp_All exp_all = player_self()->modify_experience_inter(base_exp,
						true, true, false, true, 1, opening_card_scene_id_);
				if (0 == exp_all.result) {
					acv_msg.property.push_back(
							Property(PT_EXP_CURRENT, exp_all.all));
					acv_msg.vip_exp = exp_all.vip_exp;
					acv_msg.world_level = player_self()->get_world_level_add();
				}
			}
			if (!money_add_list.empty()) {
				this->player_self()->pack_add_money(money_add_list, MONEY_OPT_REAL);
			}
			if (!pack_item_vec.empty()) {
				this->player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
						pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_FB));
				// 副本获得侦听
				std::vector<Item_Info> gain_items;
				for(Item_Vec::iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it){
					Item_Info item;
					item.id = it->id_;
					item.amount = it->amount_;
					item.bind = it->bind_;
					gain_items.push_back(item);
				}
				player_self()->hook_fb_gain_item(Elite_Scene_Type, sg_first_pass_scene_id_, gain_items);
			}
			if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}
		//
		if(to_client){
			MSG_50100208 msg;
			msg.result = 1;
			THIS_SEND_TO_CLIENT(msg);
		}

	}
	return 0;
}

void Single_Dungeon::add_elite_reset_times(void) {
	sgl_dungeon_detail_->elite_reset_times = 1;
	sgl_dungeon_detail_->detail_change();
}

int Single_Dungeon::get_normal_elite_active_id(void) {
	Sgl_Dungeon_Map::iterator it;
	for (it = sgl_dungeon_detail_->nor_elite_map_.begin();
			it != sgl_dungeon_detail_->nor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE) {
			return it->second.dungeon_id_;
		}
	}
	return 0;
}

void Single_Dungeon::clear_elite_dungeon(const Scene_Config* scene_config) {
	if (!scene_config) {
		return;
	}
	int scene_id = scene_config->dungeon.id;
	int difficulty = scene_config->dungeon.difficulty;
	int next_scene_id = scene_config->dungeon.next_id;
	Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id, difficulty);
	if (!elite_data) {
		MSG_USER("cannot locate dungeon info, dungeon_id=%d, role_id=%ld",
				scene_id, this->role_id());
		return;
	}
	if(elite_data->progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE &&
			elite_data->progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_REAP){
		return;
	}
	is_sg_first_pass_ = false;
	int8_t elite_pre_progress = elite_data->progress_;
	elite_data->progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP;
	if (elite_pre_progress == Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE) {// 激活下一个
		a_new_elite_dungeon(*this->sgl_dungeon_detail_, next_scene_id, difficulty);
		// 记录最优通关
		if (this->player_self()->get_ttl_force() <= 0) {
			MSG_USER_TRACE("elite_complete tt_force == 0,role_id:%ld",
					this->player_self()->role_id());
		} else {
			FB_Info_Detail* fb_detail =
					CAMPFIRE_MANAGER->get_fb_info_detail(scene_id);
			if (fb_detail) {
				if (this->player_self()->get_ttl_force()
						< fb_detail->best_force) {
					CAMPFIRE_MANAGER->set_fb_info_detail(scene_id,
							this->role_id(), this->role_name(),
							this->player_self()->get_ttl_force());
				}
			} else {
				CAMPFIRE_MANAGER->set_fb_info_detail(scene_id,
						this->role_id(), this->role_name(),
						this->player_self()->get_ttl_force());
			}
		}
		// 检测是否激活扫荡
		if (difficulty == 1) {
			sgl_dungeon_detail_->nor_elite_complete_num =
					this->get_nor_elite_complete_num();
			//监听上古通关称号
			player_self()->title_listen_ancient_instance();
			// 扫荡激活
			if(sgl_dungeon_detail_->act_nor_elite_wipe == 0){
				bool act_result = this->is_nor_elite_wipe_out_active(scene_id);
				if(act_result){
					sgl_dungeon_detail_->act_nor_elite_wipe = 1;
				}
			}
			// 魔神殿第一个副本激活
			if(scene_id == CONFIG_CACHE_DUNGEON->get_lord_open_id()){
				activate_lord_temple();
			}
			//		if(scene_config->dungeon.is_cost_times()){// 上古副本boss层消耗次数
			//			if(elite_data->add_prop_num > 0){// 道具
			//				elite_data->add_prop_num = 0;
			//			}else if(elite_data->add_gold_num > 0){// 钻石
			//				elite_data->add_gold_num = 0;
			//				sgl_dungeon_detail_->elite_add_num += 1;
			//			}else{// 免费
			//				sgl_dungeon_detail_->elite_ch_num += 1;
			//			}
			//		}

			// 上古首通
			const Wipe_Award_Config* award_cfg =
					CONFIG_CACHE_DUNGEON->find_wipe_award_config(scene_id);
			if(award_cfg){
				int goods_count = award_cfg->first_reward(player_self()->career()).size();
				if(goods_count > 0){
					is_sg_first_pass_ = true;
					sg_first_pass_scene_id_ = scene_id;
				}
			}

		} else {
			sgl_dungeon_detail_->hor_elite_complete_num =
					this->get_hor_elite_complete_num();
			// 扫荡激活
			if(sgl_dungeon_detail_->act_hor_elite_wipe == 0){
				bool act_result = this->is_hor_elite_wipe_out_active(scene_id);
				if(act_result){
					sgl_dungeon_detail_->act_hor_elite_wipe = 1;
				}
			}
		}

		// 副本首次通关通知
		{
			MSG_81000029 msg;
			msg.scene_id = elite_data->dungeon_id_;
			THIS_SEND_TO_CLIENT(msg);
		}
	}
	this->sgl_dungeon_detail_->detail_change();
	req_fetch_elite_dungeon_info(difficulty);
	if(difficulty == 1 && is_sg_first_pass_){
		send_elite_trail_info_to_client(difficulty);
	}
}

Sgl_Dungeon_Data* Single_Dungeon::get_single_dungeon_data(int scene_id) {
	Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->sgl_dungeon_map_.find(
			scene_id);
	if (it != sgl_dungeon_detail_->sgl_dungeon_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

Sgl_Dungeon_Data* Single_Dungeon::get_elite_dungeon_data(int scene_id,
		int difficulty) {
	if (difficulty == 1) {
		Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->nor_elite_map_.find(
				scene_id);
		if (it != sgl_dungeon_detail_->nor_elite_map_.end()) {
			return &(it->second);
		}
		return NULL;
	} else {
		Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->hor_elite_map_.find(
				scene_id);
		if (it != sgl_dungeon_detail_->hor_elite_map_.end()) {
			return &(it->second);
		}
		return NULL;
	}
	return NULL;
}

bool Single_Dungeon::is_sgl_dungeon_complete(int scene_id) {
	Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->sgl_dungeon_map_.find(
			scene_id);
	if (it != sgl_dungeon_detail_->sgl_dungeon_map_.end()) {
		if (it->second.progress_ == Sgl_Dungeon_Data::COMPLETED) {
			return true;
		}
	}
	return false;
}

int Single_Dungeon::get_shanggu_complete_num(void){
	if (sgl_dungeon_detail_)
		return sgl_dungeon_detail_->nor_elite_complete_num;
	else
		return 0;
}

int Single_Dungeon::get_lord_temple_complete_num(void){
	if (sgl_dungeon_detail_)
		return sgl_dungeon_detail_->hor_elite_complete_num;
	else
		return 0;
}

int Single_Dungeon::get_elite_nor_from_single(int scene_id) {
	return CONFIG_CACHE_SCENE->get_elite_nor_from_single(scene_id);
}

int Single_Dungeon::get_elite_hor_from_single(int scene_id) {
	return CONFIG_CACHE_SCENE->get_elite_hor_from_single(scene_id);
}

int Single_Dungeon::get_single_from_elite(int scene_id) {
	return CONFIG_CACHE_SCENE->get_single_from_elite(scene_id);
}

int Single_Dungeon::get_single_complete_num(void) {
	int num = 0;
	Sgl_Dungeon_Map::const_iterator it;
	for (it = sgl_dungeon_detail_->sgl_dungeon_map_.begin();
			it != sgl_dungeon_detail_->sgl_dungeon_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::COMPLETED) {
			num += 1;
		}
	}
	return num;
}

int Single_Dungeon::get_nor_elite_complete_num(void) {
	int num = 0;
	Sgl_Dungeon_Map::const_iterator it;
	for (it = sgl_dungeon_detail_->nor_elite_map_.begin();
			it != sgl_dungeon_detail_->nor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP
				|| it->second.progress_
						== Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
			num += 1;
		}
	}
	return num;
}

int Single_Dungeon::get_hor_elite_complete_num(void) {
	int num = 0;
	Sgl_Dungeon_Map::const_iterator it;
	for (it = sgl_dungeon_detail_->hor_elite_map_.begin();
			it != sgl_dungeon_detail_->hor_elite_map_.end(); ++it) {
		if (it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP
				|| it->second.progress_
						== Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
			num += 1;
		}
	}
	return num;
}

int Single_Dungeon::listen_strength_recover(const Time_Value& now) {
	Logic_Player* player = this->player_self();
	if (!player) {
		return -1;
	}
	if (player->fighter_detail().phy_power_recover_time == 0) {
		if (player->fighter_detail().current_strength()
				< player->fighter_detail().max_strength()) {
			Time_Value next_time = now
					+ Time_Value(PHY_POWER_RECOVER_TIME_VALUE);
			player->set_phy_power_recover_time(next_time.sec());
			//sync scene
			{
				MSG_20100030 msg;
				msg.phy_power_buy_times =
						player->fighter_detail().phy_power_buy_times;
				msg.phy_power_cur = player->fighter_detail().current_strength();
				msg.phy_power_recover_time =
						player->fighter_detail().phy_power_recover_time;
				THIS_SEND_TO_SCENE(msg);
			}
		}
		return 0;
	}

	Time_Value time = Time_Value(
			player->fighter_detail().phy_power_recover_time);
	if (now < time)
		return 0;

	int phy_power_cur = player->fighter_detail().current_strength();
	phy_power_cur += PHY_POWER_RECOVER_VALUE;
	if (phy_power_cur >= player->fighter_detail().max_strength()) {
		phy_power_cur = player->fighter_detail().max_strength();
		player->set_phy_power_recover_time(0);
	} else {
		Time_Value next_time = now + Time_Value(PHY_POWER_RECOVER_TIME_VALUE);
		player->set_phy_power_recover_time(next_time.sec());
	}
	player->set_phy_power(phy_power_cur);

	//sync scene
	{
		MSG_20100030 msg;
		msg.phy_power_buy_times = player->fighter_detail().phy_power_buy_times;
		msg.phy_power_cur = player->fighter_detail().current_strength();
		msg.phy_power_recover_time =
				player->fighter_detail().phy_power_recover_time;
		THIS_SEND_TO_SCENE(msg);
	}

	return 0;
}

int Single_Dungeon::listen_wipe_out_dungeon(const Time_Value& now) {
	if (!sgl_dungeon_detail_) {
		return -1;
	}
	if (sgl_dungeon_detail_->wipe_type == 0) {
		return 0;
	}
	if (sgl_dungeon_detail_->wipe_status != 0) {
		return 0;
	}
	if (now >= sgl_dungeon_detail_->next_wipe_time) {
		int wipe_result = dungeon_wipe_out(sgl_dungeon_detail_->wipe_type,
				true);
		if (wipe_result == 0) {
			int wipe_num = sgl_dungeon_detail_->wipe_infos.size();
			if (wipe_num >= sgl_dungeon_detail_->wipe_num) {
				sgl_dungeon_detail_->reset_wipe_info();
				// 刷新精英副本信息
				if (sgl_dungeon_detail_->wipe_type == 2) {
					req_fetch_elite_dungeon_info(1);
				}else if(sgl_dungeon_detail_->wipe_type == 3){
					req_fetch_elite_dungeon_info(2);
				}
			}
		}
	}
	return 0;
}

int Single_Dungeon::req_wipe_out(const int wipe_type, const int src_wipe_num,
		const int src_scene_id) {
	Logic_Player* player = player_self();
	if (!player) {
		return -1;
	}
	if (!sgl_dungeon_detail_) {
		return -1;
	}
	if (sgl_dungeon_detail_->wipe_type != 0) {
		return ERROR_WIPE_IS_GOING;
	}

	int level_limit = CONFIG_CACHE_DUNGEON->get_wipe_out_lv_limit(wipe_type);
	if (level() < level_limit) {
		return ERROR_WIPE_LEVEL_LIMIT;
	}
	//获取要扫荡的副本
	int wipe_num = 0;
	std::vector<int> wipe_scenes;

	// 是否开启扫荡功能判断
	if (wipe_type == 1) {
		Sgl_Dungeon_Map::iterator sgl_it =
				sgl_dungeon_detail_->sgl_dungeon_map_.find(src_scene_id);
		if (sgl_it == sgl_dungeon_detail_->sgl_dungeon_map_.end()) {
			return ERROR_WIPE_NO_COMPLETE;
		}
		//scene config
		const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
				src_scene_id);
		if (!scene_cfg) {
			return ERROR_WIPE_NO_SCENE;
		}
		// 体力
		int cost_str = WIPE_COST_STR;
		int cost_type = Scene_Config::Dungeon_Config_Detail::COST_PHY_POWER;
		Scene_Config::Dungeon_Config_Detail::CostMap::const_iterator it;
		it = scene_cfg->dungeon.cost_map.find(cost_type);
		if (it != scene_cfg->dungeon.cost_map.end()) {					//消耗体力
			cost_str = it->second;
		}
		if (cost_str == 0) {
			cost_str = WIPE_COST_STR;
		}
		int strength = player->fighter_detail().current_strength();
		int max_str_num = strength / cost_str;
		if (max_str_num <= 0) {
			return ERROR_STRENGTH_NOT_ENOUGH;
		}
		int max_wipe_num = max_str_num;
		wipe_num = src_wipe_num;
		if (wipe_num == 0) {
			wipe_num = max_wipe_num;
		}
		if (wipe_num > max_wipe_num) {
			return ERROR_STRENGTH_NOT_ENOUGH;
		}

		for (int i = 0; i < wipe_num; ++i) {
			wipe_scenes.push_back(src_scene_id);
		}
	} else if (wipe_type == 2) {
		if(sgl_dungeon_detail_->act_nor_elite_wipe == 0){
			return ERROR_WIPE_NO_ACTIVATE;
		}
		get_wipe_elite_id(wipe_type, wipe_scenes);
		wipe_num = wipe_scenes.size();
	} else {
		if(sgl_dungeon_detail_->act_hor_elite_wipe == 0){
			return ERROR_WIPE_NO_ACTIVATE;
		}
		get_wipe_elite_id(wipe_type, wipe_scenes);
		wipe_num = wipe_scenes.size();
	}
	if (wipe_num <= 0) {
		return ERROR_WIPE_NO_NUM;
	}
	// 时间
	int one_spend_time = CONFIG_CACHE_DUNGEON->get_wipe_out_cost_time(
			wipe_type);
	int spend_time = one_spend_time * wipe_num;
	Time_Value now = Time_Value::gettimeofday();

	sgl_dungeon_detail_->wipe_type = wipe_type;
	sgl_dungeon_detail_->wipe_num = wipe_num;
	sgl_dungeon_detail_->wipe_spend_time = one_spend_time;
	sgl_dungeon_detail_->wipe_begin_time = now;
	sgl_dungeon_detail_->next_wipe_time = now + Time_Value(one_spend_time);
	sgl_dungeon_detail_->wipe_finish_time = now + Time_Value(spend_time);
	sgl_dungeon_detail_->wipe_scenes = wipe_scenes;
	sgl_dungeon_detail_->wipe_infos.clear();
	sgl_dungeon_detail_->detail_change();

	MSG_50100210 msg;
	msg.wipe_type = wipe_type;
	msg.remain_time = spend_time;
	msg.remain_num = wipe_num;
	THIS_SEND_TO_CLIENT(msg);
	// 单人副本扫荡20级前，免cd
	if (wipe_type == 1) {
		int no_cd_lv = CONFIG_CACHE_DUNGEON->get_single_wipe_no_cd_lv();
		if (this->level() <= no_cd_lv) {
			for (int i = 0; i < wipe_num; ++i) {
				dungeon_wipe_out(wipe_type);
			}
			sgl_dungeon_detail_->reset_wipe_info();
			MSG_50100212 msg;
			msg.wipe_type = wipe_type;
			msg.result = -1;
			THIS_SEND_TO_CLIENT(msg);
		}
	}
	return 0;
}

int Single_Dungeon::req_interrupt_wipe_out(const int wipe_type) {
	if (!sgl_dungeon_detail_) {
		return -1;
	}
	if (sgl_dungeon_detail_->wipe_type == 0) {
		return ERROR_WIPE_INTERRUPT_NO_GOING;
	}
	if (sgl_dungeon_detail_->wipe_type != wipe_type) {
		return ERROR_WIPE_INTERRUPT_TYPE_ERR;
	}
	sgl_dungeon_detail_->reset_wipe_info();
	MSG_50100211 msg;
	msg.wipe_type = wipe_type;
	msg.result = 1;
	THIS_SEND_TO_CLIENT(msg);
	// 刷新精英副本信息
	if (wipe_type == 2) {							//
		req_fetch_elite_dungeon_info(1);
	}else if(wipe_type == 3){
		req_fetch_elite_dungeon_info(2);
	}
	return 0;
}

int Single_Dungeon::req_complete_wipe_out(const int wipe_type) {
	if (!sgl_dungeon_detail_) {
		return -1;
	}
	if (sgl_dungeon_detail_->wipe_type == 0) {
		return ERROR_WIPE_INTERRUPT_NO_GOING;
	}
	if (sgl_dungeon_detail_->wipe_type != wipe_type) {
		return ERROR_WIPE_INTERRUPT_TYPE_ERR;
	}
	int complete_num = sgl_dungeon_detail_->wipe_infos.size();
	if (complete_num >= sgl_dungeon_detail_->wipe_num) {
		return ERROR_WIPE_INTERRUPT_NO_GOING;
	}
	int remain_num = sgl_dungeon_detail_->wipe_num - complete_num;
	Time_Value now = Time_Value::gettimeofday();
	int remain_time = sgl_dungeon_detail_->wipe_finish_time.sec() - now.sec();
	int min_time = remain_time / 60;
	int min_remainder = remain_time % 60;
	int cost_min = min_time;
	if (min_remainder > 0) {
		cost_min += 1;
	}
	int one_cost_gold = CONFIG_CACHE_DUNGEON->get_wipe_out_cost_gold(wipe_type);
	int cost_gold = one_cost_gold * cost_min;
	//有精灵盟约不需要扣金币
	bool has_jlmy = player_self()->get_covenant_status(RAIDS_COVENANT);
	if (!has_jlmy) {							// 扣除钻石
		if (cost_gold > 0) {
			int ret = player_self()->pack_try_sub_money(
					Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST), cost_gold,
							MONEY_SUB_FB_COMPLETE_WIPE));
			if (ret) {
				return ret;
			}
		}
		if (cost_gold > 0) {
			player_self()->pack_sub_money(
					Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST), cost_gold,
							MONEY_SUB_FB_COMPLETE_WIPE));
		}
	}

	for (int i = 0; i < remain_num; ++i) {
		dungeon_wipe_out(wipe_type);
	}
	sgl_dungeon_detail_->reset_wipe_info();

	MSG_50100212 msg;
	msg.wipe_type = wipe_type;
	msg.result = 1;
	msg.fast_sweep_num = cost_gold;
	THIS_SEND_TO_CLIENT(msg);
	// 刷新精英副本信息
	if (wipe_type == 2) {							//
		req_fetch_elite_dungeon_info(1);
	}else if(wipe_type == 3){
		req_fetch_elite_dungeon_info(2);
	}
	return 0;
}

int Single_Dungeon::dungeon_wipe_out(const int wipe_type,
		bool to_client/* = true*/) {
	if (player_self() == NULL) {
		return -1;
	}
	if (sgl_dungeon_detail_->wipe_scenes.empty()) {
		return -1;
	}
	int wipe_scene = sgl_dungeon_detail_->wipe_scenes[0];
	int cost_str = 0;
	std::vector<Item_Info> items;
	int calc_result = calc_wipe_result(wipe_type, wipe_scene, items, cost_str);
	if (calc_result != 0) {
		return -1;
	}
	// 扣除体力
	if (wipe_type == 1) {							// 单人扣取体力
		if (offline_wipe_num_ > 0) {
			offline_wipe_num_ -= 1;
		} else {
			if (cost_str > 0) {
				if (cost_str
						> player_self()->fighter_detail().current_strength()) {
					sgl_dungeon_detail_->wipe_status = 2;
					sgl_dungeon_detail_->detail_change();
					return ERROR_STRENGTH_NOT_ENOUGH;
				}
				int ret = player_self()->cost_strength(cost_str, to_client);
				if (ret) {
					sgl_dungeon_detail_->wipe_status = 2;
					sgl_dungeon_detail_->detail_change();
					return ret;
				}
			}
		}
	} else {							// 精英扣取次数
		int difficulty = 1;
		if (wipe_type == 2) {
			difficulty = 1;
		} else {
			difficulty = 2;
		}
		Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(wipe_scene,
				difficulty);
		if (elite_data) {
			if (offline_wipe_num_ > 0) {							//
				offline_wipe_num_ -= 1;
			} else {
				if (elite_data->progress_
						== Sgl_Dungeon_Data::ELITE_DUNGEON_REAP) {
					elite_data->progress_ =
							Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP;
				}
			}
		}
	}

	//vip
	Item_Vec pack_item_vec;
	Money_Add_List money_add_list;
	std::vector<Item_Info> wipe_info_items;
	int base_exp = 0;
	int exp = 0;

	//添加到背包
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if (!items.empty()) {
		Item_Vec item_detail_vec;
		for (std::vector<Item_Info>::iterator item_it = items.begin();
				item_it != items.end(); ++item_it) {
			int id = item_it->id;
			int num = item_it->amount;
			if (id == PT_EXP_CURRENT) {
				base_exp += num;
			} else if (id == COPPER || id == GOLD || id == BIND_GOLD
					|| id == SOULS || id == DRAGON_SOUL || id == FRIENDSHIP) {
				Money_Add_Info money_add_info((Money_Type) id, num,
						Money_DM_Info(MONEY_ADD_FB_AWARD));
				// 精英扫荡双倍
				if (wipe_type == 2 && sgl_dungeon_detail_->wipe_elite_double) {
					money_add_info.nums *= 2;
				}
				money_add_list.push_back(money_add_info);
			} else {
				int32_t quantity = num;
//				Item_Detail detail_info(id, num, Item_Detail::BIND);
				// 精英扫荡双倍
				if (wipe_type == 2 && sgl_dungeon_detail_->wipe_elite_double) {
//					detail_info.amount_ *= 2;
					quantity *= 2;
				}
				make_item_vec(pack_item_vec, id, quantity, Item_Detail::BIND);
//				pack_item_vec.push_back(detail_info);
			}
		}
	}

	if (base_exp > 0) {
		int double_exp = 1;
		// 精英扫荡双倍
		if (wipe_type == 2 && sgl_dungeon_detail_->wipe_elite_double) {
			double_exp = 2;
		}
		Exp_All exp_all = player_self()->modify_experience_inter(base_exp, true,
				true, false, true, double_exp, wipe_scene);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
			exp = exp_all.all;
		}
	}
	if (!money_add_list.empty()) {
		this->player_self()->pack_add_money(money_add_list, MONEY_OPT_REAL);
		int count = money_add_list.size();
		for (int i = 0; i < count; ++i) {
			acv_msg.property.push_back(
					Property(money_add_list[i].type, money_add_list[i].nums));
			Item_Info i_info;
			i_info.id = money_add_list[i].type;
			i_info.amount = money_add_list[i].nums;
			wipe_info_items.push_back(i_info);
		}
	}
	if (pack_item_vec.empty() == false) {
		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
				pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_FB));
		for (Item_Vec::iterator item_it = pack_item_vec.begin();
				item_it != pack_item_vec.end(); ++item_it) {
			Item_Basic_Info item_base;
			(*item_it).set_item_basic_info(item_base);
			acv_msg.item_info.push_back(item_base);
			Card_Info card_info;
			if ((*item_it).set_item_card_info(card_info)) {
				acv_msg.card_info_vec.push_back(card_info);
			}
			Item_Info i_info;
			i_info.id = item_it->id_;
			i_info.amount = item_it->amount_;
			i_info.bind = Item_Detail::BIND;
			wipe_info_items.push_back(i_info);
		}
//		if(ret != 0){// 邮件发送
//			Time_Value now = Time_Value::gettimeofday();
//			Mail_Send_Info send_info;
//			send_info.sender_id = 0;
//			send_info.receiver_id = this->role_id();
//			send_info.send_type = MAIL_TYPE_SYS;
//			send_info.show_send_time = now;
//			int m_i_count;
//			m_i_count = pack_item_vec.size();
//			for(int i = 0; i < m_i_count; ++i){
//				send_info.item_vector.push_back(pack_item_vec[i]);
//			}
//			time_t time_v = now.sec();
//			struct tm tm_v;
//			localtime_r(&time_v, &tm_v);
//			std::stringstream stream;
//			stream <<"Mop up returns  -"<< (tm_v.tm_year + 1900) << "/" << (tm_v.tm_mon + 1) << "/" << (tm_v.tm_mday)<<"\r";
//			stream <<"Received awards:"<< "\r";
//			m_i_count = items.size();
//			for(int i = 0; i < m_i_count; ++i){
//				const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(items[i].id);
//				if(item_config){
//					stream << item_config->name<<" *"<< items[i].amount<< "\r";
//				}
//			}
//			send_info.sender_name = "Mop up";
//			if(wipe_type == 1){
//				send_info.title = "Single Mop up returns";
//			}else{
//				send_info.title = "Elite Mop up returns";
//			}
//			send_info.content = stream.str();
//		   this->player_self()->send_mail_to_self(send_info);
//		}
	}

	// 添加扫荡完副本信息
	int count = sgl_dungeon_detail_->wipe_infos.size();
	Wipe_Info info;
	info.scene_id = wipe_scene;
	info.index = count + 1;
	info.exp = exp;
	info.items = wipe_info_items;
	sgl_dungeon_detail_->wipe_infos.push_back(info);
	// 从扫荡副本中移除扫荡完的副本
	std::vector<int>::iterator it;
	for (it = sgl_dungeon_detail_->wipe_scenes.begin();
			it != sgl_dungeon_detail_->wipe_scenes.end(); ++it) {
		if (wipe_scene == (*it)) {
			sgl_dungeon_detail_->wipe_scenes.erase(it);
			break;
		}
	}
	// 完成下个扫荡时间
	sgl_dungeon_detail_->next_wipe_time = sgl_dungeon_detail_->wipe_begin_time
			+ Time_Value(
					sgl_dungeon_detail_->wipe_spend_time * (info.index + 1));
	sgl_dungeon_detail_->detail_change();

	// 副本通关
	player_self()->hook_compelte_scene(wipe_scene);
	// 完成活跃度
	if (wipe_type == 1) {
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_SINGLE_EXPLORE);
	} else if(wipe_type == 2){
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_ELITE_EXPLORE);
	}

	// 杀怪处理
	const Wipe_Award_Config* award_cfg =
			CONFIG_CACHE_DUNGEON->find_wipe_award_config(wipe_scene);
	if (award_cfg) {
		for (std::vector<Int_Int>::const_iterator it =
				award_cfg->monsters.begin(); it != award_cfg->monsters.end();
				++it) {
			player_self()->hook_kill_monster((*it).val_1, (*it).val_2);
		}
	}

	if (to_client) {
		Time_Value now = Time_Value::gettimeofday();
		int remain_time = sgl_dungeon_detail_->wipe_finish_time.sec()
				- now.sec();
		if (remain_time < 0) {
			remain_time = 0;
		}
		int complete_num = sgl_dungeon_detail_->wipe_infos.size();
		int remain_num = sgl_dungeon_detail_->wipe_num - complete_num;
		if (remain_num <= 0) {
			remain_time = 0;
			remain_num = 0;
		}
		MSG_50100210 msg;
		msg.wipe_type = wipe_type;
		msg.remain_time = remain_time;
		msg.remain_num = remain_num;
		msg.wipe_info.push_back(info);
		THIS_SEND_TO_CLIENT(msg);
		if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}

	return 0;
}

int Single_Dungeon::req_obtain_wipe_double_income(const int wipe_type) {
	if (wipe_type == 2) {
		Id_Amount cost_item(ITEM_ID_TOWER_SOAL, 1);
		if (cost_item.id && cost_item.amount) {
			int try_res = player_self()->pack_try_erase_item(
					PACK_T_PACKAGE_INDEX, cost_item);
			if (try_res) {
				return try_res;
			}
		}
		if (cost_item.id && cost_item.amount) {
			player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_item,
					Lose_Item_DM_Info(ITEM_LOSE_WIPE_DOUBLE));
		}

		sgl_dungeon_detail_->wipe_elite_double = true;
		sgl_dungeon_detail_->detail_change();

		MSG_50100213 res_msg;
		res_msg.wipe_type = wipe_type;
		res_msg.result = 1;
		THIS_SEND_TO_CLIENT(res_msg);
	} else {
		// TODO
	}

	return 0;
}

int Single_Dungeon::req_select_wipe_double(const int wipe_type,
		const int is_selected) {
	if (wipe_type == 2) {
		sgl_dungeon_detail_->wipe_elite_double_not_selected = !is_selected;
		sgl_dungeon_detail_->detail_change();

		MSG_50100214 res_msg;
		res_msg.wipe_type = wipe_type;
		res_msg.is_selected = is_selected;
		THIS_SEND_TO_CLIENT(res_msg);
	}

	return 0;
}

int Single_Dungeon::req_open_single_card(int index){
	if(is_opening_card_ == 0){
		return ERROR_FB_OPEN_CARD_ALREADY;
	}
	if(is_opening_card_ == 2){
		is_opening_card_ = 0;
		MSG_50100201 msg;
		msg.type = 1;
		THIS_SEND_TO_CLIENT(msg);
		return ERROR_FB_OPEN_CARD_NO_PROFIT;
	}
	is_opening_card_ = 0;
	const Wipe_Award_Config* award_cfg =
			CONFIG_CACHE_DUNGEON->find_wipe_award_config(opening_card_scene_id_);
	if(award_cfg){
		int8_t player_career = this->career();
		int card_id = 0;
		int card_amount = 0;
		int card_index = award_cfg->get_card_reward(player_career, card_id, card_amount);

		if(card_id > 0 && card_amount > 0){
			int base_exp = 0;
			Money_Add_List money_add_list;
			Item_Vec pack_item_vec;
			MSG_81000102 acv_msg;
			acv_msg.type = 0;

			if (card_id == PT_EXP_CURRENT) {
				base_exp += card_amount;
			} else if (card_id == COPPER || card_id == GOLD
							|| card_id == BIND_GOLD || card_id == SOULS
							|| card_id == DRAGON_SOUL || card_id == FRIENDSHIP) {
				Money_Type my_type = (Money_Type) card_id;
				int my_num = card_amount;
				money_add_list.push_back(
						Money_Add_Info(my_type, my_num,
								Money_DM_Info(MONEY_ADD_FB_AWARD)));
				acv_msg.property.push_back(Property(my_type, my_num));
			} else {
				Item_Vec acv_item_vec;
				make_item_vec(acv_item_vec, card_id, card_amount,
						Item_Detail::BIND);
				for (Item_Vec::iterator it = acv_item_vec.begin();
						it != acv_item_vec.end(); ++it) {
					Item_Basic_Info item_base;
					(*it).set_item_basic_info(item_base);
					acv_msg.item_info.push_back(item_base);
					Card_Info card_info;
					if ((*it).set_item_card_info(card_info)) {
						acv_msg.card_info_vec.push_back(card_info);
					}
					pack_item_vec.push_back(*it);
				}
			}

			if(base_exp > 0){
				Exp_All exp_all = player_self()->modify_experience_inter(base_exp,
						true, true, false, true, 1, opening_card_scene_id_);
				if (0 == exp_all.result) {
					acv_msg.property.push_back(
							Property(PT_EXP_CURRENT, exp_all.all));
					acv_msg.vip_exp = exp_all.vip_exp;
					acv_msg.world_level = player_self()->get_world_level_add();
				}
			}
			if (!money_add_list.empty()) {
				this->player_self()->pack_add_money(money_add_list, MONEY_OPT_REAL);
			}
			if (!pack_item_vec.empty()) {
				this->player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
						pack_item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_FB));
			}

			if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}

		std::vector<Single_Card_Config> show_cards;
		award_cfg->get_show_card(player_career, index, card_index, show_cards);
		{
			MSG_50100201 msg;
			msg.type = 1;
			int num = show_cards.size();
			for(int i = 0; i < num; ++i){
				Reward_Info info;
				info.reward_id = show_cards[i].id;
				info.quantity = show_cards[i].amount;
				info.bind = Item_Detail::BIND;
				msg.award.push_back(info);
			}
			THIS_SEND_TO_CLIENT(msg);
		}

	}
	return 0;
}

int Single_Dungeon::calc_wipe_result(const int wipe_type, int scene_id,
		std::vector<Item_Info>& items, int& cost_str) {
	if (player_self() == NULL) {
		return -1;
	}
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
			scene_id);
	if (!scene_cfg) {
		return -1;
	}
	const Wipe_Award_Config* award_cfg =
			CONFIG_CACHE_DUNGEON->find_wipe_award_config(scene_id);
	if (!award_cfg) {
		return -1;
	}
	const Wipe_Base_Config* base_cfg = CONFIG_CACHE_DUNGEON->wipe_base_config();
	if (!base_cfg) {
		return -1;
	}
	int8_t player_career = this->career();
	if (wipe_type == 1) {
		// 扣除体力
		cost_str = WIPE_COST_STR;
		int cost_type = Scene_Config::Dungeon_Config_Detail::COST_PHY_POWER;
		Scene_Config::Dungeon_Config_Detail::CostMap::const_iterator it;
		it = scene_cfg->dungeon.cost_map.find(cost_type);
		if (it != scene_cfg->dungeon.cost_map.end()) {				//消耗体力
			cost_str = it->second;
		}
		if (cost_str == 0) {
			cost_str = WIPE_COST_STR;
		}
	}
	// monster
	int cfg_exp = 0;
	int cfg_copper = 0;
	int cfg_soul = 0;
	int num;
	num = award_cfg->monsters.size();
	for (int i = 0; i < num; ++i) {
		int ms_count = award_cfg->monsters[i].val_2;
		for (int j = 0; j < ms_count; ++j) {
			int monster_id = award_cfg->monsters[i].val_1;
			// 任务掉落
			Int_Int_Vec task_items = player_self()->get_item_list(monster_id);
			int task_items_count = task_items.size();
			for (int i = 0; i < task_items_count; ++i) {
				if (task_items[i].val_2 > random() % 100) {
					int taks_item_id = task_items[i].val_1;
					add_wipe_items(taks_item_id, 1, items);
				}
			}

			// 掉落物
			const NPC_Config_Cache* monster_cfg =
					CONFIG_CACHE_NPC->find_npc_config_cache(monster_id);
			if (monster_cfg) {
				cfg_exp += monster_cfg->exp;
				cfg_copper += monster_cfg->money;
				cfg_soul += monster_cfg->soul;
				//drop
				int d_count = monster_cfg->drops_vec.size();
				for (int k = 0; k < d_count; ++k) {
					int drop_id = monster_cfg->drops_vec[k];
					const Drop_Config* drop_cfg =
							CONFIG_CACHE_NPC->find_drop_config_cache(drop_id);
					if (drop_cfg) {
						Drop_Vec drops_all;
						NPC_MANAGER->process_drop_mode(player_career, drops_all,
								drop_cfg, 1.0, 1.0);
						int da_count = drops_all.size();
						for (int n = 0; n < da_count; ++n) {
							add_wipe_items(drops_all[n].item,
									drops_all[n].amount, items);
						}
					}
				}
			}
		}
	}
	add_wipe_items(PT_EXP_CURRENT, cfg_exp, items);
	add_wipe_items(COPPER, cfg_copper, items);
	add_wipe_items(SOULS, cfg_soul, items);

	// box
	num = award_cfg->box.size();
	for (int i = 0; i < num; ++i) {
		int box_count = award_cfg->box[i].val_2;
		for (int j = 0; j < box_count; ++j) {
			int drop_id = award_cfg->box[i].val_1;
			const Drop_Config* drop_cfg =
					CONFIG_CACHE_NPC->find_drop_config_cache(drop_id);
			if (drop_cfg) {
				Drop_Vec drops_all;
				NPC_MANAGER->process_drop_mode(player_career, drops_all,
						drop_cfg, 1.0, 1.0);
				int da_count = drops_all.size();
				for (int n = 0; n < da_count; ++n) {
					add_wipe_items(drops_all[n].item, drops_all[n].amount,
							items);
				}
			}
		}
	}
	//vip_box
	if (player_self()->vip() > 0) {
		num = award_cfg->vip_box.size();
		for (int i = 0; i < num; ++i) {
			int box_count = award_cfg->vip_box[i].val_2;
			for (int j = 0; j < box_count; ++j) {
				int drop_id = award_cfg->vip_box[i].val_1;
				const Drop_Config* drop_cfg =
						CONFIG_CACHE_NPC->find_drop_config_cache(drop_id);
				if (drop_cfg) {
					Drop_Vec drops_all;
					NPC_MANAGER->process_drop_mode(player_career, drops_all,
							drop_cfg, 1.0, 1.0);
					int da_count = drops_all.size();
					for (int n = 0; n < da_count; ++n) {
						add_wipe_items(drops_all[n].item, drops_all[n].amount,
								items);
					}
				}
			}
		}
	}
	// 副本结算奖励
	int goods_count = award_cfg->copy_reward(player_career).size();
	for (int i = 0; i < goods_count; ++i) {
		int id = award_cfg->copy_reward(player_career)[i].val_1;
		int num = award_cfg->copy_reward(player_career)[i].val_2;
		add_wipe_items(id, num, items);
	}
	// 翻牌奖励
	int card_id = 0;
	int card_amount = 0;
	award_cfg->get_card_reward(player_career, card_id, card_amount);
	add_wipe_items(card_id, card_amount, items);
	return 0;
}

void Single_Dungeon::add_wipe_items(const int item_id, const int num,
		std::vector<Item_Info>& items) {
	if (num <= 0) {
		return;
	}
	int count = items.size();
	for (int i = 0; i < count; ++i) {
		if (item_id == items[i].id) {
			items[i].amount += num;
			return;
		}
	}
	Item_Info info;
	info.id = item_id;
	info.amount = num;
	items.push_back(info);
}

void Single_Dungeon::init_wipe_out(void) {
	if (!sgl_dungeon_detail_) {
		return;
	}
	offline_wipe_num_ = 0;
	if (sgl_dungeon_detail_->wipe_type != 0
			&& sgl_dungeon_detail_->wipe_status == 0) {
		int single_spend_time = CONFIG_CACHE_DUNGEON->get_wipe_out_cost_time(
				sgl_dungeon_detail_->wipe_type);
		sgl_dungeon_detail_->wipe_spend_time = single_spend_time;
		Time_Value now = Time_Value::gettimeofday();
		int total_time = sgl_dungeon_detail_->wipe_num
				* sgl_dungeon_detail_->wipe_spend_time;
		sgl_dungeon_detail_->wipe_finish_time =
				sgl_dungeon_detail_->wipe_begin_time + Time_Value(total_time);
		int spend_time = now.sec() - sgl_dungeon_detail_->wipe_begin_time.sec();
		int num = spend_time / sgl_dungeon_detail_->wipe_spend_time;
		if (num > sgl_dungeon_detail_->wipe_num) {
			num = sgl_dungeon_detail_->wipe_num;
		}
		int complete_num = sgl_dungeon_detail_->wipe_infos.size();
		if (num > complete_num) {
			offline_wipe_num_ = num - complete_num;
			if (sgl_dungeon_detail_->wipe_type == 1) {				// 单人扣除体力
				// 体力
				int one_cost_str = WIPE_COST_STR;
				if(sgl_dungeon_detail_->wipe_scenes.size() > 0){
					int offline_scene_id = sgl_dungeon_detail_->wipe_scenes[0];
					const Scene_Config* offline_scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
							offline_scene_id);
					if(offline_scene_cfg){
						int cost_type = Scene_Config::Dungeon_Config_Detail::COST_PHY_POWER;
						Scene_Config::Dungeon_Config_Detail::CostMap::const_iterator it;
						it = offline_scene_cfg->dungeon.cost_map.find(cost_type);
						if (it != offline_scene_cfg->dungeon.cost_map.end()) {					//消耗体力
							one_cost_str = it->second;
						}
						if (one_cost_str == 0) {
							one_cost_str = WIPE_COST_STR;
						}
					}

				}
				int str_cost = offline_wipe_num_ * one_cost_str;
				player_self()->cost_strength(str_cost, false);
			} else {				// 精英扣取次数
				int difficulty = 1;
				if (sgl_dungeon_detail_->wipe_type == 2) {
					difficulty = 1;
				} else {
					difficulty = 2;
				}
				int scene_num = sgl_dungeon_detail_->wipe_scenes.size();
				for (int i = 0; i < offline_wipe_num_ && i < scene_num; ++i) {
					int wipe_scene = sgl_dungeon_detail_->wipe_scenes[i];
					Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(
							wipe_scene, difficulty);
					if (elite_data) {
						if (elite_data->progress_
								== Sgl_Dungeon_Data::ELITE_DUNGEON_REAP) {
							elite_data->progress_ =
									Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP;
						}
					}
				}
			}
//			for(int i = 0; i < wipe_num; ++i){
//				dungeon_wipe_out(sgl_dungeon_detail_->wipe_type, false);
//			}
			sgl_dungeon_detail_->next_wipe_time = Time_Value::zero;
		} else {
			sgl_dungeon_detail_->next_wipe_time =
					sgl_dungeon_detail_->wipe_begin_time
							+ Time_Value(
									sgl_dungeon_detail_->wipe_spend_time
											* (complete_num + 1));
		}
		if (complete_num >= sgl_dungeon_detail_->wipe_num) {
			sgl_dungeon_detail_->wipe_status = 1;
		}
	}
}

int Single_Dungeon::get_single_dungeon_status(const int scene_id) {
	Sgl_Dungeon_Data* data = get_single_dungeon_data(scene_id);
	if (data) {
		return data->progress_;
	}
	return Sgl_Dungeon_Data::NOT_COMPLETED;
}

void Single_Dungeon::get_elite_dungeon_status(const int scene_id, int& status) {
	status = 0;
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
			scene_id);
	if (scene_cfg) {
		Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id,
				scene_cfg->dungeon.difficulty);
		if (elite_data) {
			status = elite_data->progress_;
		}
	}
	return;
}

bool Single_Dungeon::check_relation_single_dungeon(const int scene_id){
	if(scene_id == 0){
		return true;
	}
	return is_single_dungeon_complete(scene_id);
}

bool Single_Dungeon::check_relation_elite_dungeon(const int scene_id){
	if(scene_id == 0){
		return true;
	}
	return is_elite_dungeon_complete(scene_id);
}

bool Single_Dungeon::is_single_dungeon_complete(const int scene_id) {
	int status = this->get_single_dungeon_status(scene_id);
	if (status == Sgl_Dungeon_Data::COMPLETED) {
		return true;
	}
	return false;
}

bool Single_Dungeon::is_elite_dungeon_complete(const int scene_id) {
	int status = 0;
	get_elite_dungeon_status(scene_id, status);
	if (status == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP
			|| status == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
		return true;
	}
	return false;
}

bool Single_Dungeon::is_elite_dungeon_unreap(const int scene_id) {
	int status = 0;
	get_elite_dungeon_status(scene_id, status);
	if (status == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP) {
		return true;
	}
	return false;
}

int Single_Dungeon::get_elite_dungeon_activation_num(void) {
	int num = 0;
	Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(1);
	for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
		Sgl_Dungeon_Data* data = this->get_elite_dungeon_data(*it, 1);
		if (!data || data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK
				|| data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_INACTIVE) {
			break;
		}
		++num;
	}
	return num;
}

int Single_Dungeon::get_elite_challenge_id(const int scene_id){
	Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id, 1);
	if(!elite_data){
		return scene_id;
	}
	while(elite_data){
		const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(elite_data->dungeon_id_);
		if(!scene_cfg){
			return elite_data->dungeon_id_;
		}
		if(scene_cfg->dungeon.is_cost_times() || elite_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE){
			return elite_data->dungeon_id_;
		}
		elite_data = get_elite_dungeon_data(scene_cfg->dungeon.next_id, 1);
	}
	return scene_id;
}

int Single_Dungeon::check_elite_challenge(const int scene_id){
	Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id, 1);
	if(!elite_data){
		return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
	}
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(elite_data->dungeon_id_);
	if(!scene_cfg){
		return ERROR_SCENE_NOT_EXIST;
	}
	if(elite_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK ||
			elite_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_INACTIVE){
		return ERROR_FB_INACTIVE;
	}
	if(scene_cfg->dungeon.is_cost_times()){
		if(elite_data->add_prop_num > 0){//
			return 0;
		}
		if(elite_data->add_gold_num > 0){
			return 0;
		}
		if(elite_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){// 已经打过的副本，只能用付费打
			return ERROR_FB_NEED_ADD_NUM;
		}else{
			if(is_elite_free_challenge() == false){
				return ERROR_FB_NO_CH_NUM;
			}
		}
	}else{
		if(elite_data->progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE){
			return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
		}
	}
	return 0;
}

int Single_Dungeon::check_elite_tran(const int scene_id){
	Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id, 1);
	if(!elite_data){
		return ERROR_NOT_ALLOW_ENTER_DES_SCENE;
	}
	const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(elite_data->dungeon_id_);
	if(!scene_cfg){
		return ERROR_SCENE_NOT_EXIST;
	}
	if(elite_data->add_prop_num > 0){//
		return 0;
	}
	if(elite_data->add_gold_num > 0){
		return 0;
	}
	if(scene_cfg->dungeon.is_cost_times() &&
			elite_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){
		//prop
		int prop_id = CONFIG_CACHE_DUNGEON->get_elite_add_num_prop_id();
		int prop_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, prop_id);
		if(prop_id != 0 && prop_count > 0){// 道具购买
			MSG_81000800 msg;
			msg.scene_id = scene_id;
			THIS_SEND_TO_CLIENT(msg);
			//return ERROR_FB_NEED_ADD_NUM;
			return -1;
		}
		int max_add_num = CONFIG_CACHE_DUNGEON->get_elite_add_num();
		if(sgl_dungeon_detail_->elite_add_buy_num >= max_add_num){
			return ERROR_FB_ELITE_ADD_NUM_LIMIT;
		}
		MSG_81000800 msg;
		msg.scene_id = scene_id;
		THIS_SEND_TO_CLIENT(msg);
		//return ERROR_FB_NEED_ADD_NUM;
		return -1;
	}else{
		if(is_elite_free_challenge()){
			return 0;
		}
		//prop
		int prop_id = CONFIG_CACHE_DUNGEON->get_elite_add_num_prop_id();
		int prop_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, prop_id);
		if(prop_id != 0 && prop_count > 0){// 道具购买
			MSG_81000800 msg;
			msg.scene_id = scene_id;
			THIS_SEND_TO_CLIENT(msg);
			return ERROR_FB_NO_CH_NUM;
		}
		int max_add_num = CONFIG_CACHE_DUNGEON->get_elite_add_num();
		if(sgl_dungeon_detail_->elite_add_buy_num >= max_add_num){
			return ERROR_FB_ELITE_ADD_NUM_LIMIT;
		}
		MSG_81000800 msg;
		msg.scene_id = scene_id;
		THIS_SEND_TO_CLIENT(msg);
		return ERROR_FB_NO_CH_NUM;
	}
}

bool Single_Dungeon::is_elite_free_challenge(void){
	int max_ch_num = CONFIG_CACHE_DUNGEON->get_elite_ch_num()+sgl_dungeon_detail_->elite_task_add_num;
	if(sgl_dungeon_detail_->elite_ch_num < max_ch_num ){
		return true;
	}
	return false;
}

bool Single_Dungeon::is_in_wipe_out(void) {
	if (sgl_dungeon_detail_->wipe_type != 0) {
		return true;
	}
	return false;
}

int Single_Dungeon::get_wipe_elite_id(const int wipe_type,
		std::vector<int>& wipe_scenes) {
	if (!sgl_dungeon_detail_) {
		return -1;
	}
	Sgl_Dungeon_Map::iterator it;
	if (wipe_type == 2) {// 普通
		Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(1);
		for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
			int scene_id = *it;
			Sgl_Dungeon_Data* data = this->get_elite_dungeon_data(scene_id, 1);
			if (!data) {
				break;
			}
			if (data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP) {
				wipe_scenes.push_back(scene_id);
			}
		}
	} else if (wipe_type == 3) {
		Int_Vec scene_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(2);
		for(Int_Vec::iterator it = scene_ids.begin(); it != scene_ids.end(); ++it){
			int scene_id = *it;
			Sgl_Dungeon_Data* data = this->get_elite_dungeon_data(scene_id, 2);
			if (!data) {
				break;
			}
			if (data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP) {
				wipe_scenes.push_back(scene_id);
			}
		}
	}
	return 0;
}

bool Single_Dungeon::is_nor_elite_wipe_out_active(const int scene_id) {
	return CONFIG_CACHE_DUNGEON->is_nor_elite_wipe_out_activate(scene_id);
}

bool Single_Dungeon::is_hor_elite_wipe_out_active(const int scene_id) {
	return CONFIG_CACHE_DUNGEON->is_hor_elite_wipe_out_activate(scene_id);
}

void Single_Dungeon::hook_single_dungeon_first_complete(const int scene_id) {
	// 上古副本状态修改
	for (Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->nor_elite_map_.begin();
			it != sgl_dungeon_detail_->nor_elite_map_.end(); ++it) {
		if (it->second.progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK) {
			continue;
		}
		const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
				it->second.dungeon_id_);
		if (scene_cfg) {
			if (scene_cfg->dungeon.elite == scene_id) {
				it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
				sgl_dungeon_detail_->detail_change();
			}
		}
	}

	//单人副本章节完成
	int chapter_id = CONFIG_CACHE_DUNGEON->check_single_dungeon_chapter_finish(
			scene_id);
	if (chapter_id > 0) {
		this->sgl_dungeon_detail_->set_chapter_status(chapter_id, 1);
		this->hook_single_dungeon_chapter_complete(chapter_id);
		// 正常返回主城，弹章节奖励领取
		{
			MSG_82100009 msg;
			msg.chapter_id = chapter_id;
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	// 上古神器
	player_self()->anci_artifact_listen_unban(scene_id);
}

void Single_Dungeon::act_first_elite_dungeon(void) {
	if (sgl_dungeon_detail_) {
		sgl_dungeon_detail_->nor_elite_map_.clear();
		player_self()->a_new_elite_dungeon(*sgl_dungeon_detail_,
				CONFIG_CACHE_ROLE->get_elite_first_id(1), 1);
		sgl_dungeon_detail_->hor_elite_map_.clear();
		player_self()->a_new_elite_dungeon(*sgl_dungeon_detail_,
				CONFIG_CACHE_ROLE->get_elite_first_id(2), 2);
	}
}

void Single_Dungeon::act_first_single_dungeon(void) {
	if (sgl_dungeon_detail_) {
		sgl_dungeon_detail_->sgl_dungeon_map_.clear();
		player_self()->a_new_single_dungeon(*sgl_dungeon_detail_,
				CONFIG_CACHE_ROLE->get_single_first_id());
	}
}

void Single_Dungeon::hook_single_dungeon_chapter_complete(
		const int chapter_id) {
	player_self()->achievement_listen_single_dungeon_chapter_complete(
			chapter_id);
}

void Single_Dungeon::hook_fb_gain_item(const int scene_type, const int scene_id, const std::vector<Item_Info> items) {
	// 上古副本获得追踪
	if (scene_type == Elite_Scene_Type) {
		int diff = 1;
		const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
		if(cfg){
			diff = cfg->dungeon.difficulty;
		}
		elite_gain_items(items, diff);
		return;
	}

}

void Single_Dungeon::task_add_elite_challenge_num(const int add_num){
	if(!sgl_dungeon_detail_){
		return;
	}
	sgl_dungeon_detail_->elite_task_add_num += add_num;
	sgl_dungeon_detail_->detail_change();
	req_fetch_elite_dungeon_info(1);
}

int Single_Dungeon::get_last_elite_boss_scene_id(void){
	int result_id = 0;
	int scene_id = CONFIG_CACHE_ROLE->player_init_cache().first_nor_elite_id;
	Sgl_Dungeon_Data* elite_data = get_elite_dungeon_data(scene_id, 1);
	while(elite_data){
		if(elite_data->progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_REAP &&
				elite_data->progress_ != Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){
			break;
		}
		const Scene_Config* scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(elite_data->dungeon_id_);
		if(!scene_cfg){
			break;
		}
		if(scene_cfg->dungeon.is_cost_times()){
			result_id = elite_data->dungeon_id_;
		}
		elite_data = get_elite_dungeon_data(scene_cfg->dungeon.next_id, 1);
	}
	return result_id;
}

void Single_Dungeon::elite_gain_items(const std::vector<Item_Info> items, const int diff) {
	if (!sgl_dungeon_detail_) {
		return;
	}
	if(diff == 1){
		for (std::vector<Item_Info>::const_iterator it = items.begin();
				it != items.end(); ++it) {
			sgl_dungeon_detail_->elite_trail_info.elite_items[it->id].id = it->id;
			sgl_dungeon_detail_->elite_trail_info.elite_items[it->id].amount +=
					it->amount;
			sgl_dungeon_detail_->elite_trail_info.elite_items[it->id].bind = it->bind;
		}
		sgl_dungeon_detail_->detail_change();
	}else{
		for (std::vector<Item_Info>::const_iterator it = items.begin();
				it != items.end(); ++it) {
			sgl_dungeon_detail_->lord_trail_info.elite_items[it->id].id = it->id;
			sgl_dungeon_detail_->lord_trail_info.elite_items[it->id].amount +=
					it->amount;
			sgl_dungeon_detail_->lord_trail_info.elite_items[it->id].bind = it->bind;
		}
		sgl_dungeon_detail_->detail_change();
	}
	// to client
	send_elite_trail_info_to_client(diff);
}

void Single_Dungeon::send_elite_trail_info_to_client(const int diff) {
	if (!sgl_dungeon_detail_) {
		return;
	}
	MSG_81000090 msg;
	msg.type = diff;
	if(diff == 1){
		for (Item_Info_Map::iterator it = sgl_dungeon_detail_->elite_trail_info.elite_items.begin();
				it != sgl_dungeon_detail_->elite_trail_info.elite_items.end(); ++it) {
			if (it->first == ITEM_ID_ELITE_COIN) {
				msg.coin_num = it->second.amount;
				continue;
			}
			msg.items.push_back(it->second);
		}
		//上古首通
		if(is_sg_first_pass_){
			msg.first_pass = 1;
			const Wipe_Award_Config* award_cfg = CONFIG_CACHE_DUNGEON->find_wipe_award_config(sg_first_pass_scene_id_);
			if (award_cfg) {
				int8_t player_career = player_self()->career();
				int goods_count = award_cfg->first_reward(player_career).size();
				for (int i = 0; i < goods_count; ++i) {
					Item_Info item_info;
					item_info.id = award_cfg->first_reward(player_career)[i].val_1;
					item_info.amount = award_cfg->first_reward(player_career)[i].val_2;
					msg.first_pass_items.push_back(item_info);
				}
			}
		}
	}else{
		for (Item_Info_Map::iterator it = sgl_dungeon_detail_->lord_trail_info.elite_items.begin();
				it != sgl_dungeon_detail_->lord_trail_info.elite_items.end(); ++it) {
			if (it->first == ITEM_ID_LORD_COIN) {
				msg.coin_num = it->second.amount;
				continue;
			}
			msg.items.push_back(it->second);
		}
	}
	THIS_SEND_TO_CLIENT(msg);
}

void Single_Dungeon::check_elite_wipe_open(void){
	if (!sgl_dungeon_detail_) {
		return;
	}
	// 上古副本
	{
		int scene_id = CONFIG_CACHE_DUNGEON->get_elite_wipe_out_activate_id(1);
		Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->nor_elite_map_.find(scene_id);
		if(it != sgl_dungeon_detail_->nor_elite_map_.end()){
			if(it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
				it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){
				sgl_dungeon_detail_->act_nor_elite_wipe = 1;
			}
		}
	}
	// 魔神殿
	{
		int scene_id = CONFIG_CACHE_DUNGEON->get_elite_wipe_out_activate_id(2);
		Sgl_Dungeon_Map::iterator it = sgl_dungeon_detail_->hor_elite_map_.find(scene_id);
		if(it != sgl_dungeon_detail_->hor_elite_map_.end()){
			if(it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
				it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){
				sgl_dungeon_detail_->act_hor_elite_wipe = 1;
			}
		}
	}
}

void Single_Dungeon::activate_lord_temple(void){
	int first_lord_id = CONFIG_CACHE_ROLE->get_elite_first_id(2);
	Sgl_Dungeon_Data* lord_data = get_elite_dungeon_data(first_lord_id, 2);
	if(lord_data){
		if(lord_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK ||
				lord_data->progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_INACTIVE){
			lord_data->progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
		}
	}else{
		Sgl_Dungeon_Data data;
		data.dungeon_id_ = first_lord_id;
		data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
		sgl_dungeon_detail_->hor_elite_map_[data.dungeon_id_] = data;
	}
	// send mail
	if(player_self()){
		std::string mail_sender = "system";
		std::string mail_title = "honor arena open";
		std::string mail_content = "honor arena open";
		const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1048);
		if(cfg_mail){
			mail_sender = cfg_mail->sender_name;
			mail_title = cfg_mail->mail_title;
			mail_content = cfg_mail->mail_content;
		}

		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.sender_name = mail_sender;
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.show_send_time = Time_Value::gettimeofday();
		send_info.receiver_id = player_self()->role_id();
		send_info.title = mail_title;
		send_info.content = mail_content;
		player_self()->send_mail_to_self(send_info);
	}

}

