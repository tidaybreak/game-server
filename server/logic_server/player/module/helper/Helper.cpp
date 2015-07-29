/*
 * Helper.cpp
 *
 *  Created on: 2014-06-25
 *      Author: xiaoliang
 */

#include "Helper.h"
#include "Helper_Struct.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Daily.h"
#include "Msg_Daily_Struct.h"
#include "Helper_Def.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "pack/Pack_Def.h"
#include "pack/Pack_Struct.h"
#include "Logic_Player.h"
#include "active/Active_Manager.h"
#include "Config_Cache_Upgrade.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "equiper/Equiper_Def.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Alchemy.h"
#include "Config_Cache_Active.h"
#include "alchemy/Alchemy_Struct.h"
#include "active/Active_Def.h"
#include "Msg_Active_Struct.h"
#include "equiper/Equiper_Func.h"
#include "Config_Cache_Role.h"
#include "Helper_Manager.h"
#include "dragon/Dragon_Vale_Struct.h"
#include "public/Public_Struct.h"
#include "Config_Cache_Gang.h"
#include "gang/Gang_Manager.h"
#include "fashion/Fashion_Struct.h"
#include "mount/Mount_Struct.h"
#include "wing/Wing_Struct.h"
#include "Config_Cache_Team.h"
#include "fighter/Logic_Fighter_Struct.h"

Helper::Helper(void) {
	reset();
}

Helper::~Helper() {
	// TODO Auto-generated destructor stub
}

void Helper::reset(void) {
	helper_detail_ = 0;
	last_listen_force_time = Time_Value::zero;
}

int Helper::create_init(Helper_Detail &detail) {

	return 0;
}

void Helper::load_detail(Helper_Detail &detail) {
	helper_detail_ = &detail;
}

void Helper::save_detail(Helper_Detail &detail) {
	if (daily_detail().is_change) {
		detail = daily_detail();
		helper_detail_->is_change = false;
	}
}

int Helper::trigger_daily_zero(const Time_Value &now) {
	helper_detail_->today_is_check_in = 0;
	HELPER_MANAGER->tick(now);
	if(helper_detail_->last_check_in_refresh_time.sec() != HELPER_MANAGER->get_refresh_time().sec()) {
		helper_detail_->last_check_in_refresh_time = HELPER_MANAGER->get_refresh_time();
		helper_detail_->check_in_day = 0;
	}

	helper_detail_->detail_change();
	return 0;
}

int Helper::trigger_daily_six(const Time_Value &now) {
	helper_detail_->today_activity = 0;
	helper_detail_->today_award_receive.clear();
	helper_detail_->receive_vip_activity = 0;
	for(UInt_UInt_Map::iterator it = helper_detail_->daily_num_map_.begin(); it != helper_detail_->daily_num_map_.end(); ++it) {
		it->second = 0;
	}
	for(UInt_UInt_Map::iterator it = helper_detail_->activity_num_map_.begin(); it != helper_detail_->activity_num_map_.end(); ++it) {
		it->second = 0;
	}
	helper_detail_->detail_change();
	player_self()->task_listen_activeness_of_today(helper_detail_->today_activity);
	return 0;
}

int Helper::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Helper::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Helper::move_scene_completed(void) {

	return 0;
}

int Helper::client_ready_now(void) {
	if(player_self()->is_icon_open(HELPER_COMMON_DIALY_ICON_ID)) {
		send_check_in_msg();
	}
	if(player_self()->vip() >= HELPER_COMMON_AUTO_CHECK_IN_VIP_LEVEL) {
		req_helper_check_in();
	}
//	if(player_self()->level() >= HELPER_COMMON_CHECK_IN_DAILY_LEVEL) { //日常图标
//		MSG_81000408 msg;
//		msg.open = 1;
//		msg.active = 0;
//		msg.time = 0;
//		msg.icon_id = 5;
//		THIS_SEND_TO_CLIENT(msg);
//	}
	return 0;
}

