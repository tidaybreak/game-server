/*
 * NPC_Struct.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#include "NPC_Struct.h"
#include "NPC.h"
#include "monster/Monster.h"
#include "Config_Cache_NPC.h"
#include "NPC_Manager.h"
#include "Move_Scene.h"
#include "Scene_Player.h"
#include "NPC_Config.h"
#include "Battle_Scene.h"
#include "Global_Scene_Manager.h"
#include "expedition_scene/Expedition_Scene.h"

Walk_Path::Walk_Path(void) : curr_path_index(0), curr_coord_index(0),
	move_gride_count(0)
{

}

void Walk_Path::reset(void) {
	walk_path.clear();
	curr_path_index = 0;
	walk_coord.clear();
	curr_coord_index = 0;
	move_gride_count = 0;

}

bool Walk_Path::has_walk_path(void) {
	if (walk_path.empty()) return false;

	return true;
}


int Walk_Path::get_next_path(Coord &coord) {
	int count = walk_path.size();
	if (curr_path_index >= count) {
		return -1;
	}

	coord = walk_path[curr_path_index];
	curr_path_index++;
	return 0;
}

int Walk_Path::get_loop_path(Coord &coord) {
	if (walk_path.empty()) return 0;
	int count = walk_path.size();
	if (curr_path_index >= count - 1) {
		reverse(walk_path.begin(),walk_path.end());
		curr_path_index = 0;
	}

	coord = walk_path[curr_path_index];
	curr_path_index++;
	return 0;
}

int Walk_Path::get_last_path(Coord &coord) {
	if (walk_path.size() == 0) {
//		MSG_USER("ERROR_9 ai");
		return 0;
	}
	int count = walk_path.size();
	if (curr_path_index >= count - 1) {
		coord = walk_path.back();
		return 0;
	}

	coord = walk_path[curr_path_index];
	curr_path_index++;
	return 0;
}

bool Walk_Path::has_walk_coord(void) {
	if (!walk_coord.empty() && curr_coord_index > 0) return true;
	return false;
}

bool Walk_Path::is_arrived(void) {
	if (curr_coord_index == 0) return true;
	return false;
}

int Walk_Path::clean_walk_coord(void) {
	walk_coord.clear();
	curr_coord_index = 0;
	move_gride_count = 0;
	return 0;
}

int Walk_Path::get_next_coord(Coord &coord) {
	if(curr_coord_index < 1){
		return -1;
	}
	coord = walk_coord[curr_coord_index - 1];
	curr_coord_index--;
	move_gride_count++;
	if (curr_coord_index == 0)
		clean_walk_coord();

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//NPC_Addition
NPC_Addition_Info::NPC_Addition_Info(void) {
	reset();
};

void NPC_Addition_Info::reset(void) {
	name = "";
	jid = 0;
	toward = 0;
	default_skill = 0;
	layer_id = 0;
	birth_by_skill = 0;
	birth_coord.reset();
	level = 0;
	ex_val1 = 0;
	ex_val2 = 0;
	unmoveable_birth = false;
	ext_skill_set.clear();
	prop_vec.clear();
//	additional_id.clear();
//	identify.reset();
//	additional_info.clear();
//	for (Fighter_Detail::Property::iterator it = property.begin(); it != property.end(); ++it)
//		it->clear();
}


//////////////////////////////////////////////////////////////////////////

void NPC_Config_Cache::reset(void) {
	id = 0;
	name = "";
	level = 0;
	title_id = 0;
	jid = 0;
	type = 0;
	m_type = 0;
	race.clear();
	prop_value_vec.clear();
	skill_vec.clear();
	passive_skills.clear();
	battle_ai_vec.clear();
	drop_type = 0;
	drop_mult_mode = 0;
	drop_mult_coe = 0;
	drop_lv_limit = 0;
	drop_lv_coe = 0;
	drops_vec.clear();
	money = 0;
	exp = 0;
	soul = 0;
	res_mult_coe = 0;
	res_lv_limit = 0;
	res_lv_coe = 0;
	chat_vec.clear();

//	s_type = 0;
//	speed = 0;
//	camps = 0;
//	appear_effect = -1;
//	disappear_effect = -1;
//	default_skill = 0;
//	skill_pause_time = Time_Value::zero;
//	broadcast = 0;
//	buff_set.clear();
//	die_drops.clear();
//	trigger_set.clear();
//	die_by_attack_nums.clear();
//	ext_skill_set.clear();
//	die_no_exp = false;
//	die_drop_type = false;
//	invincible = false;
//	die_repel = true;
//	push_pull = true;
//	exp_attenuate = 0;
//	is_skill_deplete = false;
//	experience = 0.0;
////	property.resize(Fighter_Detail::PROPERTY_TYPE_END);
////	for (Fighter_Detail::Property::iterator it = property.begin(); it != property.end(); ++it)
////		it->resize(Fighter_Detail::ADDITION_TYPE_END);
////	for (Fighter_Detail::Property::iterator it = property.begin(); it != property.end(); ++it) {
////		for (Fighter_Detail::Property_Value::iterator pit = it->begin(); pit != it->end(); ++pit) {
////			(*pit).reset();
////		}
////	}
//
//	collect_times = -1;
//	times_per_player = 0;
//	collect_level = 0;
//	need_item_id = 0;
//	collect_jid = 0;

}


void NPC_Detail::reset(void) {
	npc_id = 0;
	npc_type_id = 0;
	title_id = 0;
	operation_type = 0;
	operation_state = 0;
	state_type = 0;
	npc_status = 0;
	team_pos = 0;
	team_count = 0;
	master_master = 0;
	birth_time = Time_Value::gettimeofday();
	life_time.set(INT_MAX, 0);
	birth_coord.reset();
	chase_coord.reset();
	direct_master = 0;
	birth_by_skill = 0;
	move_speed = 0;
	talk_vec.clear();
	chat_vec.clear();
	add_id.clear();
	///  AI
	ai_id = 0;
	//ai_config = 0;
	//watcher_tick.clear();
	is_ai_start = true;
	ai_running_info.reset();

	master_name = "";
	fighter_detail_.reset();
	mover_detail_.reset();
	check_ai_tick = false;
	ai_tick_time = Time_Value::zero;
}

Monster_Detail::Monster_Detail(void) {
	reset();
}

void Monster_Detail::reset(void) {
	name = "";
	master_name = "";
	monster_status = 0;
	master_role_id = 0;
	caller_id = 0;
	jid = 0;
	team_hatred_type = 0;
//	additional_id.clear();
//	hurt_export.clear();
	drop_coord.reset();
//	default_skill = 0;
//	current_skill = 0;
	speed[0] = 0;
	speed[1] = 0;
	speed[2] = 0;
	config_cache = 0;
}

void Material_Detail::reset(void) {
	jid = 0;
	name = "";
	lv_limit = 0;
	gather_time = 0;
	rate = 0;
	drops_vec.clear();
	tasks_vec.clear();
}

void Machine_Detail::reset(void){
	jid = 0;
	name = "";
	config_cache = NULL;
}

Drop_One::Drop_One(void) {
	reset();
}

void Drop_One::reset(void) {
	item = 0;
	amount = 0;
	bind = 0;
	announce_type = 0;
	announce_item.clear();
}

Drops_Detail::Drops_Detail(void)
{
	reset();
};

void Drops_Detail::reset(void) {
	open = false;
	drop_type = 0;
	name = "";
	type = 0;
	jid = 0;
//	amount = 0;
//	item = 0;
//	index = 0;
//	visible = 0;
//	belong = 0;
//	collect_time = 0;
//	bind = 0;

	drops_vec.clear();
//	get_limit = 0;
	drops_status = DROPS_STATUS_NORMAL;
//	get_roles.clear();
	auto_owner = 0;

	role_drop_map.clear();
}

void Drops_Detail::set_normal_status(void) {
	drops_status = DROPS_STATUS_NORMAL;
}

void Drops_Detail::set_getting_status(void) {
	drops_status = DROPS_STATUS_GETTING;
}

Drops_Detail::Drops_Status Drops_Detail::get_status(void) {
	return drops_status;
}


/////////////////////////////////////////////////////////////////////////
//
//NPC_Record_Info::NPC_Record_Info(void) {
//	reset();
//};
//
//void NPC_Record_Info::reset(void) {
//	id = 0;
//	scene_id = 0;
//	type = 0;
//	view_range = 0;
//	move_range = 0;
//	chase_range = 0;
//	speed[0] = 0;
//	speed[1] = 0;
//	bird_coord.reset();
//	vec_npc.clear();
//}

NPC_Group::NPC_Group(void) {
	reset();
}

void NPC_Group::reset(void)
{
	config.reset();
}

NPC_Record::NPC_Record(void) {
	reset();
}

void NPC_Record::reset(void) {
	config.reset();

	max_count = 0;
	cur_ref_times = 0;
	cur_ref_time = Time_Value::zero;

	cur_gt_times = 0;
	cur_gt_cd = Time_Value::zero;

	birth_by = 0;
	birth_freq = Time_Value::zero;
	birth_time = Time_Value::zero;
	layer_id = 0;

	scene = 0;
	battle_scene = 0;

	alive_npc_map.clear();
	leader_id = 0;
	check_refresh = false;
	fight_player_set.clear();

	// share hp
	share_hp_cur = 0;
	share_hp_max = 0;
	share_hp_tick = 0;
	battle_id_set.clear();
}

int NPC_Record::get_npc_num(void){
	return alive_npc_map.size();
}

void NPC_Record::reset_scene(Move_Scene* scene_) {
//	if (NULL == scene_) return;
//	scene = scene_;
//
//	for (NPC_Type_Map::iterator it_npc = npc_type_map.begin(); it_npc != npc_type_map.end(); ++it_npc) {
//		for (NPC_Map::iterator it = it_npc->second.alive_npc_map.begin(); it != it_npc->second.alive_npc_map.end(); ++it) {
//			it->second->reset_scene(scene_);
//		}
//	}

}

void NPC_Record::tick(void){
	if(!is_share_hp()){
		return;
	}
	if(share_hp_cur <= 0){
		return;
	}
	if(share_hp_tick > 3){
		share_hp_tick = 0;
		refresh_share_hp();
	}else{
		++share_hp_tick;
	}
}

bool NPC_Record::is_share_hp(void){
	if(config.mult_type == 200){
		return true;
	}
	return false;
}

void NPC_Record::init_share_hp(void){
	if(!is_share_hp()){
		return;
	}
	if(config.npc_vec.empty()){
		return;
	}
	int monster_type_id = config.npc_vec[0].val_1;
	const NPC_Config_Cache* cfg_npc = CONFIG_CACHE_NPC->find_npc_config_cache(monster_type_id);
	if(!cfg_npc){
		return;
	}
	int count = cfg_npc->prop_value_vec.size();
	for(int i = 0; i < count; ++i){
		if(cfg_npc->prop_value_vec[i].id == 506205){
			share_hp_cur = cfg_npc->prop_value_vec[i].value;
			share_hp_max = cfg_npc->prop_value_vec[i].value;
			break;
		}
	}
}

void NPC_Record::refresh_share_hp(void){
	Role_Id_Set::iterator it;
	for(it = battle_id_set.begin(); it != battle_id_set.end(); ++it){
		Battle_Scene* battle_scene = GLOBAL_SCENE_MANAGER->find_battle_scene(*it);
		if(battle_scene){
			battle_scene->set_world_boss_blood(share_hp_cur);
		}
	}
}

void NPC_Record::be_hurt_and_sub_blood(double sub_blood, double current_blood, role_id_t attacker_id){
	 if(!is_share_hp()){
		 return;
	 }
	 if(share_hp_cur <= 0){
		 return;
	 }
	share_hp_cur -= sub_blood;
	if(share_hp_cur <= 0){
		share_hp_cur = 0;
		refresh_share_hp();
	}
	//win
}

int NPC_Record::remove_npc_from_alive(int npc_type_id, role_id_t npc_id) {
	alive_npc_map.erase(npc_id);
	return 0;
}

void NPC_Record::record_recover_all_npc(void) {
	for (RoleID_NPC_Map::iterator it = alive_npc_map.begin(); it != alive_npc_map.end(); ++it)
	{
		it->second->recover_self();
	}
	//alive_npc_map.clear();	// 正常已清,保险下
}

void NPC_Record::record_normal_die_all_npc(void) {
	for (RoleID_NPC_Map::iterator it = alive_npc_map.begin(); it != alive_npc_map.end(); ++it)
	{
			it->second->normal_die_self();		// 正常已清,保险下
	}
	//alive_npc_map.clear();
}

void NPC_Record::npc_add_movevalue(int move_value) {
//	for (NPC_Type_Map::iterator it_npc = npc_type_map.begin(); it_npc != npc_type_map.end(); ++it_npc) {
//		for (RoleID_NPC_Map::iterator it = it_npc->second.alive_npc_map.begin(); it != it_npc->second.alive_npc_map.end(); ++it) {
//			it->second->mover_detail().add_moveable_value(move_value);
//		}
//	}
}

void NPC_Record::npc_add_hatred(role_id_t fighter_id, double hurt_export) {
//	for (NPC_Type_Map::iterator it_npc = npc_type_map.begin(); it_npc != npc_type_map.end(); ++it_npc) {
//		for (RoleID_NPC_Map::iterator it = it_npc->second.alive_npc_map.begin(); it != it_npc->second.alive_npc_map.end(); ++it) {
//			it->second->add_new_hatred(fighter_id, hurt_export);
//		}
//	}
}

void NPC_Record::npc_add_hatred(int npc_type_id, role_id_t fighter_id, double hurt_export) {
//	NPC_Type_Map::iterator it_npc = npc_type_map.find(npc_type_id);
//	if (it_npc != npc_type_map.end()) {
//		for (RoleID_NPC_Map::iterator it = it_npc->second.alive_npc_map.begin(); it != it_npc->second.alive_npc_map.end(); ++it) {
//			it->second->add_new_hatred(fighter_id, hurt_export);
//		}
//	}
}

int NPC_Record::get_team_hatred_type(int npc_type_id) {
//	if (team_hatred) return 1;
//
//	NPC_Type_Map::iterator it_npc = npc_type_map.find(npc_type_id);
//	if (it_npc != npc_type_map.end()) {
//		if (it_npc->second.team_hatred) return 2;
//	}
	return 0;
}

int NPC_Record::get_battle_group(){
	int rate = random()%1000;
	int count = this->config.group_vec.size();
	int rate_total = 0;
	for(int i=0; i<count;++i){
		rate_total += this->config.group_vec[i].val_2;
		if(rate<rate_total){
			return config.group_vec[i].val_1;
		}
	}
	if (0 == count) return 0;
	return config.group_vec[random()%count].val_1;
}

void NPC_Record::remove_point(const role_id_t battle_id/* = 0*/){
	if(this->scene == 0){
		return;
	}
	if(battle_scene){
		battle_scene->force_finish_battle();
		battle_scene = NULL;
	}

	{
		Role_Id_Set::iterator it;
		for(it = battle_id_set.begin(); it != battle_id_set.end(); ++it){
			Battle_Scene* battle_scene = GLOBAL_SCENE_MANAGER->find_battle_scene(*it);
			if(battle_scene){
				battle_scene->force_finish_battle();
			}
		}
		battle_id_set.clear();
	}


	for(RoleID_NPC_Map::iterator npc_it = alive_npc_map.begin();
			npc_it != alive_npc_map.end(); ++npc_it){
		NPC* npc = npc_it->second;
		if(npc){
			npc->exit_move_scene();
		}
	}
	alive_npc_map.clear();
	fight_player_set.clear();
	this->set_refresh();
	init_share_hp();
}

