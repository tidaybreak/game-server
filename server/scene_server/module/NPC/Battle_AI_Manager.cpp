/*
 * Battle_AI_Manager.cpp
 *
 *  Created on: 2014年4月17日
 *      Author: lys
 */
#include "Battle_AI_Manager.h"
#include "NPC/monster/Monster.h"
#include "Config_Cache_NPC.h"
#include "Battle_Scene.h"
#include "Config_Cache_Fighter.h"
#include "NPC/hero/Hero.h"
#include "status/Status_Struct.h"
#include "status/Status.h"


bool Greater(Monster_AI_Info* info1, Monster_AI_Info* info2){
	return info1->config->pri > info2->config->pri;
}

bool Pri_Greater(Skill_AI_Info info1, Skill_AI_Info info2){
	return info1.priority() > info2.priority();
}
//////////////////////////////////////////////////////////////////
int Skill_AI_Com::priority(void){
	return config_.priority_add;
}

void Skill_AI_Com::set_skill_ai_conifg(Skill_AI_Config* config){
	config_ = *config;
	int count = config_.strike_vec.size();
	for(int i=0; i<count; ++i){
		result_vec.push_back(false);
	}
}
//1) 己方存在生命值低于%*1000
//2) 敌方有人指定BUFF层数大于等于..
//3) 敌方有人生命值低于%,*1000
//4) 己方怒气大于等于..%*1000
//5)己方有人指定buFF层数大于等于
void Skill_AI_Com::hp_change(bool own, int percent){
	int count = config_.strike_vec.size();
	for(int i =0; i<count; ++i){
		if(config_.strike_vec[i].id == 1 && own){
			if( percent < config_.strike_vec[i].val2){
				result_vec[i] = true;
			}else{
				result_vec[i]  = false;
			}
		}else if(config_.strike_vec[i].id == 3 && !own){
			if(percent < config_.strike_vec[i].val2){
				result_vec[i] = true;
			}else{
				result_vec[i]  = false;
			}
		}
	}
	bool valid = false;
	for(int i=0; i<count; ++i){
		if(result_vec[i]){
			valid = true;
			break;
		}
	}
	this->valid_ = valid;
}

void Skill_AI_Com::mp_change(bool own, int percent){
	int count = config_.strike_vec.size();
	for(int i =0; i<count; ++i){
		if(config_.strike_vec[i].id == 1 && own){
			if( percent >= config_.strike_vec[i].val2){
				result_vec[i] = true;
			}else{
				result_vec[i]  = false;
			}
		}
	}
	bool valid = false;
	for(int i=0; i<count; ++i){
		if(result_vec[i]){
			valid = true;
			break;
		}
	}
	this->valid_ = valid;
}

void Skill_AI_Com::status_overlay(bool own, const int status_id, const int num){
	int count = config_.strike_vec.size();
	for(int i =0; i<count; ++i){
		if(config_.strike_vec[i].id == status_id && own){
			if( num >= config_.strike_vec[i].val2){
				result_vec[i] = true;
			}else{
				result_vec[i]  = false;
			}
		}
	}
	bool valid = false;
	for(int i=0; i<count; ++i){
		if(result_vec[i]){
			valid = true;
			break;
		}
	}
	this->valid_ = valid;
}

////////////////////////////////////////////////////////////////////////////////////////
int Skill_AI_Info::priority(void){
	int total = pri;
	std::vector<Skill_AI_Com>::iterator it;
	for(it = skill_com_vec.begin(); it != skill_com_vec.end(); ++it){
		if((*it).is_valid()){
			total += (*it).priority();
		}
	}
	return total;
}
void Skill_AI_Info::hp_change(bool own, int percent){
	std::vector<Skill_AI_Com>::iterator it;
	for(it = skill_com_vec.begin(); it != skill_com_vec.end(); ++it){
		(*it).hp_change(own, percent);
	}
}

void Skill_AI_Info::mp_change(bool own, int percent){
	std::vector<Skill_AI_Com>::iterator it;
	for(it = skill_com_vec.begin(); it != skill_com_vec.end(); ++it){
		(*it).mp_change(own, percent);
	}
}

void Skill_AI_Info::status_overlay(bool own, const int status_id, const int num){
	std::vector<Skill_AI_Com>::iterator it;
	for(it = skill_com_vec.begin(); it != skill_com_vec.end(); ++it){
		(*it).status_overlay(own, status_id, num);
	}
}

//////////////////////////////////////////////////////////////////

Player_AI_Object::Player_AI_Object(void):owner_(0){

}

Player_AI_Object::~Player_AI_Object(void){

}

void Player_AI_Object::reset(void){
	owner_ = NULL;
	skill_ai_vec.clear();
}

void Player_AI_Object::fight_tick(const Time_Value &now){

}

int Player_AI_Object::init(Fighter* fighter){
	if(fighter == NULL){
		return -1;
	}
	this->owner_ = fighter;
	//get all learn skill
	const Fighter_Detail& fighter_detail = fighter->fighter_detail();
	Talent_Map::const_iterator it = fighter_detail.talent_map.find(fighter_detail.cur_used_talent_);
	if (it == fighter_detail.talent_map.end()) {
		return 0;
	}
//	Talent_Page_Map::const_iterator it = fighter_detail.talent_page_map.find(fighter_detail.cur_used_talent_page);
//	if (it == fighter_detail.talent_page_map.end()) {
//		return 0;
//	}
	Skill_DB_Info_Map::const_iterator sit;
	for(sit= it->second.skill_db_info_map.begin(); sit != it->second.skill_db_info_map.end();++sit){
		const Skill_DB_Info& info = sit->second;
		Skill_AI_Info ai_info;
		ai_info.skill_id = info.skill_id;
		const Skill_Config* skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(info.skill_id);
		if(skill_config){
			ai_info.pri = skill_config->skill_priority;
			std::vector<Skill_AI_Config*> ai_config_vec;
			CONFIG_CACHE_FIGHTER->get_skill_ai_config(info.skill_id, ai_config_vec);
			int ai_c_count = ai_config_vec.size();
			for(int i=0; i<ai_c_count; ++i){
				Skill_AI_Com ai_com;
				ai_com.set_skill_ai_conifg(ai_config_vec[i]);
				ai_info.skill_com_vec.push_back(ai_com);
			}
			this->skill_ai_vec.push_back(ai_info);
		}
	}
	return 0;
}

