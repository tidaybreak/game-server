/*
 * Fighter_Detail.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: ti
 */
#include "Fighter_Detail.h"
#include "DB_Operator.h"
#include "Config_Cache_Fighter.h"

////////////////////////////////////////////////////////////////////

Prop_Base::Prop_Base(void) {
	reset();
}

Prop_Base::Prop_Base (double basic,double fixed, double percent) : basic_value(basic), fixed_value(fixed), percent_value(percent) {};

void Prop_Base::serialize(Block_Buffer & w) const {
	w.write_double(basic_value);
	w.write_double(fixed_value);
	w.write_double(percent_value);
}
int Prop_Base::deserialize(Block_Buffer & r) {
	if(r.read_double(basic_value)  ||  r.read_double(fixed_value)  ||  r.read_double(percent_value))
			return -1;
		return 0;
}

void Prop_Base::reset(void) {
	basic_value = 0.0;
	fixed_value = 0.0;
	percent_value = 0.0;
}


Prop_Value::Prop_Value(void) {
	reset(0);
}

void Prop_Value::reset(int type_) {
	type = type_;
	for (int i = AT_BASIC; i < AT_END; ++i) {
		(&(this->basic) + i)->reset();
	}
}

void Prop_Value::serialize(Block_Buffer & w) const {
	for (int i = AT_BASIC; i < AT_END; ++i) {
		(&(this->basic) + i)->serialize(w);
	}
}

int Prop_Value::deserialize(Block_Buffer & r) {
	for (int i = AT_BASIC; i < AT_END; ++i) {
		if ((&(this->basic) + i)->deserialize(r))
			return -1;
	}
	return 0;
}

void Prop_Value::reset_single(const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).reset();
}

double Prop_Value::log_value_total(const int offset) {

	if (offset == AT_INITIATIVE_SKILL || offset == AT_STATUS) return 0.0;
	if (offset == AT_BASIC) return this->basic.basic_value;
	double equip_value = this->equip.basic_value * (1 + this->equip.percent_value) + this->equip.fixed_value;
	if (offset == AT_EQUIP) return equip_value;
	double conversion_value = this->conversion.basic_value * this->conversion.percent_value;
	if (offset == AT_CONVERSION) return conversion_value;

	double total_value = this->basic.basic_value + equip_value + conversion_value;

	for (int i = AT_PASSIVE_SKILL; i < AT_END; ++i) {
		total_value += (&(this->basic) + i)->fixed_value;
	}

	return total_value * (&(this->basic) + offset)->percent_value + (&(this->basic) + offset)->fixed_value;
}

double Prop_Value::total(const int elemadd) const {
	double total_value = 0.0;
	switch (elemadd) {
	case ELEM_NORMAL: {
		// 装备模块总值＝装备基础值×（1＋装备乘法加成）＋装备加法加成
		double equip_value = this->equip.basic_value * (1 + this->equip.percent_value) + this->equip.fixed_value;
		// 一二级转换属性=一级属性×职业换算参数
		double conversion_value = (this->conversion.basic_value + this->conversion.fixed_value) *
				this->conversion.percent_value;

		double fixed_total = 0.0;
		double precent_total = 1.0;

		for (int i = AT_STATUS; i < AT_END; ++i) {
			fixed_total += (&(this->basic) + i)->fixed_value;
			precent_total += (&(this->basic) + i)->percent_value;
		}
		// 属性总值＝（一二级转换属性 + 角色基础值＋装备模块＋加法修正）×（1＋乘法修正）
		total_value = (conversion_value + this->basic.basic_value + equip_value + fixed_total) * precent_total;
		break;
	}
	case ELEM_FIGHT: {
		// 装备模块总值＝装备基础值×（1＋装备乘法加成）＋装备加法加成
		double equip_value = this->equip.basic_value * (1 + this->equip.percent_value) + this->equip.fixed_value;
		// 一二级转换属性=一级属性×职业换算参数
		double conversion_value = (this->conversion.basic_value + this->conversion.fixed_value) *
				this->conversion.percent_value;

		double fixed_total = 0.0;
		double precent_total = 1.0;

		for (int i = AT_STATUS; i < AT_END; ++i) {
			fixed_total += (&(this->basic) + i)->fixed_value;
			precent_total += (&(this->basic) + i)->percent_value;
		}
		// 属性总值＝（一二级转换属性 + 角色基础值＋装备模块＋加法修正）×（1＋乘法修正）
		total_value = (conversion_value + this->basic.basic_value + equip_value + fixed_total) * precent_total;

		total_value *= 1 + this->initiative_skill.percent_value;
		total_value += this->initiative_skill.fixed_value;
		break;
	}
	case ELEM_FORCE: {
		// 装备模块总值＝装备基础值×（1＋装备乘法加成）＋装备加法加成
		double equip_value = this->equip.basic_value * (1 + this->equip.percent_value) + this->equip.fixed_value;
		// 一二级转换属性=一级属性×职业换算参数
		double conversion_value = this->conversion.basic_value * this->conversion.percent_value;

		double fixed_total = 0.0;
		double precent_total = 1.0;

		for (int i = AT_PASSIVE_SKILL; i < AT_END; ++i) {
			fixed_total += (&(this->basic) + i)->fixed_value;
			precent_total += (&(this->basic) + i)->percent_value;
		}
		// 属性总值＝（一二级转换属性 + 角色基础值＋装备模块＋加法修正）×（1＋乘法修正）
		total_value = (conversion_value + this->basic.basic_value + equip_value + fixed_total) * precent_total;
		break;
	}
	default: {
		break;
	}
	}

	return total_value;
}

void Prop_Value::modify_property(const int offset, Operate_Type oen, double basic, double fixed, double percent, int op_code) {
	switch (oen) {
	case O_ADD: {
		if (op_code & Fighter_Detail::BASE_OP)
			add_basic_single(basic, offset);
		if (op_code & Fighter_Detail::FIX_OP)
			add_fixed_single(fixed, offset);
		if (op_code & Fighter_Detail::PERCENT_OP)
			add_percent_single(percent, offset);
		break;
	}
	case O_SUB: {
		if (op_code & Fighter_Detail::BASE_OP)
			reduce_basic_single(basic, offset);
		if (op_code & Fighter_Detail::FIX_OP)
			reduce_fixed_single(fixed, offset);
		if (op_code & Fighter_Detail::PERCENT_OP)
			reduce_percent_single(percent, offset);
		break;
	}
	case O_SET: {
		if (op_code & Fighter_Detail::BASE_OP)
			set_basic_single(basic, offset);
		if (op_code & Fighter_Detail::FIX_OP)
			set_fixed_single(fixed, offset);
		if (op_code & Fighter_Detail::PERCENT_OP)
			set_percent_single(percent, offset);
		break;
	}
	default: {
		MSG_USER_TRACE("Operate_Type = %d, error !\n", oen);
		break;
	}
	}

}

void Prop_Value::set_single(const double basic, const double fixed, const double percent, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).basic_value = basic;
    (*ptr).fixed_value = fixed;
    (*ptr).percent_value = percent;
}

