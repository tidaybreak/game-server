/*
 * Campfire_Struct.h
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#ifndef CAMPFIRE_STRUCT_H_
#define CAMPFIRE_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"

struct Campfire_Detail:public Detail{

	Campfire_Detail(void);
	virtual ~Campfire_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	int id;
	int level;
	role_id_t lighter; //点火者
	int64_t gang_id;
	std::string gang_name;
	int firewood_num;
	int firewood_num_temp; //本场活动添加的次数 不保存
	int8_t burning;
	//
	int level_firewood_num;
	int level_max_firewood_num;
	boost::unordered_set<role_id_t> players_attacked_goddess;
	//
	Coord coord;
};
typedef boost::unordered_map<int64_t, Campfire_Detail> Campfire_Detail_Map;

// 副本相关信息
struct FB_Info_Detail:public Detail{
	FB_Info_Detail(void);
	virtual ~FB_Info_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	int best_force;					// 通最优通关战斗力
	role_id_t best_role_id;			// 最优通关玩家
	std::string best_role_name;	// 最优通关玩家
};
typedef boost::unordered_map<int64_t, FB_Info_Detail> FB_Info_Detail_Map;

#endif /* CAMPFIRE_STRUCT_H_ */
