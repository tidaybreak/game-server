/*
 * Covenant.cpp
 *
 *  Created on: 2014-07-15
 *      Author: xiaoliang
 */

#include "Covenant.h"
#include "Covenant_Struct.h"
#include "Logic_Module_Head.h"
#include "Covenant_Config.h"
#include "Msg_Active_Struct.h"
#include "Covenant_Def.h"
#include "Config_Cache_Covenant.h"
#include "pack/Pack_Struct.h"
#include "Logic_Player.h"
#include "Err_Code.h"
#include "Msg_Covenant_Struct.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Mail.h"
#include "Logic_Public.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "announce/Logic_Announce.h"
#include "Config_Cache_Buff.h"
#include "Config_Cache_Scene.h"
#include "public/Public_Def.h"

Covenant::Covenant(void) {
	reset();
}

Covenant::~Covenant() {
	// TODO Auto-generated destructor stub
}

void Covenant::reset(void) {
	covenant_detail_ = 0;
	status_has_effect_ = false;
}

int Covenant::create_init(Covenant_Detail &detail) {

	return 0;
}

void Covenant::load_detail(Covenant_Detail &detail) {
	covenant_detail_ = &detail;
}

void Covenant::save_detail(Covenant_Detail &detail) {
	if (covenant_detail().is_change) {
		detail = covenant_detail();
		covenant_detail_->is_change = false;
	}
}

int Covenant::trigger_daily_zero(const Time_Value &now) {
	for(Covenant_Data_Map::iterator cdit = covenant_detail_->covenant_data_map.begin();
		cdit != covenant_detail_->covenant_data_map.end(); ++cdit) {
		cdit->second.today_become_time = Time_Value::zero;
		covenant_detail_->detail_change();
	}
	return 0;
}

int Covenant::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

void Covenant::listen_covenant_expired(const Time_Value &now) {
	MSG_82200222 msg;
	msg.reset();
	uint32_t over_num = 0;
	for(Covenant_Data_Map::iterator cdit = covenant_detail_->covenant_data_map.begin();
			cdit != covenant_detail_->covenant_data_map.end(); ++cdit) {
		if(cdit->second.over_time <= now) {
			++over_num;
		}
		if(cdit->second.over_time <= now && cdit->second.init_status == COVENANT_HAVE_BUY) {
			const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(cdit->second.id);
			if(conf) {
				msg.id.push_back(conf->id);
				//send_expired_mail(conf->mail_id);
				cdit->second.init_status = COVENANT_HAVE_EXPIRED;
				covenant_detail_->detail_change();
				covenant_expired_listen(conf->id, cdit->second.over_time, now);
			}
		}
	}
	if(msg.id.size()) {
		THIS_SEND_TO_CLIENT(msg);
		//如果全部过期需要发
		if(covenant_detail_->covenant_data_map.size() <= over_num) {
			send_active_msg(player_self());
		}
	}
}

int Covenant::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Covenant::move_scene_completed(void) {

	return 0;
}

int Covenant::client_ready_now(void) {
	send_active_msg(player_self());
	return 0;
}

void Covenant::module_init(void) {
	const Covenant_Conf_Map &covenant_conf = CONFIG_CACHE_COVENANT->covenant_conf_map();
	Covenant_Data cd;
	for(Covenant_Conf_Map::const_iterator cit = covenant_conf.begin(); cit != covenant_conf.end(); ++cit) {
	//	uint32_t id = cit->second.id;
		Covenant_Data_Map::iterator cdit = covenant_detail_->covenant_data_map.find(cit->first);
		if(cdit == covenant_detail_->covenant_data_map.end()) {
			cd.reset();
			cd.id = cit->second.id;
			covenant_detail_->covenant_data_map.insert(std::make_pair(cit->first,cd));
			covenant_detail_->detail_change();
		}
	}
	std::set<uint32_t> to_del;
	to_del.clear();
	for(Covenant_Data_Map::iterator cdit = covenant_detail_->covenant_data_map.begin(); cdit != covenant_detail_->covenant_data_map.end();++cdit) {
		if(covenant_conf.find(cdit->first) == covenant_conf.end()) {
			to_del.insert(cdit->first);
		}
	}
	for(std::set<uint32_t>::iterator dit = to_del.begin(); dit != to_del.end(); ++dit) {
		Covenant_Data_Map::iterator cdit = covenant_detail_->covenant_data_map.find(*dit);
		if(cdit != covenant_detail_->covenant_data_map.end()) {
			covenant_detail_->covenant_data_map.erase(cdit);
		}
	}
}

