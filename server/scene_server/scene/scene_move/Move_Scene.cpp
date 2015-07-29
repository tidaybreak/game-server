/*
 * Move_Scene.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: ti
 */

#include "Move_Scene.h"
#include "fighter/Fighter.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_NPC.h"
#include "Err_Code.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "NPC/NPC_Config.h"
#include "Scene_Player.h"
#include "Base_Detail.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "Config_Cache_Scene.h"
#include "dragon_vale_scene/Dragon_Vale_Scene.h"
#include "Global_Scene_Manager.h"
#include "Battle_Scene.h"

Move_Scene::Move_Scene(){
	// TODO Auto-generated constructor stub
	reset();
}

Move_Scene::~Move_Scene() {
	// TODO Auto-generated destructor stub
}

void Move_Scene::reset(void) {
	Move_Scene_Base::reset();

	create_scene_args_.reset();
	transmit_point.clear();
	npc_section_.reset();

	scene_status_ = SCENE_NORMAL_STATUS;
	is_scene_pass_ = false;
	is_scene_complete_ = false;
	pass_status_ = 0;
	scene_param_1_ = 0;
	scene_param_2_ = 0;
	scene_param_3_ = 0;

	recover_time_ = Time_Value::zero;
	scene_begin_time_ = Time_Value::zero;

	player_enter_battle_set_.clear();
	no_profit_player_set_.clear();

	is_delay_show_ts_point_ = false;
	is_show_ts_point_ = false;
	ts_point_delay_time_ = Time_Value::zero;

	enter_players_set_.clear();
	nor_tick_time_ = Time_Value::zero;

	player_map_.clear();
	monster_clear_point_set_.clear();
}

int Move_Scene::enter_scene(Mover *mover) {
	if (0 == mover->player_self()){
		return 0;
	}

	player_map_.insert(std::make_pair(mover->role_id(), mover));

	int id = mover->player_self()->base_detail().rela_scene_id;
	if (0 != id && id == scene_config_->dungeon.id){//多层副本时，取上一层有收益情况
		bool no_profit = mover->player_self()->is_no_profit();
		if(no_profit){
			this->set_no_profit_player(mover->role_id());
		}
		player_enter_battle_set_.insert(mover->role_id());
	}else{
		bool no_profit = this->is_no_profit(mover->role_id());
		mover->player_self()->set_no_profit(no_profit);
		mover->fighter()->clear_currnet_morale();
	}
	return 0;
}

int Move_Scene::exit_scene(Mover *mover, bool offline) {
	if (0 == mover->player_self()){
		return 0;
	}

	player_map_.erase(mover->role_id());

	if (offline){
		return 0;
	}
	if(!scene_config_){
		return 0;
	}
	if (0 != scene_config_->dungeon.relation
			&& scene_config_->dungeon.relation
					== mover->player_self()->base_detail().rela_scene_id){//多层副本
		return 0;
	}else{
		mover->fighter()->clear_currnet_morale();
		mover->player_self()->set_rela_scene_id(0);
		mover->player_self()->set_no_profit(false);
	}
	return 0;
}

int Move_Scene::init_scene_normal(int scene_id, int scene_order, int map_id) {
	Move_Scene_Base::init_scene_normal(scene_id, scene_order, map_id);
	if(scene_config()){
		transmit_point = scene_config()->transmit_point;
		if(scene_config()->type == FuBen_Scene_Type &&
				scene_config()->dungeon.first_layer_id != 0){//多层副本
			//非首层
			if(scene_config()->dungeon.id != scene_config()->dungeon.first_layer_id){
				set_pass_status(Move_Scene::SCENE_PASS_STATUS_NO_COMPELTE);
			}
		}
	}
	Time_Value time_now = Time_Value::gettimeofday();
	scene_begin_time_ = time_now;
	nor_tick_time_ = time_now;
	npc_init(scene_id, 0, 0);

	return 0;
}

