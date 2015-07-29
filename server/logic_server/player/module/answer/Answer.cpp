/*
 * Answer.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "Answer.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Answer.h"
#include "pack/Pack_Def.h"
#include "Msg_Active_Struct.h"
#include "Answer_Def.h"
#include "Err_Code.h"
#include "pack/Pack_Struct.h"
#include "fighter/Fighter_Def.h"
#include "Logic_Player.h"
#include "helper/Helper_Def.h"
#include "Config_Cache_Scene.h"
#include "Scene_Def.h"
#include "fighter/Logic_Fighter_Struct.h"

Answer::Answer(void) {
	reset();
}

Answer::~Answer() {
	// TODO Auto-generated destructor stub
}

void Answer::reset(void) {

}
//
//int Answer::create_init(Answer_Detail &detail) {
//
//	return 0;
//}
//
//void Answer::load_detail(Answer_Detail &detail) {
//
//}
//
//void Answer::save_detail(Answer_Detail &detail) {
//
//}

int Answer::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Answer::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Answer::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Answer::move_scene_completed(void) {
	const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if(cfg && cfg->type == Answer_Type) {
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_ANSWER);
	}
	return 0;
}

int Answer::client_ready_now(void) {

	return 0;
}

void Answer::module_init(void) {

}

void Answer::sign_in(void) {

}

void Answer::sign_out(void) {

}

void Answer::trusteeship_sign_out(void) {

}

int Answer::data_channle(Block_Buffer& buf) {
	uint8_t type = 0;
	buf.read_uint8(type);
	switch(type) {
		case ANSWER_DATA_CHANNLE_TYPE_SL_GET_INTEGRAL_REWARD: {
			uint32_t sorce = 0;
			buf.read_uint32(sorce);
			return get_integral_reward(sorce);
		}
	}
	return 0;
}

int Answer::get_integral_reward(uint32_t sorce) {
	const Answer_Integral_Reward_Config *reward_conf = CONFIG_CACHE_ANSWER->get_integral_reward_by_level(player_self()->level());
	if(reward_conf) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		//item
		if(!reward_conf->reward.item.empty()) {
			Item_Vec item_list;
			for(Answer_Reward_Item_Vec::const_iterator item_it = reward_conf->reward.item.begin(); item_it != reward_conf->reward.item.end(); ++item_it) {
					Item_Detail item(item_it->id, item_it->num*sorce, Item_Detail::BIND);
					item_list.push_back(item);
					Item_Basic_Info ibi;
					item.set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
			}
			if(!item_list.empty()) {
				int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_ANSWER));
				if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
					return result;
				}
			}
		}
		//money
		if(!reward_conf->reward.money.empty()) {
			Money_Add_List money;
			money.clear();
			for(Answer_Reward_Money_Vec::const_iterator money_it = reward_conf->reward.money.begin(); money_it != reward_conf->reward.money.end(); ++money_it) {
					money.push_back(Money_Add_Info(Pack::Money_Type(money_it->id), money_it->num*sorce, Money_DM_Info(Pack::MONEY_ADD_ANSWER)));
					acv_msg.property.push_back(Property(money_it->id, money_it->num*sorce));
			}
			if(!money.empty()) {
				player_self()->pack_add_money(money);
			}
		}
		//exp
		uint32_t exp = reward_conf->reward.exp * sorce;
		if(exp) {
			Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, true);
			if (0 == exp_all.result) {
				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
				acv_msg.vip_exp = exp_all.vip_exp;
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}
		if(!acv_msg.property.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}
	return 0;
}

int Answer::sign_in_repeat(void) {

	return 0;
}


int Answer::get_rank_reward(uint16_t rank) {
	const Answer_Rank_Reward_Config_List &reward_conf_list = CONFIG_CACHE_ANSWER->get_rank_reward_list();
	const double mod =  CONFIG_CACHE_ANSWER->get_modulus_by_level(player_self()->level());
	const double mod_item =  CONFIG_CACHE_ANSWER->get_modulus_item_by_level(player_self()->level());
	const Answer_Rank_Reward_Config *reward_conf = 0;
	for(Answer_Rank_Reward_Config_List::const_iterator it = reward_conf_list.begin(); it != reward_conf_list.end();  ++it) {
		if(it->end_rank) {
			if(rank >= it->start_rank && rank <= it->end_rank) {
				reward_conf = &(*it);
				break;
			}
		} else {
			if(rank >= it->start_rank) {
				reward_conf = &(*it);
				break;
			}
		}
	}

	if(reward_conf) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		//item
		if(!reward_conf->reward.item.empty()) {
			Item_Vec item_list;
			for(Answer_Reward_Item_Vec::const_iterator item_it = reward_conf->reward.item.begin(); item_it != reward_conf->reward.item.end(); ++item_it) {
					Item_Detail item(item_it->id, item_it->num*mod_item, Item_Detail::BIND);
					item_list.push_back(item);
					Item_Basic_Info ibi;
					item.set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
			}
			if(!item_list.empty()) {
				int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_ANSWER));
				if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
					return result;
				}
			}
		}
		//money
		if(!reward_conf->reward.money.empty()) {
			Money_Add_List money;
			money.clear();
			for(Answer_Reward_Money_Vec::const_iterator money_it = reward_conf->reward.money.begin(); money_it != reward_conf->reward.money.end(); ++money_it) {
					money.push_back(Money_Add_Info(Pack::Money_Type(money_it->id), money_it->num*mod, Money_DM_Info(Pack::MONEY_ADD_ANSWER)));
					acv_msg.property.push_back(Property(money_it->id, money_it->num*mod));
			}
			if(!money.empty()) {
				player_self()->pack_add_money(money);
			}
		}
		//exp
		uint32_t exp = reward_conf->reward.exp * mod;
		if(exp) {
			Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, true);
			if (0 == exp_all.result) {
				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
				acv_msg.vip_exp = exp_all.vip_exp;
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}
		if(!acv_msg.property.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}

	return 0;
}
