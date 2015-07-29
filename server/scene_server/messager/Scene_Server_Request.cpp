/*
 * Scene_Server_Request.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#include "Scene_Server_Request.h"
#include "Scene_Client_Request.h"
#include "Scene_Monitor.h"
#include "Scene_Public.h"
#include "Pool_Manager.h"
#include "Scene_Player.h"
#include "Fighter_Detail.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "NPC/NPC_Manager.h"
#include "NPC/hero/Hero.h"
#include "Global_Scene_Manager.h"
#include "Err_Code.h"
#include "Battle_Scene.h"
#include "heroer/Logic_Heroer_Def.h"
#include "team/Scene_Team_Manager.h"
#include "war/Scene_War_Manager.h"
#include "Move_Scene.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "main_scene/Main_Scene_Manager.h"
#include "main_scene/Main_Scene.h"
#include "valentine_monster/Valentine_Monster_Manager.h"
#include "world_boss/Gang_Boss_Manager.h"
#include "world_boss/World_Boss_Manager.h"
#include "honor_arena/Honor_Arena_Manager.h"
#include "scene_outpost/Scene_Outpost_Manager.h"
#include "Config_Cache_Scene.h"
#include "campfire/Campfire_Data_Manager.h"
#include "campfire/Scene_Campfire.h"
#include "Msg_Active_Struct.h"
#include "Msg_Dungeon_Struct.h"
#include "Config_Cache_Arena.h"
#include "public/Public_Def.h"
#include "Msg_Role_Scene_Struct.h"
#include "team/Team_Arena_Controler.h"
#include "inner_arena/Inner_Arena_Manager.h"
#include "knight_trials/Scene_Knight_Trials_Manager.h"
#include "expedition_scene/Expedition_Global_Scene_Manager.h"
#include "answer/Answer_Scene_Manager.h"
#include "module/campfire_scene/Campfire_Scene_Manager.h"

Scene_Server_Request::Scene_Server_Request() {
	// TODO Auto-generated constructor stub
	monitor_ = SCENE_MONITOR;
}

Scene_Server_Request::~Scene_Server_Request() {
	// TODO Auto-generated destructor stub
}

int Scene_Server_Request::find_player_by_buf(Block_Buffer &buf, Scene_Player *&player) {
	role_id_t role_id = 0;
	if (buf.read_int64(role_id)) {
		MSG_USER_TRACE("read role id:%ld", role_id);
		return CALL_RETURN;
	}

	player = monitor()->find_scene_player_by_role_id(role_id);
	if (! player) {
		MSG_USER_TRACE("can not find scene player:%ld", role_id);
		return CALL_RETURN;
	}

	return 0;
}

int Scene_Server_Request::logic_login_ready(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	if (player) {
		MSG_TRACE_ABORT("player exist!");
		return -1;
	}

	Scene_Player *s_player = POOL_MANAGER->pop_scene_player();
	if (!s_player) {
		MSG_TRACE_ABORT("pop player error");
		return -1;
	}

	s_player->load_detail(buf);

	// 如果上线时场景还存在就进入场景,否则回去通知逻辑服回滚场景
	int des_scene = 0;
	int exist_scene = s_player->login_scene_exist(des_scene);

	if (exist_scene) {
		s_player->module_init();

		monitor()->bind_scene_player(*s_player);

		s_player->notice_enter_scene();
	}

	Block_Buffer inner_buf;
	inner_buf.make_message(LOGIC_SCENE_LOGIN_READY);
	inner_buf.write_int32(exist_scene);
	inner_buf.write_int32(des_scene);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), s_player->role_id());
	monitor()->send_to_server_by_cid(cid, inner_buf);

	MSG_DEBUG("login scene finish, role:%ld", role_id);

	return 0;
}

int Scene_Server_Request::logic_logout_ready(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	if (!player) {
		MSG_USER("can not find player, role:%ld", role_id);
		return 0;
	}

	Time_Value sign_out_flag;
	sign_out_flag.deserialize(buf);

	int out_reason = 0, need_return = 0;
	buf.read_int32(out_reason);
	buf.read_int32(need_return);

	Monitor_Unique_ID logic_muid = player->monitor_link().logic_muid;
	if (player) {
		MSG_DEBUG("scene logout success, role:%ld, out_reason:%d, notice_logic:%d, force:%d", role_id, out_reason, need_return, player->fighter_detail().force);

		player->sign_out(!need_return);
		monitor()->unbind_scene_player(role_id);
	} else {
		MSG_USER("scene logout error, role:%ld, out_reason:%d, notice_logic:%d", role_id, out_reason, need_return);
	}

	// 登陆错误时不需要回去
	if (need_return) {
		Block_Buffer inner_buf;
		inner_buf.make_message(LOGIC_SCENE_LOGOUT_READY);
		sign_out_flag.serialize(inner_buf);
		inner_buf.finish_message();
		inner_buf.make_head(inner_buf.get_msg_id(), role_id);
		monitor()->send_to_monitor(logic_muid, inner_buf);
	}

	return 0;
}


int Scene_Server_Request::process_20110001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	player->load_scene_detail(buf);

	return 0;
}

int Scene_Server_Request::process_20200050(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	// 战斗中，退出战斗
	if (player->is_in_battle()){
		player->exit_battle_scene(14);
		player->notify_gate_exit_battle(14);
	}

	Coord coord;
	Monitor_Unique_ID scene_muid;
	Create_Scene_Args scene_args;
	scene_muid.deserialize(buf);
	coord.deserialize(buf);
	scene_args.deserialize(buf);

	int src_scene_id = player->move_scene_id();

	{// tbay
		const Scene_Config* target_cfg = CONFIG_CACHE_SCENE->scene_config_cache(scene_muid.id);
		if(target_cfg){
			if(target_cfg->type == Tbay_Scene_Type){
				int64_t monster_role_id = scene_args.id;
				if(player->move_scene()){
					Mover* mover = player->move_scene()->find_mover_with_layer(0, monster_role_id);
					if(mover && mover->monster_self()){
						int target_id = 0;
						int target_time = 0;
						int target_type = 0;
						mover->monster_self()->get_ex_param(target_id, target_time, target_type);
						scene_args.type = target_type;
						scene_args.sub_sort = player->move_scene_id();
						scene_args.int_value = target_time;
					}
				}
			}
		}
	}

	player->set_rela_scene_id(scene_muid.id);
	player->gang_war_info().exit_ = true;

	player->exit_move_scene(false);

	player->set_scene_muid(scene_muid);
	player->set_create_scene_args(scene_args.type, scene_args.time, scene_args.id, scene_args.sort, scene_args.sub_sort, scene_args.int_value);
	player->set_grid_coord(coord.grid_coord().x, coord.grid_coord().y);
	player->set_pixel_coord(coord.pixel_coord().x, coord.pixel_coord().y);

	player->scene_init();

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_TRAN_SCENE_SAME_PRO);
	scene_muid.serialize(inner_buf);
	coord.serialize(inner_buf);
	inner_buf.finish_message();
	player->send_to_logic(inner_buf);

	player->notice_enter_scene();

	MSG_DEBUG("tran scene in same process , role:%ld, src:%d, des scene:%d", role_id, src_scene_id, player->move_scene_id());

	return 0;
}

int Scene_Server_Request::process_20200055(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	Monitor_Unique_ID logic_muid = player->monitor_link().logic_muid;
	if (player) {
		MSG_DEBUG("logout scene success, role:%ld, scene:%d", role_id, player->move_scene_id());

		if (player->move_scene_id() == 0) {
			MSG_TRACE_ABORT("scene tran error, role:%ld", role_id);
		}

		//在不同进程转场景时调用sign_out_ex,这个是非掉线的，要将offline_exit设为false
		player->sign_out(false, false);

		monitor()->unbind_scene_player(role_id);

	} else {
		MSG_USER("logout scene error, role:%ld, scene:%d", role_id, player->monitor_link().scene_muid.id);
	}

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ASK_SCENE_SIGN_OUT);
	inner_buf.copy(&buf);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), role_id);
	return monitor()->send_to_monitor(logic_muid, inner_buf);
}

int Scene_Server_Request::process_20200056(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	if (player) {
		MSG_TRACE_ABORT("player exist! role:%ld", role_id);
		return -1;
	}

	Scene_Player *s_player = POOL_MANAGER->pop_scene_player();
	if (!s_player) {
		MSG_TRACE_ABORT("pop player error");
		return -1;
	}

	s_player->buff_insert_handle_switch() = true;
	s_player->load_detail(buf);

	s_player->module_init();

	monitor()->bind_scene_player(*s_player);

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ASK_SCENE_SIGN_IN);
	s_player->monitor_link().scene_muid.serialize(inner_buf);
	s_player->grid_coord().serialize(inner_buf);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), s_player->role_id());
	monitor()->send_to_server_by_cid(cid, inner_buf);

	s_player->notice_enter_scene();

	MSG_DEBUG("transmit scene finish, role:%ld", role_id);

	return 0;
}

int Scene_Server_Request::process_20200100(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	int from = 0;
	Prop_Setter_Vec setter_vec;
	Prop_Setter setter;
	buf.read_int32(from);

	uint16_t vec_size = 0;
	if(buf.read_uint16(vec_size)) {
		return -1;
	}
	setter_vec.reserve(vec_size);

	for(uint16_t i = 0; i < vec_size; ++i) {
		setter.reset();
		if(setter.deserialize(buf)) {
			return -1;
		}

		switch (setter.prop_type) {
		case PT_EXP_CURRENT: {
			player->modify_experience(setter.basic);
			break;
		}
		default : {
			setter_vec.push_back(setter);
		}
		}
	}

	if (! setter_vec.empty()) {
		//player->fight_modify_fight_prop(setter_vec, true);
		player->modify_props_normal(setter_vec, false, from);
	}

	return 0;
}

int Scene_Server_Request::process_20300000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//JUDGE_PLAYER_NULL(player, role_id);

	MSG_20300000 msg;
	msg.deserialize(buf);
	Drops* drops = NPC_MANAGER->find_drops_npc(msg.drops_role_id);
	if(drops){
		drops->erase_drops_item(msg.item_index, role_id, player);
	}
	return 0;
}

int Scene_Server_Request::process_20100018(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100018 msg;
	msg.deserialize(buf);
	JUDGE_PLAYER_NULL(player, role_id);
	return player ->learn_skill(msg.skill_id, msg.skill_lv, msg.page_id);
}

int Scene_Server_Request::process_20100019(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100019 msg;
	msg.deserialize(buf);
	JUDGE_PLAYER_NULL(player, role_id);
	return player->erase_skill(msg.type, msg.page_id);
}

int Scene_Server_Request::process_20100020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100020 msg;
	msg.deserialize(buf);
	JUDGE_PLAYER_NULL(player, role_id);
	return player->change_skill_loc(msg.skill_id, msg.new_loc, msg.page_id);
}

int Scene_Server_Request::process_20100022(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100022 msg;
	msg.deserialize(buf);
	JUDGE_PLAYER_NULL(player, role_id);
	//return player->open_a_talent_page(msg.page_id);
	return player->open_a_talent(msg.page_id);
}

int Scene_Server_Request::process_20100023(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100023 msg;
	msg.deserialize(buf);
	JUDGE_PLAYER_NULL(player, role_id);
	return player->use_a_talent_page(msg.page_id);
}

int Scene_Server_Request::process_20100025(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100025 msg;
	msg.deserialize(buf);
	JUDGE_PLAYER_NULL(player, role_id);
	return player->skill_cost_and_save(msg.type, msg.arg1, msg.arg2);
}

int Scene_Server_Request::process_20300020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20300020 msg;
	msg.deserialize(buf);
	if(msg.plot_step == 0){
		player->begin_plot(msg.plot_id);
	}else{
		player->end_plot(msg.plot_id);
	}

	return 0;
}


int Scene_Server_Request::process_20300030(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	JUDGE_PLAYER_NULL(player, role_id);

	if (player->is_in_battle()) {
		MSG_DEBUG("creat arena fail player in battle");
		return ERROR_PLAYER_IN_BATTLE;
	}
	if (player->dead_status()) {
		MSG_DEBUG("creat arena fail player dead");
		return ERROR_PLAYER_DIE;
	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
	if(!scene){
		return -1;
	}
	scene->set_battle_type(Battle_Scene::BATTLE_TYPE_ARENA);
	int fight_map_id = CONFIG_CACHE_ARENA->get_arena_fight_map_id();
	scene->set_map_id(fight_map_id);
//	role_id_t match_role_id = 0;
//	buf.read_int64(match_role_id);
//	if(match_role_id == 0){
//
//	}else{
//
//	}
	int32_t inner_area = 0;
	int8_t career = 0;
	int8_t gender = 0;
	std::string role_name;
	buf.read_int32(inner_area);
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
		MSG_DEBUG("creat arena monster fail");
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


int Scene_Server_Request::process_20300032(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	return INNER_ARENA_AREA_MANAGER->inner_arena_sync_rank_info(buf);
}

int Scene_Server_Request::process_20300033(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	return INNER_ARENA_AREA_MANAGER->inner_arena_fetch_fight_data(buf);
}

int Scene_Server_Request::process_20300034(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	return INNER_ARENA_AREA_MANAGER->inner_arena_match_player(buf);
}

int Scene_Server_Request::process_20300035(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	return INNER_ARENA_AREA_MANAGER->inner_arena_fetch_rank_info(buf);
}

int Scene_Server_Request::process_20300038(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	return INNER_ARENA_AREA_MANAGER->inner_arena_battle_result(buf);
}

int Scene_Server_Request::process_20300040(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf){
	return INNER_ARENA_AREA_MANAGER->inner_arena_season_finish(buf);
}
int Scene_Server_Request::process_20400000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int8_t detail_type = 0;
	buf.read_int8(detail_type);
	if (detail_type != OUTER_HERO_ONLY && detail_type != FIGHT_HERO_ONLY && detail_type != OUTER_FIGHT_HERO) {
		return -1;
	}
	switch (detail_type) {
	case OUTER_HERO_ONLY:
		process_20400000_1(buf, player);
		break;
	case FIGHT_HERO_ONLY:
		process_20400000_2(buf, player);
		break;
	case OUTER_FIGHT_HERO:
		process_20400000_3(buf, player);
		break;
	default:
		break;
	}

	return 0;
}

int Scene_Server_Request::process_20400000_1(Block_Buffer &buf, Scene_Player *player) {
	Hero_Base_Detail detail;
	detail.reset();
	detail.deserialize(buf);

	Hero *hero = SCENE_PUBLIC->find_master_id_hero(detail.master_role_id);
	if (! hero) {
		hero = POOL_MANAGER->pop_hero();
		if (hero) {
			hero->reset();
			detail.hero_role_id = NPC_MANAGER->generate_hero_order();	//生成hero地图服id
			hero->init(detail.hero_role_id, player->move_scene(), player->grid_coord());
			NPC_MANAGER->hero_map()[detail.hero_role_id] = hero;
			SCENE_PUBLIC->bind_master_id_hero(detail.master_role_id, *hero);
		}
	}
	if (hero && !hero->validate_battle_scene()) {
		hero->refresh_hero_base_detail(detail);
	}

	return 0;
}

int Scene_Server_Request::process_20400000_2(Block_Buffer &buf, Scene_Player *player) {
	Hero_Base_Detail detail;
	detail.reset();
	detail.deserialize(buf);

	Hero *hero = SCENE_PUBLIC->find_fight_hero(detail.master_role_id);
	if (! hero) {
		hero = POOL_MANAGER->pop_hero();
		if (hero) {
			hero->reset();
			detail.hero_role_id = NPC_MANAGER->generate_hero_order();	//生成hero地图服id
			hero->init(detail.hero_role_id, player->move_scene(), player->grid_coord());
			NPC_MANAGER->hero_map()[detail.hero_role_id] = hero;
			SCENE_PUBLIC->bind_fight_hero(detail.master_role_id, *hero);
		}
	}
	if (hero && !hero->validate_battle_scene()) {
		hero->refresh_hero_base_detail(detail);
	}

	return 0;
}

int Scene_Server_Request::process_20400000_3(Block_Buffer &buf, Scene_Player *player) {
	Hero_Base_Detail detail;
	detail.reset();
	detail.deserialize(buf);

	// 跟随英雄
	{
		Hero *outer_hero = SCENE_PUBLIC->find_master_id_hero(detail.master_role_id);
		if (! outer_hero) {
			outer_hero = POOL_MANAGER->pop_hero();
			if (outer_hero) {
				outer_hero->reset();
				detail.hero_role_id = NPC_MANAGER->generate_hero_order();	//生成hero地图服id
				outer_hero->init(detail.hero_role_id, player->move_scene(), player->grid_coord());
				NPC_MANAGER->hero_map()[detail.hero_role_id] = outer_hero;
				SCENE_PUBLIC->bind_master_id_hero(detail.master_role_id, *outer_hero);
			}
		}
		if (outer_hero && !outer_hero->validate_battle_scene()) {
			outer_hero->refresh_hero_base_detail(detail);
		}
	}

	// 战斗英雄
	{
		Hero *fight_hero = SCENE_PUBLIC->find_fight_hero(detail.master_role_id);
		if (! fight_hero) {
			fight_hero = POOL_MANAGER->pop_hero();
			if (fight_hero) {
				fight_hero->reset();
				detail.hero_role_id = NPC_MANAGER->generate_hero_order();	//生成hero地图服id
				fight_hero->init(detail.hero_role_id, player->move_scene(), player->grid_coord());
				NPC_MANAGER->hero_map()[detail.hero_role_id] = fight_hero;
				SCENE_PUBLIC->bind_fight_hero(detail.master_role_id, *fight_hero);
			}
		}
		if (fight_hero && !fight_hero->validate_battle_scene()) {
			fight_hero->refresh_hero_base_detail(detail);
		}
	}

	return 0;
}

int Scene_Server_Request::process_20400001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	int8_t detail_type = 0;
	buf.read_int8(detail_type);
	if (detail_type != OUTER_HERO_ONLY && detail_type != FIGHT_HERO_ONLY && detail_type != OUTER_FIGHT_HERO) {
		return -1;
	}
	switch (detail_type) {
	case OUTER_HERO_ONLY:
		process_20400001_1(buf);
		break;
	case FIGHT_HERO_ONLY:
		process_20400001_2(buf);
		break;
	case OUTER_FIGHT_HERO:
		process_20400001_3(buf);
		break;
	default:
		break;
	}

	return 0;
}

int Scene_Server_Request::process_20400001_1(Block_Buffer &buf) {
	Fighter_Detail detail;
	detail.reset();
	detail.deserialize(buf);

	role_id_t master_role_id = 0;
	buf.read_int64(master_role_id);

	Hero *hero = SCENE_PUBLIC->find_master_id_hero(master_role_id);
	if (hero && !hero->validate_battle_scene()) {
		hero->refresh_hero_fighter_detail(detail);
	} else {
		MSG_USER("cannot find outer_hero, master_role_id = %ld", master_role_id);
		return -1;
	}

	return 0;
}

int Scene_Server_Request::process_20400001_2(Block_Buffer &buf) {
	Fighter_Detail detail;
	detail.reset();
	detail.deserialize(buf);

	role_id_t master_role_id = 0;
	buf.read_int64(master_role_id);

	Hero *hero = SCENE_PUBLIC->find_fight_hero(master_role_id);
	if (hero && !hero->validate_battle_scene()) {
		hero->refresh_hero_fighter_detail(detail);
		hero->load_status_info(buf);
	} else {
		int battle_id = hero ? hero->battle_scene_id() : -2;
		MSG_USER("cannot find fight_hero, master_role_id = %ld, b_id:%d", master_role_id, battle_id);
		return -1;
	}

	return 0;
}

int Scene_Server_Request::process_20400001_3(Block_Buffer &buf) {
	Fighter_Detail detail;
	detail.reset();
	detail.deserialize(buf);

	role_id_t master_role_id = 0;
	buf.read_int64(master_role_id);

	// 跟随英雄
	Hero *outer_hero = SCENE_PUBLIC->find_master_id_hero(master_role_id);
	if (outer_hero && !outer_hero->validate_battle_scene()) {
		outer_hero->refresh_hero_fighter_detail(detail);
	} else {
		MSG_USER("cannot find outer_hero, master_role_id = %ld", master_role_id);
		return -1;
	}

	// 战斗英雄
	Hero *fight_hero = SCENE_PUBLIC->find_fight_hero(master_role_id);
	if (fight_hero && !fight_hero->validate_battle_scene()) {
		fight_hero->refresh_hero_fighter_detail(detail);
	} else {
		MSG_USER("cannot find fight_hero, master_role_id = %ld", master_role_id);
		return -1;
	}

	return 0;
}

int Scene_Server_Request::process_20400002(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	return 0;
}

int Scene_Server_Request::process_20400003(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20400003 msg;
	msg.deserialize(buf);

	Hero *hero = SCENE_PUBLIC->find_master_id_hero(role_id);
	if (! hero) {
		//MSG_USER("hero fight, can't find hero, master_id = %ld, is_out = %d", msg.master_role_id, msg.is_out);
		// 登陆时会到这里
		return -1;
	}

	// 出战
	if (msg.is_out) {
		if (player->is_in_battle()) {
			//player->hero_battle_birth();
		} else if (player->move_scene()) {
			if (player->move_scene()->scene_type() == War_Scene_Type) {
				return 0;
			}
			const Coord &player_coord = player->mover_detail().grid_coord;
			if (0 != hero->birth(hero->hero_const_base_detail().hero_role_id, player->move_scene(), player_coord)) {
				hero->outer_hero_recycle();
				return -1;
			}
		}

	// 收回
	} else {
		hero->outer_hero_recycle();
	}

	return 0;
}

int Scene_Server_Request::process_20400007(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20400007 msg;
	msg.deserialize(buf);

	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id);
	if (! hero) {
		return -1;
	}

	// 出战
	if (msg.is_fight) {

	// 收回
	} else {
		if (! hero->validate_battle_scene()) {	// 战斗中，不能回收英雄
			//MSG_DEBUG("hero battle id:%d", hero->battle_scene_id());
			hero->fight_hero_recycle();
		}
	}

	return 0;
}

int Scene_Server_Request::process_20410001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	MSG_50200032 res_msg;
	res_msg.role_id = player->role_id();
	res_msg.open_type = 1;

	player->fighter_detail().property_list(res_msg.prop_value);
	Hero *hero = SCENE_PUBLIC->find_fight_hero(player->role_id());
	if (hero) {
		int hero_init_morale_type = hero->fighter_detail().init_morale.type;
		double hero_init_morale_val = hero->fighter_detail().init_morale.total();
		for (uint16_t i = 0; i < res_msg.prop_value.size(); ++i) {
			if (res_msg.prop_value[i].type == hero_init_morale_type) {
				res_msg.prop_value[i].value += hero_init_morale_val;
			}
		}
	}
	res_msg.prop_value.push_back(Property(PT_CAREER, player->career()));

	res_msg.role_name = player->role_name();/*人物姓名*/
	res_msg.gang_name = player->base_detail().gang_name;/*人物id*/
	res_msg.gender = player->base_detail().gender;/*性别（1男，2女）*/
	res_msg.gang_id = player->base_detail().gang_id;/*职业*/
	res_msg.headship = player->ganger_detail().headship;
	res_msg.avatar_vec = player->base_detail().avatar_vec;

    OBJ_SEND_TO_CLIENT(res_msg, (*player));
	return 0;
}
int Scene_Server_Request::process_20100200(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	role_id_t role_id_2 = 0;
	int8_t open_type = 0;
	buf.read_int64(role_id_2);
	buf.read_int8(open_type);

	MSG_50100032 res_msg;
	res_msg.role_id = role_id_2;
	res_msg.open_type = open_type;

	player->fighter_detail().property_list(res_msg.prop_value);
	Hero *hero = SCENE_PUBLIC->find_fight_hero(player->role_id());
	if (hero) {
		int hero_init_morale_type = hero->fighter_detail().init_morale.type;
		double hero_init_morale_val = hero->fighter_detail().init_morale.total();
		for (uint16_t i = 0; i < res_msg.prop_value.size(); ++i) {
			if (res_msg.prop_value[i].type == hero_init_morale_type) {
				res_msg.prop_value[i].value += hero_init_morale_val;
			}
		}
	}
	res_msg.prop_value.push_back(Property(PT_CAREER, player->career()));
	res_msg.prop_value.push_back(Property(PT_CHARM, player->base_detail().charm)); /*魅力值*/

	res_msg.role_name = player->role_name();/*人物姓名*/
	res_msg.gang_name = player->base_detail().gang_name;/*人物id*/
	res_msg.gender = player->base_detail().gender;/*性别（1男，2女）*/
	res_msg.gang_id = player->base_detail().gang_id;/*职业*/
	res_msg.headship = player->ganger_detail().headship;
	res_msg.avatar_vec = player->base_detail().avatar_vec;
	res_msg.vip = player->vip();

//    OBJ_SEND_TO_CLIENT(res_msg, (*player));

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_REQ_OPEN_PANEL);
	res_msg.serialize(inner_buf);
	inner_buf.finish_message();
	player->send_to_logic(inner_buf);
	return 0;
}

