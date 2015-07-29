/*
 * Team_Arena_Struct.h
 *
 *  Created on: 2014年6月18日
 *      Author: Linqiyou
 */

#ifndef TEAM_ARENA_STRUCT_H_
#define TEAM_ARENA_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"

//struct Arena_Team_Common_Detail {
//	typedef boost::unordered_map<role_id_t, Role_Team_Info> RoleID_RoleTeamInfo_Map;
//	Arena_Team_Common_Detail(void) {
//		reset();
//	}
//
//	void reset(void) {
//
//	}
//
//	uint32_t team_id;
//	uint32_t create_time;		// 创建时间
//	Role_Team_Info leader;	// 队长
//	RoleID_RoleTeamInfo_Map member_map;	// 队员
//	uint32_t total_match;		// 总场数
//	uint32_t win_match;			// 胜场
//	uint32_t draw_match;		// 平场
//	uint32_t win_streak;		// 连胜场次
//	int point;				// 积分
//
//	int inter_server_fail_times;	//
//	int inter_server_win_times;		//
//
//	Int_Role_Id_Vec lineup_; 					// 布阵信息
//	int8_t in_battle_;	// 队伍开始战斗
//	int32_t enter_level_;// 要用请先找相关人员(组队副本时：副本进入等级；组队竞技时：战队等级)
//};
//
////struct Arena_Team_Common_PtrGreater {
////	bool operator()(const Arena_Team_Common_Detail *team1, const Arena_Team_Common_Detail *team2) const;
////};
//
//struct Arena_Team_Detail {
//	Arena_Team_Detail(void) {
//		reset();
//	}
//
//	void reset(void) {
//
//	}
//
//	Arena_Team_Common_Detail common_detail;
//	int status;					// 战队当前状态
//};

//typedef boost::unordered_map<int64_t, Arena_Team_Common_Detail*> Arena_Team_Common_Map;
////typedef std::set<Arena_Team_Common_Detail*, Arena_Team_Common_PtrGreater> Arena_Team_RankingSet;
//typedef std::vector<Arena_Team_Common_Detail*> Arena_Team_Common_Vec;
typedef std::pair<int64_t, int64_t> Matched_Team_Pair;
struct Matched_Teams {
	Matched_Teams(void) {
		reset();
	}

	void reset(void) {
		match_time = Time_Value::zero;
		matched_pair.first = 0;
		matched_pair.second = 0;
	}

	Time_Value match_time;
	Matched_Team_Pair matched_pair;
};

typedef std::deque<Matched_Teams> MatchedTeam_Deque;

struct Uint_Greater {
	bool operator()(const uint32_t &ui1, const uint32_t &ui2) {
		return ui1 < ui2;
	}
};
typedef std::map<uint32_t, Role_Vec, Uint_Greater> UInt_UIntVec_Map_Reverse;

struct Team_Role_Score {
	Team_Role_Score(void) { reset(); };
	void reset(void) {
		role_id = 0;
		name = "";
		area = "";
		score = 0;
		force = 0;
		change = true;
		rank = 0;
	}
	role_id_t role_id;
	std::string name;
	std::string area;
	int score;
	int force;

	bool change;
	int rank;
};

struct Team_Role_Score_Greater {
	bool operator()(const Team_Role_Score *team1, const Team_Role_Score *team2) {
		if (team1->score == team2->score)
			return team1->force > team2->force;
		return team1->score > team2->score;
	}
};

typedef boost::unordered_map<role_id_t, Team_Role_Score> Team_Role_Score_Map;
typedef std::vector<Team_Role_Score*> Team_Role_Score_Vec;
typedef boost::unordered_map<int, Team_Role_Score_Vec> Level_Team_Role_Score_Map;

struct Team_Id_Score {
	Team_Id_Score(void) : team_id(0), score(0), match_fail(0) {};
	role_id_t team_id;
	int score;
	int match_fail;
};
typedef std::vector<Team_Id_Score> Team_Id_Score_Vec;

struct Team_Score_Greater {
	bool operator()(const Team_Id_Score &team1, const Team_Id_Score &team2) {
		if (team1.score == team2.score) {
			return team1.match_fail > team2.match_fail;
		}
		return team1.score > team2.score;
	}
};

struct Team_Score_Lesser {
	bool operator()(const Team_Id_Score &team1, const Team_Id_Score &team2) {
		if (team1.score == team2.score) {
			return team1.match_fail > team2.match_fail;
		}
		return team1.score < team2.score;
	}
};

#endif /* TEAM_ARENA_STRUCT_H_ */
