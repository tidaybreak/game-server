/*
 * Pool_Manager.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#include "Pool_Manager.h"
#include "DB_Worker.h"
#include "Global_Timer.h"
#include "Logic_Player.h"
#include "Gate_Player.h"
#include "Robot_Player.h"
#include "Scene_Player.h"
#include "Chat_Player.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "Scene_Player_Detail.h"
#include "team/Team.h"
#include "Battle_Scene.h"
#include "NPC/hero/Hero.h"
#include "mongo/client/dbclient.h"
#include "status/Status_Struct.h"
#include "gang/Gang_Struct.h"
#include "arena/Arena_Rank_Player.h"
#include "inner_arena/Inner_Arena_Struct.h"
#include "module/team/Map_Team.h"
#include "http_client/Curl_Worker.h"
#include "http_client/Http_Struct.h"
#include "ranking/Ranking_Struct.h"
#include "world_boss/World_Boss_Manager.h"
#include "inner_arena/Inner_Arena_Manager.h"
#include "expedition_scene/Expedition_Scene_Struct.h"


Pool_Manager::Pool_Manager() {
	// TODO Auto-generated constructor stub
}

Pool_Manager::~Pool_Manager() {
	// TODO Auto-generated destructor stub
}

int Pool_Manager::init(void) {

	return 0;
}

#define SHOW_POOL_STATUS(NAME, POOL) \
	if (POOL.free_obj_list_size() || POOL.used_obj_list_size()) { \
		LOG_TEXT("%-30s free:%-10d, used:%-10d, obj_size:%d", NAME, POOL.free_obj_list_size(), POOL.used_obj_list_size(), POOL.all_obj_sizeof());	\
	}


void Pool_Manager::show_pool_status(void) {
	SHOW_POOL_STATUS("player_data_pool_", player_data_pool_);
	player_data_pool_.show_debug();

	SHOW_POOL_STATUS("scene_player_data_pool_", scene_player_data_pool_);
	SHOW_POOL_STATUS("logic_player_pool_", logic_player_pool_);
	SHOW_POOL_STATUS("gate_player_pool_", gate_player_pool_);
	SHOW_POOL_STATUS("robot_player_pool_", robot_player_pool_);
	SHOW_POOL_STATUS("scene_player_pool_", scene_player_pool_);
	SHOW_POOL_STATUS("chat_player_pool_", chat_player_pool_);
	SHOW_POOL_STATUS("player_db_view_pool_", player_db_view_pool_);
	SHOW_POOL_STATUS("db_oper_block_pool_", db_oper_block_pool_);
	SHOW_POOL_STATUS("db_worker_pool_", db_worker_pool_);
	SHOW_POOL_STATUS("global_timer_handler_pool_", global_timer_handler_pool_);
	SHOW_POOL_STATUS("db_connection_pool_", db_connection_pool_);
	SHOW_POOL_STATUS("item_pool_", item_pool_);
	item_pool_.show_debug();

	SHOW_POOL_STATUS("team_pool_", team_pool_);
	SHOW_POOL_STATUS("battle_scene_pool_", battle_scene_pool_);
	SHOW_POOL_STATUS("hero_pool_", hero_pool_);
	SHOW_POOL_STATUS("status_pool_", status_pool_);
	SHOW_POOL_STATUS("gang_db_view_pool_", gang_db_view_pool_);
	SHOW_POOL_STATUS("ganger_detail_pool_", ganger_detail_pool_);
	SHOW_POOL_STATUS("status_info_pool_", status_info_pool_);
	SHOW_POOL_STATUS("arena_player_pool_", arena_player_pool_);
	SHOW_POOL_STATUS("inner_arena_player_pool_", inner_arena_player_pool_);
	SHOW_POOL_STATUS("map_team_pool_", map_team_pool_);
	SHOW_POOL_STATUS("dragon_vale_pool_", dragon_vale_pool_);
	SHOW_POOL_STATUS("knight_trials_pool_", knight_trials_pool_);
	SHOW_POOL_STATUS("soldier_pool_", soldier_pool_);
}

Logic_Player *Pool_Manager::pop_logic_player(void) {
	return logic_player_pool_.pop();
}

int Pool_Manager::push_logic_player(Logic_Player *player) {
	return logic_player_pool_.push(player);
}

Gate_Player *Pool_Manager::pop_gate_player(void) {
	return gate_player_pool_.pop();
}

int Pool_Manager::push_gate_player(Gate_Player *player) {
	return gate_player_pool_.push(player);
}

Robot_Player *Pool_Manager::pop_robot_player(void) {
	return robot_player_pool_.pop();
}

int Pool_Manager::push_robot_player(Robot_Player *player) {
	return robot_player_pool_.push(player);
}

Scene_Player *Pool_Manager::pop_scene_player(void) {
	return scene_player_pool_.pop();
}

int Pool_Manager::push_scene_player(Scene_Player *player) {
	return scene_player_pool_.push(player);
}

Chat_Player *Pool_Manager::pop_chat_player(void) {
	return chat_player_pool_.pop();
}

int Pool_Manager::push_chat_player(Chat_Player *player) {
	return chat_player_pool_.push(player);
}

Player_DB_View *Pool_Manager::pop_player_db_view(void) {
	return player_db_view_pool_.pop();
}

int Pool_Manager::push_player_db_view(Player_DB_View *db_view) {
	return player_db_view_pool_.push(db_view);
}

Block_Buffer *Pool_Manager::pop_db_oper_block_pool(void) {
	return db_oper_block_pool_.pop();
}

int Pool_Manager::push_db_oper_block_pool(Block_Buffer *block) {
	return db_oper_block_pool_.push(block);
}


DB_Worker *Pool_Manager::pop_db_worker_pool(void) {
	return db_worker_pool_.pop();
}

int Pool_Manager::push_db_worker_pool(DB_Worker *worker) {
	return db_worker_pool_.push(worker);
}

Global_Timer_Handler *Pool_Manager::pop_global_timer_handler_pool(void) {
	return global_timer_handler_pool_.pop();
}

int Pool_Manager::push_global_timer_handler_pool(Global_Timer_Handler *block) {
	return global_timer_handler_pool_.push(block);
}

mongo::DBClientConnection *Pool_Manager::pop_dbclientConnection_pool(void) {
	return db_connection_pool_.pop();
}

int Pool_Manager::push_dbclientConnection_pool(mongo::DBClientConnection *block) {
	return db_connection_pool_.push(block);
}

Logic_Player_Detail *Pool_Manager::pop_player_data_pool(int type) {
	player_data_pool_.debug(1, type);
	return player_data_pool_.pop();
}

int Pool_Manager::push_player_data_pool(Logic_Player_Detail *data, int type) {
	player_data_pool_.debug(1, type);
	return player_data_pool_.push(data);
}

Scene_Player_Detail *Pool_Manager::pop_scene_player_data_pool(void) {
	return scene_player_data_pool_.pop();
}

int Pool_Manager::push_scene_player_data_pool(Scene_Player_Detail *data) {
	return scene_player_data_pool_.push(data);
}

Item_Detail *Pool_Manager::pop_item_pool(int type) {
	item_pool_.debug(1, type);
	return item_pool_.pop();
}

int Pool_Manager::push_item_pool(Item_Detail *item_detail, int type) {
	item_pool_.debug(1, type);
	return item_pool_.push(item_detail);
}

Team *Pool_Manager::pop_team_pool(void) {
	return team_pool_.pop();
}

int Pool_Manager::push_team_pool(Team *team) {
	return team_pool_.push(team);
}

Battle_Scene *Pool_Manager::pop_battle_scene_pool(void) {
	return battle_scene_pool_.pop();
}

int Pool_Manager::push_battle_scene_pool(Battle_Scene *scene) {
	return battle_scene_pool_.push(scene);
}

Hero *Pool_Manager::pop_hero(void) {
	return hero_pool_.pop();
}

int Pool_Manager::push_hero(Hero *hero) {
	return hero_pool_.push(hero);
}

World_Boss_Manager *Pool_Manager::pop_world_boss_manager(void){
	return world_boss_manager_pool_.pop();
}

int Pool_Manager::push_world_boss_manager(World_Boss_Manager* world_boss){
	return world_boss_manager_pool_.push(world_boss);
}

Inner_Arena_Manager *Pool_Manager::pop_inner_arena_manager(void){
	return inner_arena_manager_pool_.pop();
}

int Pool_Manager::push_inner_arena_manager(Inner_Arena_Manager* inner_arena){
	return inner_arena_manager_pool_.push(inner_arena);
}

Status_Detail* Pool_Manager::pop_status_detail_pool(void) {
	return status_pool_.pop();
}
int Pool_Manager::push_status_detail_pool(Status_Detail* status){
	return status_pool_.push(status);
}

Gang_DB_View* Pool_Manager::pop_gang_db_view(void) {
	return gang_db_view_pool_.pop();
}

int Pool_Manager::push_gang_db_view(Gang_DB_View* gang_db_view) {
	return gang_db_view_pool_.push(gang_db_view);
}

Status_Info *Pool_Manager::pop_status_info_pool(void) {
	return status_info_pool_.pop();
}

int Pool_Manager::push_status_info_pool(Status_Info *status_info) {
	return status_info_pool_.push(status_info);
}

Ganger_Detail* Pool_Manager::pop_ganger_detail() {
	return ganger_detail_pool_.pop();
}

int Pool_Manager::push_ganger_detail(Ganger_Detail* detail) {
	return ganger_detail_pool_.push( detail );
}

Arena_Rank_Player *Pool_Manager::pop_arena_player_pool(void){
	return arena_player_pool_.pop();
}

int Pool_Manager::push_arena_player_pool(Arena_Rank_Player *obj){
	return arena_player_pool_.push(obj);
}

Inner_Arena_Rank_Player *Pool_Manager::pop_inner_arena_player_pool(void){
	return inner_arena_player_pool_.pop();
}

int Pool_Manager::push_inner_arena_player_pool(Inner_Arena_Rank_Player *obj){
	return inner_arena_player_pool_.push(obj);
}

Map_Team *Pool_Manager::pop_map_team_pool(void) {
	return map_team_pool_.pop();
}

int Pool_Manager::push_map_team_pool(Map_Team *team) {
	return map_team_pool_.push(team);
}

Curl_Worker *Pool_Manager::pop_curl_worker_pool(void) {
	return curl_worker_pool_.pop();
}

int Pool_Manager::push_curl_worker_pool(Curl_Worker *curl) {
	return curl_worker_pool_.push(curl);
}

Http_Info *Pool_Manager::pop_http_info_pool(void) {
	return http_info_pool_.pop();
}

int Pool_Manager::push_http_info_pool(Http_Info *info) {
	return http_info_pool_.push(info);
}

Ranking *Pool_Manager::rank_data_pool(void) {
	return ranking_pool_.pop();
}

int Pool_Manager::push_rank_data_pool(Ranking *rank) {
	return ranking_pool_.push(rank);
}

Knight_Trials_Player *Pool_Manager::pop_knight_trials_player_data_pool(void) {
	return knight_trials_pool_.pop();
}

int Pool_Manager::push_knight_trials_player_data_pool(Knight_Trials_Player *player) {
	return knight_trials_pool_.push(player);
}

Soldier *Pool_Manager::pop_soldier_pool(void) {
	return soldier_pool_.pop();
}

int Pool_Manager::push_soldier_pool(Soldier *soldier) {
	return soldier_pool_.push(soldier);
}

Expedition_Manager_Data *Pool_Manager::pop_expedition_manager_data(void) {
	return expedition_manager_data_pool_.pop();
}
int Pool_Manager::push_expedition_manager_data(Expedition_Manager_Data *data) {
	return expedition_manager_data_pool_.push(data);
}
