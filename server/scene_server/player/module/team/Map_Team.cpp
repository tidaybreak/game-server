/*
 * Scene_Map_Team.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: Linqiyou
 */


#include "Msg_Active_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Dungeon_Struct.h"
#include "Err_Code.h"
#include "Msg_Inner_Struct.h"
#include "team/Team_Struct.h"
#include "team/Team_Func.h"
#include "team/Team_Config.h"
#include "Config_Cache_Team.h"
#include "Map_Team.h"
#include "Scene_Team_Manager.h"
#include "Scene_Player.h"
#include "Config_Cache.h"
#include "Record_Client.h"
#include "Scene_Monitor.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_NPC.h"
#include "Utility_Func.h"
#include "module/team/Team_Arena_Controler.h"
#include "Global_Scene_Manager.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Item.h"
#include "equiper/Equiper_Func.h"
#include "Config_Cache_Hero.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Config_Cache_Equip.h"
#include "Battle_Scene.h"


Map_Team::Map_Team(): team_member_set_(CONFIG_CACHE_TEAM->team_config().max_player), offline_teamer_map_(CONFIG_CACHE_TEAM->team_config().max_player), offline_time_map_(CONFIG_CACHE_TEAM->team_config().max_player) {
	reset();
}

Map_Team::~Map_Team() {}

void Map_Team::reset(void) {
	scene_id_ = 0;
	password_.clear();
	type_ = 0;
	degree_ = 0;
	words_.clear();
	index_list_.clear();
	is_all_offline_ = false;
	all_offline_time_ = Time_Value::zero;
	team_index_ = 0;
	team_id_ = 0;
	team_id_for_scene_ = 0;
	leader_role_id_ = 0;
	team_member_set_.clear();
	offline_teamer_map_.clear();
	offline_time_map_.clear();
	lineup_.clear();
	in_battle_ = 0;
	last_act_time_ = Time_Value::zero;
	is_not_first_team_fb_ = false;
	//初始化位置索引，默认两位
	index_list_.push_back(1);
	index_list_.push_back(1);
	Int_Role_Id int_role_id;
	lineup_.push_back(Int_Role_Id_Vec::value_type(int_role_id));
	lineup_.push_back(Int_Role_Id_Vec::value_type(int_role_id));
	lineup_.push_back(Int_Role_Id_Vec::value_type(int_role_id));

	enter_level_ = 0;
	sports_status_ = TEAM_SPORTS_NORMAL;
	point_ = 0;
	fail_times_ = 0;
	match_fail_ = 0;
	sports_battle_level_ = 0;
}

int Map_Team::begin_match(void) {
	TEAM_ARENA_CONTROLER->add_wait_match_team(this->team_id_);
	this->sports_status_ = TEAM_SPORTS_SIGNED;
	return 0;
}

int Map_Team::cancel_match(void) {
	if (TEAM_SPORTS_SIGNED == this->sports_status_) {
		this->sports_status_ = TEAM_SPORTS_NORMAL;
//		this->unready_all();
		TEAM_ARENA_CONTROLER->del_wait_match_team(this->team_id_);
	}
	return 0;
}

Map_Team::Map_Team_Sports_Status &Map_Team::sports_status(void) {
	return this->sports_status_;
}