int Scene_Server_Request::process_20100028(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	std::string new_name = "";
	if (0 != buf.read_string(new_name) || new_name.empty()) {
		return -1;
	}
	player->set_role_name(new_name);
	return 0;
}

int Scene_Server_Request::process_20100029(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	uint8_t gender = 0;
	buf.read_uint8(gender);
	player->modify_gender(gender);
    MSG_81000060 msg;
    msg.role_id = player->role_id();
    Prop_Change change_info;
    change_info.prop_type = PT_GENDER;
    change_info.current_val = gender;
    msg.change_info.push_back(change_info);
    Block_Buffer active_buf;
    MAKE_MSG_BUF(msg, active_buf);
    // 广播给周围玩家
    player->scene_broad_around(active_buf,Mover::SCENE_MOVE);
	return 0;
}

int Scene_Server_Request::process_20100031(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	uint16_t energy = 0;
	buf.read_uint16(energy);
	player->set_energy(energy);
	return 0;
}

int Scene_Server_Request::process_20100032(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int skill_id = 0;
	buf.read_int32(skill_id);
	player->set_life_skill(skill_id);
	return 0;
}

int Scene_Server_Request::process_20100030(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100030 msg;
	msg.deserialize(buf);
	player->sync_phy_power_info(msg.phy_power_cur, msg.phy_power_recover_time,
			msg.phy_power_buy_times);
	return 0;
}

