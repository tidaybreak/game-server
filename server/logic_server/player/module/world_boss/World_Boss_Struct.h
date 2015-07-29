/*
 * World_Boss_Struct.h
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */

#ifndef WORLD_BOSS_STRUCT_H_
#define WORLD_BOSS_STRUCT_H_

#include "Server_Struct.h"

struct World_Boss_Detail : public Detail {
	World_Boss_Detail(void);
	virtual ~World_Boss_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	int64_t act_id;
	int8_t act_state;
	int open_num;			// 开放次数
	int kill_num;			// 击杀次数
	int boss_lv;			// boss等级
};



#endif /* WORLD_BOSS_STRUCT_H_ */
