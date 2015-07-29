/*
 * Mount_Struct.h
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#ifndef MOUNT_STRUCT_H_
#define MOUNT_STRUCT_H_

#include "Server_Struct.h"
#include "Msg_Struct.h"

struct Horse_Info{
	int horse_id;
	int8_t invalid;					//	是否无效 0：有效的 1：无效
	Time_Value deadline;
	Time_Value activate_time;
	int train_lv;					// 培养等级
	int train_num;				// 培养成功次数
	int train_fail_num;		   // 培养失败次数
	int8_t deploy;						//	是否出战 0：不出战 1：出战
	int deploy_skill_1;				// 出战技能
	int deploy_skill_2;
	int deploy_skill_3;
	//不存数据库的变量
	int luck_value;
	int cur_train_attr1;
	int cur_train_attr2;
	int cur_train_attr3;
	int cur_train_attr4;
	int cur_train_attr5;
	int max_train_attr1;
	int max_train_attr2;
	int max_train_attr3;
	int max_train_attr4;
	int max_train_attr5;
	int total_train_num;					// 驯养成功总次数
	int next_update_num;				   // 升星需要的次数(当前星级)
	int next_lv_total_train_num;		// 提升下一等级驯养成功总次数
	Horse_Info(void){
		reset();
	}
	void reset(void){
		horse_id = 0;
		invalid = 0;
		deadline = Time_Value::zero;
		activate_time = Time_Value::zero;

		train_lv = 0;
		train_num = 0;
		train_fail_num = 0;
		deploy = 0;
		deploy_skill_1 = 0;
		deploy_skill_2 = 0;
		deploy_skill_3 = 0;

		luck_value = 0;
		cur_train_attr1 = 0;
		cur_train_attr2 = 0;
		cur_train_attr3 = 0;
		cur_train_attr4 = 0;
		cur_train_attr5 = 0;
		max_train_attr1 = 0;
		max_train_attr2 = 0;
		max_train_attr3 = 0;
		max_train_attr4 = 0;
		max_train_attr5 = 0;
		total_train_num = 0;
		next_update_num = 0;
		next_lv_total_train_num = 0;
	}
};

struct Horse_Group_Info{
	int group_id;
	int status;			//0: noactive 1: activate 2:new activate

	Horse_Group_Info(void){
		reset();
	}

	void reset(void){
		group_id = 0;
		status = 0;
	}
};

struct Mount_Detail:public Detail{
	Mount_Detail(void);
	virtual ~Mount_Detail(void);

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	void add_horse(const int horse_id, const int train_lv, const Time_Value& deadline, const Time_Value& activate_time,
			const int cur_attr_1, const int cur_attr_2, const int cur_attr_3, const int cur_attr_4, const int cur_attr_5,
			const int max_attr_1, const int max_attr_2, const int max_attr_3, const int max_attr_4, const int max_attr_5,
			const int next_update_num);
	void del_horse(int horse_id);
	bool has_horse(int horse_id);
	bool horse_is_invalid(const int horse_id);
	void valid_horse(const int horse_id, const Time_Value& deadline);
	void invalid_horse(const int horse_id);
	int valid_group(int group_id, int status = 2);
	void invalid_group(int group_id);
	bool has_group(int group_id);
	int change_group_status(int group_id, int status);
	int get_first_valid_horse(void);
	// 获取坐骑驯养等级
	int get_train_lv(const int horse_id);
	int mount_id;														// 默认坐骑
	int ride;															//  0：下马 1：上马
	int deploy_num;													// 上阵坐骑个数
	std::vector<Horse_Info> horse_list;							// 拥有的坐骑
	std::vector<Horse_Group_Info> group_list;					// 获得的组合

};



#endif /* MOUNT_STRUCT_H_ */
