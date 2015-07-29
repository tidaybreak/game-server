/*
 * Wing.cpp
 *
 *  Created on: 2014年10月13日
 *      Author: root
 */

#include "Wing.h"
#include "Logic_Player.h"
#include "Wing_Struct.h"
#include "Wing_Config.h"
#include "Config_Cache_Public.h"
#include "Config_Cache_Wing.h"
#include "Config_Cache_Role.h"
#include "Msg_Fashion_Struct.h"
#include "Err_Code.h"
#include "Fighter_Detail.h"
#include "mall/Mall_Manager.h"
#include "Msg_Active_Struct.h"
#include "public/Public_Def.h"
#include "oper_activity/Oper_Activity_Def.h"

static const int WING_ITEM_ID = 20103501;

Wing::~Wing(void) {

}

void Wing::reset(void) {
	wing_detail_ = 0;
}

void Wing::load_detail(Wing_Detail &detail) {
	wing_detail_ = &detail;
}

void Wing::save_detail(Wing_Detail &detail) {
	if (wing_detail_->is_change) {
		detail = wing_detail();
		wing_detail_->is_change = false;
	}
}

void Wing::sign_int(void) {

}

void Wing::sign_out(void) {

}

void Wing::module_init(void) {

	if (!is_wing_normal()) return;
	if (0 == wing_detail_->lv) return;

	const Wing_Config* cfg = CONFIG_CACHE_WING->get_wing_config(wing_detail_->lv);
	if (0 == cfg) return;
	Prop_Setter_Vec vec;
	Prop_Setter setter;
	for (Wing_Prop_Value_Map::const_iterator it = cfg->player_prop_map.begin();
			it != cfg->player_prop_map.end(); ++it) {
		setter.reset();
		setter.addi_type = Addition_Type::AT_WING;
		setter.operate = O_ADD;
		setter.prop_type = Property_Type(it->first);
		setter.basic = 0.0;
		setter.fixed = it->second.fix;
		setter.percent = it->second.percent;
		vec.push_back(setter);
	}

	if (!vec.empty()) player_self()->fight_prop_modify(vec, 222);

	for (Int_Vec::const_iterator it = cfg->skill_vec.begin(); it != cfg->skill_vec.end(); ++it) {
		player_self()->init_be_skill_extra(*it, 1);
	}

	refresh_wing_avatar();
}

int Wing::trigger_daily_six(const Time_Value& now) {		// 在线每天六点清零，非在线，上线计算
//	wing_detail_->last_time = Time_Value::zero;
//	wing_detail_->progress = 0;
//	wing_detail_->rate = 0;
//	wing_detail_->times = 0;
	return 0;
}

void Wing::listen_wing_experience_expired(const Time_Value& now) {
	if (0 == wing_detail_) return;
	if (Wing_Detail::WING_EXP != wing_detail_->state) return;
	const int secs = CONFIG_CACHE_PUBLIC->get_wing_experience_mins() * 60;
	Time_Value expired_time = wing_detail_->active_time + Time_Value(secs);

	// 未过期
	if (expired_time > now) return;

	//已处理过期
	if (0 == wing_detail_->lv) return;

	// 过期处理
	int old_lv = wing_detail_->lv;
	wing_detail_->lv = 0;
	wing_detail_->is_show = Wing_Detail::SHOW_OFF;

	refresh_wing_attr(old_lv, wing_detail_->lv);
	player_self()->refresh_avatar();
	player_self()->player_try_time_out(TRY_MAGIC_WING_TIMEOUT);
}

int Wing::handle_active_wing_experience(void) {

	if (0 == wing_detail_) return ERROR_INNER_SERVER;
	if (wing_detail_->state != Wing_Detail::WING_NOT_ACTIVE)
		return ERROR_WING_HAS_ACTIVVE;

	if (!player_self()->is_magic_wing_try_task_submit())
		return ERROR_PRE_TASK_NOT_FINISH;

	int exp_lv = CONFIG_CACHE_PUBLIC->get_wing_experience_lv();
	wing_detail_->state = Wing_Detail::WING_EXP;
	wing_detail_->lv = exp_lv;
	wing_detail_->is_show = Wing_Detail::SHOW_ON;

	wing_detail_->active_time = Time_Value::gettimeofday();
	wing_detail_->detail_change();

	refresh_wing_attr(0, wing_detail_->lv);
	player_self()->refresh_avatar();

	return 0;
}

