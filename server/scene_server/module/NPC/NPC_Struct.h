/*
 * NPC_Struct.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef NPC_STRUCT_H_
#define NPC_STRUCT_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"
#include "Time_Value.h"
#include "mover/Mover_Struct.h"
#include "Fighter_Detail.h"
#include "NPC/NPC_Config.h"
#include "NPC/monster/Monster_Def.h"

class NPC;
class Monster;
class Move_Scene;
class Battle_Scene;
class NPC_Section;
class AI_Info;
class Fighter;
class Scene_Player;

struct Monster_Point_Config;
struct Monster_Group_Config;

enum OWNER_MONSTER_POINT_ID{
	DEBUG_MONSTER_POINT_ID    = 1001,
	//END 1500
};

// 主城特殊怪物点(
enum MAIN_SCENE_M_P_ID{
	SECRET_SHOP_POINT_ID      = 2001,
	MAIN_SCENE_M_P_ID_VT_1 	  = 3000,
	MAIN_SCENE_M_P_ID_VT_2 	  = 3001,
	MAIN_SCENE_M_P_ID_VT_3 	  = 3999,
	//END 9999
};

enum GLOBAL_MONSTER_POINT_ID{
	L_REVIVE_BALL_B			  = 2011,
	L_REVIVE_BALL_E			  = 2018,
	D_REVIVE_BALL_B           = 2021,
	D_REVIVE_BALL_E           = 2028,
	L_MINE_B                  = 2031,
	L_MINE_E                  = 2036,
	D_MINE_B                  = 2041,
	D_MINE_E                  = 2046,
	GANG_WAR_RED_REVIVE_BALL_B	= 2051,
	GANG_WAR_RED_REVIVE_BALL_E	= 2058,
	GANG_WAR_BLUE_REVIVE_BALL_B = 2061,
	GANG_WAR_BLUE_REVIVE_BALL_E = 2068,
	//END 9999

};


struct Walk_Path {
	Coord_Vec walk_path;
	int curr_path_index;

	Coord_Vec walk_coord;
	int curr_coord_index;

	//追击使用
	int move_gride_count;	//移动的格子数

	Walk_Path(void);
	void reset(void);

	bool has_walk_path(void);
	// 顺序取
	int get_next_path(Coord &coord);
	// 循环取
	int get_loop_path(Coord &coord);
	// 顺序取,最后返回最后值
	int get_last_path(Coord &coord);

	bool has_walk_coord(void);
	int get_next_coord(Coord &coord);
	bool is_arrived(void);
	int clean_walk_coord(void);
};

enum NPC_Type {
	Normal_NPC = 0,
	Monster_NPC = 5,
	Material_NPC = 4,
};

inline NPC_Type get_npc_type_by_type_id(int npc_type_id) {
	return static_cast<NPC_Type>(npc_type_id / 100000000);
}

enum Player_Type {
	Person_Type = 1,
	Material_Type,
	Monster_Type,
	Special_Monster_Type,
	Player_Monster_Type,
	Drops_Type,
	Pet_Type,
	Pet_Monster_Type,
	Machine_Type,
	NON_Type,
};

static const role_id_t monster_role_id_range[2] = {1000000, 2000000};
static const role_id_t special_npc_id_range[2] = {2000000, 3000000};
static const role_id_t player_npc_id_range[2] = {3000000, 4000000};
static const role_id_t material_role_id_range[2] = {4000000, 5000000};
static const role_id_t drops_role_id_range[2] = {5000000, 6000000};
static const role_id_t pet_role_id_range[2] = {6000000, 7000000};
static const role_id_t pet_npc_id_range[2] = {7000000, 8000000};
static const role_id_t ai_event_id_range[2] = {8000000, 9000000};
static const role_id_t hero_role_id_range[2] = {9000000, 10000000};
static const role_id_t machine_role_id_range[2] = {10000000, 11000000};
static const role_id_t player_role_id_range[2] = {1000000000000000, 9999999999999999};

inline Player_Type get_player_type(role_id_t role_id) {
	if (role_id >= player_role_id_range[0] && role_id <= player_role_id_range[1])
		return Person_Type;
	else if (role_id >= material_role_id_range[0] && role_id <= material_role_id_range[1])
		return Material_Type;
	else if (role_id >= monster_role_id_range[0] && role_id <= monster_role_id_range[1])
		return Monster_Type;
	else if (role_id >= special_npc_id_range[0] && role_id <= special_npc_id_range[1])
		return Special_Monster_Type;
	else if (role_id >= player_npc_id_range[0] && role_id <= player_npc_id_range[1])
		return Player_Monster_Type;
	else if (role_id >= drops_role_id_range[0] && role_id <= drops_role_id_range[1])
		return Drops_Type;
	else if (role_id >= pet_role_id_range[0] && role_id <= pet_role_id_range[1])
		return Pet_Type;
	else if (role_id >= pet_npc_id_range[0] && role_id <= pet_npc_id_range[1])
		return Pet_Monster_Type;
	else if (role_id >= machine_role_id_range[0] && role_id <= machine_role_id_range[1])
		return Machine_Type;
	else
		return NON_Type;
};


/////////////////////

struct Tick_Time {
	Time_Value interval;
	Time_Value tick;

	Tick_Time(Time_Value inter = Time_Value::zero) {
		interval = inter;
		tick = Time_Value::zero;
	};
	void reset(void) {
		interval = Time_Value::zero;
		tick = Time_Value::zero;
	};
	inline void set_interval(const Time_Value &tv) {
		interval.set(tv.sec(), tv.usec());
	}
	inline void set_interval(int sec, int usec) {
		interval.set(sec, usec);
	}
	inline void set_tick_now(void) {
		tick = Time_Value::gettimeofday();
	};
	inline bool cycle_interval(void) {
		if (Time_Value::gettimeofday() - tick >= interval) return true;
		return false;
	}
};

struct AI_Running_Info {
	int id;							/// ID
	int path_index;

	Tick_Time tick_vision_;		  	///  视野tick
	Tick_Time tick_patrol_;   	 	///  巡逻频率

	void reset(void) {
		id = 0;
		path_index = 0;

		tick_vision_.reset();
		tick_patrol_.reset();
	}
};

struct NPC_Config_Cache {

	// need
	int id;											/// NPC ID
	std::string name;
	int level;
	int title_id;
	int jid;					  							// 模型
	int type;										//0:xiaoguai 1:boss
	int m_type;
	std::vector<int> race;
	std::vector<Int_Double> prop_value_vec;
	std::vector<int> skill_vec;
	std::vector<int> passive_skills;
	std::vector<int> battle_ai_vec;
	int drop_type;
	int drop_mult_mode;
	double drop_mult_coe;
	int drop_lv_limit;
	double drop_lv_coe;
	std::vector<int> drops_vec;
	int money;
	int exp;
	int soul;
	double res_mult_coe;
	int res_lv_limit;
	double res_lv_coe;
	std::vector<int> chat_vec;

//	int s_type;
//	int speed;
//	int camps;
//	int broadcast;
//	double experience;
//	Int_Set buff_set;
//	int default_skill;
//	Int_Set ext_skill_set;
//	Int_Set die_drops;
//	Int_Set trigger_set;
//	Int_Int_Map die_by_attack_nums;
//	int appear_effect;
//	int disappear_effect;
//	bool die_drop_type;
//	bool die_no_exp;
//	bool invincible;
//	bool die_repel;						// 是否可被击退
//	bool push_pull;						// 是否可被推&拉
//	int exp_attenuate;			  		// 经验衰减
//	bool is_skill_deplete;				// 技能是否消耗蓝
////	Fighter_Detail::Property property;
//	Time_Value skill_pause_time;
//
//	int times_per_player;				// 每个玩家可采次数
//	int collect_level;					// 可采集等级
//	int need_item_id;					// 需要道具才能采集
//	int collect_times;					// 采集物可被采集次数
//	int collect_jid;					// 集物物采集后变模型

	void reset(void);
};
typedef boost::unordered_map<int, NPC_Config_Cache> NPC_Config_Cache_Map;


struct NPC_Talk_Cache {
	int id;
	std::vector<std::string> content;

	void reset(void) {
		id = 0;
		content.clear();
	}
};
typedef boost::unordered_map<int, NPC_Talk_Cache> NPC_Talk_Cache_Map;

struct NPC_Detail {
	NPC_Detail(void) { reset(); };
	void reset(void);

	///  base
	role_id_t npc_id;
	int npc_type_id;
	int title_id;
	int operation_type;
	uint8_t operation_state;
	int state_type;
	int npc_status;
	int team_pos;
	int team_count;
	Time_Value birth_time;
	Time_Value life_time;
	Coord birth_coord;
	Coord chase_coord;
	role_id_t direct_master;	// 直接主人
	role_id_t master_master;	// 主人的主人
	role_id_t birth_by_skill;
	std::string master_name;
	int move_speed;
	std::vector<Int_Int> talk_vec;
	std::vector<Int_Int> chat_vec;
	std::vector<role_id_t> add_id;		// 附属id
	///  AI
	int ai_id;
	bool is_ai_start;
	//const AI_Info *ai_config;
	AI_Running_Info ai_running_info;
	Fighter_Detail fighter_detail_;
	Mover_Detail mover_detail_;
	bool check_ai_tick;
	Time_Value ai_tick_time;
};

struct Monster_Detail {
	enum Additional_Type {
		ADDITION_BIRTH    = 0,	/// 出生出现
		ADDITION_HIT_BACK = 1,	/// 瞬移 击退   [1]:skill_id [2]:start px [3]:start py [4]:end px [5]:end py
		ADDITION_QZHL     = 2,	/// 千转寒流 附加目标坐标 [1]:x [2]:y
	};

	int monster_status;
	role_id_t master_role_id;
	role_id_t caller_id;

	std::string name;
	std::string master_name;
	int jid;					  		// 模型
	int team_hatred_type;				// 队伍仇恨类型
//	Role_Vec additional_id;				// 附加id
	Coord drop_coord;		  		// 掉落坐标
	int speed[3];

//	Skill_Running_Info *default_skill;	// 默认技能
//	Skill_Running_Info *current_skill;
//	RoleID_Hatred_Vec hurt_export;
	const NPC_Config_Cache *config_cache;

	Monster_Detail(void);
	void reset(void);
};

struct Material_Detail {
	int jid;					  		// 模型
	std::string name;
	int type;
	int lv_limit;
	int gather_time;
	double rate;
	std::vector<int> drops_vec;
	std::vector<int> tasks_vec;

	void reset(void);
};

struct Machine_Detail {
	int jid;					  		// 模型
	std::string name;
	int moveable_index;
	int moveable_type;
	const NPC_Config_Cache *config_cache;
	void reset(void);
};

struct Drop_One {
	int32_t item;/*道具id*/
	int32_t amount;/*数量(金钱或道具)*/
	int8_t bind;
	int announce_type;
	Int_Set announce_item;

	Drop_One(void);
	void reset(void);
};
typedef std::vector<Drop_One> Drop_Vec;

