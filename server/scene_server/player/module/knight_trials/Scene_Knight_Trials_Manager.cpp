/*
 * Scene_Knight_Trials_Manager.cpp
 *
 *  Created on: 2014年9月16日
 *      Author: xiaoliang
 */

#include "knight_trials/Scene_Knight_Trials_Manager.h"
#include "knight_trials/Knight_Trials_Def.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Knight_Trials.h"
#include "Global_Scene_Manager.h"
#include "NPC/NPC_Manager.h"
#include "Scene_Player.h"
#include "scene_battle/Battle_Scene.h"

void Scene_Knight_Trials_Manager::reset() {

}

int Scene_Knight_Trials_Manager::scene_knight_trials_match_player(Block_Buffer& buf) {
	uint8_t type = 0;
	buf.read_uint8(type);
	role_id_t role_id = 0;
	buf.read_int64(role_id);
	if(!type || !role_id) {
		return 0;
	}
	int64_t from_uuid = 0;
	buf.read_int64(from_uuid);
	switch(type) {
		case KNIGHT_TRIALS_SIX_MATCH: {
			int ttl_force = 0;
			buf.read_int32(ttl_force);
			std::vector<int64_t> uuid_vector;
			SCENE_MONITOR->logic_server_uuid_to_vector(uuid_vector);
			//为了排除自己服;
			for(std::vector<int64_t>::iterator it = uuid_vector.begin(); it != uuid_vector.end(); ++it) {
				if(*it == from_uuid) {
					uuid_vector.erase(it);
					break;
				}
			}
			if(uuid_vector.empty()) {
				return 0;
			}
			std::map<int64_t, std::vector<uint8_t>> uuid_num_map;
			const Knight_Trials_Points_Conf_Map &conf = CONFIG_CACHE_KNIGHT_TRIALS->get_points_conf();
			for(uint8_t i=1; i<=conf.size(); i++){
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint32_t index = random() % uuid_vector.size();
				std::map<int64_t, std::vector<uint8_t>>::iterator it = uuid_num_map.find(uuid_vector[index]);
				if(it == uuid_num_map.end()) {
					std::vector<uint8_t> rank_vec;
					rank_vec.clear();
					rank_vec.push_back(i);
					uuid_num_map.insert(std::make_pair(uuid_vector[index], rank_vec));
				} else {
					it->second.push_back(i);
				}
			}
			for(std::map<int64_t, std::vector<uint8_t>>::iterator it = uuid_num_map.begin(); it != uuid_num_map.end(); ++it) {
				Block_Buffer block_buf;
				block_buf.make_message(INNER_KNIGHT_TRIALS_MATCH_PLAYER);
				block_buf.write_uint8(KNIGHT_TRIALS_SIX_MATCH);
				block_buf.write_int64(role_id);
				block_buf.write_int64(from_uuid);
				block_buf.write_int32(ttl_force);
				block_buf.write_uint8(it->second.size());
				for(std::vector<uint8_t>::iterator rit = it->second.begin(); rit != it->second.end(); ++rit) {
					block_buf.write_uint8(*rit);
				}
				block_buf.finish_message();
				SCENE_MONITOR->send_to_logic(it->first, block_buf);
			}
			break;
		}
		case KNIGHT_TRIALS_ONE_MATCH: {
			break;
		}
		case KNIGHT_TRIALS_SIX_MATCH_BACK: {
			Block_Buffer block_buf;
			block_buf.make_message(INNER_KNIGHT_TRIALS_MATCH_PLAYER);
			block_buf.write_uint8(KNIGHT_TRIALS_SIX_MATCH_BACK);
			block_buf.write_int64(role_id);
			block_buf.write_int64(from_uuid);
			block_buf.copy(&buf);
			SCENE_MONITOR->send_to_logic(from_uuid, block_buf);
			break;
		}
		case KNIGHT_TRIALS_ONE_MATCH_BACK: {
			break;
		}
	}
	return 0;
}

int Scene_Knight_Trials_Manager::scene_knight_trials_enter_fighter(Scene_Player *player, Block_Buffer& buf) {
	if(player == NULL){
		return -1;
	}
	if(player->battle_scene()) { //正在战斗中
		MSG_DEBUG("knight trials enter fighter is on fighter role_id:%ld, battle_status_:%d, battle_type_:%d",
				player->role_id(), player->battle_scene()->battle_status() , player->battle_scene()->get_battle_type());
		//MSG_DEBUG("knight trials enter fighter is on fighter role_id:%ld", 	player->role_id());
		return -1;
	}
	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
	if(!scene){
		return -1;
	}
	scene->set_battle_type(Battle_Scene::BATTLE_TYPE_KNIGHT_TRIALS);
	scene->set_map_id(KNIGHT_TRIALS_FIGHTER_SCENE_MAP_ID);

	std::string role_name;
	role_name.clear();
	int8_t career = 0;
	int8_t gender = 0;
	buf.read_int8(career);
	buf.read_int8(gender);
	buf.read_string(role_name);
	int8_t size = 0;
	buf.read_int8(size);
	Int_Vec avatar_vec;
	for (int8_t i=0; i<size; ++i) {
		int avatar_id = 0;
		buf.read_int32(avatar_id);
		avatar_vec.push_back(avatar_id);
	}

	NPC_Addition_Info add_info;
	add_info.birth_coord.x = 1;
	add_info.birth_coord.y = 5;
	add_info.ex_val1 = career;
	add_info.ex_val2 = gender;
	add_info.name = role_name;
	int monster_type = 63800108;
	Player_Monster* player_monster;
	player_monster = NPC_MANAGER->create_player_monster(monster_type, NULL, add_info, scene, buf);
	if(player_monster == NULL){
		return -1;
	}

	player_monster->set_player_monster_avatar(avatar_vec);
	player_monster->hero_battle_birth(buf, scene);
	player_monster->battle_enter_appaer(scene);
	double morale_init = player_monster->fighter_detail().fetch_fight_property(Property_Type::PT_INIT_MORALE, Prop_Value::ELEM_NORMAL);
	Hero* hero = player_monster->find_hero();
	if(0 != hero) {
		morale_init += hero->fighter_detail().fetch_fight_property(Property_Type::PT_INIT_MORALE, Prop_Value::ELEM_NORMAL);
	}
	player_monster->reset_morale_and_hp(morale_init, player_monster->blood_max(), false);

	if(player->set_battle_position(0, Position_Key::LINE_TWO_FRONT, scene) == 0){
		player->enter_battle_scene(scene);
	}
	return 0;
}
