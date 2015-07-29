/*
 * Alchemy.h
 *
 *  Created on: Mar 7, 2014
 *      Author: XIAOLIANG
 */

#ifndef ALCHEMY_H_
#define ALCHEMY_H_

#include "Logic_Player_Base.h"

class Logic_Player;
struct Alchemy_Detail;

class Alchemy : virtual public Packer {
public:
	Alchemy(void);
	virtual ~Alchemy();
	void reset(void);

	// 登录时detail赋值
	void load_detail(Alchemy_Detail &detail);
	// 保存时detail赋值
	void save_detail(Alchemy_Detail &data);
	// 登录时module初始化
	void module_init(void);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);
	void trigger_daily_six(const Time_Value &now);

	void refresh_cards(void);
	int req_alchemy_info(uint8_t mtype = 0);
	int req_alchemy(uint8_t tabs, uint8_t pay_type);
	int req_souls_alchemy(uint8_t tabs, uint8_t pay_type);

	Alchemy_Detail const &alchemy_detail(void) const;

	int get_alchemy_chances_free(void);//得到炼金总免费次数
	int get_over_alchemy_free_about(void);//得到炼金剩余免费次数
	int get_over_alchemy_pay_about(void);//得到炼金剩余付费次数

	int get_alchemy_souls_chances_free(void);//得到炼金总免费次数
	int get_over_alchemy_souls_free_about(void);//得到炼金剩余免费次数
	int get_over_alchemy_souls_pay_about(void);//得到炼金剩余付费次数


private:
	Alchemy_Detail *alchemy_detail_;
};

inline Alchemy_Detail const &Alchemy::alchemy_detail(void) const{
	return *alchemy_detail_;
}


#endif /* ALCHEMY_H_ */
