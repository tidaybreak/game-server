/*
 * Alchemy_Sruct.h
 *
 *  Created on: 2014-05-12
 *      Author: xiaoliang
 */

#ifndef ALCHEMY_SRUCT_H_
#define ALCHEMY_SRUCT_H_

#include "Server_Struct.h"
#include "Alchemy_Config.h"

struct Alchemy_Detail : public Detail{
	Alchemy_Detail();
	virtual ~Alchemy_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	Cards_info current_hero_critical;
	Cards_info current_hero_gold;
	uint8_t used_free_bout;
	uint8_t used_pay_bout;
	uint8_t last_tabs;
	uint8_t six_critical_num;


	Cards_info current_souls_hero_critical;
	Cards_info current_souls_hero_gold;
	uint8_t used_souls_free_bout;
	uint8_t used_souls_pay_bout;
	uint8_t last_souls_tabs;
	uint8_t six_critical_souls_num;
};



#endif /* ALCHEMY_SRUCT_H_ */
