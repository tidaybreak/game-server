/*
 * Hero.h
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#ifndef HERO_H_
#define HERO_H_

#include "NPC/NPC.h"
#include "Hero_Struct.h"

struct Hero_Base_Detail;
class Scene_Player;

class Hero: public NPC {
public:
	enum {
		CALL_BACK = 1,			//召回
		DEAD = 2,				//死亡
	};

	Hero(void);
	virtual ~Hero(void);

	inline const Hero_Base_Detail &hero_const_base_detail(void);
	inline Hero_Base_Detail &hero_base_detail(void);
	Scene_Player *master(void);

	int init(role_id_t role_id, Move_Scene* scene, const Coord& birth_coord);
	int birth(role_id_t role_id, Move_Scene* scene, const Coord& birth_coord);

	int battle_init(role_id_t role_id, Battle_Scene* scene, const Coord& birth_coord);
	int battle_birth(role_id_t role_id, Battle_Scene* scene, const Coord& birth_coord);

	inline bool has_init(void);

	int init(Move_Scene *scene);
	int fini(void);
	int reset(void);

	virtual int tick(Time_Value& now);
	virtual void fight_tick(const Time_Value &now);
	virtual bool is_hero(void);

	///////////////////////////////////////////////////////////////

	virtual int move_appear_info(Block_Buffer &buf);
	virtual int make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y);

	////////////////////////////////////////////////////////////////

	virtual int battle_appear_info(Block_Buffer &buf);

	virtual int battle_appear(void);

	virtual int battle_disappear(void);

	virtual int enter_battle_scene(Battle_Scene *scene);

	virtual int exit_battle_scene(int exit_from = 0);

	/////////////////////////////////////////////////////////////////

//	void make_pet_appear_info(MSG_800006 &msg);

	//宠物回收, 默认参数0表示玩家重登陆，或者地图服切换时候，临时从该地图服移除宠物，宠物实际还是出战状态 或者玩家手动移除宠物
	//1表示收回宠物，宠物被收回，地图服移除宠物，2表示死亡
	int outer_hero_recycle(int type = 0);
	int fight_hero_recycle(int type = 0);

	int register_timer(void);
	int unregister_timer(void);

	int hero_pull_back(void);

	int refresh_hero_base_detail(Hero_Base_Detail &detail);			//刷新英雄基本信息
	//int refresh_fight_hero_base(Hero_Base_Detail &detail);		//刷新战斗英雄基本信息
	int refresh_hero_fighter_detail(Fighter_Detail &detail);		//刷新英雄战斗信息

	int sync_hero_fighter_detail_to_logic(void);
	int sync_hero_die_to_logic(void);
	int sync_hero_call_back_to_logic();
	int sync_hero_info_to_logic();
	int send_to_logic(Block_Buffer &buf);

	int skill_insert_status(void);
	int skill_erase_status(void);
	//int add_hero_skill(void);

	int modify_pet_experience(double value, int master_level);
	int pet_notify_fight_info(void);

//	virtual role_id_t get_role_id(void) { return pet_base_detail_.pet_role_id; }

	virtual int get_npc_type_id(void) { return 0; }


	inline role_id_t master_role_id(void);
	inline role_id_t role_id(void);

	void set_hero_speed(void);

//	void set_hero_camps(int camps);
//	void set_hero_country(int country);
//	void set_hero_team_id(int team_id);
//	void set_hero_faction_id(faction_id_t faction_id);
////	void set_hero_fight_mode(Fighter_Detail::Fight_Mode fight_mode);
//	void set_hero_name(std::string name);
//	void set_hero_cid(int hero_cid);

	virtual Hero *hero_self(void);
//	virtual int die_process(const Attack_Info &info, Fighter &killer);
	virtual int die_process(const Attack_Param &info);

	virtual int level(void);

	// 英雄是否有战斗ai(竞技场类型的英雄会有ai)
	virtual int ai_select_skill(void);
	void set_battle_ai(void);
	bool has_battle_ai(void);

	int set_pos_key(const int pos, const int idx);

private:
	Hero_Base_Detail hero_base_detail_;
	bool hero_die_;				//是否死亡
	Time_Value tick_time_;
	bool is_register_timer_;	//是否注册定时器
	bool is_sync_;
	bool has_init_;				// 是否init
	bool has_battle_ia_;			// 战斗ai
};

////////////////////////////////////////////////////////////////////////////////

inline const Hero_Base_Detail &Hero::hero_const_base_detail(void){
	return hero_base_detail_;
}

inline Hero_Base_Detail &Hero::hero_base_detail(void){
	return hero_base_detail_;
}

inline bool Hero::has_init(void) {
	return has_init_;
}

inline role_id_t Hero::master_role_id(void) {
	return hero_base_detail_.master_role_id;
}

inline role_id_t Hero::role_id(void) {
	return hero_base_detail_.hero_role_id;
}

//inline Scene_Player *Hero::master(void) {
//	if (hero_base_detail_.master_role_id) {
//		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(hero_base_detail_.master_role_id);
//		if (player) {
//			return player;
//		}
//	}
//	return NULL;
//}

#endif /* HERO_H_ */
