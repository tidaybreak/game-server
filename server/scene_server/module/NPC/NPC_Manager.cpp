/*
 * NPC_Manager.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */


#include "NPC_Manager.h"
#include "Config_Cache_NPC.h"
#include "Utility_Func.h"
#include "Scene_Player.h"
#include "Move_Scene.h"
#include "Battle_Scene.h"
#include "Scene_Module_Head.h"
#include "Global_Scene_Manager.h"
#include "Msg_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Struct.h"
#include "pack/Pack_Struct.h"
#include "Config_Cache_Dungeon.h"
#include "Battle_Scene_Struct.h"

NPC_Manager::NPC_Manager(void)
: ai_event_order_(ai_event_id_range[0]),
  monster_order_(monster_role_id_range[0]),
  monster_map_(bucket_num),
  move_scene_npc_map_(bucket_num),
  battle_scene_npc_map_(bucket_num),
  special_monster_order_(special_npc_id_range[0]),
  special_monster_map_(bucket_num),
  player_monster_order_(player_npc_id_range[0]),
  player_monster_map_(bucket_num),
  pet_monster_order_(pet_npc_id_range[0]),
  pet_monster_map_(bucket_num),
  material_order_ (material_role_id_range[0]),
  material_map_(bucket_num),
  machine_order_ (machine_role_id_range[0]),
  machine_map_(bucket_num),
  drops_npc_order_ (drops_role_id_range[0]),
  drops_npc_map_(bucket_num),
  pet_order_(pet_role_id_range[0]),
  pet_map_(bucket_num),
  hero_order_(hero_role_id_range[0]),
  hero_map_(bucket_num)

{ }

NPC_Manager::~NPC_Manager(void) { }


int NPC_Manager::init(void) {
	npc_manager_record_.reset();
	special_monster_order_ = special_npc_id_range[0];
	drops_npc_order_ = drops_role_id_range[0];
	monster_order_ = monster_role_id_range[0];
	ai_event_order_ = ai_event_id_range[0];
	material_order_ = material_role_id_range[0];
	machine_order_ = machine_role_id_range[0];
	player_monster_order_ = player_npc_id_range[0];
	pet_monster_order_ = pet_npc_id_range[0];
	pet_order_ = pet_role_id_range[0];

	return 0;
}

NPC_Record *NPC_Manager::create_npc_record(void) {
	return npc_record_pool.pop();
}

void NPC_Manager::release_npc_record(NPC_Record *npc_record) {
	npc_record_pool.push(npc_record);
}

Monster *NPC_Manager::create_monster(int npc_type_id, int operate_type, Move_Scene* move_scene, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info, int team_count /* = 0 */, int team_pos /* = 0 */) {
	if (npc_type_id == 0) {
		MSG_USER("error npc type");
		return 0;
	}

	if (!move_scene) {
		MSG_USER_TRACE("move scene null");
		return 0;
	}

	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
		MSG_USER("monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
		return 0;
	}

	if (!npc_addition_info.unmoveable_birth && !move_scene->validate_grid_coord(npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y)) {
		MSG_USER("monster birth coord:%d %d error", npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y);
		return 0;
	}

	Monster *monster = monster_pool().pop();
	if(!monster){
		return 0;
	}
	role_id_t npc_id = NPC_MANAGER->generate_monster_order();
	if (monster->birth(npc_id, npc_type_id, operate_type, move_scene, npc_record, npc_addition_info) != 0) {
		monster_pool().push(monster);
		return 0;
	}
	monster->set_team_info(team_count, team_pos);
	monster_map()[monster->role_id()] = monster;
	move_scene_npc_map()[monster->role_id()] = monster;
	if(npc_record){
		npc_record->alive_npc_map[monster->role_id()] = monster;
	}
	return monster;

//	if (monster->walk_path().has_walk_path() && get_scene_type(monster->scene_id()) == FB_Type) {
//		Coord coord;
//		monster->walk_path().get_loop_path(coord);
//		//monster->walk_path().clean_walk_coord();
//		int ret = find_path_cache(monster->scene_id(), monster->grid_coord(), coord, monster->walk_path().walk_coord);
//
//		if (ret != 0) {
//			Coord_Vec grid_coord_vec;
//			monster->path_find(monster->grid_coord(), coord, grid_coord_vec, false);
//			if (grid_coord_vec.size() > 0) {
//				save_path_cache(monster->scene_id(), monster->grid_coord(), coord, grid_coord_vec);
//				monster->walk_path().walk_coord = grid_coord_vec;
//				monster->walk_path().curr_coord_index = grid_coord_vec.size();
//			} else {
//				MSG_USER("can not find path scene:%d x:%d y:%d => x:%d y:%d", monster->scene_id(), monster->grid_coord().x, monster->current_coord().y, coord.x, coord.y);
//			}
//		}
//	}
//
//	return monster;
}



Player_Monster *NPC_Manager::create_player_monster(Player_Monster *monster, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info) {
//	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
//		MSG_USER("Special monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return 0;
//	}
//
//	if (!scene->validate_grid_coord(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y)) {
//		MSG_USER("Special monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return 0;
//	}
//
//	role_id_t npc_id = NPC_MANAGER->generate_player_monster_order();
//	if (monster->birth(npc_id, npc_type_id, scene, npc_addition_info) != 0) {
//		monster->reset();
//		player_monster_pool().push(monster);
//		return 0;
//	}
//
//	player_monster_map()[monster->npc_id()] = monster;
//
//	return monster;
	return NULL;
}

Pet_Monster *NPC_Manager::create_pet_monster(Pet_Monster *monster, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info) {
//	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
//		MSG_USER("pet monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return 0;
//	}
//
//	if (!scene->validate_grid_coord(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y)) {
//		MSG_USER("pet monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return 0;
//	}
//
//	monster->set_friend_foe_identify(npc_addition_info.identify);
//	role_id_t npc_id = NPC_MANAGER->generate_pet_monster_order();
//	if (monster->birth(npc_id, npc_type_id, scene, npc_addition_info) != 0) {
//		monster->reset();
//		pet_monster_pool().push(monster);
//		return 0;
//	}
//
//	pet_monster_map()[monster->npc_id()] = monster;
//
//	return monster;
	return NULL;
}

Special_Monster *NPC_Manager::create_special_monster(int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info) {
//	if (npc_type_id == 0) {
//		MSG_USER("error npc type");
//		return 0;
//	}
//
//	if (!scene) {
//		MSG_USER_TRACE("scene null");
//		return 0;
//	}
//
//	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
//		MSG_USER("monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return 0;
//	}
//
//	if (!npc_addition_info.unmoveable_birth && !scene->validate_grid_coord(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y)) {
//		MSG_USER("monster birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return 0;
//	}
//
//	Special_Monster *monster = special_monster_pool().pop();
//	monster->reset();
//	monster->set_friend_foe_identify(npc_addition_info.identify);
//	role_id_t npc_id = NPC_MANAGER->generate_special_monster_order();
//	if (monster->birth(npc_id, npc_type_id, scene, npc_addition_info) != 0) {
//		monster->reset();
//		special_monster_pool().push(monster);
//		return 0;
//	}
//
//	special_monster_map()[npc_id] = monster;
//
//	return monster;
	return NULL;
}

Material *NPC_Manager::create_material(int npc_type_id, Move_Scene *scene, NPC_Record* point, NPC_Addition_Info &npc_addition_info) {
	if (npc_type_id == 0) {
		MSG_USER("error npc type");
		return 0;
	}

	if (!scene) {
		MSG_USER_TRACE("scene null");
		return 0;
	}

	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
		MSG_USER("material birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
		return 0;
	}

	if (!scene->validate_grid_coord(npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y)) {
		MSG_USER("material birth coord:%d %d error", npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y);
		return 0;
	}

	Material *material = material_pool().pop();
	if(!material){
		return 0;
	}
	role_id_t npc_id = NPC_MANAGER->generate_material_order();
	if (material->birth(npc_id, npc_type_id, scene, point, npc_addition_info) != 0) {
		material_pool().push(material);
		return 0;
	}

	material_map()[npc_id] = material;
	move_scene_npc_map()[material->role_id()] = material;
	if(point){
		point->alive_npc_map[material->role_id()] = material;
	}
	return material;
}


