/*
 * Exchange.h
 *
 *  Created on: 2014年6月25日
 *      Author: Linqiyou
 */

#ifndef EXCHANGE_H_
#define EXCHANGE_H_

#include "Logic_Player_Base.h"

class Packer;
struct Exchange_Detail;

class Exchange : virtual public Packer {
public:
	Exchange(void);
	virtual ~Exchange(void);
	void reset(void);

	static int create_init(Exchange_Detail &detail);

	void load_detail(Exchange_Detail &detail);

	void save_detail(Exchange_Detail &detail);

	Exchange_Detail const &exchange_detail(void) const;

	void module_init(void);

	void sign_in(void);

	void sign_in_repeat(void);

	void client_ready_now(void);
	//	每天0点触发
	int trigger_daily_zero(const Time_Value &now, bool logic_check = false);
	// 每天6点触发
	int trigger_daily_six(const Time_Value& now, bool logic_check = false);

	void sign_out(void);

	int move_scene_completed(void);

	int sync_scene_module_info(Block_Buffer &buf);

	void trusteeship_sign_out(void);

	// msg
	int req_exchange(const int32_t &item_id, const int16_t &type, const int32_t amount);
	int req_fetch_exchange_info(const int16_t type);
	int req_exchange_skill_item(int item_id);
	// msg

private:
	Exchange_Detail *exchange_detail_;
};

inline Exchange_Detail const &Exchange::exchange_detail(void) const {
	return *exchange_detail_;
}

#endif /* EXCHANGE_H_ */
