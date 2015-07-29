/*
 * Scene_Dragon_Vale_Manager.h
 *
 *  Created on: 2014年7月31日
 *      Author: gaojianshan
 */

#ifndef DRAGON_VALE_MANAGER_H_
#define DRAGON_VALE_MANAGER_H_

#include <sys/types.h>
#include "Singleton.h"
#include "Scene_Dragon_Vale_Def.h"
#include "Msg_Inner_Struct.h"
#include "Module_Manager.h"

struct MSG_20600002;
class Block_Buffer;
class Scene_Player;
class Dragon_Vale_Scene;
class Move_Scene;

class Scene_Dragon_Vale_Manager : public Module_Manager {
public:
	Scene_Dragon_Vale_Manager() {
		reset();
	}
	void reset();

	virtual void tick(Time_Value &now);
	void listen_auto_enter_battle();

	int process_rob_dragon_vale_match(int64_t uuid, std::string server_name, int64_t role_id, uint32_t coin, uint32_t last_coin);
	int process_rob_match(const MSG_20600002 &msg);
	int process_rob_fight_load_player_data(int64_t to_uuid, int64_t to_role_id,
									      std::string to_server_name, std::string to_role_name,
										  int64_t uuid, int64_t role_id);
	int  process_rob_fight(Block_Buffer &buf);
	void process_win_and_lose(Scene_Player &win_player, Scene_Player &lose_player);
	void process_win(Scene_Player &win_player);
	void process_lose(Scene_Player &lose_player);
	void notify_online_player_battle_result(Scene_Player &player, Move_Scene *scene);
	void notify_total_rob_production(Scene_Player &player, Move_Scene *scene);
	void process_rob_enter_dragon_vale(int64_t to_uuid, int64_t to_role_id, int64_t uuid, int64_t role_id, uint8_t limit_time);
	void process_rob_enter_dragon_vale(const MSG_20600008 &msg);
	void process_gain_rob_production(const MSG_20600009 &msg);
	void process_gain_rob_production_res(const MSG_20600010 &msg);
	void process_broadcast_dragon_vale_scene(int64_t creater, Block_Buffer &buf);
	void process_rob_match_fail(Block_Buffer &buf);
	void process_refresh_dragon_vale_monster(uint8_t type, int64_t role_id, int64_t id, uint8_t quality);
	void process_drive_thief(int64_t player_id, uint8_t type, int64_t role_id, int64_t monster_id, uint8_t quality);
	void process_rob_match_fail_back_coin(int64_t uuid, int64_t role_id, uint32_t coin, uint8_t type);
	void process_dragon_vale_rob(int64_t to_uuid, int64_t to_role_id, std::string to_server_name, std::string to_role_name,
								int64_t uuid, int64_t role_id, std::string server_name, std::string role_name, uint32_t coin, uint8_t flag);
	void porcess_dragon_vale_rob_offline(Scene_Player &player);

	inline Dragon_Vale_Rob_Fight_Map& get_dragon_vale_target_rob_map();
	inline Dragon_Vale_Rob_Fight_Map& get_dragon_vale_rob_target_map();
	inline Scene_Complete_Map& get_scene_complete_rob_map();
	inline Scene_Complete_Map& get_scene_complete_target_map();
	inline Total_Rob_Production_Info_Map& get_total_rob_production_info_map();
private:
	Dragon_Vale_Rob_Fight_Map dragon_vale_rob_target_map;
	Dragon_Vale_Rob_Fight_Map dragon_vale_target_rob_map;
	Scene_Complete_Map scene_complete_rob_map;
	Scene_Complete_Map scene_complete_target_map;
	Total_Rob_Production_Info_Map total_rob_production_info_map;
};

Dragon_Vale_Rob_Fight_Map& Scene_Dragon_Vale_Manager::get_dragon_vale_target_rob_map() {
	return dragon_vale_target_rob_map;
}

Dragon_Vale_Rob_Fight_Map& Scene_Dragon_Vale_Manager::get_dragon_vale_rob_target_map() {
	return dragon_vale_rob_target_map;
}

Scene_Complete_Map& Scene_Dragon_Vale_Manager::get_scene_complete_rob_map() {
	return scene_complete_rob_map;
}

Scene_Complete_Map& Scene_Dragon_Vale_Manager::get_scene_complete_target_map() {
	return scene_complete_target_map;
}

Total_Rob_Production_Info_Map& Scene_Dragon_Vale_Manager::get_total_rob_production_info_map() {
	return total_rob_production_info_map;
}

typedef Singleton<Scene_Dragon_Vale_Manager> Scene_Dragon_Vale_Manager_Single;
#define SCENE_DRAGON_VALE_MANAGER   (Scene_Dragon_Vale_Manager_Single::instance())


#endif /* DRAGON_VALE_MANAGER_H_ */
