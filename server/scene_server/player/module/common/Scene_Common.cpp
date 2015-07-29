/*
 * Scene_Common.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Linqiyou
 */

#include "Scene_Common.h"
#include "NPC/NPC_Manager.h"
#include "NPC/Material.h"
#include "Msg_NPC_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_World_Boss_Struct.h"
#include "Msg_Role_Scene_Struct.h"
#include "Msg_Dungeon_Struct.h"
#include "Move_Scene.h"
#include "Err_Code.h"
#include "Scene_Player.h"
#include "single_scene/Single_Scene.h"
#include "Battle_Scene.h"
#include "Config_Cache_NPC.h"
#include "status/Status.h"
#include "status/Status_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Scene_Monitor.h"
#include "chater/Chater_Def.h"
#include "server_monitor/Server_Monitor.h"
#include "Global_Scene_Manager.h"
#include "team/Scene_Team_Manager.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "Msg_Active_Enum.h"
#include "Config_Cache_Dragon_Vale.h"
#include "world_boss/Gang_Boss_Manager.h"
#include "world_boss/World_Boss_Manager.h"
#include "honor_arena/Honor_Arena_Manager.h"
#include "scene_outpost/Scene_Outpost_Manager.h"
#include "tbay_scene/Tbay_Scene.h"
#include "Config_Cache_World_Boss.h"
#include "team/Scene_Teamer.h"
#include "Config_Cache_Dungeon.h"
#include "Config_Cache_Gang.h"
#include "Config_Cache_World_Boss.h"
#include  "gang/Gang_Def.h"
#include "Config_Cache_LFSkill.h"
#include "Utility_Func.h"

Scene_Common::Scene_Common() {
	reset();
}

Scene_Common::~Scene_Common() {
	// TODO Auto-generated destructor stub
}

// 解Common::serialize发过来的数据
void Scene_Common::load_public_detail(Block_Buffer &buf) {

}

void Scene_Common::module_init(void) {
}

void Scene_Common::sign_in(void) {
    return ;
}

void Scene_Common::sign_out(void) {
	stop_gather();
    return ;
}

void Scene_Common::reset(void) {
	is_revive_ = false;
	revive_time_ = Time_Value::zero;
	battle_plot_id_ = "";
	plot_step_ = Move_Scene::PLOT_NULL;
	plot_record_id_ = 0;
	plot_status_ = false;
	is_take_lift_ = false;
	is_drgaon_vale_rob_ = false;
}

int Scene_Common::time_up(const Time_Value& now){
	if(is_revive_){
		if(now >= revive_time_){
			is_revive_ = false;
			Block_Buffer buf;
			MSG_81000026 msg;
			msg.role_id = role_id();
			msg.state = 0;
			MAKE_MSG_BUF(msg, buf);
			player_self()->scene_broad_around(buf, Mover::SCENE_MOVE);
			// update move_able
			if(move_scene()){
				move_scene()->update_move_able_info(player_self());
			}
		}
	}
	return 0;
}

bool Scene_Common::is_revive(void){
	return is_revive_;
}

void Scene_Common::set_revive_info(bool is_revive, Time_Value revive_time){
	is_revive_ = is_revive;
	revive_time_ = revive_time;
}

void Scene_Common::reset_revive_info(void){
	is_revive_ = false;
	revive_time_ = Time_Value::zero;
}

int Scene_Common::gather_goods_result(role_id_t role_id, int result){
	Material* material = NPC_MANAGER->find_material_npc(role_id);
	if(!material){
		return -1;
	}
	material->gather_goods_result(this->role_id(), result);
	return 0;
}

int Scene_Common::get_drops_goods(MSG_10210001& msg){
	Drops* drops = NPC_MANAGER->find_drops_npc(msg.role_id);
	if(!drops)
	{
		MSG_USER("drops null, role_id = %ld", msg.role_id);
		return -1;
	}
	if(drops->get_drops_status() != Drops_Detail::DROPS_STATUS_NORMAL){
		MSG_USER("drops busy, role_id = %ld", msg.role_id);
		return -1;
	}
	if (drops->had_collect(player_self()->role_id())) {
		return ERROR_GATHER_DONE;
	}

	drops->drops_detail().set_getting_status();
	//send to logic
	MSG_20300000 logic_msg;
	logic_msg.drops_role_id = msg.role_id;
	logic_msg.item_index = msg.index;
	drops->get_drops_goods_by_index(player_self(), msg.index, logic_msg.item_vec);
	if(drops->move_scene()){
		logic_msg.scene_type = drops->move_scene()->scene_type();
		logic_msg.scene_id = drops->move_scene()->scene_id();
	}
	Block_Buffer logic_buf;
	MAKE_MSG_BUF(logic_msg, logic_buf);
	return this->send_to_logic(logic_buf);
}

int Scene_Common::start_gather(MSG_10210100& msg){
	if(get_player_type(msg.material_role_id) == Material_Type){
		Material* material = NPC_MANAGER->find_material_npc(msg.material_role_id);
		if(!material){
			//send gather fail to client
			MSG_50210100 res_msg;
			res_msg.material_role_id = msg.material_role_id;
			res_msg.result = 0;
			THIS_SEND_TO_CLIENT(res_msg);
			return -1;//ERROR_GATHER_ID_NONEXIST;
		}
		int ret = start_gather_material(material);
		if (0 == ret) {
			material_gather_set_.insert(msg.material_role_id);
		}
		return ret;
	}else{
		Machine* machine = NPC_MANAGER->find_machine_npc(msg.material_role_id);
		if(!machine){
			//send gather fail to client
			MSG_50210100 res_msg;
			res_msg.material_role_id = msg.material_role_id;
			res_msg.result = 0;
			THIS_SEND_TO_CLIENT(res_msg);
			return -1;//ERROR_GATHER_ID_NONEXIST;
		}
		return start_gather_machine(machine);
	}
	return 0;
}

