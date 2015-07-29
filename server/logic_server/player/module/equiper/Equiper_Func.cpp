/*
 * Equiper_Func.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#include "Equiper_Func.h"
#include "Config_Cache_Role.h"
#include "Fighter_Detail.h"
#include "task/Tasker.h"
#include "fighter/Logic_Fighter.h"
#include "Config_Cache_Item.h"

void Equip_Func::reset_fight_prop(Logic_Fighter &fighter, Addition_Type addi_type) {
	Prop_Setter_Vec setter_vec;
	Prop_Setter setter;
	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		setter.reset();
		setter.prop_type = static_cast<Property_Type>(*it_prop);
		setter.operate = O_SET;
		setter.addi_type = addi_type;
		setter.basic = 0.0;
		setter.fixed = 0.0;
		setter.percent = 0.0;
		setter_vec.push_back(setter);
	}
	fighter.fight_prop_modify(setter_vec, 214);
}

int Equip_Func::calculate_equip_force(const uint equip_id) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(equip_id);
	if (! item_config) {
		return 0;
	}

	std::map<int, double> pro_val_map;
	const Prop_Value_Vec &cfg_prop_val_vec = item_config->equip_cfg.prop_val_vec;
	if (! cfg_prop_val_vec.empty()) {
		for (size_t i = 0; i < cfg_prop_val_vec.size(); ++i) {
			pro_val_map[cfg_prop_val_vec[i].id] += cfg_prop_val_vec[i].value;
		}
	}

	if (! pro_val_map.empty()) {
		Equip_Force_Info force_info;
		for (std::map<int, double>::const_iterator it_pro_val_map = pro_val_map.begin();
				it_pro_val_map != pro_val_map.end(); ++it_pro_val_map) {
			if (it_pro_val_map->first && it_pro_val_map->second > 0.0) {
				std::map<int, double>::iterator it_force = force_info.pt_val_map.find(it_pro_val_map->first);
				if (it_force != force_info.pt_val_map.end()) {
					it_force->second += it_pro_val_map->second;
				}
			}
		}
		return ceil(force_info.calculate_force());
	}

	return 0;
}

int Equip_Func::calculate_force(const std::map<int, double> &pro_val_map) {
	if (! pro_val_map.empty()) {
		Equip_Force_Info force_info;
		for (std::map<int, double>::const_iterator it_pro_val_map = pro_val_map.begin();
				it_pro_val_map != pro_val_map.end(); ++it_pro_val_map) {
			if (it_pro_val_map->first && it_pro_val_map->second > 0.0) {
				std::map<int, double>::iterator it_force = force_info.pt_val_map.find(it_pro_val_map->first);
				if (it_force != force_info.pt_val_map.end()) {
					it_force->second += it_pro_val_map->second;
				}
			}
		}
		return ceil(force_info.calculate_force());
	}
	return 0;
}

