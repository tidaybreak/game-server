/*
 * Team_Arena_Controler.h
 *
 *  Created on: 2014年6月18日
 *      Author: Linqiyou
 */

#ifndef TEAM_ARENA_CONTROLER_H_
#define TEAM_ARENA_CONTROLER_H_

#include "Map_Team.h"
#include "Team_Arena_Struct.h"
#include "Module_Manager.h"

class Team_Arena_Controler : public Module_Manager {
public:
	static const int MATCH_GAP = 10;// second
	static const int ENTER_WAIT = 3;
	enum {
		TEAM_SPORTS_ACTIVITY_CLOSE 	= 0,
		TEAM_SPORTS_ACTIVITY_OPEN 	= 1,

		TEAM_MATCH_FAIL_TIMES = 3,
	};
	Team_Arena_Controler(void);
	~Team_Arena_Controler(void);
	virtual void tick(Time_Value &now);
	int open_tick(void);

	int transfer_handle(Map_Team &team1, Map_Team &team2);

	int add_wait_match_team(int64_t team_id);
	int del_wait_match_team(int64_t team_id);

	// 多人竞技
	int open_sports_activity(void); // 开始匹配
	void update_role_score(Block_Buffer &buf); // 积分数据
	int close_sports_activity(void); // 结束匹配
	bool is_sports_open(void);
	bool &team_sports_test(void);

	int get_role_score(role_id_t role_id);

	int get_role_score_list(role_id_t role_id, int level, Block_Buffer &buf);
	int refresh_rank_list(role_id_t role_id = 0);

private:
	int match_arena_team(void); // 处理匹配
	int matched_handle(int64_t team_id1, int64_t team_id2); // 记录、广播匹配的两队信息
	int dispatch(void); // 按等级分段处理
	int merge_dispatch(void); // 向下合并少于3队的段数、向上合并最后一段少于3队的段数

	uint32_t already_match_time_;		// 本次活动已赛场次
	Int64_Map_Team_Map team_map_; // 已准备好进行匹配的队伍
//	Team_RankingSet team_set_;
	MatchedTeam_Deque team_deque_;
	UInt_Set team_matched_;
	UInt_UIntVec_Map_Reverse level_team_ids_map_; // 按等级段区分的队伍id
	int32_t team_amount_; // 按等级区分后的队伍总数
	Time_Value last_match_time_;
	Time_Value last_rank_refresh_time_;


	int8_t sports_activity_switch_;		// 多人竞技开关
	bool team_sports_test_;
	Team_Role_Score_Map team_role_score_map;
	Level_Team_Role_Score_Map level_team_role_score_map;
};

typedef Singleton<Team_Arena_Controler> Team_Arena_Controler_Single;
#define TEAM_ARENA_CONTROLER Team_Arena_Controler_Single::instance()


#endif /* TEAM_ARENA_CONTROLER_H_ */
