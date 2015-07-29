/*
 * Expedition_Func.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: linqiyou
 */

#include "Msg_Struct.h"
#include "Expedition_Func.h"
#include "expedition_scene/Expedition_Scene_Struct.h"

int expedition_set_brief_info_from_gang_rank_info(const Expedition_Gang_Rank_Info& egri, Expedition_Gang_Brief_Info &egbi) {
	egbi.uuid = egri.uuid;
	egbi.server_name = egri.server_name;
	egbi.gang_id = egri.gang_id;
	egbi.gang_name = egri.gang_name;
	return 0;
}


int insert_soldier_list(Soldier_List &list, Soldier *soldier) {
	for (Soldier_List::iterator it = list.begin(); it != list.end(); ++it) {
		if (*it == soldier) {
			return -1;
		}
	}
	list.push_back(soldier);
	return 0;
}

int remove_soldier_list(Soldier_List &list, Soldier *soldier) {
	list.remove(soldier);
	return 0;
}

void modify_prop_for_monster_modify(Int_Double &id) {
	switch(id.id) {
	case PT_PHY_ATTACK: {
		break;
	}
	case PT_SPELL_ATTACK: {
		break;
	}
	case PT_PHY_DEFENSE: {
		break;
	}
	case PT_SPELL_DEFENSE: {
		break;
	}
	case PT_BLOOD_MAX: {
		break;
	}
	default:
		break;
	}
	return;
}