int Scene_Server_Request::process_20100303(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100303 msg;
	player->syn_teamer_info(buf);
	return 0;
}

int Scene_Server_Request::process_20100304(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	SCENE_TEAM_MANAGER->add_team(buf);
	return 0;
}

int Scene_Server_Request::process_20100305(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100305 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->use_drug_inner(msg.tp, msg.index, msg.val, msg.amount, buf);
	return 0;
}

int Scene_Server_Request::process_20100306(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	int8_t switch_ = 0;
	buf.read_int8(switch_);
	if (switch_ == 0) {
		TEAM_ARENA_CONTROLER->team_sports_test() = false;
		TEAM_ARENA_CONTROLER->close_sports_activity();
	} else {
		TEAM_ARENA_CONTROLER->team_sports_test() = true;
		TEAM_ARENA_CONTROLER->open_sports_activity();
	}
	return 0;
}

int Scene_Server_Request::process_20100400(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->restore_full_prop(buf);
	return 0;
}

int Scene_Server_Request::process_20100401(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->req_battle_status(buf);
	return 0;
}

int Scene_Server_Request::process_20100500(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->load_status_info(buf);
	return 0;
}

int Scene_Server_Request::process_20100501(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	player->inner_status_insert(buf);
	return 0;
}

int Scene_Server_Request::process_20100502(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int status_id = 0;
	buf.read_int32(status_id);
	if (0 == player->erase_status_type(status_id)) {
		MSG_81000071 msg_scene;
		msg_scene.type = SFT_SCENE;
		Status_Info_Msg msg_info;
		msg_info.status_id = status_id;
		msg_info.overlay = 0;
		msg_info.status_owner = player->role_id();
		msg_scene.buff_detail.push_back(msg_info);
		if (!msg_scene.buff_detail.empty()) {
			OBJ_SEND_TO_CLIENT(msg_scene, (*player));
		}
	}
	return 0;
}

