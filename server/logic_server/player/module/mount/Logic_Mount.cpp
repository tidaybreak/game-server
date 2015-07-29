/*
 * Logic_Mount.cpp
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#include "Logic_Module_Head.h"
#include "Mount_Struct.h"
#include "Config_Cache_Mount.h"
#include "Logic_Player.h"
#include "pack/Pack_Func.h"
#include "Msg_Mount_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Err_Code.h"
#include "Config_Cache_Item.h"
#include "fighter/Fighter_Struct.h"
#include "announcement/Announcement_Struct.h"
#include "Msg_Active_Struct.h"
#include "oper_activity/Oper_Activity_Def.h"

#define MAX_DEPLOY_MOUNT_NUM 3

const int ANN_ID_MOUNT_TRIAN_UP = 1501;		// 坐骑升级广播
const int ANN_ID_MOUNT_SHAPE_UP = 1505;		// 坐骑变身广播

Logic_Mount::Logic_Mount(void):mount_detail_(0){

}

Logic_Mount::~Logic_Mount(void){

}

void Logic_Mount::reset(void){
	mount_detail_ = 0;
}

void Logic_Mount::load_detail(Mount_Detail &detail){
	mount_detail_ = &detail;
}
void Logic_Mount::save_detail(Mount_Detail &detail){
	if(mount_detail_->is_change){
		detail = *mount_detail_;
		mount_detail_->is_change = false;
	}
}

void Logic_Mount::daily_refresh(void){

}

int Logic_Mount::trigger_daily_zero(const Time_Value &now){
	return 0;
}

int Logic_Mount::trigger_daily_six(const Time_Value &now){
	return 0;
}

void Logic_Mount::module_init(void){
	if(this->player_self() == NULL){
		return;
	}
	// 基础属性
	this->cal_train_attr_value();
	std::vector<Horse_Info>::iterator it;
	for(it = mount_detail_->horse_list.begin(); it != mount_detail_->horse_list.end(); ++it){
		if(it->invalid == 0 && it->deploy != 0){
			mount_detail_->deploy_num += 1;
		}
		add_mount_horse_prop(it->horse_id, true);
	}
	horse_group_first_check();
	// 组合属性
	int count = mount_detail_->group_list.size();
	for(int i = 0; i < count; ++i){
		if(mount_detail_->group_list[i].status == 0){
			continue;
		}
		this->add_mount_group_prop(mount_detail_->group_list[i].group_id, true);
	}
	// 骑乘状态
	player_self()->base_detail_no_const().ride = mount_detail_->ride;
	player_self()->base_detail_no_const().mount_model_id = 0;
	if(mount_detail_->ride != 0){
		Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(mount_detail_->mount_id);
		if(h_cfg){
			int train_lv = mount_detail_->get_train_lv(mount_detail_->mount_id);
			int model_id = h_cfg->get_mod_id(train_lv);
			player_self()->base_detail_no_const().mount_model_id = model_id;
			double speed_rate = ((double)h_cfg->speed)/100;
			player_self()->fight_prop_modify(PT_SPEED, AT_MOUNT, O_ADD, 0, 0, speed_rate, 105);
		}
	}
}

int Logic_Mount::create_init(Mount_Detail& detail){

	//
	return 0;
}

void Logic_Mount::sign_in(void){

}

void Logic_Mount::sign_out(void){

}

int Logic_Mount::listen_mount_deadline(const Time_Value& now){
	bool change = false;
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count ; ++i){
		if(mount_detail_->horse_list[i].invalid == 0){
			if(mount_detail_->horse_list[i].deadline != Time_Value::zero){
				if(now > mount_detail_->horse_list[i].deadline){
					this->invalid_horse(mount_detail_->horse_list[i].horse_id);
					change = true;
				}
			}
		}
	}
	if(change){
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
		//英雄属性刷新
		player_self()->fight_heroer_prop_refresh();
	}
	return 0;
}
// 请求坐骑信息
int Logic_Mount::req_fecth_mount_info(void){
	if(!mount_detail_){
		return -1;
	}
	MSG_50100600 msg;
	msg.cur_mount = mount_detail_->mount_id;
	msg.ride = mount_detail_->ride;
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count ; ++i){
		Mount_Info info;
		info.mount_id = mount_detail_->horse_list[i].horse_id;
		info.invalid = mount_detail_->horse_list[i].invalid;
		info.deadline = mount_detail_->horse_list[i].deadline.sec();
		info.activate_time = mount_detail_->horse_list[i].activate_time.sec();
		info.train_lv = mount_detail_->horse_list[i].train_lv;
		info.lv = mount_detail_->horse_list[i].train_num;
		info.deploy = mount_detail_->horse_list[i].deploy;
		info.cur_attr.push_back(mount_detail_->horse_list[i].cur_train_attr1);
		info.cur_attr.push_back(mount_detail_->horse_list[i].cur_train_attr2);
		info.cur_attr.push_back(mount_detail_->horse_list[i].cur_train_attr3);
		info.cur_attr.push_back(mount_detail_->horse_list[i].cur_train_attr4);
		info.cur_attr.push_back(mount_detail_->horse_list[i].cur_train_attr5);
		info.max_attr.push_back(mount_detail_->horse_list[i].max_train_attr1);
		info.max_attr.push_back(mount_detail_->horse_list[i].max_train_attr2);
		info.max_attr.push_back(mount_detail_->horse_list[i].max_train_attr3);
		info.max_attr.push_back(mount_detail_->horse_list[i].max_train_attr4);
		info.max_attr.push_back(mount_detail_->horse_list[i].max_train_attr5);
		info.luck_value = mount_detail_->horse_list[i].luck_value;
		info.skill_1 = mount_detail_->horse_list[i].deploy_skill_1;
		info.skill_2 = mount_detail_->horse_list[i].deploy_skill_2;
		info.skill_3 = mount_detail_->horse_list[i].deploy_skill_3;
		// 是否处于进阶中
		if(mount_detail_->horse_list[i].next_update_num != 0 &&
				mount_detail_->horse_list[i].train_num >= mount_detail_->horse_list[i].next_update_num){
			Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(mount_detail_->horse_list[i].horse_id);
			if(h_cfg){
				int max_train_lv = CONFIG_CACHE_MOUNT->get_max_train_lv(h_cfg->quality);
				if(mount_detail_->horse_list[i].train_lv < max_train_lv){
					info.evolve = 1;
				}
			}
		}
		if(mount_detail_->horse_list[i].next_update_num == 0 ||
				mount_detail_->horse_list[i].train_num >= mount_detail_->horse_list[i].next_update_num){
			info.train_rate = 100;
		}else{
			info.train_rate = (int)mount_detail_->horse_list[i].train_num*100/mount_detail_->horse_list[i].next_update_num;
		}
		msg.mount_list.push_back(info);
	}
	count = mount_detail_->group_list.size();
	for(int i = 0; i < count; ++i){
		Mount_Group_Info info;
		info.id = mount_detail_->group_list[i].group_id;
		info.status = mount_detail_->group_list[i].status;
		msg.mount_group_list.push_back(info);
	}
	get_mount_attr(msg.base_attr, msg.train_attr);

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

// 请求坐骑马厩信息
int Logic_Mount::req_fecth_mount_stable_info(void){
	if(!mount_detail_){
		return -1;
	}
	MSG_50100601 msg;
	int count;
	count = mount_detail_->horse_list.size();
	for(int i = 0; i < count ; ++i){
		Mount_Info info;
		info.mount_id = mount_detail_->horse_list[i].horse_id;
		info.invalid = mount_detail_->horse_list[i].invalid;
		info.deadline = mount_detail_->horse_list[i].deadline.sec();
		info.activate_time = mount_detail_->horse_list[i].activate_time.sec();
		msg.mount_info_list.push_back(info);
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

// 请求坐骑骑乘
int Logic_Mount::req_mount_ride(int ride, int horse_id){
	if(!mount_detail_){
		return -1;
	}
	if(mount_detail_->mount_id == 0){
		return -1;
	}
	if(ride == mount_detail_->ride && horse_id == mount_detail_->mount_id){
		return -1;
	}
	int add_speed = 0;
	int model_id = 0;
	if(ride == 0){// 下马
		if(mount_detail_->ride == 0){
			return -1;
		}
		mount_detail_->ride = ride;
		mount_detail_->detail_change();
		// 设置速度
		Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(mount_detail_->mount_id);
		if(h_cfg){
			double speed_rate = ((double)h_cfg->speed)/100;
			player_self()->fight_prop_modify(PT_SPEED, AT_MOUNT, O_SUB, 0, 0, speed_rate, 106);
		}
		model_id = 0;
	}else{// 上马
		if(mount_detail_->ride != 0){
			// 设置速度
			Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(mount_detail_->mount_id);
			if(h_cfg){
				double speed_rate = ((double)h_cfg->speed)/100;
				player_self()->fight_prop_modify(PT_SPEED, AT_MOUNT, O_SUB, 0, 0, speed_rate);
			}
		}
		int target_horse_id = horse_id;
		if(target_horse_id == 0){
			target_horse_id = mount_detail_->mount_id;
		}
		mount_detail_->ride = ride;
		mount_detail_->mount_id = target_horse_id;
		mount_detail_->detail_change();
		Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(target_horse_id);
		if(h_cfg){
			int tran_lv = mount_detail_->get_train_lv(target_horse_id);
			model_id = h_cfg->get_mod_id(tran_lv);
			double speed_rate = ((double)h_cfg->speed)/100;
			player_self()->fight_prop_modify(PT_SPEED, AT_MOUNT, O_ADD, 0, 0, speed_rate);
		}
	}
	this->mount_detail_->detail_change();
	player_self()->base_detail_no_const().ride = ride;
	player_self()->base_detail_no_const().mount_model_id = model_id;
	MSG_50100602 msg;
	msg.result = 1;
	msg.ride = ride;

	THIS_SEND_TO_CLIENT(msg);
	// 同步到场景服
	{
		MSG_20100040 msg;
		msg.ride = ride;
		msg.model_id = model_id;
		msg.speed = add_speed;
		THIS_SEND_TO_SCENE(msg);
	}
	req_fecth_mount_info();
	return 0;
}

// 请求坐骑激活
int Logic_Mount::req_mount_activate(const int horse_id, const uint8_t type){
	if(player_self() == NULL){
		return -1;
	}
	Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(!h_cfg){
		return -1;
	}
	bool b_react = false;
	if(h_cfg->time == 0){
		if(mount_detail_->has_horse(horse_id)){
			return ERROR_MOUNT_ACTIVATE_TIMES_LIMIT;
		}
	}else{//限时的
		if(mount_detail_->has_horse(horse_id)){//重新激活
			if(mount_detail_->horse_is_invalid(horse_id) == false){
				return ERROR_MOUNT_ACTIVATE_TIMES_LIMIT;
			}
			b_react = true;
		}
	}
	if(type == 0){// 道具激活
		//check card
		int card_id = h_cfg->card_id;
		int card_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, card_id);
		if(card_count > 0){
			// vip con
			if(h_cfg->vip_own == 1){
				if(player_self()->vip() <= 0){
					return ERROR_MOUNT_ACTIVATE_VIP_LIMIT;
				}
			}
	    	//扣物品
			int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(card_id, 1),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_ACTIVATE));
			if (0 != result) {
				return ERROR_SUB_ITEM_FAIL;
			}
			//
			{
				MSG_81000102 msg;
				msg.type = 1;
				Item_Detail item(card_id, 1, Item_Detail::BIND);
				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				msg.item_info.push_back(item_base);
				THIS_SEND_TO_CLIENT(msg);
			}
		}else{
			if(h_cfg->num.val_1 == 0){
				return -1;
			}
			int piece_id = h_cfg->num.val_1;
			int piece_count_need = h_cfg->num.val_2;
			int piece_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, piece_id);
			if(piece_count < piece_count_need){
				return -1;
			}
	    	//扣物品
			int result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(piece_id, piece_count_need),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_ACTIVATE));
			if (0 != result) {
				return ERROR_SUB_ITEM_FAIL;
			}
			{
				MSG_81000102 msg;
				msg.type = 1;
				Item_Detail item(piece_id, piece_count_need, Item_Detail::BIND);
				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				msg.item_info.push_back(item_base);
				THIS_SEND_TO_CLIENT(msg);
			}
		}
	}else{//　融合激活
		Horse_Fuse_Config* fuse_cfg = CONFIG_CACHE_MOUNT->get_horse_fuse_config(horse_id);
		if(!fuse_cfg){
			return ERROR_CONFIG_NOT_EXIST;
		}
		// 是否限时
		if(fuse_cfg->time_ture != 0){
			bool check_fuse_time = CONFIG_CACHE_MOUNT->check_fuse_time(horse_id);
			if(!check_fuse_time){
				req_mount_fuse_time();
				return ERROR_MOUNT_FUSE_NO_TIME;
			}
		}
		// 融合条件检测
		for(Int_Int_Vec::iterator it = fuse_cfg->condition.begin(); it != fuse_cfg->condition.end(); ++it){
			bool check_star_lv = is_horse_star_lv_enough(it->val_2, it->val_1);
			if(!check_star_lv){
				return ERROR_MOUNT_FUSE_NO_ENOUGH;
			}
		}
		// 融合消耗检测
		for(Int_Int_Vec::iterator it = fuse_cfg->consume.begin(); it != fuse_cfg->consume.end(); ++it){
			int item_id = it->val_1;
			int item_amount = it->val_2;
			if(item_amount > 0){
				Money_Sub_Type sub_type = get_money_sub_type(item_id);
				if(sub_type != MONEY_SUB_TYPE_END){
					Money_Sub_Info sub_info(Money_Sub_Info(sub_type, item_amount, MONEY_SUB_MOUNT_FUSE));
					int sub_result = player_self()->pack_try_sub_money(sub_info);
					if(sub_result != 0){
						return sub_result;
					}
				}else{
					int sub_result = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(item_id, item_amount), CON_OVERDUE);
					if(sub_result != 0){
						if(sub_result == ERROR_PACK_ITEM_OVERDUE){
							return ERROR_PACK_FUSE_ITEM_OVERDUE;
						}else{
							return sub_result;
						}

					}
				}
			}
		}
		// 融合消耗
		for(Int_Int_Vec::iterator it = fuse_cfg->consume.begin(); it != fuse_cfg->consume.end(); ++it){
			int item_id = it->val_1;
			int item_amount = it->val_2;
			if(item_amount > 0){
				Money_Sub_Type sub_type = get_money_sub_type(item_id);
				if(sub_type != MONEY_SUB_TYPE_END){
					Money_Sub_Info sub_info(Money_Sub_Info(sub_type, item_amount, MONEY_SUB_MOUNT_FUSE));
					player_self()->pack_sub_money(sub_info);
				}else{
					player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(item_id, item_amount)
							,Lose_Item_DM_Info(ITEM_LOSE_MOUNT_FUSE), USE_ITEM, WITH_TRY, CON_OVERDUE);
					MSG_81000102 msg;
					msg.type = 1;
					Item_Detail item(item_id, item_amount, Item_Detail::BIND);
					Item_Basic_Info item_base;
					item.set_item_basic_info(item_base);
					msg.item_info.push_back(item_base);
					THIS_SEND_TO_CLIENT(msg);
				}
			}
		}
	}
	//
	if(b_react == false){
		Time_Value now = Time_Value::gettimeofday();
		Time_Value deadline;
		if(h_cfg->time == 0){
			deadline = Time_Value::zero;
		}else{
			deadline = now + Time_Value(h_cfg->time*Time_Value::ONE_HOUR_IN_SECS);
		}
		int max_attr_1 = 0;
		int max_attr_2 = 0;
		int max_attr_3 = 0;
		int max_attr_4 = 0;
		int max_attr_5 = 0;
		int next_update_num = 0;
		int train_lv = 0;
		CONFIG_CACHE_MOUNT->cal_first_train_lv_max_attr(horse_id, train_lv, max_attr_1, max_attr_2, max_attr_3, max_attr_4, max_attr_5, next_update_num);
		this->add_horse(horse_id, train_lv, deadline, now, h_cfg->attr_1, h_cfg->attr_2, h_cfg->attr_3, h_cfg->attr_4, h_cfg->attr_5,
				max_attr_1, max_attr_2, max_attr_3, max_attr_4, max_attr_5, next_update_num);
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
		//英雄属性刷新
		player_self()->fight_heroer_prop_refresh();
	}else{
		Time_Value now = Time_Value::gettimeofday();
		Time_Value deadline;
		if(h_cfg->time == 0){
			deadline = Time_Value::zero;
		}else{
			deadline = now + Time_Value(h_cfg->time*Time_Value::ONE_HOUR_IN_SECS);
		}
		this->valid_horse(horse_id, deadline);
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
		//英雄属性刷新
		player_self()->fight_heroer_prop_refresh();
	}
	this->mount_detail_->detail_change();
	//
	{
		MSG_50100603 msg;
		msg.result = 1;
		msg.mount_id = horse_id;
		msg.type = type;
		THIS_SEND_TO_CLIENT(msg);
	}
	req_fecth_mount_info();
	player_self()->achievement_listen_horse_get(horse_id);
	return 0;
}

int Logic_Mount::mount_activate_by_item(int horse_id){
	if(player_self() == NULL){
		return -1;
	}
	Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(!h_cfg){
		return -1;
	}
	bool b_react = false;
	if(h_cfg->time == 0){
		if(mount_detail_->has_horse(horse_id)){
			return ERROR_MOUNT_ACTIVATE_TIMES_LIMIT;
		}
	}else{//限时的
		if(mount_detail_->has_horse(horse_id)){//重新激活
			if(mount_detail_->horse_is_invalid(horse_id) == false){
				return ERROR_MOUNT_ACTIVATE_TIMES_LIMIT;
			}
			b_react = true;
		}
	}
	if(b_react == false){
		Time_Value now = Time_Value::gettimeofday();
		Time_Value deadline;
		if(h_cfg->time == 0){
			deadline = Time_Value::zero;
		}else{
			deadline = now + Time_Value(h_cfg->time*Time_Value::ONE_HOUR_IN_SECS);
		}
		int max_attr_1 = 0;
		int max_attr_2 = 0;
		int max_attr_3 = 0;
		int max_attr_4 = 0;
		int max_attr_5 = 0;
		int next_update_num = 0;
		int train_lv = 0;
		CONFIG_CACHE_MOUNT->cal_first_train_lv_max_attr(horse_id, train_lv, max_attr_1, max_attr_2, max_attr_3, max_attr_4, max_attr_5, next_update_num);
		this->add_horse(horse_id, train_lv, deadline, now, h_cfg->attr_1, h_cfg->attr_2, h_cfg->attr_3, h_cfg->attr_4, h_cfg->attr_5,
				max_attr_1, max_attr_2, max_attr_3, max_attr_4, max_attr_5, next_update_num);
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
		//英雄属性刷新
		player_self()->fight_heroer_prop_refresh();
	}else{
		Time_Value now = Time_Value::gettimeofday();
		Time_Value deadline;
		if(h_cfg->time == 0){
			deadline = Time_Value::zero;
		}else{
			deadline = now + Time_Value(h_cfg->time*Time_Value::ONE_HOUR_IN_SECS);
		}
		this->valid_horse(horse_id, deadline);
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
		//英雄属性刷新
		player_self()->fight_heroer_prop_refresh();
	}
	this->mount_detail_->detail_change();
	//
	MSG_50100603 msg;
	msg.result = 1;

	THIS_SEND_TO_CLIENT(msg);

	req_fecth_mount_info();
	return 0;
}

int Logic_Mount::cmd_mount_timeout(int horse_id, int min){
	Time_Value now = Time_Value::gettimeofday();
	int h_count = mount_detail_->horse_list.size();
	for(int i = 0; i < h_count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			if(mount_detail_->horse_list[i].invalid == 0 &&
					mount_detail_->horse_list[i].deadline != Time_Value::zero){
				mount_detail_->horse_list[i].deadline = now + Time_Value(min*Time_Value::ONE_MINUTE_IN_SECS);
				mount_detail_->detail_change();
				break;
			}
		}

	}
	return 0;
}

int Logic_Mount::req_click_mount_group(int group_id){
	if(!mount_detail_){
		return -1;
	}
	int result = mount_detail_->change_group_status(group_id, 1);
	this->mount_detail_->detail_change();
	if(result == 0){
		MSG_50100604 msg;
		msg.group_id = group_id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}


int Logic_Mount::req_mount_train(int horse_id){
	Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(!h_cfg){
		return -1;
	}
	int count = mount_detail_->horse_list.size();
	int horse_index = -1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return -1;
	}
	int train_lv_cur = mount_detail_->horse_list[horse_index].train_lv;
	int max_train_lv = CONFIG_CACHE_MOUNT->get_max_train_lv(h_cfg->quality);
	if(train_lv_cur > max_train_lv){
		return ERROR_MOUNT_TRAIN_LV_MAX;
	}
	if(train_lv_cur == max_train_lv){
		int max_train_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(train_lv_cur);
		if(mount_detail_->horse_list[horse_index].train_num >= max_train_num){
			return ERROR_MOUNT_TRAIN_LV_MAX;
		}
	}
	if(mount_detail_->horse_list[horse_index].train_num <
			mount_detail_->horse_list[horse_index].next_update_num){// 驯养
		int prop_id = 0;
		int prop_count = 0;
		int train_money = 0;
		int skill_ratio = 10;
		int base_ratio = 10;
		int add_ratio = 10;
		int add_luck = 10;
		int max_luck = 100;
		int update_num = 1;
		int qlty_coe = 1;
		int8_t is_rise = 0;		// 是否升星
		int8_t is_evolve = 0;	// 是否处于进阶
		int auto_result = 0;

		CONFIG_CACHE_MOUNT->get_horse_train_cost(train_lv_cur, mount_detail_->horse_list[horse_index].train_num,
																prop_id, prop_count, train_money,
																base_ratio, add_ratio, add_luck, max_luck, update_num, qlty_coe);
		skill_ratio = CONFIG_CACHE_MOUNT->get_horse_train_skill_ratio(h_cfg->quality);
		int cost_num = 0;
		int cost_money = 0;
		int cur_prop_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, prop_id);
		int one_cost = 5;
		Money_Sub_Type one_cost_type = BIND_GOLD_FIRST;
		const Item_Detail_Config * cfg_ditem = CONFIG_CACHE_ITEM->find_item(prop_id);
		if(cfg_ditem){
			one_cost = cfg_ditem->sys_sell;
			one_cost_type = Money_Sub_Type(cfg_ditem->sys_sell_type);
		}
		if(cur_prop_count < prop_count){
			cost_num = cur_prop_count;
			cost_money = one_cost*(prop_count-cur_prop_count);
			//check money
			int sub_result = player_self()->pack_try_sub_money(Money_Sub_Info(one_cost_type, cost_money, MONEY_SUB_MOUNT_TRAIN));
			if(sub_result != 0){
				return sub_result;
			}
		}else{
			cost_num = prop_count;
		}
		//
		//扣物品
		if(cost_num > 0){
			int sub_item_result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(prop_id, cost_num),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_TRAIN));
			if (sub_item_result != 0) {
				return sub_item_result;
			}

			{
				MSG_81000102 msg;
				msg.type = 1;
				Item_Detail item(prop_id, cost_num, Item_Detail::BIND);
				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				msg.item_info.push_back(item_base);
				THIS_SEND_TO_CLIENT(msg);
			}
		}
		//扣金钱
		if(cost_money > 0){
			int sub_money_result = player_self()->pack_sub_money(Money_Sub_Info(one_cost_type, cost_money, MONEY_SUB_MOUNT_TRAIN));
			if(sub_money_result != 0){
				return sub_money_result;
			}
		}
		//
		player_self()->active_content_listen_mount_evolve();
		player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT, add_luck/10);
		player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT_DOUBLE, add_luck/10);
		//幸运值判断
		int luck = mount_detail_->horse_list[horse_index].train_fail_num*add_luck+add_luck;
		if(luck < max_luck){
			int ratio = base_ratio+mount_detail_->horse_list[horse_index].train_fail_num*add_ratio;
			if(ratio < random()%1000){
				auto_result = 0;
				mount_detail_->horse_list[horse_index].train_fail_num += 1;
				mount_detail_->horse_list[horse_index].luck_value += add_luck;
				mount_detail_->detail_change();
				// 培养失败返回
				{
					MSG_50100605 msg;
					msg.mount_id = horse_id;
					msg.result = 0;
					msg.rise = 0;
					if(mount_detail_->horse_list[horse_index].next_update_num == 0 ||
							mount_detail_->horse_list[horse_index].train_num >= mount_detail_->horse_list[horse_index].next_update_num){
						msg.train_rate = 100;
					}else{
						msg.train_rate = (int)mount_detail_->horse_list[horse_index].train_num*100/mount_detail_->horse_list[horse_index].next_update_num;
					}
					msg.luck_value = mount_detail_->horse_list[horse_index].luck_value;
					msg.train_lv = mount_detail_->horse_list[horse_index].train_lv;
					msg.lv = mount_detail_->horse_list[horse_index].train_num;
					msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr1);
					msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr2);
					msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr3);
					msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr4);
					msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr5);
					msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr1);
					msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr2);
					msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr3);
					msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr4);
					msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr5);
					THIS_SEND_TO_CLIENT(msg);
				}
				return 0;
			}else{
				auto_result = 1;
			}
		}else{
			auto_result = 2;
		}
		bool learn_skill = false;
		//
		int add_val1 = h_cfg->attr_qlty1*qlty_coe/1000;
		int add_val2 = h_cfg->attr_qlty2*qlty_coe/1000;
		int add_val3 = h_cfg->attr_qlty3*qlty_coe/1000;
		int add_val4 = h_cfg->attr_qlty4*qlty_coe/1000;
		int add_val5 = h_cfg->attr_qlty5*qlty_coe/1000;
		mount_detail_->horse_list[horse_index].train_num += 1;
		mount_detail_->horse_list[horse_index].total_train_num += 1;
		mount_detail_->horse_list[horse_index].train_fail_num = 0;
		mount_detail_->horse_list[horse_index].luck_value = 0;
		mount_detail_->horse_list[horse_index].cur_train_attr1 += add_val1;
		mount_detail_->horse_list[horse_index].cur_train_attr2 += add_val2;
		mount_detail_->horse_list[horse_index].cur_train_attr3 += add_val3;
		mount_detail_->horse_list[horse_index].cur_train_attr4 += add_val4;
		mount_detail_->horse_list[horse_index].cur_train_attr5 += add_val5;
		bool new_group_act = false;
		// 是不升星
		if(mount_detail_->horse_list[horse_index].train_num >=
				mount_detail_->horse_list[horse_index].next_update_num){
			int update_prop_id = 0;
			int update_prop_count = 0;
			if(mount_detail_->horse_list[horse_index].train_lv < max_train_lv){
				if(CONFIG_CACHE_MOUNT->get_horse_train_up_cost(mount_detail_->horse_list[horse_index].train_lv,
						update_prop_id, update_prop_count) == false){// 是否可以直接进阶
					is_rise = 1;
					// 更新坐骑组合
					new_group_act = update_horse_group(mount_detail_->horse_list[horse_index].horse_id, mount_detail_->horse_list[horse_index].train_lv+1);
					mount_detail_->horse_list[horse_index].train_num = 0;
					mount_detail_->horse_list[horse_index].train_lv += 1;
					mount_detail_->horse_list[horse_index].next_update_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(mount_detail_->horse_list[horse_index].train_lv);
					this->hook_mount_star_upgrade(mount_detail_->horse_list[horse_index].horse_id,
							mount_detail_->horse_list[horse_index].train_lv);
					if(mount_detail_->horse_list[horse_index].train_lv <= max_train_lv){
						int next_update_num = mount_detail_->horse_list[horse_index].next_update_num;
						mount_detail_->horse_list[horse_index].next_lv_total_train_num += next_update_num;
						for(int k = 0; k < next_update_num; ++k){
							Horse_Train_Config* nht_cfg = CONFIG_CACHE_MOUNT->get_horse_train_config(mount_detail_->horse_list[horse_index].train_lv, k);
							if(!nht_cfg){
								continue;
							}
							mount_detail_->horse_list[horse_index].max_train_attr1 += ((int)(h_cfg->attr_qlty1*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr2 += ((int)(h_cfg->attr_qlty2*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr3 += ((int)(h_cfg->attr_qlty3*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr4 += ((int)(h_cfg->attr_qlty4*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr5 += ((int)(h_cfg->attr_qlty5*nht_cfg->qlty_coe/1000));
						}

					}
				}else{
					is_evolve = 1;
				}
			}

		}
		mount_detail_->detail_change();
		// 添加属性
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		setter.operate = O_ADD;
		setter.addi_type = AT_MOUNT;
		setter.fixed = 0;
		setter.percent = 0.0;

		setter.prop_type = PT_PHY_ATTACK;
		setter.fixed = add_val1;
		setter_vec.push_back(setter);

		setter.prop_type = PT_SPELL_ATTACK;
		setter.fixed = add_val2;
		setter_vec.push_back(setter);

		setter.prop_type = PT_PHY_DEFENSE;
		setter.fixed = add_val3;
		setter_vec.push_back(setter);

		setter.prop_type = PT_SPELL_DEFENSE;
		setter.fixed = add_val4;
		setter_vec.push_back(setter);

		setter.prop_type = PT_BLOOD_MAX;
		setter.fixed = add_val5;
		setter_vec.push_back(setter);

		player_self()->fight_prop_modify(setter_vec, 220);
		// 培养返回
		{
			MSG_50100605 msg;
			msg.mount_id = horse_id;
			msg.result = auto_result;
			msg.rise = is_rise;
			msg.evolve = is_evolve;
			if(mount_detail_->horse_list[horse_index].next_update_num == 0 ||
					mount_detail_->horse_list[horse_index].train_num >= mount_detail_->horse_list[horse_index].next_update_num){
				msg.train_rate = 100;
			}else{
				msg.train_rate = (int)mount_detail_->horse_list[horse_index].train_num*100/mount_detail_->horse_list[horse_index].next_update_num;
			}
			msg.luck_value = mount_detail_->horse_list[horse_index].luck_value;
			msg.train_lv = mount_detail_->horse_list[horse_index].train_lv;
			msg.lv = mount_detail_->horse_list[horse_index].train_num;
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr1);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr2);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr3);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr4);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr5);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr1);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr2);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr3);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr4);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr5);
			Int_Vec base_attr;
			this->get_mount_attr(base_attr, msg.train_attr);
			THIS_SEND_TO_CLIENT(msg);
		}

		//	有概率学习到技能
		int open_skill_num = 1;
		int max_skill_lv = 1;
		CONFIG_CACHE_MOUNT->get_deploy_skill_info(mount_detail_->horse_list[horse_index].train_lv,
				mount_detail_->horse_list[horse_index].train_num,
				open_skill_num, max_skill_lv);
		if(skill_ratio > random()%1000){
			int cur_skill_num = 0;		// 已经学习上阵技能个数
			if(mount_detail_->horse_list[horse_index].deploy_skill_1 != 0){
				cur_skill_num += 1;
			}
			if(mount_detail_->horse_list[horse_index].deploy_skill_2 != 0){
				cur_skill_num += 1;
			}
			if(mount_detail_->horse_list[horse_index].deploy_skill_3 != 0){
				cur_skill_num += 1;
			}
			if(cur_skill_num < open_skill_num){// 有空上阵技能没有学习
				int rd_skill_id = CONFIG_CACHE_MOUNT->get_random_deploy_skill(h_cfg->race, max_skill_lv);
				if(rd_skill_id != 0){
					if(cur_skill_num == 0){
						mount_detail_->horse_list[horse_index].deploy_skill_1 = rd_skill_id;
					}else if(cur_skill_num == 1){
						mount_detail_->horse_list[horse_index].deploy_skill_2 = rd_skill_id;
					}else{
						mount_detail_->horse_list[horse_index].deploy_skill_3 = rd_skill_id;
					}
					// 如果已经上阵,添加上阵技能
					if(mount_detail_->horse_list[horse_index].deploy != 0){
						std::vector<Skill_Info> skill_vec;
						Skill_Info skill;
						skill.skill_id = rd_skill_id;
						skill.skill_lv = 1;
						skill_vec.push_back(skill);
						player_self()->add_be_skills(skill_vec);
					}
					learn_skill = true;
					// 学习技能返回
					{
						MSG_50100608 msg;
						msg.mount_id = horse_id;
						msg.skill_id = rd_skill_id;
						msg.result = 1;
						msg.deploy_skill_1 = mount_detail_->horse_list[horse_index].deploy_skill_1;
						msg.deploy_skill_2 = mount_detail_->horse_list[horse_index].deploy_skill_2;
						msg.deploy_skill_3 = mount_detail_->horse_list[horse_index].deploy_skill_3;
						THIS_SEND_TO_CLIENT(msg);
					}
				}
			}
		}
		if(new_group_act || learn_skill){
			//英雄上阵技能刷新
			player_self()->fight_heroer_skill_refresh();
			//英雄属性刷新
			player_self()->fight_heroer_prop_refresh();
		}else{
			//英雄属性刷新
			player_self()->fight_heroer_prop_refresh();
		}
		if(new_group_act){
			req_fecth_mount_info();
		}
		return 0;
	}else{// 进阶
		if(train_lv_cur == max_train_lv){//
			return ERROR_MOUNT_TRAIN_LV_MAX;
		}
		int8_t is_rise = 0;
		int update_prop_id = 0;
		int update_prop_count = 0;
		CONFIG_CACHE_MOUNT->get_horse_train_up_cost(mount_detail_->horse_list[horse_index].train_lv,
						update_prop_id, update_prop_count);
		if(update_prop_id != 0 && update_prop_count > 0){// 晋级物品消耗
			int try_result = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(update_prop_id, update_prop_count));
			if(try_result != 0){
				return try_result;
			}
			const Item_Detail_Config *p_cfg = CONFIG_CACHE_ITEM->find_item(update_prop_id);
			if (!p_cfg) {
				return ERROR_CONFIG_NOT_EXIST;
			}
			// 使用等级检测
			if(this->level() < p_cfg->needLevel){
				return ERROR_ROLE_LV_LIM;
			}
			int sub_item_result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					 Id_Amount(update_prop_id, update_prop_count),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_EVOLVE));
			if (sub_item_result != 0) {
				return sub_item_result;
			}
			{
				MSG_81000102 msg;
				msg.type = 1;
				Item_Detail item(update_prop_id, update_prop_count, Item_Detail::BIND);
				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				msg.item_info.push_back(item_base);
				THIS_SEND_TO_CLIENT(msg);
			}
		}
		//
		player_self()->active_content_listen_mount_evolve();
		is_rise = 1;
		// 更新坐骑组合
		bool new_group_act = update_horse_group(mount_detail_->horse_list[horse_index].horse_id, mount_detail_->horse_list[horse_index].train_lv+1);
		mount_detail_->horse_list[horse_index].train_num = 0;
		mount_detail_->horse_list[horse_index].train_lv += 1;
		mount_detail_->horse_list[horse_index].next_update_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(mount_detail_->horse_list[horse_index].train_lv);
		this->hook_mount_star_upgrade(mount_detail_->horse_list[horse_index].horse_id,
				mount_detail_->horse_list[horse_index].train_lv);
		if(mount_detail_->horse_list[horse_index].train_lv <= max_train_lv){
			int next_update_num = mount_detail_->horse_list[horse_index].next_update_num;
			mount_detail_->horse_list[horse_index].next_lv_total_train_num += next_update_num;
			for(int k = 0; k < next_update_num; ++k){
				Horse_Train_Config* nht_cfg = CONFIG_CACHE_MOUNT->get_horse_train_config(mount_detail_->horse_list[horse_index].train_lv, k);
				if(!nht_cfg){
					continue;
				}
				mount_detail_->horse_list[horse_index].max_train_attr1 += ((int)(h_cfg->attr_qlty1*nht_cfg->qlty_coe/1000));
				mount_detail_->horse_list[horse_index].max_train_attr2 += ((int)(h_cfg->attr_qlty2*nht_cfg->qlty_coe/1000));
				mount_detail_->horse_list[horse_index].max_train_attr3 += ((int)(h_cfg->attr_qlty3*nht_cfg->qlty_coe/1000));
				mount_detail_->horse_list[horse_index].max_train_attr4 += ((int)(h_cfg->attr_qlty4*nht_cfg->qlty_coe/1000));
				mount_detail_->horse_list[horse_index].max_train_attr5 += ((int)(h_cfg->attr_qlty5*nht_cfg->qlty_coe/1000));
			}

		}
		mount_detail_->detail_change();
		// 培养返回
		{
			MSG_50100605 msg;
			msg.mount_id = horse_id;
			msg.result = 1;
			msg.rise = 1;
			msg.evolve = 0;
			if(mount_detail_->horse_list[horse_index].next_update_num == 0 ||
					mount_detail_->horse_list[horse_index].train_num >= mount_detail_->horse_list[horse_index].next_update_num){
				msg.train_rate = 100;
			}else{
				msg.train_rate = (int)mount_detail_->horse_list[horse_index].train_num*100/mount_detail_->horse_list[horse_index].next_update_num;
			}
			msg.luck_value = mount_detail_->horse_list[horse_index].luck_value;
			msg.train_lv = mount_detail_->horse_list[horse_index].train_lv;
			msg.lv = mount_detail_->horse_list[horse_index].train_num;
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr1);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr2);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr3);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr4);
			msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr5);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr1);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr2);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr3);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr4);
			msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr5);
			Int_Vec base_attr;
			this->get_mount_attr(base_attr, msg.train_attr);
			THIS_SEND_TO_CLIENT(msg);
		}
		if(new_group_act){
			//英雄上阵技能刷新
			player_self()->fight_heroer_skill_refresh();
			//英雄属性刷新
			player_self()->fight_heroer_prop_refresh();
			req_fecth_mount_info();
		}
		return 0;
	}
	return 0;
}

int Logic_Mount::req_mount_train_auto_lv(int horse_id, int type){
	Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(!h_cfg){
		return -1;
	}
	int count = mount_detail_->horse_list.size();
	int horse_index = -1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return -1;
	}
	int train_lv_cur = mount_detail_->horse_list[horse_index].train_lv;
	int max_train_lv = CONFIG_CACHE_MOUNT->get_max_train_lv(h_cfg->quality);
	if(train_lv_cur > max_train_lv){
		return ERROR_MOUNT_TRAIN_LV_MAX;
	}
	if(train_lv_cur == max_train_lv){
		int max_train_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(train_lv_cur);
		if(mount_detail_->horse_list[horse_index].train_num >= max_train_num){
			return ERROR_MOUNT_TRAIN_LV_MAX;
		}
	}else{
		int max_train_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(train_lv_cur);
		if(mount_detail_->horse_list[horse_index].train_num >= max_train_num){// 自动购买道具进阶
			int8_t is_rise = 0;
			int update_prop_id = 0;
			int update_prop_count = 0;
			CONFIG_CACHE_MOUNT->get_horse_train_up_cost(mount_detail_->horse_list[horse_index].train_lv,
							update_prop_id, update_prop_count);
			if(update_prop_id != 0 && update_prop_count > 0){// 晋级物品消耗
				const Item_Detail_Config *p_cfg = CONFIG_CACHE_ITEM->find_item(update_prop_id);
				if (!p_cfg) {
					return ERROR_CONFIG_NOT_EXIST;
				}
				// 使用等级检测
				if(this->level() < p_cfg->needLevel){
					return ERROR_ROLE_LV_LIM;
				}
				int pack_num = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, update_prop_id);
				int mall_buy_num = update_prop_count - pack_num;
				if(mall_buy_num > 0){//商城直接购买
					int mall_buy_result = player_self()->req_fast_buy_item_in_mall_tabs(update_prop_id, mall_buy_num, 1);
					if(mall_buy_result != 0){
						return mall_buy_result;
					}
				}
				int try_result = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,
						 Id_Amount(update_prop_id, update_prop_count));
				if(try_result != 0){
					return try_result;
				}
				int sub_item_result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
						 Id_Amount(update_prop_id, update_prop_count),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_EVOLVE));
				if (sub_item_result != 0) {
					return sub_item_result;
				}
				{
					MSG_81000102 msg;
					msg.type = 1;
					Item_Detail item(update_prop_id, update_prop_count, Item_Detail::BIND);
					Item_Basic_Info item_base;
					item.set_item_basic_info(item_base);
					msg.item_info.push_back(item_base);
					THIS_SEND_TO_CLIENT(msg);
				}
			}
			//
			player_self()->active_content_listen_mount_evolve();
			is_rise = 1;
			// 更新坐骑组合
			bool new_group_act = update_horse_group(mount_detail_->horse_list[horse_index].horse_id, mount_detail_->horse_list[horse_index].train_lv+1);
			mount_detail_->horse_list[horse_index].train_num = 0;
			mount_detail_->horse_list[horse_index].train_lv += 1;
			mount_detail_->horse_list[horse_index].next_update_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(mount_detail_->horse_list[horse_index].train_lv);
			this->hook_mount_star_upgrade(mount_detail_->horse_list[horse_index].horse_id,
					mount_detail_->horse_list[horse_index].train_lv);
			if(mount_detail_->horse_list[horse_index].train_lv <= max_train_lv){
				int next_update_num = mount_detail_->horse_list[horse_index].next_update_num;
				mount_detail_->horse_list[horse_index].next_lv_total_train_num += next_update_num;
				for(int k = 0; k < next_update_num; ++k){
					Horse_Train_Config* nht_cfg = CONFIG_CACHE_MOUNT->get_horse_train_config(mount_detail_->horse_list[horse_index].train_lv, k);
					if(!nht_cfg){
						continue;
					}
					mount_detail_->horse_list[horse_index].max_train_attr1 += ((int)(h_cfg->attr_qlty1*nht_cfg->qlty_coe/1000));
					mount_detail_->horse_list[horse_index].max_train_attr2 += ((int)(h_cfg->attr_qlty2*nht_cfg->qlty_coe/1000));
					mount_detail_->horse_list[horse_index].max_train_attr3 += ((int)(h_cfg->attr_qlty3*nht_cfg->qlty_coe/1000));
					mount_detail_->horse_list[horse_index].max_train_attr4 += ((int)(h_cfg->attr_qlty4*nht_cfg->qlty_coe/1000));
					mount_detail_->horse_list[horse_index].max_train_attr5 += ((int)(h_cfg->attr_qlty5*nht_cfg->qlty_coe/1000));
				}

			}
			mount_detail_->detail_change();
			// 培养返回
			{
				MSG_50100605 msg;
				msg.mount_id = horse_id;
				msg.result = 1;
				msg.rise = 1;
				msg.evolve = 0;
				if(mount_detail_->horse_list[horse_index].next_update_num == 0 ||
						mount_detail_->horse_list[horse_index].train_num >= mount_detail_->horse_list[horse_index].next_update_num){
					msg.train_rate = 100;
				}else{
					msg.train_rate = (int)mount_detail_->horse_list[horse_index].train_num*100/mount_detail_->horse_list[horse_index].next_update_num;
				}
				msg.luck_value = mount_detail_->horse_list[horse_index].luck_value;
				msg.train_lv = mount_detail_->horse_list[horse_index].train_lv;
				msg.lv = mount_detail_->horse_list[horse_index].train_num;
				msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr1);
				msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr2);
				msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr3);
				msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr4);
				msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr5);
				msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr1);
				msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr2);
				msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr3);
				msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr4);
				msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr5);
				Int_Vec base_attr;
				this->get_mount_attr(base_attr, msg.train_attr);
				THIS_SEND_TO_CLIENT(msg);
			}
			if(new_group_act){
				//英雄上阵技能刷新
				player_self()->fight_heroer_skill_refresh();
				//英雄属性刷新
				player_self()->fight_heroer_prop_refresh();
				req_fecth_mount_info();
			}
			return 0;
		}
	}
	//
	int prop_id = 0;
	int prop_count = 0;
	int train_money = 0;
	int skill_ratio = 10;
	int base_ratio = 10;
	int add_ratio = 10;
	int add_luck = 10;
	int max_luck = 100;
	int update_num = 1;
	int qlty_coe = 1;

	CONFIG_CACHE_MOUNT->get_horse_train_cost(train_lv_cur, mount_detail_->horse_list[horse_index].train_num,
															prop_id, prop_count, train_money,
															base_ratio, add_ratio, add_luck, max_luck, update_num, qlty_coe);
	skill_ratio = CONFIG_CACHE_MOUNT->get_horse_train_skill_ratio(h_cfg->quality);

	int cur_prop_count = player_self()->pack_calc_unlock_item(PACK_T_PACKAGE_INDEX, prop_id);

	int auto_result = -1;
	// 驯养终止次数
	int need_train_num = 1;
	// 幸运值
	int luck_value = mount_detail_->horse_list[horse_index].luck_value;
	// 失败次数
	int train_fail_num = mount_detail_->horse_list[horse_index].train_fail_num;

	int train_num = 0;

	int one_cost = 5;
	Money_Sub_Type one_cost_type = BIND_GOLD_FIRST;
	const Item_Detail_Config * cfg_ditem = CONFIG_CACHE_ITEM->find_item(prop_id);
	if(cfg_ditem){
		one_cost = cfg_ditem->sys_sell;
		one_cost_type = (Money_Sub_Type)cfg_ditem->sys_sell_type;
	}

	int cost_total_prop_num = 0;
	int cost_total_money = 0;
	if(type == 1){// 只用道具
		if(cur_prop_count < prop_count){// 道具不足
			player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(prop_id, prop_count));
			return ERROR_MOUNT_LEARN_SKILL_NO_PROP;
		}
		int sub_item_result = 0;
		while(sub_item_result == 0){
			if(cost_total_prop_num+prop_count <= cur_prop_count){
				sub_item_result = 0;
			}else{
				sub_item_result = -1;
			}

			if(sub_item_result == 0){
				//
				player_self()->active_content_listen_mount_evolve();
				player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT, add_luck/10);
				player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT_DOUBLE, add_luck/10);
				cost_total_prop_num += prop_count;
				//幸运值判断
				bool train_result = true;
				int luck = train_fail_num*add_luck+add_luck;
				if(luck < max_luck){
					int ratio = base_ratio+train_fail_num*add_ratio;
					if(ratio < random()%1000){
						train_result = false;
						auto_result = 0;
					}else{
						auto_result = 1;
					}
				}else{
					auto_result = 2;
				}
				if(train_result){// 驯养成功
					train_num += 1;
					train_fail_num = 0;
					luck_value = 0;
				}else{// 驯养失败
					train_fail_num += 1;
					luck_value += add_luck;
				}
				if(train_result){// 驯养成功
					break;
				}
			}else{// 没有道具消耗
				break;
			}
		}
	}else{// 道具用完，再使用粉钻、钻石
		train_money = one_cost*prop_count;
		int sub_item_result = 0;
		int sub_money_result = 0;
		while(sub_item_result == 0){
			if(cost_total_prop_num+prop_count <= cur_prop_count){
				sub_item_result = 0;
			}else{
				sub_item_result = -1;
			}
			if(sub_item_result == 0){
				cost_total_prop_num += prop_count;
				//
				player_self()->active_content_listen_mount_evolve();
				player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT, add_luck/10);
				player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT_DOUBLE, add_luck/10);
				//幸运值判断
				bool train_result = true;
				int luck = train_fail_num*add_luck+add_luck;
				if(luck < max_luck){
					int ratio = base_ratio+train_fail_num*add_ratio;
					if(ratio < random()%1000){
						train_result = false;
						auto_result = 0;
					}else{
						auto_result = 1;
					}
				}else{
					auto_result = 2;
				}
				if(train_result){// 驯养成功
					train_num += 1;
					train_fail_num = 0;
					luck_value = 0;
				}else{// 驯养失败
					train_fail_num += 1;
					luck_value += add_luck;
				}
				if(train_result){// 驯养成功
					break;
				}
			}else{// 没有道具消耗
				break;
			}
		}
		// 没有达到进阶的,继续驯养
		if(train_num < need_train_num){// 没有达到进阶的,继续驯养
			int remain_prop_num = cur_prop_count%prop_count;
			if(remain_prop_num > 0){// 道具有剩余,使用钻石补足
				if(cost_total_prop_num+remain_prop_num <= cur_prop_count){
					sub_item_result = 0;
				}else{
					sub_item_result = -1;
				}
				if(sub_item_result == 0){
					int remain_cost_money = one_cost*(prop_count-remain_prop_num);
					if(remain_cost_money > 0){
						sub_money_result = player_self()->pack_try_sub_money(Money_Sub_Info(one_cost_type, cost_total_money+remain_cost_money, MONEY_SUB_MOUNT_TRAIN_AUTO));
						if(sub_money_result == 0){
							cost_total_prop_num += remain_prop_num;
							cost_total_money += remain_cost_money;
							//幸运值判断
							bool train_result = true;
							int luck = train_fail_num*add_luck+add_luck;
							if(luck < max_luck){
								int ratio = base_ratio+train_fail_num*add_ratio;
								if(ratio < random()%1000){
									train_result = false;
									auto_result = 0;
								}else{
									auto_result = 1;
								}
							}else{
								auto_result = 2;
							}
							if(train_result){// 驯养成功
								train_num += 1;
								train_fail_num = 0;
								luck_value = 0;
							}else{// 驯养失败
								train_fail_num += 1;
								luck_value += add_luck;
							}
						}
					}
				}
			}
			// 没有达到进阶的,继续钻石驯养
			if(train_num < need_train_num){
				while(sub_money_result == 0){
					sub_money_result = player_self()->pack_try_sub_money(Money_Sub_Info(one_cost_type, cost_total_money+train_money, MONEY_SUB_MOUNT_TRAIN_AUTO));
					if(sub_money_result == 0){
						cost_total_money += train_money;
						//
						player_self()->active_content_listen_mount_evolve();
						player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT, add_luck/10);
						player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TRAIN_MOUNT_DOUBLE, add_luck/10);
						//幸运值判断
						bool train_result = true;
						int luck = train_fail_num*add_luck+add_luck;
						if(luck < max_luck){
							int ratio = base_ratio+train_fail_num*add_ratio;
							if(ratio < random()%1000){
								train_result = false;
								auto_result = 0;
							}else{
								auto_result = 1;
							}
						}else{
							auto_result = 2;
						}
						if(train_result){// 驯养成功
							train_num += 1;
							train_fail_num = 0;
							luck_value = 0;
						}else{// 驯养失败
							train_fail_num += 1;
							luck_value += add_luck;
						}
						if(train_num >= need_train_num){// 达到最大驯养次数
							break;
						}
					}else{
						break;
					}
				}
			}
		}
		if(auto_result == -1){
			return ERROR_GOLD_NOT_ENOUGH;
		}
	}
	//
	mount_detail_->horse_list[horse_index].train_num += train_num;
	mount_detail_->horse_list[horse_index].total_train_num += train_num;
	mount_detail_->horse_list[horse_index].train_fail_num = train_fail_num;
	mount_detail_->horse_list[horse_index].luck_value = luck_value;
	mount_detail_->detail_change();
	// 扣除道具
	if(cost_total_prop_num > 0){
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
							 Id_Amount(prop_id, cost_total_prop_num),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_AUTO_TRAIN));
		{
			MSG_81000102 msg;
			msg.type = 1;
			Item_Detail item(prop_id, cost_total_prop_num, Item_Detail::BIND);
			Item_Basic_Info item_base;
			item.set_item_basic_info(item_base);
			msg.item_info.push_back(item_base);
			THIS_SEND_TO_CLIENT(msg);
		}
	}
	// 扣除粉钻、钻石
	if(cost_total_money > 0){
		player_self()->pack_sub_money(Money_Sub_Info(one_cost_type, cost_total_money, ITEM_LOSE_MOUNT_AUTO_TRAIN));
	}

	int is_rise = 0;
	int is_evolve = 0;
	// 驯养属性添加
	if(train_num > 0){
		int add_val1 = ((int)(h_cfg->attr_qlty1*qlty_coe/1000))*train_num;
		int add_val2 = ((int)(h_cfg->attr_qlty2*qlty_coe/1000))*train_num;
		int add_val3 = ((int)(h_cfg->attr_qlty3*qlty_coe/1000))*train_num;
		int add_val4 = ((int)(h_cfg->attr_qlty4*qlty_coe/1000))*train_num;
		int add_val5 = ((int)(h_cfg->attr_qlty5*qlty_coe/1000))*train_num;

		mount_detail_->horse_list[horse_index].cur_train_attr1 += add_val1;
		mount_detail_->horse_list[horse_index].cur_train_attr2 += add_val2;
		mount_detail_->horse_list[horse_index].cur_train_attr3 += add_val3;
		mount_detail_->horse_list[horse_index].cur_train_attr4 += add_val4;
		mount_detail_->horse_list[horse_index].cur_train_attr5 += add_val5;

		// 添加属性
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		setter.operate = O_ADD;
		setter.addi_type = AT_MOUNT;
		setter.fixed = 0;
		setter.percent = 0.0;

		setter.prop_type = PT_PHY_ATTACK;
		setter.fixed = add_val1;
		setter_vec.push_back(setter);

		setter.prop_type = PT_SPELL_ATTACK;
		setter.fixed = add_val2;
		setter_vec.push_back(setter);

		setter.prop_type = PT_PHY_DEFENSE;
		setter.fixed = add_val3;
		setter_vec.push_back(setter);

		setter.prop_type = PT_SPELL_DEFENSE;
		setter.fixed = add_val4;
		setter_vec.push_back(setter);

		setter.prop_type = PT_BLOOD_MAX;
		setter.fixed = add_val5;
		setter_vec.push_back(setter);

		player_self()->fight_prop_modify(setter_vec, 218);

		//	有概率学习到技能
		bool learn_skill = false;
		for(int i = 0; i < train_num; ++i){
			int open_skill_num = 1;
			int max_skill_lv = 1;
			CONFIG_CACHE_MOUNT->get_deploy_skill_info(mount_detail_->horse_list[horse_index].train_lv,
					mount_detail_->horse_list[horse_index].train_num,
					open_skill_num, max_skill_lv);
			if(skill_ratio > random()%1000){
				int cur_skill_num = 0;		// 已经学习上阵技能个数
				if(mount_detail_->horse_list[horse_index].deploy_skill_1 != 0){
					cur_skill_num += 1;
				}
				if(mount_detail_->horse_list[horse_index].deploy_skill_2 != 0){
					cur_skill_num += 1;
				}
				if(mount_detail_->horse_list[horse_index].deploy_skill_3 != 0){
					cur_skill_num += 1;
				}
				if(cur_skill_num < open_skill_num){// 有空上阵技能没有学习
					int rd_skill_id = CONFIG_CACHE_MOUNT->get_random_deploy_skill(h_cfg->race, max_skill_lv);
					if(rd_skill_id != 0){
						if(cur_skill_num == 0){
							mount_detail_->horse_list[horse_index].deploy_skill_1 = rd_skill_id;
						}else if(cur_skill_num == 1){
							mount_detail_->horse_list[horse_index].deploy_skill_2 = rd_skill_id;
						}else{
							mount_detail_->horse_list[horse_index].deploy_skill_3 = rd_skill_id;
						}
						// 如果已经上阵,添加上阵技能
						if(mount_detail_->horse_list[horse_index].deploy != 0){
							std::vector<Skill_Info> skill_vec;
							Skill_Info skill;
							skill.skill_id = rd_skill_id;
							skill.skill_lv = 1;
							skill_vec.push_back(skill);
							player_self()->add_be_skills(skill_vec);
						}
						learn_skill = true;
						// 学习技能返回
						{
							MSG_50100608 msg;
							msg.mount_id = horse_id;
							msg.skill_id = rd_skill_id;
							msg.result = 1;
							msg.deploy_skill_1 = mount_detail_->horse_list[horse_index].deploy_skill_1;
							msg.deploy_skill_2 = mount_detail_->horse_list[horse_index].deploy_skill_2;
							msg.deploy_skill_3 = mount_detail_->horse_list[horse_index].deploy_skill_3;
							THIS_SEND_TO_CLIENT(msg);
						}
					}
				}
			}
		}
		bool new_group_act = false;
		// 直接升星
		if(mount_detail_->horse_list[horse_index].train_num >= mount_detail_->horse_list[horse_index].next_update_num){
			int update_prop_id = 0;
			int update_prop_count = 0;
			if(mount_detail_->horse_list[horse_index].train_lv < max_train_lv){
				if(CONFIG_CACHE_MOUNT->get_horse_train_up_cost(mount_detail_->horse_list[horse_index].train_lv,
						update_prop_id, update_prop_count) == false){// 是否可以直接进阶
					is_rise = 1;
					// 更新坐骑组合
					new_group_act = update_horse_group(mount_detail_->horse_list[horse_index].horse_id, mount_detail_->horse_list[horse_index].train_lv+1);
					mount_detail_->horse_list[horse_index].train_num = 0;
					mount_detail_->horse_list[horse_index].train_lv += 1;
					mount_detail_->horse_list[horse_index].next_update_num = CONFIG_CACHE_MOUNT->get_next_update_train_num(mount_detail_->horse_list[horse_index].train_lv);
					this->hook_mount_star_upgrade(mount_detail_->horse_list[horse_index].horse_id,
							mount_detail_->horse_list[horse_index].train_lv);
					if(mount_detail_->horse_list[horse_index].train_lv <= max_train_lv){
						int next_update_num = mount_detail_->horse_list[horse_index].next_update_num;
						mount_detail_->horse_list[horse_index].next_lv_total_train_num += next_update_num;
						for(int k = 0; k < next_update_num; ++k){
							Horse_Train_Config* nht_cfg = CONFIG_CACHE_MOUNT->get_horse_train_config(mount_detail_->horse_list[horse_index].train_lv, k);
							if(!nht_cfg){
								continue;
							}
							mount_detail_->horse_list[horse_index].max_train_attr1 += ((int)(h_cfg->attr_qlty1*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr2 += ((int)(h_cfg->attr_qlty2*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr3 += ((int)(h_cfg->attr_qlty3*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr4 += ((int)(h_cfg->attr_qlty4*nht_cfg->qlty_coe/1000));
							mount_detail_->horse_list[horse_index].max_train_attr5 += ((int)(h_cfg->attr_qlty5*nht_cfg->qlty_coe/1000));
						}
					}
				}else{
					is_evolve = 1;
				}
			}
		}

		if(new_group_act || learn_skill){
			//英雄上阵技能刷新
			player_self()->fight_heroer_skill_refresh();
			//英雄属性刷新
			player_self()->fight_heroer_prop_refresh();
		}else{
			//英雄属性刷新
			player_self()->fight_heroer_prop_refresh();
		}
		if(new_group_act){
			req_fecth_mount_info();
		}
	}


	// 培养返回
	{
		MSG_50100605 msg;
		msg.mount_id = horse_id;
		msg.result = auto_result;
		msg.rise = is_rise;
		msg.evolve = is_evolve;
		if(mount_detail_->horse_list[horse_index].next_update_num == 0 ||
				mount_detail_->horse_list[horse_index].train_num >= mount_detail_->horse_list[horse_index].next_update_num){
			msg.train_rate = 100;
		}else{
			msg.train_rate = (int)mount_detail_->horse_list[horse_index].train_num*100/mount_detail_->horse_list[horse_index].next_update_num;
		}
		msg.luck_value = mount_detail_->horse_list[horse_index].luck_value;
		msg.train_lv = mount_detail_->horse_list[horse_index].train_lv;
		msg.lv = mount_detail_->horse_list[horse_index].train_num;
		msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr1);
		msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr2);
		msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr3);
		msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr4);
		msg.attr_cur.push_back(mount_detail_->horse_list[horse_index].cur_train_attr5);
		msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr1);
		msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr2);
		msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr3);
		msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr4);
		msg.attr_max.push_back(mount_detail_->horse_list[horse_index].max_train_attr5);
		Int_Vec base_attr;
		this->get_mount_attr(base_attr, msg.train_attr);
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Logic_Mount::req_mount_deploy(int horse_id, int deploy_pos){
	if(deploy_pos == 0){
		int count = mount_detail_->horse_list.size();
		int horse_index = -1;
		for(int i = 0; i < count; ++i){
			if(mount_detail_->horse_list[i].horse_id == horse_id){
				horse_index = i;
				break;
			}
		}
		if(horse_index == -1){
			return -1;
		}
		if(mount_detail_->horse_list[horse_index].invalid == 1){
			return -1;
		}
		if(mount_detail_->horse_list[horse_index].deploy == 0){
			return -1;
		}
		remove_deploy_skill(horse_id);	// 移除上阵技能
		mount_detail_->horse_list[horse_index].deploy = 0;
		mount_detail_->deploy_num -= 1;
		mount_detail_->detail_change();
		{
			MSG_50100606 msg;
			msg.result = 1;
			msg.mount_id_1 = horse_id;
			THIS_SEND_TO_CLIENT(msg);
		}
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
		return 0;
	}else{
		int count = mount_detail_->horse_list.size();
		int d_index = -1;
		int ud_index = -1;
		for(int i = 0; i < count; ++i){
			if(mount_detail_->horse_list[i].horse_id == horse_id){
				d_index = i;
			}else if(mount_detail_->horse_list[i].deploy == deploy_pos){
				ud_index = i;
			}
		}
		if(d_index == -1){
			return -1;
		}
		if(mount_detail_->horse_list[d_index].invalid == 1){
			return -1;
		}
		if(mount_detail_->horse_list[d_index].deploy == 0){//上阵
			mount_detail_->horse_list[d_index].deploy = deploy_pos;
			mount_detail_->deploy_num += 1;
			add_deploy_skill(horse_id);//添加上阵技能
			MSG_50100606 msg;
			msg.result = 1;
			msg.mount_id_1 = horse_id;
			msg.deploy_pos_1 = deploy_pos;
			if(ud_index != -1){//目标位置下阵
				remove_deploy_skill(mount_detail_->horse_list[ud_index].horse_id);
				mount_detail_->horse_list[ud_index].deploy = 0;
				mount_detail_->deploy_num -= 1;
				msg.mount_id_2 = mount_detail_->horse_list[ud_index].horse_id;
				msg.deploy_pos_2 = 0;
			}
			mount_detail_->detail_change();
			THIS_SEND_TO_CLIENT(msg);
			//英雄上阵技能刷新
			player_self()->fight_heroer_skill_refresh();
		}else{// 位置交换
			MSG_50100606 msg;
			msg.result = 1;
			msg.mount_id_1 = horse_id;
			msg.deploy_pos_1 = deploy_pos;
			int src_deploy_pos = mount_detail_->horse_list[d_index].deploy;
			mount_detail_->horse_list[d_index].deploy = deploy_pos;
			if(ud_index != -1){//目标位置
				mount_detail_->horse_list[ud_index].deploy = src_deploy_pos;
				msg.mount_id_2 = mount_detail_->horse_list[ud_index].horse_id;
				msg.deploy_pos_2 = mount_detail_->horse_list[ud_index].deploy;
			}
			mount_detail_->detail_change();
			THIS_SEND_TO_CLIENT(msg);
			//英雄上阵技能刷新
			player_self()->fight_heroer_skill_refresh();
		}
	}
	return 0;
}


int Logic_Mount::req_mount_learn_skill(int horse_id, int prop_id){
	int prop_count = player_self()->pack_calc_item(PACK_T_PACKAGE_INDEX, prop_id);
	if(prop_count <= 0){
		return ERROR_MOUNT_LEARN_SKILL_NO_PROP;
	}
	const Item_Detail_Config *p_cfg = CONFIG_CACHE_ITEM->find_item(prop_id);
	if (!p_cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if(p_cfg->special_cfg.type != 1) {
		return ERROR_MOUNT_LEARN_SKILL_ERROR_PROP;
	}
	//
	Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(!h_cfg){
		return ERROR_CONFIG_NOT_EXIST;
	}
	int count = mount_detail_->horse_list.size();
	int horse_index = -1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return ERROR_MOUNT_NOT_ACTIVATE;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return ERROR_MOUNT_NOT_ACTIVATE;
	}
	// 种族限制
	if(CONFIG_CACHE_MOUNT->is_race_deploy_skill(h_cfg->race, prop_id) == false){
		return ERROR_MOUNT_RACE_ERROR;
	}
	// 学习等级限定
	int cur_skill_num = 0;
	int max_skill_lv = 1;
	int open_skill_num = 0;
	int max_skill_num = CONFIG_CACHE_MOUNT->get_deploy_max_skill_num(h_cfg->quality);
	CONFIG_CACHE_MOUNT->get_deploy_skill_info(mount_detail_->horse_list[horse_index].train_lv,
			mount_detail_->horse_list[horse_index].train_num,
			open_skill_num, max_skill_lv);
	if(open_skill_num <= 0){
		return ERROR_MOUNT_SKILL_NO_NUM;
	}
	if(p_cfg->color > max_skill_lv){
		return ERROR_MOUNT_QTC_ERROR;
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_1 != 0){
		cur_skill_num += 1;
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_2 != 0){
		cur_skill_num += 1;
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_3 != 0){
		cur_skill_num += 1;
	}
	//扣物品
	int sub_item_result = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
			 Id_Amount(prop_id, 1),Lose_Item_DM_Info(ITEM_LOSE_MOUNT_TRAIN));
	if (sub_item_result != 0) {
		return sub_item_result;
	}

	{
		MSG_81000102 msg;
		msg.type = 1;
		Item_Detail item(prop_id, 1, Item_Detail::BIND);
		Item_Basic_Info item_base;
		item.set_item_basic_info(item_base);
		msg.item_info.push_back(item_base);
		THIS_SEND_TO_CLIENT(msg);
	}

	int skill_id = p_cfg->special_cfg.addon.mount_skill;//技能id
	int rp_skill_id = 0;
	int skill_num_valid = open_skill_num>max_skill_num?max_skill_num:open_skill_num;
	if(cur_skill_num < skill_num_valid){//有空技能栏的，新增技能
		if(cur_skill_num == 0){
			mount_detail_->horse_list[horse_index].deploy_skill_1 = skill_id;
		}else if(cur_skill_num == 1){
			mount_detail_->horse_list[horse_index].deploy_skill_2 = skill_id;
		}else{
			mount_detail_->horse_list[horse_index].deploy_skill_3 = skill_id;
		}
	}else{//没有空技能，随机替换技能
		int rp_index = random()%cur_skill_num;
		if(rp_index == 0){
			rp_skill_id = mount_detail_->horse_list[horse_index].deploy_skill_1;
			mount_detail_->horse_list[horse_index].deploy_skill_1 = skill_id;
		}else if(rp_index == 1){
			rp_skill_id = mount_detail_->horse_list[horse_index].deploy_skill_2;
			mount_detail_->horse_list[horse_index].deploy_skill_2 = skill_id;
		}else{
			rp_skill_id = mount_detail_->horse_list[horse_index].deploy_skill_3;
			mount_detail_->horse_list[horse_index].deploy_skill_3 = skill_id;
		}
	}
	mount_detail_->detail_change();
	if(mount_detail_->horse_list[horse_index].deploy == 1){
		if(rp_skill_id != 0 ){
//			Int_Vec rm_skill_vec;
//			rm_skill_vec.push_back(rp_skill_id);
			std::vector<Skill_Info> skill_vec;
			Skill_Info skill;
			skill.skill_id = rp_skill_id;
			skill.skill_lv = 1;
			skill_vec.push_back(skill);
			player_self()->remove_be_skills(skill_vec);
		}
		std::vector<Skill_Info> skill_vec;
		Skill_Info skill;
		skill.skill_id = skill_id;
		skill.skill_lv = 1;
		skill_vec.push_back(skill);
		player_self()->add_be_skills(skill_vec);
		//英雄上阵技能刷新
		player_self()->fight_heroer_skill_refresh();
	}

	// 学习成功返回
	{
		MSG_50100608 msg;
		msg.mount_id = horse_id;
		msg.result = 1;
		msg.skill_id = skill_id;
		msg.deploy_skill_1 = mount_detail_->horse_list[horse_index].deploy_skill_1;
		msg.deploy_skill_2 = mount_detail_->horse_list[horse_index].deploy_skill_2;
		msg.deploy_skill_3 = mount_detail_->horse_list[horse_index].deploy_skill_3;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Logic_Mount::req_mount_fuse_time(void){
	MSG_50100609 msg;
	CONFIG_CACHE_MOUNT->get_fuse_time(msg.times);
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Logic_Mount::sync_mount_ride(int ride, int model_id, int speed){
	return 0;
}

int Logic_Mount::add_mount_horse_prop(int id, bool b_init/* = false*/){
	int count = mount_detail_->horse_list.size();
	int horse_index = -1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return -1;
	}
	// 添加属性
	Prop_Setter_Vec setter_vec;
	Prop_Setter setter;
	setter.operate = O_ADD;
	setter.addi_type = AT_MOUNT;
	setter.fixed = 0;
	setter.percent = 0.0;

	setter.prop_type = PT_PHY_ATTACK;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr1;
	setter_vec.push_back(setter);

	setter.prop_type = PT_SPELL_ATTACK;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr2;
	setter_vec.push_back(setter);

	setter.prop_type = PT_PHY_DEFENSE;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr3;
	setter_vec.push_back(setter);

	setter.prop_type = PT_SPELL_DEFENSE;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr4;
	setter_vec.push_back(setter);

	setter.prop_type = PT_BLOOD_MAX;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr5;
	setter_vec.push_back(setter);

	player_self()->fight_prop_modify(setter_vec, 216);

	//上阵技能
	this->add_deploy_skill(id, b_init);
	return 0;
}
int Logic_Mount::del_mount_horse_prop(int id){
	int count = mount_detail_->horse_list.size();
	int horse_index = -1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return -1;
	}

	if(player_self() == NULL){
		return -1;
	}
	// 添加属性
	Prop_Setter_Vec setter_vec;
	Prop_Setter setter;
	setter.operate = O_SUB;
	setter.addi_type = AT_MOUNT;

	setter.prop_type = PT_PHY_ATTACK;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr1;
	setter_vec.push_back(setter);

	setter.prop_type = PT_SPELL_ATTACK;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr2;
	setter_vec.push_back(setter);

	setter.prop_type = PT_PHY_DEFENSE;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr3;
	setter_vec.push_back(setter);

	setter.prop_type = PT_SPELL_DEFENSE;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr4;
	setter_vec.push_back(setter);

	setter.prop_type = PT_BLOOD_MAX;
	setter.fixed = mount_detail_->horse_list[horse_index].cur_train_attr5;
	setter_vec.push_back(setter);

	player_self()->fight_prop_modify(setter_vec, 217);
	return 0;
}

