/*
 * World_Boss_Active_Manager.cpp
 *
 *  Created on: 2015年1月14日
 *      Author: root
 */


#include "World_Boss_Active_Manager.h"
#include "Config_Cache_World_Boss.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "DB_Operator.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Err_Code.h"
#include "announcement/Announcement_Struct.h"
#include "mail/Mail_Struct.h"
#include "pack/Packer.h"
#include "pack/Pack_Struct.h"
#include "Config_Cache_Mail.h"
#include "active/Active_Manager.h"
#include "active/Active_Def.h"
#include "Msg_Active_Struct.h"
#include "title/Title.h"
#include "helper/Helper_Def.h"
#include "active/Active_Content_Manager.h"
#include "oper_activity/Oper_Activity_Def.h"

const int world_boss_act_icon_id = 16;


World_Boss_Active_Manager::World_Boss_Active_Manager(void):first_start_(false), act_id_(0),
		act_open_(false), icon_open_(false){

}

World_Boss_Active_Manager::~World_Boss_Active_Manager(void){

}

int World_Boss_Active_Manager::start(void){
	load_active_info();
//	Time_Value now = Time_Value::gettimeofday();
//	cal_active_time(now, active_status_);
//	first_start_ = true;
	return 0;
}

int World_Boss_Active_Manager::tick(const Time_Value& now){
	return 0;
	if(first_start_){
		first_start_ = false;
		if(active_status_.status == 2){
			active_ready();
		}else if(active_status_.status == 3){
			active_start();
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
		}else if(active_status_.status == 3){// 开启 ----> 关闭
			cal_active_time(now, active_status_);
		}
		for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
			if(it->second){
				it->second->send_act_scene_to_client(world_boss_act_icon_id);
			}
		}
		//activate to scene
		if(active_status_.status == 3){
			active_start();
		}else if(active_status_.status == 1){
			active_finish();
		}else if(active_status_.status == 2){
			active_ready();
		}
	}
	return 0;
}

void World_Boss_Active_Manager::daily_refresh(void){

}

int World_Boss_Active_Manager::req_enter_scene(Logic_Player* player){
	if(!player){
		return 0;
	}
	int level_limit = get_enter_level_limit();
	if (player->level() < level_limit) {
		return ERROR_WB_ROLE_LV_LIMIT;
	}
	if (!act_open_) {
		return ERROR_BONFIRE_NO_START;
	}
	int dest_scene = 80209001;
	Coord coord(0, 0, 1);
	std::string arg3 = "";
	int64_t arg1 = 0;
	// sync to scene
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_PLAYER_ENTER);
	buf.write_int32(world_boss_act_icon_id);
	buf.write_int64(player->gang_id());
	buf.write_int64(player->role_id());
	buf.write_string(player->role_name());
	int vip_lv = player->vip();
	buf.write_int32(vip_lv);
	buf.write_int32(player->career());
	buf.finish_message();
	player->monitor()->send_to_scene(dest_scene, 0, buf);
	// enter scene
	int transmit_result = player->inner_transmit(dest_scene, 0, coord, arg1, arg3);
	if(transmit_result == 0){
		// 任务
		player->task_listen_world_boss();
		if(enter_player_set_.count(player->role_id()) == 0){
			//
			enter_player_set_.insert(player->role_id());
			player->daily_helper_listen(DAILY_FULFILL_LISTEN_WORLD_BOSS);
		}
	}
	return transmit_result;
}

int World_Boss_Active_Manager::get_enter_level_limit(void){
	return CONFIG_CACHE_WORLD_BOSS->get_enter_lv_limit(world_boss_act_icon_id);
}

void World_Boss_Active_Manager::get_active_info(int& status, int& remain_time, int &party_id){
	if(icon_open_){
		status = 0;
		remain_time = 0;

	}else{

	}
	Time_Value now = Time_Value::gettimeofday();
	status = active_status_.status;
	remain_time = active_status_.next_status_time.sec() - now.sec();
	party_id = act_id_;
}

int World_Boss_Active_Manager::cmd_sync_active_info(Block_Buffer& buf){
	return 0;
}

