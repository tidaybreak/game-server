/*
 * Machine.h
 *
 *  Created on: 2014年5月5日
 *      Author: linyisheng
 */

#ifndef MACHINE_H_
#define MACHINE_H_


#include "NPC.h"

class Scene_Player;
class Mover;

class Machine: public NPC {
	typedef boost::unordered_map<role_id_t, Time_Value> Gather_Player_Map;
	typedef boost::unordered_map<role_id_t, bool> Take_Lift_Player_Map;
	enum{
		PLOT_STEP_1 = 1,					//触发前剧情阶段
		PLOT_STEP_2 = 2,					//触发后剧情阶段
	};
	enum{
		OPERATE_CLICK 					= 0,				// 点击触发
		OPERATE_GAHTER 			 	= 1,				// 采集触发
		OPERATE_AUTO	 				= 2,				// 进入范围触发
		OPERATE_AUTO_TEAM          = 3,				// 队伍进入范围触发
		OPERATE_AUTO_SECTOR 			= 4,				// 进入扇形范围触发
		OPERATE_AUTO_SECTOR_TEAM	= 5,				// 队伍进入扇形范围触发
		OPERATE_NONE	= 6,								//
	};
public:
	Machine(void);
	virtual ~Machine(void);

	int birth(role_id_t npc_id, int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info);

	int init(int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info);
	int fini(void);
	int reset(void);

	virtual int tick(Time_Value &now);
	virtual int move_appear_info(Block_Buffer &buf);

	void check_event_team_trigger(bool sector = false);

	void check_event_trigger(bool sector = false);

	int event_trigger(std::vector<Scene_Player*>& target_vec);

	int event_execute(int id, Scene_Player* target);

	int event_execute(int id, role_id_t role_id);

//	virtual int die_process(const Attack_Info &info, Fighter &killer);

	virtual bool is_machine(void);

	virtual Machine *machine_self(void);

	inline Machine_Detail &machine_detail(void);


	int change_self(int jid);

	// 通知逻辑服死亡
	int notice_logic_die(role_id_t attacker);

	int open_machine(Scene_Player* player);
	int start_gather(Scene_Player* player);
	int stop_gather(role_id_t role_id);

	int gather_time(void);
	int machine_event_id(void);

	int beat_back(Scene_Player* player, const Coord &player_coord);

	int take_lift_finish(Scene_Player* player);
	// 电梯
	bool is_lift(void);
	// 电梯复位
	void lift_reset(void);
private:
	int create_accessory(int npc_type_id, Move_Scene *scene, NPC_Record* point, NPC_Addition_Info &npc_addition_info);
	int destory_accessory(role_id_t role_id);
	inline void add_gather_status(void);
	inline void del_gather_status(void);
	bool check_player_in_plot(void);
	void check_gathering_player(const Time_Value& now);
	int operate_start_gather(Scene_Player* player, bool add_gather_status = true);
	int operate_end_gather(Scene_Player* player, bool del_gather_status = true);
	int operate_plot_step1(Scene_Player* player, std::string plot_id);
	int operate_plot_step2(Scene_Player* player, std::string plot_id);

	bool all_take_lift_finish(void);
private:
	Machine_Detail machine_detail_;
	Gather_Player_Map gather_player_map;
	int gather_status;
	int plot_step_;
	role_id_t gather_player_role_id_;
	std::vector<role_id_t> plot_player_vec_;
	role_id_t acc_role_id_;
	Take_Lift_Player_Map take_lift_player_map_;
	bool check_out_range_;		// 需要出去检测范围后，检测范围才生效
	bool lift_return_;			// 电梯返回
	int tick_count_;				//
};

void Machine::add_gather_status(void){
	gather_status += 1;
}

void Machine::del_gather_status(void){
	gather_status -= 1;
}

#endif /* MACHINE_H_ */
