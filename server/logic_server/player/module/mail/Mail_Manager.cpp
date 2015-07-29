/*
 * Mail.cpp
 *
 *  Created on: 2012-6-15
 *      Author: xiaoliang
 */

#include "Mail_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "public/Public_Def.h"
#include "Mail_Struct.h"
#include "Logic_Public.h"
#include "Logic_Player_Struct.h"

Mail_Manager::Mail_Manager() {
	// TODO Auto-generated constructor stub

}

Mail_Manager::~Mail_Manager() {
	// TODO Auto-generated destructor stub
}

int Mail_Manager::start(void) {
	MONGO_OPERATOR->load_sys_mail_list(sys_mail_);//初始化sys_mail
	return 0;
}

int Mail_Manager::send_sys_mail_to_level_rang(int32_t slevel, int32_t elevel, std::string &sender, std::string &title,
		std::string &contents, int32_t gold, std::vector<Item_Info> &item, int32_t career) {
	Mail_Send_Info send_mail_info = Mail_Send_Info(0l, 0l, 0, sender, title, contents);
	send_mail_info.gold = gold;
	send_mail_info.send_type = MAIL_TYPE_PHP;
	send_mail_info.item_vector.clear();
	for(std::vector<Item_Info>::iterator it = item.begin(); it != item.end(); ++it) {
		send_mail_info.item_vector.push_back(Item_Detail(it->id, it->amount, it->bind));
	}
	send_mail_info.show_send_time = Time_Value::gettimeofday();
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
	for(; iter != LOGIC_MONITOR->db_view()->id_player_view_map.end(); ++iter) {
		if(iter->second->level >= slevel && iter->second->level <= elevel) {
			if(career ==0 || career == iter->second->career ) {
				send_mail_info.receiver_id = iter->second->role_id;
				//Mail_Send_Info mail = send_mail_info;//拆分物品时候有可能被改写
				LOGIC_PUBLIC->send_mail(send_mail_info);
			}
		}
	}
	return 0;
}

int Mail_Manager::send_sys_mail_to_all(std::string &sender, std::string &title, std::string &contents, int32_t gold, std::vector<Item_Info> &item) {
	Time_Value send_time = Time_Value::gettimeofday();
	//save sys mail
	Mail_Data mail_info_;
	mail_info_.reset();
	mail_info_.show_send_time = send_time;
	mail_info_.have_read = 0;
	mail_info_.sender_name = sender;
	mail_info_.mail_title = title;
	mail_info_.mail_content = contents;
	mail_info_.mail_type = MAIL_TYPE_PHP;
	mail_info_.gold = gold;
	mail_info_.item_vector.clear();
	for(std::vector<Item_Info>::iterator it = item.begin(); it != item.end(); ++it) {
		mail_info_.item_vector.push_back(Item_Detail(it->id, it->amount, it->bind));
	}
		//save to sys_mail_
	sys_mail_.push_back(mail_info_);
		//save to db
	MONGO_OPERATOR->save_sys_mail_list(mail_info_);

	//send to all online player;
	for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator iter = LOGIC_MONITOR->player_role_id_map().begin(); iter != LOGIC_MONITOR->player_role_id_map().end(); iter++) {
		Mail_Send_Info send_mail_info = Mail_Send_Info(iter->first, 0l, mail_info_.mail_type, sender, title, contents);
		send_mail_info.gold = mail_info_.gold;
		send_mail_info.item_vector =  mail_info_.item_vector;
		send_mail_info.show_send_time = send_time;
		iter->second->send_mail_to_self(send_mail_info);
		iter->second->set_last_read_sys_mail_time(send_time);
	}

	return 0;
}