int Player_AI_Object::ai_select_skill(void){
	if(owner_ == NULL){
		return -1;
	}
	if(this->skill_ai_vec.empty()){
		return 0;
	}
	std::sort(skill_ai_vec.begin(), skill_ai_vec.end(), Pri_Greater);
	int count = skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		if(owner_->can_use_skill(skill_ai_vec[i].skill_id) == 0){
			return skill_ai_vec[i].skill_id;
		}
	}
	return 0;
}

void Player_AI_Object::start_fight(Fighter* fighter){
	if(fighter == NULL){
		return;
	}
	if(fighter != owner_){
		return;
	}

	int skill_id = this->ai_select_skill();
	fighter->set_first_used_skill(skill_id);
	return;
}

void Player_AI_Object::fight_round(Fighter* fighter){

}

void Player_AI_Object::use_skill(Fighter* fighter, int skill_id){
	return;
}

void Player_AI_Object::hp_change(Fighter* fighter){
	if(this->owner_ == NULL || fighter == NULL){
		return;
	}
	bool own = false;
	if(this->owner_->battle_scene() && fighter->battle_scene()){
		if(this->owner_->battle_position().pos == fighter->battle_position().pos){
			own = true;
		}
	}
	int percent = fighter->current_blood_rate()*1000;
	int count = this->skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		skill_ai_vec[i].hp_change(own, percent);
	}
	return;
}

void Player_AI_Object::mp_change(Fighter* fighter){
	if(this->owner_ == NULL || fighter == NULL){
		return;
	}
	bool own = false;
	if(this->owner_->battle_scene() && fighter->battle_scene()){
		if(this->owner_->battle_position().pos == fighter->battle_position().pos){
			own = true;
		}
	}
	int percent = fighter->current_mp_rate()*1000;
	int count = this->skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		skill_ai_vec[i].mp_change(own, percent);
	}
	return;
}

void Player_AI_Object::status_overlay(Fighter* fighter, const int status_id, const int num){
	if(this->owner_ == NULL || fighter == NULL){
		return;
	}
	bool own = false;
	if(this->owner_->battle_scene() && fighter->battle_scene()){
		if(this->owner_->battle_position().pos == fighter->battle_position().pos){
			own = true;
		}
	}
	int count = this->skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		skill_ai_vec[i].status_overlay(own, status_id, num);
	}
	return;
}

void Player_AI_Object::die(Fighter* fighter){
	return;
}

void Player_AI_Object::play_plot(std::string plot_id){
	return;
}

void Player_AI_Object::play_talk(int talk_id){
	return;
}

/////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////

Hero_AI_Object::Hero_AI_Object(void):owner_(0){

}

Hero_AI_Object::~Hero_AI_Object(void){

}

void Hero_AI_Object::reset(void){
	owner_ = NULL;
	skill_ai_vec.clear();
}

void Hero_AI_Object::fight_tick(const Time_Value &now){

}

int Hero_AI_Object::init(Fighter* fighter){
	if(fighter == NULL){
		return -1;
	}
	this->owner_ = fighter;
	//get all learn skill
	const Fighter_Detail& fighter_detail = fighter->fighter_detail();
	Talent_Map::const_iterator it = fighter_detail.talent_map.find(fighter_detail.cur_used_talent_);
	if (it == fighter_detail.talent_map.end()) {
		return 0;
	}
//	Talent_Page_Map::const_iterator it = fighter_detail.talent_page_map.find(fighter_detail.cur_used_talent_page);
//	if (it == fighter_detail.talent_page_map.end()) {
//		return 0;
//	}
	Skill_DB_Info_Map::const_iterator sit;
	for(sit= it->second.skill_db_info_map.begin(); sit != it->second.skill_db_info_map.end();++sit){
		const Skill_DB_Info& info = sit->second;
		Skill_AI_Info ai_info;
		ai_info.skill_id = info.skill_id;
		const Skill_Config* skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(info.skill_id);
		if(skill_config){
			ai_info.pri = skill_config->skill_priority;
			std::vector<Skill_AI_Config*> ai_config_vec;
			CONFIG_CACHE_FIGHTER->get_skill_ai_config(info.skill_id, ai_config_vec);
			int ai_c_count = ai_config_vec.size();
			for(int i=0; i<ai_c_count; ++i){
				Skill_AI_Com ai_com;
				ai_com.set_skill_ai_conifg(ai_config_vec[i]);
				ai_info.skill_com_vec.push_back(ai_com);
			}
			this->skill_ai_vec.push_back(ai_info);
		}
	}
	return 0;
}

int Hero_AI_Object::ai_select_skill(void){
	if(owner_ == NULL){
		return -1;
	}
	if(this->skill_ai_vec.empty()){
		return 0;
	}
	std::sort(skill_ai_vec.begin(), skill_ai_vec.end(), Pri_Greater);
	int count = skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		if(owner_->can_use_skill(skill_ai_vec[i].skill_id) == 0){
			return skill_ai_vec[i].skill_id;
		}
	}
	return 0;
}