int Scene_Common::stop_gather(MSG_10210101& msg){
	if(get_player_type(msg.material_role_id) == Material_Type){
		Material* material = NPC_MANAGER->find_material_npc(msg.material_role_id);
		if(!material){
			return -1;//ERROR_GATHER_ID_NONEXIST;
		}
		return stop_gather_material(material);

	}else{
		Machine* machine = NPC_MANAGER->find_machine_npc(msg.material_role_id);
		if(!machine){
			return -1;//ERROR_GATHER_ID_NONEXIST;
		}
		return stop_gather_machine(machine);
	}
}

int Scene_Common::stop_gather(void) {
	for (Role_Hash_Set::const_iterator it = material_gather_set_.begin(); it != material_gather_set_.end(); ++it) {
		Material *mat = NPC_MANAGER->find_material_npc(*it);
		if (mat) {
			mat->stop_gather(role_id());
		}
	}
	return 0;
}

int Scene_Common::start_gather_material(Material* material){
	int result = material->start_gather(this->player_self());
	if (result == 1000) {
		//gang_war;
		return -1;
	}
	if(result != 0){
		//send gather fail to client
		MSG_50210100 res_msg;
		res_msg.material_role_id = material->role_id();
		res_msg.result = 0;
		THIS_SEND_TO_CLIENT(res_msg);
		return result;
	}

	MSG_50210100 res_msg;
	res_msg.material_role_id = material->role_id();
	res_msg.gather_time = material->material_detail().gather_time;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}


