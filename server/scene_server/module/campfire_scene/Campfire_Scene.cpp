/*
 * Campfire_Scene.cpp
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#include "Campfire_Scene.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_Secret_Shop.h"
#include "Scene_Player.h"
#include "Msg_Active_Struct.h"
#include "NPC/monster/Monster.h"
#include "Config_Cache_Dungeon.h"
#include "Err_Code.h"
#include "Config_Cache_NPC.h"
#include "campfire/Campfire_Data_Manager.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "status/Status.h"
#include "status/Status_Struct.h"
#include "announcement/Annoucement.h"
#include "announcement/Announcement_Struct.h"
#include "chater/Chater_Def.h"
#include "gang/Gang_Manager.h"
#include "campfire/Campfire_Data_Manager.h"
#include "Msg_Campfire_Struct.h"
#include "DB_Manager.h"

const int FIRE_EXP_BUF_ID = 34401001;
const int FIRE_FS_BUF_ID = 34401002;
const int ANNOUNCE_ID_BONFIRE_LEVEL_UP = 1320;
const int ANNOUNCE_ID_BONFIRE_FIRE = 1321;

const int ANNOUNCE_ID_BONFIRE_LIT = 1396;
const int ANNOUNCE_ID_BONFIRE_ADD_WOOD = 1397;

Campfire_Scene::Campfire_Scene() :
		act_key_(0), exp_time_(0), dancing_time_(0) {
	// TODO Auto-generated constructor stub

}

Campfire_Scene::~Campfire_Scene() {
	// TODO Auto-generated destructor stub
}

void Campfire_Scene::reset(void) {
	Move_Scene::reset();
	meat_cfg_.reset();
	over_time_ = Time_Value::zero;
	fire_point_vec_.clear();
	fire_gain_player_map_.clear();
	dancing_player_map_.clear();
	act_key_ = 0;
	exp_time_ = 0;
	dancing_time_ = 0;
	matchman_point_cfg_.clear();
	matchman_map_.clear();
	goddess_cfg_.reset();
	goddess_killer_set_.clear();
}

int Campfire_Scene::tick(const Time_Value &now) {
	if (this->is_recover_status()) {
		return 0;
	}
	Move_Scene::tick(now);
	if (now > over_time_) {
		finish_campfire();
		return 0;
	}
	check_fire_gain(now);
	check_dancing(now);
	return 0;
}

int Campfire_Scene::init_scene_normal(int scene_id, int scene_order,
		int map_id/* = 0*/) {
	init_matchman();

	Move_Scene_Base::init_scene_normal(scene_id, scene_order, map_id);
	transmit_point = scene_config()->transmit_point;
	scene_begin_time_ = Time_Value::gettimeofday();
	int remain_time = CAMPFIRE_DATA_MANAGER->get_campfire_remain_time();
	over_time_ = scene_begin_time_ + Time_Value(remain_time);
	npc_init(scene_id, 0, 0);
	act_key_ = Time_Value::gettimeofday().sec();
	exp_time_ = CONFIG_CACHE_SECRET_SHOP->get_campfire_exp_time();
	dancing_time_ = CONFIG_CACHE_SECRET_SHOP->get_campfire_dancing_time();

	return 0;
}

int Campfire_Scene::release_scene_normal(void) {
	MSG_DEBUG("campfire scene release,scene_id:%d", scene_id());
	Move_Scene::release_scene_normal();

	return 0;
}

int Campfire_Scene::enter_scene(Mover *mover) {
	Move_Scene::enter_scene(mover);

	if (0 == mover->player_self()) {
		return 0;
	}

	CAMPFIRE_DATA_MANAGER->check_if_came_in(mover->player_self());
	mover->player_self()->send_campfire_personal_info();

	return 0;
}

int Campfire_Scene::exit_scene(Mover *mover, bool offline) {
	Move_Scene::exit_scene(mover, offline);
	if (mover && mover->player_self()) {
		clear_fire_buf(mover->player_self());
		del_dancing_act(mover->player_self());
	}
	return 0;
}

int Campfire_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene::client_scene_complete(fighter);
	if (fighter.player_self()) {
		Scene_Campfire_Info* sc_info =
		CAMPFIRE_DATA_MANAGER->get_player_campfire_info(
				fighter.player_self()->role_id());
		if (sc_info) {
			fighter.player_self()->set_campfire_info(sc_info);
		}
		send_gang_campfire_info(fighter.player_self());
		fighter.player_self()->send_campfire_personal_info();
		hook_player_move(fighter.player_self());
	}
	return 0;
}

void Campfire_Scene::hook_gather_success(Mover* mover, int point_id,
		int material_type) {
	Scene_Player* player = mover->player_self();
	if (!player) {
		return;
	}
	if (material_type == MOVER_T_MATERIAL_CAMPFIRE_MEAT) { // 烤肉
		// 更改刷新点
		//change_random_coord(point_id);
		click_meat(player, point_id);
	}
//	else if(material_type == MOVER_T_MATERIAL_CAMPFIRE_FIRE){// 篝火
//		Material* material = find_material_from_point(point_id);
//		if(material){
//			click_fire(player, point_id, material);
//		}
//	}
	return;
}

void Campfire_Scene::hook_player_move(Scene_Player* player) {
	if (!player) {
		return;
	}
	// 删除跳舞状态
	del_dancing_player_check(player);
	del_dancing_act(player);
	//
	Coord coord = player->grid_coord();
	int point_id = 0;
	gang_id_t gang_id = 0;
	this->get_belong_fire_point_id(coord, point_id, gang_id);
	this->add_fire_gain_player(player, point_id, gang_id);
	if (point_id != 0) {
		add_dancing_player_check(player);
	}
}

void Campfire_Scene::npc_init(int scene_id, int sort, int sub_sort) {
	section_init(scene_id, sort, sub_sort);
	NPC_MANAGER->section_refresh(&npc_section_);
	refresh_fire();
}

