/*
 * Wing_Struct.h
 *
 *  Created on: 2014年10月13日
 *      Author: root
 */

#ifndef WING_STRUCT_H_
#define WING_STRUCT_H_

#include "Server_Struct.h"

struct Wing_Detail : public Detail {
	enum Wing_State {
		WING_NOT_ACTIVE = 0,
		WING_EXP	= 1,
		WING_ACTIVE = 2,
	};

	enum Wing_Show_State {
		SHOW_OFF	= 0,
		SHOW_ON		= 1,
	};

	Wing_Detail(void) { reset(); }
	virtual ~Wing_Detail(void);

	void reset(void);

	int load(void);
	int save(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	void set_wing_state(int state);
	inline bool get_wing_state(void) const;

	int state;
	int is_show;
	int lv;
	int rate;
	int progress;
	int times;
	int got_reward_stage;
	Time_Value active_time;
	Time_Value last_time;
};

inline bool Wing_Detail::get_wing_state(void) const {
	return is_show == Wing_Show_State::SHOW_ON;
}

#endif /* WING_STRUCT_H_ */
