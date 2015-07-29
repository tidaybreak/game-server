/*
 * Logic_Public.h
 *
 *  Created on: Nov 25, 2013
 *      Author: 杂项/公共
 */

#ifndef LOGIC_PUBLIC_H_
#define LOGIC_PUBLIC_H_

#include "Game_Typedef.h"

struct Item_Detail;
struct Mail_Send_Info;
typedef std::vector<Item_Detail> Item_Vec;
struct Item_Basic_Info;

class Logic_Public {
public:
	Logic_Public();
	virtual ~Logic_Public();
	int init(void);
	int start(void);

	Time_Value &reboot_time(void);
	void set_reboot_time(Time_Value t);

	void tick(const Time_Value &now);

	int rand_role_name(void);

	// 发邮件
	int send_mail(Role_Vec &role_vec, Mail_Send_Info &send_info);
	int send_mail(int id, Role_Vec &role_vec);
	int send_mail(const role_id_t role_id, const std::string &sender_name, const std::string &title, const std::string &contents, const Item_Vec &item_vector);
	int send_mail(Mail_Send_Info &send_info);
	int send_mail(int id, role_id_t role_id, const std::string &title = "", const std::string &contents = "");
	int send_mail(int id, role_id_t role_id, const Item_Vec &item_vector, const std::string &title = "", const std::string &contents = "");
	int send_mail(int id, role_id_t role_id, const std::vector<Item_Basic_Info> &item_vector, const std::string &title = "", const std::string &contents = "");
	int send_mail(int id, role_id_t role_id, const uint32_t gold, const std::vector<Item_Basic_Info> &item_vector, const std::string &title = "", const std::string &contents = "");
	int send_mail(int id, role_id_t role_id, const uint32_t gold, const std::string &title = "", const std::string &contents = "");
	//充值发放
	int recharge_grant(role_id_t role_id, int pay_num);
	int inner_recharge_grant(role_id_t role_id, int pay_num);
	//你提我改
	int reply_proposal(role_id_t role_id, std::string &sender, std::string &title, std::string &contents, int32_t gold);
	//facebook分享点赞奖励邮件
	int send_facebook_share_mail(role_id_t role_id, uint8_t type);

	bool has_role_by_id(role_id_t role_id);

	std::string get_name(int gender);
	void erase_name(int gender, const std::string & name);

	int record_access_count(uint32_t type);
	void save_access_count(const Time_Value &now);

private:

	Time_Value reboot_time_;
	String_Set male_name_set;
	String_Set female_name_set;

	UInt_UInt_Map access_count;
	Time_Value access_count_record_time;
};

typedef Singleton<Logic_Public> Logic_Public_Single;
#define LOGIC_PUBLIC    (Logic_Public_Single::instance())

#endif /* LOGIC_PUBLIC_H_ */