int Map_Team::team_fb_create_player_monster(Scene_Player &player) {
	if (!player.is_in_battle()) {
		MSG_USER("has in battle");
		return -1;
	}

	if (player.dead_status()) {
		MSG_USER("has die");
		return -1;
	}
	Battle_Scene *scene = player.battle_scene();
	if(!scene){
		return -1;
	}

	int pos = -1;
	for (Battle_Scene::Debug_Mover_Map::const_iterator it = scene->pos_debug_map().begin();
			it != scene->pos_debug_map().end(); ++it) {
		if (it->second == player.role_id()) {
			pos = it->first.pos;
			break;
		}
	}
	if (pos == -1) {
		return -1;
	}
	Int_Hash_Set idx_set;
	{
		for (Battle_Scene::Debug_Mover_Map::const_iterator it = scene->pos_debug_map().begin();
				it != scene->pos_debug_map().end(); ++it) {
			if (it->first.pos != pos) continue;
			idx_set.insert(it->first.idx);
		}
		int ret = ensure_battle_pos_unique(idx_set);
		if (0 != ret) {
			return ret;
		}
	}
	if ((pos != 0 && pos != 1) || idx_set.size() >= 3){
		return -1;
	}

	Block_Buffer buf;
	const Team_Robot_Config_Map crc = CONFIG_CACHE_TEAM->team_robot_cfg();
	int y = Position_Key::LINE_ONE_BEHIND;
	int i = 0;
	for (Team_Robot_Config_Map::const_iterator it_crc = crc.begin(); it_crc != crc.end(); ++it_crc) {
		buf.reset();
		// 机器人信息
		create_team_player_detail(it_crc->second, buf);

		get_empty_pos(idx_set, y);
		y = get_other_idx(y);
		if (0 == y) {
			continue;
		}
		idx_set.insert(y);

		NPC_Addition_Info add_info;
		add_info.birth_coord.x = pos;
		add_info.birth_coord.y = y;

		add_info.ex_val1 = it_crc->second.career;
		add_info.ex_val2 = 1; // gender
		add_info.name = it_crc->second.name;
		int monster_type = 63800108;
		Player_Monster* player_monster = NPC_MANAGER->create_player_monster(monster_type, NULL, add_info, scene, buf);
		if(player_monster == NULL){
			return -1;
		}
		Int_Vec avatar_vec;
		if (i == 0) {
			avatar_vec.push_back(10731102);
			avatar_vec.push_back(10931102);
			avatar_vec.push_back(10831102);
		} else {
			avatar_vec.push_back(10711102);
			avatar_vec.push_back(10911102);
			avatar_vec.push_back(10811102);
		}
		player_monster->set_player_monster_avatar(avatar_vec);
		player_monster->set_clear_label(false);
		player_monster->restore_hp(player_monster->blood_max(), true);

		// 战斗英雄信息
		buf.reset();
		create_team_hero_detail(it_crc->second, buf);
		player_monster->hero_battle_birth(buf, scene);
		player_monster->battle_enter_appaer(scene);
		y += 2;
		++i;
	}

//	player.restore_hp(player.blood_max(), true);
//	player.set_battle_position(0, Position_Key::LINE_TWO_FRONT);
//	player.enter_battle_scene(scene);
	return 0;
}

int Map_Team::lineup_replace(const Int_Role_Id_Vec &lineup) {
	if (lineup_check(lineup)) {
		this->lineup_.assign(lineup.begin(), lineup.end());
	} else {
		return ERROR_TEAM_LINEUP_INCORRECT;
	}
	return 0;
}

int Map_Team::lineup_clear(Scene_Player &player) {
	Int_Role_Id int_role_id;
	int_role_id.role_id = 0;
	int_role_id.int_value = 0;
	Int_Role_Id_Vec lineup(this->lineup_);
	if (this->is_role_in_lineup(player.role_id())) { // impossile
		this->lineup_.clear();
		for (Int_Role_Id_Vec::iterator it = lineup.begin(); it != lineup.end(); ++it) {
			if (it->role_id == player.role_id()) {
				this->lineup_.push_back(int_role_id);
			} else {
				this->lineup_.push_back(*it);
			}
		}
	}

	this->lineup_active();
	return 0;
}

bool Map_Team::lineup_check(const Int_Role_Id_Vec &lineup) {
	if (lineup.size() != 3) {
		return false;
	}
	for (Int_Role_Id_Vec::const_iterator it = lineup.begin(); it != lineup.end(); ++it) {
		if (it->role_id != 0 && (!this->is_in_team(it->role_id) || it->int_value < POSITION_START || it->int_value > POSITION_END)) {
			return false;
		}
	}
	return true;
}

bool Map_Team::is_role_in_lineup(role_id_t role_id) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (role_id == it->role_id) {
			return true;
		}
	}
	return false;
}

bool Map_Team::is_lineup_empty(void) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (it->role_id) {
			return false;
		}
	}
	return true;
}

Int_Role_Id *Map_Team::get_lineup(role_id_t role_id) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (it->role_id == role_id) {
			return &(*it);
		}
	}
	return 0;
}

int Map_Team::get_position(const role_id_t &role_id) {
	int i = Position_Key::LINE_ONE_FRONT;
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (it->role_id == role_id) {
			if (POSITION_FRONT != it->int_value) {
				i += 3;
			}
			return i;
		}
		++i;
	}
	return 0;
}

int Map_Team::lineup_refresh(void) {
	for (Int_Role_Id_Vec::iterator it = this->lineup_.begin(); it != this->lineup_.end(); ++it) {
		if (!this->is_in_team(it->role_id)) {
			it->role_id = 0;
			it->int_value = 0;
		}
	}
	this->lineup_active();
	return 0;
}

int Map_Team::lineup_active(void) {
//	MSG_80100319 act_msg;
//	act_msg.info = this->lineup_;
//	Block_Buffer buf;
//	buf.make_message(act_msg.msg_id);
//	act_msg.serialize(buf);
//	buf.finish_message();
//	this->info_all_online_player(act_msg.msg_id, 0, &buf);
	return 0;
}


bool Map_Team::is_team_all_offline(void) {
	if (this->offline_teamer_map().empty() && this->team_member_set().empty() && NULL == this->get_team_leader()) {
		return true;
	}
	return false;
}