void Campfire_Scene::section_init(int scene_id, int sort, int sub_sort) {
	fire_point_vec_.clear();

	npc_section_.scene_ = this;
	npc_section_.is_team = scene_config()->dungeon.can_team > 0 ? true : false;
	Monster_Section_Config* s_config =
	CONFIG_CACHE_NPC->get_monster_section_config(scene_id);
	if (s_config == NULL) {

	}
	Monster_Point_Config_Map::iterator p_it;
	for (p_it = s_config->point_map.begin(); p_it != s_config->point_map.end();
			++p_it) {
		if (p_it->second.point_type == REFRESH_POINT_TYPE_MATERIAL
				&& p_it->second.npc_vec.empty() == false) {
			const NPC_Config_Cache* m_cfg =
			CONFIG_CACHE_NPC->find_npc_config_cache(
					p_it->second.npc_vec[0].val_1);
			if (m_cfg) {
				if (m_cfg->type == MOVER_T_MATERIAL_CAMPFIRE_MEAT) {
					meat_cfg_ = p_it->second;
					continue;
				} else if (m_cfg->type == MOVER_T_MATERIAL_CAMPFIRE_FIRE) {
					fire_point_vec_.push_back(p_it->second.id);
				}
			}
		}
		else if (p_it->second.point_type == REFRESH_POINT_TYPE_MONSTER
				&& p_it->second.npc_vec.empty() == false) {
			const NPC_Config_Cache* m_cfg =
					CONFIG_CACHE_NPC->find_npc_config_cache(
							p_it->second.npc_vec[0].val_1);
			if (m_cfg && m_cfg->type == MOVER_T_MONSTER_CAMPFIRE_MATCHMAN) {
				matchman_point_cfg_.insert(std::make_pair(p_it->second.id, p_it->second));
				continue;
			} else if (m_cfg && m_cfg->type == MOVER_T_MONSTER_CAMPFIRE_GODDESS) {
				goddess_cfg_ = p_it->second;
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
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(p_it->first, record));
	}

	Monster_Group_Config_Map::const_iterator group_iter;
	for (group_iter = s_config->group_map.begin(); group_iter != s_config->group_map.end(); ++group_iter) {
		NPC_Group group;
		group.config = group_iter->second;
		npc_section_.group_map.insert(std::make_pair(group_iter->second.id, group));
	}

	init_matchman();
	init_goddess();
	init_meat();
	npc_section_.path_guide_vec = s_config->path_guide_vec;
}

int Campfire_Scene::finish_campfire(void) {
	kick_all_player();
	goddess_killer_set_.clear();
	finish_list_.clear();
	this->set_scene_status(SCENE_RECOVER_STATUS);
	recover_time_ = Time_Value::gettimeofday() + Time_Value(10);
	CAMPFIRE_DATA_MANAGER->cmd_campfire_finish();
	return 0;
}

void Campfire_Scene::listen_gang_info_sync(Scene_Player* player) {
	this->hook_player_move(player);
	this->send_gang_campfire_info(player);
}

void Campfire_Scene::reset_fire(Scene_Player* player) {
	if (!player) {
		return;
	}
	gang_id_t gang_id = player->base_detail().gang_id;
	Campfire_Detail* detail =
	CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
	if (!detail) {
		return;
	}
	int point_id = detail->id;
	Material* material = find_material_from_point(point_id);
	if (!material) {
		return;
	}
	detail->burning = 0;
	detail->id = 0;
	Appear_Addition app_add;
	app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE;
	app_add.str = "";
	app_add.array.push_back(0);
	app_add.array.push_back(0);
	app_add.array.push_back(0);
	app_add.array.push_back(0);
	app_add.int64_vec.push_back(0);
	material->set_addition(app_add);
	send_gang_campfire_info_to_all(gang_id);
	reset_meat(point_id);
}
void Campfire_Scene::reset_firewood(Scene_Player* player) {
	if (!player) {
		return;
	}
	player->campfire_info()->add_firewood = 0;
	Scene_Campfire_Info* sc_info =
	CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player->role_id());
	if (sc_info) {
		sc_info->add_firewood = 0;
		sc_info->rank = 0;
	}
	std::vector<role_id_t>::iterator iter = finish_list_.begin(); //按照完成顺序
	while (iter != finish_list_.end()) {
		if ((*iter) == sc_info->role_id)
		{
			iter = finish_list_.erase(iter);
			CAMPFIRE_DATA_MANAGER->max_rank_minus_one();
		}
		else
			++iter;
	}
	send_gang_campfire_info(player);
}

void Campfire_Scene::kick_all_player(void) {
	std::vector<Scene_Player*> player_vec;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				player_vec.push_back(player);
			}
		}
	}

	std::vector<Scene_Player*>::iterator it;
	for (it = player_vec.begin(); it != player_vec.end(); ++it) {
		if((*it)->battle_scene()){
			(*it)->force_finish_battle_scene();
		}

		this->clear_fire_buf(*it);
		int dest = 0;
		(*it)->inner_transmit(dest, 0, 0);
	}
}

void Campfire_Scene::refresh_fire(void) {
	int lv_max_firewood =
	CONFIG_CACHE_SECRET_SHOP->get_campfire_level_max_firewood(0);
	NPC_Record_Map::iterator it;
	for (it = npc_section_.record_map.begin();
			it != npc_section_.record_map.end(); ++it) {
		if (it->second.config.point_type == REFRESH_POINT_TYPE_MATERIAL) {
			RoleID_NPC_Map::iterator s_it = it->second.alive_npc_map.begin();
			if (s_it != it->second.alive_npc_map.end()) {
				if (s_it->second->material_self()) {
					if (s_it->second->mover_type()
							== MOVER_T_MATERIAL_CAMPFIRE_FIRE) {
						// std: 				公会名称
						// array:			【等级、等级当前柴火数量、等级提升柴火数量、是否点燃】
						// int64_vec:		公会id
						Appear_Addition app_add;
						app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE;
						app_add.str = "";
						app_add.array.push_back(0);
						app_add.array.push_back(0);
						app_add.array.push_back(lv_max_firewood);
						app_add.array.push_back(0);
						app_add.int64_vec.push_back(0);
						s_it->second->set_addition(app_add);
						continue;
					}
				}
			}
		}
	}
	//
}

void Campfire_Scene::init_meat(void) {
	std::vector<int>::iterator it;
	for (it = fire_point_vec_.begin(); it != fire_point_vec_.end(); ++it) {
		int fire_pt = *it;
		NPC_Record* record = this->get_npc_record(fire_pt);
		if (!record) {
			continue;
		}
		Coord src_coord = record->config.birth_coord;
		int pt1 = fire_pt * 10;
		int pt2 = pt1 + 7;
		for (int i = pt1; i <= pt2; ++i) {
			int count =
					CONFIG_CACHE_SECRET_SHOP->campfire_meat_point_config()->meat_coord.size();
			int index = i % count;
			Coord coord =
					CONFIG_CACHE_SECRET_SHOP->campfire_meat_point_config()->meat_coord[index];
			coord.x = coord.x + src_coord.x;
			coord.y = coord.y + src_coord.y;
			NPC_Record record;
			record.config = meat_cfg_;
			record.config.birth_coord = coord;
			record.scene = this;
			record.leader_id = fire_pt;
			record.config.mult_type = 0;
			if (record.config.refresh_time == 0) {
				record.config.refresh_time = 180;
			}
			Campfire_Detail* detail =
			CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(fire_pt);
			if (detail && detail->burning != 0) {
				record.config.refresh_times = 0;
				record.config.refresh_con = 0;
				record.check_refresh = true;
				int delay_time = record.config.refresh_time + random() % 30
						- 60;
				record.birth_freq = Time_Value::gettimeofday()
						+ Time_Value(delay_time);
			} else {
				record.config.refresh_times = -1;
			}
			record.init_share_hp();
			npc_section_.record_map.insert(std::make_pair(i, record));
		}
	}
}

void Campfire_Scene::refresh_meat(int point_id, Coord& src_coord) {
	NPC_Record* p_record = get_npc_record(point_id);
	if (!p_record) {
		return;
	}
	int pt1 = point_id * 10;
	int pt2 = pt1 + 7;
	for (int i = pt1; i <= pt2; ++i) {
		NPC_Record* record = get_npc_record(i);
		if (record) {
			record->clear_point();
			record->config.refresh_times = 0;
			record->config.refresh_con = 0;
			record->check_refresh = true;
			int delay_time = record->config.refresh_time + random() % 30 - 60;
			record->birth_freq = Time_Value::gettimeofday()
					+ Time_Value(delay_time);
		}
	}
}

