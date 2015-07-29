/*
 * Move_Scene_Base.h
 *
 *  Created on: May 11, 2012
 *      Author: Ti
 *
 */

#ifndef MOVE_SCENE_BASE_H_
#define MOVE_SCENE_BASE_H_

#include "Scene_Def.h"
#include "Scene_Func.h"
#include "Scene_Struct.h"
#include "mover/Mover.h"

class Mover;
class Fighter;

typedef bool (*Castable_Judge_Func)(Fighter &active_fighter, Fighter &passive_fighter, int extra_param);
const int DEFAULT_COLLECT_NUMS = 40;

const static int toward_direction[9][2] = {	/// 方位相对坐标, 用toward_up/toward_right_up/toward_right... 索引
	{0, 0},			/// null stub
	{0, -1},		/// 上
	{1, -1},		/// 右上
	{1, 0},			/// 右
	{1, 1},			/// 右下
	{0, 1},			/// 下
	{-1, 1},		/// 左下
	{-1, 0},		/// 左
	{-1, -1}		/// 左上
};

class Move_Scene_Base {
public:

	enum {
		UNMOVE_ABLE_VAL = 0,
		MOVE_ABLE_FROM 	= 1,
		MOVE_ABLE_WATER_7 = 7,
		MOVE_ABLE_WATER_8 = 8,
		MOVE_ABLE_END  	= 30,
		UNABLE_MOVE		= 0,
	};

	/// 位置相关
	typedef boost::unordered_map<role_id_t, Mover *> Mover_Map;
	typedef boost::unordered_map<role_id_t, size_t> Mover_Offset_Map;
	typedef std::vector<uint16_t> Grid_Mover_Count;

	/// 寻路相关
	typedef std::map<Coord, Path_Node> Path_Node_Map;
	typedef std::set<Path_Node> Path_Node_Set;

	/// 方向相关
	typedef boost::unordered_set<uint16_t> Coord_Set;
	typedef std::pair<uint16_t, uint16_t> Offset_Coord;
	typedef std::vector<std::pair<int, int> > Offset_Coord_Vec;

	/// 九宫格块
	struct Scene_Block {
		uint16_t block_idx_;
		Mover_Map block_player_map_;
		Mover_Map block_mover_map_;
		Block_Buffer block_buf_;
		Mover_Offset_Map offset_map_;

		Scene_Block(void)
		: block_idx_(0),
		  block_buf_(0)
		{ }

		void reset(void);

		void flush_reset(void);

		void flush(bool scene_limit = true);

		/**
		 * 在玩家跨块时调用
		 * 防止玩家在跨块的时候丢失掉他原来所在块的部分数据
		 */
		void flush_to_mover(Mover &mover);

		bool bind_mover(Mover *mover);

		bool unbind_mover(Mover *mover);

		void push_data(Block_Buffer &buf);

		void get_block_appear_msg(Block_Buffer &buf);

		int get_block_disappear_msg(Block_Buffer &buf);

		bool no_player(void);
	};

	typedef std::vector<Scene_Block> Scene_Block_Vec;
	typedef std::vector<Int_Int> Move_Value;

	struct Scene_Layer {
		enum {
			FREE_LAYER,
			USED_LAYER,
		};
		size_t layer_id_;
		int layer_flag_;
		Mover_Map mover_map_;
		Grid_Mover_Count grid_mover_count_, grid_npc_count_, grid_player_count_;
		Scene_Block_Vec scene_block_vec_;
		Time_Value next_flush_ts_;
		Role_Int_Map relive_bead_use;
		Move_Value move_value;

		Scene_Layer(void)
		: layer_id_(0), layer_flag_(FREE_LAYER), mover_map_(1500) { }
	};

	typedef std::vector<Scene_Layer> Scene_Layer_Vec;
	typedef std::set<size_t> Scene_Layer_ID_Set;

	/// 区块大小
	//static const int block_width_ 		= 13; /// 17 grid
	//static const int block_height_ 		= 16; /// 16

	static const int grid_coord_flag	= 1;
	static const int pix_coord_flag		= 2;

	/**
	 * toward label:
	 *
	 * +----+----+----+
	 * | 8  | 1  | 2  |
	 * +----+----+----+
	 * | 7  | *  | 3  |
	 * +----+----+----+
	 * | 6  | 5  | 4  |
	 * +----+----+----+
	 *
	 */
	static const int toward_null 		= 0;
	static const int toward_up 			= 1;
	static const int toward_right_up 	= 2;
	static const int toward_right 		= 3;
	static const int toward_right_down 	= 4;
	static const int toward_down 		= 5;
	static const int toward_left_down 	= 6;
	static const int toward_left 		= 7;
	static const int toward_left_up 	= 8;

	/// 方向坐标偏移
	static const std::pair<int, int> toward_null_offset;			/// 0
	static const std::pair<int, int> toward_up_offset;				/// 1
	static const std::pair<int, int> toward_right_up_offset;		/// 2
	static const std::pair<int, int> toward_right_offset;			/// 3
	static const std::pair<int, int> toward_right_down_offset;		/// 4
	static const std::pair<int, int> toward_down_offset;			/// 5
	static const std::pair<int, int> toward_left_down_offset;		/// 6
	static const std::pair<int, int> toward_left_offset;			/// 7
	static const std::pair<int, int> toward_left_up_offset;			/// 8