void NPC_Record::clear_point(const role_id_t battle_id/* = 0*/){
	if(this->scene == 0){
		return;
	}
	if(battle_scene){
		battle_scene = NULL;
	}
	fight_player_set.clear();

	if(is_share_hp()){
		Role_Id_Set::iterator it = battle_id_set.find(battle_id);
		if(it != battle_id_set.end()){
			battle_id_set.erase(it);
		}
	}
	if(battle_id_set.empty() == false){
		return;
	}
	this->scene->npc_section().push_clear_point(this->config.id);
	for(RoleID_NPC_Map::iterator npc_it = alive_npc_map.begin();
			npc_it != alive_npc_map.end(); ++npc_it){
		NPC* npc = npc_it->second;
		if(npc){
			npc->exit_move_scene();
		}
	}
	alive_npc_map.clear();
	cur_gt_times = 0;

	if (get_scene_type(this->scene->scene_id()) == Expedition_Type) {
		Expedition_Scene *es = (Expedition_Scene*)scene;
		es->del_refreshed_monster(this->config.id);
	}

	this->set_refresh();
	init_share_hp();
}

void NPC_Record::reset_point(const role_id_t battle_id/* = 0*/){
	fight_player_set.clear();
	battle_scene = NULL;

	if(is_share_hp()){
		Role_Id_Set::iterator it = battle_id_set.find(battle_id);
		if(it != battle_id_set.end()){
			battle_id_set.erase(it);
		}
	}
	if(battle_id_set.empty() == false){
		return;
	}
	set_npc_status(NPC::NORMAL_STATUS);
}

