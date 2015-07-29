/*
 * Move_Scene.h
 *
 *  Created on: Aug 13, 2013
 *      Author: ti
 */

#ifndef MOVE_SCENE_H_
#define MOVE_SCENE_H_

#include "Move_Scene_Base.h"
#include "NPC/NPC_Struct.h"

class Move_Scene: public Move_Scene_Base {
public:
	enum {
		PLOT_NULL = 0,
		PLOT_START_REQ,
		PLOT_BEGIN,
	};
	enum {
		SCENE_NORMAL_STATUS = 0,
		SCENE_READY_STATUS,
		SCENE_RECOVER_STATUS,   		//,
	};
	enum{
		SCENE_PASS_CON_KILL_BOSS = 1,		//杀boss
		SCENE_PASS_CON_TO_COORD = 2,			//行走到坐标
		SCENE_PASS_CON_USE_GOODS = 3,		//使用道具
		SCENE_PASS_CON_KILL_ALL = 4,			//杀所有怪
	};
	enum{
		SCENE_PASS_STATUS_NO_START 				= 0,			// 未开始
		SCENE_PASS_STATUS_NO_COMPELTE 			= 1,			// 未通关
		SCENE_PASS_STATUS_COMPELTE_RECEIVE 		= 2,			// 通关已领宝箱
		SCENE_PASS_STATUS_COMPELTE_NO_RECEIVE 	= 3,			// 通关未领宝箱
	};
public:
	Move_Scene();
	virtual ~Move_Scene();

	virtual void reset(void);

	virtual int enter_scene(Mover *mover);

	virtual int exit_scene(Mover *mover, bool offline = true);

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);
	virtual int release_scene_normal(void);

	virtual int client_scene_complete(Fighter &fighter);

	virtual int add_new_monster_point(const int point, bool is_boss, Coord point_coord);

	virtual int scene_pass_to_logic(void);

	virtual void send_path_guide(int point, Scene_Player* player);

	virtual void hook_player_enter_battle(Scene_Player* player);

	virtual void hook_scene_pass(void);

	virtual void hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monters);

	virtual void hook_gather_special_handle(Mover* mover, int point_id, int material_type);
	virtual void hook_gather_start_special(Mover* mover, int point_id, int material_type);
	virtual void hook_gather_success(Mover* mover, int point_id, int material_type);
	virtual void hook_player_move(Scene_Player* player);
	virtual void hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums);
	// 获取复活点
	virtual int get_revive_point(Coord &coord, int index = 0);
	// 是否双倍收益
	virtual bool is_double_income(void);
	// 怪点清理侦听
	virtual void hook_monster_point_clear(const int point_id);

	virtual void be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id);

	// 获取最近复活点
	int get_nearest_revive_point(Coord& coord, const Coord src_coord);

	bool is_scene_pass(void);

	bool is_scene_complete(void);

	virtual int tick(const Time_Value &now);
	// 1000毫秒tick
	int tick_1000ms(const Time_Value &now);


	int set_create_scene_args(const Create_Scene_Args &create_scene_args);

	bool is_recover_status(void);
	bool is_recover_time(Time_Value &now);
	void set_scene_status(int status);
	int scene_type(void);
	int get_scene_status();

	void npc_init(int scene_id, int sort, int sub_sort);
	void npc_release(void);
	void section_init(int scene_id, int sort, int sub_sort);
	NPC_Group* get_npc_group(int id);
	NPC_Record* get_npc_record(uint64_t id);
	bool add_npc_record(const uint64_t id, NPC_Record npc_record);
	bool del_npc_record(const uint64_t id);

	virtual void save_battle_plot_date(std::string plot_id, int plot_step, int record_id);

	void refresh_monster_for_debug(Coord& bird_coord, int npc_type_id, role_id_t role_id);
	int refresh_monster_point(int point_id, role_id_t role_id);
	void check_npc_refresh(const Time_Value &now);

	NPC_Section& npc_section(void);

	void set_no_profit_player(role_id_t role_id);
	//没有收益的玩家
	bool is_no_profit(role_id_t role_id);

	void reset_delay_show_ts_point_time(void);
	// 获取场景额外参数
	void get_scene_param(int& param1, int& param2, int& param3);
	void set_scene_param(const int param1, const int param2, const int param3, bool to_client = false);
	// 获取副本通关状态
	int get_pass_status(void);
	// 设置副本通关状态
	void set_pass_status(int value, bool to_client = false);

	//
	void add_battle_scene_id(role_id_t battle_id);
	void remove_battle_scene_id(role_id_t battle_id);

	void battle_scene_release(void);
	void init_player_own_point(role_id_t role_id);

	bool is_monster_point_all_clear(void);
private:

protected:
	Create_Scene_Args create_scene_args_;				// 创建场景信息
	Scene_Config::Transmit_Point_Vec transmit_point;	// 传送点
	NPC_Section	npc_section_;							//
	Role_Id_Set monster_clear_point_set_;

	int scene_status_;
	bool is_scene_pass_;						// 通关(多层副本的需要所有完成才算通关)
	bool is_scene_complete_;					// 完成
	int pass_status_;							// 副本通关状态 0：未开始 1：未通关 2：通关已领宝箱 3：通关未领宝箱

	int scene_param_1_;						// 场景参数1（0：正常1：单人副本复活中）
	int scene_param_2_;						// 场景参数2（单人副本复活中表示剩余免费复活次数）
	int scene_param_3_;						// 场景参数3（单人副本复活中表示付费复活所需钻石）

	Time_Value recover_time_;
	Time_Value scene_begin_time_;				//

	Role_Id_Set player_enter_battle_set_;		// 已经开打的玩家
	Role_Id_Set no_profit_player_set_;			// 无收益玩家

	bool is_delay_show_ts_point_;				// 延时发送通关传送阵
	bool is_show_ts_point_; 					// 发送通关传送阵

	Mover_Map player_map_;						// 只要有enter_scene都记录在这里
	Time_Value ts_point_delay_time_;			// 延时发送通关传送阵时间
	Role_Hash_Set enter_players_set_;	   		// 进入过场景的玩家
	Time_Value nor_tick_time_;					// 场景普通刷新频率
	Role_Id_Set battle_id_set_;
};

#endif /* MOVE_SCENE_H_ */
