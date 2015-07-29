/*
 * Equiper_Func.h
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#ifndef EQUIPER_FUNC_H_
#define EQUIPER_FUNC_H_

#include "Err_Code.h"
#include "pack/Pack_Def.h"
#include "Equiper_Def.h"
#include "fighter/Fighter_Def.h"
//#include "fighter/Logic_Fighter.h"
#include "Utility_Func.h"
#include "Equiper_Struct.h"

class Logic_Fighter;

inline bool is_equip_part(const int part) {
	return EP_BEGIN <= part && part < EP_END;
}

inline bool is_equip_repeatable_part(const int part) {
	return EP_JEWELRY == part || EP_RING == part;
}

inline bool is_true(const int i) {
	return i == 1;
}

inline bool check_equip_idx_by_part(const int index, const int part) {
	switch (part) {
	case EP_JEWELRY:
		return EQUIP_IDX_JEWELRY_BEGIN <= index && index < EQUIP_IDX_JEWELRY_BEGIN + EQUIP_IDX_REPEAT_GAP;
	case EP_RING:
		return EQUIP_IDX_RING_BEGIN <= index && index < EQUIP_IDX_RING_BEGIN + EQUIP_IDX_REPEAT_GAP;
	case EP_MEDAL:
		return Pack::PACK_T_EQUIP_INDEX + EP_REPEART_COUNT + EP_MEDAL == index;
	default:
		return Pack::PACK_T_EQUIP_INDEX + part == index;
	}
}

template<typename Key_Type, typename Map_Type>
inline typename Map_Type::mapped_type get_map_second_by_key(Key_Type key, const Map_Type &map) {
	typename Map_Type::const_iterator it = map.find(key);
	if (it != map.end()) {
		return it->second;
	} else {
		return 0;
	}
}

namespace Equip_Func {
	// 重置战斗属性
	void reset_fight_prop(Logic_Fighter &fighter, Addition_Type addi_type);
	// 装备基本属性分数
	int calculate_equip_force(const uint equip_id);
	int calculate_force(const std::map<int, double> &pro_val_map);
}
#define E_FUNC	Equip_Func

#endif /* EQUIPER_FUNC_H_ */
