/*
 * Mail.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: zhouyajun
 */
#include "Logic_Module_Head.h"
#include "Mail_Struct.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Mail.h"
#include "Msg_Mail_Struct.h"
#include "Mail_Manager.h"
#include "Config_Cache_Mail.h"
#include "Config_Cache_Item.h"
#include "Config_Cache.h"

Mail::Mail(void) {
	reset();
}

Mail::~Mail() {
	// TODO Auto-generated destructor stub
}

void Mail::reset(void) {
	mail_detail_ = 0;
}

int Mail::create_init(Mail_Detail &detail) {
	detail.last_read_sys_mail_time = Time_Value::gettimeofday();
	detail.detail_change();
	return 0;
}

void Mail::load_detail(Mail_Detail &detail) {
	mail_detail_ = &detail;
}

void Mail::save_detail(Mail_Detail &detail) {
	if (mail_detail().is_change) {
		detail = mail_detail();
		mail_detail_->is_change = false;
	}
}

// 每日0点刷新
int Mail::trigger_daily_zero(const Time_Value &now) {
	mail_detail_->max_gold = 0;
	mail_detail_->detail_change();
	return 0;
}

void Mail::module_init(void) {
	const int save_day = CONFIG_CACHE_MAIL->find_common_config("save_day");
	const int sys_save_day = CONFIG_CACHE_MAIL->find_common_config("sys_save_day");
	const int php_save_day = CONFIG_CACHE_MAIL->find_common_config("php_save_day");
	//user mail
	Mail_Detail::Mail_Map::iterator select_iter = mail_detail_->mail_map.begin();
	for( ;select_iter != mail_detail_->mail_map.end(); ++select_iter){
		if(select_iter->second.show_send_time.sec() + save_day*24*3600 <= Time_Value::gettimeofday().sec()) {
			mail_detail_->mail_map.erase(select_iter, mail_detail_->mail_map.end());
			mail_detail_->detail_change();
			break;
		}
	}
	//sys mail
	select_iter = mail_detail_->sys_mail_map.begin();
	for( ;select_iter != mail_detail_->sys_mail_map.end(); select_iter++){
		int day = sys_save_day;
		if(select_iter->second.mail_type == MAIL_TYPE_PHP) {
			day = php_save_day;
		}
		if(select_iter->second.show_send_time.sec() + day*24*3600 <= Time_Value::gettimeofday().sec()) {
			MSG_DEBUG("time over del mail, mail_title:%s, mail_contents:%s, role_id:%ld", select_iter->second.mail_title.c_str(), select_iter->second.mail_content.c_str(), player_self()->role_id());
			mail_detail_->sys_mail_map.erase(select_iter);
			mail_detail_->detail_change();
			break;
		}
	}
	return;
}

void Mail::sign_in(void) {
	for(Sys_Mail_Vec::const_iterator iter = MAIL_MANAGER->get_sys_mail_vec().begin(); iter != MAIL_MANAGER->get_sys_mail_vec().end(); iter++) {
		if(iter->show_send_time.sec() > mail_detail_->last_read_sys_mail_time.sec()) {
			Mail_Send_Info mail_info ;
			mail_info.send_type = iter->mail_type;
			mail_info.sender_name = iter->sender_name;
			mail_info.title = iter->mail_title;
			mail_info.content = iter->mail_content;
			mail_info.show_send_time = iter->show_send_time;
			mail_info.item_vector = iter->item_vector;
			this->send_mail_to_self(mail_info);
		}
	}
	mail_detail_->last_read_sys_mail_time = Time_Value::gettimeofday();
    return;
}

void Mail::sign_out(void) {
	return;
}