int Map_Team::set_team_all_offline(void) {
	is_all_offline_ = true;
	all_offline_time_ = Time_Value::gettimeofday();
	return 0;
}

int Map_Team::set_team_not_all_offline(void) {
	is_all_offline_ = false;
	return 0;
}

void Map_Team::info_leader(int msg_id, Block_Buffer* buf) {
	Scene_Player *team_leader = get_team_leader();
	if (NULL != team_leader) {
		team_leader->send_to_logic(*buf);
	}
}

void Map_Team::info_member(int msg_id, Block_Buffer* buf) {
	Block_Buffer buf_bak;
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		Scene_Player *team_player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
		if (NULL != team_player) {
			buf_bak.reset();
			buf_bak.copy(buf);
			team_player->send_to_logic(buf_bak);
		}
	}
}

void Map_Team::info_member_without_self(int msg_id, int result, Block_Buffer* buf, role_id_t role_id) {
}

void Map_Team::create_team_player_detail(const Team_Robot_Config &tr_cfg, Block_Buffer &buf) {
	Fighter_Detail fighter_detail;
	fighter_detail.modify_normal_property(PT_LEVEL, O_SET, tr_cfg.lvl);

	if (tr_cfg.lvl > MAX_LEVEL) {
		fighter_detail.modify_normal_property(PT_LEVEL, O_SET, MAX_LEVEL);
	}

	// 等级基础属性
	const Prop_Value_Vec *prop_base_value = CONFIG_CACHE_ROLE->level_fighter_prop_cache(tr_cfg.lvl);
	if (prop_base_value) {
		for (Prop_Value_Vec::const_iterator it = prop_base_value->begin(); it != prop_base_value->end(); ++it) {
			Property_Type prop_type = Property_Type((*it).id);
			if (prop_type == PT_SKILL_POINT) continue;
			fighter_detail.modify_basic_property(prop_type, AT_BASIC, O_SET, (*it).value);
		}
	}

	// 一二级转化率
	const Player_Init_Cache &init_config = CONFIG_CACHE_ROLE->player_init_cache();
	Player_Init_Cache::Prop_Conversion::const_iterator find_it = init_config.prop_conversion.find(tr_cfg.career);
	if (find_it != init_config.prop_conversion.end()) {
		for (Player_Init_Cache::Prop_Value_Vec::const_iterator it = find_it->second.begin(); it != find_it->second.end(); ++it) {
			Property_Type prop_type = Property_Type((*it).id);
			fighter_detail.modify_percent_property(prop_type, AT_CONVERSION, O_SET, (*it).value);
		}
	}

	// 基础属性
	for (Player_Init_Cache::Prop_Value_Vec::const_iterator it_skill = init_config.initial_attribute.begin();
			it_skill != init_config.initial_attribute.end(); ++it_skill) {
		Property_Type prop_type = Property_Type((*it_skill).id);
		fighter_detail.modify_basic_property(prop_type, AT_BASIC, O_SET, (*it_skill).value);
	}
	// 装备处理 base_prop_refresh(item_config, pro_val_map);
	std::map<int, double> pro_val_map;
	for (Int_Hash_Set::const_iterator it_equip = tr_cfg.equip.begin();
			it_equip != tr_cfg.equip.end(); ++it_equip) {
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(*it_equip);
		if (! item_config) {
			continue;
		}

		// 装备基础属性
		const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
		if (! cfg_prop_val_vec.empty()) {
			for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
				pro_val_map[cfg_prop_val_vec[i].id] += cfg_prop_val_vec[i].value;
			}
		}

		const Equip_Unban_Lvl_Cfg *unban_lvl_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_cfg(20);
		if (unban_lvl_cfg && unban_lvl_cfg->add_prop_rate > 0) {
			const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
			if (! cfg_prop_val_vec.empty()) {
				for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
					int prop = cfg_prop_val_vec[i].id;
					double base_val = cfg_prop_val_vec[i].value;
					double add_val = base_val * unban_lvl_cfg->add_prop_rate * 0.01;
					pro_val_map[prop] += add_val;
				}
			}
		}
	}
	// player_self()->addition_prop_refresh(AT_EQUIP, PST_BASIC, pro_val_map);
	Prop_Setter setter;
	Prop_Setter_Vec setter_vec;
	bool pst_basic = true;
	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		setter.reset();
		setter.prop_type = static_cast<Property_Type>(*it_prop);
		setter.operate = O_SET;
		setter.addi_type = AT_EQUIP;
		setter.basic = pst_basic ? get_map_second_by_key(*it_prop, pro_val_map) : 0.0;
