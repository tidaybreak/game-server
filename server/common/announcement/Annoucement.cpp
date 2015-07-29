/*
 * Annoucement.cpp
 *
 *  Created on: May 7, 2014
 *      Author: caijianwei
 */

#include "Annoucement.h"

Annoucement::~Annoucement() { }

Annoucement::Annoucement() {
	reset();
}

void Annoucement::reset() {
	district = 0;
	announce_id = 0;
	CLEAR_VECTOR(Role_Vec, recievers);
	CLEAR_VECTOR(std::vector<Chat_Content>, ann_contents);
}

void Annoucement::make_announce(int32_t district, int32_t ann_id, gang_id_t gang_id) {
	this->reset();
	this->district = district;
	this->announce_id = ann_id;
	this->gang_id = gang_id;
}

void Annoucement::announce_push_text(const std::string& text) {
	Chat_Content new_content;
	new_content.type = ANN_CTTYPE_TEXT;
	new_content.text = text;
	ann_contents.push_back( new_content );
}

void Annoucement::announce_push_recievers(const std::vector<int64_t>& recievers) {
	this->recievers = recievers;
}

void Annoucement::announce_push_item(const Chat_Item_Info& item) {
	Chat_Content new_content;
	new_content.type = ANN_CTTYPE_ITEM;
	new_content.item = item;
	ann_contents.push_back( new_content );
}

void Annoucement::announce_push_role(const Role_Info& role) {
	Chat_Content new_content;
	new_content.type = ANN_CTTYPE_ROLE_INFO;
	new_content.role_Info = role;
	ann_contents.push_back( new_content );
}

void Annoucement::announce_push_prop_id(const int32_t prop_id) {
	Chat_Content new_content;
	new_content.type = ANN_CTTYPE_PROP_ID;
	new_content.prop_id = prop_id;
	ann_contents.push_back( new_content );
}

//void Annoucement::announce_push_gang_info(const Gang_Row_Info& gang_info) {
//	Chat_Content new_content;
//	new_content.type = ANN_CTTYPE_GANG_INFO;
//	// TODO assign gang info
//	ann_contents.push_back( new_content );
//}