void Covenant::sign_in(void) {

}

void Covenant::sign_out(void) {

}

void Covenant::trusteeship_sign_out(void) {

}

int Covenant::sign_in_repeat(void) {

	return 0;
}

int Covenant::check_covenant_use(Logic_Player *player) {
	if(!player) {
		return ERROR_INNER_SERVER;
	}
//	if(player->level() < 40) {
//		return ERROR_COVENANT_HANDSEL_LEVEL_LIMIT;
//	}
	return 0;
}

bool Covenant::get_covenant_status(uint32_t id) {
	Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.find(id);
	const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(id);
	Time_Value now = Time_Value::gettimeofday();
	if(iter != covenant_detail_->covenant_data_map.end() && conf) {
		if(iter->second.over_time <= now) {
			return false;
		} else if(conf->become_time && iter->second.today_become_time <= now) {
			return false;
		} else {
			return true;
		}
	}
	return false;
}

int Covenant::req_start_covenant_become_time(uint32_t id) {
	{	//判断自己是否能够使用
		int res = check_covenant_use(player_self());
		if(res){
			return res;
		}
	}
	const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(id);
	Time_Value now = Time_Value::gettimeofday();
	if(conf) {
		if(conf->become_time) {//判断是否需要本日计时
			Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.find(id);
			if(iter != covenant_detail_->covenant_data_map.end()) {//如果新加的配置;会进不来
				if(iter->second.over_time < now) { //没开通过;或者过期
					return ERROR_CLIENT_PARAM;
				}
				iter->second.today_become_time.sec(now.sec() + conf->become_time * 60);
				covenant_detail_->detail_change();
				set_status(iter->second);
				//回应客户端
				MSG_50106102 msg;
				msg.reset();
				msg.id = id;
				msg.today_become_time = iter->second.today_become_time.sec();
				THIS_SEND_TO_CLIENT(msg);
			}
		}
	} else { //如果没有;删除
		Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.find(id);
		if(iter != covenant_detail_->covenant_data_map.end()) {
			covenant_detail_->covenant_data_map.erase(iter);
			covenant_detail_->detail_change();
		}
	}
	return 0;
}

void Covenant::open_covenant(uint32_t id, uint32_t days) {
	open_covenant_hours(id, days * 24);
}

void Covenant::open_covenant_by_card(uint32_t id, uint32_t time) {
	const Covenant_Conf_Map &covenant_conf = CONFIG_CACHE_COVENANT->covenant_conf_map();
	MSG_82200223 msg;
	msg.reset();
	if(id == COVEANT_ALL_OPEN_EXP_CARD_ID) {
		msg.exp = 1;
	}
	switch(id) {
		case  COVEANT_ALL_OPEN_EXP_CARD_ID:
		case COVEANT_ALL_OPEN_CARD_ID: { //道具卡ID  开通全部
			Uint_Uint uu;
			for(Covenant_Conf_Map::const_iterator cit = covenant_conf.begin(); cit != covenant_conf.end(); ++cit) {
				uu.reset();
				uu.val_1 = cit->first;
				if(get_covenant_status(cit->first)) {
					uu.val_2 = COVEANT_CARD_OPEN_TYPE_RENEWALS;
				} else {
					uu.val_2 = COVEANT_CARD_OPEN_TYPE_OPEN;
				}
				msg.info.push_back(uu);
				open_covenant_hours(cit->first, time);
			}
			player_self()->create_ann_open_all_covenant();
			break;
		}
		default: { //开通单个
			if(covenant_conf.find(id) == covenant_conf.end()) {
				return;
			}
			Uint_Uint uu;
			uu.reset();
			uu.val_1 = id;
			if(get_covenant_status(id)) {
				uu.val_2 = COVEANT_CARD_OPEN_TYPE_RENEWALS;
			} else {
				uu.val_2 = COVEANT_CARD_OPEN_TYPE_OPEN;
			}
			msg.info.push_back(uu);
			open_covenant_hours(id, time);
			player_self()->create_ann_open_covenant(id + 3000);
		}
	}
	if(msg.info.size()){
		THIS_SEND_TO_CLIENT(msg);
	}
}