void Campfire_Scene::reset_meat(int point_id) {
	NPC_Record* p_record = get_npc_record(point_id);
	if (!p_record) {
		return;
	}
	int pt1 = point_id * 10;
	int pt2 = pt1 + 7;
	for (int i = pt1; i <= pt2; ++i) {
		NPC_Record* record = get_npc_record(i);
		if (record) {
			record->config.refresh_times = -1;
			record->check_refresh = false;
		}
	}
}

int Campfire_Scene::req_campfire_fire(Scene_Player* player,
		Material* material) {
	if (!player || !material) {
		return -1;
	}
	if (material->npc_record() == NULL) {
		return -1;
	}
	if (player->base_detail().gang_id == 0) {
		return ERROR_BONFIRE_FIRE_NO_GANG;
	}

	if (player->campfire_info()->add_firewood != 0) {
		return ERROR_BONFIRE_FIRE_BURNING;
	}

	// 判断是不是杀了7个火柴人
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player->role_id());
	if (player_info == 0)
		return -1;
	if (player_info->killed_match_man.size() < matchman_map_.size())
		return ERROR_BONFIRE_FIRE_NO_PROB;

	int point_id = material->npc_record()->config.id;
	gang_id_t gang_id = player->base_detail().gang_id;
	Campfire_Detail* detail;
	detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(point_id);
	if (detail) { //篝火被占领了
		if (detail->burning != 0) { //篝火已经被点燃
			return ERROR_BONFIRE_FIRE_BURNING;
		}
		return ERROR_BONFIRE_FIRE_GANG_LIMIT; //不会进这里
	} else {
		Campfire_Detail* target_detail =
		CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
		if (target_detail) { //自己公会的篝火在别处
			if (target_detail->burning != 0) {
				return ERROR_BONFIRE_FIRE_GANG_LIMIT;
			}
			target_detail->burning = 1; // 点燃篝火，不会进这里
			target_detail->id = point_id;
			target_detail->gang_name = player->base_detail().gang_name;
			target_detail->coord = material->pixel_coord();
			target_detail->lighter = player->role_id();
			player->save_campfire_player_info();
		} else {
			Campfire_Detail info;
			info.burning = 1;
			info.gang_id = gang_id;
			info.gang_name = player->base_detail().gang_name;
			info.id = point_id;
			info.level = 1;
			info.lighter = player->role_id();
			int lv_max_firewood =
			CONFIG_CACHE_SECRET_SHOP->get_campfire_level_max_firewood(
					info.level);
			info.level_max_firewood_num = lv_max_firewood;
			info.coord = material->pixel_coord();
			CAMPFIRE_DATA_MANAGER->add_campfire_detail(info);
			player->save_campfire_player_info();
		}
		detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(
				point_id);
	}
	if (detail) { //点火成功
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE;
		app_add.str = detail->gang_name;
		app_add.array.push_back(detail->level);
		app_add.array.push_back(detail->level_firewood_num);
		app_add.array.push_back(detail->level_max_firewood_num);
		app_add.array.push_back(detail->burning);
		app_add.int64_vec.push_back(detail->gang_id);
		material->set_addition(app_add);

		// 刷新点火者的完成时间
		player_info->finish_time = Time_Value::gettimeofday() - scene_begin_time_;
		CAMPFIRE_DATA_MANAGER->max_rank_plus_plus();
		player_info->rank = CAMPFIRE_DATA_MANAGER->max_rank();
//		player_info->killed_match_man.clear();
		player_info->recieve_stat = 1;
		player_info->add_firewood++;
		player_info->is_lighter = 1;

		player->save_campfire_player_info();

		finish_list_.push_back(player_info->role_id);

		// to logic  更改篝火状态
		sync_campfire_info_to_logic(player, detail);
		//refresh gain
		refresh_fire_gain();
		// 点火成功飘字
		std::string text_fire_success =
				CONFIG_CACHE_SECRET_SHOP->get_text_fire_success();
		notice_player(player, 0, text_fire_success);

		// 发公告(公会，世界)
		this->send_annouce_fire_lit(player, material, detail, ANNOUNCE_DISTRICT_WORLD);
		this->send_annouce_fire_lit(player, material, detail, ANNOUNCE_DISTRICT_GANG);

		// to all gang member 弹窗
		send_gang_campfire_info_to_all(player->base_detail().gang_id);

		//refresh meat
		refresh_meat(material->npc_record()->config.id,
				material->npc_record()->config.birth_coord);
	}

	send_campfire_personal_info_to_all();

	return 0;
}

int Campfire_Scene::req_campfire_fire_old(Scene_Player* player, Material* material) {
	if(!player || !material){
		return -1;
	}
	if(material->npc_record() == NULL){
		return -1;
	}
	if(player->base_detail().gang_id == 0){
		return ERROR_BONFIRE_FIRE_NO_GANG;
	}
	if(player->ganger_detail().headship != 2 &&
			player->ganger_detail().headship != 3){
		return ERROR_BONFIRE_FIRE_NO_HEADSHIP;
	}
	int point_id = material->npc_record()->config.id;
	gang_id_t gang_id = player->base_detail().gang_id;
	Campfire_Detail* detail;
	detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(point_id);
	if(detail){
		if(detail->burning != 0){
			return ERROR_BONFIRE_FIRE_BURNING;
		}
		return ERROR_BONFIRE_FIRE_GANG_LIMIT;
	}else{
		Campfire_Detail* target_detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
		if(target_detail){
			if(target_detail->burning != 0){
				return ERROR_BONFIRE_FIRE_GANG_LIMIT;
			}
			target_detail->burning = 1;
			target_detail->id = point_id;
			target_detail->gang_name = player->base_detail().gang_name;
			target_detail->coord = material->pixel_coord();
		}else{
			Campfire_Detail info;
			info.burning = 1;
			info.gang_id = gang_id;
			info.gang_name = player->base_detail().gang_name;
			info.id = point_id;
			info.level = 1;
			int lv_max_firewood = CONFIG_CACHE_SECRET_SHOP->get_campfire_level_max_firewood(info.level);
			info.level_max_firewood_num = lv_max_firewood;
			info.coord = material->pixel_coord();
			CAMPFIRE_DATA_MANAGER->add_campfire_detail(info);
		}
		detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(point_id);
	}
	if(detail){
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE;
		app_add.str = detail->gang_name;
		app_add.array.push_back(detail->level);
		app_add.array.push_back(detail->level_firewood_num);
		app_add.array.push_back(detail->level_max_firewood_num);
		app_add.array.push_back(detail->burning);
		app_add.int64_vec.push_back(detail->gang_id);
		material->set_addition(app_add);
		// to logic
		sync_campfire_info_to_logic(player, detail);
		// to all gang member
		send_gang_campfire_info_to_all(player->base_detail().gang_id);
		//refresh meat
		refresh_meat(material->npc_record()->config.id, material->npc_record()->config.birth_coord);
		//refresh gain
		refresh_fire_gain();
		// 点火成功飘字
		std::string text_fire_success = CONFIG_CACHE_SECRET_SHOP->get_text_fire_success();
		notice_player(player, 0, text_fire_success);
		// 点火成功公会公告
		this->system_announce_guide_old(player, ANNOUNCE_ID_BONFIRE_FIRE, material->role_name());
	}
	return 0;
}