	struct Toward_Offset {
		Offset_Coord_Vec disappear_coord;	/// "始"块坐标加该偏移为广播消失块
		Offset_Coord_Vec move_coord;		/// "始"块坐标加该偏移为广播移动块
		Offset_Coord_Vec appear_coord;		/// "终"块坐标加该偏移为广播出现块

		void reset(void) {
			disappear_coord.clear();
			move_coord.clear();
			appear_coord.clear();
		}
	};

	typedef std::vector<Toward_Offset> Toward_Offset_Vec;

	Move_Scene_Base(void);

	virtual ~Move_Scene_Base(void);

	virtual void reset(void);

	const Scene_Config *scene_config();

	virtual int init_scene_normal(int scene_id, int scene_order, int map_id = 0);

	virtual int client_scene_complete(Fighter &fighter);

	void init_toward_offset(void);

	int init_map_block(void);

	int init_save_area(int scene_id);

	int init_move_value(void);

	virtual int change_move_value(int16_t mp_value, bool moveable);

	void flush_tick(const Time_Value &now);

	/// 强制推送所有块内的广播数据(用于副本回收场景时将广播数据全部发送出去, 仅可用于人工调用)
	void force_flush_tick(size_t layer_id, uint block_index);

	virtual int tick(const Time_Value &now);

	size_t pop_layer(void);

	int push_layer(size_t layer_id);

	int scene_id(void);

	int scene_order(void);

	virtual int extra_id(void) { return 0; }

	Scene_Type scene_type(void);

	virtual int enter_scene(Mover *mover);
	virtual int exit_scene(Mover *mover, bool offline = true);
	virtual int move(Mover *mover, uint16_t x, uint16_t y, int coord_flag = pix_coord_flag);

	int appear(Mover *mover);

	int disappear(Mover *mover);

	int correct_mover_layer(Mover *mover);

	/// 向周围广播
	int broadcast_around(size_t layer_id, int block_index, Block_Buffer &buf);
	int broadcast_around_s(size_t layer_id, int block_index, Block_Buffer &buf);

	/// 全地图广播(该接口会大量耗费地图服务器资源, 慎用)
	inline int broadcast_all(size_t layer_id, Block_Buffer &buf);
	inline int broadcast_all_s(size_t layer_id, Block_Buffer &buf);
	inline int broadcast_all_within_battle(size_t layer_id, Block_Buffer &buf); //(含战斗)

	// 全地图聊天广播(含战斗)
	inline int broadcast_chat_all(size_t layer_id, Block_Buffer &buf);


	int path_find(size_t layer_id, Coord &src,
			Coord &dst,
			Coord_Vec &path,
			Int_Int &moveable_value,
			bool mover = true,
			bool obstacle = true);

	/// 视线寻路法(DDA直线算法)
	int path_find_sight_line(size_t layer_id, Coord &src_coord,
			Coord &dst_coord,
			Coord_Vec &path,
			Int_Int &moveable_value,
			bool mover,
			bool obstacle);

	/// A星寻路法
	int path_find_a_star(size_t layer_id, Coord &src_coord,
			Coord &dst_coord,
			Coord_Vec &path,
			Int_Int &moveable_value,
			bool mover,
			bool obstacle, int max_find = INT_MAX);

	/// A星寻路法
	bool path_arrive_a_star(size_t layer_id, Coord &src_coord,
			Coord &dst_coord,
			Int_Int &moveable_value,
			bool mover,
			bool obstacle);

	Mover *find_mover_with_layer(size_t layer_id, role_id_t role_id);

	const Map_Head &head(void);

	const Map_Content &content(void);

	int grid_mover_count(Coord &coord, size_t layer_id = 0, int mover_type = 0);

	/// 根据坐标获取mpt值 大于等于5可走
	int mpt_value_by_coord(uint16_t x, uint16_t y);

	/// 根据坐标收集格子范围内mover
	int collect_grid_mover(size_t layer_id, const Coord &coord, Mover_Set &mover, int range);
	int collect_grid_fighter(Fighter &active, Castable_Judge_Func judge_func, int extra_param, int max_collect,
			const Coord &coord, Fighter_Set &passive, int range);

	/// 对全场景内mover
	int collect_scene_fighter(Fighter &active, Castable_Judge_Func judge_func, int extra_param, int max_collect,
			Fighter_Set &passive);

	/// 获得某个NPC的子怪
	int collect_npc_children(role_id_t npc_role_id, std::vector<Mover> &children);

	/// 根据坐标收集圆形范围内mover
	int collect_round_mover(size_t layer_id, const Coord &coord, Mover_Set &mover, int range);
	int collect_round_player(size_t layer_id, const Coord &coord, Scene_Player_Set &players, int range);
	int collect_round_fight_player(size_t layer_id, const Coord &coord, Scene_Player_Set &players, int range);
	int collect_round_fighter(Fighter &active, Castable_Judge_Func judge_func, int extra_param, int max_collect,
			 const Coord &coord, int range, Fighter_Set &passive);

	/// 根据坐标收集椭圆范围内mover, mover_type = mover类型  distance < min(block_width_, block_height_)
	int collect_oval_mover(size_t layer_id, const Coord &coord, Mover_Set &mover, int range, int focus);
	int collect_oval_fighter(Fighter &active, Castable_Judge_Func judge_func, int extra_param, int max_collect,
			const Coord &coord, int range, int focus, Fighter_Set &passive);