//					setter.fixed = pst_fixed  ? get_map_second_by_key(*it_prop, pro_val_map) : 0.0;
//					setter.percent = pst_percent  ? get_map_second_by_key(*it_prop, pro_val_map) : 0.0;
		setter_vec.push_back(setter);
	}
	if (! setter_vec.empty()) {
		for(uint16_t i = 0; i < setter_vec.size(); ++i) {
			fighter_detail.modify_fight_property(setter_vec[i].prop_type, setter_vec[i].addi_type,
					setter_vec[i].operate, setter_vec[i].basic, setter_vec[i].fixed, setter_vec[i].percent);
		}
	}
	// 装备处理 end

	// 技能初始化 init_skill(init_config);
	{
		Player_Init_Cache::Role_Default_Skill_Map::const_iterator role_skill_it =
				init_config.default_skill.find(tr_cfg.career);
		if (role_skill_it != init_config.default_skill.end()) {
			Player_Init_Cache::Default_Skill_Map::const_iterator find_it =
					role_skill_it->second.find(1);
			if (find_it != role_skill_it->second.end()) {
				fighter_detail.default_skill = find_it->second.val_1;

				// 技能初始化
				if (fighter_detail.talent_map.empty()) {
					Talent talent;
					talent.reset();
					talent.talent_id = 1;
					talent.skill_point = fighter_detail.ttl_skill_points_;

					Skill_DB_Info skill;
					skill.skill_id = find_it->second.val_1;
					skill.skill_level = 1;
					skill.skill_loc = 0;
					talent.skill_db_info_map[skill.skill_id] = skill;

					skill.reset();
					skill.skill_id = find_it->second.val_2;
					skill.skill_level = 1;
					skill.skill_loc = 1;
					talent.skill_db_info_map[skill.skill_id] = skill;

					fighter_detail.talent_map[talent.talent_id] = talent;
					fighter_detail.cur_used_talent_ = 1;
				}
			}
		}
		// 升级加技能点
		int skill_point = CONFIG_CACHE_ROLE->level_up_add_skill_points(tr_cfg.lvl);
		if ( skill_point > 0) {
			fighter_detail.ttl_skill_points_ += skill_point;
			for (Talent_Map::iterator it = fighter_detail.talent_map.begin();
					it != fighter_detail.talent_map.end(); ++it) {
				it->second.skill_point += skill_point;
			}
		}

		for (Int_Hash_Set::const_iterator it_skill = tr_cfg.skill.begin(); it_skill != tr_cfg.skill.end(); ++it_skill) {
			int skill_id = *it_skill;
			int skill_lvl = 1;
			const Skill_Config * scfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
			const Skill_Detail * cfg = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lvl);
			if (!scfg || !cfg) continue;

			Talent_Map::iterator find_it = fighter_detail.talent_map.find(scfg->skill_series);
			if (find_it == fighter_detail.talent_map.end()) continue;
			if (find_it->first != scfg->skill_series && 0 != scfg->skill_series) continue;
			Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.find(skill_id);
			if (it == find_it->second.skill_db_info_map.end()) {
				int loc = 0;
				if (Skill_Type::TYPE_UNIQUE_SKILL == scfg->skill_type) {
					loc = 5;
					for (it = find_it->second.skill_db_info_map.begin();
							it != find_it->second.skill_db_info_map.end(); ++it) {
						if (it->second.skill_loc == loc) {
							loc = 0;
							break;
						}
					}
				} else if (Skill_Sort::ACT_SKILL == scfg->skill_sort) {
					bool loc_vec[] = {false, false, false, false, false};
					for (Skill_DB_Info_Map::const_iterator it = find_it->second.skill_db_info_map.begin();
							it != find_it->second.skill_db_info_map.end(); ++it) {
						if (it->second.skill_loc != 0 && it->second.skill_loc != 5){
							if ((it->second.skill_loc > 4) || (it->second.skill_loc < 1)) {
								MSG_USER("skill index error");
							} else {
								loc_vec[it->second.skill_loc] = true; //此位置存在技能
							}
						}
					}
					for (int i=1; i < 5; ++i) {
						if (loc_vec[i] == false) { //此位置未放置技能
							loc = i;
							break;
						}
					}
				}
				Skill_DB_Info info;
				info.skill_id = skill_id;
				info.skill_level = skill_lvl;
				info.skill_loc = loc;
				find_it->second.skill_db_info_map.insert(std::make_pair(skill_id, info));
			} else {
				it->second.skill_level = skill_lvl;
			}
		}
	}
