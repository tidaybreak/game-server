/*
 * Mail.h
 *
 *  Created on: Mar 7, 2014
 *      Author: zhouyajun
 */

#ifndef MAIL_H_
#define MAIL_H_

#include "Logic_Player_Base.h"
#include "Mail_Def.h"

class Packer;
struct Mail_Detail;
struct Mail_Data;
struct Mail_Info;
struct Mail_Send_Info;
typedef std::map<long long int, Mail_Data, std::greater<long long int> > Mail_Map;

class Mail : virtual public Packer {
public:
	Mail(void);
	virtual ~Mail();
	void reset(void);

	// 创建角色时初始化
	static int create_init(Mail_Detail &detail);
	// 登录时detail赋值
	void load_detail(Mail_Detail &detail);
	// 保存时detail赋值
	void save_detail(Mail_Detail &data);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	Mail_Detail const &mail_detail(void) const;

 //   void insert_mail(int mail_id, const Mail_Data &mail_info_detail);
	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	void client_ready_now(void);

	int fetch_mail_list(const int &page_count, const Mail_Map &mail_map, const int8_t have_one_key_pickup);

	int fetch_mail_state_list(const int &page_count, const int &mail_state);

	int view_mail(int mail_id);

	int client_send_mail(Mail_Info mail_info);

	// 发给自己
	int send_mail_to_self(Mail_Send_Info &mail_info);

	int mutidelete_mail(Int_Vec muti_mail_id);

	int pickup_mail_item(int mail_id, UInt_Vec index, uint8_t type = 0);
	int pickup_mail_all_item(void);

	//interface
	void set_last_read_sys_mail_time(Time_Value &time);

	int get_not_read_count(void);

	void del_all_mail(void);

private:
	Mail_Detail *mail_detail_;

	int send_mail_to_self(Mail_Data &mail_data);
};

inline Mail_Detail const &Mail::mail_detail(void) const{
	return *mail_detail_;
}


#endif /* MAIL_H_ */
