/*
 * Map_Map_Team.h
 *
 *  Created on: 2014年6月19日
 *      Author: Linqiyou
 */

#ifndef MAP_TEAM_H_
#define MAP_TEAM_H_


#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "NPC/NPC_Struct.h"
#include "pack/Pack_Struct.h"

class Scene_Player;
//typedef void (*Map_Team_Member_Handle_Func)(const int, const int, Scene_Player &player);
struct Team_Robot_Config;


class Map_Team {
public:
	typedef boost::unordered_map<role_id_t, Online_Teamer_Info> RoleID_OnlineMap_TeamerInfo_Map;
	typedef boost::unordered_map<role_id_t, Time_Value> RoleID_TimeValue_Map;
	typedef std::vector<Reward_Info> Reward_Info_Vec;
	typedef boost::unordered_map<role_id_t, Reward_Info_Vec> Role_RewardInfoVec_Map;
	enum {
		DISMISS_TIMEOUT_SEC = 120
	};

	enum {
		TEAM_TYPE_START = 0,
		TEAM_TYPE_FB = 0,		// 组队副本
		TEAM_TYPE_SPORTS = 1,	// 组队竞技
		TEAM_TYPE_HERO_DREAM = 2,	// 组队梦境
		TEAM_TYPE_TIMELIMIT = 3,		// 限时击杀副本
		TEAM_TYPE_END
	};

	enum Map_Team_Sports_Status {
		TEAM_SPORTS_NORMAL = 0,
		TEAM_SPORTS_SIGNED = 1,
		TEAM_SPORTS_MATCHED	= 2,
//		TEAM_SPORTS_FIGHT	= 3,
	};


	static const int FAIL_TIMES = 3;// 连败超过3次，则往下一个段
	enum Position_Type {
		POSITION_1 = 0, // 所在行数
		POSITION_2 = 1,
		POSITION_3 = 3,

		POSITION_START = 0, // 所在位置
		POSITION_FRONT = 0,
		POSITION_BEHIND = 1,
		POSITION_END = 1,
	};

	// 组队副本：翻牌索引
	enum Open_Card_Index {
		OPEN_CARD_INDEX_START = 0,
		OPEN_CARD_INDEX_END = 4,
	};
	static const int OPEN_CARD_TIME = 7;// s
	static const int MAX_DROPS = 5;

	enum Sync_Operate{
		SYNC_ADD = 0,
		SYNC_DEL = 1,
	};

	Map_Team(void);
	~Map_Team(void);

	void reset(void);

	bool is_team_all_offline(void);
	int set_team_all_offline(void);									// 若一定时间内无小队成员在线，解散队伍
	int set_team_not_all_offline(void);								// 队员上线时
	inline bool is_team_offline_over_time(const Time_Value &now);

	inline bool is_in_team(role_id_t role_id) const;
	inline bool is_leader(role_id_t role_id) const;
	inline bool is_team_member(role_id_t role_id) const;
	inline bool is_in_team_onlinelist(role_id_t role_id) const;
	inline bool is_in_team_offlinelist(role_id_t role_id) const;

	// 组队竞技
	int begin_match(void); // 开始匹配
	int cancel_match(void); // 取消匹配
	Map_Team_Sports_Status &sports_status(void); // 匹配状态
	int team_fb_create_player_monster(Scene_Player &player);

	//-- 组队布阵
	int lineup_replace(const Int_Role_Id_Vec &lineup); // 布阵处理
	int lineup_clear(Scene_Player &player); // 人员离开时处理
	bool lineup_check(const Int_Role_Id_Vec &lineup); // 布阵内容检查
	bool is_role_in_lineup(role_id_t role_id); // 人物是否在布阵阵型中
	bool is_lineup_empty(void); // 布阵信息是否为空
	Int_Role_Id *get_lineup(role_id_t role_id); // 按角色取其布阵信息
	int get_position(const role_id_t &role_id);
	int lineup_refresh(void);// 修正去除已离线玩家
	int lineup_active(void);
	//-- 组队布阵

	inline int get_online_team_member_nums(void) const;
	inline int get_offline_team_member_nums(void) const;
	inline int get_all_team_member_nums(void) const;
	inline int get_index_nums(void) const; // 未关闭的索引位
	inline int get_position_max(void) const; // 总的人数
	inline int get_unused_position(void) const; // 剩余位数

	inline int64_t get_team_id(void) const;

	inline void set_team_index(uint32_t team_index);
	inline uint32_t get_team_index(void) const;

	Scene_Player *team_leader(void) const;

	inline Role_Hash_Set &team_member_set(void);
	inline void team_all_member_set(Role_Hash_Set &role_set) const;

	inline Scene_Player *get_team_leader(void);
	bool is_leader_online(void) const;

	int check_all_member_ready(void); /// 检测所有队员是否准备

	int get_team_score(void);

	int &scene_id(void);
	std::string &password(void);
	int8_t &type(void);
	std::string &words(void);
	int8_t &degree(void);
	Int8_Vec &index_list(void);
	int8_t &in_battle(void);
	Time_Value &last_act_time(void);
	const role_id_t &leader_role_id(void);
	int set_leader_role_id(const role_id_t &role_id);
	const int &enter_level(void) const;
	int set_enter_level(const int lvl);
	bool is_not_first_team_fb(void);

	void info_all_online_player(int msg_id, Block_Buffer* buf); // 将buf广播给logic所有在线
	RoleID_TimeValue_Map &offline_time_map(void);
	RoleID_OnlineMap_TeamerInfo_Map &offline_teamer_map(void);
	Int_Role_Id_Vec &lineup(void);