void Mail::client_ready_now(void) {
	//通知客户端有新邮件;
	MSG_82200000 msg_s;
	msg_s.mail_count = mail_detail_->mail_map.size();
	msg_s.not_read_count = get_not_read_count();
	THIS_SEND_TO_CLIENT(msg_s);
}
/*
void Mail::insert_mail(int mail_id, const Mail_Data &mail_info_detail){
	mail_detail_->mail_map.insert(std::make_pair(mail_id, mail_info_detail));
	mail_detail_->mail_all_count++;
}
*/
int Mail::fetch_mail_state_list(const int &page_count, const int &mail_state){
	mail_detail_->mail_state_page = mail_state;
	if(mail_state == ALL_MAIL) {
		int8_t have_one_key_pickup = 0;
		Mail_Detail::Mail_Map all_mail;
		all_mail.clear();
		Mail_Detail::Mail_Map::iterator select_iter = mail_detail_->mail_map.begin();
		for( ;select_iter != mail_detail_->mail_map.end(); select_iter++){
			if(select_iter->second.gold || select_iter->second.item_vector.size()) {
				have_one_key_pickup = 1;
			}
			if(select_iter->second.have_read){
				all_mail.insert(std::make_pair(select_iter->first,select_iter->second));
			} else {
				all_mail.insert(std::make_pair(select_iter->first + pow(2,32),select_iter->second));
			}
		}
		select_iter = mail_detail_->sys_mail_map.begin();
		for( ;select_iter != mail_detail_->sys_mail_map.end(); select_iter++){
			if(select_iter->second.gold || select_iter->second.item_vector.size()) {
				have_one_key_pickup = 1;
			}
			if(select_iter->second.have_read){
				all_mail.insert(std::make_pair(select_iter->first,select_iter->second));
			} else {
				all_mail.insert(std::make_pair(select_iter->first + pow(2,32),select_iter->second));
			}
		}
		return fetch_mail_list(page_count, all_mail, have_one_key_pickup);
	} else if (mail_state == HAVE_READ) {
		int8_t have_one_key_pickup = 0;
		Mail_Detail::Mail_Map have_read_list;
		have_read_list.clear();
		Mail_Detail::Mail_Map::iterator select_iter = mail_detail_->mail_map.begin();
		for( ;select_iter != mail_detail_->mail_map.end(); select_iter++){
			if(select_iter->second.gold || select_iter->second.item_vector.size()) {
				have_one_key_pickup = 1;
			}
			if(select_iter->second.have_read == true){
				have_read_list.insert(std::make_pair(select_iter->first,select_iter->second));
			}
		}
		select_iter = mail_detail_->sys_mail_map.begin();
		for( ;select_iter != mail_detail_->sys_mail_map.end(); select_iter++){
			if(select_iter->second.gold || select_iter->second.item_vector.size()) {
				have_one_key_pickup = 1;
			}
			if(select_iter->second.have_read == true){
				have_read_list.insert(std::make_pair(select_iter->first,select_iter->second));
			}
		}
		return fetch_mail_list(page_count, have_read_list, have_one_key_pickup);
	} else if (mail_state == HAVE_NOT_READ) {
		int8_t have_one_key_pickup = 0;
		Mail_Detail::Mail_Map have_not_read_list;
		have_not_read_list.clear();
		Mail_Detail::Mail_Map::iterator select_iter = mail_detail_->mail_map.begin();
		for( ;select_iter != mail_detail_->mail_map.end(); select_iter++){
			if(select_iter->second.gold || select_iter->second.item_vector.size()) {
				have_one_key_pickup = 1;
			}
			if(select_iter->second.have_read == false){
				have_not_read_list.insert(std::make_pair(select_iter->first, select_iter->second));
			}
		}
		select_iter = mail_detail_->sys_mail_map.begin();
		for( ;select_iter != mail_detail_->sys_mail_map.end(); select_iter++){
			if(select_iter->second.gold || select_iter->second.item_vector.size()) {
				have_one_key_pickup = 1;
			}
			if(select_iter->second.have_read == false){
				have_not_read_list.insert(std::make_pair(select_iter->first,select_iter->second));
			}
		}
		return fetch_mail_list(page_count, have_not_read_list, have_one_key_pickup);
	}
	return 0;
}

