/*
 * BattleAIManager.h
 *
 *  Created on: 2014年4月17日
 *      Author: lys
 */

#ifndef BATTLE_AI_MANAGER_H_
#define BATTLE_AI_MANAGER_H_

#include "NPC/NPC_Struct.h"
#include "Object_Pool.h"
#include "fighter/Fighter_Struct.h"

class Fighter;

class AI_Object{

public:
	AI_Object(void){}
	virtual ~AI_Object(void){}
public:
	void reset(void){};
	virtual void fight_tick(const Time_Value &now){}
	virtual int init(Fighter* fighter){return 0;}
	virtual int ai_select_skill(void){return 0;}

	virtual void start_fight(Fighter* fighter){}
	virtual void fight_round(Fighter* fighter){}
	virtual void use_skill(Fighter* fighter, int skill_id){}
	virtual void break_use_skill(Fighter* fighter, int skill_id){}
	virtual void hp_change(Fighter* fighter){}
	virtual void mp_change(Fighter* fighter){}
	virtual void status_overlay(Fighter* fighter, const int status_id, const int num){}
	virtual void die(Fighter* fighter){}
	virtual void play_plot(std::string plot_id){}
	virtual void play_talk(int talk_id){}
	virtual void play_ai(int ai_id){}
};

struct Skill_AI_Com{
	bool  valid_;
	std::vector<bool> result_vec;
	void reset(void){
		valid_ = false;
		config_.reset();
	}
	Skill_AI_Com(void){
		reset();
	}

	bool is_valid(void){
		return valid_;
	}
	int priority(void);
	void set_skill_ai_conifg(Skill_AI_Config* config);

	void hp_change(bool own, int percent);
	void mp_change(bool own, int percent);
	void status_overlay(bool own, const int status_id, const int num);
private:
	Skill_AI_Config config_;
};

struct Skill_AI_Info{
	int skill_id;
	int pri;
	std::vector<Skill_AI_Com> skill_com_vec;
	void reset(void){
		skill_id = 0;
		pri = 0;
	}
	Skill_AI_Info(void){
		reset();
	}
	int priority(void);
	void hp_change(bool own, int percent);
	void mp_change(bool own, int percent);
	void status_overlay(bool own, const int status_id, const int num);
};

class Player_AI_Object:public AI_Object{

public:
	Player_AI_Object(void);
	virtual ~Player_AI_Object(void);
public:
	void reset(void);
	virtual void fight_tick(const Time_Value &now);
	virtual int init(Fighter* fighter);
	virtual int ai_select_skill(void);

	virtual void start_fight(Fighter* fighter);
	virtual void fight_round(Fighter* fighter);
	virtual void use_skill(Fighter* fighter, int skill_id);
	virtual void hp_change(Fighter* fighter);
	virtual void mp_change(Fighter* fighter);
	virtual void status_overlay(Fighter* fighter, const int status_id, const int num);
	virtual void die(Fighter* fighter);
	virtual void play_plot(std::string plot_id);
	virtual void play_talk(int talk_id);

private:
	Fighter* owner_;
	std::vector<Skill_AI_Info> skill_ai_vec;
};


class Hero_AI_Object:public AI_Object{

public:
	Hero_AI_Object(void);
	virtual ~Hero_AI_Object(void);
public:
	void reset(void);
	virtual void fight_tick(const Time_Value &now);
	virtual int init(Fighter* fighter);
	virtual int ai_select_skill(void);

	virtual void start_fight(Fighter* fighter);
	virtual void fight_round(Fighter* fighter);
	virtual void use_skill(Fighter* fighter, int skill_id);
	virtual void hp_change(Fighter* fighter);
	virtual void mp_change(Fighter* fighter);
	virtual void status_overlay(Fighter* fighter, const int status_id, const int num);
	virtual void die(Fighter* fighter);
	virtual void play_plot(std::string plot_id);
	virtual void play_talk(int talk_id);

private:
	Fighter* owner_;
	std::vector<Skill_AI_Info> skill_ai_vec;
};

