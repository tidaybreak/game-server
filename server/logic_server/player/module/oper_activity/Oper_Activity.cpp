/*
 * Oper_Activity.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include <oper_activity/Oper_Activity.h>
#include <oper_activity/Oper_Activity_Struct.h>
#include <oper_activity/Oper_Activity_Def.h>
#include "Logic_Module_Head.h"
#include "Oper_Activity_Manager.h"
#include "Config_Cache_Oper_Activity.h"
#include "Msg_Oper_Activity_Struct.h"
#include "Logic_Player.h"
#include "pack/Pack_Struct.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "Logic_Public.h"
#include "mount/Mount_Struct.h"
#include "wing/Wing_Struct.h"
#include "Config_Cache_Wing.h"

Oper_Activity::Oper_Activity(void) {
	reset();
}

Oper_Activity::~Oper_Activity() {
	// TODO Auto-generated destructor stub
}

void Oper_Activity::reset(void) {
	oper_activity_detail_ = 0;
}

int Oper_Activity::create_init(Oper_Activity_Detail &detail) {

	return 0;
}

void Oper_Activity::load_detail(Oper_Activity_Detail &detail) {
	oper_activity_detail_ = &detail;
}

void Oper_Activity::save_detail(Oper_Activity_Detail &detail) {
	if (oper_activity_detail().is_change) {
		detail = oper_activity_detail();
		oper_activity_detail_->is_change = false;
	}
}

int Oper_Activity::trigger_daily_zero(const Time_Value &now) {
	oper_activity_login_day();
	return 0;
}

int Oper_Activity::trigger_daily_six(const Time_Value &now) {
	oper_activity_daily_refresh();
	oper_activity_login_day();
	return 0;
}

int Oper_Activity::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Oper_Activity::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Oper_Activity::move_scene_completed(void) {

	return 0;
}

int Oper_Activity::client_ready_now(void) {
	oper_activity_check_reset_on_restart_act();
	const UInt_Set &effect_actid_set = CONFIG_CACHE_OPER_ACTIVITY->get_has_effect_id();
	for(UInt_Set::const_iterator eait = effect_actid_set.begin(); eait != effect_actid_set.end(); ++eait) {
		const Oper_Activity_Project_Config *conf = CONFIG_CACHE_OPER_ACTIVITY->get_project_config_by_id(*eait);
		if(conf) {
			//if(conf->append_effect.size() != oper_activity_detail_->actid_append_effect[conf->act_id].size()) {//影响特殊情况的热更配置; 但会带来效率
				for(Oper_Activity_Append_Effect_Config_Map::const_iterator ae_iter = conf->append_effect.begin(); ae_iter != conf->append_effect.end(); ++ae_iter) {//遍历特殊效果
						player_self()->oper_activity_append_effect(*conf, ae_iter->second);
				}
			//}
		}
	}
	oper_activity_daily_refresh();
	return 0;
}

void Oper_Activity::module_init(void) {

}

void Oper_Activity::sign_in(void) {
	oper_activity_login_day();
}

void Oper_Activity::oper_activity_login_day() {
	oper_activity_check_reset_on_restart_act();
	player_self()->oper_activity_calc_login_day(OPER_ACTIVITY_ITEM_RECHARGE_NUM_AND_LOGIN_DAILY_NUM, 1, OPER_ACTIVITY_LISTEN_TYPE_EXT1);
	player_self()->oper_activity_calc_login_day(OPER_ACTIVITY_ITEM_LOGIN_DAILY_NUM);
}

void Oper_Activity::sign_out(void) {

}

void Oper_Activity::trusteeship_sign_out(void) {

}

int Oper_Activity::sign_in_repeat(void) {

	return 0;
}

void Oper_Activity::oper_activity_close_append_effect_buff_type(const Oper_Activity_Project_Config &conf, const Oper_Activity_Append_Effect_Config &ae_config) {
	for(Effect_Argv_Prop_Map::const_iterator prop_it = ae_config.prop_map.begin(); prop_it != ae_config.prop_map.end(); ++prop_it) {
		oper_activity_close_append_effect_buff_type_status_id(prop_it->first);
	}
}

void Oper_Activity::oper_activity_append_effect(const Oper_Activity_Project_Config &conf, const Oper_Activity_Append_Effect_Config &ae_config) {
	oper_activity_check_reset_on_restart_act();
	if(oper_activity_detail_->actid_append_effect[conf.act_id].find(ae_config.ae_id) != oper_activity_detail_->actid_append_effect[conf.act_id].end()) return;
	switch(ae_config.type) {
		case OPER_ACTIVITY_APPEND_BUFF_EFFECT: {
			oper_activity_append_effect_buff_type(conf, ae_config);
			break;
		}
	}
}

void Oper_Activity::oper_activity_close_append_effect_buff_type_status_id(const uint32_t status_id) {
	if(player_self()->has_status_id(status_id)) {
		player_self()->logic_del_status(status_id);
	}
}

void Oper_Activity::oper_activity_append_effect_buff_type(const Oper_Activity_Project_Config &conf, const Oper_Activity_Append_Effect_Config &ae_config) {
	Time_Value now = Time_Value::gettimeofday();
	const Time_Value &prop_time = OPER_ACTIVE_MANAGER->get_item_end_time(conf.act_id);
	if(prop_time.sec() <= now.sec()) return;

	UInt_Set::iterator bit = oper_activity_detail_->actid_append_effect[conf.act_id].find(ae_config.ae_id);
	if(bit == oper_activity_detail_->actid_append_effect[conf.act_id].end()) {
		oper_activity_detail_->actid_append_effect[conf.act_id].insert(ae_config.ae_id);
		//设置状态
		for(Effect_Argv_Prop_Map::const_iterator prop_it = ae_config.prop_map.begin(); prop_it != ae_config.prop_map.end(); ++prop_it) {
			oper_activity_close_append_effect_buff_type_status_id(prop_it->first);
			Status_Effect_Argv argv;
			argv.caster_id = role_id();
			argv.status_id = prop_it->first;
			argv.last_time_ms = (int64_t)(prop_time.msec() - now.msec());
			argv.status_from = 1;
			argv.is_fight_buff = false;
			argv.data = prop_it->second;
			player_self()->logic_insert_status(argv);
			player_self()->hero_insert_status(argv);
			player_self()->heroer_prop_refresh(player_self()->heroer_detail().fighter_index, true, true);
		}
		this->oper_activity_detail_->detail_change();
	}
}

int Oper_Activity::req_oper_activity_content_info(void) {
	//oper_activity_check_reset_on_restart_act();
	oper_activity_login_day();
	const Oper_Activity_Project_Config_Map &project_map = CONFIG_CACHE_OPER_ACTIVITY->get_project_map();
	MSG_50117000 msg;
	msg.reset();
	for(Oper_Activity_Project_Config_Map::const_iterator it = project_map.begin(); it != project_map.end(); ++it) {
		if(!OPER_ACTIVE_MANAGER->item_is_open(it->second.act_id)) continue;//活动还没开启
		if(it->second.not_show) continue;
		//if(it->second.item_line.empty()) continue;
		Oper_Activity_Item_Info info;
		oper_activity_proect_content_info(it->second, info);
		msg.item.push_back(info);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Oper_Activity::req_oper_activity_content_reward(uint32_t act_id, uint32_t rec_id) {
	//oper_activity_check_reset_on_restart_act();
	oper_activity_login_day();
	if(!OPER_ACTIVE_MANAGER->item_is_open(act_id)) return 0;//活动还没开启
	const Oper_Activity_Project_Config *config  = CONFIG_CACHE_OPER_ACTIVITY->get_project_config_by_id(act_id);
	if(!config) return 0;
	Oper_Activity_Item_Line_Config_Map::const_iterator item_line = config->item_line.find(rec_id);
	if(item_line == config->item_line.end()) return 0;
	int res = oper_activity_proect_item_line_reward(item_line->second, config->act_id);
	if(res) {
		return res;
	}
	MSG_50117001 msg;
	msg.act_id = act_id;
	msg.rec_id = rec_id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Oper_Activity::req_oper_activity_content_icon_num(void) {
	//oper_activity_check_reset_on_restart_act();
	oper_activity_login_day();
	const Oper_Activity_Project_Config_Map &project_map = CONFIG_CACHE_OPER_ACTIVITY->get_project_map();
	MSG_50117002 msg;
	msg.reset();
	for(Oper_Activity_Project_Config_Map::const_iterator it = project_map.begin(); it != project_map.end(); ++it) {
		if(!OPER_ACTIVE_MANAGER->item_is_open(it->second.act_id)) continue;//活动还没开启
		if(it->second.not_show) continue;
		//if(it->second.item_line.empty()) continue;
		Oper_Activity_Item_Info info;//not use
		msg.num += oper_activity_proect_content_info(it->second, info);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
	return 0;
}

int Oper_Activity::req_oper_activity_content_item(uint32_t act_id) {
	//oper_activity_check_reset_on_restart_act();
	oper_activity_login_day();
	const Oper_Activity_Project_Config_Map &project_map = CONFIG_CACHE_OPER_ACTIVITY->get_project_map();
	Oper_Activity_Project_Config_Map::const_iterator it = project_map.find(act_id);
	if(it != project_map.end()) {
		if(!OPER_ACTIVE_MANAGER->item_is_open(it->second.act_id)) return 0;//活动还没开启
		if(it->second.not_show) return 0;
		MSG_50117003 msg;
		msg.reset();
		oper_activity_proect_content_info(it->second, msg.item);
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

uint32_t Oper_Activity::oper_activity_proect_content_info(const Oper_Activity_Project_Config &proect, Oper_Activity_Item_Info &info) {
	info.act_id = proect.act_id;
	info.end_time = OPER_ACTIVE_MANAGER->get_item_end_time(proect.act_id).sec();
	uint32_t rec_num = 0;
	for(Oper_Activity_Item_Line_Config_Map::const_iterator iter = proect.item_line.begin(); iter != proect.item_line.end(); ++iter) {
		//进度
		oper_activity_item_line_uu_proc(proect.act_id, iter->second, info.uu1);
		//领取次数
		oper_activity_item_line_uu_mum(proect.act_id, iter->second, info.uu2);
		//领取状态
		rec_num += oper_activity_item_line_uu_status(proect.act_id, iter->second, info.uu3);
		//进度扩展
		oper_activity_item_line_uu_proc_ext1(proect.act_id, iter->second, info.uu4);
	}
	return rec_num;
}

int Oper_Activity::oper_activity_proect_item_line_reward(const Oper_Activity_Item_Line_Config &item_line, const uint32_t act_id) {
	Uint_Uint u;
	switch(item_line.type) {
		case OPER_ACTIVITY_ITEM_EXCHANGE_ITEM: {
			int res = oper_activity_item_line_uu_status_by_exchange_item(act_id, item_line, u);
			if(res) {
				return res;
			}
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) {
				Id_Amount_Vec erase_items;
				oper_activity_item_line_parm_2arr_to_index_amount_vec(act_id, item_line, erase_items);
				res = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, erase_items, Lose_Item_DM_Info(ITEM_LOSE_OPER_ACTIVITY, act_id, item_line.rec_id));
				if(res) {
					return res;
				}
			}
			break;
		}
		case OPER_ACTIVITY_ITEM_RECHARGE_NUM_AND_LOGIN_DAILY_NUM: {
			oper_activity_item_line_uu_status_by_common_ext1(act_id, item_line, u);
			break;
		}
		case OPER_ACTIVITY_ITEM_WING_LEVEL: {
			oper_activity_item_line_uu_status_by_wing_level(act_id, item_line, u);
			break;
		}
		case OPER_ACTIVITY_ITEM_MOUNT_AND_START: {
			oper_activity_item_line_uu_status_by_mount_on_start(act_id, item_line, u);
			break;
		}
		case OPER_ACTIVITY_ITEM_RECHARGE_ON_DAY: {
			oper_activity_item_line_uu_status_by_common_type_num_day(act_id, item_line, u);
			break;
		}
		default: {
			oper_activity_item_line_uu_status_by_common(act_id, item_line, u);
			break;
		}
	}
	if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_ED) {
		return ERROR_REWARD_ALREADY_RECEIVE;
	}
	if(u.val_2 != OPER_ACTIVITY_CONTENT_RECEIVE_CAN) {
		return ERROR_CLIENT_PARAM;
	}
	int res = oper_activity_proect_item_line_receive_reward(item_line.reward_info, Gain_Item_DM_Info(Pack::ITEM_GAIN_OPER_ACTIVITY, act_id), Money_DM_Info(MONEY_ADD_OPER_ACTIVITY, act_id));
	if(res) return res;
	oper_activity_detail_->actid_receive_id[act_id].insert(item_line.rec_id);
	++oper_activity_detail_->actid_receive_id_num[act_id][item_line.rec_id];
	oper_activity_detail_->detail_change();
	return 0;
}

int Oper_Activity::oper_activity_proect_item_line_receive_reward(const Oper_Activity_Reward_Info_Config &reward_info, const Gain_Item_DM_Info &item_dm_info, const Money_DM_Info &money_dm_info_) {
	MSG_81000102 acv_msg;
	Item_Vec item_list;
	item_list.clear();
	if(reward_info.item.size()) {
		for(Oper_Activity_Item_Vec::const_iterator item_it = reward_info.item.begin(); item_it != reward_info.item.end(); ++item_it) {
			if(item_it->career == 0 || item_it->career == (uint32_t)player_self()->career()) {
				Item_Detail item(item_it->item_id, item_it->num, Item_Detail::BIND);
				item_list.push_back(item);
			}
		}
		if(item_list.size()) {
			int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, item_dm_info);
			if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	Money_Add_List money;
	money.clear();
	if(reward_info.money.size()) {
		for(Oper_Activity_Money_Vec::const_iterator money_it = reward_info.money.begin(); money_it != reward_info.money.end(); ++money_it) {
			if(money_it->career == 0 || money_it->career == (uint32_t)player_self()->career()) {
				money.push_back(Money_Add_Info(Money_Type(money_it->money_id), money_it->num, money_dm_info_));
			}
		}
		if(money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if(reward_info.exp && (reward_info.career == 0 || reward_info.career == (uint32_t)player_self()->career())) {
		Exp_All exp_all = player_self()->modify_experience_inter(reward_info.exp, false, false, true);
		if (0 == exp_all.result) {
			money.push_back(Money_Add_Info(Money_Type(PT_EXP_CURRENT), reward_info.exp, money_dm_info_));
		}
	}
	player_self()->client_get_item_effect(item_list,money,acv_msg);
	return 0;
}

int Oper_Activity::oper_activity_proect_item_line_receive_reward_on_mail(const uint32_t mail_id, const Oper_Activity_Reward_Info_Config &reward_info) {
	if(reward_info.item.size()) {
			Item_Vec item_list;
			for(Oper_Activity_Item_Vec::const_iterator item_it = reward_info.item.begin(); item_it != reward_info.item.end(); ++item_it) {
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

void Oper_Activity::oper_activity_listen_type_common_add(const uint32_t act_id, const uint32_t type, const uint32_t num, const uint32_t num_type) {
	if(OPER_ACTIVE_MANAGER->item_is_open(act_id)) {
		switch(num_type) {
			case OPER_ACTIVITY_LISTEN_TYPE_NUM: {
				oper_activity_detail_->actid_type_record_val[act_id][type].num += num;
				break;
			}
			case OPER_ACTIVITY_LISTEN_TYPE_EXT1: {
				oper_activity_detail_->actid_type_record_val[act_id][type].ext1 += num;
				break;
			}
			case OPER_ACTIVITY_LISTEN_TYPE_EXT2: {
				oper_activity_detail_->actid_type_record_val[act_id][type].ext2 += num;
				break;
			}
			case OPER_ACTIVITY_LISTEN_TYPE_EXT3: {
				oper_activity_detail_->actid_type_record_val[act_id][type].ext3 += num;
				break;
			}
			default: {
				oper_activity_detail_->actid_type_record_val[act_id][type].type_num[num_type] += num;
			}
		}
	}
}

void Oper_Activity::oper_activity_calc_login_day(const uint32_t type, const uint32_t num, const uint32_t num_type) {
	const UInt_Set *conf = CONFIG_CACHE_OPER_ACTIVITY->get_type_actidset(type);
	if(conf) {
		Date_Time time =Date_Time();
		for(UInt_Set::const_iterator it = conf->begin(); it != conf->end(); ++it) {
			if(!OPER_ACTIVE_MANAGER->item_is_open(*it)) {
				continue;
			}
			if(oper_activity_detail_->actid_login_day[*it] != time.weekday()+1) {
				oper_activity_listen_type_common_add(*it, type, num, num_type);
				oper_activity_detail_->actid_login_day[*it] = time.weekday()+1;
				oper_activity_detail_->detail_change();
			}
		}
	}
}

void Oper_Activity::oper_activity_listen_type_common(const uint32_t type, const uint32_t num, const uint32_t num_type) {
	const UInt_Set *conf = CONFIG_CACHE_OPER_ACTIVITY->get_type_actidset(type);
	if(conf) {
		for(UInt_Set::const_iterator it = conf->begin(); it != conf->end(); ++it) {
			oper_activity_listen_type_common_add(*it, type, num, num_type);
		}
	}
	req_oper_activity_content_icon_num();
}

void Oper_Activity::oper_activity_listen_type_common_re(const uint32_t type, const uint32_t num, const uint32_t num_type) {
	const UInt_Set *conf = CONFIG_CACHE_OPER_ACTIVITY->get_type_actidset(type);
	if(conf) {
		for(UInt_Set::const_iterator it = conf->begin(); it != conf->end(); ++it) {
			if(OPER_ACTIVE_MANAGER->item_is_open(*it)) {
				switch(num_type) {
					case OPER_ACTIVITY_LISTEN_TYPE_NUM: {
						oper_activity_detail_->actid_type_record_val[*it][type].num = num;
						break;
					}
					case OPER_ACTIVITY_LISTEN_TYPE_EXT1: {
						oper_activity_detail_->actid_type_record_val[*it][type].ext1 = num;
						break;
					}
					case OPER_ACTIVITY_LISTEN_TYPE_EXT2: {
						oper_activity_detail_->actid_type_record_val[*it][type].ext2 = num;
						break;
					}
					case OPER_ACTIVITY_LISTEN_TYPE_EXT3: {
						oper_activity_detail_->actid_type_record_val[*it][type].ext3 = num;
						break;
					}
					default: {
						oper_activity_detail_->actid_type_record_val[*it][type].type_num[num_type] = num;
					}
				}
			}
		}
	}
	req_oper_activity_content_icon_num();
}


//进度
void Oper_Activity::oper_activity_item_line_uu_proc(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	switch(item_line.type) {
		case OPER_ACTIVITY_ITEM_EXCHANGE_ITEM: {
			oper_activity_item_line_uu_proc_by_exchange_item(act_id, item_line, uu);
			break;
		}
		case OPER_ACTIVITY_ITEM_RECHARGE_ON_DAY: {
			oper_activity_item_line_uu_proc_by_common_type_num_day(act_id, item_line, uu);
			break;
		}
		case OPER_ACTIVITY_ITEM_WING_LEVEL: {
			oper_activity_item_line_uu_proc_by_wing_level(act_id, item_line, uu);
			break;
		}
		case OPER_ACTIVITY_ITEM_MOUNT_AND_START: {
			oper_activity_item_line_uu_proc_by_mount_on_start(act_id, item_line, uu);
			break;
		}
		default: {
			oper_activity_item_line_uu_proc_by_common(act_id, item_line, uu);
			break;
		}
	}
}

//进度ext
void Oper_Activity::oper_activity_item_line_uu_proc_ext1(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	switch(item_line.type) {
		case OPER_ACTIVITY_ITEM_RECHARGE_NUM_AND_LOGIN_DAILY_NUM: {
			oper_activity_item_line_uu_proc_by_common_ext1(act_id, item_line, uu);
			break;
		}
	}
}

//领取次数
void Oper_Activity::oper_activity_item_line_uu_mum(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	if(item_line.ditto) {
		Uint_Uint u;
		u.val_1 = item_line.rec_id;
		u.val_2 = oper_activity_detail_->actid_receive_id_num[act_id][item_line.rec_id];
		uu.push_back(u);
	}
}
//领取状态
uint32_t Oper_Activity::oper_activity_item_line_uu_status(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	uint32_t rec_num = 0;
	switch(item_line.type) {
		case OPER_ACTIVITY_ITEM_EXCHANGE_ITEM: {
			Uint_Uint u;
			oper_activity_item_line_uu_status_by_exchange_item(act_id, item_line, u);
			uu.push_back(u);
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) ++rec_num;
			break;
		}
		case OPER_ACTIVITY_ITEM_RECHARGE_NUM_AND_LOGIN_DAILY_NUM: {
			Uint_Uint u;
			oper_activity_item_line_uu_status_by_common_ext1(act_id, item_line, u);
			uu.push_back(u);
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) ++rec_num;
			break;
		}
		case OPER_ACTIVITY_ITEM_WING_LEVEL: {
			Uint_Uint u;
			oper_activity_item_line_uu_status_by_wing_level(act_id, item_line, u);
			uu.push_back(u);
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) ++rec_num;
			break;
		}
		case OPER_ACTIVITY_ITEM_MOUNT_AND_START: {
			Uint_Uint u;
			oper_activity_item_line_uu_status_by_mount_on_start(act_id, item_line, u);
			uu.push_back(u);
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) ++rec_num;
			break;
		}
		case OPER_ACTIVITY_ITEM_RECHARGE_ON_DAY: {
			Uint_Uint u;
			oper_activity_item_line_uu_status_by_common_type_num_day(act_id, item_line, u);
			uu.push_back(u);
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) ++rec_num;
			break;
		}
		default: {
			Uint_Uint u;
			oper_activity_item_line_uu_status_by_common(act_id, item_line, u);
			uu.push_back(u);
			if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) ++rec_num;
			break;
		}
	}
	return rec_num;
}

void Oper_Activity::oper_activity_item_line_uu_proc_by_common(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	Uint_Uint u;
	u.val_1 = item_line.rec_id;
	u.val_2 = oper_activity_detail_->actid_type_record_val[act_id][item_line.type].num;
	uu.push_back(u);
}

void Oper_Activity::oper_activity_item_line_uu_proc_by_common_type_num_day(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	Uint_Uint u;
	u.val_1 = item_line.rec_id;
	int32_t day = item_line.parm - 1;
	if(day >= 0) {
		const Time_Value &start_time = OPER_ACTIVE_MANAGER->get_item_start_time(act_id);
		Date_Time start_tmp_date = Date_Time(start_time);
		start_tmp_date.hour(0);
		start_tmp_date.second(0);
		start_tmp_date.minute(0);
		u.val_2 = oper_activity_detail_->actid_type_record_val[act_id][item_line.type].type_num[start_tmp_date.time_sec() + day*Time_Value::ONE_DAY_IN_SECS];
	} else {
		u.val_2 = 0;
	}
	uu.push_back(u);
}

void Oper_Activity::oper_activity_item_line_uu_proc_by_wing_level(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	Uint_Uint u;
	u.val_1 = item_line.rec_id;
	int lv = player_self()->wing_detail().lv;
	const Wing_Config *win_conf = CONFIG_CACHE_WING->get_wing_config(lv);
	if(win_conf) {
		u.val_2 = win_conf->show_stage;
	}
	uu.push_back(u);
}

void Oper_Activity::oper_activity_item_line_uu_proc_by_mount_on_start(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	Uint_Uint u;
	u.val_1 = item_line.rec_id;
	u.val_2 = oper_activity_get_mount_star_level_by_mount_id(item_line.parm);
	uu.push_back(u);
}

void Oper_Activity::oper_activity_item_line_uu_proc_by_exchange_item(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	Uint_Uint u;
	u.val_1 = item_line.rec_id;
	u.val_2 = oper_activity_detail_->actid_receive_id_num[act_id][item_line.rec_id];
	uu.push_back(u);
}

void Oper_Activity::oper_activity_item_line_uu_proc_by_common_ext1(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, std::vector<Uint_Uint> &uu) {
	Uint_Uint u;
	u.val_1 = item_line.rec_id;
	u.val_2 = oper_activity_detail_->actid_type_record_val[act_id][item_line.type].ext1;
	uu.push_back(u);
}

void Oper_Activity::oper_activity_item_line_uu_status_by_common(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u) {
	u.val_1 = item_line.rec_id;
	if(oper_activity_detail_->actid_receive_id[act_id].find(item_line.rec_id) != oper_activity_detail_->actid_receive_id[act_id].end()) {
		if(item_line.ditto) {
			//重复领取当前进度 =  总完成次数 - 领取次数 * 该完成次数
			int sub_num = oper_activity_detail_->actid_type_record_val[act_id][item_line.type].num - oper_activity_detail_->actid_receive_id_num[act_id][item_line.rec_id] * item_line.num;
			if(sub_num >= 0 && (uint32_t)sub_num >= item_line.num) {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
			} else {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
			}
		} else {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_ED;
		}
	} else {
		if(oper_activity_detail_->actid_type_record_val[act_id][item_line.type].num >= item_line.num) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
		} else {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
		}
	}
}

void Oper_Activity::oper_activity_item_line_uu_status_by_common_type_num_day(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u) {
	u.val_1 = item_line.rec_id;
	if(oper_activity_detail_->actid_receive_id[act_id].find(item_line.rec_id) != oper_activity_detail_->actid_receive_id[act_id].end()) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_ED;
	} else {
		int32_t day = item_line.parm - 1;
		if(day < 0) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
		} else {
			const Time_Value &start_time = OPER_ACTIVE_MANAGER->get_item_start_time(act_id);
			Date_Time start_tmp_date = Date_Time(start_time);
			start_tmp_date.hour(0);
			start_tmp_date.second(0);
			start_tmp_date.minute(0);
			if(item_line.num <= oper_activity_detail_->actid_type_record_val[act_id][item_line.type].type_num[start_tmp_date.time_sec() + day*Time_Value::ONE_DAY_IN_SECS]) {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
			} else {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
			}
		}
	}
}

void Oper_Activity::oper_activity_item_line_uu_status_by_wing_level(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u) {
	u.val_1 = item_line.rec_id;
	if(oper_activity_detail_->actid_receive_id[act_id].find(item_line.rec_id) != oper_activity_detail_->actid_receive_id[act_id].end()) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_ED;
	} else {
		int lv = player_self()->wing_detail().lv;
		const Wing_Config *win_conf = CONFIG_CACHE_WING->get_wing_config(lv);
		if(win_conf && (uint32_t)win_conf->show_stage >= item_line.num) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
		} else {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
		}

	}
}

void Oper_Activity::oper_activity_item_line_uu_status_by_mount_on_start(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u) {
	u.val_1 = item_line.rec_id;
	if(oper_activity_detail_->actid_receive_id[act_id].find(item_line.rec_id) != oper_activity_detail_->actid_receive_id[act_id].end()) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_ED;
	} else {
		if(oper_activity_get_mount_star_level_by_mount_id(item_line.parm) >= item_line.num) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
		} else {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
		}

	}
}

int Oper_Activity::oper_activity_item_line_uu_status_by_exchange_item(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u) {
	u.val_1 = item_line.rec_id;
	int res = 0;
	if(oper_activity_detail_->actid_receive_id_num[act_id][item_line.rec_id]  >= item_line.num) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_ED;
	} else {
		Id_Amount_Vec erase_items;
		oper_activity_item_line_parm_2arr_to_index_amount_vec(act_id, item_line, erase_items);
		res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, erase_items, false);
		if(res) {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
		} else {
			u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
		}
	}
	return res;
}

void Oper_Activity::oper_activity_item_line_parm_2arr_to_index_amount_vec(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Id_Amount_Vec &u) {
	u.clear();
	for(UInt_2_Vec::const_iterator it = item_line.parm_ex_2arr.begin(); it != item_line.parm_ex_2arr.end(); ++it) {
		if(it->size() >= 2) {
			u.push_back(Id_Amount((*it)[0], (*it)[1]));
		}
	}
}

void Oper_Activity::oper_activity_item_line_uu_status_by_common_ext1(const uint32_t act_id, const Oper_Activity_Item_Line_Config &item_line, Uint_Uint &u) {
	u.val_1 = item_line.rec_id;
	oper_activity_item_line_uu_status_by_common(act_id, item_line, u);
	if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) {
		if(oper_activity_detail_->actid_receive_id[act_id].find(item_line.rec_id) != oper_activity_detail_->actid_receive_id[act_id].end()) {
			if(item_line.ditto) {
				//重复领取当前进度 =  总完成次数 - 领取次数 * 该完成次数
				int sub_num = oper_activity_detail_->actid_type_record_val[act_id][item_line.type].ext1 - oper_activity_detail_->actid_receive_id_num[act_id][item_line.rec_id] * item_line.parm;
				if(sub_num >= 0 && (uint32_t)sub_num >= item_line.parm) {
					u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
				} else {
					u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
				}
			} else {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_ED;
			}
		} else {
			if(oper_activity_detail_->actid_type_record_val[act_id][item_line.type].ext1 >= item_line.parm) {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_CAN;
			} else {
				u.val_2 = OPER_ACTIVITY_CONTENT_RECEIVE_NO;
			}
		}
	}
}

void Oper_Activity::oper_activity_check_reset_on_restart_act(void) {
	const Oper_Activity_Project_Config_Map  &conf = CONFIG_CACHE_OPER_ACTIVITY->get_project_map();
	for(Oper_Activity_Project_Config_Map::const_iterator cit = conf.begin(); cit != conf.end(); ++cit) {
		if(oper_activity_detail_->last_item_start_time[cit->second.act_id].sec() != OPER_ACTIVE_MANAGER->get_item_start_time(cit->second.act_id).sec()) {
			oper_activity_detail_->actid_type_record_val[cit->second.act_id].clear();//活动ID->类型->完成记录(次数,类型->值)
			oper_activity_detail_->actid_receive_id[cit->second.act_id].clear();//活动ID对应已经领取的ID集合
			oper_activity_detail_->actid_receive_id_num[cit->second.act_id].clear();//活动ID->(项目)领取ID->领取次数
			oper_activity_detail_->actid_append_effect[cit->second.act_id].clear();
			oper_activity_detail_->last_item_start_time[cit->second.act_id] = OPER_ACTIVE_MANAGER->get_item_start_time(cit->second.act_id);
			oper_activity_detail_->actid_login_day[cit->second.act_id] = 0;
			oper_activity_detail_->detail_change();
		}
	}
}

void Oper_Activity::oper_activity_daily_refresh(void) {
	Time_Value now = Time_Value::gettimeofday();
	const Oper_Activity_Project_Config_Map  &conf = CONFIG_CACHE_OPER_ACTIVITY->get_project_map();
	for(Oper_Activity_Project_Config_Map::const_iterator cit = conf.begin(); cit != conf.end(); ++cit) {
		if(!OPER_ACTIVE_MANAGER->item_is_open(cit->second.act_id))  continue;
		UInt_Set refresh_type_id;
		Oper_Activity_Reward_Info_Config reward_info;
		for(Oper_Activity_Item_Line_Config_Map::const_iterator lit = cit->second.item_line.begin(); lit != cit->second.item_line.end(); ++lit) {
			if(!lit->second.refresh) continue;
			int spl_day = (now.sec() - oper_activity_detail_->actid_recid_last_refresh_time[cit->first][lit->first].sec())/Time_Value::ONE_DAY_IN_SECS;
			if(spl_day > 0 && lit->second.refresh <= (uint32_t)spl_day) {
				//判断是否可领
				Uint_Uint u;
				switch(lit->second.type) {
					default: {
						oper_activity_item_line_uu_status_by_common(cit->second.act_id, lit->second, u);
						break;
					}
				}
				if(u.val_2 == OPER_ACTIVITY_CONTENT_RECEIVE_CAN) {
					reward_info.push_back(lit->second.reward_info);
				}
				//记录时间
				Date_Time date_(now);
				date_.hour(6);
				date_.minute(0);
				date_.second(0);
				oper_activity_detail_->actid_recid_last_refresh_time[cit->first][lit->first].sec(date_.time_sec());
				//清空记录
				UInt_Set::iterator it = oper_activity_detail_->actid_receive_id[cit->first].find(lit->second.rec_id);
				if(it != oper_activity_detail_->actid_receive_id[cit->first].end()) {
					oper_activity_detail_->actid_receive_id[cit->first].erase(it);
				}
				oper_activity_detail_->actid_receive_id_num[cit->first][lit->first] = 0;
				if(refresh_type_id.find(lit->second.type) == refresh_type_id.end()) {
					refresh_type_id.insert(lit->second.type);
				}
			}
		}
		//send mail
		oper_activity_proect_item_line_receive_reward_on_mail(cit->second.mail_id, reward_info);
		for(UInt_Set::iterator it = refresh_type_id.begin(); it != refresh_type_id.end(); ++it) {
			oper_activity_detail_->actid_type_record_val[cit->second.act_id][*it].reset();
		}
	}
	oper_activity_detail_->detail_change();
	req_oper_activity_content_icon_num();
}

uint32_t Oper_Activity::oper_activity_get_mount_star_level_by_mount_id(uint32_t mount_id) {
	std::vector<Horse_Info>::const_iterator it;
	if(mount_id) {
		for(it = player_self()->mount_detail().horse_list.begin(); it != player_self()->mount_detail().horse_list.end(); ++it){
			if((uint32_t)it->horse_id == mount_id){
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

void Oper_Activity::oper_activity_add_login_day_on_test_cmd(int n) {
	oper_activity_detail_->actid_login_day.clear();
	oper_activity_login_day();
}

void Oper_Activity::sub_n_day_actid_recid_last_refresh_time(int n) {
	for(UInt_UInt_Time_Map::iterator iter = oper_activity_detail_->actid_recid_last_refresh_time.begin(); iter != oper_activity_detail_->actid_recid_last_refresh_time.end(); ++iter) {
		for(UInt_Time_Map::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			it->second.sec(it->second.sec() - n*Time_Value::ONE_DAY_IN_SECS);
		}
	}
	oper_activity_detail_->detail_change();
	oper_activity_daily_refresh();
}
