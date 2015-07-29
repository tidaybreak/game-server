/*
 * Chat_Teamer.h
 *
 *  Created on: 2014年6月4日
 *      Author: Linqiyou
 */

#ifndef CHAT_TEAMER_H_
#define CHAT_TEAMER_H_

#include "Chat_Player_Base.h"
#include "team/Team_Struct.h"

struct Teamer_Detail;

class Chat_Teamer : public Chat_Player_Base {
public:
	Chat_Teamer();
	virtual ~Chat_Teamer();

//	int syn_team_info(Block_Buffer &buf);
	int syn_teamer_info(Block_Buffer &buf);

	int64_t get_team_id();

	void reset(void);

private:
	Teamer_Detail teamer_detail_;
};


#endif /* CHAT_TEAMER_H_ */
