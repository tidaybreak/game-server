/*
 * Active_Struct.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef ACTIVE_STRUCT_H_
#define ACTIVE_STRUCT_H_

#include "Server_Struct.h"
#include "Game_Typedef.h"
#include "Active_Config.h"

struct Switch_On_Event {
	Time_Value on_time; //事件应该执行时间
	uint32_t action;//事件代码 参考枚举:ACTION_TYPE
	UInt_Vec argv;//事件扩展参数
	bool is_run;//事件是否已经运行过
	Switch_On_Event(void) { reset(); }
	void reset(void);
};
typedef std::vector<Switch_On_Event> Switch_On_Event_Vec;
struct Switch_On_Active {
	Time_Value start_time;//开始时间 正式
	Time_Value before_start_time;//活动提前开始时间, 例如倒计时开始
	Time_Value end_time;//活动正常结束时间
	uint32_t id;//活动ID
	uint32_t type;//活动类型
	uint32_t icon;//活动图标
	uint32_t openLevel;//活动开启等级
	Switch_On_Event_Vec event_action;//活动所有执行事件列表
	Switch_On_Active(void) { reset(); }
	void reset(void);
};

typedef boost::unordered_map<uint32_t, Switch_On_Active> Switch_On_Active_Map;

struct Active_Data_Stauts {
	Active_Data_Stauts(void) { reset(); }
	Active_Data_Stauts(uint32_t id_, uint32_t num_, int64_t b, int64_t e) {
		id = id_;
		num = num_;
		begin_time.sec(b);
		end_time.sec(e);
	}
	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(num);
		w.write_int64(begin_time.sec());
		w.write_int64(end_time.sec());

	}

	int deserialize(Block_Buffer & r) {
		r.read_uint32(id);
		r.read_uint32(num);
		int64_t time = 0;
		r.read_int64(time);
		begin_time.sec(time);
		time = 0;
		r.read_int64(time);
		end_time.sec(time);
	 	return 0;
	}
	void reset(void);
	uint32_t id;
	uint32_t num;
	Time_Value begin_time;
	Time_Value end_time;
};

typedef boost::unordered_map<uint32_t, Active_Data_Stauts> Active_Data_Map;

struct DB_Active_Data_Info {
	DB_Active_Data_Info(void) { reset(); }
	void reset(void) {
		id = 0;
		start_time = Time_Value::zero;
		end_time = Time_Value::zero;
		midway_end_time = Time_Value::zero;
	}
	uint32_t id;
	Time_Value start_time;
	Time_Value end_time;
	Time_Value midway_end_time;
};
typedef boost::unordered_map<uint32_t, DB_Active_Data_Info> DB_Active_Data_Info_Map;

#endif /* ACTIVE_STRUCT_H_ */
