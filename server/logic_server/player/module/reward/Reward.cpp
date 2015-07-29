/*
 * Reward.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "Reward.h"
#include "Reward_Struct.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Reward.h"
#include "Msg_Active_Struct.h"
#include "Err_Code.h"
#include "pack/Pack_Def.h"
#include "pack/Pack_Struct.h"
#include "Logic_Player.h"
#include "Msg_Reward_Struct.h"
#include "Reward_Def.h"
#include "DB_Def.h"
#include "Msg_Reward_Enum.h"
#include "Reward_Manager.h"
#include "fighter/Logic_Fighter_Struct.h"

Reward::Reward(void) {
	reset();
}

Reward::~Reward() {
}

void Reward::reset(void) {
	reward_detail_ = 0;
	is_update_online_reward = false;
}

int Reward::create_init(Reward_Detail &detail) {

	return 0;
}

void Reward::load_detail(Reward_Detail &detail) {
	reward_detail_ = &detail;
}

void Reward::save_detail(Reward_Detail &detail) {
	if (reward_detail().is_change) {
		reward_detail_->last_retime = Time_Value::gettimeofday();
		detail = reward_detail();
		reward_detail_->is_change = false;
	}
}

int Reward::trigger_daily_zero(const Time_Value &now) {
	{//删除过期信息 - 奖励找回
		std::vector<long long int> del;
		del.clear();
		for( Timesec_UInt_UInt_Map::iterator iter = reward_detail_->reward_back_all_num.begin(); iter != reward_detail_->reward_back_all_num.end(); ++iter) {
			Time_Value time;
			time.sec(iter->first);
			if(get_six_time_subdays(time, Time_Value::gettimeofday()) > 7) {
				del.push_back(iter->first);
			}
		}
		for(std::vector<long long int>::iterator iter = del.begin(); iter != del.end(); ++iter) {
			Timesec_UInt_UInt_Map::iterator del_iter = reward_detail_->reward_back_all_num.find(*iter);
			if(del_iter != reward_detail_->reward_back_all_num.end()) {
				reward_detail_->reward_back_all_num.erase(del_iter);
			}
		}
	}
	//记录登陆天数
	++reward_detail_->login_days;
	reward_detail_->detail_change();
	return 0;
}

int Reward::trigger_daily_six(const Time_Value &now) {
	reward_detail_->online_reward_num = 0;
	reward_detail_->online_reward_time = Time_Value::zero;
	for(UInt_UInt_UInt_Map::iterator it = reward_detail_->reward_back_item_value_tmp.begin(); it != reward_detail_->reward_back_item_value_tmp.end(); ++it) {
		if(reward_detail_->reward_back_item_value[it->first][PT_EXP_CURRENT] <= it->second[PT_EXP_CURRENT]) {//按经验判断是否需要把最新的奖励安排到领取奖励里面
			//相等用最新 此操作是为了拿历史最高的奖励;对于那些要动态获取奖励找回的项目来说
			reward_detail_->reward_back_item_value[it->first] = it->second;
		}
	}
	reward_detail_->detail_change();
	is_update_online_reward = true;
	update_online_reward();
	send_online_reward_msg();
	req_reward_tip_num();
	return 0;
}

int Reward::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Reward::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Reward::move_scene_completed(void) {

	return 0;
}

int Reward::client_ready_now(void) {
//	if(player_self()->level() >= 20) {
		send_online_reward_msg();
//	}
//	if(player_self()->level() >= 15) {
		send_reward_back_msg();
//	}
	return 0;
}

void Reward::module_init(void) {

}

void Reward::sign_in(void) {
	if(!is_update_online_reward) {
		update_online_reward();
	}
}

void Reward::update_online_reward(void) {
	if(reward_detail_->last_online_refresh_time.sec() != REWARD_MANAGER->get_refresh_time().sec()) {
		reward_detail_->last_online_refresh_time = REWARD_MANAGER->get_refresh_time();
		reward_detail_->online_reward_num = 0;
		reward_detail_->online_reward_time = Time_Value::zero;
		reward_detail_->detail_change();
	}
	reward_detail_->online_reward_over = 0;
	const uint32_t online_reward_time = REWARD_MANAGER->get_online_gift_time_by_index(reward_detail_->online_reward_num);
	if(online_reward_time) {
		if(reward_detail_->online_reward_time == Time_Value::zero) {
			reward_detail_->online_reward_time.sec(Time_Value::gettimeofday().sec() + online_reward_time * Time_Value::ONE_MINUTE_IN_SECS);
		} else {
			//以防老号的问题
			if(reward_detail_->last_retime == Time_Value::zero) {
				reward_detail_->last_retime = Time_Value::gettimeofday();
			}
			reward_detail_->online_reward_time.sec(reward_detail_->online_reward_time.sec() +
					//(Time_Value::gettimeofday().sec() - reward_detail_->last_retime.sec()) //离线时长
					(Time_Value::gettimeofday().sec() - player_self()->base_detail().last_sign_out_time.sec()) //离线时长
			);//可领取时间,延后,延后时间为离线时长
		}
		//错误处理,如果时间超大,直接设置为当前时间
		if((reward_detail_->online_reward_time.sec() - Time_Value::gettimeofday().sec()) > ((long)online_reward_time * Time_Value::ONE_MINUTE_IN_SECS)) {
			reward_detail_->online_reward_time.sec(Time_Value::gettimeofday().sec() + online_reward_time * Time_Value::ONE_MINUTE_IN_SECS);
		}
	} else {
		reward_detail_->online_reward_over = 1;
	}

	reward_detail_->detail_change();
}

void Reward::sign_out(void) {

}

void Reward::trusteeship_sign_out(void) {

}

int Reward::sign_in_repeat(void) {

	return 0;
}

int Reward::req_reward_login_seven_info(void) {
	MSG_50106301 msg;
	msg.reset();
	Uint_Uint uu;
	const Login_Seven_Conf_Map conf = CONFIG_CACHE_REWARD->get_login_seven_config();
	for(Login_Seven_Conf_Map::const_iterator iter = conf.begin(); iter != conf.end(); ++iter) {
		uu.reset();
		uu.val_1 = iter->second.day;
		if(iter->second.day > reward_detail_->login_days) {
			uu.val_2 = 0;
		} else {
			UInt_Set::iterator it = reward_detail_->receive_seven_days.find(iter->second.day);
			if(it == reward_detail_->receive_seven_days.end()) {
				uu.val_2 = 1;
			} else {
				uu.val_2 = 2;
			}
		}
		msg.status.push_back(uu);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Reward::req_receive_login_seven(uint32_t days) {
	UInt_Set::iterator it = reward_detail_->receive_seven_days.find(days);
	if(it != reward_detail_->receive_seven_days.end()) {
		return ERROR_REWARD_ALREADY_RECEIVE;
	}
	return receive_login_seven(days);
}

int Reward::req_receive_activation_code_reward(std::string &code) {
//	Block_Buffer buf;
//	buf.make_message(MONGO_ACTIVATION_CODE_OP);
//	buf.write_uint32(ACTIVATION_CODE_OP_QUERY);
//	buf.write_string(code);
//	buf.write_int64(player_self()->role_id()); //请求需要角色ID
//	buf.finish_message();
//	DB_MANAGER->push_data_block(buf, 0);
	MSG_DEBUG("player_id: %ld req_receive_activation_code_reward code:%s", player_self()->role_id(), code.c_str());
	Activation_Code_Info code_info;
	REWARD_MANAGER->activation_code_decode(code, code_info);
	if(!code_info.type || !code_info.id) {
		return ERROR_ACTIVATION_CODE_INVALID;
	}
	if(REWARD_MANAGER->activation_code_has_receive_id(code_info.id*1000000+code_info.type)) {
		return ERROR_ACTIVATION_CODE_USED;
	}
	UInt_UInt_Map::iterator iter = reward_detail_->activation_code_receive_info.find(code_info.type);
	const Activation_Code_Config *conf = CONFIG_CACHE_REWARD->get_activation_code_conf_by_type(code_info.type);
	if(!conf) {//激活码对应类型没有配置
		return ERROR_CONFIG_NOT_EXIST;
	}
	if(iter != reward_detail_->activation_code_receive_info.end()) {
		if(iter->second && iter->second >= conf->num) {
			return ERROR_ACTIVATION_CODE_TYPE_NUM;
		}
	} else {
		std::pair< UInt_UInt_Map::iterator, bool > ret = reward_detail_->activation_code_receive_info.insert(std::make_pair(code_info.type, 0));
		if(ret.second){
			iter = ret.first;
		} else {
			return ERROR_INNER_SERVER;
		}
	}

	Item_Detail item(conf->item_id, 1, Item_Detail::BIND);
	int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(Pack::ITEM_GAIN_GIFT));
	if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
		return result;
	}
	++iter->second;
	reward_detail_->detail_change();
	REWARD_MANAGER->activation_code_receive_save_id(code_info.id*1000000+code_info.type);
	return 0;
}

int Reward::req_reward_back_receive(uint32_t type, uint32_t item_id) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	MSG_50106305 msg;
	msg.reset();
	msg.type = type;
	msg.item_id = item_id;
	switch(type) {
		case REWARD_BACK_RECEIVE_COPPER: {//= 1, //金钱追回
			const Reward_Back_Config *conf = CONFIG_CACHE_REWARD->get_reward_back_conf(item_id);
			if(!conf) {
				return ERROR_CLIENT_PARAM;
			}
			if(conf->is_be_group) {//如果是分组汇聚项,不参与计算
				return ERROR_CLIENT_PARAM;
			}

			//得到奖励
			Reward_Back_Info rbi;
			rbi.reset();
			get_reward_back_info_by_conf(*conf, rbi);
			if(!rbi.copper_cost || (!rbi.info_item.size() && !rbi.info_money.size() && !rbi.exp)) { //没有奖励可以领取
				return ERROR_CLIENT_PARAM;
			}

			{//检查是否够钱
				int result = player_self()->pack_try_sub_money(
						Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), rbi.copper_cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
				if(result) {
					return result;
				}
			}

			Reward_Back_Ratio_Cost_Map::const_iterator rc_iter = conf->ratio_cost_map.find(player_self()->level());//得到等级对应的消耗和奖励比例
			//计算奖励比例
			Money_Add_List money_list;
			money_list.clear();
			Item_Vec item_list;
			item_list.clear();
			for(std::vector<Uint_Uint>::iterator iter_money = rbi.info_money.begin(); iter_money != rbi.info_money.end(); ++iter_money) {
				//根据两份配置定位等级对应的奖励汇率
				int nums_ = 0;
				if(conf->copper_ratio) {
					nums_ = (iter_money->val_2 * conf->copper_ratio)/100.0 + 0.51;
				} else if (rc_iter != conf->ratio_cost_map.end()) {
					nums_ = (iter_money->val_2 * rc_iter->second.copper_ratio)/100.0 + 0.51;
				}
				if(nums_) {
					money_list.push_back(Money_Add_Info(Money_Type(iter_money->val_1), nums_, Money_DM_Info(MONEY_ADD_REWARD_BACK)));
					{
						Uint_Uint re_uu;
						re_uu.reset();
						re_uu.val_1 = iter_money->val_1;
						re_uu.val_2 = nums_;
						msg.money_info.push_back(re_uu);
					}
					acv_msg.property.push_back(Property(iter_money->val_1, nums_));
				}
			}
			for(std::vector<Uint_Uint>::iterator iter_item = rbi.info_item.begin(); iter_item != rbi.info_item.end(); ++iter_item) {
				//根据两份配置定位等级对应的奖励汇率
				int nums_ = 0;
				if(conf->copper_ratio) {
					nums_ = (iter_item->val_2 * conf->copper_ratio)/100.0 + 0.51;
				} else if (rc_iter != conf->ratio_cost_map.end()) {
					nums_ = (iter_item->val_2 * rc_iter->second.copper_ratio)/100.0 + 0.51;
				}
				if(nums_) {
					Item_Detail item(iter_item->val_1, nums_, Item_Detail::BIND);
					item_list.push_back(item);
					{
						Uint_Uint re_uu;
						re_uu.reset();
						re_uu.val_1 = iter_item->val_1;
						re_uu.val_2 = nums_;
						msg.item_info.push_back(re_uu);
					}
					{
						Item_Basic_Info ibi;
						item.set_item_basic_info(ibi);
						acv_msg.item_info.push_back(ibi);
					}
				}
			}

			if(!money_list.empty()) {
				//判断金钱是否能插入成功
				int result = player_self()->pack_try_add_money(money_list);
				if(result) {
					return result;
				}
			}
			{//扣钱
				int result = player_self()->pack_sub_money(
						Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), rbi.copper_cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
				if(result) {
					return result;
				}
			}
			if(!money_list.empty()) {
				player_self()->pack_add_money(money_list);
			}
			if(!item_list.empty()) {
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_REWARD_BACK));
			}
			{//加经验
				int exp =0;
				if(conf->copper_ratio) {
					exp = (rbi.exp * conf->copper_ratio)/100.0 + 0.51;
				} else if (rc_iter != conf->ratio_cost_map.end()) {
					exp = (rbi.exp * rc_iter->second.copper_ratio)/100.0 + 0.51;
				}
				if(exp) {
					Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, false, true);
					if (0 == exp_all.result) {
						acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
						acv_msg.vip_exp = exp_all.vip_exp;
						acv_msg.world_level = player_self()->get_world_level_add();
					}
					msg.exp = exp_all.all;
				}
			}

			//更新奖励最后获取时间
			update_reward_back_reveice_time(conf->id);
			break;
		}
		case REWARD_BACK_RECEIVE_GOLD : {//= 2,   //钻石追回
			if(player_self()->vip() == 0) {
				return ERROR_CLIENT_PARAM;
			}
			const Reward_Back_Config *conf = CONFIG_CACHE_REWARD->get_reward_back_conf(item_id);
			if(!conf) {
				return ERROR_CLIENT_PARAM;
			}
			if(conf->is_be_group) {//如果是分组汇聚项,不参与计算
				return ERROR_CLIENT_PARAM;
			}
			//得到奖励
			Reward_Back_Info rbi;
			rbi.reset();
			get_reward_back_info_by_conf(*conf, rbi);
			if(!rbi.gold_cost || (!rbi.info_item.size() && !rbi.info_money.size() && !rbi.exp)) { //没有奖励可以领取
				return ERROR_CLIENT_PARAM;
			}

			{//检查是否够钱
				int result = player_self()->pack_try_sub_money(
						Money_Sub_Info(Money_Sub_Type(conf->gold_cost_type), rbi.gold_cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
				if(result) {
					return result;
				}
			}
			Reward_Back_Ratio_Cost_Map::const_iterator rc_iter = conf->ratio_cost_map.find(player_self()->level());//得到等级对应的消耗和奖励比例
			//计算奖励比例
			Money_Add_List money_list;
			money_list.clear();
			Item_Vec item_list;
			item_list.clear();
			for(std::vector<Uint_Uint>::iterator iter_money = rbi.info_money.begin(); iter_money != rbi.info_money.end(); ++iter_money) {
				int nums_ = 0;
				if(conf->gold_ratio) {
					nums_ = (iter_money->val_2 * conf->gold_ratio)/100.0 + 0.51;
				} else if(rc_iter != conf->ratio_cost_map.end()) {
					nums_ = (iter_money->val_2 * rc_iter->second.gold_ratio)/100.0 + 0.51;
				}
				if(nums_) {
					money_list.push_back(Money_Add_Info(Money_Type(iter_money->val_1), nums_, Money_DM_Info(MONEY_ADD_REWARD_BACK)));
					{
						Uint_Uint re_uu;
						re_uu.reset();
						re_uu.val_1 = iter_money->val_1;
						re_uu.val_2 = nums_;
						msg.money_info.push_back(re_uu);
					}
					acv_msg.property.push_back(Property(iter_money->val_1, nums_));
				}
			}
			for(std::vector<Uint_Uint>::iterator iter_item = rbi.info_item.begin(); iter_item != rbi.info_item.end(); ++iter_item) {
				int nums_ = 0;
				if(conf->gold_ratio) {
					nums_ = (iter_item->val_2 * conf->gold_ratio)/100.0 + 0.51;
				} else if(rc_iter != conf->ratio_cost_map.end()) {
					nums_ = (iter_item->val_2 * rc_iter->second.gold_ratio)/100.0 + 0.51;
				}
				if(nums_) {
					Item_Detail item(iter_item->val_1, nums_, Item_Detail::BIND);
					item_list.push_back(item);
					{
						Uint_Uint re_uu;
						re_uu.reset();
						re_uu.val_1 = iter_item->val_1;
						re_uu.val_2 = nums_;
						msg.item_info.push_back(re_uu);
					}
					{
						Item_Basic_Info ibi;
						item.set_item_basic_info(ibi);
						acv_msg.item_info.push_back(ibi);
					}
				}
			}
			if(!money_list.empty()) {
				//判断金钱是否能插入成功
				int result = player_self()->pack_try_add_money(money_list);
				if(result) {
					return result;
				}
			}
			{//扣钱
				int result = player_self()->pack_sub_money(
						Money_Sub_Info(Money_Sub_Type(conf->gold_cost_type), rbi.gold_cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
				if(result) {
					return result;
				}
			}
			if(!money_list.empty()) {
				player_self()->pack_add_money(money_list);
			}
			if(!item_list.empty()) {
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_REWARD_BACK));
			}
			{//加经验
				int exp = 0;
				if(conf->gold_ratio) {
					exp = (rbi.exp * conf->gold_ratio)/100.0 + 0.51;
				} else if (rc_iter != conf->ratio_cost_map.end()) {
					exp = (rbi.exp * rc_iter->second.gold_ratio)/100.0 + 0.51;
				}
				if(exp) {
					Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, false, true);
					if (0 == exp_all.result) {
						acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
						acv_msg.vip_exp = exp_all.vip_exp;
						acv_msg.world_level = player_self()->get_world_level_add();
					}
					msg.exp = exp_all.all;
				}
			}

			//更新奖励最后获取时间
			update_reward_back_reveice_time(conf->id);
			break;
		}
		case REWARD_BACK_RECEIVE_ALL_COPPER : {//= 3,//一键金钱追回
			const Reward_Back_Conf_Map *conf = CONFIG_CACHE_REWARD->get_reward_back_map();
			if(!conf) {
				return ERROR_CLIENT_PARAM;
			}
			uint32_t cost = 0; //需要花费的金额
			//得到奖励
			Money_Add_List money_list;
			money_list.clear();
			Item_Vec item_list;
			item_list.clear();
			int exp = 0;
			{
				Reward_Back_Info rbi;
				for(Reward_Back_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					rbi.reset();
					if(iter->second.is_be_group) {//如果是分组汇聚项,不参与计算
						continue;
					}
					get_reward_back_info_by_conf(iter->second, rbi);
					if(!rbi.copper_cost || (!rbi.info_item.size() && !rbi.info_money.size() && !rbi.exp)) { //没有奖励可以领取
						continue;
					}
					Reward_Back_Ratio_Cost_Map::const_iterator rc_iter = iter->second.ratio_cost_map.find(player_self()->level());//得到等级对应的消耗和奖励比例
					cost += rbi.copper_cost; //累计需要花费的金额
					//计算奖励比例
					for(std::vector<Uint_Uint>::iterator iter_money = rbi.info_money.begin(); iter_money != rbi.info_money.end(); ++iter_money) {
						int nums_ = 0;
						if(iter->second.copper_ratio) {
							nums_ = (iter_money->val_2 * iter->second.copper_ratio)/100.0 + 0.51;
						} else if(rc_iter != iter->second.ratio_cost_map.end()) {
							nums_ = (iter_money->val_2 * rc_iter->second.copper_ratio)/100.0 + 0.51;
						}
						if(nums_) {
							money_list.push_back(Money_Add_Info(Money_Type(iter_money->val_1), nums_, Money_DM_Info(MONEY_ADD_REWARD_BACK)));
							{
								Uint_Uint re_uu;
								re_uu.reset();
								re_uu.val_1 = iter_money->val_1;
								re_uu.val_2 = nums_;
								msg.money_info.push_back(re_uu);
							}
							acv_msg.property.push_back(Property(iter_money->val_1, nums_));
						}
					}
					//道具
					for(std::vector<Uint_Uint>::iterator iter_item = rbi.info_item.begin(); iter_item != rbi.info_item.end(); ++iter_item) {
						int nums_ = 0;
						if(iter->second.copper_ratio) {
							nums_ = (iter_item->val_2 * iter->second.copper_ratio)/100.0 + 0.51;
						} else if(rc_iter != iter->second.ratio_cost_map.end()) {
							nums_ = (iter_item->val_2 * rc_iter->second.copper_ratio)/100.0 + 0.51;
						}
						if(nums_) {
							Item_Detail item(iter_item->val_1, nums_, Item_Detail::BIND);
							item_list.push_back(item);
							{
								Uint_Uint re_uu;
								re_uu.reset();
								re_uu.val_1 = iter_item->val_1;
								re_uu.val_2 = nums_;
								msg.item_info.push_back(re_uu);
							}
							{
								Item_Basic_Info ibi;
								item.set_item_basic_info(ibi);
								acv_msg.item_info.push_back(ibi);
							}
						}
					}
					//累计加经验
					if(iter->second.copper_ratio) {
						exp += (rbi.exp * iter->second.copper_ratio)/100.0 + 0.51;
					} else if(rc_iter != iter->second.ratio_cost_map.end()) {
						exp += (rbi.exp * rc_iter->second.copper_ratio)/100.0 + 0.51;
					}
				}//循环配置结束
			}
			if(cost <= 0) {
				return ERROR_CLIENT_PARAM;//肯定有错;如果为0是什么都不能领
			}
			{//检查是否够钱
				int result = player_self()->pack_try_sub_money(
						Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
				if(result) {
					return result;
				}
			}

			if(!money_list.empty()) {
				//判断金钱是否能插入成功
				int result = player_self()->pack_try_add_money(money_list);
				if(result) {
					return result;
				}
			}
			{//扣钱
				int result = player_self()->pack_sub_money(
						Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
				if(result) {
					return result;
				}
			}
			if(!money_list.empty()) {
				player_self()->pack_add_money(money_list);
			}
			if(!item_list.empty()) {
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_REWARD_BACK));
			}
			{//加经验
				if(exp) {
					Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, false, true);
					if (0 == exp_all.result) {
						acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
						acv_msg.vip_exp = exp_all.vip_exp;
						acv_msg.world_level = player_self()->get_world_level_add();
					}
					msg.exp = exp_all.all;
				}
			}

			//更新奖励最后获取时间
			for(Reward_Back_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				update_reward_back_reveice_time(iter->second.id);
			}
			break;
		}
		case REWARD_BACK_RECEIVE_ALL_GOLD : {//= 4,//至尊追回
			if(player_self()->vip() == 0) {
				return ERROR_CLIENT_PARAM;
			}
			const Reward_Back_Conf_Map *conf = CONFIG_CACHE_REWARD->get_reward_back_map();
			if(!conf) {
				return ERROR_CLIENT_PARAM;
			}
			Money_Sub_List sub_list;//记录所需花费的金额
			sub_list.clear();

			//得到奖励
			Money_Add_List money_list;
			money_list.clear();
			Item_Vec item_list;
			item_list.clear();
			int exp = 0;
			{
				Reward_Back_Info rbi;
				for(Reward_Back_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
					rbi.reset();
					if(iter->second.is_be_group) {//如果是分组汇聚项,不参与计算
						continue;
					}
					get_reward_back_info_by_conf(iter->second, rbi);
					if(!rbi.gold_cost || (!rbi.info_item.size() && !rbi.info_money.size() && !rbi.exp)) { //没有奖励可以领取
						continue;
					}
					Reward_Back_Ratio_Cost_Map::const_iterator rc_iter = iter->second.ratio_cost_map.find(player_self()->level());//得到等级对应的消耗和奖励比例
					//记录所需花费的金额
					sub_list.push_back(Money_Sub_Info(Money_Sub_Type(iter->second.gold_cost_type), rbi.gold_cost, MONEY_SUB_MONEY_REWARD_BACK_COST));
					//计算奖励比例
					for(std::vector<Uint_Uint>::iterator iter_money = rbi.info_money.begin(); iter_money != rbi.info_money.end(); ++iter_money) {
						int nums_ = 0;
						if(iter->second.gold_ratio) {
							nums_ = (iter_money->val_2 * iter->second.gold_ratio)/100.0 + 0.51;
						} else if(rc_iter != iter->second.ratio_cost_map.end()) {
							nums_ = (iter_money->val_2 * rc_iter->second.gold_ratio)/100.0 + 0.51;
						}
						if(nums_) {
							money_list.push_back(Money_Add_Info(Money_Type(iter_money->val_1), nums_, Money_DM_Info(MONEY_ADD_REWARD_BACK)));
							{
								Uint_Uint re_uu;
								re_uu.reset();
								re_uu.val_1 = iter_money->val_1;
								re_uu.val_2 = nums_;
								msg.money_info.push_back(re_uu);
							}
							acv_msg.property.push_back(Property(iter_money->val_1, nums_));
						}
					}
					for(std::vector<Uint_Uint>::iterator iter_item = rbi.info_item.begin(); iter_item != rbi.info_item.end(); ++iter_item) {
						int nums_ = 0;
						if(iter->second.gold_ratio) {
							nums_ = (iter_item->val_2 * iter->second.gold_ratio)/100.0 + 0.51;
						} else if(rc_iter != iter->second.ratio_cost_map.end()) {
							nums_ = (iter_item->val_2 * rc_iter->second.gold_ratio)/100.0 + 0.51;
						}
						if(nums_) {
							Item_Detail item(iter_item->val_1, nums_, Item_Detail::BIND);
							item_list.push_back(item);
							{
								Uint_Uint re_uu;
								re_uu.reset();
								re_uu.val_1 = iter_item->val_1;
								re_uu.val_2 = nums_;
								msg.item_info.push_back(re_uu);
							}
							{
								Item_Basic_Info ibi;
								item.set_item_basic_info(ibi);
								acv_msg.item_info.push_back(ibi);
							}
						}
					}
					//累计加经验
					if(iter->second.gold_ratio) {
						exp += (rbi.exp * iter->second.gold_ratio)/100.0 + 0.51;
					} else if(rc_iter != iter->second.ratio_cost_map.end()) {
						exp += (rbi.exp * rc_iter->second.gold_ratio)/100.0 + 0.51;
					}
				}
			}
			if(sub_list.empty()) {
				return ERROR_CLIENT_PARAM;//都不能领
			}
			{//检查是否够钱
				int result = player_self()->pack_try_sub_money(sub_list);
				if(result) {
					return result;
				}
			}

			if(!money_list.empty()) {
				//判断金钱是否能插入成功
				int result = player_self()->pack_try_add_money(money_list);
				if(result) {
					return result;
				}
			}
			{//扣钱
				int result = player_self()->pack_sub_money(sub_list);
				if(result) {
					return result;
				}
			}
			if(!money_list.empty()) {
				player_self()->pack_add_money(money_list);
			}
			if(!item_list.empty()) {
				player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_REWARD_BACK));
			}
			{//加经验
				if(exp) {
					Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, false, true);
					if (0 == exp_all.result) {
						acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
						acv_msg.vip_exp = exp_all.vip_exp;
						acv_msg.world_level = player_self()->get_world_level_add();
					}
					msg.exp = exp_all.all;
				}
			}
			//更新奖励最后获取时间
			for(Reward_Back_Conf_Map::const_iterator iter = conf->begin(); iter != conf->end(); ++iter) {
				update_reward_back_reveice_time(iter->second.id);
			}
			break;
		}
		default: {
			return ERROR_CLIENT_PARAM;
		}
	}
	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	//返回客户端消息
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Reward::req_have_receive_keep_reward(void) {
//	if(reward_detail_->keep_reward_status == KEEP_REWARD_IS_RECEIVE) {
//		return ERROR_HAS_RECEIVE_KEEP_REWARD;
//	}
	MSG_50106306 msg;
	msg.reset();
	msg.keep_reward_status = reward_detail_->keep_reward_status;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Reward::req_keep_reward_success(void) {
	reward_detail_->keep_reward_status = KEEP_REWARD_CAN_RECEIVE;
	reward_detail_->detail_change();
	return 0;
}

int Reward::req_receive_keep_reward(void) {
	if(reward_detail_->keep_reward_status == KEEP_REWARD_IS_RECEIVE) {
		return ERROR_HAS_RECEIVE_KEEP_REWARD;
	}
	const Keep_Reward_Config *conf = CONFIG_CACHE_REWARD->get_keep_reward_config();
	if(conf) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		//发放奖励
		Item_Vec item_list;
		for(UInt_UInt_Map::const_iterator iter_item = conf->reward_item.begin(); iter_item != conf->reward_item.end(); ++iter_item) {
			Item_Detail item(iter_item->first, iter_item->second, Item_Detail::BIND);
			item_list.push_back(item);
			{
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
			}
		}
		if(!item_list.empty()) {
			player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_KEEP_REWARD));
		}
		Money_Add_List money_list;
		money_list.clear();
		for(UInt_UInt_Map::const_iterator iter_money = conf->reward_money.begin(); iter_money != conf->reward_money.end(); ++iter_money) {
			money_list.push_back(Money_Add_Info(Money_Type(iter_money->first), iter_money->second, Money_DM_Info(MONEY_ADD_KEEP_REWARD)));
			acv_msg.property.push_back(Property(iter_money->first, iter_money->second));
		}
		if(!money_list.empty()) {
			player_self()->pack_add_money(money_list);
		}
		if(conf->exp) {
			Exp_All exp_all = player_self()->modify_experience_inter(conf->exp, false, false, false, true);
			if (0 == exp_all.result) {
				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
				acv_msg.vip_exp = exp_all.vip_exp;
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}

		reward_detail_->keep_reward_status = KEEP_REWARD_IS_RECEIVE;
		reward_detail_->detail_change();
		if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		MSG_50106308 msg;
		msg.reset();
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Reward::req_reward_back_info(void) {
	const Reward_Back_Conf_Map *conf_map = CONFIG_CACHE_REWARD->get_reward_back_map();
	if(conf_map) {
		MSG_50106304 msg;
		msg.reset();
		Reward_Back_Info rbi;
		for(Reward_Back_Conf_Map::const_iterator iter = conf_map->begin(); iter != conf_map->end(); ++iter) {
			if(iter->second.is_be_group) {
				continue; //如果是汇聚项; 不参与显示
			}
			rbi.reset();
			get_reward_back_info_by_conf(iter->second, rbi);
			if(rbi.info_item.size() || rbi.info_money.size() || rbi.exp) {
				msg.list.push_back(rbi);
			}
		}
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

//int Reward::process_activation_code_data(Activation_Code_Info &code_info) {
//	if(code_info.code.empty() && !code_info.type) {
//		return player_self()->send_error_to_client(ERROR_ACTIVATION_CODE_INVALID, REQ_RECEIVE_ACTIVATION_CODE_REWARD);
//	}
//	if(code_info.status) {
//		return player_self()->send_error_to_client(ERROR_ACTIVATION_CODE_USED, REQ_RECEIVE_ACTIVATION_CODE_REWARD);
//	}
//	UInt_UInt_Map::iterator iter = reward_detail_->activation_code_receive_info.find(code_info.type);
//	const Activation_Code_Config *conf = CONFIG_CACHE_REWARD->get_activation_code_conf_by_type(code_info.type);
//	if(!conf) {//激活码对应类型没有配置
//		return player_self()->send_error_to_client(ERROR_CONFIG_NOT_EXIST, REQ_RECEIVE_ACTIVATION_CODE_REWARD);
//	}
//	if(iter != reward_detail_->activation_code_receive_info.end()) {
//		if(iter->second && iter->second >= conf->num) {
//			return player_self()->send_error_to_client(ERROR_ACTIVATION_CODE_TYPE_NUM, REQ_RECEIVE_ACTIVATION_CODE_REWARD);
//		}
//	} else {
//		std::pair< UInt_UInt_Map::iterator, bool > ret = reward_detail_->activation_code_receive_info.insert(std::make_pair(code_info.type, 0));
//		if(ret.second){
//			iter = ret.first;
//		} else {
//			return player_self()->send_error_to_client(ERROR_INNER_SERVER, REQ_RECEIVE_ACTIVATION_CODE_REWARD);
//		}
//	}
//
//	Item_Detail item(conf->item_id, 1, Item_Detail::BIND);
//	int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(Pack::ITEM_GAIN_GIFT));
//	if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
//		return result;
//	}
//	++iter->second;
//	reward_detail_->detail_change();
//	//更新数据库激活码状态
//	Block_Buffer buf;
//	buf.make_message(MONGO_ACTIVATION_CODE_OP);
//	buf.write_uint32(ACTIVATION_CODE_OP_UPDATE);
//	buf.write_string(code_info.code);
//	buf.finish_message();
//	DB_MANAGER->push_data_block(buf, 0);
//
//	{
//		MSG_81000102 acv_msg;
//		acv_msg.type = 0;
//		Item_Basic_Info ibi;
//		item.set_item_basic_info(ibi);
//		acv_msg.item_info.push_back(ibi);
//		THIS_SEND_TO_CLIENT(acv_msg);
//	}
//	//通知客户端
//	MSG_50106303 msg;
//	msg.reset();
//	THIS_SEND_TO_CLIENT(msg);
//	return 0;
//}

int Reward::req_reward_tip_num(void) {
	MSG_50106309 msg;
	msg.reset();
	msg.num = (reward_detail_->login_days > 7?7:reward_detail_->login_days) - reward_detail_->receive_seven_days.size();
	const Reward_Back_Conf_Map *conf_map = CONFIG_CACHE_REWARD->get_reward_back_map();
	if(conf_map) {
		Reward_Back_Info rbi;
		for(Reward_Back_Conf_Map::const_iterator iter = conf_map->begin(); iter != conf_map->end(); ++iter) {
			if(iter->second.is_be_group) {
				continue; //如果是汇聚项; 不参与显示
			}
			rbi.reset();
			get_reward_back_info_by_conf(iter->second, rbi);
			if(rbi.info_item.size() || rbi.info_money.size() || rbi.exp) {
				++msg.num;
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Reward::req_receive_online_reward(void) {
	if(reward_detail_->online_reward_time.sec() > Time_Value::gettimeofday().sec()) {
		MSG_USER("reward time error role:%ld %ld  %ld", role_id(), reward_detail_->online_reward_time.sec(), Time_Value::gettimeofday().sec());
		return ERROR_ONLINE_REWARD_NOT_ENOUGH_TIME;
	}
	return receive_online_reward();
}

void Reward::send_online_reward_msg(void) {
	MSG_82200202 msg;
	msg.reset();
	msg.online_id = REWARD_MANAGER->get_online_gift_id_by_index(reward_detail_->online_reward_num);
	msg.online_reward_over = reward_detail_->online_reward_over;
	msg.online_reward_time = reward_detail_->online_reward_time.sec();
	MSG_DEBUG("reward time update role:%ld %ld  %ld", role_id(), reward_detail_->online_reward_time.sec(), Time_Value::gettimeofday().sec());
	THIS_SEND_TO_CLIENT(msg);
}

void Reward::send_reward_back_msg(void) {
	MSG_81000408 msg;
	msg.open = 1;
	msg.active = 1;
	msg.time = 0;
	msg.icon_id = 4;
	THIS_SEND_TO_CLIENT(msg);
}

int Reward::reward_back_listen_fulfill(uint32_t item_id) {
	UInt_UInt_Map award_map;
	award_map.clear();
	return reward_back_listen_fulfill(item_id, award_map);
}

int Reward::reward_back_listen_accumulated_rewards(uint32_t id, const Int_Int_Map &reward_item,const uint32_t uniqid) {
	const Reward_Back_Config *conf = CONFIG_CACHE_REWARD->get_reward_back_conf(id);
	if(conf)  {//记录最后一次奖励
		UInt_UInt_UInt_Map::iterator iter = reward_detail_->reward_back_item_value_tmp.find(conf->id);
		UInt_UInt_Map::iterator record_it = reward_detail_->reward_back_item_record.find(conf->id);
		if(iter != reward_detail_->reward_back_item_value_tmp.end()) {
			if(record_it == reward_detail_->reward_back_item_record.end() || record_it->second != uniqid) {
				iter->second.clear();
			}
			for(Int_Int_Map::const_iterator it = reward_item.begin(); it != reward_item.end(); ++it) {
				//MSG_DEBUG("reward back_list accumulated rewards id:%d, int:%d, int:%d , uniqid:%d",id, it->first, it->second, uniqid);
				if(it->second <= 0) {
					MSG_USER_TRACE("reward back_list accumulated rewards NUM ERROR");
					continue;
				}
				iter->second[it->first] += it->second;
			}
		} else {
			UInt_UInt_Map reward_item_;
			reward_item_.clear();
			for(Int_Int_Map::const_iterator it = reward_item.begin(); it != reward_item.end(); ++it) {
				//MSG_DEBUG("reward back_list accumulated rewards id:%d, int:%d, int:%d , uniqid:%d",id, it->first, it->second, uniqid);
				if(it->second <= 0) {
					MSG_USER_TRACE("reward back_list accumulated rewards NUM ERROR");
					continue;
				}
				reward_item_[it->first] += it->second;
			}
			reward_detail_->reward_back_item_value_tmp.insert(std::make_pair(conf->id, reward_item_));
		}
		//record
		reward_detail_->reward_back_item_record[conf->id] = uniqid;
	}
	return 0;
}

int Reward::reward_back_listen_fulfill(uint32_t item_id, UInt_UInt_Map &reward_item) {
	const Reward_Back_Config *conf = CONFIG_CACHE_REWARD->get_reward_back_conf(item_id);
	if(conf) {
		Time_Value today_zero = Time_Value::zero; //求出今日凌晨时间
		get_six_time(Time_Value::gettimeofday(), today_zero);
		if(today_zero == Time_Value::zero) {
			return -1;
		}
		{//记录最后一次奖励
			UInt_UInt_UInt_Map::iterator iter = reward_detail_->reward_back_item_value.find(conf->id);
			if(iter != reward_detail_->reward_back_item_value.end()) {
				if(!reward_item.empty()) {//有的只监听完成次数; 没有传对应奖励的,奖励是分批传过来的
					iter->second = reward_item;
				}
			} else {
				reward_detail_->reward_back_item_value.insert(std::make_pair(conf->id, reward_item));
			}
		}
		{//记录和判断次数
			Timesec_UInt_UInt_Map::iterator iter = reward_detail_->reward_back_all_num.find(today_zero.sec());
			if(iter != reward_detail_->reward_back_all_num.end()) {//当天有记录
				UInt_UInt_Map::iterator num_iter = iter->second.find(conf->id);
				if(num_iter != iter->second.end()) {//当天有项目次数
					if(num_iter->second < conf->day_num){//项目完成次数还没达到配置要求的最大次数
						++num_iter->second;
					}
				} else {//当天没有该项目次数
					iter->second.insert(std::make_pair(conf->id, 1));
				}
			} else {//当天没记录
				UInt_UInt_Map reward_all_num;
				reward_all_num.clear();
				reward_all_num.insert(std::make_pair(conf->id, 1));
				reward_detail_->reward_back_all_num.insert(std::make_pair(today_zero.sec(), reward_all_num));
			}
		}
		{//记录项目的第一次完成时间
			if(reward_detail_->reward_back_first_time.find(conf->id) == reward_detail_->reward_back_first_time.end()) {
				reward_detail_->reward_back_first_time.insert(std::make_pair(conf->id, Time_Value::gettimeofday()));
			}

		}


		reward_detail_->detail_change();
	}
	return 0;
}

int Reward::receive_online_reward(void) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	{//领取奖励
		const Online_Gift_Config *conf = REWARD_MANAGER->get_online_gift_conf(reward_detail_->online_reward_num);
		if(!conf) {
			return ERROR_CLIENT_OPERATE;
		}
		{
			Money_Add_List money_list;
			money_list.clear();
			for(Online_Money_Conf_Vec::const_iterator iter = conf->money_list.begin(); iter != conf->money_list.end(); ++iter) {
				money_list.push_back(Money_Add_Info(Money_Type(iter->money_id), iter->num, Money_DM_Info(Pack::MONEY_ADD_ONLINE_REWARD)));
				acv_msg.property.push_back(Property(iter->money_id, iter->num));
			}
			if(money_list.size()) {
				int result = player_self()->pack_add_money(money_list);
				if(result) {
					return result;
				}
			}
		}
		{
			Item_Vec item_list;
			item_list.clear();
			for(Online_Item_Conf_Vec::const_iterator iter = conf->item_list.begin(); iter != conf->item_list.end(); ++iter) {
				Item_Detail item(iter->item_id, iter->num, Item_Detail::BIND);
				item_list.push_back(item);
				{
					Item_Basic_Info ibi;
					item.set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
				}
			}
			if(item_list.size()) {
				int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_ONLINE_AWARD));
				if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
					return result;
				}
			}
		}
	}
	{//刷新下一个奖励
		const Online_Gift_Config *conf = REWARD_MANAGER->get_online_gift_conf(++reward_detail_->online_reward_num);
		if(conf) {
			reward_detail_->online_reward_over = 0;
			reward_detail_->online_reward_time.sec(Time_Value::gettimeofday().sec() + conf->online_time * 60);
		} else {
			reward_detail_->online_reward_over = 1;
			reward_detail_->online_reward_time = Time_Value::zero;
		}
	}
	reward_detail_->detail_change();
	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	send_online_reward_msg();
	return 0;
}

int Reward::receive_login_seven(uint32_t days) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	{//领取奖励
		const Login_Seven_Config *conf = CONFIG_CACHE_REWARD->get_login_seven_conf_by_day(days);
		if(!conf) {
			return ERROR_CLIENT_OPERATE;
		}
		Item_Detail item(conf->item_id, 1, Item_Detail::BIND);
		int result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(Pack::ITEM_GAIN_CON_LOGIN));
		if(result && ERROR_PACK_FULL_BUT_MAIL != result) {
			return result;
		}
		Item_Basic_Info ibi;
		item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
	}
	reward_detail_->receive_seven_days.insert(days);
	reward_detail_->detail_change();

	if (!acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	//回应客户端
	MSG_50106302 msg;
	msg.reset();
	msg.days = days;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

uint32_t Reward::get_reward_back_fulfill_num_by_id(uint32_t id, uint32_t pid) {
	uint32_t num = 0;
	for( Timesec_UInt_UInt_Map::iterator iter = reward_detail_->reward_back_all_num.begin(); iter != reward_detail_->reward_back_all_num.end(); ++iter) {
		Time_Value time;
		time.sec(iter->first);
		Time_Value last_receive_time = Time_Value::zero;
		UInt_Time_Map::iterator it = reward_detail_->reward_back_receive_time.find(id);
		if(pid) { //如果是分组的子项目; 按父项目的最后领取时间
			it = reward_detail_->reward_back_receive_time.find(pid);
		}
		if(it != reward_detail_->reward_back_receive_time.end()) {
			get_six_time(it->second, last_receive_time); //得到最近领取时间那天的零点时间赋值给 last_receive_time
		}
		int spli_day = get_six_time_subdays(time, Time_Value::gettimeofday());
		if(iter->first >= last_receive_time.sec() &&  spli_day <= 7 && spli_day) {
			UInt_UInt_Map::iterator uuiter = iter->second.find(id);
			if(uuiter != iter->second.end()) {
				num += uuiter->second;
			}
		}
	}
	return num;
}

uint32_t Reward::get_reward_back_can_reveice_days(uint32_t id, uint32_t pid) {
	Time_Value last_receive_time = Time_Value::zero;
	//Time_Value last_receive_time = player_self()->base_detail().create_time;
	UInt_Time_Map::iterator fit = reward_detail_->reward_back_first_time.find(id);
	if(fit == reward_detail_->reward_back_first_time.end()) {
		return 0;
	} else {
		last_receive_time = fit->second;
	}
	UInt_Time_Map::iterator it = reward_detail_->reward_back_receive_time.find(id);
	if(pid) { //如果是分组的子项目; 按父项目的最后领取时间
		it = reward_detail_->reward_back_receive_time.find(pid);
	}
	if(it != reward_detail_->reward_back_receive_time.end() && it->second.sec() > last_receive_time.sec()) {
		get_six_time(it->second, last_receive_time);
	}
	int day = get_six_time_subdays(last_receive_time, Time_Value::gettimeofday());
	if(day > 7) {
		day = 7;
	} else if(day < 0) {
		day = 0;
	}
	return day;
}

void Reward::get_reward_back_info_by_conf(const Reward_Back_Config& conf, Reward_Back_Info &rbi) {
	rbi.reset();
	rbi.id = conf.id;

	//获取精英副本状态（status 0：未解锁 1：激活 2：未激活 3：未打（通关的） 4：已打（通关的））
	if(conf.is_be_group == 1) {//如果是精英副本-只统计已经通关的 目前分组项为1的是精英副本
		int status = 0;
		player_self()->get_elite_dungeon_status(conf.id, status);
		if(status != 4 && status != 3) {
			return;//只统计已经通关的
		}
	}

	UInt_UInt_Map money_list;
	money_list.clear();
	UInt_UInt_Map item_list;
	item_list.clear();
	//如果是子分类;要知道父亲分类的领取时间; 子分类没有显示出来,没有领取时间;只有完成时间和次数
	uint32_t fulfill_num = get_reward_back_fulfill_num_by_id(conf.id, conf.pid);//得到对应项目的已经完成次数
//	//如果是汇聚项,本身自己不可能会有完成次数
//	if(conf.group.size()) { //如果有汇聚项目;把汇聚项目计算累加
//		for(UInt_Vec::const_iterator it = conf.group.begin(); it != conf.group.end(); ++it) {
//			fulfill_num += get_reward_back_fulfill_num_by_id(*it, conf.id);
//		}
//	}
	uint32_t days = get_reward_back_can_reveice_days(conf.id, conf.pid);//最后一次领取时间到现在的天数
	//找出找回次数 = 最后一次领取时间到现在的天数*配置的每天次数 - 已经完成的次数
	int32_t reward_back_num = conf.day_num * days - fulfill_num;
	if(reward_back_num <= 0 && conf.group.size() <= 0) {
		return;
	}
	rbi.back_num = reward_back_num; //当前项目的可找回次数

	{//开始计算每个奖励值的数量 - 道具
		Uint_Uint uu;
		for(UInt_UInt_Map::const_iterator uuiter = conf.reward_item.begin(); uuiter != conf.reward_item.end(); ++uuiter) {
			uu.reset();
			uu.val_1 = uuiter->first;
			//得到等级预设置奖励值
			Reward_Back_Level_Item_Key iv_key;
			iv_key.level = player_self()->level();
			iv_key.item = uuiter->first;//项目ID
			Reward_Back_Level_Item_Val_Map::const_iterator iv_iter = conf.level_item_val.find(iv_key);
			if(uuiter->second) {//如果配置有设置奖励值的;就使用配置的值
				uu.val_2 = uuiter->second * reward_back_num;//需要乘上找回次数
			} else if (iv_iter != conf.level_item_val.end()) {
				uu.val_2 = iv_iter->second.value * reward_back_num;//需要乘上找回次数
			} else {//最后一次该项目对应的奖励
				UInt_UInt_UInt_Map::iterator item_value_iter = reward_detail_->reward_back_item_value.find(conf.id);
				if(item_value_iter != reward_detail_->reward_back_item_value.end()) {
					UInt_UInt_Map::iterator value_iter = item_value_iter->second.find(uuiter->first);
					if(value_iter != item_value_iter->second.end()) {
						uu.val_2 = value_iter->second * reward_back_num;//需要乘上找回次数
					}
				}
			}
			if(uu.val_2) {
				UInt_UInt_Map::iterator ilit = item_list.find(uu.val_1);
				if(ilit != item_list.end()) {
					ilit->second += uu.val_2;
				} else {
					item_list.insert(std::make_pair(uu.val_1, uu.val_2));
				}
				//rbi.info_item.push_back(uu);
			}
		}
	}//结束计算每个奖励值的数量 - 道具
	{//开始计算每个奖励值的数量 - 货币
		Uint_Uint uu;
		for(UInt_UInt_Map::const_iterator uuiter = conf.reward_money.begin(); uuiter != conf.reward_money.end(); ++uuiter) {
			uu.reset();
			uu.val_1 = uuiter->first;
			//得到等级预设置奖励值
			Reward_Back_Level_Item_Key iv_key;
			iv_key.level = player_self()->level();
			iv_key.item = uuiter->first;//项目ID
			Reward_Back_Level_Item_Val_Map::const_iterator iv_iter = conf.level_item_val.find(iv_key);
			if(uuiter->second) {//如果配置有设置奖励值的;就使用配置的值
				uu.val_2 = uuiter->second * reward_back_num;//需要乘上找回次数
			} else if (iv_iter != conf.level_item_val.end()) {
				uu.val_2 = iv_iter->second.value * reward_back_num;//需要乘上找回次数
			} else {//最后一次该项目对应的奖励
				UInt_UInt_UInt_Map::iterator item_value_iter = reward_detail_->reward_back_item_value.find(conf.id);
				if(item_value_iter != reward_detail_->reward_back_item_value.end()) {
					UInt_UInt_Map::iterator value_iter = item_value_iter->second.find(uuiter->first);
					if(value_iter != item_value_iter->second.end()) {
						uu.val_2 = value_iter->second * reward_back_num;//需要乘上找回次数
					}
				}
			}
			if(uu.val_2) {
				UInt_UInt_Map::iterator ilit = money_list.find(uu.val_1);
				if(ilit != money_list.end()) {
					ilit->second += uu.val_2;
				} else {
					money_list.insert(std::make_pair(uu.val_1, uu.val_2));
				}
				//rbi.info_money.push_back(uu);
			}
		}
	}//结束计算每个奖励值的数量 - 货币
	{//开始计算每个奖励值的数量 - 经验
		//得到等级预设置奖励值
		Reward_Back_Level_Item_Key iv_key;
		iv_key.level = player_self()->level();
		iv_key.item = PT_EXP_CURRENT;//项目ID--经验
		Reward_Back_Level_Item_Val_Map::const_iterator iv_iter = conf.level_item_val.find(iv_key);
		if(conf.exp) {
			rbi.exp = conf.exp * reward_back_num;//需要乘上找回次数
		} else if(iv_iter != conf.level_item_val.end()) {
			rbi.exp = iv_iter->second.value * reward_back_num;//需要乘上找回次数
		} else {
			UInt_UInt_UInt_Map::iterator item_value_iter = reward_detail_->reward_back_item_value.find(conf.id);
			if(item_value_iter != reward_detail_->reward_back_item_value.end()) {
				UInt_UInt_Map::iterator value_iter = item_value_iter->second.find(PT_EXP_CURRENT);
				if(value_iter != item_value_iter->second.end()) {
					rbi.exp = value_iter->second * reward_back_num;//需要乘上找回次数
				}
			}
		}
	}//结束计算每个奖励值的数量 - 经验

	Reward_Back_Ratio_Cost_Map::const_iterator rc_iter = conf.ratio_cost_map.find(player_self()->level());//得到等级对应的消耗和奖励比例
	//金币追回消耗金币数
	if(conf.copper_cost || rc_iter == conf.ratio_cost_map.end()) {
		rbi.copper_cost = reward_back_num * conf.copper_cost;
	} else {
		rbi.copper_cost = reward_back_num * rc_iter->second.copper_cost;
	}

	//钻石追回消耗钻石数
	if(conf.gold_cost || rc_iter == conf.ratio_cost_map.end()) {
		rbi.gold_cost = reward_back_num * conf.gold_cost;
	} else {
		rbi.gold_cost = reward_back_num * rc_iter->second.gold_cost;
	}
	//开始计算分组累加
	if(conf.group.size()) {
		for(UInt_Vec::const_iterator it = conf.group.begin(); it != conf.group.end(); ++it) {
			const Reward_Back_Config *tmp_conf = CONFIG_CACHE_REWARD->get_reward_back_conf(*it);
			if(tmp_conf) {
				Reward_Back_Info tmp_rbi;
			    get_reward_back_info_by_conf(*tmp_conf, tmp_rbi);
			    rbi.copper_cost += tmp_rbi.copper_cost;
			    rbi.gold_cost += tmp_rbi.gold_cost;
			    rbi.exp += tmp_rbi.exp;
			    rbi.back_num += tmp_rbi.back_num;
			    for(std::vector<Uint_Uint>::iterator tmp_it = tmp_rbi.info_item.begin(); tmp_it != tmp_rbi.info_item.end(); ++tmp_it) {
			    	//rbi.info_item.push_back(*tmp_it);
					UInt_UInt_Map::iterator ilit = item_list.find(tmp_it->val_1);
					if(ilit != item_list.end()) {
						ilit->second += tmp_it->val_2;
					} else {
						item_list.insert(std::make_pair(tmp_it->val_1, tmp_it->val_2));
					}
			    }
			    for(std::vector<Uint_Uint>::iterator tmp_it = tmp_rbi.info_money.begin(); tmp_it != tmp_rbi.info_money.end(); ++tmp_it) {
			    	//rbi.info_money.push_back(*tmp_it);
					UInt_UInt_Map::iterator ilit = money_list.find(tmp_it->val_1);
					if(ilit != money_list.end()) {
						ilit->second += tmp_it->val_2;
					} else {
						money_list.insert(std::make_pair(tmp_it->val_1, tmp_it->val_2));
					}
			    }
			}
		}
	}
	//end计算分组累加

	//合并货币类型和道具类型的值后赋值返回
	Uint_Uint uu;
	for(UInt_UInt_Map::iterator ilit = item_list.begin(); ilit != item_list.end(); ++ilit) {
		uu.reset();
		uu.val_1 = ilit->first;
		uu.val_2 = ilit->second;
		rbi.info_item.push_back(uu);
	}
	for(UInt_UInt_Map::iterator ilit = money_list.begin(); ilit != money_list.end(); ++ilit) {
		uu.reset();
		uu.val_1 = ilit->first;
		uu.val_2 = ilit->second;
		rbi.info_money.push_back(uu);
	}
}

void Reward::update_reward_back_reveice_time(uint32_t id) {
	UInt_Time_Map::iterator iter = reward_detail_->reward_back_receive_time.find(id);
	if(iter != reward_detail_->reward_back_receive_time.end()) {
		iter->second = Time_Value::gettimeofday();
	} else {
		reward_detail_->reward_back_receive_time.insert(std::make_pair(id, Time_Value::gettimeofday()));
	}
	reward_detail_->detail_change();
}

void Reward::reward_back_listen_level_up(const int level) {
//	if(level == 20) {
//		send_online_reward_msg();
//	}
//	if(level == 15) {
//		send_reward_back_msg();
//	}
}

void Reward::test_set_seven_is_receive(void) {
	//reward_detail_->login_seven_is_receive = 0;
	++reward_detail_->login_days;
	reward_detail_->detail_change();
}

void Reward::test_all_reward_back_sub_day(void) {
//	Timesec_UInt_UInt_Map rb_all_num = reward_detail_->reward_back_all_num;
//	reward_detail_->reward_back_all_num.clear();
//	for(Timesec_UInt_UInt_Map::iterator it = rb_all_num.begin(); it != rb_all_num.end(); ++it) {
//		Time_Value old_time = Time_Value(it->first);
//		long long int time = old_time.sec() - Time_Value::ONE_DAY_IN_SECS;
//		reward_detail_->reward_back_all_num.insert(std::make_pair(time, it->second));
//	}
	for(UInt_Time_Map::iterator it = reward_detail_->reward_back_first_time.begin(); it != reward_detail_->reward_back_first_time.end(); ++it) {
		it->second.sec(it->second.sec() - Time_Value::ONE_DAY_IN_SECS);
	}
	for(UInt_Time_Map::iterator it = reward_detail_->reward_back_receive_time.begin(); it != reward_detail_->reward_back_receive_time.end(); ++it) {
		it->second.sec(it->second.sec() - Time_Value::ONE_DAY_IN_SECS);
	}
	reward_detail_->detail_change();
}