void Helper::module_init(void) {
	{
		const Conf_Key_Map &daily_conf_ = CONFIG_CACHE_DAILY->daily_config_key_map();
		for(Conf_Key_Map::const_iterator iter = daily_conf_.begin(); iter != daily_conf_.end(); ++iter) {
			UInt_UInt_Map::iterator it = helper_detail_->daily_num_map_.find(iter->first);
			if(it == helper_detail_->daily_num_map_.end()) {
				helper_detail_->daily_num_map_.insert(std::make_pair(iter->first, 0));
				helper_detail_->detail_change();
			}
		}
		//删除配置没有的
		UInt_Vec to_del;
		to_del.clear();
		for(UInt_UInt_Map::iterator it = helper_detail_->daily_num_map_.begin(); it != helper_detail_->daily_num_map_.end(); ++it) {
			Conf_Key_Map::const_iterator cit = daily_conf_.find(it->first);
			if(cit == daily_conf_.end()) {
				to_del.push_back(it->first);
			}
		}
		for(UInt_Vec::iterator it = to_del.begin(); it != to_del.end(); ++it) {
			UInt_UInt_Map::iterator dit = helper_detail_->daily_num_map_.find(*it);
			if(dit != helper_detail_->daily_num_map_.end()) {
				helper_detail_->daily_num_map_.erase(dit);
				helper_detail_->detail_change();
			}
		}
	}
	{
		const Conf_Key_Map &conf_ = CONFIG_CACHE_DAILY->activity_config_key_map();
		for(Conf_Key_Map::const_iterator iter = conf_.begin(); iter != conf_.end(); ++iter) {
			UInt_UInt_Map::iterator it = helper_detail_->activity_num_map_.find(iter->first);
			if(it == helper_detail_->activity_num_map_.end()) {
				helper_detail_->activity_num_map_.insert(std::make_pair(iter->first, 0));
				helper_detail_->detail_change();
			}
		}
		//删除配置没有的
		UInt_Vec to_del;
		to_del.clear();
		for(UInt_UInt_Map::iterator it = helper_detail_->activity_num_map_.begin(); it != helper_detail_->activity_num_map_.end(); ++it) {
			Conf_Key_Map::const_iterator cit = conf_.find(it->first);
			if(cit == conf_.end()) {
				to_del.push_back(it->first);
			}
		}
		for(UInt_Vec::iterator it = to_del.begin(); it != to_del.end(); ++it) {
			UInt_UInt_Map::iterator dit = helper_detail_->activity_num_map_.find(*it);
			if(dit != helper_detail_->activity_num_map_.end()) {
				helper_detail_->activity_num_map_.erase(dit);
				helper_detail_->detail_change();
			}
		}
	}
}

void Helper::sign_in(void) {
	if(player_self()->vip() >= 2 && !helper_detail_->receive_vip_activity){
		helper_detail_->receive_vip_activity = 1;
		helper_detail_->today_activity += HELPER_COMMON_ACTIVITY_VIP_ADD;
		helper_detail_->all_activity += HELPER_COMMON_ACTIVITY_VIP_ADD;
		player_self()->task_listen_activeness_of_today(helper_detail_->today_activity);
	}
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_DAILY_LOGIN); //日常活跃度监听
}

void Helper::sign_out(void) {

}

void Helper::trusteeship_sign_out(void) {

}

void Helper::daily_helper_listen(uint32_t type, uint32_t type_id) {
	listen_daily(type, type_id);
	listen_activity(type, type_id);
}