int Logic_Mount::add_mount_group_prop(int group_id, bool b_init/* = false*/){
	if(player_self() == NULL){
		return -1;
	}
	Horse_Group_Config* g_cfg = CONFIG_CACHE_MOUNT->get_horse_group_config(group_id);
	if(!g_cfg){
		return -1;
	}
	// 添加组合技能
	if(g_cfg->skill != 0){
		if(b_init){
			player_self()->init_be_skill_extra(g_cfg->skill, 1);
			return 0;
		}else{
			std::vector<Skill_Info> skill_vec;
			Skill_Info skill_info;
			skill_info.skill_id = g_cfg->skill;
			skill_info.skill_lv = 1;
			skill_vec.push_back(skill_info);
			return player_self()->add_be_skills(skill_vec);
		}

	}
	return 0;
}

int Logic_Mount::del_mount_group_prop(int group_id){
	if(player_self() == NULL){
		return -1;
	}
	Horse_Group_Config* g_cfg = CONFIG_CACHE_MOUNT->get_horse_group_config(group_id);
	if(!g_cfg){
		return -1;
	}
	// 删除组合技能
	if(g_cfg->skill != 0){
		Skill_Info_Vec skill_vec;
		Skill_Info skill;
		skill.skill_id = g_cfg->skill;
		skill.skill_lv = 1;
//		Int_Vec skill_vec;
//		skill_vec.push_back(g_cfg->skill);
		return player_self()->remove_be_skills(skill_vec);
	}
	return 0;
}

