/*
 * Pet.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef PET_H_
#define PET_H_

#include "NPC.h"


class Scene_Player;
class Pet: public NPC {
public:
	enum {
		CALL_BACK = 1,			//召回
		DEAD = 2,				//死亡
	};

	Pet(void);
	virtual ~Pet(void);

//	Pet_Base_Detail const &pet_base_detail(void);
	inline Scene_Player *master(void);

	int init(Move_Scene *scene);
	int reset(void);

	virtual int make_appear_info(Block_Buffer &buf);
//	void make_pet_appear_info(MSG_800006 &msg);

	//宠物回收, 默认参数0表示玩家重登陆，或者地图服切换时候，临时从该地图服移除宠物，宠物实际还是出战状态 或者玩家手动移除宠物
	//1表示收回宠物，宠物被收回，地图服移除宠物，2表示死亡
	int pet_recycle(int type = 0);

	int register_timer(void);
	int unregister_timer(void);
	int time_up(Time_Value &now);
	int pet_pull_back(void);

//	int refresh_pet_base_detail(Pet_Base_Detail &detail);		//刷新宠物基本信息
	int refresh_pet_fighter_detail(Fighter_Detail &detail);		//刷新宠物战斗信息

	int sync_pet_fighter_detail_to_logic(void);
	int sync_pet_die_to_logic(void);
	int sync_pet_call_back_to_logic();
	int sync_pet_info_to_logic();

	virtual int send_to_logic(Block_Buffer &buf);

	int skill_insert_status(void);
	int skill_erase_status(void);
	int add_pet_skill(void);

	int modify_pet_experience(double value, int master_level);
	int pet_notify_fight_info(void);

//	virtual role_id_t get_role_id(void) { return pet_base_detail_.pet_role_id; }

	virtual int get_npc_type_id(void) { return 0; }

	virtual bool is_pet(void) { return true; }

	virtual role_id_t master_id(void);

	void set_pet_camps(int camps);
	void set_pet_country(int country);
	void set_pet_team_id(int team_id);
	void set_pet_faction_id(faction_id_t faction_id);
//	void set_pet_fight_mode(Fighter_Detail::Fight_Mode fight_mode);
	void set_pet_name(std::string name);
	void set_pet_cid(int pet_cid);

	virtual Pet *pet_self(void);

	virtual void displacement_only_vision(const Coord &coord_from, Coord &coord_to, int skill_id, int step = 0, int action = 0);

private:
//	Pet_Base_Detail pet_base_detail_;
	bool pet_die_;		//宠物是否死亡
	Time_Value tick_time_;
	bool is_register_timer_;	//是否注册定时器
	bool is_sync_;
};


inline Scene_Player *Pet::master(void) {
//	return MAP_MANAGER_INSTANCE->find_role_id_player(pet_base_detail_.master_role_id);
	return NULL;
}

#endif /* PET_H_ */