int Helper::req_daily(void) {
	const Daily_Conf_Map &conf_map = CONFIG_CACHE_DAILY->daily_config_map();
	MSG_50106000 msg;
	msg.reset();
	Uint_Uint uu;

	for(Daily_Conf_Map::const_iterator iter = conf_map.begin(); iter != conf_map.end(); ++iter) {
		uu.reset();
		uu.val_1 = iter->second.id;
		UInt_UInt_Map::iterator it = helper_detail_->daily_num_map_.find(iter->second.id);
		if(it != helper_detail_->daily_num_map_.end()) {
			uu.val_2 = it->second;
		}
		//需要动态获取的完成次数
		switch(iter->second.task_type) {
			case DAILY_FULFILL_LISTEN_RING_TASK: {
				uu.val_2 = player_self()->get_did_ring_task_num();
				break;
			}
		}
		msg.daily_info.push_back(uu);
		//得到最大限制次数
		uu.val_2 = iter->second.fulfil_num;//配置的最大次数
		//需要动态获取的最大次数
		switch(iter->second.task_type) {
			case DAILY_FULFILL_LISTEN_ARENA: {
				int total_num;
				int enter_num;
				player_self()->get_arena_enter_num_info(total_num, enter_num);
				uu.val_2 = total_num;
				break;
			}
			case DAILY_FULFILL_LISTEN_ELITE_EXPLORE: {
				uu.val_2 = player_self()->get_elite_dungeon_activation_num();
				break;
			}
			case DAILY_FULFILL_LISTEN_REWARD_TASK: {
				uu.val_2 = player_self()->total_num_of_can_do_reward_task();
				break;
			}
			case DAILY_FULFILL_LISTEN_GANG_LOTTERY: {
				Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(player_self()->gang_id());
				if (! gang_view) {
					break;
				}
				uu.val_2 = CONFIG_CACHE_GANG->base(gang_view->level).daily_draw_times;
				break;
			}
			case DAILY_FULFILL_LISTEN_SINGLE_EXPLORE: {
				//当前可以参与次数
				uint32_t num = player_self()->fighter_detail().current_strength()/HELPER_COMMON_SINGLE_EXPLORE_COST_STRENGTH;
				//加上已经参与的次数
				uu.val_2 = num + it->second;
				break;
			}
			case DAILY_FULFILL_LISTEN_ALCHEMY: {
				//免费次数+付费次数
				uu.val_2 = player_self()->get_alchemy_chances_free() + CONFIG_CACHE_ALCHEMY->get_chances_pay_by_vip_level(player_self()->vip());
				break;
			}
			case DAILY_FULFILL_LISTEN_RING_TASK: {
				uu.val_2 = player_self()->get_total_ring_task_num();
				break;
			}
		}
		msg.daily_max.push_back(uu);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Helper::req_activity_receive(uint8_t type, uint32_t id) {
	switch(type) {
		case TODAY_AWARD: {
			//判断是否领取过
			if(helper_detail_->today_award_receive.find(id) != helper_detail_->today_award_receive.end()) {
					return ERROR_HAS_RECEIVED;
			}
			//判断是否有该礼包 和是否能领取
			const Activity_Award_Map &conf_map = CONFIG_CACHE_DAILY->today_act_award_map();
			Activity_Award_Map::const_iterator award_iter = conf_map.find(id);
			if(award_iter == conf_map.end() || award_iter->second.act > helper_detail_->today_activity) {
				return ERROR_CLIENT_PARAM;
			}
			//判断背包
			Item_Detail item = Item_Detail(award_iter->second.award_id, 1, Item_Detail::Bind_Type::BIND);
			int return_status = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX,item);
			if(return_status != 0) {
				return return_status;
			}
			if(player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,item, Gain_Item_DM_Info(Pack::ITEM_GAIN_DAILY))) {
				return ERROR_INNER_SERVER;
			}
			{
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				THIS_SEND_TO_CLIENT(acv_msg);
			}
			helper_detail_->today_award_receive.insert(id);
			helper_detail_->detail_change();
			MSG_50106002 msg;
			msg.reset();
			msg.id = id;
			msg.type = type;
			THIS_SEND_TO_CLIENT(msg);
			break;
		}
		case ALL_AWARD: {
			//判断是否领取过
			for(UInt_Vec::iterator it = helper_detail_->all_award_receive.begin(); it != helper_detail_->all_award_receive.end(); ++it) {
				if(id == *it) {
					return ERROR_HAS_RECEIVED;
				}
			}
			//判断是否有该礼包 和是否能领取
			const Activity_Award_Map &conf_map = CONFIG_CACHE_DAILY->all_act_award_map();
			Activity_Award_Map::const_iterator award_iter = conf_map.find(id);
			if(award_iter == conf_map.end() || award_iter->second.act > helper_detail_->all_activity) {
				return ERROR_CLIENT_PARAM;
			}
			//判断背包
			Item_Detail item = Item_Detail(award_iter->second.award_id, award_iter->second.num, Item_Detail::Bind_Type::BIND);
			int return_status = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX,item);
			if(return_status != 0) {
				return return_status;
			}
			if(player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,item, Gain_Item_DM_Info(Pack::ITEM_GAIN_DAILY))) {
				return ERROR_INNER_SERVER;
			}
			{
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				THIS_SEND_TO_CLIENT(acv_msg);
			}
			helper_detail_->all_award_receive.push_back(id);
			helper_detail_->detail_change();
			MSG_50106002 msg;
			msg.reset();
			msg.id = id;
			msg.type = type;
			THIS_SEND_TO_CLIENT(msg);
			break;
		}
	}
	return 0;
}