void Covenant::open_covenant_hours(uint32_t id, uint32_t hours, uint32_t type) {
	Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.find(id);
	Time_Value now = Time_Value::gettimeofday();
	if(iter != covenant_detail_->covenant_data_map.end()) {
		iter->second.init_status = COVENANT_HAVE_BUY;
		if(iter->second.over_time < now) {
			iter->second.over_time.sec(now.sec() + (hours * 3600)); //新开通或已过期开通的情况
		} else {
			iter->second.over_time.sec(iter->second.over_time.sec() + (hours * 3600)); //未过期开通的情况
		}
		set_status(iter->second);
	} else {
		//特殊情况;可能出现在热更配置
		Covenant_Data cd;
		cd.reset();
		cd.init_status = COVENANT_HAVE_BUY;
		cd.id = id;
		cd.over_time.sec(now.sec() + (hours * 3600));
		std::pair<Covenant_Data_Map::iterator, bool> sult = covenant_detail_->covenant_data_map.insert(std::make_pair(cd.id, cd));
		iter = sult.first;
		set_status(cd);
	}
	covenant_detail_->detail_change();
	send_active_msg(player_self());
}

void Covenant::set_status(Covenant_Data &cd) {
	const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(cd.id);
	//如果没有该配置,或该配置没有配置状态属性
	if(conf && conf->prop_map.size()) {
		//针对特殊的每日倒计时的精灵记录时间
		Time_Value prop_time = cd.over_time;
		if(conf->become_time && cd.over_time.sec() > cd.today_become_time.sec()) {
			prop_time = cd.today_become_time;
		}

		//特殊精灵未开启时候;有可能时间不会生效
		Time_Value now = Time_Value::gettimeofday();
		if(prop_time.sec() <= now.sec()) {
			return;
		}

		//设置状态
		for(Effect_Argv_Prop_Map::const_iterator prop_it = conf->prop_map.begin(); prop_it != conf->prop_map.end(); ++prop_it) {
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
	}
}

void Covenant::get_covenant_status_effect(uint32_t id,  std::vector<Status_Effect_Argv> status) {
	status.clear();
	Covenant_Data_Map::iterator it = covenant_detail_->covenant_data_map.find(id);
	if(it == covenant_detail_->covenant_data_map.end()) {
		return;
	}
	const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(it->second.id);
	//如果没有该配置,或该配置没有配置状态属性
	if(conf && conf->prop_map.size()) {
		//针对特殊的每日倒计时的精灵记录时间
		Time_Value prop_time = it->second.over_time;
		if(conf->become_time && it->second.over_time.sec() > it->second.today_become_time.sec()) {
			prop_time = it->second.today_become_time;
		}

		//特殊精灵未开启时候;有可能时间不会生效
		Time_Value now = Time_Value::gettimeofday();
		if(prop_time.sec() <= now.sec()) {
			return;
		}

		//设置状态
		for(Effect_Argv_Prop_Map::const_iterator prop_it = conf->prop_map.begin(); prop_it != conf->prop_map.end(); ++prop_it) {
			Status_Effect_Argv argv;
			argv.caster_id = role_id();
			argv.status_id = prop_it->first;
			argv.last_time_ms = (int64_t)(prop_time.msec() - now.msec());
			argv.status_from = 1;
			argv.is_fight_buff = false;
			argv.data = prop_it->second;
			status.push_back(argv);
		}
	}
}

//void Covenant::covenant_hook_hero_fight(const int32_t fight_index, const int32_t last_fight_index) {
//	if (fight_index == 0) {
//		return ;
//	}
//	Covenant_Data_Map::iterator it = covenant_detail_->covenant_data_map.find(FIGHT_COVENANT);
//	if(it == covenant_detail_->covenant_data_map.end()) {
//		return;
//	}
//	bool status_has_effect = false;
//	const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(it->second.id);
//	//如果没有该配置,或该配置没有配置状态属性
//	if(conf && conf->prop_map.size()) {
//		//针对特殊的每日倒计时的精灵记录时间
//		Time_Value prop_time = it->second.over_time;
//		if(conf->become_time && it->second.over_time.sec() > it->second.today_become_time.sec()) {
//			prop_time = it->second.today_become_time;
//		}
//
//		//特殊精灵未开启时候;有可能时间不会生效
//		Time_Value now = Time_Value::gettimeofday();
//		if(prop_time.sec() <= now.sec()) {
//			return;
//		}
//
//		std::vector<Status_Effect_Argv> sea_vec;
//		//设置状态
//		for(Effect_Argv_Prop_Map::const_iterator prop_it = conf->prop_map.begin(); prop_it != conf->prop_map.end(); ++prop_it) {
//			Status_Effect_Argv argv;
//			argv.caster_id = role_id();
//			argv.status_id = prop_it->first;
//			argv.last_time_ms = (int64_t)(prop_time.msec() - now.msec());
//			argv.status_from = 1;
//			argv.is_fight_buff = false;
//			argv.data = prop_it->second;
//			sea_vec.push_back(argv);
//		}
//		player_self()->hero_insert_status(sea_vec);
//		player_self()->heroer_prop_refresh(fight_index, false, true);
//		status_has_effect = true;
//		status_has_effect_ = true;
//	}
//	if (!status_has_effect && (status_has_effect_ || (last_fight_index == fight_index && fight_index != 0))) {
//		player_self()->heroer_prop_refresh(fight_index, false, true);
//		status_has_effect_ = false;
//	}
//}

//void Covenant::covenant_hero_fight_prop_refresh(std::map<int, double> &pro_val_map) {
//	Covenant_Data_Map::iterator it = covenant_detail_->covenant_data_map.find(FIGHT_COVENANT);
//	if(it == covenant_detail_->covenant_data_map.end()) {
//		return;
//	}
//	const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(it->second.id);
//	//如果没有该配置,或该配置没有配置状态属性
//	if(conf && conf->prop_map.size()) {
//		//针对特殊的每日倒计时的精灵记录时间
//		Time_Value prop_time = it->second.over_time;
//		if(conf->become_time && it->second.over_time.sec() > it->second.today_become_time.sec()) {
//			prop_time = it->second.today_become_time;
//		}
//
//		//特殊精灵未开启时候;有可能时间不会生效
//		Time_Value now = Time_Value::gettimeofday();
//		if(prop_time.sec() <= now.sec()) {
//			return;
//		}
//
//		const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
//		//设置状态
//		for(Effect_Argv_Prop_Map::const_iterator prop_it = conf->prop_map.begin(); prop_it != conf->prop_map.end(); ++prop_it) {
//			for (Effect_Argv_Prop_Vec::const_iterator it_data = prop_it->second.begin();
//					it_data != prop_it->second.end(); ++it_data) {
//				const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(prop_it->first);
//				if (bbc && sc) {
//					if (!bbc->buff_scene.empty()) {
//						bool need_continue = true;
//						for (Int_Vec::const_iterator it_bs = bbc->buff_scene.begin(); it_bs != bbc->buff_scene.end(); ++it_bs) {
//							if (*it_bs == sc->type) {
//								need_continue = false;
//							}
//						}
//						if (need_continue) {
//							continue;
//						}
//					}
//				}
//				pro_val_map[it_data->i_effect_val1] += it_data->d_effect_val2;
//			}
//		}
//	}
//}

int Covenant::req_buy_covenant(uint8_t type, uint32_t id) {
	{	//判断自己是否能够使用
		int res = check_covenant_use(player_self());
		if(res){
			return res;
		}
	}
	MSG_50106100 msg;
	msg.reset();
	switch(type) {
		case COVENANT_BUY_ONE: {
			const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(id);
			if(conf) {
				//是否有足够的金钱
				int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), conf->price, MONEY_SUB_MONEY_BUY_COVENANT));
				if(return_status != 0) {
					return return_status;
				}
				//减掉相应的钱
				if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), conf->price, MONEY_SUB_MONEY_BUY_COVENANT)) == 0) {
					open_covenant(conf->id, conf->days);
					msg.id.push_back(conf->id);
					player_self()->create_ann_open_covenant(conf->id + 3000);
				}
			} else {
				return ERROR_CLIENT_OPERATE;
			}
			break;
		}
		case COVENANT_BUY_ALL_NOT_OPEN: { //开通全部
			const Covenant_Conf_Map &conf_map = CONFIG_CACHE_COVENANT->covenant_conf_map();
			uint32_t price = 0;
			UInt_UInt_Map will_open_id_days;
			will_open_id_days.clear();
			Time_Value now = Time_Value::gettimeofday();
			for(Covenant_Conf_Map::const_iterator conf_iter = conf_map.begin(); conf_iter != conf_map.end(); ++conf_iter) {
				Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.find(conf_iter->second.id);
				if(iter != covenant_detail_->covenant_data_map.end()) {
					if(iter->second.over_time < now) {
						price += conf_iter->second.price;
						will_open_id_days.insert(std::make_pair(conf_iter->second.id, conf_iter->second.days));
					}
				} else {
					price += conf_iter->second.price;
					will_open_id_days.insert(std::make_pair(conf_iter->second.id, conf_iter->second.days));
				}
			}
			//打折
			int mod = (price * 7) % 10;
			price = ((int32_t)((price * 7) / 10) + (mod?1:0));
			//是否有足够的金钱
			int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MONEY_BUY_COVENANT));
			if(return_status != 0) {
				return return_status;
			}
			//减掉相应的钱
			if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MONEY_BUY_COVENANT)) == 0) {
				for(UInt_UInt_Map::iterator iter = will_open_id_days.begin(); iter != will_open_id_days.end(); ++iter) {
					open_covenant(iter->first, iter->second);
					msg.id.push_back(iter->first);
				}
				player_self()->create_ann_open_all_covenant();
			}
			break;
		}
		case COVENANT_BUY_ALL_OPEN: { //续费全部
			uint32_t price = 0;
			UInt_UInt_Map will_open_id_days;
			{
				will_open_id_days.clear();
				Time_Value now = Time_Value::gettimeofday();
				for(Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.begin(); iter != covenant_detail_->covenant_data_map.end(); ++iter) {
					if(iter->second.over_time >= now) {
						const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(iter->first);
						if(conf) {
							price += conf->price;
							will_open_id_days.insert(std::make_pair(conf->id, conf->days));
						}
					}
				}
			}
			//打折
			int mod = (price * 7) % 10;
			price = ((int32_t)((price * 7) / 10) + (mod?1:0));
			//是否有足够的金钱
			int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MONEY_BUY_COVENANT));
			if(return_status != 0) {
				return return_status;
			}
			//减掉相应的钱
			if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MONEY_BUY_COVENANT)) == 0) {
				for(UInt_UInt_Map::iterator iter = will_open_id_days.begin(); iter != will_open_id_days.end(); ++iter) {
					open_covenant(iter->first, iter->second);
					msg.id.push_back(iter->first);
				}
				player_self()->create_ann_open_all_covenant();
			}
			break;
		}
		default:{
			return ERROR_CLIENT_OPERATE;
		}
	}

	THIS_SEND_TO_CLIENT(msg);
	send_active_msg(player_self());
	return 0;
}