int Campfire_Scene::req_campfire_add_firewood(Scene_Player* player,
		Material* material) {
	if (!player || !material) {
		return -1;
	}
	if (material->npc_record() == NULL) {
		return -1;
	}
	if (player->campfire_info()->add_firewood != 0) {
		return ERROR_BONFIRE_FIREWOOD_DONE;
	}
	if (player->base_detail().gang_id == 0) {
		return ERROR_BONFIRE_FIREWOOD_NO_GANG;
	}

	gang_id_t gang_id = player->base_detail().gang_id;
	Campfire_Detail* detail =
	CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
	if (!detail) {
		return ERROR_BONFIRE_FIREWOOD_NO_GANG_FIRE;
	}
	int point_id = material->npc_record()->config.id;
	Campfire_Detail* target_detail =
	CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(point_id);
	if (!target_detail || (detail->gang_id != target_detail->gang_id)) {
		return ERROR_BONFIRE_FIREWOOD_GANG_LIMIT;
	}

	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player->role_id());
	if (player_info == 0)
		return -1;
	if (player_info->killed_match_man.size() < matchman_map_.size())
		return ERROR_BONFIRE_FIRE_NO_PROB;
	if (player_info->rank > 0)
		return ERROR_BONFIRE_FIREWOOD_DONE;

	player_info->finish_time = Time_Value::gettimeofday() - scene_begin_time_;
	CAMPFIRE_DATA_MANAGER->max_rank_plus_plus();
	player_info->rank = CAMPFIRE_DATA_MANAGER->max_rank();
//	player_info->killed_match_man.clear();
	player_info->recieve_stat = 1;
	player_info->add_firewood = 1;

	player->save_campfire_player_info();

	finish_list_.push_back(player_info->role_id);

	detail->firewood_num += 1;
	detail->firewood_num_temp += 1;
	if (detail->firewood_num_temp == 10)
	{
		refresh_goddess(point_id);
	}
	detail->level_firewood_num += 1;
	if (detail->level_firewood_num >= detail->level_max_firewood_num) {
		detail->level += 1;
		detail->level_firewood_num = 0;
		int max_num = 1;
		double exp_add = 1.0;
		Campfire_Gain_Config* gain_cfg =
		CONFIG_CACHE_SECRET_SHOP->get_campfire_gain(detail->level);
		if (gain_cfg) {
			max_num = gain_cfg->firewood_num;
			exp_add = gain_cfg->exp_add;
		}
		detail->level_max_firewood_num = max_num;
		// 篝火升级公告
		std::string str1 = detail->gang_name;
		std::stringstream s_stream2;
		s_stream2 << detail->level;
		std::string str2 = s_stream2.str();
		std::stringstream s_stream3;
		s_stream3 << (exp_add * 100);
		std::string str3 = s_stream3.str();
		this->system_announce(chat_scene, ANNOUNCE_ID_BONFIRE_LEVEL_UP, str1,
				str2, str3);
	}
	// fire add
	{
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE;
		app_add.str = detail->gang_name;
		app_add.array.push_back(detail->level);
		app_add.array.push_back(detail->level_firewood_num);
		app_add.array.push_back(detail->level_max_firewood_num);
		app_add.array.push_back(detail->burning);
		app_add.int64_vec.push_back(detail->gang_id);
		material->set_addition(app_add);
	}

	//前100名添柴广播
	// 发公告(公会，世界)
	if (CAMPFIRE_DATA_MANAGER->max_rank() <= 100) {
		this->send_annouce_fire_add(player, player_info->rank, detail, ANNOUNCE_DISTRICT_WORLD);
		this->send_annouce_fire_add(player, player_info->rank, detail, ANNOUNCE_DISTRICT_GANG);
	}

	std::string str_notice =
	CONFIG_CACHE_SECRET_SHOP->get_text_firewood_success();
	notice_player(player, 0, str_notice);
	// sync to logic
	int8_t sync_type = 1;
	role_id_t p_role_id = player->role_id();
	Block_Buffer buf;
	buf.make_message(INNER_CAMPFIRE_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	buf.write_int64(p_role_id);
	detail->serialize(buf);
	buf.finish_message();
	player->send_to_logic(buf);
	//to all gang menber
	send_gang_campfire_info_to_all(player->base_detail().gang_id);
	player->send_campfire_personal_info();
	return 0;
}

int Campfire_Scene::req_campfire_add_firewood_old(Scene_Player* player,
		Material* material) {
	if(!player || !material){
		return -1;
	}
	if( material->npc_record() == NULL){
		return -1;
	}
	if(player->campfire_info()->add_firewood != 0){
		return ERROR_BONFIRE_FIREWOOD_DONE;
	}
	if(player->base_detail().gang_id == 0){
		return ERROR_BONFIRE_FIREWOOD_NO_GANG;
	}
	gang_id_t gang_id = player->base_detail().gang_id;
	Campfire_Detail* detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
	if(!detail){
		return ERROR_BONFIRE_FIREWOOD_NO_GANG_FIRE;
	}
	int point_id = material->npc_record()->config.id;
	Campfire_Detail* target_detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(point_id);
	if(!target_detail || (detail->gang_id != target_detail->gang_id)){
		return ERROR_BONFIRE_FIREWOOD_GANG_LIMIT;
	}
	player->campfire_info()->add_firewood = 1;
	Scene_Campfire_Info* sc_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player->role_id());
	if(sc_info){
		sc_info->add_firewood = 1;
	}
	detail->firewood_num += 1;
	detail->level_firewood_num += 1;
	if(detail->level_firewood_num >= detail->level_max_firewood_num){
		detail->level += 1;
		detail->level_firewood_num = 0;
		int max_num = 1;
		double exp_add = 1.0;
		Campfire_Gain_Config* gain_cfg = CONFIG_CACHE_SECRET_SHOP->get_campfire_gain(detail->level);
		if(gain_cfg){
			max_num = gain_cfg->firewood_num;
			exp_add = gain_cfg->exp_add;
		}
		detail->level_max_firewood_num = max_num;
		// 篝火升级公告
		std::string str1 = detail->gang_name;
		std::stringstream s_stream2;
		s_stream2<<detail->level;
		std::string str2 = s_stream2.str();
		std::stringstream s_stream3;
		s_stream3<<(exp_add*100);
		std::string str3 = s_stream3.str();
		this->system_announce(chat_scene, ANNOUNCE_ID_BONFIRE_LEVEL_UP, str1, str2, str3);
	}
	// fire add
	{
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE;
		app_add.str = detail->gang_name;
		app_add.array.push_back(detail->level);
		app_add.array.push_back(detail->level_firewood_num);
		app_add.array.push_back(detail->level_max_firewood_num);
		app_add.array.push_back(detail->burning);
		app_add.int64_vec.push_back(detail->gang_id);
		material->set_addition(app_add);
	}
	std::string str_notice = CONFIG_CACHE_SECRET_SHOP->get_text_firewood_success();
	notice_player(player, 0, str_notice);
	// sync to logic
	int8_t sync_type = 1;
	role_id_t p_role_id = player->role_id();
	Block_Buffer buf;
	buf.make_message(INNER_CAMPFIRE_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	buf.write_int64(p_role_id);
	detail->serialize(buf);
	buf.finish_message();
	player->send_to_logic(buf);
	//to all gang menber
	send_gang_campfire_info_to_all(player->base_detail().gang_id);
	return 0;
}