void Prop_Value::set_basic_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).basic_value = value;
}

void Prop_Value::set_fixed_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).fixed_value = value;
}

void Prop_Value::set_percent_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).percent_value = value;
}

void Prop_Value::add_single(const double basic, const double fixed, const double percent, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).basic_value += basic;
    (*ptr).fixed_value += fixed;
    (*ptr).percent_value += percent;
}

void Prop_Value::add_basic_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).basic_value += value;
}

void Prop_Value::add_fixed_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).fixed_value += value;
}

void Prop_Value::add_percent_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).percent_value += value;
}

void Prop_Value::reduce_single(const double basic, const double fixed, const double percent, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).basic_value -= basic;
    (*ptr).fixed_value -= fixed;
    (*ptr).percent_value -= percent;
}

void Prop_Value::reduce_basic_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).basic_value -= value;
//    if ((*ptr).basic_value < 0.0) {
//    	MSG_USER_TRACE("type:%d, value now:%f value:%f", type, (*ptr).basic_value, value);
//    }
}

void Prop_Value::reduce_fixed_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).fixed_value -= value;
//    if ((*ptr).fixed_value < 0.0) {
//    	MSG_USER_TRACE("type:%d, value now:%f value:%f", type, (*ptr).fixed_value, value);
//    }
}

void Prop_Value::reduce_percent_single(const double value, const int offset) {
    if (offset < 0 || AT_END <= offset)
        return;

    Prop_Base *ptr = &(this->basic) + offset;
    (*ptr).percent_value -= value;
//    if ((*ptr).percent_value < 0.0) {
//    	MSG_USER_TRACE("type:%d, value now:%f value:%f", type, (*ptr).percent_value, value);
//    }
}

void Skill_DB_Info::reset(void) {
	skill_id = 0;
	skill_level = 0;
	skill_loc = 0;
}

int Skill_DB_Info::serialize(Block_Buffer &buf) const {
	buf.write_int32(skill_id);
	buf.write_int32(skill_level);
	buf.write_int32(skill_loc);
	return 0;
}

int Skill_DB_Info::deserialize(Block_Buffer &buf){
	buf.read_int32(skill_id);
	buf.read_int32(skill_level);
	buf.read_int32(skill_loc);
	return 0;
}

void Talent::reset(void) {
	talent_id = 0;
	skill_point = 0;
	skill_db_info_map.clear();
}

int Talent::serialize(Block_Buffer &buf) const {
	buf.write_int32(talent_id);
	buf.write_int32(skill_point);
	buf.write_uint16(skill_db_info_map.size());
	for (Skill_DB_Info_Map::const_iterator it = skill_db_info_map.begin(); it != skill_db_info_map.end(); ++it) {
		it->second.serialize(buf);
	}
	return 0;
}

int Talent::deserialize(Block_Buffer &buf) {
	buf.read_int32(talent_id);
	buf.read_int32(skill_point);

	skill_db_info_map.clear();
	uint16_t size = 0;
	buf.read_uint16(size);
	Skill_DB_Info skill_db_info;
	for (uint16_t i=0; i<size; ++i) {
		skill_db_info.reset();
		skill_db_info.deserialize(buf);
		skill_db_info_map[skill_db_info.skill_id] = skill_db_info;
	}
	return 0;
}

Skill_Running_Info::Skill_Running_Info(void) {
	reset();
}

void Skill_Running_Info::reset(void) {
	skill_id = 0;
	last_use_timestamp = Time_Value::zero;
	use_times = 0;
	step_target_map.clear();
	last_step = 0;

	config_cache = 0;
}

int Skill_Running_Info::serialize(Block_Buffer &buf) const {
	buf.write_int32(skill_id);

	buf.write_uint32(last_use_timestamp.sec());
	buf.write_uint32(last_use_timestamp.usec());

	buf.write_int32(use_times);

	buf.write_int32(last_step);

	return 0;
}

int Skill_Running_Info::deserialize(Block_Buffer &buf) {
	uint32_t sec = 0, usec = 0;

	buf.read_int32(skill_id);

	buf.read_uint32(sec);
	buf.read_uint32(usec);
	last_use_timestamp.sec(sec);
	last_use_timestamp.usec(usec);

	buf.read_int32(use_times);
	uint16_t size;
	buf.read_uint16(size);

	buf.read_int32(last_step);
	buf.read_uint32(sec);
	buf.read_uint32(usec);

	config_cache = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (NULL == config_cache) {
		return -1;
	}

	return 0;
}

Fighter_Detail::Fighter_Detail(void) {
	reset();
}

Fighter_Detail::~Fighter_Detail(void) {

}

