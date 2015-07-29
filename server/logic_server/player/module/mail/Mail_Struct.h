/*
 * Mail_Sruct.h
 *
 *  Created on: Mar 14, 2014
 *      Author: zhouyajun
 */

#ifndef MAIL_SRUCT_H_
#define MAIL_SRUCT_H_

#include "Server_Struct.h"
#include "../pack/Pack_Struct.h"

/*
邮件发送信息
备注：
*/


struct Mail_Send_Info {
	role_id_t receiver_id;/*接收人*/
	role_id_t sender_id;/*发送人*/
	int32_t send_type;/*邮件类型*/
	std::string sender_name;/**/
	std::string title;/**/
	std::string content;/**/
	int gold;
	Time_Value show_send_time;
	Item_Vec item_vector;
	Mail_Send_Info(void) { reset(); };
	Mail_Send_Info(role_id_t r_id, role_id_t s_id, int s_type, std::string &s_name, std::string &title, std::string &content);
	Mail_Send_Info(role_id_t r_id, role_id_t s_id, int s_type, std::string &s_name, std::string &title, std::string &content, int s_gold);
//	Mail_Send_Info(role_id_t r_id, role_id_t s_id, int s_type, std::string &s_name, std::string &title, std::string &content, Item_Vec &item_vector);

	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);
};

struct Mail_Data {
	role_id_t mail_id;
	int8_t mail_type;						// 0:sys,1:user,2,php
	int8_t have_read;						// 1是 0否已读
	int32_t gold;
	std::string sender_name;              	// 收到邮件时为发件人名称，发送邮件时为收件人名称
	std::string mail_title;
	std::string mail_content;
	Item_Vec item_vector;
	Time_Value show_send_time;				// 显示的发送时间
	Time_Value real_send_time;				// 实际发送时间

	Mail_Data(void) { reset(); };
	void reset();
};

struct Mail_Detail : public Detail{
	Mail_Detail();
	virtual ~Mail_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	typedef std::map<long long int, Mail_Data, std::greater<long long int> > Mail_Map;	//邮件map要按照邮件id排序，不能改成unordered_map

	int mail_all_count; 	//邮件的历史总数量，即目前为止收到的所有邮件数,用于产生邮件ID
	int8_t mail_page_count;
	int8_t mail_current_page;
	int8_t mail_state_page;     //0:all, 1:have_read, 2:have_not_read
	Mail_Map mail_map;
	Mail_Map sys_mail_map;
	//int not_read_count;      //未读邮件个数
	Time_Value last_read_sys_mail_time;

	uint32_t max_gold;
};

#endif /* MAIL_SRUCT_H_ */