int Move_Scene::release_scene_normal(void) {
	MSG_DEBUG("move scene release,scene_id:%d", scene_id());
	battle_scene_release();
	npc_release();

	// 如果到这里还有人在场景，让他们退出，异常
	Mover_Map player_map = player_map_;
	for (Mover_Map::iterator it = player_map.begin(); it != player_map.end(); ++it) {
		MSG_USER_TRACE("scene player exit role:%ld scene", it->first, it->second->move_scene_id());
		if (it->second->validate_mover_scene()) {
			it->second->exit_move_scene(false);
			it->second->player_self()->inner_transmit(0, 0, 0);
		}
	}
	return 0;
}

int Move_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene_Base::client_scene_complete(fighter);
	Scene_Player *player = fighter.player_self();
	if (!player) {
		return -1;
	}

	{
		// 更新地图传送点
		MSG_81000040 tran_point;
		int count = transmit_point.size();
		for (int i = 0; i < count; ++i) {
			if (transmit_point[i].visible == 0) {
				tran_point.transmit_point.push_back(transmit_point[i]);
			} else if (transmit_point[i].visible == 1 || transmit_point[i].visible == 2) {
				if (is_scene_complete_ && is_show_ts_point_) {
					// 精英副本最后一关，不显示继续挑战
					if(this->scene_type() == Elite_Scene_Type){
						const Scene_Config *des_scene_config =
								CONFIG_CACHE_SCENE->scene_config_cache(transmit_point[i].des_scene_id);
						if(des_scene_config){
							tran_point.transmit_point.push_back(transmit_point[i]);
						}
					}else{
						tran_point.transmit_point.push_back(transmit_point[i]);
					}
				}
			}
		}
		OBJ_SEND_TO_CLIENT(tran_point, fighter);
	}
	//发送指引路径点
	if (this->npc_section_.path_guide_vec.empty() == false) {
		int point = this->npc_section_.get_next_path_guide();
		if (point != -1) {
			send_path_guide(point, player);
		}
	}
	//
	if (enter_players_set_.count(fighter.role_id()) == 0) {
		enter_players_set_.insert(fighter.role_id());
		init_player_own_point(fighter.role_id());
	}
	// 副本状态更改通知
	{
		MSG_81000019 msg;
		msg.status = get_pass_status();
		msg.value1 = scene_param_1_;
		msg.value2 = scene_param_2_;
		msg.value3 = scene_param_3_;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}


	return 0;
}

int Move_Scene::add_new_monster_point(const int point, bool is_boss, Coord point_coord) {
	monster_clear_point_set_.erase(point);
	return 0;
}

int Move_Scene::scene_pass_to_logic(void) {
	return 0;
}

void Move_Scene::send_path_guide(int point, Scene_Player* player) {
	return;
}

void Move_Scene::hook_player_enter_battle(Scene_Player* player) {
	return;
}

void Move_Scene::hook_scene_pass(void) {
	return;
}

void Move_Scene::hook_battle_finish(int battle_type, int win_pos,
		std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec,
		std::vector<int> kill_monters) {
	return;
}

void Move_Scene::hook_gather_special_handle(Mover* mover, int point_id,
		int material_type) {
	return;
}
void Move_Scene::hook_gather_start_special(Mover* mover, int point_id,
		int material_type) {
	return;
}

void Move_Scene::hook_gather_success(Mover* mover, int point_id,
		int material_type) {
	return;
}

void Move_Scene::hook_player_move(Scene_Player* player) {
	return;
}

void Move_Scene::hook_award_player(Scene_Player* player, const Int_Int_Map &prop, const int kill_monster_nums) {
	return;
}

bool Move_Scene::is_scene_pass(void) {
	return is_scene_pass_;
}

bool Move_Scene::is_scene_complete(void) {
	return is_scene_complete_;
}

