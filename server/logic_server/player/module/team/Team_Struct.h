/*
 * Team_Struct.h
 *
 *  Created on: 2014年4月15日
 *      Author: Linqiyou
 */

#ifndef TEAM_STRUCT_H_
#define TEAM_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"

struct Team_Sports_Ranking_Detail {
	Team_Sports_Ranking_Detail(void) {
		reset();
	}
	void reset(void) {
		role_id_ = 0;
		integrate_ = 0;
		name_ = "";
		win_times = 0;
		fail_times = 0;
		force = 0;
		rank = 0;
	}
	void serialize(Block_Buffer &buf) const;
	void deserialize(Block_Buffer &buf);

	friend bool compress(const Team_Sports_Ranking_Detail &detail1, const Team_Sports_Ranking_Detail &detail2);

	role_id_t role_id_;
	int32_t integrate_;
	int force;
	std::string name_;
	uint16_t win_times;
	uint16_t fail_times;
	int rank;
};
typedef boost::unordered_map<role_id_t, Team_Sports_Ranking_Detail> RoleId_TeamSportsRankingDetail_Map;
typedef std::vector<Team_Sports_Ranking_Detail*> Team_Sports_Ranking_Info;

struct Teamer_Detail : public Detail {
	enum Ready_Status {
		READY_START = 0,
		UNREADY = 0,
		READY = 1,
		READY_END
	};
	Teamer_Detail(void);
	virtual ~Teamer_Detail(void);

	void reset(void);
	int serialize(Block_Buffer &buf) const;
	int deserialize(Block_Buffer &buf);
	int load(void);
	int save(void);

	inline void save_tick(void) { is_change = true; }

	role_id_t role_id;
	int64_t team_id;
	bool ready;
	int profile_num_fb;							// 收益次数
	int profile_num_fb_pre;							// 额外优先使用收益次数
	int profile_num_sports;						// 组队竞技收益次数
	int profile_num_hero_dream;					// 英雄梦境收益次数
	int profile_num_defend;						// 守卫帝都
	Time_Value profile_time;
	bool is_not_first_team_fb; // 是否非第一次组队副本
	Int_Vec team_award;							// 组队竞技段位奖励

	int8_t wait_area_type;
	Team_Match_Info team_match_info;
};

struct First_Lvl_Detail {
	First_Lvl_Detail(void) { reset(); }
	void reset(void) {
		first_lvl_role_id_his.clear();
		first_lvl_ck_his.clear();
		first_lvl_zs_his.clear();
		first_lvl_yss_his.clear();
	}
	void serialize(Block_Buffer &w) const;
	void deserialize(Block_Buffer &r);
	Int_Uid first_lvl_role_id_his; // 本服务器第一个达到级的玩家
	Int_Uid first_lvl_ck_his; // 本服务器第一个达到级的刺客
	Int_Uid first_lvl_zs_his; // 本服务器第一个达到级的战士
	Int_Uid first_lvl_yss_his; // 本服务器第一个达到级的元素师
};
#endif /* TEAM_STRUCT_H_ */