int NPC_Record::enter_battle(Scene_Player* player){
	if(this->config.start_plot != ""){
		set_npc_status(NPC::PLOT_STATUS);
		//send to client
		player->play_plot(this->config.start_plot, this->config.id);
	}else{
		create_battle(player);
	}
	return 0;
}

int NPC_Record::create_battle(Scene_Player* player){
	Battle_Scene* bt_scene = NPC_MANAGER->create_battle(player, this);
	if(bt_scene){
		if(is_share_hp()){
			if(battle_id_set.empty()){
				set_npc_status(NPC::FIGHT_STATUS);
			}
			battle_id_set.insert(bt_scene->battle_id());
		}else{
			battle_scene = bt_scene;
			set_npc_status(NPC::FIGHT_STATUS);
		}
		return 0;
	}else{
		return -1;
	}
}

int NPC_Record::join_battle(Fighter* fighter, int pos1, int pos2){
	if (battle_scene) {
		if(fighter->set_battle_position(pos1, pos2, battle_scene) == 0){
			if (fighter->player_self()) {
				Position_Key player_pos(pos1, pos2);	// 主角位置
				//根据英雄位置作调整
				battle_scene->init_battle_hero_info(fighter->player_self(), player_pos);
			}
			fighter->enter_battle_scene(battle_scene);

//			// 拉队员进入
//			if (fighter->player_self()) {
//				fighter->player_self()->enter_battle_scene_team_member(battle_scene);
//			}
		}
	}
	return 0;
}