void Move_Scene::section_init(int scene_id, int sort, int sub_sort) {
	npc_section_.scene_ = this;
	npc_section_.is_team = scene_config()->dungeon.can_team > 0 ? true : false;
	Monster_Section_Config* s_config =
			CONFIG_CACHE_NPC->get_monster_section_config(scene_id);
	if (s_config == NULL) {
		MSG_TRACE_ABORT("scene_id config not exist (%d)", scene_id);
		return ;
	}
	Monster_Group_Config_Map::iterator g_it;
	for (g_it = s_config->group_map.begin(); g_it != s_config->group_map.end();
			++g_it) {
		NPC_Group group;
		group.config = g_it->second;
		npc_section_.group_map.insert(std::make_pair(g_it->first, group));
	}
	npc_section_.path_guide_vec = s_config->path_guide_vec;

	Monster_Point_Config_Map::iterator p_it;
	for (p_it = s_config->point_map.begin(); p_it != s_config->point_map.end();
			++p_it) {
		if (p_it->second.refresh_times == -2) { //随机刷或者不刷
			if (random() % 100 >= 50) {
				continue;
			}
		}
		NPC_Record record;
		record.config = p_it->second;
		record.scene = this;
		if (record.config.refresh_con == 1) {
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday()
					+ Time_Value(record.config.con_time / 1000,
							record.config.con_time % 1000);
		}
		if (p_it->second.point_type == REFRESH_POINT_TYPE_MONSTER_BOX) { // 随机刷怪物或者掉落物
			if (random() % 100 >= 50) { // 怪物
				record.config.point_type = REFRESH_POINT_TYPE_MONSTER;
				if (p_it->second.npc_vec.empty() == false) {
					record.config.npc_vec.clear();
					Int_Int npc_m_id = p_it->second.npc_vec[0];
					record.config.npc_vec.push_back(npc_m_id);
				}
			} else { // 掉落物
				record.config.point_type = REFRESH_POINT_TYPE_BOX;
				if (p_it->second.npc_vec.size() >= 2) {
					record.config.npc_vec.clear();
					Int_Int npc_b_id = p_it->second.npc_vec[1];
					record.config.npc_vec.push_back(npc_b_id);
				}
			}
		}else if(p_it->second.point_type == REFRESH_POINT_TYPE_MONSTER){
			if(p_it->second.monster_chance == 0){
				monster_clear_point_set_.insert(p_it->first);
			}
		}
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(p_it->first, record));
	}
}

int Move_Scene::get_revive_point(Coord &coord, int index/* = 0*/) {
	if(!scene_config()){
		return -1;
	}
	int count = scene_config()->revive_point.size();
	if(index < 0 || count <= 0 || index >= count){
		MSG_USER("error revive point scene:%d", scene_id());
		return -1;
	}
	coord = scene_config()->revive_point[index];
	return 0;
}

bool Move_Scene::is_double_income(void){
	return false;
}

void Move_Scene::hook_monster_point_clear(const int point_id){
	return;
}

void Move_Scene::be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id){
	return;
}

int Move_Scene::get_nearest_revive_point(Coord& coord, const Coord src_coord){
	if(!scene_config()){
		return -1;
	}
	int count = scene_config()->revive_point.size();
	if (count <= 0) {
		MSG_USER("error revive point scene:%d", scene_id());
		return -1;
	}
	int nearest_distance = 0;
	int nearest_index = 0;
	for(int i = 0; i < count; ++i){
		Coord coord1 = src_coord;
		Coord coord2 = scene_config()->revive_point[i];
		if(i == 0){
			nearest_distance = calculate_pixels_distance(coord1.grid_coord(),coord2.grid_coord());
			nearest_index = 0;
		}else{
			int dis = calculate_pixels_distance(coord1.grid_coord(),coord2.grid_coord());
			if(dis < nearest_distance){
				nearest_distance = dis;
				nearest_index = i;
			}
		}
	}
	coord = scene_config()->revive_point[nearest_index];
	return 0;
}

void Move_Scene::npc_init(int scene_id, int sort, int sub_sort) {
	section_init(scene_id, sort, sub_sort);
	NPC_MANAGER->section_refresh(&npc_section_);
}

void Move_Scene::npc_release(void) {
	Scene_Layer_Vec::iterator it;
	for (it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		Mover_Map tmp_mover = (*it).mover_map_;
		for (Mover_Map::iterator s_it = tmp_mover.begin();
				s_it != tmp_mover.end(); ++s_it) {
			if (s_it->second && s_it->second->npc_self()) {
				if(s_it->second->npc_self()->recover_state() == false){
					s_it->second->npc_self()->exit_move_scene();
				}
			}
		}
	}

	npc_section_.reset();
}

