/*
 * Config_Cache_NPC.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_NPC_H_
#define CONFIG_CACHE_NPC_H_

#include "Cache_Base.h"
#include "NPC/NPC_Config.h"
#include "NPC/NPC_Struct.h"
#include "ai_machine/AI_Struct.h"

class Config_Cache_NPC : public Cache_Base {
public:

	// == NPC
	const NPC_Config_Cache* find_npc_config_cache(const role_id_t id) const;
	NPC_Record_Map* npc_record_map(void);
	Monster_Section_Config* get_monster_section_config(int scene_key);
	const Drop_Config* find_drop_config_cache(int id) const;
	Material_Section_Config* get_material_section_config(int scene_key);
	Machine_Section_Config* get_machine_section_config(int scene_key);
	const Machine_Event_Config* find_machine_event_config(int id);
	const Monster_AI_Config* find_monster_ai_config(const int id) const;

	const Monster_Chest_Config* find_monster_chest_config(const int id);
	const Chest_Lv_Config* find_chest_lv_config(const int lv);
	int get_monster_chest_id(std::vector<Int_Int> int_int_vec);
	int get_monster_chest_pos(std::vector<int8_t> int_vec);
	// == AI
	const AI_Info* find_ai_config_cache(const int id) const;

	Vagrant_Item_Config_Map* get_vagrant_item_config_map(void);
	int vagrant_item_trigger(const int scene_id, const int point);
	int tbay_trigger(const int scene_id, const int point, int& target_scene_id, int& target_scene_time, int& target_scene_type);

	Tbay_Gold_Npc_Config* get_tbay_gold_npc_config(const int8_t bless_index);
public:
	void refresh_config_cache(void);

	// == NPC
	void refresh_npc_config(void);
	void refresh_npc_record(void);
	void refresh_npc_group(void);
	void refresh_npc_path_guide(void);
	void refresh_drop_config(void);
	void refresh_material_config(void);
	void refresh_machine_config(void);
	void refresh_machine_event_config(void);
	void refresh_monster_ai_config(void);
	void refresh_monster_chest_config(void);
	void refresh_chest_lv_config(void);
	// == AI
	void refresh_ai_config(void);
	//
	void refresh_outpost_trigger(void);
	void refresh_vagrant_item(void);
	void refresh_tbay_gold_npc(void);
	// 检测怪物战斗位置
	void check_monster_position(void);
	bool check_monster_position_valid(const std::vector<int>& monster_vec, const std::vector<int>& wave_vec, const std::vector<int>& pos_vec);
	// 怪物坐标
	void check_monster_coord(void);

private:

	// 怪点出生点有效检
	bool check_monster_birth_coord(Coord& birth_coord, std::vector<Coord>& add_coord, int map_id);
	// 怪点出生点有效检测
	bool check_monster_walk_coord(std::vector<Coord>& walk_coord, int map_id);
	//
	bool check_drop(void);
	bool check_drop_box(void);
private:
	NPC_Config_Cache_Map npc_config_map_;
	Monster_Section_Config_Map monster_section_config_map_;
	Drop_Config_Map drop_config_map_;
	Material_Section_Config_Map material_section_config_map_;
	Machine_Section_Config_Map machine_section_config_map_;
	Machine_Event_Config_Map machine_event_map_;
	Monster_AI_Config_Map monster_ai_config_map_;
	// == AI
	AI_Info_Map ai_info_map_;
	//
	Outpost_Trigger_Config_Map outpost_trigger_map_;
	Vagrant_Item_Config_Map vagrant_item_map_;
	Tbay_Gold_Npc_Config_Map tbay_gold_npc_map_;
	// monster chest
	Monster_Chest_Config_Map monster_chest_map_;
	Chest_Lv_Config_Map chest_lv_map_;

};
typedef Singleton<Config_Cache_NPC> Config_Cache_NPC_Single;
#define CONFIG_CACHE_NPC    (Config_Cache_NPC_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_NPC_H_ */