int Logic_Mount::add_deploy_skill(int horse_id, bool b_init/* = false*/){
	int horse_index = -1;
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].deploy == 0){
		return -1;
	}
	std::vector<Skill_Info> skill_vec;
	if(mount_detail_->horse_list[horse_index].deploy_skill_1 != 0){
		Skill_Info skill;
		skill.skill_id = mount_detail_->horse_list[horse_index].deploy_skill_1;
		skill.skill_lv = 1;
		skill_vec.push_back(skill);
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_2 != 0){
		Skill_Info skill;
		skill.skill_id = mount_detail_->horse_list[horse_index].deploy_skill_2;
		skill.skill_lv = 1;
		skill_vec.push_back(skill);
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_3 != 0){
		Skill_Info skill;
		skill.skill_id = mount_detail_->horse_list[horse_index].deploy_skill_3;
		skill.skill_lv = 1;
		skill_vec.push_back(skill);
	}
	if(skill_vec.empty() == false){
		if(b_init){
			int count = skill_vec.size();
			for(int i = 0; i < count; ++i){
				player_self()->init_be_skill_extra(skill_vec[i].skill_id, skill_vec[i].skill_lv);
			}
			return 0;
		}else{
			return player_self()->add_be_skills(skill_vec);
		}
	}
	return 0;
}

