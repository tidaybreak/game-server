/*
 * Chat_Gang_Manager.h
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#ifndef CHAT_GANG_MANAGER_H_
#define CHAT_GANG_MANAGER_H_

#include "Singleton.h"
#include "Chat_Gang_Struct.h"

struct Chat_Record;

class Chat_Gang_Manager {
public:
	typedef boost::unordered_map<int64_t, Chat_Gang_Info> Gang_ID_Info_Map;
	typedef boost::unordered_map<int64_t, Gang_Chat_Record> Gang_ID_Chat_Record_Map;
	Chat_Gang_Manager();
	~Chat_Gang_Manager();

	void start();
	void save();

	void add_ganger(int64_t gang_id, role_id_t role_id);
	void remove_ganger(int64_t gang_id, role_id_t role_id);
	Chat_Gang_Info* gang_info_by_id(int64_t gang_id);
	const Gang_ID_Info_Map& get_gang_map(void);

	void save_chat_record(gang_id_t gang_id, Chat_Record& chat_record);
	std::vector<Chat_Record>* get_chat_record_by_id(gang_id_t gang_id);

	void reset();

private:
	Gang_ID_Info_Map gang_id_map;
	Gang_ID_Chat_Record_Map gand_chat_record_map;
};

typedef Singleton<Chat_Gang_Manager> Chat_Gang_Manager_Single;
#define CHAT_GANG_MANAGER Chat_Gang_Manager_Single::instance()

#endif /* CHAT_GANG_MANAGER_H_ */