Coord NPC_Record::grid_coord(){
	RoleID_NPC_Map::iterator it = alive_npc_map.begin();
	if(it != alive_npc_map.end()){
		return it->second->mover_detail().grid_coord;
	}
	return config.birth_coord.grid_coord();
}

void NPC_Record::set_npc_status(int status){
	RoleID_NPC_Map::iterator it = alive_npc_map.begin();
	for(; it != alive_npc_map.end(); ++it){
		it->second->set_npc_status(status);
	}
}

void NPC_Record::set_chase_target(role_id_t role_id){
	RoleID_NPC_Map::iterator it = alive_npc_map.begin();
	for(; it != alive_npc_map.end(); ++it){
		if(it->second->monster_self()){
			it->second->monster_self()->set_chase_target(role_id);
		}
	}
}

void NPC_Record::clear_chase_target(void){
	RoleID_NPC_Map::iterator it = alive_npc_map.begin();
	for(; it != alive_npc_map.end(); ++it){
		if(it->second->monster_self()){
			it->second->monster_self()->clear_chase_target();
		}
	}
}

void NPC_Record::set_refresh(void){
	if(config.refresh_times == 0 ||
			(config.refresh_times>0 && cur_ref_times < config.refresh_times)
		)
	{
		birth_freq = Time_Value::gettimeofday() + Time_Value(config.refresh_time,0);
		check_refresh = true;
	}
	//
	if(this->scene && config.after_point != 0){
		NPC_Record* record = this->scene->get_npc_record(config.after_point);
		if(record){
			record->birth_freq = Time_Value::gettimeofday() +
					Time_Value(record->config.after_time/1000,record->config.after_time%1000);
			record->check_refresh = true;
		}
	}
}

