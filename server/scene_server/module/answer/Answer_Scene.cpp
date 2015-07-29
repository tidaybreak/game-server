/*
 * Answer_Scene.cpp
 *
 *  Created on: 2014年8月13日
 *      Author: XIAOLIANG
 */

#include "Answer_Scene.h"
#include "Err_Code.h"
#include "Config_Cache_Answer.h"
#include "Msg_Active_Struct.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Answer_Struct.h"
#include "Config_Cache.h"
#include "Base_Detail.h"


Answer_Scene::Answer_Scene() {
	// TODO Auto-generated constructor stub
	reset();
}

Answer_Scene::~Answer_Scene() {
	// TODO Auto-generated destructor stub
}

void Answer_Scene::reset(void) {
	end_time = Time_Value::zero;
	q.reset();
}

int Answer_Scene::tick(const Time_Value &now){

	if(now.sec() >= start_time.sec()
			&& now.sec() < (start_time.sec()  + get_ones_time_sec() * CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_QUESTION_NUM))) {
		//检测生成题目
		check_make_question(now);
		//检测当前题目所处于时间的应该触发状态
		update_question_status(now);
	}

	if(end_time.sec() && now.sec() >= end_time.sec()) {
		send_to_logic_get_rank_reward();
		kick_all_palyer();
		this->set_scene_status(Move_Scene::SCENE_RECOVER_STATUS);
	}

	return 0;
}


int Answer_Scene::init_scene_normal(int scene_id, int scene_order, int map_id/* = 0*/){
	Move_Scene_Base::init_scene_normal(scene_id, scene_order, map_id);
	transmit_point = scene_config()->transmit_point;
	scene_begin_time_= Time_Value::gettimeofday();

	return 0;
}

int Answer_Scene::release_scene_normal(void){
	MSG_DEBUG("answer scene release,scene_id:%d, time:%ld", scene_id(), Time_Value::gettimeofday().sec());
	Move_Scene::release_scene_normal();

	return 0;
}

void Answer_Scene::kick_all_palyer(void) {
	std::vector<Scene_Player*> player_vec;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				player_vec.push_back(player);
			}
		}
	}
	std::vector<Scene_Player*>::iterator it;
	for(it = player_vec.begin(); it != player_vec.end(); ++it){
		int dest = 0;
		(*it)->inner_transmit(dest,0,0);
	}
}

int Answer_Scene::enter_scene(Mover *mover){
	Move_Scene::enter_scene(mover);
	return 0;
}

int Answer_Scene::exit_scene(Mover *mover, bool offline){
	Move_Scene::exit_scene(mover, offline);
	return 0;
}

int Answer_Scene::client_scene_complete(Fighter &fighter){
	Move_Scene::client_scene_complete(fighter);
	send_client_question_info(&fighter);
	send_client_rank_info(&fighter);
	return 0;
}

void Answer_Scene::init_scene_data(const Time_Value &stime, const Answer_All_Data_Info &answer_info_) {
	start_time = stime;
	answer_info = answer_info_;
	refresh_rank();
}

void Answer_Scene::scene_close_note(const Time_Value &etime) {
	end_time.sec(etime.sec() + 10);
	MSG_82330002 msg;
	msg.time = end_time.sec();
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
					it->second->send_to_client(buf__);
			}
		}
	}

}

int Answer_Scene::req_use_answer_free(Scene_Player &player) {
	if(q.id == 0 || q.status == QUESTION_STATUS_INIT) {
		return ERROR_ANSWER_NOT_START;
	}
	if(q.status == QUESTION_STATUS_ANN || q.status == QUESTION_STATUS_OK) {
		return ERROR_ANSWER_THIS_QUISC;
	}
	//获得所有次数;
	uint32_t max = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_FREE);
	if(player.vip()) {
		max = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_FREE);
	}
	//判断次数
	RoleID_Answer_Role_Data_Info_Map::iterator it = answer_info.role_data_info_map.find(player.role_id());
	if(it != answer_info.role_data_info_map.end()) {
		if(it->second.cur_free_qid == q.id) {
			return ERROR_NOT_DOUBLE_TO_USE;
		}
		if(it->second.use_free_num >= max) {
			return ERROR_ANSWER_FREE_NUM;
		}
		++it->second.use_free_num;
		it->second.cur_free_qid = q.id;
	} else {
		Answer_Role_Data_Info data;
		data.reset();
		data.cur_free_qid = q.id;
		++data.use_free_num;
		answer_info.role_data_info_map.insert(std::make_pair(player.role_id(), data));
	}
	MSG_50209001 msg;
	Block_Buffer buf__;
	uint32_t max_free = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_FREE);
	if(player.vip()) {
		max_free = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_FREE);
	}
	RoleID_Answer_Role_Data_Info_Map::iterator iter = answer_info.role_data_info_map.find(player.role_id());
	if(iter != answer_info.role_data_info_map.end()) {
		int free_num = max_free - iter->second.use_free_num;
		if(free_num > 0) {
			msg.num = free_num;
		}
	} else {
		msg.num = max_free;
	}
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	player.send_to_client(buf__);
	return 0;
}

