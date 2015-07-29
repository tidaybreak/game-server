/*
 * Scene_Dragon_Vale_Manager.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: gaojianshan
 */


#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Dragon_Vale_Enum.h"
#include "Msg_Dragon_Vale_Struct.h"
#include "Scene_Player.h"
#include "Global_Scene_Manager.h"
#include "NPC/NPC_Manager.h"
#include "Err_Code.h"
#include "Block_Buffer.h"
#include "scene_battle/Battle_Scene.h"
#include "Config_Cache_Dragon_Vale.h"
#include "Msg_Active_Struct.h"
#include "Msg_Active_Enum.h"
#include "dragon_vale_scene/Dragon_Vale_Scene.h"
#include "dragon/Dragon_Vale_Def.h"
#include "dragon_vale_scene/Dragon_Vale_Scene_Manager.h"

void Scene_Dragon_Vale_Manager::reset() {
	dragon_vale_target_rob_map.clear();
	dragon_vale_rob_target_map.clear();
	scene_complete_rob_map.clear();
	scene_complete_target_map.clear();
	total_rob_production_info_map.clear();
}

int Scene_Dragon_Vale_Manager::process_rob_dragon_vale_match(int64_t uuid, std::string server_name, int64_t role_id,
														 uint32_t coin, uint32_t last_coin) {
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
	if(!player) {
		return 0;
	}

	std::vector<int64_t> uuid_vector;
	SCENE_MONITOR->logic_server_uuid_to_vector(uuid_vector);
	if(uuid_vector.empty()) {
		MSG_USER("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
		MSG_USER("DRAGON_VALE_ROB: uuid_vector is empty");
		MSG_USER("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

		//back coin
		MSG_20600020 msg;
		msg.uuid = uuid;
		msg.role_id = role_id;
		msg.coin = coin;
		msg.type = 1;

		Block_Buffer block_buf;
		block_buf.make_message(INNER_ROB_MATCH_FAIL_BACK_COIN);
		msg.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(uuid, block_buf);

		//error notice
		MSG_82300027 msg_82300027;
		msg_82300027.error_id = ERROR_ROB_MATCH_FAIL;
		msg_82300027.coin = last_coin;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		return 0;
	}

	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());

	uint32_t index = random() % uuid_vector.size();
	const Monitor_Unique_ID* monitor_unique_id =
			SCENE_MONITOR->get_monitor_unique_id_by_uuid(uuid_vector[index], Server_Enum::MONITOR_LOGIC);
	if(!monitor_unique_id) {
		MSG_USER("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
		MSG_USER("DRAGON_VALE_ROB: monitor_unique_id is null");
		MSG_USER("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

		//back coin
		MSG_20600020 msg;
		msg.uuid = uuid;
		msg.role_id = role_id;
		msg.coin = coin;
		msg.type = 1;

		Block_Buffer block_buf;
		block_buf.make_message(INNER_ROB_MATCH_FAIL_BACK_COIN);
		msg.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(uuid, block_buf);

		//error notice
		MSG_82300027 msg_82300027;
		msg_82300027.error_id = ERROR_ROB_MATCH_FAIL;
		msg_82300027.coin = last_coin;
		OBJ_SEND_TO_CLIENT(msg_82300027, (*player));
		return 0;
	}

	MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	MSG_USER("server size:%d", uuid_vector.size());
	for(std::vector<int64_t>::const_iterator it = uuid_vector.begin(); it != uuid_vector.end();++it) {
		MSG_USER("     uuid=%ld", *it);
		const Monitor_Unique_ID* test_id =
				SCENE_MONITOR->get_monitor_unique_id_by_uuid(*it, Server_Enum::MONITOR_LOGIC);
		if(!test_id) {
			continue;
		}
		MSG_USER("     ip=%s", test_id->ip.c_str());
		MSG_USER("     port=%d", test_id->port);
		MSG_USER("     type=%d", (int)test_id->type);
	}
	MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	MSG_USER("         matched uuid=%ld", uuid_vector[index]);
	MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	MSG_20600001 msg;
	msg.uuid = uuid;
	msg.role_id = role_id;
	msg.server_name = server_name;
	msg.role_name = player->role_name();
	msg.level = player->level();
	msg.coin = coin;
	msg.last_coin = last_coin;

	Block_Buffer block_buf;
	block_buf.make_message(INNER_SCENE_TO_LOGIC_ROB_MATCH);
	msg.serialize(block_buf);
	block_buf.finish_message();

	SCENE_MONITOR->send_to_logic(uuid_vector[index], block_buf);
	return 0;
}

int Scene_Dragon_Vale_Manager::process_rob_match(const MSG_20600002 &msg) {
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(msg.to_role_id);
	if(!player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}

	MSG_50180016 msg_50180016;
	msg_50180016.to_uuid = msg.to_uuid;
	msg_50180016.to_server_name = msg.to_server_name;
	msg_50180016.uuid = msg.uuid;
	msg_50180016.role_id = msg.role_id;
	msg_50180016.server_name = msg.server_name;
	msg_50180016.fight = msg.fight;
	msg_50180016.rob_gold_mine = msg.rob_gold_mine;
	msg_50180016.rob_moon_weel = msg.rob_moon_well;
	msg_50180016.rob_totem = msg.rob_totem;
	msg_50180016.coin = msg.coin;
	msg_50180016.rob_match_member_list.clear();
	msg_50180016.rob_match_member_list.assign(msg.match_member_list.begin(), msg.match_member_list.end());

	OBJ_SEND_TO_CLIENT(msg_50180016, (*player));
	return 0;
}

int Scene_Dragon_Vale_Manager::process_rob_fight_load_player_data(int64_t to_uuid, int64_t to_role_id,
				        std::string to_server_name, std::string to_role_name,
						int64_t uuid, int64_t role_id) {

	{
		MSG_20600034 msg;
		msg.role_id = role_id;
		msg.flag = 1;
		Block_Buffer block_buf;
		block_buf.make_message(msg.msg_id);
		msg.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(uuid, block_buf);
	}


	MSG_20600003 msg;
	msg.to_uuid = to_uuid;
	msg.to_role_id = to_role_id;
	msg.to_server_name = to_server_name;
	msg.to_role_name = to_role_name;
	msg.role_id = role_id;

	Block_Buffer block_buf;
	block_buf.make_message(INNER_REQ_DRAGON_VALE_ROB_FIGHT_LOAD);
	msg.serialize(block_buf);
	block_buf.finish_message();

	SCENE_MONITOR->send_to_logic(uuid, block_buf);
	return 0;
}

int Scene_Dragon_Vale_Manager::process_rob_fight(Block_Buffer &buf) {
	MSG_DEBUG("================================================");
	MSG_DEBUG("before enter battle scene");

	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	int64_t to_uuid = 0;
	int64_t to_role_id = 0;
	std::string to_server_name;
	std::string to_role_name;
	int64_t uuid = 0;
	int64_t role_id = 0;
	std::string server_name;
	std::string role_name;
	buf.read_int64(to_uuid);
	buf.read_int64(to_role_id);
	buf.read_string(to_server_name);
	buf.read_string(to_role_name);
	buf.read_int64(uuid);
	buf.read_int64(role_id);
	buf.read_string(server_name);
	buf.read_string(role_name);

	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(to_role_id);
	if(!player) {
		MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		MSG_USER("player is null");
		return ERROR_DRAGON_VALE_ROBBER_OFFLINE;
	}

	if (player->is_in_battle()) {
		MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		MSG_USER("has in battle");
		return ERROR_DRAGON_VALE_ROBBER_IN_BATTLE;
	}

	if (player->dead_status()) {
		MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		MSG_USER("has die");
		return ERROR_DRAGON_VALE_ROBBER_DEAD;
	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
	if(!scene){
		MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		MSG_USER("scene is null");
		return -1;
	}
	scene->set_battle_type(Battle_Scene::BATTLE_TYPE_DRAGON_VALE);
	scene->set_map_id(main_base_cfg.battle_map_id);

	int8_t career = 0;
	int8_t gender = 0;
	buf.read_int8(career);
	buf.read_int8(gender);

	Int_Vec avatar_vec;
	uint32_t size = 0;
	buf.read_uint32(size);

	for(uint32_t i = 0; i < size; ++i) {
		int32_t avatar = 0;
		buf.read_int32(avatar);
		avatar_vec.push_back(avatar);
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
		MSG_USER("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		MSG_USER("player_monster is null");
		avatar_vec.clear();
		return -1;
	}

	Dragon_Vale_Rob_Fight rob;
	rob.uuid = to_uuid;
	rob.role_id = to_role_id;
	rob.server_name = to_server_name;
	rob.role_name = to_role_name;
	Dragon_Vale_Rob_Fight target;
	target.uuid = uuid;
	target.role_id = role_id;
	target.server_name = server_name;
	target.role_name = role_name;
	get_dragon_vale_target_rob_map().insert(std::make_pair(target, rob));
	get_dragon_vale_rob_target_map().insert(std::make_pair(rob, target));

	if(!avatar_vec.empty()) {
		player_monster->set_player_monster_avatar(avatar_vec);
	}

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

	avatar_vec.clear();
	MSG_DEBUG("=============================================");
	MSG_DEBUG("enter battle scene complete");
	return 0;
}

void Scene_Dragon_Vale_Manager::tick(Time_Value &now) {
	listen_auto_enter_battle();
}

void Scene_Dragon_Vale_Manager::listen_auto_enter_battle() {
	if(dragon_vale_rob_target_map.empty()) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	Scene_Player *rob_player = 0;
	Dragon_Vale_Rob_Fight_Map temp;
	temp.clear();
	temp.insert(dragon_vale_rob_target_map.begin(), dragon_vale_rob_target_map.end());

	for(Dragon_Vale_Rob_Fight_Map::iterator it = temp.begin();
			it != temp.end(); ++it) {
		rob_player = SCENE_MONITOR->find_scene_player_by_role_id(it->first.role_id);
		if(!rob_player) {
			continue;
		}
		if(!rob_player->get_dragon_vale_rob()) {
			continue;
		}

		if(rob_player->get_begin_rob_count_down().sec() + 10 > now.sec()) {
			continue;
		}

		process_rob_fight_load_player_data(it->first.uuid, it->first.role_id,
							it->first.server_name, it->first.role_name, it->second.uuid, it->second.role_id);
		rob_player->set_dragon_vale_rob(false);
	}

	temp.clear();
}

void Scene_Dragon_Vale_Manager::process_win_and_lose(Scene_Player &win_player, Scene_Player &lose_player) {
	Dragon_Vale_Rob_Fight_Map::const_iterator rob_it = get_dragon_vale_rob_target_map().end();

	Dragon_Vale_Rob_Fight win;
	win.uuid = win_player.monitor_link().logic_muid.uuid;
	win.role_id = win_player.role_id();

	Dragon_Vale_Rob_Fight lose;
	lose.uuid = lose_player.monitor_link().logic_muid.uuid;
	lose.role_id = lose_player.role_id();

	rob_it = get_dragon_vale_rob_target_map().find(win);
	if(rob_it != get_dragon_vale_rob_target_map().end()) {
		if(rob_it->second.uuid != lose.uuid || rob_it->second.role_id != lose.role_id) {
			MSG_USER("rob  error, rob=%ld, target=%ld, win_player=%ld, lose_player=%ld",
					rob_it->first.role_id, rob_it->second.role_id, win_player.role_id(), lose_player.role_id());
			return;
		}

		Scene_Complete rob_scene;
		rob_scene.uuid = rob_it->second.uuid;
		rob_scene.role_id = rob_it->second.role_id;
		rob_scene.server_name = rob_it->second.server_name;
		rob_scene.role_name = rob_it->second.role_name;
		rob_scene.result = 1;
		get_scene_complete_rob_map().insert(std::make_pair(rob_it->first, rob_scene));

		Scene_Complete target_scene;
		target_scene.uuid = rob_it->first.uuid;
		target_scene.role_id = rob_it->first.role_id;
		target_scene.server_name = rob_it->first.server_name;
		target_scene.role_name = rob_it->first.role_name;
		target_scene.result = 2;
		get_scene_complete_target_map().insert(std::make_pair(rob_it->second, target_scene));

		get_dragon_vale_rob_target_map().erase(win);
		get_dragon_vale_target_rob_map().erase(lose);
		return;
	}

	rob_it = get_dragon_vale_rob_target_map().find(lose);
	if(rob_it != get_dragon_vale_rob_target_map().end()) {
		if(rob_it->second.uuid != win.uuid || rob_it->second.role_id != win.role_id) {
			MSG_USER("rob  error, rob=%ld, target=%ld, win_player=%ld, lose_player=%ld",
					rob_it->first.role_id, rob_it->second.role_id, win_player.role_id(), lose_player.role_id());
			return;
		}

		Scene_Complete rob_scene;
		rob_scene.uuid = rob_it->second.uuid;
		rob_scene.role_id = rob_it->second.role_id;
		rob_scene.server_name = rob_it->second.server_name;
		rob_scene.role_name = rob_it->second.role_name;
		rob_scene.result = 2;
		get_scene_complete_rob_map().insert(std::make_pair(rob_it->first, rob_scene));

		Scene_Complete target_scene;
		target_scene.uuid = rob_it->first.uuid;
		target_scene.role_id = rob_it->first.role_id;
		target_scene.server_name = rob_it->first.server_name;
		target_scene.role_name = rob_it->first.role_name;
		target_scene.result = 1;
		get_scene_complete_target_map().insert(std::make_pair(rob_it->second, target_scene));

		get_dragon_vale_rob_target_map().erase(lose);
		get_dragon_vale_target_rob_map().erase(win);
		return;
	}
}

void Scene_Dragon_Vale_Manager::process_win(Scene_Player &win_player) {
	Dragon_Vale_Rob_Fight rob;
	rob.uuid = win_player.monitor_link().logic_muid.uuid;
	rob.role_id = win_player.role_id();

	Dragon_Vale_Rob_Fight_Map::const_iterator rob_it = get_dragon_vale_rob_target_map().find(rob);
	if(rob_it != get_dragon_vale_rob_target_map().end()) {
		Scene_Complete rob_scene;
		rob_scene.uuid = rob_it->second.uuid;
		rob_scene.role_id = rob_it->second.role_id;
		rob_scene.server_name = rob_it->second.server_name;
		rob_scene.role_name = rob_it->second.role_name;
		rob_scene.result = 1;
		get_scene_complete_rob_map().insert(std::make_pair(rob_it->first, rob_scene));

/*		MSG_20600006 msg_20600006;
		msg_20600006.server_name = rob_it->first.server_name;
		msg_20600006.player_name = rob_it->first.role_name;
		msg_20600006.result = 2;
		msg_20600006.role_id = rob_it->second.role_id;
		Block_Buffer block_buf;
		block_buf.make_message(INNER_NOTIFY_LOGIC_SAVE_TARGET_RESULT);
		msg_20600006.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(rob_it->second.uuid, block_buf);*/

		Dragon_Vale_Rob_Fight target;
		target.uuid = rob_it->second.uuid;
		target.role_id = rob_it->second.role_id;
		get_dragon_vale_rob_target_map().erase(rob);
		get_dragon_vale_target_rob_map().erase(target);
		return;
	}

	Dragon_Vale_Rob_Fight target;
	target.uuid = win_player.monitor_link().logic_muid.uuid;
	target.role_id = win_player.role_id();

	Dragon_Vale_Rob_Fight_Map::const_iterator target_it = get_dragon_vale_target_rob_map().find(target);
	if(target_it != get_dragon_vale_target_rob_map().end()) {
/*		MSG_20600005 msg_20600005;
		msg_20600005.server_name = target_it->first.server_name;
		msg_20600005.player_name = target_it->first.role_name;
		msg_20600005.result = 2;
		msg_20600005.role_id = target_it->second.role_id;
		Block_Buffer block_buf;
		block_buf.make_message(INNER_NOTIFY_LOGIC_SAVE_ROB_RESULT);
		msg_20600005.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(target_it->second.uuid, block_buf);*/

		Scene_Complete target_scene;
		target_scene.uuid = target_it->second.uuid;
		target_scene.role_id = target_it->second.role_id;
		target_scene.server_name = target_it->second.server_name;
		target_scene.role_name = target_it->second.role_name;
		target_scene.result = 1;
		get_scene_complete_target_map().insert(std::make_pair(target_it->first, target_scene));

		rob.uuid = target_it->second.uuid;
		rob.role_id = target_it->second.role_id;
		get_dragon_vale_rob_target_map().erase(rob);
		get_dragon_vale_target_rob_map().erase(target);
		return;
	}
}

void Scene_Dragon_Vale_Manager::process_lose(Scene_Player &lose_player) {
	Dragon_Vale_Rob_Fight rob;
	rob.uuid = lose_player.monitor_link().logic_muid.uuid;
	rob.role_id = lose_player.role_id();

	Dragon_Vale_Rob_Fight_Map::const_iterator rob_it = get_dragon_vale_rob_target_map().find(rob);
	if(rob_it != get_dragon_vale_rob_target_map().end()) {
		Scene_Complete rob_scene;
		rob_scene.uuid = rob_it->second.uuid;
		rob_scene.role_id = rob_it->second.role_id;
		rob_scene.server_name = rob_it->second.server_name;
		rob_scene.role_name = rob_it->second.role_name;
		rob_scene.result = 2;
		get_scene_complete_rob_map().insert(std::make_pair(rob_it->first, rob_scene));

/*		MSG_20600006 msg_20600006 ;
		msg_20600006.server_name = rob_it->first.server_name;
		msg_20600006.player_name = rob_it->first.role_name;
		msg_20600006.result = 1;
		msg_20600006.role_id = rob_it->second.role_id;
		Block_Buffer block_buf;
		block_buf.make_message(INNER_NOTIFY_LOGIC_SAVE_TARGET_RESULT);
		msg_20600006.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(rob_it->second.uuid, block_buf);*/

		Dragon_Vale_Rob_Fight target;
		target.uuid = rob_it->second.uuid;
		target.role_id = rob_it->second.role_id;
		get_dragon_vale_rob_target_map().erase(rob);
		get_dragon_vale_target_rob_map().erase(target);
		return;
	}

	Dragon_Vale_Rob_Fight target;
	target.uuid = lose_player.monitor_link().logic_muid.uuid;
	target.role_id = lose_player.role_id();

	Dragon_Vale_Rob_Fight_Map::const_iterator target_it = get_dragon_vale_target_rob_map().find(target);
	if(target_it != get_dragon_vale_target_rob_map().end()) {
/*		MSG_20600005 msg_20600005;
		msg_20600005.server_name = target_it->first.server_name;
		msg_20600005.player_name = target_it->first.role_name;
		msg_20600005.result = 1;
		msg_20600005.role_id = target_it->second.role_id;
		Block_Buffer block_buf;
		block_buf.make_message(INNER_NOTIFY_LOGIC_SAVE_ROB_RESULT);
		msg_20600005.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(target_it->second.uuid, block_buf);*/

		Scene_Complete target_scene;
		target_scene.uuid = target_it->second.uuid;
		target_scene.role_id = target_it->second.role_id;
		target_scene.server_name = target_it->second.server_name;
		target_scene.role_name = target_it->second.role_name;
		target_scene.result = 2;
		get_scene_complete_target_map().insert(std::make_pair(target_it->first, target_scene));

		rob.uuid = target_it->second.uuid;
		rob.role_id = target_it->second.role_id;
		get_dragon_vale_rob_target_map().erase(rob);
		get_dragon_vale_target_rob_map().erase(target);
		return;
	}
}

void Scene_Dragon_Vale_Manager::notify_online_player_battle_result(Scene_Player &player, Move_Scene *scene) {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	Dragon_Vale_Rob_Fight player_rob_fight;
	player_rob_fight.uuid = player.monitor_link().logic_muid.uuid;
	player_rob_fight.role_id = player.role_id();
	Scene_Complete_Map::const_iterator rob_it = get_scene_complete_rob_map().find(player_rob_fight);
	if(rob_it != get_scene_complete_rob_map().end()) {
		//rob
		MSG_82300014 msg_82300014;
		msg_82300014.server_name = rob_it->second.server_name;
		msg_82300014.player_name = rob_it->second.role_name;
		msg_82300014.result = (rob_it->second.result == 1) ? 1 : 2;
		msg_82300014.uuid = (int32_t)rob_it->second.uuid;
		msg_82300014.role_id = rob_it->second.role_id;
		if(1 == msg_82300014.result) {
			msg_82300014.limit_time = 30;

			MSG_20600027 msg_20600027;
			Block_Buffer buf_20600027;
			buf_20600027.make_message(INNNER_ROBBER_WIN_IN_ROB_FIGHT);
			msg_20600027.serialize(buf_20600027);
			buf_20600027.finish_message();
			player.send_to_logic(buf_20600027);
		}//end if 1

		if(2 == msg_82300014.result) {
			//notice board
			{
				MSG_20600017 msg_20600017;
				msg_20600017.name = player.role_name();
				msg_20600017.role_id = rob_it->second.role_id;
				Block_Buffer block_buf;
				block_buf.make_message(INNER_SAVE_NOTICE_BOARD_DRIVE_ROBBER);
				msg_20600017.serialize(block_buf);
				block_buf.finish_message();
				SCENE_MONITOR->send_to_logic(rob_it->second.uuid, block_buf);
			}

			//clear rob protect status
			{
				MSG_20600021 msg_20600021;
				msg_20600021.role_id = rob_it->second.role_id;
				Block_Buffer buf_20600021;
				buf_20600021.make_message(INNER_CLEAR_DRAGON_VALE_ROB_PROTECT_STATUS);
				msg_20600021.serialize(buf_20600021);
				buf_20600021.finish_message();
				SCENE_MONITOR->send_to_logic(rob_it->second.uuid, buf_20600021);
			}

			//被掠夺者成功守护龙谷，全服广播
			{
				MSG_20600026 msg_20600026;
				msg_20600026.to_server_name = rob_it->first.server_name;
				msg_20600026.to_role_name = rob_it->first.role_name;
				msg_20600026.role_id = rob_it->second.role_id;
				Block_Buffer buf_20600026;
				buf_20600026.make_message(INNER_SUCCESS_PROTECT_DRAGON_VALE);
				msg_20600026.serialize(buf_20600026);
				buf_20600026.finish_message();
				SCENE_MONITOR->send_to_logic(rob_it->second.uuid, buf_20600026);
			}
		}//end if 2

		OBJ_SEND_TO_CLIENT(msg_82300014, player);

		////////////////
		//target
		{
			MSG_20600034 msg;
			msg.role_id = rob_it->second.role_id;
			msg.flag = 0;
			Block_Buffer block_buf;
			block_buf.make_message(msg.msg_id);
			msg.serialize(block_buf);
			block_buf.finish_message();
			SCENE_MONITOR->send_to_logic(rob_it->second.uuid, block_buf);
		}

		Dragon_Vale_Rob_Fight target_fight;
		target_fight.uuid = rob_it->second.uuid;
		target_fight.role_id = rob_it->second.role_id;

		Scene_Player *target_player = SCENE_MONITOR->find_scene_player_by_role_id(rob_it->second.role_id);
		if(!target_player) {
			MSG_20600032 msg_20600032;
			msg_20600032.reset();
			msg_20600032.role_id = target_fight.role_id;
			msg_20600032.server_name = rob_it->first.server_name;
			msg_20600032.player_name = rob_it->first.role_name;
			msg_20600032.result = (msg_82300014.result == 1) ? 2 : 1;

			if(1 == msg_20600032.result) {
				MSG_20600033 msg_20600033;
				msg_20600033.left_time = 0;
				msg_20600033.role_id = target_fight.role_id;
				Block_Buffer buf_20600033;
				buf_20600033.make_message(INNER_DRAONG_VALE_NO_ENTER);
				msg_20600033.serialize(buf_20600033);
				buf_20600033.finish_message();
				SCENE_MONITOR->send_to_logic(rob_it->second.uuid, buf_20600033);
			}//end  1

			if(2 == msg_20600032.result) {
				msg_20600032.is_drive = 0;

				MSG_20600033 msg_20600033;
				msg_20600033.left_time = main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;;
				msg_20600033.role_id = target_fight.role_id;
				Block_Buffer buf_20600033;
				buf_20600033.make_message(INNER_DRAONG_VALE_NO_ENTER);
				msg_20600033.serialize(buf_20600033);
				buf_20600033.finish_message();
				SCENE_MONITOR->send_to_logic(rob_it->second.uuid, buf_20600033);
			}//end  2

			Block_Buffer buf_20600032;
			buf_20600032.make_message(INNER_NOTIFY_MASTER_OF_DRAGON_FIGHT_END);
			msg_20600032.serialize(buf_20600032);
			buf_20600032.finish_message();
			SCENE_MONITOR->send_to_logic(rob_it->second.uuid, buf_20600032);

			get_scene_complete_rob_map().erase(player_rob_fight);
			get_scene_complete_target_map().erase(target_fight);
			return;
		}//end if
		else {
			MSG_82300015 msg_82300015;
			msg_82300015.reset();
			msg_82300015.server_name = rob_it->first.server_name;
			msg_82300015.player_name = rob_it->first.role_name;
			msg_82300015.result = (msg_82300014.result == 1) ? 2 : 1;

			if(1 == msg_82300015.result) {
				MSG_82300026 msg_82300026;
				msg_82300026.left_time = 0;
				OBJ_SEND_TO_CLIENT(msg_82300026, (*target_player));
			}//end 1

			if(2 == msg_82300015.result) {
				msg_82300015.is_drive = 1;

//				MSG_82300026 msg_82300026;
//				msg_82300026.left_time = main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
//				OBJ_SEND_TO_CLIENT(msg_82300026, (*target_player));

				MSG_20600033 msg_20600033;
				msg_20600033.left_time = main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;;
				msg_20600033.role_id = target_fight.role_id;
				Block_Buffer buf_20600033;
				buf_20600033.make_message(INNER_DRAONG_VALE_NO_ENTER);
				msg_20600033.serialize(buf_20600033);
				buf_20600033.finish_message();
				SCENE_MONITOR->send_to_logic(rob_it->second.uuid, buf_20600033);
			}//end 2

			OBJ_SEND_TO_CLIENT(msg_82300015, (*target_player));

			get_scene_complete_rob_map().erase(player_rob_fight);
			get_scene_complete_target_map().erase(target_fight);
		}//end else

		return;
	}//end if
}

void Scene_Dragon_Vale_Manager::notify_total_rob_production(Scene_Player &player, Move_Scene *scene) {
	Total_Rob_Production_Info_Map::iterator it = get_total_rob_production_info_map().find(player.role_id());
	if(it == get_total_rob_production_info_map().end()) {
		return;
	}

	MSG_82300017 msg_82300017;
	msg_82300017.to_uuid = it->second.to_uuid;
	msg_82300017.to_role_id = it->second.to_role_id;
	msg_82300017.uuid = it->second.uuid;
	msg_82300017.role_id = it->second.role_id;
	msg_82300017.server_name = it->second.server_name;
	msg_82300017.player_name = it->second.role_name;

	Rob_Production_Map::iterator prodcution_it = it->second.rob_production_map.find((uint8_t)BUILDING_GOLD_MINE);
	if(prodcution_it != it->second.rob_production_map.end()) {
		msg_82300017.total_gold_mine = prodcution_it->second;
	}

	prodcution_it = it->second.rob_production_map.find((uint8_t)BUILDING_MOON_WELL);
	if(prodcution_it != it->second.rob_production_map.end()) {
		msg_82300017.total_moon_weel = prodcution_it->second;
	}

	prodcution_it = it->second.rob_production_map.find((uint8_t)BUILDING_TOTEM);
	if(prodcution_it != it->second.rob_production_map.end()) {
		msg_82300017.total_totem = prodcution_it->second;
	}

	OBJ_SEND_TO_CLIENT(msg_82300017, player);

	//clear rob production process
	{
		MSG_20600022 msg_20600022;
		msg_20600022.to_role_id = it->second.to_role_id;
		Block_Buffer buf_20600022;
		buf_20600022.make_message(INNER_CLEAR_ROB_PRODUCTION_PROCESS);
		msg_20600022.serialize(buf_20600022);
		buf_20600022.finish_message();
		SCENE_MONITOR->send_to_logic(it->second.uuid, buf_20600022);
	}

	//notice board
	{
		MSG_20600018 msg_20600018;
		msg_20600018.role_id = it->second.role_id;
		msg_20600018.name = it->second.to_role_name;
		msg_20600018.gold_mine = msg_82300017.total_gold_mine;
		msg_20600018.moon_well = msg_82300017.total_moon_weel;
		msg_20600018.totem = msg_82300017.total_totem;
		Block_Buffer buf_20600018;
		buf_20600018.make_message(INNER_SAVE_NOTICE_BOARD_ROB_PRODUCTION);
		msg_20600018.serialize(buf_20600018);
		buf_20600018.finish_message();
		SCENE_MONITOR->send_to_logic(it->second.uuid, buf_20600018);
	}

	//notify TOD assistant
	{
		MSG_20600023 msg_20600023;
		msg_20600023.to_role_id = it->second.to_role_id;
		Block_Buffer buffer_20600023;
		buffer_20600023.make_message(INNNER_NOTIFY_TOD_ROB_PRODUCTION_FINISH);
		msg_20600023.serialize(buffer_20600023);
		buffer_20600023.finish_message();
		SCENE_MONITOR->send_to_logic(it->second.to_uuid, buffer_20600023);
	}

	//set first enter dragon vale  after robbed production
	{
		MSG_20600024 msg_20600024;
		msg_20600024.role_id = it->second.role_id;
		Block_Buffer buffer_20600024;
		buffer_20600024.make_message(INNER_SET_FIRST_ENTER_AFTER_ROBBED);
		msg_20600024.serialize(buffer_20600024);
		buffer_20600024.finish_message();
		SCENE_MONITOR->send_to_logic(it->second.uuid, buffer_20600024);
	}

	{
		MSG_20600028 msg_20600028;
		msg_20600028.role_id = player.role_id();
		msg_20600028.gold_mine = msg_82300017.total_gold_mine;
		msg_20600028.moon_well = msg_82300017.total_moon_weel;
		msg_20600028.totem = msg_82300017.total_totem;
		Block_Buffer buf_20600028;
		buf_20600028.make_message(INNER_ROBBER_GAIN_TOTAL_PRODUCTION);
		msg_20600028.serialize(buf_20600028);
		buf_20600028.finish_message();
		player.send_to_logic(buf_20600028);
	}

	MSG_82300018 msg_82300018;
	msg_82300018.server_name  = it->second.to_server_name;
	msg_82300018.player_name = it->second.to_role_name;
	msg_82300018.total_gold_mine = msg_82300017.total_gold_mine;
	msg_82300018.total_moon_weel = msg_82300017.total_moon_weel;
	msg_82300018.total_totem = msg_82300017.total_totem;

	Scene_Player *target_player = SCENE_MONITOR->find_scene_player_by_role_id(it->second.role_id);
	if(!target_player) {
		MSG_20600011 msg_20600011;
		msg_20600011.server_name  = it->second.to_server_name;
		msg_20600011.role_name = it->second.to_role_name;
		msg_20600011.total_gold_mine = msg_82300017.total_gold_mine;
		msg_20600011.total_moon_weel = msg_82300017.total_moon_weel;
		msg_20600011.total_totem = msg_82300017.total_totem;
		msg_20600011.role_id = it->second.role_id;
		Block_Buffer block_buf;
		block_buf.make_message(INNER_NOTIFY_LOGIC_SAVE_TARGET_ROBBED_PRODUCTION);
		msg_20600011.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(it->second.uuid, block_buf);
	}
	else {
		MSG_82300018 msg_82300018;
		msg_82300018.server_name  = it->second.to_server_name;
		msg_82300018.player_name = it->second.to_role_name;
		msg_82300018.total_gold_mine = msg_82300017.total_gold_mine;
		msg_82300018.total_moon_weel = msg_82300017.total_moon_weel;
		msg_82300018.total_totem = msg_82300017.total_totem;
		OBJ_SEND_TO_CLIENT(msg_82300018, (*target_player));
	}

	it->second.rob_production_map.clear();
	get_total_rob_production_info_map().erase(player.role_id());
}

void Scene_Dragon_Vale_Manager::process_rob_enter_dragon_vale(int64_t to_uuid, int64_t to_role_id,
									int64_t uuid, int64_t role_id, uint8_t limit_time) {
	MSG_20600007 msg_20600007;
	msg_20600007.to_uuid = to_uuid;
	msg_20600007.to_role_id = to_role_id;
	msg_20600007.uuid = uuid;
	msg_20600007.role_id = role_id;
	msg_20600007.limit_time = limit_time;
	Block_Buffer block_buf;
	block_buf.make_message(INNNER_ROB_ENTER_DRAGON_VALE);
	msg_20600007.serialize(block_buf);
	block_buf.finish_message();
	SCENE_MONITOR->send_to_logic(uuid, block_buf);
}

void Scene_Dragon_Vale_Manager::process_rob_enter_dragon_vale(const MSG_20600008 &msg) {
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(msg.to_role_id);
	if(!player) {
		return;
	}

	MSG_82300020 msg_82300020;
	msg_82300020.reset();
	msg_82300020.to_uuid = msg.to_uuid;
	msg_82300020.limit_time = msg.limit_time;
	msg_82300020.uuid = msg.uuid;
	msg_82300020.builing_info_list.assign(msg.builing_info_list.begin(), msg.builing_info_list.end());
	msg_82300020.head_info.role_id = msg.head_info.role_id;
	msg_82300020.head_info.role_name = msg.head_info.role_name;
	msg_82300020.head_info.star_num = msg.head_info.star_num;
	msg_82300020.head_info.head_photo_id = msg.head_info.head_photo_id;
	msg_82300020.head_info.star_add_speed = msg.head_info.star_add_speed;
	msg_82300020.head_info.thief_sub_speed = msg.head_info.thief_sub_speed;
	msg_82300020.head_info.foot_list.assign(msg.head_info.foot_list.begin(), msg.head_info.foot_list.end());
	msg_82300020.is_dragon_born = msg.is_dragon_born;
	msg_82300020.born_time = msg.born_time;
	msg_82300020.progress = msg.progress;
	msg_82300020.is_paly_dragon = msg.is_paly_dragon;
	msg_82300020.total_gold_mine = msg.total_gold_mine;
	msg_82300020.total_moon_weel = msg.total_moon_weel;
	msg_82300020.total_totem = msg.total_totem;

	OBJ_SEND_TO_CLIENT(msg_82300020, (*player));
}

void Scene_Dragon_Vale_Manager::process_gain_rob_production(const MSG_20600009 &msg) {
	Block_Buffer block_buf;
	block_buf.make_message(INNER_GAIN_ROB_PRODUCTION);
	msg.serialize(block_buf);
	block_buf.finish_message();
	SCENE_MONITOR->send_to_logic(msg.uuid, block_buf);
}

void Scene_Dragon_Vale_Manager::process_gain_rob_production_res(const MSG_20600010 &msg) {
	if(!msg.error_id) {
		Total_Rob_Production_Info_Map::iterator total_it = get_total_rob_production_info_map().find(msg.to_role_id);
		if(total_it == get_total_rob_production_info_map().end()) {
			Rob_Production_Map rob_production_map;
			rob_production_map.insert(std::make_pair(msg.building_info.type, msg.gain_production));
			Total_Rob_Production_Info total_info;
			total_info.to_uuid = msg.to_uuid;
			total_info.to_role_id = msg.to_role_id;
			total_info.to_server_name = msg.to_server_name;
			total_info.to_role_name = msg.to_role_name;
			total_info.uuid = msg.uuid;
			total_info.role_id = msg.role_id;
			total_info.server_name = msg.server_name;
			total_info.role_name = msg.role_name;
			total_info.rob_production_map.insert(rob_production_map.begin(), rob_production_map.end());
			get_total_rob_production_info_map().insert(std::make_pair(msg.to_role_id, total_info));
		}
		else {
				if(total_it->second.to_uuid != msg.to_uuid || total_it->second.to_role_id != msg.to_role_id
					|| total_it->second.uuid != msg.uuid || total_it->second.role_id != msg.role_id) {
					get_total_rob_production_info_map().clear();
					Rob_Production_Map rob_production_map;
					rob_production_map.insert(std::make_pair(msg.building_info.type, msg.gain_production));
					Total_Rob_Production_Info total_info;
					total_info.to_uuid = msg.to_uuid;
					total_info.to_role_id = msg.to_role_id;
					total_info.to_server_name = msg.to_server_name;
					total_info.to_role_name = msg.to_role_name;
					total_info.uuid = msg.uuid;
					total_info.role_id = msg.role_id;
					total_info.server_name = msg.server_name;
					total_info.role_name = msg.role_name;
					total_info.rob_production_map.insert(rob_production_map.begin(), rob_production_map.end());
					get_total_rob_production_info_map().insert(std::make_pair(msg.to_role_id, total_info));
				}
				else {
					Total_Rob_Production_Info &total_info = total_it->second;
					Rob_Production_Map::iterator it = total_info.rob_production_map.find(msg.building_info.type);
					if(it == total_info.rob_production_map.end()) {
						total_info.rob_production_map.insert(std::make_pair(msg.building_info.type, msg.gain_production));
					}
					else {
						it->second += msg.gain_production;
					}
				}
		}
	}

	Block_Buffer block_buf;
	block_buf.make_message(INNER_RES_GAIN_ROB_PRODUCTION);
	msg.serialize(block_buf);
	block_buf.finish_message();
	SCENE_MONITOR->send_to_logic(msg.to_uuid, block_buf);
}

void Scene_Dragon_Vale_Manager::process_broadcast_dragon_vale_scene(int64_t creater, Block_Buffer &buf) {
	Move_Scene *scene = DRAGON_VALE_SCENE_MANAGER->find_scene(creater);
	if(!scene) {
		return;
	}

	Dragon_Vale_Scene *dragon_vale_scene = 0;
	dragon_vale_scene = dynamic_cast<Dragon_Vale_Scene*>(scene);
	if(!dragon_vale_scene) {
		return;
	}

	dragon_vale_scene->broadcast_all(0, buf);
}

void Scene_Dragon_Vale_Manager::process_rob_match_fail(Block_Buffer &buf) {
	int64_t player_id = 0;
	buf.read_int64(player_id);
	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(player_id);
	if(!player) {
		return;
	}

	player->send_to_client(buf);
}

void Scene_Dragon_Vale_Manager::process_refresh_dragon_vale_monster(uint8_t type, int64_t role_id, int64_t id, uint8_t quality) {
	if(type != 1 && type != 2) {
		return;
	}

	if(quality != THIEF_QUALITY_WHITE && quality != THIEF_QUALITY_BLUE && quality != THIEF_QUALITY_ORANGE) {
		return;
	}

	Move_Scene *scene = DRAGON_VALE_SCENE_MANAGER->find_scene(role_id);
	if(!scene) {
		return;
	}

	Dragon_Vale_Scene *dragon_vale_scene = 0;
	dragon_vale_scene = dynamic_cast<Dragon_Vale_Scene*>(scene);
	if(!dragon_vale_scene) {
		return;
	}

	if(dragon_vale_scene->get_dragon_vale_scene_monster_num() >= 3) {
		return;
	}

	if(1 == type) {
		NPC_Record *npc_record = dragon_vale_scene->get_npc_record(id);
		if(!npc_record) {
			return;
		}

		if(npc_record->get_npc_num() >= 1) {
			return;
		}

		NPC_MANAGER->record_refresh(npc_record);

		RoleID_NPC_Map &alive_npc_map = npc_record->alive_npc_map;
		for(RoleID_NPC_Map::iterator it = alive_npc_map.begin(); it != alive_npc_map.end(); ++it) {
			Appear_Addition addition;
			addition.type = 200;
			addition.int_value1 = quality;
			it->second->set_addition(addition);
		}
	}
}

void Scene_Dragon_Vale_Manager::process_drive_thief(int64_t player_id, uint8_t type, int64_t role_id, int64_t monster_id, uint8_t quality) {
	if(2 != type) {
		return;
	}


	Monster *monster = NPC_MANAGER->find_monster(monster_id);
	if(!monster) {
		return;
	}
	NPC_Record *npc_record = monster->npc_record();
	if(!npc_record) {
		return;
	}
	npc_record->clear_point();

	uint32_t id = npc_record->config.id;
	MSG_20600016 msg;
	msg.player_id = player_id;
	msg.role_id = role_id;
	msg.id = id;
	msg.quality = quality;

	Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(player_id);
	if(!player) {
		return;
	}

	Block_Buffer block_buf;
	block_buf.make_message(INNER_CLEAR_DRAGON_VALE_MONSTER);
	msg.serialize(block_buf);
	block_buf.finish_message();
	player->send_to_logic(block_buf);
}

void Scene_Dragon_Vale_Manager::process_rob_match_fail_back_coin(int64_t uuid, int64_t role_id, uint32_t coin, uint8_t type) {
	MSG_20600020 msg;
	msg.uuid = uuid;
	msg.role_id = role_id;
	msg.coin = coin;
	msg.type = type;

	Block_Buffer block_buf;
	block_buf.make_message(INNER_ROB_MATCH_FAIL_BACK_COIN);
	msg.serialize(block_buf);
	block_buf.finish_message();
	SCENE_MONITOR->send_to_logic(uuid, block_buf);
}

void Scene_Dragon_Vale_Manager::process_dragon_vale_rob(int64_t to_uuid, int64_t to_role_id, std::string to_server_name, std::string to_role_name,
							int64_t uuid, int64_t role_id, std::string server_name, std::string role_name, uint32_t coin, uint8_t flag) {
	if(0 == flag) {
		Scene_Player *rob = SCENE_MONITOR->find_scene_player_by_role_id(to_role_id);
		if(!rob) {
			MSG_USER("DRAGON_VALE_ROB: rob is offline, flag=%d, to_uuid=%ld, to_role_id=%ld, to_server_name=%s, to_role_name=%s, "
					"uuid=%ld, role_id=%ld, server_name=%s, role_name=%s", flag, to_uuid, to_role_id, to_server_name.c_str(), to_role_name.c_str(),
					uuid, role_id, server_name.c_str(), role_name.c_str());
			return;
		}

		MSG_82300013 msg;
		msg.limit_time = 10;
		Block_Buffer block_buf;
		block_buf.make_message(msg.msg_id);
		msg.serialize(block_buf);
		block_buf.finish_message();
		rob->send_to_client(block_buf);
		SCENE_DRAGON_VALE_MANAGER->process_rob_fight_load_player_data(to_uuid, to_role_id, to_server_name, to_role_name, uuid, role_id);

		const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
		MSG_20600033 msg_20600033;
		msg_20600033.left_time = 10 + main_base_cfg.rob_down_time * Time_Value::ONE_MINUTE_IN_SECS;
		msg_20600033.role_id = role_id;
		Block_Buffer buf_20600033;
		buf_20600033.make_message(INNER_DRAONG_VALE_NO_ENTER);
		msg_20600033.serialize(buf_20600033);
		buf_20600033.finish_message();
		SCENE_MONITOR->send_to_logic(uuid, buf_20600033);
		return;
	}//end if 0

	if(1 == flag) {
		Scene_Player *target = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
		if(!target) {
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 2;
			Block_Buffer buf_20600031;
			buf_20600031.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf_20600031);
			buf_20600031.finish_message();
			SCENE_MONITOR->send_to_logic(uuid, buf_20600031);
			return;
		}

		if(target->is_safe_time() || target->is_in_battle() || target->dead_status()){
			MSG_20600031 msg;
			msg.to_uuid = to_uuid;
			msg.to_role_id = to_role_id;
			msg.to_server_name = to_server_name;
			msg.to_role_name = to_role_name;
			msg.uuid = uuid;
			msg.role_id = role_id;
			msg.server_name = server_name;
			msg.role_name = role_name;
			msg.coin = coin;
			msg.flag = 2;
			Block_Buffer buf_20600031;
			buf_20600031.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
			msg.serialize(buf_20600031);
			buf_20600031.finish_message();
			target->send_to_logic(buf_20600031);
			return;
		}

		MSG_20600031 msg;
		msg.to_uuid = to_uuid;
		msg.to_role_id = to_role_id;
		msg.to_server_name = to_server_name;
		msg.to_role_name = to_role_name;
		msg.uuid = uuid;
		msg.role_id = role_id;
		msg.server_name = server_name;
		msg.role_name = role_name;
		msg.coin = coin;
		msg.flag = 1;
		Block_Buffer buf_20600031;
		buf_20600031.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
		msg.serialize(buf_20600031);
		buf_20600031.finish_message();
		target->send_to_logic(buf_20600031);
		return;
	}//end if 1

	if(2 == flag) {
		Scene_Player *rob = SCENE_MONITOR->find_scene_player_by_role_id(to_role_id);
		if(!rob) {
			MSG_USER("DRAGON_VALE_ROB: rob is offline, flag=%d, to_uuid=%ld, to_role_id=%ld, to_server_name=%s, to_role_name=%s, "
					"uuid=%ld, role_id=%ld, server_name=%s, role_name=%s", flag, to_uuid, to_role_id, to_server_name.c_str(), to_role_name.c_str(),
					uuid, role_id, server_name.c_str(), role_name.c_str());
			return;
		}

		MSG_82300013 msg;
		msg.limit_time = 10;
		Block_Buffer block_buf;
		block_buf.make_message(msg.msg_id);
		msg.serialize(block_buf);
		block_buf.finish_message();
		rob->send_to_client(block_buf);

		rob->set_dragon_vale_rob(true);
		Time_Value now = Time_Value::gettimeofday();
		rob->set_begin_rob_count_down(now);
		return;
	}
}

void Scene_Dragon_Vale_Manager::porcess_dragon_vale_rob_offline(Scene_Player &player) {
	Dragon_Vale_Rob_Fight rob;
	rob.uuid = player.monitor_link().logic_muid.uuid;
	rob.role_id = player.role_id();
	Dragon_Vale_Rob_Fight_Map::iterator rob_it =  SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().find(rob);
	if(rob_it == SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().end()) {
		return;
	}

	Dragon_Vale_Rob_Fight_Map::iterator target_it =
			 	 SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().find(rob_it->second);
	if(target_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().end()) {
		if(target_it->second.uuid == rob.uuid && target_it->second.role_id == rob.role_id) {
			MSG_20600034 msg;
			msg.role_id = rob_it->second.role_id;
			msg.flag = 0;
			Block_Buffer block_buf;
			block_buf.make_message(msg.msg_id);
			msg.serialize(block_buf);
			block_buf.finish_message();
			SCENE_MONITOR->send_to_logic(rob_it->second.uuid, block_buf);

			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(rob_it->second);
		}
	}

	SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().erase(rob);
}
