/*
 * Logic_Heroer_Func.h
 *
 *  Created on: Apr 3, 2014
 *      Author: chenyulin
 */

#ifndef LOGIC_HEROER_FUNC_H_
#define LOGIC_HEROER_FUNC_H_

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "equiper/Equiper_Def.h"
#include "equiper/Equiper.h"
#include "equiper/Equiper_Struct.h"
#include "equiper/Equiper_Func.h"
#include "Logic_Heroer_Def.h"
#include "Logic_Heroer.h"
#include "Logic_Heroer_Struct.h"
#include "Logic_Heroer_Func.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Hero_Struct.h"

struct Hero_Info_Detail;
struct Heroer_Detail;

inline bool check_hero_equip_idx_by_part(const int index, const int part) {
	int part_in_index = (index - PACK_T_HERO_EQUIP_INDEX) % ONE_HERO_EQUIP_GAP;
	int index_like_equip = PACK_T_EQUIP_INDEX + part_in_index;
	return check_equip_idx_by_part(index_like_equip, part);
}

void refresh_Hero_skill(Hero_Info_Detail &detail);

void refresh_Heros_skill(Heroer_Detail &detail);

void get_hero_assist_info(
		const Hero_Info_Detail &hero, Hero_Assist_Info &assist_info);

void figure_out_assist_hero_add_prop(
		const Hero_Info_Detail &hero, Int_Double_Map &prop_map);

double fighure_out_assist_prop_value(
		const Int_Double_Map& cfg_prop, const Int_Double_Map& cfg_correct,
		const Int_Double_Map& cfg_convert, const Property_Type& type1,
		const Property_Type& type2);

int figure_out_assist_hero_state(
		const Hero_Info_Detail &hero);

#endif /* LOGIC_HEROER_FUNC_H_ */