Machine *NPC_Manager::create_machine(int npc_type_id, Move_Scene *scene, NPC_Record* point, NPC_Addition_Info &npc_addition_info) {
	if (npc_type_id == 0) {
		MSG_USER("error npc type");
		return 0;
	}

	if (!scene) {
		MSG_USER_TRACE("scene null");
		return 0;
	}

	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
		MSG_USER("machine birth coord:%d %d error", npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
		return 0;
	}

	if (!scene->validate_grid_coord(npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y)) {
		MSG_USER("machine birth coord:%d %d error", npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y);
		return 0;
	}

	Machine *machine = machine_pool().pop();
	if(!machine){
		return 0;
	}
	role_id_t npc_id = NPC_MANAGER->generate_machine_order();
	if (machine->birth(npc_id, npc_type_id, scene, point, npc_addition_info) != 0) {
		machine_pool().push(machine);
		return 0;
	}

	machine_map()[npc_id] = machine;
	move_scene_npc_map()[machine->role_id()] = machine;
	if(point){
		point->alive_npc_map[machine->role_id()] = machine;
	}
	return machine;
}

int NPC_Manager::insert_section_record(Move_Scene *scene, NPC_Section *npc_section, NPC_Record *npc_record, const Json::Value &records, int assign_id) {
//	if (scene == NULL || npc_section == NULL || npc_record == NULL || records == Json::Value::null) {
//		MSG_USER("insert section record error parm");
//		return -1;
//	}
//
//	const Json::Value &npc_conf = records["npc"];
//	if (npc_conf == Json::Value::null || npc_conf.size() == 0)  {
//		MSG_USER_TRACE("no npc insert section record error npc_conf");
//		return -1;
//	}
//
//	npc_record->reset();
//	npc_record->scene = scene;
//	npc_record->section = npc_section;
//	npc_record->type = records["type"] == Json::Value::null ? -1 : records["type"].asInt();
//	npc_record->team_hatred = records["team_mode"].asInt();
//	npc_record->ext_d_1 = records["ext_d_1"].asDouble();
//
//	if (records["freq"].asInt() >= 0)
//		npc_record->frequency.sec(records["freq"].asInt());
//	else
//		npc_record->frequency.sec(INT_MAX);
//
//	if (records["birth_freq"].asInt() >= 0) {
//		npc_record->birth_freq.sec(records["birth_freq"].asInt());
//	}
//
//	Coord grid_coord;
//	NPC_Record_Info record_coord;
//	for (size_t j = 0; j < npc_conf.size(); ++j) {
//		record_coord.reset();
//
//		// npc id规则 优化考虑传进来的参数,然后 id  rand_id
//		int npc_id = assign_id > 0 ? assign_id : npc_conf[j]["id"].asInt();
//		if (npc_id == 0 && npc_conf[j]["rand_id"] != Json::Value::null) {
//			int rand_index = random()%npc_conf[j]["rand_id"].size();
//			npc_id = npc_conf[j]["rand_id"][rand_index].asInt();
//		}
//
////		const Json::Value &npc_json = CONFIG_CACHE->npc_json(npc_id);
////		if (npc_json == Json::Value::null) {
////			MSG_USER("insert section record npc:%d config not exist", npc_conf[j]["id"].asInt());
////			continue;
////		}
//		record_coord.camps = npc_conf[j]["camps"].asInt();
//		record_coord.toward = npc_conf[j]["toward"].asInt();
//		record_coord.team_hatred = npc_conf[j]["team_mode"].asInt();
//
//		/// count = null 时按坐标刷
//		if(npc_conf[j]["count"] != Json::Value::null) {
//			record_coord.npc_count = npc_conf[j]["count"].asInt();
//			size_t npc_count = record_coord.npc_count;
//			if(0 < npc_count && npc_count < npc_conf[j]["coord"].size()) {//rand
//				Coord_Vec coord_vec;
//				for (size_t k = 0; k < npc_conf[j]["coord"].size(); ++k) {
//					grid_coord.x = npc_conf[j]["coord"][k][0u].asInt();
//					grid_coord.y = npc_conf[j]["coord"][k][1u].asInt();
//					coord_vec.push_back(grid_coord);
//				}
//				while(!coord_vec.empty() && record_coord.coord_vec.size() < npc_count) {
//					uint32_t size = coord_vec.size();
//					Coord_Vec::iterator it_begin = coord_vec.begin();
//					Coord_Vec::iterator first = it_begin;
//					uint32_t rand = std::rand() % size;
//					std::advance(first, rand);		// 随机抽中玩家
//					record_coord.coord_vec.push_back(*first);
//					coord_vec.erase(first);
//				}
//			} else {
//				for (size_t k = 0; k < npc_conf[j]["coord"].size(); ++k) {
//					grid_coord.x = npc_conf[j]["coord"][k][0u].asInt();
//					grid_coord.y = npc_conf[j]["coord"][k][1u].asInt();
//					record_coord.coord_vec.push_back(grid_coord);
//				}
//			}
//		} else {
//			for (size_t k = 0; k < npc_conf[j]["coord"].size(); ++k) {
//				grid_coord.x = npc_conf[j]["coord"][k][0u].asInt();
//				grid_coord.y = npc_conf[j]["coord"][k][1u].asInt();
//				record_coord.coord_vec.push_back(grid_coord);
//			}
//			record_coord.npc_count = record_coord.coord_vec.size();
//		}
//		npc_record->npc_type_map.insert(std::make_pair(npc_id, record_coord));
//
//		npc_record->max_count += record_coord.npc_count;
//	}
//
//	/// max_count != null 时
////	if (records["max_count"] != Json::Value::null)
////		npc_record->max_count = records["max_count"].asInt();
//
//	if (npc_record->type < 0) {
//		npc_section->insert_record(npc_section->record_index, npc_record);
//		npc_section->record_index++;
//	} else {
//		npc_section->insert_record(npc_record->type, npc_record);
//	}

	return 0;
}


int NPC_Manager::insert_section_record(Move_Scene *scene, NPC_Section *npc_section, NPC_Record *npc_record, Coord_Vec &birth_coord, int npc_id, int freq, int toward) {
//	if (scene == NULL || npc_section == NULL || npc_record == NULL) {
//		MSG_USER("insert section record error parm");
//		return -1;
//	}
//
//	npc_record->reset();
//	npc_record->scene = scene;
//	npc_record->section = npc_section;
//	npc_record->type = -1;
//
//	npc_record->frequency.sec(freq);
//
//	NPC_Record_Info record_coord;
//	record_coord.reset();
//
////	const Json::Value &npc_json = CONFIG_CACHE->npc_json(npc_id);
////	if (npc_json == Json::Value::null) {
////		MSG_USER("insert section record npc:%d config not exist", npc_id);
////		return -1;
////	}
//	record_coord.coord_vec = birth_coord;
//
//	record_coord.npc_count =  record_coord.coord_vec.size();
//	record_coord.toward = toward;
//	npc_record->npc_type_map.insert(std::make_pair(npc_id, record_coord));
//
//	npc_record->max_count += record_coord.npc_count;
//
//	npc_section->insert_record(npc_section->record_index, npc_record);
//	npc_section->record_index++;

	return 0;
}

int NPC_Manager::insert_section_record(Move_Scene *scene, NPC_Section *npc_section, Coord_Vec &birth_coord, int npc_type_id) {
//	if (scene == NULL || npc_section == NULL) {
//		MSG_USER("insert section record error parm");
//		return -1;
//	}
//
//	NPC_Record *npc_record = create_npc_record();
//	if (!npc_record) {
//		MSG_USER("insert section record error : create record");
//		return -1;
//	}
//
//	npc_record->reset();
//	npc_record->scene = scene;
//	npc_record->section = npc_section;
//	npc_record->type = -1;
//
//	NPC_Record_Info record_coord;
//	record_coord.reset();
//
////	const Json::Value &npc_json = CONFIG_CACHE->npc_json(npc_type_id);
////	if (npc_json == Json::Value::null) {
////		MSG_USER("insert section record npc:%d config not exist", npc_type_id);
////		return -1;
////	}
//	record_coord.coord_vec = birth_coord;
//
//	record_coord.npc_count =  record_coord.coord_vec.size();
//	npc_record->npc_type_map.insert(std::make_pair(npc_type_id, record_coord));
//	npc_record->max_count += record_coord.npc_count;
//	npc_section->insert_record(npc_section->record_index, npc_record);
//	npc_section->record_index++;

	return 0;
}

int NPC_Manager::active_record(NPC_Section *npc_section, int record_type) {
	return 0;
}

int NPC_Manager::deactive_record(NPC_Section *npc_section, int record_type) {
	return 0;
}

int NPC_Manager::section_refresh(NPC_Section *npc_section, int record_type) {
	Time_Value now = Time_Value::gettimeofday();
	for (NPC_Record_Map::iterator it = npc_section->record_map.begin();
			it != npc_section->record_map.end(); ++it)
	{
		if (it->second.config.refresh_times != -1)
		{
			if (record_type == -1 || record_type == it->second.config.type)
			{
				// 多人宝箱场景初始时不刷，每个玩家进入后再刷一个
				if(it->second.config.mult_type == 1){
					continue;
				}
				if(it->second.config.monster_chance != 0){
					int rd_value = random()%1000;
					if((1000-it->second.config.monster_chance) <= rd_value){
						continue;
					}
				}
				record_refresh(&(it->second));
			}
		}
	}
	return 0;
}

