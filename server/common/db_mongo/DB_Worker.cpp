/*
 * DB_Worker.cpp
 *
 *  Created on: Sep 6, 2012
 *      Author: Ti
 */

#include "DB_Worker.h"
#include "DB_Manager.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "Logic_Monitor.h"
#include "Chat_Monitor.h"
#include "Logic_Player_Detail.h"
#include "operating/Operating_Def.h"
#include "chater/Chater_Def.h"
#include "DB_Def.h"
#include "ranking/Ranking_Def.h"
#include "ranking/Ranking_Struct.h"
#include "ranking/Ranking_Manager.h"
#include "reward/Reward_Def.h"
#include "dragon/Dragon_Vale_Struct.h"
#include "campfire/Campfire_Struct.h"
#include "world_boss/World_Boss_Struct.h"
#include "active/Active_Content_Def.h"
#include "auction/Auction_Def.h"
#include "auction/Auction_Struct.h"
#include "invest_plan/Invest_Plan_Def.h"
#include "invest_plan/Invest_Plan_Manager.h"
#include "thorlottery/ThorLottery_Struct.h"
#include "Msg_Inner_Struct.h"
#include "answer/Answer_Struct_Scene.h"
#include "heroer/Logic_Heroer_Func.h"
#include "Config_Cache.h"
#include "expedition_scene/Expedition_Scene_Struct.h"
#include "active/Active_Struct.h"
#include "campfire_scene/Campfire_Scene_Manager.h"
#include "campfire/Scene_Campfire_Struct.h"

DB_Worker::DB_Worker(void) { }

DB_Worker::~DB_Worker(void) { }

void DB_Worker::run_handler(void) {
	process_list();
}

bool DB_Worker::block_list_empty(void) {
	return block_list_.empty();
}

void DB_Worker::process_list(void) {
	Block_Buffer *buff = 0;
	Logic_Player_Detail *detail = 0;
	bool all_empty = true;

	timespec pause_ts;
	pause_ts.tv_sec = 1;
	pause_ts.tv_nsec = 0;

	while (1) {
		buff = 0;
		detail = 0;
		all_empty = true;

		if ((buff = block_list_.pop_front()) != 0) {
			all_empty = false;
			process_block(buff);
			POOL_MANAGER->push_db_oper_block_pool(buff);
		}

		if (! save_detail_list_.empty()) {
			all_empty = false;
			detail = save_detail_list_.pop_front();
			detail->save_change_detail();

			POOL_MANAGER->push_player_data_pool(detail, 12);
		}

		if(! save_rank_list_.empty()) {
			all_empty = false;
			Ranking *rank = save_rank_list_.pop_front();
			process_rank_block(rank);

			POOL_MANAGER->push_rank_data_pool(rank);
		}

		if(! save_gang_list_.empty()) {
			all_empty = false;
			Gang_DB_View *gang = save_gang_list_.pop_front();
			MONGO_OPERATOR->save_gang_db_view(*gang);
			POOL_MANAGER->push_gang_db_view(gang);
		}

		if(! save_expedition_manager_data_list_.empty()) {
			all_empty = false;
			Expedition_Manager_Data *gang = save_expedition_manager_data_list_.pop_front();
			MONGO_OPERATOR->save_expedition_manager_data(*gang);
			POOL_MANAGER->push_expedition_manager_data(gang);
		}

		if(all_empty) {
			::nanosleep (&pause_ts, 0);
		}

	}
}



int DB_Worker::process_rank_block(Ranking *rank) {
	if(!rank) {
		MSG_DEBUG("rank == 0");
		return -1;
	}
	Ranking_Map rank_;
	rank_.clear();
	rank_.insert(std::make_pair(rank->type, *rank));
	MONGO_OPERATOR->save_ranking(rank_);
	return 0;
}

