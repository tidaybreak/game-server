/*
* Generate by devtool
*/

#ifndef _MSG_HERO_H_
#define _MSG_HERO_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取玩家英雄信息
*/
struct MSG_10102000  {
	MSG_10102000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取玩家英雄信息(返回)
*/
struct MSG_50102000 : public Base_Msg  {
	std::vector<Hero_Info> hero_info_vec;
	int32_t outer_index;
	int32_t fighter_index;
	int8_t show_hero;
	std::vector<Int_Int> assist_hero_vec;
	MSG_50102000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __hero_info_vec_vec_size = hero_info_vec.size();
		w.write_uint16(__hero_info_vec_vec_size);
		for(uint16_t i = 0; i < __hero_info_vec_vec_size; ++i) {
				hero_info_vec[i].serialize(w);
		}

		w.write_int32(outer_index);
		w.write_int32(fighter_index);
		w.write_int8(show_hero);
		uint16_t __assist_hero_vec_vec_size = assist_hero_vec.size();
		w.write_uint16(__assist_hero_vec_vec_size);
		for(uint16_t i = 0; i < __assist_hero_vec_vec_size; ++i) {
				assist_hero_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __hero_info_vec_vec_size;
		if(r.read_uint16(__hero_info_vec_vec_size)  )
				return -1;
		hero_info_vec.reserve(__hero_info_vec_vec_size);
		for(uint16_t i = 0; i < __hero_info_vec_vec_size; ++i) {
				Hero_Info v;
				if(v.deserialize(r))
						return -1;
				hero_info_vec.push_back(v);
		}

		if( r.read_int32(outer_index)  ||  r.read_int32(fighter_index)  ||  r.read_int8(show_hero)  )
				return -1;
 		uint16_t __assist_hero_vec_vec_size;
		if(r.read_uint16(__assist_hero_vec_vec_size)  )
				return -1;
		assist_hero_vec.reserve(__assist_hero_vec_vec_size);
		for(uint16_t i = 0; i < __assist_hero_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				assist_hero_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50102000;
		hero_info_vec.clear();
		outer_index = 0;
		fighter_index = 0;
		show_hero = 0;
		assist_hero_vec.clear();
	}
};

/*
英雄升级
*/
struct MSG_10102001  {
	int32_t hero_index;
	MSG_10102001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_index = 0;
	}
};

/*
英雄升级（返回）
备注：通过英雄信息更新81001001返回
*/
struct MSG_50102001 : public Base_Msg  {
	int8_t result;
	int32_t hero_index;
	int32_t hero_level;
	MSG_50102001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(hero_index);
		w.write_int32(hero_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(hero_index)  ||  r.read_int32(hero_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102001;
		result = 0;
		hero_index = 0;
		hero_level = 0;
	}
};

/*
英雄装备穿脱
*/
struct MSG_10102002  {
	uint32_t index_from;
	uint32_t index_to;
	MSG_10102002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index_from);
		w.write_uint32(index_to);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index_from)  ||  r.read_uint32(index_to)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index_from = 0;
		index_to = 0;
	}
};

/*
英雄装备穿脱(返回)
*/
struct MSG_50102002 : public Base_Msg  {
	MSG_50102002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50102002;
	}
};

/*
英雄跟随
*/
struct MSG_10102003  {
	int32_t hero_index;
	int8_t is_out;
	MSG_10102003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int8(is_out);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int8(is_out)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_index = 0;
		is_out = 0;
	}
};

/*
英雄跟随（返回）
*/
struct MSG_50102003 : public Base_Msg  {
	Hero_Info hero_info;
	int8_t is_die;
	int8_t is_success;
	int8_t is_alert_res;
	int8_t is_out;
	MSG_50102003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		hero_info.serialize(w);
		w.write_int8(is_die);
		w.write_int8(is_success);
		w.write_int8(is_alert_res);
		w.write_int8(is_out);
	}

	int deserialize(Block_Buffer & r) {
		if( hero_info.deserialize(r)  ||  r.read_int8(is_die)  ||  r.read_int8(is_success)  ||  r.read_int8(is_alert_res)  ||  r.read_int8(is_out)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102003;
		hero_info.reset();
		is_die = 0;
		is_success = 0;
		is_alert_res = 0;
		is_out = 0;
	}
};

