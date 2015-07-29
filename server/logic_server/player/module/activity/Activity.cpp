/*
 * Activity.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include <activity/Activity.h>
#include <activity/Activity_Struct.h>
#include "Logic_Module_Head.h"
#include "Config_Cache_Activity.h"
#include "Err_Code.h"
#include "Activity_Def.h"
#include "pack/Pack_Def.h"
#include "Platform_Activity_Struct.h"
#include "active/Active_Content_Config.h"
#include "Msg_Active_Struct.h"
#include "pack/Pack_Struct.h"
#include "fighter/Fighter_Def.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "active/Active_Manager.h"
#include "active/Active_Def.h"

Activity::Activity(void) {
	reset();
}

Activity::~Activity() {
	// TODO Auto-generated destructor stub
}

void Activity::reset(void) {
	activity_detail_ = 0;
}

int Activity::create_init(Activity_Detail &detail) {

	return 0;
}

void Activity::load_detail(Activity_Detail &detail) {
	activity_detail_ = &detail;
}

void Activity::save_detail(Activity_Detail &detail) {
	if (activity_detail().is_change) {
		detail = activity_detail();
		activity_detail_->is_change = false;
	}
}

int Activity::trigger_daily_zero(const Time_Value &now) {
	activity_detail_->is_rec_r2_gift = false;
	activity_detail_->detail_change();
	return 0;
}

int Activity::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Activity::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Activity::move_scene_completed(void) {

	return 0;
}

int Activity::client_ready_now(void) {

	return 0;
}

void Activity::module_init(void) {

}

void Activity::sign_in(void) {
	//R2情人节符石活动送礼包
	if(activity_detail_->is_rec_r2_gift == false && ACTIVE_MANAGER->get_active_up_day(ACTION_ACTIVE_RUNE_STONE_UP)) {
		LOGIC_PUBLIC->send_mail(2011, player_self()->role_id());
		activity_detail_->is_rec_r2_gift = true;
		activity_detail_->detail_change();
	}
}

void Activity::sign_out(void) {

}

void Activity::trusteeship_sign_out(void) {

}

int Activity::sign_in_repeat(void) {

	return 0;
}

void Activity::set_37_vip_level(int32_t level) {
	activity_detail_->vip37_level = level;
	activity_detail_->detail_change();
}

int Activity::req_open_37vip_level_reward(int32_t level) {
	set_37_vip_level(level);
	const Vip37_Conf_Map  &conf = CONFIG_CACHE_ACTIVITY->get_vip_config();
	MSG_50112001 msg;
	Uint_Uint u;
	for(Vip37_Conf_Map::const_iterator it = conf.begin(); it != conf.end(); ++it) {
		u.reset();
		u.val_1 = it->first;
		if((uint32_t)activity_detail_->vip37_level >= it->second.vip_lv) {
			if(activity_detail_->vip37_receive_id.find(it->second.id) == activity_detail_->vip37_receive_id.end()) {
				u.val_2 = COMMON_STATUS_CAN_RECEIVE;
			} else {
				u.val_2 = COMMON_STATUS_HAS_RECEIVE;
			}
		} else {
			u.val_2 = COMMON_STATUS_NOT_RECEIVE;
		}
		msg.status.push_back(u);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Activity::req_receive_37vip_level_reward(uint32_t id) {
	const Vip37_Conf_Map  &conf = CONFIG_CACHE_ACTIVITY->get_vip_config();
	Vip37_Conf_Map::const_iterator iter = conf.find(id);
	if(iter == conf.end()) {
		return ERROR_REWARD_NOT_EXIST;
	}
	if(activity_detail_->vip37_receive_id.find(iter->second.id) != activity_detail_->vip37_receive_id.end()) {
		return ERROR_REWARD_HAS_RECEIVE;
	}
	if(iter->second.vip_lv > (uint32_t)activity_detail_->vip37_level) {
		return ERROR_VIP_LEVEL_LIMIT;
	}
	int res = activity_receive_reward(iter->second.reward);
	if(res) {
		return res;
	}
	activity_detail_->vip37_receive_id.insert(id);
	activity_detail_->detail_change();
	MSG_50112000 msg;
	msg.id = id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Activity::activity_receive_reward(const Active_Reward_Info_Config &reward) {
	//发放奖励
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if (reward.item.size()) {
		Item_Vec item;
		for (Active_Item_Vec::const_iterator item_it = reward.item.begin();
				item_it != reward.item.end(); ++item_it) {
			Item_Detail it_em(item_it->item_id, item_it->num, Item_Detail::BIND);
			item.push_back(it_em);
			Item_Basic_Info ibi;
			it_em.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(Pack::ITEM_GAIN_37VIP_REWARD));
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (reward.money.size()) {
		Money_Add_List money;
		money.clear();
		for (Active_Money_Vec::const_iterator money_it = reward.money.begin();
				money_it != reward.money.end(); ++money_it) {
			money.push_back(Money_Add_Info(Pack::Money_Type(money_it->money_id), money_it->num, Money_DM_Info(Pack::MONEY_ADD_37VIP_REWARD)));
			acv_msg.property.push_back(
					Property(money_it->money_id, money_it->num));
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (reward.exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(reward.exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}
