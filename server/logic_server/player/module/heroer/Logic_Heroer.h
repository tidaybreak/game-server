/*
 * Logic_Heroer.h
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#ifndef LOGIC_HEROER_H_
#define LOGIC_HEROER_H_

#include "Logic_Player_Base.h"
#include "Logic_Heroer_Def.h"
#include "fighter/Fighter_Def.h"

//class Packer;
class Equiper;
struct Hero_Base_Detail;
struct Heroer_Detail;
struct Hero_Info;
struct Hero_Info_Detail;
struct Hero_Config;
class Logic_Player;
struct Status_Effect_Argv;
struct Status_Info;
struct Btl_Buff_Cfg;
struct Status_Effect_Detail;
struct Status_Info_Detail;

class Logic_Heroer: virtual public Equiper {
public:
	Logic_Heroer(void);
	virtual ~Logic_Heroer();
	void reset(void);

	static int create_init(Heroer_Detail &detail);

	void load_detail(Heroer_Detail &detail);
	void save_detail(Heroer_Detail &detail);

	void sign_in(void);
	void sign_out(void);

	void module_init(void);
	int client_ready_now(void);
	int move_scene_completed(void);
	int enter_move_scene(void);
	void hero_time_up(const Time_Value &now);
	int trigger_daily_six(const Time_Value &now);

	// MSG start ===================================
	int req_hero_amount(void);
	int req_fetch_heroer_info(void);																	// 获取玩家英雄信息
	int req_hero_level_up(const int32_t hero_index, const bool without_condition = false);													// 英雄升级
	int req_hero_equip_on_off(const int32_t index_from, const int32_t index_to);						// 英雄装备穿脱
	// 设置英雄展示状态
	int req_set_hero_out_state(int8_t is_out);
	int req_hero_out(const int32_t hero_index, const int8_t is_out, bool alert_res = true, bool transfer = false);	// 英雄出战
	int req_hero_recruit(const int32_t hero_id, const int8_t use_diamond);											// 英雄招募
	int req_hero_set_formation(const int32_t hero_index, const int32_t hero_formation, bool is_alert = true);		// 英雄设置阵型
	int req_hero_set_fight(const int32_t hero_index, const int8_t is_fight, bool alert_res = true, bool transfer = false);// 设置战斗英雄
	int req_hero_awake(const int8_t is_hero, const int32_t hero_index, const bool without_condition = false);									// 英雄觉醒
	int req_artifact_upgrade(const int8_t is_hero, const int32_t artifact_id, const int32_t hero_index);// 神器升级
	int req_hero_rune_stone_on_off(const int index_from, const int index_to);
	int req_hero_rune_stone_move(const int index_from, const int index_to);
	int req_new_hero_click(const int hero_id);
	int req_fetch_clicked_new_heros(void);

	int active_hero_info(const int hero_index, int active_type = 0);
	int active_panal_hero_info(Logic_Player* target_player);
	int active_panal_hero_tags(Logic_Player* target_player);
	void active_hero_int_info_change(void);
	// MSG end =====================================

	int hero_add(const int hero_id, bool use_diamond = false, const bool send_client = false);
	int hero_erase(const int hero_index);

	int hero_swap_equips(const int32_t hero_idx_from, const int32_t hero_idx_to);
	int hero_swap_equip(const int32_t pack_idx_from, const int32_t pack_idx_to);

	int update_hero_sync_info(Hero_Info_Detail &hero_info_detail);						//更新英雄同步信息
	int sync_hero_base_detail_to_scene(const int detail_type);
	int sync_hero_fighter_detail_to_scene(const int detail_type, const Fighter_Detail &hero_fighter_detail);
	int sync_hero_out_to_scene(int is_out);
	int sync_hero_fight_to_scene(int is_fight);
	int sync_hero_detail_to_scene(const int detail_type);
	int sync_heroer_module_info(Block_Buffer &buf);

	int fight_heroer_prop_refresh(bool sync_scene = true, bool sync_client = true);
	int fight_heroer_skill_refresh(bool sync_scene = true, bool sync_client = true);
	int heroer_prop_refresh(const int hero_index, bool sync_scene = true, bool sync_client = true);
	int heroer_skill_refresh(const int hero_index, bool sync_scene = true, bool sync_client = true, bool init_be_skill = true);
	void heroer_assist_skill_refresh(const Hero_Config &cfg, Hero_Info_Detail &hero);
	int heroer_prop_refresh(void);


	int refresh_heroer_base_detail(void);
	int refresh_heroer_fight_detail();

	void refresh_artifacts_pro_val(void);	// 刷新所有神器属性值总和

	/*
	 * outer interfaces
	 */
	int heroer_get_outer_hero_formation(void);
	int heroer_get_hero_index(const int hero_id);
	int heroer_get_hero_id(const int hero_index);
	int get_fight_hero_lvl_limit(const int hero_index);
	int get_fight_hero_lvl(const int hero_index);
	int get_heros_lvl(Int_Int_Map &id_lvl_map);
	int get_heros_awake_lvl(Int_Int_Map &id_lvl_map);
	int get_fight_hero_force(void);
	int get_outer_hero_awake_lvl(void);
	int get_hero_rune_pro_val(std::map<int, double> &pro_val_map) const;

	// status------------------------
	int hero_status_load_init(void);