int NPC_Manager::record_refresh(NPC_Record *npc_record, int record_max_refresh) {
	if (! npc_record) {
		MSG_USER("dead_npc == 0");
		return -1;
	}
	npc_record->check_refresh = false;
	int current_npc_count = 0;
	current_npc_count += npc_record->alive_npc_map.size();

	npc_record->cur_ref_time = Time_Value::gettimeofday();
	npc_record->cur_ref_times += 1;
	NPC_Addition_Info npc_addition_info;
	npc_addition_info.birth_coord = npc_record->config.birth_coord;
	int npc_count = npc_record->config.npc_vec.size();
	std::vector<NPC*> vec_monster;
	int leader_index = -1;
	int team_first_index = 0;
	int team_pos_step = 0;
	//leader
	if(npc_record->config.point_type == REFRESH_POINT_TYPE_MONSTER){
		for(int i=0; i<npc_count; ++i)
		{
			if(npc_record->config.npc_vec[i].val_2 == 1)
			{
				leader_index = i;
				team_first_index = i;
				break;
			}
		}
	}

	for (int i=0; i<npc_count; ++i)
	{
		npc_addition_info.birth_coord = npc_record->config.birth_coord;
		if (npc_record->config.point_type == REFRESH_POINT_TYPE_MONSTER) {
			const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_record->config.npc_vec[i].val_1);
			if (config_cache == 0) {
				MSG_USER("create npc config:%d error", npc_record->config.npc_vec[i].val_1);
				continue;
			}
			int team_pos = 0;
			if(team_first_index != i){
				team_pos_step += 1;
				team_pos = team_pos_step;
				if(team_pos<=MAX_TEAMMATE_NUM && npc_count <= MAX_TEAMMATE_NUM){
					Coord team_coord;
					team_coord.x = npc_addition_info.birth_coord.grid_coord().x + TEAM_POS_X[npc_count][team_pos];
					team_coord.y = npc_addition_info.birth_coord.grid_coord().y + TEAM_POS_Y[npc_count][team_pos];
					if((team_coord.x != 0 || team_coord.y != 0) &&
								npc_record->scene->validate_grid_coord(team_coord.x, team_coord.y)&&
								npc_record->scene->mpt_value_by_coord(team_coord.x, team_coord.y) != Move_Scene::UNMOVE_ABLE_VAL)
					{
						npc_addition_info.birth_coord = team_coord.pixel_coord();
					}
				}
			}
			int add_coord_count = npc_record->config.add_coord_vec.size();

			if(add_coord_count > 0){
				int c_index = i%add_coord_count;
				npc_addition_info.birth_coord = npc_record->config.add_coord_vec[c_index];
			}

			Monster* pmonster = create_monster(npc_record->config.npc_vec[i].val_1, npc_record->config.type,
					npc_record->scene, npc_record, npc_addition_info, npc_count, team_pos);
			if(pmonster){
				vec_monster.push_back(pmonster);
				int talk_count = npc_record->config.talk_vec.size();
				if(i < talk_count){
					std::vector<Int_Int> talk_vec;
					talk_vec = npc_record->config.talk_vec[i];
					pmonster->set_talk_vec(talk_vec);
				}

				int chat_count = npc_record->config.chat_vec.size();
				if(i < chat_count){
					std::vector<Int_Int> chat_vec;
					chat_vec = npc_record->config.chat_vec[i];
					pmonster->set_chat_vec(chat_vec);
				}
			}


		} else if(npc_record->config.point_type == REFRESH_POINT_TYPE_MATERIAL){
			create_material(npc_record->config.npc_vec[i].val_1, npc_record->scene, npc_record, npc_addition_info);
		}
		else if(npc_record->config.point_type == REFRESH_POINT_TYPE_MACHINE){
			create_machine(npc_record->config.npc_vec[i].val_1, npc_record->scene, npc_record, npc_addition_info);
		}
		else if(npc_record->config.point_type == REFRESH_POINT_TYPE_BOX){
			int drop_type = Drops::DROP_TYPE_BOX;
			int drops_id = npc_record->config.npc_vec[i].val_1;
			const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(drops_id);
			if(!drop_config){
				return -1;
			}
			Drop_Vec drops_all;
			this->process_drop_mode(0, drops_all, drop_config, 1.0, 1.0);
			Coord drop_coord = npc_record->config.birth_coord;
			this->create_drops(drop_config->name, drop_type, drops_id, drop_config->jid, drops_all, npc_record->scene, npc_record, drop_coord, npc_record->birth_by, 0, 0);
		}else if(npc_record->config.point_type == REFRESH_POINT_TYPE_SCATTER){
			int drop_type = Drops::DROP_TYPE_SCATTER;
			int drops_id = npc_record->config.npc_vec[i].val_1;
			const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(drops_id);
			if(!drop_config){
				return -1;
			}
			Drop_Vec drops_all;
			this->process_drop_mode(0, drops_all, drop_config, 1.0, 1.0);
			Coord drop_coord = npc_record->config.birth_coord;
			int drop_count = drops_all.size();
			for(int i=0; i<drop_count; ++i){
				Coord bird_coord;
				bird_coord.type = Coord::PIXEL_TYPE;
				bird_coord.x = drop_coord.pixel_coord().x + 32 - random()%64;
				bird_coord.y = drop_coord.pixel_coord().y + 32 - random()%64;
				bool validate = npc_record->scene->validate_grid_coord(bird_coord.grid_coord().x, bird_coord.grid_coord().y);
				if(!validate){
					bird_coord = drop_coord;
				}
				Drop_Vec drop_scatter;
				drop_scatter.push_back(drops_all[i]);
				this->create_drops("", drop_type, drops_all[i].item, 0, drop_scatter, npc_record->scene, NULL, bird_coord, npc_record->birth_by, 0, 0);
			}
		}
	}

	if(npc_record->config.point_type == REFRESH_POINT_TYPE_MONSTER){
		int monster_count = vec_monster.size();
		if( npc_count > 1 && monster_count == npc_count)
		{
			if(leader_index != -1)
			{
				for(int i=0; i<npc_count; ++i)
				{
					if(i != leader_index)
					{
						vec_monster[i]->set_leader_id(vec_monster[leader_index]->role_id());
						vec_monster[leader_index]->add_teammate(vec_monster[i]->role_id());
					}
				}
			}
		}
	}

	return 0;
}

int NPC_Manager::drop_by_amount(int drops_id, Coord &birth_coord, Move_Scene *scene, int amount,
		role_id_t visible, role_id_t belong, role_id_t birth_by) {
//	if (scene == NULL) return -1;
//
//	const Json::Value &drops_json = CONFIG_CACHE->npc_drops(drops_id);
//	if (drops_json == Json::Value::null) return -1;
//
//	if (!Utility_Func::lottery_success(drops_json["total_rate"].asDouble())) return 0;
//
//	int drops_index = 0;
//	Drops_Detail::Drop_Map drops_all;
//	process_drops_mode(drops_all, drops_json, drops_index, visible, belong, amount);
//
//	return create_drops(drops_id, drops_all, scene, birth_coord, birth_by, drops_json["ai"].asInt(), drops_json["ai_config"].asInt(),
//			drops_json["limit"].asInt(), 1, 0, drops_json["coord_offset"].asInt());
		return 0;
}

int NPC_Manager::drop_by_killer(int drops_id, Coord &birth_coord, Move_Scene *scene, role_id_t killer_id) {
//	if (scene == NULL) return -1;
//
//	const Json::Value &drops_json = CONFIG_CACHE->npc_drops(drops_id);
//	if (drops_json == Json::Value::null) return -1;
//
//	if (!Utility_Func::lottery_success(drops_json["total_rate"].asDouble())) return 0;
//
//	Fighter *killer_obj = 0;
//	if (killer_id > monster_role_id_range[0] && killer_id <= special_npc_id_range[1]) {
//		killer_obj = NPC_MANAGER->find_monster(killer_id);
//	} else if (get_player_type(killer_id) == Pet_Type) {
//		killer_obj = NPC_MANAGER->find_pet(killer_id);
//	} else if (get_player_type(killer_id) == Person_Type) {
//		killer_obj = MAP_MANAGER_INSTANCE->find_role_id_player(killer_id);
//	} else {
//		MSG_USER("can not find killer role id:%d", killer_id);
//		return 0;
//	}
//
//	int drops_index = 0;
//	Drops_Detail::Drop_Map drops_all;
//	int mode = drops_json["belong_mode"].asInt();
//	role_id_t visible = drops_json["visible"].asInt();
//	if (mode == Drops::MODE_BELONG_ALL) {
//		process_drops_mode(drops_all, drops_json, drops_index, 0, 0);
//	} else if (mode == Drops::MODE_LAST_ATTACK) {
//		if (visible == 1) visible = killer_id;
//		process_drops_mode(drops_all, drops_json, drops_index, visible, killer_id);
//	}
//
//	return create_drops(drops_id, drops_all, scene, birth_coord, killer_id, drops_json["ai"].asInt(), drops_json["ai_config"].asInt(),
//			drops_json["limit"].asInt(), 0, 0, drops_json["coord_offset"].asInt());
	return 0;
}

