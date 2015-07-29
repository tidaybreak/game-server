/*
 * Single_Dungeon_Struct.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: DonaldShaw
 */

#include "Single_Dungeon_Struct.h"
#include "Pool_Manager.h"
#include "DB_Operator.h"
#include "Msg_Struct.h"


Dungeon_Trail_Info::Dungeon_Trail_Info(void){
	reset();
}

void Dungeon_Trail_Info::reset(void){
	elite_items.clear();
}



/////////////////////////////////////////////////////////////////


Sgl_Dungeon_Data::Sgl_Dungeon_Data(void) {
	reset();
}

void Sgl_Dungeon_Data::reset() {
	dungeon_id_ 				= 0;
	type_						= 0;
	progress_					= COMPLETED;
	ttl_counter_ 				= 0;
	daily_counter_			= 0;
	clear_monster_count_ 	= 0;
	add_gold_num = 0;
	add_prop_num = 0;
}

int Sgl_Dungeon_Data::serialize(Block_Buffer &buffer) const {
	//ToDo: serialize sgl_dungeon_info data
	return 0;
}

int Sgl_Dungeon_Data::deserialize(Block_Buffer &buffer) {
	//Todo: deserialize
	return 0;
}

bool Sgl_Dungeon_Data::is_completed(void)const {
	return (this->progress_ == COMPLETED);
}

void Sgl_Dungeon_Data::daily_reset(void) {
	this->daily_counter_ = 0;
}

Sgl_Dungeon_Detail::Sgl_Dungeon_Detail(void) {
	reset();
}

Sgl_Dungeon_Detail::~Sgl_Dungeon_Detail() {
	//Todo Destrcutor
}

void Sgl_Dungeon_Detail::reset(void) {
	role_id = 0;
	sgl_dungeon_map_.clear();
	nor_elite_map_.clear();
	hor_elite_map_.clear();
	CLEAR_VECTOR(std::vector<Int_Int>, chapter_award_vec);
	elite_trail_info.reset();
	lord_trail_info.reset();

	single_ver = 0;
	elite_ver = 0;
	lord_ver = 0;
	single_complete_num = 0;
	nor_elite_complete_num = 0;
	hor_elite_complete_num = 0;
	elite_reset_times = 0;
	elite_ch_num = 0;
	elite_add_num = 0;
	elite_add_buy_num = 0;
	elite_task_add_num = 0;

	wipe_type = 0;
	wipe_num = 0;
	wipe_status = 0;
	wipe_spend_time = 0;								//
	wipe_elite_double = false;
	wipe_elite_double_not_selected = false;
	wipe_begin_time = Time_Value::zero;			//
	next_wipe_time = Time_Value::zero;
	wipe_finish_time = Time_Value::zero;
	CLEAR_VECTOR(Int_Vec, wipe_scenes);
	CLEAR_VECTOR(std::vector<Wipe_Info>, wipe_infos);

	act_nor_elite_wipe = 0;
	act_hor_elite_wipe = 0;
}

int Sgl_Dungeon_Detail::serialize(Block_Buffer &buffer) const {
	//Todo serialize sgl_dungeon_detail data
	return 0;
}

int Sgl_Dungeon_Detail::deserialize(Block_Buffer &buffer) {
	//todo deserialize sgl_dungeon_detail data
	return 0;
}

int Sgl_Dungeon_Detail::load(void) {
	return MONGO_OPERATOR->load_sgl_dungeon_detail(*this);
}

int Sgl_Dungeon_Detail::save(void) {
	if (this->role_id)
		return MONGO_OPERATOR->save_sgl_dungeon_detail(*this);
	else
		return 0;
}

void Sgl_Dungeon_Detail::reset_clear_monster_counter(int dungeon_id) {
	if (0 == dungeon_id){
		return;
	}
	Sgl_Dungeon_Map::iterator it = this->sgl_dungeon_map_.find(dungeon_id);
	if (it != this->sgl_dungeon_map_.end()){
		it->second.clear_monster_count_ = 0;
	}
}

void Sgl_Dungeon_Detail::set_chapter_status(int chapter, int status){
	int count = this->chapter_award_vec.size();
	for(int i=0; i<count; ++i){
		if(chapter_award_vec[i].val_1 == chapter){
			chapter_award_vec[i].val_2 = status;
			detail_change();
			return;
		}
	}
	Int_Int chapter_int_int;
	chapter_int_int.val_1 = chapter;
	chapter_int_int.val_2 = status;
	chapter_award_vec.push_back(chapter_int_int);
	detail_change();
}

int Sgl_Dungeon_Detail::get_chapter_status(int chapter) const {
	int count = this->chapter_award_vec.size();
	for(int i=0; i<count; ++i){
		if(chapter_award_vec[i].val_1 == chapter){
			return chapter_award_vec[i].val_2;
		}
	}
	return 0;
}

const Sgl_Dungeon_Data* Sgl_Dungeon_Detail::get_dungeon_data(int scene){
	Sgl_Dungeon_Map::iterator it = sgl_dungeon_map_.find(scene);
	if(it != sgl_dungeon_map_.end()){
		return &(it->second);
	}
	return NULL;
}

void Sgl_Dungeon_Detail::reset_wipe_info(void){
	wipe_type = 0;
	wipe_num = 0;
	wipe_spend_time = 0;								//
	wipe_begin_time = Time_Value::zero;			//
	wipe_finish_time = Time_Value::zero;
	wipe_scenes.clear();
	wipe_infos.clear();
	this->detail_change();
}
