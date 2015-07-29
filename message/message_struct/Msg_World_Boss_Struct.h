/*
* Generate by devtool
*/

#ifndef _MSG_WORLD_BOSS_H_
#define _MSG_WORLD_BOSS_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求使用特殊技能
*/
struct MSG_10211100  {
	int8_t type;
	int8_t icon_id;
	MSG_10211100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		icon_id = 0;
	}
};

/*
请求使用特殊技能返回
*/
struct MSG_50211100 : public Base_Msg  {
	int8_t type;
	int8_t result;
	int8_t remain_num;
	int8_t icon_id;
	MSG_50211100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		w.write_int8(result);
		w.write_int8(remain_num);
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  r.read_int8(result)  ||  r.read_int8(remain_num)  ||  r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211100;
		type = 0;
		result = 0;
		remain_num = 0;
		icon_id = 0;
	}
};

/*
请求世界boss公会祝福
*/
struct MSG_10211101  {
	int8_t icon_id;
	MSG_10211101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		icon_id = 0;
	}
};

/*
请求世界boss公会祝福返回
*/
struct MSG_50211101 : public Base_Msg  {
	int8_t result;
	int8_t icon_id;
	MSG_50211101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50211101;
		result = 0;
		icon_id = 0;
	}
};

/*
请求世界boss击杀信息
*/
struct MSG_10111102  {
	int8_t icon_id;
	MSG_10111102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		icon_id = 0;
	}
};

/*
请求世界boss击杀信息返回
*/
struct MSG_50111102 : public Base_Msg  {
	int8_t icon_id;
	int32_t kill_num;
	int32_t value1;
	int32_t value2;
	int32_t boss_lv;
	MSG_50111102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
		w.write_int32(kill_num);
		w.write_int32(value1);
		w.write_int32(value2);
		w.write_int32(boss_lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  ||  r.read_int32(kill_num)  ||  r.read_int32(value1)  ||  r.read_int32(value2)  ||  r.read_int32(boss_lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50111102;
		icon_id = 0;
		kill_num = 0;
		value1 = 0;
		value2 = 0;
		boss_lv = 0;
	}
};

/*
请求设置世界boss开启时间
*/
struct MSG_10111103  {
	int8_t icon_id;
	int8_t hour;
	int8_t minute;
	MSG_10111103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
		w.write_int8(hour);
		w.write_int8(minute);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  ||  r.read_int8(hour)  ||  r.read_int8(minute)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		icon_id = 0;
		hour = 0;
		minute = 0;
	}
};

/*
请求设置世界boss开启时间
*/
struct MSG_50111103 : public Base_Msg  {
	int8_t icon_id;
	int8_t result;
	int8_t hour;
	int8_t minute;
	MSG_50111103(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(icon_id);
		w.write_int8(result);
		w.write_int8(hour);
		w.write_int8(minute);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(icon_id)  ||  r.read_int8(result)  ||  r.read_int8(hour)  ||  r.read_int8(minute)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50111103;
		icon_id = 0;
		result = 0;
		hour = 0;
		minute = 0;
	}
};



#endif