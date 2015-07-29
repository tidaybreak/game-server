/*
 * Expedition_Func.h
 *
 *  Created on: Jan 17, 2015
 *      Author: linqiyou
 */

#ifndef EXPEDITION_FUNC_H_
#define EXPEDITION_FUNC_H_

#include "Base_Struct.h"
#include "Server_Struct.h"
#include "Game_Typedef.h"

struct Expedition_Gang_Rank_Info;
struct Expedition_Gang_Brief_Info;
struct Soldier;
typedef std::list<Soldier*> Soldier_List;

int expedition_set_brief_info_from_gang_rank_info(const Expedition_Gang_Rank_Info& egri, Expedition_Gang_Brief_Info &egbi);

int insert_soldier_list(Soldier_List &list, Soldier *soldier);
int remove_soldier_list(Soldier_List &list, Soldier *soldier);

void modify_prop_for_monster_modify(Int_Double &id);

#endif /* EXPEDITION_FUNC_H_ */