//	fighter_detail.update_force();
	fighter_detail.serialize(buf);
}
void Map_Team::create_team_hero_detail(const Team_Robot_Config &tr_cfg, Block_Buffer &buf) {
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(tr_cfg.hero_id);
	if (!hero_config) {
		buf.write_int8(0);
		return ;
	}

	Hero_Info_Detail hero_info_detail;
	hero_info_detail.hero_index = 1;
	hero_info_detail.hero_id = tr_cfg.hero_id;
	hero_info_detail.hero_facade_id = tr_cfg.hero_id;
	hero_info_detail.hero_name = hero_config->hero_name;
	hero_info_detail.level = tr_cfg.hero_lvl;
	hero_info_detail.formation = 5;
	hero_info_detail.awake_lvl = tr_cfg.hero_awake_lvl;

	// 一二级转化率
	for (Int_Double_Map::const_iterator it_prop_val = hero_config->hero_prop_conver.begin();
			it_prop_val != hero_config->hero_prop_conver.end(); ++it_prop_val) {
		hero_info_detail.hero_fighter_detail.modify_percent_property(Property_Type(it_prop_val->first),
				AT_CONVERSION, O_SET, it_prop_val->second);
	}

	// heroer_prop_refresh(it_hero->second.hero_index, syn_scene, false);
	{
		for (int addition_type = AT_BASIC; addition_type < AT_END; ++addition_type) {
			if (addition_type == AT_CONVERSION || addition_type == AT_PASSIVE_SKILL) {
				continue;
			}
			for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
				hero_info_detail.hero_fighter_detail.modify_fight_property((Property_Type)(*it_prop), (Addition_Type)addition_type, O_SET, 0.0, 0.0, 0.0);
				if (PT_BLOOD_MAX == *it_prop) {
					hero_info_detail.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, (Addition_Type)addition_type, O_SET, 0.0, 0.0, 0.0);
				}
			}
		}

		// 英雄等级属性 level_prop_refresh(it_hero->second, hero_config);
		{
			std::map<int, double> pro_val_map;
			const Int_Double_Map *hero_lvl_prop = CONFIG_CACHE_HERO->find_hero_lvl_prop(hero_info_detail.level);
			for (Int_Double_Map::const_iterator it_lvl_pro = hero_lvl_prop->begin();
					it_lvl_pro != hero_lvl_prop->end(); ++it_lvl_pro) {
				int prop = it_lvl_pro->first;
				double value = it_lvl_pro->second;
				if (it_lvl_pro->first > 0 && it_lvl_pro->second > 0.0) {
					pro_val_map[prop] += value;
				}
			}

			// 属性修正（改为只修正英雄等级属性）correct_prop_refresh(hero_info_detail, pro_val_map, hero_config);
			if (! pro_val_map.empty()) {
				for (std::map<int, double>::iterator it_pro_val_map = pro_val_map.begin();
						it_pro_val_map != pro_val_map.end(); ++it_pro_val_map) {
					if (it_pro_val_map->first && it_pro_val_map->second > 0.0) {
						Int_Double_Map::const_iterator it_correct_val = hero_config->hero_prop_correct.find(it_pro_val_map->first);
						if (it_correct_val != hero_config->hero_prop_correct.end()) {
							it_pro_val_map->second *= it_correct_val->second;
						} else {
							it_pro_val_map->second = 0;
						}
					}
				}
			}

			// 属性加成 hero_modify_fight_property(hero_info_detail, AT_BASIC, PST_BASIC, pro_val_map);
			bool pst_basic = true, pst_fixed = false, pst_percent = false;
			for (std::map<int, double>::const_iterator iter = pro_val_map.begin(); iter != pro_val_map.end(); ++iter) {
				if (iter->first) {
					double basic = pst_basic ? iter->second : 0.0;
					double fixed = pst_fixed ? iter->second : 0.0;
					double percent = pst_percent ? iter->second : 0.0;
					hero_info_detail.hero_fighter_detail.modify_fight_property((Property_Type)iter->first, AT_BASIC, O_SET, basic, fixed, percent);
				}
			}
		}

		// 装备属性 equip_prop_refresh(it_hero->second);
		{
			std::map<int, double> pro_val_map;
			for (Int_Hash_Set::const_iterator it_equip = tr_cfg.hero_equip.begin();
					it_equip != tr_cfg.hero_equip.end(); ++it_equip) {
				const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(*it_equip);
				if (! item_config) {
					continue;
				}

				// 装备基础属性
				const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
				if (! cfg_prop_val_vec.empty()) {
					for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
						pro_val_map[cfg_prop_val_vec[i].id] += cfg_prop_val_vec[i].value;
					}
				}
			}
			// 属性加成hero_modify_fight_property(hero_info_detail, AT_EQUIP, PST_BASIC, pro_val_map);
			Prop_Setter setter;
			Prop_Setter_Vec setter_vec;
			bool pst_basic = true, pst_fixed = false, pst_percent = false;
			for (std::map<int, double>::const_iterator iter = pro_val_map.begin(); iter != pro_val_map.end(); ++iter) {
				if (iter->first) {
					double basic = pst_basic ? iter->second : 0.0;
					double fixed = pst_fixed ? iter->second : 0.0;
					double percent = pst_percent ? iter->second : 0.0;
					hero_info_detail.hero_fighter_detail.modify_fight_property((Property_Type)iter->first, AT_EQUIP, O_SET, basic, fixed, percent);
				}
			}
		}

