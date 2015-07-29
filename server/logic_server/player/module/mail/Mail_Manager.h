/*
 * Mail.h
 *
 *  Created on: 2012-6-15
 *      Author: xiao liang
 */

#ifndef MAIL_MANAGER_H_
#define MAIL_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"

struct Mail_Data;
typedef std::vector<Mail_Data> Sys_Mail_Vec;

class Mail_Manager {

public:
	Mail_Manager();
	virtual ~Mail_Manager();

	int start(void);
	const Sys_Mail_Vec &get_sys_mail_vec(void);

	int send_sys_mail_to_all(std::string &sender, std::string &title, std::string &contents, int32_t gold, std::vector<Item_Info> &item);
	int send_sys_mail_to_level_rang(int32_t slevel, int32_t elevel, std::string &sender, std::string &title,
			std::string &contents, int32_t gold, std::vector<Item_Info> &item, int32_t career);

private:
	Sys_Mail_Vec sys_mail_;
};

inline const Sys_Mail_Vec &Mail_Manager::get_sys_mail_vec(void) {
	return sys_mail_;
}

typedef Singleton<Mail_Manager> Mail_Manager_Single;
#define MAIL_MANAGER    (Mail_Manager_Single::instance())

#endif /* MALL_H_ */
