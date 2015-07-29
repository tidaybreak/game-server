/*
 * Rune_Func.h
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_FUNC_H_
#define RUNE_FUNC_H_

#include "Rune_Struct.h"
#include "Rune_Config_Struct.h"
#include "pack/Pack_Def.h"

int exp_to_lvl(int &remain_exp, int &lvl, const int max_lvl, const Int_Vec &cfg_exp_vec) {
	// 经验转化为等级(满级不返还剩余经验)
	for (uint16_t i = lvl; i < cfg_exp_vec.size(); ++i) {
		int cfg_lvl_exp = cfg_exp_vec[i];
		if (remain_exp - cfg_lvl_exp >= 0) {
			// 够升级或还有剩
			remain_exp -= cfg_lvl_exp;
			if (++lvl >= max_lvl) {
				// 级满
				break;
			}
		} else {
			// 不够升级
			break;
		}
	}
	return 0;
}

int exp_to_lvl(int &remain_exp, int &lvl, const int max_lvl, const Int_Vec &cfg_exp_vec, int &sub_times) {
	// 经验转化为等级(满级不返还剩余经验)
	for (uint16_t i = lvl; i < cfg_exp_vec.size(); ++i) {
		int cfg_lvl_exp = cfg_exp_vec[i];
		++sub_times;
		if (remain_exp - cfg_lvl_exp >= 0) {
			// 够升级或还有剩
			remain_exp -= cfg_lvl_exp;
			if (++lvl >= max_lvl) {
				// 级满
				break;
			}
		} else {
			// 不够升级
			break;
		}
	}
	return 0;
}

//int exp_to_lvl_json(int &remain_exp, int &lvl, const int max_lvl, const Json::Value &json) {
//	// 经验转化为等级(满级不返还剩余经验)
//	for (uint16_t i = lvl; i < json.size(); ++i) {
//		int cfg_lvl_exp = json[i].asInt();
//		if (remain_exp - cfg_lvl_exp >= 0) {
//			// 够升级或还有剩
//			remain_exp -= cfg_lvl_exp;
//			if (++lvl >= max_lvl) {
//				// 级满
//				break;
//			}
//		} else {
//			// 不够升级
//			break;
//		}
//	}
//	return 0;
//}

bool is_hero_rune(const int rune_index) {
	return PACK_T_HERO_EQUIP_INDEX < rune_index && rune_index < PACK_T_HERO_EQUIP_INDEX_END;
}

#endif /* RUNE_FUNC_H_ */
