/*
 * Player_Test.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: ti
 */

#include "Scene_Player.h"
#include "Battle_Scene.h"
#include "Scene_Module_Head.h"
#include "Global_Scene_Manager.h"
#include "Err_Code.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_NPC_Struct.h"
#include "Move_Scene.h"
#include "status/Status_Struct.h"
#include "NPC/NPC_Manager.h"
#include "team/Team_Arena_Controler.h"
#include "war_scene/War_Scene.h"
#include "Config_Cache.h"
#include "campfire_scene/Campfire_Scene.h"
#include "expedition_scene/Expedition_Global_Scene_Manager.h"
#include "expedition_scene/Expedition_Scene.h"

struct Chat_Content;

int Scene_Player::echo_test_result(std::string result, role_id_t role_id, double value) {
	MSG_83000000 msg;
	msg.chat_type = 100;
	std::stringstream stream;
	stream <<  result;

	stream <<  role_id;
	if (value >= 0.0)
		stream <<  value;

	Chat_Content content;
	content.text = stream.str();
	msg.content.push_back(content);

	Block_Buffer buf;
	msg.serialize(buf);
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Scene_Player::test_command(std::string cmd_string) {
	std::vector<std::string> cmd_tok;

	std::istringstream cmd_stream(cmd_string);
	std::copy(std::istream_iterator<std::string>(cmd_stream),
			std::istream_iterator<std::string>(), std::back_inserter(cmd_tok));

	std::cout << "cmd_string = [" << (cmd_string) << "]" << std::endl;
	if (cmd_tok.empty())
		return 0;

	// 0:关闭 1:全开 2:开放部分命令
	int command_mode = CONFIG_CACHE->server_maintainer_cache().command_mode;
	if (command_mode == 1) {
	} else if (command_mode == 2) {
		if (!CONFIG_CACHE->server_maintainer_cache().command.count(cmd_tok[0]))
			return 0;
	} else {
		return 0;
	}


	if (cmd_tok[0] == "test") {
		int i = 0;
		i = atoi(cmd_tok[1].c_str());
		MSG_DEBUG("test, i%d", i);
	}

	if(cmd_tok[0] == "s_server_status") {
		monitor()->message_unit()->server_status();
		return 0;
	}

	if (cmd_tok[0] == "abort") {
		LOG_TRACE_ABORT("");
	}

	if (cmd_tok[0] == "test1") {
		int loop = 0;
		loop = atoi(cmd_tok[1].c_str());
		role_id_t role = 0;
		{
			PERF_MON("PERF_MON1");
			for (int i = 0; i < loop; i++) {
				role = role_id();
			}
		}
		{
			PERF_MON("PERF_MON2");
			for (int i = 0; i < loop; i++) {
				role = base_detail_->role_id;
			}
		}
		{
			Mover *mover = this;
			PERF_MON("PERF_MON3");
			for (int i = 0; i < loop; i++) {
				role = mover->role_id();
			}
		}
	}

	if (cmd_tok[0] == "role_info") {
		echo_test_result("role id:", role_id());
		echo_test_result("level:", level());
		echo_test_result("move scene id:", move_scene_id());
		echo_test_result("move scene order:", move_scene_order());
		echo_test_result("blood current:", 0, this->blood_current());
		echo_test_result("blood max:", 0, this->blood_max());
		echo_test_result("coord x:", 0, this->grid_coord().x);
		echo_test_result("coord y:", 0, this->grid_coord().y);
		if (this->battle_scene()) {
			echo_test_result("b_id:", 0, this->battle_scene()->battle_id());
			echo_test_result("b_status:", 0, this->battle_scene()->battle_status());
			echo_test_result("b_over_step:", 0, this->battle_scene()->over_step());
			echo_test_result("b_type:", 0, this->battle_scene()->get_battle_type());
			echo_test_result("b_last_tick:", 0, this->battle_scene()->last_tick().sec());
		}
	}

	if (cmd_tok[0] == "hp") {
		if (cmd_tok.size() == 1) {
			this->restore_hp(blood_max(), true);
		}
		if (cmd_tok.size() == 2) {
			double hp = abs(atoi(cmd_tok[1].c_str()));
			this->fight_basic_prop_modify(PT_BLOOD_CURRENT, AT_BASIC, O_SET, hp);
		}
	}

	if (cmd_tok[0] == "level") {
		if (cmd_tok.size() == 2) {
			int level = abs(atoi(cmd_tok[1].c_str()));
			if (level <= player_self()->level()) {
				return echo_test_result("level must be > level now.");
			}

			if (level <= MAX_LEVEL)
				this->fight_normal_prop_modify(PT_LEVEL, O_SET, level);
			else
				echo_test_result("max level:", MAX_LEVEL);
		}
	}
	if (cmd_tok[0] == "total") {
		int size = this->move_scene()->first_scene_layer().mover_map_.size();

		if (cmd_tok.size() == 1) {
			echo_test_result("scene total:", SCENE_MONITOR->player_total());
			echo_test_result("scene mover:", size);
		}

		if (cmd_tok.size() == 2) {
			MSG_83000000 msg;
			msg.chat_type = 100;
			std::stringstream stream;
			stream <<  "broad scene total: ";
			stream <<  size;

			Chat_Content content;
			content.text = stream.str();
			msg.content.push_back(content);

			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);

			this->scene_broad_around(buf);
		}
	}

	if (cmd_tok[0] == "appear_test") {
		Block_Buffer buf;
		appear_buff().set_ext_flag(true);
		this->move_appear_info(buf);
	}

	if (cmd_tok[0] == "exp") {
		if (cmd_tok.size() == 2) {
			int exp = abs(atoi(cmd_tok[1].c_str()));
			this->modify_experience(exp);
		}
	}

	if (cmd_tok[0] == "team") {
		if (cmd_tok.size() == 2) {
			int i = atoi(cmd_tok[1].c_str());
			switch(i) {
			case 2: {
				echo_test_result("scene team_id:", player_self()->get_team_id());
				echo_test_result("team_pos:pos ", player_self()->team_position().pos);
				echo_test_result("team_pos:idx", player_self()->team_position().idx);
				break;
			}
			default:
				break;
			}
		}
	}
	if (cmd_tok[0] == "pack") {
		if (cmd_tok.size() == 2) {
			int count = abs(atoi(cmd_tok[1].c_str()));
			for (int i = 0; i < count; ++i) {
				echo_test_result("scene total:", i);
			}
		}
	}

	if (cmd_tok[0] == "prop") {
		if (cmd_tok.size() == 3) {
			Property_Type prop_type = Property_Type(abs(atoi(cmd_tok[1].c_str())));
			double prop_val = atoi(cmd_tok[2].c_str());
			if (prop_val > 0.0)
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_ADD, prop_val);
			else
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_SUB, -prop_val);
		}
	}
	if (cmd_tok[0] == "trace_abort") {
		LOG_TRACE_ABORT();
	}
	if (cmd_tok[0] == "speed") {
		double speed = 250;
		if (cmd_tok.size() > 1) {
			speed = atoi(cmd_tok[1].c_str());
			speed = (speed > 1500) ? 1500 : speed;
		}
		if (speed > 0) {
			this->fight_basic_prop_modify(Property_Type::PT_SPEED, AT_BASIC, O_SET, speed);
		}
	}

	if(cmd_tok[0] == "sleep") {
		sleep(300);
		return 0;
	}

	if (cmd_tok[0] == "bt") {
		Int_Int_Map pp;
		pp.insert(std::make_pair(504605, 100000));
		pp.insert(std::make_pair(504905, 100000));
		pp.insert(std::make_pair(507405, 100));
		for (Int_Int_Map::const_iterator it = pp.begin(); it != pp.end(); ++it) {
			Property_Type prop_type = Property_Type(it->first);
			double prop_val = it->second;
			if (prop_val > 0.0)
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_ADD, prop_val);
			else
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_SUB, -prop_val);
		}
	}
	if (cmd_tok[0] == "ag") {
		Int_Int_Map pp;
		pp.insert(std::make_pair(507405, 100));
		for (Int_Int_Map::const_iterator it = pp.begin(); it != pp.end(); ++it) {
			Property_Type prop_type = Property_Type(it->first);
			double prop_val = it->second;
			if (prop_val > 0.0)
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_ADD, prop_val);
			else
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_SUB, -prop_val);
		}
	}
	if (cmd_tok[0] == "sp") {
		Int_Int_Map pp;
		pp.insert(std::make_pair(Property_Type::PT_SPEED, 700));
		for (Int_Int_Map::const_iterator it = pp.begin(); it != pp.end(); ++it) {
			Property_Type prop_type = Property_Type(it->first);
			double prop_val = it->second;
			if (prop_val > 0.0)
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_ADD, prop_val);
			else
				this->fight_basic_prop_modify(prop_type, AT_BASIC, O_SUB, -prop_val);
		}
	}

	if (cmd_tok[0] == "battle_enter_test") {
		Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(this->move_scene());

		if(set_battle_position(0, Position_Key::LINE_TWO_FRONT, scene)== 0){
			enter_battle_scene(scene);
		}
	}

	if (cmd_tok[0] == "battle_exit_test") {
		//exit_battle_scene();
	}

	if (cmd_tok[0] == "ha_match") {
		this->req_honor_arena_match();
		return 0;
	}
	if (cmd_tok[0] == "ha_receive") {
		int type = 1000;
		if (cmd_tok.size() > 1) {
			type = atoi(cmd_tok[1].c_str());
		}
		this->req_honor_arena_receive(type);
		return 0;
	}

	if(cmd_tok[0] == "81000005")
	{
		Block_Buffer buf;
		MSG_81000005 msg;
		msg.npc_info.coord = Coord(17*32,21*32);
		msg.npc_info.hp_curr = 100;
		msg.npc_info.hp_max = 100;
		msg.npc_info.npc_type = 1613836291;
		msg.npc_info.role_id = 1110;
		msg.npc_info.role_name = "monster1";
		msg.npc_info.speed = 180;
		THIS_SEND_TO_CLIENT(msg);
	}
	if(cmd_tok[0] == "m_b_2")
	{
		Block_Buffer buf;
		MSG_81000008 msg;
		msg.fighter_info.role_id = 315;
		msg.fighter_info.role_name = "1_5";
		msg.fighter_info.position.val_1 = 1;
		msg.fighter_info.position.val_2 = 5;
		int model;
		model = 1613836291;
		msg.fighter_info.avatar.push_back(model);
		THIS_SEND_TO_CLIENT(msg);
	}
	if(cmd_tok[0] == "m_b_1")
	{
		Block_Buffer buf;
		MSG_81000008 msg;
		msg.fighter_info.role_id = 304;
		msg.fighter_info.role_name = "0_4";
		msg.fighter_info.position.val_1 = 0;
		msg.fighter_info.position.val_2 = 4;
		int model;
		model = 1613836291;
		msg.fighter_info.avatar.push_back(model);
		THIS_SEND_TO_CLIENT(msg);
	}
	if(cmd_tok[0] == "m_b_3")
	{
		Block_Buffer buf;
		MSG_81000008 msg;
		msg.fighter_info.role_id = 3304;
		msg.fighter_info.role_name = "1_9";
		msg.fighter_info.position.val_1 = 1;
		msg.fighter_info.position.val_2 = 9;
		int model;
		model = 1613836291;
		msg.fighter_info.avatar.push_back(model);
		THIS_SEND_TO_CLIENT(msg);
	}
	if(cmd_tok[0] == "81000020")
	{
		Block_Buffer buf;
		MSG_81000020 msg;
		msg.role_id = 1110;
		msg.coord.push_back(Coord(17*32,24*32));
		THIS_SEND_TO_CLIENT(msg);
	}

	if(cmd_tok[0] == "s_pack_test") {
		if(cmd_tok.size() == 2){
			int pack_len = atoi(cmd_tok[1].c_str());
			Block_Buffer buf;
			buf.make_message(81000020);
			for (int i = 0; i < pack_len; ++i) {
				buf.write_int32(i);
			}
			buf.finish_message();
			this->send_to_client(buf);
		}
	}

	if(cmd_tok[0] == "drops_item"){
		Block_Buffer buf;
		MSG_81000005 msg;
		msg.npc_info.role_id = 1000;
		msg.npc_info.npc_type = 1619066881;
		msg.npc_info.role_name = "item";
		Coord coord = this->mover_detail().pixel_coord;
		coord.x += 10;
		coord.y += 10;
		msg.npc_info.coord = coord;
		msg.npc_info.hp_curr = 0;
		msg.npc_info.hp_max = 0;
		msg.npc_info.speed = 0;
		int ava;
		ava = 10102401;
		msg.npc_info.avatar.push_back(ava);
		THIS_SEND_TO_CLIENT(msg);
	}
	if(cmd_tok[0] == "drops_box"){
		Block_Buffer buf;
		MSG_81000005 msg;
		msg.npc_info.role_id = 1001;
		msg.npc_info.npc_type = 1619066881;
		msg.npc_info.role_name = "item";
		Coord coord = this->mover_detail().pixel_coord;
		coord.x += 10;
		coord.y += 10;
		msg.npc_info.coord = coord;
		msg.npc_info.hp_curr = 0;
		msg.npc_info.hp_max = 0;
		msg.npc_info.speed = 0;
		int ava;
		ava = 1619066881;
		msg.npc_info.avatar.push_back(ava);
		THIS_SEND_TO_CLIENT(msg);
	}
	if(cmd_tok[0] == "pick_drops1"){
		MSG_10210001 msg;
		msg.role_id = 5000001;
		get_drops_goods(msg);
	}
	if(cmd_tok[0] == "pick_drops2"){
		MSG_10210001 msg;
		msg.role_id = 5000002;
		get_drops_goods(msg);
	}
	if (cmd_tok[0] == "skill_point") {
		int pointer = 10;
		if (cmd_tok.size() > 1) {
			pointer = atoi(cmd_tok[1].c_str());
		}
		add_skill_pointer(pointer);
	}
	if(cmd_tok[0] == "refresh_monster"){
		if(cmd_tok.size() == 2){
			int npc_type_id = atoi(cmd_tok[1].c_str());
			if(this->move_scene() && this->battle_scene() == NULL){
				Coord birth_coord = this->mover_detail().grid_coord;
				birth_coord.x += 3;
				bool validate = move_scene()->validate_grid_coord(birth_coord.x, birth_coord.y);
				if(!validate){
					birth_coord.x -= 3;
				}
				this->move_scene()->refresh_monster_for_debug(birth_coord, npc_type_id, player_self()->role_id());
			}
		}
	}
	if(cmd_tok[0] == "battle_with_monster_type"){
		if(cmd_tok.size() == 2){
			int npc_type_id = atoi(cmd_tok[1].c_str());
			if(this->move_scene() && this->battle_scene() == NULL){
				NPC_MANAGER->create_battle_with_monster(this->player_self(), npc_type_id, 0);
			}
		}
		return 0;
	}

	if(cmd_tok[0] == "battle_with_monster_group"){
		if(cmd_tok.size() == 2){
			int npc_type_id = atoi(cmd_tok[1].c_str());
			if(this->move_scene() && this->battle_scene() == NULL){
				NPC_MANAGER->create_battle_with_monster(this->player_self(), npc_type_id, 1);
			}
		}
		return 0;
	}
	if (cmd_tok[0] == "blood_pool") {
		double max = blood_pool_max();
		Prop_Setter_Vec pool_setter;
		Prop_Setter setter;
		setter.prop_type = PT_BLOOD_POOL_CURRENT;
		setter.operate = O_SET;
		setter.addi_type = AT_BASIC;
		setter.basic = max;
		pool_setter.push_back(setter);
		// fight_modify_fight_prop(pool_setter, false);
		modify_props_normal(pool_setter, false, 18);
	}
	if(cmd_tok[0] == "create_battle_monster"){
		if(cmd_tok.size() == 3){
			int npc_type_id = atoi(cmd_tok[1].c_str());
			int pos = atoi(cmd_tok[2].c_str());
			MSG_10210200 msg;
			msg.monter_type_id = npc_type_id;
			msg.pos = pos;
			this->create_battle_monster(msg);
		}
	}
	if (cmd_tok[0] == "buf") {
		if (cmd_tok.size() == 6) {
			int buff_id = atoi(cmd_tok[1].c_str());
			int last_time_ms = atoi(cmd_tok[2].c_str());
			int value = atoi(cmd_tok[3].c_str());
			int value1 = atoi(cmd_tok[4].c_str());
			int value2 = atoi(cmd_tok[5].c_str());
			Status_Effect_Argv argv;
			argv.caster_id = role_id();
			argv.status_id = buff_id;
			argv.last_time_ms = last_time_ms;
			argv.status_from = 1;
			argv.is_fight_buff = true;
			Effect_Argv_Prop prop;
			prop.i_effect_val1 = value;
			prop.d_effect_val1 = (double)value1;
			prop.d_effect_val2 = (double)value2;
			argv.data.push_back(prop);
			this->insert_status(argv);
		} else if (cmd_tok.size() == 2) {
			int buff_id = atoi(cmd_tok[1].c_str());
			this->del_status_overlay(buff_id, 1000);
		} else if (cmd_tok.size() == 3) {
			int buff_id = atoi(cmd_tok[1].c_str());
			int overlay = atoi(cmd_tok[2].c_str());
			this->del_status_overlay(buff_id, overlay);
		}
	}
	if(cmd_tok[0] == "ref_101"){
		if(this->move_scene()){
			NPC_Record* point = move_scene()->get_npc_record(101);
			if(point){
				NPC_MANAGER->record_refresh(point);
			}
		}
	}
	if(cmd_tok[0] == "cost_strength"){
		cost_phy_power(5);
	}