int Wing::handle_get_wing_item(void) {
	// 条件检查，是否完成任务
	if (!player_self()->is_last_magic_wing_task_submit()) return ERROR_TASK_NOT_COMLETED;
	Item_Detail item_detail(WING_ITEM_ID, 1, Item_Detail::BIND);

	int ret = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_detail,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_WING));
	if (0 != ret) return ret;

	MSG_50105011 msg;
	msg.result = 0;

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Wing::handle_active_wing(void) {

	if (wing_detail_->state == Wing_Detail::WING_ACTIVE) return ERROR_INNER_SERVER;

	Id_Amount item(WING_ITEM_ID, 1);
	int ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, item);
	if (ret != 0) return ret;

	player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item, Lose_Item_DM_Info(ITEM_LOSE_ACTIVE_WING));

	int old_lv = wing_detail_->lv;

	wing_detail_->state = Wing_Detail::WING_ACTIVE;
	wing_detail_->lv = 1;
	wing_detail_->is_show = Wing_Detail::Wing_Show_State::SHOW_ON;
	wing_detail_->active_time = Time_Value::gettimeofday();
	wing_detail_->detail_change();

	MSG_50105012 msg;
	msg.result = 0;

	THIS_SEND_TO_CLIENT(msg);

	handle_get_wing_info();

	refresh_wing_attr(old_lv, wing_detail_->lv);
	player_self()->refresh_avatar();

	player_self()->create_ann_wing_active();

	//task listen
	player_self()->task_listen_activate_wing();
	player_self()->task_listen_magic_wing_active_to_clear_task();

	return 0;
}

int Wing::handle_get_wing_info(void) {

	MSG_50105013 msg;
	if (0 == wing_detail_) {
		msg.lv = 0;
	} else {
		msg.lv = wing_detail_->lv;
		msg.progress = wing_detail_->progress;
		msg.use_times = wing_detail_->times;
		msg.is_show = wing_detail_->is_show;
		msg.state = wing_detail_->state;
		msg.ret_reward_stage = wing_detail_->got_reward_stage;

		if (Wing_Detail::WING_ACTIVE == wing_detail_->state) {
			int secs = CONFIG_CACHE_PUBLIC->get_wing_experience_ret_reward_time() * 60;
			Time_Value expired_time = wing_detail_->active_time + Time_Value(secs);
			const Time_Value &now = Time_Value::gettimeofday();
			if (expired_time > now) {
				msg.secs = (expired_time - now).sec();
			}
		}
	}
	int cur_lv = msg.lv;
	int next_lv = msg.lv + 1;
	if (0 == msg.lv) {
		cur_lv = 1;
		next_lv = 2;
	}

	msg.cur_score = get_wing_stage_score(cur_lv);
	msg.next_score = get_wing_stage_score(next_lv);

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Wing::handle_upgrade_wing_once(int is_auto_buy) {

	if (0 == wing_detail_) return ERROR_INNER_SERVER;
	if (Wing_Detail::WING_ACTIVE != wing_detail_->state) return ERROR_INNER_SERVER;
	if (0 == wing_detail_->lv) return ERROR_INNER_SERVER;

	const Wing_Cfg_Map& wing_cfg_map = CONFIG_CACHE_WING->get_wing_config_map();
	int max_lv = wing_cfg_map.size();
	int old_lv = wing_detail_->lv;

	if (old_lv >= max_lv) return ERROR_INNER_SERVER;

	const Wing_Config *cfg = CONFIG_CACHE_WING->get_wing_config(old_lv);
	if (0 == cfg) return ERROR_INNER_SERVER;

	int ret = handle_upgrade_wing(*cfg, is_auto_buy);
	if (0 != ret) return ret;

	int new_lv = wing_detail_->lv;

	MSG_50105014 msg;
	msg.result = 0;
	msg.lv = wing_detail_->lv;
	msg.progress = wing_detail_->progress;
	msg.use_times = wing_detail_->times;
	msg.cur_score = get_wing_stage_score(wing_detail_->lv);
	msg.next_score = get_wing_stage_score(wing_detail_->lv + 1);

	THIS_SEND_TO_CLIENT(msg);

	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_WING_TRAIN);

	if (old_lv == new_lv) return 0;
	refresh_wing_attr(old_lv, new_lv);
	player_self()->refresh_avatar();

	return 0;
}