int Mail::fetch_mail_list(const int &page_count, const Mail_Detail::Mail_Map &mail_map, const int8_t have_one_key_pickup){
	MSG_50150000 msg;
	msg.reset();
	if(mail_map.empty()){
		THIS_SEND_TO_CLIENT(msg);
	    return 0;
	}

	mail_detail_->mail_current_page = page_count; //当前页
	int total_mail = mail_map.size(); //总条数
	mail_detail_->mail_page_count = (total_mail / 6) + (total_mail % 6 == 0 ? 0 : 1); //总页数
	if(mail_detail_->mail_current_page > mail_detail_->mail_page_count) {  //保证当前页的正确性
		mail_detail_->mail_current_page = mail_detail_->mail_page_count;
	}
	int page_start = ((mail_detail_->mail_current_page - 1) * 6); // 当前页开始偏移量

	Mail_Info mail_info;
	int page_current = 0; //页内偏移量
	Mail_Detail::Mail_Map::const_iterator iter = mail_map.begin();
	for(int i = 0; i < page_start; i++){//直接跳转到当前页开始那条记录
		iter++;
	}
	for(; iter != mail_map.end() && page_current < 6; iter++) {
		mail_info.reset();
		mail_info.mail_id = iter->second.mail_id;
		mail_info.have_read = iter->second.have_read;
		mail_info.time = iter->second.show_send_time.sec();
		mail_info.mail_title = iter->second.mail_title;
		mail_info.gold = iter->second.gold;
		mail_info.mail_content = iter->second.mail_content;
		mail_info.mail_type = iter->second.mail_type;
		mail_info.role_name = iter->second.sender_name;
		//附件
		Item_Basic_Info it_info;
		for (Item_Vec::const_iterator it = iter->second.item_vector.begin(); it != iter->second.item_vector.end(); ++it) {
			it_info.reset();
			it_info.id      = it->id_;
			it_info.amount  = it->amount_;
			it_info.bind    = it->bind_;

			mail_info.item_vector.push_back(it_info);
		}

		if (mail_info.item_vector.size() > 0){
			mail_info.have_item = 1;
		}

		msg.mail_info_list.push_back(mail_info);
		page_current++;
	}


    msg.current_page = mail_detail_->mail_current_page;//当前页
    msg.total_page_num = mail_detail_->mail_page_count;//总页数
    msg.one_key_pickup = have_one_key_pickup;
    THIS_SEND_TO_CLIENT(msg);
    return 0;

}

int Mail::view_mail(int check_mail_id) {
	Mail_Detail::Mail_Map::iterator iter = mail_detail_->mail_map.find(check_mail_id);
	if (iter == mail_detail_->mail_map.end()) {
		iter = mail_detail_->sys_mail_map.find(check_mail_id);
		if(iter == mail_detail_->sys_mail_map.end()) {
			return ERROR_CLIENT_PARAM;
		}
	}

	iter->second.have_read = 1;

	Block_Buffer buf;
	MSG_50150001 msg_s;
	msg_s.reset();
	msg_s.check_back_id  = check_mail_id;
	mail_detail_->detail_change();
	THIS_SEND_TO_CLIENT(msg_s);

	return 0;
}

