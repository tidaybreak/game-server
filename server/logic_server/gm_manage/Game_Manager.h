/*
 * Game_Manager.h
 *
 *  Created on: Jan 15, 2014
 *      Author: ti
 *		后台GM模块
 *      存放后台数据
 */

#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Game_Typedef.h"

class Game_Manager {
public:

	Game_Manager();
	virtual ~Game_Manager();

	void start(void);
	//初始化禁用列表
	void init_forbid_set();
	void set_forbid_account(std::string &account, Time_Value time); //PHP通知封帐号
	void set_forbid_role_id(role_id_t role_id, Time_Value time);    //PHP通知封角色ID
	void erase_forbid_account(std::string &account);				//PHP通知解封帐号
	void erase_forbid_role_id(role_id_t role_id);					//PHP通知解封角色ID
	void set_forbid_ip(std::string &ip_address, Time_Value time); //PHP通知封IP
	void erase_forbid_ip(std::string &ip_address);				//PHP通知解封IP

	bool check_forbid_ip_set(std::string &ip);
	bool check_forbid_account_list(std::string &account);
	bool check_forbid_role_id_list(role_id_t role_id);

private:
	// 封禁列表
	String_Time_Map forbid_ip_set_;
	String_Time_Map forbid_account_list_;
	Role_Id_Time_Map forbid_role_id_list_;
	//String_Set forbid_ip_set_;
	//String_Set forbid_account_list_;
	//Role_Id_Set forbid_role_id_list_;
};

typedef Singleton<Game_Manager> Game_Manager_Single;
#define GM_MANAGER    (Game_Manager_Single::instance())

#endif /* GAME_MANAGER_H_ */