NPC_Group* Move_Scene::get_npc_group(int id) {
	NPC_Group_Map::iterator it = npc_section_.group_map.find(id);
	if (it != npc_section_.group_map.end()) {
		return &(it->second);
	}
	return NULL;
}

NPC_Record* Move_Scene::get_npc_record(uint64_t id) {
	NPC_Record_Map::iterator it = npc_section_.record_map.find(id);
	if (it != npc_section_.record_map.end()) {
		return &(it->second);
	}
	return NULL;
}

bool Move_Scene::add_npc_record(const uint64_t id, NPC_Record npc_record){
	NPC_Record_Map::iterator it = npc_section_.record_map.find(id);
	if (it != npc_section_.record_map.end()) {
		it->second.remove_point();
	}
	npc_record.scene = this;
	npc_record.init_share_hp();
	npc_section_.record_map[id] = npc_record;
	return true;
}

bool Move_Scene::del_npc_record(const uint64_t id){
	NPC_Record_Map::iterator it = npc_section_.record_map.find(id);
	if (it != npc_section_.record_map.end()) {
		it->second.remove_point();
		npc_section_.record_map.erase(it);
		return true;
	}
	return false;
}

int Move_Scene::tick(const Time_Value &now) {
	if (is_recover_status()) {
		return 0;
	}
	if(now > nor_tick_time_){//
		nor_tick_time_ = now + Time_Value(1);
		tick_1000ms(now);
	}

	return 0;
}

int Move_Scene::tick_1000ms(const Time_Value &now){
	check_npc_refresh(now);
	if (is_delay_show_ts_point_) {
		if (now >= ts_point_delay_time_) {
			is_delay_show_ts_point_ = false;
			is_show_ts_point_ = true;
			// 更新地图传送点
			Block_Buffer buf;
			MSG_81000040 msg;
			int count = transmit_point.size();
			for (int i = 0; i < count; ++i) {
				if (transmit_point[i].visible == 1 || transmit_point[i].visible == 2) {
					// 精英副本最后一关，不显示继续挑战
					if(this->scene_type() == Elite_Scene_Type){
						const Scene_Config *des_scene_config =
								CONFIG_CACHE_SCENE->scene_config_cache(transmit_point[i].des_scene_id);
						if(des_scene_config){
							msg.transmit_point.push_back(transmit_point[i]);
						}
					}else{
						msg.transmit_point.push_back(transmit_point[i]);
					}
				}
			}
			buf.make_message(msg.msg_id);
			msg.serialize(buf);
			buf.finish_message();
			this->broadcast_all(0, buf);
		}
	}
	return 0;
}

int Move_Scene::set_create_scene_args(
		const Create_Scene_Args &create_scene_args) {
	create_scene_args_ = create_scene_args;
	return 0;
}

bool Move_Scene::is_recover_status(void) {
	return scene_status_ == SCENE_RECOVER_STATUS;
}

bool Move_Scene::is_recover_time(Time_Value &now) {
	return now > recover_time_;
}

void Move_Scene::set_scene_status(int status) {
	scene_status_ = status;
}

int Move_Scene::get_scene_status() {
	return scene_status_;
}

int Move_Scene::scene_type(void) {
	if (scene_config()) {
		return scene_config()->type;
	}
	return 0;
}
void Move_Scene::save_battle_plot_date(std::string plot_id, int plot_step,
		int record_id) {
	return;
}

void Move_Scene::check_npc_refresh(const Time_Value &now) {
	NPC_Record_Map::iterator it = this->npc_section_.record_map.begin();
	for (; it != this->npc_section_.record_map.end(); ++it) {
		it->second.tick();
		if (it->second.check_refresh) {
			if (now >= it->second.birth_freq) {
				NPC_MANAGER->record_refresh(&(it->second));
			}
		}
	}
}

NPC_Section& Move_Scene::npc_section(void) {
	return this->npc_section_;
}