void Hero_AI_Object::start_fight(Fighter* fighter){
	if(fighter == NULL){
		return;
	}
	if(fighter != owner_){
		return;
	}

	int skill_id = this->ai_select_skill();
	fighter->set_first_used_skill(skill_id);
	return;
}

void Hero_AI_Object::fight_round(Fighter* fighter){

}

void Hero_AI_Object::use_skill(Fighter* fighter, int skill_id){
	return;
}

void Hero_AI_Object::hp_change(Fighter* fighter){
	if(this->owner_ == NULL || fighter == NULL){
		return;
	}
	bool own = false;
	if(this->owner_->battle_scene() && fighter->battle_scene()){
		if(this->owner_->battle_position().pos == fighter->battle_position().pos){
			own = true;
		}
	}
	int percent = fighter->current_blood_rate()*1000;
	int count = this->skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		skill_ai_vec[i].hp_change(own, percent);
	}
	return;
}

void Hero_AI_Object::mp_change(Fighter* fighter){
	if(this->owner_ == NULL || fighter == NULL){
		return;
	}
	if(this->owner_->hero_self() == NULL){
		return;
	}
	bool own = false;
	role_id_t master_id = this->owner_->hero_self()->master_role_id();
	if(master_id == fighter->role_id()){
		own = true;
	}
	int percent = fighter->current_mp_rate()*1000;
	int count = this->skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		skill_ai_vec[i].mp_change(own, percent);
	}
	return;
}

void Hero_AI_Object::status_overlay(Fighter* fighter, const int status_id, const int num){
	if(this->owner_ == NULL || fighter == NULL){
		return;
	}
	bool own = false;
	if(this->owner_->battle_scene() && fighter->battle_scene()){
		if(this->owner_->battle_position().pos == fighter->battle_position().pos){
			own = true;
		}
	}
	int count = this->skill_ai_vec.size();
	for(int i=0; i<count; ++i){
		skill_ai_vec[i].status_overlay(own, status_id, num);
	}
	return;
}

void Hero_AI_Object::die(Fighter* fighter){
	return;
}

void Hero_AI_Object::play_plot(std::string plot_id){
	return;
}

void Hero_AI_Object::play_talk(int talk_id){
	return;
}

/////////////////////////////////////////////////////////////////



Battle_AI_Object::Battle_AI_Object(void):owner_(NULL),
		ai_id_(0), skill_index_(0), high_skill_(0), fight_round_(0),
		break_use_skill_id_(0), break_use_skill_(false){

}

Battle_AI_Object::~Battle_AI_Object(void){

}
int Battle_AI_Object::get_random_index(std::vector<int>& rate_vec){
	int count = rate_vec.size();
	int rand_val = random()%1000;
	int sum_val = 0;
	for(int i=0; i<count; ++i){
		sum_val += rate_vec[i];
		if(sum_val > rand_val){
			return i;
		}
	}
	return -1;
}
Monster_AI_Info* Battle_AI_Object::insert_ai_info(int id, Monster_AI_Info& ai_info){
	std::pair<AI_MAP::iterator, bool> reit;
	reit = running_ai_data_.insert(std::make_pair(id, ai_info));
//	if(reit.second){
//		return &(reit.first->second);
//	}
	AI_MAP::iterator it = running_ai_data_.find(id);
	if(it != running_ai_data_.end()){
		return &(it->second);
	}
	return NULL;
}

bool Battle_AI_Object::ai_is_valid(Monster_AI_Info* ai_info, bool type_spe/* = false*/){
	if(ai_info->can_trigger() == false){
		return false;
	}
	bool spe = false;
	switch(ai_info->config->a_type){
		case MONSTER_AI_A_TALK:
		case MONSTER_AI_A_PLOT:
		case MONSTER_AI_A_CLEAN_MP:
		case MONSTER_AI_A_CLEAN_BUF:
		case MONSTER_AI_A_REFRESH_HERO:
		case MONSTER_AI_A_ADD_BUF:
		case MONSTER_AI_A_KILL_MONSTER:
		case MONSTER_AI_A_TRANSFORM:{
			spe = true;
			break;
		}
		default:{
			break;
		}
	}
	if(type_spe != spe){
		return false;
	}
	if(ai_info->is_valid){
		if(ai_info->config->a_type == MONSTER_AI_A_ONE_SKILL_BB){
			return true;
		}else if(ai_info->config->a_type == MONSTER_AI_A_ONE_SKILL_DL){
			if(Time_Value::gettimeofday() >
			ai_info->valid_time + Time_Value(ai_info->config->a_val3)){
				return true;
			}else{
				return false;
			}
		}else{
			ai_info->is_valid = false;
			return true;
		}
		if(ai_info->config->a_type != MONSTER_AI_A_ONE_SKILL_BB){
			ai_info->is_valid = false;
		}
	}
	bool is_valid = false;
	switch(ai_info->config->c_type){
		case MONSTER_AI_C_NULL:{
			is_valid = true;
			break;
		}
		case MONSTER_AI_C_HP:{
//			double cur_val = owner_->blood_current();
//			double max_val = owner_->blood_max();
//			int cmp_val;
//			if(ai_info->config->c_val2 == 0){
//				cmp_val = cur_val;
//			}else{
//				cmp_val = 1000*(cur_val/max_val);
//			}
//			if(ai_info->config->c_val1 == 0){
//				if(cmp_val >= ai_info->config->c_val3){
//					is_valid = true;
//				}
//			}else{
//				if(cmp_val <= ai_info->config->c_val3){
//					is_valid = true;
//				}
//			}
			break;
		}
		case MONSTER_AI_C_MP:{
//			double cur_val = owner_->mp_current();
//			double max_val = owner_->mp_max();
//			int cmp_val;
//			if(ai_info->config->c_val2 == 0){
//				cmp_val = cur_val;
//			}else{
//				cmp_val = 1000*(cur_val/max_val);
//			}
//			if(ai_info->config->c_val1 == 0){
//				if(cmp_val >= ai_info->config->c_val3){
//					is_valid = true;
//				}
//			}else{
//				if(cmp_val <= ai_info->config->c_val3){
//					is_valid = true;
//				}
//			}
			break;
		}
		case MONSTER_AI_C_DIE:{
			break;
		}
		case MONSTER_AI_C_SKILL:{
			break;
		}
		case MONSTER_AI_C_TIMES:{
			break;
		}
		case MONSTER_AI_C_TIME:{
			int cur_val;
			int cmp_val;
			Time_Value last_time;
			cmp_val = ai_info->config->c_val2;
			if(ai_info->last_time == Time_Value::zero){// 第一次触发
				last_time = Time_Value::gettimeofday() - begin_time_;
			}else{//
				if(ai_info->has_trigger){
					ai_info->has_trigger = false;
					ai_info->last_time = Time_Value::gettimeofday();
					last_time = Time_Value::zero;
				}else{
					last_time = Time_Value::gettimeofday() - ai_info->last_time;
				}
				if(ai_info->config->c_val1 != 0){
					cmp_val = ai_info->config->c_val1;
				}
			}
			cur_val = last_time.sec();
			if(cur_val >= cmp_val){
				is_valid = true;
			}
			break;
		}
		default:{
			break;
		}
	}
	return is_valid;
}

