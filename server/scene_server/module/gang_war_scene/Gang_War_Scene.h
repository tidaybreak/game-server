/*
 * Gang_War_Scene.h
 *
 *  Created on: 2014年8月1日
 *      Author: Linqiyou
 */

#ifndef GANG_WAR_SCENE_H_
#define GANG_WAR_SCENE_H_


#include "Move_Scene.h"
#include "gang/Gang_War_Struct.h"
#include "dungeon/Dungeon_Config.h"
#include "Msg_Struct.h"

class Gang_War_Scene: public Move_Scene{
	typedef boost::unordered_map<role_id_t, Gang_War_Rank_Detail> Offline_Mover_Map;
//	typedef boost::unordered_map<role_id_t, Time_Value> RoleID_TimeValue_Map;
//	typedef boost::unordered_map<int8_t, RoleID_TimeValue_Map> Camp_RoleIDTimeValueMap_Map;
public:
	Gang_War_Scene(void);
	virtual ~Gang_War_Scene(void);
public:
	void reset(void);
	virtual int tick(const Time_Value &now);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline);

	virtual int client_scene_complete(Fighter &fighter);
	virtual int update_move_able_info(Mover* mover);
	virtual int change_move_value(int16_t mp_value, bool moveable);

	void npc_init(int scene_id, int sort, int sub_sort);
	void section_init(int scene_id, int sort, int sub_sort);
//	void nor_mine_init(Monster_Point_Config* point_cfg);
//	void revive_ball_init(Monster_Point_Config* point_cfg);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int scene_pass_to_logic(void);

	virtual void hook_player_enter_battle(Scene_Player* player);
	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters);
	virtual void hook_gather_special_handle(Mover* mover, int point_id, int material_type);
	virtual void hook_gather_start_special(Mover* mover, int point_id, int material_type);
	virtual void hook_gather_success(Mover* mover, int point_id, int material_type);

	int scene_status(void);
public:
	int req_war_scene_fetch_rank(Scene_Player* player);

	int req_gang_war_heal(Scene_Player* player);

	void war_battle_finish(int win_camp);

	void sync_rank_info(const std::vector<Gang_War_Rank_Info> &vec, const int win_league,
			int64_t gang_id,
			int64_t gang_id_lose);
	void broadcast_tower_info(Scene_Player *player = NULL);
	void sync_gang_war_result(const int win_league);
	int gang_war_scene_announce(int type, role_id_t role_id = 0, std::string role_name = "",
			std::string name = "", std::string gang_name = "", int value1 = 0, int value2 = 0, int value3 = 0);
	void kick_all_player(void);

	int set_war_over_force(void); // 强取赢方
private:
	void send_war_info(Mover* mover);
	void send_player_info(Scene_Player* player);
	void broadcast_gang_war_info(void);
	void send_end_time(Mover* mover = NULL);
	Gang_War_Rank_Detail* find_offline_mover(role_id_t role_id);
	void add_offline_mover(role_id_t role_id, Gang_War_Rank_Detail& info);
	void del_offline_mover(role_id_t role_id);
//	void check_recover_mine(void);
	void check_over(void);
//	bool check_player_empty(void);
//	void check_offline_player(const Time_Value& now);
//	void check_inactive_player(const Time_Value& now);
	void war_over(const int win_league);
//	void recover_mine(int league, Mover_Set& mover_set);
	void kick_player(Scene_Player* player, int reason);
//	void pop_random_coord(int point_id, Coord& coord);
//	void push_random_coord(int point_id, const Coord& coord);
//	void change_random_coord(int point_id);

	void material_tick(void);
	void material_insert_sort_list(Gang_War_Material_Info *info);
	void material_remove_sort_list(Gang_War_Material_Info *info);
	int8_t material_get_index_from_point_id(const int point_id);
private:
	int player_initial_score_;

	int red_score_; // 资源数
	int red_num_; // 人数
	int red_tower_num_; // 资源塔数
	role_id_t gang_id_red_;
	std::string gang_name_red_;
	int blue_score_;
	int blue_num_;
	int blue_tower_num_;
	role_id_t gang_id_blue_;
	std::string gang_name_blue_;
	bool red_has_broadcast_will_win_;
	bool blue_has_broadcast_will_win_;

	bool check_over_;
	Time_Value war_over_time_;
	Offline_Mover_Map offline_mover_map_;

	Coord src_recover_coord_[2];
	War_Random_Coord_Config random_coord_;
	int war_total_time_;

	Camp_IndexGangWarMaterialInfoMap_Map material_complete_map_; // 开始产积分资源塔
	Index_GangWarMaterialInfo_Map material_hold_map_; // 已被占资源塔
	GangWarMaterialInfo_List material_ready_list_; // 心跳：已被占->开始产积分
	Int_Hash_Set material_ready_set_; // 已被占->开始产积分
};



#endif /* GANG_WAR_SCENE_H_ */
