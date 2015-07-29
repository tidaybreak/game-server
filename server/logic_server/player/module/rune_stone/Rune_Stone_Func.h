/*
 * Rune_Stone_Func.h
 *
 *  Created on: Aug 5, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_STONE_FUNC_H_
#define RUNE_STONE_FUNC_H_

#include "Rune_Stone_Struct.h"

inline int std_equip_cap(const int level) {
	int cap = level / 10;
	cap = cap > 8  ?  8 : cap;
	return cap;
}


#endif /* RUNE_STONE_FUNC_H_ */