int Battle_AI_Object::do_special_thing(Monster_AI_Info* ai_info){
	switch(ai_info->config->a_type){
		case MONSTER_AI_A_PLOT:{
			//check stop_sing
			if(owner_->battle_scene()){
				std::stringstream ss;
				int tmp_value = ai_info->config->a_val1;
				ss<<tmp_value;
				if (ai_info->config->battle_over == 1) {
					owner_->battle_scene()->set_end_battle_after_plot(true);
				}
				return owner_->battle_scene()->play_battle_plot(ss.str(),
						ai_info->config->a_val2);
			}
			break;
		}
		case MONSTER_AI_A_TALK:{
			//check stop_sing
			if(owner_->battle_scene()){
				return owner_->battle_scene()->play_battle_talk(owner_->role_id(), ai_info->config->a_val1,
						ai_info->config->a_val2);
			}
			break;
		}
		case MONSTER_AI_A_REFRESH_HERO:{
			// refresh hero
			break;
		}
		case MONSTER_AI_A_ADD_BUF:{
			ai_info->config->buf_argv.caster_id = owner_->role_id();
			owner_->insert_status(ai_info->config->buf_argv);
			break;
		}
		case MONSTER_AI_A_KILL_MONSTER:{
			if(owner_->battle_scene()){
				return owner_->battle_scene()->ai_kill_all_monsters(
						owner_->mover_detail().battle_position.pos);
			}
			break;
		}
		case MONSTER_AI_A_TRANSFORM:{
			if(owner_->monster_self()){
				owner_->monster_self()->ai_transform(ai_info->config->a_val1, ai_info->config->a_val1);
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

void Battle_AI_Object::reset(void){
	owner_ = NULL;

	running_ai_data_.clear();
	loop_skill_ai_.clear();
	one_skill_ai_.clear();
	special_ai_.clear();

	base_skill_vec_.clear();
	ai_id_ = 0;
	skill_index_ = 0;
	skill_vec_.clear();
	skill_rate_vec_.clear();
	high_skill_ = 0;

	fight_round_ = 0;
	begin_time_ = Time_Value::zero;
	break_use_skill_id_ = 0;
	break_use_skill_ = false;
}

void Battle_AI_Object::fight_tick(const Time_Value &now){
	//special_ai
	int count = special_ai_.size();
	for(int i=0; i<count; ++i){
		 if(ai_is_valid(special_ai_[i], true)){
			 //check stop_sing
			 if(special_ai_[i]->config->stop_sing){
				 if(owner_->stop_sing() == 0){
					 if(do_special_thing(special_ai_[i]) == 0 &&
							 special_ai_[i]->config->rate > random()%1000){
						 special_ai_[i]->trigger(now);
						 this->play_ai(special_ai_[i]->config->id);
					 }
				 }
			 }else{
				 if(do_special_thing(special_ai_[i]) == 0 &&
						 special_ai_[i]->config->rate > random()%1000){
					 special_ai_[i]->trigger(now);
					 this->play_ai(special_ai_[i]->config->id);
				 }
			 }
		 }
	}
}

int Battle_AI_Object::init(Fighter* fighter){
	if(fighter == NULL){
		return -1;
	}
	this->owner_ = fighter;
	Monster* monster = (Monster*)fighter;
	//base skill
	base_skill_vec_ = monster->monster_detail().config_cache->skill_vec;
	int count = monster->monster_detail().config_cache->battle_ai_vec.size();
	for(int i=0; i<count; ++i){
		int ai_id = monster->monster_detail().config_cache->battle_ai_vec[i];
		const Monster_AI_Config* config = CONFIG_CACHE_NPC->find_monster_ai_config(ai_id);
		if(config){
			Monster_AI_Info tmp_info;
			tmp_info.config = (Monster_AI_Config*)config;
			Monster_AI_Info* ai_info = this->insert_ai_info(tmp_info.config->id, tmp_info);
			if(ai_info){
				//
				int db_skill_count = ai_info->config->skill_vec.size();
				for(int i=0; i<db_skill_count; ++i){
					Skill_DB_Info skill_info;
					skill_info.skill_level = 1;
					skill_info.skill_loc = 1;
					skill_info.skill_id = ai_info->config->skill_vec[i];
					monster->npc_detail().fighter_detail_.talent_map[1].skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
				}

				switch(ai_info->config->a_type){
					case MONSTER_AI_A_LOOP_SKILLS:
					case MONSTER_AI_A_RAN_SKILLS:{
						loop_skill_ai_.push_back(ai_info);
						break;
					}
					case MONSTER_AI_A_ONE_SKILL_DL:
					case MONSTER_AI_A_ONE_SKILL_BB:
					case MONSTER_AI_A_ONE_SKILL:{
						one_skill_ai_.push_back(ai_info);
						break;
					}
					case MONSTER_AI_A_TALK:
					case MONSTER_AI_A_PLOT:
					case MONSTER_AI_A_CLEAN_MP:
					case MONSTER_AI_A_CLEAN_BUF:
					case MONSTER_AI_A_REFRESH_HERO:
					case MONSTER_AI_A_ADD_BUF:
					case MONSTER_AI_A_KILL_MONSTER:
					case MONSTER_AI_A_TRANSFORM:{
						special_ai_.push_back(ai_info);
						break;
					}
				}
			}
		}
	}
	if(loop_skill_ai_.empty() == false){
		std::sort(loop_skill_ai_.begin(), loop_skill_ai_.end(), Greater);
	}
	if(one_skill_ai_.empty() == false){
		std::sort(one_skill_ai_.begin(), one_skill_ai_.end(), Greater);
	}
	if(special_ai_.empty() == false){
		std::sort(special_ai_.begin(), special_ai_.end(), Greater);
	}
	return 0;
}

int Battle_AI_Object::ai_select_skill(void){

	handle_ai_foreshow();
	if(break_use_skill_){
		if(break_use_skill_id_ != 0){
			if(owner_->can_use_skill(break_use_skill_id_) == 0){
				return break_use_skill_id_;
			}else{
				//break_use_skill_id_ = 0;
			}
		}
		//break_use_skill_ = false;
	}

	//find valid loop_skill
	Monster_AI_Info* loop_ai_info = NULL;
	bool ran_skill = false;
	int pre_ai_id = ai_id_;
	int pre_skill_index = skill_index_;
	int loop_pri = 0;
	skill_vec_.clear();
	skill_rate_vec_.clear();
	ai_id_ = 0;

	int l_count = loop_skill_ai_.size();
	for(int i=0; i<l_count; ++i){
		if(ai_is_valid(loop_skill_ai_[i]) &&
				loop_skill_ai_[i]->config->rate > random()%1000){
			skill_vec_ = loop_skill_ai_[i]->config->skill_vec;
			skill_rate_vec_ = loop_skill_ai_[i]->config->skill_rate_vec;
			ai_id_ = loop_skill_ai_[i]->config->id;
			loop_ai_info = loop_skill_ai_[i];
			loop_pri = loop_skill_ai_[i]->config->pri;
			if(ai_id_ == pre_ai_id){
				skill_index_ = pre_skill_index;
			}
			if(loop_skill_ai_[i]->config->a_type == MONSTER_AI_A_RAN_SKILLS){
				ran_skill = true;
			}
			break;
		}
	}

	//select one_skill
	Time_Value now = Time_Value::gettimeofday();
	int o_count = one_skill_ai_.size();
	for(int i=0; i<o_count; ++i){
		int one_pri = one_skill_ai_[i]->config->pri;
		if( (one_pri >= loop_pri) && ai_is_valid(one_skill_ai_[i]) &&
				one_skill_ai_[i]->config->rate > random()%1000){
			int skill = one_skill_ai_[i]->config->a_val1;
			if(owner_->can_use_skill(skill) == 0){
				one_skill_ai_[i]->trigger(now);
				//
				this->play_ai(one_skill_ai_[i]->config->id);

				if(one_skill_ai_[i]->config->a_val2 == 1){
					this->break_use_skill_id_ = skill;
					one_skill_ai_[i]->is_valid = false;
				}
				return skill;
			}
		}
	}

	//select skill in loop_skill
	if(ai_id_ != 0 && skill_vec_.empty() == false){
		int skill_index;
		int count = skill_vec_.size();
		if(ran_skill){
			skill_index = this->get_random_index(skill_rate_vec_);
		}else{
			skill_index = skill_index_% count;
		}
		if(skill_index >= 0 && skill_index < count){
			int skill_id = skill_vec_[skill_index];
			if(owner_->can_use_skill(skill_id) == 0){
				if(loop_ai_info){
					loop_ai_info->trigger(now);
				}
				if(ran_skill == false){
					skill_index_ += 1;
					int count = skill_vec_.size();
					if(skill_index_ >= count){
						skill_index_ = 0;
					}
				}
				return skill_id;
			}
		}
		return 0;
	}

	//select skill in base_skill
	int count = base_skill_vec_.size();
	if(count > 0){
		int skill_index = random()%count;
		int skill_id = base_skill_vec_[skill_index];
		if(owner_->can_use_skill(skill_id) == 0){
			return skill_id;
		}
	}
	return 0;
}

void Battle_AI_Object::start_fight(Fighter* fighter){
	if(fighter == NULL){
		return;
	}
	// own start fight
	if(fighter != owner_){
		return;
	}
	begin_time_ = Time_Value::gettimeofday();
	fight_round_ = 0;
	fight_round(fighter);
	int skill_id = this->ai_select_skill();
	fighter->set_first_used_skill(skill_id);

}


void Battle_AI_Object::fight_round(Fighter* fighter){
	fight_round_ += 1;
	AI_MAP::iterator it;
	for(it = running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.is_valid == false){
			if(it->second.config->c_type == MONSTER_AI_C_TIMES){
				int cur_val = fight_round_;
				int cmp_val = it->second.config->c_val2;
				if(it->second.config->c_val1 != 0){
					cur_val = cur_val%it->second.config->c_val1;
					cmp_val = it->second.config->c_val2%it->second.config->c_val1;
				}
				if(cur_val == cmp_val){
					it->second.is_valid = true;
					it->second.valid_time = Time_Value::gettimeofday();
				}
			}
		}
	}
	this->break_use_skill_ = false;
	this->break_use_skill_id_ = 0;
}

void Battle_AI_Object::use_skill(Fighter* fighter, int skill_id){
	if(fighter == NULL){
		return;
	}
	//check skill condition
	AI_MAP::iterator it;
	for(it = running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.is_valid == false){
			if(it->second.config->c_type == MONSTER_AI_C_SKILL){
				if(it->second.config->c_val1 == 0){// 非默认技能触发
					if(owner_ && owner_->get_default_skill() != skill_id){
						it->second.is_valid = true;
						it->second.valid_time = Time_Value::gettimeofday();
					}
				}else if(it->second.config->c_val1 == skill_id){
					it->second.is_valid = true;
					it->second.valid_time = Time_Value::gettimeofday();
				}
			}
		}
	}

}

void Battle_AI_Object::break_use_skill(Fighter* fighter, int skill_id){
	if(fighter == NULL){
		return;
	}
	if(fighter != owner_){
		return;
	}
	break_use_skill_ = true;
}

void Battle_AI_Object::hp_change(Fighter* fighter){
	//check own hp condition
	if(fighter == NULL){
		return;
	}
	if(fighter != owner_){
		return;
	}
	AI_MAP::iterator it;
	for(it= running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.config->c_type == MONSTER_AI_C_HP){
			if(it->second.is_recover == false && it->second.is_valid == false){
				bool is_valid = false;
				double cur_val = fighter->blood_current();
				double max_val = fighter->blood_max();
				int cmp_val;
				if(it->second.config->c_val2 == 0){
					cmp_val = cur_val;
				}else{
					if(max_val > 0){
						cmp_val = 1000*(cur_val/max_val);
					}else{
						cmp_val = 1000;
					}
				}
				if(it->second.config->c_val1 == 0){
					if(cmp_val >= it->second.config->c_val3){
						is_valid = true;
					}
				}else{
					if(cmp_val <= it->second.config->c_val3){
						is_valid = true;
					}
				}
				it->second.is_valid = is_valid;
				if(is_valid){
					it->second.valid_time = Time_Value::gettimeofday();
				}
			}
		}
	}
}

void Battle_AI_Object::mp_change(Fighter* fighter){
	if(fighter == NULL){
		return;
	}
	// check own mp condition
	if(fighter != owner_){
		return;
	}
	AI_MAP::iterator it;
	for(it= running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.config->c_type == MONSTER_AI_C_HP){
			if(it->second.is_recover == false && it->second.is_valid == false){
				bool is_valid = false;
				double cur_val = fighter->mp_current();
				double max_val = fighter->mp_max();
				int cmp_val;
				if(it->second.config->c_val2 == 0){
					cmp_val = cur_val;
				}else{
					if(max_val > 0){
						cmp_val = 1000*(cur_val/max_val);
					}else{
						cmp_val = 1000;
					}

				}
				if(it->second.config->c_val1 == 0){
					if(cmp_val >= it->second.config->c_val3){
						is_valid = true;
					}
				}else{
					if(cmp_val <= it->second.config->c_val3){
						is_valid = true;
					}
				}
				it->second.is_valid = is_valid;
				if(is_valid){
					it->second.valid_time = Time_Value::gettimeofday();
				}
			}
		}
	}
}

