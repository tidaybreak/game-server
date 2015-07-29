/*
 * Logic_Mount.h
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#ifndef LOGIC_MOUNT_H_
#define LOGIC_MOUNT_H_


#include "Logic_Player_Base.h"
struct Mount_Detail;
struct Horse_Info;
struct Prop_Setter;
typedef std::vector<Prop_Setter> Prop_Setter_Vec;

class Logic_Mount: virtual public Logic_Player_Base{
public:
	Logic_Mount(void);
	virtual ~Logic_Mount(void);
public:
	void reset(void);

	void load_detail(Mount_Detail &detail);
	void save_detail(Mount_Detail &detail);

	const Mount_Detail& mount_detail(void) const;

	void daily_refresh(void);
	// 凌晨0点刷新
	int trigger_daily_zero(const Time_Value &now);
	// 凌晨6点刷新
	int trigger_daily_six(const Time_Value &now);

	void module_init(void);
	static int create_init(Mount_Detail& detail);
	void sign_in(void);
	void sign_out(void);

	int listen_mount_deadline(const Time_Value& now);
public:
	// 获取激活坐骑个数
	int get_mount_num(void);
	// listen vip level change
	void mount_listen_vip_level_change(uint32_t new_vip_level, uint32_t old_vip_level);
	// 获取所有激活坐骑星星等级
	void get_mount_star_lv(std::vector<Int_Int> &info);
	// 获取组合、上阵技能
	void get_mount_skill(Int_Vec& skills);
	// 驯养属性
	void get_mount_props_for_hero(Prop_Setter_Vec &vec);
public:
	// 请求坐骑信息
	int req_fecth_mount_info(void);
	// 请求坐骑马厩信息
	int req_fecth_mount_stable_info(void);
	// 请求坐骑骑乘
	int req_mount_ride(int ride, int horse_id);
	// 请求坐骑激活
	int req_mount_activate(const int horse_id, const uint8_t type);
	// 点击新组合
	int req_click_mount_group(int group_id);

	// 请求培养
	int req_mount_train(int horse_id);
	// 一键驯养 type = 1 自动购买
	int req_mount_train_auto_lv(int horse_id, int type);
	// 请求上阵
	int req_mount_deploy(int horse_id, int deploy_pos);
	// 请求学习坐骑技能
	int req_mount_learn_skill(int horse_id, int prop_id);

	// 请求坐骑融合生效时间
	int req_mount_fuse_time(void);

	int sync_mount_ride(int ride, int model_id, int speed);
	// 道具激活坐骑
	int mount_activate_by_item(int horse_id);


	int cmd_mount_timeout(int horse_id, int min);

	int add_horse(const int horse_id, const int train_lv, const Time_Value deadline, const Time_Value activate_time,
			const int cur_attr_1, const int cur_attr_2, const int cur_attr_3, const int cur_attr_4, const int cur_attr_5,
			const int max_attr_1, const int max_attr_2, const int max_attr_3, const int max_attr_4, const int max_attr_5,
			const int next_update_num);

private:
	// 坐骑生效
	int valid_horse(int horse_id, const Time_Value deadline = Time_Value::zero);
	// 坐骑失效
	int invalid_horse(int horse_id);
	int add_mount_horse_prop(int id, bool b_init = false);
	int del_mount_horse_prop(int id);
	int add_mount_group_prop(int group_id, bool b_init = false);
	int del_mount_group_prop(int group_id);
	// 坐骑上阵技能
	int add_deploy_skill(int horse_id, bool b_init = false);
	int remove_deploy_skill(int horse_id);

	void get_mount_attr(std::vector<int> &base_attr, std::vector<int> &train_attr);
	void push_horse_attr(std::vector<Int_Int>& attr_list, Int_Int& attr_value);
	void get_valid_group(const int horse_id, const int train_lv, std::vector<int>& groups);
	void get_invalid_group(int horse_id, std::vector<int>& groups);
	void cal_train_attr_value(void);

	//
	void hook_mount_star_upgrade(const int horse_id, const int star_level);

	// 获取坐骑模型
	int get_mount_model_id(const int horse_id, const int train_lv);
	// 驯养升级广播
	int announce_train_up(const int ann_id, std::string str1 = "", std::string str2 = "", std::string str3 = "");
	// 更新坐骑组合
	bool update_horse_group(const int horse_id, const int train_lv);
	//
	void horse_group_first_check(void);
	// 坐骑是否达到相应星级
	bool is_horse_star_lv_enough(const int horse_id, const int star_lv);
protected:
	Mount_Detail* mount_detail_;
};


inline Mount_Detail const &Logic_Mount::mount_detail(void) const{
	return *mount_detail_;
}



#endif /* LOGIC_MOUNT_H_ */
