/*
 * Rune_Stone_Struct.h
 *
 *  Created on: Aug 5, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_STONE_STRUCT_H_
#define RUNE_STONE_STRUCT_H_

#include "Server_Struct.h"

struct Smelter_Info;

/*
熔炉
*/
struct Smelter_Detail {
	Smelter_Detail(void) { reset(); };
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int8_t smelter_idx;/*熔炉index[1-5]*/
	int8_t is_open;/*是否开启*/
};
typedef std::map<int, Smelter_Detail> Smelter_Detail_Map;

struct Rune_Stone_Detail: public Detail {
	enum {
		SMELTER_IDX_BEGIN = 1,
		SMELTER_IDX_END = 6,
	};

	Rune_Stone_Detail(void);
	virtual ~Rune_Stone_Detail();
	virtual void reset(void);

	virtual int serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);
	virtual int load(void);
	virtual int save(void);
	void make_smelter_info(const int smelter_idx, Smelter_Info &info) const;

	int points;
	int remain_double_times;
	Smelter_Detail_Map smelter_detail_map;
	std::map<int, int> combine_cnt_map;
	std::map<int, double> pro_val_map;				// 用于接口获取本模块的属性加成
};

#endif /* RUNE_STONE_STRUCT_H_ */
