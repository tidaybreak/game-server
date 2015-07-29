/*
 * Fighter_Func.h
 *
 *  Created on: Mar 24, 2014
 *      Author: ti
 */

#ifndef FIGHTER_FUNC_H_
#define FIGHTER_FUNC_H_

#include "Fighter_Def.h"

class Fighter;
struct Attack_Param;
struct Skill_Detail;
struct Prop_Setter;
typedef std::vector<Prop_Setter> Prop_Setter_Vec;

void get_attack_by_default(const Attack_Param &attack_param, Fighter &attacker, Fighter &peer, double &attack);

double get_cure_by_default(const Fighter& attacker, Fighter &peer, int type, double value);

// 闪避率
double avoid_rate(Fighter &attacker, Fighter &peer);

// 暴击率
double crit_rate(Fighter &attacker, Fighter &peer);

Attack_Result select_attack_result(Fighter &attacker, Fighter &peer);

void get_be_skill_add_attr(const Skill_Detail &dtl, Prop_Setter_Vec& setter_vec, bool sub = false);

double get_race_hurt_add(Fighter &attacker, Fighter &injurer);

void be_skill_trigger_active_skill(Fighter &fighter, int type);

#endif /* FIGHTER_FUNC_H_ */
