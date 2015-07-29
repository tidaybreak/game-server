/*
 * Fighter_Func.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: ti
 */

#include "Fighter_Detail.h"
#include "Fighter_Func.h"
#include "Fighter.h"
#include "Fighter_Struct.h"
#include "NPC/monster/Monster.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Enum.h"

// HP损失（初）= （攻击力×攻击力）/（攻击力＋防御力）
double fighter_calc_basic_physical_attack(const Attack_Param &attack_param,
		Fighter &attacker, Fighter &peer, bool is_ignore_def, double def_rate) {

	double attack = attacker.fighter_detail().fetch_fight_property(PT_PHY_ATTACK, Prop_Value::ELEM_FIGHT);
	double ignore_def = attacker.fighter_detail().fetch_fight_property(PT_IGNORE_DEF, Prop_Value::ELEM_FIGHT);
	double defense = peer.fighter_detail().fetch_fight_property(PT_PHY_DEFENSE);

	// 效果无视对方%防御
	defense *= 1 - ignore_def;

	defense = is_ignore_def ? 0.0 : defense;

	if (attack + defense <= 0.0) return 0.0;

	double phy_hurt_fix = attacker.fighter_detail().fetch_fight_property(PT_PHY_HURT_FIX, Prop_Value::ELEM_FIGHT);
	double phy_hurt_rate = attacker.fighter_detail().fetch_fight_property(PT_PHY_HURT_RATE, Prop_Value::ELEM_FIGHT);
	double phy_hurt_rate_sub = peer.fighter_detail().fetch_fight_property(PT_PHY_HURT_RATE_SUB, Prop_Value::ELEM_FIGHT);
	double skill_hurt_rate = attacker.fighter_detail().fetch_fight_property(PT_SKILL_HURT_RATE, Prop_Value::ELEM_FIGHT) + 1;

	double rate = 1 + phy_hurt_rate + phy_hurt_rate_sub;
	rate = rate < 0 ? 0 : rate;

	return pow(attack, 2) / (attack + defense * def_rate) * skill_hurt_rate * rate + phy_hurt_fix;
}

// HP损失（初）= （攻击力×攻击力）/（攻击力＋防御力）
double fighter_calc_basic_spell_attack(const Attack_Param &attack_param,
		Fighter &attacker, Fighter &peer, bool is_ignore_def, double def_rate) {
	double attack = attacker.fighter_detail().fetch_fight_property(PT_SPELL_ATTACK, Prop_Value::ELEM_FIGHT);
	double ignore_def = attacker.fighter_detail().fetch_fight_property(PT_IGNORE_DEF, Prop_Value::ELEM_FIGHT);
	double defense = peer.fighter_detail().fetch_fight_property(PT_SPELL_DEFENSE);

	// 效果无视对方%防御
	defense *= 1- ignore_def;

	defense = is_ignore_def ? 0.0 : defense;

	if (attack + defense <= 0.0) return 0.0;

	double spell_hurt_fix = attacker.fighter_detail().fetch_fight_property(PT_SPELL_HURT_FIX, Prop_Value::ELEM_FIGHT);
	double spell_hurt_rate = attacker.fighter_detail().fetch_fight_property(PT_SPELL_HURT_RATE, Prop_Value::ELEM_FIGHT);
	double spell_hurt_rate_sub = peer.fighter_detail().fetch_fight_property(PT_SPELL_HURT_RATE_SUB, Prop_Value::ELEM_FIGHT);
	double skill_hurt_rate = attacker.fighter_detail().fetch_fight_property(PT_SKILL_HURT_RATE, Prop_Value::ELEM_FIGHT) + 1;

	double rate = 1 + spell_hurt_rate + spell_hurt_rate_sub;
	rate = rate < 0 ? 0 : rate;

	return pow(attack, 2) / (attack + defense * def_rate) * skill_hurt_rate * rate + spell_hurt_fix;
}