void Battle_AI_Object::die(Fighter* fighter){
	if(fighter == NULL){
		return;
	}
	AI_MAP::iterator it;
	for(it = running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.is_valid == false){
			if(it->second.config->c_type == MONSTER_AI_C_DIE){
				if(fighter->monster_self() &&
						(it->second.config->c_val1 == 0 || it->second.config->c_val1 == fighter->monster_self()->npc_type_id()))
				it->second.is_valid = true;
				it->second.valid_time = Time_Value::gettimeofday();
			}
		}
	}
}

void Battle_AI_Object::play_plot(std::string plot_id){
	//check plot condition
	AI_MAP::iterator it;
	for(it = running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.is_valid == false){
			if(it->second.config->c_type == MONSTER_AI_C_PLOT){
				std::stringstream ss;
				int tmp_value = it->second.config->c_val1;
				ss<<tmp_value;
				if(ss.str() == plot_id){
					it->second.is_valid = true;
					it->second.valid_time = Time_Value::gettimeofday();
				}

				//use skill
//				if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL){
//					int skill_id = it->second.config->a_val1;
//					if(owner_->can_use_skill(skill_id) == 0){
//						owner_->use_skill(skill_id);
//					}
//				}
			}
		}
	}
}

void Battle_AI_Object::play_talk(int talk_id){
	//check plot condition
	AI_MAP::iterator it = running_ai_data_.begin();
	for(; it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.is_valid == false){
			if(it->second.config->c_type == MONSTER_AI_C_TALK &&
					it->second.config->c_val1 == talk_id){
				it->second.is_valid = true;
				it->second.valid_time = Time_Value::gettimeofday();
			}
		}
	}
}

