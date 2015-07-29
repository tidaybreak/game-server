/*
* Generate by devtool
*/

#ifndef _MSG_LIFE_SKILL_H_
#define _MSG_LIFE_SKILL_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
打开生活技能界面
*/
struct MSG_10116000  {
	MSG_10116000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
打开生活技能界面返回
*/
struct MSG_50116000 : public Base_Msg  {
	std::vector<Int_Int> life_skill_vec;
	MSG_50116000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __life_skill_vec_vec_size = life_skill_vec.size();
		w.write_uint16(__life_skill_vec_vec_size);
		for(uint16_t i = 0; i < __life_skill_vec_vec_size; ++i) {
				life_skill_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __life_skill_vec_vec_size;
		if(r.read_uint16(__life_skill_vec_vec_size)  )
				return -1;
		life_skill_vec.reserve(__life_skill_vec_vec_size);
		for(uint16_t i = 0; i < __life_skill_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				life_skill_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50116000;
		life_skill_vec.clear();
	}
};

/*
学习生活技能
*/
struct MSG_10116001  {
	uint8_t type;
	MSG_10116001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
学习生活技能返回
*/
struct MSG_50116001 : public Base_Msg  {
	Int_Int life_skill;
	MSG_50116001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		life_skill.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( life_skill.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50116001;
		life_skill.reset();
	}
};

/*
生活技能升级
*/
struct MSG_10116002  {
	int32_t skill_id;
	MSG_10116002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
生活技能升级返回
*/
struct MSG_50116002 : public Base_Msg  {
	Int_Int life_skill;
	uint8_t type;
	MSG_50116002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		life_skill.serialize(w);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( life_skill.deserialize(r)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50116002;
		life_skill.reset();
		type = 0;
	}
};

/*
晶石精练
*/
struct MSG_10116003  {
	std::vector<uint32_t> id_vec;
	MSG_10116003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __id_vec_vec_size = id_vec.size();
		w.write_uint16(__id_vec_vec_size);
		for(uint16_t i = 0; i < __id_vec_vec_size; ++i) {
				w.write_uint32(id_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __id_vec_vec_size;
		if(r.read_uint16(__id_vec_vec_size)  )
				return -1;
		id_vec.reserve(__id_vec_vec_size);
		for(uint16_t i = 0; i < __id_vec_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				id_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		id_vec.clear();
	}
};

/*
晶石精练返回
*/
struct MSG_50116003 : public Base_Msg  {
	uint32_t item_id;
	MSG_50116003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50116003;
		item_id = 0;
	}
};

/*
卡牌制作
*/
struct MSG_10116004  {
	uint8_t type;
	MSG_10116004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
卡牌制作返回
*/
struct MSG_50116004 : public Base_Msg  {
	uint8_t type;
	MSG_50116004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50116004;
		type = 0;
	}
};

/*
晶石熔炼
*/
struct MSG_10116005  {
	int32_t tab_id;
	uint16_t amont;
	MSG_10116005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(tab_id);
		w.write_uint16(amont);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(tab_id)  ||  r.read_uint16(amont)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		tab_id = 0;
		amont = 0;
	}
};

/*
晶石熔炼返回
*/
struct MSG_50116005 : public Base_Msg  {
	uint8_t type;
	MSG_50116005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50116005;
		type = 0;
	}
};



#endif