const static int Race_Attr_Mapping[][2] = {{0,0},
										   {PT_HUMAN_ADD, PT_HUMAN_SUB},
										   {PT_ELF_ADD, PT_ELF_SUB},
										   {PT_ORC_ADD, PT_ORC_SUB},
										   {PT_DRAGON_ADD, PT_DRAGON_SUB},
										   {PT_UNDEAD_ADD, PT_UNDEAD_SUB},
										   {PT_DEMON_ADD, PT_DEMON_SUB},
										   {PT_SUMMON_ADD, PT_SUMMON_SUB}};

double get_race_hurt_add(Fighter &attacker, Fighter &injurer) {

	if (0 == injurer.monster_self()) return 0.0;
	if (0 == injurer.monster_self()->monster_detail().config_cache) return 0.0;
	double rate = 1.0;
	if (injurer.monster_self()->monster_detail().config_cache->type == MOVER_T_MONSTER_BOSS ||
		injurer.monster_self()->monster_detail().config_cache->type == MOVER_T_MONSTER_ELITE_BOSS) {
		rate = 0.2;
	}
	double value = 0.0;
	for (Int_Vec::const_iterator it = injurer.monster_self()->monster_detail().config_cache->race.begin();
			it != injurer.monster_self()->monster_detail().config_cache->race.end(); ++it) {
		if (*it < 0 || *it > 7) continue;
		value += attacker.fighter_detail().fetch_fight_property(static_cast<Property_Type>(Race_Attr_Mapping[*it][0]),
				Prop_Value::ELEM_FIGHT) * rate;
	}
	return value;
}

double get_race_hurt_sub(Fighter &attacker, Fighter &injurer) {
	if (0 == attacker.monster_self()) return 0.0;
	if (0 == attacker.monster_self()->monster_detail().config_cache) return 0.0;
	double rate = 1.0;
	if (attacker.monster_self()->monster_detail().config_cache->type == MOVER_T_MONSTER_BOSS ||
		attacker.monster_self()->monster_detail().config_cache->type == MOVER_T_MONSTER_ELITE_BOSS) {
		rate = 0.2;
	}
	double value = 0.0;
	for (Int_Vec::const_iterator it = attacker.monster_self()->monster_detail().config_cache->race.begin();
			it != attacker.monster_self()->monster_detail().config_cache->race.end(); ++it) {
		if (*it < 0 || *it > 7) continue;
		value += injurer.fighter_detail().fetch_fight_property(static_cast<Property_Type>(Race_Attr_Mapping[*it][1]),
				Prop_Value::ELEM_FIGHT) * rate;
	}
	return value;
}

double get_pve_hurt_add(Fighter &attacker, Fighter &injurer) {

	if (0 == injurer.monster_self()) return 0.0;
	if (0 == injurer.monster_self()->monster_detail().config_cache) return 0.0;
	if (injurer.monster_self()->monster_detail().config_cache->race.empty()) return 0.0;

	double value = attacker.fighter_detail().fetch_fight_property(PT_PVE_HURT_ADD, Prop_Value::ELEM_FIGHT);
	return value;
}

double get_pve_hurt_sub(Fighter &attacker, Fighter &injurer) {

	if (0 == attacker.monster_self()) return 0.0;
	if (0 == attacker.monster_self()->monster_detail().config_cache) return 0.0;
	if (attacker.monster_self()->monster_detail().config_cache->race.empty()) return 0.0;

	double value = injurer.fighter_detail().fetch_fight_property(PT_PVE_HURT_SUB, Prop_Value::ELEM_FIGHT);
	return value;
}