int World_Boss_Active_Manager::load_active_info(void){
	active_info_.reset();
	MONGO_OPERATOR->load_world_boss_detail(active_info_);
	if(active_info_.boss_lv == 0){
		active_info_.boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(
				active_info_.kill_num, world_boss_act_icon_id);
	}
	return 0;
}

int World_Boss_Active_Manager::active_ready(void){
	//准备世界公告
	//system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_READY);
	return 0;
}
int World_Boss_Active_Manager::active_start(void){
	Time_Value now = Time_Value::gettimeofday();
	int fight_time = active_status_.act_time - 2*60;
	if(fight_time < 300){
		fight_time = 300;
	}
	int remain_time = active_status_.next_status_time.sec() - now.sec();
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_START);
	buf.write_int32(world_boss_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(fight_time);
	buf.write_int32(remain_time);
	active_info_.serialize(buf);
	buf.finish_message();
	int dest_scene = 80209001;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	// 开启世界公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 7007);
	act_id_ = now.sec();
	enter_player_set_.clear();
	return 0;
}

int World_Boss_Active_Manager::active_finish(void){
	// 结束世界公告
	//system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_FINISH);
	return 0;
}

void World_Boss_Active_Manager::cal_active_time(const Time_Value& now, Active_Status& active_status){
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
	CONFIG_CACHE_WORLD_BOSS->cal_active_time(now_time, valid_status,
			valid_time, valid_act_time, next_day);

	active_status_.act_finish = false;
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

void World_Boss_Active_Manager::system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id/* = 0*/, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/){
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

void World_Boss_Active_Manager::resource_to_item(std::vector<Money_Add_Info> money_list, std::vector<Item_Detail>& item_vec){
	int pack_num = 5000;
	int item_amount = 0;
	int item_id = 0;
	std::vector<Money_Add_Info>::iterator it;
	for(it = money_list.begin(); it != money_list.end(); ++it){
		item_id = 0;
		if(it->type == COPPER){
			item_id = 20701201;
		}else if(it->type == GOLD){
			item_id = 0;
		}else if(it->type == BIND_GOLD){
			item_id = 0;
		}else if(it->type == DRAGON_SOUL){
			item_id = 20704301;
		}else if(it->type == SOULS){
			item_id = 20703201;
		}else if(it->type == FRIENDSHIP){
			item_id = 0;
		}
		if(item_id != 0){
			item_amount = it->nums/pack_num;
			if(item_amount <= 0){
				item_amount = 1;
			}
			item_vec.push_back(Item_Detail(item_id, item_amount, Item_Detail::BIND));
		}
	}
}

int World_Boss_Active_Manager::cmd_start_active(int time){
//	if(active_status_.status == 3 && active_status_.act_finish == false){// 已经开启不重新激活
//		return -1;
//	}
//	if(time < 60){
//		time = 60;
//	}
//	if(time > 1400){
//		time = 1400;
//	}
//	active_status_.act_finish = false;
//	active_status_.status = 3;
//	active_status_.ready_time = 5*60;
//	active_status_.act_time = 20*60;
//	active_status_.next_status_time = Time_Value::gettimeofday() + Time_Value(time);
//	active_start();
//	for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
//			it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
//		if(it->second){
//			it->second->send_act_scene_to_client(world_boss_act_icon_id);
//		}
//	}
	Time_Value now = Time_Value::gettimeofday();
	ACTIVE_MANAGER->gm_open_active(ACTION_ACTIVE_WORLD_BOSS, now);
	return 0;
}

int World_Boss_Active_Manager::cmd_finish_active(void){
	if(active_status_.status != 3){//
		return -1;
	}
	int remain_time = 2;
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_END);
	buf.write_int32(world_boss_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(remain_time);
	active_info_.serialize(buf);
	buf.finish_message();
	int dest_scene = 80209001;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	return 0;
}

int World_Boss_Active_Manager::cmd_set_world_boss_lv(const int lv){
	if(lv <= 0){
		return -1;
	}
	if(act_open_){//
		return -1;
	}
	active_info_.kill_num = CONFIG_CACHE_WORLD_BOSS->get_boss_kill_num_from_lv(lv, world_boss_act_icon_id);
	active_info_.boss_lv = lv;
	return 0;
}

int World_Boss_Active_Manager::cmd_set_world_boss_kill_num(const int kill_num){
	if(act_open_){//
		return -1;
	}
	active_info_.kill_num = kill_num;
	active_info_.boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(kill_num, world_boss_act_icon_id);;
	return 0;
}

int World_Boss_Active_Manager::get_boss_type_id(void){
	int kill_num = active_info_.kill_num;
	if(kill_num != 0){
		kill_num -= kill_num/3;
	}
	if(kill_num < 0){
		kill_num = 0;
	}
	int cfg_boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(kill_num, world_boss_act_icon_id);
	const World_Boss_Level_Config* cfg_lv = CONFIG_CACHE_WORLD_BOSS->get_level_config_from_lv(cfg_boss_lv, world_boss_act_icon_id);
	if(!cfg_lv){
		return 0;
	}
	return cfg_lv->monster_id;
}

bool World_Boss_Active_Manager::is_open(void){
	return act_open_;
}

void World_Boss_Active_Manager::listen_open_active(Time_Value before_start_time, Time_Value start_time, Time_Value end_time){
	int fight_time = end_time.sec() - start_time.sec();
	int remain_time = end_time.sec() - Time_Value::gettimeofday().sec();
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_START);
	buf.write_int32(world_boss_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(fight_time);
	buf.write_int32(remain_time);
	active_info_.serialize(buf);
	buf.finish_message();
	int dest_scene = 80209001;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	// 开启世界公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 7007);
	act_open_ = true;
	Time_Value now = Time_Value::gettimeofday();
	act_id_ = now.sec();
	enter_player_set_.clear();
}

void World_Boss_Active_Manager::listen_close_active(void){
	act_open_ = false;
}

void World_Boss_Active_Manager::listen_open_active_icon(Time_Value before_start_time, Time_Value start_time, Time_Value end_time){
	icon_open_ = true;
	before_start_time_ = before_start_time;
	start_time_ = start_time;
	end_time_ = end_time;
}

void World_Boss_Active_Manager::listen_close_active_icon(void){
	icon_open_ = false;
}

int World_Boss_Active_Manager::inner_sync_info(Logic_Player* player, Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 1:{// 结算奖励
			int act_player_num = 0;
			buf.read_int32(act_player_num);
			for(int i = 0; i < act_player_num; ++i){
				role_id_t role_id = 0;
				int damage = 0;
				double damage_rate = 0;		// 伤害百分比
				int rank = 0;
				int8_t send_mail = 0;
				int item_num = 0;
				std::vector<Item_Detail> item_vec;
				Money_Add_List money_list;
				buf.read_int64(role_id);
				buf.read_int32(damage);
				buf.read_double(damage_rate);
				buf.read_int32(rank);
				buf.read_int8(send_mail);
				buf.read_int32(item_num);
				for(int k = 0; k < item_num; ++k){
					int item_id = 0;
					int item_amount = 0;
					buf.read_int32(item_id);
					buf.read_int32(item_amount);
					if(item_id < 1000000){
						money_list.push_back(Money_Add_Info((Pack::Money_Type)item_id, item_amount, Money_DM_Info(MONEY_ADD_WORLD_BOSS)));
					}else{
						item_vec.push_back(Item_Detail(item_id, item_amount, Item_Detail::BIND));
					}
				}
				ACTIVE_CONTENT_MANAGER->active_content_listen_world_boss_end_info(role_id, damage, rank, damage_rate*100);
				Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
				if(player){
					player->oper_activity_listen_type_common_re(OPER_ACTIVITY_ITEM_WORLD_BOSS_PER, damage_rate*100);
					if(send_mail == 0){
						MSG_81000102 res_msg;
						res_msg.type = 0;
						if(money_list.empty() == false){
							player->pack_add_money(money_list, MONEY_OPT_REAL);
							for (Money_Add_List::iterator money_it = money_list.begin(); money_it != money_list.end();
									++money_it) {
								res_msg.property.push_back(Property((*money_it).type, (*money_it).nums));
							}
						}
						if(item_vec.empty() == false){
							int pack_result = player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
														item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_WORLD_BOSS));
							if(pack_result == 0){
								Item_Basic_Info item_base;
								Card_Info card_info;
								for (Item_Vec::iterator item_it = item_vec.begin(); item_it != item_vec.end(); ++item_it) {
									if (item_it->type_ == Item_Detail::CARD && item_it->amount_ > 1) {
										for (int i = 0; i < item_it->amount_; ++i) {
											item_base.reset();
											(*item_it).set_item_basic_info(item_base);
											item_base.amount = 1;
											res_msg.item_info.push_back(item_base);
											card_info.reset();
											if ((*item_it).set_item_card_info(card_info)) {
												res_msg.card_info_vec.push_back(card_info);
											}
										}
									} else {
										item_base.reset();
										(*item_it).set_item_basic_info(item_base);
										res_msg.item_info.push_back(item_base);
										card_info.reset();
										if ((*item_it).set_item_card_info(card_info)) {
											res_msg.card_info_vec.push_back(card_info);
										}
									}
								}
							}
						}
						//
						OBJ_SEND_TO_CLIENT(res_msg, (*player));
					}else{
						std::string mail_sender = "system";
						std::string mail_title = "world boss";
						std::string mail_content = "world boss reward";
						const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1108);
						if(cfg_mail){
							mail_sender = cfg_mail->sender_name;
							mail_title = cfg_mail->mail_title;
							mail_content = cfg_mail->mail_content;
						}
						Mail_Send_Info send_info;
						send_info.sender_id = 0;
						send_info.sender_name = mail_sender;
						send_info.send_type = MAIL_TYPE_SYS;
						send_info.show_send_time = Time_Value::gettimeofday();
						send_info.receiver_id = role_id;
						send_info.title = mail_title;
						send_info.content = mail_content;
						send_info.item_vector = item_vec;
						if(money_list.empty() == false){// 资源转5000的资源包
							std::vector<Item_Detail> rc_item_vec;
							resource_to_item(money_list, rc_item_vec);
							std::vector<Item_Detail>::iterator it;
							for(it = rc_item_vec.begin(); it != rc_item_vec.end(); ++it){
								send_info.item_vector.push_back(*it);
							}
						}
						player->send_mail_to_self(send_info);
					}
				}else{//
					std::string mail_sender = "system";
					std::string mail_title = "world boss";
					std::string mail_content = "world boss reward";
					const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1108);
					if(cfg_mail){
						mail_sender = cfg_mail->sender_name;
						mail_title = cfg_mail->mail_title;
						mail_content = cfg_mail->mail_content;
					}
					Mail_Send_Info send_info;
					send_info.sender_id = 0;
					send_info.sender_name = mail_sender;
					send_info.send_type = MAIL_TYPE_SYS;
					send_info.show_send_time = Time_Value::gettimeofday();
					send_info.receiver_id = role_id;
					send_info.title = mail_title;
					send_info.content = mail_content;
					send_info.item_vector = item_vec;
					if(money_list.empty() == false){// 资源转5000的资源包
						std::vector<Item_Detail> rc_item_vec;
						resource_to_item(money_list, rc_item_vec);
						std::vector<Item_Detail>::iterator it;
						for(it = rc_item_vec.begin(); it != rc_item_vec.end(); ++it){
							send_info.item_vector.push_back(*it);
						}
					}
					MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
				}
			}
			break;
		}
		case 2:{// 首杀奖励
			role_id_t fb_role_id = 0;
			int item_num = 0;
			std::vector<Item_Detail> item_vec;
			Money_Add_List money_list;
			buf.read_int64(fb_role_id);
			buf.read_int32(item_num);
			for(int i = 0; i < item_num; ++i){
				int item_id = 0;
				int item_amount = 0;
				buf.read_int32(item_id);
				buf.read_int32(item_amount);
				if(item_id < 1000000){
					money_list.push_back(Money_Add_Info((Pack::Money_Type)item_id, item_amount, Money_DM_Info(MONEY_ADD_WORLD_BOSS_FB)));
				}else{
					item_vec.push_back(Item_Detail(item_id, item_amount, Item_Detail::BIND));
				}
			}
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(fb_role_id);
			if(player){
				player->achievement_listen_first_boss_against();

				MSG_81000102 res_msg;
				res_msg.type = 0;
				if(money_list.empty() == false){
					player->pack_add_money(money_list, MONEY_OPT_REAL);
					for (Money_Add_List::iterator money_it = money_list.begin(); money_it != money_list.end();
							++money_it) {
						res_msg.property.push_back(Property((*money_it).type, (*money_it).nums));
					}
				}
				if(item_vec.empty() == false){
					int pack_result = player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
												item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_WORLD_BOSS_FB));
					if(pack_result == 0){
						Item_Basic_Info item_base;
						Card_Info card_info;
						for (Item_Vec::iterator item_it = item_vec.begin(); item_it != item_vec.end(); ++item_it) {
							if (item_it->type_ == Item_Detail::CARD && item_it->amount_ > 1) {
								for (int i = 0; i < item_it->amount_; ++i) {
									item_base.reset();
									(*item_it).set_item_basic_info(item_base);
									item_base.amount = 1;
									res_msg.item_info.push_back(item_base);
									card_info.reset();
									if ((*item_it).set_item_card_info(card_info)) {
										res_msg.card_info_vec.push_back(card_info);
									}
								}
							} else {
								item_base.reset();
								(*item_it).set_item_basic_info(item_base);
								res_msg.item_info.push_back(item_base);
								card_info.reset();
								if ((*item_it).set_item_card_info(card_info)) {
									res_msg.card_info_vec.push_back(card_info);
								}
							}
						}
					}
				}
				OBJ_SEND_TO_CLIENT(res_msg, (*player));
			}else{//
				std::string mail_sender = "system";
				std::string mail_title = "world boss";
				std::string mail_content = "world boss first blood reward";
				const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1109);
				if(cfg_mail){
					mail_sender = cfg_mail->sender_name;
					mail_title = cfg_mail->mail_title;
					mail_content = cfg_mail->mail_content;
				}
				Mail_Send_Info send_info;
				send_info.sender_id = 0;
				send_info.sender_name = mail_sender;
				send_info.send_type = MAIL_TYPE_SYS;
				send_info.show_send_time = Time_Value::gettimeofday();
				send_info.receiver_id = fb_role_id;
				send_info.title = mail_title;
				send_info.content = mail_content;
				send_info.item_vector = item_vec;
				if(money_list.empty() == false){// 资源转5000的资源包
					std::vector<Item_Detail> rc_item_vec;
					resource_to_item(money_list, rc_item_vec);
					std::vector<Item_Detail>::iterator it;
					for(it = rc_item_vec.begin(); it != rc_item_vec.end(); ++it){
						send_info.item_vector.push_back(*it);
					}
				}
				MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);

			}
			break;
		}
		case 3:{// 每场战斗金币奖励
			role_id_t ft_role_id = 0;
			int copper_num = 0;
			buf.read_int64(ft_role_id);
			buf.read_int32(copper_num);
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(ft_role_id);
			if(player && copper_num > 0){
				int add_result = player->pack_add_money(Money_Add_Info(Pack::COPPER,copper_num, MONEY_ADD_WORLD_BOSS_FT));
				if(add_result == 0){
					MSG_81000102 res_msg;
					res_msg.type = 0;
					res_msg.property.push_back(Property(COPPER, copper_num));
					OBJ_SEND_TO_CLIENT(res_msg, (*player));
				}
			}
			break;
		}
		case 4:{// 击杀boss
			role_id_t ft_role_id = 0;
			buf.read_int64(ft_role_id);
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(ft_role_id);
			if(player){
				player->title_listen_boss();
			}
			break;
		}
		case 10:{// 公会祝福价格--钻石
			int cost_glod = 0;
			buf.read_int32(cost_glod);
			if(player && cost_glod > 0 ){
				int sub_result = player->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), cost_glod, MONEY_SUB_WORLD_BOSS_BLESS));
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(world_boss_act_icon_id);
				buf.write_int8(50);
				buf.write_int32(sub_result);
				buf.finish_message();
				player->send_to_scene(buf);
			}
			break;
		}
		case 11:{
			int cost_glod = 0;
			buf.read_int32(cost_glod);
			if(player && cost_glod > 0 ){
				Money_Sub_Type cost_type = GOLD_ONLY;
				int cost_tp = CONFIG_CACHE_WORLD_BOSS->get_ability1_cost_type(world_boss_act_icon_id);
				if(cost_tp == 1){
					cost_type = BIND_GOLD_FIRST;
				}
				int sub_result = player->pack_sub_money(Money_Sub_Info(Money_Sub_Type(cost_type), cost_glod, MONEY_SUB_WORLD_BOSS_REBORN));
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(world_boss_act_icon_id);
				buf.write_int8(51);
				buf.write_int32(sub_result);
				buf.finish_message();
				player->send_to_scene(buf);
				if(sub_result == 0){
					player->task_listen_world_boss_immediate_revive();
				}
			}
			break;
		}
		case 12:{
			int cost_glod = 0;
			buf.read_int32(cost_glod);
			if(player && cost_glod > 0 ){
				Money_Sub_Type cost_type = GOLD_ONLY;
				int cost_tp = CONFIG_CACHE_WORLD_BOSS->get_ability2_cost_type(world_boss_act_icon_id);
				if(cost_tp == 1){
					cost_type = BIND_GOLD_FIRST;
				}
				int sub_result = player->pack_sub_money(Money_Sub_Info(Money_Sub_Type(cost_type), cost_glod, MONEY_SUB_WORLD_BOSS_POWER_UP));
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(world_boss_act_icon_id);
				buf.write_int8(52);
				buf.write_int32(sub_result);
				buf.finish_message();
				player->send_to_scene(buf);
			}
			break;
		}
		case 13:{
			int cost_glod = 0;
			buf.read_int32(cost_glod);
			if(player && cost_glod > 0 ){
				Money_Sub_Type cost_type = GOLD_ONLY;
				int cost_tp = CONFIG_CACHE_WORLD_BOSS->get_ability3_cost_type(world_boss_act_icon_id);
				if(cost_tp == 1){
					cost_type = BIND_GOLD_FIRST;
				}
				int sub_result = player->pack_sub_money(Money_Sub_Info(Money_Sub_Type(cost_type), cost_glod, MONEY_SUB_WORLD_BOSS_POWER_UP));
				Block_Buffer buf;
				buf.make_message(INNER_ACT_SCENE_SYNC_INFO);
				buf.write_int32(world_boss_act_icon_id);
				buf.write_int8(53);
				buf.write_int32(sub_result);
				buf.finish_message();
				player->send_to_scene(buf);
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int World_Boss_Active_Manager::inner_act_end(Block_Buffer& buf){
	role_id_t gang_id = 0;
	int result = 0;
	buf.read_int64(gang_id);
	buf.read_int32(result);
	if(result != 0){// 0：击杀boss失败， 1：普遍击杀boss 2：快速击杀boss
		if(result == 1){
			active_info_.kill_num += 1;
			active_info_.boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(
					active_info_.kill_num, world_boss_act_icon_id);
		}else{
			active_info_.boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(
					active_info_.kill_num, world_boss_act_icon_id);
			int max_boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_max_lv(world_boss_act_icon_id);
			if(active_info_.boss_lv < max_boss_lv){
				active_info_.boss_lv += 1;
				active_info_.kill_num = CONFIG_CACHE_WORLD_BOSS->get_boss_kill_num_from_lv(
						active_info_.boss_lv, world_boss_act_icon_id);
			}else{
				active_info_.kill_num += 1;
			}
		}
		active_status_.act_finish = true;
		for(Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = LOGIC_MONITOR->player_role_id_map().begin();
				it != LOGIC_MONITOR->player_role_id_map().end(); ++it) {
			if(it->second){
				it->second->send_act_scene_to_client(world_boss_act_icon_id);
			}
		}
	}
	active_info_.open_num += 1;
	active_info_.detail_change();
	active_info_.save();
	Time_Value now = Time_Value::gettimeofday();
	ACTIVE_MANAGER->inner_close_active(ACTION_ACTIVE_WORLD_BOSS, now);
	act_open_ = false;
	return 0;
}

