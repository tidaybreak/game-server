/*
 * Campfire_Manager.cpp
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#include "Campfire_Manager.h"
#include "DB_Operator.h"
#include "Campfire_Struct.h"
#include "Msg_Active_Struct.h"
#include "Logic_Monitor.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Secret_Shop.h"
#include "announcement/Announcement_Struct.h"
#include "public/Public_Struct.h"
#include "Logic_Public.h"


Campfire_Manager::Campfire_Manager(void):first_start_(false), act_id(0){

}

Campfire_Manager::~Campfire_Manager(void){

}

int Campfire_Manager::start(void){
	load_fb_info();
	load_campfire();
	Time_Value now = Time_Value::gettimeofday();
	cal_active_time(now, active_status_);
	first_start_ = true;
	return 0;
}

int Campfire_Manager::tick(const Time_Value& now){
	if(first_start_){
		first_start_ = false;
		if(active_status_.status == 2){
			campfire_ready();
		}else if(active_status_.status == 3){
			campfire_start();
		}
		return 0;
	}
	if(active_status_.status != 0 &&
			now > active_status_.next_status_time){
		if(active_status_.status == 1){// 关闭 ----> 准备
			active_status_.status = 2;
			active_status_.next_status_time = active_status_.next_status_time +
					Time_Value(active_status_.ready_time);
		}else if(active_status_.status == 2){// 准备 ----> 开启
			active_status_.status = 3;
			active_status_.next_status_time = active_status_.next_status_time +
					Time_Value(active_status_.act_time);
		}else if(active_status_.status == 3){// 战场开启 ----> 关闭
			cal_active_time(now, active_status_);
		}
		for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
			if(it->second){
				it->second->send_bonfire_active_to_client();
			}
		}
		//activate campfire to scene
		if(active_status_.status == 3){
			campfire_start();
		}else if(active_status_.status == 1){
			campfire_finish();
		}else if(active_status_.status == 2){
			campfire_ready();
		}
	}
	return 0;
}

void Campfire_Manager::daily_refresh(void){
	// 刷新篝火
//	std::vector<Campfire_Detail>::iterator it;
//	for(it = campfire_info_.begin(); it != campfire_info_.end(); ++it){
//		if(it->burning != 0){
//			it->burning = 0;
//			it->detail_change();
//			it->save();
//		}
//	}
}

int Campfire_Manager::get_enter_level_limit(void){
	return CONFIG_CACHE_SECRET_SHOP->get_campfire_level_limit();
}

int Campfire_Manager::get_dancing_time(void){
	return CONFIG_CACHE_SECRET_SHOP->get_campfire_dancing_time();
}

void Campfire_Manager::get_active_info(int& status, int& remain_time, int &party_id){
	Time_Value now = Time_Value::gettimeofday();
	status = active_status_.status;
	remain_time = active_status_.next_status_time.sec() - now.sec();
	party_id = this->act_id;
}

int Campfire_Manager::get_campfire_point_id(gang_id_t gang_id){
	Campfire_Detail_Map::iterator it = campfire_info_.find(gang_id);
	if(it != campfire_info_.end()){
		return it->second.id;
	}
	return 0;
}

int Campfire_Manager::cmd_sync_campfire_info(Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 0:{// 篝火晚会采集烤肉加公会贡献
			role_id_t p_role_id = 0;
			int add_con = 0;
			buf.read_int64(p_role_id);
			buf.read_int32(add_con);
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(p_role_id);
			if(player && add_con > 0){
				player->add_contribute(add_con);
			}
			break;
		}
		case 1:{// 公会成员添加柴火
			role_id_t p_role_id = 0;
			Campfire_Detail detail;
			buf.read_int64(p_role_id);
			detail.deserialize(buf);
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(p_role_id);
			if(player){
				player->cmd_add_firewood(1);
			}
			Campfire_Detail*p_detail = get_campfire_detail(detail.gang_id);
			if(p_detail){
				p_detail->level = detail.level;
				p_detail->firewood_num = detail.firewood_num;
				p_detail->level_firewood_num = detail.level_firewood_num;
				p_detail->level_max_firewood_num = detail.level_max_firewood_num;
				p_detail->detail_change();
				p_detail->save();
			}
			break;
		}
		case 2:{
			Campfire_Detail detail;
			detail.deserialize(buf);
			Campfire_Detail*p_detail = get_campfire_detail(detail.gang_id);
			if(p_detail){
				p_detail->firewood_num = detail.firewood_num;
				p_detail->level_firewood_num = detail.level_firewood_num;
				p_detail->level_max_firewood_num = detail.level_max_firewood_num;
				p_detail->burning = detail.burning;
				p_detail->level = detail.level;
				p_detail->gang_id = detail.gang_id;
				p_detail->gang_name = detail.gang_name;
				p_detail->id = detail.id;
				p_detail->detail_change();
				p_detail->save();
			}else{
				campfire_info_.insert(std::make_pair(detail.gang_id, detail));
				detail.detail_change();
				detail.save();
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Campfire_Manager::load_campfire(void){
	campfire_info_.clear();
	MONGO_OPERATOR->load_campfire(campfire_info_);
	return 0;
}

int Campfire_Manager::load_fb_info(void){
	fb_info_detail_map_.clear();
	return MONGO_OPERATOR->load_fb_info(fb_info_detail_map_);
}

int Campfire_Manager::campfire_ready(void){
	// 篝火晚会准备世界公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_READY);
	return 0;
}
int Campfire_Manager::campfire_start(void){
	Time_Value now = Time_Value::gettimeofday();
	int remain_time = active_status_.next_status_time.sec() - now.sec();
	int count = campfire_info_.size();
	Block_Buffer buf;
	buf.make_message(INNER_CAMPFIRE_SCENE_START);
	buf.write_int32(remain_time);
	buf.write_int32(count);
	Campfire_Detail_Map::iterator it;
	for(it = campfire_info_.begin(); it != campfire_info_.end(); ++it){
		it->second.id = 0;
		it->second.serialize(buf);
	}
	buf.finish_message();
	int dest_scene = 80214001;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	// 篝火晚会开启世界公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_START);
	system_announce(ANNOUNCE_DISTRICT_GANG, ANNOUNCE_ID_BONFIRE_START_1);
	act_id = now.sec();
	return 0;
}

int Campfire_Manager::campfire_finish(void){
	// 篝火晚会结束世界公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_FINISH);
	return 0;
}

Campfire_Detail* Campfire_Manager::get_campfire_detail(gang_id_t gang_id){
	Campfire_Detail_Map::iterator it = campfire_info_.find(gang_id);
	if(it != campfire_info_.end()){
		return &(it->second);
	}
	return NULL;
}

void Campfire_Manager::cal_active_time(const Time_Value& now, Active_Status& active_status){
	tm tm1;
	time_t t1 = now.sec();
	localtime_r(&t1, &tm1);
	int tm_hour = tm1.tm_hour;
	int tm_min = tm1.tm_min;
	int tm_sec = tm1.tm_sec;
	int now_time = tm_hour*3600+tm_min*60+tm_sec;
	int valid_status = 0;
	int valid_time = 0;
	int valid_act_time = 0;
	bool next_day = false;
	CONFIG_CACHE_SECRET_SHOP->cal_campfire_open_time(now_time, valid_status,
			valid_time, valid_act_time, next_day);

	active_status_.status = valid_status;
	active_status_.ready_time = 5*60;
	active_status_.act_time = valid_act_time;
	if(active_status_.status == 1){
		int dt_time = valid_time - active_status_.ready_time - now_time;
		if(next_day){
			dt_time += Time_Value::ONE_DAY_IN_SECS;
		}
		active_status_.next_status_time = now + Time_Value(dt_time);
	}else if(active_status_.status == 2){
		int dt_time = valid_time - now_time;
		if(next_day){
			dt_time += Time_Value::ONE_DAY_IN_SECS;
		}
		active_status_.next_status_time = now + Time_Value(dt_time);
	}else if(active_status_.status == 3){
		int dt_time = valid_time + active_status_.act_time - now_time;
		if(next_day){
			dt_time += Time_Value::ONE_DAY_IN_SECS;
		}
		active_status_.next_status_time = now + Time_Value(dt_time);
	}
}

void Campfire_Manager::system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id/* = 0*/, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/){
	std::vector<Chat_Content> ann_contents;
	Chat_Content new_content;
	if(str1 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str1;
		ann_contents.push_back(new_content);
	}
	if(str2 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str2;
		ann_contents.push_back(new_content);
	}
	if(str3 != ""){
		new_content.reset();
		new_content.type = ANN_CTTYPE_TEXT;
		new_content.text = str3;
		ann_contents.push_back(new_content);
	}

	MSG_20500000 msg;
	msg.district = chat_type;
	msg.announce_id = ann_id;
	msg.contents = ann_contents;
	msg.gang_id = gang_id;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}