void Campfire_Scene::system_announce_guide_old(Scene_Player* player, const int ann_id, std::string str1, std::string str2, std::string str3, std::string str4){
	if(player == NULL){
		return;
	}
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	if(str4 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	MSG_20500000 msg;
	msg.district = ANNOUNCE_DISTRICT_GANG;
	msg.announce_id = ann_id;
	msg.contents = ann_contents;
	msg.gang_id = player->gang_id();

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	player->send_to_local_chat(buf);
	return;
}

int Campfire_Scene::click_meat(Scene_Player* player, const int point_id) {
	if (player->base_detail().gang_id == 0) {
		return -1;
	}
	int min_cont =
	CONFIG_CACHE_SECRET_SHOP->campfire_base_config()->meat_cont.val_1;
	int max_cont =
	CONFIG_CACHE_SECRET_SHOP->campfire_base_config()->meat_cont.val_2;
	int cont = random() % (max_cont - min_cont + 1) + min_cont;
	add_player_contribute(player, cont);
//	char text_gang_gain[128] ={'\0'};
//	std::string str_gang_gain = CONFIG_CACHE_SECRET_SHOP->get_text_gang_gain();
//	snprintf(text_gang_gain, sizeof(text_gang_gain), str_gang_gain.c_str(), cont);
//	notice_player(player, 0, text_gang_gain);

	MSG_81000102 msg;
	msg.type = 0;
	msg.property.push_back(Property(503703, cont));
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

Material* Campfire_Scene::find_material_from_point(const int point) {
	NPC_Record* record = get_npc_record(point);
	if (record) {
		RoleID_NPC_Map::iterator it = record->alive_npc_map.begin();
		if (it != record->alive_npc_map.end()) {
			return it->second->material_self();
		}
	}
	return NULL;
}

void Campfire_Scene::send_gang_campfire_info(Scene_Player* player) {
	if (!player) {
		return;
	}
	MSG_81000700 msg;
	gang_id_t gang_id = player->base_detail().gang_id;
	std::string gang_name = player->base_detail().gang_name;
	int8_t add_firewood = player->campfire_info()->add_firewood;
	const Campfire_Detail* detail =
	CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
	if (detail) {
		int cm_level = detail->level;
		double fs_up_rate = 1.0;
		double incom_rate = 1.0;
		Campfire_Gain_Config* gain_cfg =
		CONFIG_CACHE_SECRET_SHOP->get_campfire_gain(cm_level);
		if (gain_cfg) {
			incom_rate = gain_cfg->exp_add;
			fs_up_rate = gain_cfg->fs_add;
		}
		msg.campfire_info.firewood = add_firewood;
		msg.campfire_info.fs_up_rate = fs_up_rate;
		msg.campfire_info.incom_rate = incom_rate;
		msg.campfire_info.level = detail->level;
		msg.campfire_info.lv_cur_num = detail->level_firewood_num;
		msg.campfire_info.lv_max_num = detail->level_max_firewood_num;
		msg.campfire_info.total_num = detail->firewood_num;
		msg.campfire_info.burning = detail->burning;
		msg.coord = detail->coord;
	} else {
		msg.campfire_info.firewood = add_firewood;
		msg.campfire_info.fs_up_rate = 0;
		msg.campfire_info.incom_rate = 0;
		msg.campfire_info.level = 0;
		msg.campfire_info.lv_cur_num = 0;
		msg.campfire_info.lv_max_num = 0;
		msg.campfire_info.total_num = 0;
		msg.campfire_info.burning = 0;
	}

	OBJ_SEND_TO_CLIENT(msg, (*player));

}

void Campfire_Scene::send_gang_campfire_info_to_all(const gang_id_t gang_id) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				gang_id_t tg_gang_id =
						it->second->player_self()->base_detail().gang_id;
				if (gang_id == tg_gang_id) {
					this->send_gang_campfire_info(it->second->player_self());
				}
			}
		}
	}
}