int Mail::pickup_mail_all_item(void) {
	Mail_Detail::Mail_Map all_mail;
	all_mail.clear();
	Mail_Detail::Mail_Map::iterator select_iter = mail_detail_->mail_map.begin();
	for( ;select_iter != mail_detail_->mail_map.end(); select_iter++){
		if(select_iter->second.have_read){
			all_mail.insert(std::make_pair(select_iter->first,select_iter->second));
		} else {
			all_mail.insert(std::make_pair(select_iter->first + pow(2,32),select_iter->second));
		}
	}
	select_iter = mail_detail_->sys_mail_map.begin();
	for( ;select_iter != mail_detail_->sys_mail_map.end(); select_iter++){
		if(select_iter->second.have_read){
			all_mail.insert(std::make_pair(select_iter->first,select_iter->second));
		} else {
			all_mail.insert(std::make_pair(select_iter->first + pow(2,32),select_iter->second));
		}
	}
	//保证顺序
	int res = 0;
	MSG_50150004 msg;
	msg.reset();
	for(Mail_Detail::Mail_Map::iterator iter = all_mail.begin(); iter != all_mail.end(); ++iter) {
		bool pick = false;
		//start try gold
		if(iter->second.gold){
			int money_result = player_self()->pack_try_add_money(Money_Add_Info(Money_Type(GOLD), iter->second.gold, MONEY_ADD_MAIL_PICKUP));
			if(money_result != 0) {
				res = money_result;
				break;
			}
		}
		//end try gold
		//start try item
		Item_Vec iv; //道具背包
		iv.clear();
		Item_Vec iv_r;//符石
		iv_r.clear();
		for(Item_Vec::iterator i_iter = iter->second.item_vector.begin(); i_iter !=  iter->second.item_vector.end(); ++i_iter) {
				Item_Detail &item =(* i_iter);
				const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item.id_);
				if (!item_config) {
					continue;
				}
				if (item_config->rune_stone.rune_type) {
					iv_r.push_back(item);
				} else {
					iv.push_back(item);
				}
		}
		if(iv.size()) {
			int result = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX, iv);
			if(result != 0) {
				res =  result;
				break;
			}
		}
		if(iv_r.size()) {
			int result = player_self()->pack_try_insert_item(Pack::PACK_T_RUNE_STONE_PACK_INDEX, iv_r);
			if(result != 0) {
				res =  result;
				break;
			}
		}
		//end try item
		if(iter->second.gold){
			int money_res = player_self()->pack_add_money(Money_Add_Info(Money_Type(GOLD), iter->second.gold, MONEY_ADD_MAIL_PICKUP));
			if(money_res == 0) {
				pick = true;
			}
		}
		//保存物品到背包
		if(iv.size()) {
			pick = true;
			player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, iv, Gain_Item_DM_Info(ITEM_GAIN_MAIL, iter->second.mail_type, iter->second.mail_id));
		}
		if(iv_r.size()) {
			pick = true;
			player_self()->pack_insert_item(Pack::PACK_T_RUNE_STONE_PACK_INDEX, iv_r, Gain_Item_DM_Info(ITEM_GAIN_MAIL, iter->second.mail_type, iter->second.mail_id));
		}
		if(pick){
			Mail_Detail::Mail_Map::iterator mail_iter = mail_detail_->mail_map.find(iter->second.mail_id);
			if (mail_iter == mail_detail_->mail_map.end()) {
				mail_iter = mail_detail_->sys_mail_map.find(iter->second.mail_id);
			}
			if(mail_iter != mail_detail_->mail_map.end() || mail_iter != mail_detail_->sys_mail_map.end()) {
				mail_iter->second.item_vector.clear();
				mail_iter->second.gold = 0;
				mail_iter->second.have_read = 1;
				msg.mail_id.push_back(iter->second.mail_id);
			}
		}
	}
	if(res) {
		msg.keep = 1;//是否还可以一键提取附件 0.不能 1.能
	}
	if(msg.mail_id.size() || msg.keep) {
		THIS_SEND_TO_CLIENT(msg);
		MSG_82200000 msg_s;//通知客户端邮件数量和未读邮件数量
		msg_s.mail_count = mail_detail_->mail_map.size();
		msg_s.not_read_count = get_not_read_count();
		THIS_SEND_TO_CLIENT(msg_s);
	}
	return res;
}