struct Drop_Role_Detail {
	Drop_Role_Detail(void) { reset(); }
	void reset(void) {
		open = false;
		drops_vec.clear();
	}

	bool open;
	Drop_Vec drops_vec;
};
typedef boost::unordered_map<role_id_t, Drop_Role_Detail> Role_Drop_Map;

struct Drops_Detail {
	enum Drops_Status {
		DROPS_STATUS_NORMAL,
		DROPS_STATUS_GETTING,
	};

	bool open;
	int drop_type;					//
	std::string name;
	int8_t type;					/*掉落类型 0:宝箱掉落 1:散落掉落 2:自动掉落*/
	int32_t jid;					//
//	int16_t amount;				/*数量(金钱或道具)*/
//	int32_t item;					/*道具id*/
//	int32_t index;					/*标识(拾取时用)*/
//	int64_t visible;				/*可见role_id      当visible=0 或 visible=自己id时可见*/
//	int64_t belong;				/*可拾取role id    当belong=0 或 belong=自己id时可见*/
//	uint8_t collect_time;		/*采集时间*/
//	int8_t bind;					/*是否绑定*/

	Drop_Vec drops_vec;
	Drops_Status drops_status;
//	uint get_limit;
//	std::set<role_id_t> get_roles;
	role_id_t auto_owner;

