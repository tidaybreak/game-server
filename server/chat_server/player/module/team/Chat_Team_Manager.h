/*
 * Chat_Team_Manager.h
 *
 *  Created on: 2014年6月4日
 *      Author: Linqiyou
 */

#ifndef CHAT_TEAM_MANAGER_H_
#define CHAT_TEAM_MANAGER_H_

#include "team/Team.h"

class Chat_Team_Manager {
public:
	Chat_Team_Manager(void);
	~Chat_Team_Manager(void);

	Team *get_team(int64_t team_id);
	int add_team(Block_Buffer &buf);
	int del_team(int64_t team_id);

	void reset(void);

private:
	Int64_Team_Map_Detail team_map_;			// 队伍 id->队伍 映射表
	UInt_Uid team_index_id_map_;		// 队伍 index->id 映射表
};

typedef Singleton<Chat_Team_Manager> Chat_Team_Manager_Single;
#define CHAT_TEAM_MANAGER Chat_Team_Manager_Single::instance()

#endif /* CHAT_TEAM_MANAGER_H_ */