	/// 收集矩形内的mover
	//int collect_square_mover(size_t layer_id, const Pixel_Rect_Range &rect, Mover_Set &mover);
	//int collect_square_fighter(Fighter &active, Castable_Judge_Func judge_func, int extra_param, int max_collect,
	//		const Pixel_Rect_Range &rect, Fighter_Set &passive);

	/// 收集扇形内的mover degree 为角度
	int collect_sector_mover(size_t layer_id, const Coord &orgin, const Coord &arc_mid, int degree, Mover_Set &mover, bool check_around = true);
	int collect_sector_fight_player(size_t layer_id, const Coord &orgin, const Coord &arc_mid, int degree, Scene_Player_Set &players, bool check_around = true);
	int collect_sector_fighter(Fighter &active, Castable_Judge_Func judge_func, int extra_param, int max_collect,
			const Coord &orgin, const Coord &arc_mid, int degree, Fighter_Set &passive);

	/// 随机获取coord周围无物体坐标
	int collect_available_coord(size_t layer_id, Coord &coord, Coord &available_coord, int distance_from = 0, int distance_to = 5);

	/// 获取coord一定范围内有多少mover
	int collect_mover_count(size_t layer_id, Coord &coord, int distance_x, int distance_y);

	/// coord一定范围内是否有mover
	bool calculate_coord_has_mover(size_t layer_id, Coord &coord, int distance_from = 0, int distance_to = 3);

	/// 根据移动格子(gird)坐标计算移动朝向
	int calculate_toward_by_grid_coord(uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y);
	int calculate_toward_by_coord_s(uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y);

	/// 验证坐标、下标是否合法
	bool validate_grid_coord(uint16_t x, uint16_t y);

	int mover_change_coord(Mover *mover, Coord &new_coord, bool cal_toward = true);

	int broadcast_mover_change_coord(Mover *mover,
			const Coord &new_coord,
			Block_Buffer *disappear_msg_buf,
			Block_Buffer *move_msg_buf,
			Block_Buffer *appear_msg_buf);

	// DDA画线
	int calc_point_in_line_by_direction(const Mover &mover, const Coord &pointStart, int real_dx, int real_dy, Coord &pointC, bool check_move_able);

	// 根据距离计算出同一条直线上的点C，遇到不可走点要停止
	int calc_point_in_line_by_b(const Mover &mover, const Coord &pointA, const Coord &pointB, int distance, Coord &pointC, int toward, bool check_move_able = true, bool from_end_point = true);
	// A为起始点
	int calc_point_in_line_by_a(const Mover &mover, const Coord &pointA, const Coord &pointB, int distance, Coord &pointC, int toward, bool check_move_able = true);
	// 一个较靠近B的点
	int calc_point_in_line_near_b(const Mover &mover, const Coord &pointA, const Coord &pointB, Coord &pointC, bool check_move_able = true);


	bool is_in_scene(size_t layer_id, role_id_t role_id);

	// 在场景内取一个随机可走点
	bool get_rand_coord(Coord &coord);

	Scene_Layer &first_scene_layer(void);

	Scene_Layer *scene_layer(size_t layer_id);
	
	bool validate_layer_id(size_t layer_id);

	bool validate_pix_coord(int pix_x, int pix_y);

	/// 根据格子(grid)坐标计算区块(block)下标
	int calculate_block_index_by_grid_coord(uint16_t grid_x, uint16_t grid_y, uint16_t &block_index);
	int calculate_block_index_by_grid_coord_s(uint16_t grid_x, uint16_t grid_y, uint16_t &block_index);

	virtual int update_move_able_info(Mover* mover);
protected:
	size_t add_new_scene_layer(void);
private:
	/// 判断around里的区块是否有玩家, 没有玩家 返回true, 有玩家 返回false
	bool no_player_around(size_t layer_id, std::vector<int> &around);
	bool no_player_around_s(size_t layer_id, std::vector<int> &around);

	void init_scene_layer(size_t layer_id, Scene_Layer &scene_layer);

	/// 根据格子(grid_coord)坐标计算格子下标(grid_index)
	int calculate_grid_index_by_grid_coord(uint16_t grid_x, uint16_t grid_y, uint32_t &grid_index);
	int calculate_grid_index_by_grid_coord_s(uint16_t grid_x, uint16_t grid_y, uint32_t &grid_index);

	/// 根据格子(grid)坐标计算区块(block)坐标
	int calculate_block_coord_by_grid_coord(uint16_t grid_x, uint16_t grid_y, uint16_t &block_x, uint16_t &block_y);
	int calculate_block_coord_by_grid_coord_s(uint16_t grid_x, uint16_t grid_y, uint16_t &block_x, uint16_t &block_y);

	/// 根据像素(pixel)坐标计算格子(grid)坐标
	int calculate_grid_coord_by_pix_coord(uint16_t pix_x, uint16_t pix_y, uint16_t &grid_x, uint16_t &grid_y);
	int calculate_grid_coord_by_pix_coord_s(uint16_t pix_x, uint16_t pix_y, uint16_t &grid_x, uint16_t &grid_y);

	/// 根据区块(block)下标计算周围区块(block)下标
	int calculate_around_block_by_block_index(int center_idx, std::vector<int> &block_set);
	int calculate_around_block_by_block_index_s(int center_idx, std::vector<int> &block_set);

	/// 根据区块(block)坐标计算周围区块(block)下标
	int calculate_around_block_by_block_coord(int block_x, int block_y, std::vector<int> &block_set);
	int calculate_around_block_by_block_coord_s(int block_x, int block_y, std::vector<int> &block_set);

