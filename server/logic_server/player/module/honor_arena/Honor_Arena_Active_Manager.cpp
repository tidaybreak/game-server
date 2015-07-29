/*
 * Honor_Arena_Active_Manager.cpp
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */


#include "Honor_Arena_Active_Manager.h"
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
#include "Config_Cache_Honor_Arena.h"
#include "Config_Cache_Mail.h"
#include "Config_Cache_Scene.h"
#include "active/Active_Manager.h"
#include "active/Active_Def.h"
#include "Msg_Active_Struct.h"
#include "title/Title.h"
#include "helper/Helper_Def.h"
#include "active/Active_Content_Manager.h"

const int honor_arena_act_icon_id = 22;
const int DEST_SCENE_ID = 80219001;
const int MERIT_ID = 503003;


Honor_Arena_Active_Manager::Honor_Arena_Active_Manager(void){
	reset();
}

Honor_Arena_Active_Manager::~Honor_Arena_Active_Manager(void){

}
void Honor_Arena_Active_Manager::reset(void){
	active_status_.reset();
	first_start_ = false;
	act_id_ = 0;
	enter_player_set_.clear();
	//
	act_open_ = false;
	icon_open_ = false;
	icon_ann_step_ = 0;
	before_start_time_ = Time_Value::zero;
	start_time_ = Time_Value::zero;
	end_time_ = Time_Value::zero;
}

int Honor_Arena_Active_Manager::start(void){
	return 0;
}

int Honor_Arena_Active_Manager::tick(const Time_Value& now){
	if(icon_open_){//
		int remain_sec = start_time_.sec() - now.sec();
		if(remain_sec > 240 && remain_sec <= 300){// 荣耀竞技场将在5分钟后开启
			if(icon_ann_step_ != 9008){
				icon_ann_step_ = 9008;
				system_announce(ANNOUNCE_DISTRICT_SYSTEM, icon_ann_step_);
			}
		}else if(remain_sec > 120 && remain_sec <= 180){// 荣耀竞技场将在3分钟后开启
			if(icon_ann_step_ != 9009){
				icon_ann_step_ = 9009;
				system_announce(ANNOUNCE_DISTRICT_SYSTEM, icon_ann_step_);
			}
		}else if(remain_sec > 0 && remain_sec <= 60){// 荣耀竞技场将在1分钟后开启
			if(icon_ann_step_ != 9010){
				icon_ann_step_ = 9010;
				system_announce(ANNOUNCE_DISTRICT_SYSTEM, icon_ann_step_);
			}
		}
	}
	return 0;
}

void Honor_Arena_Active_Manager::daily_refresh(void){

}

int Honor_Arena_Active_Manager::req_enter_scene(Logic_Player* player){
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
	int dest_scene = DEST_SCENE_ID;
	Coord coord(0, 0, 1);
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(
			dest_scene);
	if(scene_config){
		int coord_count = scene_config->transmit_coord.size();
		int coord_index = 0;
		if(coord_count > 0){
			coord_index = random()%coord_count;
		}
		int c_index = 0;
		Scene_Config::Transmit_Coord_Map::const_iterator coord_it;
		for(coord_it = scene_config->transmit_coord.begin(); coord_it != scene_config->transmit_coord.end(); ++coord_it){
			if(c_index == coord_index){
				coord = coord_it->second.coord;
				break;
			}
			c_index += 1;
		}
	}

	std::string arg3 = "";
	int64_t arg1 = 0;
	// sync to scene
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_PLAYER_ENTER);
	buf.write_int32(honor_arena_act_icon_id);
	buf.write_int64(player->gang_id());
	buf.write_int64(player->role_id());
	buf.write_string(player->role_name());
	int vip_lv = player->vip();
	buf.write_int32(vip_lv);
	buf.write_int32(player->career());
	buf.write_int32(player->get_ttl_force());
	buf.write_string(player->base_detail().gang_name);
	buf.finish_message();
	player->monitor()->send_to_scene(dest_scene, 0, buf);
	// enter scene
	int transmit_result = player->inner_transmit(dest_scene, 0, coord, arg1, arg3);
	if(transmit_result == 0){
//		// 任务
//		player->task_listen_world_boss();
//		if(enter_player_set_.count(player->role_id()) == 0){
//			//
//			enter_player_set_.insert(player->role_id());
//			player->daily_helper_listen(DAILY_FULFILL_LISTEN_WORLD_BOSS);
//		}
	}
	return transmit_result;
}