int Logic_Mount::remove_deploy_skill(int horse_id){
	int count = mount_detail_->horse_list.size();
	int horse_index = -1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			horse_index = i;
			break;
		}
	}
	if(horse_index == -1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].invalid == 1){
		return -1;
	}
	if(mount_detail_->horse_list[horse_index].deploy == 0){
		return -1;
	}
	Skill_Info_Vec skill_vec;
	Skill_Info skill;
//	Int_Vec skill_vec;
	if(mount_detail_->horse_list[horse_index].deploy_skill_1 != 0){
		skill.reset();
		skill.skill_id = mount_detail_->horse_list[horse_index].deploy_skill_1;
		skill.skill_lv = 1;
//		skill_vec.push_back(mount_detail_->horse_list[horse_index].deploy_skill_1);
		skill_vec.push_back(skill);
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_2 != 0){
		skill.reset();
		skill.skill_id = mount_detail_->horse_list[horse_index].deploy_skill_2;
		skill.skill_lv = 1;
		skill_vec.push_back(skill);
//		skill_vec.push_back(mount_detail_->horse_list[horse_index].deploy_skill_2);
	}
	if(mount_detail_->horse_list[horse_index].deploy_skill_3 != 0){
		skill.reset();
		skill.skill_id = mount_detail_->horse_list[horse_index].deploy_skill_3;
		skill.skill_lv = 1;
		skill_vec.push_back(skill);
//		skill_vec.push_back(mount_detail_->horse_list[horse_index].deploy_skill_3);
	}
	if(skill_vec.empty() == false){
		return player_self()->remove_be_skills(skill_vec);
	}
	return 0;
}