	Role_Drop_Map role_drop_map;

	Drops_Detail(void);
	void reset(void);

	void set_normal_status(void);
	void set_getting_status(void);
	Drops_Status get_status(void);
};

// NPC附加信息
enum Role_Addition_Type {
	RAT_GATHER_MONSTER = 60,			// 采集怪
	RAT_DRIVING_MONSTER = 80,			// 驱赶怪
};

struct NPC_Addition_Info {
	NPC_Addition_Info(void);
	void reset(void);

	Coord birth_coord;					// 出生坐标[必须]
	std::string name;					// npc名[可选]
	int toward;							// 朝向[可选]
	int layer_id;						// 层[可选]
	int birth_by_skill;					// 由xx技能创建[可选]
	int jid;							// 模型[可选]
	int level;							// 等级[可选]
	int default_skill;					// 默认技能[可选]
	int ex_val1;
	int ex_val2;
	bool unmoveable_birth;				// 是否能在不可走点出生
	Int_Set ext_skill_set;				// 扩展技能[可选]
	Prop_Setter_Vec prop_vec;			// 战斗属性改变
//	Fighter_Detail::Property property;	// 属性[可选]
//	Role_Vec additional_id;				// 附加id[可选]
//	Friend_Foe_IDentify identify;		// 主人信息[可选]
//	Role_Addition_Vec additional_info;	// 附加信息
};