/*
英雄招募
*/
struct MSG_10102004  {
	int32_t hero_id;
	int8_t use_diamond;
	MSG_10102004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
		w.write_int8(use_diamond);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  ||  r.read_int8(use_diamond)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_id = 0;
		use_diamond = 0;
	}
};

/*
英雄招募(返回)
*/
struct MSG_50102004 : public Base_Msg  {
	int32_t hero_id;
	int8_t result;
	int8_t use_diamond;
	MSG_50102004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
		w.write_int8(result);
		w.write_int8(use_diamond);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  ||  r.read_int8(result)  ||  r.read_int8(use_diamond)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102004;
		hero_id = 0;
		result = 0;
		use_diamond = 0;
	}
};

/*
英雄设置阵型
*/
struct MSG_10102005  {
	int32_t hero_index;
	int32_t hero_formation;
	int8_t is_alert;
	MSG_10102005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int32(hero_formation);
		w.write_int8(is_alert);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int32(hero_formation)  ||  r.read_int8(is_alert)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_index = 0;
		hero_formation = 0;
		is_alert = 0;
	}
};

/*
英雄设置阵型（返回）
*/
struct MSG_50102005 : public Base_Msg  {
	int32_t hero_index;
	int32_t hero_formation;
	int8_t is_alert;
	MSG_50102005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int32(hero_formation);
		w.write_int8(is_alert);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int32(hero_formation)  ||  r.read_int8(is_alert)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102005;
		hero_index = 0;
		hero_formation = 0;
		is_alert = 0;
	}
};

/*
设置战斗英雄
*/
struct MSG_10102006  {
	int32_t hero_index;
	int8_t is_fight;
	MSG_10102006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_index);
		w.write_int8(is_fight);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_index)  ||  r.read_int8(is_fight)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_index = 0;
		is_fight = 0;
	}
};

/*
设置战斗英雄（返回）
*/
struct MSG_50102006 : public Base_Msg  {
	Hero_Info hero_info;
	int8_t is_success;
	int8_t is_alert_res;
	uint8_t is_fight;
	MSG_50102006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		hero_info.serialize(w);
		w.write_int8(is_success);
		w.write_int8(is_alert_res);
		w.write_uint8(is_fight);
	}

	int deserialize(Block_Buffer & r) {
		if( hero_info.deserialize(r)  ||  r.read_int8(is_success)  ||  r.read_int8(is_alert_res)  ||  r.read_uint8(is_fight)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102006;
		hero_info.reset();
		is_success = 0;
		is_alert_res = 0;
		is_fight = 0;
	}
};

/*
英雄觉醒
*/
struct MSG_10102007  {
	int8_t is_hero;
	int32_t hero_index;
	MSG_10102007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_hero);
		w.write_int32(hero_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_hero)  ||  r.read_int32(hero_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_hero = 0;
		hero_index = 0;
	}
};

/*
英雄觉醒（返回）
*/
struct MSG_50102007 : public Base_Msg  {
	int8_t is_hero;
	int32_t hero_index;
	int8_t result;
	int32_t new_awake_skill;
	MSG_50102007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(is_hero);
		w.write_int32(hero_index);
		w.write_int8(result);
		w.write_int32(new_awake_skill);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(is_hero)  ||  r.read_int32(hero_index)  ||  r.read_int8(result)  ||  r.read_int32(new_awake_skill)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102007;
		is_hero = 0;
		hero_index = 0;
		result = 0;
		new_awake_skill = 0;
	}
};

/*
英雄神器升级
*/
struct MSG_10102020  {
	int32_t is_hero;
	int32_t artifact_id;
	int32_t hero_index;
	MSG_10102020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(is_hero);
		w.write_int32(artifact_id);
		w.write_int32(hero_index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(is_hero)  ||  r.read_int32(artifact_id)  ||  r.read_int32(hero_index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_hero = 0;
		artifact_id = 0;
		hero_index = 0;
	}
};

/*
英雄神器升级（返回）
*/
struct MSG_50102020 : public Base_Msg  {
	int32_t is_hero;
	int32_t artifact_id;
	int32_t hero_index;
	int8_t result;
	int8_t is_step_up;
	int32_t artifact_lvl;
	MSG_50102020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(is_hero);
		w.write_int32(artifact_id);
		w.write_int32(hero_index);
		w.write_int8(result);
		w.write_int8(is_step_up);
		w.write_int32(artifact_lvl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(is_hero)  ||  r.read_int32(artifact_id)  ||  r.read_int32(hero_index)  ||  r.read_int8(result)  ||  r.read_int8(is_step_up)  ||  r.read_int32(artifact_lvl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102020;
		is_hero = 0;
		artifact_id = 0;
		hero_index = 0;
		result = 0;
		is_step_up = 0;
		artifact_lvl = 0;
	}
};

/*
点击新英雄去掉new
*/
struct MSG_10102021  {
	int32_t hero_id;
	MSG_10102021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_id = 0;
	}
};

/*
点击新英雄去掉new（返回）
备注：

*/
struct MSG_50102021 : public Base_Msg  {
	int32_t hero_id;
	int8_t result;
	MSG_50102021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102021;
		hero_id = 0;
		result = 0;
	}
};