void Logic_Mount::get_mount_attr(std::vector<int> &base_attr, std::vector<int> &train_attr){
	int base_1 = 0;
	int base_2 = 0;
	int base_3 = 0;
	int base_4 = 0;
	int base_5 = 0;

	int train_1 = 0;
	int train_2 = 0;
	int train_3 = 0;
	int train_4 = 0;
	int train_5 = 0;

	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].invalid == 1){
			continue;
		}
		Horse_Base_Config* cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(mount_detail_->horse_list[i].horse_id);
		if(cfg == NULL){
			continue;
		}
		base_1 += cfg->attr_1;
		base_2 += cfg->attr_2;
		base_3 += cfg->attr_3;
		base_4 += cfg->attr_4;
		base_5 += cfg->attr_5;
		train_1 += mount_detail_->horse_list[i].cur_train_attr1 - cfg->attr_1;
		train_2 += mount_detail_->horse_list[i].cur_train_attr2 - cfg->attr_2;
		train_3 += mount_detail_->horse_list[i].cur_train_attr3 - cfg->attr_3;
		train_4 += mount_detail_->horse_list[i].cur_train_attr4 - cfg->attr_4;
		train_5 += mount_detail_->horse_list[i].cur_train_attr5 - cfg->attr_5;
	}
	base_attr.push_back(base_1);
	base_attr.push_back(base_2);
	base_attr.push_back(base_3);
	base_attr.push_back(base_4);
	base_attr.push_back(base_5);

	train_attr.push_back(train_1);
	train_attr.push_back(train_2);
	train_attr.push_back(train_3);
	train_attr.push_back(train_4);
	train_attr.push_back(train_5);
}