void Campfire_Scene::system_announce(const int chat_type, const int type,
		std::string str1, std::string str2, std::string str3,
		std::string str4) {
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if (str1 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if (str2 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if (str3 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	if (str4 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	MSG_83000000 msg;
	msg.chat_type = chat_type;
	msg.system_type = type;
	msg.content = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	broadcast_all(0, buf);
	return;
}

void Campfire_Scene::system_announce_guide(Scene_Player* player,
		const int ann_id, const int district_id, std::string str1, std::string str2, std::string str3,
		std::string str4) {
	if (player == NULL) {
		return;
	}
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	if (str1 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if (str2 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if (str3 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	if (str4 != "") {
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}
	MSG_20500000 msg;
	msg.district = district_id;//ANNOUNCE_DISTRICT_WORLD
	msg.announce_id = ann_id;
	msg.contents = ann_contents;
	msg.gang_id = player->gang_id();

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	player->send_to_local_chat(buf);
	return;
}

void Campfire_Scene::notice_player(Scene_Player* player, int type,
		std::string text) {
	if (!player) {
		return;
	}
	MSG_81003000 msg;
	msg.type = type;
	msg.text = text;
	OBJ_SEND_TO_CLIENT(msg, (*player));
}

void Campfire_Scene::add_player_contribute(Scene_Player* player,
		const int value) {
	if (!player) {
		return;
	}
	int8_t sync_type = 0;
	role_id_t p_role_id = player->role_id();
	Block_Buffer buf;
	buf.make_message(INNER_CAMPFIRE_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	buf.write_int64(p_role_id);
	buf.write_int32(value);
	buf.finish_message();
	player->send_to_logic(buf);
}

void Campfire_Scene::get_belong_fire_point_id(Coord& src_coord, int& point_id,
		gang_id_t& gang_id) {
	point_id = 0;
	gang_id = 0;
	int fire_range =
	CONFIG_CACHE_SECRET_SHOP->campfire_base_config()->fire_range;
	std::vector<int>::iterator it;
	for (it = fire_point_vec_.begin(); it != fire_point_vec_.end(); ++it) {
		int tg_point_id = *it;
		Campfire_Detail* detail =
		CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(tg_point_id);
		if (!detail || detail->burning == 0) {
			continue;
		}
		NPC_Record* record = get_npc_record(tg_point_id);
		if (record) {
			Coord coord = record->config.birth_coord;
			bool in_range = is_in_range(coord.grid_coord(),
					src_coord.grid_coord(), fire_range);
			if (in_range) {
				point_id = detail->id;
				gang_id = detail->gang_id;
				return;
			}
		}
	}
	return;
}

void Campfire_Scene::add_fire_gain_player(Scene_Player* player, int point_id,
		gang_id_t gang_id) {
	if (!player) {
		return;
	}
	if (point_id == 0) {
		Fire_Player_Gain_Map::iterator it = fire_gain_player_map_.find(
				player->role_id());
		if (it != fire_gain_player_map_.end()) {
			fire_gain_player_map_.erase(it);
			del_fire_buf(player);
			// 远离篝火飘字
			std::string text_leave_fire;
			text_leave_fire = CONFIG_CACHE_SECRET_SHOP->get_text_leave_fire();
			notice_player(player, 0, text_leave_fire);
			return;
		}
	} else {
		bool is_gang_gain = false;
		std::string txt_gang_name = "";
		if (player->base_detail().gang_id == gang_id) {
			is_gang_gain = true;
		}

		Fire_Player_Gain_Map::iterator it = fire_gain_player_map_.find(
				player->role_id());
		if (it != fire_gain_player_map_.end()) {
			if (it->second.point_id != point_id
					|| is_gang_gain != it->second.is_gang_gain) {
				del_fire_buf(player);
				it->second.point_id = point_id;
				it->second.is_gang_gain = is_gang_gain;
				it->second.time = Time_Value::gettimeofday() + Time_Value(30);

				double add_exp = 0;
				double add_fs = 0;
				Campfire_Detail* detail =
				CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
				if (detail) {
					int lv = detail->level;
					Campfire_Gain_Config* gain_cfg =
					CONFIG_CACHE_SECRET_SHOP->get_campfire_gain(lv);
					if (gain_cfg) {
						add_fs = gain_cfg->fs_add;
						add_exp = gain_cfg->exp_add;
					}
					txt_gang_name = detail->gang_name;
				}
				add_fire_buf(player, add_exp, add_fs);
				// 靠近篝火飘字
				if (is_gang_gain) {
					std::string text_enter_fire;
					text_enter_fire =
					CONFIG_CACHE_SECRET_SHOP->get_text_enter_own_fire();
					notice_player(player, 0, text_enter_fire);
				} else {
					std::string text_enter_fire;
					text_enter_fire =
					CONFIG_CACHE_SECRET_SHOP->get_text_enter_other_fire();
					char txt_out[256] = { '\0' };
					snprintf(txt_out, sizeof(txt_out), text_enter_fire.c_str(),
							txt_gang_name.c_str());
					notice_player(player, 0, txt_out);
				}
				return;
			}
		} else {
			Scene_Campfire_Gain_Info info;
			info.point_id = point_id;
			info.is_gang_gain = is_gang_gain;
			info.time = Time_Value::gettimeofday() + Time_Value(30);
			fire_gain_player_map_.insert(
					std::make_pair(player->role_id(), info));

			double add_exp = 0;
			double add_fs = 0;
			Campfire_Detail* detail =
			CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(gang_id);
			if (detail) {
				int lv = detail->level;
				Campfire_Gain_Config* gain_cfg =
				CONFIG_CACHE_SECRET_SHOP->get_campfire_gain(lv);
				if (gain_cfg) {
					add_fs = gain_cfg->fs_add;
					add_exp = gain_cfg->exp_add;
				}
				txt_gang_name = detail->gang_name;
			}
			add_fire_buf(player, add_exp, add_fs);
			// 靠近篝火飘字
			if (is_gang_gain) {
				std::string text_enter_fire;
				text_enter_fire =
				CONFIG_CACHE_SECRET_SHOP->get_text_enter_own_fire();
				notice_player(player, 0, text_enter_fire);
			} else {
				std::string text_enter_fire;
				text_enter_fire =
				CONFIG_CACHE_SECRET_SHOP->get_text_enter_other_fire();
				char txt_out[256] = { '\0' };
				snprintf(txt_out, sizeof(txt_out), text_enter_fire.c_str(),
						txt_gang_name.c_str());
				notice_player(player, 0, txt_out);
			}
		}
	}
}

void Campfire_Scene::sync_campfire_info_to_logic(Scene_Player* player,
		Campfire_Detail* detail) {
	if (!player || !detail) {
		return;
	}
	int8_t sync_type = 2;
	Block_Buffer buf;
	buf.make_message(INNER_CAMPFIRE_SCENE_SYNC_INFO);
	buf.write_int8(sync_type);
	detail->serialize(buf);
	buf.finish_message();
	player->send_to_logic(buf);
}

void Campfire_Scene::check_fire_gain(const Time_Value& now) {
	Fire_Player_Gain_Map::iterator it;
	for (it = fire_gain_player_map_.begin(); it != fire_gain_player_map_.end();
			++it) {
		if (now > it->second.time) {
			Campfire_Detail* detail =
			CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(
					it->second.point_id);
			if (detail && detail->burning != 0) {
				if (player_fire_gain(it->first, it->second.is_gang_gain,
						detail->level, now) == 0) {
					it->second.time = now + Time_Value(exp_time_);
				} else {
					fire_gain_player_map_.erase(it);
					break;
				}
			} else {
				fire_gain_player_map_.erase(it);
				break;
			}
		}
	}
}

void Campfire_Scene::check_dancing(const Time_Value& now) {
	Dancing_Player_Map::const_iterator it;
	for (it = dancing_player_map_.begin(); it != dancing_player_map_.end();) {
		if (now > it->second) {
			role_id_t role_id = it->first;
			Mover* mover = find_mover_with_layer(0, role_id);
			if (mover && mover->player_self()) {
				add_dancing_act(mover->player_self());
			}
			it = dancing_player_map_.erase(it);
		} else {
			++it;
		}
	}
}

int Campfire_Scene::player_fire_gain(role_id_t role_id, bool is_gang_gain,
		int fire_level, const Time_Value& now) {
	Mover* mover = find_mover_with_layer(0, role_id);
	if (mover && mover->player_self()) {
		int exp;
		double exp_add = 1.0;
		Campfire_Gain_Config* gain_cfg =
		CONFIG_CACHE_SECRET_SHOP->get_campfire_gain(fire_level);
		if (gain_cfg) {
			exp_add = gain_cfg->exp_add;
		}
		int level_exp = CONFIG_CACHE_SECRET_SHOP->get_campfire_level_exp(
				mover->level());
		if (is_gang_gain == false) {
			exp = level_exp * exp_add * 0.7;
		} else {
			exp = level_exp * exp_add;
		}
		if (exp <= 0) {
			exp = 1;
		}
		MSG_81000102 msg;
		msg.type = 0;
		msg.property.push_back(Property(PT_EXP_CURRENT, exp));
		OBJ_SEND_TO_CLIENT(msg, (*mover));
		mover->player_self()->modify_experience(exp);
		//篝火晚会奖励找回（只需经验）
		{
			Block_Buffer buf;
			buf.make_message(INNER_SYNC_TEAM_CUMULATE_AWARD);
			buf.write_int32(scene_config_->type);
			buf.write_int64(act_key_);
			buf.write_uint16(1);
			buf.write_int32(PT_EXP_CURRENT);
			buf.write_int32(exp);
			buf.finish_message();
			mover->player_self()->send_to_logic(buf);
		}
		return 0;
	}
	return -1;
}

void Campfire_Scene::refresh_fire_gain(void) {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				hook_player_move(player);
			}
		}
	}
}

void Campfire_Scene::add_fire_buf(Scene_Player* player, const double add_exp,
		const double add_fs) {
	if (!player) {
		return;
	}
	{
		Status_Effect_Argv argv;
		argv.caster_id = player->role_id();
		argv.status_id = FIRE_EXP_BUF_ID;
		argv.last_time_ms = -1;
		Effect_Argv_Prop prop;
		prop.i_effect_val1 = 0;
		prop.d_effect_val1 = add_exp;
		argv.data.push_back(prop);
		player->insert_status_out_fight(argv);
	}
	{
		Status_Effect_Argv argv;
		argv.caster_id = player->role_id();
		argv.status_id = FIRE_FS_BUF_ID;
		argv.last_time_ms = -1;
		Effect_Argv_Prop prop;
		prop.i_effect_val1 = 0;
		prop.d_effect_val1 = add_fs;
		argv.data.push_back(prop);
		player->insert_status_out_fight(argv);
	}

}

void Campfire_Scene::del_fire_buf(Scene_Player* player) {
	if (!player) {
		return;
	}
	{
		int result = player->erase_status_type(FIRE_EXP_BUF_ID);
		if (result == 0) {
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = FIRE_EXP_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = player->role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player->scene_broad_around(buf, Mover::SCENE_MOVE);
		}
	}

	{
		int result = player->erase_status_type(FIRE_FS_BUF_ID);
		if (result == 0) {
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = FIRE_FS_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = player->role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player->scene_broad_around(buf, Mover::SCENE_MOVE);
		}
	}
}

void Campfire_Scene::clear_fire_buf(Scene_Player* player) {
	if (!player) {
		return;
	}
	{
		int result = player->erase_status_type(FIRE_EXP_BUF_ID);
		if (result == 0) {
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = FIRE_EXP_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = player->role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player->send_to_client(buf);
		}
	}

	{
		int result = player->erase_status_type(FIRE_FS_BUF_ID);
		if (result == 0) {
			MSG_81000071 msg;
			Status_Info_Msg msg_info;
			msg_info.status_id = FIRE_FS_BUF_ID;
			msg_info.overlay = 0;
			msg_info.last_time_ms = -1;
			msg_info.status_owner = player->role_id();
			msg.buff_detail.push_back(msg_info);
			Block_Buffer buf;
			MAKE_MSG_BUF(msg, buf);
			player->send_to_client(buf);
		}
	}
}

void Campfire_Scene::add_dancing_act(Scene_Player* player) {
	if (!player) {
		return;
	}
	Appear_Addition app_add;
	app_add.type = MOVER_APPEAR_ADDITION_CAMPFIRE_DANCING;
	player->set_addition(app_add);
}

void Campfire_Scene::del_dancing_act(Scene_Player* player) {
	if (!player) {
		return;
	}
	player->del_addition(MOVER_APPEAR_ADDITION_CAMPFIRE_DANCING);
}

void Campfire_Scene::add_dancing_player_check(Scene_Player* player) {
	if (!player) {
		return;
	}
	role_id_t role_id = player->role_id();
	Time_Value finish_time = Time_Value::gettimeofday()
			+ Time_Value(dancing_time_);
	dancing_player_map_[role_id] = finish_time;
}

void Campfire_Scene::send_annouce_fire_add(Scene_Player* player, uint32_t rank,
		Campfire_Detail* detail, const int district_id) {
	if (!detail || !player)
			return;

		MSG_20500000 msg;
		msg.district = district_id;
		msg.announce_id = ANNOUNCE_ID_BONFIRE_ADD_WOOD;

		Chat_Content content;
		content.reset();
		content.type = ANN_CTTYPE_ROLE_INFO;
		content.role_Info.role_id = player->role_id();
		content.role_Info.role_name = player->role_name();
		msg.contents.push_back(content);

		std::stringstream stream;
		stream << rank;
		content.reset();
		content.type = ANN_CTTYPE_TEXT;
		content.text = stream.str();
		msg.contents.push_back(content);

		content.reset();
		content.type = ANN_CTTYPE_TEXT;
		content.text = detail->gang_name;
		msg.contents.push_back(content);

		msg.gang_id = detail->gang_id;

		Block_Buffer buf;
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		player->send_to_local_chat(buf);
}

void Campfire_Scene::del_dancing_player_check(Scene_Player* player) {
	if (!player) {
		return;
	}
	role_id_t role_id = player->role_id();
	Dancing_Player_Map::iterator it = dancing_player_map_.find(role_id);
	if (it != dancing_player_map_.end()) {
		dancing_player_map_.erase(it);
	}
}

void Campfire_Scene::req_rank_list_info(Scene_Player* player) {
	if (!player) {
		return;
	}

	MSG_50200703 msg;

	unsigned int cnt = 3;
	if (finish_list_.size() < cnt)
		cnt = finish_list_.size();
	for (int i = 0; i < (int)cnt; ++i) {
		Scene_Campfire_Info* sc_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(finish_list_[i]);
		if (!sc_info)
			continue;
		campfire_rank_info info;
		info.role_id = sc_info->role_id;
		info.role_name = sc_info->role_name;
		info.rank = sc_info->rank;
		info.time = sc_info->finish_time.sec();

		msg.rank_info.push_back(info);
	}

	Player_Campfire_Map map = CAMPFIRE_DATA_MANAGER->get_campfire_finished_player();
//	for (Player_Campfire_Map::const_iterator iter = map.begin();
//				iter != map.end(); ++iter) {
//		if (iter->second.rank > 3)
//			continue;
//
//		campfire_rank_info info;
//		info.role_id = iter->second.role_id;
//		info.role_name = iter->second.role_name;
//		info.rank = iter->second.rank;
//		info.time = iter->second.finish_time.sec();
//
//		msg.rank_info.push_back(info);
//	}

	if (map.find(player->role_id()) != map.end())
	{
		msg.rank = map.find(player->role_id())->second.rank;
		msg.time = map.find(player->role_id())->second.finish_time.sec();
	}
	else
		msg.rank = 0;

	OBJ_SEND_TO_CLIENT(msg, (*player));
}

void Campfire_Scene::hook_battle_finish(int battle_type, int win_pos,
		std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec,
		std::vector<int> kill_monters) {

	for (std::vector<Mover*>::const_iterator iter = win_mover_vec.begin();
			iter != win_mover_vec.end();
			++iter) {
		Mover *winner = *iter;

		if (!winner)
			continue;

		if (!winner->player_self())
			continue;

		role_id_t role_id = winner->player_self()->role_id();

		Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(role_id);
		if (player_info == 0)
			continue;

		for (unsigned int i = 0; i < kill_monters.size(); ++i) {
			//处理火柴人
			if (matchman_map_.find(kill_monters.at(i)) != matchman_map_.end())
				player_info->killed_match_man.insert(kill_monters.at(i));
			//火女
			const NPC_Config_Cache* m_cfg =
					CONFIG_CACHE_NPC->find_npc_config_cache(kill_monters.at(i));
			if (m_cfg && m_cfg->type == MOVER_T_MONSTER_CAMPFIRE_GODDESS) {
				goddess_killer_set_.insert(role_id);
			}

			winner->player_self()->save_campfire_player_info();
		}

		winner->player_self()->send_campfire_personal_info();
	}
}

void Campfire_Scene::init_matchman(void) {
	std::vector<Coord> coords = CONFIG_CACHE_SECRET_SHOP->get_campfire_monster_points();
	for (Monster_Point_Config_Map::const_iterator iter = matchman_point_cfg_.begin();
			iter != matchman_point_cfg_.end(); ++iter) {
		Coord coord;
		if (coords.size()) {
			int i = random() % coords.size();
			coord = coords.at(i);
			coord.type = Coord::PIXEL_TYPE;
			coords.erase(coords.begin() + i);
		} else {
			int times_try = 5000;
			while (times_try--) {
				if (get_rand_coord(coord))
					break;
			}
		}

		NPC_Record record;
		record.config = matchman_point_cfg_.find(iter->first)->second;
		record.config.birth_coord = coord;
		record.config.mult_type = 1;
		record.scene = this;
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(iter->first, record));

		Scene_Campfire_MatchMan_Info info;
		info.id = iter->second.npc_vec[0].val_1;
		info.birth_coord = record.config.birth_coord;
		info.name = CONFIG_CACHE_NPC->find_npc_config_cache(info.id)->name;
		matchman_map_.insert(std::make_pair(info.id, info));
	}
}

void Campfire_Scene::init_goddess(void) {
	std::vector<int>::iterator it;
	for (it = fire_point_vec_.begin(); it != fire_point_vec_.end(); ++it) {
		int fire_pt = *it;
		NPC_Record* src_record = this->get_npc_record(fire_pt);
		if (!src_record) {
			continue;
		}
		Coord src_coord = src_record->config.birth_coord;
		Coord coord = CONFIG_CACHE_SECRET_SHOP->get_goddess_offset();
		coord.x += src_coord.x;
		coord.y += src_coord.y;

		NPC_Record record;
		record.config = goddess_cfg_;
		record.config.birth_coord = coord;
		record.config.mult_type = 1;
		record.scene = this;
		record.leader_id = fire_pt;
		if (record.config.refresh_time == 0) {
			record.config.refresh_time = 1;
		}
		Campfire_Detail* detail =
				CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(fire_pt);
		if (detail && detail->burning != 0) {
			record.config.refresh_times = 0;
		} else {
			record.config.refresh_times = -1;
		}

		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(fire_pt * 100, record));
	}
}

void Campfire_Scene::refresh_goddess(int point_id) {
	NPC_Record* record = get_npc_record(point_id * 100);
	if (!record) {
		return;
	}
	Campfire_Detail* detail =
			CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(point_id);
	if (detail && detail->burning != 0) {
		record->config.refresh_times = 0;
		record->check_refresh = true;
		record->config.refresh_con = 2;
		record->birth_freq = Time_Value::gettimeofday();
	} else {
		record->config.refresh_times = -1;
	}
}

void Campfire_Scene::send_campfire_personal_info(Scene_Player* player) {
	if (!player) {
		return;
	}
	Scene_Campfire_Info* player_info = CAMPFIRE_DATA_MANAGER->get_player_campfire_info(player->role_id());
	if (player_info == 0)
		return;

	MSG_81000702 msg;
	std::vector<role_id_t> role_ids;
	for (Campfire_Matchman_Map::const_iterator matchman_iter = matchman_map_.begin();
			matchman_iter != matchman_map_.end(); ++matchman_iter) {
		role_ids.push_back(matchman_iter->first);
	}
	std::sort(role_ids.begin(), role_ids.end());
	std::vector<role_id_t>::const_iterator role_id_iter;
	for (role_id_iter = role_ids.begin(); role_id_iter != role_ids.end(); ++ role_id_iter) {
		Campfire_Collect_Info info;
		info.f_coord = matchman_map_.find(*role_id_iter)->second.birth_coord;
		info.role_id = matchman_map_.find(*role_id_iter)->second.id;
		info.name = matchman_map_.find(*role_id_iter)->second.name;
		info.result = player_info->killed_match_man.find(info.role_id) == player_info->killed_match_man.end() ? 0 : 1;
		msg.f_coord.push_back(info);
	}

	msg.rank = (int8_t)player_info->rank;

	Campfire_Detail* detail =
			CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_gang(player->gang_id());
	if (player_info->is_lighter)
		msg.if_receive = 0;
	else if (detail && detail->burning != 0) {
		msg.if_receive = 1;
	} else {
		msg.if_receive = 0;
	}

	msg.receive = player_info->recieve_stat;

	OBJ_SEND_TO_CLIENT(msg, (*player));
}

void Campfire_Scene::send_annouce_fire_lit(Scene_Player* player, Material *material, Campfire_Detail *detail,
		const int district_id) {
	if (!material || !detail || !player)
		return;
	if (material->npc_record() == NULL)
		return;

	MSG_20500000 msg;
	msg.district = district_id;
	msg.announce_id = ANNOUNCE_ID_BONFIRE_LIT;

	Chat_Content content;
	content.reset();
	content.type = ANN_CTTYPE_ROLE_INFO;
	content.role_Info.role_id = player->role_id();
	content.role_Info.role_name = player->role_name();
	msg.contents.push_back(content);

	content.reset();
	content.type = ANN_CTTYPE_TEXT;
	content.text = detail->gang_name;
	msg.contents.push_back(content);

	content.reset();
	content.type = ANN_CTTYPE_TEXT;
	content.text = material->role_name();
	msg.contents.push_back(content);

	msg.gang_id = detail->gang_id;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	player->send_to_local_chat(buf);
	return;
}

void Campfire_Scene::send_campfire_personal_info_to_all() {
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				it->second->player_self()->send_campfire_personal_info();
			}
		}
	}
}

int Campfire_Scene::player_req_battle(Scene_Player* player,
		role_id_t monster_id) {
	if (!player)
		return 0;

	Mover *mover = find_mover_with_layer(0, monster_id);
	if (!mover)
		return 0;

	if (mover->monster_self()->monster_config()->type == MOVER_T_MONSTER_CAMPFIRE_GODDESS) {
		role_id_t fire_pt = mover->monster_self()->npc_record()->leader_id;
		Campfire_Detail* detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(fire_pt);
		if (!detail || detail->burning == 0)
			return -1;
		if (player->gang_id() != detail->gang_id)
			return ERROR_GODDESS_NOT_OUR_GANG;
		//没打赢过才能打
		if (goddess_killer_set_.find(player->role_id()) == goddess_killer_set_.end()) {
			NPC_MANAGER->create_battle_with_monster(player, mover->npc_self()->npc_type_id(), 0);
			return 0;
		} else {
			return ERROR_HAS_KILL_GODDESS;
		}
	}
	return 1;
}
