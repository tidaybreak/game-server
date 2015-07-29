/*
 * Chat_Control_Manager.h
 *
 *  Created on: 2014-06-19
 *      Author: xiaoliang
 */

#ifndef CHATER_CONTROL_MANAGER_H_
#define CHATER_CONTROL_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Msg_Struct.h"

class Chat_Player;

struct Chat_Control_Info {
	Chat_Control_Info(void) { reset(); }
	void reset(void) {
		role_id_gag.clear();
		account_gag.clear();
	}
	Role_Id_Time_Map role_id_gag;
	String_Time_Map account_gag;
};

class Chat_Control_Manager {
public:
	Chat_Control_Manager();
	~Chat_Control_Manager();
	void reset();

	void tick(const Time_Value &now);
	void start(void);

	bool is_gag(Chat_Player *player);

	void set_bag_account(std::string &account, uint32_t time);
	void set_role_id_account(role_id_t &role_id, uint32_t time);

	void unset_bag_account(std::string &account);
	void unset_role_id_account(role_id_t &role_id);

private:

	Chat_Control_Info control_info;

};

typedef Singleton<Chat_Control_Manager> Chat_Control_Manager_Single;
#define CHATER_CONTROL_MANAGER Chat_Control_Manager_Single::instance()

#endif /* CHATER_CONTROL_MANAGER_H_ */