int DB_Worker::process_block(Block_Buffer *buff) {
	if (! buff) {
		MSG_USER("buff == 0");
		return -1;
	}

	uint32_t len = 0, msg_id = 0;

	buff->read_uint32(len);
	buff->read_uint32(msg_id);

	switch (msg_id) {
	case MONGO_LOAD_PLAYER_ALL_DATA: {
		Logic_Player_Detail *player_data = POOL_MANAGER->pop_player_data_pool(1);
		if (! player_data) {
			LOG_ABORT("player_data_pool_.pop() return 0.");
			return -1;
		}

		buff->read_int64(player_data->role_id);

		player_data->login_player_load();
		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_PLAYER, player_data, Unit_Message::TYPE_UNIT_LOOP);
		break;
	}
	case OPERATING_OFFLINE_TEAM_AWARD: {
		int64_t role_id = 0;
		int level_id = 0;
		buff->read_int64(role_id);
		buff->read_int32(level_id);
		MONGO_OPERATOR->save_operating_team_award(role_id, level_id);
		break;
	}
	case MONGO_OPERATING_OFFLINE_DATA: {
		int type = 0;
		buff->read_int32(type);
		switch (type) {
			case OPERATING_OFFLINE_MAIL: {
				Mail_Send_Info send_info;
				send_info.deserialize(*buff);
				if (!send_info.receiver_id) {
					MSG_USER("error");
					return 0;
				}
				MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
				break;

			}
			case OPERATING_OFFLINE_FIREND_ADD: {
				int64_t role_id = 0;
				int64_t other_role_id = 0;
				int64_t addtime = 0;
				buff->read_int64(role_id);
				buff->read_int64(other_role_id);
				buff->read_int64(addtime);
				MONGO_OPERATOR->save_operating_offline_friend_add(role_id, other_role_id, addtime);
				break;
			}
			case OPERATING_OFFLINE_APPLY_HANDEL: {
				int64_t role_id = 0;
				int64_t other_role_id = 0;
				int8_t handel = 0;
				buff->read_int64(role_id);
				buff->read_int64(other_role_id);
				buff->read_int8(handel);
				MONGO_OPERATOR->save_operating_offline_apply_handel(role_id, other_role_id, handel);
				break;
			}
			case OPERATING_OFFLINE_GANGER_INFO: {
				int64_t role_id = 0;
				Ganger_Detail detail;
				buff->read_int64( role_id );
				detail.deserialize( *buff );
				MONGO_OPERATOR->save_operating_offline_ganger_info(role_id, detail);
				break;
			}
			case OPERATING_OFFLINE_ARENA_THREE_DAY_REWARD: {
				int64_t role_id = 0;
				buff->read_int64( role_id );
				Arena_Reward_Info info;
				info.deserialize(*buff);
				MONGO_OPERATOR->save_operating_offline_arena_three_day_reward(role_id, info);
				break;
			}
			case OPERATING_OFFLINE_ARENA_SEVEN_DAY_REWARD: {
				int64_t role_id = 0;
				buff->read_int64( role_id );
				Arena_Reward_Info info;
				info.deserialize(*buff);
				MONGO_OPERATOR->save_operating_offline_arena_seven_day_reward(role_id, info);
				break;
			}
			case OPERATING_OFFLINE_RECEIVE_FLOWERS: {
				role_id_t role_id = 0;
				buff->read_int64( role_id );
				std::string role_name;
				role_name.clear();
				buff->read_string(role_name);
				uint32_t item_id = 0;
				buff->read_uint32( item_id );
				uint32_t charm = 0;
				buff->read_uint32( charm );
				uint32_t friend_num = 0;
				buff->read_uint32( friend_num );
				std::string msg;
				msg.clear();
				buff->read_string(msg);
				role_id_t receive_role_id = 0;
				buff->read_int64(receive_role_id);
				MONGO_OPERATOR->save_operating_offline_receive_flowers(role_id, role_name, item_id, charm, friend_num, msg, receive_role_id);
				break;
			}
			case  OPERATING_OFFLINE_DRAGON_VALE_BUILDING: {
				role_id_t role_id = 0;
				uint8_t notice_type = 0;
				uint8_t gold_mine = 0;
				uint8_t moon_well = 0;
				uint8_t totem = 0;
				buff->read_int64(role_id);
				buff->read_uint8(notice_type);
				buff->read_uint8(gold_mine);
				buff->read_uint8(moon_well);
				buff->read_uint8(totem);
				MONGO_OPERATOR->save_operating_offline_dragon_vale_building(role_id, notice_type, gold_mine, moon_well, totem);
				break;
			}
			case OPERATING_OFFLINE_DRAGON_VALE_ROB_FIGHT_RESULT: {
				role_id_t role_id = 0;
				uint8_t notice_type = 0;
				std::string server_name;
				std::string player_name;
				uint8_t result = 0;
				buff->read_int64(role_id);
				buff->read_uint8(notice_type);
				buff->read_string(server_name);
				buff->read_string(player_name);
				buff->read_uint8(result);
				MONGO_OPERATOR->save_operating_offline_dragon_vale_rob_fight_result(role_id, notice_type,
											server_name, player_name, result);
				break;
			}
			case OPERATING_OFFLINE_DRAGON_VALE_GAIN_ROB_PRODUCTION: {
				role_id_t role_id = 0;
				std::string server_name;
				std::string player_name;
				uint32_t gold_mine = 0;
				uint32_t moon_well = 0;
				uint32_t totem = 0;
				buff->read_int64(role_id);
				buff->read_string(server_name);
				buff->read_string(player_name);
				buff->read_uint32(gold_mine);
				buff->read_uint32(moon_well);
				buff->read_uint32(totem);
				MONGO_OPERATOR->save_operating_offline_dragon_vale_gain_rob_production(role_id, server_name, player_name,
											gold_mine, moon_well, totem);
				break;
			}
			case OPERATING_OFFLINE_DRAGON_VALE_SPECIAL: {
				role_id_t role_id = 0;
				bool is_special = false;
				buff->read_int64(role_id);
				buff->read_bool(is_special);
				MONGO_OPERATOR->save_operating_offline_dragon_vale_special(role_id, is_special);
				break;
			}
			case OPERATING_OFFLINE_INVITE_PLAYER: {
				role_id_t role_id = 0;
				int64_t time_ = 0;
				buff->read_int64(role_id);
				buff->read_int64(time_);
				MONGO_OPERATOR->save_operating_offline_invite_player(role_id, time_);
				break;
			}
			case OPERATING_OFFLINE_LIKE_SUCCESS: {
				role_id_t role_id = 0;
				buff->read_int64(role_id);
				MONGO_OPERATOR->save_operating_offline_like_success(role_id);
				break;
			}
			case OPERATING_OFFLINE_ACTIVE_CONTENT_LISTEN_WORLD_BOSS_END_INFO: {
				role_id_t role_id = 0;
				buff->read_int64(role_id);
				uint32_t damage;
				uint32_t rank;
				uint32_t damage_rate;
				buff->read_uint32(damage);
				buff->read_uint32(rank);
				buff->read_uint32(damage_rate);
				MONGO_OPERATOR->save_operating_offline_active_content_listen_world_boss_end_info(role_id, damage, rank, damage_rate);
				break;
			}
		}
		break;
	}
	case MONGO_SAVE_CHAT_RECORD: {
		role_id_t role_id = 0;
		int64_t target = 0;
		uint8_t chat_type = 0;
		int8_t size = 0;
		std::vector<Chat_Content> contents;
		Chat_Content detail;
		buff->read_int64( role_id );
		buff->read_int64( target );
		buff->read_uint8( chat_type );
		buff->read_int8( size );
		for(; size > 0; size--){
			detail.deserialize( *buff );
			contents.push_back(detail);
		}
		MONGO_OPERATOR->save_chat_record(role_id, target, chat_type, contents);

		break;
	}
	case MONGO_READ_CHAT_RECORD: {
		int type = 0;
		buff->read_int32(type);
		switch(type) {
			case READ_OFFLINE: {
				role_id_t role_id = 0;
				int64_t time_sec = 0;
				buff->read_int64( role_id );
				buff->read_int64( time_sec );
				std::vector<Chat_Record> chat_record;
				chat_record.clear();
				Time_Value time_val = Time_Value(time_sec);
				MONGO_OPERATOR->load_offline_private_chat_record(role_id, time_val, chat_record);

				//return;
				//Block_Buffer *buf = POOL_MANAGER->pop_block_group(0);
				Block_Buffer *buf = CHAT_MONITOR->message_unit()->pop_block(0);
				buf->make_message(MONGO_READ_CHAT_RECORD);
				buf->write_int32(READ_OFFLINE);
				buf->write_int64(role_id);
				buf->write_int64(time_sec);
				buf->write_uint8(chat_record.size());
				for(std::vector<Chat_Record>::iterator iter = chat_record.begin(); iter != chat_record.end(); iter++) {
					iter->serialize(*buf);
				}
				buf->finish_message();
				CHAT_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, Unit_Message::TYPE_UNIT_LOOP);
				break;
			}
			case READ_HISTORY: {
				role_id_t role_id = 0;
				int64_t target = 0;
				buff->read_int64( role_id );
				buff->read_int64( target );
				std::vector<Chat_Record> chat_record;
				MONGO_OPERATOR->load_private_chat_record(role_id, target, chat_record);

				//return;
				Block_Buffer *buf = CHAT_MONITOR->message_unit()->pop_block(0);
				buf->make_message(MONGO_READ_CHAT_RECORD);
				buf->write_int32(READ_HISTORY);
				buf->write_int64(role_id);
				buf->write_int64(target);
				buf->write_uint8(chat_record.size());
				for(std::vector<Chat_Record>::iterator iter = chat_record.begin(); iter != chat_record.end(); iter++) {
					iter->serialize(*buf);
				}
				buf->finish_message();
				CHAT_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, Unit_Message::TYPE_UNIT_LOOP);
				break;
			}
		}
		break;
	}
	case MONGO_SAVE_QUESTIONNAIRE_INFO: {
		std::vector<Questionnaire_Answer_Info> ans_info;
		ans_info.clear();
		role_id_t role_id = 0;
		buff->read_int64(role_id);
		uint16_t size = 0;
		buff->read_uint16(size);
		for(uint16_t i = 0; i < size; ++i) {
			Questionnaire_Answer_Info info;
			info.deserialize(*buff);
			ans_info.push_back(info);
		}
		MONGO_OPERATOR->save_questionnaire_info(role_id, ans_info);
		break;
	}
	case MONGO_REFRESH_RANKING_RECORD: {
//		uint32_t type = 0;
//		uint32_t num = 0;
//		buff->read_uint32(type);
//		buff->read_uint32(num);
//		Ranking rank_;
//		MONGO_OPERATOR->refresh_ranking(rank_, type, num);
//
//		//return
//		Block_Buffer *buf = LOGIC_MONITOR->message_unit()->pop_block(0);
//		buf->make_message(MONGO_REFRESH_RANKING_RECORD);
//		buf->write_uint32(type);
//		rank_.serialize(*buf);
//		buf->finish_message();
//		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, Unit_Message::TYPE_UNIT_LOOP);
		Ranking *rank_ = POOL_MANAGER->rank_data_pool();
		if (! rank_) {
			LOG_ABORT("rank_pool_.pop() return 0.");
			return -1;
		}
		uint32_t type = 0;
		uint32_t num = 0;
		buff->read_uint32(type);
		buff->read_uint32(num);
		MONGO_OPERATOR->refresh_ranking(*rank_, type, num);
		rank_->type = type;

		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_RANKING, rank_, Unit_Message::TYPE_UNIT_LOOP);
		break;
	}
	case MONGO_DATA_ACTIVE_CONTENT: {
		uint32_t type = 0;
		buff->read_uint32(type);
		switch(type) {
			case ACTIVE_CONTENT_MONGO_SAVE: {
				Active_Content_Info c;
				c.deserialize(*buff);
				MONGO_OPERATOR->save_active_content(c);
				break;
			}
			case ACTIVE_CONTENT_MONGO_LOAD: {
				break;
			}
		}
		break;
	}
	case MONGO_SAVE_TWO_RECHARGE_INFO: {
		Two_Recharge_Info info;
		info.deserialize(*buff);
		MONGO_OPERATOR->save_two_recharge_info(info);
		break;
	}
	case MONGO_SAVE_MALL_BUY_NUM: {
		Item_Role_Set_Map item_sales_num;
		Tabs_Item_Role_Set_Map tabs_item_sales_num;
		item_sales_num.clear();
		tabs_item_sales_num.clear();
		{
			uint16_t size = 0;
			buff->read_uint16(size);
			for(; size > 0; --size) {
				int32_t key = 0;
				buff->read_int32(key);

				Role_Id_MSet role_mset;
				role_mset.clear();
				uint16_t set_size = 0;
				buff->read_uint16(set_size);
				for(; set_size > 0; --set_size) {
					int64_t role_id = 0;
					buff->read_int64(role_id);
					role_mset.insert(role_id);
				}

				item_sales_num.insert(std::make_pair(key, role_mset));
			}
		}
		{
			uint16_t t_size = 0;
			buff->read_uint16(t_size);
			for(; t_size > 0; --t_size) {
				int32_t t_k = 0;
				buff->read_int32(t_k);
				Item_Role_Set_Map t_val;
				t_val.clear();

				uint16_t size = 0;
				buff->read_uint16(size);
				for(; size > 0; --size) {
					int32_t key = 0;
					buff->read_int32(key);

					Role_Id_MSet role_mset;
					role_mset.clear();
					uint16_t set_size = 0;
					buff->read_uint16(set_size);
					for(; set_size > 0; --set_size) {
						int64_t role_id = 0;
						buff->read_int64(role_id);
						role_mset.insert(role_id);
					}

					t_val.insert(std::make_pair(key, role_mset));
				}

				tabs_item_sales_num.insert(std::make_pair(t_k,t_val));
			}
		}
		MONGO_OPERATOR->save_item_sales_num(item_sales_num, tabs_item_sales_num);
		break;
	}
	case MONGO_DATA_SAVE_PROPOSAL: {
		uint8_t type = 0;
		std::string contents;
		contents.clear();
		int64_t role_id = 0;
		int64_t time = 0;
		buff->read_int64(role_id);
		buff->read_uint8(type);
		buff->read_string(contents);
		buff->read_int64(time);
		MONGO_OPERATOR->save_proposal(role_id, type, contents, time);
		break;
	}
	case MONGO_SAVE_ACTIVE_DATA: {
		Active_Data_Map data_map;
		data_map.clear();
		uint16_t size = 0;
		buff->read_uint16(size);
		Active_Data_Stauts val;
		for(; size > 0; --size) {
			val.deserialize(*buff);
			data_map.insert(std::make_pair(val.id, val));
		}
		MONGO_OPERATOR->save_active_data(data_map);
		break;
	}
	case MONGO_SAVE_RANKING_RECORD: {
		uint32_t type = 0;
		buff->read_uint32(type);
		switch(type) {
			case RankingDef::SAVE_RANKING: {
				Ranking_Map rank_;
				rank_.clear();
				uint8_t size = 0;
				buff->read_uint8(size);
				for(; size > 0; --size) {
					int32_t frist = 0;
					buff->read_int32(frist);
					Ranking r;
					r.reset();
					r.deserialize(*buff);
					rank_.insert(std::make_pair(frist, r));
				}
				MONGO_OPERATOR->save_ranking(rank_);
				break;
			}
			case RankingDef::SAVE_EQUIPMENT_CHANGE: {
				int8_t op = 0;
				buff->read_int8(op);
				if(op == RankingDef::CHANGE_DELETE) {
	 				role_id_t role_id = 0;
					buff->read_int64(role_id);
					int item_id = 0;
					buff->read_int32(item_id);
					int item_index = 0;
					buff->read_int32(item_index);
					MONGO_OPERATOR->del_ranking_equipment_data(role_id, item_id, item_index);
				} else {
	 				role_id_t role_id = 0;
					buff->read_int64(role_id);
					int item_id = 0;
					buff->read_int32(item_id);
					int item_index = 0;
					buff->read_int32(item_index);
					std::string role_name;
					role_name.clear();
					buff->read_string(role_name);
					std::string item_name;
					item_name.clear();
					buff->read_string(item_name);
					int score = 0;
					buff->read_int32(score);

					Item_Equip_Info item_info;
					item_info.reset();
					item_info.deserialize(*buff);

					Item_Basic_Info item_base;
					item_base.reset();
					item_base.deserialize(*buff);

					MONGO_OPERATOR->save_ranking_equipment_data(role_id, item_id, item_index, role_name, item_name, score, item_info, item_base);
				}

				break;
			}
		}
		break;
	}
	case MONGO_LOAD_ARENA_FIGHT_DATA: {
		Logic_Player_Detail *player_data = POOL_MANAGER->pop_player_data_pool(2);
		if (! player_data) {
			LOG_ABORT("player_data_pool_.pop() return 0.");
			return -1;
		}
		// 战斗玩家id
		buff->read_int64(player_data->arenaer_detail.role_id);
		//
		buff->read_int64(player_data->role_id);
		player_data->arena_fighter_load();
		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_ARENA_FIGHTER, player_data, Unit_Message::TYPE_UNIT_LOOP);
		break;
	}
	case MANGO_SAVE_ARENA_SEASON_DATA: {
		Arena_Season_Detail season_detail;
		season_detail.deserialize(*buff);
		MONGO_OPERATOR->save_arena_season_detail(season_detail);
		break;
	}

	case MANGO_CLAER_ARENA_PLAYER_DATA: {
		MONGO_OPERATOR->clear_arena_player_data();
		break;
	}

	case MANGO_SAVE_ARENA_KING_TIME: {
		role_id_t arena_role_id;
		int king_time = 0;
		buff->read_int64(arena_role_id);
		buff->read_int32(king_time);
		MONGO_OPERATOR->save_arena_king_time(arena_role_id, king_time);
		break;
	}
	case MANGO_SAVE_ARENA_BE_WORSHIP: {
		role_id_t arena_role_id;
		int be_worship = 0;
		buff->read_int64(arena_role_id);
		buff->read_int32(be_worship);
		MONGO_OPERATOR->save_arena_be_worship(arena_role_id,be_worship);
		break;
	}

	case MONGO_KNIGHT_TRIALS_MATCH_PLAYER: {
		int64_t to_uuid= 0;
		int64_t to_role_id = 0;
		int64_t match_day = 0;
		bool clear = false;
		buff->read_int64(to_uuid);
		buff->read_int64(to_role_id);
		buff->read_int64(match_day);
		buff->read_bool(clear);

		uint16_t size = 0;
		buff->read_uint16(size);
		for(;size > 0; --size) {
			Logic_Player_Detail *player_data = POOL_MANAGER->pop_player_data_pool(3);
			if (! player_data) {
				LOG_ABORT("player_data_pool_.pop() return 0.");
				return -1;
			}
			buff->read_int64(player_data->role_id);
			player_data->knight_trials_fighter_load();

			player_data->knight_trials_detail.to_uuid = to_uuid;
			player_data->knight_trials_detail.to_role_id = to_role_id;

			if(player_data->status == Logic_Player_Detail::SUCCESS_LOADED) {
				Knight_Trials_Player *kplayer = POOL_MANAGER->pop_knight_trials_player_data_pool();
				if (! kplayer) {
					LOG_ABORT("pop_knight_trials_player_data_pool.pop() return 0.");
					return -1;
				}
				kplayer->uuid = CONFIG_CACHE->server_flag();
				kplayer->server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
				kplayer->base = player_data->base_detail;
				kplayer->fighter = player_data->fighter_detail;
				kplayer->heroer = player_data->heroer_detail;
				refresh_Heros_skill(kplayer->heroer);
				if(match_day <= 1) {
					MONGO_OPERATOR->push_cur_knight_trials_player(to_role_id, *kplayer, clear);
				} else {
					MONGO_OPERATOR->push_hide_knight_trials_player(to_role_id, *kplayer, clear);
				}
				clear = false;//这一句非常非常重要
				POOL_MANAGER->push_knight_trials_player_data_pool(kplayer);
			}

			LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_KNIGHT_TRIALS_MATCH_PLAYER, player_data, Unit_Message::TYPE_UNIT_LOOP);
		}
		break;
	}
	case MONGO_KNIGHT_TRIALS_LOAD_FIGHTER_PLAYER: {
		Knight_Trials_Player *kplayer = POOL_MANAGER->pop_knight_trials_player_data_pool();
		if (! kplayer) {
			LOG_ABORT("pop_knight_trials_player_data_pool.pop() return 0.");
			return -1;
		}
		int64_t role_id = 0;
		int64_t player_id = 0;
		buff->read_int64(role_id);
		buff->read_int64(player_id);
		if(MONGO_OPERATOR->find_cur_kinght_trials_player(role_id, player_id, *kplayer)) {
			POOL_MANAGER->push_knight_trials_player_data_pool(kplayer);
			MSG_DEBUG("pop_knight_trials_player_data_pool.pop() not find kplayer,krole_id:%d, player_id:%d",player_id, role_id);
			return -1;
		}
		kplayer->to_role_id = role_id;
		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_KNIGHT_TRIALS_LOAD_FIGHTER_PLAYER, kplayer, Unit_Message::TYPE_UNIT_LOOP);
		break;
	}
	case MONGO_KNIGHT_TRIALS_RESERT_PLAYER: {
		int64_t role_id = 0;
		buff->read_int64(role_id);
		if(role_id) {
			MONGO_OPERATOR->reset_kinght_trials_player(role_id);
		} else {
			MSG_DEBUG("mongo knight trials resert player role_id is zero");
		}
		break;
	}
	case MONGO_SAVE_INNER_CHECK_INFO: {
		uint8_t ver = 0;
		buff->peek_uint8(ver);
		if (ver == 2) {
			buff->read_uint8(ver);
			MONGO_OPERATOR->save_inner_check_info(*buff);
		} else {
			MSG_20999999 msg;
			msg.deserialize(*buff);
			std::string ip;
			if (buff->readable_bytes()) {
				buff->read_string(ip);
			}
			MONGO_OPERATOR->save_inner_check_info(msg, ip);
		}
		break;
	}
	case MONGO_SAVE_INVEST_PLAN_HISTORY_RECORD:{
		uint8_t size = 0;
		buff->read_uint8(size);

		Invest_Plan_History_Record_List history_list;
		for(uint8_t i = 0; i < size; ++i) {
			Invest_Plan_History_Record history_record;
			buff->read_uint8(history_record.invest_type);
			buff->read_uint8(history_record.history_type);
			buff->read_uint32(history_record.money);
			buff->read_string(history_record.player_name);
			history_list.push_back(history_record);
		}

		MONGO_OPERATOR->save_invest_plan_history_record(history_list);
		break;
	}
	case MONGO_LOAD_INVEST_PLAN_HISTORY_RECORD: {
		Invest_Plan_History_Record_List history_list;
		MONGO_OPERATOR->load_invest_plan_history_record(history_list);
		INVEST_PLAN_MANAGER->load_invest_plan_history_record_list(history_list);

		break;
	}

	case MONGO_LOAD_DRAGON_VALE_ROB_MATCH: {
		Logic_Player_Detail *player_data = POOL_MANAGER->pop_player_data_pool(4);
		if (! player_data) {
			LOG_ABORT("player_data_pool_.pop() return 0.");
			return -1;
		}
		int64_t to_uuid= 0;
		int64_t to_role_id = 0;
		std::string to_server_name;
		std::string to_role_name;
		buff->read_int64(to_uuid);
		buff->read_int64(to_role_id);
		buff->read_string(to_server_name);
		buff->read_string(to_role_name);
		buff->read_int64(player_data->role_id);
		uint32_t coin = 0;
		buff->read_uint32(coin);

		player_data->dragon_vale_fighter_load();
		player_data->dragon_vale_detail.to_uuid = to_uuid;
		player_data->dragon_vale_detail.to_role_id = to_role_id;
		player_data->dragon_vale_detail.to_server_name = to_server_name;
		player_data->dragon_vale_detail.to_role_name = to_role_name;
		player_data->dragon_vale_detail.coin = coin;

		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_DRAGON_VALE_ROB_MATCH, player_data, Unit_Message::TYPE_UNIT_LOOP);
		break;
	}
	case MONGO_LOAD_DRAGON_VALE_ROB_FIGHT: {
		Logic_Player_Detail *player_data = POOL_MANAGER->pop_player_data_pool(5);
		if (! player_data) {
			LOG_ABORT("player_data_pool_.pop() return 0.");
			return -1;
		}
		int64_t to_uuid= 0;
		int64_t to_role_id = 0;
		std::string to_server_name;
		std::string to_role_name;
		buff->read_int64(to_uuid);
		buff->read_int64(to_role_id);
		buff->read_string(to_server_name);
		buff->read_string(to_role_name);
		buff->read_int64(player_data->role_id);

		player_data->dragon_vale_fighter_load();
		player_data->dragon_vale_detail.to_uuid = to_uuid;
		player_data->dragon_vale_detail.to_role_id = to_role_id;
		player_data->dragon_vale_detail.to_server_name = to_server_name;
		player_data->dragon_vale_detail.to_role_name = to_role_name;

		LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_DRAGON_VALE_ROB_FIGHT, player_data, Unit_Message::TYPE_UNIT_LOOP);
		break;
	}

	case MONGO_SAVE_DRAGON_VALE_DETAIL: {
		Dragon_Vale_DB_View dragon_vale_db_view;
		dragon_vale_db_view.deserialize(*buff);
		dragon_vale_db_view.save();
		break;
	}
	case MONGO_SAVE_DRAGON_VALE_IS_SPECIAL: {
		int64_t role_id = 0;
		bool is_special = false;
		buff->read_int64(role_id);
		buff->read_bool(is_special);
		MONGO_OPERATOR->save_dragon_vale_is_special(role_id, is_special);
		break;
	}
	case MONGO_SAVE_DRAGON_VALE_RESET: {
		int64_t role_id = 0;
		uint32_t matched_num = 0;
		uint8_t star_num = 0;
		uint8_t award_foot_num = 0;
		Foot_Info_Map foot_info_map;
		Foot_Id_Set foot_id_set;
		uint8_t dragon_heart_num = 0;
		uint32_t play_dragon_num = 0;
		uint32_t drive_thief_num = 0;

		buff->read_int64(role_id);
		buff->read_uint32(matched_num);
		buff->read_uint8(star_num);
		buff->read_uint8(award_foot_num);

		uint8_t size = 0;
		buff->read_uint8(size);
		for(uint8_t i = 0; i < size; ++i) {
			int64_t key = 0;
			buff->read_int64(key);
			Foot_Info foot_info;
			buff->read_uint8(foot_info.sex);
			buff->read_string(foot_info.name);
			foot_info_map.insert(std::make_pair(key, foot_info));
		}

		buff->read_uint8(size);
		for(uint8_t i = 0; i < size; ++i) {
			int64_t id = 0;
			buff->read_int64(id);
			foot_id_set.insert(id);
		}

		buff->read_uint8(dragon_heart_num);
		buff->read_uint32(play_dragon_num);
		buff->read_uint32(drive_thief_num);

		MONGO_OPERATOR->save_dragon_vale_reset(role_id, matched_num, star_num, award_foot_num,
				foot_info_map, foot_id_set, dragon_heart_num, play_dragon_num, drive_thief_num);
		break;
	}
	case MONGO_SAVE_GANG: {
		Gang_DB_View gang_view;
		gang_view.deserialize(*buff);
		MONGO_OPERATOR->save_gang_db_view(gang_view);
		break;
	}
	case MONGO_SAVE_TITLE_INFO: {
		Title_Record_Detail detail;
		detail.deserialize(*buff);
		MONGO_OPERATOR->save_title_record(detail);
		break;
	}
	case MONGO_SAVE_TEAM_INFO: {
		RoleId_TeamSportsRankingDetail_Map detail;
		uint16_t len = 0;
		buff->read_uint16(len);
		Team_Sports_Ranking_Detail info;
		for (uint16_t i = 0; i < len; ++i) {
			info.reset();
			info.deserialize(*buff);
			detail.insert(std::make_pair(info.role_id_, info));
		}

		Time_Value team_sports_start_time;
		team_sports_start_time.deserialize(*buff);
		First_Lvl_Detail first_lvl;
		first_lvl.deserialize(*buff);

		int s_game_times = 0;
		buff->read_int32(s_game_times);

		MONGO_OPERATOR->save_team_role(detail, team_sports_start_time, first_lvl, s_game_times);
		break;
	}
	case MONGO_ACTIVATION_CODE_OP: {
		uint32_t op_type = 0;
//		std::string code;
//		code.clear();
		buff->read_uint32(op_type);
//		buff->read_string(code);
		switch(op_type) {
//			case ACTIVATION_CODE_OP_QUERY: {
//				role_id_t role_id = 0;
//				buff->read_int64(role_id);
//				Activation_Code_Info code_info;
//				MONGO_OPERATOR->query_activation_code_info(code_info, code);
//
//				//return;
//				Block_Buffer *buf = LOGIC_MONITOR->message_unit()->pop_block(0);
//				buf->make_message(MONGO_ACTIVATION_CODE_OP);
//				buf->write_uint32(op_type);
//				buf->write_int64(role_id);
//				code_info.serialize(*buf);
//				buf->finish_message();
//				LOGIC_MONITOR->message_unit()->push_ptr_data(Unit_Message::TYPE_DATA_BUFFER, buf, Unit_Message::TYPE_UNIT_LOOP);
//
//				break;
//			}
//			case ACTIVATION_CODE_OP_UPDATE: {
//
//				MONGO_OPERATOR->update_activation_code_info(code);
//
//				break;
//			}
			case ACTIVATION_CODE_RECEIVE_ID: {
				long_id_t id = 0;
				buff->read_int64(id);
				MONGO_OPERATOR->save_activation_code_receive_id(id);
				break;
			}
		}
		break;
	}
	case MONGO_SAVE_GANG_WAR_INFO: {
		Gang_War_Info gwi;
		gwi.deserialize(*buff);
		MONGO_OPERATOR->save_gang_war_info(gwi);
		break;
	}
	case MONGO_SAVE_CAMPFIRE_DETAIL: {
		Campfire_Detail cf_detail;
		cf_detail.deserialize(*buff);
		MONGO_OPERATOR->save_campfire_detail(cf_detail);
		break;
	}
	case MONGO_SAVE_WORLD_BOSS_INFO:{
		World_Boss_Detail wb_detail;
		wb_detail.deserialize(*buff);
		MONGO_OPERATOR->save_world_boss_detail(wb_detail);
		break;
	}
	case MONGO_SAVE_FB_INFO:{
		FB_Info_Detail fb_detail;
		fb_detail.deserialize(*buff);
		MONGO_OPERATOR->save_fb_info_detail(fb_detail);
		break;
	}
	case  MONGO_SAVE_ANSWER_INFO: {
		Answer_All_Data_Info answer_info;
		answer_info.deserialize(*buff);
		MONGO_OPERATOR->save_answer_info_detail(answer_info);
		break;
	}
	case MONGO_SACE_ACCESS_DATA_COUNT: {
		uint8_t size = 0;
		UInt_UInt_Map access_count;
		access_count.clear();
		buff->read_uint8(size);
		for(;size > 0; --size) {
			uint32_t type = 0;
			uint32_t num = 0;
			buff->read_uint32(type);
			buff->read_uint32(num);
			access_count.insert(std::make_pair(type, num));
		}
		MONGO_OPERATOR->save_access_count(access_count);
		break;
	}
	case MONGO_DATA_AUCTION_GOODS: {
		uint8_t type = 0;
		buff->read_uint8(type);
		switch(type) {
			case MONGO_DATA_UPDATE: {
				Auction_Goods_Data goods_data;
				goods_data.reset();
				goods_data.deserialize(*buff);
				MONGO_OPERATOR->save_action_goods_data(goods_data);
				break;
			}
			case MONGO_DATA_DELETE: {
				uint32_t id = 0;
				buff->read_uint32(id);
				MONGO_OPERATOR->delete_action_goods_data(id);
				break;
			}
		}
		break;
	}
	case MONGO_SAVE_THORLOTTERY_RECORD: {
		ThorLottery_Manager_Detail detail;
		detail.deserialize(*buff);
		detail.save();
		break;
	}
	case MONGO_SAVE_EXPEDITION_MANAGER: {
		Expedition_Manager_Detail detail;
		detail.deserialize(*buff);
		MONGO_OPERATOR->save_expedition_manager_detail(detail);
		break;
	}
	case MONGO_SAVE_EXPEDITION_MANAGER_DATA: {
		Expedition_Global_Manager_Data detail;
		detail.deserialize(*buff);
		MONGO_OPERATOR->save_expedition_global_manager_data(detail);
		break;
	}
	default: {
		MSG_USER("unknown msg_id = %d.", msg_id);
		break;
	}
	}
	return 0;
}