//
//struct NPC_Record_Info {
//	NPC_Record_Info(void);
//	void reset(void);
//
//	int id;
//	int scene_id;
//	int type;
//	int view_range;
//	int move_range;
//	int chase_range;
//	int speed[2];
//	Coord bird_coord;
//	std::vector<role_id_t> vec_npc;
//};
//typedef boost::unordered_map<int, NPC_Record_Info> NPC_Record_Info_Map;

struct NPC_Group {
	NPC_Group(void);
	void reset(void);
	Monster_Group_Config config;
};
typedef boost::unordered_map<int, NPC_Group> NPC_Group_Map;

struct NPC_Record {
	Monster_Point_Config config;
	int max_count;								/// NPC最大数量
	int  cur_ref_times;						/// NPC刷新平率(秒) -1不刷  0只刷一次
	Time_Value cur_ref_time;			/// 上次刷新时间，用于控制刷新频率

	int cur_gt_times;
	Time_Value cur_gt_cd;

	role_id_t birth_by;						/// 创建者
	Time_Value birth_freq;					/// NPC创建频率
	Time_Value birth_time;					/// 上次创建时间
	int team_hatred;						/// 小队仇恨
	int layer_id;

	Move_Scene *scene;
	Battle_Scene* battle_scene;

	RoleID_NPC_Map alive_npc_map;
	role_id_t leader_id;

	bool check_refresh;

	Fighter_Set fight_player_set;				//跟怪点战斗的玩家

	//
	int share_hp_cur;
	int share_hp_max;
	int share_hp_tick;
	Role_Id_Set battle_id_set;

	NPC_Record(void);
	void reset(void);
	int get_npc_num(void);
	void reset_scene(Move_Scene *scene);
	void tick(void);
	bool is_share_hp(void);
	void init_share_hp(void);
	void refresh_share_hp(void);
	void be_hurt_and_sub_blood(double sub_blood, double current_blood, role_id_t attacker_id);

	int remove_npc_from_alive(int npc_type_id, role_id_t npc_id);
	void record_recover_all_npc(void);
	void record_normal_die_all_npc(void);
	void npc_add_movevalue(int move_value);
	void npc_add_hatred(role_id_t fighter_id, double hurt_export);
	void npc_add_hatred(int npc_type_id, role_id_t fighter_id, double hurt_export);
	int get_team_hatred_type(int npc_type_id);