int Answer_Scene::req_answer_free_double_num(Scene_Player &player) {
	MSG_50209003 msg;
	uint32_t max_free = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_FREE);
	uint32_t max_double = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_DOUBLE);
	if(player.vip()) {
		max_free = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_FREE);
		max_double = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_DOUBLE);
	}
	RoleID_Answer_Role_Data_Info_Map::iterator iter = answer_info.role_data_info_map.find(player.role_id());
	if(iter != answer_info.role_data_info_map.end()) {
		int free_num = max_free - iter->second.use_free_num;
		int double_num = max_double - iter->second.use_double_num;
		if(free_num > 0) {
			msg.free = free_num;
		}
		if(double_num > 0) {
			msg.double_num = double_num;
		}
	} else {
		msg.free = max_free;
		msg.double_num = max_double;
	}
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	player.send_to_client(buf__);
	return 0;
}

int Answer_Scene::req_use_answer_double(Scene_Player &player) {
	if(q.id == 0 || q.status == QUESTION_STATUS_INIT) {
		return ERROR_ANSWER_NOT_START;
	}
	if(q.status == QUESTION_STATUS_ANN || q.status == QUESTION_STATUS_OK) {
		return ERROR_ANSWER_THIS_QUISC;
	}
	//获得所有次数;
	uint32_t max = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_DOUBLE);
	if(player.vip()) {
		max = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_DOUBLE);
	}
	//判断次数
	RoleID_Answer_Role_Data_Info_Map::iterator it = answer_info.role_data_info_map.find(player.role_id());
	if(it != answer_info.role_data_info_map.end()) {
		if(it->second.cur_double_qid == q.id) {
			return ERROR_NOT_DOUBLE_TO_USE;
		}
		if(it->second.use_double_num >= max) {
			return ERROR_ANSWER_NOT_DOUBLE;
		}
		++it->second.use_double_num;
		it->second.cur_double_qid = q.id;
	} else {
		Answer_Role_Data_Info data;
		data.reset();
		data.cur_double_qid = q.id;
		++data.use_double_num;
		answer_info.role_data_info_map.insert(std::make_pair(player.role_id(), data));
	}
	MSG_50209002 msg;
	uint32_t max_double = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_DOUBLE);
	if(player.vip()) {
		max_double = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_DOUBLE);
	}
	RoleID_Answer_Role_Data_Info_Map::iterator iter = answer_info.role_data_info_map.find(player.role_id());
	if(iter != answer_info.role_data_info_map.end()) {
		int double_num = max_double - iter->second.use_double_num;
		if(double_num > 0) {
			msg.num = double_num;
		}
	} else {
		msg.num = max_double;
	}
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	player.send_to_client(buf__);
	return 0;
}

void Answer_Scene::check_make_question(const Time_Value &now) {
	//检测当前题目是否完成状态
	if(q.status != QUESTION_STATUS_OK && q.id != 0) {
		return;
	}
	//检测当前时间处于第几题
	int sl_sec = now.sec() - start_time.sec();
	if(sl_sec < 0) {
		return;
	}
	uint32_t ones_sec = get_ones_time_sec();
	uint16_t this_qno = sl_sec/ones_sec + 1;
	//检测当前是否正处于哪一题
	if(this_qno <= answer_info.question_id_list.size()) {
		return;
	}
	//随机生成题目
	uint32_t qid = rand_make_question();
	if(qid == 0)  {
		return;
	}
	//更新题目信息
	if(make_active_question(qid)){
		return;
	}
	answer_info.cur_qno = this_qno;
	 answer_info.question_id_list.insert(qid);
	send_to_logic_save_data();
}