int Campfire_Manager::bonfire_act_start(int time){
	if(active_status_.status == 3){// 已经开启不重新激活
		return -1;
	}
	if(time < 20){
		time = 20;
	}
	active_status_.status = 3;
	active_status_.ready_time = 5*60;
	active_status_.act_time = time;
	active_status_.next_status_time = Time_Value::gettimeofday() + Time_Value(time);
	campfire_start();
	for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
			it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
		if(it->second){
			it->second->send_bonfire_active_to_client();
		}
	}
	return 0;
}

int Campfire_Manager::bonfire_act_finish(void){
	if(active_status_.status != 3){//
		return -1;
	}
	return 0;
}

FB_Info_Detail* Campfire_Manager::get_fb_info_detail(int scene_id){
	int64_t id = scene_id;
	FB_Info_Detail_Map::iterator it = fb_info_detail_map_.find(id);
	if(it != fb_info_detail_map_.end()){
		return &(it->second);
	}
	return NULL;
}

void Campfire_Manager::set_fb_info_detail(int scene_id, role_id_t role_id, std::string role_name, int force){
	FB_Info_Detail info;
	info.role_id = scene_id;
	info.best_role_id = role_id;
	info.best_force = force;
	info.best_role_name = role_name;
	fb_info_detail_map_[info.role_id] = info;
	info.detail_change();
	info.save();
}