int Mail::pickup_mail_item(int mail_id, UInt_Vec index, uint8_t type){
	Mail_Detail::Mail_Map::iterator iter = mail_detail_->mail_map.find(mail_id);
	if (iter == mail_detail_->mail_map.end()) {
		iter = mail_detail_->sys_mail_map.find(mail_id);
		if(iter == mail_detail_->sys_mail_map.end()) {
			return ERROR_CLIENT_PARAM;
		}
	}
	Mail_Data &mail = iter->second;
	//检查钻石是否能够放入背包
	if(type && iter->second.gold) {
		int money_result = player_self()->pack_try_add_money(Money_Add_Info(Money_Type(GOLD), iter->second.gold, MONEY_ADD_MAIL_PICKUP));
		if(money_result != 0) {
			return money_result;
		}
	}
	//检测所有索引是否存在
	Item_Vec iv; //用于插入背包
	iv.clear();
	Item_Vec iv_r;//符石
	iv_r.clear();
	UInt_Set index_;//用户返回客户端
	index_.clear();
	std::vector<Item_Vec::iterator> iter_vec;//用户删除邮件里面的附件
	iter_vec.clear();
	MSG_81000102 res_msg;
	res_msg.type = 0;
	for(UInt_Vec::iterator index_iter = index.begin(); index_iter != index.end(); ++index_iter) {
		//for (Item_Vec::iterator it = iter->second.item_vector.begin(); it != iter->second.item_vector.end(); ++it) {
		if(mail.item_vector.size() > *index_iter && index_.find(*index_iter) == index_.end()) {
			index_.insert(*index_iter);
			Item_Detail &item = mail.item_vector[*index_iter];
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item.id_);
			if (!item_config) {
				continue;
			}
			if (item_config->rune_stone.rune_type) {
				iv_r.push_back(item);
			} else {
				iv.push_back(item);
			}

			Item_Vec::iterator it = mail.item_vector.begin() + (*index_iter);
			iter_vec.push_back(it);

			Item_Basic_Info item_base;
			item.set_item_basic_info(item_base);
			res_msg.item_info.push_back(item_base);
		}
		//}
	}
	if(iv.size()) {
		int result = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX, iv);
		if(result != 0) {
			return result;
		}
	}
	if(iv_r.size()) {
		int result = player_self()->pack_try_insert_item(Pack::PACK_T_RUNE_STONE_PACK_INDEX, iv_r);
		if(result != 0) {
			return result;
		}
	}

	//保存物品到背包
	if(iv.size()) {
		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, iv, Gain_Item_DM_Info(ITEM_GAIN_MAIL, mail.mail_type, mail.mail_id));
	}
	if(iv_r.size()) {
		player_self()->pack_insert_item(Pack::PACK_T_RUNE_STONE_PACK_INDEX, iv_r, Gain_Item_DM_Info(ITEM_GAIN_MAIL, mail.mail_type, mail.mail_id));
	}
	if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(res_msg);
	}

	for(std::vector<Item_Vec::iterator>::iterator it =  iter_vec.begin(); it != iter_vec.end(); ++it) {
		mail.item_vector.erase(*it);
	}
	//save gold
	if(type && mail.gold) {
		int money_res = player_self()->pack_add_money(Money_Add_Info(Money_Type(GOLD), mail.gold, MONEY_ADD_MAIL_PICKUP));
		if(money_res == 0) {
			mail.gold = 0;
		}
	}
	mail_detail_->detail_change();
	MSG_50150003 msg;
	msg.reset();
	msg.item_vector_id = index;
	msg.type = type;
	THIS_SEND_TO_CLIENT(msg);

	iv.clear();
	return 0;
}

