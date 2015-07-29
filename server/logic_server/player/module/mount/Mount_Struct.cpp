/*
 * Mount_Struct.cpp
 *
 *  Created on: 2014年7月23日
 *      Author: root
 */

#include "Mount_Struct.h"
#include "DB_Operator.h"

Mount_Detail::Mount_Detail(void){
	reset();
}

Mount_Detail::~Mount_Detail(void){

}

void Mount_Detail::reset(void){
	role_id = 0;
	mount_id = 0;
	ride = 0;
	deploy_num = 0;
	CLEAR_VECTOR(std::vector<Horse_Info>, horse_list);
	CLEAR_VECTOR(std::vector<Horse_Group_Info>, group_list);
}

int Mount_Detail::serialize(Block_Buffer &buffer) const{
	return 0;
}

int Mount_Detail::deserialize(Block_Buffer &buffer){
	return 0;
}

int Mount_Detail::load(void){
	return MONGO_OPERATOR->load_mount_detail(*this);
}

int Mount_Detail::save(void){
	if (role_id){
		return MONGO_OPERATOR->save_mount_detail(*this);
	}
	return 0;
}

void Mount_Detail::add_horse(const int horse_id, const int train_lv, const Time_Value& deadline, const Time_Value& activate_time,
		const int cur_attr_1, const int cur_attr_2, const int cur_attr_3, const int cur_attr_4, const int cur_attr_5,
		const int max_attr_1, const int max_attr_2, const int max_attr_3, const int max_attr_4, const int max_attr_5,
		const int next_update_num){
	Horse_Info info;
	info.horse_id = horse_id;
	info.train_lv = train_lv;
	info.deadline = deadline;
	info.activate_time = activate_time;
	info.cur_train_attr1 = cur_attr_1;
	info.cur_train_attr2 = cur_attr_2;
	info.cur_train_attr3 = cur_attr_3;
	info.cur_train_attr4 = cur_attr_4;
	info.cur_train_attr5 = cur_attr_5;
	info.max_train_attr1 = max_attr_1;
	info.max_train_attr2 = max_attr_2;
	info.max_train_attr3 = max_attr_3;
	info.max_train_attr4 = max_attr_4;
	info.max_train_attr5 = max_attr_5;
	info.next_update_num = next_update_num;
	info.next_lv_total_train_num = next_update_num;
	horse_list.push_back(info);
	detail_change();
}

void Mount_Detail::del_horse(int horse_id){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(it->horse_id == horse_id){
			horse_list.erase(it);
			break;
		}
	}
	if(this->mount_id == horse_id){
		this->mount_id = get_first_valid_horse();
	}
	detail_change();
}

bool Mount_Detail::has_horse(int horse_id){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(it->horse_id == horse_id){
			return true;
		}
	}
	return false;
}

bool Mount_Detail::horse_is_invalid(const int horse_id){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(it->horse_id == horse_id && it->invalid == 1){
			return true;
		}
	}
	return false;
}

void Mount_Detail::valid_horse(const int horse_id, const Time_Value& deadline){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(it->horse_id == horse_id){
			it->deadline = deadline;
			it->invalid = 0;
			detail_change();
			break;
		}
	}
}
void Mount_Detail::invalid_horse(const int horse_id){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(it->horse_id == horse_id){
			it->invalid = 1;
			it->deploy = 0;
			detail_change();
			break;
		}
	}
}

int Mount_Detail::valid_group(int group_id, int status/* = 2*/){
	bool b_new = true;
	std::vector<Horse_Group_Info>::iterator it;
	for(it = group_list.begin(); it != group_list.end(); ++it){
		if(it->group_id == group_id){
			it->status = 2;
			b_new = false;
			break;
		}
	}
	if(b_new){
		Horse_Group_Info info;
		info.group_id = group_id;
		info.status = status;
		group_list.push_back(info);
	}
	detail_change();
	return 0;
}

void Mount_Detail::invalid_group(int group_id){
	std::vector<Horse_Group_Info>::iterator it;
	for(it = group_list.begin(); it != group_list.end(); ++it){
		if(it->group_id == group_id){
			it->status = 0;
			detail_change();
			break;
		}
	}
}

bool Mount_Detail::has_group(int group_id){
	std::vector<Horse_Group_Info>::iterator it;
	for(it = group_list.begin(); it != group_list.end(); ++it){
		if(it->group_id == group_id){
			return true;
		}
	}
	return false;
}

int Mount_Detail::change_group_status(int group_id, int status){
	std::vector<Horse_Group_Info>::iterator it;
	for(it = group_list.begin(); it != group_list.end(); ++it){
		if(it->group_id == group_id){
			it->status = status;
			this->detail_change();
			return 0;
		}
	}
	return -1;
}

int Mount_Detail::get_first_valid_horse(void){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(it->invalid == 0){
			return it->horse_id;
		}
	}
	return 0;
}

int Mount_Detail::get_train_lv(const int horse_id){
	std::vector<Horse_Info>::iterator it;
	for(it = horse_list.begin(); it != horse_list.end(); ++it){
		if(horse_id == it->horse_id){
			return it->train_lv;
		}
	}
	return 0;
}