void NPC_Record::set_cur_gt_cd(const Time_Value &now){
	if(this->config.gt_cd != 0){
		this->cur_gt_cd = now + Time_Value(this->config.gt_cd, 0);
	}
}

bool NPC_Record::check_gt_cd(const Time_Value &now){
	if(this->config.gt_cd == 0){
		return true;
	}
	return now >= this->cur_gt_cd;
}

bool NPC_Record::is_teams_up(void){
	if(this->scene){
		return this->scene->npc_section().is_teams_up();
	}
	return false;
}

void NPC_Record::hook_fighter_enter(Fighter* fighter){
	fight_player_set.insert(fighter);
}

void NPC_Record::hook_fighter_leave(Fighter* fighter){
	fight_player_set.erase(fighter);
}

bool NPC_Record::is_fighting(void){
	return fight_player_set.empty()?false:true;
}

int NPC_Record::release(void){
	return 0;
}

//////////////////////////////////////////////////////////////////////////

NPC_Section::NPC_Section(void) : scene_(0){
	reset();
}

void NPC_Section::reset(void) {
	scene_ = 0;
	record_map.clear();
	group_map.clear();
	is_team = false;
	path_guide_vec.clear();
	clear_point_vec.clear();
}

void NPC_Section::reset_scene(Move_Scene* scene) {
	if (NULL == scene) return;
	scene_ = scene;
//	for (Record_Map::iterator it = record_map_.begin(); it != record_map_.end(); ++it)
//	{
//		//it->second->reset_scene(scene_);
//	}
}

