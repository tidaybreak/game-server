/*
 * Chat_Gang_Struct.h
 *
 *  Created on: May 8, 2014
 *      Author: caijianwei
 */

#ifndef CHAT_GANG_STRUCT_H_
#define CHAT_GANG_STRUCT_H_

#include "Game_Typedef.h"

struct Chat_Record;

struct Chat_Gang_Info {
	Chat_Gang_Info();
	void reset();

	int64_t gang_id;
	Role_Hash_Set members;
};

struct Gang_Chat_Record {
	Gang_Chat_Record();
	void reset();

	std::vector<Chat_Record> chat_record_vec;
	bool is_change;
};

#endif /* CHAT_GANG_STRUCT_H_ */