int NPC_Manager::drop_by_monster(Coord& coord, Move_Scene* scene, int drops_id, std::vector<role_id_t>& player_vec, int drop_type, double gold_fix_coe, double item_fix_coe,
		int mult_mode, double mult_coe, int lv_limit, double lv_coe){
	if(scene == NULL) return -1;
	const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(drops_id);
	if (drop_config == NULL) return -1;
	int player_count = player_vec.size();
	if(player_count == 0){
		return -1;
	}
	int double_income = 1;
	if(scene->is_double_income()){
		double_income = 2;
	}
	std::vector<role_id_t> drops_player_vec;
	if(player_count == 1){
		drops_player_vec.push_back(player_vec[0]);
		mult_coe = 1.0;
	}else{
		if(mult_mode == 0){
			int sel_index = random()%player_count;
			drops_player_vec.push_back(player_vec[sel_index]);
		}else{
			drops_player_vec = player_vec;
		}
	}
	int drops_player_count = drops_player_vec.size();
	for(int i=0; i<drops_player_count; ++i){
		double k_mult_coe = mult_coe;
		double k_lv_coe = lv_coe;
		role_id_t killer_id = drops_player_vec[i];
		Scene_Player *killer_obj = SCENE_MONITOR->find_scene_player_by_role_id(killer_id);
		if(killer_obj){
			if(killer_obj->level() >= lv_limit){
				k_lv_coe = 1.0;
			}
		}
	//	if (killer_id > monster_role_id_range[0] && killer_id <= special_npc_id_range[1]) {
	//		killer_obj = NPC_MANAGER->find_monster(killer_id);
	//	} else if (get_player_type(killer_id) == Pet_Type) {
	//		killer_obj = NPC_MANAGER->find_pet(killer_id);
	//	} else if (get_player_type(killer_id) == Person_Type) {
	//		killer_obj = SCENE_MONITOR->find_scene_player_by_role_id(killer_id);
	//	}
		int8_t player_career = killer_obj->career();
		Drop_Vec drops_all;
		process_drop_mode(player_career, drops_all, drop_config, k_mult_coe, k_lv_coe, gold_fix_coe, item_fix_coe);
		int drop_count = drops_all.size();
		if(drop_count <= 0)
		{
			return 0;
		}
		// 掉落坐标选取
		Coord drop_coord = coord;
		if (drop_type == Drops::DROP_TYPE_BOX) {
			this->create_drops("", drop_type, drops_id, drop_config->jid, drops_all, scene, NULL, drop_coord, killer_id, 0, 0);
		} else if(drop_type == Drops::DROP_TYPE_SCATTER){
			for(int i=0; i<drop_count; ++i){
				Coord bird_coord;
				bird_coord.type = Coord::PIXEL_TYPE;
				bird_coord.x = drop_coord.pixel_coord().x + 32 - random()%96;
				bird_coord.y = drop_coord.pixel_coord().y + 32 - random()%64;
				bool validate = scene->validate_grid_coord(bird_coord.grid_coord().x, bird_coord.grid_coord().y);
				if(!validate){
					bird_coord = drop_coord;
				}
				Drop_Vec drop_scatter;
				drop_scatter.push_back(drops_all[i]);
				this->create_drops("", drop_type, drops_all[i].item, 0, drop_scatter, scene, NULL, bird_coord, killer_id, 0, 0);
			}
		}else{
			//TO LOGIC
			if(killer_obj){
				Int_Int_Map scene_reward_map;
				std::vector<Drops_Item> drops_item_vec;
				int count = drops_all.size();
				for(int i=0; i<count; ++i){
					Drops_Item item;
					item.item_id = drops_all[i].item;
					item.item_amount = drops_all[i].amount*double_income;
					item.bind = drops_all[i].bind;
					drops_item_vec.push_back(item);
					if(item.item_id == PT_EXP_CURRENT){
						//vip
						int vip_level = killer_obj->vip();
						int vip_rate = CONFIG_CACHE_DUNGEON->get_single_copy_vip_rate(vip_level);
						int base_exp = item.item_amount;
						int prop_exp = base_exp*killer_obj->exp_sum();
						int vip_exp = base_exp*(double)vip_rate/100;
						int exp = base_exp + prop_exp + vip_exp;
						scene_reward_map[item.item_id] += exp;
					}else{
						scene_reward_map[item.item_id] += item.item_amount*double_income;
					}

				}
				int scene_type = 0;
				int scene_id = 0;
				if(scene){
					scene_type = scene->scene_type();
					scene_id = scene->scene_id();
					if(scene_reward_map.empty() == false){
						scene->hook_award_player(killer_obj, scene_reward_map, 0);
					}
				}
				killer_obj->add_drops_to_package(scene_type, scene_id, drops_item_vec);
			}
		}
	}


	return 0;
}

int NPC_Manager::create_drops(std::string drops_name, int drop_type, int drops_type_id, int jid, Drop_Vec &drops_all, Move_Scene *scene, NPC_Record* point, Coord &birth_coord,
		role_id_t birth_by, int ai, int ai_config, int limit, int coord_mode, role_id_t owner, int coord_offset) {
	if(drops_all.empty() || scene == 0 ) return -1;

//	if (scene)
//		 scene->scene_get_drop_coord(birth_coord);

	if (birth_coord.x == 0 && birth_coord.y == 0) return -1;

	const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(drops_type_id);
	int cfg_max_gt_times = 1;
	if (drop_config) {
		cfg_max_gt_times = drop_config->cfg_max_gt_times;
	}

	Drops *drops = NPC_MANAGER->drops_npc_pool().pop();
	if(!drops){
		return -1;
	}
	role_id_t npc_id = NPC_MANAGER->generate_drops_npc_order();

	drops->npc_detail().direct_master = birth_by;
	drops->npc_detail().npc_type_id = drops_type_id;
	if(birth_by != 0){
		drops->npc_detail().add_id.push_back(birth_by);
	}
	drops->drops_detail().drop_type = drop_type;
	drops->drops_detail().drops_vec = drops_all;
	// 双倍收益
	if(scene->is_double_income()){
		int count = drops->drops_detail().drops_vec.size();
		for(int i = 0; i < count; ++i){
//			if(drops->drops_detail().drops_vec[i].item == PT_EXP_CURRENT){
//				continue;
//			}
			drops->drops_detail().drops_vec[i].amount *= 2;
		}
	}
	drops->drops_detail().jid = jid;
//	drops->drops_detail().get_limit = limit;
	drops->drops_detail().auto_owner = owner;
	drops->drops_detail().name = drops_name;
	drops->set_cfg_max_gt_times(cfg_max_gt_times);

	if (drops->birth(npc_id, scene, point, birth_coord, ai, ai_config) == 0) {
		drops_npc_map()[drops->npc_id()] = drops;
		move_scene_npc_map()[drops->role_id()] = drops;
		if(point){
			point->alive_npc_map[drops->role_id()] = drops;
		}
	} else {
		MSG_USER("drop error drop id:%d", npc_id);
		NPC_MANAGER->drops_npc_pool().push(drops);
	}

	return 0;
}

int NPC_Manager::process_drop_mode(int8_t career, Drop_Vec &drops_all, const Drop_Config* drop_config,
		double mult_coe/* = 1.0*/, double lv_coe /*= 1.0*/, const double gold_fix_coe/* = 1.0*/, const double item_fix_coe/* = 1.0*/){
	if(drop_config->box_rate_vec.size() != drop_config->box_vec.size()){
		return -1;
	}
	int rate_type = drop_config->rate_type;
	if(rate_type == 0){
		int count = drop_config->box_vec.size();
		for(int i=0; i<count; ++i){
			if(Utility_Func::lottery_success(drop_config->box_rate_vec[i])){
				process_drop_box_mode(career, drops_all, drop_config->box_vec[i], mult_coe, lv_coe, gold_fix_coe, item_fix_coe);
			}
		}
	}else{
		int i = this->get_drop_success_index(drop_config->box_rate_vec);
		if(i != -1)
		{
			process_drop_box_mode(career, drops_all, drop_config->box_vec[i], mult_coe, lv_coe, gold_fix_coe, item_fix_coe);
		}
	}
	return 0;
}