//	int hero_insert_status(Block_Buffer &buf);
	int handle_hero_status_info_from_scene(Block_Buffer &buf);
	int hero_insert_status(Status_Effect_Argv &argv, const bool from_scene = false);
//	int hero_insert_status(std::vector<Status_Effect_Argv> &argv);
	int hero_insert_status(Status_Info &detail);
	int hero_erase_status_type(const int status_id, const bool from_scene = false);
	int hero_sync_status_add(Status_Effect_Argv &argv);
	int hero_sync_status_erase(const Int_Vec status_id);
	int hero_merge_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from,const Btl_Buff_Cfg *status_config);
	int hero_merge_status_to_status_overlay(Status_Info &status_info_to, Status_Info &status_info_from, bool has_overlay);
	int hero_cover_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from);
	//效果操作
	int hero_insert_status_effect_handle(Status_Info_Detail &status_info, Status_Effect_Detail &effect);
	int hero_opt_status_effect_handle_by_scene_change(Status_Info_Detail &status_info, Status_Effect_Detail &effect, const int des_scene);
//	int hero_insert_status_effect_handle(Status_Effect_Detail &effect);//间隔为0的状态的延迟处理
//	int hero_insert_status_effect_handle(Status_Effect_Detail &effect, const int scene);//转场景处理
	int hero_update_status_effect_handle(Status_Info_Detail &status_info, Status_Info_Detail &status_info_from,
			Status_Effect_Detail &old_effect, Status_Effect_Detail &new_effect);
//	int hero_opt_status_effect_handle(Status_Effect_Detail &effect, const Time_Value &now_time,
//			int skill_id = 0, Role_Vec role_vec = Role_Vec());//会导致死亡，清除status_map,所以操作完要检查
	int hero_erase_status_effect_handle(Status_Effect_Detail &effect);
	bool hero_check_status_effect(Status_Effect_Detail &effect); // 检测buff场景设置
	bool hero_check_status_effect(Status_Effect_Detail &effect, const int scene); // 检测buff场景设置
	//效果操作
	// 加属性
	int hero_add_status_prop(Status_Effect_Detail& fix, int skill_id = 0);
	int hero_sub_status_prop(Status_Effect_Detail& fix);
	//直接计算前后effect差值
	int hero_cover_status_prop(Status_Effect_Detail& fix, Status_Effect_Detail& fix_from);
	int hero_add_status_prop_by_per(Status_Effect_Detail& fix, int skill_id = 0);
	int hero_hook_scene_change(const int src_scene, const int des_scene);
	// status------------------------

	inline int get_hero_index_by_pack_index(const int index);
	inline bool is_hero_index(const int hero_index);
	bool has_hero_index(const int hero_index);
	bool has_hero_id(const int hero_id);
	int test_awake_lvl_up(const int8_t is_hero, const int32_t hero_index);
	bool is_fini_tasks(const int32_t hero_id, const Int_Vec &tasks);

	int test_prop(const int hero_index, const int prop_type, const double prop_val);

	const std::map<int, double>& artifacts_pro_val(void);

	inline const Heroer_Detail &heroer_detail(void);
	inline Heroer_Detail& heroer_detail_no_const();

	//
	int hero_fight_prop_modify(const std::vector<Prop_Setter> &setter_vec);

	//获得英雄品质对应数量的map
	void get_hero_quality_num_map(std::map<int32_t, uint32_t> &hero_quality_num_map);

	//
	int req_inner_add_and_fight(const int hero_id);

	int req_inner_erase_recycle(const int hero_id);
	// 跟随英雄
	int get_hero_out_index(void);
	// 出战英雄
	int get_hero_fight_index(void);
	// 设置英雄展示状态
	void set_hero_out_state(int state);

	// 伙伴助阵
	void update_hero_assist_info(Hero_Info_Detail &hero);
	int handle_hero_assist(int type, int hero_idx, int loc);
	int hero_assist_on(int hero_idx, int loc);
	int hero_assist_off(int hero_idx, int loc);
	int handle_hero_community(int id, int answer, int hero_idx);
	int hero_intimacy_upgrade(Hero_Info_Detail &hero, const Hero_Config &cfg_hero);
	int handle_gift_hero(int item_id, int hero_idx);

	int pre_refresh_hero_assist(Int_Double_Map &props, Int_Int_Vec &old_skills);
	int pos_refresh_hero_assist(Int_Double_Map &props, Int_Int_Vec &old_skills);

	void refresh_outer_fight_hero_assist_prop(Hero_Info_Detail &hero);