void Fighter_Detail::reset(void) {
	logout = false;
	role_id = 0;
	power.reset(PT_POWER);
	agility.reset(PT_AGILITY);
	dexterity.reset(PT_DEXTERITY);
	armor.reset(PT_ARMOR);
	stamina.reset(PT_STAMINA);

	physical_attack.reset(PT_PHY_ATTACK);
	spell_attack.reset(PT_SPELL_ATTACK);
	physical_defense.reset(PT_PHY_DEFENSE);
	spell_defense.reset(PT_SPELL_DEFENSE);
	hit_rate.reset(PT_HIT);
	avoid.reset(PT_AVOID);
	ignore_def.reset(PT_IGNORE_DEF);
	blood_max.reset(PT_BLOOD_MAX);
	cure.reset(PT_CURE);
	cure_rate.reset(PT_CURE_RATE);
	sing_rate.reset(PT_SING_RATE);
	crit.reset(PT_CRIT);
	crit_def.reset(PT_CRIT_DEF);
	init_morale.reset(PT_INIT_MORALE);
	morale_caps.reset(PT_MORALE_CAPS);
	crit_hurt.reset(PT_CRIT_HURT);
	crit_hurt_sub.reset(PT_CRIT_HURT_SUB);
	hit_rate_addition.reset(PT_HIT_RATE_ADDITION);
	avoid_rate_addition.reset(PT_AVOID_RATE_ADDITION);
	crit_rate_addition.reset(PT_CRIT_RATE_ADDITION);
	crit_def_rate_addition.reset(PT_CRIT_DEF_RATE_ADDITION);
	speed.reset(PT_SPEED);
	blood_current.reset(PT_BLOOD_CURRENT);
	morale_current.reset(PT_MORALE_CURRENT);
	strength_current.reset(PT_STRENGTH_CURRENT);
	strength_max.reset(PT_STRENGTH_MAX);
	blood_pool_current.reset(PT_BLOOD_POOL_CURRENT);
	blood_pool_max.reset(PT_BLOOD_POOL_MAX);

	ignore_def_rate.reset(PT_IGNORE_DEF_RATE);
	status_hit_rate.reset(PT_STATUS_HIT_RATE);
	status_def_rate.reset(PT_STATUS_DEF_RATE);
	blood_steal.reset(PT_BLOOD_STEAL);

	phy_hurt_fix.reset(PT_PHY_HURT_FIX);
	phy_hurt_rate.reset(PT_PHY_HURT_RATE);
	spell_hurt_fix.reset(PT_SPELL_HURT_FIX);
	spell_hurt_rate.reset(PT_SPELL_HURT_RATE);
	hurt_fix.reset(PT_HURT_FIX);
	hurt_rate.reset(PT_HURT_RATE);
	be_hurt_fix.reset(PT_BE_HURT_FIX);
	be_hurt_rate.reset(PT_BE_HURT_RATE);

	pk_hurt_add.reset(PT_PK_HURT_ADD);
	pk_hurt_sub.reset(PT_PK_HURT_SUB);
	phy_hurt_rate_sub.reset(PT_PHY_HURT_RATE_SUB);
	spell_hurt_rate_sub.reset(PT_SPELL_HURT_RATE_SUB);
	skill_hurt_rate.reset(PT_SKILL_HURT_RATE);

	human_add.reset(PT_HUMAN_ADD);
	elf_add.reset(PT_ELF_ADD);
	orc_add.reset(PT_ORC_ADD);
	dragon_add.reset(PT_DRAGON_ADD);
	undead_add.reset(PT_UNDEAD_ADD);
	demon_add.reset(PT_DEMON_ADD);
	human_sub.reset(PT_HUMAN_SUB);
	elf_sub.reset(PT_ELF_SUB);
	orc_sub.reset(PT_ORC_SUB);
	dragon_sub.reset(PT_DRAGON_SUB);
	undead_sub.reset(PT_UNDEAD_SUB);
	demon_sub.reset(PT_DEMON_SUB);
	summon_add.reset(PT_SUMMON_ADD);
	summon_sub.reset(PT_SUMMON_SUB);

	be_cure_rate.reset(PT_BE_CURE_RATE);
	effect_rate.reset(PT_EFFECT_RATE);

	pve_hurt_add.reset(PT_PVE_HURT_ADD);
	pve_hurt_sub.reset(PT_PVE_HURT_SUB);

	dizz_strengthen.reset(PT_DIZZ_STRENGTHEN);
	dizz_weaken.reset(PT_DIZZ_WEAKEN);

	phy_power_recover_time = 0;
	phy_power_buy_times = 0;
	level = 0;
	experience = 0;
	elite_btl_morale = 0.0;

	force = 0;
	highest_force = 0;
	ttl_force = 0;

	default_skill = 0;
	ttl_skill_points_ = 0;
	cur_used_talent_ = 0;
	reset_skill_times = 0;

	talent_map.clear();
	extra_skill_vec.clear();
	skill_running_info_map.clear();

	is_clear_moral_ = true;
}

int Fighter_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);

	power.serialize(buffer);
	agility.serialize(buffer);
	dexterity.serialize(buffer);
	armor.serialize(buffer);
	stamina.serialize(buffer);

	physical_attack.serialize(buffer);
	spell_attack.serialize(buffer);
	physical_defense.serialize(buffer);
	spell_defense.serialize(buffer);
	hit_rate.serialize(buffer);
	avoid.serialize(buffer);
	ignore_def.serialize(buffer);
	blood_max.serialize(buffer);
	cure.serialize(buffer);
	cure_rate.serialize(buffer);
	sing_rate.serialize(buffer);
	crit.serialize(buffer);
	crit_def.serialize(buffer);
	init_morale.serialize(buffer);
	morale_caps.serialize(buffer);
	crit_hurt.serialize(buffer);
	crit_hurt_sub.serialize(buffer);
	hit_rate_addition.serialize(buffer);
	avoid_rate_addition.serialize(buffer);
	crit_rate_addition.serialize(buffer);
	crit_def_rate_addition.serialize(buffer);
	speed.serialize(buffer);
	blood_current.serialize(buffer);
	morale_current.serialize(buffer);
	strength_current.serialize(buffer);
	strength_max.serialize(buffer);
	blood_pool_current.serialize(buffer);
	blood_pool_max.serialize(buffer);

	ignore_def_rate.serialize(buffer);
	status_hit_rate.serialize(buffer);
	status_def_rate.serialize(buffer);
	blood_steal.serialize(buffer);

	phy_hurt_fix.serialize(buffer);
	phy_hurt_rate.serialize(buffer);
	spell_hurt_fix.serialize(buffer);
	spell_hurt_rate.serialize(buffer);
	hurt_fix.serialize(buffer);
	hurt_rate.serialize(buffer);
	be_hurt_fix.serialize(buffer);
	be_hurt_rate.serialize(buffer);

	pk_hurt_add.serialize(buffer);
	pk_hurt_sub.serialize(buffer);
	phy_hurt_rate_sub.serialize(buffer);
	spell_hurt_rate_sub.serialize(buffer);
	skill_hurt_rate.serialize(buffer);

	human_add.serialize(buffer);
	elf_add.serialize(buffer);
	orc_add.serialize(buffer);
	dragon_add.serialize(buffer);
	undead_add.serialize(buffer);
	demon_add.serialize(buffer);
	human_sub.serialize(buffer);
	elf_sub.serialize(buffer);
	orc_sub.serialize(buffer);
	dragon_sub.serialize(buffer);
	undead_sub.serialize(buffer);
	demon_sub.serialize(buffer);
	summon_add.serialize(buffer);
	summon_sub.serialize(buffer);

	be_cure_rate.serialize(buffer);
	effect_rate.serialize(buffer);

	pve_hurt_add.serialize(buffer);
	pve_hurt_sub.serialize(buffer);

	dizz_strengthen.serialize(buffer);
	dizz_weaken.serialize(buffer);

	buffer.write_int32(phy_power_recover_time);
	buffer.write_int32(phy_power_buy_times);
	buffer.write_int32(level);
	buffer.write_double(experience);
	buffer.write_double(elite_btl_morale);
	buffer.write_int32(force);
	buffer.write_int32(highest_force);
	buffer.write_int32(ttl_force);

	buffer.write_int32(ttl_skill_points_);
	buffer.write_int32(default_skill);
	buffer.write_int32(cur_used_talent_);
	buffer.write_int32(reset_skill_times);

	buffer.write_uint16(talent_map.size());
	for (Talent_Map::const_iterator it = talent_map.begin(); it != talent_map.end(); ++it) {
		it->second.serialize(buffer);
	}

	buffer.write_uint16(extra_skill_vec.size());
	for (Skill_DB_Info_Vec::const_iterator it = extra_skill_vec.begin(); it != extra_skill_vec.end(); ++it) {
		it->serialize(buffer);
	}

	buffer.write_bool(is_clear_moral_);

	return 0;
}

