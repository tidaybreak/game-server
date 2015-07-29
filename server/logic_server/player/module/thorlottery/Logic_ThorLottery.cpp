/*
 * Logic_ThorLottery.cpp
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#include "Logic_ThorLottery.h"
#include "ThorLottery_Manager.h"
#include "Config_Cache_ThorLottery.h"
#include "Config_Cache.h"
#include "Msg_ThorLottery_Struct.h"
#include "Msg_Active_Struct.h"
#include "ThorLottery_Struct.h"
#include "ThorLottery_Config.h"
#include "Msg_Struct.h"
#include "Err_Code.h"
#include "pack/Pack_Struct.h"
#include "pack/Pack_Def.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "Logic_Player.h"

const static int LOTTERY_WEIGHT_TIMES = 10;
const static int THORLOTTERY_COST_GOODS_ID = 20516401;

const static int COST_IN_ONE_TIME = 20;
const static int COST_IN_TEN_TIME = 18;
const static int COST_IN_FIFTH_TIME = 18;

const static int COST_IN_ONE_TIME_EN = 50;
const static int COST_IN_TEN_TIME_EN = 45;
const static int COST_IN_FIFTH_TIME_EN = 45;

Logic_ThorLottery::Logic_ThorLottery(void){ reset(); }

Logic_ThorLottery::~Logic_ThorLottery(void) {

}

void Logic_ThorLottery::reset(void) {

	thorlottery_detail_ = 0;
}

void Logic_ThorLottery::load_detail(ThorLottery_Detail &detail) {

	thorlottery_detail_ = &detail;
}

void Logic_ThorLottery::save_detail(ThorLottery_Detail &detail) {

	if (thorlottery_detail_->is_change) {
		detail = thorlottery_detail();
		thorlottery_detail_->is_change = false;
	}
}

void Logic_ThorLottery::sign_in(void) {

}

void Logic_ThorLottery::sign_out(void){

}

void Logic_ThorLottery::module_init(void) {

	if (0 == thorlottery_detail_) return;

	const ThorLottery_Config* cfg =
			CONFIG_CACHE_THORLOTTERY->get_thorlottery_config(thorlottery_detail_->season_id);
	if (0 == cfg) return;

	ThorLottery_Item item;
	for (ThorLottery_Order_Config_Map::const_iterator it = cfg->thorlottery_order_config_map.begin();
			it !=  cfg->thorlottery_order_config_map.end(); ++it) {
		if (it->second.max_lottery <= 0 && it->second.min_lottery <= 0) continue;

		ThorLottery_Item_Map::iterator find_it =
				thorlottery_detail_->thorlottery_item_map_.find(it->second.order);
		if (find_it != thorlottery_detail_->thorlottery_item_map_.end()) continue;
		item.reset();
		item.index_ = it->second.order;
		thorlottery_detail_->thorlottery_item_map_[item.index_] = item;
	}
}

int Logic_ThorLottery::trigger_daily_six(const Time_Value& now) {

	// 六点清零，但分数不清零
//	thorlottery_detail_->season_id 		= 0;
//	thorlottery_detail_->lottery_time_ 	= Time_Value::zero;
//	thorlottery_detail_->thorlottery_item_map_.clear();
//	thorlottery_detail_->is_change = true;
	thorlottery_detail_->daily_lottery_times_ = 0;
	return 0;
}

const ThorLottery_Detail& Logic_ThorLottery::thorlottery_detail(void) {
	return *thorlottery_detail_;
}

int Logic_ThorLottery::get_thorlottery_info(void) {

	if (0 == thorlottery_detail_) return ERROR_INNER_SERVER;
	MSG_50108001 msg;
	const ThorLottery_Manager_Detail& manager_detail = THORLOTTERY_MANAGER->get_thorlottery_manager_detail();
	msg.season_id = manager_detail.season_id_;
	msg.score = thorlottery_detail_->score;
	msg.daily_lottery_times = thorlottery_detail_->daily_lottery_times_;
	msg.time_left = THORLOTTERY_MANAGER->get_thorlottery_end_time(Time_Value::gettimeofday());
	ThorLottery_Log log;
	for (ThorLottery_Log_Vec::const_iterator it = manager_detail.lottery_log_vec_.begin();
			it != manager_detail.lottery_log_vec_.end(); ++it) {
		log.reset();
		log.time = it->time_.sec();
		log.role_name = it->role_name_;
		log.role_id = it->role_id_;
		log.item_id = it->item_id_;
		log.item_num = it->item_num_;
		log.state = it->bind_state_;

		msg.log_vec.push_back(log);
	}

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

void Logic_ThorLottery::init_player_lottery_data(int season_id) {

	thorlottery_detail_->season_id 		= 0;
	thorlottery_detail_->lottery_time_ 	= Time_Value::zero;
	thorlottery_detail_->thorlottery_item_map_.clear();

	const ThorLottery_Config* cfg = CONFIG_CACHE_THORLOTTERY->get_thorlottery_config(season_id);
	if (0 == cfg) {
		MSG_USER("cannot find thorlottery config data, season_id=%d", season_id);
		return;
	}

	thorlottery_detail_->season_id = season_id;
	ThorLottery_Item item;
	for (ThorLottery_Order_Config_Map::const_iterator it = cfg->thorlottery_order_config_map.begin();
			it != cfg->thorlottery_order_config_map.end(); ++it) {
		if (it->second.max_lottery <= 0 && it->second.min_lottery <= 0) continue;
		item.reset();
		item.index_ = it->second.order;
		thorlottery_detail_->thorlottery_item_map_[item.index_] = item;
	}
}

int Logic_ThorLottery::handle_lottery(int type, int times) {

	if (times <= 0) return ERROR_CLIENT_PARAM;
	int state = THORLOTTERY_MANAGER->get_thorlottery_state();
	if (ThorLottery_Manager::STATE_OFF == state) return ERROR_THORLOTTERY_NOT_OPEN;
	if (0 == thorlottery_detail_) return ERROR_INNER_SERVER;

	// 重置抽奖数据
	int season_id = THORLOTTERY_MANAGER->get_thorlottery_season_id();
	if (0 == season_id) return ERROR_INNER_SERVER;
	if (season_id != thorlottery_detail_->season_id) {
		init_player_lottery_data(season_id);
	}

	// backup player's item map
	ThorLottery_Item_Map backup_item_map = thorlottery_detail_->thorlottery_item_map_;

	Get_Item_Map get_item_map;
	int ret_val = 0;
	ThorLottery_Item_Return_Vec item_vec;
	if (0 == type) {
		ret_val = lottery_by_diamond(times, get_item_map, item_vec);
	} else if (1 == type) {
		ret_val = lottery_by_goods(times, get_item_map, item_vec);
	} else if (2 == type){
		ret_val = lottery_by_diamond_and_goods(times, get_item_map, item_vec);
	} else{
		return ERROR_CLIENT_PARAM;
	}

	// 抽奖失败处理
	if (0 != ret_val) {

		if (get_item_map.empty()) {
			backup_item_map.clear();
			return ret_val;
		}
		MSG_USER("lottery_error, size =%d", get_item_map.size());
		thorlottery_detail_->thorlottery_item_map_.clear();
		thorlottery_detail_->thorlottery_item_map_ = backup_item_map;
		backup_item_map.clear();

		for (Get_Item_Map::iterator it = get_item_map.begin(); it != get_item_map.end(); ++it) {
			THORLOTTERY_MANAGER->del_thorlottery_item(it->second->order);
		}

		return ret_val;
	}

	MSG_50108002 msg;
	MSG_81000102 item_msg;
	//add goods to depot
	Item_Basic_Info item_basic;
	Card_Info card_info;
	Chat_Item_Info chat_item_info;
	ThorLottery_Reward_Item reward_item;

	for (ThorLottery_Item_Return_Vec::iterator it = item_vec.begin(); it != item_vec.end(); ++it) {

		item_basic.reset();
		card_info.reset();
		it->item.set_item_basic_info(item_basic);
		item_msg.item_info.push_back(item_basic);
		if (it->item.set_item_card_info(card_info)) {
			item_msg.card_info_vec.push_back(card_info);
		}
		reward_item.reset();
		reward_item.item_index = it->num;
		reward_item.item_index = it->cfg_thorlottery_item->order;
		reward_item.id = it->item.id_;
		reward_item.num = it->item.amount_;
		reward_item.state = it->item.bind_;
		msg.item_vec.push_back(reward_item);

		THORLOTTERY_MANAGER->add_thorlottery_log(role_id(),  role_name(),
				it->item.id_, it->item.amount_, it->item.bind_);

		if (1 == it->cfg_thorlottery_item->is_broad) {
			chat_item_info.reset();
			it->item.set_item_basic_info(chat_item_info.item_base_info);
			if (it->item.set_item_card_info(chat_item_info.card_info)) {
				chat_item_info.type = Item_Detail::CARD;
			}
			player_self()->thorlottery_item(chat_item_info);
		}
	}

	thorlottery_detail_->score += times;
	thorlottery_detail_->daily_lottery_times_ += times;
	thorlottery_detail_->is_change = true;
	THORLOTTERY_MANAGER->set_detail_change(true);

	msg.score = thorlottery_detail_->score;
	msg.daily_lottery_times = thorlottery_detail_->daily_lottery_times_;
	const ThorLottery_Manager_Detail& manager_detail =
			THORLOTTERY_MANAGER->get_thorlottery_manager_detail();
	ThorLottery_Log log;
	for (ThorLottery_Log_Vec::const_iterator it = manager_detail.lottery_log_vec_.begin();
			it != manager_detail.lottery_log_vec_.end(); ++it) {
		log.reset();
		log.time = it->time_.sec();
		log.role_name = it->role_name_;
		log.role_id = it->role_id_;
		log.item_id = it->item_id_;
		log.item_num = it->item_num_;
		log.state = it->bind_state_;

		msg.log_vec.push_back(log);
	}

	if (!item_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(item_msg);
	}

	THIS_SEND_TO_CLIENT(msg);

	player_self()->task_listen_lottery_draw(times);

	return 0;
}

int Logic_ThorLottery::lottery_by_diamond(
		int times, Get_Item_Map& get_item_map, ThorLottery_Item_Return_Vec& item_return_vec) {

	int diamond_cost = handle_calc_cost_need(0, times);
	Money_Sub_Info money_sub_info(Money_Sub_Type(GOLD_ONLY),
			diamond_cost, MONEY_SUb_THOR_LOTTERY);
	int ret_val = player_self()->pack_try_sub_money(money_sub_info);
	if (0 != ret_val) return ret_val;

	ThorLottery_Item_Config const *item = 0;
	for (int i = 1; i <= times; ++i) {
		item = handle_lottery_once();
		if (0 == item) return ERROR_INNER_SERVER;

		get_item_map[i] = item;
		// add value to thorlottery_manager_detail
		THORLOTTERY_MANAGER->add_thorlottery_item(item->order);
	}

	Item_Vec item_vec;
	Item_Vec temp_item_vec;
	ThorLottery_Item_Return item_return;
	for (Get_Item_Map::iterator it = get_item_map.begin(); it != get_item_map.end(); ++it) {

		temp_item_vec.clear();
		int num = it->second->num;
		if (1 == (thorlottery_detail_->daily_lottery_times_ + it->first)) {
			num *= 2;
		}
		make_item_vec(temp_item_vec, it->second->id, num , it->second->bind_state);
		for (Item_Vec::iterator item_it = temp_item_vec.begin(); item_it != temp_item_vec.end(); ++item_it) {

			item_vec.push_back(*item_it);
			item_return.reset();
			item_return.num = it->first;
			item_return.item = *item_it;
			item_return.cfg_thorlottery_item = it->second;
			item_return_vec.push_back(item_return);
		}
	}

	ret_val = player_self()->pack_insert_item(PACK_T_THOR_LOTTERY_PACK_INDEX,
			item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_THOR_LOTTERY));
	if (0 != ret_val) return ret_val;

	player_self()->pack_sub_money(money_sub_info);

	return 0;
}

int Logic_ThorLottery::lottery_by_goods(
		int times, Get_Item_Map& get_item_map, ThorLottery_Item_Return_Vec& item_return_vec) {

	Id_Amount item_cost(THORLOTTERY_COST_GOODS_ID, times);
	int ret_val = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, item_cost);
	if (0 != ret_val) return ret_val;

	ThorLottery_Item_Config const *item = 0;
	for (int i = 1; i <= times; ++i) {
		item = handle_lottery_once();
		if (0 == item) return ERROR_INNER_SERVER;

		get_item_map[i] = item;
		// add value to thorlottery_manager_detail
		THORLOTTERY_MANAGER->add_thorlottery_item(item->order);
	}

	Item_Vec item_vec;
	Item_Vec temp_item_vec;
	ThorLottery_Item_Return item_return;
	for (Get_Item_Map::iterator it = get_item_map.begin(); it != get_item_map.end(); ++it) {

		temp_item_vec.clear();
		int num = it->second->num;
		if (1 == (thorlottery_detail_->daily_lottery_times_ + it->first)) {
			num *= 2;
		}
		make_item_vec(temp_item_vec, it->second->id, num , it->second->bind_state);
		for (Item_Vec::iterator item_it = temp_item_vec.begin(); item_it != temp_item_vec.end(); ++item_it) {

			item_vec.push_back(*item_it);
			item_return.reset();
			item_return.num = it->first;
			item_return.item = *item_it;
			item_return.cfg_thorlottery_item = it->second;
			item_return_vec.push_back(item_return);
		}
	}

	ret_val = player_self()->pack_insert_item(PACK_T_THOR_LOTTERY_PACK_INDEX,
			item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_THOR_LOTTERY));
	if (0 != ret_val) return ret_val;

	player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost,
			Lose_Item_DM_Info(Pack::ITEM_LOSE_THOR_LOTTERY));

	return 0;
}

int Logic_ThorLottery::lottery_by_diamond_and_goods(
		int times, Get_Item_Map& get_item_map, ThorLottery_Item_Return_Vec& item_return_vec) {

	int amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX,
			THORLOTTERY_COST_GOODS_ID);
	amount = times > amount ? amount : times;
	int diamond_cost = handle_calc_cost_need(amount, times);
	Money_Sub_Info money_sub_info(Money_Sub_Type(GOLD_ONLY),
			diamond_cost, MONEY_SUb_THOR_LOTTERY);
	if (0 != diamond_cost) {
		int ret = player_self()->pack_try_sub_money(money_sub_info);
		if (0 != ret ) return ret;
	}

	ThorLottery_Item_Config const *item = 0;
	for (int i = 1; i <= times; ++i) {
		item = handle_lottery_once();
		if (0 == item) return ERROR_INNER_SERVER;

		get_item_map[i] = item;
		// add value to thorlottery_manager_detail
		THORLOTTERY_MANAGER->add_thorlottery_item(item->order);
	}

	Item_Vec item_vec;
	Item_Vec temp_item_vec;
	ThorLottery_Item_Return item_return;
	for (Get_Item_Map::iterator it = get_item_map.begin(); it != get_item_map.end(); ++it) {

		temp_item_vec.clear();
		int num = it->second->num;
		if (1 == (thorlottery_detail_->daily_lottery_times_ + it->first)) {
			num *= 2;
		}
		make_item_vec(temp_item_vec, it->second->id, num , it->second->bind_state);
		for (Item_Vec::iterator item_it = temp_item_vec.begin(); item_it != temp_item_vec.end(); ++item_it) {

			item_vec.push_back(*item_it);
			item_return.reset();
			item_return.num = it->first;
			item_return.item = *item_it;
			item_return.cfg_thorlottery_item = it->second;
			item_return_vec.push_back(item_return);
		}
	}

	int ret_val = player_self()->pack_insert_item(PACK_T_THOR_LOTTERY_PACK_INDEX,
			item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_THOR_LOTTERY));
	if (0 != ret_val) return ret_val;

	if (amount > 0) {
		Id_Amount item_cost(THORLOTTERY_COST_GOODS_ID, amount);
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
				item_cost, Lose_Item_DM_Info(Pack::ITEM_LOSE_THOR_LOTTERY));
	}
	if (diamond_cost > 0) {
		player_self()->pack_sub_money(money_sub_info);
	}

	return 0;

}

int Logic_ThorLottery::handle_lottery(int times) {

typedef boost::unordered_map<int, ThorLottery_Item_Config const*> Get_Item_Map;

	if (times <= 0) return ERROR_INNER_SERVER;
	int state = THORLOTTERY_MANAGER->get_thorlottery_state();
	if (ThorLottery_Manager::STATE_OFF == state) return ERROR_THORLOTTERY_NOT_OPEN;
	if (0 == thorlottery_detail_) return ERROR_INNER_SERVER;

	// 重置抽奖数据
	int season_id = THORLOTTERY_MANAGER->get_thorlottery_season_id();
	if (0 == season_id) return ERROR_INNER_SERVER;
	if (season_id != thorlottery_detail_->season_id) {
		init_player_lottery_data(season_id);
	}

	// backup player's item map
	ThorLottery_Item_Map backup_item_map = thorlottery_detail_->thorlottery_item_map_;

	// check goods amount and gold
	int amount = player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, THORLOTTERY_COST_GOODS_ID);
	amount = times > amount ? amount : times;
	int diamond_cost = handle_calc_cost_need(amount, times);
	Money_Sub_Info money_sub_info(Money_Sub_Type(GOLD_ONLY), diamond_cost, MONEY_SUb_THOR_LOTTERY);
	if (0 != diamond_cost) {
		int ret = player_self()->pack_try_sub_money(money_sub_info);
		if (0 != ret) return ret;
	}

	Get_Item_Map get_item_map;
	ThorLottery_Item_Config const* item = 0;
	for(int i=1; i<=times; ++i) {
		item = handle_lottery_once();
		if (0 == item) break;

		get_item_map[i] = item;
		// add value to thorlottery_manager_detail
		THORLOTTERY_MANAGER->add_thorlottery_item(item->order);
	}

	if (0 == item) {
		MSG_USER("lottey_error, size=%d", get_item_map.size());
		thorlottery_detail_->thorlottery_item_map_.clear();
		thorlottery_detail_->thorlottery_item_map_ = backup_item_map;
		backup_item_map.clear();
		// del value from thorlottery_manager_detail
		for (Get_Item_Map::iterator it = get_item_map.begin(); it != get_item_map.end(); ++it) {
			THORLOTTERY_MANAGER->del_thorlottery_item(it->second->order);
		}

		return ERROR_INNER_SERVER;
	}

	// cost goods and money
	if (amount > 0) {
		Id_Amount item_cost(THORLOTTERY_COST_GOODS_ID, amount);
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item_cost, Lose_Item_DM_Info(Pack::ITEM_LOSE_THOR_LOTTERY));
	}
	if (diamond_cost > 0) {
		player_self()->pack_sub_money(money_sub_info);
	}

	MSG_81000102 item_msg;
	MSG_50108002 msg;
	//add goods to depot
	Item_Vec item_vec;
	Item_Vec item_vec_temp;
	Item_Basic_Info item_basic;
	Card_Info card_info;
	Chat_Item_Info chat_item_info;
	ThorLottery_Reward_Item reward_item;
	for (Get_Item_Map::iterator it = get_item_map.begin(); it != get_item_map.end(); ++it) {
		item_basic.reset();
		card_info.reset();
		item_vec_temp.clear();
		int num = it->second->num;
		if (1 == (thorlottery_detail_->daily_lottery_times_ + it->first)) {
			num *= 2;
		}
		make_item_vec(item_vec_temp, it->second->id, num, it->second->bind_state);
//		Item_Detail item_detail(it->second->id, it->second->num, it->second->bind_state);
//		item_vec.push_back(item_detail);

		for (Item_Vec::iterator item_it = item_vec_temp.begin();
				item_it != item_vec_temp.end(); ++item_it) {
			item_vec.push_back(*item_it);
			(*item_it).set_item_basic_info(item_basic);
			item_msg.item_info.push_back(item_basic);
			if ((*item_it).set_item_card_info(card_info)) {
				item_msg.card_info_vec.push_back(card_info);
			}
			reward_item.reset();
			reward_item.index = it->first;
			reward_item.item_index = it->second->order;
			reward_item.id = item_it->id_;
			reward_item.num = item_it->amount_;
			reward_item.state = item_it->bind_;
			msg.item_vec.push_back(reward_item);

			THORLOTTERY_MANAGER->add_thorlottery_log(role_id(), role_name(),
					item_it->id_, item_it->amount_, item_it->bind_);

			if (1 == it->second->is_broad) {
				chat_item_info.reset();
				(*item_it).set_item_basic_info(chat_item_info.item_base_info);
				if ((*item_it).set_item_card_info(chat_item_info.card_info)) {
					chat_item_info.type = Item_Detail::CARD;
				}
				player_self()->thorlottery_item(chat_item_info);
			}
		}
	}

	int ret = player_self()->pack_insert_item(PACK_T_THOR_LOTTERY_PACK_INDEX, item_vec,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_THOR_LOTTERY));
	if (0 != ret) return ret;

	thorlottery_detail_->score += times;
	thorlottery_detail_->daily_lottery_times_ += times;
	thorlottery_detail_->is_change = true;
	THORLOTTERY_MANAGER->set_detail_change(true);

	msg.score = thorlottery_detail_->score;
	msg.daily_lottery_times = thorlottery_detail_->daily_lottery_times_;
	const ThorLottery_Manager_Detail& manager_detail = THORLOTTERY_MANAGER->get_thorlottery_manager_detail();
	ThorLottery_Log log;
	for (ThorLottery_Log_Vec::const_iterator it = manager_detail.lottery_log_vec_.begin();
			it != manager_detail.lottery_log_vec_.end(); ++it) {
		log.reset();
		log.time = it->time_.sec();
		log.role_name = it->role_name_;
		log.role_id = it->role_id_;
		log.item_id = it->item_id_;
		log.item_num = it->item_num_;
		log.state = it->bind_state_;

		msg.log_vec.push_back(log);
	}

	if (!item_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(item_msg);
	}

	THIS_SEND_TO_CLIENT(msg);

	player_self()->task_listen_lottery_draw(times);
	return 0;
}

ThorLottery_Item_Config const* Logic_ThorLottery::handle_lottery_once(void) {
	int season_id = THORLOTTERY_MANAGER->get_thorlottery_season_id();
	const ThorLottery_Config* cfg = CONFIG_CACHE_THORLOTTERY->get_thorlottery_config(season_id);
	if (0 == cfg) {
		MSG_USER("cannot find torlottery config, season id=%d", season_id);
		return 0;
	}
	if (cfg->thorlottery_order_config_map.empty()) {
		MSG_USER("cannot find season's items, season id =%d", season_id);
		return 0;
	}

	const ThorLottery_Order_Config_Map& cfg_order_map = cfg->thorlottery_order_config_map;
	const ThorLottery_Manager_Detail& manager_detail = THORLOTTERY_MANAGER->get_thorlottery_manager_detail();
	Int_Int_Vec order_weight_vec;
	Int_Int order_weight;
	for (ThorLottery_Order_Config_Map::const_iterator it = cfg_order_map.begin(); it != cfg_order_map.end(); ++it) {
		int wgh = get_item_weight(it->second, manager_detail);
		if (wgh < 0) continue;
		order_weight.reset();
		order_weight.val_1 = it->second.order;
		order_weight.val_2 = wgh;
		order_weight_vec.push_back(order_weight);
	}

	int weight_ttl = 0;
	for (Int_Int_Vec::iterator it = order_weight_vec.begin(); it != order_weight_vec.end(); ++it) {
		weight_ttl += it->val_2;
	}

	if (weight_ttl <= 0) {
		MSG_USER("cannot find any reward items, please check the configuration, season id=%d", season_id);
		return 0;
	}

	int rand = random() % weight_ttl;
	int select_order = 0;
	for (Int_Int_Vec::iterator it = order_weight_vec.begin(); it != order_weight_vec.end(); ++it) {
		if (rand >= it->val_2) {
			rand -= it->val_2;
			continue;
		}

		select_order = it->val_1;
		break;
	}

	ThorLottery_Order_Config_Map::const_iterator find_it = cfg_order_map.find(select_order);
	if (cfg_order_map.end() == find_it) return 0;

	weight_ttl = 0;
	for (ThorLottery_Item_Config_Map::const_iterator it = find_it->second.thorlottery_item_config_map.begin();
			it != find_it->second.thorlottery_item_config_map.end(); ++it) {
		weight_ttl += it->second.weight;
	}

	ThorLottery_Item_Config const* item_cfg = 0;
	rand = random() % weight_ttl;
	for (ThorLottery_Item_Config_Map::const_iterator it = find_it->second.thorlottery_item_config_map.begin();
			it != find_it->second.thorlottery_item_config_map.end(); ++it) {
		if (rand >= it->second.weight) {
			rand -= it->second.weight;
			continue;
		}
		item_cfg = &(it->second);
		break;
	}

	if (0 == item_cfg)
		return 0;

	for (ThorLottery_Item_Map::iterator it = thorlottery_detail_->thorlottery_item_map_.begin();
			it != thorlottery_detail_->thorlottery_item_map_.end(); ++it) {
		if (it->second.index_ == find_it->second.order) {
			it->second.counter_ = 0;
		} else {
			++it->second.counter_;
		}
	}

	return item_cfg;
}

int Logic_ThorLottery::get_item_weight(const ThorLottery_Order_Config &cfg,
		const ThorLottery_Manager_Detail& manager_detail) {

	int player_get_times = 0;
	ThorLottery_Item_Map::const_iterator find_it = thorlottery_detail_->thorlottery_item_map_.find(cfg.order);
	if (thorlottery_detail_->thorlottery_item_map_.end() == find_it) {
		player_get_times = 0;
	} else {
		player_get_times = find_it->second.counter_;
	}

	int server_get_times = 0;
	ThorLottery_Item_Map::const_iterator manager_find_it = manager_detail.thorlottery_item_map_.find(cfg.order);
	if (manager_detail.thorlottery_item_map_.end() == manager_find_it) {
		server_get_times = 0;
	} else {
		server_get_times = manager_find_it->second.counter_;
	}

	if (cfg.max_lottery >0 && player_get_times >= cfg.max_lottery) return cfg.weight * LOTTERY_WEIGHT_TIMES;
	if (cfg.server_limit > 0 && server_get_times >= cfg.server_limit) return 0;
	if (player_get_times < cfg.min_lottery) return 0;

	return cfg.weight;
}

int Logic_ThorLottery::handle_calc_cost_need(int goods_num, int times) {
	if (goods_num >= times) return 0;
	int once_price = 0;
	if (CONFIG_CACHE->server_maintainer_cache().game_ver_type == 1) {
		if (1 == times) {
			return COST_IN_ONE_TIME_EN;
		} else if (10 == times) {
			return COST_IN_TEN_TIME_EN * (times - goods_num);
		} else if (50 == times) {
			return COST_IN_FIFTH_TIME_EN * (times - goods_num);
		} else {
			once_price = COST_IN_ONE_TIME_EN;
		}

	} else {
		if (1 == times) {
			return COST_IN_ONE_TIME;
		} else if (10 == times) {
			return COST_IN_TEN_TIME * (times - goods_num);
		} else if (50 == times) {
			return COST_IN_FIFTH_TIME * (times - goods_num);
		} else {
			once_price = COST_IN_ONE_TIME;
		}

	}

	MSG_USER("error args, times = %d, role_id=%ld", times, role_id());
	return once_price * (times - goods_num);
}

int Logic_ThorLottery::handle_exchange_item(int order, int item_id) {

	if (0 == thorlottery_detail_) return ERROR_INNER_SERVER;
	int season_id = THORLOTTERY_MANAGER->get_thorlottery_season_id();
	if (0 == season_id) return ERROR_INNER_SERVER;

	const ThorLottery_Item_Config *cfg = CONFIG_CACHE_THORLOTTERY->get_thorlottery_item_config(season_id, order, item_id);
	if (0 == cfg) return ERROR_INNER_SERVER;

	if (cfg->exchange_cost <= 0) return ERROR_INNER_SERVER;

	if (thorlottery_detail_->score < cfg->exchange_cost) return ERROR_THORLOTTERY_SCORE_LIMIT;

	MSG_81000102 item_msg;

	Item_Vec item_vec;
	Item_Basic_Info item_basic;
	Card_Info card_info;
	Chat_Item_Info chat_item_info;
//	Item_Detail item(cfg->id, cfg->num, cfg->bind_state);
	make_item_vec(item_vec, cfg->id, cfg->num, cfg->bind_state);
//	item_vec.push_back(item);
	for (Item_Vec::iterator it = item_vec.begin(); it != item_vec.end(); ++it) {
		item_basic.reset();
		card_info.reset();
		(*it).set_item_basic_info(item_basic);
		item_msg.item_info.push_back(item_basic);
		if ((*it).set_item_card_info(card_info)) {
			item_msg.card_info_vec.push_back(card_info);
		}
		chat_item_info.reset();
		(*it).set_item_basic_info(chat_item_info.item_base_info);
		(*it).set_item_card_info(chat_item_info.card_info);
		if (1 == cfg->is_broad) {
			player_self()->thorlottery_exchange(chat_item_info);
		}
	}
//	item.set_item_basic_info(item_basic);
//	item.set_item_basic_info(chat_item_info.item_base_info);
	int ret = player_self()->pack_insert_item(PACK_T_THOR_LOTTERY_PACK_INDEX, item_vec,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_THOR_LOTTERY));

	if (0 != ret) return ret;

//	if (1 == cfg->is_broad) {
//		player_self()->thorlottery_exchange(chat_item_info);
//	}

	thorlottery_detail_->score -= cfg->exchange_cost;
	thorlottery_detail_->is_change = true;

//	item_msg.item_info.push_back(item_basic);
	if (!item_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(item_msg);
	}

	MSG_50108003 msg;
	msg.score = thorlottery_detail_->score;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