	/// 根据区块(block)坐标计算区块(block)下标
	int calculate_block_index_by_block_coord(uint16_t block_x, uint16_t block_y, uint16_t &block_index);
	int calculate_block_index_by_block_coord_s(uint16_t block_x, uint16_t block_y, uint16_t &block_index);

	/// 根据格子下标(grid_index)计算格子坐标(grid_coord)
	int calculate_grid_coord_by_grid_index(int grid_idx, int &grid_x, int &grid_y);
	int calculate_grid_coord_by_grid_index_s(int grid_idx, int &grid_x, int &grid_y);

	/// 跨区块计算消失、移动、出现区块
	int calculate_diff_block_broadcast_by_block_coord(
			uint16_t start_block_x,
			uint16_t start_block_y,
			uint16_t end_block_x,
			uint16_t end_block_y,
			Coord_Set &disappear_set,
			Coord_Set &move_set,
			Coord_Set &appear_set);

	void calculate_block_coord_set_by_offset(uint16_t block_x,
			uint16_t block_y,
			Offset_Coord_Vec &offset_vec,
			Coord_Set &coord_set);

	bool validate_grid_index(int grid_index);

	bool validate_block_coord(int block_x, int block_y);
	bool validate_block_coord_s(int block_x, int block_y);

	bool validate_block_index(int block_idx);

	bool validate_move_coord(int coord_flag, int x, int y);

protected:
	int scene_id_;
	int scene_order_;
	int map_id_;
	int scene_config_id_;
	const Scene_Config *scene_config_;
	std::string scene_name_;
	int block_width_, block_height_; /// 九宫格大小(配置文件)
	int block_x_number_, block_y_number_; /// 九宫格个数
	const Scene_Map_Data *mpt_data_;
	Toward_Offset_Vec toward_offset_vec_;
	Scene_Layer_Vec scene_layer_vec_;
	Scene_Layer_ID_Set used_layer_set_;
	Scene_Layer_ID_Set free_layer_set_;

	Time_Value safe_area_time_;
	Scene_Safe_Area safe_area_;

	int log_scene_id_;
	Time_Value log_reset_time_;
};

///////////////////////////////////////////////////////////////////////////////

inline const Scene_Config *Move_Scene_Base::scene_config() {
	return scene_config_;
}

inline void Move_Scene_Base::reset(void) {
	if (scene_id_) {
		log_scene_id_ = scene_id_;
		log_reset_time_ = Time_Value::gettimeofday();
	}

	scene_id_ = 0;
	scene_order_ = 0;
	map_id_ = 0;
	scene_config_ = 0;
	scene_config_id_ = 0;
	scene_name_ = "";
	block_x_number_ = block_y_number_ = 0;
	/// toward_offset_vec_.clear(); 这个不用reset !
	CLEAR_VECTOR(Scene_Layer_Vec, scene_layer_vec_);
	CLEAR_VECTOR(Scene_Safe_Area, safe_area_);

	used_layer_set_.clear();
	free_layer_set_.clear();
	safe_area_time_ = Time_Value::zero;
}

inline int Move_Scene_Base::scene_id(void) {
	return scene_id_;
}

inline int Move_Scene_Base::scene_order(void) {
	return scene_order_;
}

inline Scene_Type Move_Scene_Base::scene_type(void) {
	return get_scene_type(scene_id_);
}

inline int Move_Scene_Base::calculate_toward_by_grid_coord(uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y) {
	if (old_x < 0 || old_y < 0 || new_x < 0 || new_y < 0) {
		MSG_USER("invalid coord\n");
		return -1;
	}

	if (new_x < old_x) {
		if (new_y < old_y) {
			return toward_left_up;
		} else if (new_y == old_y) {
			return toward_left;
		} else if (new_y > old_y) {
			return toward_left_down;
		} else {
			MSG_USER("old_x = %h, old_y = %h, new_x = %h, new_y = %h", old_x, old_y, new_x, new_y);
			return toward_null;
		}
	} else if (new_x == old_x) {
		if (new_y < old_y) {
			return toward_up;
		} else if (new_y == old_y) {
			MSG_USER("old_x = %d, old_y = %d, new_x = %d, new_y = %d", old_x, old_y, new_x, new_y);
			return toward_null;
		} else if (new_y > old_y) {
			return toward_down;
		} else {
			MSG_USER("old_x = %h, old_y = %h, new_x = %h, new_y = %h", old_x, old_y, new_x, new_y);
			return toward_null;
		}
	} else if (new_x > old_x) {
		if (new_y < old_y) {
			return toward_right_up;
		} else if (new_y == old_y) {
			return toward_right;
		} else if (new_y > old_y) {
			return toward_right_down;
		} else {
			MSG_USER("old_x = %h, old_y = %h, new_x = %h, new_y = %h", old_x, old_y, new_x, new_y);
			return toward_null;
		}
	} else {
		MSG_USER("old_x = %h, old_y = %h, new_x = %h, new_y = %h", old_x, old_y, new_x, new_y);
		return toward_null;
	}
}