int Wing::handle_upgrade_wing(const Wing_Config &cfg, int is_auto_buy) {
	Money_Sub_Info money_sub_info(Money_Sub_Type(COPPER_ONLY), cfg.gold_cost, MONEY_SUB_UPGRADE_WING);
	int ret = player_self()->pack_try_sub_money(money_sub_info);
	if (0 != ret) return ret;

	int item_amount = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, cfg.goods_cost.val_1);
	if (item_amount < cfg.goods_cost.val_2 && 0== is_auto_buy) {
		MSG_82100008 msg;
		msg.item_ids.push_back(cfg.goods_cost.val_1);
		THIS_SEND_TO_CLIENT(msg);
		return ERROR_ITEM_NOT_ENOUGH;
	}
	item_amount = item_amount > cfg.goods_cost.val_2 ?
			cfg.goods_cost.val_2 : item_amount;

	if (0 != item_amount) {
		Id_Amount pack_item(cfg.goods_cost.val_1, item_amount);
		ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, pack_item);
		if (0 != ret) return ret;
	}
	if (item_amount < cfg.goods_cost.val_2) {
		int amount_need_to_buy = cfg.goods_cost.val_2 - item_amount;
		ret = player_self()->req_fast_buy_item_in_mall_tabs(cfg.goods_cost.val_1, amount_need_to_buy, 1);
		if (0 != ret) return ret;
	}
	if (item_amount > 0) {
		Id_Amount pack_item(cfg.goods_cost.val_1, item_amount);
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, pack_item,
				Lose_Item_DM_Info(ITEM_LOSE_UPGRADE_WING));
	}
	player_self()->pack_sub_money(money_sub_info);

	int progress = wing_detail_->progress;
	int rate = wing_detail_->rate;

	if (handle_upgrade_wing(progress, rate, cfg)) {
		wing_detail_->lv += 1;
		wing_detail_->rate = 0;
		wing_detail_->progress = 0;
		wing_detail_->times = 0;
	} else {
		wing_detail_->rate = rate;
		wing_detail_->progress = progress;
		wing_detail_->times += 1;
	}
	wing_detail_->last_time = Time_Value::gettimeofday();
	wing_detail_->detail_change();
	if(item_amount) {
		player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_WING_USE_DAN, item_amount);
	}
	return 0;
}