int NPC_Manager::process_drop_box_mode(int8_t career, Drop_Vec &drops_all, const Drop_Box_Config& drop_box_config,
		double mult_coe /*= 1.0*/, double lv_coe /*= 1.0*/, const double gold_fix_coe/* = 1.0*/, const double item_fix_coe/* = 1.0*/){
	int rate_type = drop_box_config.rate_type;
	int count = drop_box_config.item_id(career).size();
	int rate_count = drop_box_config.item_rate_vec.size();
	int bind_count = drop_box_config.item_bind_vec.size();
	int amount_count = drop_box_config.item_amount_vec.size();
	if(count != rate_count || count != bind_count || count != amount_count)
	{
		return -1;
	}
	if(rate_type == 0){
		for(int i=0; i<count; ++i)
		{
			if(Utility_Func::lottery_success(drop_box_config.item_rate_vec[i])){
				double fix_coe;
				int item_id = drop_box_config.item_id(career)[i];
				if(item_id == COPPER || item_id == SOULS){
					fix_coe = gold_fix_coe;
				}else{
					fix_coe = item_fix_coe;
				}
				int item_count = round((double)drop_box_config.item_amount_vec[i]*mult_coe*lv_coe*fix_coe);
				if(item_count > 0)
				{
					Drop_One drop_one;
					drop_one.item = item_id;
					drop_one.amount = item_count;
					drop_one.bind = drop_box_config.item_bind_vec[i];
					drops_all.push_back(drop_one);
				}
			}
		}
	}else{
		int item_index = this->get_drop_success_index(drop_box_config.item_rate_vec);
		if(item_index != -1){
			double fix_coe;
			int item_id = drop_box_config.item_id(career)[item_index];
			if(item_id == COPPER || item_id == SOULS){
				fix_coe = gold_fix_coe;
			}else{
				fix_coe = item_fix_coe;
			}

			int item_count = round((double)drop_box_config.item_amount_vec[item_index]*mult_coe*lv_coe*fix_coe);
			if( item_count > 0){
				Drop_One drop_one;
				drop_one.item = item_id;
				drop_one.amount = item_count;
				drop_one.bind = drop_box_config.item_bind_vec[item_index];
				drops_all.push_back(drop_one);
			}
		}
	}
	return 0;
}

//int NPC_Manager::drops_money_map(Drops_Detail::Drop_Map &drops_all, const Json::Value &drop_rule,
//		int &drops_index, int money_type, role_id_t visible, role_id_t belong) {
//	Drop_One drop_one;
//	for (size_t i = 0; i < drop_rule.size(); ++i) {
//		const Json::Value &drop_config = drop_rule[i];
//
//		// 错误配制
//		if (drop_config["list"].size() != drop_config["weight"].size()) continue;
//
//		int loop = drop_config["loop"].asInt();
//		loop = loop <= 0 ? 1 : loop;
//		for (int j = 0; j < loop; ++j) {
//			// 概率限制
//			if (drop_config["drop_mode"].asInt() == 0 && !Utility_Func::lottery_success(drop_config["rate"].asDouble())) continue;
//
//			// 职业限制
//			if (drop_config["career"].size() > 0 && belong > 0) {
//				Map_Player *belong_obj = MAP_MANAGER_INSTANCE->find_role_id_player(belong);
//				if (belong_obj != 0 && !in_json_array(belong_obj->base_detail().career, drop_config["career"])) {
//					continue;
//				}
//			}
//
//			// 掉落模式
//			int index = -1;
//			int drop_mode = drop_config["drop_mode"].asInt();
//			if (drop_mode) {
//				index = get_loop_index(drop_config["weight"]);
//			} else {
//				index = get_rand_index(drop_config["weight"]);
//			}
//			if (index < 0) continue;
//
//			drop_one.reset();
//			drop_one.drops_base.type = money_type;
//			drop_one.drops_base.index = drops_index;
//			drop_one.drops_base.amount = drop_config["list"][index][0u].asInt();
//			drop_one.drops_base.visible = visible;
//			drop_one.drops_base.belong = belong;
//			drop_one.drops_base.ext_value = drop_config["ext_value"].asInt();
//
//			drops_all.insert(std::make_pair(drops_index, drop_one));
//			drops_index++;
//		}
//	}
//
//	return 0;
//}

int NPC_Manager::get_rand_pixels_coord(Move_Scene *scene, Coord &coord, uint16_t &pix_x, uint16_t &pix_y, int rand_area, int mode) {
//	switch (mode) {
//	case 0 : {
//		for (int i = 0; i < 10; ++i) {
//			pix_x = get_pixel_width_by_coord(coord.x) + rand_area - random()%(rand_area * 2);
//			pix_y = get_pixel_height_by_coord(coord.y) + rand_area - random()%(rand_area * 2);
//
//			int moveable = scene->mpt_value_by_coord(get_coord_x_by_pixel(pix_x), get_coord_y_by_pixel(pix_y));
//			if (moveable >= Move_Scene::MOVE_ABLE_FROM && moveable < Move_Scene::MOVE_ABLE_END) return 0;
//		}
//		break;
//	}
//	case 1 : {
//		int rand_x = 0, rand_y = 0;
//		for (int i = 0; i < 10; ++i) {
//			rand_x = rand_area - random()%(2 * rand_area);
//			rand_x = rand_x > 0 ? rand_x + 120 : rand_x - 120;
//
//			rand_y = rand_area - random()%(2 * rand_area);
//			rand_y = rand_y > 0 ? rand_y + 120 : rand_y - 120;
//
//			pix_x = get_pixel_width_by_coord(coord.x) + rand_x;
//			pix_y = get_pixel_height_by_coord(coord.y) + rand_y;
//
//			int moveable = scene->mpt_value_by_coord(get_coord_x_by_pixel(pix_x), get_coord_y_by_pixel(pix_y));
//			if (moveable >= Move_Scene::MOVE_ABLE_FROM && moveable < Move_Scene::MOVE_ABLE_END) return 0;
//		}
//		break;
//	}
//	}
//
//	pix_x = get_pixel_width_by_coord(coord.x);
//	pix_y = get_pixel_height_by_coord(coord.y);

	return 0;
}

int NPC_Manager::make_npc_announce(NPC *npc, int announce_type, role_id_t role_id, int broad_item) {
	if (npc == 0 || announce_type == 0) return 0;
//
//	MSG_700500 msg;
//	Broad_Field broad_field;
//
//	switch (announce_type) {
//	case ANN_T_GET_WORLD_BOSS_DROPS: {
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_ROLE;
//		broad_field.int_value = role_id;
//		msg.broad_info.push_back(broad_field);
//
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_MONSTER;
//		broad_field.int_value = npc->npc_detail().direct_master;
//		msg.broad_info.push_back(broad_field);
//
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_ITEM;
//		broad_field.int_value = broad_item;
//		msg.broad_info.push_back(broad_field);
//		break;
//	}
//	case ANN_T_GET_ELITE_BOSS_DROPS: {
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_ROLE;
//		broad_field.int_value = role_id;
//		msg.broad_info.push_back(broad_field);
//
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_MONSTER;
//		broad_field.int_value = npc->npc_detail().direct_master;
//		msg.broad_info.push_back(broad_field);
//
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_ITEM;
//		broad_field.int_value = broad_item;
//		msg.broad_info.push_back(broad_field);
//		break;
//	}
//	case ANN_T_GET_XIANJU_DROP: {
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_ROLE;
//		broad_field.int_value = role_id;
//		msg.broad_info.push_back(broad_field);
//
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_MONSTER;
//		broad_field.int_value = npc->npc_detail().direct_master;
//		msg.broad_info.push_back(broad_field);
//
//		broad_field.reset();
//		broad_field.type = ANN_PARA_T_ITEM;
//		broad_field.int_value = broad_item;
//		msg.broad_info.push_back(broad_field);
//		break;
//	}
//	default : {
//		MSG_USER("announce error:%d", announce_type);
//		return -1;
//		break;
//	}
//	}
//
//	Block_Buffer inner_buf;
//	msg.type = announce_type;
//	inner_buf.make_message(INNER_NOTICE_LOGIC_BROAD);
//	msg.serialize(inner_buf);
//	inner_buf.finish_message();
//
//	MAP_MANAGER_INSTANCE->send_to_logic_server(inner_buf);

	return 0;
}

void NPC_Manager::free_pool_cache(void) {
	monster_pool_.shrink_all();
	special_monster_pool_.shrink_all();
	material_pool_.shrink_all();
	drops_npc_pool_.shrink_all();
	pet_pool_.shrink_all();
}


int NPC_Manager::save_path_cache(int scene, Coord &src, Coord &des, Coord_Vec &grid_coord_vec) {
	if (grid_coord_vec.size() <= 0) return 0;

	Path_Cache path_cache;
	path_cache.des = des;
	path_cache.src = src;
	path_cache.grid_coord_vec = grid_coord_vec;

	Scene_Path_Cache_Vec::iterator it = scene_path_cache_.find(scene);
	if (it == scene_path_cache_.end()) {
		Path_Cache_Vec path_cache_vec;
		path_cache_vec.push_back(path_cache);
		scene_path_cache_.insert(std::make_pair(scene, path_cache_vec));
	} else {
		it->second.push_back(path_cache);
	}

	return 0;
}