inline int Move_Scene_Base::calculate_toward_by_coord_s(uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y) {
	if (new_x < old_x) {
		if (new_y < old_y) {
			return toward_left_up;
		} else if (new_y == old_y) {
			return toward_left;
		} else if (new_y > old_y) {
			return toward_left_down;
		} else {
			MSG_USER("old_x = %d, old_y = %d, new_x = %d, new_y = %d", old_x, old_y, new_x, new_y);
			return toward_null;
		}
	} else if (new_x == old_x) {
		if (new_y < old_y) {
			return toward_up;
		} else if (new_y == old_y) {
			MSG_USER("old_x = %d, old_y = %d, new_x = %d, new_y = %d", old_x, old_y, new_x, new_y);
			return toward_null;
		} else if (new_y > old_y) {
			return toward_down;
		} else {
			MSG_USER("old_x = %d, old_y = %d, new_x = %d, new_y = %d", old_x, old_y, new_x, new_y);
			return toward_null;
		}
	} else if (new_x > old_x) {
		if (new_y < old_y) {
			return toward_right_up;
		} else if (new_y == old_y) {
			return toward_right;
		} else if (new_y > old_y) {
			return toward_right_down;
		} else {
			MSG_USER("old_x = %d, old_y = %d, new_x = %d, new_y = %d", old_x, old_y, new_x, new_y);
			return toward_null;
		}
	} else {
		MSG_USER("old_x = %d, old_y = %d, new_x = %d, new_y = %d", old_x, old_y, new_x, new_y);
		return toward_null;
	}
}

inline int Move_Scene_Base::calculate_grid_index_by_grid_coord(uint16_t grid_x, uint16_t grid_y, uint32_t &grid_index) {
	if (! validate_grid_coord(grid_x, grid_y)) {
		MSG_USER("invalid coord");
		return -1;
	}
	return calculate_grid_index_by_grid_coord_s(grid_x, grid_y, grid_index);
}

inline int Move_Scene_Base::calculate_grid_index_by_grid_coord_s(uint16_t grid_x, uint16_t grid_y, uint32_t &grid_index) {
	grid_index = grid_y * mpt_data_->head.x_len + grid_x;
	return 0;
}

inline int Move_Scene_Base::calculate_block_index_by_grid_coord(uint16_t grid_x, uint16_t grid_y, uint16_t &block_index) {
	if (! validate_grid_coord(grid_x, grid_y)) {
		MSG_USER("invalid coord");
		return -1;
	}
	uint16_t bx = 0, by = 0;
	if (calculate_block_coord_by_grid_coord(grid_x, grid_y, bx, by) != 0)
		return -1;
	block_index = by * block_x_number_ + bx;
	return 0;
}

inline int Move_Scene_Base::calculate_block_index_by_grid_coord_s(uint16_t grid_x, uint16_t grid_y, uint16_t &block_index) {
	uint16_t bx = 0, by = 0;
	calculate_block_coord_by_grid_coord_s(grid_x, grid_y, bx, by);
	block_index = by * block_x_number_ + bx;
	return 0;
}

inline int Move_Scene_Base::calculate_block_coord_by_grid_coord(uint16_t grid_x, uint16_t grid_y, uint16_t &block_x, uint16_t &block_y) {
	if (! validate_grid_coord(grid_x, grid_y)) {
		MSG_USER("invalid coord\n");
		return -1;
	}
	return calculate_block_coord_by_grid_coord_s(grid_x, grid_y, block_x, block_y);
}

inline int Move_Scene_Base::calculate_block_coord_by_grid_coord_s(uint16_t grid_x, uint16_t grid_y, uint16_t &block_x, uint16_t &block_y) {
	block_x = grid_x / block_width_;
	block_y = grid_y / block_height_;
	return 0;
}

inline int Move_Scene_Base::calculate_grid_coord_by_pix_coord(uint16_t pix_x, uint16_t pix_y, uint16_t &grid_x, uint16_t &grid_y) {
	if (! validate_pix_coord(pix_x, pix_y)) {
		MSG_USER("invalid coord\n");
		return -1;
	}
	grid_x = (pix_x / mpt_data_->head.cell_width);
	grid_y = (pix_y / mpt_data_->head.cell_height);
	return 0;
}

inline int Move_Scene_Base::calculate_grid_coord_by_pix_coord_s(uint16_t pix_x, uint16_t pix_y, uint16_t &grid_x, uint16_t &grid_y) {
	grid_x = (pix_x / mpt_data_->head.cell_width);
	grid_y = (pix_y / mpt_data_->head.cell_height);
	return 0;
}

inline int Move_Scene_Base::calculate_around_block_by_block_index(int center_idx, std::vector<int> &block_set) {
	if (! validate_block_index(center_idx)) {
		MSG_USER("invalid block_index\n");
		return -1;
	}
	return calculate_around_block_by_block_coord((center_idx % block_x_number_), (center_idx / block_x_number_), block_set);
}

inline int Move_Scene_Base::calculate_around_block_by_block_index_s(int center_idx, std::vector<int> &block_set) {
	return calculate_around_block_by_block_coord_s((center_idx % block_x_number_), (center_idx / block_x_number_), block_set);
}

inline int Move_Scene_Base::calculate_around_block_by_block_coord(int block_x, int block_y, std::vector<int> &block_set) {
	if (! validate_block_coord(block_x, block_y)) {
		MSG_USER("invalid coord\n");
		return -1;
	}
	return calculate_around_block_by_block_coord_s(block_x, block_y, block_set);
}