//					// 神器属性
//					artifacts_prop_refresh(it_hero->second);
//
//					// 龙谷属性
//					dragon_vale_prop_refresh(it_hero->second);
//
//					// 符石属性
//					rune_stone_prop_refresh(it_hero->second);
//
//					// 公会技能属性
//					gang_skill_hero_prop_refresh(it_hero->second);
//
//					// 符文属性
//					if (it_hero->first == heroer_detail_->fighter_index) {
//						hero_rune_prop_refresh(it_hero->second);
//					}
//
//					// 战斗精灵属性加成
//					covenant_prop_refresh(it_hero->second);

		// ---------------------- 属性加成 END -------------------------

		// 更新：更新英雄战力
//		hero_info_detail.hero_fighter_detail.update_force();

		// 刷新当前血量
		double blood_max = hero_info_detail.hero_fighter_detail.fetch_fight_property(PT_BLOOD_MAX);
		hero_info_detail.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, blood_max, 0.0, 0.0);
	}

	// heroer_skill_refresh(hero_info_detail.hero_index, syn_scene, false);
	{

		// -----------Reset hero skills START --------------------------------------------
		hero_info_detail.hero_fighter_detail.talent_map.clear();
		hero_info_detail.hero_fighter_detail.cur_used_talent_ = 0;
		hero_info_detail.hero_fighter_detail.default_skill = 0;

		// reset skill prop
		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			hero_info_detail.hero_fighter_detail.modify_fight_property((Property_Type)(*it_prop), AT_PASSIVE_SKILL, O_SET, 0.0, 0.0, 0.0);
			if (PT_BLOOD_MAX == *it_prop) {
				hero_info_detail.hero_fighter_detail.modify_fight_property(PT_BLOOD_CURRENT, AT_PASSIVE_SKILL, O_SET, 0.0, 0.0, 0.0);
			}
		}

		// -----------Reset hero skills END ----------------------------------------------

		//int player_level = player_self()->level();
		int skill_loc = 6;
		int default_skill = 0;

		Talent talent;
		talent.talent_id = 1;
		Skill_DB_Info skill_db_info;

		// 英雄等级技能
		for (uint16_t i_skill_id_vec = 0; i_skill_id_vec < hero_config->skill_id_vec.size(); ++i_skill_id_vec) {
			int skill_id = hero_config->skill_id_vec[i_skill_id_vec];
			const Skill_Config *skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
			if (! skill_config) {
				continue;
			}

			int skill_level = 0;
			for (Skill_Detail_Map::const_iterator it_skill_lvl_map = skill_config->skill_level_map.begin();
					it_skill_lvl_map != skill_config->skill_level_map.end(); ++it_skill_lvl_map) {
				if (it_skill_lvl_map->second.skill_level > skill_level &&
						hero_info_detail.level >= it_skill_lvl_map->second.lv_lim) {
					skill_level = it_skill_lvl_map->second.skill_level;
				}
			}
			if (! skill_level) {
				continue;
			}
			default_skill = !default_skill ? skill_id : default_skill;
			skill_db_info.reset();
			skill_db_info.skill_id = skill_id;
			skill_db_info.skill_level = 1;

			if (skill_config->skill_type == TYPE_ADD_SKILL) {
				skill_db_info.skill_loc = 9;	// 追加技能放最后位置
			} else {
				skill_db_info.skill_loc = (skill_id == default_skill) ? 0 : skill_loc++;
			}

			talent.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
		}

		// 英雄觉醒技能
		uint16_t awake_start_index = 0;
		if(hero_info_detail.awake_lvl > 4){
			awake_start_index = hero_info_detail.awake_lvl - 4;
		}
		for (uint16_t i_skill_id_vec = 0;
				i_skill_id_vec < hero_info_detail.awake_lvl && i_skill_id_vec < hero_config->awake_skill_id_vec.size();
				++i_skill_id_vec) {
			if(i_skill_id_vec < awake_start_index){
				continue;
			}
			int skill_id = hero_config->awake_skill_id_vec[i_skill_id_vec];
			const Skill_Config *skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
			if (! skill_config) {
				continue;
			}

			int skill_level = 0;
			for (Skill_Detail_Map::const_iterator it_skill_lvl_map = skill_config->skill_level_map.begin();
					it_skill_lvl_map != skill_config->skill_level_map.end(); ++it_skill_lvl_map) {
				if (it_skill_lvl_map->second.skill_level > skill_level &&
						hero_info_detail.level >= it_skill_lvl_map->second.lv_lim) {
					skill_level = it_skill_lvl_map->second.skill_level;
				}
			}
			if (! skill_level) {
				continue;
			}
			default_skill = !default_skill ? skill_id : default_skill;

			skill_db_info.reset();
			skill_db_info.skill_id = skill_id;
			skill_db_info.skill_level = skill_level;
			skill_db_info.skill_loc = 0;

			talent.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
		}
		hero_info_detail.hero_fighter_detail.talent_map[talent.talent_id] = talent;
		hero_info_detail.hero_fighter_detail.cur_used_talent_ = talent.talent_id;
		hero_info_detail.hero_fighter_detail.default_skill = default_skill;

		hero_info_detail.hero_fighter_detail.set_be_skill_attr();
	}
	buf.write_int8(1);

	Hero_Base_Detail base_detail;
	base_detail.master_role_id = 0;		// 主人id
	base_detail.master_role_name = tr_cfg.name;	// 主人名称
	base_detail.hero_index = hero_info_detail.hero_index;					// 英雄index
	base_detail.hero_id = hero_info_detail.hero_id;					// 英雄ID
	base_detail.hero_name = hero_info_detail.hero_name;			// 英雄name
	base_detail.hero_facade_id = hero_info_detail.hero_id;				// 英雄facade_id
	base_detail.hero_formation = hero_info_detail.formation;				// 英雄阵型
	base_detail.hero_level = hero_info_detail.level;					// 英雄等级
	base_detail.awake_lvl = hero_info_detail.awake_lvl;					// 觉醒等级
	base_detail.serialize(buf);

	hero_info_detail.hero_fighter_detail.serialize(buf);
}