void Logic_Mount::push_horse_attr(std::vector<Int_Int>& attr_list, Int_Int& attr_value){
	int count = attr_list.size();
	for(int i = 0; i < count; ++i){
		if(attr_list[i].val_1 == attr_value.val_1){
			attr_list[i].val_2 += attr_value.val_2;
			return;
		}
	}
	attr_list.push_back(attr_value);
}

int Logic_Mount::get_mount_num(void){
	if(mount_detail_){
		int num = 0;
		int count = mount_detail_->horse_list.size();
		for(int i = 0; i < count; ++i){
			if(mount_detail_->horse_list[i].invalid == 1){
				continue;
			}
			num += 1;
		}
		return num;
	}
	return 0;
}

void Logic_Mount::mount_listen_vip_level_change(uint32_t new_vip_level, uint32_t old_vip_level){
	if(mount_detail_ == NULL){
		return;
	}
	int vip_valid = -1;
	if(old_vip_level == 0){
		if(new_vip_level >= 1){
			vip_valid = 1;
		}
	}else{
		if(new_vip_level <= 0){
			vip_valid = 0;
		}
	}
	if(vip_valid == -1){
		return;
	}
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		int horse_id = mount_detail_->horse_list[i].horse_id;
		Horse_Base_Config* cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
		if(cfg == NULL){
			continue;
		}
		if(cfg->vip_own == 0){
			continue;
		}
		if(mount_detail_->horse_list[i].invalid == 0){
			if(vip_valid == 0){//vip失效
				this->invalid_horse(horse_id);
				//英雄上阵技能刷新
				player_self()->fight_heroer_skill_refresh();
				//英雄属性刷新
				player_self()->fight_heroer_prop_refresh();
			}
		}else{
			if(vip_valid == 1){//自动激活
				this->valid_horse(horse_id);
				//英雄上阵技能刷新
				player_self()->fight_heroer_skill_refresh();
				//英雄属性刷新
				player_self()->fight_heroer_prop_refresh();
			}
		}
	}
}

