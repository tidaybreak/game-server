/*
 * Team.h
 *
 *  Created on: 2014年4月16日
 *      Author: Linqiyou
 */

#ifndef TEAM_H_
#define TEAM_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "NPC/NPC_Struct.h"
#include "pack/Pack_Struct.h"

class Logic_Player;
//typedef void (*Team_Member_Handle_Func)(const int, const int, Logic_Player &player);


class Team {
public:
	typedef boost::unordered_map<role_id_t, Online_Teamer_Info> RoleID_OnlineTeamerInfo_Map;
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
		TEAM_TYPE_DEFEND = 4,
		TEAM_TYPE_END
	};

	enum Team_Sports_Status {
		TEAM_SPORTS_NORMAL = 0,
		TEAM_SPORTS_SIGNED = 1,
		TEAM_SPORTS_MATCHED	= 2,
//		TEAM_SPORTS_FIGHT	= 3,
	};

//	enum Team_Status_Change_Type {
//		ONLINE,
//		OFFLINE,
//		JOIN,
//		LEAVE,
//		DISMISS,
//		LEADER_CHANGE,
//	};

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
	static const int OPEN_SHOW_TIME = 7;// s
	static const int OPEN_CARD_TIME = 14;// s
	static const int MAX_DROPS = 5;

	enum Sync_Operate{
		SYNC_ADD = 0,
		SYNC_DEL = 1,
	};

	enum {
		BATTLE_OVER_FLAG_SET = 0,
		BATTLE_OVER_FLAG_UNSET = 1,

		NOT_TIMELIMIT = 0,
		TIMELIMIT = 1,
	};

	Team(void);
	~Team(void);

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

	int player_online(Logic_Player &player);
	int player_offline(Logic_Player &player);
	/*
	 * is_timelimit
	 * 限时副本进副本时，要创建队伍.
	 * 已进入副本、无需转场景
	 * */
	int player_join(Logic_Player &player, const bool is_auto_profile_set = true, const bool is_timelimit = false);
	int player_leave(role_id_t role_id, int8_t is_kick = 0);

	// 按类型分别处理开始按钮
	int start(void);
	// 组队副本
	int enter_fb(void);
	// 组队竞技
	int begin_match(void); // 开始匹配
	int cancel_match(void); // 取消匹配
	Team_Sports_Status &sports_status(void); // 匹配状态

	//-- 组队布阵
	int get_out_index(Logic_Player &player); // 取英雄的位置
	int lineup_replace(Logic_Player &player); // 人员加入时处理
	int lineup_replace(const Int_Role_Id_Vec &lineup); // 布阵处理
	int lineup_clear(Logic_Player &player); // 人员离开时处理
	bool lineup_check(const Int_Role_Id_Vec &lineup); // 布阵内容检查
	bool is_role_in_lineup(role_id_t role_id); // 人物是否在布阵阵型中
	bool is_lineup_empty(void); // 布阵信息是否为空
	Int_Role_Id *get_lineup(role_id_t role_id); // 按角色取其布阵信息
	int get_position(const role_id_t &role_id);
	int lineup_refresh(void);// 修正去除已离线玩家
	int lineup_active(void);
	int lineup_active(const role_id_t &role_id);
	//-- 组队布阵

	int player_lead(Logic_Player &player, const bool is_leader_leave = false); // 改变队长
	int player_lead(role_id_t role_id); // 改变队长（所有人离线时）

	int team_dismiss(void); // 解散队伍
	int team_dismiss_without_leader(void); // 解散队伍不处理队长

	void refresh_teammer_info(void); // 给所有队员广播房间信息
	void refresh_teammer_info(Logic_Player &player); // 给队员广播房间信息

	inline int get_online_team_member_nums(void) const;
	inline int get_offline_team_member_nums(void) const;
	inline int get_all_team_member_nums(void) const;
	inline int get_index_nums(void) const; // 未关闭的索引位
	inline int get_position_max(void) const; // 总的人数
	inline int get_unused_position(void) const; // 剩余位数

	inline void set_team_id(int64_t team_id);
	inline int64_t get_team_id(void) const;

	inline void set_team_index(uint32_t team_index);
	inline uint32_t get_team_index(void) const;

	Logic_Player *team_leader(void) const;

	inline Role_Hash_Set &team_member_set(void);
	inline void team_all_member_set(Role_Hash_Set &role_set) const;

	void info_all_team_info(Logic_Player& player);
	void info_all_team_info(void);

	Logic_Player *get_team_leader(void);
	bool is_leader_online(void) const;

	int check_all_member_ready(void); /// 检测所有队员是否准备
	int unready_all(void); // 取消所有人准备状态
	int unready_leader(void); // 取消所有人准备状态

