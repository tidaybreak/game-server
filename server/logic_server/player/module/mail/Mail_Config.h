/*
 * Mail_Config.h
 *
 *  Created on: 2014-05-05
 *      Author: xiaoliang
 */

#ifndef MAIL_CONFIG_H_
#define MAIL_CONFIG_H_

#include "Base_Struct.h"

struct Mail_Base_Config {
	Mail_Base_Config(void) { reset(); }
	void reset(void) {
		type_id = 0;
		sender_name.clear();
		mail_title.clear();
		mail_content.clear();
		item_vector.clear();
	}
	uint32_t type_id;
	std::string sender_name;
	std::string mail_title;
	std::string mail_content;
	std::vector<Item_Basic_Info> item_vector;
};

typedef boost::unordered_map<uint32_t, Mail_Base_Config> Mail_Config_Map;

struct Mail_Postage_Config {
	Mail_Postage_Config(void) { reset(); }
	void reset(void) {
		item_number = 0;
		postage = 0;
	}
	uint32_t item_number;
	uint32_t postage;
};

typedef boost::unordered_map<uint32_t, Mail_Postage_Config> Mail_Postage_Config_Map;


typedef boost::unordered_map<std::string, int> Mail_Common_Config_Map;

#endif /* MAIL_CONFIG_H_ */