int Mail::mutidelete_mail(Int_Vec muti_mail_id){
	Block_Buffer buf;
	MSG_50150005 msg_s;
	msg_s.reset();
	int status = 0;
	for(Int_Vec::iterator id_ite = muti_mail_id.begin(); id_ite != muti_mail_id.end(); id_ite++){
		Mail_Detail::Mail_Map::iterator mail_map_iter = mail_detail_->mail_map.find(*id_ite);
		if(mail_map_iter != mail_detail_->mail_map.end()) {
			if(mail_map_iter->second.item_vector.size() || mail_map_iter->second.gold) {
				status = ERROR_HAVE_ITEM;
			} else {
				MSG_DEBUG("mutidelete_mail, role_id:%ld, mail_title:%s, mail_contents:%s", player_self()->role_id(), mail_map_iter->second.mail_title.c_str(), mail_map_iter->second.mail_content.c_str());
				mail_detail_->mail_map.erase(mail_map_iter);
				msg_s.mutil_mail_id.push_back(*id_ite);
			}
		}
	}
	// delete system mail
	for(Int_Vec::iterator id_ite = muti_mail_id.begin(); id_ite != muti_mail_id.end(); id_ite++){
		Mail_Detail::Mail_Map::iterator mail_map_iter = mail_detail_->sys_mail_map.find(*id_ite);
		if(mail_map_iter != mail_detail_->sys_mail_map.end()) {
			if(mail_map_iter->second.item_vector.size() || mail_map_iter->second.gold) {
				status = ERROR_HAVE_ITEM;
			} else {
				MSG_DEBUG("mutidelete_mail, role_id:%ld, mail_title:%s, mail_contents:%s", player_self()->role_id(), mail_map_iter->second.mail_title.c_str(), mail_map_iter->second.mail_content.c_str());
				mail_detail_->sys_mail_map.erase(mail_map_iter);
				msg_s.mutil_mail_id.push_back(*id_ite);
			}
		}
	}
    mail_detail_->detail_change();

	THIS_SEND_TO_CLIENT(msg_s);

	fetch_mail_state_list(mail_detail_->mail_current_page, mail_detail_->mail_state_page);
	return status;
}

int Mail::client_send_mail(Mail_Info mail_info) {
//	if(CONFIG_CACHE->server_maintainer_cache().game_ver_type) {////判断国内(0)还是国外
//		if(player_self()->level() < 35) {
//			return ERROR_LEVEL_LIMIT_SEND_MAIL;
//		}
//	} else {
		if(player_self()->level() < 15) {
			return ERROR_LEVEL_LIMIT_SEND_MAIL;
		}
//	}
	{//判断道具数量限制
		const uint32_t item_limited = CONFIG_CACHE_MAIL->find_common_config("item_limited");
		if(mail_info.item_vector.size() > item_limited) {
			return ERROR_CLIENT_OPERATE;
		}
	}
	// 标题内容长度检查
	if (mail_info.mail_title.size() <= 0 || mail_info.mail_title.size() > MAIL_TITLE_MAX_LEN || mail_info.mail_content.size() > MAIL_CONTENT_MAX_LEN) {
		MSG_USER("title:%s, content:%s", mail_info.mail_title.c_str(), mail_info.mail_content.c_str());
		return ERROR_CONTENT_TITLE;
	}

	// 目标是否存在
	role_id_t receiver_id = LOGIC_MONITOR->get_role_id_by_name(mail_info.role_name);
	if (! receiver_id) {
		MSG_50150002 msg_s;
		msg_s.verify_code = 0; //客户端要求; 目标不存在需要发送消息 0 失败
		THIS_SEND_TO_CLIENT(msg_s);
		return ERROR_ROLE_NOT_EXIST;
	}

	// 不能给自己发
	if (receiver_id == role_id())
		return ERROR_RECEIVER_NOT_SELF;

	// 附件是否存在
	Item_Vec item_vector;
	for (std::vector<Item_Basic_Info>::iterator it = mail_info.item_vector.begin();it != mail_info.item_vector.end(); ++it) {
		const Item_Detail *item = this->pack_get_const_item(it->index);
		if (item == NULL || item->is_item_nontradable())
			return ERROR_CLIENT_PARAM;

		item_vector.push_back(*item);
	}

	//start  验证邮资是否足够
	//检查金币是否够
	uint32_t postage_copper = CONFIG_CACHE_MAIL->find_postage_config(mail_info.item_vector.size());
	if(postage_copper) {
		int return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), postage_copper, MONEY_SUB_MAIL_POSTAGE));
		if(return_status != 0) {
			return return_status;
		}
	} else {
		return ERROR_CONFIG_NOT_EXIST;
	}

	//如果发钻石,检查钻石是否足够支付(含钻石邮费)
	uint32_t postage_gold = 0;
	if(mail_info.gold) {
		//max发送钻石
		if(mail_detail_->max_gold + mail_info.gold > 700) {
			return ERROR_SEND_GOLD_MAX_LIMIT;
		}

		//计算邮费
		int mod = (mail_info.gold * 700) % 10000;
		postage_gold = (((int32_t)(mail_info.gold * 0.07)) + (mod?1:0));

		int return_status = pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), mail_info.gold + postage_gold, MONEY_SUB_MAIL_POSTAGE));
		if(return_status != 0) {
			return return_status;
		}
	}
	//end  验证邮资是否足够
	// 附件数量不为0时,就从玩家背包内删除道具
	int result = 0;
	for (std::vector<Item_Basic_Info>::iterator it = mail_info.item_vector.begin();it != mail_info.item_vector.end(); ++it) {
		result = player_self()->pack_erase_item(Index_Amount(it->index, it->amount, UNBIND_ONLY),
				Lose_Item_DM_Info(ITEM_LOSE_MAIL, receiver_id));
		if (result != 0)
			return result;
	}
	//金币邮费
	int return_status = 0;
	return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY), postage_copper, MONEY_SUB_MAIL_POSTAGE));
	if(return_status) {
		return return_status;
	}
	if(mail_info.gold) {
		//钻石邮费
		return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), postage_gold, MONEY_SUB_MAIL_POSTAGE));
		if(return_status) {
			return return_status;
		}
		//钻石
		return_status = pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), mail_info.gold, MONEY_SUB_MAIL_MONEY));
		if(return_status) {
			return return_status;
		}
	}

	mail_detail_->max_gold += mail_info.gold;//记录总发送钻石
	mail_detail_->detail_change();
	Mail_Send_Info mail_send_info = Mail_Send_Info(receiver_id, role_id(), MAIL_TYPE_USER, mail_info.role_name, mail_info.mail_title, mail_info.mail_content);
	mail_send_info.item_vector = item_vector;
	mail_send_info.show_send_time = Time_Value::gettimeofday();
	mail_send_info.gold = mail_info.gold;
	LOGIC_PUBLIC->send_mail(mail_send_info);

	//任务监听发邮件成功 start
	player_self()->task_listen_send_mail();
	//任务监听发邮件成功 end

	MSG_50150002 msg_s;
	msg_s.verify_code = 1;////客户端要求; 需要发送消息 1 成功
	THIS_SEND_TO_CLIENT(msg_s);

    return 0;
}