int Fighter_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);

	power.deserialize(buffer);
	agility.deserialize(buffer);
	dexterity.deserialize(buffer);
	armor.deserialize(buffer);
	stamina.deserialize(buffer);

	physical_attack.deserialize(buffer);
	spell_attack.deserialize(buffer);
	physical_defense.deserialize(buffer);
	spell_defense.deserialize(buffer);
	hit_rate.deserialize(buffer);
	avoid.deserialize(buffer);
	ignore_def.deserialize(buffer);
	blood_max.deserialize(buffer);
	cure.deserialize(buffer);
	cure_rate.deserialize(buffer);
	sing_rate.deserialize(buffer);
	crit.deserialize(buffer);
	crit_def.deserialize(buffer);
	init_morale.deserialize(buffer);
	morale_caps.deserialize(buffer);
	crit_hurt.deserialize(buffer);
	crit_hurt_sub.deserialize(buffer);
	hit_rate_addition.deserialize(buffer);
	avoid_rate_addition.deserialize(buffer);
	crit_rate_addition.deserialize(buffer);
	crit_def_rate_addition.deserialize(buffer);
	speed.deserialize(buffer);
	blood_current.deserialize(buffer);
	morale_current.deserialize(buffer);
	strength_current.deserialize(buffer);
	strength_max.deserialize(buffer);
	blood_pool_current.deserialize(buffer);
	blood_pool_max.deserialize(buffer);

	ignore_def_rate.deserialize(buffer);
	status_hit_rate.deserialize(buffer);
	status_def_rate.deserialize(buffer);
	blood_steal.deserialize(buffer);

	phy_hurt_fix.deserialize(buffer);
	phy_hurt_rate.deserialize(buffer);
	spell_hurt_fix.deserialize(buffer);
	spell_hurt_rate.deserialize(buffer);
	hurt_fix.deserialize(buffer);
	hurt_rate.deserialize(buffer);
	be_hurt_fix.deserialize(buffer);
	be_hurt_rate.deserialize(buffer);

	pk_hurt_add.deserialize(buffer);
	pk_hurt_sub.deserialize(buffer);
	phy_hurt_rate_sub.deserialize(buffer);
	spell_hurt_rate_sub.deserialize(buffer);
	skill_hurt_rate.deserialize(buffer);

	human_add.deserialize(buffer);
	elf_add.deserialize(buffer);
	orc_add.deserialize(buffer);
	dragon_add.deserialize(buffer);
	undead_add.deserialize(buffer);
	demon_add.deserialize(buffer);
	human_sub.deserialize(buffer);
	elf_sub.deserialize(buffer);
	orc_sub.deserialize(buffer);
	dragon_sub.deserialize(buffer);
	undead_sub.deserialize(buffer);
	demon_sub.deserialize(buffer);
	summon_add.deserialize(buffer);
	summon_sub.deserialize(buffer);

	be_cure_rate.deserialize(buffer);
	effect_rate.deserialize(buffer);

	pve_hurt_add.deserialize(buffer);
	pve_hurt_sub.deserialize(buffer);

	dizz_strengthen.deserialize(buffer);
	dizz_weaken.deserialize(buffer);

	buffer.read_int32(phy_power_recover_time);
	buffer.read_int32(phy_power_buy_times);
	buffer.read_int32(level);
	buffer.read_double(experience);
	buffer.read_double(elite_btl_morale);
	buffer.read_int32(force);
	buffer.read_int32(highest_force);
	buffer.read_int32(ttl_force);

	buffer.read_int32(ttl_skill_points_);
	buffer.read_int32(default_skill);
	buffer.read_int32(cur_used_talent_);
	buffer.read_int32(reset_skill_times);

	uint16_t s = 0;
	buffer.read_uint16(s);
	Talent talent;
	talent_map.clear();
	for (uint16_t i=0; i < s; ++i) {
		talent.reset();
		talent.deserialize(buffer);
		talent_map[talent.talent_id] = talent;
	}

	s = 0;
	buffer.read_uint16(s);
	Skill_DB_Info skill_info;
	extra_skill_vec.clear();
	for (uint16_t i=0; i<s; ++i) {
		skill_info.reset();
		skill_info.deserialize(buffer);
		extra_skill_vec.push_back(skill_info);
	}

	buffer.read_bool(is_clear_moral_);

	return 0;
}

int Fighter_Detail::save(void) {
	if (role_id)
		return MONGO_OPERATOR->save_fighter_detail(*this);
	return 0;
}

int Fighter_Detail::load(void) {
	return MONGO_OPERATOR->load_fighter_detail(*this);
}

int Fighter_Detail::arena_load(void){
	return MONGO_OPERATOR->load_arena_fighter_detail(*this);
}

void Fighter_Detail::modify_basic_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic) {
	modify_fight_property(pen, aen, oen, basic, -1, -1, OP_CODE::BASE_OP);
}

void Fighter_Detail::modify_fixed_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double fixed) {
	modify_fight_property(pen, aen, oen, -1, fixed, -1, OP_CODE::FIX_OP);
}

void Fighter_Detail::modify_percent_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double percent) {
	modify_fight_property(pen, aen, oen, -1, -1, percent, OP_CODE::PERCENT_OP);
}

int64_t Fighter_Detail::fetch_normal_property(Property_Type pen) {
	switch (pen) {
	case PT_LEVEL: {
		return level;
		break;
	}
	case PT_EXP_CURRENT: {
		return experience;
		break;
	}
	default : {
		MSG_USER_TRACE("unknow property:%d", pen);
	}
	}

	return 0;
}

int Fighter_Detail::modify_normal_property(Property_Type pen, Operate_Type oen, int value) {
	switch (pen) {
	case PT_LEVEL: {
		int old_level = level;
		modify_by_operate(oen, level, value);
		if (level <= 0 || level > MAX_LEVEL)
			level = old_level;
		break;
	}
	case PT_EXP_CURRENT: {
		double exp = experience;
		modify_by_operate(oen, experience, value);
		if (experience < 0)
			experience = exp;
		break;
	}
	default : {
		MSG_USER_TRACE("unknow property:%d", pen);
		return -1;
	}
	}

	this->detail_change();

	return 0;
}