int Helper::req_activity(void) {
	MSG_50106001 msg;
	msg.reset();
	Uint_Uint uu;
	for(UInt_UInt_Map::iterator it = helper_detail_->activity_num_map_.begin(); it != helper_detail_->activity_num_map_.end(); ++it ) {
		uu.reset();
		uu.val_1 = it->first;
		uu.val_2 = it->second;
		msg.activity_info.push_back(uu);
	}
	msg.today_activity = helper_detail_->today_activity;
	msg.all_activity = helper_detail_->all_activity;
	const Activity_Award_Map &award_conf = CONFIG_CACHE_DAILY->today_act_award_map();
	for(Activity_Award_Map::const_iterator award_iter = award_conf.begin(); award_iter != award_conf.end(); ++award_iter) {
		uu.reset();
		uu.val_1 = award_iter->first;
		if(helper_detail_->today_award_receive.find(award_iter->first) != helper_detail_->today_award_receive.end()) {
			uu.val_2 = 1; //已领取
		} else {
			uu.val_2 = 0; //未领取
		}
		msg.today_award_receive.push_back(uu);
	}
	msg.all_award_receive = helper_detail_->all_award_receive;
	msg.receive_vip_activity = helper_detail_->receive_vip_activity;
	msg.toady_activity_group = CONFIG_CACHE_DAILY->get_activity_group_id();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Helper::listen_daily(uint32_t type, uint32_t type_id) {
	const Daily_Conf_Map &conf_map = CONFIG_CACHE_DAILY->daily_config_map();
	Config_Key key;
	key.reset();
	key.task_type = type;
	key.type_id = type_id;
	Daily_Conf_Map::const_iterator iter = conf_map.find(key);
	if(iter != conf_map.end()) {
		UInt_UInt_Map::iterator it = helper_detail_->daily_num_map_.find(iter->second.id);
		if(it != helper_detail_->daily_num_map_.end()) {
			//iter->second.fulfil_num == 0 处理无限次数
			//if(it->second < iter->second.fulfil_num || iter->second.fulfil_num == 0) {
				++it->second;
				helper_detail_->detail_change();
				active_to_client(DAILY_TYPE, iter->second.id, it->second);
			//}
		} else {
			helper_detail_->daily_num_map_.insert(std::make_pair(iter->second.id, 1));
			helper_detail_->detail_change();
			active_to_client(DAILY_TYPE, iter->second.id, 1);
		}
	}
}

void Helper::listen_activity(uint32_t type, uint32_t type_id) {
	const Activity_Conf_Map &conf_map = CONFIG_CACHE_DAILY->activity_config_map();
	Config_Key key;
	key.reset();
	key.task_type = type;
	key.type_id = type_id;
	Activity_Conf_Map::const_iterator iter = conf_map.find(key);
	if(iter != conf_map.end() && iter->second.fulfil_num) {
		UInt_UInt_Map::iterator it = helper_detail_->activity_num_map_.find(iter->second.id);
		if(it != helper_detail_->activity_num_map_.end()) {
			if(it->second < iter->second.fulfil_num) {
				++it->second;
				//增加活跃度
				if(it->second >= iter->second.fulfil_num) {
					helper_detail_->today_activity += iter->second.activity;
					helper_detail_->all_activity += iter->second.activity;
					player_self()->task_listen_activeness_of_today(helper_detail_->today_activity);
				}
				helper_detail_->detail_change();
				active_to_client(ACTIVITY_TYPE, iter->second.id, it->second);
			}
		} else {
			helper_detail_->activity_num_map_.insert(std::make_pair(iter->second.id, 1));
			//增加活跃度
			if(1 >= iter->second.fulfil_num) {
				helper_detail_->today_activity += iter->second.activity;
				helper_detail_->all_activity += iter->second.activity;
				player_self()->task_listen_activeness_of_today(helper_detail_->today_activity);
			}
			helper_detail_->detail_change();
			active_to_client(ACTIVITY_TYPE, iter->second.id, 1);
		}
	}
}

void Helper::active_to_client(uint32_t type, uint32_t id, uint32_t num) {
	MSG_82200200 msg;
	msg.reset();
	msg.type = type;
	msg.id = id;
	msg.num = num;
	msg.today_activity = helper_detail_->today_activity;
	msg.all_activity = helper_detail_->all_activity;
	msg.receive_vip_activity = helper_detail_->receive_vip_activity;
	THIS_SEND_TO_CLIENT(msg);
}

int Helper::sign_in_repeat(void) {

	return 0;
}

void Helper::helper_listen_join_arena_force(const int last_join_arena_force) {
	helper_detail_->last_join_arena_force = last_join_arena_force;
}

void Helper::helper_listen_world_chat(void) {
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_WORLD_CHAT); //日常活跃度监听
}

void Helper::helper_listen_icon_open(const uint16_t icon_id) {
	if(icon_id == HELPER_COMMON_DIALY_ICON_ID) {
		bool need_send = true;
		if(player_self()->vip() >= HELPER_COMMON_AUTO_CHECK_IN_VIP_LEVEL) {
			if(req_helper_check_in()){
				need_send = false;
			}
		}
		if(need_send) {
			send_check_in_msg();
		}
	}
}

//void Helper::reward_back_listen_level_up(const int level) {
//	if(level == HELPER_COMMON_CHECK_IN_DAILY_LEVEL) {
//		bool need_send = false;
//		if(player_self()->vip() >= HELPER_COMMON_AUTO_CHECK_IN_VIP_LEVEL) {
//			if(req_helper_check_in()){
//				need_send = true;
//			}
//		}
//		if(need_send) {
//			send_check_in_msg();
//		}
//	}
//}

void Helper::calculate_two_properties(const std::map<int, double> &pro_val_map,  std::map<int, double> &out_pro_val_map) {
	out_pro_val_map.clear();
	// 一二级转化率
	const Player_Init_Cache &init_config = CONFIG_CACHE_ROLE->player_init_cache();
	Player_Init_Cache::Prop_Conversion::const_iterator find_it = init_config.prop_conversion.find(career());
	if (find_it != init_config.prop_conversion.end()) {
		for (Player_Init_Cache::Prop_Value_Vec::const_iterator it = find_it->second.begin(); it != find_it->second.end(); ++it) {
			int32_t prop_type = (*it).id;
			double value = 0.0;
			switch(prop_type) {
				case Property_Type::PT_PHY_ATTACK: //物攻转化率
				case Property_Type::PT_SPELL_ATTACK: {//法攻转化率
					std::map<int, double>::const_iterator pro_it = pro_val_map.find(Property_Type::PT_FORCE);//斗力
					if(pro_it != pro_val_map.end()) {
						value = pro_it->second * it->value;
					}
					break;
				}
				case Property_Type::PT_PHY_DEFENSE://物防转化率
				case Property_Type::PT_SPELL_DEFENSE: {//法防转化率
					std::map<int, double>::const_iterator pro_it = pro_val_map.find(Property_Type::PT_ARMOR);//护甲
					if(pro_it != pro_val_map.end()) {
						value = pro_it->second * it->value;
					}
					break;
				}
				case Property_Type::PT_BLOOD_MAX: {//HP转化率
					std::map<int, double>::const_iterator pro_it = pro_val_map.find(Property_Type::PT_STAMINA);//耐力
					if(pro_it != pro_val_map.end()) {
						value = pro_it->second * it->value;
					}
					break;
				}
			}
			//判断是否有对应的二级属性
			std::map<int, double>::const_iterator pro_it = pro_val_map.find(prop_type);
			if(pro_it != pro_val_map.end()) {
				value += pro_it->second;
			}
			//往二级属性添加
			std::map<int, double>::const_iterator two_pro_it = out_pro_val_map.find(prop_type);
			if(two_pro_it == out_pro_val_map.end()) {
				out_pro_val_map.insert(std::make_pair(prop_type, value));
			}
		}
	}
}