int Mail::send_mail_to_self(Mail_Data &mail_data) {
	MSG_DEBUG("mail_to_self, role_id:%ld, title:%s, contents:%s, mail_type:%d, item_num:%d", player_self()->role_id(), mail_data.mail_title.c_str(), mail_data.mail_content.c_str(), mail_data.mail_type, mail_data.item_vector.size());
	mail_data.mail_id = mail_detail_->mail_all_count++;
	if(mail_data.mail_type == MAIL_TYPE_USER) {
		//普通邮件数量超过100，删除最后一封
		while (mail_detail_->mail_map.size() >= CONFIG_CACHE_MAIL->find_common_config("max_user_mail")) {
			mail_detail_->mail_map.erase(--mail_detail_->mail_map.end());
		}
		mail_detail_->mail_map.insert(std::make_pair(mail_data.mail_id, mail_data));
	} else {
		//系统邮件数量超过20，删除最后一封
		while (mail_detail_->sys_mail_map.size() >= CONFIG_CACHE_MAIL->find_common_config("max_sys_mail")) {
			Mail_Map::iterator iter = --mail_detail_->sys_mail_map.end();
			MSG_DEBUG("max sys mail num to del, mail_title:%s, mail_contents:%s, item_num:%d",iter->second.mail_title.c_str(), iter->second.mail_content.c_str(), iter->second.item_vector.size());
			mail_detail_->sys_mail_map.erase(iter);
		}
		mail_detail_->sys_mail_map.insert(std::make_pair(mail_data.mail_id, mail_data));
	}
	return 0;
}