//	if (cmd_tok[0] == "gang") {
//		if (cmd_tok.size() >= 2) {
////			int i = 0;
////			i = atoi(cmd_tok[1].c_str());
////			switch(i) {
//////			case 1:
//////				GANG_WAR_CONTROLER->open_activity();
//////				break;
//////			case 2:
//////				GANG_WAR_CONTROLER->close_activity();
//////				break;
////			case 3:
//////				player_self()->req_gang_war_enter_scene();
////				break;
////			default:
////				break;
////			}
//		}
//	}
	if(cmd_tok[0] == "expe"){
		if (cmd_tok.size() == 1) {
			if(this->move_scene() && Expedition_Type == get_scene_type(this->move_scene()->scene_id())){
				Expedition_Scene* sce = dynamic_cast<Expedition_Scene*>(move_scene());
				int64_t gang_id = player_self()->ganger_detail().gang_id;
				echo_test_result("gang_id: ", gang_id);
				if(sce){
					echo_test_result("map_gang_in_num: ", sce->gang_ids().size());//公会入驻个数
					echo_test_result("point_gang_id: ", sce->point_gang_id().size());
					echo_test_result("map_base_point_num: ", sce->map_base_point_num());//地图数据中基地的总数
					echo_test_result("cur_line: ",sce->cur_line()); //场景的当前线
					Int_Hash_Set *p_area = sce->find_areavalue_by_gang_id(gang_id);
					if (p_area) {
						echo_test_result("area nums: ", p_area->size());
					}

					Expedition_Scene_Manager *manager = EXPEDITION_GLOBAL_SCENE_MANAGER->find_manager_by_uuid(player_self()->monitor_link().logic_muid.uuid);
					if (manager) {
						int point_id = manager->find_point_id_by_gang_id(gang_id);
						echo_test_result("point_id: ", point_id);
						const Int_Hash_Set *ihs = sce->find_base_territorys(point_id);
						if (ihs) {
							echo_test_result("base_territorys nums: ", ihs->size());
						}
					}
				}
			}
		}
		return 0;
	}
	if(cmd_tok[0] == "war_scene_finish"){
		if(this->move_scene()){
			War_Scene* scene = dynamic_cast<War_Scene*>(move_scene());
			if(scene){
				int win_camp = 0;
				if (cmd_tok.size() == 2) {
					win_camp = atoi(cmd_tok[1].c_str());
				}
				scene->war_battle_finish(win_camp);
			}
		}
		return 0;
	}
	if(cmd_tok[0] == "war_scene_kick"){
		if(this->move_scene()){
			War_Scene* scene = dynamic_cast<War_Scene*>(move_scene());
			if(scene){
				scene->gm_kick_player(this->role_id());
			}
		}
		return 0;
	}
	if(cmd_tok[0] == "bonfire_reset1"){
		if(player_self()){
			if(move_scene()){
				Campfire_Scene* scene = dynamic_cast<Campfire_Scene*>(move_scene());
				if(scene){
					scene->reset_firewood(player_self());
				}
			}
		}
	}
	if(cmd_tok[0] == "bonfire_reset2"){
		if(player_self()){
			if(move_scene()){
				Campfire_Scene* scene = dynamic_cast<Campfire_Scene*>(move_scene());
				if(scene){
					scene->reset_fire(player_self());
				}
			}
			player_self()->campfire_info()->add_firewood = 0;
		}
	}

	if (cmd_tok[0] == "give_up") {
		if (player_self()) {
			this->handle_give_up_battle();
		}
	}
	return 0;
}