void Answer_Scene::update_question_status(const Time_Value &now) {
	int this_start_sec = get_ones_time_sec() * (answer_info.cur_qno-1);
	if(this_start_sec < 0) {
		return;
	}
	uint32_t this_start_time = start_time.sec() + this_start_sec;
	int slep_sec = now.sec() - this_start_time;//当前题目开始时间与当前时间间隔的秒数
	if(slep_sec < 0) {
		return;
	}
	//计算当前时间该题目应该处于神马状态
	uint32_t read_sec = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_READ_TIME);
	uint32_t answer_sec = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_ANSWER_TIME);
	uint32_t ann_sec = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_ANN_TIME);
	if((uint32_t)slep_sec <= read_sec) {
		//阅题
		if(q.status == QUESTION_STATUS_READ) {
			return;
		} else {
			q.status = QUESTION_STATUS_READ;
			q.over_time = this_start_time + read_sec;
			send_client_question_info();
		}
	} else if((uint32_t)slep_sec <= (answer_sec+read_sec)) {
		//答题
		if(q.status == QUESTION_STATUS_ANSWER) {
			return;
		} else {
			q.status = QUESTION_STATUS_ANSWER;
			q.over_time = this_start_time + read_sec + answer_sec;
			send_client_question_info();
		}
	} else if((uint32_t)slep_sec <= (ann_sec+answer_sec+read_sec)) {
		//公布
		if(q.status == QUESTION_STATUS_ANN) {
			return;
		} else {
			q.status = QUESTION_STATUS_ANN;
			q.over_time = this_start_time + read_sec + answer_sec + ann_sec;
			//计算动态答案
			make_active_question_on_fulfil(q.id);
			send_client_question_info();
			fulfil_active_question();
		}
	} else {
		if(q.status == QUESTION_STATUS_OK) {
				return;
		} else {
			  q.status = QUESTION_STATUS_OK;
		}
	}
}