protected:
	int master_whole_prop_refresh(Hero_Info_Detail &hero_info_detail, const int master_whole_lvl, std::map<int, double> &pro_val_ma);

private:
	bool is_fini_recruit_task(const int32_t hero_id, const Hero_Config *hero_config);
	bool is_awake_limit(const Hero_Info_Detail &hero_info_detail);
	int get_level_limit(const Hero_Info_Detail &hero_info_detail);
	int get_detail_type(Hero_Info_Detail &hero_info_detail);

	int level_prop_refresh(Hero_Info_Detail &hero_info_detail, const Hero_Config *hero_config);
	int awake_prop_refresh(Hero_Info_Detail &hero_info_detail, const Hero_Config *hero_config);
	int equip_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int artifacts_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int correct_prop_refresh(Hero_Info_Detail &hero_info_detail, std::map<int, double> &pro_val_map, const Hero_Config *hero_config);
	int dragon_vale_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int rune_stone_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int gang_skill_hero_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int hero_rune_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int hero_modify_fight_property(Hero_Info_Detail &hero_info_detail, Addition_Type addi_type, Prop_Setter_Type pst,
			const std::map<int, double> &pro_val_map);
	int status_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int anci_artifact_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int wing_prop_refresh(Hero_Info_Detail &hero_info_detail);
	int pet_prop_refresh(Hero_Info_Detail &hero_info_detail);

	void init_artifacts(void);
	static void init_artifacts(Heroer_Detail &detail);
	static void init_artifacts(Hero_Info_Detail &hero_info_detail);
	void refresh_artifacts_pro_val(Hero_Info_Detail &hero_info_detail);

	void init_panal_tags(void);
	void refresh_panal_tags(void);

	int set_hero_rune_pro_val(const std::map<int, double> &pro_val_map);

	int hero_equip_skill_refresh(Hero_Info_Detail &hero_info_detail);

	// 坐骑技能
	int mount_skill_refresh(Hero_Info_Detail &hero_info_detail);
	// 坐骑属性
	int mount_prop_refresh(Hero_Info_Detail &hero_info_detail);
private:
    Heroer_Detail *heroer_detail_;				// 玩家英雄信息
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Heroer_Detail &Logic_Heroer::heroer_detail(void) {
	return *heroer_detail_;
}

inline int Logic_Heroer::get_hero_index_by_pack_index(const int index) {
	int hpm = pack_get_hpm(index);
	switch (hpm) {
	case HPM_EQUIP_INDEX:
		return (index - PACK_T_HERO_EQUIP_INDEX) / ONE_HERO_EQUIP_GAP;
	case HPM_RUNE_STONE_EQUIP_INDEX:
		return (index - PACK_T_HERO_RUNE_STONE_EQUIP_INDEX) / ONE_HERO_EQUIP_GAP;
	default:
		return 0;
	}
}

inline bool Logic_Heroer::is_hero_index(const int hero_index) {
	return HERO_INDEX_BEGIN <= hero_index && hero_index < HERO_INDEX_END;
}

inline Heroer_Detail& Logic_Heroer::heroer_detail_no_const() {
	return *heroer_detail_;
}

#endif /* LOGIC_HEROER_H_ */