/*
获取新英雄信息
*/
struct MSG_10102022  {
	MSG_10102022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取点击过的新英雄信息（返回）
*/
struct MSG_50102022 : public Base_Msg  {
	std::vector<int32_t> clicked_new_heros;
	MSG_50102022(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __clicked_new_heros_vec_size = clicked_new_heros.size();
		w.write_uint16(__clicked_new_heros_vec_size);
		for(uint16_t i = 0; i < __clicked_new_heros_vec_size; ++i) {
				w.write_int32(clicked_new_heros[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __clicked_new_heros_vec_size;
		if(r.read_uint16(__clicked_new_heros_vec_size)  )
				return -1;
		clicked_new_heros.reserve(__clicked_new_heros_vec_size);
		for(uint16_t i = 0; i < __clicked_new_heros_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				clicked_new_heros.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50102022;
		clicked_new_heros.clear();
	}
};

/*
邀请英雄助阵
*/
struct MSG_10102032  {
	int8_t type;
	int32_t hero_idx;
	int32_t loc;
	MSG_10102032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int32(hero_idx);
		w.write_int32(loc);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int32(hero_idx)  ||  r.read_int32(loc)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		hero_idx = 0;
		loc = 0;
	}
};

/*
邀请英雄助阵（返回）
*/
struct MSG_50102032 : public Base_Msg  {
	int32_t result;
	std::vector<Int_Int> hero_assist_vec;
	MSG_50102032(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
		uint16_t __hero_assist_vec_vec_size = hero_assist_vec.size();
		w.write_uint16(__hero_assist_vec_vec_size);
		for(uint16_t i = 0; i < __hero_assist_vec_vec_size; ++i) {
				hero_assist_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		uint16_t __hero_assist_vec_vec_size;
		if(r.read_uint16(__hero_assist_vec_vec_size)  )
				return -1;
		hero_assist_vec.reserve(__hero_assist_vec_vec_size);
		for(uint16_t i = 0; i < __hero_assist_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				hero_assist_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50102032;
		result = 0;
		hero_assist_vec.clear();
	}
};

/*
助阵英雄互动
*/
struct MSG_10102033  {
	int32_t id;
	int32_t answer;
	int32_t hero_idx;
	MSG_10102033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(answer);
		w.write_int32(hero_idx);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(answer)  ||  r.read_int32(hero_idx)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		answer = 0;
		hero_idx = 0;
	}
};

/*
助阵英雄互动（返回）
*/
struct MSG_50102033 : public Base_Msg  {
	int32_t result;
	MSG_50102033(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102033;
		result = 0;
	}
};

/*
赠送助阵英雄礼品
*/
struct MSG_10102034  {
	int32_t item_id;
	int32_t hero_idx;
	MSG_10102034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(hero_idx);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(hero_idx)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		hero_idx = 0;
	}
};

/*
赠送助阵英雄礼品(返回）
*/
struct MSG_50102034 : public Base_Msg  {
	int32_t result;
	MSG_50102034(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102034;
		result = 0;
	}
};

/*
更新单个英雄助阵信息(返回）
*/
struct MSG_50102031 : public Base_Msg  {
	int32_t hero_idx;
	int32_t hero_id;
	Hero_Assist_Info hero_assist_info;
	MSG_50102031(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_idx);
		w.write_int32(hero_id);
		hero_assist_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_idx)  ||  r.read_int32(hero_id)  ||  hero_assist_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50102031;
		hero_idx = 0;
		hero_id = 0;
		hero_assist_info.reset();
	}
};



#endif