/*
 * Logic_Heroer_Struct.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#include "DB_Operator.h"
#include "Logic_Heroer_Struct.h"
#include "Logic_Heroer_Def.h"
#include "Config_Cache_Role.h"
#include "equiper/Equiper_Func.h"
#include "Logic_Heroer_Func.h"

void Artifact_Detail::serialize(Block_Buffer &w) const  {
	w.write_int32(artifact_id);
	w.write_int32(artifact_lvl);
}

int Artifact_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int32(artifact_id) ||
			r.read_int32(artifact_lvl)) {
		return -1;
	}
	return 0;
}

void Artifact_Detail::reset(void) {
	artifact_id = 0;
	artifact_lvl = 0;
}

Hero_Info_Detail::Hero_Info_Detail(void) {
	reset();
}

void Hero_Info_Detail::reset(void) {
	hero_id = 0;
	fight_date = 0;
	hero_index = 0;
	hero_facade_id = 0;
	hero_name.clear();
	is_fighter = 0;
	level = 0;
	formation = 0;
	awake_lvl = 0;
	exp = 0.0;
	CLEAR_VECTOR(std::vector<Int_Double>, prop_value);
	artifacts.clear();
	is_outer = 0;
	is_task_tmp_hero = false;

	intimacy_lv = 1;
	intimacy_exp = 0;
	community_times = 0;
	gift_times = 0;
	assist_skill.reset();

	hero_fighter_detail.reset();
	suit_cnt_map.clear();
}

void Hero_Info_Detail::serialize(Block_Buffer &w) const {
	w.write_int32(hero_id);
	w.write_int32(hero_index);
	w.write_int32(hero_facade_id);
	w.write_string(hero_name);
	w.write_int8(is_outer);
	w.write_int8(is_fighter);
	w.write_int32(level);
	w.write_int32(formation);
	w.write_int32(awake_lvl);
	w.write_double(exp);

	uint16_t __prop_value_vec_size = prop_value.size();
	w.write_uint16(__prop_value_vec_size);
	for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
			prop_value[i].serialize(w);
	}

	w.write_uint16(artifacts.size());
	for (Artifact_Map::const_iterator it = artifacts.begin(); it != artifacts.end(); ++it) {
		it->second.serialize(w);
	}
	hero_fighter_detail.serialize(w);
}

int Hero_Info_Detail::deserialize(Block_Buffer &r) {
	if( r.read_int32(hero_id)  ||  r.read_int32(hero_index)  ||  r.read_int32(hero_facade_id)  ||
			r.read_string(hero_name)  ||  r.read_int8(is_outer) ||  r.read_int8(is_fighter)  ||
			r.read_int32(level)  ||  r.read_int32(formation)  || r.read_int32(awake_lvl) ||
			r.read_double(exp)	)
			return -1;

	uint16_t __prop_value_vec_size;
	if(r.read_uint16(__prop_value_vec_size) )
			return -1;
	prop_value.reserve(__prop_value_vec_size);
	for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
			Int_Double v;
			if(v.deserialize(r))
					return -1;
			prop_value.push_back(v);
	}

	uint16_t size = 0;
	Artifact_Detail artifact_detail;
	r.read_uint16(size);
	for(uint16_t i = 0; i < size; ++i) {
		artifact_detail.reset();
		if (artifact_detail.deserialize(r)) {
			continue;
		};
		artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
	}
	hero_fighter_detail.deserialize(r);
	return 0;
}

Panal_Hero_Tag::Panal_Hero_Tag(void) {
	reset();
}

void Panal_Hero_Tag::reset(void) {
	is_fight = 0;
	is_last_fight = 0;
	hero_level = 0;
	hero_index = 0;
	hero_id = 0;
}

bool operator<(const Panal_Hero_Tag& lhs, const Panal_Hero_Tag& rhs) {
	if (lhs.is_fight < rhs.is_fight) {
		return true;
	} else if (lhs.is_fight > rhs.is_fight) {
		return false;
	}

	if (lhs.is_last_fight < rhs.is_last_fight) {
		return true;
	} else if (lhs.is_last_fight > rhs.is_last_fight) {
		return false;
	}

	if (lhs.hero_level < rhs.hero_level) {
		return true;
	} else if (lhs.hero_level > rhs.hero_level) {
		return false;
	}

	if (lhs.hero_index < rhs.hero_index) {
		return true;
	} else if (lhs.hero_index > rhs.hero_index) {
		return false;
	}

	return false;
}

Heroer_Detail::Heroer_Detail() {
	reset();
}

Heroer_Detail::~Heroer_Detail() {

}

int Heroer_Detail::serialize(Block_Buffer &buffer) const {
	// not necessary
	return 0;
}

int Heroer_Detail::deserialize(Block_Buffer &buffer) {
	// not necessary
	return 0;
}

int Heroer_Detail::serialize_fighter(Block_Buffer &buffer) const{
	this->hero_base_detail_.serialize(buffer);
	buffer.write_int32(this->fighter_index);

	Hero_Map::const_iterator it = this->hero_map.find(this->fighter_index);
	if(it != this->hero_map.end()){
		int8_t has_hero = 1;
		buffer.write_int8(has_hero);
		it->second.serialize(buffer);
	}else{
		int8_t has_hero = 0;
		buffer.write_int8(has_hero);
	}
	return 0;
}

int Heroer_Detail::deserialize_fighter(Block_Buffer &buffer){
	this->hero_base_detail_.deserialize(buffer);
	buffer.read_int32(this->fighter_index);
	int8_t has_hero = 0;
	buffer.read_int8(has_hero);
	if(has_hero != 0){
		Hero_Info_Detail info;
		info.deserialize(buffer);
		this->hero_map[this->fighter_index] = info;
	}
	return 0;
}

int Heroer_Detail::load(void) {
	return MONGO_OPERATOR->load_heroer_detail(*this);
}

int Heroer_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_heroer_detail(*this);
	return 0;
}

int Heroer_Detail::load_fight_hero(void) {
	return MONGO_OPERATOR->load_fight_hero_detail(*this);
}

void Heroer_Detail::get_hero_info(const int hero_index, Hero_Info &hero_info) const {
	Skill_Info skill_info;
	Hero_Map::const_iterator it_hero = hero_map.find(hero_index);
	if (it_hero == hero_map.end()) {
		return;
	}

	hero_info.hero_id = it_hero->second.hero_id;
	hero_info.fight_date = it_hero->second.fight_date;
	hero_info.hero_index = it_hero->second.hero_index;
	hero_info.hero_facade_id = it_hero->second.hero_facade_id;
	hero_info.hero_name = it_hero->second.hero_name;
	hero_info.is_outer = it_hero->second.is_outer;
	hero_info.is_fighter = it_hero->second.is_fighter;
	hero_info.level = it_hero->second.level;
	hero_info.formation = it_hero->second.formation;
	hero_info.awake_lvl = it_hero->second.awake_lvl;
	hero_info.exp = it_hero->second.exp;

	// 英雄战斗属性
	Int_Double prop_value;
	std::map<int, double> pro_val_map;	// 用来算战力的
	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		prop_value.reset();
		prop_value.id = *it_prop;
		prop_value.value = it_hero->second.hero_fighter_detail.fetch_fight_property((Property_Type)prop_value.id);
		if (prop_value.id > 0 && prop_value.value != 0.0) {
			hero_info.prop_value.push_back(prop_value);

			pro_val_map[prop_value.id] += prop_value.value;
		}
	}

	// 战力
	hero_info.prop_value.push_back(Int_Double(PT_FORCE,
			it_hero->second.hero_fighter_detail.fetch_fight_property(PT_FORCE)));
//	if (! pro_val_map.empty()) {
//		int force = E_FUNC::calculate_force(pro_val_map);
//		if (force) {
//			hero_info.prop_value.push_back(Int_Double(PT_FORCE, force));
//		}
//	}

	// 英雄技能
	for (Talent_Map::const_iterator it_talent = it_hero->second.hero_fighter_detail.talent_map.begin();
			it_talent != it_hero->second.hero_fighter_detail.talent_map.end(); ++it_talent) {
//		for (Talent_Page_Map::const_iterator it_talent = it->second.hero_fighter_detail.talent_page_map.begin();
//				it_talent != it->second.hero_fighter_detail.talent_page_map.end(); ++it_talent) {

		for (Skill_DB_Info_Map::const_iterator it_skill_db = it_talent->second.skill_db_info_map.begin();
				it_skill_db != it_talent->second.skill_db_info_map.end(); ++it_skill_db) {
			if (it_skill_db->second.skill_id == it_hero->second.hero_fighter_detail.default_skill) {
				continue;
			}
			skill_info.reset();
			skill_info.skill_id = it_skill_db->second.skill_id;
			skill_info.skill_lv = it_skill_db->second.skill_level;
			skill_info.skill_loc = it_skill_db->second.skill_loc;
			hero_info.skill_info_vec.push_back(skill_info);
		}
	}

	// 神器
	Artifact_Info artifact_info;
	for (Artifact_Map::const_iterator it_artifact = it_hero->second.artifacts.begin();
			it_artifact != it_hero->second.artifacts.end(); ++it_artifact) {
		artifact_info.reset();
		artifact_info.artifact_id = it_artifact->second.artifact_id;
		artifact_info.artifact_lvl = it_artifact->second.artifact_lvl;
		hero_info.artifact_vec.push_back(artifact_info);
	}

	get_hero_assist_info(it_hero->second, hero_info.hero_assist_info);
}

void Heroer_Detail::refresh_hero_base_detail(const int hero_index, role_id_t master_role_id, std::string master_role_name) {
	//Hero_Base_Detail hero_base_detail;
	Hero_Map::const_iterator it = hero_map.find(hero_index);
	if (it != hero_map.end()) {
		hero_base_detail_.master_role_id = master_role_id;
		hero_base_detail_.master_role_name = master_role_name;
		hero_base_detail_.hero_id = it->second.hero_id;
		hero_base_detail_.hero_index = it->second.hero_index;
		hero_base_detail_.hero_facade_id = it->second.hero_facade_id;
		hero_base_detail_.hero_name = it->second.hero_name;
		hero_base_detail_.hero_formation = it->second.formation;
		hero_base_detail_.hero_level = it->second.level;
		hero_base_detail_.awake_lvl = it->second.awake_lvl;
	}
}

void Heroer_Detail::refresh_fight_hero_base(const int hero_index, role_id_t master_role_id, std::string master_role_name) {
	Hero_Map::const_iterator it = hero_map.find(hero_index);
	if (it != hero_map.end()) {
		fight_hero_base_.master_role_id = master_role_id;
		fight_hero_base_.master_role_name = master_role_name;
		fight_hero_base_.hero_id = it->second.hero_id;
		fight_hero_base_.hero_index = it->second.hero_index;
		fight_hero_base_.hero_facade_id = it->second.hero_facade_id;
		fight_hero_base_.hero_name = it->second.hero_name;
		fight_hero_base_.hero_formation = it->second.formation;
		fight_hero_base_.hero_level = it->second.level;
		fight_hero_base_.awake_lvl = it->second.awake_lvl;
		logic_status_detail_.role_id = it->second.hero_id;
	}
}

void Heroer_Detail::reset(void) {
	role_id = 0;
	outer_index = 0;
	fighter_index = 0;
	last_fighter_idx = 0;
	hero_assist_map.clear();

	hero_map.clear();

	hero_base_detail_.reset();
	fight_hero_base_.reset();
	logic_status_detail_.reset();
	out_status_timepri_list.clear();
	status_pro_val.clear();
	status_pro_val_per.clear();
	buff_scene_status.clear();

	clicked_new_heros.clear();

	hero_rune_pro_val.clear();

	CLEAR_VECTOR(std::vector<Panal_Hero_Tag>, panal_hero_tags);


	is_change = false;
}
