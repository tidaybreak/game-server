/*
 * Status_Config.h
 *
 *  Created on: 2014年5月5日
 *      Author: Linqiyou
 */

#ifndef STATUS_CONFIG_H_
#define STATUS_CONFIG_H_

#include "Game_Typedef.h"
#include "status/Status_Struct.h"

struct Attribute_Number {
	Attribute_Number(void) {
		reset();
	}
	void reset(void) {
		attribute_number_id = 0;
		attr_id = 0;
		add = 0;
		percent = 0;
		cover = 0;
	}
	int attribute_number_id;
	int attr_id;
	bool add;
	bool percent;
	bool cover;
};
typedef boost::unordered_map<int, Attribute_Number> Attribute_Number_Map;

struct Btl_Buff_Effect {
	int buff_effect_id;			//效果ID
	int buff_effect_type;		//效果类型 Status_Def::STATUS_ID
	int buff_hit;				//是否状态命中
	Int_Vec buff_delay;				//延时生效
	int buff_direct;			//BUFF指向方向

	/* beat_type_vec 一个是从vec处理成set，方便查询(count)
	 * 一个是分别取出时间类型、出手次数类型的心跳间隔，用于程序直接赋值
	 * */
	Int_Vec beat_type_vec;		//心跳类型 ,用于对应buff_hbeat_dur
	Int_Vec beat_gap_vec;		//心跳间隔
	Int_Hash_Set beat_type_set; //处理:心跳类型 用于查询
	int time_gap;				//处理:时间类型的心跳间隔
	int action_gap;				//处理:出手类型的心跳间隔
	int passive_gap;			//处理:受击类型的心跳间隔
	int delay_time_gap;			//处理:延迟的时间类型的心跳间隔
	int delay_action_gap;		//处理:延迟的出手类型的心跳间隔
	int delay_passive_gap;		//处理:延迟的受击类型的心跳间隔

	bool buff_overlay;			//是否可叠加
	int buff_trigger_type;		//触发类别
	int buff_trigger_skill;	//触发技能
	double buff_chance;			//触发概率 千分制
	Status_Effect_Argv trigger_buff;//触发BUFF
	int val1;
	int val2;
	int val3;
	int val4;
	int val5;
	Btl_Buff_Effect(void) { reset(); }
	void reset(void);
};
typedef boost::unordered_map<int, Btl_Buff_Effect> Buff_Effect_Map;
typedef std::vector<Btl_Buff_Effect> Buff_Effect_Vec;

struct Btl_Buff_Cfg {
	int buff_id;
	int buff_charactor;				//类型 良性(1) 中性(2) 恶性(3)
	Int_Vec buff_disappear_vec;	//消失类型
	Int_Hash_Set buff_disappear;	//消失类型
	Int_Hash_Set buff_spdisappear;	//特殊消失
	int overlay;					//叠加数
	bool is_dispel;					//能否驱散
	bool is_pure;					//能否净化
	bool is_stoic;					//是否受霸体影响
	Buff_Effect_Vec effect_map;		//效果ID
	bool is_fight_buff;				// 是否战斗内buff
	Int_Vec buff_scene;		// 指定场景有效;未填不限定
	Int_Vec scene_visible;

	bool is_effect_un_overlay;	//是否effect里存在不叠加效果

	Btl_Buff_Cfg(void) { reset(); }
	void reset(void);
};
typedef boost::unordered_map<int, Btl_Buff_Cfg> Btl_Buff_Map;

#endif /* STATUS_CONFIG_H_ */
