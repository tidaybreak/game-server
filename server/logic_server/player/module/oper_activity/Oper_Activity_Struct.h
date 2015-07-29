/*
 * Oper_Activity_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: xiaoliang
 */

#ifndef OPER_ACTIVITY_STRUCT_H_
#define OPER_ACTIVITY_STRUCT_H_

#include "Server_Struct.h"
#include "Oper_Activity_Def.h"

struct Oper_Activity_Record_Val {
	Oper_Activity_Record_Val(void) {
		reset();
	}
	void reset(void) {
		num = 0;
		ext1 = 0;
		ext2 = 0;
		ext3 = 0;
		type_num.clear();
	}
	uint32_t num;
	uint32_t ext1;
	uint32_t ext2;
	uint32_t ext3;
	UInt_UInt_Map type_num;
};


typedef boost::unordered_map<uint32_t, Oper_Activity_Record_Val> UInt_Oper_Activity_Record_Val_Map;
typedef boost::unordered_map<uint32_t, UInt_Oper_Activity_Record_Val_Map> UInt_UInt_Oper_Activity_Record_Val_Map;

typedef boost::unordered_map<uint32_t, UInt_Set> UInt_UInt_Set_Map;

struct Oper_Activity_Detail: public Detail {
	Oper_Activity_Detail(void);
	virtual ~Oper_Activity_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);

	UInt_Time_Map last_item_start_time;//每一项活动ID对应最后一次开始时间

	UInt_UInt_Oper_Activity_Record_Val_Map actid_type_record_val;//活动ID->类型->完成记录(次数,类型->值)
	UInt_UInt_Set_Map actid_receive_id;//活动ID对应已经领取的ID集合
	UInt_UIntUIntMap_Map actid_receive_id_num;//活动ID->(项目)领取ID->领取次数

	UInt_UInt_Set_Map actid_append_effect;//活动ID执行过的附加效果
	UInt_UInt_Time_Map actid_recid_last_refresh_time;//活动ID->(项目)领取ID->最后一次刷新时间
	UInt_UInt_Map actid_login_day;////活动ID->登录日时间
};

#endif /* OPER_ACTIVITY_STRUCT_H_ */