void Battle_AI_Object::play_ai(int ai_id){
	//check plot condition
	AI_MAP::iterator it = running_ai_data_.begin();
	for(; it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.is_valid == false){
			if(it->second.config->c_type == MONSTER_AI_C_AI &&
					it->second.config->c_val1 == ai_id){
				it->second.is_valid = true;
				it->second.valid_time = Time_Value::gettimeofday();
			}
		}
	}
}

void Battle_AI_Object::handle_ai_foreshow(void){
	if(!owner_){
		return;
	}
	if(owner_->battle_scene() == NULL){
		return;
	}
	std::vector<AI_Foreshow_Info> foreshow_vec;
	AI_Foreshow_Info info;
	AI_MAP::iterator it;
	int value;
	int type;
	int skill_id;
	int buffer_id;
	for(it = running_ai_data_.begin(); it != running_ai_data_.end(); ++it){
		if(it->second.is_recover == false && it->second.config->foreshow == 1){
			if(it->second.config->c_type == MONSTER_AI_C_AI){// type 0
				if(it->second.is_valid == false){
					continue;
				}
				skill_id = 0;
				buffer_id = 0;
				if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_DL){//
					continue;
				}
				else if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_BB ||
						it->second.config->a_type == MONSTER_AI_A_ONE_SKILL){
					skill_id = it->second.config->a_val1;
				}else if(it->second.config->a_type == MONSTER_AI_A_ADD_BUF){
					buffer_id = it->second.config->buf_argv.status_id;
				}else{
					continue;
				}

				//type
				type = 0;
				// value
				value = 0;

			}else if(it->second.config->c_type == MONSTER_AI_C_TIMES){// type 1
				skill_id = 0;
				buffer_id = 0;
				if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_DL){//
					continue;
				}
				else if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_BB ||
						it->second.config->a_type == MONSTER_AI_A_ONE_SKILL){
					skill_id = it->second.config->a_val1;
				}else if(it->second.config->a_type == MONSTER_AI_A_ADD_BUF){
					buffer_id = it->second.config->buf_argv.status_id;
				}else{
					continue;
				}

				//type
				type = 1;
				// value
				int cur_value = this->fight_round_;

				if(it->second.config->c_val1 != 0){
					cur_value = cur_value%it->second.config->c_val1;
					int cam_value = it->second.config->c_val2%it->second.config->c_val1;
					if(cur_value > cam_value){
						cam_value += it->second.config->c_val1;
					}
					value = cam_value - cur_value;
				}else{
					value = it->second.config->c_val2 - cur_value;
				}
				if(value < 0){
					value = 0;
				}
				// 被中断技下回继续使用的技能
				if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_BB){
					if(it->second.is_valid ||
							(break_use_skill_ && break_use_skill_id_ != 0 && skill_id == break_use_skill_id_)){
						value = 0;
					}
				}


			}else if(it->second.config->c_type == MONSTER_AI_C_TIME){// type 2
				skill_id = 0;
				buffer_id = 0;
				if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_DL){//
					continue;
				}
				else if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_BB ||
						it->second.config->a_type == MONSTER_AI_A_ONE_SKILL){
					skill_id = it->second.config->a_val1;
				}else if(it->second.config->a_type == MONSTER_AI_A_ADD_BUF){
					buffer_id = it->second.config->buf_argv.status_id;
				}else{
					continue;
				}

				//type
				type = 2;
				// value
				int cur_val;
				int cmp_val;
				Time_Value last_time;
				cmp_val = it->second.config->c_val2;
				if(it->second.last_time == Time_Value::zero){// 第一次触发
					last_time = Time_Value::gettimeofday() - begin_time_;
				}else{//
					if(it->second.has_trigger){
						it->second.has_trigger = false;
						it->second.last_time = Time_Value::gettimeofday();
						last_time = Time_Value::zero;
					}else{
						last_time = Time_Value::gettimeofday() - it->second.last_time;
					}

					if(it->second.config->c_val1 != 0){
						cmp_val = it->second.config->c_val1;
					}
				}
				cur_val = last_time.sec();
				value = cmp_val - cur_val;
				if(value <= 0){
					value = 1;
				}
			}else if(it->second.config->c_type == MONSTER_AI_C_HP){// type 3
				skill_id = 0;
				buffer_id = 0;
				if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_DL){//
					continue;
				}
				else if(it->second.config->a_type == MONSTER_AI_A_ONE_SKILL_BB ||
						it->second.config->a_type == MONSTER_AI_A_ONE_SKILL){
					skill_id = it->second.config->a_val1;
				}else if(it->second.config->a_type == MONSTER_AI_A_ADD_BUF){
					buffer_id = it->second.config->buf_argv.status_id;
				}else{
					continue;
				}

				// 条件检测
				bool hp_valid = false;
				{
					double cur_val = owner_->blood_current();
					double max_val = owner_->blood_max();
					int cmp_val;
					if(it->second.config->c_val2 == 0){
						cmp_val = cur_val;
					}else{
						if(max_val > 0){
							cmp_val = 1000*(cur_val/max_val);
						}else{
							cmp_val = 1000;
						}
					}
					if(it->second.config->c_val1 == 0){
						if(cmp_val >= it->second.config->c_val3){
							hp_valid = true;
						}
					}else{
						if(cmp_val <= it->second.config->c_val3){
							hp_valid = true;
						}
					}
				}

				if(hp_valid){// 满足条件 cd中
					type = 2;
					Time_Value now = Time_Value::gettimeofday();
					value = it->second.config->cd + it->second.last_time.sec() - now.sec();
					if(value <= 0){
						value = 1;
					}
				}else{// 不满足条件
					// value
					int cur_value = it->second.config->c_val3;

					if(it->second.config->c_val2 == 0){// abs
						if(it->second.config->c_val1 == 0){//  > abs
							type = 6;
						}else{
							type = 5;
						}
					}else{// %
						if(it->second.config->c_val1 == 0){ // > %
							type = 4;
							cur_value = cur_value/10;
						}else{
							type = 3;
							cur_value = cur_value/10;
						}
					}

					if(cur_value < 0){
						cur_value = 0;
					}
					value = cur_value;
				}

			}else{
				continue;
			}

			info.reset();
			info.monster_role_id = owner_->role_id();
			info.ai_id = it->second.config->id;
			info.type = type;
			info.value = value;
			info.skill_id = skill_id;
			info.buffer_id = buffer_id;
			foreshow_vec.push_back(info);
		}
	}
	owner_->battle_scene()->add_ai_foreshow_info(owner_->role_id(), foreshow_vec);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