inline int Move_Scene_Base::calculate_around_block_by_block_coord_s(int block_x, int block_y, std::vector<int> &block_set) {
	int pos[9][2];
	pos[0][0] = block_x; 			pos[0][1] = block_y;		/// 中心
	pos[1][0] = block_x + 1;  		pos[1][1] = block_y; 		/// 右
	pos[2][0] = block_x + 1;		pos[2][1] = block_y + 1;	/// 右下
	pos[3][0] = block_x;			pos[3][1] = block_y + 1;	/// 下
	pos[4][0] = block_x - 1,		pos[4][1] = block_y + 1;	/// 左下
	pos[5][0] = block_x - 1,		pos[5][1] = block_y;		/// 左
	pos[6][0] = block_x - 1,		pos[6][1] = block_y - 1;	/// 左上
	pos[7][0] = block_x,			pos[7][1] = block_y - 1;	/// 上
	pos[8][0] = block_x + 1,		pos[8][1] = block_y - 1;	/// 右上
	for (int i = 0; i < 9; ++i) {
		if (pos[i][0] < 0 || pos[i][0] >= block_x_number_ || pos[i][1] < 0 || pos[i][1] >= block_y_number_)
			continue;
		block_set.push_back(pos[i][1] * block_x_number_ + pos[i][0]);
	}
	return 0;
}

inline int Move_Scene_Base::calculate_block_index_by_block_coord(uint16_t block_x, uint16_t block_y, uint16_t &block_index) {
	if (! validate_block_coord(block_x, block_y)) {
		MSG_USER("invalid coord");
		return -1;
	}
	block_index = block_y * block_x_number_ + block_x;
	return 0;
}

inline int Move_Scene_Base::calculate_block_index_by_block_coord_s(uint16_t block_x, uint16_t block_y, uint16_t &block_index) {
	block_index = block_y * block_x_number_ + block_x;
	return 0;
}

inline int Move_Scene_Base::calculate_grid_coord_by_grid_index(int grid_idx, int &grid_x, int &grid_y) {
	if (! validate_grid_index(grid_idx)) {
		MSG_USER("invalid cindex");
		return -1;
	}
	grid_x = grid_idx % (this->mpt_data_->head.x_len);
	grid_y = (grid_idx / mpt_data_->head.x_len) + (grid_x > 0 ? 1 : 0);
	return 0;
}

inline int Move_Scene_Base::calculate_grid_coord_by_grid_index_s(int grid_idx, int &grid_x, int &grid_y) {
	grid_x = grid_idx % (this->mpt_data_->head.x_len);
	grid_y = (grid_idx / mpt_data_->head.x_len) + (grid_x > 0 ? 1 : 0);
	return 0;
}

inline bool Move_Scene_Base::validate_grid_coord(uint16_t x, uint16_t y) {
	if (x < 0 || x >= mpt_data_->head.x_len || y < 0 || y >= mpt_data_->head.y_len) {
		MSG_USER("invalid grid_x=%d, grid_y=%d\n", x, y);
		return false;
	} else {
		return true;
	}
}

inline bool Move_Scene_Base::validate_grid_index(int grid_index) {
	if (grid_index < 0 || grid_index >= (mpt_data_->head.x_len * mpt_data_->head.y_len)) {
		MSG_USER("invalid grid_index=%d\n", grid_index);
		return false;
	} else {
		return true;
	}
}

inline bool Move_Scene_Base::validate_block_coord(int block_x, int block_y) {
	return validate_block_coord_s(block_x, block_y);
}

inline bool Move_Scene_Base::validate_block_coord_s(int block_x, int block_y) {
	if (block_x < 0 || block_x >= this->block_x_number_ || block_y < 0 || block_y >= this->block_y_number_) {
		return false;
	} else {
		return true;
	}
}

inline bool Move_Scene_Base::validate_layer_id(size_t layer_id) {
	if (layer_id >= 0 && layer_id < scene_layer_vec_.size() && scene_layer_vec_[layer_id].layer_flag_ == Scene_Layer::USED_LAYER)
		return true;
	else
		return false;
}

inline bool Move_Scene_Base::validate_block_index(int block_idx) {
	if (block_idx >= 0 && block_idx < (block_x_number_ * block_y_number_)) {
		return true;
	} else {
		MSG_USER("invalid block_idx=%d\n", block_idx);
		return false;
	}
}

inline bool Move_Scene_Base::validate_pix_coord(int pix_x, int pix_y) {
	if (pix_x < 0 || pix_x >= mpt_data_->head.map_width || pix_y < 0 || pix_y >= mpt_data_->head.map_height) {
		MSG_USER("invalid pix_x=%d, pix_y=%d\n", pix_x, pix_y);
		return false;
	} else {
		return true;
	}
}

inline int Move_Scene_Base::broadcast_around(size_t layer_id, int block_index, Block_Buffer &buf) {
	if (! validate_layer_id(layer_id) || ! validate_block_index(block_index)) {
		MSG_USER("invalid args, layer_id = %d, block_index = %d", layer_id, block_index);
		return -1;
	}

	return broadcast_around_s(layer_id, block_index, buf);
}

inline int Move_Scene_Base::broadcast_around_s(size_t layer_id, int block_index, Block_Buffer &buf) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	std::vector<int> around;
	calculate_around_block_by_block_index_s(block_index, around);
	for (std::vector<int>::iterator it = around.begin(); it != around.end(); ++it) {
		scene_layer.scene_block_vec_[*it].push_data(buf);
	}
	return 0;
}

inline int Move_Scene_Base::broadcast_all(size_t layer_id, Block_Buffer &buf) {
	if (! validate_layer_id(layer_id)) {
		MSG_USER("invalid args, layer_id = %d", layer_id);
		return -1;
	}
	return broadcast_all_s(layer_id, buf);
}