void Helper::helper_listen_fighter_force(int new_force) {
	if(new_force <= helper_detail_->last_tip_force) {
		return;
	}
	helper_detail_->last_tip_force = new_force;
	helper_detail_->detail_change();
	if(player_self()->level() < 25) {
		return; //25级之前不触发
	}

	Time_Value now = Time_Value::gettimeofday();
	if(now.sec() < (last_listen_force_time.sec() + 5*Time_Value::ONE_MINUTE_IN_SECS)) {
		return; //半小时冷却
	}
	last_listen_force_time = now;//记录当前时间

	MSG_82200210 msg;
	msg.reset();
	if(helper_detail_->last_join_arena_force &&
	  ((new_force - helper_detail_->last_join_arena_force)*100)/helper_detail_->last_join_arena_force > 300) {
		msg.model_type.push_back(FORCE_CHANGE_ACTIVE_AREA);
	}//战力提升超过上次参加竞技场战力的300%时，触发
	if(helper_detail_->last_join_arena_force == 0 && new_force >= 2000 && !helper_detail_->no_join_areana_tip) {
		helper_detail_->no_join_areana_tip = true;
		helper_detail_->detail_change();
		msg.model_type.push_back(FORCE_CHANGE_ACTIVE_AREA);
	}// 如果没有参加过竞技场;战力达到一定值也提示参加竞技场,并只提示一次
	int elite_active_id = player_self()->get_normal_elite_active_id();
	int need_force = CONFIG_CACHE_DAILY->get_force_by_elite_explore_id(elite_active_id);
	if(elite_active_id && need_force && new_force >= need_force) {
		msg.model_type.push_back(FORCE_CHANGE_ACTIVE_ELITE_EXPLORE);
	}//战斗力达到下一个精英副本的设定战力时，触发提示。设定战力配置。
	THIS_SEND_TO_CLIENT(msg);
}