Battle_AI_Manager::Battle_AI_Manager(void){

}

Battle_AI_Manager::~Battle_AI_Manager(void){

}

void Battle_AI_Manager::join(Fighter* fighter){
	AI_Object* ai_obj = create_ai_object(fighter);
	if(ai_obj){
		ai_object_map_.insert(std::make_pair(fighter, ai_obj));
		AI_Object* obj = get_ai_obj(fighter);
		if(obj){
			obj->init(fighter);
		}
	}
}

void Battle_AI_Manager::fight_tick(const Time_Value &now){
	AI_OBJ_MAP::iterator it = ai_object_map_.begin();
	for(; it != ai_object_map_.end(); ++it){
		it->second->fight_tick(now);
	}
}

void Battle_AI_Manager::leave(Fighter* fighter){
	relase_ai_object(fighter);
}

AI_Object* Battle_AI_Manager::create_ai_object(Fighter* fighter){
	if(!fighter){
		return NULL;
	}
	if(fighter->player_monster_self()){
		Player_AI_Object* obj = palyer_ai_obj_pool_.pop();
		return obj;
	}else if(fighter->monster_self()){
		Battle_AI_Object* obj = battle_ai_obj_pool_.pop();
		return obj;
	}else if(fighter->hero_self() && fighter->hero_self()->has_battle_ai()){
		Hero_AI_Object* obj = hero_ai_obj_pool_.pop();
		return obj;
	}
	return NULL;
}