Scene_Player *Map_Team::team_leader(void) const {
	if (0 == leader_role_id_) {
		return NULL;
	} else {
		return SCENE_MONITOR->find_scene_player_by_role_id(leader_role_id_);
	}
}

Scene_Player *Map_Team::get_team_leader(void) {
	if (0 != leader_role_id_) {
		return SCENE_MONITOR->find_scene_player_by_role_id(leader_role_id_);
	} else {
		return NULL;
	}
}

bool Map_Team::is_leader_online(void) const {
	if (0 != leader_role_id_) {
		return (0 != SCENE_MONITOR->find_scene_player_by_role_id(leader_role_id_));
	} else {
		return false;
	}
}

int Map_Team::check_all_member_ready(void) {
	uint32_t member_min_limit = CONFIG_CACHE_TEAM->team_config().max_player;//CONFIG_INSTANCE->island_config()["min_member"].asUInt();
	if (member_min_limit < 1) {
		member_min_limit = 3;
	}
	if (team_member_set_.size() < member_min_limit - 1) {
		return ERROR_MEMBER_NOT_ENOUGH;
	}

	for (Role_Hash_Set::const_iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(*it);
		if (player) {
			if (!player->team_get_ready_status()) {
				return ERROR_TEAMMER_NOT_READY;
			}
		} else {
			return ERROR_TEAMMER_NOT_READY;
		}
	}
	Scene_Player *leader = this->get_team_leader();
	if (NULL == leader || !leader->team_get_ready_status()) {
		return  ERROR_TEAMMER_NOT_READY;
	}
	return 0;
}

int Map_Team::get_team_score(void) {
	int score = TEAM_ARENA_CONTROLER->get_role_score(leader_role_id_);
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		score += TEAM_ARENA_CONTROLER->get_role_score(*it);
	}

	return score/(1 + team_member_set_.size());
}

int &Map_Team::scene_id(void) {
	return this->scene_id_;
}

std::string &Map_Team::password(void) {
	return this->password_;
}

int8_t &Map_Team::type(void) {
	return this->type_;
}

std::string &Map_Team::words(void) {
	return this->words_;
}

int8_t &Map_Team::degree(void) {
	return this->degree_;
}

Int8_Vec &Map_Team::index_list(void) {
	return this->index_list_;
}

int8_t &Map_Team::in_battle(void) {
	return this->in_battle_;
}

Time_Value &Map_Team::last_act_time(void) {
	return this->last_act_time_;
}

const role_id_t &Map_Team::leader_role_id(void) {
	return this->leader_role_id_;
}

int Map_Team::set_leader_role_id(const role_id_t &role_id) {
	return this->leader_role_id_ = role_id;
}

const int &Map_Team::enter_level(void) const{
	return this->enter_level_;
}

int Map_Team::set_enter_level(const int lvl) {
	this->enter_level_ = lvl;
	return 0;
}

bool Map_Team::is_not_first_team_fb(void) {
	return this->is_not_first_team_fb_;
}

void Map_Team::info_all_online_player(int msg_id, Block_Buffer* buf) {
	Block_Buffer buf_bak;
	buf_bak.copy(buf);
	info_leader(msg_id, &buf_bak);
	buf_bak.reset();
	buf_bak.copy(buf);
	info_member(msg_id, &buf_bak);
}