inline int Move_Scene_Base::broadcast_all_s(size_t layer_id, Block_Buffer &buf) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (Scene_Block_Vec::iterator it = scene_layer.scene_block_vec_.begin(); it != scene_layer.scene_block_vec_.end(); ++it) {
		it->push_data(buf);
	}
	return 0;
}
inline int Move_Scene_Base::broadcast_all_within_battle(size_t layer_id, Block_Buffer &buf) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (Move_Scene_Base::Mover_Map::iterator it = scene_layer.mover_map_.begin(); it != scene_layer.mover_map_.end(); ++it) {
		if (it->second->player_self()) {
			it->second->send_to_client(buf);
		}
	}
	return 0;
}

inline int Move_Scene_Base::broadcast_chat_all(size_t layer_id, Block_Buffer &buf) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (Move_Scene_Base::Mover_Map::iterator it = scene_layer.mover_map_.begin(); it != scene_layer.mover_map_.end(); ++it) {
		it->second->send_to_client(buf);
	}
	return 0;
}

inline int Move_Scene_Base::mpt_value_by_coord(uint16_t x, uint16_t y) {
	if (x < 0 || x >= mpt_data_->head.x_len || y < 0 || y >= mpt_data_->head.y_len) {
		return -1;
	}

	uint32_t grid_index = 0;
	calculate_grid_index_by_grid_coord_s(x, y, grid_index);
	return this->mpt_data_->content[grid_index];
}

inline Mover *Move_Scene_Base::find_mover_with_layer(size_t layer_id, role_id_t role_id) {
	Mover *mover = 0;
	Scene_Layer *layer = scene_layer(layer_id);
	if (layer) {
		Mover_Map::iterator it = layer->mover_map_.find(role_id);
		if (it != layer->mover_map_.end())
			mover = it->second;
	}
	return mover;
}

inline const Map_Head &Move_Scene_Base::head(void) {
	return mpt_data_->head;
}

inline const Map_Content &Move_Scene_Base::content(void) {
	return mpt_data_->content;
}

inline int Move_Scene_Base::grid_mover_count(Coord &coord, size_t layer_id, int mover_type) {
	uint32_t grid_index = 0;
	if (calculate_grid_index_by_grid_coord(coord.x, coord.y, grid_index) != 0) {
		return -1;
	} else {
		if (mover_type == 0)
			return scene_layer_vec_[layer_id].grid_npc_count_[grid_index];
		else
			return scene_layer_vec_[layer_id].grid_player_count_[grid_index];
	}
	return 0;
}

const static int rand_area[2] = {1,-1};
inline int Move_Scene_Base::collect_available_coord(size_t layer_id, Coord &coord, Coord &available_coord, int distance_from, int distance_to) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	int grid_index, x, y;
	for (int loop = distance_from; loop <= distance_to; ++loop) {
		if (rand()%2 == 0) {
			x = coord.x + loop - rand()%(2*loop + 1);
			y = coord.y  + loop * rand_area[rand()%2];
		} else {
			x = coord.x + loop * rand_area[rand()%2];
			y = coord.y + loop - rand()%(2*loop + 1);
		}

		for (int i = 0; i < 8 * loop; ++i) {
			if (x > 0 && x < mpt_data_->head.x_len && y < mpt_data_->head.y_len && y > 0) {
				grid_index = y * mpt_data_->head.x_len + x;
				if (mpt_data_->content[grid_index] >= MOVE_ABLE_FROM && scene_layer.grid_mover_count_[grid_index] == 0) {
					available_coord.x = x;
					available_coord.y = y;
					return 0;
				}
			}

			if (x < coord.x + loop && y == (coord.y - loop)) {
				x++;
			} else if (x == (coord.y + loop) && y < coord.y + loop) {
				y++;
			} else if (x > coord.x - loop && y == (coord.y + loop)) {
				x--;
			} else if (x == (coord.x - loop) && y > coord.y - loop) {
				y--;
			}
		}
	}
	return -1;
}

inline int Move_Scene_Base::collect_mover_count(size_t layer_id, Coord &coord, int distance_x = 2, int distance_y = 5) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	int grid_index = 0, x = 0, y = 0, count = 0;

	for (x = coord.x - distance_x; x <= coord.x + distance_x; ++x) {
		for (y = coord.y - distance_y; y <= coord.y + distance_y; ++y) {
			if (x > 0 && x < mpt_data_->head.x_len && y < mpt_data_->head.y_len && y > 0) {
				grid_index = y * mpt_data_->head.x_len + x;
				if (scene_layer.grid_mover_count_[grid_index] > 0 || mpt_data_->content[grid_index] < MOVE_ABLE_FROM) {
					count++;
				}
			}
		}
	}

	return count;
}

