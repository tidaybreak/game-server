/*
 * Scene_Monitor.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef SCENE_MONITOR_H_
#define SCENE_MONITOR_H_

#include "Server_Monitor.h"
#include "Scene_Message_Unit.h"

class Scene_Player;
struct Hero;

class Scene_Monitor: public Server_Monitor {
public:
	typedef Hash_Map<role_id_t, Scene_Player *, Null_Mutex> Scene_Player_Role_Id_Map;
	typedef Hash_Map<role_id_t, Hero *, Null_Mutex> Master_ID_Hero_Map;

	Scene_Monitor();
	virtual ~Scene_Monitor();

	int init(int monitor_index);

	int start(void);

	virtual Scene_Monitor *scene_monitor(void);

	virtual int start_monitor_timer(void);

	virtual int server_monitor_status(void);

	virtual int process_monitor_close(void);

	virtual int monitor_link_drop(const int cid);

	virtual int process_muid_drop(Monitor_Unique_ID &muid);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int send_to_gate(Monitor_Unique_ID &idx, Block_Buffer &buf);

	int send_to_logic(long_id_t uuid, Block_Buffer &buf);

	int send_to_logic(const Monitor_Unique_ID &idx, Block_Buffer &buf);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	Scene_Player *find_scene_player_by_role_id(role_id_t role_id);

	int bind_scene_player(Scene_Player &player);
	int unbind_scene_player(role_id_t role_id);
	int player_total(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int abnormal_logout(Scene_Player *player, int close_reason);

	int process_time_up(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	Scene_Message_Unit message_unit_;

	Scene_Player_Role_Id_Map player_role_id_map_; 	/// role_id - Logic_Player map
};

typedef Singleton<Scene_Monitor> Scene_Monitor_Single;
#define SCENE_MONITOR    (Scene_Monitor_Single::instance())

inline Scene_Monitor *Scene_Monitor::scene_monitor(void) {
	return this;
}

inline Scene_Player *Scene_Monitor::find_scene_player_by_role_id(role_id_t role_id) {
	Scene_Player_Role_Id_Map::iterator it = player_role_id_map_.find(role_id);
	if (it != player_role_id_map_.end())
		return it->second;
	else
		return 0;
}

//inline Hero *Scene_Monitor::find_master_id_hero(role_id_t role_id) {
//	Master_ID_Hero_Map::iterator it = master_id_hero_map_.find(role_id);
//	if (it != master_id_hero_map_.end())
//		return it->second;
//	else
//		return 0;
//}

#endif /* SCENE_MONITOR_H_ */
