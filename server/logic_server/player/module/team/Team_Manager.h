/*
 * Team_Manager.h
 *
 *  Created on: 2014年4月15日
 *      Author: Linqiyou
 */

#ifndef TEAM_MANAGER_H_
#define TEAM_MANAGER_H_

#include "Team.h"

class Team;

class Team_Manager {
public:
	enum {
		ADD_INFO = 1,
		CHANGE_INFO = 2,
		ESTIMATE_MAX_TEAM = 2048,
		TEAMIDKEY_BUCKET = ESTIMATE_MAX_TEAM + ESTIMATE_MAX_TEAM,

		TEAM_OFFLINE_NUMS = 1,

		TEAM_SPORTS_SWITCH_CLOSE = 0,
		TEAM_SPORTS_SWITCH_OPEN = 1,
	};
	const static int TEAM_SPORTS_SCENE_ID;
	Team_Manager(void);
	~Team_Manager(void);
	int start(void);
	int load_detail(void);
	int save_detail(void);

	Team *create_team(int scene_id, std::string password, int8_t type, int8_t degree);
	int set_team_index(Team *team);
	int set_team_id_for_scene(Team *team);
	void release_team(Team *team);
	int recycle_team_index(Team &team);

	Team *get_team(int64_t team_id);

	int listen_team_info(const int8_t & type, role_id_t role_id);
	int cancel_listen_team_info(const int8_t & type, role_id_t role_id);
	int cancel_listen_team_info(role_id_t role_id);

	int team_wait(Logic_Player& player, int type);
	int cancel_wait(Logic_Player &player, int type);

	void tick(const Time_Value &now);

	void free_pool_cache(void);
	void handle_ranking_checkout(void); // 清榜结算

	const Int64_Team_Map &team_map(void);
	const Team_Vec &team_fb_vec(void);
	const Team_Vec &team_sports_vec(void);

	// 根据队伍类型，将队伍添加到相应组队集里，清除队伍原所在组队集（新创建队伍类开相同，则不用处理）
	int change_team_type(Team *team, int8_t type, const int32_t scene_id, const int32_t degree);
	// 添加进组队副本集（有判断已存在）
	int insert_fb_vec(Team *team, const int32_t scene_id);
	// 添加进组队竞技集（有判断已存在）
	int insert_sports_vec(Team *team);
	// 清除队伍原所在组队集(处理)
	friend int erase_team_vec(Team *team, Team_Vec &team_vec);
	// 清除队伍原所在组队集（新创建队伍类开相同，则不用处理）
	int erase_team_type(Team *team, int8_t type);
	// 设置战队等级
	int set_sports_level(Team *team);
	// set fb enter_level
	int set_fb_level(Team *team, const int scene_id = -1);

	// 通知房间相关信息至自己、等待区玩家
	int active_team_add(int8_t type, Logic_Player *player, const Int64_Team_Map &team_map, bool to_all = false);
	int active_team_del(int8_t type, Logic_Player *player, const Int64_Team_Map &team_map, bool to_all = false);
	int active_team_update(int8_t type, Logic_Player *player, const Int64_Team_Map &team_map, bool to_all = false);

	int active_listen_player(const int8_t type, std::vector<Team_Basic_Info> &del_teams); // 广播给监听的人

	// 通知等待区相关信息至自己、等待区玩家
	int active_team_waiter_add(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all = false);
	int active_team_waiter_del(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all = false);
	int active_team_waiter_update(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all = false);
	int active_team_waiter_list(int8_t type, Logic_Player *player, const UInt_Role_Set_Map &role_map, bool to_all = false);
	int get_role_info_from_logic_player(Logic_Player *player, Role_Team_Info &role);

	UInt_Role_Set_Map &wait_area_player_map(void);

	// 组队竞技积分处理
	int integrage_change(role_id_t role_id, int32_t integrate);
	void battle_result(role_id_t role_id, int result, int score, Item_Vec &pack_item_vec);
	int active_integrate(const int64_t &role_id, int rank = -1);
	int active_integrate(void);
	int active_integrate(Team &team);
	const RoleId_TeamSportsRankingDetail_Map &integrate_map(void);
	const Team_Sports_Ranking_Info &integrate_ranking(void);
	const bool team_sports_is_open(void);
	int open_team_sports(void);
	int close_team_sports(void);
	int team_sports_test(void);
	int team_sports_test_close(void);
	void hook_level_change(const int new_level, Logic_Player &player);
	Int_Uid &first_lvl_role_id_his(void); // 本服务器第一个达到级的玩家
	Int_Uid &first_lvl_ck_his(void); // 本服务器第一个达到级的刺客
	Int_Uid &first_lvl_zs_his(void); // 本服务器第一个达到级的战士
	Int_Uid &first_lvl_yss_his(void); // 本服务器第一个达到级的元素师
	const Time_Value &team_sports_open_time(void);

	int get_role_team_level(role_id_t role_id);
	Time_Value &team_sports_start_time(void);

	void change_team_score(role_id_t role_id, int score);

private:
	Team_Manager(const Team_Manager &);
	const Team_Manager &operator=(const Team_Manager &);

	int64_t generate_team_id(void);
	uint32_t generate_team_index(void);
	int64_t generate_team_id_for_scene(void);

	void cancel_assign_apply(Logic_Player &player, int scene_id);

	int recycle_team_index(uint32_t team_index);

	void team_sports_tick(const Time_Value &now);


private:
	int64_t team_id_;					// 队伍id
	UInt_Set team_id_unused_;			// 闲置的队伍id
	Int64_Team_Map team_map_;			// 队伍 id->队伍 映射表
	UInt_UInt_Map team_index_id_map_;	// 队伍 index->id 映射表
	int64_t team_id_for_scene_;

	UInt_Role_Set_Map wait_area_player_map_;	// 等待区：队伍的类型-报名的玩家
	UInt_Role_Set_Map listen_player_map_;	// 监听组队界面的玩家
	RoleID_Int8Set_Map player_listen_type_map_; // 监听组队界面的玩家:玩家id-类型

	Team_Vec team_fb_vec_;				// 多人副本类排序后的
	Team_Vec team_sports_vec_;			// 多人竞技

	RoleId_TeamSportsRankingDetail_Map integrate_map_;
	Team_Sports_Ranking_Info integrate_ranking_;	// 组队竞技：积分榜
	int8_t team_sports_switch_;
//	int8_t team_sports_ranking_checkout_switch_;
	Time_Value team_sports_start_time_;
	bool team_sports_test_; // 测试命令开启的，不自动关闭
	Time_Value team_sports_open_time_; // 当前局开始时间

	First_Lvl_Detail first_lvl_;
	Time_Value save_tick_;
	int team_sports_open_times_;	// 开启次数
};

typedef Singleton<Team_Manager> Team_Manager_Single;

#define TEAM_MANAGER Team_Manager_Single::instance()

inline Time_Value &Team_Manager::team_sports_start_time(void) {
	return team_sports_start_time_;
}

#endif /* TEAM_MANAGER_H_ */
