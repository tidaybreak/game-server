/*
 * Knight_Trials_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef KNIGHT_TRIALS_STRUCT_H_
#define KNIGHT_TRIALS_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Detail.h"
#include "Fighter_Detail.h"
#include "heroer/Logic_Heroer_Struct.h"

struct Knight_Trials_Player {
	Knight_Trials_Player(void) { reset(); }
	void reset(void) {
		to_role_id = 0;
		uuid = 0;
		server_name.clear();

		base.reset();
		fighter.reset();
		heroer.reset();
	}

	role_id_t to_role_id;
	int64_t uuid;
	std::string server_name;

	Base_Detail base;
	Fighter_Detail fighter;
	Heroer_Detail heroer;
};


typedef std::vector<Knight_Trials_Player> Player_Fighter_Info_Vec;
typedef std::vector<Knight_Trials_Role_Info> Knight_Trials_Role_Info_Vec;

struct Record_Box_Reward {
	Record_Box_Reward(void) { reset(); }
	void reset(void) {
		id = 0;
		type = 0;
		num = 0;
	}
	uint32_t id;
	uint32_t type;
	uint32_t num;
};
typedef std::list<Record_Box_Reward> Record_Box_Reward_List;
typedef boost::unordered_map<uint8_t, Record_Box_Reward_List> Record_Box_Reward_Conf_Map;

struct Knight_Trials_Detail: public Detail {
	Knight_Trials_Detail(void);
	virtual ~Knight_Trials_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	uint8_t cur_points; //当前通关到第几关
	uint8_t cur_receive; //当前领取到了第几个
	Role_Id_Set role_id_set;//登陆初始化时候本服匹配出来的角色ID
	int64_t match_day;//记录当前是第几天耍人,因为第一天的话; 异步跨服或者异步数据库或者本服在线匹配返回的玩家要往当前关卡数组放人; 否则往隐藏的关卡放人
	//也同时用于第一天不能点刷新按钮的问题
	std::vector<Uint_Uint> reward; //累计领取的奖励(ID对应个数)
	bool reset_status;//今日是否重置
	Record_Box_Reward_Conf_Map recode_box_reward;

	//以下不存库;用于数据库异步交换数据的标识
	role_id_t to_role_id;
	int64_t to_uuid;
};

struct Knight_Trials_Player_Detail: public Detail {
	Knight_Trials_Player_Detail(void);
	virtual ~Knight_Trials_Player_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	Knight_Trials_Role_Info_Vec cur_player_fighter_vec;
	Knight_Trials_Role_Info_Vec hide_player_fighter_vec;
//	Player_Fighter_Info_Vec cur_player_fighter_vec;
//	Player_Fighter_Info_Vec hide_player_fighter_vec;

};

#endif /* KNIGHT_TRIALS_STRUCT_H_ */