int Honor_Arena_Active_Manager::get_enter_level_limit(void){
	return CONFIG_CACHE_HONOR_ARENA->get_enter_lv_limit();
}

void Honor_Arena_Active_Manager::get_active_info(int& status, int& remain_time, int &party_id){
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

int Honor_Arena_Active_Manager::cmd_sync_active_info(Block_Buffer& buf){
	return 0;
}

int Honor_Arena_Active_Manager::active_ready(void){
	//准备世界公告
	//system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_READY);
	return 0;
}
int Honor_Arena_Active_Manager::active_start(void){
	Time_Value now = Time_Value::gettimeofday();
	int fight_time = active_status_.act_time;
	if(fight_time < 300){
		fight_time = 300;
	}
	int remain_time = active_status_.next_status_time.sec() - now.sec();
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_START);
	buf.write_int32(honor_arena_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(fight_time);
	buf.write_int32(remain_time);
	buf.finish_message();
	int dest_scene = DEST_SCENE_ID;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	// 开启公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 9011);
	act_id_ = now.sec();
	enter_player_set_.clear();
	return 0;
}

int Honor_Arena_Active_Manager::active_finish(void){
	// 结束世界公告
	//system_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BONFIRE_FINISH);
	return 0;
}

void Honor_Arena_Active_Manager::system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id/* = 0*/, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/){
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

void Honor_Arena_Active_Manager::resource_to_item(std::vector<Money_Add_Info> money_list, std::vector<Item_Detail>& item_vec){
	int pack_num = 5000;
	int item_amount = 0;
	int item_id = 0;
	std::vector<Money_Add_Info>::iterator it;
	for(it = money_list.begin(); it != money_list.end(); ++it){
		item_id = 0;
		if(it->type == COPPER){
			pack_num = 5000;
			item_id = 20701201;
		}else if(it->type == GOLD){
			item_id = 0;
		}else if(it->type == BIND_GOLD){
			item_id = 0;
		}else if(it->type == DRAGON_SOUL){
			pack_num = 5000;
			item_id = 20704301;
		}else if(it->type == SOULS){
			pack_num = 5000;
			item_id = 20703201;
		}else if(it->type == FRIENDSHIP){
			item_id = 0;
		}else if(it->type == MERIT_ID){
			pack_num = 50;
			item_id = 20706401;
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

int Honor_Arena_Active_Manager::cmd_start_active(int time){
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
//	Time_Value now = Time_Value::gettimeofday();
//	ACTIVE_MANAGER->gm_open_active(ACTION_ACTIVE_WORLD_BOSS, now);

	Time_Value now = Time_Value::gettimeofday();
	Time_Value end = now + Time_Value(time);
	listen_open_active(now , now , end);
	return 0;
}

int Honor_Arena_Active_Manager::cmd_finish_active(void){
	if(active_status_.status != 3){//
		return -1;
	}
	int remain_time = 2;
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_END);
	buf.write_int32(honor_arena_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(remain_time);
	buf.finish_message();
	int dest_scene = DEST_SCENE_ID;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	return 0;
}

bool Honor_Arena_Active_Manager::is_open(void){
	return act_open_;
}

void Honor_Arena_Active_Manager::listen_open_active(Time_Value before_start_time, Time_Value start_time, Time_Value end_time){
	int fight_time = end_time.sec() - start_time.sec();
	int remain_time = end_time.sec() - Time_Value::gettimeofday().sec();
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_START);
	buf.write_int32(honor_arena_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(fight_time);
	buf.write_int32(remain_time);
	buf.finish_message();
	int dest_scene = DEST_SCENE_ID;
	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	// 开启公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 9011);
	act_open_ = true;
	Time_Value now = Time_Value::gettimeofday();
	act_id_ = now.sec();
	enter_player_set_.clear();
}

void Honor_Arena_Active_Manager::listen_close_active(void){
//	act_open_ = false;
//	int remain_time = 2;
//	Block_Buffer buf;
//	buf.make_message(INNER_ACT_SCENE_END);
//	buf.write_int32(honor_arena_act_icon_id);
//	buf.write_int64(0);
//	buf.write_int32(remain_time);
//	buf.finish_message();
//	int dest_scene = DEST_SCENE_ID;
//	LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
}

void Honor_Arena_Active_Manager::listen_open_active_icon(Time_Value before_start_time, Time_Value start_time, Time_Value end_time){
	icon_open_ = true;
	icon_ann_step_ = 0;
	before_start_time_ = before_start_time;
	start_time_ = start_time;
	end_time_ = end_time;
	int remain_sec = start_time_.sec() - Time_Value::gettimeofday().sec();
	if(remain_sec > 240 && remain_sec <= 300){// 荣耀竞技场将在5分钟后开启
		icon_ann_step_ = 9008;
	}else if(remain_sec > 120 && remain_sec <= 180){// 荣耀竞技场将在3分钟后开启
		icon_ann_step_ = 9009;
	}else if(remain_sec > 0 && remain_sec <= 60){// 荣耀竞技场将在1分钟后开启
		icon_ann_step_ = 9010;
	}
	if(icon_ann_step_ != 0){
		system_announce(ANNOUNCE_DISTRICT_SYSTEM, icon_ann_step_);
	}
}

void Honor_Arena_Active_Manager::listen_close_active_icon(void){
	icon_open_ = false;
	icon_ann_step_ = 0;
}

int Honor_Arena_Active_Manager::inner_sync_info(Logic_Player* player, Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 1:{// 结算奖励
			int act_player_num = 0;
			buf.read_int32(act_player_num);
			for(int i = 0; i < act_player_num; ++i){
				role_id_t role_id = 0;
				int rank = 0;
				int8_t send_mail = 0;
				int item_num = 0;
				std::vector<Item_Detail> item_vec;
				Money_Add_List money_list;
				buf.read_int64(role_id);
				buf.read_int32(rank);
				buf.read_int8(send_mail);
				buf.read_int32(item_num);
				for(int k = 0; k < item_num; ++k){
					int item_id = 0;
					int item_amount = 0;
					buf.read_int32(item_id);
					buf.read_int32(item_amount);
					if(item_id < 1000000){
						money_list.push_back(Money_Add_Info((Pack::Money_Type)item_id, item_amount, Money_DM_Info(MONEY_ADD_HONOR_ARENA_RANK)));
					}else{
						item_vec.push_back(Item_Detail(item_id, item_amount, Item_Detail::BIND));
					}
				}
				//
				Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
				if(player){
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
														item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_HONOR_ARENA_RANK));
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
						std::string mail_title = "honor arena";
						std::string mail_content = "honor arena reward rank:%d";
						const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1047);
						if(cfg_mail){
							mail_sender = cfg_mail->sender_name;
							mail_title = cfg_mail->mail_title;
							mail_content = cfg_mail->mail_content;
						}
						std::stringstream mail_stream;
						char text_content[256] = {'\0'};
						snprintf(text_content, sizeof(text_content), mail_content.c_str(), rank);
						mail_stream<<text_content;

						Mail_Send_Info send_info;
						send_info.sender_id = 0;
						send_info.sender_name = mail_sender;
						send_info.send_type = MAIL_TYPE_SYS;
						send_info.show_send_time = Time_Value::gettimeofday();
						send_info.receiver_id = role_id;
						send_info.title = mail_title;
						send_info.content = mail_stream.str();
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
					std::string mail_title = "honor arena";
					std::string mail_content = "honor arena reward rank:%d";
					const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1047);
					if(cfg_mail){
						mail_sender = cfg_mail->sender_name;
						mail_title = cfg_mail->mail_title;
						mail_content = cfg_mail->mail_content;
					}
					std::stringstream mail_stream;
					char text_content[256] = {'\0'};
					snprintf(text_content, sizeof(text_content), mail_content.c_str(), rank);
					mail_stream<<text_content;

					Mail_Send_Info send_info;
					send_info.sender_id = 0;
					send_info.sender_name = mail_sender;
					send_info.send_type = MAIL_TYPE_SYS;
					send_info.show_send_time = Time_Value::gettimeofday();
					send_info.receiver_id = role_id;
					send_info.title = mail_title;
					send_info.content = mail_stream.str();
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
		case 2:{// 荣誉竞技场界面展示奖励（首胜、十战、连胜）
			role_id_t fb_role_id = 0;
			int show_type = 0;
			int item_num = 0;
			std::vector<Item_Detail> item_vec;
			Money_Add_List money_list;
			buf.read_int64(fb_role_id);
			buf.read_int32(show_type);
			buf.read_int32(item_num);
			for(int i = 0; i < item_num; ++i){
				int item_id = 0;
				int item_amount = 0;
				buf.read_int32(item_id);
				buf.read_int32(item_amount);
				if(item_id < 1000000){
					money_list.push_back(Money_Add_Info((Pack::Money_Type)item_id, item_amount, Money_DM_Info(MONEY_ADD_HONOR_ARENA_SHOW)));
				}else{
					item_vec.push_back(Item_Detail(item_id, item_amount, Item_Detail::BIND));
				}
			}
			Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(fb_role_id);
			if(player){
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
												item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_HONOR_ARENA_SHOW));
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
				std::string mail_title = "honor arena";
				std::string mail_content = "honor arena reward";
//				const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1109);
//				if(cfg_mail){
//					mail_sender = cfg_mail->sender_name;
//					mail_title = cfg_mail->mail_title;
//					mail_content = cfg_mail->mail_content;
//				}
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
		case 3:{// 每场战斗奖励
			role_id_t lose_role_id = 0;
			int lose_copper = 0;
			int lose_merit = 0;
			role_id_t win_role_id = 0;
			int win_copper = 0;
			int win_merit = 0;
			buf.read_int64(lose_role_id);
			buf.read_int32(lose_copper);
			buf.read_int32(lose_merit);
			buf.read_int64(win_role_id);
			buf.read_int32(win_copper);
			buf.read_int32(win_merit);

			Logic_Player* lose_player = LOGIC_MONITOR->find_logic_player_by_role_id(lose_role_id);
			if(lose_player){
				 MSG_81000102 res_msg;
				 res_msg.type = 0;
				 if(lose_copper > 0){
					 int add_result = lose_player->pack_add_money(Money_Add_Info(Pack::COPPER,lose_copper, MONEY_ADD_HONOR_ARENA_FIGHT));
					 if(add_result == 0){
						 res_msg.property.push_back(Property(COPPER, lose_copper));
					 }
				 }
				 if(lose_merit > 0){
					 lose_player->change_exploit_val(lose_merit);
					 res_msg.property.push_back(Property(MERIT_ID, lose_merit));
				 }
				 if(res_msg.property.empty() == false){
					 OBJ_SEND_TO_CLIENT(res_msg, (*lose_player));
				 }
			}

			Logic_Player* win_player = LOGIC_MONITOR->find_logic_player_by_role_id(win_role_id);
			if(win_player){
				 MSG_81000102 res_msg;
				 res_msg.type = 0;
				 if(win_copper > 0){
					 int add_result = win_player->pack_add_money(Money_Add_Info(Pack::COPPER,win_copper, MONEY_ADD_HONOR_ARENA_FIGHT));
					 if(add_result == 0){
						 res_msg.property.push_back(Property(COPPER, win_copper));
					 }
				 }
				 if(win_merit > 0){
					 win_player->change_exploit_val(win_merit);
					 res_msg.property.push_back(Property(MERIT_ID, win_merit));
				 }
				 if(res_msg.property.empty() == false){
					 OBJ_SEND_TO_CLIENT(res_msg, (*win_player));
				 }
			}
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Honor_Arena_Active_Manager::inner_act_end(Block_Buffer& buf){
	return 0;
}



