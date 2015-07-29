/*
 * Scene_Struct.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *      只在场景服用
 */

#ifndef SCENE_STRUCT_H_
#define SCENE_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "Scene_Def.h"

class Mover;

//////////////////////////////////////////////////////////////////////////////

typedef std::vector<int16_t> Map_Content;



struct Path_Node {
	Coord cur_coord;
	Coord prev_coord;
	int g_val, h_val;
	bool valid_node;

	Path_Node(void);
	Path_Node(int g, int h, Coord cur = Coord(), Coord prev = Coord());
	inline void reset(void);

	void set(int g, int h, Coord cur = Coord(), Coord prev = Coord());

	inline int f(void) const { return g_val + h_val; }

	bool operator==(const Path_Node &node) {
		return cur_coord == node.cur_coord;
	}

	class greater {
	public:
		inline bool operator()(const Path_Node *a, const Path_Node *b) {
			return (a->f()) > (b->f());
		}
	};

	bool operator<(const Path_Node &node) const {
		return f() < node.f();
	}
};

struct Battle_Fighter {
	int p_id;
	Mover *mover;

	Battle_Fighter(void);
	void reset(void);
};

struct Map_Head {
	int16_t cell_width;		/// 格子长宽
	int16_t cell_height;
	int16_t x_len;			/// 格子数
	int16_t y_len;
	int16_t map_width;		/// 地图长宽(像素)
	int16_t map_height;
	int16_t i_point_y;

	Map_Head(void) { reset(); };
	void reset(void);
};

struct Scene_Map_Data {
	int id;
	Map_Head head;
	Map_Content content;

	Scene_Map_Data(void) { reset(); };
	void reset(void);
};

struct Safe_Data {
	Safe_Data(void);

	Coord coord;
	Mover_Set mover_set;
	int range_type;
	int distance;
};
typedef std::vector<Safe_Data> Scene_Safe_Area;

////////////////////////////////////////////////////////////////////////////////////////

struct Scene_Config {
	struct Coord_Value {
		int scene_id;
		Coord coord;
		int value;
		Coord_Vec coord_vec;
		Coord_Value(void) { reset(); };
		void reset(void);
	};

	struct Transmit_Coord {
		int src_scene_id;
		Coord coord;
		Transmit_Coord(void) { reset(); };
		void reset(void);
	};
	struct Dungeon_Config_Detail {
		typedef boost::unordered_map<int, int> OpenConMap;
		typedef boost::unordered_map<int, int> CostMap;
		typedef boost::unordered_map<int, int> VipExtMap;
		enum {
			CANNOT_TEAM = 0,
			CAN_TEAM = 1,
		};
		enum{
			COST_PHY_POWER 	= 1,     //消耗体力
			COST_ENTER_NUMBER = 2,		//消耗进入次数
			COST_PROP			= 3,		//消耗道具
			COST_RESOURCE		= 4,     //消耗资源
		};
		enum{
			DUNGEON_DIFF_NOR = 1,      // 普通
			DUNGEON_DIFF_HOR = 2,      // 噩梦
		};
		Dungeon_Config_Detail(void) { reset(); }
		void reset(void) {
			id 			= 0;
			lv 			= 0;
			difficulty = 0;
			pass_con 		= 0;
			team_lim 		= 0;
			relation = 0;
			next_id 		= 0;
			elite = 0;

			be_enter		= 0;
			be_mop		= 0;
			can_team = 0;
			require_task = 0;

			name.clear();
			open_con_map.clear();
			cost_map.clear();

			first_layer_id = 0;
			pre_id = 0;
		}
		int get_dungeon_lv_limit(void){
			return lv;
		}
		bool is_cost_times(void) const{
			CostMap::const_iterator it = cost_map.find(2);
			if(it != cost_map.end()){
				if(it->second > 0){
					return true;
				}
				return false;
			}
			return false;
		}
		int id;
		int lv;
		int difficulty;		// 难度
		int pass_con;
		int team_lim;
		int relation;			//多层副本下层副本
		int next_id;
		int elite;				// 精英副本对应的单人副本

		int be_enter;
		int be_mop;
		int can_team;
		int require_task;			// 进入需要完成的任务

		std::string name;
		OpenConMap open_con_map;
		CostMap cost_map;
		VipExtMap vip_ext_map;

		//
		int first_layer_id;			// 多层副本的首层副本id
		int pre_id;						// 前一个副本id
	};

	typedef std::vector<Int_Int> Move_Value;
	typedef std::vector<Transmit_Point> Transmit_Point_Vec;
	typedef boost::unordered_map<int, Transmit_Coord> Transmit_Coord_Map;

	int scene_id;
	int order_total;
	int scene_block_x;
	int scene_block_y;
	int role_limit;
	int inter_order;		// 如果是跨服场景要强制order, 每个服这个值可能不一样
	Scene_Type type;
	int manager;
	std::string scene_name;
	Transmit_Point_Vec transmit_point;
	Transmit_Coord_Map transmit_coord;
	Coord_Vec revive_point;
	Int_Set des_scene_limit;
	Time_Value scene_keep_time;
	Move_Value move_value;
	bool login_create;
	int8_t timeLimit; // 是否限时击杀;相应处理：进入时，自动组队

	int relive_protect_time;			// 复活后加保护时间
	double relive_per;					// 复活后回血百分比
	int map_id;							// 地图id
	int pet_back;					// 是否回收宠物
	int mount_back;					// 是否下坐骑
	int peace;						// 是否是和平场景
	double restore_status;			// 退出异空间回血量
	int fly_sky;					// 是否可飞行
	int enter_level;				// 进入等级
	int relive_drug_type;			// 每次复活复活珠累加多少
	int same_scene_forbid_cloud;	// 同场景是否不可使用腾云符
	int same_scene_transfer_inner;	// 同场景传送时是否走简单传送 0 简单传送 1 走传送流程
	int trace_scene_level;			// 回滚场景级别0 1 2
	int exp;						// 每隔一段时间加经验数
	Time_Value exp_add_gap;			// 增加经验的间隔
	Int_Int_Map exp_add_map;		// 等级-增加经验map
	bool hero_disable;

	Dungeon_Config_Detail dungeon; //副本

	void reset(void);
	bool is_elite_scene(void) const;				// 上古副本
};
typedef boost::unordered_map<int, Scene_Config> Scene_Config_Map_Cache;
typedef boost::unordered_map<int, Scene_Map_Data> Scene_Map_Data_Map_Cache;

////////////////////////////////////////////////////////////////////////////////////////////////


#endif /* SCENE_STRUCT_H_ */