int Helper::req_upgrade_get_status(UInt_Vec get_type, std::string &tip, int32_t item_id, uint8_t isAuto) {
	MSG_50106005 msg;
	msg.reset();
	Upgrade_Get_Info gui;
	for(UInt_Vec::iterator it = get_type.begin(); it != get_type.end(); ++it) {
		gui.reset();
		gui.id = *it;
		gui.status = UPGRADE_GET_STATUS_ACTIVE;
		switch(*it) {
			case UPGRADE_HEROER_TEAM_FB_H://英雄之门
			case UPGRADE_HEROER_TEAM_FB: {//英雄之门
				if(player_self()->profile_num_fb_all() > 0) {//// 当前剩余收益次数
					gui.status = UPGRADE_GET_STATUS_ACTIVE;
				} else {
					gui.status = UPGRADE_GET_STATUS_NOT_OPEN;
				}
								// 配置的收益次数
				gui.cur_num = player_self()->profile_num_fb_all_cfg() - player_self()->profile_num_fb_all();
				gui.all_num = player_self()->profile_num_fb_all_cfg();
				break;
			}
			case UPGRADE_GET_ALCHEMY: {//炼金
				const int free_chances =  player_self()->get_alchemy_chances_free();
				const int pay_chances =  CONFIG_CACHE_ALCHEMY->get_chances_pay_by_vip_level(player_self()->vip());
				if(player_self()->level() < 23 ||
						(player_self()->alchemy_detail().used_free_bout >= free_chances&&
						player_self()->alchemy_detail().used_pay_bout >= pay_chances)){
					gui.status = UPGRADE_GET_STATUS_NOT_OPEN;
				} else {
					gui.status = UPGRADE_GET_STATUS_ACTIVE;
				}
				gui.cur_num = player_self()->alchemy_detail().used_free_bout + player_self()->alchemy_detail().used_pay_bout;
				gui.all_num = free_chances + pay_chances;
				break;
			}
			case UPGRADE_GET_ARENA: {//竞技
				break;
			}
			case UPGRADE_GET_ELITE_EXPLORE: {//精英副本
				break;
			}
			case UPGRADE_GET_REWARD_TASK: {//悬赏任务
				if(player_self()->is_can_do_reward_task()) {
					gui.status = UPGRADE_GET_STATUS_ACTIVE;
				} else {
					gui.status = UPGRADE_GET_STATUS_NOT_OPEN;
				}
				uint32_t total_num = player_self()->total_num_of_can_do_reward_task();
				uint32_t can_do_num = player_self()->total_num_of_can_do_reward_task();
				gui.cur_num = total_num - can_do_num;
				gui.all_num = total_num;
				break;
			}
			case UPGRADE_GET_TEAM_HOOK: {//英雄梦境
				if(player_self()->profile_num_hero_dream() > 0) {//// 当前剩余收益次数
					gui.status = UPGRADE_GET_STATUS_ACTIVE;
				} else {
					gui.status = UPGRADE_GET_STATUS_NOT_OPEN;
				}
				gui.cur_num = CONFIG_CACHE_TEAM->team_config().profile_num_hero_dream - player_self()->profile_num_hero_dream();
				gui.all_num = CONFIG_CACHE_TEAM->team_config().profile_num_hero_dream;
				break;
			}
			case UPGRADE_GET_GAIN_PROD: {//资源收获(龙谷)
				break;
			}
			case UPGRADE_GET_DRAGON_BRING_UP: {//龙培养
				break;
			}
			case UPGRADE_GET_WORLD_BOSS: {//世界boss
				uint32_t hour_min = 0;
				int status = ACTIVE_MANAGER->get_active_status(ACTION_ACTIVE_WORLD_BOSS, hour_min);
				gui.status = status;
				gui.time = hour_min;
				break;
			}
			case UPGRADE_GET_WARER: {//战场
				uint32_t hour_min = 0;
				int status = ACTIVE_MANAGER->get_active_status(ACTION_ACTIVE_WARER, hour_min);
				gui.status = status;
				gui.time = hour_min;
				break;
			}
			case UPGRADE_GET_LEISURE_ACTIVE: {//休闲活动 篝火舞会
				uint32_t hour_min = 0;
				int status = ACTIVE_MANAGER->get_active_status(ACTION_ACTIVE_CAMPFIRE, hour_min);
				gui.status = status;
				gui.time = hour_min;
				break;
			}
			case UPGRADE_GET_TEAM_ARENA: {//组队竞技
				uint32_t hour_min = 0;
				int status = ACTIVE_MANAGER->get_active_status(ACTION_ACTIVE_TAME_AREA, hour_min);
				gui.status = status;
				gui.time = hour_min;
				break;
			}
			case UPGRADE_GET_GANG_FIGHT: {//公会战
				uint32_t hour_min = 0;
				int status = ACTIVE_MANAGER->get_active_status(ACTION_ACTIVE_GANG_WARER, hour_min);
				gui.status = status;
				gui.time = hour_min;
				break;
			}
		}
		msg.info.push_back(gui);
	}
	msg.tip_name = tip;
	msg.goodsId = item_id;
	msg.isAuto = isAuto;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Helper::req_upgrade_score() {
	const Upgrade_Conf_Map conf_map = CONFIG_CACHE_UPGRADE->upgrade_conf_map();
	MSG_50106004 msg;
	msg.reset();
	Uint_Uint uu;
	for(Upgrade_Conf_Map::const_iterator it = conf_map.begin(); it != conf_map.end(); ++it) {
		uu.reset();
		switch(it->first) {
			case UPGRADE_SCORE_EQUIPER_BASE: {
				Int_Vec equip_index;
				//人物装备
				player_self()->get_equip_index(PACK_T_EQUIP_INDEX, equip_index);

				//英雄装备
				if(player_self()->heroer_detail().fighter_index) {
					int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP;
					for (int index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
						equip_index.push_back(index);
					}
				}
				//计算分数
				uint32_t sum = 0;
				const Item_Detail *equip = NULL;
				for (Int_Vec::const_iterator it_int = equip_index.begin(); it_int != equip_index.end(); ++it_int) {
					 equip = player_self()->pack_get_const_item(*it_int);
					 if(!equip) {
						 continue;
					 }
					 uint32_t index = equip->index_;
					 if(index > PACK_T_HERO_EQUIP_INDEX) {
						 if(player_self()->heroer_detail().fighter_index) {
							 int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP;
							 index = index - hero_pack_idx_gap + PACK_T_EQUIP_INDEX;
						 } else {
							 continue;
						 }
					 }
					 const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
					 if(!item_config) {
						 continue;
					 }
					 const uint32_t conf_value = CONFIG_CACHE_DAILY->get_can_equip_by_level_index(player_self()->level(), index);
					 if((uint32_t)item_config->needLevel >= conf_value && conf_value) {
						 ++sum;
					 }
					 //sum += player_self()->calculate_equip_force(equip->id_);//当前装备评分累加
				}
				//计算评分
				uu.val_1 = it->first;
				uu.val_2 = (int32_t)(sum/(16 + 0.0) * 100);
//				uu.val_1 = it->first;
//				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
//				if(expect_it != it->second.level_expect.end()) {
//					if(expect_it->second){
//						uu.val_2 = (int32_t)(sum/(expect_it->second + 0.0) * 100);
//					}
//				}
				break;
			}
			case UPGRADE_SCORE_EQUIPER_UNBAN_LVL: {
				Int_Vec equip_index;
				//人物装备
				player_self()->get_equip_index(PACK_T_EQUIP_INDEX, equip_index);

				//英雄装备
				if(player_self()->heroer_detail().fighter_index) {
					int hero_pack_idx_gap = PACK_T_HERO_EQUIP_INDEX + player_self()->heroer_detail().fighter_index * ONE_HERO_EQUIP_GAP;
					for (int index = hero_pack_idx_gap + EQUIP_BEGIN; index < hero_pack_idx_gap + EQUIP_END; ++index) {
						equip_index.push_back(index);
					}
				}
				//计算解封进度
				uint32_t sum = 0;
				uint32_t max_sum = 0;
				const Item_Detail *equip = NULL;
				for (Int_Vec::const_iterator it_int = equip_index.begin(); it_int != equip_index.end(); ++it_int) {
					 equip = player_self()->pack_get_const_item(*it_int);
					 if(!equip) {
						 continue;
					 }
					const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip->id_);
					if (! item_config) {
						continue;
					}
					 sum += equip->addon.equip.unban_lvl;//当前装备解封次数累加
					 max_sum += item_config->equip_cfg.max_unban_lvl;//当前装备最高可解封次数累加
				}
				//计算评分
				uu.val_1 = it->first;
				if(max_sum) {
					uu.val_2 =  (int32_t)(sum/(max_sum + 0.0) * 100);
				}
				break;
			}
			//int force = E_FUNC::calculate_force(pro_val_map);
			case UPGRADE_SCORE_GANG_SKILL: {
				uu.val_1 = it->first;
				std::map<int, double> pro_val_map;
				player_self()->get_ganger_skill_pro_val(pro_val_map);
				int force = E_FUNC::calculate_force(pro_val_map);
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(force/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_CARD: {
				uu.val_1 = it->first;
				std::map<int, double> pro_val_map;
				player_self()->get_all_embed_card_prop(pro_val_map);
				int force = E_FUNC::calculate_force(pro_val_map);
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(force/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_RUNE: {
				uu.val_1 = it->first;
				std::map<int, double> pro_val_map;
				player_self()->get_all_embed_gemstone_prop(pro_val_map);
				int force = E_FUNC::calculate_force(pro_val_map);
				//英雄
				//player_self()->get_hero_rune_pro_val(pro_val_map);
				//force += E_FUNC::calculate_force(pro_val_map);
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(force/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_HERO_LEVEL_UP: {
				uu.val_1 = it->first;
				int hero_index = player_self()->heroer_detail().fighter_index;
				int hero_lvl = player_self()->get_fight_hero_lvl(hero_index);
				int hero_lvl_limit = player_self()->get_fight_hero_lvl_limit(hero_index);
				if(hero_lvl_limit) {
					uu.val_2 = hero_lvl / (hero_lvl_limit + 0.0) * 100;
				}
				break;
			}
			case UPGRADE_SCORE_ENCHANT: {
				uu.val_1 = it->first;
				//这里获取的就是主角加英雄的
				int cur = player_self()->get_equips_enchant_star();
				int max = player_self()->get_equips_max_possible_enchant_star();
				if(max) {
					uu.val_2 = (int32_t)((cur/(max + 0.0)) * 100);
				}
				break;
			}
			case UPGRADE_SCORE_DRAGON_PROP: {
				uint32_t sum = player_self()->dragon_vale_detail().dragon_info.power_level +
						player_self()->dragon_vale_detail().dragon_info.armor_level +
						player_self()->dragon_vale_detail().dragon_info.stamina_level +
						player_self()->dragon_vale_detail().dragon_info.hero_power_level +
						player_self()->dragon_vale_detail().dragon_info.hero_armor_level +
						player_self()->dragon_vale_detail().dragon_info.hero_stamina_level +
						player_self()->dragon_vale_detail().dragon_info.crit_level +
						player_self()->dragon_vale_detail().dragon_info.prevent_crit_level +
						player_self()->dragon_vale_detail().dragon_info.hero_crit_level +
						player_self()->dragon_vale_detail().dragon_info.hero_prevent_crit_level;
				//计算评分
				uu.val_1 = it->first;
				uu.val_2 = (int32_t)(sum/(player_self()->level() * 6 + 0.0) * 100);

				break;
			}
			case UPGRADE_SCORE_ARTTIFACT: {
				uint32_t sum = 0;
				const Artifact_Map &artifacts = player_self()->public_detail().artifacts;
				for(Artifact_Map::const_iterator iter = artifacts.begin(); iter != artifacts.end(); ++iter) {
					sum += iter->second.artifact_lvl;
				}
				Heroer_Detail::Hero_Map::const_iterator hiter = player_self()->heroer_detail().hero_map.find(player_self()->heroer_detail().fighter_index);
				if(hiter !=  player_self()->heroer_detail().hero_map.end()) {
					for(Artifact_Map::const_iterator iter = hiter->second.artifacts.begin(); iter != hiter->second.artifacts.end(); ++iter) {
						sum += iter->second.artifact_lvl;
					}
				}
				uu.val_1 = it->first;
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(sum/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_RUNE_STONE: {
				uu.val_1 = it->first;
				std::map<int, double> pro_val_map;
				player_self()->get_rune_stone_prop(pro_val_map);
				int force = E_FUNC::calculate_force(pro_val_map);
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(force/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_MASTER: {
				int num = player_self()->get_master_effect_lvl_sum_on_player();
				num += player_self()->get_master_effect_lvl_sum_on_heroer();
				uu.val_1 = it->first;
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(num/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
//			case UPGRADE_SCORE_MOUNT: {
//				int num = player_self()->get_mount_num();
//				uu.val_1 = it->first;
//				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
//				if(expect_it != it->second.level_expect.end()) {
//					if(expect_it->second){
//						uu.val_2 = (int32_t)(num/(expect_it->second + 0.0) * 100);
//					}
//				}
//				break;
//			}
			case UPGRADE_SCORE_MOUNT: {
				uint32_t cur_num = 0, max_num = 0;
				std::vector<Horse_Info>::const_iterator iter;
				for(iter = player_self()->mount_detail().horse_list.begin(); iter != player_self()->mount_detail().horse_list.end(); ++iter){
					cur_num += iter->total_train_num;
					max_num += iter->next_lv_total_train_num;
				}
				//计算评分
				uu.val_1 = it->first;
				if(max_num) {
					uu.val_2 = (int32_t)(cur_num/(max_num + 0.0) * 100);
				}
				break;
			}
			case UPGRADE_SCORE_MOUNT_COLLECT: {
				int num = player_self()->mount_detail().horse_list.size();
				uu.val_1 = it->first;
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(num/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_WING: {
				int num = player_self()->wing_detail().lv;
				uu.val_1 = it->first;
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(num/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_FASHION: {
				int num = player_self()->get_fashion_strengthen_level();
				uu.val_1 = it->first;
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(num/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			case UPGRADE_SCORE_FASHION_COLLECT: {
				int num = player_self()->get_fashion_collect_num();
				uu.val_1 = it->first;
				Int_Int_Map::const_iterator expect_it = it->second.level_expect.find(player_self()->level());
				if(expect_it != it->second.level_expect.end()) {
					if(expect_it->second){
						uu.val_2 = (int32_t)(num/(expect_it->second + 0.0) * 100);
					}
				}
				break;
			}
			default: {
				uu.val_1 = it->first;
				uu.val_2 = 0;
				break;
			}
		}
		msg.upgrade.push_back(uu);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Helper::req_today_open_active(void) {
	MSG_50106003 msg;
	msg.reset();
	msg.open_active = ACTIVE_MANAGER->get_all_today_open_active();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Helper::req_helper_check_in(void) {
	if(helper_detail_->today_is_check_in == 0 && player_self()->is_icon_open(HELPER_COMMON_DIALY_ICON_ID)) {
		return helper_check_in();
	}
	return 0;
}

int Helper::helper_check_in(void) {
	const Check_In_Conf *conf = HELPER_MANAGER->get_check_in_conf_by_day(helper_detail_->check_in_day + 1);
	if(!conf) {
		return ERROR_INNER_SERVER;
	}
	int num = conf->num;
	if(conf->vip_double && conf->vip_double <= player_self()->vip()) {
		num *= 2;
	}
	MSG_81000102 acv_msg; //特效
	if(conf->exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf->num, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if(conf->item_id) {
		Item_Detail item(conf->item_id, num, Item_Detail::Bind_Type::BIND);
		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,item, Gain_Item_DM_Info(Pack::ITEM_GAIN_DAILY_SIGN));
		Item_Basic_Info ibi;
		item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
	}
	if(conf->money_id) {
		player_self()->pack_add_money(Money_Add_Info(Money_Type(conf->money_id), num, Pack::MONEY_ADD_DAILY_CHECK_IN));
		acv_msg.property.push_back(Property(conf->money_id, num));
	}
	++helper_detail_->check_in_day;
	helper_detail_->today_is_check_in = 1;
	helper_detail_->detail_change();
	send_check_in_msg();
	{
		acv_msg.type = 0;
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	MSG_50106006 msg;
	msg.reset();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Helper::send_check_in_msg(void) {
	MSG_82200201 msg;
	msg.reset();
	msg.check_in_day = helper_detail_->check_in_day;
	msg.group_id = HELPER_MANAGER->get_group_id();
	msg.refresh_time = HELPER_MANAGER->get_refresh_time().sec();
	msg.today_is_check_in = helper_detail_->today_is_check_in;
	THIS_SEND_TO_CLIENT(msg);
}

void Helper::test_set_today_is_check_in(void) { //todo 测试命令使用;要删除
	helper_detail_->today_is_check_in = 0;
	helper_detail_->detail_change();
	send_check_in_msg();
}

int Helper::get_fashion_strengthen_level(void) {
	int level = 0;
//	for(Fashion_Item_Map::const_iterator it = player_self()->fashion_detail().fashion_item_map_.begin();
//			it != player_self()->fashion_detail().fashion_item_map_.end(); ++it) {
//		if(it->second.state_ == Fashion_Item::STATE_ON) {
//			level += it->second.lv_;
//		}
//	}
	return level;
}

int Helper::get_fashion_collect_num(void) {
//	return player_self()->fashion_detail().fashion_item_map_.size();
	return 0;
}