	/***************************************************************************
	 * 组队竞技
	 */
	const int32_t &point(void) const;
	int add_point(const int point);
	int16_t &fail_times(void);
	int16_t &match_fail(void);
	int32_t &sports_battle_level(void);
	//**************************************************************************

	void serialize(Block_Buffer &buf); // 传需要的数据到聊天服
	void deserialize(Block_Buffer &buf); // 传需要的数据到聊天服
	friend bool compare_less(const Map_Team &team1, const Map_Team &team2);
	int sync_info_logic(void); //
private:
	void info_leader(int msg_id, Block_Buffer* buf);
	void info_member(int msg_id, Block_Buffer* buf);
	void info_member_without_self(int msg_id, int result, Block_Buffer* buf, role_id_t role_id);
	void create_team_player_detail(const Team_Robot_Config &tr_cfg, Block_Buffer &buf);
	void create_team_hero_detail(const Team_Robot_Config &tr_cfg, Block_Buffer &buf);

private:
	int8_t type_;								// 队伍类型
	std::string password_;						// 房间密码
	std::string words_;							// 组队备注
	int scene_id_;								// 副本id
	int8_t degree_;								// 副本难度（弃用，用不同的副本id）
	Int8_Vec index_list_;						// 关闭的位置索引
	bool is_all_offline_;						// 是否无人在线
	Time_Value all_offline_time_;				// 无人在线的时间点
	uint32_t team_index_; 						// 小队位置索引index为0，表示进入副本
	int64_t team_id_;							// 小队id
	int64_t team_id_for_scene_;
	role_id_t leader_role_id_;					// 队长role_id
	Role_Hash_Set team_member_set_;				// 队员role_id
	RoleID_OnlineMap_TeamerInfo_Map offline_teamer_map_;	// 不在线队员
	RoleID_TimeValue_Map offline_time_map_;		// 不在线对应时间
	Int_Role_Id_Vec lineup_; 					// 布阵信息
	int8_t in_battle_;	// 队伍开始战斗
	Time_Value last_act_time_; // 最后操作时间,用于未操作时间计算：超过规定时间，解除队伍
	bool is_not_first_team_fb_;

	int32_t enter_level_;// 要用请先找相关人员(组队副本时：副本进入等级；组队竞技时：战队等级)

	// 多人竞技
	Map_Team_Sports_Status sports_status_;
	int32_t point_;
	int16_t fail_times_; // 连败次数
	int16_t match_fail_; // 轮空次数
	int32_t sports_battle_level_; // 本次战斗的所在等级段
};

struct Map_Team_PtrGreater {
	bool operator()(const Map_Team *team1, const Map_Team *team2) const;
};
typedef std::map<int64_t, Map_Team*> Int64_Map_Team_Map;
typedef std::map<int64_t, Map_Team> UInt_Map_Team_Map_Detail; // 聊天服、场景服记录数据
typedef std::vector<Map_Team*> Map_Team_Vec;
typedef std::set<Map_Team*, Map_Team_PtrGreater> Map_Team_RankingSet;


inline bool Map_Team::is_in_team(role_id_t role_id) const {
	return (is_in_team_onlinelist(role_id) || is_in_team_offlinelist(role_id))? true : false;
}

inline bool Map_Team::is_in_team_onlinelist(role_id_t role_id) const {
	return (is_leader(role_id) || is_team_member(role_id))? true : false;
}

inline bool Map_Team::is_in_team_offlinelist(role_id_t role_id) const {
	return (offline_teamer_map_.count(role_id) == 0)? false : true;
}

inline bool Map_Team::is_leader(role_id_t role_id) const {
	return (role_id == leader_role_id_)? true : false;
}

inline bool Map_Team::is_team_member(role_id_t role_id) const {
	return (team_member_set_.count(role_id) == 0)? false : true;
}

//inline void Map_Team::set_team_id(uint32_t team_id) {
//	team_id_ = team_id;
//}

inline int64_t Map_Team::get_team_id(void) const {
	return team_id_;
}

inline void Map_Team::set_team_index(uint32_t team_index) {
	team_index_ = team_index;
}

inline uint32_t Map_Team::get_team_index(void) const {
	return team_index_;
}

inline Role_Hash_Set& Map_Team::team_member_set(void) {
	return team_member_set_;
}

inline void Map_Team::team_all_member_set(Role_Hash_Set &role_set) const {
	role_set.clear();
	for (Role_Hash_Set::const_iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		role_set.insert(*it);
	}
	role_set.insert(leader_role_id_);
	return ;
}

inline int Map_Team::get_online_team_member_nums(void) const {
	int num = team_member_set_.size();
	if (this->is_leader_online()) {
		++num;
	}
	return num;
}

inline int Map_Team::get_offline_team_member_nums(void) const {
	return offline_teamer_map_.size();
}

inline int Map_Team::get_all_team_member_nums(void) const {
	return get_online_team_member_nums() + get_offline_team_member_nums();
}

inline int Map_Team::get_index_nums(void) const {
	int nums = 0;
	for (Int8_Vec::const_iterator it = this->index_list_.begin(); it != this->index_list_.end(); ++it) {
		if (*it) {
			++nums;
			continue;
		}
	}
	return nums;
}

inline int Map_Team::get_position_max(void) const {
	return this->get_index_nums() + 1;
}

inline int Map_Team::get_unused_position(void) const {
	return get_position_max() - get_all_team_member_nums();
}

inline bool Map_Team::is_team_offline_over_time(const Time_Value &now) {
	if (is_all_offline_ == false) {
		return false;
	} else {
		return (now.sec() - all_offline_time_.sec()) > DISMISS_TIMEOUT_SEC;
	}
}


#endif /* MAP_TEAM_H_ */