int NPC_Manager::find_path_cache(int scene, Coord &src, Coord &des, Coord_Vec &grid_coord_vec) {
	Scene_Path_Cache_Vec::iterator find_it = scene_path_cache_.find(scene);
	if (find_it == scene_path_cache_.end()) {

		return -1;
	}

	for (Path_Cache_Vec::iterator it = find_it->second.begin(); it != find_it->second.end(); ++it) {
		if ((*it).src == src && (*it).des == des) {
			grid_coord_vec = (*it).grid_coord_vec;
			return 0;
		}
	}

	return -1;
}

int NPC_Manager::tick(void)
{
	Time_Value now = Time_Value::gettimeofday();
	for(NPC_Map::iterator it = move_scene_npc_map_.begin(); it != move_scene_npc_map_.end(); )
	{
		if(it->second->recover_state())
		{
			destroy_npc(it->second);
			it = move_scene_npc_map_.erase(it);
		}
		else
		{
			it->second->tick(now);
			++it;
		}

	}

	for(NPC_Map::iterator it = battle_scene_npc_map_.begin(); it != battle_scene_npc_map_.end(); )
	{
		if(it->second->recover_state())
		{
			destroy_npc(it->second);
			it = battle_scene_npc_map_.erase(it);
//		}else if(it->second->npc_dead_state()){
//			it->second->exit_battle_scene();
//			++it;
		}
		else
		{
			++it;
		}

	}
	return 0;
}

void NPC_Manager::show_pool_status(void) {
	LOG_TEXT("%-30s free:%-10d, used:%d", "npc_record_pool", npc_record_pool.free_obj_list_size(), npc_record_pool.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "drops_npc_pool_", drops_npc_pool_.free_obj_list_size(), drops_npc_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "ai_event_pool_", ai_event_pool_.free_obj_list_size(), ai_event_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "monster_pool_", monster_pool_.free_obj_list_size(), monster_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "special_monster_pool_", special_monster_pool_.free_obj_list_size(), special_monster_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "scene_player_pool_", player_monster_pool_.free_obj_list_size(), player_monster_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "pet_monster_pool_", pet_monster_pool_.free_obj_list_size(), pet_monster_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "camera_pool_", camera_pool_.free_obj_list_size(), camera_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "material_pool_", material_pool_.free_obj_list_size(), material_pool_.used_obj_list_size());
	LOG_TEXT("%-30s free:%-10d, used:%d", "machine_pool_", machine_pool_.free_obj_list_size(), machine_pool_.used_obj_list_size());
}

void NPC_Manager::npc_time_up(const Time_Value &now) {
	for(NPC_Map::iterator it = move_scene_npc_map_.begin(); it != move_scene_npc_map_.end(); ++it) {
		it->second->time_up(now);
	}

	for(NPC_Map::iterator it = battle_scene_npc_map_.begin(); it != battle_scene_npc_map_.end(); ++it) {
		it->second->time_up(now);
	}
}

Battle_Scene *NPC_Manager::create_battle(Fighter* player, NPC_Record* record)
{
	if(record == NULL){
		return NULL;
	}
	if(player == NULL){
		return NULL;
	}
	//create battle_scene

	Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
	if(scene == NULL)
	{
		return NULL;
	}

	Monster_Battle_Info battle_info;
	int group_id = record->get_battle_group();
	const NPC_Group* npc_group = record->scene->get_npc_group(group_id);
	if(!npc_group){
		//MSG_USER("npc_group nonexist! scene_id: %d, groud_id: %d", monster->npc_record()->scene->scene_id(), groud_id)
		GLOBAL_SCENE_MANAGER->push_destroy_scene(scene);
		return NULL;
	}
	int m_count = npc_group->config.monster.size();
	int p_count = npc_group->config.pos.size();
	int w_count = npc_group->config.wave.size();
	if(m_count != p_count || p_count != w_count){
		//MSG_USER("npc_group config error! scene_id: %d, groud_id: %d", monster->npc_record()->scene->scene_id(), groud_id)
		GLOBAL_SCENE_MANAGER->push_destroy_scene(scene);
		return NULL;
	}
	int wave = 1;
	Monster_Wave_Info wave_info;
	battle_info.wave_vec.push_back(wave_info);
	for(int i=0; i<m_count; ++i){
		if(wave != npc_group->config.wave[i]){
			wave += 1;
			wave_info.reset();
			battle_info.wave_vec.push_back(wave_info);
		}
		battle_info.wave_vec[wave-1].monster_vec.push_back(npc_group->config.monster[i]);
		battle_info.wave_vec[wave-1].pos_vec.push_back(npc_group->config.pos[i]);
	}
	battle_info.wave_amount = wave;
	battle_info.record = record;
	battle_info.group_id = group_id;
	if(record->config.drops_coord.empty() == false){
		int d_count = record->config.drops_coord.size();
		int d_index = random()%d_count;
		battle_info.coord = record->config.drops_coord[d_index];
	}else{
		battle_info.coord = record->grid_coord();
	}

	battle_info.start_plot_id = record->config.start_plot;
	battle_info.start_plot_time = record->config.start_plot_time;
	battle_info.end_plot_id = record->config.end_plot;
	battle_info.end_plot_time = record->config.end_plot_time;

	// 单人副本或者组队副本刷宝箱怪
	if(battle_info.wave_vec.empty() == false && npc_group->config.chest_id.empty() == false){
		if(battle_info.wave_vec[0].monster_vec.size() < 9){
			if(player->move_scene()){
				if(player->move_scene()->scene_type() == FuBen_Scene_Type ||
						player->move_scene()->scene_type() == Team_Fb_Scene_Type ||
						player->move_scene()->scene_type() == HERO_DREAM_Scene_Type){
					int chest_lv  = 1;
					if(player->move_scene()->scene_type() == FuBen_Scene_Type){
						chest_lv = player->level();
					}else{
						if(player->move_scene()->scene_config()){
							chest_lv = player->move_scene()->scene_config()->dungeon.lv;
						}
					}
					if(chest_lv < 1){
						chest_lv = 1;
					}

					int chest_id = CONFIG_CACHE_NPC->get_monster_chest_id(npc_group->config.chest_id);
					if(chest_id > 0){
						int chest_pos = CONFIG_CACHE_NPC->get_monster_chest_pos(battle_info.wave_vec[0].pos_vec);
						if(chest_pos >= 1 && chest_pos <= 9){
							const Monster_Chest_Config* chest_cfg = CONFIG_CACHE_NPC->find_monster_chest_config(chest_id);
							const Chest_Lv_Config* chest_lv_cfg = CONFIG_CACHE_NPC->find_chest_lv_config(chest_lv);
							if(chest_cfg && chest_lv_cfg){
								// 添加宝箱怪到怪组
								battle_info.wave_vec[0].pos_vec.push_back(chest_pos);
								battle_info.wave_vec[0].monster_vec.push_back(chest_lv_cfg->monster_id);
								// 记录宝箱怪信息
								battle_info.monster_chest_info.monster_type_id = chest_lv_cfg->monster_id;
								battle_info.monster_chest_info.drop_type = chest_cfg->drop_type;
								battle_info.monster_chest_info.drops_vec = chest_cfg->dropid;
								battle_info.monster_chest_info.monster_model_id = chest_cfg->jta;
								battle_info.monster_chest_info.monster_name = chest_cfg->chest_name;
								int mult = 1;
								if(chest_cfg->soul_max > 0){
									mult = random()%chest_cfg->soul_max + 1;
								}
								battle_info.monster_chest_info.gold_fix_coe = ((double)(mult*chest_lv_cfg->base_correction))/1000.0;
								battle_info.monster_chest_info.item_fix_coe = ((double)(mult*chest_lv_cfg->item))/1000.0;

							}
						}
					}
				}
			}
		}
	}

	scene->set_map_id(record->config.fightMap_key);
	scene->init_battle_info(battle_info, player);
	//battle_refresh(monster->npc_record(), scene);
	return scene;
}