int Answer_Scene::make_active_question(uint32_t qid) {
		const Answer_Question_Config_Map &conf = CONFIG_CACHE_ANSWER->get_question();
		Answer_Question_Config_Map::const_iterator it = conf.find(qid);
		if(it == conf.end()) {
			return -1;
		}
		q.id = qid;
		q.status = QUESTION_STATUS_INIT;
		q.a.clear();
		q.b.clear();
		q.true_answer = 0;
		switch(qid) {
			case ACTIVE_TYPE_20005: {
				if(answer_info.force_rank.size() < 2) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.force_rank[0];
					q.b = answer_info.force_rank[1];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.force_rank[0];
					q.a = answer_info.force_rank[1];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20006: {
				if(answer_info.force_rank.size() < 3) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.force_rank[1];
					q.b = answer_info.force_rank[2];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.force_rank[1];
					q.a = answer_info.force_rank[2];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20007: {
				if(answer_info.force_rank.size() < 3) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.force_rank[2];
					q.b = answer_info.force_rank[1];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.force_rank[2];
					q.a = answer_info.force_rank[1];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20008: {
				if(answer_info.arena_rank.size() < 2) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.arena_rank[0];
					q.b = answer_info.arena_rank[1];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.arena_rank[0];
					q.a = answer_info.arena_rank[1];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20009: {
				if(answer_info.arena_rank.size() < 3) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.arena_rank[1];
					q.b = answer_info.arena_rank[2];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.arena_rank[1];
					q.a = answer_info.arena_rank[2];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20010: {
				if(answer_info.arena_rank.size() < 3) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.arena_rank[2];
					q.b = answer_info.arena_rank[1];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.arena_rank[2];
					q.a = answer_info.arena_rank[1];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20014: {
				if(answer_info.gang_rank.size() < 2) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.gang_rank[0];
					q.b = answer_info.gang_rank[1];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.gang_rank[0];
					q.a = answer_info.gang_rank[1];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			case ACTIVE_TYPE_20015: {
				if(answer_info.gang_rank.size() < 2) {break;}
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint8_t t = random() % 2;
				if(t){
					q.a = answer_info.gang_rank[1];
					q.b = answer_info.gang_rank[0];
					q.true_answer = ANSWER_TYPE_A_TRUE;
				} else {
					q.b = answer_info.gang_rank[1];
					q.a = answer_info.gang_rank[0];
					q.true_answer = ANSWER_TYPE_B_TRUE;
				}
				break;
			}
			default: {
				q.a = it->second.answer_a;
				q.b = it->second.answer_b;
				q.true_answer = it->second.true_answer;
				break;
			}
		}
		return 0;
}

int Answer_Scene::make_active_question_on_fulfil(uint32_t qid) {
	switch(qid) {
		case ACTIVE_TYPE_20003: {
			uint32_t a = 0;
			uint32_t b = 0;
			get_a_and_b_num(a, b);
			if(a == b) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(a > b){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
		case ACTIVE_TYPE_20004: {
			uint32_t a = 0;
			uint32_t b = 0;
			get_a_and_b_num(a, b);
			if(a == b) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(a < b){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
		case ACTIVE_TYPE_20016: {
			uint32_t a = 0;
			uint32_t b = 0;
			get_a_and_b_male_num(a, b);
			if(a == b) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(a < b){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
		case ACTIVE_TYPE_20017: {
			uint32_t a = 0;
			uint32_t b = 0;
			get_a_and_b_male_num(a, b);
			if(a == b) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(a > b){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
		case ACTIVE_TYPE_20018: {
			uint32_t a = 0;
			uint32_t b = 0;
			get_a_and_b_female_num(a, b);
			if(a == b) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(a > b){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
		case ACTIVE_TYPE_20019: {
			uint32_t a = 0;
			uint32_t b = 0;
			get_a_and_b_female_num(a, b);
			if(a == b) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(a < b){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
		case ACTIVE_TYPE_10162: {
			uint32_t m = 0;
			uint32_t f = 0;
			get_male_and_female_num(m,f);
			if(m == f) {
				q.true_answer = ANSWER_TYPE_ALL_TRUE;
			} else if(m > f){
				q.true_answer = ANSWER_TYPE_A_TRUE;
			} else {
				q.true_answer = ANSWER_TYPE_B_TRUE;
			}
			break;
		}
	}
	return 0;
}

int Answer_Scene::fulfil_active_question() {
		if(q.status != QUESTION_STATUS_ANN) return 0;
		const Answer_Question_Config_Map &conf = CONFIG_CACHE_ANSWER->get_question();
		Answer_Question_Config_Map::const_iterator it = conf.find(q.id);
		if(it == conf.end()) {
			return 0;
		}
		//计算刷新积分积分和排名
		refresh_sorce();
		refresh_rank();
		//note logic
		send_to_logic_save_data();
		return 0;
}

uint32_t Answer_Scene::rand_make_question(void) {
	const Answer_Question_Config_Map &conf = CONFIG_CACHE_ANSWER->get_question();
	Answer_Question_Config_Map::const_iterator iter = conf.find(q.id);
	if( iter != conf.end() && iter->second.next_id) {
		Answer_Question_Config_Map::const_iterator iter2 = conf.find(iter->second.next_id);
		if(iter2 != conf.end()) {
			if(iter2->second.true_answer == 0) {
				++answer_info.ext_num;
			}
			return iter2->first;
		}
	} else {
		UInt_Vec qlist; qlist.clear();
		for(Answer_Question_Config_Map::const_iterator it = conf.begin(); it != conf.end(); ++it) {
			if(answer_info.question_id_list.find(it->first) == answer_info.question_id_list.end()) {
				qlist.push_back(it->first);
			}
		}
		srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
		uint32_t index = random() % qlist.size();
		Answer_Question_Config_Map::const_iterator it = conf.find(qlist[index]);
		if(it != conf.end()) {
			if(it->second.true_answer == 0) {
				++answer_info.ext_num;
			}
			return it->first;
		}
	}
	return 0;
}

uint32_t Answer_Scene::get_ones_time_sec(void) {
	return CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_READ_TIME)
			+CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_ANSWER_TIME)
			+CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_ANN_TIME);
}

void Answer_Scene::refresh_sorce(void) {
	const Answer_Question_Config *conf = CONFIG_CACHE_ANSWER->get_question_byid(q.id);
	if(!conf) {
		return;
	}
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && (it->second)->player_self()) {
			//判断能是否答对
			if(q.true_answer != ANSWER_TYPE_ALL_TRUE && !get_is_set_free(it->second) && get_mover_xy_to_answer(it->second) != q.true_answer) {
				continue;
			}
			role_id_t role_id = (it->second)->role_id();
			RoleID_Data_Map::iterator iter = answer_info.role_sorce_map.find(role_id);
			if(iter != answer_info.role_sorce_map.end()) {
				iter->second.role_id = role_id;
				iter->second.name = (it->second)->role_name();
				iter->second.level = (it->second)->level();
				uint32_t sorce = conf->integral * get_is_set_multiple(it->second);
				iter->second.sorce += sorce;
				++iter->second.true_num;
				const Answer_Integral_Reward_Config *reward_conf = CONFIG_CACHE_ANSWER->get_integral_reward_by_level(iter->second.level);
				if(reward_conf) {
					iter->second.exp += reward_conf->reward.exp * sorce;
					send_logic_get_integral_reward((it->second), sorce);
				}
			} else {
				Answer_Rank_Role_Data data;
				data.reset();
				data.role_id = role_id;
				data.name = (it->second)->role_name();
				data.level = (it->second)->level();
				data.sorce = conf->integral * get_is_set_multiple(it->second);
				data.true_num = 1;
				const Answer_Integral_Reward_Config *reward_conf = CONFIG_CACHE_ANSWER->get_integral_reward_by_level(data.level);
				if(reward_conf) {
					data.exp = reward_conf->reward.exp * data.sorce;
					send_logic_get_integral_reward((it->second), data.sorce);
				}
				answer_info.role_sorce_map.insert(std::make_pair(role_id, data));
			}
		}
	}
}

void Answer_Scene::refresh_rank(void) {
	role_sorce_rank.clear();
	for(RoleID_Data_Map::iterator it = answer_info.role_sorce_map.begin(); it != answer_info.role_sorce_map.end(); ++it) {
		RoleID_Data_Vec_Map::iterator iter = role_sorce_rank.find(it->second.sorce);
		if(iter != role_sorce_rank.end()) {
			iter->second.push_back(it->second);
		} else {
			RoleID_Data_Vec vec;
			vec.clear();
			vec.push_back(it->second);
			role_sorce_rank.insert(std::make_pair(it->second.sorce, vec));
		}
	}
	send_client_rank_info();
}

void Answer_Scene::send_to_logic_save_data(void) {
	//以下代码;策划要求改跨服需要特殊处理
	Block_Buffer buf;
	buf.make_message(INNER_ANSWER_DATA_CHANNEL);
	buf.write_uint8(ANSWER_DATA_CHANNLE_TYPE_SL_SAVE_DATA);

	//DATA START
	answer_info.serialize(buf);
	//DATA END

	buf.finish_message();
	SCENE_MONITOR->send_to_logic(CONFIG_CACHE->server_flag(), buf);
}

void Answer_Scene::send_to_logic_get_rank_reward(void) {
	refresh_rank();
	//以下代码;策划要求改跨服需要特殊处理
	Block_Buffer buf;
	buf.make_message(INNER_ANSWER_DATA_CHANNEL);
	buf.write_uint8(ANSWER_DATA_CHANNLE_TYPE_SL_GET_RANK_REWARD);
	buf.write_uint16(role_sorce_rank.size());
	for(RoleID_Data_Vec_Map::iterator iter = role_sorce_rank.begin(); iter != role_sorce_rank.end(); ++iter) {
		buf.write_uint16(iter->second.size());
		for(RoleID_Data_Vec::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			buf.write_int64(it->role_id);
		}
	}
	SCENE_MONITOR->send_to_logic(CONFIG_CACHE->server_flag(), buf);
}

void Answer_Scene::send_client_question_info(void) {
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && it->second->player_self()) {
			send_client_question_info(it->second);
		}
	}
}

void Answer_Scene::send_logic_get_integral_reward(Mover *mover, const uint32_t sorce) {
	if(!mover) return;
	Block_Buffer buf;
	buf.make_message(INNER_ANSWER_DATA_CHANNEL);
	buf.write_uint8(ANSWER_DATA_CHANNLE_TYPE_SL_GET_INTEGRAL_REWARD);
	//DATA START
	buf.write_uint32(sorce);
	//DATA END
	buf.finish_message();
	mover->send_to_logic(buf);
}

uint32_t Answer_Scene::get_mover_xy_to_answer(Mover *mover) {
	if(mover) {
		if(mover->mover_detail().pixel_coord.x <= CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_CENTER_X)) {
			return ANSWER_TYPE_A_TRUE;
		} else {
			return ANSWER_TYPE_B_TRUE;
		}
	}
	return 0;
}

bool Answer_Scene::get_is_set_free(Mover *mover) {
	if(mover) {
		RoleID_Answer_Role_Data_Info_Map::iterator it = answer_info.role_data_info_map.find(mover->role_id());
		if(it != answer_info.role_data_info_map.end()) {
			if(it->second.cur_free_qid == q.id) {
				return true;
			}
		}
	}
	return false;
}

uint8_t Answer_Scene::get_is_set_multiple(Mover *mover) {
	if(mover) {
		RoleID_Answer_Role_Data_Info_Map::iterator it = answer_info.role_data_info_map.find(mover->role_id());
		if(it != answer_info.role_data_info_map.end()) {
			if(it->second.cur_double_qid == q.id) {
				return 2;
			}
		}
	}
	return 1;
}
void Answer_Scene::send_client_question_info(Mover *mover) {
	MSG_82330000 msg;
	msg.id = q.id;
	msg.status = q.status;
	msg.a = q.a;
	msg.b = q.b;
	msg.qno = answer_info.cur_qno;
	if(q.status == QUESTION_STATUS_ANN) {
		msg.true_answer = q.true_answer;
	} else {
		msg.true_answer = 0;
	}
	msg.time = q.over_time;
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	mover->send_to_client(buf__);
}

void Answer_Scene::send_client_rank_info(void) {
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && it->second->player_self()) {
			send_client_rank_info(it->second);
		}
	}
}
void Answer_Scene::send_client_rank_info(Mover *mover) {
	if(!mover) return;
	MSG_82330001 msg;
	msg.start_time = start_time.sec();
	send_client_rank_info_msg_rank(msg.rank);

	RoleID_Data_Map::iterator it = answer_info.role_sorce_map.find(mover->role_id());
	if(it != answer_info.role_sorce_map.end()) {
		msg.true_num = it->second.true_num;
		msg.sorce = it->second.sorce;
		msg.exp = it->second.exp;
	}
	uint32_t max_free = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_FREE);
	uint32_t max_double = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_MAX_DOUBLE);
	if(mover->player_self()->vip()) {
		max_free = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_FREE);
		max_double = CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_VIP_DOUBLE);
	}
	RoleID_Answer_Role_Data_Info_Map::iterator iter = answer_info.role_data_info_map.find(mover->role_id());
	if(iter != answer_info.role_data_info_map.end()) {
		int free_num = max_free - iter->second.use_free_num;
		int double_num = max_double - iter->second.use_double_num;
		if(free_num > 0) {
			msg.free_num = free_num;
		}
		if(double_num > 0) {
			msg.double_num = double_num;
		}
	} else {
		msg.free_num = max_free;
		msg.double_num = max_double;
	}
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	mover->send_to_client(buf__);
}

void Answer_Scene::send_client_rank_info_msg_rank(std::vector<Answer_Rank_Role_Info> &rank) {
	uint8_t i = 0;
	Answer_Rank_Role_Info info;
	for(RoleID_Data_Vec_Map::iterator iter = role_sorce_rank.begin(); iter != role_sorce_rank.end(); ++iter) {
		for(RoleID_Data_Vec::iterator it = iter->second.begin(); it != iter->second.end(); ++it) {
			info.reset();
			info.level = it->level;
			info.role_id = it->role_id;
			info.role_name = it->name;
			info.sorce = it->sorce;
			rank.push_back(info);
		}
		++i;
		if(i >= 10) {
			return;
		}
	}
}

void Answer_Scene::get_a_and_b_num(uint32_t &a, uint32_t &b) {//累加得到左右两边人数
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && it->second->player_self()) {
			if(it->second->mover_detail().pixel_coord.x <= CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_CENTER_X)) {
				++a;
			} else {
				++b;
			}
		}
	}
}
void Answer_Scene::get_a_and_b_male_num(uint32_t &a, uint32_t &b) {//累加得到左右两边男性人数
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::const_iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && it->second->player_self() && it->second->player_self()->base_detail().gender == 1) {
			if(it->second->mover_detail().pixel_coord.x <= CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_CENTER_X)) {
				++a;
			} else {
				++b;
			}
		}
	}
}
void Answer_Scene::get_a_and_b_female_num(uint32_t &a, uint32_t &b) {//累加得到左右两边女性人数
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::const_iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && it->second->player_self() && it->second->player_self()->base_detail().gender == 2) {
			if(it->second->mover_detail().pixel_coord.x <= CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_CENTER_X)) {
				++a;
			} else {
				++b;
			}
		}
	}
}

void Answer_Scene::get_male_and_female_num(uint32_t &male, uint32_t &female) {
	Scene_Layer *layer = scene_layer(0);
	if (!layer) { return; }
	for (Mover_Map::const_iterator it = layer->mover_map_.begin();
			it != layer->mover_map_.end(); ++it) {
		if(it->second && it->second->player_self()) {
			if(it->second->player_self()->base_detail().gender) {
				++male;
			} else {
				++female;
			}
		}
	}
}