int Wing::handle_upgrade_wing_auto(int is_auto_buy) {

	if (0 == wing_detail_) return ERROR_INNER_SERVER;
	if (Wing_Detail::WING_ACTIVE != wing_detail_->state) return ERROR_INNER_SERVER;
	if (0 == wing_detail_->lv) return ERROR_INNER_SERVER;
	const Wing_Cfg_Map &wing_cfg_map = CONFIG_CACHE_WING->get_wing_config_map();

	int max_lv = wing_cfg_map.size();
	int old_lv = wing_detail_->lv;
	if (old_lv >= max_lv) return ERROR_INNER_SERVER;

//	int new_lv = old_lv;
	const Wing_Config *cfg = CONFIG_CACHE_WING->get_wing_config(old_lv);
	if (0 == cfg) return ERROR_INNER_SERVER;

	int handle_times = 0;
	while(handle_times < 5000) {
		int ret = handle_upgrade_wing(*cfg, is_auto_buy);
		if (0 == ret) { // success
			if (wing_detail_->lv > old_lv) break;
		} else { // fail
			if (handle_times > 0) break;
			if (ERROR_ITEM_NOT_ENOUGH != ret) return ret;
			MSG_82100008 msg;
			msg.item_ids.push_back(cfg->goods_cost.val_1);
			THIS_SEND_TO_CLIENT(msg);
			return ret;
		}
		++handle_times;
	}

//	int price = MALL_MANAGER->find_goods_price_by_tabs_and_id(2, cfg->goods_cost.val_1);

//	Money_Sub_Info money_sub_info(Money_Sub_Type(COPPER_ONLY), 0, MONEY_SUB_UPGRADE_WING);
//	Money_Sub_Info auto_use_diamond(Money_Sub_Type(BIND_GOLD_FIRST), 0, MONEY_SUB_UPGRADE_WING);
//	int item_amount = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, cfg->goods_cost.val_1);
//	int need_goods_num = 0;
//	int progress = wing_detail_->progress;
//	int rate = wing_detail_->rate;
//	int handle_times = 0;
//	int ret_val = 0;
//	int is_upgraded = false;
//
//	while (handle_times < 5000) {
//
//		Money_Sub_Info money_sub_info()
//		money_sub_info.nums = cfg->gold_cost * (handle_times + 1);
//		ret_val = player_self()->pack_try_sub_money(money_sub_info);
//		if (0 != ret_val) break;
//		need_goods_num = cfg->goods_cost.val_2 * (handle_times + 1);
//		if (item_amount < need_goods_num) {
//			if (0 == is_auto_buy || 0 == price) {
//				ret_val = ERROR_ITEM_NOT_ENOUGH;
//				break;
//			}
//			auto_use_diamond.nums = (need_goods_num - item_amount) * price;
//			ret_val = player_self()->pack_try_sub_money(auto_use_diamond);
//			if (0 != ret_val) break;
//		}
//
//		is_upgraded = handle_upgrade_wing(progress, rate, *cfg);
//		++handle_times;
//		if (is_upgraded) break;
//	}
//
//	if (handle_times == 0) {
//		if (ERROR_ITEM_NOT_ENOUGH == ret_val) {
//			MSG_82100008 msg;
//			msg.item_ids.push_back(cfg->goods_cost.val_1);
//			THIS_SEND_TO_CLIENT(msg);
//		}
//		return ret_val;
//	}
//
//	money_sub_info.nums = cfg->gold_cost * handle_times;
//	player_self()->pack_sub_money(money_sub_info);
//	auto_use_diamond.nums = 0;
//	need_goods_num = cfg->goods_cost.val_2 * handle_times;
//	if (item_amount < need_goods_num) {
//		auto_use_diamond.nums = (need_goods_num - item_amount) * price;
//		need_goods_num = item_amount;
//	}
//	if (need_goods_num > 0) {
//		Id_Amount item(cfg->goods_cost.val_1, need_goods_num);
//		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item, Lose_Item_DM_Info(Pack::ITEM_LOSE_UPGRADE_WING));
//	}
//	if (0 != is_auto_buy && auto_use_diamond.nums > 0) {
//		player_self()->pack_sub_money(auto_use_diamond);
//	}
//
//	if (is_upgraded) {
//		wing_detail_->lv += 1;
//		wing_detail_->rate = 0;
//		wing_detail_->progress = 0;
//		wing_detail_->times = 0;
//	} else {
//		wing_detail_->rate = rate;
//		wing_detail_->progress = progress;
//		wing_detail_->times += handle_times;
//	}
	wing_detail_->last_time = Time_Value::gettimeofday();
//	wing_detail_->detail_change();
//
//	new_lv = wing_detail_->lv;

	MSG_50105015 msg;
	msg.result = 0;
	msg.lv = wing_detail_->lv;
	msg.progress = wing_detail_->progress;
	msg.use_times = wing_detail_->times;
	msg.cur_score = get_wing_stage_score(wing_detail_->lv);
	msg.next_score = get_wing_stage_score(wing_detail_->lv + 1);

	THIS_SEND_TO_CLIENT(msg);
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_WING_TRAIN, handle_times);

	if (wing_detail_->lv == old_lv) return 0;
	refresh_wing_attr(old_lv, wing_detail_->lv);
	player_self()->refresh_avatar();

	return 0;
}

int Wing::handle_change_wing_show_state(int state) {

	if (!is_wing_normal()) return ERROR_INNER_SERVER;
	if (0 == wing_detail_->lv) return ERROR_INNER_SERVER;
	if (Wing_Detail::SHOW_OFF != state && Wing_Detail::SHOW_ON != state)
		return ERROR_INNER_SERVER;

	if (state == wing_detail_->is_show) return ERROR_SAME_WEAR_STATE;

	wing_detail_->is_show = state;

	MSG_50105016 msg;
	msg.result = 0;
	msg.state = state;

	THIS_SEND_TO_CLIENT(msg);

	player_self()->refresh_avatar();

	return 0;
}

bool Wing::handle_upgrade_wing (int &progress, int &rate, const Wing_Config& cfg) {
	int delta = cfg.rate_min_max.val_2 - cfg.rate_min_max.val_1;
	int add_rate = cfg.rate_min_max.val_1;
	if (delta > 0) {
		add_rate += random() % (delta+1);
	}

	progress += add_rate;
	progress = progress > cfg.rate_show_max ? cfg.rate_show_max : progress;

	if (progress < cfg.rate_show_min) return false;
	if (progress >= cfg.rate_show_max)  return true;

	rate = rate < cfg.rate_base ? cfg.rate_base : rate;
	int rand = random() % 1000;
	if (rand >= rate) return false;

	rate += cfg.rate_add_value;
	return true;
}