Battle_Scene *NPC_Manager::create_battle_with_monster(Scene_Player* player, int monster_id, int8_t type, const uint32_t map_id,
		const CreateMonsterArgv_Vec *wave_count, const Other_Info *ot){
	if(!player){
		return NULL;
	}
	if(!player->move_scene()){
		return NULL;
	}
	if(type == 0){// 怪物id
		Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
		if(scene == NULL){
			return NULL;
		}
		Monster_Battle_Info battle_info;
		int wave = 1;
		Monster_Wave_Info wave_info;
		wave_info.monster_vec.push_back(monster_id);
		wave_info.pos_vec.push_back(5);
		battle_info.wave_vec.push_back(wave_info);
		battle_info.wave_amount = wave;
		battle_info.record = NULL;
		battle_info.group_id = 0;
		battle_info.start_plot_id = "";
		battle_info.start_plot_time = 0;
		battle_info.end_plot_id = "";
		battle_info.end_plot_time = 0;
		battle_info.coord = player->grid_coord();
		scene->set_map_id(map_id);
		scene->init_battle_info(battle_info, player);
		return scene;
	}else if(type == 1){// 怪组id
		NPC_Group* npc_group = player->move_scene()->get_npc_group(monster_id);
		if(!npc_group){
			return NULL;
		}
		int m_count = npc_group->config.monster.size();
		int p_count = npc_group->config.pos.size();
		int w_count = npc_group->config.wave.size();
		if(m_count != p_count || p_count != w_count){
			return NULL;
		}
		Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
		if(scene == NULL){
			return NULL;
		}
		Monster_Battle_Info battle_info;
		int wave = 1;
		Monster_Wave_Info wave_info;
		battle_info.wave_vec.push_back(wave_info);
		for(int i=0; i<m_count; ++i){
			if(wave != npc_group->config.wave[i]){
				wave += 1;
				wave_info.reset();
				battle_info.wave_vec.push_back(wave_info);
			}
			battle_info.wave_vec[wave-1].monster_vec.push_back(npc_group->config.monster[i]);
			battle_info.wave_vec[wave-1].pos_vec.push_back(npc_group->config.pos[i]);
		}
		battle_info.wave_amount = wave;
		battle_info.record = NULL;
		battle_info.group_id = monster_id;
		battle_info.coord = player->grid_coord();
		scene->set_map_id(map_id);
		scene->init_battle_info(battle_info, player);
		return scene;
	} else if(type == 2) {// 怪组id expedition
		if (!wave_count || wave_count->empty()) {
			return NULL;
		}
		Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());
		if(scene == NULL){
			return NULL;
		}
		if (ot) {
			scene->set_other_info(*ot);
		}

		Monster_Battle_Info battle_info;
		int wave = 1;
		uint16_t group_len = 0;
		for (CreateMonsterArgv_Vec::const_iterator it = wave_count->begin(); it != wave_count->end(); ++it) {
			NPC_Group* npc_group = player->move_scene()->get_npc_group(it->i_v1);
			if(!npc_group){
				return NULL;
			}
			int m_count = npc_group->config.monster.size();
			int p_count = npc_group->config.pos.size();
			int w_count = npc_group->config.wave.size();
			if(m_count != p_count || p_count != w_count){
				return NULL;
			}
			if (battle_info.wave_vec.empty()) {
				Monster_Wave_Info wave_info;
				battle_info.wave_vec.push_back(wave_info);
				wave = (int)battle_info.wave_vec.size();
			}

			for (uint16_t n = 0; n < it->i_v2; ++n) {
				bool has_monster = false;
				for (int i=0; i < m_count && i < 2; ++i) {
					if (!has_monster
							&& group_len != 0 && group_len % 3 == 0) {
						Monster_Wave_Info wave_info;
						battle_info.wave_vec.push_back(wave_info);
						wave = (int)battle_info.wave_vec.size();
					}

					battle_info.wave_vec[wave-1].monster_vec.push_back(npc_group->config.monster[i]);
					int8_t pos = (i == 0) ? 5 : 8;
					int lef = group_len % 3;
					if (1 == lef) {
						pos -= 1;
					} else if (2 == lef) {
						pos += 1;
					}
					battle_info.wave_vec[wave-1].pos_vec.push_back(pos);
					battle_info.wave_vec[wave-1].int_1.push_back(it->i_v1);
					battle_info.wave_vec[wave-1].int64_1.push_back(it->i64_v1);
					has_monster = true;
				}
				if (has_monster) {
					++group_len;
				}
			}
		}

		battle_info.wave_amount = wave;
		battle_info.record = NULL;
		battle_info.group_id = monster_id;
		battle_info.coord = player->grid_coord();
		scene->set_map_id(map_id);
		scene->init_battle_info(battle_info, player);
		return scene;
	}

	return NULL;
}

int NPC_Manager::battle_refresh(NPC_Record *npc_record, Battle_Scene* pscene)
{
	npc_record->battle_scene = pscene;
	int group_count = npc_record->config.group_vec.size();
	if(npc_record->scene == NULL)
	{
		MSG_USER("move_scene nonexist");
		return -1;
	}
	//rate
	for(int i=0; i<group_count; i++)
	{
		if(i == group_count-1)
		{

			const NPC_Group* npc_group = npc_record->scene->get_npc_group(npc_record->config.group_vec[i].val_1);
			if(npc_group == NULL)
			{
				MSG_USER("npc_group nonexist, id %d",npc_record->config.group_vec[i].val_1);
				return -1;
			}
			int monster_count = npc_group->config.monster.size();
			for(int i=0; i<monster_count; i++)
			{
				NPC_Addition_Info npc_addition_info;
				npc_addition_info.birth_coord.x = 1;
				npc_addition_info.birth_coord.y = npc_group->config.pos[i];
				create_battle_monster(npc_group->config.monster[i], npc_record, npc_addition_info, pscene);
			}
			break;
		}
	}
	return 0;
}

Monster *NPC_Manager::create_battle_monster(int npc_type_id, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene) {
	if (npc_type_id == 0) {
		MSG_USER("error npc type");
		return 0;
	}

	if (!battle_scene) {
		MSG_USER_TRACE("battle_scene null");
		return 0;
	}

	Monster *monster = monster_pool().pop();
	if(!monster){
		return 0;
	}
	role_id_t npc_id = NPC_MANAGER->generate_monster_order();
	if (monster->battle_birth(npc_id, npc_type_id, npc_record, npc_addition_info, battle_scene) != 0) {
		monster_pool().push(monster);
		return 0;
	}
	monster_map()[monster->role_id()] = monster;
	battle_scene_npc_map()[monster->role_id()] = monster;
	return monster;
}

Monster* NPC_Manager::create_battle_monster(int npc_type_id, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info,
		Battle_Scene* battle_scene, const Fix_Call_Monster_Vec& fix_call_monster_vec) {
	if (npc_type_id == 0) {
		MSG_USER("error npc type");
		return 0;
	}

	if (!battle_scene) {
		MSG_USER_TRACE("battle_scene null");
		return 0;
	}
	Prop_Setter setter;
	for (Fix_Call_Monster_Vec::const_iterator it = fix_call_monster_vec.begin();
			it != fix_call_monster_vec.end(); ++it) {
		setter.prop_type = static_cast<Property_Type>(it->type);
		setter.addi_type = Addition_Type::AT_BASIC;
		if (it->value > 0) {
			setter.operate = Operate_Type::O_ADD;
			setter.basic = it->value;
		}
		if (it->cfg_percent > 0) {
			setter.operate = Operate_Type::O_SET;
			setter.percent = it->cfg_percent;
		}
		npc_addition_info.prop_vec.push_back(setter);
	}
	Monster *monster = monster_pool().pop();
	if(!monster){
		return 0;
	}
	role_id_t npc_id = NPC_MANAGER->generate_monster_order();
	if (monster->battle_birth(npc_id, npc_type_id, npc_record, npc_addition_info, battle_scene) != 0) {
		monster_pool().push(monster);
		return 0;
	}
	monster_map()[monster->role_id()] = monster;
	battle_scene_npc_map()[monster->role_id()] = monster;

	return monster;
}


Player_Monster *NPC_Manager::create_player_monster(int npc_type_id, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf) {
	if (npc_type_id == 0) {
		MSG_USER("error npc type");
		return 0;
	}

	if (!battle_scene) {
		MSG_USER_TRACE("battle_scene null");
		return 0;
	}

	Player_Monster *player_monster = player_monster_pool().pop();
	if(!player_monster){
		return 0;
	}
	role_id_t npc_id = NPC_MANAGER->generate_player_monster_order();
	if (player_monster->battle_birth(npc_id, npc_type_id, npc_record, npc_addition_info, battle_scene, buf) != 0) {
		player_monster_pool().push(player_monster);
		return 0;
	}

	player_monster_map()[player_monster->role_id()] = player_monster;
	battle_scene_npc_map()[player_monster->role_id()] = player_monster;
	return player_monster;
}

int NPC_Manager::get_drop_success_index(const std::vector<double>& rate_vec){
	int resIndex = -1;
	int random_nums = random() % 10000;
	int total_nums = 0;
	int count = rate_vec.size();
	for(int i=0; i<count; ++i){
		total_nums += rate_vec[i]*10000;
		if(random_nums < total_nums){
			resIndex = i;
			break;
		}
	}
	return resIndex;
}