Map_Team::RoleID_TimeValue_Map &Map_Team::offline_time_map(void) {
	return this->offline_time_map_;
}

Map_Team::RoleID_OnlineMap_TeamerInfo_Map &Map_Team::offline_teamer_map(void) {
	return this->offline_teamer_map_;
}

Int_Role_Id_Vec &Map_Team::lineup(void) {
	return this->lineup_;
}

const int32_t &Map_Team::point(void) const {
	return this->point_;
}

int Map_Team::add_point(const int32_t point) {
	if (point > 0) {
		this->point_ += point;
	}
	return 0;
}

int16_t &Map_Team::fail_times(void) {
	return this->fail_times_;
}

int16_t &Map_Team::match_fail(void) {
	return this->match_fail_;
}

int32_t &Map_Team::sports_battle_level(void) {
	return this->sports_battle_level_;
}

void Map_Team::serialize(Block_Buffer &buf) {
	buf.write_int8(type_);
	buf.write_string(words_);
	buf.write_int32(scene_id_);
	buf.write_int8(degree_);
	buf.write_uint32(team_index_);
	buf.write_int64(team_id_);
	buf.write_int64(team_id_for_scene_);
	buf.write_int64(leader_role_id_);
	uint16_t len = 0;
	len = team_member_set_.size();
	buf.write_uint16(len);
	for (Role_Hash_Set::iterator it = team_member_set_.begin(); it != team_member_set_.end(); ++it) {
		buf.write_int64(*it);
	}
	len = offline_teamer_map_.size();
	buf.write_uint16(len);
	for (RoleID_OnlineMap_TeamerInfo_Map::iterator it = offline_teamer_map_.begin(); it != offline_teamer_map_.end(); ++it) {
		buf.write_int64(it->first);
	}
	buf.write_int8(in_battle_);
	len = lineup_.size();
	buf.write_uint16(len);
	for (Int_Role_Id_Vec::iterator it = lineup_.begin(); it != lineup_.end(); ++it) {
		it->serialize(buf);
	}
	buf.write_int32(enter_level_);
	buf.write_int16(fail_times_);
	buf.write_int16(match_fail_);
	buf.write_bool(is_not_first_team_fb_);
}

void Map_Team::deserialize(Block_Buffer &buf) {
	buf.read_int8(type_);
	buf.read_string(words_);
	buf.read_int32(scene_id_);
	buf.read_int8(degree_);
	buf.read_uint32(team_index_);
	buf.read_int64(team_id_);
	buf.read_int64(team_id_for_scene_);
	buf.read_int64(leader_role_id_);
	uint16_t len = 0;
	buf.read_uint16(len);
	int64_t role_id = 0;
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(role_id);
		team_member_set_.insert(role_id);
	}
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(role_id);
		offline_teamer_map_.insert(std::make_pair(role_id, Online_Teamer_Info()));
	}
	buf.read_int8(in_battle_);
	buf.read_uint16(len);
	Int_Role_Id int_role_id;
	Int_Role_Id_Vec lineup;
	for (uint16_t i = 0; i < len; ++i) {
		int_role_id.deserialize(buf);
		lineup.push_back(int_role_id);
	}
	this->lineup_replace(lineup);
	buf.read_int32(enter_level_);
	buf.read_int16(fail_times_);
	buf.read_int16(match_fail_);
	buf.read_bool(is_not_first_team_fb_);
}

bool compare_less(const Map_Team &team1, const Map_Team &team2) {
	if (team1.enter_level() < team2.enter_level()) {
		return true;
	} else if (team1.enter_level() == team2.enter_level()
			&& team1.get_unused_position() < team2.get_unused_position()) {
		return true;
	} else if (team1.enter_level() == team2.enter_level()
			&& team1.get_unused_position() == team2.get_unused_position()
			&& team1.get_team_index() < team2.get_team_index()) {
		return true;
	}
	return false;
}

bool Map_Team_PtrGreater::operator ()(const Map_Team* team1, const Map_Team* team2) const {
	if (team1->point() > team2->point()) {
		return true;
	} else if (team1->point() < team2->point()) {
		return false;
	}

	return team1->get_team_id() > team2->get_team_id();
}

int Map_Team::sync_info_logic(void) {
	MSG_20100304 inner_msg;
	Block_Buffer buf;
	Time_Value now;
	now = Time_Value::gettimeofday();
	buf.make_message(inner_msg.msg_id);
	buf.write_int64(team_id_);
	buf.write_int16(fail_times_);
	buf.write_int16(match_fail_);
	now.serialize(buf);
	buf.finish_message();
	this->info_all_online_player(inner_msg.msg_id, &buf);
	return 0;
}