void Move_Scene::refresh_monster_for_debug(Coord& bird_coord, int npc_type_id,
		role_id_t role_id) {
	int index_debug = DEBUG_MONSTER_POINT_ID;
	uint64_t pt_id = NPC_MANAGER->get_own_monster_point_id(index_debug,
			role_id);
	NPC_Record_Map::iterator it_r = npc_section_.record_map.find(pt_id);
	if (it_r != npc_section_.record_map.end()) {
		if (it_r->second.is_fighting()) {
			return;
		}
		it_r->second.clear_point();
		npc_section_.record_map.erase(it_r);
	}
	NPC_Group_Map::iterator it_g = npc_section_.group_map.find(index_debug);
	if (it_g != npc_section_.group_map.end()) {
		npc_section_.group_map.erase(it_g);
	}

	NPC_Record record;

	Monster_Point_Config point_config;
	point_config.id = index_debug;
	point_config.point_type = REFRESH_POINT_TYPE_MONSTER;
	//point_config.mult_type = 1;
	point_config.type = 0;
	point_config.refresh_times = 1;
	point_config.scene_key = this->scene_id();
	point_config.speed[0] = 100;
	point_config.speed[1] = 100;

	point_config.birth_coord = bird_coord;

	Int_Int int_group;
	int_group.val_1 = index_debug;
	int_group.val_2 = 1000;
	point_config.group_vec.push_back(int_group);
	Int_Int npc;
	npc.val_1 = npc_type_id;
	npc.val_2 = 0;
	point_config.npc_vec.push_back(npc);

	record.scene = this;
	record.config = point_config;

	NPC_Group group;
	Monster_Group_Config group_config;
	group_config.id = index_debug;
	group_config.monster.push_back(npc_type_id);
	group_config.pos.push_back(5);
	group_config.scene_key = this->scene_id();
	group_config.wave.push_back(1);
	group.config = group_config;
	record.init_share_hp();
	npc_section_.record_map.insert(std::make_pair(pt_id, record));
	npc_section_.group_map.insert(std::make_pair(index_debug, group));

	NPC_Record* own_point;
	own_point = get_npc_record(pt_id);
	if (own_point) {
		NPC_MANAGER->record_refresh(own_point);
	}
}

int Move_Scene::refresh_monster_point(int point_id, role_id_t role_id) {
	NPC_Record* point = get_npc_record(point_id);
	if (!point) {
		return -1;
	}
	if (point->config.mult_type == 1) {
		uint64_t pt_id = NPC_MANAGER->get_own_monster_point_id(point_id,
				role_id);
		NPC_Record* own_point;
		own_point = get_npc_record(pt_id);
		if (own_point) {
			own_point->clear_point();
			NPC_MANAGER->record_refresh(own_point);
		} else {
			NPC_Record record;
			record.config = point->config;
			record.scene = this;
			if (record.config.refresh_con == 1) {
				record.check_refresh = true;
				record.birth_freq = Time_Value::gettimeofday()
						+ Time_Value(record.config.con_time / 1000,
								record.config.con_time % 1000);
			}
			record.birth_by = role_id;
			record.init_share_hp();
			std::pair<NPC_Record_Map::iterator, bool> insert_it;
			insert_it = npc_section_.record_map.insert(
					std::make_pair(pt_id, record));
			if (insert_it.second) {
				NPC_Record* ref_record = &(insert_it.first->second);
				NPC_MANAGER->record_refresh(ref_record);
			}
		}
	} else {
		NPC_MANAGER->record_refresh(point);
	}
	return 0;
}

void Move_Scene::set_no_profit_player(role_id_t role_id) {
	no_profit_player_set_.insert(role_id);
}

bool Move_Scene::is_no_profit(role_id_t role_id) {
	return no_profit_player_set_.count(role_id);
}

