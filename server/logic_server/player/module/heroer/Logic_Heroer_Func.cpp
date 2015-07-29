/*
 * Logic_Heroer_Func.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "equiper/Equiper_Def.h"
#include "equiper/Equiper.h"
#include "equiper/Equiper_Struct.h"
#include "equiper/Equiper_Func.h"
#include "Config_Cache_Hero.h"
#include "Config_Cache_Fighter.h"
#include "Logic_Heroer_Def.h"
#include "Logic_Heroer.h"
#include "Logic_Heroer_Struct.h"
#include "Logic_Heroer_Func.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Hero_Struct.h"

void refresh_Hero_skill(Hero_Info_Detail &detail) {

	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(detail.hero_id);
	if (! hero_config) return;

	int skill_loc = 6;
	int default_skill = 0;

	Talent talent;
	talent.talent_id = 1;

	Skill_DB_Info skill_db_info;

	// 英雄等级技能
	for (uint16_t i = 0; i < hero_config->skill_id_vec.size(); ++i) {
		int skill_id = hero_config->skill_id_vec[i];
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
		if (0 == cfg) continue;

		int skill_level = 0;
		for (Skill_Detail_Map::const_iterator it = cfg->skill_level_map.begin();
				it != cfg->skill_level_map.end(); ++it) {
			if (it->second.skill_level <= skill_level || detail.level < it->second.lv_lim) continue;
			skill_level = it->second.skill_level;
		}
		if (0 == skill_level) continue;

		default_skill = !default_skill ? skill_id : default_skill;

		skill_db_info.reset();
		skill_db_info.skill_id = skill_id;
		skill_db_info.skill_level = skill_level;

		if (cfg->skill_type == TYPE_ADD_SKILL) {
			skill_db_info.skill_loc = 9;	// 追加技能放最后位置
		} else {
			skill_db_info.skill_loc = (skill_id == default_skill) ? 0 : skill_loc++;
		}

		talent.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
	}

	// 英雄觉醒技能
	uint16_t awake_start_index = 0;
	if(detail.awake_lvl > 4){
		awake_start_index = detail.awake_lvl - 4;
	}
	for (uint16_t i = 0; i < detail.awake_lvl && i < hero_config->awake_skill_id_vec.size(); ++i) {
		int skill_id = hero_config->awake_skill_id_vec[i];
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
		if (0 == cfg) continue;

		if(i < awake_start_index){
			continue;
		}

		int skill_level = 0;
		for (Skill_Detail_Map::const_iterator it = cfg->skill_level_map.begin();
				it != cfg->skill_level_map.end(); ++it) {
			if (it->second.skill_level <= skill_level || detail.level < it->second.lv_lim) continue;
			skill_level = it->second.skill_level;
		}
		if (0 == skill_level) continue;

		default_skill = !default_skill ? skill_id : default_skill;

		skill_db_info.reset();
		skill_db_info.skill_id = skill_id;
		skill_db_info.skill_level = skill_level;
		skill_db_info.skill_loc = 0;

		talent.skill_db_info_map.insert(std::make_pair(skill_db_info.skill_id, skill_db_info));
	}

	detail.hero_fighter_detail.talent_map[talent.talent_id] = talent;
	detail.hero_fighter_detail.cur_used_talent_ = talent.talent_id;
	detail.hero_fighter_detail.default_skill = default_skill;
}

void refresh_Heros_skill(Heroer_Detail &detail) {
	for (Heroer_Detail::Hero_Map::iterator it = detail.hero_map.begin();
			it != detail.hero_map.end(); ++it) {
		refresh_Hero_skill(it->second);
	}
}

void get_hero_assist_info(const Hero_Info_Detail &hero, Hero_Assist_Info &assist_info) {

	assist_info.skill_id = hero.assist_skill.val_1;
	assist_info.skill_lv = hero.assist_skill.val_2;
	assist_info.intimacy_lv = hero.intimacy_lv;
	assist_info.intimacy_val = hero.intimacy_exp;
	assist_info.state = figure_out_assist_hero_state(hero);
	int cmm_times = CONFIG_CACHE_HERO->find_intimacy_common(1);
	cmm_times = cmm_times - hero.community_times;
	assist_info.communite_times = cmm_times > 0 ? cmm_times : 0;
	int gift_times = CONFIG_CACHE_HERO->find_intimacy_common(2);
	gift_times = gift_times - hero.gift_times;
	assist_info.gift_times = gift_times > 0 ? gift_times : 0;
	Int_Double_Map props;
	figure_out_assist_hero_add_prop(hero, props);
	for (Int_Double_Map::const_iterator it = props.begin(); it != props.end(); ++it) {
		assist_info.props.push_back(Int_Double(it->first, it->second));
	}
}

void figure_out_assist_hero_add_prop(
		const Hero_Info_Detail &hero, Int_Double_Map &prop_map) {

	const Int_Double_Map *cfg_prop =  CONFIG_CACHE_HERO->find_hero_lvl_prop(
			hero.level);
	if (0 == cfg_prop) return;
	const Int_Double_Map *cfg_correct = CONFIG_CACHE_HERO->find_hero_id_correct_prop(
			hero.hero_id);
	if (0  == cfg_correct) return;
	const Int_Double_Map *cfg_convert = CONFIG_CACHE_HERO->find_hero_id_conver_prop(
			hero.hero_id);
	if (0 == cfg_convert) return;

	const Intimacy_Base* cfg_intimacy_base = CONFIG_CACHE_HERO->find_intimacy_base(
			hero.intimacy_lv);
	if (0 == cfg_intimacy_base) return;

	double value =0.0;
	value = fighure_out_assist_prop_value(
			*cfg_prop, *cfg_correct, *cfg_convert, PT_PHY_ATTACK, PT_FORCE);
	prop_map[PT_PHY_ATTACK] += std::floor(value * cfg_intimacy_base->convert);

	value = fighure_out_assist_prop_value(
			*cfg_prop, *cfg_correct, *cfg_convert, PT_SPELL_ATTACK, PT_FORCE);
	prop_map[PT_SPELL_ATTACK] += std::floor(value * cfg_intimacy_base->convert);

	value = fighure_out_assist_prop_value(
			*cfg_prop, *cfg_correct, *cfg_convert, PT_PHY_DEFENSE, PT_ARMOR);
	prop_map[PT_PHY_DEFENSE] += std::floor(value * cfg_intimacy_base->convert);

	value = fighure_out_assist_prop_value(
			*cfg_prop, *cfg_correct, *cfg_convert, PT_SPELL_DEFENSE, PT_ARMOR);
	prop_map[PT_SPELL_DEFENSE] += std::floor(value * cfg_intimacy_base->convert);

	value = fighure_out_assist_prop_value(
			*cfg_prop, *cfg_correct, *cfg_convert, PT_BLOOD_MAX, PT_STAMINA);
	prop_map[PT_BLOOD_MAX] += std::floor(value * cfg_intimacy_base->convert);
}

double fighure_out_assist_prop_value(
		const Int_Double_Map& cfg_prop, const Int_Double_Map& cfg_correct,
		const Int_Double_Map& cfg_convert, const Property_Type& type1,
		const Property_Type& type2) {

	double val = 0.0;
	Int_Double_Map::const_iterator it_prop_1 = cfg_prop.find(type1);
	if (it_prop_1 == cfg_prop.end()) return val;
	Int_Double_Map::const_iterator it_correct_1 = cfg_correct.find(type1);
	if (it_correct_1 == cfg_correct.end()) return val;
	val = it_prop_1->second * it_correct_1->second;

	Int_Double_Map::const_iterator it_prop_2 = cfg_prop.find(type2);
	if (it_prop_2 == cfg_prop.end()) return val;
	Int_Double_Map::const_iterator it_correct_2 = cfg_correct.find(type2);
	if (it_correct_2 == cfg_correct.end()) return val;
	Int_Double_Map::const_iterator it_convert = cfg_convert.find(type1);
	if (it_convert == cfg_convert.end()) return val;

	val += it_prop_2->second * it_convert->second * it_correct_2->second;
	return val;
}

int figure_out_assist_hero_state(const Hero_Info_Detail &hero) {

	return 0;
}
