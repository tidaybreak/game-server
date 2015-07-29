/*
 * Pool_Manager.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *      对象池都放这里
 */

#ifndef POOL_MANAGER_H_
#define POOL_MANAGER_H_

#include "Server_Struct.h"

class Logic_Player;
class Gate_Player;
class Robot_Player;
class Scene_Player;
class Chat_Player;
class DB_Worker;
class Global_Timer_Handler;
class Battle_Scene;
struct Player_DB_View;
struct Logic_Player_Detail;
struct Scene_Player_Detail;
struct Item_Detail;
class Team;
struct Hero;
struct Status_Detail;
struct Gang_DB_View;
struct Ganger_Detail;
struct Status_Info;
struct Arena_Rank_Player;
struct Inner_Arena_Rank_Player;
class Map_Team;

class Curl_Worker;
struct Http_Info;

struct Ranking;
struct Knight_Trials_Player;

class World_Boss_Manager;
class Inner_Arena_Manager;
struct Soldier;
struct Expedition_Manager_Data;

namespace mongo {
	class DBClientConnection;
}

class Pool_Manager {
	typedef Object_Pool_Ex<Logic_Player_Detail, Thread_Mutex> Logic_Player_Data_Pool;
	typedef Object_Pool_Ex<Scene_Player_Detail, Thread_Mutex> Scene_Player_Data_Pool;
	typedef Object_Pool_Ex<Logic_Player, Thread_Mutex> Logic_Player_Pool;
	typedef Object_Pool_Ex<Gate_Player, Thread_Mutex> Gate_Player_Pool;
	typedef Object_Pool_Ex<Robot_Player, Thread_Mutex> Robot_Player_Pool;
	typedef Object_Pool_Ex<Scene_Player, Thread_Mutex> Scene_Player_Pool;
	typedef Object_Pool_Ex<Chat_Player, Thread_Mutex> Chat_Player_Pool;

	typedef Object_Pool_Ex<Player_DB_View, Thread_Mutex> Player_DB_View_Pool;
	typedef Object_Pool_Ex<Block_Buffer, Thread_Mutex> DB_Oper_Block_Pool;
	typedef Object_Pool<DB_Worker, Thread_Mutex> DB_Worker_Pool;
	typedef Object_Pool<mongo::DBClientConnection, Thread_Mutex> Connection_Pool;

	typedef Object_Pool_Ex<Global_Timer_Handler, Thread_Mutex> Global_Timer_Pool;

	typedef Object_Pool_Ex<Battle_Scene, NULL_MUTEX> Battle_Scene_Pool;

	typedef Object_Pool_Ex<Item_Detail, Thread_Mutex> Item_Pool;

	typedef Object_Pool_Ex<Team, Thread_Mutex> Team_Pool;

	typedef Object_Pool_Ex<Hero, Thread_Mutex> Hero_Pool;

	typedef Object_Pool_Ex<World_Boss_Manager, Thread_Mutex> World_Boss_Manager_Pool;

	typedef Object_Pool_Ex<Inner_Arena_Manager, Thread_Mutex> Inner_Arena_Manager_Pool;

	typedef Object_Pool_Ex<Status_Detail, Thread_Mutex> Status_Pool;
	typedef Object_Pool_Ex<Gang_DB_View, Thread_Mutex> Gang_DB_View_Pool;
	typedef Object_Pool_Ex<Ganger_Detail, Thread_Mutex> Ganger_Detail_Pool;

	typedef Object_Pool_Ex<Status_Info, Thread_Mutex> Status_Info_Pool;

	typedef Object_Pool_Ex<Arena_Rank_Player, Thread_Mutex> Arena_Rank_Player_Pool;
	typedef Object_Pool_Ex<Inner_Arena_Rank_Player, Thread_Mutex> Inner_Arena_Rank_Player_Pool;
	typedef Object_Pool_Ex<Map_Team, Thread_Mutex> Map_Team_Pool;

	typedef Object_Pool_Ex<Logic_Player, Thread_Mutex> Dragon_Vale_Pool;

	typedef Object_Pool<Curl_Worker, Thread_Mutex> Curl_Worker_Pool;
	typedef Object_Pool_Ex<Http_Info, Thread_Mutex> Http_Info_Pool;

	typedef Object_Pool_Ex<Ranking, Thread_Mutex> Ranking_Pool;
	typedef Object_Pool_Ex<Knight_Trials_Player, Thread_Mutex> Knight_Trials_Pool;

	typedef Object_Pool_Ex<Soldier, Thread_Mutex> Soldier_Pool;

	typedef Object_Pool_Ex<Expedition_Manager_Data, Thread_Mutex> Expedition_Manager_Data_Pool;
	///////////////////////////////////////////////////////////////////////////////////////////

public:

	Pool_Manager();
	virtual ~Pool_Manager();

	int init(void);

	void show_pool_status(void);

	Logic_Player_Detail *pop_player_data_pool(int type = 0);
	int push_player_data_pool(Logic_Player_Detail *data, int type = 0);

	Scene_Player_Detail *pop_scene_player_data_pool(void);
	int push_scene_player_data_pool(Scene_Player_Detail *data);

	Logic_Player *pop_logic_player(void);
	int push_logic_player(Logic_Player *player);

	Gate_Player *pop_gate_player(void);
	int push_gate_player(Gate_Player *player);

	Robot_Player *pop_robot_player(void);
	int push_robot_player(Robot_Player *player);