int Campfire_Manager::player_get_bonfire_award(Logic_Player* player,
		const MSG_20300113 &msg) {
	if (!player)
		return -1;
	Item_Vec detail_vec;

	std::vector<Item_Info>::const_iterator iter;
	for (iter = msg.items.begin(); iter != msg.items.end(); ++iter) {
		Item_Detail detail;
		detail.id_ = (*iter).id;
		detail.amount_ = (*iter).amount;
		detail.bind_ = (*iter).bind;
		detail_vec.push_back(detail);
	}

	Gain_Item_DM_Info dm_info(ITEM_GAIN_CAMPFIRE_CONSOLATION);
	switch (msg.type) {
	case 1:
		dm_info.type = ITEM_GAIN_CAMPFIRE_LIT;
		break;
	case 2:
		dm_info.type = ITEM_GAIN_CAMPFIRE_ADD_WOOD;
		break;
	case 3:
		dm_info.type = ITEM_GAIN_CAMPFIRE_CONSOLATION;
		break;
	case 4:
		dm_info.type = ITEM_GAIN_CAMPFIRE_RANK;
		break;
	default:
		break;
	}

	int result = player->pack_insert_item(PACK_T_PACKAGE_INDEX, detail_vec, dm_info);

	//飘字
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	for (Item_Vec::iterator item_it = detail_vec.begin(); item_it != detail_vec.end(); ++item_it) {
		Item_Basic_Info ibi;
		(*item_it).set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
		Card_Info card_info;
		if ((*item_it).set_item_card_info(card_info)) {
			acv_msg.card_info_vec.push_back(card_info);
		}
	}
	Block_Buffer buffer1;
	buffer1.make_message(acv_msg.msg_id);
	acv_msg.serialize(buffer1);
	buffer1.finish_message();
	player->send_to_client(buffer1);

	MSG_20300114 msg_res;
	msg_res.result = result;
	msg_res.type = msg.type;

	Block_Buffer buffer;
	buffer.make_message(msg_res.msg_id);
	msg_res.serialize(buffer);
	buffer.finish_message();
	player->send_to_scene(buffer);

	return 0;
}

int Campfire_Manager::load_player_info(Logic_Player* player) {
	if (!player)
			return -1;

	MSG_20300116 msg;
	msg.role_id = player->role_id();
	msg.finish_time = player->public_detail().campfire_finish_time;
	msg.rank = player->public_detail().campfire_rank;
	boost::unordered_set<int32_t>::const_iterator iter;
	for (iter = player->public_detail().killed_match_man.begin();
			iter != player->public_detail().killed_match_man.end(); ++iter) {
		msg.killed_match_man.push_back(*iter);
	}
	msg.is_lighter = player->public_detail().campfire_lighter;
	msg.recieve_stat = player->public_detail().campfire_recieve_stat;
	msg.rank_rev_stat = player->public_detail().campfire_rank_rev_stat;
	msg.add_firewood = player->public_detail().add_campfire;

	Block_Buffer buff;
	buff.make_message(msg.msg_id);
	msg.serialize(buff);
	buff.finish_message();

	player->send_to_scene(buff);
	return 0;
}

int Campfire_Manager::save_player_info(Logic_Player* player,
		const MSG_20300116& msg) {
	if (!player)
		return -1;

	return player->save_campfire_player_info(msg);
}

int Campfire_Manager::mail_to_player(Logic_Player* player,
		const MSG_20300117& msg) {
	if (!player)
		return -1;

	LOGIC_PUBLIC->send_mail(msg.mail_id, player->role_id(), msg.items);
	return 0;
}