int Scene_Common::stop_gather_material(Material* material){
	int result = material->stop_gather(role_id());
	if(result != 0){
		return result;
	}

	MSG_50210101 res_msg;
	res_msg.material_role_id = material->role_id();
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Scene_Common::open_machine(MSG_10210103& msg){
	Machine* machine = NPC_MANAGER->find_machine_npc(msg.role_id);
	if(!machine){
		return -1;//ERROR_GATHER_ID_NONEXIST;
	}
	return machine->open_machine(this->player_self());
}


int Scene_Common::start_gather_machine(Machine* machine){
	return machine->start_gather(this->player_self());
}


int Scene_Common::stop_gather_machine(Machine* machine){
	int result = machine->stop_gather(role_id());
	if(result != 0){
		return result;
	}

	MSG_50210101 res_msg;
	res_msg.material_role_id = machine->role_id();
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}


int Scene_Common::play_plot(std::string plot_id, int record_id, bool is_send_to_client/* = true*/){
	battle_plot_id_ = plot_id;
	plot_step_ = Move_Scene::PLOT_START_REQ;
	plot_record_id_ = record_id;
	//save to scene
	if(this->move_scene()){
		this->move_scene()->save_battle_plot_date(battle_plot_id_,
				plot_step_, plot_record_id_);
	}
	//send to client
	if(is_send_to_client){
		MSG_81000201 msg;
		msg.plot_id = plot_id;
		THIS_SEND_TO_CLIENT(msg);
	}
	plot_status_ = true;
	return 0;
}

int Scene_Common::begin_plot(std::string plot_id){
	if(plot_id != "" && plot_id == battle_plot_id_){
		if(plot_step_ == Move_Scene::PLOT_START_REQ ||
				plot_step_ == Move_Scene::PLOT_BEGIN ){

			plot_step_ = Move_Scene::PLOT_BEGIN;
			//save to scene
			if(this->move_scene()){
				this->move_scene()->save_battle_plot_date(battle_plot_id_,
						plot_step_, plot_record_id_);
			}
		}
	}

	plot_status_ = true;

	return 0;
}
bool Scene_Common::plot_status(void){
	return plot_status_;
}
int Scene_Common::add_drops_to_package(const int scene_type, const int scene_id, std::vector<Drops_Item>& drops_item){
	Block_Buffer buf;
	MSG_20300000 msg;
	msg.scene_type = scene_type;
	msg.scene_id = scene_id;
	msg.item_index = -2;
	msg.item_vec = drops_item;
	MAKE_MSG_BUF(msg, buf);
	send_to_logic(buf);
	return 0;
}

int Scene_Common::end_plot(std::string plot_id){
	if(plot_id != "" && plot_id == battle_plot_id_){
		if(plot_step_ == Move_Scene::PLOT_START_REQ || plot_step_ == Move_Scene::PLOT_BEGIN){
			if(this->move_scene()){
				NPC_Record* record = this->move_scene()->get_npc_record(plot_record_id_);
				if(record){
					record->create_battle(this->player_self());
				}
			}
		}

		//reset
		battle_plot_id_ = "";
		plot_step_ = Move_Scene::PLOT_NULL;
		plot_record_id_ = 0;
		//save to scene
		if(this->move_scene()){
			this->move_scene()->save_battle_plot_date(battle_plot_id_,
					plot_step_, plot_record_id_);
		}
	}
	plot_status_ = false;
	return 0;
}

void Scene_Common::notify_end_plot(void){
	//reset
	battle_plot_id_ = "";
	plot_step_ = Move_Scene::PLOT_NULL;
	plot_record_id_ = 0;
	//save to scene
	if(this->move_scene()){
		this->move_scene()->save_battle_plot_date(battle_plot_id_,
				plot_step_, plot_record_id_);
	}
	plot_status_ = false;
}

void Scene_Common::set_battle_plot_data(std::string plot_id, int plot_step, int record_id){
	battle_plot_id_ = plot_id;
	plot_step_ = plot_step;
	plot_record_id_ = record_id;
	if(battle_plot_id_ != "" && plot_step_ == Move_Scene::PLOT_START_REQ ){
		//send to client
		MSG_81000201 msg;
		msg.plot_id = plot_id;
		THIS_SEND_TO_CLIENT(msg);
		plot_status_ = true;
	}
}

int Scene_Common::create_battle_monster(MSG_10210200& msg){
	if(this->player_self() == NULL){
		return -1;
	}
	if(this->player_self()->battle_scene() == NULL){
		return -1;
	}
	NPC_Addition_Info add_info;
	add_info.birth_coord.x = this->player_self()->battle_position().pos;
	add_info.birth_coord.y = msg.pos;
	Monster* monster = NPC_MANAGER->create_battle_monster(msg.monter_type_id, NULL,
			add_info, this->player_self()->battle_scene());
	if(!monster){
		return -1;
	}
	this->player_self()->battle_scene()->listen_start_fight(monster->fighter());
	if (monster->use_skill() == 0){
		monster->set_battle_status(BATTLING);
	}
	return 0;
}

int Scene_Common::beat_back(MSG_10210104& msg){
	if(this->player_self() == NULL){
		return -1;
	}
	int id = msg.event_id;
	const Machine_Event_Config *event = CONFIG_CACHE_NPC->find_machine_event_config(id);
	if(!event){
		return -1;
	}
	Scene_Player* player = this->player_self();
	if(!player){
		return -1;
	}
	if(event->type == MACHINE_EVENT_BEAK_BACK){
		Coord player_coord = msg.p_coord.grid_coord();
		if(this->move_scene()){
			Coord point_a(msg.m_coord.x, msg.m_coord.y, Coord::PIXEL_TYPE);
			Coord point_b(msg.p_coord.x, msg.p_coord.y, Coord::PIXEL_TYPE);
			int distance = event->val_1;
			Coord point_c = point_b.grid_coord();
			this->move_scene()->calc_point_in_line_by_b(*player, point_a.grid_coord(), point_b.grid_coord(),
		   distance, point_c, player->mover_detail().toward, true, true);
		   this->move_scene()->mover_change_coord(player, point_c, false);
			//broadcast to player
			Block_Buffer buf;
			MSG_81000023 msg;
			msg.role_id = player->role_id();
			msg.pos_x = point_c.pixel_coord().x;
			msg.pos_y = point_c.pixel_coord().y;
			MAKE_MSG_BUF(msg, buf);
			player->scene_broad_around(buf, Mover::SCENE_MOVE);
		}
	}else if(event->type == MACHINE_EVENT_BUFFER){
		Status_Effect_Argv argv;
		argv.caster_id = this->role_id();
		argv.status_id = event->val_1;
		argv.last_time_ms = event->val_2;
		Effect_Argv_Prop prop;
		prop.i_effect_val1 = event->val_3;
		prop.d_effect_val1 = event->val_4;
		prop.d_effect_val2 = event->val_5;
		argv.data.push_back(prop);
		player->insert_status_out_fight(argv);
	}
	return 0;
}

int Scene_Common::pick_up_goods(std::vector<role_id_t> role_id_vec){
	int count = role_id_vec.size();
	int  result = 0;
	for(int i=0; i<count; ++i){
		role_id_t role_id = role_id_vec[i];
		Drops* drops = NPC_MANAGER->find_drops_npc(role_id);
		if(!drops)
		{
			MSG_USER("pick up drops null, role_id = %ld", role_id);
			result =  ERROR_ROLE_ID_NOEXIST;
			break;
		}
		if(drops->get_drops_status() != Drops_Detail::DROPS_STATUS_NORMAL){
			MSG_USER("drops busy, role_id = %ld", role_id);
			result = -1;
			break;
		}
		if (drops->had_collect(player_self()->role_id())) {
			return -1;
		}

		// 没有收益处理
		if(player_self()->move_scene()->scene_type() == Team_Fb_Scene_Type
				|| player_self()->move_scene()->scene_type() == HERO_DREAM_Scene_Type
				|| player_self()->move_scene()->scene_type() == TEAM_DEFEND_Scene_Type){
			if(!player_self()->is_use_profile()){
				drops->drops_detail().set_getting_status();
				drops->erase_drops_item(-1, this->role_id(), this->fighter());
			} else {
				drops->drops_detail().set_getting_status();
				//send to logic
				MSG_20300000 logic_msg;
				logic_msg.drops_role_id = role_id;
				logic_msg.item_index = -1;

				drops->get_drops_goods_by_index(player_self(), -1, logic_msg.item_vec);
				if(drops->move_scene()){
					logic_msg.scene_type = drops->move_scene()->scene_type();
					logic_msg.scene_id = drops->move_scene()->scene_id();
				}
				Block_Buffer logic_buf;
				MAKE_MSG_BUF(logic_msg, logic_buf);
				result = this->send_to_logic(logic_buf);
			}
		}else{
			if(player_self()->is_no_profit()){
				drops->drops_detail().set_getting_status();
				drops->erase_drops_item(-1, this->role_id(), this->fighter());
			} else {
				drops->drops_detail().set_getting_status();
				//send to logic
				MSG_20300000 logic_msg;
				logic_msg.drops_role_id = role_id;
				logic_msg.item_index = -1;
				drops->get_drops_goods_by_index(player_self(), -1, logic_msg.item_vec);
				if(drops->move_scene()){
					logic_msg.scene_type = drops->move_scene()->scene_type();
					logic_msg.scene_id = drops->move_scene()->scene_id();
				}
				Block_Buffer logic_buf;
				MAKE_MSG_BUF(logic_msg, logic_buf);
				result = this->send_to_logic(logic_buf);
			}
		}
	}
	return result;
}

int Scene_Common::req_refresh_scene_point(int point_id){
	if(player_self() == NULL){
		return -1;
	}
	if(move_scene()){
		return move_scene()->refresh_monster_point(point_id, player_self()->role_id());
	}
	return 0;
}

int Scene_Common::req_fight_scene_point(int point_id){
	if(move_scene() && player_self()){
		if(player_self()->is_in_battle()){
			Battle_Scene * bt_scene = player_self()->battle_scene();
			if(bt_scene){
				MSG_USER("req_fight_scene_point error, role_id = %ld, battle_id: %ld, battle_status: %d, over_step: %d", this->role_id(),
						bt_scene->battle_id(), bt_scene->battle_status(), bt_scene->over_step());
			}else{
				MSG_USER("req_fight_scene_point error, role_id = %ld", this->role_id());
			}
		}else{
			NPC_Record* point = move_scene()->get_npc_record(point_id);
			if(point){
				point->create_battle(player_self());
			}
		}
	}
	return 0;
}

int Scene_Common::req_take_lift_finish(role_id_t role_id){
	Machine* machine = NPC_MANAGER->find_machine_npc(role_id);
	if(!machine){
		return -1;
	}
	if(this->player_self()){
		return machine->take_lift_finish(this->player_self());
	}
	return 0;
}

bool Scene_Common::is_take_lift(void){
	return is_take_lift_;
}

void Scene_Common::set_take_lift_begin(void){
	is_take_lift_ = true;
}

void Scene_Common::set_take_lift_finish(void){
	is_take_lift_ = false;
}

int Scene_Common::inner_send_bless_buf(role_id_t sender_role_id, int32_t bless_buf_id) {
//	Scene_Player* target_player = SCENE_MONITOR->find_scene_player_by_role_id(target_role_id);
//	if (! target_player) {
//		MSG_USER_TRACE("can not find scene player:%ld", target_role_id);
//		return ERROR_PLAYER_NOT_ONLINE;
//	}

	int8_t has_buf_id = 0;
	const Gang_Cfg_Bless_Buf_Id_Map& bless_buf_id_map = CONFIG_CACHE_GANG->bless_buf_id_map();
	for (Gang_Cfg_Bless_Buf_Id_Map::const_iterator it_buf_map = bless_buf_id_map.begin();
			it_buf_map != bless_buf_id_map.end(); ++it_buf_map) {
		if (player_self()->has_status_id(it_buf_map->first)) {
			has_buf_id = 1;
			break;
		}
	}

	//int8_t has_buf_id = target_player->has_status_id(bless_buf_id); (需求改成所有公会祝福buf都是互斥的)

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_SEND_BLESS_BUF);
	inner_buf.write_int64(sender_role_id);
	inner_buf.write_int32(bless_buf_id);
	inner_buf.write_int8(has_buf_id);
	inner_buf.finish_message();
	player_self()->send_to_logic(inner_buf);

	return 0;
}

