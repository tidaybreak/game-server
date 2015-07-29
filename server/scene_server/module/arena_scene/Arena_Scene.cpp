/*
 * Arena_Scene.cpp
 *
 *  Created on: 15 2 2015
 *      Author: zhoujin
 */

#include "Arena_Scene.h"
#include "NPC/NPC_Manager.h"
#include "valentine_monster/Valentine_Monster_Manager.h"

Arena_Scene::Arena_Scene() {
	// TODO Auto-generated constructor stub

}

Arena_Scene::~Arena_Scene() {
	// TODO Auto-generated destructor stub
}

void Arena_Scene::reset(void) {

}

//int Main_Scene::open_secret_shop(const Coord& coord, const int shop_type){
//	int npc_type_id = 0;
//	if(shop_type == 1){
//		npc_type_id = 60440001;
//	}else{
//		npc_type_id = 60440002;
//	}
//	uint64_t index_debug = SECRET_SHOP_POINT_ID;
//	NPC_Record_Map::iterator it_r = npc_section_.record_map.find(index_debug);
//	if(it_r != npc_section_.record_map.end()){
//		it_r->second.clear_point();
//		npc_section_.record_map.erase(it_r);
//	}
//
//	NPC_Record record;
//
//	Monster_Point_Config point_config;
//	point_config.id = index_debug;
//	point_config.refresh_times = 1;
//	point_config.scene_key = this->scene_id();
//	point_config.speed[0] = 100;
//	point_config.speed[1] = 100;
//	point_config.type = 5;
//	point_config.birth_coord = coord;
//	Int_Int npc;
//	npc.val_1 = npc_type_id;
//	npc.val_2 = 0;
//	point_config.npc_vec.push_back(npc);
//
//	record.scene = this;
//	record.config = point_config;
//	record.init_share_hp();
//	npc_section_.record_map.insert(std::make_pair(index_debug, record));
//	NPC_Record* ref_point = this->get_npc_record(index_debug);
//	if(ref_point){
//		NPC_MANAGER->record_refresh(ref_point);
//	}
//	return 0;
//}
//
//int Main_Scene::close_secret_shop(void){
//	uint64_t index_debug = SECRET_SHOP_POINT_ID;
//	NPC_Record_Map::iterator it_r = npc_section_.record_map.find(index_debug);
//	if(it_r != npc_section_.record_map.end()){
//		it_r->second.clear_point();
//		npc_section_.record_map.erase(it_r);
//	}
//	return 0;
//}
//
//void Main_Scene::be_hurt_and_sub_blood(Battle_Scene* battle_scene, double sub_blood, double current_blood, role_id_t attacker_id, int point_id){
//	VALENTINE_MONSTER_MANAGER->be_hurt_and_sub_blood(this->scene_id(), battle_scene,
//			sub_blood, current_blood, attacker_id, point_id);
//}