double Fighter_Detail::fetch_fight_property(Property_Type pen, Prop_Value::ELEMADD aen) const {
	if (pen < PT_FIGHT_PROP_START || pen > Property_Type::PT_FIGHT_PROP_END) {
		MSG_USER_TRACE("Property_type = %d, error !\n", pen);
		return 0.0;
	}

	double val = 0.0;
	// 通常的需要所有属性的情况 减少IF的判断次数

	switch (pen) {
	case PT_FORCE: {
		val = force;
		break;
	}
	case PT_POWER: {
		val = power.total(aen);
		break;
	}
	case PT_AGILITY: {
		val = agility.total(aen);
		break;
	}
	case PT_DEXTERITY: {
		val = dexterity.total(aen);
		break;
	}
	case PT_ARMOR: {
		val = armor.total(aen);
		break;
	}
	case PT_STAMINA: {
		val = stamina.total(aen);
		break;
	}
	case PT_PHY_ATTACK: {
		val = physical_attack.total(aen);
		break;
	}
	case PT_SPELL_ATTACK: {
		val = spell_attack.total(aen);
		break;
	}
	case PT_PHY_DEFENSE: {
		val = physical_defense.total(aen);
		break;
	}
	case PT_SPELL_DEFENSE: {
		val = spell_defense.total(aen);
		break;
	}
	case PT_HIT: {
		val = hit_rate.total(aen);
		break;
	}
	case PT_IGNORE_DEF: {
		val = ignore_def.total(aen);
		break;
	}
	case PT_AVOID: {
		val = avoid.total(aen);
		break;
	}
	case PT_BLOOD_MAX: {
		val = blood_max.total(aen);
		break;
	}
	case PT_CRIT: {
		val = crit.total(aen);
		break;
	}
	case PT_CURE: {
		//maybe will rewrite this
		val = cure.total(aen);
		break;
	}
	case PT_CURE_RATE: {
		val = cure_rate.total(aen);
		break;
	}
	case PT_SING_RATE: {
		val = sing_rate.total(aen);
		break;
	}
	case PT_CRIT_DEF: {
		val = crit_def.total(aen);
		break;
	}
	case PT_INIT_MORALE: {
		val = init_morale.total(aen);
		break;
	}
	case PT_MORALE_CAPS: {
		val = morale_caps.total(aen);
		break;
	}
	case PT_CRIT_HURT: {
		val = crit_hurt.total(aen);
		break;
	}
	case PT_CRIT_HURT_SUB: {
		val = crit_hurt_sub.total(aen);
		break;
	}
	case PT_HIT_RATE_ADDITION: {
		val = hit_rate_addition.total(aen);
		break;
	}
	case PT_AVOID_RATE_ADDITION: {
		val = avoid_rate_addition.total(aen);
		break;
	}
	case PT_CRIT_RATE_ADDITION: {
		val = crit_rate_addition.total(aen);
		break;
	}
	case PT_CRIT_DEF_RATE_ADDITION: {
		val = crit_def_rate_addition.total(aen);
		break;
	}
	case PT_SPEED: {
		val = speed.total(aen);
		break;
	}
	case PT_BLOOD_CURRENT: {
		val = blood_current.total(aen);
		break;
	}
	case PT_MORALE_CURRENT: {
		val = morale_current.total(aen);
		break;
	}
	case PT_STRENGTH_CURRENT: {
		val = strength_current.total(aen);
		break;
	}
	case PT_STRENGTH_MAX: {
		val = strength_max.total(aen);
		break;
	}
	case PT_BLOOD_POOL_CURRENT: {
		val = blood_pool_current.total(aen);
		break;
	}
	case PT_BLOOD_POOL_MAX: {
		val = blood_pool_max.total(aen);
		break;
	}
	case PT_IGNORE_DEF_RATE: {
		val = ignore_def_rate.total(aen);
		break;
	}
	case PT_STATUS_HIT_RATE: {
		val = status_hit_rate.total(aen);
		break;
	}
	case PT_STATUS_DEF_RATE: {
		val = status_def_rate.total(aen);
		break;
	}
	case PT_BLOOD_STEAL: {
		val = blood_steal.total(aen);
		break;
	}
	case PT_PHY_HURT_FIX: {
		val = phy_hurt_fix.total(aen);
		break;
	}
	case PT_PHY_HURT_RATE: {
		val = phy_hurt_rate.total(aen);
		break;
	}
	case PT_SPELL_HURT_FIX: {
		val = spell_hurt_fix.total(aen);
		break;
	}
	case PT_SPELL_HURT_RATE: {
		val = spell_hurt_rate.total(aen);
		break;
	}
	case PT_HURT_FIX: {
		val = hurt_fix.total(aen);
		break;
	}
	case PT_HURT_RATE: {
		val = hurt_rate.total(aen);
		break;
	}
	case PT_BE_HURT_FIX: {
		val = be_hurt_fix.total(aen);
		break;
	}
	case PT_BE_HURT_RATE: {
		val = be_hurt_rate.total(aen);
		break;
	}
	case PT_PK_HURT_ADD: {
		val = pk_hurt_add.total(aen);
		break;
	}
	case PT_PK_HURT_SUB: {
		val = pk_hurt_sub.total(aen);
		break;
	}
	case PT_PHY_HURT_RATE_SUB: {
		val = phy_hurt_rate_sub.total(aen);
		break;
	}
	case PT_SPELL_HURT_RATE_SUB: {
		val = spell_hurt_rate_sub.total(aen);
		break;
	}
	case PT_SKILL_HURT_RATE: {
		val = skill_hurt_rate.total(aen);
		break;
	}
	case PT_HUMAN_ADD: {
		val = human_add.total(aen);
		break;
	}
	case PT_ELF_ADD: {
		val = elf_add.total(aen);
		break;
	}
	case PT_ORC_ADD: {
		val = orc_add.total(aen);
		break;
	}
	case PT_DRAGON_ADD: {
		val = dragon_add.total(aen);
		break;
	}
	case PT_UNDEAD_ADD: {
		val = undead_add.total(aen);
		break;
	}
	case PT_DEMON_ADD: {
		val = demon_add.total(aen);
		break;
	}
	case PT_HUMAN_SUB: {
		val = human_sub.total(aen);
		break;
	}
	case PT_ELF_SUB: {
		val = elf_sub.total(aen);
		break;
	}
	case PT_ORC_SUB: {
		val = orc_sub.total(aen);
		break;
	}
	case PT_DRAGON_SUB: {
		val = dragon_sub.total(aen);
		break;
	}
	case PT_UNDEAD_SUB: {
		val = undead_sub.total(aen);
		break;
	}
	case PT_DEMON_SUB: {
		val = demon_sub.total(aen);
		break;
	}
	case PT_SUMMON_ADD: {
		val = summon_add.total(aen);
		break;
	}
	case PT_SUMMON_SUB: {
		val = summon_sub.total(aen);
		break;
	}
	case PT_BE_CURE_RATE: {
		val = be_cure_rate.total(aen);
		break;
	}
	case PT_EFFECT_RATE: {
		val = effect_rate.total(aen);
		break;
	}
	case PT_PVE_HURT_ADD: {
		val = pve_hurt_add.total(aen);
		break;
	}
	case PT_PVE_HURT_SUB: {
		val = pve_hurt_sub.total(aen);
		break;
	}
	case PT_DIZZ_STRENGTHEN: {
		val = dizz_strengthen.total(aen);
		break;
	}
	case PT_DIZZ_WEAKEN: {
		val = dizz_weaken.total(aen);
		break;
	}
	default: {
		MSG_USER_TRACE("Property_Type = %d, Addition_Type = %d error !\n",
				pen, aen);
		break;
	}
	}

	return val;
}

