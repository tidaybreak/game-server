/*
 * Logic_Public.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Logic_Monitor.h"
#include "Pool_Manager.h"
#include "hot_update/Hot_Update.h"
#include "Record_Client.h"
#include "DB_Manager.h"
#include "mail/Mail_Struct.h"
#include "pack/Pack_Struct.h"
#include "friend/Friend_Struct.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "Config_Cache.h"
#include "Logic_Player_Struct.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Mail.h"
#include "Logic_Client_Request.h"
#include "DB_Def.h"
#include "Config_Cache_Item.h"


Logic_Public::Logic_Public() {

}

Logic_Public::~Logic_Public() {
	// TODO Auto-generated destructor stub
}

int Logic_Public::init(void) {
	reboot_time_ = Time_Value::zero;
	access_count.clear();
	access_count_record_time = Time_Value::gettimeofday();
	return 0;
}

int Logic_Public::start(void) {

	return 0;
}

int Logic_Public::rand_role_name(void) {
	const String_Vec &fm_vec = CONFIG_CACHE_ROLE->get_family_name_vec();
	const String_Vec &m_vec = CONFIG_CACHE_ROLE->get_male_name_vec();
	const String_Vec &f_vec = CONFIG_CACHE_ROLE->get_female_name_vec();

	int loop = 500;
	while (male_name_set.size() < 300) {
		int fm_rand = random()%fm_vec.size();
		String_Vec::const_iterator fm_it = fm_vec.begin();
		std::advance(fm_it, fm_rand);

		int m_rand = random()%m_vec.size();
		String_Vec::const_iterator m_it = m_vec.begin();
		std::advance(m_it, m_rand);

		std::string name;
		if (CONFIG_CACHE->server_maintainer_cache().game_ver_type == 1) {
			name = (*m_it) + (*fm_it);
		} else {
			name = (*fm_it) + (*m_it);
		}
		DB_View::Name_Player_View_Map::iterator n_find_it = LOGIC_CLIENT_REQUEST->monitor()->db_view()->name_player_view_map.find(name);
		if (n_find_it == LOGIC_CLIENT_REQUEST->monitor()->db_view()->name_player_view_map.end()) {
			male_name_set.insert(name);
		}

		if (--loop < 0) {
			MSG_USER("name no enouth!!!");
			break;
		}
	}

	loop = 500;
	while (female_name_set.size() < 300) {
		int fm_rand = random()%fm_vec.size();
		String_Vec::const_iterator fm_it = fm_vec.begin();
		std::advance(fm_it, fm_rand);

		int f_rand = random()%f_vec.size();
		String_Vec::const_iterator f_it = f_vec.begin();
		std::advance(f_it, f_rand);

		std::string name;
		if (CONFIG_CACHE->server_maintainer_cache().game_ver_type == 1) {
			name = (*f_it) + (*fm_it);
		} else {
			name = (*fm_it) + (*f_it);
		}
		DB_View::Name_Player_View_Map::iterator n_find_it = LOGIC_CLIENT_REQUEST->monitor()->db_view()->name_player_view_map.find(name);
		if (n_find_it == LOGIC_CLIENT_REQUEST->monitor()->db_view()->name_player_view_map.end()) {
			female_name_set.insert(name);
		}

		if (--loop < 0) {
			MSG_USER("name no enouth!!!");
			break;
		}
	}

	return 0;
}

Time_Value &Logic_Public::reboot_time(void) {
	return reboot_time_;
}

void Logic_Public::set_reboot_time(Time_Value t) {
	reboot_time_ = t;
}

void Logic_Public::tick(const Time_Value &now) {
	save_access_count(now);
}

bool Logic_Public::has_role_by_id(role_id_t role_id) {
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view", role_id);
		return false;
	}
	return true;
}


int Logic_Public::send_mail(Role_Vec &role_vec, Mail_Send_Info &send_info) {
	//群发邮件 send_info至少包含(sender_id,发送人  send_type,邮件类型  title,标题  content,内容)
	if (send_info.title.size() > MAIL_TITLE_MAX_LEN || send_info.content.size() > MAIL_CONTENT_MAX_LEN) {
		MSG_USER("title:%s, content:%s", send_info.title.c_str(), send_info.content.c_str());
		return ERROR_CONTENT_TITLE;
	}
	if (send_info.sender_id) {
		send_info.sender_name = LOGIC_MONITOR->get_role_name_by_id(send_info.sender_id);
	}
	for(Role_Vec::iterator iter = role_vec.begin(); iter != role_vec.end(); ++iter) {
		send_info.show_send_time = Time_Value::gettimeofday();
		send_info.receiver_id = *iter;
		send_mail(send_info);
	}
	return 0;
}

int Logic_Public::send_mail(int id, Role_Vec &role_vec) {
	for(Role_Vec::iterator iter = role_vec.begin(); iter != role_vec.end(); ++iter) {
		send_mail(id, *iter);
	}
	return 0;
}

int Logic_Public::send_mail(const role_id_t role_id, const std::string &sender_name, const std::string &title, const std::string &contents, const Item_Vec &item_vector) {
	Mail_Send_Info send_info;
	send_info.sender_id = 0;
	send_info.receiver_id = role_id;
	send_info.send_type = MAIL_TYPE_SYS;
	send_info.show_send_time = Time_Value::gettimeofday();
	send_info.sender_name = sender_name;
	send_info.title = title;
	send_info.content = contents;

	for(Item_Vec::const_iterator item_iter  = item_vector.begin(); item_iter != item_vector.end(); item_iter++) {
		const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(item_iter->id_);
		if (!idc) {
			if(MAIL_ITEM_MAX_NUM < item_iter->amount_) {
				MSG_TRACE_ABORT("Mail Item amount ERROR");
				return ERROR_PERM_LIMIT;
			}
		} else if(MAIL_ITEM_MAX_NUM < item_iter->amount_ / idc->overlay) {
			MSG_TRACE_ABORT("Mail Item amount ERROR");
			return ERROR_PERM_LIMIT;
		}
		send_info.item_vector.push_back(*item_iter);
	}
	return send_mail(send_info);

//	bool has_send = true;
//	for(Item_Vec::const_iterator item_iter  = item_vector.begin(); item_iter != item_vector.end(); item_iter++) {
//		send_info.item_vector.push_back(*item_iter);
//		has_send = true;
//		if(send_info.item_vector.size() >= 4) {
//			send_mail(send_info);
//			has_send = false;
//			send_info.item_vector.clear();
//		}
//	}
//
//	if(has_send) {
//		send_mail(send_info);
//	}
	return 0;
}

int Logic_Public::send_mail(Mail_Send_Info &send_info) {
	if (send_info.title.size() > MAIL_TITLE_MAX_LEN || send_info.content.size() > MAIL_CONTENT_MAX_LEN) {
		MSG_USER("title:%s, content:%s", send_info.title.c_str(), send_info.content.c_str());
		return ERROR_CONTENT_TITLE;
	}

	if (send_info.sender_id) {
		send_info.sender_name = LOGIC_MONITOR->get_role_name_by_id(send_info.sender_id);
	}

	if ( send_info.receiver_id <= 0 || send_info.send_type < 0 || send_info.sender_id < 0 || send_info.sender_name.empty()) {
		std::stringstream log_str;
		log_str << "send_mail fail," << ",receiver_id=" << send_info.receiver_id
					<< ",mail_type=" << send_info.send_type << ",sender_id=" << send_info.sender_id
					<< ",sender_name=" << send_info.sender_name << ",mail_title=" << send_info.title
					<< ",item=[";

		MSG_USER("%s", log_str.str().c_str());
		return ERROR_CLIENT_PARAM;
	}

	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(send_info.receiver_id);
	if (receiver) {
		receiver->send_mail_to_self(send_info);
	} else {
		Logic_Player::offline_mail(send_info);
	}

	return 0;
}

int Logic_Public::send_mail(int id, role_id_t role_id, const std::string &title, const std::string &contents) {

	const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
	Mail_Config_Map::const_iterator iter = mail_config.find(id);
	if(iter == mail_config.end()) {
		return -1;
	}

	return send_mail(id, role_id, iter->second.item_vector, title, contents);
}

int Logic_Public::send_mail(int id, role_id_t role_id, const Item_Vec &item_vector, const std::string &title, const std::string &contents) {
	std::vector<Item_Basic_Info> item_base_vector;
	item_base_vector.clear();
	Item_Basic_Info base_info;
	for(Item_Vec::const_iterator iter = item_vector.begin(); iter != item_vector.end(); ++iter) {
		const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(iter->id_);
		if (!idc) {
			if(MAIL_ITEM_MAX_NUM < iter->amount_) {
				MSG_TRACE_ABORT("Mail Item amount ERROR");
				return ERROR_PERM_LIMIT;
			}
		} else if(MAIL_ITEM_MAX_NUM < iter->amount_ / idc->overlay) {
			MSG_TRACE_ABORT("Mail Item amount ERROR");
			return ERROR_PERM_LIMIT;
		}
		base_info.reset();
		iter->set_item_basic_info(base_info);
		item_base_vector.push_back(base_info);
	}
	return send_mail(id, role_id, item_base_vector, title, contents);
}

int Logic_Public::send_mail(int id, role_id_t role_id, const std::vector<Item_Basic_Info> &item_vector, const std::string &title, const std::string &contents) {

	const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
	Mail_Config_Map::const_iterator iter = mail_config.find(id);
	if(iter == mail_config.end()) {
		return -1;
	}

	Mail_Send_Info send_info;
	send_info.sender_id = 0;
	send_info.receiver_id = role_id;
	send_info.send_type = MAIL_TYPE_SYS;
	send_info.show_send_time = Time_Value::gettimeofday();

	send_info.sender_name = iter->second.sender_name;
	send_info.title = (title.empty())?iter->second.mail_title:title;
	send_info.content = (contents.empty())?iter->second.mail_content:contents;
	for(std::vector<Item_Basic_Info>::const_iterator item_iter  = item_vector.begin(); item_iter != item_vector.end(); item_iter++) {
		const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(item_iter->id);
		if (!idc) {
			if(MAIL_ITEM_MAX_NUM < item_iter->amount) {
				MSG_TRACE_ABORT("Mail Item amount ERROR");
				return ERROR_PERM_LIMIT;
			}
		} else if(MAIL_ITEM_MAX_NUM < item_iter->amount / idc->overlay) {
			MSG_TRACE_ABORT("Mail Item amount ERROR");
			return ERROR_PERM_LIMIT;
		}
		send_info.item_vector.push_back(Item_Detail(item_iter->id, item_iter->amount, item_iter->bind));
	}

	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
//	bool has_send = true;
//	for(std::vector<Item_Basic_Info>::const_iterator item_iter  = item_vector.begin(); item_iter != item_vector.end(); item_iter++) {
//		send_info.item_vector.push_back(Item_Detail(item_iter->id, item_iter->amount, item_iter->bind));
//		has_send = true;
//		if(send_info.item_vector.size() >= 4) {
//			if (receiver) {
//				receiver->send_mail_to_self(send_info);
//			} else {
//				Logic_Player::offline_mail(send_info);
//			}
//			has_send = false;
//			send_info.item_vector.clear();
//		}
//	}

//	if(has_send) {
		if (receiver) {
			return receiver->send_mail_to_self(send_info);
		} else {
			return Logic_Player::offline_mail(send_info);
		}
//	}

	return 0;
}

int Logic_Public::send_mail(int id, role_id_t role_id, const uint32_t gold, const std::vector<Item_Basic_Info> &item_vector, const std::string &title, const std::string &contents) {
	const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
	Mail_Config_Map::const_iterator iter = mail_config.find(id);
	if(iter == mail_config.end()) {
		return -1;
	}

	Mail_Send_Info send_info;
	send_info.sender_id = 0;
	send_info.receiver_id = role_id;
	send_info.send_type = MAIL_TYPE_SYS;
	send_info.show_send_time = Time_Value::gettimeofday();
	send_info.gold = gold;

	send_info.sender_name = iter->second.sender_name;
	send_info.title = (title.empty())?iter->second.mail_title:title;
	send_info.content = (contents.empty())?iter->second.mail_content:contents;
	for(std::vector<Item_Basic_Info>::const_iterator item_iter  = item_vector.begin(); item_iter != item_vector.end(); item_iter++) {
		const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(item_iter->id);
		if (!idc) {
			if(MAIL_ITEM_MAX_NUM < item_iter->amount) {
				MSG_TRACE_ABORT("Mail Item amount ERROR");
				return ERROR_PERM_LIMIT;
			}
		} else if(MAIL_ITEM_MAX_NUM < item_iter->amount / idc->overlay) {
			MSG_TRACE_ABORT("Mail Item amount ERROR");
			return ERROR_PERM_LIMIT;
		}
		send_info.item_vector.push_back(Item_Detail(item_iter->id, item_iter->amount, item_iter->bind));
	}

	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (receiver) {
		receiver->send_mail_to_self(send_info);
	} else {
		Logic_Player::offline_mail(send_info);
	}

	return 0;
}

int Logic_Public::send_mail(int id, role_id_t role_id, const uint32_t gold, const std::string &title, const std::string &contents) {
	const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
	Mail_Config_Map::const_iterator iter = mail_config.find(id);
	if(iter == mail_config.end()) {
		return -1;
	}

	Mail_Send_Info send_info;
	send_info.sender_id = 0;
	send_info.receiver_id = role_id;
	send_info.send_type = MAIL_TYPE_SYS;
	send_info.show_send_time = Time_Value::gettimeofday();
	send_info.gold = gold;

	send_info.sender_name = iter->second.sender_name;
	send_info.title = (title.empty())?iter->second.mail_title:title;
	send_info.content = (contents.empty())?iter->second.mail_content:contents;

	send_info.item_vector.clear();

	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (receiver) {
		receiver->send_mail_to_self(send_info);
	} else {
		Logic_Player::offline_mail(send_info);
	}
	return 0;
}

int Logic_Public::recharge_grant(role_id_t role_id, int pay_num) {
	if (pay_num <= 0) {
		MSG_USER("info:%s, pay_num:%d, role_id:%ld", "充值数量错误", pay_num, role_id);
		return ERROR_ADD_MONEY_TO_LARGE;
	}
	Player_DB_View *p = LOGIC_MONITOR->find_player_db_view_by_role_id(role_id);
	if(!p) {
		return ERROR_PLAYER_NOT_EXIST; //角色ID不存在
	}
	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (receiver) {
		int res = receiver->pack_add_money(Money_Add_Info(GOLD, pay_num, Money_DM_Info(MONEY_ADD_RECHARGE,0,0,Time_Value::gettimeofday().sec())));
		if(!res) {
			receiver->recharge_listen(pay_num);
			MSG_USER("info:%s, pay_num:%d, role_id:%ld", "玩家充值(在线) 插入金钱到背包成功", pay_num, role_id);
		} else {
			MSG_USER("info:%s, pay_num:%d, role_id:%ld", "玩家充值(在线) 插入金钱到背包失败", pay_num, role_id);
		}
		return res;
	} else {
		MSG_USER("info:%s, pay_num:%d, role_id:%ld", "玩家充值(不在线)", pay_num, role_id);
		return 1;
	}

	return 0;
}

int Logic_Public::inner_recharge_grant(role_id_t role_id, int pay_num) {
	if (pay_num <= 0) {
		MSG_USER("info:%s, pay_num:%d, role_id:%ld", "充值数量错误", pay_num, role_id);
		return ERROR_ADD_MONEY_TO_LARGE;
	}
	Player_DB_View *p = LOGIC_MONITOR->find_player_db_view_by_role_id(role_id);
	if(!p) {
		return ERROR_PLAYER_NOT_EXIST; //角色ID不存在
	}
	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (receiver) {
		int res = receiver->pack_add_money(Money_Add_Info(GOLD, pay_num, Money_DM_Info(MONEY_ADD_INNER_RECHARGE,0,0,Time_Value::gettimeofday().sec())));
		if(!res) {
			receiver->recharge_listen(pay_num);
			MSG_USER("info:%s, pay_num:%d, role_id:%ld", "玩家充值(在线) 插入金钱到背包成功", pay_num, role_id);
		} else {
			MSG_USER("info:%s, pay_num:%d, role_id:%ld", "玩家充值(在线) 插入金钱到背包失败", pay_num, role_id);
		}
		return res;
	} else {
		MSG_USER("info:%s, pay_num:%d, role_id:%ld", "玩家充值(不在线)", pay_num, role_id);
		return 1;
	}

	return 0;
}

std::string Logic_Public::get_name(int gender) {
	String_Set &set = (gender == 1) ? male_name_set : female_name_set;
	int size = set.size();
	if (size < 200) {
		rand_role_name();
	}

	size = set.size();
	if (size <= 0) {
		Time_Value now = Time_Value::gettimeofday();
		int num = now.sec() % 1000000;
		std::ostringstream ost;
		ost << num;
		return ost.str();
	}
	size = (size > 10000) ? 10000 : size;
	int rand = random() % size;
	String_Set::iterator it = set.begin();
	std::advance(it, rand);
	return *it;
}

void Logic_Public::erase_name(int gender, const std::string &name) {
	String_Set &set = (gender == 1) ? male_name_set : female_name_set;
	set.erase(name);
}

int Logic_Public::record_access_count(uint32_t type) {
	UInt_UInt_Map::iterator ait = access_count.find(type);
	if(ait != access_count.end()) {
		++ait->second;
	} else {
		access_count.insert(std::make_pair(type, 1));
	}
	return 0;
}

void Logic_Public::save_access_count(const Time_Value &now) {
	if(now.sec() - access_count_record_time.sec() > Time_Value::ONE_MINUTE_IN_SECS) {
		if(access_count.size() == 0) {
			access_count_record_time = now;
		}
		Block_Buffer buf;
		buf.make_message(MONGO_SACE_ACCESS_DATA_COUNT);
		buf.write_uint8(access_count.size());
		for(UInt_UInt_Map::iterator ait = access_count.begin(); ait != access_count.end(); ++ait) {
			buf.write_uint32(ait->first);
			buf.write_uint32(ait->second);
		}
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
		access_count.clear();
	}
}


int Logic_Public::reply_proposal(role_id_t role_id, std::string &sender, std::string &title, std::string &contents, int32_t gold) {
	Mail_Send_Info send_info;
	send_info.reset();
	send_info.gold = gold;
	send_info.content = contents;
	send_info.sender_name = sender;
	send_info.title = title;
	send_info.receiver_id = role_id; //收件人
	send_info.show_send_time = Time_Value::gettimeofday();
	return send_mail(send_info);
}

int Logic_Public::send_facebook_share_mail(role_id_t role_id, uint8_t type) {
	MSG_DEBUG("send_facebook_share: role_id:%ld type:%d",role_id, type);
	switch(type) {
		case 1: {//分享
			DB_View::ID_Player_View_Map::iterator find_it =
					LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
			if(find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
				send_mail(2002, role_id);
				MSG_STREAM("`stream_fb_click`(`player_id`, `level`, `ip`, `type`, `time`)"" VALUES (%ld,%d,'%s',%d, %ld)",
						role_id,
						find_it->second->level,
						find_it->second->ip.c_str(),
						8,//8是成功领取share的奖励
						Time_Value::gettimeofday().sec());
			}
			break;
		}
		case 2: {//点赞
			Logic_Player *invite_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
			if(invite_player) {//online
				invite_player->get_like_reward();
			} else { //not online
				Logic_Player::offline_like_success(role_id);
			}
			break;
		}
		default: {
			MSG_DEBUG("send_facebook_share_mail error role_id:%ld, type:%d", role_id, type);
		}
	}
	return 0;
}
