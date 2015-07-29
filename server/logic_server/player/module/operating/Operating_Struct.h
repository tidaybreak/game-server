/*
 * Operating_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef OPERATING_STRUCT_H_
#define OPERATING_STRUCT_H_

#include "Server_Struct.h"
#include "mail/Mail_Struct.h"
#include "gang/Gang_Struct.h"
#include "arena/Arena_Struct.h"
#include "dragon/Dragon_Vale_Def.h"

struct Operating_Recharge {
	Operating_Recharge(void) { reset(); }
	void reset(void) {
		gold = 0;
		time = 0;
		order_id.clear();
	}
	std::string order_id;
	uint32_t gold;
	int64_t time;
};

typedef std::vector<Operating_Recharge> Operating_Recharge_Vec;

struct Operating_Detail: public Detail {
	Operating_Detail(void);
	virtual ~Operating_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	typedef std::vector<Mail_Send_Info> Offline_Mail;
	typedef std::vector<Ganger_Detail> Offline_Ganger_Info;
	typedef std::vector<Arena_Reward_Info> Offline_Arena_Reward;
	typedef std::vector<Flowers_Info> Offline_Flowers_Vec;

	Offline_Mail mail_list;
	Operating_Recharge_Vec recharge_gold;
	Role_Id_Time_Map invite_list;
	RoleID_Int8_Map apply_list;
	Offline_Ganger_Info ganger_info;
	Offline_Arena_Reward arena_three_day_reward;
	Offline_Arena_Reward arena_seven_day_reward;
	Offline_Flowers_Vec receive_flowers_list;
	Building_Notice_Map building_notice_map;
	Rob_Fight_Result_Map rob_fight_result_map;
	Gain_Rob_Prodcutin_Set gain_rob_production_set;
	bool is_dragon_vale_special;
	int64_t inv_time;
	uint8_t like_success;
	uint32_t wbrank;
	uint32_t damage;
	uint32_t damage_rate;
	Int_Vec team_award;
//	std::vector<role_id_t> mail_list;
};

#endif /* OPERATING_STRUCT_H_ */
