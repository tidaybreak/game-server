/*
 * Gang_Boss_Manager.h
 *
 *  Created on: 2015年2月7日
 *      Author: root
 */

#ifndef GANG_BOSS_MANAGER_H_
#define GANG_BOSS_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"

class World_Boss_Manager;

class Gang_Boss_Manager{
	typedef boost::unordered_map<role_id_t, World_Boss_Manager*> World_Boss_Manager_Map;
public:
	Gang_Boss_Manager(void);
	~Gang_Boss_Manager(void);
public:
	void reset(void);
	World_Boss_Manager* world_boss_manager(role_id_t gang_id);
	World_Boss_Manager* get_world_boss_manager(role_id_t gang_id);
	role_id_t get_gang_id(const role_id_t role_id);
public:
	int req_world_boss_act_guild_bless(Scene_Player* player);
	int req_world_boss_use_ability(Scene_Player* player, int type);
	int req_fight_world_boss(Scene_Player* player);
	int inner_player_enter(const role_id_t gang_id, Block_Buffer& buf);
	int inner_sync_info(Scene_Player* player, Block_Buffer& buf);
	int inner_act_start(const role_id_t gang_id, Block_Buffer& buf);
	int inner_act_end(const role_id_t gang_id, Block_Buffer& buf);
	void listen_gang_info_sync(const gang_id_t src_gang_id, Scene_Player* player);
	void listen_vip_info_change(Scene_Player* player);
private:
	World_Boss_Manager_Map world_boss_manager_map_;
};

typedef Singleton<Gang_Boss_Manager> Gang_Boss_Manager_Single;
#define GANG_BOSS_MANAGER (Gang_Boss_Manager_Single::instance())

#endif /* GANG_BOSS_MANAGER_H_ */