class Battle_AI_Object:public AI_Object{
public:
	typedef boost::unordered_map<int, Monster_AI_Info> AI_MAP;
	typedef std::vector<Monster_AI_Info*> AI_VEC;
public:
	Battle_AI_Object(void);
	virtual ~Battle_AI_Object(void);
public:
	void reset(void);
	virtual void fight_tick(const Time_Value &now);
	virtual int init(Fighter* fighter);
	virtual int ai_select_skill(void);

	virtual void start_fight(Fighter* fighter);
	virtual void fight_round(Fighter* fighter);
	virtual void use_skill(Fighter* fighter, int skill_id);
	virtual void break_use_skill(Fighter* fighter, int skill_id);
	virtual void hp_change(Fighter* fighter);
	virtual void mp_change(Fighter* fighter);
	virtual void die(Fighter* fighter);
	virtual void play_plot(std::string plot_id);
	virtual void play_talk(int talk_id);
	virtual void play_ai(int ai_id);
private:
	Monster_AI_Info* insert_ai_info(int id, Monster_AI_Info& ai_info);
	bool ai_is_valid(Monster_AI_Info* ai_info, bool type_spe = false);
	int get_random_index(std::vector<int>& rate_vec);
	int do_special_thing(Monster_AI_Info* ai_info);
	// 怪物ai预告信息
	void handle_ai_foreshow(void);
private:
	Fighter* owner_;

	AI_MAP running_ai_data_;
	AI_VEC loop_skill_ai_;
	AI_VEC one_skill_ai_;
	AI_VEC special_ai_;

	std::vector<int> base_skill_vec_;
	int ai_id_;
	int skill_index_;
	std::vector<int> skill_vec_;
	std::vector<int> skill_rate_vec_;
	int high_skill_;

	int fight_round_;
	Time_Value begin_time_;

	int break_use_skill_id_;		// 中断技能后使用的技能
	bool break_use_skill_; 			// 中断技能

};

class Battle_AI_Manager{
public:
	typedef Object_Pool_Ex<Battle_AI_Object, Thread_Mutex> Battle_AI_Obj_Pool;
	typedef Object_Pool_Ex<Player_AI_Object, Thread_Mutex> Player_AI_Obj_Pool;
	typedef Object_Pool_Ex<Hero_AI_Object, Thread_Mutex> Hero_AI_Obj_Pool;
	typedef boost::unordered_map<Fighter*, AI_Object*> AI_OBJ_MAP;
public:
	Battle_AI_Manager();
	virtual ~Battle_AI_Manager();
public:
	void join(Fighter* fighter);
	void leave(Fighter* fighter);
	void fight_tick(const Time_Value &now);
	int ai_select_skill(Fighter* fighter);
public:
	void start_fight(Fighter* fighter);
	void fight_round(Fighter* fighter);
	void use_skill(Fighter* fighter, int skill_id);
	void break_use_skill(Fighter* fighter, int skill_id);
	void hp_change(Fighter* fighter);
	void mp_change(Fighter* fighter);
	void status_overlay(Fighter* fighter, const int status_id, const int num);
	void die(Fighter* fighter);
	void play_plot(std::string plot_id);
	void play_talk(int talk_id);
private:
	AI_Object* create_ai_object(Fighter* fighter);
	int relase_ai_object(Fighter* fighter);
	AI_Object* get_ai_obj(Fighter* fighter);
private:
	AI_OBJ_MAP ai_object_map_;
	Battle_AI_Obj_Pool battle_ai_obj_pool_;
	Player_AI_Obj_Pool palyer_ai_obj_pool_;
	Hero_AI_Obj_Pool hero_ai_obj_pool_;
};



#endif /* BATTLE_AI_MANAGER_H_ */