//	int sub_online_profile_num(void); // 减少离线队员收益次数

	int &scene_id(void);
	int8_t &scene_degree(void);
	std::string &password(void);
	int8_t &type(void);
	std::string &words(void);
	int8_t &degree(void);
	Int8_Vec &index_list(void);
	int8_t &in_battle(void);
	Time_Value &last_act_time(void);
	const role_id_t &leader_role_id(void);
	const int &enter_level(void) const;
	int set_enter_level(const int lvl);
	int64_t get_team_id_for_scene(void);
	int set_team_id_for_scene(const int64_t id);

	void info_all_online_player(int msg_id, Block_Buffer* buf); // 将buf广播给所有在线
	void info_all_online_player_without_self(int msg_id, Block_Buffer* buf, role_id_t role_id); // 将buf广播给所有在线(除了自己)
	void get_team_basic_info(Team_Basic_Info &team_info); // 取队伍基本信息
	RoleID_TimeValue_Map &offline_time_map(void);
	RoleID_OnlineTeamerInfo_Map &offline_teamer_map(void);
	Int_Role_Id_Vec &lineup(void);

	/***************************************************************************
	 * 多人副本处理
	 */
	bool &has_notice_card(void);
	Time_Value &start_open_card_time(void);
	Int_Uid &open_card_list(void);
	boost::unordered_map<int, Drop_Vec> &drops_all_vec(void);
	Role_RewardInfoVec_Map &role_reward_map(void);
	int req_open_card(int index, Logic_Player &player); // 请求翻牌
	int team_card_unused_index(void); // 卡牌索引
	int team_card_reward_vec(int index, Reward_Info_Vec &reward_list, role_id_t role_id); //从掉落列表中取一组
	int team_card_reward_msg(const int &index, const role_id_t &role_id, std::vector<Reward_Info_List> &list, bool from_client = false); // 管理翻牌索引(获得的物品传入消息列表)
	int team_card_reward_from_drop(int index, const role_id_t &role_id,
			Reward_Info_Vec &reward_list, Item_Vec &pack_item_vec); // 从产生的掉落物中取出一组
	int team_card_handle_drops(void); // 产生掉落物品列表（5个）
//	int team_card_process_drop_mode(int8_t player_career, Drop_Vec &drops_all, const Drop_Config* drop_config,
//			double mult_coe/* = 1.0*/, double lv_coe /*= 1.0*/); // 按概率为玩家产生掉落物品
//	int team_card_process_drop_box_mode(int8_t player_career, Drop_Vec &drops_all, const Drop_Box_Config& drop_box_config,
//			double mult_coe /*= 1.0*/, double lv_coe /*= 1.0*/); // 按概率为玩家产生掉落物品(宝箱)
//	int team_card_drop_success_index(const std::vector<double>& rate_vec); // 从掉落物的箱子概率列表中取得随机的一个索引位
	Role_Vec &team_fb_role_id_non_strength(void); // 无体力的而进了副本的玩家
	bool &is_not_first_team_fb(void);
	Role_Id_Set &has_sub_profile(void);
	//**************************************************************************

	/***************************************************************************
	 * 组队竞技
	 */
	int set_sports_level(bool is_strict = true);

	const int32_t &point(void) const;
	int add_point(const int point);
	int16_t &fail_times(void);
	int16_t &match_fail(void);
	Time_Value &match_fail_time(void);
	bool &battle_over_flag(void);
	//**************************************************************************

	/***************************************************************************
	 * 英雄梦境
	 */
	int team_hero_dream_award(void); // 结算奖励
	//**************************************************************************

	// team_defend
	RoleID_Double_Map &hurt_map(void);
	RoleID_Double_Map &be_hurt_map(void);
	RoleID_Double_Map &cure_map(void);

	bool has_profile_set(role_id_t role_id);
	int profile_set_add(role_id_t role_id);
	int profile_set_del(role_id_t role_id);
	bool has_profile_nums(const int8_t type = -1);

	int add_monster(const std::string &monster);
	int del_monster(void);
	std::string get_monster(void);

	void sync_team_info_to_chat(int8_t operate);
	void sync_team_info_to_scene(int8_t operate);
	void serialize(Block_Buffer &buf); // 传需要的数据到聊天服
	void deserialize(Block_Buffer &buf); // 传需要的数据到聊天服
	void set_last_act_time_now(void);
	void set_last_act_time_zero(void);
	friend bool compare_less(const Team &team1, const Team &team2);
private:
	void info_leader(int msg_id, Block_Buffer* buf);
	void info_member(int msg_id, Block_Buffer* buf);
	void info_member_without_self(int msg_id, Block_Buffer* buf, role_id_t role_id);