int Scene_Common::req_dungeon_panel_close(void){
	if(player_self() && player_self()->move_scene()){
		player_self()->move_scene()->reset_delay_show_ts_point_time();
	}
	return 0;
}

int Scene_Common::req_single_dungeon_revive(int8_t type){
	if(player_self() && player_self()->move_scene() &&
			player_self()->move_scene()->scene_type() == FuBen_Scene_Type){
		int param1 = 0;
		int param2 = 0;
		int param3 = 0;
		player_self()->move_scene()->get_scene_param(param1, param2, param3);
		if(param1 != 1){
			return 0;
		}
		if(type == 0){// 免费复活
			if(param2 <= 0){// 免费复活次数
				{
					MSG_50200215 msg;
					msg.result = 0;
					THIS_SEND_TO_CLIENT(msg);
				}
				return ERROR_FB_REVIVE_NO_FREE;
			}
			param2 -= 1;
			param1 = 0;
			player_self()->move_scene()->set_scene_param(param1, param2, param3, true);
			player_self()->reset_revive_info();
			{
				MSG_81000026 msg;
				msg.role_id = player_self()->role_id();
				msg.state = 0;
				THIS_SEND_TO_CLIENT(msg);
			}
			{
				MSG_50200215 msg;
				msg.result = 1;
				THIS_SEND_TO_CLIENT(msg);
			}
		}else{// 付费复活
			if(param2 > 0){// 免费复活次数
				{
					MSG_50200215 msg;
					msg.result = 0;
					THIS_SEND_TO_CLIENT(msg);
				}
				return ERROR_FB_REVIVE_HAS_FREE;
			}
			Block_Buffer buf;
			buf.make_message(INNER_WAR_SCENE_SYNC_INFO);
			buf.write_int8(3);
			buf.write_int32(param3);
			buf.finish_message();
			this->send_to_logic(buf);
		}
	}
	return 0;
}

