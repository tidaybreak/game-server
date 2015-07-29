/*
 * Config_Cache_Expedition.h
 *
 *  Created on: 2014-05-12
 *      Author: "xiaoliang"
 *
 */

#ifndef CONFIG_CACHE_EXPEDITION_H_
#define CONFIG_CACHE_EXPEDITION_H_

#include "Cache_Base.h"
#include "expedition/Expedition_Config.h"


class Config_Cache_Expedition : public Cache_Base {
public:
	typedef std::map<int, Item_Info_Vec> Int_ItemInfoVec_Map;


	const Expedition_Map_Point_Cfg *get_map_point_conf_by_id(uint32_t id) const;
	const ExpeditionGangHomeCfg_Map& gang_home_conf_map(void) const;
	const Expedition_Gang_Home_Cfg *get_gang_home_conf_by_rank(uint32_t rank) const;
	const ExpeditionMapPointCfg_Map &get_map_point_conf_map(void) const;
	const Expedition_Common_Cfg *get_common_conf_by_key(uint32_t key) const;

	int get_territory_area_by_point_id(const int point_id, Int_Hash_Set &territorys, Int_Hash_Set &used_point_ids) const;
	int get_territory_by_point_id(const int point_id, Int_Hash_Set &territorys) const;
	const Int_Hash_Set *get_base_territory_by_point_id_from_conf(const int point_id) const;
	const Int_IntHashSet_Map &base_territorys_conf(void) const;

	int find_monster_group_force(const int monster_point_id);
	const Int_Int_Map &monster_group_force(void);

	int get_percent_for_beast_by_count(const int count);
	int get_percent_for_chest_by_count(const int count);

	const Expedition_Award_Demage_Cfg& demage_award(void);
	const Item_Info_Vec *find_settle_award(const int type) const;
	const Expedition_Daily_Award_Big_Data *get_big_settle_award_by_nums(const int nums);
	const ExpeditionDailyAwardBigData_Map &settle_award_big();
	int get_settle_award_decay(const int line);
	const Item_Info_Vec *find_demage_rank_award(const int rank) const;

	const Item_Info_Vec *find_occupy_castle_rank_award(const int nums) const;
	Item_Info_Vec find_occupy_castle_rank_award(const int rank, const int had_get_nums) const;
	const Int_ItemInfoVec_Map &occupy_castle_rank_award(void);

	const Expedition_World_Level_Monster_Modify *get_monster_modify_by_world_level(const int world_level);

	bool is_demage_count_time(void);
	bool is_demage_settle_time(int &settle_time);
	int get_next_settle_time(const int settle_time);

	void refresh_monster_timing(const Time_Value &now, Int_Hash_Set &need_refresh_points);
	void refresh_drop_or_material(const Time_Value &now, Int_Hash_Set &need_refresh_points, Int_Hash_Set &need_clear_points);
	int find_monster_refresh_gap_by_point_id(const int point_id);
	bool is_drop_need_boradcast(const int item_id);


	void refresh_config_cache(void);
	void refresh_map_point_conf_cache(void);
	void refresh_gang_home_conf_cache(void);
	void refresh_common_conf_cache(void);
	void refresh_monster_group(void);
	void refresh_award(void);

private:
	int check_territorys(void);
	int init_point_data(void);

	ExpeditionMapPointCfg_Map map_point_conf_map_;
	ExpeditionGangHomeCfg_Map gang_home_conf_map_;
	ExpeditionCommonCfg_Map common_conf_map_;
	Int_Hash_Set base_points_conf_;// 公会基地
	Int_IntHashSet_Map base_territorys_conf_; // conf 公会基地：不配建筑领土，程序处理
	Int_Int_Map monster_group_force_;

	ExpeditionBoxData_Vec beast_award_;
	ExpeditionBoxData_Vec chest_award_;
	Expedition_Award_Demage_Cfg demage_award_;
	Int_Item_Info_Vec_Map settle_award_;
	ExpeditionDailyAwardBigData_Map settle_award_big_;
	Int_Int_Std_Map settle_award_decay_;
	Int_Item_Info_Vec_Map demage_rank_award_;
	Int_ItemInfoVec_Map occupy_castle_rank_award_;

	Expe_WorldLevelMonsterModify_Map world_level_monster_modify_;

	ExpeditionDemageCountTime_Vec demage_count_time_cfg_;
	Int_Set demage_settle_time_cfg_;

	Expedition_Refresh_Monster_Vec refresh_monster_cfg_;
	Expedition_Refresh_Drop_Or_Material_Vec refresh_drop_or_material_cfg_;
	Int_Hash_Set drop_need_broadcast_cfg_;

	Expedition_Refresh_Monster_Kill_Gap_Vec refresh_monster_by_kill_gap_cfg_;
};
typedef Singleton<Config_Cache_Expedition> Config_Cache_Expedition_Single;
#define CONFIG_CACHE_EXPEDITION    (Config_Cache_Expedition_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_EXPEDITION_H_ */
