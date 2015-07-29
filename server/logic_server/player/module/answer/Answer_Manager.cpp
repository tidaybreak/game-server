/*
 * Answer_Manager.cpp
 *
 *  Created on: 2014年8月14日
 *      Author: root
 */

#include "Answer_Manager.h"
#include "answer/Answer_Struct_Scene.h"
#include "Logic_Monitor.h"
#include "Config_Cache_Answer.h"
#include "Msg_Inner_Enum.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "Logic_Player_Struct.h"
#include "DB_Def.h"
#include "DB_Operator.h"
#include "ranking/Ranking_Manager.h"
#include "arena/Arena_Manager.h"
#include "Err_Code.h"
#include "announce/Logic_Announce_Manager.h"

Answer_Manager::Answer_Manager(void) {
	level_limit = 0;
	is_close = false;
}

Answer_Manager::~Answer_Manager(void) {

}

int Answer_Manager::start(void) {
	MONGO_OPERATOR->load_answer_info_detail(answer_info);
	return 0;
}

int Answer_Manager::tick(const Time_Value& now) {

	return 0;
}

void Answer_Manager::open_active(const Time_Value &st, const Time_Value &et, const Time_Value &bt, const uint32_t lv_limit) {
	start_time = st;
	end_time = et;
	bef_start_time = bt;
	level_limit = lv_limit;
	if(start_time.sec() != answer_info.last_start_time.sec()) {
		answer_info.reset();
		open_active_make_answer_info(answer_info);
		answer_info.last_start_time = start_time;
	}
	send_scene_open_action();
	is_close = false;
}

void Answer_Manager::close_active(const Time_Value &et) {
	end_time = et;
	send_scene_close_action();
}

const Time_Value &Answer_Manager::get_start_time(void) {
	return start_time;
}

const Time_Value &Answer_Manager::get_end_time(void) {
	return end_time;
}

uint32_t Answer_Manager::get_continued_time_sec(void) {
	return (CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_READ_TIME)
			+CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_ANSWER_TIME)
			+CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_ANN_TIME))
			* CONFIG_CACHE_ANSWER->get_common_cfg_by_key(ANSWER_COMMON_CFG_KEY_QUESTION_NUM);
}

int Answer_Manager::is_open(Logic_Player *player) {
	if((uint32_t)player->level() < level_limit) {
		return ERROR_LEVEL_LIMIT;
	}
	Time_Value now = Time_Value::gettimeofday();
	if(now.sec() >= bef_start_time.sec() && now.sec() < end_time.sec() && !is_close) {
		MSG_DEBUG("req enter answer scene on logic role_id:%ld, endtime:%ld, thistime:%ld", player->role_id(), end_time.sec(), now.sec());
		return 0;
	}
	return ERROR_ANSWER_ACTIVE_NOT_OPEN;
}

int Answer_Manager::data_channle(Block_Buffer& buf) {
	uint8_t type = 0;
	buf.read_uint8(type);
	switch(type) {
		case ANSWER_DATA_CHANNLE_TYPE_SL_GET_RANK_REWARD: {
			uint16_t size = 0;
			buf.read_uint16(size);
			for(uint16_t i = 0; i < size; ++i) {
				uint16_t nsize = 0;
				buf.read_uint16(nsize);
				for(uint16_t j = 0; j < nsize; ++j) {
					role_id_t role_id = 0;
					buf.read_int64(role_id);
					reward_to_player(role_id, i+1);
				} //end j
			}//end i
			break;
		}// end case
		case ANSWER_DATA_CHANNLE_TYPE_SL_SAVE_DATA: {
			save_data(buf);
			break;
		}
	}
	return 0;
}

void Answer_Manager::send_scene_open_action(void) {
	Block_Buffer buf;
	buf.make_message(INNER_ANSWER_DATA_CHANNEL);
	buf.write_uint8(ANSWER_DATA_CHANNLE_TYPE_LS_OPEN_ACTIVE);
	buf.write_int64(start_time.sec());
	//DATA START
	answer_info.serialize(buf);
	//DATA END

	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(ANSWER_INNER_SCENE_ID, 0, buf);
}

void Answer_Manager::send_scene_close_action(void) {
	is_close = true;
	Block_Buffer buf;
	buf.make_message(INNER_ANSWER_DATA_CHANNEL);
	buf.write_uint8(ANSWER_DATA_CHANNLE_TYPE_LS_CLOSE_ACTIVE);

	//DATA START
	buf.write_int64(end_time.sec());
	//DATA END

	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(ANSWER_INNER_SCENE_ID, 0, buf);
}