int Scene_Common::req_scene_chat(std::vector<Chat_Content> &content, int32_t sys_type) {
	if(player_self() && player_self()->move_scene()){
		MSG_83000000 msg;
		msg.chat_type = chat_scene;
		msg.system_type = sys_type;
		msg.role_type = player_self()->base_detail().role_type;
		msg.content = content;
		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		player_self()->move_scene()->broadcast_chat_all(player_self()->mover_detail().layer_id, buf);
	}
	return 0;
}

int Scene_Common::sync_mount_ride(int ride, int horse_model_id, int horse_speed){
	if(player_self()){
		player_self()->set_mount_ride(ride);
		player_self()->set_mount_horse_model_id(horse_model_id);
//		Operate_Type op_type;
//		if(ride == 0){// 下马
//			op_type = O_SUB;
//		}else{
//			op_type = O_ADD;
//		}
//		player_self()->fight_normal_prop_modify(PT_SPEED, op_type, horse_speed, true);
		broadcast_avatar_to_client();
	}
	return 0;
}

void Scene_Common::broadcast_avatar_to_client(void) {
	MSG_81000003 msg;
	msg.role_id = role_id();
	msg.avatar.assign(player_self()->base_detail().avatar_vec.begin(),
			player_self()->base_detail().avatar_vec.end());
	if(player_self()->base_detail().ride != 0){
		msg.avatar.push_back(player_self()->base_detail().mount_model_id);
	}
	Block_Buffer buf;
	MAKE_MSG_BUF(msg, buf);
	player_self()->scene_broad_around(buf, Mover::SCENE_MOVE);
}

