/*
 * Rune_Struct.h
 *
 *  Created on: May 15, 2014
 *      Author: chenyulin
 */

#ifndef RUNE_STRUCT_H_
#define RUNE_STRUCT_H_

#include "Server_Struct.h"

struct Rune_Part_Info;
struct Rune_Hole_Info;

/*
符文孔信息
*/
struct Rune_Hole_Detail {
	Rune_Hole_Detail(void) { reset(); };
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int32_t index;/*索引[1开始]*/
	int32_t level;/*等级*/
	int32_t energy;/*能量*/
};
//typedef boost::unordered_map<int, Rune_Hole_Detail> Rune_Hole_Detail_Map;
typedef std::map<int, Rune_Hole_Detail> Rune_Hole_Detail_Map;

/*
符文部位信息
*/
struct Rune_Part_Detail {
	Rune_Part_Detail(void) { reset(); };
	void serialize(Block_Buffer & w) const;
	int deserialize(Block_Buffer & r);
	void reset(void);

	int32_t part;/*符文部位[默认值是身上装备index,属性类型配置是装备部位]*/
	Rune_Hole_Detail_Map hole_detail_map;
};
//typedef boost::unordered_map<int, Rune_Part_Detail> Rune_Part_Detail_Map;
typedef std::map<int, Rune_Part_Detail> Rune_Part_Detail_Map;

/*
符文信息
*/
struct Rune_Detail: public Detail {
	Rune_Detail(void);
	virtual ~Rune_Detail();
	virtual void reset(void);

	virtual int serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);
	virtual int load(void);
	virtual int save(void);
	void get_rune_part_info(const int part, Rune_Part_Info &info) const;
	void get_rune_hole_info(const int part, const int hole_index, Rune_Hole_Info &info) const;

	Rune_Part_Detail_Map part_detail_map;
	std::map<int, double> all_rune_pro_val;				// 模块加成的属性，用于外部接口，不存
};

struct Bat_Consume_Info {
	Bat_Consume_Info(void) { reset(); }
	void reset(void) {
		copper = 0;
		money = 0;

		item1_id = 0;
		item1_num_erase = 0;
		item1_num_buy = 0;

		item2_id = 0;
		item2_num_erase = 0;
		item2_num_buy = 0;

		remain_exp = 0;
	}

	/*
	 * use money
	 */
	int copper;
	int money;	// 不包括铜钱的一种金钱

	/*
	 * use material(item1、item2)
	 */
	int item1_id;
	int item1_num_erase;	// pack erase
	int item1_num_buy;		// auto buy

	int item2_id;
	int item2_num_erase;
	int item2_num_buy;

	/*
	 * remain exp or energy
	 */
	int remain_exp;
};

#endif /* RUNE_STRUCT_H_ */
