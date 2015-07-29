/*
 * Equiper_Def.h
 *
 *  Created on: Mar 26, 2014
 *      Author: chenyulin
 */

#ifndef EQUIPER_DEF_H_
#define EQUIPER_DEF_H_

#define EQUIPER_TICK_SEC 10

enum Equip_Part {
	EP_BEGIN = 1,

	EP_WEAPONS = 1,		// 武器
	EP_HAT = 2,			// 帽子
	EP_CLOTHING = 3,	// 衣服
	EP_SHOES = 4,		// 鞋子
	EP_JEWELRY = 5,		// 首饰
	EP_RING = 6,		// 戒指
	EP_MEDAL = 7,		// 勋章

	EP_END,

	EP_REPEART_COUNT = 2,
};

enum {
	EQUIP_BEGIN = 1,
	EQUIP_END = 13,

	EQUIP_SUIT_BEGIN = 1,
	EQUIP_SUIT_END = 9,

	EQUIP_SUIT_ATT_AMOUNT = 4,
	EQUIP_SUIT_DEF_AMOUNT = 4,

	EQUIP_IDX_JEWELRY_BEGIN = 10005,
	EQUIP_IDX_RING_BEGIN = 10007,
	EQUIP_IDX_SUIT_END = 10009,
	EQUIP_IDX_REPEAT_GAP = 2,

	EQUIP_IDX_MEDAL = 10009,
};

enum {
	UNBAN_LVL_MAX = 12,
};

#endif /* EQUIPER_DEF_H_ */