int Scene_Common::req_world_boss_act_guild_bless(int8_t icon_id){
	switch(icon_id){
		case 16:{
			return WORLD_BOSS_MANAGER->req_world_boss_act_guild_bless(this->player_self());
			break;
		}
		case 19:{
			return GANG_BOSS_MANAGER->req_world_boss_act_guild_bless(this->player_self());
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Scene_Common::req_world_boss_use_ability(int8_t icon_id, int type){
	switch(icon_id){
		case 16:{
			return WORLD_BOSS_MANAGER->req_world_boss_use_ability(this->player_self(), type);
			break;
		}
		case 19:{
			return GANG_BOSS_MANAGER->req_world_boss_use_ability(this->player_self(), type);
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

void Scene_Common::world_boss_add_power_up_buf(const int8_t icon_id){
	if(player_self()){
		Status_Effect_Argv argv;
		const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(icon_id);
		if(cfg_base){
			Status_Effect_Argv argv;
			argv = cfg_base->power_up_buff;
			argv.caster_id = role_id();
			argv.last_time_ms = -1;
			player_self()->insert_status_out_fight(argv);
			player_self()->insert_status_to_hero(argv);
		}
	}
}

void Scene_Common::world_boss_del_power_up_buf(const int8_t icon_id){

}

void  Scene_Common::world_boss_clear_power_up_buf(const int8_t icon_id){
	if(player_self()){
		const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(icon_id);
		if(cfg_base){
			int buf_id = cfg_base->power_up_buff.status_id;
			int result = player_self()->erase_status_type(buf_id);
			if(result == 0){
				MSG_81000071 msg;
				Status_Info_Msg msg_info;
				msg_info.status_id = buf_id;
				msg_info.overlay = 0;
				msg_info.last_time_ms = -1;
				msg_info.status_owner = role_id();
				msg.buff_detail.push_back(msg_info);
				Block_Buffer buf;
				MAKE_MSG_BUF(msg, buf);
				this->send_to_client(buf);
			}
			player_self()->del_status_to_hero(buf_id);
		}
	}
}

void Scene_Common::world_boss_add_leader_buf(const int8_t icon_id){
	if(player_self()){
		Status_Effect_Argv argv;
		const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(icon_id);
		if(cfg_base){
			Status_Effect_Argv argv;
			argv = cfg_base->leader_buff;
			argv.caster_id = role_id();
			argv.last_time_ms = -1;
			player_self()->insert_status_out_fight(argv);
		}
	}
}

void Scene_Common::world_boss_del_leader_buf(const int8_t icon_id){

}

void Scene_Common::world_boss_clear_leader_buf(const int8_t icon_id){
	if(player_self()){
		const World_Boss_Base_Config* cfg_base = CONFIG_CACHE_WORLD_BOSS->get_base_config(icon_id);
		if(cfg_base){
			int buf_id = cfg_base->leader_buff.status_id;
			int result = player_self()->erase_status_type(buf_id);
			if(result == 0){
				MSG_81000071 msg;
				Status_Info_Msg msg_info;
				msg_info.status_id = buf_id;
				msg_info.overlay = 0;
				msg_info.last_time_ms = -1;
				msg_info.status_owner = role_id();
				msg.buff_detail.push_back(msg_info);
				Block_Buffer buf;
				MAKE_MSG_BUF(msg, buf);
				this->send_to_client(buf);
			}
		}
	}
}

int Scene_Common::req_honor_arena_match(void){
	int ret = HONOR_ARENA_MANAGER->req_match(player_self());
	return ret;
}

int Scene_Common::req_honor_arena_receive(const int type){
	return HONOR_ARENA_MANAGER->req_receive(player_self(), type);
}

int Scene_Common::req_honor_arena_rank(void){
	return HONOR_ARENA_MANAGER->req_rank(player_self());
}

int Scene_Common::req_honor_arena_panel_info(void){
	return HONOR_ARENA_MANAGER->req_panel_info(player_self());
}

int Scene_Common::req_honor_arena_set_auto_match(const int8_t is_auto){
	return HONOR_ARENA_MANAGER->req_set_auto_match(player_self(), is_auto);
}

int Scene_Common::req_vagrant_item_info(const int64_t monster_role_id){
	return SCENE_OUTPOST_MANAGER->req_vagrant_item_info(player_self(), monster_role_id);
}
int Scene_Common::req_vagrant_buy_item(const int64_t monster_role_id, const int item_index){
	return SCENE_OUTPOST_MANAGER->req_vagrant_buy_item(player_self(), monster_role_id, item_index);
}

int Scene_Common::req_fetch_tbay_info(void){
	Move_Scene* scene = player_self()->move_scene();
	if(!scene){
		return 0;
	}
	Tbay_Scene* tbay_scene = dynamic_cast<Tbay_Scene*>(scene);
	if(!tbay_scene){
		return 0;
	}
	return tbay_scene->req_fetch_tbay_info(player_self());
}

int Scene_Common::req_enter_scene_outpost(const int64_t monster_role_id){
	Move_Scene* scene = move_scene();
	if(!scene){
		return 0;
	}
	Single_Scene* single_scene = dynamic_cast<Single_Scene*>(scene);
	if(!single_scene){
		return 0;
	}
	Mover* mover = single_scene->find_mover_with_layer(0, monster_role_id);
	if(!mover || mover->monster_self() == NULL){
		return 0;
	}
	// 没有收益的玩家不能进去
	if(scene->is_no_profit(role_id())){
		return ERROR_FB_ENTER_TBAY_NO_PROFIT;
	}
	int target_scene_id = 0;
	int target_scene_time = 0;
	int target_scene_type = 0;
	mover->monster_self()->get_ex_param(target_scene_id, target_scene_time, target_scene_type);
	if(target_scene_id == 0){
		return 0;
	}
	player_self()->inner_transmit(target_scene_id,0,monster_role_id);
	return 0;
}

int Scene_Common::req_tbay_bless(const int8_t bless_index){
	Move_Scene* scene = player_self()->move_scene();
	if(!scene){
		return 0;
	}
	Tbay_Scene* tbay_scene = dynamic_cast<Tbay_Scene*>(scene);
	if(!tbay_scene){
		return 0;
	}
	return tbay_scene->req_tbay_bless(player_self(), bless_index);
}

int Scene_Common::req_attck_rob_match_member(int64_t to_uuid, std::string to_server_name,
							   int64_t uuid, int64_t role_id, std::string server_name, std::string role_name, uint32_t coin) {
	if (player_self()->is_in_battle()) {
		MSG_USER("DRAGON_VALE_ROB: player is in battle, role_name=%s", player_self()->role_name().c_str());
		SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(to_uuid, player_self()->role_id(), coin, 2);
		return ERROR_DRAGON_VALE_ROBBER_IN_BATTLE;
	}

	if (player_self()->dead_status()) {
		MSG_USER("DRAGON_VALE_ROB: player is die, role_name=%s", player_self()->role_name().c_str());
		SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(to_uuid, player_self()->role_id(), coin, 2);
		return ERROR_DRAGON_VALE_ROBBER_DEAD;
	}


	MSG_20600019 msg_20600019;
	msg_20600019.role_id = role_id;
	Block_Buffer buf_20600019;
	buf_20600019.make_message(INNER_ENTER_ROB_PROTECT_TIME);
	msg_20600019.serialize(buf_20600019);
	buf_20600019.finish_message();
	SCENE_MONITOR->send_to_logic(uuid, buf_20600019);

	//notify logic robber begin attck
	{
		MSG_20600029 msg_20600029;
		THIS_SEND_TO_LOGIC(msg_20600029);
	}

	//通知逻辑服，掠夺任务监听
	{
		MSG_20600025 msg_20600025;
		Block_Buffer buf_20600025;
		buf_20600025.make_message(INNER_ROB_DRAGON_VALE_TASK_LISTEN);
		msg_20600025.serialize(buf_20600025);
		buf_20600025.finish_message();
		player_self()->send_to_logic(buf_20600025);
	}

	Dragon_Vale_Rob_Fight rob;
	rob.uuid  = to_uuid;
	rob.role_id = player_self()->role_id();
	rob.server_name = to_server_name;
	rob.role_name = player_self()->role_name();
	Dragon_Vale_Rob_Fight target;
	target.uuid = uuid;
	target.role_id = role_id;
	target.server_name = server_name;
	target.role_name = role_name;

	Dragon_Vale_Rob_Fight_Map::const_iterator target_it =
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().find(target);
	if(target_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().end()) {
		if(target_it->second.uuid == to_uuid && target_it->second.role_id == player_self()->role_id()) {
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(target);
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().erase(rob);
		}//end if
		else {
			MSG_USER("DRAGON_VALE_ROB: target is in battle, role_name=%s", player_self()->role_name().c_str());
			SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(to_uuid, player_self()->role_id(), coin, 2);
			return ERROR_TARGET_IS_IN_BATTLE;
		}//end else
	}//end if

	Dragon_Vale_Rob_Fight_Map::const_iterator rob_it =
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().find(target);
	if(rob_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().end()) {
		MSG_USER("DRAGON_VALE_ROB: target is in battle, role_name=%s", player_self()->role_name().c_str());
			SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(to_uuid, player_self()->role_id(), coin, 2);
			return ERROR_TARGET_IS_IN_BATTLE;
	}

	rob_it = SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().find(rob);
	if(rob_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().end()) {
		if(rob_it->second.uuid == uuid && rob_it->second.role_id == role_id) {
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(target);
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().erase(rob);
		}
		else {
			Dragon_Vale_Rob_Fight_Map::const_iterator find_it =
					SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().find(rob_it->second);
			if(find_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().end()) {
				if(find_it->second.uuid == rob.uuid && find_it->second.role_id == rob.role_id) {
					SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(rob_it->second);
				}
			}

			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().erase(rob);
		}
	}//end if

	target_it = SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().find(rob);
	if(target_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().end()) {
		if(target_it->second.uuid == uuid && target_it->second.role_id == role_id) {
			MSG_USER("DRAGON_VALE_ROB: target is in battle, role_name=%s", player_self()->role_name().c_str());
			SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(to_uuid, player_self()->role_id(), coin, 2);
			return ERROR_TARGET_IS_IN_BATTLE;
		}
		else {
			Dragon_Vale_Rob_Fight_Map::const_iterator find_it =
					SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().find(target_it->second);
			if(find_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().end()) {
				MSG_USER("DRAGON_VALE_ROB: other is robbing your dragon_vale, role_name=%s", player_self()->role_name().c_str());
				SCENE_DRAGON_VALE_MANAGER->process_rob_match_fail_back_coin(to_uuid, player_self()->role_id(), coin, 2);
				return ERROR_OTHER_ROB_YOUR_DRAGON_VALE;
			}
			else {
				SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(rob);
			}
		}
	}//end if

	SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().insert(std::make_pair(target, rob));
	SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().insert(std::make_pair(rob, target));

	{
		MSG_20600031 msg;
		msg.to_uuid = to_uuid;
		msg.to_role_id = player_self()->role_id();
		msg.to_server_name = to_server_name;
		msg.to_role_name = player_self()->role_name();
		msg.uuid = uuid;
		msg.role_id = role_id;
		msg.server_name = server_name;
		msg.role_name = role_name;
		msg.coin = coin;
		msg.flag = 0;
		Block_Buffer buf_20600031;
		buf_20600031.make_message(INNER_TARGET_LOGIC_PROCESS_ROB_DRAGON_VALE);
		msg.serialize(buf_20600031);
		buf_20600031.finish_message();
		SCENE_MONITOR->send_to_logic(uuid, buf_20600031);
	}

	return 0;
}

bool Scene_Common::get_dragon_vale_rob() {
	return is_drgaon_vale_rob_;
}

void Scene_Common::set_dragon_vale_rob(bool flag) {
	is_drgaon_vale_rob_ = flag;
}

int Scene_Common::req_accept_fight_with_robber(int64_t uuid, int64_t role_id) {
	const Main_Base_Cfg &main_base_cfg = CONFIG_CACHE_DRAGON_VALE->find_main_base_cfg();
	Dragon_Vale_Rob_Fight rob;
	rob.uuid = uuid;
	rob.role_id = role_id;
	Dragon_Vale_Rob_Fight target;
	target.uuid = player_self()->monitor_link().logic_muid.uuid;
	target.role_id = player_self()->role_id();

	Scene_Player *rob_player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
	if(!rob_player) {
		SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(target);
		SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().erase(rob);
		MSG_USER("DRAGON_VALE_ROB: rob_player is offline");
		return ERROR_DRAGON_VALE_ROBBER_OFFLINE;
	}

	if(!rob_player->get_dragon_vale_rob()) {
		return ERROR_DRAGON_VALE_ROB_FALSE;
	}

	Dragon_Vale_Rob_Fight_Map::iterator target_it =
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().find(target);
	if(target_it == SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().end()) {
		Dragon_Vale_Rob_Fight_Map::const_iterator rob_it =
				SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().find(rob);
		if(rob_it != SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().end()) {
			if(rob_it->second.uuid == target.uuid && rob_it->second.role_id == target.role_id) {
				SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().insert(std::make_pair(target, rob));
			}
			else {
				MSG_USER("DRAGON_VALE_ROB: rob time out, to_uuid=%ld, to_role_id= %ld, role_name=%s", uuid, role_id, player_self()->role_name().c_str());
				return ERROR_DRAGON_VALE_ROB_NOT_EXIST;
			}
		}
		else {
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().insert(std::make_pair(target, rob));
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().insert(std::make_pair(rob, target));
		}
	}//end if

	Dragon_Vale_Rob_Fight_Map::iterator rob_it =
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().find(rob);
	if(rob_it == SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().end()) {
		SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_rob_target_map().insert(std::make_pair(rob, target));
	}
	else {
		if(rob_it->second.uuid != target.uuid || rob_it->second.role_id != target.role_id) {
			SCENE_DRAGON_VALE_MANAGER->get_dragon_vale_target_rob_map().erase(target);
			MSG_USER("DRAGON_VALE_ROB: rob time out");
			return ERROR_DRAGON_VALE_ROB_NOT_EXIST;
		}
	}

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(this->move_scene());
	if(!scene) {
		MSG_TRACE_ABORT("create battle scene error, role_name=%s", player_self()->role_name().c_str());
		return ERROR_CREATE_BATTLE_SCENE_ERROR;
	}

	{
		MSG_20600034 msg;
		msg.role_id = target.role_id;
		msg.flag = 1;
		Block_Buffer block_buf;
		block_buf.make_message(msg.msg_id);
		msg.serialize(block_buf);
		block_buf.finish_message();
		SCENE_MONITOR->send_to_logic(target.uuid, block_buf);
	}

	rob_player->set_dragon_vale_rob(false);

	scene->set_battle_type(Battle_Scene::BATTLE_TYPE_DRAGON_VALE);
	scene->set_map_id(main_base_cfg.battle_map_id);

	if(rob_player->set_battle_position(0, Position_Key::LINE_TWO_FRONT, scene) == 0){
		rob_player->enter_battle_scene(scene);
	}
	else {
		MSG_TRACE_ABORT("set_battle_position error, role_name=%s", rob_player->role_name().c_str());
	}

	if(player_self()->set_battle_position(1, Position_Key::LINE_TWO_FRONT, scene) == 0){
		player_self()->enter_battle_scene(scene);
	}
	else {
		MSG_TRACE_ABORT("set_battle_position error, role_name=%s", player_self()->role_name().c_str());
	}

	return 0;
}

Time_Value& Scene_Common::get_begin_rob_count_down() {
	return begin_rob_count_down_;
}

void Scene_Common::set_begin_rob_count_down(const Time_Value &now) {
	begin_rob_count_down_.set(now);
}

int Scene_Common::req_gain_rob_production(int64_t to_uuid, int64_t to_role_id, int64_t uuid, int64_t role_id,
							uint32_t building_id, uint8_t building_type) {

	return 0;
}

int Scene_Common::try_sub_energy(const uint16_t energy) {

	if (energy > 0 ) {
		if ( energy_ < energy )
		   return ERROR_ENERGY_NOT_ENOUGHT;
	}
	return 0;
}

int Scene_Common::sub_energy(const uint16_t energy,const bool check ) {

    if ( !check) {
    	int result = try_sub_energy(energy);
    	if ( 0 != result )
    		return ERROR_ENERGY_NOT_ENOUGHT;
    }
    //
    energy_ = energy_- energy;
    Block_Buffer inner_buf;
    inner_buf.make_message(SYSN_ENERGY_TO_LOGIC);
    inner_buf.write_uint16(energy_);
    inner_buf.finish_message();
    this->send_to_logic(inner_buf);
	return 0;
}

void Scene_Common::set_energy(const uint16_t energy) {
	energy_ = energy;
}

void Scene_Common::set_life_skill(const int skill_id) {
	life_skill_id_ = skill_id;
}

int Scene_Common::spar_gather(const int id,Item_Info_Vec &item_vec) {
	Int_Int_Map item_map;
	item_map.clear();
	const Gather_Con_Unit *unit = CONFIG_CACHE_LFSKILL->gather_config(id);
	if ( NULL == unit )
		return 0;
	if ( 0 == life_skill_id_ ) {
		return ERROR_NEED_LEARN_BASE_SKILL;
	}
	if ( life_skill_id_ < unit->skill_id ) {
		return ERROR_LIFE_SKILL_LEVEL_NOT_ENOUGHT;
	}
	if ( unit->award_vec.size() != unit->chance_vec.size() || 0 == unit->gather_num )
		return ERROR_CONFIG_NOT_EXIST;
	Int_Vec weight;
	// 垮阶权重
	if (life_skill_id_%1000/100 >unit->skill_id%1000/100)
		weight = unit->chance_vec;
	else {
		// 跨级权重
		// 之前自己SB，算等级是用%10   但是是最后两位表示等级需要%100
//		const Gather_Change_Cof *chance_unit = CONFIG_CACHE_LFSKILL->gather_change_config(life_skill_id_%10);
		const Gather_Change_Cof *chance_unit = CONFIG_CACHE_LFSKILL->gather_change_config(life_skill_id_%100);
		if ( NULL == chance_unit || unit->chance_vec.size() != chance_unit->gather_lv_chance.size())
			return ERROR_CONFIG_NOT_EXIST;
        for (uint16_t i = 0;i<unit->chance_vec.size();++i) {
        	weight.push_back(unit->chance_vec[i] + chance_unit->gather_lv_chance[i]);
        }
	}

	int result = try_sub_energy(unit->energy_cost);
	if ( 0!= result )
		return result;
	for (uint8_t i = 0;i<unit->gather_num;++i) {
		int index = Utility_Func::get_rand_index(weight);
		if (-1 == index || uint(index) >= unit->award_vec.size() )
			return ERROR_CONFIG_NOT_EXIST;
//		Item_Info item;
//		item.id = unit->award_vec[index];
//		item.amount = 1;
//		item.bind = Item_Detail::BIND;
//		tmp_item_vec.push_back(item);
		Int_Int_Map::iterator iter = item_map.find(unit->award_vec[index]);
		if ( iter == item_map.end() )
			item_map.insert(std::make_pair(unit->award_vec[index],1));
		else
			iter->second++;
	}
	for ( Int_Int_Map::iterator it = item_map.begin();it!= item_map.end();++it) {
		Item_Info item;
		item.id = it->first;
		item.amount = it->second;
		item.bind = Item_Detail::BIND;
		item_vec.push_back(item);
	}
	return 0;
}

int Scene_Common::spar_gather_cost(const int id) {
	const Gather_Con_Unit *unit = CONFIG_CACHE_LFSKILL->gather_config(id);
	if ( unit ) {
		sub_energy(unit->energy_cost);
	}
	return 0;
}