inline bool Move_Scene_Base::calculate_coord_has_mover(size_t layer_id, Coord &coord, int distance_from, int distance_to) {
	int grid_index;
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (int loop = distance_from; loop <= distance_to; ++loop) {
		int loop_ignore = loop - 1;
		for (int x = loop; x >= 0 - loop; --x) {
			for (int y = loop; y >= 0 - loop; --y) {
				if ((x > loop_ignore || x < (0 - loop_ignore)) || (y > loop_ignore || y < (0 - loop_ignore))) {
					if (coord.x + x < 0 || coord.x + x >= mpt_data_->head.x_len ||
							coord.y + y < 0 || coord.y + y >= mpt_data_->head.y_len) {
						continue;
					}
					grid_index = (coord.y + y) * mpt_data_->head.x_len + (coord.x + x);
					if (mpt_data_->content[grid_index] >= MOVE_ABLE_FROM && scene_layer.grid_npc_count_[grid_index] > 0) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

inline int Move_Scene_Base::calculate_diff_block_broadcast_by_block_coord(
		uint16_t start_block_x,
		uint16_t start_block_y,
		uint16_t end_block_x,
		uint16_t end_block_y,
		Coord_Set &disappear_set,
		Coord_Set &move_set,
		Coord_Set &appear_set) {
	int diff_x = std::abs(end_block_x - start_block_x);
	int diff_y = std::abs(end_block_y - start_block_y);
	if (diff_x <= 1 && diff_y <= 1) { /// 相邻九宫格块(可直接使用加减算出交/差集, 避免使用低效STL集合运算)
		int toward = this->calculate_toward_by_coord_s(start_block_x, start_block_y, end_block_x, end_block_y);
		Toward_Offset &toward_offset = toward_offset_vec_[toward];
		calculate_block_coord_set_by_offset(start_block_x, start_block_y, toward_offset.disappear_coord, disappear_set);
		calculate_block_coord_set_by_offset(start_block_x, start_block_y, toward_offset.move_coord, move_set);
		calculate_block_coord_set_by_offset(end_block_x, end_block_y, toward_offset.appear_coord, appear_set);
	} else { /// 不相邻九宫格块, 可能在[跳跃]处出现, 需使用STL集合运算算出交/差集
		std::vector<int> start_around_block_v, end_around_block_v; /// 新/旧9宫格
		calculate_around_block_by_block_coord(start_block_x, start_block_y, start_around_block_v);
		calculate_around_block_by_block_coord(end_block_x, end_block_y, end_around_block_v);

		std::set<int> old_around_block(start_around_block_v.begin(), start_around_block_v.end());
		std::set<int> new_around_block(end_around_block_v.begin(), end_around_block_v.end()); /// 新/旧9宫格

		std::set<int> intersection;	/// 新/旧9宫格交集
		std::set<int> old_diff, new_diff; /// 新/旧9宫格和其交集的差集, old_diff即为应消失(disappear)的区块, new_diff即为应出现(appear)的区块

		std::set_intersection(old_around_block.begin(), old_around_block.end(), new_around_block.begin(), new_around_block.end(),
				std::inserter(intersection, intersection.begin()));
		std::set_difference(old_around_block.begin(), old_around_block.end(), intersection.begin(), intersection.end(),
				std::inserter(old_diff, old_diff.begin()));
		std::set_difference(new_around_block.begin(), new_around_block.end(), intersection.begin(), intersection.end(),
				std::inserter(new_diff, new_diff.begin()));

		for (std::set<int>::iterator it = old_diff.begin(); it != old_diff.end(); ++it) {
			disappear_set.insert(*it);
		}
		for (std::set<int>::iterator it = intersection.begin(); it != intersection.end(); ++it) {
			move_set.insert(*it);
		}
		for (std::set<int>::iterator it = new_diff.begin(); it != new_diff.end(); ++it) {
			appear_set.insert(*it);
		}
	}
	return 0;
}

inline void Move_Scene_Base::calculate_block_coord_set_by_offset(uint16_t block_x,
		uint16_t block_y,
		Offset_Coord_Vec &offset_vec,
		Coord_Set &coord_set) {
	uint16_t dst_block_x, dst_block_y;
	uint16_t dst_block_index;
	for (Offset_Coord_Vec::iterator it = offset_vec.begin(); it != offset_vec.end(); ++it) {
		dst_block_x = block_x + it->first;
		dst_block_y = block_y + it->second;
		if (! validate_block_coord_s(dst_block_x, dst_block_y))
			continue;
		calculate_block_index_by_block_coord_s(dst_block_x, dst_block_y, dst_block_index);
		coord_set.insert(dst_block_index);
	}
}

inline bool Move_Scene_Base::Scene_Block::no_player(void) {
	return block_player_map_.empty();
}

inline bool Move_Scene_Base::no_player_around(size_t layer_id, std::vector<int> &around) {
	if (! validate_layer_id(layer_id)) {
		MSG_USER("invalid layer_id = %d", layer_id);
		return true;
	} else {
		return no_player_around_s(layer_id, around);
	}
}

inline bool Move_Scene_Base::no_player_around_s(size_t layer_id, std::vector<int> &around) {
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	bool no_player = true;
	for (std::vector<int>::iterator it = around.begin(); it != around.end(); ++it) {
		if (! scene_layer.scene_block_vec_[*it].no_player()) {
			no_player = false;
			break;
		}
	}
	return no_player;
}

inline bool Move_Scene_Base::is_in_scene(size_t layer_id, role_id_t role_id) {
	bool res = false;
	Scene_Layer *layer = scene_layer(layer_id);
	if (! layer) {
		MSG_USER("layer == 0");
	} else {
		res = layer->mover_map_.count(role_id);
	}
	return res;
}

inline Move_Scene_Base::Scene_Layer &Move_Scene_Base::first_scene_layer(void) {
	return scene_layer_vec_[0];
}

inline Move_Scene_Base::Scene_Layer *Move_Scene_Base::scene_layer(size_t layer_id) {
	Scene_Layer *scene_layer = 0;
	if (validate_layer_id(layer_id)) {
		scene_layer = &(scene_layer_vec_[layer_id]);
	}
	return scene_layer;
}

#endif /* MOVE_SCENE_BASE_H_ */
