/*
 * Single_Dungeon_Struct.h
 *
 *  Created on: Mar 14, 2014
 *      Author: DonaldShaw
 */

#ifndef SINGLE_DUNGEON_STRUCT_H_
#define SINGLE_DUNGEON_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"


struct Wipe_Info;

typedef boost::unordered_map<int, Item_Info> Item_Info_Map;

// 副本信息追踪
struct Dungeon_Trail_Info{
	Dungeon_Trail_Info(void);
	void reset(void);
	Item_Info_Map elite_items;		// 物品信息（不包括资源）
};

struct Sgl_Dungeon_Data{

	enum Dungeon_Progress {
		COMPLETED = 0,
		NOT_COMPLETED = 1,
	};

	enum Elite_Dungeon_Progress{
		ELITE_DUNGEON_LOCK = 0,
		ELITE_DUNGEON_ACTIVE = 1,
		ELITE_DUNGEON_INACTIVE = 2,
		ELITE_DUNGEON_REAP = 3,
		ELITE_DUNGEON_UNREAP = 4,
	};

	//constructor
	Sgl_Dungeon_Data(void);

	int serialize(Block_Buffer &buffer)const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);

	inline bool is_completed(void)const;
	void daily_reset(void);

	int dungeon_id_;
	int ttl_counter_;
	int daily_counter_;
	int8_t type_;
	int8_t progress_;				//精英副本状态（0：未解锁 1：激活 2：未激活 3：未打（通关的） 4：已打（通关的））
	int clear_monster_count_;
	int8_t add_gold_num;				//付费次数
	int8_t add_prop_num;				//付费次数
};

typedef boost::unordered_map<int, Sgl_Dungeon_Data> Sgl_Dungeon_Map;
struct Sgl_Dungeon_Detail : public Detail {
	Sgl_Dungeon_Detail(void);
	virtual ~Sgl_Dungeon_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer)const;
	int deserialize(Block_Buffer &buffer);

	virtual int load(void);
	virtual int save(void);

	inline void save_tick(void) { is_change = true; };
	void reset_clear_monster_counter(int scene_id);

	void set_chapter_status(int chapter, int status);
	int get_chapter_status(int chapter) const ;
	const Sgl_Dungeon_Data* get_dungeon_data(int scene);

	void reset_wipe_info(void);

	int8_t get_act_elite_wipe(const int diff){
		if(diff == 1){
			return act_nor_elite_wipe;
		}else{
			return act_hor_elite_wipe;
		}
	}

	Sgl_Dungeon_Map sgl_dungeon_map_;
	Sgl_Dungeon_Map nor_elite_map_;
	Sgl_Dungeon_Map hor_elite_map_;
	std::vector<Int_Int> chapter_award_vec;
	Dungeon_Trail_Info elite_trail_info;		// 上古副本追踪信息
	Dungeon_Trail_Info lord_trail_info;			// 神魔殿追踪信息
	int single_ver;							// 单人副本版本号
	int elite_ver;								// 上古副本版本号
	int lord_ver;								// 魔神殿副本版本号
	int single_complete_num;				// 单人副本通关数
	int nor_elite_complete_num;			// 上古副本通关数
	int hor_elite_complete_num;			// 魔神殿副本通关数
	int8_t elite_reset_times;
	int8_t elite_ch_num;					// 上古免费挑战次数
	int8_t elite_add_num;				// 上古付费挑战次数
	int8_t elite_add_buy_num;			// 上古购买付费挑战次数
	int8_t elite_task_add_num;			// 任务增加上古挑战次数（属于免费次数）

	int wipe_type;							   // 0：没有扫荡 1：单人副本扫荡 2：精英副本扫荡
	int wipe_num;								// 扫荡次数
	int wipe_status;							// 0：扫荡中 1：扫荡结束 2：扫荡中断
	int wipe_spend_time;						//
	bool wipe_elite_double;						// 精英扫荡是否双倍(server state)
	bool wipe_elite_double_not_selected;		// UI界面上不勾选精英扫荡(client state)
	Time_Value wipe_begin_time;			//
	Time_Value next_wipe_time;				//
	Time_Value wipe_finish_time;			//	扫荡结束时间
	Int_Vec wipe_scenes;			// 扫荡副本id
	std::vector<Wipe_Info> wipe_infos;  //	扫荡信息

	// 下面的字段不用写到数据库
	int8_t act_nor_elite_wipe;			// 是否开启上古副本扫荡
	int8_t act_hor_elite_wipe;			// 是否开启魔神殿副本扫荡
};

#endif /* SINGLE_DUNGEON_STRUCT_H_ */