//	void team_status_change(int type);

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
	role_id_t leader_role_id_;					// 队长role_id
	Role_Hash_Set team_member_set_;				// 队员role_id
	RoleID_OnlineTeamerInfo_Map offline_teamer_map_;	// 不在线队员
	RoleID_TimeValue_Map offline_time_map_;		// 不在线对应时间
	Int_Role_Id_Vec lineup_; 					// 布阵信息
	int8_t in_battle_;	// 队伍开始战斗
	Time_Value last_act_time_; // 最后操作时间,用于未操作时间计算：超过规定时间，解除队伍

	int64_t team_id_for_scene_; // use for transmit

	// 多人副本
	bool has_notice_card_;
	Time_Value start_open_card_time_;// 开始翻牌时间,用于之后系统翻牌
	Int_Uid open_card_list_; // 位置,人物
	Role_RewardInfoVec_Map role_reward_map_; // 人物,奖励
	boost::unordered_map<int, Drop_Vec> drops_all_vec_; // 位置,奖励
	Role_Vec role_id_non_strength_; // 副本通关后、无体力玩家:已弃用
	Int_Int_Set_Map used_box_;// 副本通关后，已翻过的箱子
	bool is_not_first_team_fb_; // 是否非第一次组队副本
	Role_Id_Set has_sub_profile_; // 是否本次副本已经扣了次数

	int32_t enter_level_;// 要用请先找相关人员(组队副本时：副本进入等级；组队竞技时：战队等级)

	// 多人竞技
	Team_Sports_Status sports_status_;
	int32_t point_;
	int16_t fail_times_; // 连败次数
	int16_t match_fail_; // 轮空次数
	Time_Value match_fail_time_;// 记录从地图服传回的轮空时间，用于去除重复处理
	bool battle_over_flag_; // 0:未开始、已经处理战斗结束;战斗结果从地图服传回场景服时，每个队员都发，些处用于标志已处理，不使多操作

	Role_Id_Set profile_set_;
	std::string monster_;

	// team_defend
	RoleID_Double_Map hurt_map_;
	RoleID_Double_Map be_hurt_map_;
	RoleID_Double_Map cure_map_;
};

struct Team_PtrGreater {
	bool operator()(const Team *team1, const Team *team2) const;
};
typedef std::map<int64_t, Team*> Int64_Team_Map;
typedef std::map<int64_t, Team> Int64_Team_Map_Detail; // 聊天服、场景服记录数据
typedef std::vector<Team*> Team_Vec;
typedef std::set<Team*, Team_PtrGreater> Team_RankingSet;


inline bool Team::is_in_team(role_id_t role_id) const {
	return (is_in_team_onlinelist(role_id) || is_in_team_offlinelist(role_id))? true : false;
}

inline bool Team::is_in_team_onlinelist(role_id_t role_id) const {
	return (is_leader(role_id) || is_team_member(role_id))? true : false;
}

inline bool Team::is_in_team_offlinelist(role_id_t role_id) const {
	return (offline_teamer_map_.count(role_id) == 0)? false : true;
}

inline bool Team::is_leader(role_id_t role_id) const {
	return (role_id == leader_role_id_)? true : false;
}

inline bool Team::is_team_member(role_id_t role_id) const {
	return (team_member_set_.count(role_id) == 0)? false : true;
}

inline void Team::set_team_id(int64_t team_id) {
	team_id_ = team_id;
}

inline int64_t Team::get_team_id(void) const {
	return team_id_;
}

inline void Team::set_team_index(uint32_t team_index) {
	team_index_ = team_index;
}

inline uint32_t Team::get_team_index(void) const {
	return team_index_;
}

inline Role_Hash_Set& Team::team_member_set(void) {
	return team_member_set_;
}

inline void Team::team_all_member_set(Role_Hash_Set &role_set) const {
	role_set.clear();
	for (Role_Hash_Set::const_iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		role_set.insert(*it);
	}
	role_set.insert(leader_role_id_);
	return ;
}

inline int Team::get_online_team_member_nums(void) const {
	int num = team_member_set_.size();
	if (this->is_leader_online()) {
		++num;
	}
	return num;
}

inline int Team::get_offline_team_member_nums(void) const {
	return offline_teamer_map_.size();
}

inline int Team::get_all_team_member_nums(void) const {
	return get_online_team_member_nums() + get_offline_team_member_nums();
}

inline int Team::get_index_nums(void) const {
	int nums = 0;
	for (Int8_Vec::const_iterator it = this->index_list_.begin(); it != this->index_list_.end(); ++it) {
		if (*it) {
			++nums;
			continue;
		}
	}
	return nums;
}



inline int Team::get_position_max(void) const {
	return this->get_index_nums() + 1;
}

inline int Team::get_unused_position(void) const {
	return get_position_max() - get_all_team_member_nums();
}

inline bool Team::is_team_offline_over_time(const Time_Value &now) {
	if (is_all_offline_ == false) {
		return false;
	} else {
		return (now.sec() - all_offline_time_.sec()) > DISMISS_TIMEOUT_SEC;
	}
}


#endif /* TEAM_H_ */