void get_attack_by_default(const Attack_Param &attack_param, Fighter &attacker, Fighter &peer, double &attack) {
	double ignore_def_rate = attacker.fighter_detail().fetch_fight_property(PT_IGNORE_DEF_RATE, Prop_Value::ELEM_FIGHT);
	double ignore_def_rate_val = (random() % 1000) / 1000.0;
	bool is_ignore_def = ignore_def_rate_val < ignore_def_rate;

	double def_rate = 1.0;
	if (attacker.is_pk_battle_type()) def_rate = 1.5;
	double physical_attack = fighter_calc_basic_physical_attack(attack_param,
			attacker, peer,is_ignore_def, def_rate);
	double spell_attack = fighter_calc_basic_spell_attack(attack_param,
			attacker, peer, is_ignore_def, def_rate);

	double hurt_fix = attacker.fighter_detail().fetch_fight_property(PT_HURT_FIX, Prop_Value::ELEM_FIGHT);
	double hurt_rate = attacker.fighter_detail().fetch_fight_property(PT_HURT_RATE, Prop_Value::ELEM_FIGHT);
	double be_hurt_fix = peer.fighter_detail().fetch_fight_property(PT_BE_HURT_FIX, Prop_Value::ELEM_FIGHT);
	double be_hurt_rate = peer.fighter_detail().fetch_fight_property(PT_BE_HURT_RATE, Prop_Value::ELEM_FIGHT);

	if (-1 != attack_param.attack_result) { // 伤害链引发的,不继续伤害链
		be_hurt_rate = peer.handle_hurt_link(attack_param, be_hurt_rate, attacker);
	}

	double rate = 1 + hurt_rate + be_hurt_rate;
	rate = rate < 0 ? 0 : rate;

	attack = (physical_attack + spell_attack) * rate + hurt_fix + be_hurt_fix;

	double race_add = get_race_hurt_add(attacker, peer);
	double race_sub = get_race_hurt_sub(attacker, peer);
	double pve_hurt_add = get_pve_hurt_add(attacker, peer);
	double pve_hurt_sub = get_pve_hurt_sub(attacker, peer);

	rate = 1 + race_add + race_sub + pve_hurt_add + pve_hurt_sub;

	if (attacker.is_pk_battle_type()) {
		double pk_hurt_add = attacker.fighter_detail().fetch_fight_property(PT_PK_HURT_ADD, Prop_Value::ELEM_FIGHT);
		rate += pk_hurt_add;
	}

	if (peer.is_pk_battle_type()) {
		double pk_hurt_sub = peer.fighter_detail().fetch_fight_property(PT_PK_HURT_SUB, Prop_Value::ELEM_FIGHT);
		rate += pk_hurt_sub;
	}

	rate = rate < 0.00000000001 ? 0.0 : rate;

	//attack = (physical_attack + spell_attack) * rate + hurt_fix + be_hurt_fix;
	attack = attack * rate;
	int base = static_cast<int>(attack * 0.1);
	double random_val = 0.0;
	if (base > 1) {
		random_val = (random() % base);
		random_val = random_val - static_cast<double>(base) / 2;
	}
	attack += random_val;
}

double get_cure_by_default(const Fighter& attacker, Fighter &peer, int type, double value) {
	double sum = 0.0;
	switch (type) {
	case CURE_BY_VALUE: {
		double phy_atk_base = attacker.fighter_detail().fetch_fight_property(PT_PHY_ATTACK, Prop_Value::ELEM_FIGHT);
		double spell_atk_base = attacker.fighter_detail().fetch_fight_property(PT_SPELL_ATTACK, Prop_Value::ELEM_FIGHT);
		sum = phy_atk_base + spell_atk_base + value;
		break;
	}
	case CURE_BY_PERCENT: {
		sum = peer.blood_max() * value;
		break;
	}
	default:
		break;
	}
	double add_basic = attacker.fighter_detail().fetch_fight_property(PT_CURE, Prop_Value::ELEM_FIGHT);
	sum += add_basic;
	double percent = attacker.fighter_detail().fetch_fight_property(PT_CURE_RATE, Prop_Value::ELEM_FIGHT);
	percent += peer.fighter_detail().fetch_fight_property(PT_BE_CURE_RATE, Prop_Value::ELEM_FIGHT);
	percent = peer.handle_cure_link(const_cast<Fighter&>(attacker), sum, percent);
	return sum * (percent + 1);
}

