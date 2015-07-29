/*
 * NPC_Manager.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef NPC_MANAGER_H_
#define NPC_MANAGER_H_

#include "NPC/monster/Special_Monster.h"
#include "NPC/monster/Player_Monster.h"
#include "NPC/monster/Pet_Monster.h"
#include "Material.h"
#include "Machine.h"
#include "Drops.h"
#include "Camera.h"
#include "Pet.h"
#include "AI_Event.h"
#include "Object_Pool.h"
#include "boost/unordered_map.hpp"
#include "NPC/hero/Hero.h"
//#include "Utility_Func.h"

class Battle_Scene;
struct Drop_Config;
struct Drop_Box_Config;
struct Monster_Chest_Info;
struct Other_Info;

struct Path_Cache {
	Coord src;
	Coord des;
	Coord_Vec grid_coord_vec;
};
typedef std::vector<Path_Cache> Path_Cache_Vec;


class NPC_Manager {
public:
	typedef Object_Pool_Ex<NPC_Record, Thread_Mutex> NPC_Record_Pool;
	typedef Object_Pool_Ex<Drops, Thread_Mutex> Drops_NPC_Pool;
	typedef boost::unordered_map<role_id_t, Drops *> Drops_NPC_Map;
	typedef boost::unordered_map<int, Drops_NPC_Map> Scene_Drops_Map;

	typedef Object_Pool_Ex<AI_Event, Thread_Mutex> AI_Event_Pool;
	typedef Object_Pool_Ex<Monster, Thread_Mutex> Monster_Pool;
	typedef Object_Pool_Ex<Special_Monster, Thread_Mutex> Special_Monster_Pool;
	typedef Object_Pool_Ex<Player_Monster, Thread_Mutex> Player_Monster_Pool;
	typedef Object_Pool_Ex<Pet_Monster, Thread_Mutex> Pet_Monster_Pool;
	typedef Object_Pool_Ex<Camera, Thread_Mutex> Camera_Pool;
	typedef Object_Pool_Ex<Material, Thread_Mutex> Material_Pool;
	typedef Object_Pool_Ex<Machine, Thread_Mutex> Machine_Pool;

	typedef boost::unordered_map<int, AI_Event *> AI_Event_Map;
	typedef boost::unordered_map<role_id_t, Monster *> Monster_Map;
	typedef boost::unordered_map<role_id_t, Special_Monster *> Special_Monster_Map;
	typedef boost::unordered_map<role_id_t, Player_Monster *> Player_Monster_Map;
	typedef boost::unordered_map<role_id_t, Pet_Monster *> Pet_Monster_Map;
	typedef boost::unordered_map<role_id_t, NPC *> NPC_Map;

	typedef boost::unordered_map<role_id_t, Material *> Material_Map;

	typedef boost::unordered_map<role_id_t, Machine *> Machine_Map;

	typedef boost::unordered_map<int, Path_Cache_Vec> Scene_Path_Cache_Vec;

	typedef boost::unordered_map<role_id_t, Pet *> Pet_Map;
	typedef Object_Pool_Ex<Pet, Thread_Mutex> Pet_Pool;

	typedef boost::unordered_map<role_id_t, Hero *> Hero_Map;
	typedef Object_Pool_Ex<Hero, Thread_Mutex> Hero_Pool;

	const static int bucket_num = 2000;

	NPC_Manager(void);
	virtual ~NPC_Manager(void);
	NPC_Manager(const NPC_Manager &);

	int init(void);

	int tick(void);

	void show_pool_status(void);

	void npc_time_up(const Time_Value &now);

	inline role_id_t generate_drops_npc_order(void);
	inline role_id_t generate_special_monster_order(void);
	inline role_id_t generate_player_monster_order(void);
	inline role_id_t generate_pet_monster_order(void);
	inline role_id_t generate_ai_event_order(void);
	inline role_id_t generate_monster_order(void);
	inline role_id_t generate_material_order(void);
	inline role_id_t generate_machine_order(void);
	inline role_id_t generate_pet_order(void);
	inline role_id_t generate_hero_order(void);
	inline uint64_t get_own_monster_point_id(int point_id, role_id_t role_id);

	inline AI_Event_Map &ai_event_map(void);
	inline Monster_Map &monster_map(void);
	inline NPC_Map &move_scene_npc_map(void);
	inline NPC_Map &battle_scene_npc_map(void);
	inline Special_Monster_Map &special_monster_map(void);
	inline Player_Monster_Map &player_monster_map(void);
	inline Pet_Monster_Map &pet_monster_map(void);
	inline Material_Map &material_map(void);
	inline Machine_Map &machine_map(void);
	inline Drops_NPC_Map &drops_npc_map(void);
	inline Pet_Map &pet_map(void);
	inline Hero_Map &hero_map(void);

	inline bool has_monster(role_id_t role_id);
	inline AI_Event *find_ai_event(role_id_t role_id);
	inline Monster *find_monster(role_id_t role_id);
	inline Drops *find_drops_npc(role_id_t role_id);
	inline Material *find_material_npc(role_id_t role_id);
	inline Machine *find_machine_npc(role_id_t role_id);
	inline Pet *find_pet(role_id_t role_id);
	inline Hero *find_hero(role_id_t role_id);

	inline AI_Event_Pool &ai_event_pool(void);
	inline Monster_Pool &monster_pool(void);
	inline Special_Monster_Pool &special_monster_pool(void);
	inline Player_Monster_Pool &player_monster_pool(void);
	inline Pet_Monster_Pool &pet_monster_pool(void);
	inline Material_Pool &material_pool(void);
	inline Machine_Pool &machine_pool(void);
	inline Drops_NPC_Pool &drops_npc_pool(void);
	inline Camera_Pool &camera_pool(void);
	inline Pet_Pool &pet_pool(void);
	inline Hero_Pool &hero_pool(void);

	NPC_Record *create_npc_record(void);
	void release_npc_record(NPC_Record *npc_record);

	/// 怪物相关
	Monster *create_monster(int npc_type_id, int operate_type, Move_Scene* move_scene, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info, int team_count = 0, int team_pos = 0 );
	Special_Monster *create_special_monster(int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info);
	Material *create_material(int npc_type_id, Move_Scene *scene, NPC_Record* point, NPC_Addition_Info &npc_addition_info);
	Machine *create_machine(int npc_type_id, Move_Scene *scene, NPC_Record* point, NPC_Addition_Info &npc_addition_info);

	Player_Monster *create_player_monster(Player_Monster *monster, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info);
	Pet_Monster *create_pet_monster(Pet_Monster *monster, int npc_type_id, Move_Scene *scene, NPC_Addition_Info &npc_addition_info);

	Hero *create_hero_in_battle_scene(Battle_Scene* scene, const Coord& birth_coord);
	/// section record
	int insert_section_record(Move_Scene *scene, NPC_Section *npc_section, NPC_Record *npc_record, const Json::Value &records, int assign_id = 0);
	int insert_section_record(Move_Scene *scene, NPC_Section *npc_section, NPC_Record *npc_record, Coord_Vec &birth_coord, int npc_ids, int freq, int toward = 0);
	int insert_section_record(Move_Scene *scene, NPC_Section *npc_section, Coord_Vec &birth_coord, int npc_type_id);
	int active_record(NPC_Section *npc_section, int record_type);
	int deactive_record(NPC_Section *npc_section, int record_type);

	int section_refresh(NPC_Section *npc_section, int record_type = -1);
	int record_refresh(NPC_Record* npc_record, int record_max_refresh = INT_MAX);

	/// 掉落相关
	int drop_by_amount(int drops_id, Coord &birth_coord, Move_Scene *scene, int amount,
			role_id_t visible = 0, role_id_t belong = 0, role_id_t birth_by = 0);
	int drop_by_monster(Coord& coord, Move_Scene* scene, int drops_id, std::vector<role_id_t>& player_vec, int drop_type, double gold_fix_coe, double item_fix_coe,
			int mult_mode, double mult_coe, int lv_limit, double lv_coe);
	int drop_by_killer(int drops_id, Coord &birth_coord, Move_Scene *scene, role_id_t killer_id);
	int create_drops(std::string drops_name, int drop_type, int drops_type_id, int jid, Drop_Vec &drops_all, Move_Scene *scene, NPC_Record* point, Coord &birth_coord, role_id_t birth_by,
			int ai, int ai_config, int limit = 0, int coord_mode = 0, role_id_t owner = 0, int coord_offset = 0);
	int drop_by_monster_point(Coord& coord, Move_Scene* scene,  NPC_Record* record, std::vector<int>& monster_type_vec, std::vector<role_id_t>& player_vec, Monster_Chest_Info chest_info);

	int process_drop_mode(int8_t career, Drop_Vec &drops_all, const Drop_Config* drop_config,
			double mult_coe = 1.0, double lv_coe = 1.0, const double gold_fix_coe = 1.0, const double item_fix_coe = 1.0);
	int process_drop_box_mode(int8_t career, Drop_Vec &drops_all, const Drop_Box_Config& drop_box_config,
			double mult_coe = 1.0, double lv_coe = 1.0, const double gold_fix_coe = 1.0, const double item_fix_coe = 1.0);

	int get_rand_pixels_coord(Move_Scene *scene, Coord &coord, uint16_t &pix_x, uint16_t &pix_y, int rand_area = 80, int mode = 0);

	/// NPC广播
	int make_npc_announce(NPC *npc, int announce_type, role_id_t role_id, int broad_item = 0);

	void free_pool_cache(void);

	int save_path_cache(int scene, Coord &src, Coord &des, Coord_Vec &grid_coord_vec);
	int find_path_cache(int scene, Coord &src, Coord &des, Coord_Vec &grid_coord_vec);

	Battle_Scene *create_battle(Fighter* player, NPC_Record* record);
	Battle_Scene *create_battle_with_monster(Scene_Player* player, int monster_id, int8_t type, const uint32_t map_id = 0,
			const CreateMonsterArgv_Vec *argc = 0, const Other_Info *ot = 0);
	int battle_refresh(NPC_Record *npc_record, Battle_Scene* pscene);
	Monster* create_battle_monster(int npc_type_id, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info, Battle_Scene* battle_scene);
	Monster* create_battle_monster(int npc_type_id, NPC_Record *npc_record, NPC_Addition_Info &npc_addition_info,
			Battle_Scene* battle_scene, const Fix_Call_Monster_Vec& fix_call_monster_vec);

	Player_Monster* create_player_monster(int npc_type_id, NPC_Record *npc_record,
			NPC_Addition_Info &npc_addition_info,Battle_Scene* battle_scene, Block_Buffer& buf);

	int destroy_npc(NPC* npc);
private:
	const NPC_Manager &operator=(const NPC_Manager &);

	int get_drop_success_index(const std::vector<double>& rate_vec);
private:
	NPC_Record npc_manager_record_;

	role_id_t ai_event_order_;
	AI_Event_Map ai_event_map_;
	AI_Event_Pool ai_event_pool_;

	role_id_t monster_order_;
	Monster_Map monster_map_;
	NPC_Map move_scene_npc_map_;
	NPC_Map battle_scene_npc_map_;
	Monster_Pool monster_pool_;

	role_id_t special_monster_order_;
	Special_Monster_Map special_monster_map_;
	Special_Monster_Pool special_monster_pool_;

	role_id_t player_monster_order_;
	Player_Monster_Map player_monster_map_;
	Player_Monster_Pool player_monster_pool_;

	role_id_t pet_monster_order_;
	Pet_Monster_Map pet_monster_map_;
	Pet_Monster_Pool pet_monster_pool_;

	role_id_t material_order_;
	Material_Map material_map_;
	Material_Pool material_pool_;


	role_id_t machine_order_;
	Machine_Map machine_map_;
	Machine_Pool machine_pool_;

	role_id_t drops_npc_order_;
	Drops_NPC_Pool drops_npc_pool_;
	Drops_NPC_Map drops_npc_map_;

	NPC_Record_Pool npc_record_pool;
	Camera_Pool camera_pool_;

	role_id_t pet_order_;
	Pet_Pool pet_pool_;
	Pet_Map pet_map_;

	role_id_t hero_order_;
	Hero_Pool hero_pool_;
	Hero_Map hero_map_;

	Scene_Path_Cache_Vec scene_path_cache_;

};
typedef Singleton<NPC_Manager> NPC_Manager_Single;
#define NPC_MANAGER    (NPC_Manager_Single::instance())

role_id_t NPC_Manager::generate_drops_npc_order(void) {
	int order = ++drops_npc_order_;
	if (drops_npc_order_ >= drops_role_id_range[1]) {
		drops_npc_order_ = drops_role_id_range[0];
		order = drops_npc_order_;
	}

	while (1) {
		if (! drops_npc_map_.count(order)) {
			break;
		} else {
			order = ++drops_npc_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_ai_event_order(void) {
	int order = ++ai_event_order_;

	while (1) {
		if (ai_event_order_ >= ai_event_id_range[1]) {
			ai_event_order_ = ai_event_id_range[0];
			order = ai_event_order_;
		}

		if (! ai_event_map_.count(order)) {
			break;
		} else {
			order = ++ai_event_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_monster_order(void) {
	int order = ++monster_order_;

	while (1) {
		if (monster_order_ >= monster_role_id_range[1]) {
			monster_order_ = monster_role_id_range[0];
			order = monster_order_;
		}

		if (! monster_map_.count(order)) {
			break;
		} else {
			order = ++monster_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_special_monster_order(void) {
	int order = ++special_monster_order_;

	while (1) {
		if (special_monster_order_ >= special_npc_id_range[1]) {
			special_monster_order_ = special_npc_id_range[0];
			order = special_monster_order_;
		}

		if (! special_monster_map_.count(order)) {
			break;
		} else {
			order = ++special_monster_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_player_monster_order(void) {
	int order = ++player_monster_order_;

	while (1) {
		if (player_monster_order_ >= player_npc_id_range[1]) {
			player_monster_order_ = player_npc_id_range[0];
			order = player_monster_order_;
		}

		if (! player_monster_map_.count(order)) {
			break;
		} else {
			order = ++player_monster_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_pet_monster_order(void) {
	int order = ++pet_monster_order_;

	while (1) {
		if (pet_monster_order_ >= pet_npc_id_range[1]) {
			pet_monster_order_ = pet_npc_id_range[0];
			order = pet_monster_order_;
		}

		if (! pet_monster_map_.count(order)) {
			break;
		} else {
			order = ++pet_monster_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_material_order(void) {
	int order = ++material_order_;

	while (1) {
		if (material_order_ >= material_role_id_range[1]) {
			material_order_ = material_role_id_range[0];
			order = material_order_;
		}

		if (! material_map_.count(order)) {
			break;
		} else {
			order = ++material_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_machine_order(void) {
	int order = ++machine_order_;

	while (1) {
		if (machine_order_ >= machine_role_id_range[1]) {
			machine_order_ = machine_role_id_range[0];
			order = machine_order_;
		}

		if (! machine_map_.count(order)) {
			break;
		} else {
			order = ++machine_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_pet_order(void) {
	int order = ++pet_order_;

	while (1) {
		if (pet_order_ >= pet_role_id_range[1]) {
			pet_order_ = pet_role_id_range[0];
			order = pet_order_;
		}

		if (! pet_map_.count(order)) {
			break;
		} else {
			order = ++pet_order_;
		}
	}

	return order;
}

role_id_t NPC_Manager::generate_hero_order(void) {
	int order = ++hero_order_;

	while (1) {
		if (hero_order_ >= hero_role_id_range[1]) {
			hero_order_ = hero_role_id_range[0];
			order = hero_order_;
		}

		if (! hero_map_.count(order)) {
			break;
		} else {
			order = ++hero_order_;
		}
	}

	return order;
}

uint64_t NPC_Manager::get_own_monster_point_id(int point_id, role_id_t role_id){
	if(point_id >= 1500){
		return point_id;
	}
	return point_id + 10000+(role_id - player_role_id_range[0])*1500;
}

NPC_Manager::Drops_NPC_Map &NPC_Manager::drops_npc_map(void) {
	return drops_npc_map_;
}

inline bool NPC_Manager::has_monster(role_id_t role_id) {
	if (get_player_type(role_id) == Monster_Type) {
		return (monster_map_.count(role_id) == 0)? false : true;
	} else {
		return (special_monster_map_.count(role_id) == 0)? false : true;
	}

	return false;
}

inline AI_Event *NPC_Manager::find_ai_event(role_id_t role_id) {
	AI_Event_Map::iterator it = ai_event_map_.find(role_id);
	if (it != ai_event_map_.end())
		return it->second;

	return NULL;
}

Monster *NPC_Manager::find_monster(role_id_t role_id) {
	switch (get_player_type(role_id)) {
	case Monster_Type: {
		Monster_Map::iterator it = monster_map_.find(role_id);
		if (it != monster_map_.end())
			return it->second;
		break;
	}
	case Special_Monster_Type: {
		Special_Monster_Map::iterator it = special_monster_map_.find(role_id);
		if (it != special_monster_map_.end())
			return it->second;
		break;
	}
	case Player_Monster_Type: {
		Player_Monster_Map::iterator it = player_monster_map_.find(role_id);
		if (it != player_monster_map_.end())
			return it->second;
		break;
	}
	case Pet_Monster_Type: {
		Pet_Monster_Map::iterator it = pet_monster_map_.find(role_id);
		if (it != pet_monster_map_.end())
			return it->second;
		break;
	}
	default : {
		break;
	}
	}

	return 0;
}

Drops *NPC_Manager::find_drops_npc(role_id_t role_id) {
	Drops_NPC_Map::iterator it = drops_npc_map_.find(role_id);
	if (it != drops_npc_map_.end())
		return it->second;
	else
		return 0;
}

Material *NPC_Manager::find_material_npc(role_id_t role_id) {
	Material_Map::iterator it = material_map_.find(role_id);
	if (it != material_map_.end())
		return it->second;
	else
		return 0;
}

Machine *NPC_Manager::find_machine_npc(role_id_t role_id) {
	Machine_Map::iterator it = machine_map_.find(role_id);
	if (it != machine_map_.end())
		return it->second;
	else
		return 0;
}

Pet *NPC_Manager::find_pet(role_id_t role_id) {
	Pet_Map::iterator it = pet_map_.find(role_id);
	if (it != pet_map_.end())
		return it->second;
	else
		return 0;
}

Hero *NPC_Manager::find_hero(role_id_t role_id) {
	Hero_Map::iterator it = hero_map_.find(role_id);
	if (it != hero_map_.end())
		return it->second;
	else
		return 0;
}


NPC_Manager::Drops_NPC_Pool &NPC_Manager::drops_npc_pool(void) {
	return drops_npc_pool_;
}

NPC_Manager::AI_Event_Pool &NPC_Manager::ai_event_pool(void) {
	return ai_event_pool_;
}

NPC_Manager::Monster_Pool &NPC_Manager::monster_pool(void) {
	return monster_pool_;
}

NPC_Manager::Camera_Pool &NPC_Manager::camera_pool(void) {
	return camera_pool_;
}

NPC_Manager::Special_Monster_Pool &NPC_Manager::special_monster_pool(void) {
	return special_monster_pool_;
}

NPC_Manager::Player_Monster_Pool &NPC_Manager::player_monster_pool(void) {
	return player_monster_pool_;
}

NPC_Manager::Pet_Monster_Pool &NPC_Manager::pet_monster_pool(void) {
	return pet_monster_pool_;
}

NPC_Manager::Material_Pool &NPC_Manager::material_pool(void) {
	return material_pool_;
}

NPC_Manager::Machine_Pool &NPC_Manager::machine_pool(void) {
	return machine_pool_;
}

NPC_Manager::Pet_Pool &NPC_Manager::pet_pool(void) {
	return pet_pool_;
}
NPC_Manager::Hero_Pool &NPC_Manager::hero_pool(void){
	return hero_pool_;
}
NPC_Manager::AI_Event_Map &NPC_Manager::ai_event_map(void) {
	return ai_event_map_;
}

NPC_Manager::Monster_Map &NPC_Manager::monster_map(void) {
	return monster_map_;
}

NPC_Manager::NPC_Map &NPC_Manager::move_scene_npc_map(void) {
	return move_scene_npc_map_;
}

NPC_Manager::NPC_Map &NPC_Manager::battle_scene_npc_map(void) {
	return battle_scene_npc_map_;
}

NPC_Manager::Special_Monster_Map &NPC_Manager::special_monster_map(void) {
	return special_monster_map_;
}

NPC_Manager::Player_Monster_Map &NPC_Manager::player_monster_map(void) {
	return player_monster_map_;
}

NPC_Manager::Pet_Monster_Map &NPC_Manager::pet_monster_map(void) {
	return pet_monster_map_;
}

NPC_Manager::Material_Map &NPC_Manager::material_map(void) {
	return material_map_;
}

NPC_Manager::Machine_Map &NPC_Manager::machine_map(void) {
	return machine_map_;
}

NPC_Manager::Pet_Map &NPC_Manager::pet_map(void) {
	return pet_map_;
}

NPC_Manager::Hero_Map &NPC_Manager::hero_map(void) {
	return hero_map_;
}

#endif /* NPC_MANAGER_H_ */