int Scene_Server_Request::process_20100503(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int8_t type = 0;
	uint16_t len = 0;
	buf.read_int8(type);
	buf.read_uint16(len);
	if (len <= 0) {
		return 0;
	}
//	player
	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id);
	if (!hero) {
		return 0;
	}
	if (type == 1) { // add
		for (uint16_t i = 0; i < len; ++i) {
			hero->inner_status_insert(buf);
		}
	} else {
		Int_Hash_Set is;
		int status_id = 0;
		for (uint16_t i = 0; i < len; ++i) {
			buf.read_int32(status_id);
			is.insert(status_id);
		}
		hero->erase_status_in_set(is);
	}
	return 0;
}

int Scene_Server_Request::process_20300012(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20300012 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->gather_goods_result(msg.role_id, msg.result);
	return 0;
}

int Scene_Server_Request::process_20420000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	role_id_t sender_role_id = 0;
	int bless_buf_id = 0;
	buf.read_int64(sender_role_id);
	buf.read_int32(bless_buf_id);
	player->inner_send_bless_buf(sender_role_id, bless_buf_id);
	return 0;
}

int Scene_Server_Request::process_20170000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {

	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20170000 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	gang_id_t src_gang_id = player->gang_id();
	player->set_gang_id(msg.gang_id);
	player->set_gang_name(msg.gang_name);
	player->ganger_detail().headship = msg.headship;
	player->listen_gang_info_sync();
	WORLD_BOSS_MANAGER->listen_gang_info_sync(src_gang_id, player);
	GANG_BOSS_MANAGER->listen_gang_info_sync(src_gang_id, player);
	if (0 == msg.gang_id && Expedition_Type == get_scene_type(player->move_scene_id())) {
		int des_scene = 0;
		player->inner_transmit(des_scene, 0, 0);
	}
	return 0;
}