int Covenant::req_handsel_covenant(uint8_t type, uint32_t id, std::string &role_name) {
	{	//判断自己是否能够使用
		int res = check_covenant_use(player_self());
		if(res){
			return res;
		}
	}
	role_id_t role_id = LOGIC_MONITOR->get_role_id_by_name(role_name);
	if(!role_id) {
		return ERROR_COVENANT_ROLE_NOT_EXIST;
	}
	if(player_self()->is_friend(role_id) != 0) {
		return ERROR_FRIEND_NOTEXIST;
	}
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(!player) { //对方不在线
		return ERROR_PLAYER_NOT_ONLINE;
	}
	{	//判断对方盟约是否能够使用
		int res = check_covenant_use(player);
		if(res){
			return res;
		}
	}
	//通知客户端
	MSG_50106101 msg;
	msg.reset();
	//记录所有赠送的盟约名称; 用于发送邮件
	std::string covenant_name;
	covenant_name.clear();
	uint32_t mail_id = 0;

	switch(type) {
		case COVENANT_HANDSEL_ONE: {
			const Covenant_Conf *conf = CONFIG_CACHE_COVENANT->find_covenant_conf_by_id(id);
			if(conf) {
				//是否有足够的金钱
				int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), conf->price, MONEY_SUB_MONEY_BUY_COVENANT));
				if(return_status != 0) {
					return return_status;
				}
				//减掉相应的钱
				if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), conf->price, MONEY_SUB_MONEY_BUY_COVENANT)) == 0) {
					player->open_covenant(conf->id, conf->days);
					msg.id.push_back(conf->id);
					covenant_name = conf->name;
					player_self()->create_ann_handsel_covenant(player, conf->id + 4000);
				}
				mail_id =  COVENANT_SEND_HANDSEL_ONE_MAIL_ID; //单一赠送邮件ID
			} else {
				return ERROR_CLIENT_OPERATE;
			}
			break;
		}
		case COVENANT_HANDSEL_ALL: { //赠送全部
			const Covenant_Conf_Map &conf_map = CONFIG_CACHE_COVENANT->covenant_conf_map();
			uint32_t price = 0;
			UInt_UInt_Map will_open_id_days;
			will_open_id_days.clear();
			//记录价钱和所有id
			for(Covenant_Conf_Map::const_iterator conf_iter = conf_map.begin(); conf_iter != conf_map.end(); ++conf_iter) {
				price += conf_iter->second.price;
				will_open_id_days.insert(std::make_pair(conf_iter->second.id, conf_iter->second.days));
			}
			//打折
			int mod = (price * 7) % 10;
			price = ((int32_t)((price * 7) / 10) + (mod?1:0));
			//是否有足够的金钱
			int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MONEY_BUY_COVENANT));
			if(return_status != 0) {
				return return_status;
			}
			//减掉相应的钱
			if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), price, MONEY_SUB_MONEY_BUY_COVENANT)) == 0) {
				for(UInt_UInt_Map::iterator iter = will_open_id_days.begin(); iter != will_open_id_days.end(); ++iter) {
					player->open_covenant(iter->first, iter->second);
					msg.id.push_back(iter->first);
				}
				player_self()->create_ann_handsel_all_covenant(player);
			}
			mail_id =  COVENANT_SEND_HANDSEL_ALL_MAIL_ID; //赠送全部邮件ID
			break;
		}
		default:{
			return ERROR_CLIENT_OPERATE;
		}
	}
	//通知自己
	THIS_SEND_TO_CLIENT(msg);
	//通知对方客户端
	{
		MSG_82200221 o_msg;
		o_msg.role_name = player_self()->role_name();
		o_msg.id = msg.id;
		OBJ_SEND_TO_CLIENT(o_msg, (*player));
	}
	send_active_msg(player);
	/********************发送邮件***************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
	if(mailc) {
		std::string contents;
		switch(type) {
			case COVENANT_HANDSEL_ONE: { //单一赠送
				const int size_buf = mailc->mail_content.size() + \
						sizeof(player_self()->role_name()) + 10 + \
						sizeof(covenant_name) + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->mail_content.c_str(), player_self()->role_name().c_str(), covenant_name.c_str());
				contents.assign(content_buf);
				break;
			}
			case COVENANT_HANDSEL_ALL: { //赠送全部
				const int size_buf = mailc->mail_content.size() + \
						sizeof(player_self()->role_name()) + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->mail_content.c_str(), player_self()->role_name().c_str());
				contents.assign(content_buf);
				break;
			}
		}
		LOGIC_PUBLIC->send_mail(mail_id, role_id, "", contents);
	}
	/************************************************/
	return 0;
}

