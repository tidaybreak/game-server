/*
 * Annoucement.h
 *
 *  Created on: May 7, 2014
 *      Author: caijianwei
 */

#ifndef ANNOUCEMENT_H_
#define ANNOUCEMENT_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"

enum ANN_CONTENT_TYPE {
	ANN_CTTYPE_TEXT = 0,
	ANN_CTTYPE_ITEM = 1,
	ANN_CTTYPE_ROLE_INFO = 2,
	ANN_CTTYPE_PROP_ID = 3,
	ANN_CTTYPE_GANG_INFO,
	ANN_CTTYPE_END
};

class Annoucement {
public:
	virtual ~Annoucement();
	Annoucement();

	void reset();
	virtual void make_announce(int32_t district, int32_t ann_id=0, gang_id_t gang_id = 0);
	virtual void announce_push_text(const std::string& text);
	virtual void announce_push_recievers(const std::vector<int64_t>& recievers);
	virtual void announce_push_item(const Chat_Item_Info& item);
	virtual void announce_push_role(const Role_Info& role);
	virtual void announce_push_prop_id(const int32_t prop_id);
//	virtual void announce_push_gang_info(const Gang_Row_Info& gang_info);
	virtual void announce_send() = 0;

protected:
	int32_t district;
	int32_t announce_id;
	int64_t gang_id;
	Role_Vec recievers;
	std::vector<Chat_Content> ann_contents;
};

#endif /* ANNOUCEMENT_H_ */