int Wing::handle_get_ret_reward(int stage) {

	if (0 == wing_detail_) return ERROR_INNER_SERVER;
	if (Wing_Detail::WING_ACTIVE != wing_detail_->state) return ERROR_WING_NOT_ACTIVE;
	if (stage < wing_detail_->got_reward_stage) return ERROR_HAS_GOT_REWARD;

	int secs = CONFIG_CACHE_PUBLIC->get_wing_experience_ret_reward_time() * 60;
	const Time_Value &now = Time_Value::gettimeofday();
	Time_Value expired_time = wing_detail_->active_time + Time_Value(secs);
	if (now >= expired_time) return ERROR_WING_ACTIVITY_EXPIRED;		// 活动过期

	if (0 == wing_detail_->lv) return ERROR_INNER_SERVER;
	const Wing_Config* cfg = CONFIG_CACHE_WING->get_wing_config(wing_detail_->lv);
	if (0 == cfg) return ERROR_CONFIG_NOT_EXIST;
	if (cfg->show_stage < stage) return ERROR_WING_STAGE_LIMIT;

	for (int i=wing_detail_->got_reward_stage+1; i <stage; ++i) {
		if (0 != CONFIG_CACHE_WING->get_wing_ret_reward(i))
			return ERROR_INNER_SERVER;
	}

	const Int_Int_Vec *reward_vec = CONFIG_CACHE_WING->get_wing_ret_reward(stage);
	if (0 == reward_vec) return ERROR_CONFIG_NOT_EXIST;

	MSG_81000102 item_msg;
	Item_Vec item_vec;
	Item_Detail detail;
	Item_Basic_Info item_basic;
	for (Int_Int_Vec::const_iterator it = reward_vec->begin(); it != reward_vec->end(); ++it) {
		detail.reset();
		item_basic.reset();
		detail.id_ = it->val_1;
		detail.amount_ = it->val_2;
		detail.bind_ = Item_Detail::BIND;
		item_vec.push_back(detail);
		detail.set_item_basic_info(item_basic);
		item_msg.item_info.push_back(item_basic);
	}

	int ret = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_vec,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_WING_RET_REWARD));
	if (0 != ret) return ret;

	wing_detail_->got_reward_stage = stage;

	MSG_50105017 msg;
	msg.result = 0;
	msg.stage = stage;

	THIS_SEND_TO_CLIENT(msg);
	THIS_SEND_TO_CLIENT(item_msg);

	return 0;
}

void Wing::refresh_wing_avatar() {
	if (0 == wing_detail_ || 0 == wing_detail_->lv) return;
	if (wing_detail_->get_wing_state() == Wing_Detail::Wing_Show_State::SHOW_OFF)
		return;

	const Wing_Config* cfg = CONFIG_CACHE_WING->get_wing_config(wing_detail_->lv);
	if (0 == cfg) return;

	Wing_Avatar_Map::const_iterator find_it = cfg->wing_avatar_map.find(player_self()->career());
	if (find_it == cfg->wing_avatar_map.end()) return;
	Int_Int_Map::const_iterator find_it_1 = find_it->second.find(player_self()->gender());
	if (find_it_1 == find_it->second.end()) return;
	if (find_it_1->second <= 0) return;
	player_self()->set_fashion_id(find_it_1->second);

}