void Fighter_Detail::modify_fight_property(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic, double fixed, double percent, int op_code) {
		if (aen < 0 || aen >= AT_END
				|| oen < 0 || oen >= OPERATE_TYPE_END){
			MSG_USER_TRACE("Property_Type = %d, Addition_Type = %d, Operate_Type = %d, value = %lf error !\n",
					pen, aen, oen, fixed);
			return ;
		}

		switch (pen) {
		case PT_POWER: {
			power.modify_property(aen, oen, basic, fixed, percent, op_code);

			double f_power_total = power.total(Prop_Value::ELEM_FORCE);
			double power_total = power.total();
			physical_attack.set_basic_single(f_power_total, AT_CONVERSION);
			physical_attack.set_fixed_single(power_total - f_power_total, AT_CONVERSION);
			spell_attack.set_basic_single(f_power_total, AT_CONVERSION);
			spell_attack.set_fixed_single(power_total - f_power_total, AT_CONVERSION);
//			update_force(aen);
			break;
		}
		case PT_AGILITY: {
			agility.modify_property(aen, oen, basic, fixed, percent, op_code);

			double f_agility_total = agility.total(Prop_Value::ELEM_FORCE);
			double agility_total = agility.total();
			hit_rate.set_basic_single(f_agility_total, AT_CONVERSION);
			hit_rate.set_fixed_single(agility_total - f_agility_total, AT_CONVERSION);
			crit.set_basic_single(f_agility_total, AT_CONVERSION);
			crit.set_fixed_single(agility_total - f_agility_total, AT_CONVERSION);
//			update_force(aen);
			break;
		}
		case PT_DEXTERITY: {
			dexterity.modify_property(aen, oen, basic, fixed, percent, op_code);

			double f_dexterity_total = dexterity.total(Prop_Value::ELEM_FORCE);
			double dexterity_total = dexterity.total();
			avoid.set_basic_single(f_dexterity_total, AT_CONVERSION);
			avoid.set_fixed_single(dexterity_total - f_dexterity_total, AT_CONVERSION);
			crit_def.set_basic_single(dexterity_total, AT_CONVERSION);
			crit_def.set_fixed_single(dexterity_total - f_dexterity_total, AT_CONVERSION);
//			update_force(aen);
			break;
		}
		case PT_ARMOR: {
			armor.modify_property(aen, oen, basic, fixed, percent, op_code);

			double f_armor_total = armor.total(Prop_Value::ELEM_FORCE);
			double armor_total = armor.total();
			physical_defense.set_basic_single(f_armor_total, AT_CONVERSION);
			physical_defense.set_fixed_single(armor_total - f_armor_total, AT_CONVERSION);
			spell_defense.set_basic_single(f_armor_total, AT_CONVERSION);
			spell_defense.set_fixed_single(armor_total - f_armor_total, AT_CONVERSION);
//			update_force(aen);
			break;
		}
		case PT_STAMINA: {
			stamina.modify_property(aen, oen, basic, fixed, percent, op_code);

			double f_stamina_total = stamina.total(Prop_Value::ELEM_FORCE);
			double stamina_total = stamina.total();
			double old_blood_max = blood_max.total();

			blood_max.set_basic_single(f_stamina_total, AT_CONVERSION);
			blood_max.set_fixed_single(stamina_total - f_stamina_total, AT_CONVERSION);
			double new_blood_max = blood_max.total();
			double delta_max = new_blood_max - old_blood_max;
			if (delta_max > 0.0 || delta_max < 0.0) {
				double new_current_blood = current_blood() + delta_max;
				new_current_blood = new_current_blood > blood_max.total() ? blood_max.total() :new_current_blood;
				new_current_blood = new_current_blood < 1.0 ? 1.0 : new_current_blood;
				blood_current.modify_property(AT_BASIC, O_SET, new_current_blood, 0.0, 0.0, BASE_OP);
			}
//			update_force(aen);
			break;
		}
		case PT_PHY_ATTACK: {
			physical_attack.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_SPELL_ATTACK: {
			spell_attack.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_PHY_DEFENSE: {
			physical_defense.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_SPELL_DEFENSE: {
			spell_defense.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_HIT: {
			hit_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_IGNORE_DEF: {
			ignore_def.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_AVOID: {
			avoid.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_CURE: {
			cure.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_CURE_RATE: {
			cure_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BLOOD_MAX: {
			double old_blood_max = blood_max.total();
			blood_max.modify_property(aen, oen, basic, fixed, percent, op_code);
			double new_blood_max = blood_max.total();
			double delta_max = new_blood_max - old_blood_max;
			if (delta_max > 0.0 || delta_max < 0.0) {
				double new_current_blood = current_blood() + delta_max;
				new_current_blood = new_current_blood > blood_max.total() ? blood_max.total() :new_current_blood;
				new_current_blood = new_current_blood < 1.0 ? 1.0 : new_current_blood;
				blood_current.modify_property(AT_BASIC, O_SET, new_current_blood, 0.0, 0.0, BASE_OP);
			}
//			update_force(aen);
			break;
		}
		case PT_SING_RATE: {
			sing_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_CRIT: {
			crit.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_CRIT_DEF: {
			crit_def.modify_property(aen, oen, basic, fixed, percent, op_code);
//			update_force(aen);
			break;
		}
		case PT_INIT_MORALE: {
			init_morale.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_MORALE_CAPS: {
			morale_caps.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_CRIT_HURT: {
			crit_hurt.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_CRIT_HURT_SUB: {
			crit_hurt_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_HIT_RATE_ADDITION: {
			hit_rate_addition.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_AVOID_RATE_ADDITION: {
			avoid_rate_addition.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_CRIT_RATE_ADDITION: {
			crit_rate_addition.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_CRIT_DEF_RATE_ADDITION: {
			crit_def_rate_addition.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SPEED: {
			speed.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BLOOD_CURRENT: {
			if (oen == O_SUB && current_blood() < basic) {
				blood_current.modify_property(aen, O_SET, 0.0, 0.0, 0.0, op_code);
				break;
			}
			if (oen == O_SET && basic < 0.0) {
				break;
			}
			blood_current.modify_property(aen, oen, basic, 0.0, 0.0, op_code);
			double blood_current_ = current_blood();
			double blood_max_ = blood_max.total();
			blood_current_ = blood_current_ > blood_max_ ? blood_max_ : blood_current_;
			blood_current_ = blood_current_ < 0.0 ? 0.0 : blood_current_;
			if (blood_current_ > current_blood() || blood_current_ < current_blood())
				blood_current.modify_property(AT_BASIC, O_SET, blood_current_, 0.0, 0.0,
						BASE_OP|FIX_OP|PERCENT_OP);
			break;
		}
		case PT_MORALE_CURRENT: {
			morale_current.modify_property(aen, oen, basic, fixed, percent, op_code);
			double morale_max_ = morale_caps.total();
			double current_moral_ = current_morale();
			if (current_moral_ > morale_max_)
				morale_current.modify_property(AT_BASIC, O_SET, morale_max_, 0.0, 0.0, BASE_OP);
			if (current_moral_ < 0.0)
				morale_current.modify_property(AT_BASIC, O_SET, 0.0, 0.0, 0.0, BASE_OP);
			break;
		}
		case PT_STRENGTH_CURRENT: {
			strength_current.modify_property(aen, oen, basic, fixed, percent, op_code);
			double max_strength = strength_max.total();
			double cur_strenght = strength_current.total();
			if (cur_strenght > max_strength || cur_strenght < 0.0) {
				cur_strenght = cur_strenght > max_strength ? max_strength : cur_strenght;
				cur_strenght = cur_strenght < 0.0 ? 0.0 : cur_strenght;
				strength_current.modify_property(AT_BASIC, O_SET, cur_strenght, 0.0, 0.0, BASE_OP);
			}
			break;
		}
		case PT_STRENGTH_MAX: {
			strength_max.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BLOOD_POOL_CURRENT: {
			if (oen == O_SUB && current_blood_pool() < basic) {
				basic = current_blood_pool();
			}
			if (oen == O_SET && basic < 0.0)
			break;
			blood_pool_current.modify_property(aen, oen, basic, 0.0, 0.0, op_code);
			double blood_pool_max_ = blood_pool_max.total();
			if (current_blood_pool() > blood_pool_max_)
			blood_pool_current.modify_property(AT_BASIC, O_SET, blood_pool_max_, 0.0, 0.0, op_code);
			break;
		}
		case PT_BLOOD_POOL_MAX: {
			blood_pool_max.modify_property(aen, oen, basic, fixed, percent, op_code);
			if (current_blood_pool() > blood_pool_max.total())
			blood_pool_current.modify_property(AT_BASIC, O_SET, blood_pool_max.total(), 0.0, 0.0, BASE_OP);
			break;
		}
		case PT_IGNORE_DEF_RATE: {
			ignore_def_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_STATUS_HIT_RATE: {
			status_hit_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_STATUS_DEF_RATE: {
			status_def_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BLOOD_STEAL: {
			blood_steal.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PHY_HURT_FIX: {
			phy_hurt_fix.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PHY_HURT_RATE: {
			phy_hurt_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SPELL_HURT_FIX: {
			spell_hurt_fix.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SPELL_HURT_RATE: {
			spell_hurt_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_HURT_FIX: {
			hurt_fix.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_HURT_RATE: {
			hurt_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BE_HURT_FIX: {
			be_hurt_fix.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BE_HURT_RATE: {
			be_hurt_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PK_HURT_ADD: {
			pk_hurt_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PK_HURT_SUB: {
			pk_hurt_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PHY_HURT_RATE_SUB: {
			phy_hurt_rate_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SPELL_HURT_RATE_SUB: {
			spell_hurt_rate_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SKILL_HURT_RATE: {
			skill_hurt_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_HUMAN_ADD: {
			human_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_ELF_ADD: {
			elf_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_ORC_ADD: {
			orc_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_DRAGON_ADD: {
			dragon_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_UNDEAD_ADD: {
			undead_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_DEMON_ADD: {
			demon_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_HUMAN_SUB: {
			human_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_ELF_SUB: {
			elf_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_ORC_SUB: {
			orc_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_DRAGON_SUB: {
			dragon_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_UNDEAD_SUB: {
			undead_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_DEMON_SUB: {
			demon_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SUMMON_ADD: {
			summon_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_SUMMON_SUB: {
			summon_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_BE_CURE_RATE: {
			be_cure_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_EFFECT_RATE: {
			effect_rate.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PVE_HURT_ADD: {
			pve_hurt_add.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_PVE_HURT_SUB: {
			pve_hurt_sub.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_DIZZ_STRENGTHEN: {
			dizz_strengthen.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		case PT_DIZZ_WEAKEN: {
			dizz_weaken.modify_property(aen, oen, basic, fixed, percent, op_code);
			break;
		}
		default: {
			MSG_USER_TRACE("Property_Type = %d, Addition_Type = %d, Operate_Type = %d, value = %lf error !\n",
					pen, aen, oen, fixed);
			break;
		}
		}

		return ;
}

void Fighter_Detail::property_list(Property_Vec &prop_list) const {

	prop_list.push_back(Property(PT_FORCE, (double)this->force));
	prop_list.push_back(Property(power.type, power.total()));
	prop_list.push_back(Property(agility.type, agility.total()));
	prop_list.push_back(Property(dexterity.type, dexterity.total()));
	prop_list.push_back(Property(armor.type, armor.total()));
	prop_list.push_back(Property(stamina.type, stamina.total()));

	prop_list.push_back(Property(physical_attack.type, physical_attack.total()));
	prop_list.push_back(Property(spell_attack.type, spell_attack.total()));
	prop_list.push_back(Property(physical_defense.type, physical_defense.total()));
	prop_list.push_back(Property(spell_defense.type, spell_defense.total()));
	prop_list.push_back(Property(hit_rate.type, hit_rate.total()));
	prop_list.push_back(Property(avoid.type, avoid.total()));
	prop_list.push_back(Property(cure.type, cure.total()));
	prop_list.push_back(Property(blood_max.type, blood_max.total()));
	prop_list.push_back(Property(crit.type, crit.total()));

	prop_list.push_back(Property(crit_def.type, crit_def.total()));
	prop_list.push_back(Property(init_morale.type, init_morale.total()));
	prop_list.push_back(Property(morale_caps.type, morale_caps.total()));
	prop_list.push_back(Property(crit_hurt.type, crit_hurt.total()));
	prop_list.push_back(Property(hit_rate_addition.type, hit_rate_addition.total()));
	prop_list.push_back(Property(avoid_rate_addition.type, avoid_rate_addition.total()));
	prop_list.push_back(Property(crit_rate_addition.type, crit_rate_addition.total()));
	prop_list.push_back(Property(crit_def_rate_addition.type, crit_def_rate_addition.total()));
	prop_list.push_back(Property(speed.type, speed.total()));
	prop_list.push_back(Property(blood_current.type, blood_current.total()));
	prop_list.push_back(Property(morale_current.type, morale_current.total()));
	prop_list.push_back(Property(blood_pool_current.type, blood_pool_current.total()));
	prop_list.push_back(Property(blood_pool_max.type, blood_pool_max.total()));
	prop_list.push_back(Property(status_hit_rate.type, status_hit_rate.total()));
	prop_list.push_back(Property(status_def_rate.type, status_def_rate.total()));
	prop_list.push_back(Property(be_cure_rate.type, be_cure_rate.total()));
	prop_list.push_back(Property(blood_steal.type, blood_steal.total()));
	prop_list.push_back(Property(sing_rate.type, sing_rate.total()));
	prop_list.push_back(Property(ignore_def.type, ignore_def.total()));
	prop_list.push_back(Property(pk_hurt_add.type, pk_hurt_add.total()));
	prop_list.push_back(Property(pk_hurt_sub.type, pk_hurt_sub.total()));
	prop_list.push_back(Property(PT_LEVEL, level));
}

const static int FIX_ARG = 5000;

void Fighter_Detail::update_force(void) {

	double phy_atk_ttl = physical_attack.total(Prop_Value::ELEM_FORCE);
	double spell_atk_ttl = spell_attack.total(Prop_Value::ELEM_FORCE);

	double atk_ttl = (phy_atk_ttl > spell_atk_ttl) ? phy_atk_ttl : spell_atk_ttl;

	double phy_def_ttl = physical_defense.total(Prop_Value::ELEM_FORCE);
	double spell_def_ttl = spell_defense.total(Prop_Value::ELEM_FORCE);

	double hp_ttl = blood_max.total(Prop_Value::ELEM_FORCE);
	double crit_ttl = crit.total(Prop_Value::ELEM_FORCE);
	double crit_def_ttl	= crit_def.total(Prop_Value::ELEM_FORCE);
	double hit_ttl = hit_rate.total(Prop_Value::ELEM_FORCE);
	double avoid_ttl = avoid.total(Prop_Value::ELEM_FORCE);
//	double ttl = (atk_ttl) + (phy_def_ttl+spell_def_ttl) * 0.5 +
//			hp_ttl * 0.1667 + crit_ttl * 2.5 + crit_def_ttl * 2.5 + hit_ttl * 5 + avoid_ttl * 5;
	double ttl = prop_force_ttl(atk_ttl, phy_def_ttl, spell_def_ttl,
			hp_ttl, crit_ttl, crit_def_ttl, hit_ttl, avoid_ttl);

	ttl += get_skill_force_ttl();

	force = static_cast<int>(ttl) - FIX_ARG;
}

double Fighter_Detail::prop_force_ttl(double atk, double phy_def, double spl_def,
		double hp, double crit, double crit_def, double hit, double avoid) {
	double ttl = atk + (phy_def + spl_def) * 0.5 +
			hp * 0.1667 + crit * 2.5 + crit_def * 2.5 + hit * 5 + avoid * 5;
	return ttl;
}

double Fighter_Detail::get_skill_force_ttl(void){

	double ttl = 0.0;
	Talent_Map::iterator find_it = talent_map.find(cur_used_talent_);
	if (find_it == talent_map.end()) return ttl;

	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Detail *dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id,
				it->second.skill_level);
		if (0 == dtl) continue;
		ttl += dtl->force_factor;
	}
	for (Skill_DB_Info_Vec::iterator it = extra_skill_vec.begin(); it != extra_skill_vec.end(); ++it) {
		const Skill_Detail* dtl = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id, it->skill_level);
		if (0 == dtl) continue;
		ttl += dtl->force_factor;
	}
	return ttl;
}

double Fighter_Detail::get_force_part(int aen) {

	if (AT_INITIATIVE_SKILL == aen) return 0.0;
	if (aen > AT_END) {
		return get_skill_force_ttl();
	}

	double phy_atk_ttl = physical_attack.total(Prop_Value::ELEM_FORCE);
	double spell_atk_ttl = spell_attack.total(Prop_Value::ELEM_FORCE);

	double atk_ttl = (phy_atk_ttl > spell_atk_ttl) ?
			physical_attack.log_value_total(aen) :
			spell_attack.log_value_total(aen);

	double phy_def_ttl = physical_defense.log_value_total(aen);
	double spell_def_ttl = spell_defense.log_value_total(aen);

	double hp_ttl = blood_max.log_value_total(aen);
	double crit_ttl = crit.log_value_total(aen);
	double crit_def_ttl	= crit_def.log_value_total(aen);
	double hit_ttl = hit_rate.log_value_total(aen);
	double avoid_ttl = avoid.log_value_total(aen);

	double ttl = prop_force_ttl(atk_ttl, phy_def_ttl, spell_def_ttl,
			hp_ttl, crit_ttl, crit_def_ttl, hit_ttl, avoid_ttl);

	if (AT_BASIC == aen) return ttl - FIX_ARG;
	return ttl;
}

int Fighter_Detail::reset_skill_cost(void) {
	int cost = 5000 * (reset_skill_times + 1);
	return cost < 100000 ? cost : 100000;
}

void Fighter_Detail::set_be_skill_attr(void) {
	Talent_Map::iterator find_it = talent_map.find(cur_used_talent_);
	if (find_it == talent_map.end()) return;
	for (Skill_DB_Info_Map::iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg) continue;
		if (Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail* detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->second.skill_id,
				it->second.skill_level);
		if (0 == detail) continue;
		for (Be_Skill_Effect_Map::const_iterator be_it = detail->be_skill_effect_map.begin();
				be_it != detail->be_skill_effect_map.end(); ++be_it) {
			for (Be_Skill_Attr_Vec::const_iterator a_it = be_it->second.attr_vec.begin();
					a_it != be_it->second.attr_vec.end(); ++a_it) {
				Operate_Type o_type = a_it->fix < 0 ? O_SUB : O_ADD;
				modify_fixed_property(static_cast<Property_Type>(a_it->id),
						Addition_Type::AT_PASSIVE_SKILL, o_type, fabs(a_it->fix) );
				o_type = a_it->percnet < 0 ? O_SUB : O_ADD;
				modify_percent_property(static_cast<Property_Type>(a_it->id),
						Addition_Type::AT_PASSIVE_SKILL, o_type, fabs(a_it->percnet));
			}
		}
	}

	for (Skill_DB_Info_Vec::iterator it = extra_skill_vec.begin();
			it != extra_skill_vec.end(); ++it) {
		const Skill_Config* cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->skill_id);
		if (0 == cfg) continue;
		if (Skill_Sort::BE_SKILL != cfg->skill_sort) continue;
		const Skill_Detail* detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(it->skill_id,
				it->skill_level);
		if (0 == detail) continue;
		for (Be_Skill_Effect_Map::const_iterator be_it = detail->be_skill_effect_map.begin();
				be_it != detail->be_skill_effect_map.end(); ++be_it) {
			for (Be_Skill_Attr_Vec::const_iterator a_it = be_it->second.attr_vec.begin();
					a_it != be_it->second.attr_vec.end(); ++a_it) {
				Operate_Type o_type = a_it->fix < 0 ? O_SUB : O_ADD;
				modify_fixed_property(static_cast<Property_Type>(a_it->id),
						Addition_Type::AT_PASSIVE_SKILL, o_type, fabs(a_it->fix) );
				o_type = a_it->percnet < 0 ? O_SUB : O_ADD;
				modify_percent_property(static_cast<Property_Type>(a_it->id),
						Addition_Type::AT_PASSIVE_SKILL, o_type, fabs(a_it->percnet));
			}
		}
	}
}

void Prop_Setter::serialize(Block_Buffer & w) const {
	w.write_int32(prop_type);
	w.write_int32(addi_type);
	w.write_uint8(operate);
	w.write_double(basic);
	w.write_double(fixed);
	w.write_double(percent);
}

int Prop_Setter::deserialize(Block_Buffer & r) {
	int prop_type_ =0, addition_type_ = 0;
	uint8_t operate_ = 0;
	if( r.read_int32(prop_type_)  ||  r.read_int32(addition_type_)  ||  r.read_uint8(operate_)  || r.read_double(basic) || r.read_double(fixed)  ||  r.read_double(percent)  )
			return -1;
	prop_type = Property_Type(prop_type_);
	addi_type = Addition_Type(addition_type_);
	operate = Operate_Type(operate_);
		return 0;
}

void Prop_Setter::reset(void) {
	prop_type = PT_START;
	addi_type = AT_END;
	operate = OPERATE_TYPE_END;
	basic = 0.0;
	fixed = 0.0;
	percent = 0.0;
}

void Common_Skill_Cd::reset(void) {
	type_ = 0;
	millisecond_ = 0;
	last_use_ = Time_Value::zero;
}

void Fighter_Buff::reset(void) {
	buff_id		= 0;
	duration	= 0;
	ext1		= 0;
	ext2		= 0;
	ext3		= -1;
}