int Battle_AI_Manager::relase_ai_object(Fighter* fighter){
	if(!fighter){
		return -1;
	}
	if(fighter->player_monster_self()){
		AI_OBJ_MAP::iterator it;
		it = ai_object_map_.find(fighter);
		if(it != ai_object_map_.end()){
			Player_AI_Object* obj = dynamic_cast<Player_AI_Object*>(it->second);
			palyer_ai_obj_pool_.push(obj);
			ai_object_map_.erase(it);
			return 0;
		}
	}else if(fighter->monster_self()){
		AI_OBJ_MAP::iterator it;
		it = ai_object_map_.find(fighter);
		if(it != ai_object_map_.end()){
			Battle_AI_Object* obj = dynamic_cast<Battle_AI_Object*>(it->second);
			battle_ai_obj_pool_.push(obj);
			ai_object_map_.erase(it);
			return 0;
		}
	}else if(fighter->hero_self() && fighter->hero_self()->has_battle_ai()){
		AI_OBJ_MAP::iterator it;
		it = ai_object_map_.find(fighter);
		if(it != ai_object_map_.end()){
			Hero_AI_Object* obj = dynamic_cast<Hero_AI_Object*>(it->second);
			hero_ai_obj_pool_.push(obj);
			ai_object_map_.erase(it);
			return 0;
		}
	}
	return -1;
}

AI_Object* Battle_AI_Manager::get_ai_obj(Fighter* fighter){
	AI_OBJ_MAP::iterator it;
	it = ai_object_map_.find(fighter);
	if(it != ai_object_map_.end()){
		return it->second;
	}
	return NULL;
}

int Battle_AI_Manager::ai_select_skill(Fighter* fighter){
	AI_Object* ai_obj = get_ai_obj(fighter);
	if(ai_obj){
		return ai_obj->ai_select_skill();
	}
	return 0;
}

void Battle_AI_Manager::start_fight(Fighter* fighter){
	AI_Object* ai_obj = get_ai_obj(fighter);
	if(ai_obj){
		return ai_obj->start_fight(fighter);
	}
}

void Battle_AI_Manager::fight_round(Fighter* fighter){
	AI_Object* ai_obj = get_ai_obj(fighter);
	if(ai_obj){
		return ai_obj->fight_round(fighter);
	}
}

void Battle_AI_Manager::use_skill(Fighter* fighter, int skill_id){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
		it->second->use_skill(fighter, skill_id);
	}
}

void Battle_AI_Manager::break_use_skill(Fighter* fighter, int skill_id){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
		it->second->break_use_skill(fighter, skill_id);
	}
}

void Battle_AI_Manager::hp_change(Fighter* fighter){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
		it->second->hp_change(fighter);
	}
}


void Battle_AI_Manager::mp_change(Fighter* fighter){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
		it->second->mp_change(fighter);
	}
}

void Battle_AI_Manager::status_overlay(Fighter* fighter, const int status_id, const int num){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
		it->second->status_overlay(fighter, status_id, num);
	}
}

void Battle_AI_Manager::die(Fighter* fighter){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
			it->second->die(fighter);
	}
}

void Battle_AI_Manager::play_plot(std::string plot_id){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
			it->second->play_plot(plot_id);
	}
}

void Battle_AI_Manager::play_talk(int talk_id){
	AI_OBJ_MAP::iterator it;
	for(it = ai_object_map_.begin(); it != ai_object_map_.end(); ++it){
			it->second->play_talk(talk_id);
	}
}