void Logic_Mount::get_mount_star_lv(std::vector<Int_Int> &info){
	if(mount_detail_ == NULL){
		return;
	}
	int count = mount_detail_->horse_list.size();
	Int_Int info1;
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].invalid == 1){
			continue;
		}
		info1.val_1 = mount_detail_->horse_list[i].horse_id;
		info1.val_2 = mount_detail_->horse_list[i].train_lv;
		info.push_back(info1);
	}
}

void Logic_Mount::get_mount_skill(Int_Vec& skills){
	if(this->mount_detail_ == NULL){
		return;
	}
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].invalid == 1 || mount_detail_->horse_list[i].deploy == 0){
			continue;
		}
		if(mount_detail_->horse_list[i].deploy_skill_1 != 0){
			skills.push_back(mount_detail_->horse_list[i].deploy_skill_1);
		}
		if(mount_detail_->horse_list[i].deploy_skill_2 != 0){
			skills.push_back(mount_detail_->horse_list[i].deploy_skill_2);
		}
		if(mount_detail_->horse_list[i].deploy_skill_3 != 0){
			skills.push_back(mount_detail_->horse_list[i].deploy_skill_3);
		}
	}

	int count_1 = mount_detail_->group_list.size();
	for(int i = 0; i < count_1; ++i){
		if(mount_detail_->group_list[i].status == 0){
			continue;
		}
		Horse_Group_Config* g_cfg = CONFIG_CACHE_MOUNT->get_horse_group_config(mount_detail_->group_list[i].group_id);
		if(!g_cfg){
			continue;
		}
		if(g_cfg->skill == 0){
			continue;
		}
		skills.push_back(g_cfg->skill);
	}
}

void Logic_Mount::get_mount_props_for_hero(Prop_Setter_Vec &vec){
	if(this->mount_detail_ == NULL){
		return;
	}
	int val1 = 0;
	int val2 = 0;
	int val3 = 0;
	int val4 = 0;
	int val5 = 0;
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].invalid == 1){
			continue;
		}
		val1 += mount_detail_->horse_list[i].cur_train_attr1;
		val2 += mount_detail_->horse_list[i].cur_train_attr2;
		val3 += mount_detail_->horse_list[i].cur_train_attr3;
		val4 += mount_detail_->horse_list[i].cur_train_attr4;
		val5 += mount_detail_->horse_list[i].cur_train_attr5;
	}
	Prop_Setter prop_setter;
	prop_setter.operate = O_ADD;
	prop_setter.addi_type = AT_MOUNT;
	if(val1 > 0){
		prop_setter.prop_type = (Property_Type)PT_PHY_ATTACK;
		prop_setter.fixed = val1;
		vec.push_back(prop_setter);
	}
	if(val2 > 0){
		prop_setter.prop_type = (Property_Type)PT_SPELL_ATTACK;
		prop_setter.fixed = val2;
		vec.push_back(prop_setter);
	}
	if(val3 > 0){
		prop_setter.prop_type = (Property_Type)PT_PHY_DEFENSE;
		prop_setter.fixed = val3;
		vec.push_back(prop_setter);
	}
	if(val4 > 0){
		prop_setter.prop_type = (Property_Type)PT_SPELL_DEFENSE;
		prop_setter.fixed = val4;
		vec.push_back(prop_setter);
	}
	if(val5 > 0){
		prop_setter.prop_type = (Property_Type)PT_BLOOD_MAX;
		prop_setter.fixed = val5;
		vec.push_back(prop_setter);
	}
}