void Answer_Manager::reward_to_player(role_id_t role_id, uint16_t rank) {
	if(rank == 1) {
		LOGIC_ANN_MANAGER->create_ann_answer_rank_first(role_id);
	}
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player) {
		player->get_rank_reward(rank);
	} else {
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", role_id);
			return;
		}
		const Answer_Rank_Reward_Config_List &reward_conf_list = CONFIG_CACHE_ANSWER->get_rank_reward_list();
		const double mod =  CONFIG_CACHE_ANSWER->get_modulus_by_level(iter->second->level);
		const double mod_item =  CONFIG_CACHE_ANSWER->get_modulus_item_by_level(iter->second->level);
		const Answer_Rank_Reward_Config *reward_conf = 0;
		for(Answer_Rank_Reward_Config_List::const_iterator it = reward_conf_list.begin(); it != reward_conf_list.end();  ++it) {
			if(it->end_rank) {
				if(rank >= it->start_rank && rank <= it->end_rank) {
					reward_conf = &(*it);
					break;
				}
			} else {
				if(rank >= it->start_rank) {
					reward_conf = &(*it);
					break;
				}
			}
		}

		if(reward_conf) {
			Item_Vec item_list;
			item_list.clear();
			//money
			if(!reward_conf->reward.money.empty()) {
				for(Answer_Reward_Money_Vec::const_iterator money_it = reward_conf->reward.money.begin(); money_it != reward_conf->reward.money.end(); ++money_it) {
					uint32_t num = ((money_it->num + 0.0) * mod)/5000;
					uint32_t item_id = 0;
					switch(money_it->id) {
						case COPPER: {
							item_id = 20701201;
							break;
						}//金币
						case GOLD : {
							//item_id = 20701201;
							break;
						} //钻石
						case BIND_GOLD : {
							//item_id = 20701201;
							break;
						} //粉钻
						case DRAGON_SOUL : {
							//item_id = 20701201;
							break;
						} // 龙魂
						case SOULS : {
							item_id = 20703201;
							break;
						}   //英灵
					}
					if(item_id && num) {
						Item_Detail item(item_id, num, Item_Detail::BIND);
						item_list.push_back(item);
					}
				}
			}
			//item
			if(!reward_conf->reward.item.empty()) {
				for(Answer_Reward_Item_Vec::const_iterator item_it = reward_conf->reward.item.begin(); item_it != reward_conf->reward.item.end(); ++item_it) {
						Item_Detail item(item_it->id, item_it->num*mod_item, Item_Detail::BIND);
						item_list.push_back(item);
				}
			}
			if(!item_list.empty()) {
				const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(ANSWER_OFFICE_PLAYER_RANK_MAIL_ID);
				if(!mailc) {
					return;
				}
				/***********************************************/
				std::string contents;
				const int size_buf = mailc->mail_content.size() + \
						sizeof(rank) + 10;
			    char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->mail_content.c_str(), rank);
				contents.assign(content_buf);
				/************************************************/
				LOGIC_PUBLIC->send_mail(ANSWER_OFFICE_PLAYER_RANK_MAIL_ID, role_id, item_list, mailc->mail_title, contents);
			} //end if(!item_list.empty()) {
		}//end if(reward_conf) {

	}
}

void Answer_Manager::save_data(Block_Buffer& buf_) {
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_ANSWER_INFO);
	buf.copy(&buf_);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
}

void Answer_Manager::open_active_make_answer_info(Answer_All_Data_Info &answer_info) {
	int i = 0;
	//战力榜
	RANKING_MANAGER->find_rank_three(RankingDef::LOCAL_PERSONAL_FORCE_RANKING, answer_info.force_rank);
	//arena
	i = 0;
	Arena_Manager::Arena_Ranking_Vec &arena_rank = ARENA_MANAGER->arena_ranking_vec();
	for(Arena_Manager::Arena_Ranking_Vec::const_iterator rit = arena_rank.begin(); rit != arena_rank.end(); ++rit) {

		answer_info.arena_rank.push_back((*rit)->role_name());
		if(i>=3) {
			break;
		}
		++i;
	}
	//gang
	Ranking *rank = RANKING_MANAGER->get_rank_by_type(RankingDef::LOCAL_GANG_RANKING);
	if(rank) {
		i = 0;
		for(Ranking_Sort_List::iterator it = rank->ranking_.begin(); it != rank->ranking_.end(); ++it) {
			answer_info.gang_rank.push_back(it->ext_2);
			if(i>=3) {
				break;
			}
			++i;
		}
	}
}
