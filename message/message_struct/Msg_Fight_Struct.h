/*
* Generate by devtool
*/

#ifndef _MSG_FIGHT_H_
#define _MSG_FIGHT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
移动
*/
struct MSG_10200102  {
	int64_t role_id;
	std::vector<Coord> path;
	MSG_10200102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __path_vec_size = path.size();
		w.write_uint16(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				path[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __path_vec_size;
		if(r.read_uint16(__path_vec_size)  )
				return -1;
		path.reserve(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				Coord v;
				if(v.deserialize(r))
						return -1;
				path.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_id = 0;
		path.clear();
	}
};

/*
移动(返回)
*/
struct MSG_50200102 : public Base_Msg  {
	Coord coord;
	MSG_50200102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200102;
		coord.reset();
	}
};

/*
战斗准备完成
*/
struct MSG_10200105  {
	MSG_10200105(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
战斗准备完成(返回)
*/
struct MSG_50200105 : public Base_Msg  {
	int8_t count;
	MSG_50200105(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(count);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(count)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200105;
		count = 0;
	}
};

/*
使用技能
*/
struct MSG_10200110  {
	int64_t role_id;
	int32_t skill;
	MSG_10200110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(skill);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(skill)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		skill = 0;
	}
};

/*
使用技能(返回)
*/
struct MSG_50200110 : public Base_Msg  {
	MSG_50200110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50200110;
	}
};

/*
邀请决斗
*/
struct MSG_10200150  {
	int64_t role_id;
	uint8_t type;
	MSG_10200150(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		type = 0;
	}
};

/*
洗点技能
*/
struct MSG_10100165  {
	int32_t type;
	int32_t talent_id;
	MSG_10100165(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  ||  r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		talent_id = 0;
	}
};

/*
技能位置移动
*/
struct MSG_10100166  {
	int32_t skill_id;
	int32_t new_loc;
	int32_t talent_id;
	MSG_10100166(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(new_loc);
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(new_loc)  ||  r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
		new_loc = 0;
		talent_id = 0;
	}
};

/*
请求和怪物战斗
*/
struct MSG_10200151  {
	int64_t role_id;
	MSG_10200151(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
学习技能
*/
struct MSG_10100141  {
	int32_t talent_id;
	int32_t skill_id;
	int32_t lv;
	MSG_10100141(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
		w.write_int32(skill_id);
		w.write_int32(lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  ||  r.read_int32(skill_id)  ||  r.read_int32(lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		talent_id = 0;
		skill_id = 0;
		lv = 0;
	}
};

/*
学习技能(返回)
*/
struct MSG_50100141 : public Base_Msg  {
	int32_t talent_id;
	int32_t skill_point;
	Skill_Info skill;
	MSG_50100141(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
		w.write_int32(skill_point);
		skill.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  ||  r.read_int32(skill_point)  ||  skill.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50100141;
		talent_id = 0;
		skill_point = 0;
		skill.reset();
	}
};

/*
获取技能列表
*/
struct MSG_10100140  {
	int32_t talent_id;
	MSG_10100140(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		talent_id = 0;
	}
};

/*
获取技能列表（返回)
*/
struct MSG_50100140 : public Base_Msg  {
	int32_t cur_used_talent;
	int32_t reset_skill_cost;
	int32_t ttl_skill_point;
	std::vector<Skill_Talent_Info> skill_talent_info_vec;
	MSG_50100140(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(cur_used_talent);
		w.write_int32(reset_skill_cost);
		w.write_int32(ttl_skill_point);
		uint16_t __skill_talent_info_vec_vec_size = skill_talent_info_vec.size();
		w.write_uint16(__skill_talent_info_vec_vec_size);
		for(uint16_t i = 0; i < __skill_talent_info_vec_vec_size; ++i) {
				skill_talent_info_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(cur_used_talent)  ||  r.read_int32(reset_skill_cost)  ||  r.read_int32(ttl_skill_point)  )
				return -1;
 		uint16_t __skill_talent_info_vec_vec_size;
		if(r.read_uint16(__skill_talent_info_vec_vec_size)  )
				return -1;
		skill_talent_info_vec.reserve(__skill_talent_info_vec_vec_size);
		for(uint16_t i = 0; i < __skill_talent_info_vec_vec_size; ++i) {
				Skill_Talent_Info v;
				if(v.deserialize(r))
						return -1;
				skill_talent_info_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50100140;
		cur_used_talent = 0;
		reset_skill_cost = 0;
		ttl_skill_point = 0;
		skill_talent_info_vec.clear();
	}
};

/*
英雄移动
*/
struct MSG_10200103  {
	int64_t role_id;
	std::vector<Coord> path;
	MSG_10200103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		uint16_t __path_vec_size = path.size();
		w.write_uint16(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				path[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		uint16_t __path_vec_size;
		if(r.read_uint16(__path_vec_size)  )
				return -1;
		path.reserve(__path_vec_size);
		for(uint16_t i = 0; i < __path_vec_size; ++i) {
				Coord v;
				if(v.deserialize(r))
						return -1;
				path.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_id = 0;
		path.clear();
	}
};

/*
英雄移动（返回）
*/
struct MSG_50200103 : public Base_Msg  {
	Coord coord;
	MSG_50200103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		coord.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( coord.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200103;
		coord.reset();
	}
};

/*
开启指定页，指定天赋
*/
struct MSG_10100142  {
	int32_t page_id;
	int32_t talent_id;
	MSG_10100142(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(page_id);
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(page_id)  ||  r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		page_id = 0;
		talent_id = 0;
	}
};

/*
开启指定天赋
*/
struct MSG_10100143  {
	int32_t talent_id;
	MSG_10100143(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		talent_id = 0;
	}
};

/*
使用天赋
*/
struct MSG_10100144  {
	int32_t talent_id;
	MSG_10100144(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		talent_id = 0;
	}
};

/*
新手期战斗引导暂停
*/
struct MSG_10200111  {
	int32_t type;
	MSG_10200111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
新手期战斗引导暂停（返回）
*/
struct MSG_50200111 : public Base_Msg  {
	int32_t type;
	MSG_50200111(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200111;
		type = 0;
	}
};

/*
战斗时间通知
*/
struct MSG_50200112 : public Base_Msg  {
	int32_t msecs_left;
	int32_t give_up_time_left;
	MSG_50200112(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(msecs_left);
		w.write_int32(give_up_time_left);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(msecs_left)  ||  r.read_int32(give_up_time_left)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200112;
		msecs_left = 0;
		give_up_time_left = 0;
	}
};

/*
被邀请决斗(此消息非龙谷消息，是移动战斗类别的消息)
*/
struct MSG_50200153 : public Base_Msg  {
	int64_t role_id;
	std::string role_name;
	uint8_t type;
	MSG_50200153(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200153;
		role_id = 0;
		role_name.clear();
		type = 0;
	}
};

/*
应答邀请决斗
*/
struct MSG_10200155  {
	int64_t role_id;
	int8_t duel;
	uint8_t type;
	MSG_10200155(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(duel);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(duel)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		duel = 0;
		type = 0;
	}
};

/*
是否接受决斗
*/
struct MSG_50200155 : public Base_Msg  {
	int8_t duel;
	std::string role_name;
	MSG_50200155(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(duel);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(duel)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200155;
		duel = 0;
		role_name.clear();
	}
};

/*
发起挑战_普通场景
*/
struct MSG_10200156  {
	int64_t role_id;
	uint8_t type;
	MSG_10200156(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		type = 0;
	}
};

/*
战斗认输
*/
struct MSG_10200113  {
	MSG_10200113(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
触发追加技
*/
struct MSG_50200114 : public Base_Msg  {
	std::vector<Addition_Skill_Trigger_Info> trigger_vec;
	int64_t role_id;
	MSG_50200114(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __trigger_vec_vec_size = trigger_vec.size();
		w.write_uint16(__trigger_vec_vec_size);
		for(uint16_t i = 0; i < __trigger_vec_vec_size; ++i) {
				trigger_vec[i].serialize(w);
		}

		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __trigger_vec_vec_size;
		if(r.read_uint16(__trigger_vec_vec_size)  )
				return -1;
		trigger_vec.reserve(__trigger_vec_vec_size);
		for(uint16_t i = 0; i < __trigger_vec_vec_size; ++i) {
				Addition_Skill_Trigger_Info v;
				if(v.deserialize(r))
						return -1;
				trigger_vec.push_back(v);
		}

		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200114;
		trigger_vec.clear();
		role_id = 0;
	}
};

/*
战斗开始，通知前端被动技能修正CD
*/
struct MSG_50200115 : public Base_Msg  {
	std::vector<Int_Int> be_skill_fix_cd_vec;
	std::vector<Int_Double> be_skill_fix_morale_cost_vec;
	std::vector<Int_Int> skill_cd_vec;
	int64_t role_id;
	MSG_50200115(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __be_skill_fix_cd_vec_vec_size = be_skill_fix_cd_vec.size();
		w.write_uint16(__be_skill_fix_cd_vec_vec_size);
		for(uint16_t i = 0; i < __be_skill_fix_cd_vec_vec_size; ++i) {
				be_skill_fix_cd_vec[i].serialize(w);
		}

		uint16_t __be_skill_fix_morale_cost_vec_vec_size = be_skill_fix_morale_cost_vec.size();
		w.write_uint16(__be_skill_fix_morale_cost_vec_vec_size);
		for(uint16_t i = 0; i < __be_skill_fix_morale_cost_vec_vec_size; ++i) {
				be_skill_fix_morale_cost_vec[i].serialize(w);
		}

		uint16_t __skill_cd_vec_vec_size = skill_cd_vec.size();
		w.write_uint16(__skill_cd_vec_vec_size);
		for(uint16_t i = 0; i < __skill_cd_vec_vec_size; ++i) {
				skill_cd_vec[i].serialize(w);
		}

		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __be_skill_fix_cd_vec_vec_size;
		if(r.read_uint16(__be_skill_fix_cd_vec_vec_size)  )
				return -1;
		be_skill_fix_cd_vec.reserve(__be_skill_fix_cd_vec_vec_size);
		for(uint16_t i = 0; i < __be_skill_fix_cd_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				be_skill_fix_cd_vec.push_back(v);
		}

		uint16_t __be_skill_fix_morale_cost_vec_vec_size;
		if(r.read_uint16(__be_skill_fix_morale_cost_vec_vec_size)  )
				return -1;
		be_skill_fix_morale_cost_vec.reserve(__be_skill_fix_morale_cost_vec_vec_size);
		for(uint16_t i = 0; i < __be_skill_fix_morale_cost_vec_vec_size; ++i) {
				Int_Double v;
				if(v.deserialize(r))
						return -1;
				be_skill_fix_morale_cost_vec.push_back(v);
		}

		uint16_t __skill_cd_vec_vec_size;
		if(r.read_uint16(__skill_cd_vec_vec_size)  )
				return -1;
		skill_cd_vec.reserve(__skill_cd_vec_vec_size);
		for(uint16_t i = 0; i < __skill_cd_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				skill_cd_vec.push_back(v);
		}

		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200115;
		be_skill_fix_cd_vec.clear();
		be_skill_fix_morale_cost_vec.clear();
		skill_cd_vec.clear();
		role_id = 0;
	}
};

/*
使用天赋(返回)
*/
struct MSG_50100144 : public Base_Msg  {
	MSG_50100144(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50100144;
	}
};

/*
请求跟怪物战斗（不在场景的怪）
*/
struct MSG_10200149  {
	int32_t monster_type_id;
	int8_t type;
	MSG_10200149(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(monster_type_id);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(monster_type_id)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		monster_type_id = 0;
		type = 0;
	}
};

/*
多人战斗认输
*/
struct MSG_10200117  {
	int32_t type;
	MSG_10200117(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
多人战斗认输(返回)
*/
struct MSG_50200117 : public Base_Msg  {
	std::vector<Id_Int_Int> give_up_vec;
	int64_t sponsor;
	int8_t side;
	int32_t secs;
	MSG_50200117(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __give_up_vec_vec_size = give_up_vec.size();
		w.write_uint16(__give_up_vec_vec_size);
		for(uint16_t i = 0; i < __give_up_vec_vec_size; ++i) {
				give_up_vec[i].serialize(w);
		}

		w.write_int64(sponsor);
		w.write_int8(side);
		w.write_int32(secs);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __give_up_vec_vec_size;
		if(r.read_uint16(__give_up_vec_vec_size)  )
				return -1;
		give_up_vec.reserve(__give_up_vec_vec_size);
		for(uint16_t i = 0; i < __give_up_vec_vec_size; ++i) {
				Id_Int_Int v;
				if(v.deserialize(r))
						return -1;
				give_up_vec.push_back(v);
		}

		if( r.read_int64(sponsor)  ||  r.read_int8(side)  ||  r.read_int32(secs)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200117;
		give_up_vec.clear();
		sponsor = 0;
		side = 0;
		secs = 0;
	}
};

/*
多人战斗认输成功返回
*/
struct MSG_50200118 : public Base_Msg  {
	int32_t result;
	MSG_50200118(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50200118;
		result = 0;
	}
};



#endif