void Move_Scene::reset_delay_show_ts_point_time(void) {
	if(is_delay_show_ts_point_){
		is_delay_show_ts_point_ = false;
		is_show_ts_point_ = true;
		ts_point_delay_time_ = Time_Value::zero;
		// 更新地图传送点
		Block_Buffer buf;
		MSG_81000040 msg;
		int count = transmit_point.size();
		for (int i = 0; i < count; ++i) {
			if (transmit_point[i].visible == 1 || transmit_point[i].visible == 2) {
				// 精英副本最后一关，不显示继续挑战
				if(this->scene_type() == Elite_Scene_Type){
					const Scene_Config *des_scene_config =
							CONFIG_CACHE_SCENE->scene_config_cache(transmit_point[i].des_scene_id);
					if(des_scene_config){
						msg.transmit_point.push_back(transmit_point[i]);
					}
				}else{
					msg.transmit_point.push_back(transmit_point[i]);
				}
			}
		}
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		this->broadcast_all(0, buf);
	}
}

void Move_Scene::init_player_own_point(role_id_t role_id) {
	std::vector<NPC_Record> new_record;
	new_record.clear();
	for (NPC_Record_Map::iterator it = npc_section_.record_map.begin();
			it != npc_section_.record_map.end(); ++it) {
		if (it->second.config.mult_type == 1 && it->second.birth_by == 0) { // 多人宝箱场景初始时不刷，每个玩家进入后再刷一个
			NPC_Record record;
			record.config = it->second.config;
			record.scene = this;
			if (record.config.refresh_con == 1) {
				record.check_refresh = true;
				record.birth_freq = Time_Value::gettimeofday()
						+ Time_Value(record.config.con_time / 1000,
								record.config.con_time % 1000);
			}
			record.birth_by = role_id;
			if (record.config.refresh_times != -1) {
				new_record.push_back(record);
			}

		}
	}
	std::vector<NPC_Record>::iterator it;
	for (it = new_record.begin(); it != new_record.end(); ++it) {
		uint64_t pt_id = NPC_MANAGER->get_own_monster_point_id(it->config.id,
				role_id);
		std::pair<NPC_Record_Map::iterator, bool> insert_it;
		insert_it = npc_section_.record_map.insert(std::make_pair(pt_id, *it));
		if (insert_it.second) {
			NPC_Record* ref_record = &(insert_it.first->second);
			NPC_MANAGER->record_refresh(ref_record);
		}
	}
}

bool Move_Scene::is_monster_point_all_clear(void){
	return this->monster_clear_point_set_.empty()?true:false;
}

void Move_Scene::get_scene_param(int& param1, int& param2, int& param3){
	param1 = scene_param_1_;
	param2 = scene_param_2_;
	param3 = scene_param_3_;
}

void Move_Scene::set_scene_param(const int param1, const int param2, const int param3, bool to_client/* = false*/){
	scene_param_1_ = param1;
	scene_param_2_ = param2;
	scene_param_3_ = param3;
	if(to_client){
		MSG_81000019 msg;
		msg.status = get_pass_status();
		msg.value1 = scene_param_1_;
		msg.value2 = scene_param_2_;
		msg.value3 = scene_param_3_;
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		broadcast_all_within_battle(0, buf);
	}
}

int Move_Scene::get_pass_status(void) {
	return pass_status_;
}

void Move_Scene::set_pass_status(int value, bool to_client/* = false*/) {
	pass_status_ = value;
	if(to_client){
		MSG_81000019 msg;
		msg.status = this->get_pass_status();
		msg.value1 = scene_param_1_;
		msg.value2 = scene_param_2_;
		msg.value3 = scene_param_3_;
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		broadcast_all_within_battle(0, buf);
	}
}

void Move_Scene::add_battle_scene_id(role_id_t battle_id){
	battle_id_set_.insert(battle_id);
}

void Move_Scene::remove_battle_scene_id(role_id_t battle_id){
	battle_id_set_.erase(battle_id);
}

void Move_Scene::battle_scene_release(void){
	Role_Id_Set tmp_set;
	tmp_set = battle_id_set_;
	Role_Id_Set::iterator it;
	for(it = tmp_set.begin(); it != tmp_set.end(); ++it){
		Battle_Scene* battle_scene = GLOBAL_SCENE_MANAGER->find_battle_scene(*it);
		if(battle_scene){
			battle_scene->force_to_recycle_battle_scene();
		}
	}
	battle_id_set_.clear();
}