int Scene_Server_Request::process_20170001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	SCENE_GANG_GLOBAL_MANAGER->gang_war_load_source_gangs_from_logic(buf);
	return 0;
}

int Scene_Server_Request::process_20170002(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	player->gang_load_detail(buf);
	return 0;
}

int Scene_Server_Request::process_20170004(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	player->ask_gang_war_rank_info(buf);
	return 0;
}
int Scene_Server_Request::process_20170006(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	SCENE_GANG_GLOBAL_MANAGER->set_activity_state(buf);
	return 0;
}

int Scene_Server_Request::process_20100309(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	TEAM_ARENA_CONTROLER->update_role_score(buf);
	return 0;
}

int Scene_Server_Request::process_20100310(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100310 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_GET_TEAM_SCORE);
	TEAM_ARENA_CONTROLER->get_role_score_list(msg.role_id, msg.level, inner_buf);
	inner_buf.finish_message();
	inner_buf.make_head(inner_buf.get_msg_id(), msg.role_id);

	monitor()->send_to_server_by_cid(cid, inner_buf);

	return 0;
}

int Scene_Server_Request::process_20300100(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	Monitor_Unique_ID logic_muid;
	role_id_t player_role_id = 0;
	int level = 0;
	int force = 0;
	role_id_t gang_id = 0;
	int8_t remain_income_num = 0;
	int16_t income_rate = 0;
	int total_merit = 0;
	int remain_time = 0;
	int act_time = 0;
	std::string server_name;
	logic_muid.deserialize(buf);
	buf.read_int64(player_role_id);
	buf.read_int32(level);
	buf.read_int32(force);
	buf.read_int64(gang_id);
	buf.read_int8(remain_income_num);
	buf.read_int16(income_rate);
	buf.read_int32(total_merit);
	buf.read_int32(remain_time);
	buf.read_int32(act_time);
	buf.read_string(server_name);
	SCENE_WAR_MANAGER->req_enter_war_scene(logic_muid, player_role_id, level,
			force, gang_id, remain_income_num, income_rate, total_merit, remain_time, act_time, server_name);
	return 0;
}

