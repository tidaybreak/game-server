/*
 * Drops.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef DROPS_H_
#define DROPS_H_

#include "NPC.h"



class Move_Scene;
struct Item_Detail;
struct Drops_Item;

class Drops: public NPC {
public:
	enum Mode {
		MODE_TEAM_ALL = 0,
		MODE_TEAM_ONE,
		MODE_BELONG_ALL,
		MODE_BELONG_TEAM,
	};
	enum Drop_Type{
		DROP_TYPE_BOX = 0,
		DROP_TYPE_SCATTER,
		DROP_TYPE_AUTOPICK,
	};

	enum Treasure_Box_Type{
		TREASURE_BOX_NORMAL 		= 0,
		TREASURE_BOX_SCENE_PASS = 1,		// 需通关才能打开
		TREASURE_BOX_VIP			= 2,	   // 需VIP才能打开
	};

	typedef boost::unordered_map<int, Item_Detail> Drop_Item_Map;

	Drops(void);
	virtual ~Drops(void);

	int birth(role_id_t &npc_id, Move_Scene *scene, NPC_Record *point, Coord &birth_coord, int ai, int ai_config);
	int init(role_id_t &npc_id, Move_Scene *scene, NPC_Record *point, Coord &birth_coord, int ai, int ai_config);
	int fini(void);
	int reset(void);

	virtual int recover_self(void);
	virtual int tick(Time_Value &now);

	virtual int active_npc(void);

	virtual int move_appear_info(Block_Buffer &buf);

//	virtual int die_process(const Attack_Info &info, Fighter &killer);

	virtual bool is_drops(void);

	virtual Drops *drops_self(void);

	inline Drops_Detail &drops_detail(void);

	int fetch_drops_list(Scene_Player* player, Block_Buffer &res_buf);

//	int check_get_goods(MSG_140015 &msg, role_id_t role_id);

	inline Drops_Detail::Drops_Status get_drops_status(void);

	int die_auto_get(void);

	//  通知移除
	int erase_drops_item(int item_index, role_id_t role_id, Fighter* player);

	//	通知飘落
	int notice_get_drops(Fighter* player, int8_t item_index);

	void add_addition(const role_id_t role_id);

	int get_drops_goods_by_index(Scene_Player *sp,const int8_t index, std::vector<Drops_Item>& item_vec);

	// 场景通关侦听
	virtual void hook_scene_pass(int scene_id);
	// 是否通关宝箱
	bool is_scene_pass_box(void);

private:
	void set_box_open(bool value);

private:
	Drops_Detail drops_detail_;

};

////////////////////////////////////////////////////////////////////////////////

Drops_Detail::Drops_Status Drops::get_drops_status(void) {
	return drops_detail_.drops_status;
}

Drops_Detail &Drops::drops_detail(void) {
	return drops_detail_;
}


#endif /* DROPS_H_ */