	int get_battle_group();
	void remove_point(const role_id_t battle_id = 0);
	void clear_point(const role_id_t battle_id = 0);
	void reset_point(const role_id_t battle_id = 0);
	int enter_battle(Scene_Player* player);
	int create_battle(Scene_Player* player);
	int join_battle(Fighter* fighter, int pos1, int pos2);
	Coord grid_coord();
	void set_npc_status(int status);
	void set_chase_target(role_id_t role_id);
	void clear_chase_target(void);
	void set_refresh(void);
	void set_cur_gt_cd(const Time_Value &now);
	bool check_gt_cd(const Time_Value &now);
	bool is_teams_up(void);
	//侦听玩家加入怪点战斗
	void hook_fighter_enter(Fighter* fighter);
	void hook_fighter_leave(Fighter* fighter);
	bool is_fighting(void);
	//
	int release(void);
};
typedef boost::unordered_map<uint64_t, NPC_Record> NPC_Record_Map;

struct NPC_Section {
	NPC_Record_Map record_map;
	NPC_Group_Map group_map;
	std::vector<int> path_guide_vec;
	bool is_team;

	std::vector<int> clear_point_vec;
	Move_Scene *scene_;

	NPC_Section(void);

	void reset(void);
	void reset_scene(Move_Scene *scene);
	void insert_record(uint64_t index, NPC_Record& record);
	void insert_group(int index, NPC_Group& group);
	void collect_npc_by_record_type(RoleID_NPC_Map &roleid_npc, int record_type = 0);
	void collect_npc_by_type_id(RoleID_NPC_Map &roleid_npc, int npc_type_id);

	void push_clear_point(int point);
	bool is_clear_point(int point);
	int get_next_path_guide(void);
	int get_monster_point_count(void);

	bool is_teams_up(void);
};


struct Attack_Args {
	int skill_id;
	Role_Vec target_id;
	Coord skill_coord;
	int skill_action;
	int step;

	int serialize(Block_Buffer &buf) const
	{
		buf.write_int32(skill_id);
		buf.write_uint16(target_id.size());
		for (Role_Vec::const_iterator it = target_id.begin(); it != target_id.end(); ++it) {
			buf.write_int64(*it);
		}
		skill_coord.serialize(buf);
		buf.write_int32(skill_action);
		buf.write_int32(step);
		return 0;
	}
	int deserialize(Block_Buffer &buf)
	{
		buf.read_int32(skill_id);
		uint16_t size = 0;
		role_id_t role_id = 0;
		target_id.clear();
		buf.read_uint16(size);
		for (uint16_t i = 0; i < size; ++i) {
			buf.read_int64(role_id);
			target_id.push_back(role_id);
		}
		skill_coord.deserialize(buf);
		buf.read_int32(skill_action);
		buf.read_int32(step);
		return 0;
	}
	void reset(void) {
		skill_id = 0;
		target_id.clear();
		skill_coord.reset();
		skill_action = 0;
		step = 0;
	}
};

struct Monster_Attack_Args {
	Attack_Args attack_args;
	Monster_Detail *monster_detail;

	Monster_Attack_Args(void) : monster_detail(0) {};
	Monster_Attack_Args(Monster_Detail *monster_detail) {
		attack_args.reset();
		this->monster_detail = monster_detail;
	}

	void reset(void) {
		attack_args.reset();
		monster_detail = 0;
	}
};

//struct AI_Skill_Running_Greater {
//	bool operator() (Skill_Running_Info *elem1, Skill_Running_Info *elem2) const {
//		return elem1->config_cache->cool.sec() > elem2->config_cache->cool.sec();
//	}
//};


//struct Hatred_Greater {
//	bool operator() (Hatred_Info *elem1, Hatred_Info *elem2) const {
//		return elem1->hurt_export > elem2->hurt_export;
//	}
//};

