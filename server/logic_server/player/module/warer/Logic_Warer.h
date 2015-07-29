/*
 * Logic_Warer.h
 *
 *  Created on: 2014年7月8日
 *      Author: root
 */

#ifndef LOGIC_WARER_H_
#define LOGIC_WARER_H_

#include "Logic_Player_Base.h"
struct Warer_Detail;

class Logic_Warer: virtual public Logic_Player_Base{
public:
	Logic_Warer(void);
	virtual ~Logic_Warer(void);
public:
	void reset(void);

	void load_detail(Warer_Detail &detail);
	void save_detail(Warer_Detail &detail);

	inline Warer_Detail* warer_detail(void);

	void daily_refresh(void);
	// 凌晨0点刷新
	int trigger_daily_zero(const Time_Value &now);
	// 凌晨6点刷新
	int trigger_daily_six(const Time_Value &now);

	void module_init(void);
	static int create_init(Warer_Detail& detail);
	void sign_in(void);
	void sign_out(void);
protected:
	Warer_Detail* warer_detail_;
};



#endif /* LOGIC_WARER_H_ */