void NPC_Section::insert_record(uint64_t index, NPC_Record& record) {
	NPC_Record_Map::iterator find_it = record_map.find(index);
	if (find_it != record_map.end()) {
		find_it->second.clear_point();
		record_map.erase(find_it);
	}
	record_map.insert(std::make_pair(index, record));
}

void NPC_Section::insert_group(int index, NPC_Group& group){
	NPC_Group_Map::iterator find_it = group_map.find(index);
	if (find_it != group_map.end()) {
		group_map.erase(find_it);
	}
	group_map.insert(std::make_pair(index, group));
}

void NPC_Section::collect_npc_by_record_type(RoleID_NPC_Map &roleid_npc, int record_type) {
	roleid_npc.clear();

//	for (Record_Map::iterator section_it = record_map_.begin(); section_it != record_map_.end(); ++section_it) {
//		if (record_type && record_type != section_it->first) continue;
//		for (RoleID_NPC_Map::iterator it = section_it->second->alive_npc_map.begin(); it != section_it->second->alive_npc_map.end(); ++it)
//		{
//			roleid_npc.insert(std::make_pair(it->first, it->second));
//		}
//	}
}

void NPC_Section::collect_npc_by_type_id(RoleID_NPC_Map &roleid_npc, int npc_type_id) {
	roleid_npc.clear();
//	for (Record_Map::iterator section_it = record_map_.begin(); section_it != record_map_.end(); ++section_it) {
//		for (RoleID_NPC_Map::iterator it = section_it->second->alive_npc_map.begin(); it != section_it->second->alive_npc_map.end(); ++it)
//		{
//			if(it->second->npc_type_id() == npc_type_id)
//			{
//				roleid_npc.insert(std::make_pair(it->first, it->second));
//			}
//		}
//	}
}