// 闪避率
double avoid_rate(Fighter &attacker, Fighter &peer) {
	// 命中率＝命中 /4000*（1-闪避/4000）＋命中率加成－闪避率加成+初始命中率
	// 初始命中率 = 0.75
	double hit = attacker.fighter_detail().fetch_fight_property(PT_HIT, Prop_Value::ELEM_FIGHT);
	double avoid = peer.fighter_detail().fetch_fight_property(PT_AVOID, Prop_Value::ELEM_FIGHT);

	if (hit + avoid <= 0.0) return 0.0;

	double avoid_rate = 1 - (hit / 4000 * (1 - avoid / 4000)) - 0.75;
	avoid_rate -= attacker.fighter_detail().fetch_fight_property(PT_HIT_RATE_ADDITION, Prop_Value::ELEM_FIGHT);
	avoid_rate += peer.fighter_detail().fetch_fight_property(PT_AVOID_RATE_ADDITION, Prop_Value::ELEM_FIGHT);

	return avoid_rate;
}

// 暴击率
double crit_rate(Fighter &attacker, Fighter &peer) {
	// 暴击率＝暴击/（等级*100）*（1-暴击防御/（等级*100））＋（暴击率加成－暴击抵抗率加成）
	int atk_lv = attacker.level();
	int pr_lv = peer.level();
	double get_crit = attacker.fighter_detail().fetch_fight_property(PT_CRIT, Prop_Value::ELEM_FIGHT);
	double crit =  (get_crit < 0.00001) ? get_crit : (get_crit  / (atk_lv * 100 + 500));
	double get_crit_def = peer.fighter_detail().fetch_fight_property(PT_CRIT_DEF, Prop_Value::ELEM_FIGHT);
	double crit_def = (get_crit_def < 0.00001) ? get_crit_def : (get_crit_def / (pr_lv * 100 + 500));

	if (crit + crit_def < 0.0) return 0.0;

	double crit_rate = crit - crit_def;
	crit_rate += attacker.fighter_detail().fetch_fight_property(PT_CRIT_RATE_ADDITION, Prop_Value::ELEM_FIGHT);
	crit_rate -= peer.fighter_detail().fetch_fight_property(PT_CRIT_DEF_RATE_ADDITION, Prop_Value::ELEM_FIGHT);

	return crit_rate;
}

Attack_Result select_attack_result(Fighter &attacker, Fighter &peer) {
	double avoid_rate_val = avoid_rate(attacker, peer);
	double crit_rate_val = crit_rate(attacker, peer);
	double normal_rate_val = 0.0;

	if (avoid_rate_val + crit_rate_val <= 1.0) {
		normal_rate_val = 1.0 - avoid_rate_val - crit_rate_val;
	} else {
		crit_rate_val = 1.0 - avoid_rate_val;
	}

	double random_val = (random() % 100) / 100.0;

	if (random_val < avoid_rate_val)
		return ATTACK_AVOID;
	if (random_val < (avoid_rate_val + crit_rate_val))
		return ATTACK_CRIT;

	return ATTACK_NORMAL;
}

void get_be_skill_add_attr(const Skill_Detail& dtl, Prop_Setter_Vec& setter_vec, bool sub) {
	typedef Be_Skill_Effect_Map::const_iterator c_iter;
	typedef Be_Skill_Attr_Vec::const_iterator a_c_iter;
	Prop_Setter setter;
	for (c_iter it = dtl.be_skill_effect_map.begin(); it != dtl.be_skill_effect_map.end(); ++it) {
		for (a_c_iter a_it = it->second.attr_vec.begin(); a_it != it->second.attr_vec.end(); ++a_it) {
			setter.reset();
			setter.prop_type = static_cast<Property_Type>(a_it->id);
			setter.addi_type = Addition_Type::AT_PASSIVE_SKILL;
			setter.operate = Operate_Type::O_ADD;
			setter.basic = 0;
			if (sub) {
				setter.fixed = -a_it->fix;
				setter.percent = -a_it->percnet;
			} else {
				setter.fixed = a_it->fix;
				setter.percent = a_it->percnet;
			}
			setter_vec.push_back(setter);
		}
	}
}

void be_skill_trigger_active_skill(Fighter &fighter, int type) {

	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_ASYNC_BE_SKILL_CALL_ACTIVE_SKILL);
	inner_buf.write_int64(fighter.role_id());
	inner_buf.write_int8(type);
	inner_buf.finish_message();

	SCENE_MONITOR->push_async_buff(inner_buf);
}
