/*
 * Life_Skill_Def.h
 *
 *  Created on: 2015年3月10日
 *      Author: zhoujin
 */

#ifndef LIFE_SKILL_DEF_H_
#define LIFE_SKILL_DEF_H_

const int MAX_ENERGY = 100;
const int ENERGY_ON_TIME = 30*60;
const int ENERGY_IN_BONFIRE_TIME = 60;
const int ENERGY_ON_ADD = 5;
const int ENERGY_BONFIRE_ADD = 1;
const int ENERGY_SIX_ADD = 50;

enum Life_Skill_Type {
	Life_Skill_Collect = 1,
	Life_Skill_Make = 2,
	Life_Skill_SMelt = 3
};
enum Life_Skill_Qual {
	Life_Skill_Primary = 0,
	Life_Skill_Intermediate = 1,
	Life_Skill_Senior = 2,
	Life_Skill_Super = 3,
	Life_Skill_Top = 4,
};
enum Card_Make_Type {
	Card_Make_Primary = 0,
	Card_Make_Intermediate = 1,
	Card_Make_Senior = 2,
	Card_Make_Super = 3,
	Card_Make_Top = 4,
};
#endif /* LIFE_SKILL_DEF_H_ */
