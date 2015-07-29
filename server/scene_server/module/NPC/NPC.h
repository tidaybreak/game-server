/*
 * NPC.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef NPC_H_
#define NPC_H_

#include "NPC_Def.h"
#include "fighter/Fighter.h"
#include "Event_Handler.h"
#include "ai_machine/behavior_tree/Behavior_Tree.h"
#include "ai_machine/AI_Struct.h"
#include "NPC/NPC_Struct.h"

class Monster;
class Material;
class Drops;
class Hero;

class NPC: 	public Event_Handler,
			public Fighter {
	struct Gauge_Coord{
		Coord coord;
		Time_Value time;
		Gauge_Coord(void){
			reset();
		}
		void reset(void){
			coord.reset();
			time = Time_Value::zero;
		}
	};
	typedef std::vector<Gauge_Coord> Gauge_Coord_Vec;
public:

	enum {
		NORMAL_STATUS  	= 0,
		PATROL_STATUS   	= 1,   // 巡逻状态
		CHASE_STATUS    	= 2,   // 追击状态
		PLOT_STATUS       = 3,   // 剧情
		FIGHT_STATUS 		= 4,   // 战斗状态
		FOLLOW_STATUS   	= 5,   // 跟随状态
		MOVE_TARGET   		= 6,   // 移动到目标
		BACK_STATUS 		= 7,   // back状态
		DEAD_STATUS 		= 8,   // 死亡状态
		RECOVER_STATUS 	= 9,   /// 请求回收状态
		DORMANCY_STATUS 	= 10,   /// 休眠状态(通常是死亡后让客户端不可见,ai还在运行)
	};

	NPC(void);

	virtual ~NPC(void);

	////////////////////////////////////////////////////////////////

	virtual role_id_t role_id(void);

	virtual int level(void);

	virtual std::string role_name(void);

	virtual void set_role_name(const std::string& name);

	virtual int career(void);

	////////////////////////////////////////////////////////////////

	int start_ai(Time_Value tick = Time_Value::zero);
	int stop_ai(void);
	int fini(void);
	void reset(void);
	int init_bt(void);

	virtual int tick(Time_Value &now);
	virtual int vision_tick(void);

	virtual int exit_move_scene(bool offline_exit = true);

	virtual int ai_config_init(void);

	virtual int active_npc(void);
	virtual int deactive_npc(void);

	virtual int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);
	virtual int respond_finer_result(int msg_id, Block_Buffer *buf = 0);

	virtual int sign_in(void);

	virtual int sign_out(void);

	virtual int targger_condition(int type, Fighter *fighter1 = 0);

	int pop_watcher_tick(Time_Value &tv);
	int pop_watcher_tick(void);

	virtual int push_watcher_tick(long sec, long usec = 0);
	virtual int push_watcher_tick(const Time_Value &tv);
	virtual int push_watcher_tick(Time_Value &tv);

	virtual int normal_die_self(void);

	virtual int recover_self(void);

//	virtual int die_process(const Attack_Info &info, Fighter &killer);

	virtual void set_npc_status(int npc_status);

	virtual bool recover_state(void);

	virtual bool npc_dead_state(void);

	virtual bool dormancy_state(void);

	virtual int handle_timeout(const Time_Value &tv);

	virtual bool is_valid_coord(Coord grid_coord);

	virtual role_id_t npc_id(void);

	virtual int npc_type_id(void);

	int npc_operation_type(void);
	void set_npc_operation_type(const int value);
	uint8_t npc_operation_state(void);
	void set_npc_operation_state(const uint8_t value);
	void broad_operation_state(void);

	void hook_gather_act(void);

	virtual int npc_status(void);

	virtual bool ai_status(void);

	virtual bool is_npc(void);

	virtual bool is_monster(void);

	virtual bool is_material(void);

	virtual bool is_machine(void);

	virtual bool is_special_npc(void);

	virtual bool is_drops(void);

	virtual bool is_hero(void);

	virtual bool is_player_monster(void);

	inline NPC_Detail &npc_detail(void);

	virtual Scene_Player *player_self(void);

	virtual NPC *npc_self(void);

	virtual Time_Value &birth_time(void);

	virtual Coord &birth_coord(void);

	//virtual const AI_Info &ai_config(void);

	virtual AI_Running_Info &ai_running_info(void);

	virtual NPC_Record *npc_record(void);
	virtual void set_record(NPC_Record *record);

	virtual role_id_t master_id(void);

	virtual int refresh_cache_config(void);

	//test
	int move_inner(int x, int y);
	int move_inner_vec(Coord_Vec coord_vec);
	int path_find(const Coord &src_coord, Coord &dst_coord, Coord_Vec &path, bool mover = true, bool obstacle = true);

	void set_leader_id(role_id_t id);
	role_id_t get_leader_id(void);
	void add_teammate(role_id_t id);
	void del_all_teammate(void);
	int get_move_inner_coord(Coord& src_coord, Coord& available_coord);
	void set_move_speed(int value);
	int get_move_speed(void);
	void refresh_move_interval(void);

	Hatred_Map& hatred_map(void);
	Hatred_Info& most_hatred_role(void);
	int find_hatred_map(const role_id_t &role_id, Hatred_Info &hatred_info);
	void del_all_hatred(bool self=true, bool other=true);

	void set_talk_vec(std::vector<Int_Int>& talk_vec);
	void set_chat_vec(std::vector<Int_Int>& chat_vec);
	int get_talk_id(void);
	int get_chat_id(void);
	void get_team_coord(const Coord &target, Coord &team_coord);
	void set_team_info(int count, int pos);

	// 纠正检测使用的坐标
	void retify_gauge_grid_coord(void);
	// 检测使用的坐标
	Coord& gauge_grid_coord(void);
	int get_gauge_coord_num(void);

	int get_cur_gt_times(void); // 当前采集次数
	int add_cur_gt_times(const int times);
	int get_cfg_max_gt_times(void); // 配置最大采集次数
	int set_cfg_max_gt_times(const int times);

	bool had_collect(const role_id_t role_id);
	void add_role_collect(const Role_Hash_Set role_set);
	void clear_role_collect(void);

protected:
	void gauge_grid_coord_tick(const Time_Value& now);
protected:
	NPC_Detail npc_detail_;
	NPC_Record *npc_record_;
	Behavior	bev_tree_root_;

	Time_Value fini_time_;			// debug use
	Time_Value disappear_time_;		// debug use

	role_id_t leader_id_;
	int team_coord_pos;
	role_id_t teammate_id_[MAX_TEAMMATE_NUM];
	int teammate_count_;

	Time_Value move_interval[3];

	Hatred_Map hatred_map_;						// 仇恨列表
	Hatred_Info most_hatred_role_;			// 当前最大仇恨
	Coord gauge_grid_coord_;					// 怪物检测使用的坐标
	Gauge_Coord_Vec gauge_coord_vec_;		// 检测使用的移动位置
	Time_Value gauge_tick_time_;				// 检测使用的移动下一步时间

	int cur_gt_times_; // 当前采集次数
	int cfg_max_gt_times_; // 配置最大采集次数
	Role_Hash_Set role_collected; // 已采人
};

NPC_Detail &NPC::npc_detail(void) {
	return npc_detail_;
}

inline Scene_Player* NPC::player_self(void){
	return NULL;
}

inline NPC *NPC::npc_self(void) {
	return this;
}

#endif /* NPC_H_ */
