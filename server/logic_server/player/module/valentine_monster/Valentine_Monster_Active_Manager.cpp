/*
 * Valentine_Monster_Active_Manager.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: root
 */


#include "Valentine_Monster_Active_Manager.h"
#include "Config_Cache_Valentine_Monster.h"
#include "Config_Cache.h"
#include "Config_Cache_Scene.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "DB_Operator.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Inner_Enum.h"
#include "world_boss/World_Boss_Active_Manager.h"
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

const int valentine_monster_act_icon_id = 20;

Valentine_Monster_Active_Manager::Valentine_Monster_Active_Manager(void){
	reset();
}

Valentine_Monster_Active_Manager::~Valentine_Monster_Active_Manager(void){

}

void Valentine_Monster_Active_Manager::reset(void){
	main_scene_map_.clear();
	act_open_ = false;
	before_start_time_ = Time_Value::zero;
	start_time_ = Time_Value::zero;
	end_time_ = Time_Value::zero;
}

void Valentine_Monster_Active_Manager::start(void){
	init_main_scene();
}

void Valentine_Monster_Active_Manager::init_main_scene(void){
	Monitor_Mark_Vec monitor_id_order;
	const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
	server_solution_cache.fetch_muid_by_type(monitor_id_order, Server_Enum::MONITOR_SCENE);
	for (Monitor_Mark_Vec::iterator it = monitor_id_order.begin(); it != monitor_id_order.end(); ++it) {
		if(it->id/10000 == 8010){
			main_scene_map_.insert(std::make_pair(it->id, (*it)));
		}
	}
}

bool Valentine_Monster_Active_Manager::is_open(void){
	return act_open_;
}

void Valentine_Monster_Active_Manager::listen_open_active(Time_Value before_start_time, Time_Value start_time, Time_Value end_time){
	int remain_time = end_time.sec() - Time_Value::gettimeofday().sec();
	int monster_id = WORLD_BOSS_ACTIVE_MANAGER->get_boss_type_id();
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_START);
	buf.write_int32(valentine_monster_act_icon_id);
	buf.write_int64(0);
	buf.write_int32(remain_time);
	buf.write_int32(monster_id);
	int scene_amount = main_scene_map_.size();
	buf.write_int32(scene_amount);
	Main_Scene_Map::iterator it;
	for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
		it->second.serialize(buf);
	}
	buf.finish_message();

	for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
		int dest_scene = it->first;
		LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	}

	// 开启公告
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 8001);
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 8002);
	act_open_ = true;
}

void Valentine_Monster_Active_Manager::listen_close_active(void){
	act_open_ = false;
	Block_Buffer buf;
	buf.make_message(INNER_ACT_SCENE_END);
	buf.write_int32(valentine_monster_act_icon_id);
	buf.write_int64(0);
	buf.finish_message();
	Main_Scene_Map::iterator it;
	for(it = main_scene_map_.begin(); it != main_scene_map_.end(); ++it){
		int dest_scene = it->first;
		LOGIC_MONITOR->send_to_scene(dest_scene, 0, buf);
	}
	system_announce(ANNOUNCE_DISTRICT_SYSTEM, 8003);
}

void Valentine_Monster_Active_Manager::cmd_start_active(int value){
	Time_Value now = Time_Value::gettimeofday();
	ACTIVE_MANAGER->gm_open_active(ACTION_ACTIVE_NPC_RUNAWAY, now);
	return;
}

void Valentine_Monster_Active_Manager::cmd_end_active(void){
	listen_close_active();
}

int Valentine_Monster_Active_Manager::inner_sync_info(Logic_Player* player, Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 1:{// boss reward
			int player_num = 0;
			buf.read_int32(player_num);
			for(int i = 0; i < player_num; ++i){
				role_id_t role_id = 0;
				int8_t send_mail = 0;
				int item_num = 0;
				std::vector<Item_Detail> item_vec;
				Money_Add_List money_list;
				buf.read_int64(role_id);
				buf.read_int8(send_mail);
				buf.read_int32(item_num);
				for(int i = 0; i < item_num; ++i){
					int item_id = 0;
					int item_amount = 0;
					buf.read_int32(item_id);
					buf.read_int32(item_amount);
					if(item_id < 1000000){
						money_list.push_back(Money_Add_Info((Pack::Money_Type)item_id, item_amount,
								Money_DM_Info(MONEY_ADD_VALENTIEN_MONSTER_KILL_BOSS)));
					}else{
						item_vec.push_back(Item_Detail(item_id, item_amount, Item_Detail::BIND));
					}
				}
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
														item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_VALENTINE_MONSTER_KILL_BOSS));
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
					}else{// send_mail = 1
						std::string mail_sender = "system";
						std::string mail_title = "valentine monster";
						std::string mail_content = "valentine monster";
						const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(2012);
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
					std::string mail_title = "valentine monster";
					std::string mail_content = "valentine monster";
					const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(2012);
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
		default:{
			break;
		}
	}
	return 0;
}


void Valentine_Monster_Active_Manager::resource_to_item(std::vector<Money_Add_Info> money_list, std::vector<Item_Detail>& item_vec){
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

void Valentine_Monster_Active_Manager::system_announce(const int chat_type, const int ann_id, const gang_id_t gang_id/* = 0*/, std::string str1/* = ""*/, std::string str2/* = ""*/, std::string str3/* = ""*/){
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
