/*
 * Base_Detail.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef BASE_DETAIL_H_
#define BASE_DETAIL_H_

#include "Base_Struct.h"
#include "Server_Struct.h"

struct Skill_Config;

struct Base_Detail: public Detail {
	/// 属性类型
	enum Property_Type {
		ROLE_ID 		= 500001,	/// 角色ID
		ROLE_NAME 		= 500101,	/// 角色名
		CAREER 			= 500201,	/// 职业
		GENDER 			= 500301,	/// 性别
		GUILD 			= 500401,	/// 公会ID
		GENERALS 		= 500501,	/// 武将ID
		//GENDER 			= 500601,  	/// 出战武将ID
	};

	Base_Detail(void);
	virtual ~Base_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	// 竞技场加载(需要加载avatar)
	int arena_load(void);

	Account_Key account_info;
	Location location;
	Create_Scene_Args create_scene_args_;
	std::string role_name;					// 角色名
	int level;     			   				// 等级[由战斗模块初始化]
	int vip;// vip等级 0-8 , 注意,因为VIP过期;等级不会清零,如需要获取正式的VIP数据,请调用 player_self()->vip(); 返回0为未开通或已过期, 返回非零为实际VIP等级
	uint8_t gender; 						// 性别2 = 女(female), 1 = 男(male)
	uint8_t career; 						// 职业 1-4	([1,4])
	Time_Value create_time;					// 创建角色时刻
	Time_Value last_sign_in_time;			// 最后登录时间
	Time_Value last_sign_out_time;			// 最后登出时间
	Time_Value last_trigger_daily_zero;		// 上次每日0点刷新时间
	Time_Value last_trigger_daily_six;		// 上次每日6点刷新时间
	Time_Value last_trigger_weekly_zero;	// 上次每周0点刷新时间
	gang_id_t gang_id;						///	帮派Id[由帮派模块初始化]
	std::string gang_name;  				///	帮派名称[由帮派模块初始化]
	int8_t online;							/// 在线状态[0:不在线 1:在线]
	int32_t title_id;						// 装备的称号id
	uint32_t head_id;						//头像ID
	uint32_t charm; 						//魅力值

	Int_Vec avatar_vec;						//时装

	Time_Value vip_over_time;			/// vip到期时间
	std::string ip;					/// IP
	uint8_t ride;						//
	int mount_model_id;				//
	int32_t rela_scene_id;			//
	int awake_lvl;					   // 觉醒等级

	uint8_t role_type;//玩家类型: 0.普通玩家 1.GM
};

#endif /* BASE_DETAIL_H_ */
