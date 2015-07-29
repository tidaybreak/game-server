/*
 * Mount_Config.h
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#ifndef MOUNT_CONFIG_H_
#define MOUNT_CONFIG_H_

#include "Game_Header.h"
#include "Base_Struct.h"
#include "status/Status_Struct.h"

struct Status_Effect_Argv;

struct Horse_Base_Config{
	int horse_id;
	std::string horse_name;
	int quality;
	std::vector<Int_Int> mod_id;

	int speed;
	int vip_own;
	int time;
	int card_id;
	Int_Int num;	//激活缩写的碎片
	std::vector<Int_Int> prop_vec;
	int race;
	int attr_1;
	int attr_2;
	int attr_3;
	int attr_4;
	int attr_5;
	int attr_qlty1;
	int attr_qlty2;
	int attr_qlty3;
	int attr_qlty4;
	int attr_qlty5;
	int initial_lv;		// 初始星级

	Horse_Base_Config(void){
		reset();
	}
	void reset(void){
		horse_id = 0;
		horse_name = "";
		quality = 0;
		mod_id.clear();
		speed = 0;
		vip_own = 0;
		time = 0;
		card_id = 0;
		num.reset();
		prop_vec.clear();
		race = 0;
		attr_1 = 0;
		attr_2 = 0;
		attr_3 = 0;
		attr_4 = 0;
		attr_5 = 0;
		attr_qlty1 = 0;
		attr_qlty2 = 0;
		attr_qlty3 = 0;
		attr_qlty4 = 0;
		attr_qlty5 = 0;
		initial_lv = 0;
	}
	// 获取坐骑模型
	int get_mod_id(const int train_lv);
};
typedef boost::unordered_map<int, Horse_Base_Config> Horse_Base_Config_Map;

struct Horse_Group_Config{
	int group_id;
	std::vector<Int_Int> group;
	int skill;

	Horse_Group_Config(void);
	void reset(void);
};
typedef boost::unordered_map<int, Horse_Group_Config> Horse_Group_Config_Map;

struct Horse_Train_Config{
	int star_lv;
	int skill_open;
	int skill_lv;
	Int_Int consume;
	Int_Int up_consume;
	int cast_money;
	int success_train_base;
	int success_train_once;
	int luck_max;
	int luck_once;
	int upgrade_num;
	int qlty_coe;
	int broadcast;

	Horse_Train_Config(void){
		reset();
	}
	void reset(void){
		star_lv = 0;
		skill_open = 0;
		skill_lv = 0;
		consume.reset();
		up_consume.reset();
		cast_money = 0;
		success_train_base = 0;
		success_train_once = 0;
		luck_max = 0;
		luck_once = 0;
		upgrade_num = 0;
		qlty_coe = 0;
		broadcast = 0;
	}
};
typedef boost::unordered_map<int, Horse_Train_Config> Horse_Train_Config_Map;


struct Horse_Quality_Config{
	int quality;
	int high_star;
	int skill_num;
	int get_skill;
	Horse_Quality_Config(void){
		reset();
	}
	void reset(void){
		quality = 0;
		high_star = 0;
		skill_num = 0;
		get_skill = 0;
	}
};
typedef boost::unordered_map<int, Horse_Quality_Config> Horse_Quality_Config_Map;

struct Horse_Skill_Config{
	int race;
	std::vector<int> items;

	Horse_Skill_Config(void){
		reset();
	}
	void reset(void){
		race = 0;
		items.clear();
	}
};
typedef boost::unordered_map<int, Horse_Skill_Config> Horse_Skill_Config_Map;


struct Horse_Squad_Config{
	int squad_num;
	int lv;

	Horse_Squad_Config(void){
		reset();
	}
	void reset(void){
		squad_num = 0;
		lv = 0;
	}
};
typedef boost::unordered_map<int, Horse_Squad_Config> Horse_Squad_Config_Map;

struct Horse_Fuse_Config{
	Horse_Fuse_Config(void){
		reset();
	}
	~Horse_Fuse_Config(void){

	}
	void reset(void){
		horse_id = 0;
		condition.clear();
		consume.clear();
		time_ture = 0;
	}

	int horse_id;
	Int_Int_Vec condition;
	Int_Int_Vec consume;
	int time_ture;
};
typedef boost::unordered_map<int, Horse_Fuse_Config> Horse_Fuse_Config_Map;


#endif /* MOUNT_CONFIG_H_ */