void Wing::refresh_wing_attr(int pre_lv, int pos_lv) {
	typedef boost::unordered_map<int, Prop_Setter> Prop_Setter_Map;
	if (0 == wing_detail_ || 0 == wing_detail_->lv) return;
	const Wing_Config* pre_cfg = CONFIG_CACHE_WING->get_wing_config(pre_lv);
	const Wing_Config* pos_cfg = CONFIG_CACHE_WING->get_wing_config(pos_lv);

	Prop_Setter_Map s_map;
	Prop_Setter_Vec vec;
	Prop_Setter setter;
	if (0 != pre_cfg) {
		for (Wing_Prop_Value_Map::const_iterator it = pre_cfg->player_prop_map.begin();
				it != pre_cfg->player_prop_map.end(); ++it) {
			setter.reset();
			setter.addi_type = Addition_Type::AT_WING;
			setter.operate = O_ADD;
			setter.prop_type = Property_Type(it->first);
			setter.basic = 0.0;
			setter.fixed = -it->second.fix;
			setter.percent = -it->second.percent;
			s_map[it->first] = setter;
		}
		std::vector<Skill_Info> skill_vec;
		Skill_Info skill;
		for (Int_Vec::const_iterator it = pre_cfg->skill_vec.begin();
				it != pre_cfg->skill_vec.end(); ++it) {
			skill.skill_id = *it;
			skill.skill_lv = 1;
			skill_vec.push_back(skill);
		}
		if (!skill_vec.empty()) {
			player_self()->remove_be_skills(skill_vec);
		}
	}

	if (0 != pos_cfg) {
		std::vector<Skill_Info> skill_vec;
		Skill_Info skill;
		for (Int_Vec::const_iterator it = pos_cfg->skill_vec.begin();
				it != pos_cfg->skill_vec.end(); ++it) {
			skill.reset();
			skill.skill_id = *it;
			skill.skill_lv = 1;
			skill_vec.push_back(skill);
		}

		if (!skill_vec.empty()) {
			player_self()->add_be_skills(skill_vec);
		}

		for (Wing_Prop_Value_Map::const_iterator it= pos_cfg->player_prop_map.begin();
				it != pos_cfg->player_prop_map.end(); ++it) {
			Prop_Setter_Map::iterator find_it = s_map.find(it->first);
			if (find_it != s_map.end()) {
				find_it->second.fixed += it->second.fix;
				find_it->second.percent += it->second.percent;
			} else {
				setter.reset();
				setter.addi_type = Addition_Type::AT_WING;
				setter.operate = O_ADD;
				setter.prop_type = Property_Type(it->first);
				setter.basic = 0.0;
				setter.fixed = it->second.fix;
				setter.percent = it->second.percent;
				s_map[it->first] = setter;
			}
		}
	}

	for (Prop_Setter_Map::iterator it = s_map.begin(); it != s_map.end(); ++it) {
		vec.push_back(it->second);
	}

	if (0 != pre_cfg || 0 != pos_cfg) {
		player_self()->fight_prop_modify(vec, 223);
		player_self()->heroer_prop_refresh();
	}

	if (0 != pre_cfg && pre_cfg->show_stage != pos_cfg->show_stage) {
		player_self()->req_active_content_tip_num();
	}

	if (pos_cfg->is_cast == 1) {
		player_self()->create_ann_wing_upgrade(pos_cfg->show_stage);
	}
}

void Wing::get_wing_props_for_hero(Prop_Setter_Vec &vec) {
	if (0 == wing_detail_ || 0 == wing_detail_->lv) return;
	const Wing_Config* cfg = CONFIG_CACHE_WING->get_wing_config(wing_detail_->lv);
	if (0 == cfg) return;
	Prop_Setter setter;
	for(Wing_Prop_Value_Map::const_iterator it = cfg->hero_prop_map.begin();
			it != cfg->hero_prop_map.end(); ++it) {
		setter.reset();
		setter.addi_type = Addition_Type::AT_WING;
		setter.operate = O_SET;
		setter.prop_type = Property_Type(it->first);
		setter.fixed = it->second.fix;
		setter.percent = it->second.percent;
		vec.push_back(setter);
	}
}

int Wing::get_wing_stage_score(int lv) {
	const Wing_Config* cfg = CONFIG_CACHE_WING->get_wing_config(lv);
	if (0 == cfg) return 0;
	int role_lv = player_self()->fighter_detail().level;
	double ref_force = CONFIG_CACHE_ROLE->get_ref_force(role_lv);

	double percent = 0.0;
	double fixed = 0.0;

	for (Wing_Prop_Value_Map::const_iterator it = cfg->player_prop_map.begin();
			it != cfg->player_prop_map.end(); ++it) {
		percent += it->second.percent;
		fixed += it->second.fix;
	}

	return static_cast<int>(floor(ref_force * percent + fixed));
}

bool Wing::is_wing_normal(void){

	if (0 == wing_detail_) {
		MSG_USER("wing data maybe not load properly, role_id=%ld", role_id());
		return false;
	}
	// 未激活状态
	if (Wing_Detail::WING_NOT_ACTIVE == wing_detail_->state) return false;
	// 体验状态，做过期检查
	if (Wing_Detail::WING_EXP == wing_detail_->state) {
		// 体验过期检查
		const int secs = CONFIG_CACHE_PUBLIC->get_wing_experience_mins() * 60;
		Time_Value expired_time = wing_detail_->active_time + Time_Value(secs);
		// 未过期
		if (expired_time > Time_Value::gettimeofday())  return true;
		// 过期，重置等级为0
		if (0 != wing_detail_->lv) {
			wing_detail_->lv = 0;
			wing_detail_->is_show = Wing_Detail::SHOW_OFF;
			player_self()->player_try_time_out(TRY_MAGIC_WING_TIMEOUT);
		}
		return false;
	}

	return true;
}