int Mail::send_mail_to_self(Mail_Send_Info &mail_info) {
	if (mail_info.title.size() > MAIL_TITLE_MAX_LEN || mail_info.content.size() > MAIL_CONTENT_MAX_LEN) {
		MSG_USER("title:%s, content:%s, role_id:%ld", mail_info.title.c_str(), mail_info.content.c_str(), player_self()->role_id());
		return ERROR_CONTENT_TITLE;
	}
	//按物品堆叠数拆分
	Item_Vec all_item;
	all_item.clear();
	for(Item_Vec::iterator it = mail_info.item_vector.begin(); it != mail_info.item_vector.end(); ++it) {
		const Item_Detail_Config *conf = CONFIG_CACHE_ITEM->find_item(it->id_);
		if(!conf){
			continue;
		}
		if(MAIL_ITEM_MAX_NUM < it->amount_ / conf->overlay) {
			MSG_TRACE_ABORT("Mail Item amount ERROR");
			return ERROR_PERM_LIMIT;
		}
		while(conf->overlay < it->amount_) {
			Item_Detail tmp = (*it);
			tmp.amount_ = conf->overlay;
			all_item.push_back(tmp);
			it->amount_ -= conf->overlay;
		}
		all_item.push_back(*it);
	}

//	Item_Vec all_item;
//	all_item.clear();
//	for(Item_Vec::iterator it = mail_info.item_vector.begin(); it != mail_info.item_vector.end(); ++it) {
//		const Item_Detail_Config *conf = CONFIG_CACHE_ITEM->find_item(it->id_);
//		if(!conf){
//			continue;
//		}
//		if(conf->overlay < it->amount_) {
//			MSG_USER_TRACE("Mail Item amount ERROR");
//			return ERROR_PERM_LIMIT;
//		}
//		all_item.push_back(*it);
//	}

	//公用信息
	Mail_Data mail_data;
	mail_data.reset();
	mail_data.mail_type = mail_info.send_type;
	mail_data.sender_name = mail_info.sender_name;
	mail_data.mail_title = mail_info.title;
	mail_data.mail_content = mail_info.content;
	mail_data.show_send_time = mail_info.show_send_time;
	mail_data.real_send_time = Time_Value::gettimeofday();

	//按4个一组发邮件
	Item_Vec group_item;
	group_item.clear();
	bool over_send = true;
	for(Item_Vec::iterator it = all_item.begin(); it != all_item.end(); ++it) {
		group_item.push_back(*it);
		if(group_item.size() >= 4) {
			mail_data.gold = mail_info.gold;
			mail_data.item_vector = group_item;
			send_mail_to_self(mail_data);
			group_item.clear();
			mail_info.gold = 0;
			over_send = false;
		}
	}

	if(group_item.size() || mail_info.gold || over_send) {
		mail_data.gold = mail_info.gold;
		mail_data.item_vector = group_item;
		send_mail_to_self(mail_data);
	}

	mail_detail_->detail_change();
	//通知客户端有新邮件;
	MSG_82200000 msg_s;
	msg_s.mail_count = mail_detail_->mail_map.size();
	msg_s.not_read_count = get_not_read_count();
	THIS_SEND_TO_CLIENT(msg_s);
	return 0;
}

void Mail::set_last_read_sys_mail_time(Time_Value &time) {
	mail_detail_->last_read_sys_mail_time = time;
}

int Mail::get_not_read_count(void) {
	int not_read_count = 0;
	for(Mail_Map::iterator it = mail_detail_->mail_map.begin(); it != mail_detail_->mail_map.end(); ++it) {
		if(it->second.have_read == 0) {
			++not_read_count;
		}
	}
	for(Mail_Map::iterator it = mail_detail_->sys_mail_map.begin(); it != mail_detail_->sys_mail_map.end(); ++it) {
		if(it->second.have_read == 0) {
			++not_read_count;
		}
	}
	return not_read_count;
}

void Mail::del_all_mail(void) {
	mail_detail_->mail_map.clear();
	mail_detail_->sys_mail_map.clear();
	mail_detail_->detail_change();
}