int NPC_Manager::drop_by_monster_point(Coord& coord, Move_Scene* scene, NPC_Record* record, std::vector<int>& monster_type_vec, std::vector<role_id_t>& player_vec, Monster_Chest_Info chest_info){
	if(!scene){
		return -1;
	}
	int double_income = 1;
	if(scene->is_double_income()){
		double_income = 2;
	}
	boost::unordered_map<Scene_Player*, Id_Int_Int> player_award; //怪物普通掉落奖励

	// 怪点掉落
	if(record){
		int pd_count = record->config.drops.size();
		for(int i = 0; i<pd_count; ++i){
			int drops_id = record->config.drops[i];
			int drop_type = record->config.drop_type;
			int mult_mode = record->config.drop_mult_mode;
			double mult_coe = record->config.drop_mult_coe;
			int lv_limit = record->config.drop_lv_limit;
			double lv_coe = record->config.drop_lv_coe;
			drop_by_monster(coord, scene, drops_id, player_vec, drop_type, 1.0, 1.0, mult_mode, mult_coe, lv_limit, lv_coe);
		}
	}

	// 怪物掉落
	int count = monster_type_vec.size();
	for(int index=0; index<count; ++index){
		int drop_monster_id = monster_type_vec[index];
		//task drops
		int player_count = player_vec.size();
		for(int i=0; i<player_count; ++i){
			Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(player_vec[i]);
			if(player){
				Int_Int_Vec items = player->get_item_list(drop_monster_id);
				std::vector<Drops_Item> d_item_vec;
				int items_count = items.size();
				for(int i=0; i<items_count; ++i){
					if(items[i].val_2 > random()%100){
						Drops_Item d_item;
						d_item.item_id = items[i].val_1;
						d_item.item_amount = 1;
						d_item.bind = Item_Detail::BIND;
						d_item_vec.push_back(d_item);
					}
				}
				if(d_item_vec.empty() == false){
					int scene_type = 0;
					int scene_id = 0;
					if(scene){
						scene_type = scene->scene_type();
						scene_id = scene->scene_id();
					}
					player->add_drops_to_package(scene_type, scene_id, d_item_vec);
				}
			}
		}


		const NPC_Config_Cache* npc_config = CONFIG_CACHE_NPC->find_npc_config_cache(drop_monster_id);
		if(npc_config){
			//exp glod soul
			double res_mult_coe = npc_config->res_mult_coe;
			if(player_count <= 1){
				res_mult_coe = 1.0f;
			}
			int res_lv_limit = npc_config->res_lv_limit;
			double res_lv_coe;
			for(int i=0; i<player_count; ++i){
				Scene_Player* player = SCENE_MONITOR->find_scene_player_by_role_id(player_vec[i]);
				if(player){
					if(player->level() >= res_lv_limit){
						res_lv_coe = 1.0;
					}else{
						res_lv_coe = npc_config->res_lv_coe;
					}
					int exp = npc_config->exp*res_lv_coe*res_mult_coe;
					int gold = npc_config->money*res_lv_coe*res_mult_coe;
					int soul = npc_config->soul*res_lv_coe*res_mult_coe;
					boost::unordered_map<Scene_Player*, Id_Int_Int>::iterator it = player_award.find(player);
					if(it != player_award.end()){
						it->second.id += exp;
						it->second.val1 += gold;
						it->second.val2 += soul;
					}else{
						Id_Int_Int n_award;
						n_award.id += exp;
						n_award.val1 += gold;
						n_award.val2 += soul;
						player_award.insert(std::make_pair(player, n_award));
					}

				}
			}

			//normal drops


			int mult_mode = npc_config->drop_mult_mode;
			double mult_coe = npc_config->drop_mult_coe;
			int lv_limit = npc_config->drop_lv_limit;
			double lv_coe = npc_config->drop_lv_coe;
			if(chest_info.is_monster_chest(drop_monster_id)){// 宝箱怪掉落
				int drops_count = chest_info.drops_vec.size();
				int drop_type = chest_info.drop_type;
				double gold_fix_coe = chest_info.gold_fix_coe;
				double item_fix_coe = chest_info.item_fix_coe;
				for(int i = 0; i<drops_count; ++i){
					int drops_id = chest_info.drops_vec[i];
					drop_by_monster(coord, scene, drops_id, player_vec, drop_type, gold_fix_coe, item_fix_coe, mult_mode, mult_coe, lv_limit, lv_coe);
				}
			}else{
				int drops_count = npc_config->drops_vec.size();
				int drop_type = npc_config->drop_type;
				for(int i = 0; i<drops_count; ++i){
					int drops_id = npc_config->drops_vec[i];
					drop_by_monster(coord, scene, drops_id, player_vec, drop_type, 1.0, 1.0, mult_mode, mult_coe, lv_limit, lv_coe);
				}
			}

		}
	}

	// 怪物普通掉落奖励
	boost::unordered_map<Scene_Player*, Id_Int_Int>::iterator it;
	for(it = player_award.begin(); it != player_award.end(); ++it){
		Scene_Player* player = it->first;
		if(!player){
			continue;
		}
		Int_Int_Map scene_reward_map;
		int base_exp = it->second.id;
		int base_gold = it->second.val1;
		int base_soul = it->second.val2;
		MSG_20300011 inner_msg;
		if(scene){
			inner_msg.scene_type = scene->scene_type();
			inner_msg.scene_id = scene->scene_id();
		}
		if(base_exp > 0){
			int vip_level = player->vip();
			int vip_rate = CONFIG_CACHE_DUNGEON->get_single_copy_vip_rate(vip_level);
			int prop_exp = base_exp*player->exp_sum();
			int vip_exp = base_exp*(double)vip_rate/100;
			int exp = base_exp*double_income + prop_exp*double_income + vip_exp*double_income;
			// 移到逻辑服
//			MSG_81000102 msg;
//			msg.vip_exp = vip_exp*double_income;
//			msg.type = 0;
//			msg.property.push_back(Property(PT_EXP_CURRENT, exp));
//			player->modify_experience(exp);
//			OBJ_SEND_TO_CLIENT(msg, (*player));
			scene_reward_map[PT_EXP_CURRENT] += exp;

			Drops_Item item;
			item.item_id = PT_EXP_CURRENT;
			item.item_amount = base_exp;
			item.bind = double_income;
			inner_msg.goods_vec.push_back(item);
		}
		if(base_gold > 0){
			Drops_Item item;
			item.item_id = COPPER;
			item.item_amount = base_gold*double_income;
			item.bind = Item_Detail::BIND;
			inner_msg.goods_vec.push_back(item);
			scene_reward_map[COPPER] += base_gold*double_income;
		}
		if(base_soul > 0){
			Drops_Item item;
			item.item_id = SOULS;
			item.item_amount = base_soul*double_income;
			item.bind = Item_Detail::BIND;
			inner_msg.goods_vec.push_back(item);
			scene_reward_map[SOULS] += base_soul*double_income;
		}
		if(inner_msg.goods_vec.empty() == false){
			Block_Buffer buf;
			buf.make_message(inner_msg.msg_id);
			inner_msg.serialize(buf);
			buf.finish_message();
			player->send_to_logic(buf);
		}
		if(scene && scene_reward_map.empty() == false){
			scene->hook_award_player(player, scene_reward_map, 0);
		}
	}

	return 0;
}

int NPC_Manager::destroy_npc(NPC* npc){
	if(npc->is_monster()){
		monster_map().erase(npc->role_id());
		monster_pool().push(npc->monster_self());
	}else if(npc->is_player_monster()){
		player_monster_map().erase(npc->role_id());
		player_monster_pool().push(npc->player_monster_self());
	}else if(npc->is_drops()){
		drops_npc_map().erase(npc->role_id());
		drops_npc_pool().push(npc->drops_self());
	}else if(npc->is_material()){
		material_map().erase(npc->role_id());
		material_pool().push(npc->material_self());
	}else if(npc->is_machine()){
		machine_map().erase(npc->role_id());
		machine_pool().push(npc->machine_self());
	}else if(npc->is_hero()){
		hero_map().erase(npc->role_id());
		hero_pool().push(npc->hero_self());
	}
	return 0;
}

Hero* NPC_Manager::create_hero_in_battle_scene(Battle_Scene* scene, const Coord& birth_coord){
	Hero *hero = hero_pool().pop();
//	hero->reset();
//	role_id_t role_id = NPC_MANAGER->generate_hero_order();
//	if (hero->battle_birth(role_id, scene, birth_coord) != 0) {
//		hero->reset();
//		hero_pool().push(hero);
//		return 0;
//	}
	if(!hero){
		return 0;
	}
	hero->hero_base_detail().hero_role_id = NPC_MANAGER->generate_hero_order();
	hero_map()[hero->role_id()] = hero;
	return hero;
}

