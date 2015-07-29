/*
 * Logic_Heroer_Struct.h
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#ifndef LOGIC_HEROER_STRUCT_H_
#define LOGIC_HEROER_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"
#include "NPC/hero/Hero_Struct.h"
#include "Fighter_Detail.h"
#include "status/Logic_Status_Struct.h"

typedef Coord Grid_Coord;

/*
神器信息
*/
struct Artifact_Detail {
	Artifact_Detail(void) { reset(); };
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int32_t artifact_id;/*神器ID*/
	int32_t artifact_lvl;/*神器等级*/
};
typedef std::map<int, Artifact_Detail> Artifact_Map;

/*
英雄信息
*/
struct Hero_Info_Detail {
	Hero_Info_Detail(void);
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int32_t hero_id;/*英雄ID*/
	int32_t fight_date;
	int32_t hero_index;/*英雄index*/
	int32_t hero_facade_id;/*英雄外观ID*/
	std::string hero_name;/*英雄名字*/
	int8_t is_outer;/*是否跟随英雄*/
	int8_t is_fighter;/*是否战斗英雄*/
	int8_t is_assist; /*是否助阵英雄*/
	int32_t level;/*英雄等级*/
	int32_t formation;/*英雄阵型*/
	int32_t awake_lvl;/*觉醒等级*/
	double exp;/*英雄经验*/
	std::vector<Int_Double> prop_value;
	Artifact_Map artifacts;				// 神器信息集
	bool is_task_tmp_hero;			    // 是否是任务临时添加英雄(除任务外，其他系统不能修改此变量)
	int32_t intimacy_lv;	// 亲密等级
	int32_t intimacy_exp;//亲密经验
	int32_t community_times;//每日对话次数
	int32_t gift_times;//每日送礼品次数

	Int_Int assist_skill; // 助阵技能

	Fighter_Detail hero_fighter_detail;	// 英雄战斗信息
	std::map<int, int> suit_cnt_map;	// 套装信息，不存
};

struct Panal_Hero_Tag {
	Panal_Hero_Tag(void);
	void reset(void);
	friend bool operator<(const Panal_Hero_Tag& lhs, const Panal_Hero_Tag& rhs);

	int is_fight;
	int is_last_fight;
	int hero_level;
	int hero_index;
	int hero_id;
};

/// 玩家英雄详情
struct Heroer_Detail: public Detail {
	typedef boost::unordered_map<int64_t, Status_Effect_Argv> Status_Effect_Argv_Map;
	typedef std::list<Status_Effect_Argv *> Status_Effect_Argv_List;
	enum {
		TURN_CARD_AMOUNT = 12,
		GET_CARD_AMOUNT = 6,
		FIGHT_PET_AMOUNT = 9,
	};

	Heroer_Detail(void);
	virtual ~Heroer_Detail();
	virtual void reset(void);

	virtual int serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);

	int serialize_fighter(Block_Buffer &buffer) const;
	int deserialize_fighter(Block_Buffer &buffer);

	virtual int load(void);
	virtual int save(void);
	virtual int load_fight_hero(void);
	void save_tick(void) { is_change_ = true; };
	void get_hero_info(const int hero_index, Hero_Info &hero_info) const;
	void refresh_hero_base_detail(const int hero_index, role_id_t master_role_id, std::string master_role_name);
	void refresh_fight_hero_base(const int hero_index, role_id_t master_role_id, std::string master_role_name);

	typedef std::map<int, Hero_Info_Detail> Hero_Map;		// 英雄信息Map

	role_id_t role_id;										// 角色ID
	int outer_index;											// 是否展示英雄 ( 0:不跟随英雄 1:跟随英雄 )
	int fighter_index;
	int last_fighter_idx;									// 上次战斗英雄index
	Int_Int_Map hero_assist_map;								// 助阵的伙伴索引

	Hero_Map hero_map;										// 英雄信息Map

	Hero_Base_Detail hero_base_detail_;						// 跟随英雄基本信息
	Hero_Base_Detail fight_hero_base_;						// 战斗英雄基本信息

	// status
	Logic_Status_Detail logic_status_detail_;
	std::list<Status_Info*> out_status_timepri_list;		// 删除buff
	std::map<int, double> status_pro_val;					// 属性
	std::map<int, double> status_pro_val_per;				// 百分比属性
	Int_Hash_Set buff_scene_status;							// 受场景影响的状态
	// status

	std::map<int, double> artifacts_pro_val;				// 所有神器属性值总和

	std::vector<Panal_Hero_Tag> panal_hero_tags;			// 角色面板英雄标签

	Int_Set clicked_new_heros;								// 点击过的未招的新英雄

	std::map<int, double> hero_rune_pro_val;				// 模块加成的属性，用于外部接口，不存

	bool is_change_;
};


#endif /* LOGIC_HEROER_STRUCT_H_ */
