/*
 * Config_Cache_Mount.h
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#ifndef CONFIG_CACHE_MOUNT_H_
#define CONFIG_CACHE_MOUNT_H_

#include "Cache_Base.h"
#include "mount/Mount_Config.h"
#include "mount/Mount_Struct.h"


class Config_Cache_Mount : public Cache_Base {
public:
	void refresh_config_cache(void);
public:
	Horse_Base_Config* get_horse_base_config(int id);
	Horse_Group_Config* get_horse_group_config(int id);
	Horse_Train_Config* get_horse_train_config(const int train_star, const int train_lv);
	Horse_Base_Config_Map* horse_base_config_map(void);
	Horse_Fuse_Config* get_horse_fuse_config(const int id);

	void find_activate_group(const std::vector<Int_Int>& horse_list, std::vector<int>& group_list, const std::vector<Horse_Group_Info> group_info);
	int find_horse_id(const int card_id);
	// 培养消耗
	bool get_horse_train_cost(
			const int train_star, const int train_lv, int &prop_id, int &prop_count, int &money,
			int &base_ratio, int &add_ratio, int &add_luck, int &max_luck, int &update_num, int &qlty_coe);
	bool get_horse_train_up_cost(const int train_star, int& prop_id, int &prop_count);
	int get_horse_train_skill_ratio(const int quality);
	// 获取最大培养等级（星级）
	int get_max_train_lv(int quality);
	// 获取升星的培养次数
	void get_star_lv_update_num(const int star_lv, int& num, int& qtyl_coe);
	// 获取升星需要培养的次数
	int get_next_update_train_num(const int train_star);
	// 获取最大上阵技能个数
	int get_deploy_max_skill_num(const int quality);
	void get_deploy_skill_info(const int train_star, const int train_lv,int &open_skill_num, int &max_skill_lv);
	// 获取上阵技能
	int get_random_deploy_skill(const int race, const int max_skill_lv);
	// 是不对应种族技能
	bool is_race_deploy_skill(const int race, const int prop_id);
	void cal_train_attr(const int horse_id, const int train_lv, const int train_num, const int max_train_lv,
			 int &next_update_num, int &total_train_num,int &next_lv_total_train_num,
			int &cur_attr_1, int &cur_attr_2, int &cur_attr_3, int &cur_attr_4, int &cur_attr_5,
			int &max_attr_1, int &max_attr_2, int &max_attr_3, int &max_attr_4, int &max_attr_5);
	void cal_first_train_lv_max_attr(const int horse_id, int &train_star, int &max_attr1, int &max_attr2,
			int &max_attr3, int &max_attr4, int &max_attr5, int &next_update_num);
	int get_train_fail_luck_value(const int train_star, const int train_lv);
	// php通知坐骑融合生效时间
	void php_notice_fuse_time(const int horse_id, const int64_t begin_time, const int64_t end_time);
	// 获取融合生效时间
	void get_fuse_time(std::vector<Int_Int_Int>& times);
	// 融合生效时间检测
	bool check_fuse_time(const int horse_id);
private:
	void refresh_horse_base(void);
	void refresh_horse_group(void);
	void refresh_horse_train(void);
	void refresh_horse_quality(void);
	void refresh_horse_skill(void);
	void refresh_horse_squad(void);
	void refresh_horse_fuse(void);
	void check_config_valid(void);
	int get_train_index(const int train_star, const int train_lv);
private:
	bool check_group_activate(const std::vector<Int_Int>& horse_list, const std::vector<Int_Int>& group_horse_list);
	bool is_in_group_info(const int group_id, const std::vector<Horse_Group_Info> group_info);
private:
	Horse_Base_Config_Map horse_base_config_map_;
	Horse_Group_Config_Map horse_group_config_map_;
	Int_Int_Map card_id_horse_id_map_;
	Horse_Train_Config_Map horse_train_map_;
	Horse_Quality_Config_Map horse_quality_map_;
	Horse_Skill_Config_Map horse_skill_map_;
	Horse_Squad_Config_Map horse_squad_map_;
	Horse_Fuse_Config_Map horse_fuse_map_;
	boost::unordered_map<int, Int_Int> horse_php_fuse_time_map_;
};
typedef Singleton<Config_Cache_Mount> Config_Cache_Mount_Single;
#define CONFIG_CACHE_MOUNT (Config_Cache_Mount_Single::instance())



#endif /* CONFIG_CACHE_MOUNT_H_ */