	Scene_Player *pop_scene_player(void);
	int push_scene_player(Scene_Player *player);

	Chat_Player *pop_chat_player(void);
	int push_chat_player(Chat_Player *player);

	Player_DB_View *pop_player_db_view(void);
	int push_player_db_view(Player_DB_View *db_view);

	Block_Buffer *pop_db_oper_block_pool(void);
	int push_db_oper_block_pool(Block_Buffer *block);

	DB_Worker *pop_db_worker_pool(void);
	int push_db_worker_pool(DB_Worker *worker);

	Global_Timer_Handler *pop_global_timer_handler_pool(void);
	int push_global_timer_handler_pool(Global_Timer_Handler *block);

	mongo::DBClientConnection *pop_dbclientConnection_pool(void);
	int push_dbclientConnection_pool(mongo::DBClientConnection *block);

	Item_Detail *pop_item_pool(int type = 0);
	int push_item_pool(Item_Detail *item_detail, int type = 0);

	Team *pop_team_pool(void);
	int push_team_pool(Team *team);

	Battle_Scene *pop_battle_scene_pool(void);
	int push_battle_scene_pool(Battle_Scene *scene);

	Hero *pop_hero(void);
	int push_hero(Hero *hero);

	World_Boss_Manager *pop_world_boss_manager(void);
	int push_world_boss_manager(World_Boss_Manager* world_boss);

	Inner_Arena_Manager *pop_inner_arena_manager(void);
	int push_inner_arena_manager(Inner_Arena_Manager* inner_arena);

	Status_Detail *pop_status_detail_pool(void);
	int push_status_detail_pool(Status_Detail* status);

	Gang_DB_View *pop_gang_db_view(void);
	int push_gang_db_view(Gang_DB_View* gang_db_view);

	Status_Info *pop_status_info_pool(void);
	int push_status_info_pool(Status_Info *status_info);

	Ganger_Detail* pop_ganger_detail();
	int push_ganger_detail(Ganger_Detail* detail);

	Arena_Rank_Player *pop_arena_player_pool(void);
	int push_arena_player_pool(Arena_Rank_Player *obj);

	Inner_Arena_Rank_Player *pop_inner_arena_player_pool(void);
	int push_inner_arena_player_pool(Inner_Arena_Rank_Player *obj);

	Map_Team *pop_map_team_pool(void);
	int push_map_team_pool(Map_Team *team);

	Curl_Worker *pop_curl_worker_pool(void);
	int push_curl_worker_pool(Curl_Worker *curl);

	Http_Info *pop_http_info_pool(void);
	int push_http_info_pool(Http_Info *info);

	Ranking *rank_data_pool(void);
	int push_rank_data_pool(Ranking *rank);

	Knight_Trials_Player *pop_knight_trials_player_data_pool(void);
	int push_knight_trials_player_data_pool(Knight_Trials_Player *player);

	Soldier *pop_soldier_pool(void);
	int push_soldier_pool(Soldier *soldier);

	Expedition_Manager_Data *pop_expedition_manager_data(void);
	int push_expedition_manager_data(Expedition_Manager_Data *data);
private:
	static Pool_Manager *instance_;

	Logic_Player_Data_Pool player_data_pool_;
	Scene_Player_Data_Pool scene_player_data_pool_;
	Logic_Player_Pool logic_player_pool_;
	Gate_Player_Pool gate_player_pool_;
	Robot_Player_Pool robot_player_pool_;
	Scene_Player_Pool scene_player_pool_;
	Chat_Player_Pool chat_player_pool_;

	Player_DB_View_Pool player_db_view_pool_;				// 所有玩家基本信息池
	DB_Oper_Block_Pool db_oper_block_pool_;					// 数据库操作Buf池
	DB_Worker_Pool db_worker_pool_;							// 数据库Worker池
	Global_Timer_Pool global_timer_handler_pool_;			// 逻辑服定时器池
	Connection_Pool db_connection_pool_;					// 数据库连接池

	Http_Info_Pool http_info_pool_;							// Http数据信息池
	Curl_Worker_Pool curl_worker_pool_;					// Curl Worker池

	Ranking_Pool ranking_pool_;					// 排行榜信息池

	Item_Pool item_pool_;									// 道具池
	Team_Pool team_pool_;									// 队伍池
	Battle_Scene_Pool battle_scene_pool_;
	Hero_Pool hero_pool_;
	World_Boss_Manager_Pool world_boss_manager_pool_;
	Inner_Arena_Manager_Pool inner_arena_manager_pool_;	//

	Status_Pool status_pool_;
	Gang_DB_View_Pool gang_db_view_pool_;					// 公会池
	Ganger_Detail_Pool ganger_detail_pool_;
	Status_Info_Pool status_info_pool_;
	Arena_Rank_Player_Pool arena_player_pool_;
	Inner_Arena_Rank_Player_Pool inner_arena_player_pool_;
	Map_Team_Pool map_team_pool_;
	Dragon_Vale_Pool dragon_vale_pool_;
	Knight_Trials_Pool knight_trials_pool_;
	Soldier_Pool soldier_pool_;
	Expedition_Manager_Data_Pool expedition_manager_data_pool_;
};
typedef Singleton<Pool_Manager> Pool_Manager_Single;
#define POOL_MANAGER    (Pool_Manager_Single::instance())

#endif /* POOL_MANAGER_H_ */
