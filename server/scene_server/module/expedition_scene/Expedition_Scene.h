/*
 * Expedition_Scene.h
 *
 *  Created on: 2014-10-14
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_SCENE_H_
#define EXPEDITION_SCENE_H_

#include "Move_Scene.h"
#include "Expedition_Scene_Struct.h"

class Expedition_Scene_Manager;
struct Expedition_Gang_Brief_Info;
struct Expedition_Build_Brief_Info;
struct MSG_10201300;
struct MSG_10201301;
struct MSG_10201302;
struct MSG_10201303;
struct MSG_10201304;
struct MSG_10201305;
struct MSG_10201306;
struct MSG_10201307;
struct MSG_10201308;
struct Expedition_Gang_Home_Cfg;
class Expedition_Global_Scene_Manager;
struct Other_Info;
struct MSG_10201309;
struct MSG_10201310;
struct Expedition_Offline_Player_Data;
struct MSG_10201311;
struct MSG_10201312;

class Expedition_Scene: public Move_Scene {
public:
	Expedition_Scene();
	virtual ~Expedition_Scene();

	void reset(void);

	//玩家进入场景
	virtual int enter_scene(Mover *mover);

	//玩家退出场景
	virtual int exit_scene(Mover *mover, bool offline);

	//场景加载完成
	virtual int client_scene_complete(Fighter &fighter);
	virtual int update_move_able_info(Mover* mover);
	virtual int change_move_value(int16_t mp_value, bool moveable);

	virtual int tick(const Time_Value &now);
	int fight_castle_tick(const Time_Value &now);
	int fight_other_tick(const Time_Value &now);
	int tick_check_and_clear_defend_when_attack_is_empty(Stronghold *sh);

	virtual void hook_player_enter_battle(Scene_Player* player);

	virtual void hook_scene_pass(void);

	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters);

	virtual void hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums);

	virtual void hook_monster_point_clear(const int point_id);

	void hook_battle_finish_for_other(Battle_Scene *bs, Stronghold *sh, Other_Info ot, int battle_type, int win_pos,
			std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);
	void hook_battle_finish_for_castle(Battle_Scene *bs, Stronghold *sh, Other_Info ot, int battle_type, int win_pos,
				std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);

	//创建场景地图基本数据
	void create_scene_map_data(uint32_t line, Expedition_Scene_Manager *manager);

	//向客户端发送场景建筑信息
	void send_strong_hold_data_to_client(Scene_Player *player);

	// msg
	int req_expedition_daily_award_get(Scene_Player *pl, const MSG_10201300 &msg);
	int req_expedition_daily_award_info(Scene_Player *pl, const MSG_10201301 &msg);
	int req_expedition_other_build_info(Scene_Player *pl, const MSG_10201302 &msg);
	int req_expedition_gang_rank(Scene_Player *pl);
	int req_expedition_battle_info(Scene_Player *pl, const MSG_10201304 &msg);
	int req_expedition_battle_info_for_beast(Scene_Player *pl, Stronghold *sh);
	int req_expedition_gang_base_build_info(Scene_Player *pl, const MSG_10201305 &msg);
	int req_expedition_fighting(Scene_Player *pl, const MSG_10201306 &msg);
	int req_expedition_help_info(Scene_Player *pl, const MSG_10201307 &msg);
	int active_expedition_help_info(Stronghold *sh);
	int active_expedition_help_info(Scene_Player *pl, const int point_id);
	int req_expedition_change_line(Scene_Player *pl, const MSG_10201308 &msg);
	int req_expedition_demage_rank(Scene_Player *pl, const MSG_10201309 &msg);
	int req_expedition_looking(Scene_Player *pl, const MSG_10201310 &msg);
	int req_expedition_occupy_castle(Scene_Player *pl, const MSG_10201311 &msg);
	int req_expedition_occupy_castle_award_get(Scene_Player *pl, const MSG_10201312 &msg);
	// msg

	// interface start --
	int check_fight_point(Scene_Player *pl);
	int join_in_fighting(Scene_Player *pl, const int point_id);
	int drop_out_fighting(Scene_Player *pl, const int point_id);
	int exit_drop_out_fighting(Scene_Player *pl, const int point_id);
	int join_in_looking(Scene_Player *pl, const int point_id);
	int join_in_looking_front(Scene_Player *pl, const int point_id);
	int drop_out_looking(Scene_Player *pl, const int point_id);
	int exit_drop_out_looking(Scene_Player *pl, const int point_id);
	int init_defend_for_beast(Scene_Player *pl, const int point_id);
	int recover_defend_for_beast(Scene_Player *pl, Stronghold &sh);

	int active_map_data(Scene_Player *pl);
	int active_map_data_by_gang_id(const int64_t gang_id);
	int active_map_all_data(void);
	int active_map_data_for_change_line(Scene_Player *pl);
	int active_map_all_data_by_point_id(const Int_Hash_Set point_id);
	int active_map_all_data_by_point_id(const int point_id);
	int active_map_data_by_point_id(Scene_Player *pl, const int point_id);
	int update_move_able_info_by_gang_id(const int64_t gang_id);
	int change_move_able_by_gang_id(const int64_t gang_id);
	int active_daily_award_by_gang_id(const int64_t gang_id);

	int active_expedition_daily_award_info(Scene_Player *pl, const bool send_un_create = false);

	int change_point_force(const int point_id);
	int enter_battle(Stronghold* sh);
	int enter_battle_more_attack(Stronghold* sh);
	int enter_battle_more_defend(Stronghold* sh);
	int enter_battle(Soldier *soldier_attack, Soldier *soldier_defend, const int point_id);
	int enter_battle(Scene_Player *pl_a, Scene_Player *pl_d, const int point_id);
	int enter_battle_for_offline_defend(Scene_Player *pl_a,
			const Expedition_Offline_Player_Data *eopd, const int point_id);
	int enter_battle_for_offline_defend_more(const Expedition_Offline_Player_Data *eopd, const Stronghold *sh);

	int enter_battle_for_other(const role_id_t role_id, Stronghold* sh);

	int active_player_battle_state(const std::vector<Mover*> &mover_vec, const int point_id, const int state);
	int active_player_battle_state(Scene_Player *pl, const int point_id, const int state, const bool is_dead = false);

	const Expedition_Gang_Home_Cfg * get_one_point_conf(void);

	// revive
	int add_offline_mover(const Expedition_Scene_Info &expedition_scene_info);
	Expedition_Scene_Info *find_offline_mover(const role_id_t role_id);
	int del_offline_mover(const int64_t role_id);

	// demage_rank
	int add_demage_rank(Scene_Player *pl, const double demage);
	void reset_demage_vec(void);

	// occupy castle
	void add_occupy_castle_rank(Soldier* pl);
	void active_occupy_castle_rank(Scene_Player *pl);

	//rename
	void update_name(const role_id_t role_id, const std::string name);

	// refreshed_monster
	void init_refreshed_monster(const Point_ExpeditionDropOrMaterial &refreshed_monster);
	void del_refreshed_monster(const int point_id);
	int add_role_collected(const int point_id, const int64_t role_id);
	// -- interface end

	// offline player monster --
	int add_offline_data(Scene_Player *pl);
	int del_offline_data(const role_id_t role_id);
	const Expedition_Offline_Player_Data *find_offline_data(const role_id_t role_id);
	// -- offline player monster

public:
	int create_drop_by_point_id(const int point_id, Scene_Player &pl);
	void award_tick(const Time_Value &now);
	void settle_tick(const Time_Value &now);
	void settle_handle(void);

	// refresh_monster
	void refresh_monster_timing_tick(const Time_Value &now);
	void sync_refresh_monster_timing_to_logic(const Int_Hash_Set &need_refresh_monster);
	void sync_refresh_monster_timing_kill_to_logic(const int point_id);
	void sync_refresh_open_box_to_logic(const int drop_id, const Int_Hash_Set items, Scene_Player *sp);
	void refresh_monster_kill_gap_tick(const Time_Value &now);
	void refresh_drop_or_material(const Time_Value &now);
	void show_monster(const int point_id);

	int get_hold_nums_by_gang_id(const int64_t gang_id, const int8_t type = 0);
	int create_and_send_daily_award_to_logic(void);
	int send_gang_hold_nums_to_logic_by_gang_id(const int64_t gang_id, const std::map<int, int> nums);

	 // build_info
	int init_gang_base_territory(const int point_id, const bool is_add = false);
	const Int_Hash_Set *find_base_territorys(const int point_id);
	int set_msg_by_type(Scene_Player &pl, const Stronghold &sh, Expedition_Build_Brief_Info &ebbi);
	int init_area_for_beast_and_chest(Int_Hash_Set &gang_id_areavalue);

	// -- areavalue
	Int_Hash_Set *find_areavalue_by_gang_id(const int64_t gang_id);
	// -- areavalue

	Stronghold *find_map_data_by_point_id(const int point_id);

	Stronghold_Map &map_data();//地图的总数据; 是所有据点(大中小城堡,关卡,宝箱点,怪物点)的数据;据点里面包含所有数据(含有攻防的玩家,状态等等)
	Player_Soldier_Map &player_soldier_map();//在据点里面的所有玩家的基本信息池的指针map
//	uint32_t &map_gang_in_num();//公会入驻个数
	Int_Uid &point_gang_id(void);
	uint32_t &map_base_point_num();//地图数据中基地的总数
	uint32_t &cur_line(); //场景的当前线
	Expedition_Scene_Manager *get_manager(void);
	const Role_Hash_Set &gang_ids(void) const;
	void get_uuids(Role_Hash_Set &uuids);

	int clear_gang_info(const int64_t gang_id, const bool from_del = false);
	int clear_occupier_by_gang_id(const int64_t gang_id);

private:
//	int join_in_fighting_defend(Stronghold &sh);
//	int join_in_fighting_attack(Stronghold &sh);
//	int drop_out_fighting_defend(Stronghold &sh);
//	int drop_out_fighting_attack(Stronghold &sh);
	int recover_resource(Stronghold_Info &info, const bool need_clear_defend);
	int join_in_looking_defend_on_monster_front(Stronghold *sh, const Soldier *soldier_d);
	int drop_out_looking_defend_on_monster(Stronghold *sh, const Soldier *soldier_d);

	// territorys --
	int change_territorys(Stronghold *sh);
	int set_base_territorys(const Int_IntHashSet_Map &conf);
	const Int_IntHashSet_Map &get_base_territorys(void);
	int add_base_territorys(const int base_point_id, const int point_id);
	int del_base_territorys(const int base_point_id);
	int del_base_territorys(const int base_point_id, const Int_Hash_Set *ihs);
	int del_base_territorys(const int base_point_id, const int point_id);
	int check_and_init_territorys(Scene_Player *pl);
	int check_and_init_beast(Stronghold *sh);
	int recover_territorys(Scene_Player *pl);
	// -- territorys

	// force --
	int change_force_from_one_to_one(const int point_from, const int point_to);
	int change_force_from_one_to_one(Stronghold *sh_from, const Stronghold *sh_to);
	// -- force

	int kick_player_by_gang_id(const int64_t gang_id);
	int kick_player(Scene_Player* pl);

	void save_tick(void);

	friend class Expedition_Global_Scene_Manager;
	friend class Expedition_Scene_Manager;
private:
//	Soldier_Pool soldier_pool_;//npc,玩家,能战斗的一些东西的基本信息池
	Stronghold_Map map_data_;//地图的总数据; 是所有据点(大中小城堡,关卡,宝箱点,怪物点)的数据;据点里面包含所有数据(含有攻防的玩家,状态等等)
	Player_Soldier_Map player_soldier_map_;//在据点里面的所有玩家的基本信息池的指针map
//	uint32_t map_gang_in_num_;//公会入驻个数
	Int_Uid point_gang_id_;
	uint32_t map_base_point_num_;//地图数据中基地的总数
	uint32_t cur_line_; //场景的当前线
	Expedition_Scene_Manager *manager_;

	Role_IntHashSet_Map gang_id_areavalue_;
//	Role_IntHashSet_Map gang_id_territory_;
	Int_IntHashSet_Map base_territorys_; // conf 公会基地：不配建筑领土，程序处理
	Role_Hash_Set gang_ids_;

	Stronghold_Set fighting_set_;
	Stronghold_List fighting_cancel_list_;
	Soldier_List soldier_offline_;

	Stronghold_Set other_fighting_set_;
	Stronghold_List other_fighting_cancel_list_;

	Time_Value award_time_;

	Role_ExpeditionSceneInfo_Map expedition_scene_info_;

	Role_ExpeditionSceneDemageRankData_Map demage_rank_;
	ExpeditionSceneDemageRankData_Vec demage_vec_;

	int settle_time_;
	Time_Value settle_time_falg_;

	// 攻城榜
	Role_ExpeditionOccupyCastleData_Map occupy_castle_rank_;
	ExpeditionOccupyCastleData_Vec occupy_castle_vec_;

	// player offline
	ExpeditionOfflinePlayerData_Map offline_data_;


	// refreshed_monster
	Point_ExpeditionDropOrMaterial refreshed_drop_or_material_;
	Int_Time_Map refreshed_drop_or_material_time_history_;
	Int_Time_Map refreshed_monster_timing_;
	Int_Time_Map refreshed_monster_kill_gap_;

//	StrongholdPoint_Map monster_data_refresh_timing_;//怪点：定时
//	StrongholdPoint_Map monster_data_refresh_kill_gap_;//怪点：打完间隔刷

	Int_Hash_Set need_broadcast_point_;
};


#endif /* EXPEDITION_SCENE_H_ */