void NPC_Section::push_clear_point(int point){
	if(this->is_clear_point(point) == false){
		this->clear_point_vec.push_back(point);
	}
}

bool NPC_Section::is_clear_point(int point){
	bool exist = false;
	std::vector<int>::iterator it;
	for(it = path_guide_vec.begin(); it != path_guide_vec.end(); ++it){
		if(point == (*it)){
			exist = true;
			break;
		}
	}
	if(!exist){
		return true;
	}
	for(it = clear_point_vec.begin(); it != clear_point_vec.end(); ++it){
		if(point == (*it)){
			return true;
		}
	}
	return false;
}

int NPC_Section::get_next_path_guide(void){
	if(this->path_guide_vec.empty()){
		return -1;
	}
	if(this->clear_point_vec.empty()){
		return this->path_guide_vec[0];
	}else{
		int clear_count = this->clear_point_vec.size();
		int count = this->path_guide_vec.size();
		if(clear_count == count ){
			return 0;
		}
		int last_clear_index = 0;
		for(int i=0; i<count; ++i){
			if(this->clear_point_vec[clear_count-1] == this->path_guide_vec[i]){
				last_clear_index = i;
				break;
			}
		}
		int index = 0;
		int point = 0;
		for(int i = 0; i<count; ++i){
			index = (last_clear_index + i)%count;
			point = this->path_guide_vec[index];
			if(is_clear_point(point) == false){
				return point;
			}
		}
	}
	return 0;
}

int NPC_Section::get_monster_point_count(void){
	int count = 0;
	NPC_Record_Map::iterator it;
	for(it = record_map.begin(); it != record_map.end(); ++it){
		if( it->second.config.point_type == REFRESH_POINT_TYPE_MONSTER){
			count += 1;
		}
	}
	return count;
}

bool NPC_Section::is_teams_up(void){
	return is_team;
}

//////////////////////////////////////////////////////////////////

Material_Point::Material_Point(void){
	reset();
}

void Material_Point::reset(void){
	config_info.reset();
	cur_ref_times = 0;
	ref_time = Time_Value::zero;
	birth_freq = Time_Value::zero;
}

//////////////////////////////////////////////////////////////////

Material_Section::Material_Section(void){
	reset();
}

void Material_Section::reset(void){
	material_point_map.clear();
}

//////////////////////////////////////////////////////////////////

Machine_Point::Machine_Point(void){
	reset();
}

void Machine_Point::reset(void){
	config.reset();
	ref_time = Time_Value::zero;
	birth_freq = Time_Value::zero;
	tri_time = Time_Value::zero;
	cur_tri_times = 0;
	cur_ref_times = 0;
}

//////////////////////////////////////////////////////////////////

Machine_Section::Machine_Section(void){
	reset();
}

void Machine_Section::reset(void){
	point_map.clear();
}

//////////////////////////////////////////////////////////////////



Monster_AI_Info::Monster_AI_Info(void){
	reset();
}

Monster_AI_Info::Monster_AI_Info(const Monster_AI_Info& info){
	config = info.config;
	cur_Times = info.cur_Times;
	last_time = info.last_time;
	is_valid = info.is_valid;
	is_recover = info.is_recover;
	has_trigger = info.has_trigger;
}
void Monster_AI_Info::reset(void){
	config = NULL;
	cur_Times = 0;
	last_time = Time_Value::zero;
	valid_time = Time_Value::zero;
	is_valid = false;
	is_recover = false;
	has_trigger = false;
}

void Monster_AI_Info::trigger(const Time_Value &now){
	if(config){
		has_trigger = true;
		last_time = now;
		if(config->times != 0){
			cur_Times += 1;
			if(cur_Times >= config->times){
				is_recover = true;
			}
		}
	}
}

bool Monster_AI_Info::can_trigger(void){
	if(is_recover == false && config){
		if(config->cd == 0 || (config->cd > 0 &&
				Time_Value::gettimeofday() >= last_time + Time_Value(config->cd, 0))){
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////