//struct Hatred_Value_Greater {
//	bool operator() (Hatred_Value elem1, Hatred_Value elem2) const {
//		return elem1.hatred_value > elem2.hatred_value;
//	}
//};
//struct Skill_Running_Args {
//	const Skill_Config_Cache *config_cache;
//	Skill_Running_Info *running_info;
//
//	Skill_Running_Args(void) {
//		reset();
//	}
//
//	void reset(void) {
//		config_cache = 0;
//		running_info = 0;
//	}
//};

// 仇恨值
struct Hatred_Info {
	role_id_t role_id;
	std::string role_name;

	double hurt_export;				///  对自己造成伤害输出
	Time_Value attack_start_time;   ///  第一次攻击时间

	Hatred_Info(void) : role_id(0), hurt_export(-1), attack_start_time(0) {}
	void reset(void) {
		role_id = 0;
		role_name = "";
		hurt_export = -1;
		attack_start_time = Time_Value::zero;
	}
};
typedef std::map<role_id_t, Hatred_Info> Hatred_Map;		/// 仇恨列表

struct Hatred_Value {
	role_id_t role_id;
	std::string role_name;
	double hurt_percent;
	double hatred_value;				///  仇恨值
};
typedef std::vector<Hatred_Value> RoleID_Hatred_Vec;

struct Monster_Wave_Info{
	std::vector<int> monster_vec;
	std::vector<int8_t> pos_vec;
	Int_Vec int_1;
	Role_Vec int64_1;
	Monster_Wave_Info(){
		reset();
	}
	void reset(){
		monster_vec.clear();
		pos_vec.clear();
		int_1.clear();
		int64_1.clear();
	}
};

struct Monster_Battle_Info{
	int group_id;
	Coord coord;
	int wave_amount;
	int cur_wave;
	std::vector<Monster_Wave_Info> wave_vec;
	std::vector<role_id_t> player_vec;
	int result;
	std::string start_plot_id;
	std::string end_plot_id;
	int start_plot_time;
	int end_plot_time;
	Monster_Chest_Info monster_chest_info;

	NPC_Record* record;
	Monster_Battle_Info(){
		reset();
	}
	void reset(){
		group_id = 0;
		coord.reset();
		wave_amount = 0;
		cur_wave = 0;
		wave_vec.clear();
		player_vec.clear();
		result = 0;
		record = 0;
		start_plot_id = "";
		end_plot_id = "";
		start_plot_time = 0;
		end_plot_time = 0;
		monster_chest_info.reset();
	}
};

struct Material_Point{
	Material_Point_Config config_info;
	Time_Value ref_time;
	Time_Value birth_freq;
	int cur_ref_times;
	Material_Point(void);
	void reset(void);
};
typedef boost::unordered_map<int, Material_Point> Material_Point_Map;

struct Material_Section{
	Material_Point_Map material_point_map;
	Material_Section(void);
	void reset(void);
};


struct Machine_Point{
	Machine_Point_Config config;
	Time_Value ref_time;
	Time_Value birth_freq;
	Time_Value tri_time;
	int cur_tri_times;
	int cur_ref_times;
	Machine_Point(void);
	void reset(void);
};
typedef boost::unordered_map<int, Machine_Point> Machine_Point_Map;

struct Machine_Section{
	Machine_Point_Map point_map;
	Machine_Section(void);
	void reset(void);
};

struct Monster_AI_Info{
	Monster_AI_Config* config;
	int cur_Times;
	Time_Value last_time;
	Time_Value valid_time;
	bool is_valid;
	bool is_recover;
	bool has_trigger;					//触发
	Monster_AI_Info(void);
	Monster_AI_Info(const Monster_AI_Info& info);
	void reset(void);
	void trigger(const Time_Value &now);
	bool can_trigger(void);
};

struct Create_Monster_Argc {
	Create_Monster_Argc(void) { reset(); }
	void reset(void) {
		i_v1 = 0;
		i_v2 = 0;
		i64_v1 = 0;
	}
	int i_v1;
	int i_v2;
	int64_t i64_v1;
};
typedef std::vector<Create_Monster_Argc> CreateMonsterArgv_Vec;
#endif /* NPC_STRUCT_H_ */