void Covenant::send_active_msg(Logic_Player *player) {
	if(player) {
		MSG_82200220 msg;
		msg.reset();
		Covenant_Status cs;
		for(Covenant_Data_Map::const_iterator cdit = player->covenant_detail().covenant_data_map.begin();
			cdit != player->covenant_detail().covenant_data_map.end(); ++cdit) {
			cs.reset();
			cs.id = cdit->second.id;
			cs.init_status = cdit->second.init_status;
			cs.over_time = cdit->second.over_time.sec();
			cs.today_become_time = cdit->second.today_become_time.sec();
			msg.covenant.push_back(cs);
		}
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
}

void Covenant::send_expired_mail(uint32_t mail_id) {
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
	if(mailc) {
		std::string contents;
		const int size_buf = mailc->mail_content.size() + \
				sizeof(player_self()->role_name()) + 10;
		char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), player_self()->role_name().c_str());
		contents.assign(content_buf);
		LOGIC_PUBLIC->send_mail(mail_id, player_self()->role_id(), "", contents);
	}
}


void Covenant::test_covenant(uint32_t id, int time, int type) {
	Covenant_Data_Map::iterator iter = covenant_detail_->covenant_data_map.find(id);
	if(iter != covenant_detail_->covenant_data_map.end()) {
		if(type) {
			Time_Value now = Time_Value::gettimeofday();
			iter->second.over_time.sec(now.sec() + time * 60);
		} else {
			iter->second.over_time.sec((iter->second.over_time.sec() + time * 60));
		}
		set_status(iter->second);
		covenant_detail_->detail_change();
		send_active_msg(player_self());
	}
}

void Covenant::covenant_expired_listen(const uint32_t id, const Time_Value &over_time, const Time_Value &now) {
	//精灵盟约过期监听 id=盟约ID
	player_self()->player_try_time_out(TRY_GENIUS_TIMEOUT, over_time);
}
