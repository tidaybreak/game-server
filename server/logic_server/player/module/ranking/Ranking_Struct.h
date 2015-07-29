/*
 * Ranking_Struct.h
 *
 *  Created on: 2014-05-22
 *      Author: xiaoliang
 */

#ifndef RANKING_STRUCT_H_
#define RANKING_STRUCT_H_

#include "Game_Typedef.h"
#include "Msg_Struct.h"
#include "Msg_Ranking_Struct.h"

typedef std::list<Ranking_Info> Ranking_Sort_List;
typedef std::map<int, Ranking_Info, std::less<int> > Ranking_Sort_Map;


struct Rank_Role_Info : public MSG_50171001 {};

typedef std::vector<Rank_Role_Info> Rank_Role_Info_Vec;

struct Rank_Item_All_Info : public MSG_50171002 {};
typedef std::vector<Rank_Item_All_Info> Rank_Item_All_Info_Vec;

struct Rank_Heroer_Info : public MSG_50171003 {};
typedef std::vector<Rank_Heroer_Info> Rank_Heroer_Info_Vec;

struct Ranking {
	Ranking(void) { reset(); };
	void reset(void) {
		tick_time = Time_Value(0);
		update_time = Time_Value(0);
		tick_sec = 0;
		ranking_.clear();
		CLEAR_VECTOR(Rank_Role_Info_Vec, role_info_);
		CLEAR_VECTOR(Rank_Item_All_Info_Vec, item_info_);
		CLEAR_VECTOR(Rank_Heroer_Info_Vec, heroer_info_);
		is_change = false;
		type = 0;
	}
	void serialize(Block_Buffer & w) const {
		w.write_int64(tick_time.sec());
		w.write_int64(update_time.sec());
		w.write_int32(tick_sec);
		w.write_int8(ranking_.size());
		for(Ranking_Sort_List::const_iterator iter = ranking_.begin(); iter != ranking_.end(); ++iter) {
			iter->serialize(w);
		}
		w.write_int8(role_info_.size());
		for(Rank_Role_Info_Vec::const_iterator iter = role_info_.begin(); iter != role_info_.end(); ++iter) {
			iter->serialize(w);
		}
		w.write_int8(item_info_.size());
		for(Rank_Item_All_Info_Vec::const_iterator iter = item_info_.begin(); iter != item_info_.end(); ++iter) {
			iter->serialize(w);
		}
		w.write_int8(heroer_info_.size());
		for(Rank_Heroer_Info_Vec::const_iterator iter = heroer_info_.begin(); iter != heroer_info_.end(); ++iter) {
			iter->serialize(w);
		}
		w.write_bool(is_change);
		w.write_uint32(type);
	}

	int deserialize(Block_Buffer & r) {
		int64_t tick_time_ = 0;
		if( r.read_int64(tick_time_))
			return -1;
		tick_time.set(tick_time_,0);

		int64_t update_time_ = 0;
		if( r.read_int64(update_time_))
			return -1;
		update_time.set(update_time_,0);

		r.read_int32(tick_sec);

		int8_t size = 0;
		if( r.read_int8(size) )
			return -1;
		Ranking_Info ri;
		for(;size > 0; --size) {
			ri.reset();
			ri.deserialize(r);
			ranking_.push_back(ri);
		}

		int8_t rri_size = 0;
		if( r.read_int8(rri_size) )
			return -1;
		Rank_Role_Info rri;
		for(;rri_size > 0; --rri_size) {
			rri.reset();
			rri.deserialize(r);
			role_info_.push_back(rri);
		}

		int8_t ria_size = 0;
		if( r.read_int8(ria_size) )
			return -1;
		Rank_Item_All_Info ria;
		for(;ria_size > 0; --ria_size) {
			ria.reset();
			ria.deserialize(r);
			item_info_.push_back(ria);
		}

		int8_t rhi_size = 0;
		if( r.read_int8(rhi_size) )
			return -1;
		Rank_Heroer_Info rhi;
		for(;rhi_size > 0; --rhi_size) {
			rhi.reset();
			rhi.deserialize(r);
			heroer_info_.push_back(rhi);
		}

		if( r.read_bool(is_change) )
			return -1;
		r.read_uint32(type);
	 	return 0;
	}
	Time_Value tick_time;
	Time_Value update_time;
	int tick_sec;
	Ranking_Sort_List ranking_;
	Rank_Role_Info_Vec role_info_;
	Rank_Item_All_Info_Vec item_info_;
	Rank_Heroer_Info_Vec heroer_info_;
	bool is_change;
	uint32_t type;
};

typedef boost::unordered_map<int, Ranking> Ranking_Map;



#endif /* RANKING_STRUCT_H_ */