int Scene_Server_Request::process_20300101(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 3:{// 单人副本付费复活消耗钻石返回
			if(player && player->move_scene() && player->move_scene()->scene_type() == FuBen_Scene_Type){
				int8_t revive_result = 0;
				buf.read_int8(revive_result);
				if(revive_result == 1){
					int param1 = 0;
					int param2 = 0;
					int param3 = 0;
					player->move_scene()->get_scene_param(param1, param2, param3);
					param1 = 0;
					player->move_scene()->set_scene_param(param1, param2, param3, true);
					player->reset_revive_info();
					{
						MSG_81000026 msg;
						msg.role_id = player->role_id();
						msg.state = 0;
						OBJ_SEND_TO_CLIENT(msg, (*player));
					}
					{
						MSG_50200215 msg;
						msg.result = 1;
						OBJ_SEND_TO_CLIENT(msg, (*player));
					}
				}else{
					MSG_50200215 msg;
					msg.result = 0;
					OBJ_SEND_TO_CLIENT(msg, (*player));
				}
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Scene_Server_Request::process_20100403(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int8_t type = 0;
	buf.read_int8(type);
	uint16_t vec_size = 0;
	if (buf.read_uint16(vec_size)) {
		return -1;
	}

	Int_Double_Map force_map;
	player->get_all_pre_force(force_map);
	int old_val = player->fighter_detail().force;

	Skill_Info skill_info;
	for (uint16_t i = 0; i < vec_size; ++i) {
		skill_info.reset();
		skill_info.deserialize(buf);
		if (type == NPT_ADD) {
			player->add_skill_extra(skill_info.skill_id, skill_info.skill_lv);
		} else if (type == NPT_REMOVE) {
			player->remove_skill_extra(skill_info.skill_id, skill_info.skill_lv);
		}
	}

	if (vec_size > 0) {
		player->sync_force_to_client();
	}

	int new_val = player->fighter_detail().force;
	player->print_all_part_force(100, new_val-old_val, force_map);

	return 0;
}

int Scene_Server_Request::process_20100026(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int32_t skill_point = 0;
	buf.read_int32(skill_point);
	if (skill_point > 0) {
		player->add_skill_pointer(skill_point);
	}
	return 0;
}

int Scene_Server_Request::process_20100040(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20100040 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return player->sync_mount_ride(msg.ride, msg.model_id, msg.speed);
}

int Scene_Server_Request::process_20100600(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	int32_t title_id = 0;
	buf.read_int32(title_id);
	player->set_title(title_id);
	return 0;
}

int Scene_Server_Request::process_20600000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600000 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_rob_dragon_vale_match(msg.uuid, msg.server_name, msg.role_id, msg.coin, msg.last_coin);
	return 0;
}

int Scene_Server_Request::process_20600002(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600002 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_rob_match(msg);
	return 0;
}

int Scene_Server_Request::process_20600004(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	SCENE_DRAGON_VALE_MANAGER->process_rob_fight(buf);
	return 0;
}

int Scene_Server_Request::process_20600007(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600007 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_rob_enter_dragon_vale(msg.to_uuid, msg.to_role_id, msg.uuid, msg.role_id, msg.limit_time);
	return 0;
}

int Scene_Server_Request::process_20600008(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600008 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_rob_enter_dragon_vale(msg);
	return 0;
}

int Scene_Server_Request::process_20600009(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600009 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_gain_rob_production(msg);
	return 0;
}

int Scene_Server_Request::process_20600010(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600010 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_gain_rob_production_res(msg);
	return 0;
}

int Scene_Server_Request::process_20600012(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	int64_t creater = 0;
	buf.read_int64(creater);
	SCENE_DRAGON_VALE_MANAGER->process_broadcast_dragon_vale_scene(creater, buf);
	return 0;
}

int Scene_Server_Request::process_20600014(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail(buf);
	return 0;
}

int Scene_Server_Request::process_20600015(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600015 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_refresh_dragon_vale_monster(msg.type, msg.role_id, msg.id, msg.quality);
	return 0;
}

int Scene_Server_Request::process_20600020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600020 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(msg.uuid, msg.role_id, msg.coin, msg.type);
	return 0;
}

int Scene_Server_Request::process_20600031(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20600031 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	SCENE_DRAGON_VALE_MANAGER->process_dragon_vale_rob(msg.to_uuid, msg.to_role_id, msg.to_server_name, msg.to_role_name,
													  msg.uuid, msg.role_id, msg.server_name, msg.role_name, msg.coin, msg.flag);
	return 0;
}


int Scene_Server_Request::process_20300110(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300110 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
//	CAMPFIRE_DATA_MANAGER->cmd_campfire_player_info_sync(buf);
	return 0;
}

int Scene_Server_Request::process_20300111(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300110 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return CAMPFIRE_DATA_MANAGER->cmd_campfire_activate(buf);
}

int Scene_Server_Request::process_20300112(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300110 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	//CAMPFIRE_DATA_MANAGER->
	return 0;
}

int Scene_Server_Request::process_20300114(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20300114 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	return player->get_campfire_award_result(msg);
}

int Scene_Server_Request::process_20300116(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	MSG_20300116 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	JUDGE_PLAYER_NULL(player, role_id);
	return player->load_campfire_player_info(msg);
}

int Scene_Server_Request::process_20300300(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300300 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	int act_icon_id = 0;
	buf.read_int32(act_icon_id);
	switch(act_icon_id){
		case 16:{
			WORLD_BOSS_MANAGER->inner_sync_info(player, buf);
			break;
		}
		case 19:{
			GANG_BOSS_MANAGER->inner_sync_info(player, buf);
			break;
		}
		case 22:{
			HONOR_ARENA_MANAGER->inner_sync_info(player, buf);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Scene_Server_Request::process_20300301(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300301 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	int act_icon_id = 0;
	int64_t gang_id = 0;
	buf.read_int32(act_icon_id);
	buf.read_int64(gang_id);
	switch(act_icon_id){
		case 16:{
			WORLD_BOSS_MANAGER->inner_player_enter(gang_id, buf);
			break;
		}
		case 19:{
			GANG_BOSS_MANAGER->inner_player_enter(gang_id, buf);
			break;
		}
		case 22:{
			HONOR_ARENA_MANAGER->inner_player_enter(gang_id, buf);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Scene_Server_Request::process_20300302(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300302 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	int act_icon_id = 0;
	role_id_t gang_id = 0;
	buf.read_int32(act_icon_id);
	buf.read_int64(gang_id);
	switch(act_icon_id){
		case 16:{
			WORLD_BOSS_MANAGER->inner_act_start(gang_id, buf);
			break;
		}
		case 19:{
			GANG_BOSS_MANAGER->inner_act_start(gang_id, buf);
			break;
		}
		case 20:{
			VALENTINE_MONSTER_MANAGER->inner_act_start(gang_id, buf);
			break;
		}
		case 22:{
			HONOR_ARENA_MANAGER->inner_act_start(gang_id, buf);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Scene_Server_Request::process_20300303(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	//MSG_20300303 msg;
	//JUDGE_ERROR_RETURN(msg.deserialize(buf));
	int act_icon_id = 0;
	role_id_t gang_id = 0;
	buf.read_int32(act_icon_id);
	buf.read_int64(gang_id);
	switch(act_icon_id){
		case 16:{
			WORLD_BOSS_MANAGER->inner_act_end(gang_id, buf);
			break;
		}
		case 19:{
			GANG_BOSS_MANAGER->inner_act_end(gang_id, buf);
			break;
		}
		case 20:{
			VALENTINE_MONSTER_MANAGER->inner_act_end(gang_id, buf);
			break;
		}
		case 22:{
			HONOR_ARENA_MANAGER->inner_act_end(gang_id, buf);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Scene_Server_Request::process_20300400(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	return SCENE_OUTPOST_MANAGER->inner_sync_info(player, buf);
}

int Scene_Server_Request::process_20200020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20200020 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->set_vip_info_change(msg.vip, msg.vip_over_time);
	return 0;
}

int Scene_Server_Request::process_20200023(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20200023 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->set_head_id(msg.head_id);
	return 0;
}

int Scene_Server_Request::process_20200022(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20200022 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->add_charm(msg.num);
	return 0;
}

int Scene_Server_Request::process_20300200(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	Monitor_Unique_ID muid;
	Coord coord;
	int shop_type = 0;
	muid.deserialize(buf);
	coord.deserialize(buf);
	buf.read_int32(shop_type);
	Create_Scene_Args cs_args;
	Move_Scene* scene = MAIN_SCENE_MANAGER->find_scene_simple(muid, 0, cs_args);
	if(scene){
		Main_Scene* main_scene = dynamic_cast<Main_Scene*>(scene);
		if(main_scene){
			if(shop_type == 0){
				main_scene->close_secret_shop();
			}else{
				main_scene->open_secret_shop(coord, shop_type);
			}
		}
	}
	return 0;
}

int Scene_Server_Request::process_20100050(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &inner_buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	uint16_t change_info_size = 0;
	inner_buf.read_uint16(change_info_size);
	if (change_info_size == 0) {
		return -1;
	}

	MSG_81000066 active_msg;
	active_msg.change_info.reserve(change_info_size);
	String_Info_Change string_info_change;
	for (uint16_t i = 0; i < change_info_size; ++i) {
		string_info_change.reset();
		if (string_info_change.deserialize(inner_buf)) {
			return -1;
		}
		active_msg.change_info.push_back(string_info_change);
	}
	inner_buf.read_int64(active_msg.role_id);

	Block_Buffer active_buf;
	MAKE_MSG_BUF(active_msg, active_buf);
	player->scene_broad_around(active_buf,Mover::SCENE_MOVE);

	return 0;
}

int Scene_Server_Request::process_20100051(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &inner_buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_81000067 active_msg;
	inner_buf.read_int64(active_msg.role_id);
	inner_buf.read_int8(active_msg.int_type);
	switch (active_msg.int_type) {
	case 32: {
		uint16_t change_info_size = 0;
		inner_buf.read_uint16(change_info_size);
		if (change_info_size == 0) {
			return -1;
		}
		Int32_Info_Change int32_info_change;
		bool is_hero_awake = false;
		for (uint16_t i = 0; i < change_info_size; ++i) {
			int32_info_change.reset();
			if (int32_info_change.deserialize(inner_buf)) {
				return -1;
			}
			if (int32_info_change.type == IICT_HERO_AWAKE_LVL) {
				is_hero_awake = true;
			}
			active_msg.vec_32_change_info.push_back(int32_info_change);
		}

		// 英雄觉醒等级更新
		if (is_hero_awake) {
			Hero *hero = SCENE_PUBLIC->find_mover_hero(player->role_id());
			if (hero) {
				active_msg.role_id = hero->role_id();
				Block_Buffer active_buf;
				MAKE_MSG_BUF(active_msg, active_buf);
				hero->scene_broad_around(active_buf,Mover::SCENE_MOVE);
			}
		}

		break;
	}
	case 8:
	case 16:
	case 64:
		break;
	default:
		break;
	}

	return 0;
}

int Scene_Server_Request::process_20010061(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);

	player->sync_fighter_detail_to_logic();

	return 0;
}

int Scene_Server_Request::process_20100060(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20100060 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	player->set_avatar(msg.avatar_vec);
	return 0;
}

int Scene_Server_Request::process_20610000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	return SCENE_KNIGHT_TRIALS_MANAGER->scene_knight_trials_match_player(buf);
}

int Scene_Server_Request::process_20610001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	MSG_20610001 msg;
	JUDGE_ERROR_RETURN(msg.deserialize(buf));
	return SCENE_KNIGHT_TRIALS_MANAGER->scene_knight_trials_enter_fighter(player, buf);
}

int Scene_Server_Request::process_20610010(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	return EXPEDITION_GLOBAL_SCENE_MANAGER->data_channle(buf);
}

int Scene_Server_Request::process_20610012(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	JUDGE_PLAYER_NULL(player, role_id);
	return player->expedition_data_channel(buf);
}

int Scene_Server_Request::process_20610020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	return ANSWER_SCENE_MANAGER->data_channle(buf);
}

int Scene_Server_Request::process_20020001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	return SCENE_PUBLIC->sync_dispath(buf);
}

int Scene_Server_Request::process_20999999(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf) {
	return SCENE_PUBLIC->inner_check_info_on_center_server(buf);
}