int Logic_Mount::add_horse(const int horse_id, const int train_lv, const Time_Value deadline, const Time_Value activate_time,
		const int cur_attr_1, const int cur_attr_2, const int cur_attr_3, const int cur_attr_4, const int cur_attr_5,
		const int max_attr_1, const int max_attr_2, const int max_attr_3, const int max_attr_4, const int max_attr_5,
		const int next_update_num){
	std::vector<int> group_list;
	this->get_valid_group(horse_id, train_lv, group_list);

	mount_detail_->add_horse(horse_id, train_lv, deadline, activate_time, cur_attr_1, cur_attr_2, cur_attr_3, cur_attr_4, cur_attr_5,
			max_attr_1, max_attr_2, max_attr_3, max_attr_4, max_attr_5, next_update_num);
	if(mount_detail_->mount_id == 0){
		mount_detail_->mount_id = horse_id;
	}
	//prop
	add_mount_horse_prop(horse_id);
	// group
	int count = group_list.size();
	for(int i = 0; i < count; ++i){
		add_mount_group_prop(group_list[i]);
		mount_detail_->valid_group(group_list[i]);
	}
	mount_detail_->detail_change();
	return 0;
}

int Logic_Mount::valid_horse(int horse_id, const Time_Value deadline/* = Time_Value::zero*/){
	int train_lv = 0;
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		if(mount_detail_->horse_list[i].horse_id == horse_id){
			train_lv = mount_detail_->horse_list[i].train_lv;
			break;
		}
	}

	std::vector<int> group_list;
	this->get_valid_group(horse_id, train_lv, group_list);
	//prop
	add_mount_horse_prop(horse_id);
	// group
	int count_g = group_list.size();
	for(int i = 0; i < count_g; ++i){
		add_mount_group_prop(group_list[i]);
		mount_detail_->valid_group(group_list[i]);
	}
	this->mount_detail_->valid_horse(horse_id, deadline);
	if(mount_detail_->mount_id == 0){
		mount_detail_->mount_id = horse_id;
	}
	mount_detail_->detail_change();
	return 0;
}

int Logic_Mount::invalid_horse(int horse_id){
	std::vector<int> group_list;
	this->get_invalid_group(horse_id, group_list);
	// prop
	del_mount_horse_prop(horse_id);
	// grop
	int count = group_list.size();
	for(int i = 0; i < count; ++i){
		del_mount_group_prop(group_list[i]);
		this->mount_detail_->invalid_group(group_list[i]);
	}
	//
	this->remove_deploy_skill(horse_id);
	this->mount_detail_->invalid_horse(horse_id);
	// 下马通知
	if(this->mount_detail_->ride == 1 &&
			this->mount_detail_->mount_id == horse_id){
		this->req_mount_ride(0, horse_id);
	}
	if(mount_detail_->mount_id == horse_id){
		mount_detail_->mount_id = mount_detail_->get_first_valid_horse();
		req_fecth_mount_info();
	}
	return 0;
}

void Logic_Mount::get_valid_group(const int horse_id, const int train_lv, std::vector<int>& groups){
	std::vector<Int_Int> h_list;
	std::vector<int> g_list;
	bool b_new_horse = true;
	int h_count = mount_detail_->horse_list.size();
	for(int i = 0; i < h_count; ++i){
		if(mount_detail_->horse_list[i].invalid == 0){
			Int_Int ii_value;
			ii_value.val_1 = mount_detail_->horse_list[i].horse_id;
			ii_value.val_2 = mount_detail_->horse_list[i].train_lv;
			if(mount_detail_->horse_list[i].horse_id == horse_id){
				b_new_horse = false;
				ii_value.val_2 = train_lv;
			}
			h_list.push_back(ii_value);
		}
	}
	if(b_new_horse){
		Int_Int ii_horse;
		ii_horse.val_1 = horse_id;
		ii_horse.val_2 = train_lv;
		h_list.push_back(ii_horse);
	}
	CONFIG_CACHE_MOUNT->find_activate_group(h_list, g_list, mount_detail_->group_list);

	int count = g_list.size();
	for(int i = 0; i < count; ++i){
		bool b_new = true;
		int o_count = mount_detail_->group_list.size();
		for(int k = 0; k < o_count; ++k){
			if(g_list[i] == mount_detail_->group_list[k].group_id){
				if(mount_detail_->group_list[k].status == 0){
					b_new = true;
				}else{
					b_new = false;
				}
				break;
			}
		}
		if(b_new){
			groups.push_back(g_list[i]);
		}
	}
}

void Logic_Mount::get_invalid_group(int horse_id, std::vector<int>& groups){
	std::vector<Int_Int> h_list;
	std::vector<int> g_list;
	int h_count = mount_detail_->horse_list.size();
	for(int i = 0; i < h_count; ++i){
		if(mount_detail_->horse_list[i].invalid == 0){
			Int_Int ii_value;
			ii_value.val_1 = mount_detail_->horse_list[i].horse_id;
			ii_value.val_2 = mount_detail_->horse_list[i].train_lv;
			if(horse_id != mount_detail_->horse_list[i].horse_id){
				h_list.push_back(ii_value);
			}
		}
	}

	std::vector<Horse_Group_Info> group_vec_empty;
	CONFIG_CACHE_MOUNT->find_activate_group(h_list, g_list, group_vec_empty);

	int count = mount_detail_->group_list.size();
	for(int i = 0; i < count; ++i){
		int o_count = g_list.size();
		bool b_old = true;
		for(int k = 0; k < o_count; ++k){
			if(mount_detail_->group_list[i].group_id == g_list[k]){
				b_old = false;
				break;
			}
		}
		if(b_old && mount_detail_->group_list[i].status != 0){
			groups.push_back(mount_detail_->group_list[i].group_id);
		}
	}
}

void Logic_Mount::cal_train_attr_value(void){
	int count = mount_detail_->horse_list.size();
	for(int i = 0; i < count; ++i){
		Horse_Base_Config* cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(mount_detail_->horse_list[i].horse_id);
		if(cfg == NULL){
			continue;
		}
		int max_train_lv = CONFIG_CACHE_MOUNT->get_max_train_lv(cfg->quality);
		// 兼用旧版本，当前驯养等级<初始等级
		if(mount_detail_->horse_list[i].train_lv > max_train_lv){
			mount_detail_->horse_list[i].train_lv = max_train_lv;
		}
		if(mount_detail_->horse_list[i].train_lv < cfg->initial_lv){
			mount_detail_->horse_list[i].train_lv = cfg->initial_lv;
		}
		Horse_Train_Config* ht_cfg = CONFIG_CACHE_MOUNT->get_horse_train_config(mount_detail_->horse_list[i].train_lv, 0);
		if(ht_cfg){
			if(mount_detail_->horse_list[i].train_num > ht_cfg->upgrade_num){
				mount_detail_->horse_list[i].train_num = ht_cfg->upgrade_num;
			}
		}

		CONFIG_CACHE_MOUNT->cal_train_attr(mount_detail_->horse_list[i].horse_id, mount_detail_->horse_list[i].train_lv,
				mount_detail_->horse_list[i].train_num, max_train_lv,
				mount_detail_->horse_list[i].next_update_num, mount_detail_->horse_list[i].total_train_num, mount_detail_->horse_list[i].next_lv_total_train_num,
				mount_detail_->horse_list[i].cur_train_attr1, mount_detail_->horse_list[i].cur_train_attr2,
				mount_detail_->horse_list[i].cur_train_attr3, mount_detail_->horse_list[i].cur_train_attr4,
				mount_detail_->horse_list[i].cur_train_attr5,
				mount_detail_->horse_list[i].max_train_attr1, mount_detail_->horse_list[i].max_train_attr2,
				mount_detail_->horse_list[i].max_train_attr3, mount_detail_->horse_list[i].max_train_attr4,
				mount_detail_->horse_list[i].max_train_attr5
				);
		int add_luck = CONFIG_CACHE_MOUNT->get_train_fail_luck_value(mount_detail_->horse_list[i].train_lv,
				mount_detail_->horse_list[i].train_num);
		mount_detail_->horse_list[i].luck_value = mount_detail_->horse_list[i].train_fail_num*add_luck;
	}
}

void Logic_Mount::hook_mount_star_upgrade(const int horse_id, const int star_level){
	// 驯养升级广播
	Horse_Train_Config* ht_cfg = CONFIG_CACHE_MOUNT->get_horse_train_config(star_level, 0);
	Horse_Base_Config* hb_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(!ht_cfg || !hb_cfg){
		return;
	}
	if(ht_cfg->broadcast != 0){
		std::stringstream s_stream1;
		s_stream1<<star_level;
		announce_train_up(ANN_ID_MOUNT_TRIAN_UP+hb_cfg->quality-2, hb_cfg->horse_name, s_stream1.str());
	}
	// 变身
	int pre_star_lv = star_level - 1;
	if(pre_star_lv < 0){
		pre_star_lv = 0;
	}
	int pre_model_id = get_mount_model_id(horse_id, pre_star_lv);
	int model_id = get_mount_model_id(horse_id, star_level);
	if(pre_model_id != model_id){// 模型改变
		// 变身广播
		{
			std::stringstream s_stream1;
			s_stream1<<star_level;
			announce_train_up(ANN_ID_MOUNT_SHAPE_UP+hb_cfg->quality-2, hb_cfg->horse_name, s_stream1.str(), hb_cfg->horse_name);
		}

		// 更新坐骑模型
		if(mount_detail_->ride != 0 && mount_detail_->mount_id == horse_id){
			this->mount_detail_->detail_change();
			player_self()->base_detail_no_const().ride = mount_detail_->ride;
			player_self()->base_detail_no_const().mount_model_id = model_id;
			// 同步到场景服
			MSG_20100040 msg;
			msg.ride = mount_detail_->ride;
			msg.model_id = model_id;
			msg.speed = 0;
			THIS_SEND_TO_SCENE(msg);
		}
	}

	// 指定id的坐骑升到xx星级
	this->player_self()->task_listen_horse_star_level(horse_id,star_level);
	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_HORSE_STAR_LEVEL, horse_id, star_level);
	player_self()->req_active_content_tip_num();
}

int Logic_Mount::get_mount_model_id(const int horse_id, const int train_lv){
	Horse_Base_Config* h_cfg = CONFIG_CACHE_MOUNT->get_horse_base_config(horse_id);
	if(h_cfg){
		return h_cfg->get_mod_id(train_lv);
	}
	return 0;
}


int Logic_Mount::announce_train_up(const int ann_id, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Chat_Content new_content;

	{
		Role_Info role_info;
		role_info.reset();
		role_info.role_id = this->role_id();
		role_info.role_name = this->role_name();

		new_content.reset();
		new_content.type = ANN_CTTYPE_ROLE_INFO;
		new_content.role_Info = role_info;
		ann_contents.push_back(new_content);
	}

	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	Block_Buffer buf;
	MSG_20500000 msg;
	msg.district = ANNOUNCE_DISTRICT_SYSTEM;
	msg.announce_id = ann_id;
	msg.contents = ann_contents;

	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
	return 0;
}

bool Logic_Mount::update_horse_group(const int horse_id, const int train_lv){
	std::vector<int> group_list;
	this->get_valid_group(horse_id, train_lv, group_list);
	// group
	int count = group_list.size();
	for(int i = 0; i < count; ++i){
		add_mount_group_prop(group_list[i]);
		mount_detail_->valid_group(group_list[i]);
	}
	mount_detail_->detail_change();
	if(count > 0){
		return true;
	}
	return false;
}

void Logic_Mount::horse_group_first_check(void){
	int num = mount_detail_->horse_list.size();
	for(int k = 0; k < num; ++k){
		if(mount_detail_->horse_list[k].invalid != 0){
			continue;
		}
		int horse_id = mount_detail_->horse_list[k].horse_id;
		int train_lv = mount_detail_->horse_list[k].train_lv;
		std::vector<int> group_list;
		this->get_valid_group(horse_id, train_lv, group_list);
		// group
		int count = group_list.size();
		for(int i = 0; i < count; ++i){
			mount_detail_->valid_group(group_list[i]);
		}
	}
}

bool Logic_Mount::is_horse_star_lv_enough(const int horse_id, const int star_lv){
	for(std::vector<Horse_Info>::iterator it = mount_detail_->horse_list.begin();
			it != mount_detail_->horse_list.end(); ++it){
		if(it->horse_id == horse_id){
			if(it->train_lv >= star_lv){
				return true;
			}
			return false;
		}
	}
	return false;
}
