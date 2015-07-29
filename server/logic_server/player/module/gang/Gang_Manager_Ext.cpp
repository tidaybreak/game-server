/*
 * Gang_Manager_Ext.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: linqiyou
 */

#include "Gang_Manager.h"
//#include "Gang_Struct.h"
#include "Config_Cache.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "world_boss/Gang_Boss_Active_Manager.h"
#include "Config_Cache_World_Boss.h"
#include "Err_Code.h"
#include "Config_Cache_Gang.h"
#include "Msg_Gang_Struct.h"
#include "Msg_Active_Struct.h"
#include "Fighter_Detail.h"
#include "Logic_Player_Struct.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "mail/Mail_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Struct.h"
#include "gang/Gang_War_Def.h"
#include "Config_Cache_Scene.h"
#include "announce/Logic_Announce.h"
#include "Config_Cache_Item.h"
#include "announce/Logic_Announce_Manager.h"
#include "title/Title_Manager.h"

const int gang_boss_act_icon_id = 19;

void Gang_Manager::create_mateched_16(void) {
	gang_war_info_.switch_ = GANG_WAR_CREATE_16;
	gang_war_info_.draw_time_ = Time_Value::gettimeofday() + Time_Value(30 * Time_Value::ONE_MINUTE_IN_SECS);
	gang_war_info_.save();
	gang_war_sync_switch();
	gang_war_icon_act();
}
void Gang_Manager::ready_activity(void) {
	gang_war_info_.switch_ = GANG_WAR_READY;
	gang_war_info_.save();
	gang_war_sync_switch();
	gang_war_icon_act();
}

void Gang_Manager::open_activity(void) {
	if (gang_war_info_.switch_ != GANG_WAR_START) {
		gang_war_info_.switch_ = GANG_WAR_START;
		gang_war_info_.open_time_for_each_match = Time_Value::gettimeofday();
		gang_war_info_.save();
		gang_war_sync_switch();

		if (gang_war_info_.draw_time_ != Time_Value::zero && gang_war_info_.draw_time_ < Time_Value::gettimeofday()) {
			gang_war_info_.draw_time_ = Time_Value::zero;
			if (!gang_war_info_.gang_war_matched_gang_map_.empty()) {
				for (GangWarMatchedGangInfo_Map::iterator it = gang_war_info_.gang_war_matched_gang_map_.begin();
						it != gang_war_info_.gang_war_matched_gang_map_.end(); ++it) {
					it->second.btn_state = 1;
				}
			}
		}

//		gang_war_handle_no_battle_spot();
		gang_war_handle_single_spot_when_open();
		gang_war_info_.save();
		gang_war_icon_act();
		const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
		for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
			it->second->create_ann_gang_war();
			break;
		}
		active_gang_war_board_notice(1, 0);
	}
}
void Gang_Manager::close_activity(void) {
	if (gang_war_info_.switch_ != GANG_WAR_STOP) {
		gang_war_info_.switch_ = GANG_WAR_STOP;
		gang_war_info_.gang_war_matched_gang_map_.clear();
		gang_war_info_.gang_war_matched_gangs_pair_vec_.clear();

		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		if (dt.weekday() == 0 && (dt.hour() > 20 || (dt.hour() == 20 && dt.minute() > 55))) {
			gang_war_produce_rank_award_when_all_close();
		}
		gang_war_info_.stage_ = -1;
		gang_war_info_.save();
		gang_war_sync_switch();
		gang_war_icon_act();
	}
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	if ((dt.weekday() == 0 && dt.hour() >=23) || (dt.weekday() == 1 && dt.hour() < 11)) {
		gang_war_rank_award_auto_get();
	}
}

int Gang_Manager::gang_war_start_sync(const int stage, const bool not_create_matched_info) {
	gang_war_info_.set_activity_from();
	gang_war_info_.stage_ = stage;
	gang_war_sync_data_to_scene(not_create_matched_info);
	gang_war_info_.save();
	return 0;
}

int Gang_Manager::gang_war_load_matched_info(Block_Buffer &buf) {
	uint16_t len = 0;
	int8_t stage = 0;
	buf.read_int8(stage);
	buf.read_uint16(len);
	MSG_USER("GANG_WAR: gang_war_load_matched_info, gang_war_info_.stage_(%d), stage(%d), len(%d)", gang_war_info_.stage_, stage, len);
	Gang_War_Matched_Gangs_Pair matched_gangs;
	Gang_War_Matched_Gang_Info matched_gangs_info;
	gang_war_info_.gang_war_matched_gang_map_.clear();
	gang_war_info_.gang_war_matched_gangs_pair_vec_.clear();
	for (uint16_t i = 0; i < len; ++i) {
		matched_gangs.reset();
		matched_gangs_info.reset();

		buf.read_int64(matched_gangs.matched_pair.first);
		buf.read_int64(matched_gangs_info.matched_index);
		buf.read_int8(matched_gangs_info.camp);
		matched_gangs_info.match_time.deserialize(buf);
		buf.read_int64(matched_gangs_info.scene_index);
		buf.read_string(matched_gangs_info.zone);
		buf.read_string(matched_gangs_info.gang_name);
		buf.read_int8(matched_gangs_info.rank);
		buf.read_int64(matched_gangs_info.force);
		if (matched_gangs.matched_pair.first > 0) {
			gang_war_info_.gang_war_matched_gang_map_.insert(std::make_pair(matched_gangs.matched_pair.first, matched_gangs_info));
		}

		matched_gangs_info.reset();
		buf.read_int64(matched_gangs.matched_pair.second);
		buf.read_int64(matched_gangs_info.matched_index);
		buf.read_int8(matched_gangs_info.camp);
		matched_gangs_info.match_time.deserialize(buf);
		buf.read_int64(matched_gangs_info.scene_index);
		buf.read_string(matched_gangs_info.zone);
		buf.read_string(matched_gangs_info.gang_name);
		buf.read_int8(matched_gangs_info.rank);
		buf.read_int64(matched_gangs_info.force);
		if (matched_gangs.matched_pair.second > 0) {
			gang_war_info_.gang_war_matched_gang_map_.insert(std::make_pair(matched_gangs.matched_pair.second, matched_gangs_info));
		}

		matched_gangs.match_time.deserialize(buf);
		buf.read_int8(matched_gangs.result);
		buf.read_int8(matched_gangs.matched_index);
		buf.read_int64(matched_gangs.scene_index);

		gang_war_info_.gang_war_matched_gangs_pair_vec_.push_back(matched_gangs);
	}

	if (gang_war_info_.stage_ == 2 && gang_war_info_.switch_ == GANG_WAR_CREATE_16) {
		for (GangWarMatchedGangsPair_Vec::iterator it = gang_war_info_.gang_war_matched_gangs_pair_vec_.begin();
					it != gang_war_info_.gang_war_matched_gangs_pair_vec_.end(); ++it) {
			Gang_DB_View* gdv = this->gang_view_by_id(it->matched_pair.first);
			if (gdv) {
				for (Gang_DB_View::Member_Map::iterator it_mm = gdv->member_map.begin();
						it_mm != gdv->member_map.end(); ++it_mm) {
					Item_Vec item_list;
					LOGIC_PUBLIC->send_mail(GANG_WAR_ATTEND, it_mm->first, item_list);
				}
			}
			gdv = this->gang_view_by_id(it->matched_pair.second);
			if (gdv) {
				for (Gang_DB_View::Member_Map::iterator it_mm = gdv->member_map.begin();
						it_mm != gdv->member_map.end(); ++it_mm) {
					Item_Vec item_list;
					LOGIC_PUBLIC->send_mail(GANG_WAR_ATTEND, it_mm->first, item_list);
				}
			}
		}
	}

	Gang_War_Spot spot;
	for (GangWarMatchedGangsPair_Vec::iterator it = gang_war_info_.gang_war_matched_gangs_pair_vec_.begin();
			it != gang_war_info_.gang_war_matched_gangs_pair_vec_.end(); ++it) {
		std::map<int8_t, Gang_War_Spot>::iterator it_res = gang_war_info_.gang_war_result_map_.find(it->matched_index);
		if (it_res == gang_war_info_.gang_war_result_map_.end()) {
			spot.reset();
			spot.index = it->matched_index;
			spot.result = it->result;
			Gang_Basic_Detail gbd;
			gbd.gang_id = it->matched_pair.first;
			gbd.gang_name = GANG_MANAGER->gang_name(it->matched_pair.first);
			gbd.index = 0;
			if (gbd.gang_id > 0) {
				const Gang_War_Matched_Gang_Info *gwmgi = gang_war_find_matched_gang_info(gbd.gang_id);
				if (gwmgi) {
					gbd.zone = gwmgi->zone;
					gbd.gang_name = gwmgi->gang_name;
					gbd.rank = gwmgi->rank;
				}
				spot.gang_vec.push_back(gbd);
			}
			gbd.reset();
			gbd.gang_id = it->matched_pair.second;
			gbd.gang_name = GANG_MANAGER->gang_name(it->matched_pair.second);
			gbd.index = 1;
			if (gbd.gang_id > 0) {
				const Gang_War_Matched_Gang_Info *gwmgi = gang_war_find_matched_gang_info(gbd.gang_id);
				if (gwmgi) {
					gbd.zone = gwmgi->zone;
					gbd.gang_name = gwmgi->gang_name;
					gbd.rank = gwmgi->rank;
				}
				spot.gang_vec.push_back(gbd);
			}
			gang_war_info_.gang_war_result_map_.insert(std::make_pair(spot.index, spot));
		}
	}
	gang_war_handle_no_battle_spot();
	gang_war_info_.save();
	return 0;
}

const Gang_War_Matched_Gang_Info *Gang_Manager::gang_war_find_matched_gang_info(const int64_t gang_id) {
	GangWarMatchedGangInfo_Map::iterator it = gang_war_info_.gang_war_matched_gang_map_.find(gang_id);
	if (it != gang_war_info_.gang_war_matched_gang_map_.end()) {
		return &(it->second);
	}
	return NULL;
}
int Gang_Manager::gang_war_set_matched_gang_info_btn_state(const int64_t gang_id, const int8_t btn_state) {
	GangWarMatchedGangInfo_Map::iterator it = gang_war_info_.gang_war_matched_gang_map_.find(gang_id);
	if (it != gang_war_info_.gang_war_matched_gang_map_.end()) {
		it->second.btn_state = btn_state;
	} else {
		return -1;
	}
	return 0;
}
int Gang_Manager::gang_war_load_spot_rank_info(Block_Buffer &buf) {
	Time_Value now = Time_Value::gettimeofday();
	Time_Value gap_time_value = now - gang_war_info_.open_time_for_each_match;
	int gap_time = gap_time_value.sec() / Time_Value::ONE_MINUTE_IN_SECS;
	int win_score = 0;
	int lose_score = 0;
	Int_Int_Map::const_iterator it_ws = CONFIG_CACHE_GANG->gang_war_setting_cfg().win_scores.find(gap_time);
	if (it_ws != CONFIG_CACHE_GANG->gang_war_setting_cfg().win_scores.end()) {
		win_score = it_ws->second;
	}
	it_ws = CONFIG_CACHE_GANG->gang_war_setting_cfg().lose_scores.find(gap_time);
	if (it_ws != CONFIG_CACHE_GANG->gang_war_setting_cfg().lose_scores.end()) {
		lose_score = it_ws->second;
	}

	int mail_id = GANG_WAR_ATTEND_AWARD;
	Item_Vec pack_item_vec_win;
	Item_Vec pack_item_vec_lose;
	std::vector<Item_Basic_Info> item_vector_win;
	std::vector<Item_Basic_Info> item_vector_lose;
	Item_Detail item_detail;
	Item_Basic_Info ibi;
	int self_contribute = 0;
	int self_mercit = 0;
	int self_contribute_lose = 0;
	int self_mercit_lose = 0;
	Item_Vec item_vec;
	Item_Vec item_vec_lose;

	const Gang_War_Rank_Detail_Cfg *cfg = CONFIG_CACHE_GANG->find_gang_war_rank(1);
	if (cfg) {
		Reward_Info_Gang_War rigw;
		for (Item_Info_Vec::const_iterator it = cfg->list.begin(); it != cfg->list.end(); ++it) {
			item_detail.reset();
			item_detail.id_ = it->id;
			item_detail.amount_ = it->amount;
			item_detail.init();
			item_vec.push_back(item_detail);
		}
		for (Item_Info_Vec::const_iterator it = cfg->lose_list.begin(); it != cfg->lose_list.end(); ++it) {
			item_detail.reset();
			item_detail.id_ = it->id;
			item_detail.amount_ = it->amount;
			item_detail.init();
			item_vec_lose.push_back(item_detail);
		}

		for (Item_Info_Vec::const_iterator it = cfg->self_list.begin(); it != cfg->self_list.end(); ++it) {
			switch (it->id) {
			case 503003: {
				self_mercit = it->amount;
				break;
			}
			case 503803: {
				self_contribute = it->amount;
				break;
			}
			default:
				item_detail.reset();
				item_detail.id_ = it->id;
				item_detail.amount_ = it->amount;
				item_detail.bind_ = Item_Detail::BIND;
				pack_item_vec_win.push_back(item_detail);
				ibi.reset();
				item_detail.set_item_basic_info(ibi);
				item_vector_win.push_back(ibi);
				break;
			}
		}

		for (Item_Info_Vec::const_iterator it = cfg->lose_self_list.begin(); it != cfg->lose_self_list.end(); ++it) {
			switch (it->id) {
			case 503003: {
				self_mercit_lose = it->amount;
				break;
			}
			case 503803: {
				self_contribute_lose = it->amount;
				break;
			}
			default:
				item_detail.reset();
				item_detail.id_ = it->id;
				item_detail.amount_ = it->amount;
				item_detail.bind_ = Item_Detail::BIND;
				pack_item_vec_lose.push_back(item_detail);
				ibi.reset();
				item_detail.set_item_basic_info(ibi);
				item_vector_lose.push_back(ibi);
				break;
			}
		}
	}

	int id = 0;
	uint16_t len = 0;
	int win_league = 0;
	int64_t win_gang_id= 0;
	int64_t lose_gang_id = 0;
	buf.read_int32(id);
	buf.read_int64(win_gang_id);
	buf.read_int64(lose_gang_id);
	buf.read_int32(win_league);
	buf.read_uint16(len);
	Gang_War_Rank_Info info;
	int8_t index = static_cast<int8_t>(id);
//	std::map<int8_t, Gang_War_Spot>::iterator it_res = gang_war_info_.gang_war_result_map_.find(index);
//	if (it_res != gang_war_info_.gang_war_result_map_.end()) {
//		if (it_res->second.result == 1) {
//			win_gang_id = it_res->second.gang_vec[0].gang_id;
//			if (it_res->second.gang_vec.size() > 1) {
//				lose_gang_id = it_res->second.gang_vec[1].gang_id;
//			}
//		} else if (it_res->second.result == 2) {
//			win_gang_id = it_res->second.gang_vec[1].gang_id;
//			lose_gang_id = it_res->second.gang_vec[0].gang_id;
//		}
//	}
	for (uint16_t i = 0; i < len; ++i) {
		info.reset();
		info.deserialize(buf);
		gang_war_info_.gang_war_spot_rank_map_[index].push_back(info);
		if (0 != gang_war_info_.gang_rank_map.count(info.gang_id)) {
			DB_View::ID_Player_View_Map::iterator iter =
											LOGIC_MONITOR->db_view()->id_player_view_map.find(info.role_id);
			if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
				info.role_name = iter->second->role_name;
			}
			gang_war_info_.gang_rank_map[info.gang_id].add_ganger_rank_info(info);
		} else {
			MSG_USER("gang_war_load_rank_spot_data : role(%ld)", info.role_id);
		}

		// 结算奖励
		Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(info.role_id);
		if (pl && info.camp == win_league) {
			pl->achievement_listen_gang_war_win_nums();
		}
//		if (info.die_num == 0 && info.hold_spot == 0 && info.kill_num == 0) { // 判断是否混水
//			continue;
//		}
		if (info.camp == win_league) {
			if (item_vec.size() == 3) {
				const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
				if(!mailc) {
					continue;
				}
				std::string contents;
				int size_buf = mailc->mail_content.size() + 10;
				std::string fund = "";
				{
					const std::string* str = CONFIG_CACHE_GANG->find_weekday_string(8);
					if (str) {
						fund = *str;
					}
				}
				std::vector<std::string> name_list;
				Int_Vec iv;
					for (Item_Vec::const_iterator it_iv = item_vec.begin(); it_iv != item_vec.end(); ++it_iv) {
					if (it_iv->id_ == 503803) {
						size_buf += 4 + 10;
						name_list.push_back(fund);
						iv.push_back(it_iv->amount_);
						continue;
					}
					const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(it_iv->id_);
					if (idc) {
						size_buf += sizeof(idc->name) * 3 + 2;
						name_list.push_back(idc->name);
						iv.push_back(it_iv->amount_);
					}
				}
				if (name_list.size() == 3 && iv.size() == 3) {
					char content_buf[size_buf];
					memset(content_buf, 0, size_buf);
					snprintf(content_buf, size_buf, mailc->mail_content.c_str(),
							name_list[0].c_str(), iv[0], name_list[1].c_str(), iv[1],
							name_list[2].c_str(), iv[2]);
					contents.assign(content_buf);
					LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_win, "", contents);
				} else {
					LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_win);
				}
			} else {
				LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_win);
			}
		} else {
			if (item_vec_lose.size() == 3) {
				const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
				if(!mailc) {
					continue;
				}
				std::string contents;
				int size_buf = mailc->mail_content.size() + 10;
				std::string fund = "";
				{
					const std::string* str = CONFIG_CACHE_GANG->find_weekday_string(8);
					if (str) {
						fund = *str;
					}
				}
				std::vector<std::string> name_list;
				Int_Vec iv;
				for (Item_Vec::const_iterator it_iv = item_vec_lose.begin(); it_iv != item_vec_lose.end(); ++it_iv) {
					if (it_iv->id_ == 503803) {
						size_buf += 4 + 10;
						name_list.push_back(fund);
						iv.push_back(it_iv->amount_);
						continue;
					}
					const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(it_iv->id_);
					if (idc) {
						size_buf += sizeof(idc->name) * 3 + 2;
						name_list.push_back(idc->name);
						iv.push_back(it_iv->amount_);
					}
				}
				if (name_list.size() == 3 && iv.size() == 3) {
					char content_buf[size_buf];
					memset(content_buf, 0, size_buf);
					snprintf(content_buf, size_buf, mailc->mail_content.c_str(),
							name_list[0].c_str(), iv[0], name_list[1].c_str(), iv[1],
							name_list[2].c_str(), iv[2]);
					contents.assign(content_buf);
					LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_lose, "", contents);
				} else {
					if (info.camp == win_league) {
						LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_win);
					} else {
						LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_lose);
					}
				}
			} else {
				LOGIC_PUBLIC->send_mail(mail_id, info.role_id, item_vector_lose);
			}
		}
	}
	if (0 != gang_war_info_.gang_rank_map.count(win_gang_id)) {
		gang_war_info_.gang_rank_map[win_gang_id].win_nums += 1;
		gang_war_info_.gang_rank_map[win_gang_id].score += win_score;
	}
	if (0 != gang_war_info_.gang_rank_map.count(lose_gang_id)) {
		gang_war_info_.gang_rank_map[lose_gang_id].score += lose_score;
	}
	gang_war_info_.save();
	return 0;
}
void Gang_Manager::gang_war_load_result_info(Block_Buffer &buf) {
//	int8_t stage = 0;
	uint16_t len = 0;
//	buf.read_int8(stage);
	buf.read_uint16(len);//should be 1
	Gang_War_Spot spot;
	for (uint16_t i = 0; i < len; ++i) {
		spot.reset();
		spot.deserialize(buf);
		std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.find(spot.index);
		if (it != gang_war_info_.gang_war_result_map_.end()) {
			it->second.result = spot.result;
		} else {
			gang_war_info_.gang_war_result_map_.insert(std::make_pair(spot.index, spot));
		}
//		gang_war_handle_next_spot(spot);
	}
	gang_war_info_.save();
}

void Gang_Manager::gang_war_ready_mail(void) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	if (dt.weekday() == 0
			&& (dt.hour() > 20
					|| (dt.hour() == 20 && dt.minute() > 30))) {
		return;
	}
	int weekday = dt.weekday();
	if (weekday == 0) {
		weekday = 7;
	}
	std::stringstream stream;
	stream <<  "20:";
	if ((dt.hour() > 20
			|| (dt.hour() == 20 && dt.minute() > 30))) {
		weekday += 2;
		if (weekday > 7) {
			weekday = 7;
		}
		stream << "00";
	} else {
		stream << "30";
	}
	std::string this_time = stream.str();

	std::string weekday_str = "";
	const std::string* str = CONFIG_CACHE_GANG->find_weekday_string(weekday);
	if (str) {
		weekday_str = *str;
	}
	gang_war_produce_matched_info();
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.begin();
			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
		if (it->second.gang_vec.size() < 2) {
			continue;
		}
		int8_t spot_len = 0;
		for (std::vector<Gang_Basic_Detail>::iterator it_vec = it->second.gang_vec.begin(); it_vec != it->second.gang_vec.end(); ++it_vec) {
			if (it_vec->gang_id > 0) {
				++spot_len;
			}
		}
		if (2 > spot_len) {
			continue;
		}
		//mail;
		int mail_id = GANG_WAR_FIGHT_ONE;
		const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
		if (!mailc) {
			continue;
		}
		{
			std::vector<Item_Basic_Info> item_vector;
			std::string contents;
			const int size_buf = mailc->mail_content.size() +
					 weekday_str.size() * 3 + this_time.size() * 3 +
					sizeof(it->second.gang_vec[0].gang_name) + 10 +
					sizeof(it->second.gang_vec[0].zone) + 10;
			char content_buf[size_buf];
			memset(content_buf, 0, size_buf);
			snprintf(content_buf, size_buf, mailc->mail_content.c_str(), weekday_str.c_str(), this_time.c_str(), it->second.gang_vec[0].zone.c_str(), it->second.gang_vec[0].gang_name.c_str());
			contents.assign(content_buf);
			/************************************************/
			Gang_ID_Map::const_iterator it_g = gang_id_map.find(it->second.gang_vec[1].gang_id);
			if (it_g != gang_id_map.end()) {
				for (Gang_DB_View::Member_Map::const_iterator it_mm = it_g->second->member_map.begin(); it_mm != it_g->second->member_map.end(); ++it_mm){
					LOGIC_PUBLIC->send_mail(mail_id, it_mm->first, item_vector, "", contents);
				}
			}
		}

		{
			std::vector<Item_Basic_Info> item_vector;
			std::string contents;
			const int size_buf = mailc->mail_content.size() +
					 weekday_str.size() * 3 + this_time.size() * 3 +
					sizeof(it->second.gang_vec[1].gang_name) + 10 +
					sizeof(it->second.gang_vec[1].zone) + 10;
			char content_buf[size_buf];
			memset(content_buf, 0, size_buf);
			snprintf(content_buf, size_buf, mailc->mail_content.c_str(), weekday_str.c_str(), this_time.c_str(), it->second.gang_vec[1].zone.c_str(), it->second.gang_vec[1].gang_name.c_str());
			contents.assign(content_buf);
			/************************************************/
			Gang_ID_Map::const_iterator it_g = gang_id_map.find(it->second.gang_vec[0].gang_id);
			if (it_g != gang_id_map.end()) {
				for (Gang_DB_View::Member_Map::const_iterator it_mm = it_g->second->member_map.begin(); it_mm != it_g->second->member_map.end(); ++it_mm){
					LOGIC_PUBLIC->send_mail(mail_id, it_mm->first, item_vector, "", contents);
				}
			}
		}
	}
}

const Gang_War_Matched_Gangs_Pair *Gang_Manager::gang_war_find_matched_gangs_pair(const int64_t gang_id) {
	for (GangWarMatchedGangsPair_Vec::const_iterator it = gang_war_info_.gang_war_matched_gangs_pair_vec_.begin();
			it != gang_war_info_.gang_war_matched_gangs_pair_vec_.end(); ++it) {
		if (it->matched_pair.first == gang_id || it->matched_pair.second == gang_id) {
			return &(*it);
		}
	}
	return NULL;
}
const Gang_War_Spot *Gang_Manager::gang_war_find_result_map(const int8_t matched_index) {
	std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.find(matched_index);
	if (it != gang_war_info_.gang_war_result_map_.end()) {
		return &(it->second);
	}
	return NULL;
}
const Gang_War_Spot *Gang_Manager::gang_war_find_result_map(const int64_t gang_id) {
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.begin();
			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
		for (std::vector<Gang_Basic_Detail>::iterator it_gbd = it->second.gang_vec.begin();
				it_gbd != it->second.gang_vec.end(); ++it_gbd) {
			if (it_gbd->gang_id == gang_id) {
				return &(it->second);
			}
		}
	}
	return NULL;
}
void Gang_Manager::gang_war_load_award(Block_Buffer &buf) {
	role_id_t gang_id = 0;
	buf.read_int64(gang_id);
	Gang_DB_View* gdv = gang_view_by_id(gang_id);
	if (gdv) {
		uint16_t len = 0;
		buf.read_uint16(len);
		int32_t fund = 0;
		Reward_Info_Gang_War rigw;
		for (uint16_t i = 0; i < len; ++i) {
			rigw.reset();
			buf.read_int32(rigw.award.reward_id);
			buf.read_int32(rigw.award.quantity);
			if (rigw.award.reward_id == 503803) {
				fund = rigw.award.quantity;
				continue;
			}
			int32_t amount = rigw.award.quantity;
			for (int32_t i = 0; i < amount; i++) {
				rigw.award.quantity = 1;
				rigw.index = gdv->make_gang_war_index();
				gdv->gang_war_award_map().insert(std::make_pair(rigw.index, rigw));
			}
			gdv->change_view();
		}
//		buf.read_int32(fund);
		if (fund > 0) {
			gdv->add_contribute(fund);

			GANG_MANAGER->refresh_rank();
			gdv->change_view();
			// notify ganger info, gang info
			GANG_MANAGER->notify_gang_info_update(*gdv);
		}
	}
}
int Gang_Manager::gang_war_help(Block_Buffer &buf) {
	int64_t gang_id = 0;
	int64_t leader_id_other = 0;
	buf.read_int64(gang_id);
	buf.read_int64(leader_id_other);
	active_gang_war_board_notice(2, leader_id_other, gang_id);
	return 0;
}
bool Gang_Manager::gang_war_is_open(void) {
	return this->gang_war_info_.switch_ == GANG_WAR_START;
}

void Gang_Manager::gang_war_award_single_spot(const Gang_War_Spot &spot) {
	// mail
	std::vector<Item_Basic_Info> item_vector;
	Item_Basic_Info ibi;
	int mail_id = GANG_WAR_FIGHT_EMPTY_ONE;
	// mail
	for (std::vector<Gang_Basic_Detail>::const_iterator it_vec = spot.gang_vec.begin(); it_vec != spot.gang_vec.end(); ++it_vec) {
		if (it_vec->gang_id == 0 || ((spot.result == 1 && it_vec->index == 1) || (spot.result == 2 && it_vec->index == 0))) {
			continue;
		}
		Gang_DB_View* gdv = gang_view_by_id(it_vec->gang_id);
		if (!gdv) {
			continue;
		}
		MSG_81170004 award_win;
		int self_contribute = 0;
		int self_mercit = 0;
		int32_t fund = 0;
		Item_Vec item_vec;
		Item_Detail item_detail;
		const Gang_War_Rank_Detail_Cfg *cfg = CONFIG_CACHE_GANG->find_gang_war_rank(gang_war_info_.stage_);
		if (!cfg) {
			continue;
		}
		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		int weekday = dt.weekday();
		if (weekday == 0) {
			weekday = 7;
		}
		std::string weekday_str = "";
		const std::string* str = CONFIG_CACHE_GANG->find_weekday_string(weekday);
		if (str) {
			weekday_str = *str;
		}
		std::stringstream stream;
		stream <<  (dt.hour() < 10 ? "0" : "") << dt.hour() << ":";
		stream << (dt.minute() < 10 ? "0" : "") << dt.minute();
		std::string this_time = stream.str();

		Reward_Info_Gang_War rigw;
		for (Item_Info_Vec::const_iterator it = cfg->list.begin(); it != cfg->list.end(); ++it) {
			item_detail.reset();
			item_detail.id_ = it->id;
			item_detail.amount_ = it->amount;
			item_detail.init();
			item_vec.push_back(item_detail);

			rigw.reset();
			rigw.award.reward_id = it->id;
			rigw.award.quantity = it->amount;
			if (rigw.award.reward_id == 503803) {
				fund = rigw.award.quantity;
				continue;
			}
			int32_t amount = rigw.award.quantity;
			for (int32_t i = 0; i < amount; i++) {
				rigw.award.quantity = 1;
				rigw.index = gdv->make_gang_war_index();
				gdv->gang_war_award_map().insert(std::make_pair(rigw.index, rigw));
			}
			gdv->change_view();
		}

		Reward_Info award;
		uint16_t len = 0;
		for (Item_Info_Vec::const_iterator it = cfg->self_list.begin(); it != cfg->self_list.end(); ++it) {
			award.reset();
			award.reward_id = it->id;
			award.quantity = it->amount;
			award_win.self.push_back(award);
			switch (it->id) {
			case 503003: {
				self_mercit = it->amount;
				break;
			}
			case 503803: {
				self_contribute = it->amount;
				break;
			}
			default:
				++len;
				ibi.reset();
				ibi.id = it->id;
				ibi.amount = it->amount;
				ibi.bind = Item_Detail::BIND;
				item_vector.push_back(ibi);
				break;
			}
		}

		if (fund > 0) {
			gdv->add_contribute(fund);

			GANG_MANAGER->refresh_rank();
			gdv->change_view();
			// notify ganger info, gang info
			GANG_MANAGER->notify_gang_info_update(*gdv);
		}
		Block_Buffer buf_bak;
		for (Gang_DB_View::Member_Map::iterator it_mm = gdv->member_map.begin();
				it_mm != gdv->member_map.end(); ++it_mm) {
			Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(it_mm->first);
			if (pl) {
				if (self_contribute > 0) {
					pl->add_contribute(self_contribute);
				}
				if (self_mercit > 0) {
					pl->change_exploit_val(self_mercit);
				}
			}
			if (item_vec.size() == 3) {
				const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
				if(!mailc) {
					continue;
				}
				std::string contents;
				int size_buf = mailc->mail_content.size() + 10 + weekday_str.size() * 3 + this_time.size() * 3;
				std::string fund = "";
				{
					const std::string* str = CONFIG_CACHE_GANG->find_weekday_string(8);
					if (str) {
						fund = *str;
					}
				}
				std::vector<std::string> name_list;
				Int_Vec iv;
				for (Item_Vec::const_iterator it_iv = item_vec.begin(); it_iv != item_vec.end(); ++it_iv) {
					if (it_iv->id_ == 503803) {
						size_buf += 4 + 10;
						name_list.push_back(fund);
						iv.push_back(it_iv->amount_);
						continue;
					}
					const Item_Detail_Config *idc = CONFIG_CACHE_ITEM->find_item(it_iv->id_);
					if (idc) {
						size_buf += sizeof(idc->name) * 3 + 2;
						name_list.push_back(idc->name);
						iv.push_back(it_iv->amount_);
					}
				}
				if (name_list.size() == 3 && iv.size() == 3) {
					char content_buf[size_buf];
					memset(content_buf, 0, size_buf);
					snprintf(content_buf, size_buf, mailc->mail_content.c_str(),
							weekday_str.c_str(), this_time.c_str(),
							name_list[0].c_str(), iv[0], name_list[1].c_str(), iv[1],
							name_list[2].c_str(), iv[2]);
					contents.assign(content_buf);
					LOGIC_PUBLIC->send_mail(mail_id, it_mm->first, item_vector, "", contents);
				} else {
					LOGIC_PUBLIC->send_mail(mail_id, it_mm->first, item_vector);
				}
			} else {
				LOGIC_PUBLIC->send_mail(mail_id, it_mm->first, item_vector);
			}
		}
		break;
	}
}

int Gang_Manager::gang_war_sync_switch(void) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_STATE);
	buf.write_int32(CONFIG_CACHE->server_maintainer_cache().game_inter_flag);
	buf.write_int8(gang_war_info_.switch_);
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(GANG_WAR_SCENE_ID, 0, buf);
	return 0;
}
void Gang_Manager::gang_war_icon_act(void) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(gang_war_info_.open_time_for_each_match);

	MSG_81000408 acv_msg;
	acv_msg.icon_id = 1;
	if(gang_war_info_.switch_ == GANG_WAR_STOP) {
		acv_msg.open = 0;
		acv_msg.active = 0;
		acv_msg.time = 0;
	}else if(gang_war_info_.switch_ == GANG_WAR_READY) {
		acv_msg.open = 0;
		acv_msg.active = 0;
		return ;
	}else if(gang_war_info_.switch_ == GANG_WAR_START){
		acv_msg.open = 1;
		acv_msg.active = 1;
		acv_msg.time = 0;
		if (now > Time_Value(dt.time_sec() + CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time + 10)) {
			return ;
		}
		acv_msg.time = dt.time_sec() + CONFIG_CACHE_GANG->gang_war_setting_cfg().finish_time - now.sec();
	} else {
		return ;
	}
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		if(it->second->level() >= 20){
			OBJ_SEND_TO_CLIENT(acv_msg, (*it->second));
		}
	}
}

Gang_War_Info &Gang_Manager::gang_war_info(void) {
	return gang_war_info_;
}
void Gang_Manager::gang_war_clear_award_list(void) {
	for (Gang_ID_Map::iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end();++it_gang) {
		for (boost::unordered_map<int64_t, Reward_Info_Gang_War>::iterator it_award = it_gang->second->gang_war_award.begin();
				it_award != it_gang->second->gang_war_award.end();) {
			it_award = it_gang->second->gang_war_award.erase(it_award);
		}
	}
}
//int64_t Gang_Manager::gang_war_get_next_index(const int64_t gang_id) {
//	int64_t matched_index = -1;
//	int64_t matched_index_res = -1;
//	for (std::map<int8_t, Gang_War_Spot>::const_iterator it = gang_war_info_.gang_war_result_map_.begin();
//			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
//		if (it->first >= 8) {
//			continue;
//		}
//		for (std::vector<Gang_Basic_Detail>::const_iterator it_gv = it->second.gang_vec.begin();
//				it_gv != it->second.gang_vec.end(); ++it_gv) {
//			if (it_gv->gang_id == gang_id) {
//				matched_index = it->first;
//				break;
//			}
//		}
//		if (matched_index != -1) {
//			break;
//		}
//	}
//	if (gang_war_info_.stage_ == 4) {
//		matched_index_res = 14;
//	} else if (gang_war_info_.stage_ == 2) {
//		matched_index_res = 15;
//	}
//	if (matched_index != -1) {
//		switch(matched_index) {
//		case 0:
//		case 1: {
//			if (gang_war_info_.stage_ == 16) {
//				matched_index_res = 8;
//			} else if (gang_war_info_.stage_ == 8) {
//				matched_index_res = 12;
//			}
//			break;
//		}
//		case 2:
//		case 3: {
//			if (gang_war_info_.stage_ == 16) {
//				matched_index_res = 9;
//			} else if (gang_war_info_.stage_ == 8) {
//				matched_index_res = 12;
//			}
//			break;
//		}
//		case 4:
//		case 5: {
//			if (gang_war_info_.stage_ == 16) {
//				matched_index_res = 10;
//			} else if (gang_war_info_.stage_ == 8) {
//				matched_index_res = 13;
//			}
//			break;
//		}
//		case 6:
//		case 7: {
//			if (gang_war_info_.stage_ == 16) {
//				matched_index_res = 11;
//			} else if (gang_war_info_.stage_ == 8) {
//				matched_index_res = 13;
//			}
//			break;
//		}
//		default:
//			break;
//		}
//	}
//	return matched_index_res;
//}

void Gang_Manager::gang_war_send_mail_for_start(void) {
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(GANG_WAR_FIGHT_ONE);
	if(!mailc) {
		return;
	}

	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	int weekday = dt.weekday();
	if (weekday == 0) {
		weekday = 7;
	}
	std::string weekday_str = "";
	const std::string* str = CONFIG_CACHE_GANG->find_weekday_string(weekday);
	if (str) {
		weekday_str = *str;
	}
	std::stringstream stream;
	stream << "20:00";
	std::string this_time = stream.str();

	for (int8_t i = 0; i < 8; ++i) {
		std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.find(i);
		if (it == gang_war_info_.gang_war_result_map_.end() || it->second.gang_vec.size() < 2) {
			continue;
		}
		Gang_DB_View* gdv = this->gang_view_by_id(it->second.gang_vec[0].gang_id);
		if (gdv) {
			for (Gang_DB_View::Member_Map::iterator it_mm = gdv->member_map.begin();
					it_mm != gdv->member_map.end(); ++it_mm) {
				Item_Vec item_list;
				std::string contents;
				const int size_buf = mailc->mail_content.size() +
						weekday_str.size() * 3 + this_time.size() * 3 +
						sizeof(it->second.gang_vec[1].gang_name) + 10 +
						sizeof(it->second.gang_vec[1].zone) + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->mail_content.c_str(), weekday_str.c_str(), this_time.c_str(), it->second.gang_vec[1].zone.c_str(), it->second.gang_vec[1].gang_name.c_str());
				contents.assign(content_buf);
				LOGIC_PUBLIC->send_mail(GANG_WAR_FIGHT_ONE, it_mm->first, item_list, "", contents);
			}
		}

		gdv = this->gang_view_by_id(it->second.gang_vec[1].gang_id);
		if (gdv) {
			for (Gang_DB_View::Member_Map::iterator it_mm = gdv->member_map.begin();
					it_mm != gdv->member_map.end(); ++it_mm) {
				Item_Vec item_list;
				std::string contents;
				const int size_buf = mailc->mail_content.size() +
						weekday_str.size() * 3 + this_time.size() * 3 +
						sizeof(it->second.gang_vec[0].gang_name) + 10 +
						sizeof(it->second.gang_vec[0].zone) + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->mail_content.c_str(), weekday_str.c_str(), this_time.c_str(), it->second.gang_vec[0].zone.c_str(), it->second.gang_vec[0].gang_name.c_str());
				contents.assign(content_buf);
				LOGIC_PUBLIC->send_mail(GANG_WAR_FIGHT_ONE, it_mm->first, item_list, "", contents);
			}
		}
	}
}

void Gang_Manager::gang_war_sync_gang_data_by_rank_limit_16(Block_Buffer &buf) {
	std::vector<Gang_DB_View_War_Rank> rank_info_vec;
	Gang_DB_View_War_Rank rank_info;
	for (Gang_ID_Map::const_iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end(); ++it_gang) {
		rank_info.reset();
		rank_info.gang_id = it_gang->first;
		rank_info.level = it_gang->second->level;
		rank_info.member_size = it_gang->second->member_map.size();
		rank_info.fund = it_gang->second->fund;
		rank_info.force_total = it_gang->second->force_total;
		rank_info_vec.push_back(rank_info);
	}
	if (! rank_info_vec.empty()) {
		std::sort(rank_info_vec.begin(), rank_info_vec.end());
		std::reverse(rank_info_vec.begin(), rank_info_vec.end());

		uint16_t len = (rank_info_vec.size() < GANG_WAR_ATTEND_NUMS) ? rank_info_vec.size() : GANG_WAR_ATTEND_NUMS;
		buf.write_uint16(len);
		Gang_DB_View* gang_view = NULL;
		for (size_t i = 0; i < rank_info_vec.size() && i < GANG_WAR_ATTEND_NUMS; ++i) {
			gang_view = gang_view_by_id(rank_info_vec[i].gang_id);
			if (gang_view) {
				gang_view->gang_war_serialize(buf);
			}
		}
	} else {
		buf.write_uint16(0);
	}
}

int Gang_Manager::gang_war_sync_data_to_scene(const bool not_create_matched_info) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_GANG_WAR_INFO);
	buf.write_int32(CONFIG_CACHE->server_maintainer_cache().game_inter_flag);
	buf.write_int8(1);
	buf.write_int64(CONFIG_CACHE->server_flag());
	buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag);
	buf.write_int8(gang_war_info_.stage_);
	gang_war_info_.gang_war_spot_rank_map_.clear();
	if (gang_war_info_.stage_ == 2) {
		buf.write_uint16(0);
		gang_war_sync_gang_data_by_rank_limit_16(buf);
		gang_war_clear_last_match_info();
	} else {
		if (!not_create_matched_info || gang_war_info_.gang_war_result_map_.empty()) {
			gang_war_produce_matched_info();
		}
		gang_war_sync_matched_info(buf);
	}
	buf.finish_message();
	LOGIC_MONITOR->send_to_scene(GANG_WAR_SCENE_ID, 0, buf);
	return 0;
}
void Gang_Manager::gang_war_clear_last_match_info(void) {
	gang_war_info_.gang_war_result_map_.clear();
	gang_war_info_.gang_war_matched_gangs_pair_vec_.clear();
	gang_war_info_.gang_war_matched_gang_map_.clear();
//	gang_war_info_.gang_war_spot_rank_map_.clear();
	gang_war_info_.gang_rank_map.clear();
	gang_war_info_.set_activity_from();
//	gang_war_info_.save();
}
void Gang_Manager::gang_war_produce_matched_info(void) {
	if (gang_war_info_.gang_rank_map.empty()) {
		return;
	}
	GangWarGangRankInfo_Vec gang_rank_vec;
	for (GangWarGangRankInfo_Map::iterator it = gang_war_info_.gang_rank_map.begin();
			it != gang_war_info_.gang_rank_map.end(); ++it) {
		gang_rank_vec.push_back(&(it->second));
	}
	std::stable_sort(gang_rank_vec.begin(), gang_rank_vec.end(), Gang_War_Rank_Info_Less());

	gang_war_info_.gang_war_result_map_.clear();
	int8_t index = 0;
	Gang_War_Spot spot;
	Gang_Basic_Detail gbd;
	bool end = false;
	int i = 0;
	for (GangWarGangRankInfo_Vec::const_iterator it = gang_rank_vec.begin(); it != gang_rank_vec.end(); ++it) {
		spot.index = index;
		gbd.reset();
		gbd.gang_id = (**it).gang_id;
		gbd.gang_name = (**it).gang_name;
		gbd.zone = (**it).zone;
		gbd.index = i % 2;
		gbd.rank = i;
		spot.gang_vec.push_back(gbd);
		if (i % 2 == 1) {
			gang_war_info_.gang_war_result_map_.insert(std::make_pair(index, spot));
			spot.reset();
			++index;
			end = true;
			++i;
			continue;
		}
		end = false;
		++i;
	}
	if (!end) {
		gang_war_info_.gang_war_result_map_.insert(std::make_pair(index, spot));
	}
	gang_war_info_.save();
}
void Gang_Manager::gang_war_sync_matched_info(Block_Buffer &buf) {
	buf.write_uint16(gang_war_info_.gang_war_result_map_.size());
	uint16_t len = 0;
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.begin();
			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
		it->second.serialize(buf);

		for (std::vector<Gang_Basic_Detail>::iterator it_vec = it->second.gang_vec.begin(); it_vec != it->second.gang_vec.end(); ++it_vec) {
			Gang_ID_Map::const_iterator it_g = gang_id_map.find(it_vec->gang_id);
			if (it_g != gang_id_map.end()) {
				++len;
			}
		}
	}
	buf.write_uint16(len);
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.begin();
			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
		for (std::vector<Gang_Basic_Detail>::iterator it_vec = it->second.gang_vec.begin(); it_vec != it->second.gang_vec.end(); ++it_vec) {
			Gang_ID_Map::const_iterator it_g = gang_id_map.find(it_vec->gang_id);
			if (it_g != gang_id_map.end()) {
				it_g->second->gang_war_serialize(buf);
			}
		}
	}
}
void Gang_Manager::gang_war_handle_single_spot_when_open(void) {
	for (std::map<int8_t, Gang_War_Spot>::iterator it = gang_war_info_.gang_war_result_map_.begin();
			it != gang_war_info_.gang_war_result_map_.end(); ++it) {
		if (1 == it->second.gang_vec.size()) {
			if (it->second.gang_vec[0].index == 0) { // 0上1下
				it->second.result = 1;
			} else {
				it->second.result = 2;
			}
			gang_war_info_.gang_rank_map[it->second.gang_vec[0].gang_id].score += CONFIG_CACHE_GANG->gang_war_setting_cfg().empty_score;
			gang_war_info_.gang_rank_map[it->second.gang_vec[0].gang_id].win_nums += 1;
//			GangWarGangRankInfo_Map::iterator it_grm = gang_war_info_.gang_rank_map.find(it->second.gang_vec[0].gang_id);
//			if (it_grm != gang_war_info_.gang_rank_map.end()) {
//				it_grm->second.score += CONFIG_CACHE_GANG->gang_war_setting_cfg().empty_score;
//			}
			gang_war_award_single_spot(it->second);
		} else {
			int8_t spot_len = 0;
			for (std::vector<Gang_Basic_Detail>::iterator it_vec = it->second.gang_vec.begin(); it_vec != it->second.gang_vec.end(); ++it_vec) {
				if (it_vec->gang_id > 0) {
					++spot_len;
				}
			}
			if (1 == spot_len) {
				for (std::vector<Gang_Basic_Detail>::iterator it_vec = it->second.gang_vec.begin(); it_vec != it->second.gang_vec.end(); ++it_vec) {
					if (it_vec->gang_id == 0) {
						continue;
					}
					if (it_vec->index == 0) { // 0上1下
						it->second.result = 1;
					} else {
						it->second.result = 2;
					}
					gang_war_info_.gang_rank_map[it_vec->gang_id].score += CONFIG_CACHE_GANG->gang_war_setting_cfg().empty_score;
					gang_war_info_.gang_rank_map[it_vec->gang_id].win_nums += 1;
//					GangWarGangRankInfo_Map::iterator it_grm = gang_war_info_.gang_rank_map.find(it_vec->gang_id);
//					if (it_grm != gang_war_info_.gang_rank_map.end()) {
//						it_grm->second.score += CONFIG_CACHE_GANG->gang_war_setting_cfg().empty_score;
//					}
//					gang_war_single_spot_direct_up(it->second);
					gang_war_award_single_spot(it->second);
					break;
				}
				continue;
			}
		}
	}
}

void Gang_Manager::gang_war_tick(const Time_Value& now) {
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	Date_Time dt_game_open_time(server_maintainer.game_open_time);
	dt_game_open_time.hour(0);
	dt_game_open_time.minute(0);
	dt_game_open_time.second(0);
	dt_game_open_time.microsec(0);
	int week_open = dt_game_open_time.weekday();
	if (week_open == 0) {
		week_open = 7;
	}
	int week_gap = 7 - week_open + 1;
	Time_Value next_week_day_start = Time_Value(week_gap * Time_Value::ONE_DAY_IN_SECS + dt_game_open_time.time_sec());
	if (now < next_week_day_start && week_open > 1) { // 周一开的服，走正常流程
		gang_war_tick_first_week(now, week_open);
	} else {
		gang_war_tick_normal(now);
	}
	if (gang_war_info_.draw_time_ != Time_Value::zero && gang_war_info_.draw_time_ < now) {
		gang_war_info_.draw_time_ = Time_Value::zero;
		if (!gang_war_info_.gang_war_matched_gang_map_.empty()) {
			for (GangWarMatchedGangInfo_Map::iterator it = gang_war_info_.gang_war_matched_gang_map_.begin();
					it != gang_war_info_.gang_war_matched_gang_map_.end(); ++it) {
				it->second.btn_state = 1;
			}
			gang_war_info_.save();
		}
		gang_war_send_mail_for_start();
	}
}
void Gang_Manager::gang_war_tick_normal(const Time_Value &now) {
	Date_Time dt(now);
	if (dt.weekday() == 1) {
		if (dt.hour() == 11 && dt.minute() > 40 && dt.minute() < 45
				&& gang_war_info_.switch_ != GANG_WAR_STOP) {
			close_activity();
			MSG_USER("GANG_WAR_CLOSE");
		}
		if (dt.hour() == 11 && dt.minute() > 50
				&& gang_war_info_.stage_ != 2
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			gang_war_start_sync(2);
			MSG_USER("GANG_WAR_SYNC 2");
		}
		if (dt.hour() == 12 && dt.minute() < 5
				&& gang_war_info_.stage_ == 2
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			create_mateched_16();
			MSG_USER("GANG_WAR_READY 2");
		}
	}
	int state = 0;
	switch(dt.weekday()) {
	case 1:
	case 3:
	case 5:
	case 0: {
		state = 1;
		break;
	}
	default: {
		break;
	}
	}
	if (state != 0) {
		if (dt.hour() == 19 && dt.minute() > 40 && dt.minute() < 45
				&& gang_war_info_.switch_ != GANG_WAR_STOP) {
			close_activity();
			MSG_USER("GANG_WAR_CLOSE %d", state);
		}
		if (dt.hour() == 19 && dt.minute() > 45 && dt.minute() < 50
				&& gang_war_info_.stage_ != state
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			gang_war_start_sync(state, true);
			MSG_USER("GANG_WAR_SYNC %d", state);
		}
		if (dt.hour() == 19 && dt.minute() > 50 && dt.minute() < 60
				&& gang_war_info_.stage_ == state
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			ready_activity();
			MSG_USER("GANG_WAR_READY %d", state);
		}
		if (dt.hour() == 20 && dt.minute() >= 0 && dt.minute() < 10
				&& gang_war_info_.stage_ == state
				&& gang_war_info_.switch_ == GANG_WAR_READY) {
			open_activity();
			MSG_USER("GANG_WAR_OPEN %d", state);
		}
		if (dt.hour() == 20 && dt.minute() == 26
				&& gang_war_info_.stage_ == state
				&& gang_war_info_.switch_ == GANG_WAR_START) {
			close_activity();
			gang_war_ready_mail();
			MSG_USER("GANG_WAR_CLOSE %d", state);
		}

		////////next-------------------------
		if (dt.hour() == 20 && dt.minute() == 27
				&& gang_war_info_.stage_ != state
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			gang_war_start_sync(state, true);
			MSG_USER("GANG_WAR_SYNC %d", state);
		}
		if (dt.hour() == 20 && dt.minute() == 29
				&& gang_war_info_.stage_ == state
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			ready_activity();
			MSG_USER("GANG_WAR_READY %d", state);
		}
		if (dt.hour() == 20 && dt.minute() >= 30 && dt.minute() < 40
				&& gang_war_info_.stage_ == state
				&& gang_war_info_.switch_ == GANG_WAR_READY) {
			open_activity();
			MSG_USER("GANG_WAR_OPEN %d", state);
		}
		if (dt.hour() == 21 && dt.minute() >= 0 && dt.minute() < 10
				&& gang_war_info_.stage_ == state
				&& gang_war_info_.switch_ == GANG_WAR_START) {
			close_activity();
			gang_war_ready_mail();
			MSG_USER("GANG_WAR_CLOSE %d", state);
		}
	}
}

void Gang_Manager::gang_war_tick_first_week(const Time_Value &now, const int week_open) {
	Date_Time dt(now);
	const int gang_war_first_open_time = gang_war_tick_get_first_open_time(week_open);
	int weekday = dt.weekday();
	if (0 == weekday) {
		weekday = 7;
	}
	if (gang_war_first_open_time == -1
			|| gang_war_first_open_time > weekday) {
		return;
	}
	const int state = gang_war_tick_get_today_state(gang_war_first_open_time);
	if (state == 0) {
		return ;
	}
	bool is_start_day = false;
	if (gang_war_first_open_time == weekday) {
		is_start_day = true;
	}
	if (is_start_day) {
		if (dt.hour() == 11 && dt.minute() > 40 && dt.minute() < 45
				&& gang_war_info_.switch_ != GANG_WAR_STOP) {
			close_activity();
			MSG_USER("GANG_WAR_CLOSE");
		}
		if (dt.hour() == 11 && dt.minute() > 50
				&& gang_war_info_.stage_ != 2
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			gang_war_start_sync(2);
			MSG_USER("GANG_WAR_SYNC 2");
		}
		if (dt.hour() == 12 && dt.minute() < 5
				&& gang_war_info_.stage_ == 2
				&& gang_war_info_.switch_ == GANG_WAR_STOP) {
			create_mateched_16();
			MSG_USER("GANG_WAR_READY 2");
		}
	}

	////////////first---------------------------------
	if (dt.hour() == 19 && dt.minute() > 40 && dt.minute() < 45
			&& gang_war_info_.switch_ != GANG_WAR_STOP) {
		close_activity();
		MSG_USER("GANG_WAR_CLOSE %d", state);
	}
	if (dt.hour() == 19 && dt.minute() > 45 && dt.minute() < 50
			&& gang_war_info_.stage_ != state
			&& gang_war_info_.switch_ == GANG_WAR_STOP) {
		gang_war_start_sync(state, true);
		MSG_USER("GANG_WAR_SYNC %d", state);
	}
	if (dt.hour() == 19 && dt.minute() > 50 && dt.minute() < 60
			&& gang_war_info_.stage_ == state
			&& gang_war_info_.switch_ == GANG_WAR_STOP) {
		ready_activity();
		MSG_USER("GANG_WAR_READY %d", state);
	}
	if (dt.hour() == 20 && dt.minute() >= 0 && dt.minute() < 10
			&& gang_war_info_.stage_ == state
			&& gang_war_info_.switch_ == GANG_WAR_READY) {
		open_activity();
		MSG_USER("GANG_WAR_OPEN %d", state);
	}
	if (dt.hour() == 20 && dt.minute() == 26
			&& gang_war_info_.stage_ == state
			&& gang_war_info_.switch_ == GANG_WAR_START) {
		close_activity();
		gang_war_ready_mail();
		MSG_USER("GANG_WAR_CLOSE %d", state);
	}

	////////next-------------------------
	if (dt.hour() == 20 && dt.minute() == 27
			&& gang_war_info_.stage_ != state
			&& gang_war_info_.switch_ == GANG_WAR_STOP) {
		gang_war_start_sync(state, true);
		MSG_USER("GANG_WAR_SYNC %d", state);
	}
	if (dt.hour() == 20 && dt.minute() == 29
			&& gang_war_info_.stage_ == state
			&& gang_war_info_.switch_ == GANG_WAR_STOP) {
		ready_activity();
		MSG_USER("GANG_WAR_READY %d", state);
	}
	if (dt.hour() == 20 && dt.minute() >= 30 && dt.minute() < 40
			&& gang_war_info_.stage_ == state
			&& gang_war_info_.switch_ == GANG_WAR_READY) {
		open_activity();
		MSG_USER("GANG_WAR_OPEN %d", state);
	}
	if (dt.hour() == 21 && dt.minute() >= 00 && dt.minute() < 10
			&& gang_war_info_.stage_ == state
			&& gang_war_info_.switch_ == GANG_WAR_START) {
		close_activity();
		gang_war_ready_mail();
		MSG_USER("GANG_WAR_CLOSE %d", state);
	}
}
int Gang_Manager::gang_war_tick_get_first_open_time(const int week_open) {
	switch(week_open) {
	case 1:
	case 2: {
		return 3;
	}
	case 3:
	case 4: {
		return 5;
	}
	case 5:
	case 6:  {
		return 7;
	}
	case 7: {
		return -1;
	}
//	case 1:{
//		return 2;
//	}
//	case 2:
//	case 3: {
//		return 4;
//	}
//	case 4:
//	case 5:  {
//		return 6;
//	}
//	case 6:  {
//		return 7;
//	}
//	case 7: {
//		return -1;
//	}
	default:
		break;
	}
	return -1;
}
int Gang_Manager::gang_war_tick_get_today_state(const int start_day) {
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	int weekday = dt.weekday();
	if (weekday == 0) {
		weekday = 7;
	}
	const int gap = weekday - start_day;
	if (gap < 0) {
		return 0;
	}
	switch(gap) {
	case 0: {
		return 1;
	}
	case 1: {
		if (dt.weekday() == 0) {
			return 1;
		}
		break;
	}
	case 2: {
		return 1;
	}
	case 3: {
		if (dt.weekday() == 0) {
			return 1;
		}
		break;
	}
	case 4: {
		return 1;
	}
	case 5: {
		return 1;
	}
	default:
		break;
	}

	return 0;
}

void Gang_Manager::gang_boss_tick(const Time_Value& now){
	int boss_open_lv = CONFIG_CACHE_GANG->get_open_gang_boss_lv();
	Gang_ID_Map::iterator it;
	for (it = gang_id_map.begin(); it != gang_id_map.end(); ++it) {
		if(it->second->level < boss_open_lv){
			continue;
		}
		if(it->second->boss_act_state == 0){// cal
			if(it->second->boss_act != 0){
				continue;
			}
			Time_Value end_time = it->second->boss_act_time + Time_Value(30*60);
			Time_Value ready_time = it->second->boss_act_time - Time_Value(5*60);
			if(now >= end_time){// close
				it->second->boss_act_state = 1;
				it->second->boss_act_next_time = it->second->boss_act_time + Time_Value(24*60*60);
				it->second->boss_act = 1;
			}else if(now >= it->second->boss_act_time){// open
				it->second->boss_act_state = 3;
				it->second->boss_act_next_time = it->second->boss_act_time + Time_Value(30*60);
				it->second->boss_act = 1;
				// send to menber
				int remain_time = it->second->boss_act_next_time.sec() - now.sec();
				if(remain_time < 0){
					remain_time = 0;
				}
				GANG_BOSS_ACTIVE_MANAGER->gang_boss_open(it->second->gang_id, 25*60,
						remain_time, it->second->boss_kill_num);
				this->gang_boss_act_notice(it->second, it->second->boss_act_state, remain_time);
			}else if(now >= ready_time){
				it->second->boss_act_state = 2;
				it->second->boss_act_next_time = it->second->boss_act_time;
				int remain_time = it->second->boss_act_next_time.sec() - now.sec();
				if(remain_time < 0){
					remain_time = 0;
				}
				this->gang_boss_act_notice(it->second, it->second->boss_act_state, remain_time);
				// send to menber
			}else{
				it->second->boss_act_state = 1;
				it->second->boss_act_next_time = it->second->boss_act_time - Time_Value(5*60);
			}
		}else if(it->second->boss_act_state == 1){// close
			if(it->second->boss_act != 0){
				continue;
			}
			if(now > it->second->boss_act_next_time){
				it->second->boss_act_state = 2;
				it->second->boss_act_next_time = it->second->boss_act_time;
				// send to menber
				int remain_time = it->second->boss_act_next_time.sec() - now.sec();
				if(remain_time < 0){
					remain_time = 0;
				}
				this->gang_boss_act_notice(it->second, it->second->boss_act_state, remain_time);

			}
		}else if(it->second->boss_act_state == 2){// ready
			if(now > it->second->boss_act_next_time){
				it->second->boss_act_state = 3;
				it->second->boss_act_next_time = it->second->boss_act_time + Time_Value(30*60);
				// send to menber
				int remain_time = it->second->boss_act_next_time.sec() - now.sec();
				if(remain_time < 0){
					remain_time = 0;
				}
				GANG_BOSS_ACTIVE_MANAGER->gang_boss_open(it->second->gang_id, 25*60, remain_time,
						it->second->boss_kill_num);
				this->gang_boss_act_notice(it->second, it->second->boss_act_state, remain_time);
			}

		}else if(it->second->boss_act_state == 3){// open
			if(now > it->second->boss_act_next_time){
				it->second->boss_act_state = 1;
				it->second->boss_act_next_time = it->second->boss_act_time + Time_Value(24*60*60);
			}
		}
	}
}

int Gang_Manager::get_boss_kill_info(const gang_id_t gang_id, int& kill_num, int& boss_lv, int& hour, int& minute){
	Gang_ID_Map::iterator it = gang_id_map.find(gang_id);
	if(it != gang_id_map.end()){
		kill_num = it->second->boss_kill_num;
		boss_lv = it->second->boss_lv;
		hour = it->second->boss_hour;
		minute = it->second->boss_minute;
	}
	return 0;
}

int Gang_Manager::set_boss_open_time(const gang_id_t gang_id, const int8_t hour, const int8_t minute, int8_t& r_hour, int8_t& r_minute){
	Gang_ID_Map::iterator it = gang_id_map.find(gang_id);
	if(it == gang_id_map.end()){
		return ERROR_GANG_NOT_EXIST;
	}
	r_hour = it->second->boss_hour;
	r_minute = it->second->boss_minute;
	if(hour == 0 && minute == 0){
		return ERROR_WB_SET_TIME_LIMIT;
	}
	if(it->second->boss_act == 0){// 今天没有打,设置时间不能比当前的时间小于30分钟
		Time_Value now = Time_Value::gettimeofday();
		Date_Time date_now(now);
		date_now.hour(it->second->boss_hour);
		date_now.minute(it->second->boss_minute);
		date_now.second(0);
		Time_Value set_time = Time_Value(date_now.time_sec());
		if(set_time.sec() - now.sec() < 30*60){
			return ERROR_WB_SET_TIME_INVALID;
		}
		it->second->boss_act_state = 0;
		it->second->boss_act_time = set_time;
	}else{
		Time_Value now = Time_Value::gettimeofday();
		Time_Value tg_time = now + Time_Value(Time_Value::ONE_DAY_IN_SECS);
		Date_Time date_now(tg_time);
		date_now.hour(it->second->boss_hour);
		date_now.minute(it->second->boss_minute);
		date_now.second(0);
		Time_Value set_time = Time_Value(date_now.time_sec());
		if(set_time.sec() - now.sec() < 30*60){
			return ERROR_WB_SET_TIME_INVALID;
		}
	}
	it->second->boss_hour = hour;
	it->second->boss_minute = minute;
	it->second->change_view(true);
	r_hour = it->second->boss_hour;
	r_minute = it->second->boss_minute;
	return 0;
}

int Gang_Manager::cmd_set_boss_lv(const gang_id_t gang_id, const int lv){
	if(lv <= 0){
		return -1;
	}
	Gang_ID_Map::iterator it = gang_id_map.find(gang_id);
	if(it == gang_id_map.end()){
		return ERROR_GANG_NOT_EXIST;
	}
	if(it->second->boss_act_state == 3){
		return -1;
	}
	it->second->boss_kill_num = CONFIG_CACHE_WORLD_BOSS->get_boss_kill_num_from_lv(lv, 19);
	it->second->boss_lv = lv;
	return 0;
}

int Gang_Manager::cmd_set_boss_kill_num(const gang_id_t gang_id, const int kill_num){
	Gang_ID_Map::iterator it = gang_id_map.find(gang_id);
	if(it == gang_id_map.end()){
		return ERROR_GANG_NOT_EXIST;
	}
	if(it->second->boss_act_state == 3){
		return -1;
	}
	it->second->boss_kill_num = kill_num;
	it->second->boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(kill_num, gang_boss_act_icon_id);
	return 0;
}

int Gang_Manager::set_boss_act_result(const gang_id_t gang_id, const int result){
	Gang_ID_Map::iterator it = gang_id_map.find(gang_id);
	if(it == gang_id_map.end()){
		return ERROR_GANG_NOT_EXIST;
	}
	if(result != 0){// 0：击杀boss失败， 1：普遍击杀boss 2：快速击杀boss
		if(result == 1){
			it->second->boss_kill_num += 1;
			it->second->boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(
								it->second->boss_kill_num, gang_boss_act_icon_id);
		}else{
			it->second->boss_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_lv_from_kill_num(
								it->second->boss_kill_num, gang_boss_act_icon_id);
			int boss_max_lv = CONFIG_CACHE_WORLD_BOSS->get_boss_max_lv(gang_boss_act_icon_id);
			if(it->second->boss_lv < boss_max_lv){
				it->second->boss_lv += 1;
				it->second->boss_kill_num = CONFIG_CACHE_WORLD_BOSS->get_boss_kill_num_from_lv(
						it->second->boss_lv, gang_boss_act_icon_id);
			}else{
				it->second->boss_kill_num += 1;
			}

		}
		it->second->boss_act_state = 3;
		it->second->boss_act_next_time = Time_Value::gettimeofday() + Time_Value(5*60);
		it->second->change_view(true);
	}else{
		it->second->boss_act_state = 1;
		it->second->boss_act_next_time = it->second->boss_act_time + Time_Value(24*60*60);
	}
	// send to menber
	int remain_time = 5*60;
	gang_boss_act_notice(it->second, it->second->boss_act_state, remain_time);
	return 0;
}

void Gang_Manager::set_gang_boss_join_flag(const role_id_t role_id, const gang_id_t gang_id){
	gang_boss_join_flag_map_[role_id] = gang_id;
}

bool Gang_Manager::can_join_gang_boss(const role_id_t role_id, const gang_id_t gang_id){
	Gang_Boss_Join_flag_Map::iterator it = gang_boss_join_flag_map_.find(role_id);
	if(it == gang_boss_join_flag_map_.end()){
		return true;
	}
	if(gang_id == it->second){
		return true;
	}
	return false;
}

bool Gang_Manager::is_in_gang(const role_id_t role_id, const gang_id_t gang_id) {
	Gang_DB_View* gang_view = gang_view_by_id(gang_id);
	if(gang_view == 0){
		return false;
	}
	std::map<role_id_t, Gang_Member_Detail>::iterator iter = gang_view->member_map.find(role_id);
	if(iter == gang_view->member_map.end()) {
		return false;
	}
	return true;
}

void Gang_Manager::gang_boss_act_notice(const Gang_DB_View* gang_view, const int state, const int remain_time){
	if(!gang_view){
		return;
	}
	Gang_DB_View::Member_Map::const_iterator it;
	for (it = gang_view->member_map.begin();it != gang_view->member_map.end(); ++it){
		role_id_t role_id = it->first;
		Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
		if(!player){
			continue;
		}
		player->send_gang_boss_active_to_client(state, remain_time);
	}
}

void Gang_Manager::get_gang_boss_act_state(const role_id_t gang_id, int& state, int& remain_time){
	Gang_ID_Map::const_iterator it = gang_id_map.find(gang_id);
	if (it != gang_id_map.end()){
		state = it->second->boss_act_state;
		remain_time = it->second->boss_act_next_time.sec() - Time_Value::gettimeofday().sec();
		if(remain_time < 0){
			remain_time = 0;
		}
	}
}

void Gang_Manager::gang_war_clear_assigned_award_list(void) {
	Date_Time dt(Time_Value::gettimeofday());
	if (dt.weekday() != 1) {
		return;
	}
	for (Gang_ID_Map::iterator it_gang = gang_id_map.begin(); it_gang != gang_id_map.end();++it_gang) {
		for (boost::unordered_map<int64_t, Reward_Info_Gang_War>::iterator it_award = it_gang->second->gang_war_award.begin();
				it_award != it_gang->second->gang_war_award.end();) {
			if (it_award->second.role_id != 0) {
				it_award = it_gang->second->gang_war_award.erase(it_award);
			} else {
				++it_award;
			}
		}
	}
}
void Gang_Manager::gang_war_produce_rank_award_when_all_close(void) {
	gang_war_info_.rank_award_.clear();
	gang_war_info_.gang_id_rank_award_.clear();
	if (gang_war_info_.gang_rank_map.empty()) {
		return;
	}
	GangWarGangRankInfo_Vec gang_rank_vec;
	for (GangWarGangRankInfo_Map::iterator it = gang_war_info_.gang_rank_map.begin();
			it != gang_war_info_.gang_rank_map.end(); ++it) {
		gang_rank_vec.push_back(&(it->second));
	}
	std::stable_sort(gang_rank_vec.begin(), gang_rank_vec.end(), Gang_War_Rank_Info_Less());

	for (GangWarGangRankInfo_Vec::const_iterator it = gang_rank_vec.begin(); it != gang_rank_vec.end(); ++it) {
		Gang_War_Rank_Award_Detail gwrad;
		gwrad.gang_id = (**it).gang_id;
		gwrad.gang_name = (**it).gang_name;
		gwrad.zone = (**it).zone;
		gwrad.score = (**it).score;
		gwrad.win_nums = (**it).win_nums;
		gwrad.kill_nums = (**it).kill_nums;
		gwrad.force = (**it).force;
		gang_war_info_.gang_id_rank_award_.insert(std::make_pair(gwrad.gang_id, gwrad));
	}
	if (!gang_war_info_.gang_id_rank_award_.empty()) {
		std::vector<Gang_War_Rank_Award_Detail*> gwrad_vec;
		for (Gang_War_Gang_Id_Rank_Award::iterator it_gira = gang_war_info_.gang_id_rank_award_.begin();
				it_gira != gang_war_info_.gang_id_rank_award_.end(); ++it_gira) {
			gwrad_vec.push_back(&(it_gira->second));
		}
		std::stable_sort(gwrad_vec.begin(), gwrad_vec.end(), Gang_War_Rank_Award_Detail_Less());
		int8_t i = 1;
		std::vector<Gang_War_Rank_Award_Detail*>::iterator it_gv = gwrad_vec.begin();
		for (; it_gv != gwrad_vec.end(); ++it_gv) {
			(**it_gv).rank = i;
			gang_war_info_.rank_award_.insert(std::make_pair(i, (**it_gv)));

			Gang_DB_View *gdv = this->gang_view_by_id((**it_gv).gang_id);
			if (gdv) {
				for (Gang_DB_View::Member_Map::const_iterator it_mm = gdv->member_map.begin();
						it_mm != gdv->member_map.end(); ++it_mm) {
					if (it_mm->second.headship == GANG_HEADSHIP_VICE
							|| it_mm ->second.headship == GANG_HEADSHIP_LEADER) {
						LOGIC_PUBLIC->send_mail(GANG_WAR_RANK_AWARD_GET, it_mm->first);
					}
					if (it_mm ->second.headship == GANG_HEADSHIP_LEADER && i == 1) {
						 TITLE_MANAGER->add_gang_war_title(it_mm->first);
					}
				}
			}
			++i;
		}
	} else {
		TITLE_MANAGER->add_gang_war_title(0);
	}
	gang_war_info_.save();
}
void Gang_Manager::gang_war_rank_award_auto_get(void) {
	int mail_id = 1039;
	for (Gang_War_Rank_Award::iterator it_ra = gang_war_info_.rank_award_.begin();
			it_ra != gang_war_info_.rank_award_.end(); ++it_ra) {
		if (it_ra->second.state == 1) {
			continue;
		}
		it_ra->second.state = 1;

		Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(it_ra->second.gang_id);
		if (!gang_view) {
			continue;
		}

		int all_contribute = 0;
		Item_Detail item_detail;
		Item_Vec item_vec;
		std::vector<Item_Basic_Info> item_vec_all;
		Item_Basic_Info ibi;
		const Gang_War_Rank_Award_Detail_Cfg *cfg = CONFIG_CACHE_GANG->find_gang_war_rank_award(it_ra->second.rank);
		if (!cfg) {
			continue;
		}
		int fund = 0;
		Reward_Info_Gang_War rigw;
		for (Item_Info_Vec::const_iterator it = cfg->list.begin(); it != cfg->list.end(); ++it) {
			rigw.reset();
			rigw.award.reward_id = it->id;
			rigw.award.quantity = it->amount;
			if (rigw.award.reward_id == 503803) {
				fund = rigw.award.quantity;
				continue;
			}
			int32_t amount = rigw.award.quantity;
			for (int32_t i = 0; i < amount; i++) {
				rigw.award.quantity = 1;
				rigw.index = gang_view->make_gang_war_index();
				gang_view->gang_war_award_map().insert(std::make_pair(rigw.index, rigw));
			}
			gang_view->change_view();
		}
		if (fund > 0) {
			gang_view->add_contribute(fund);
			GANG_MANAGER->refresh_rank();
			gang_view->change_view();
			// notify ganger info, gang info
			GANG_MANAGER->notify_gang_info_update(*gang_view);
		}

		for (Item_Info_Vec::const_iterator it = cfg->all_list.begin();
				it != cfg->all_list.end(); ++it) {
			switch (it->id) {
			case 503803: {
				all_contribute = it->amount;
				break;
			}
			default:
				item_detail.reset();
				item_detail.id_ = it->id;
				item_detail.amount_ = it->amount;
				item_detail.bind_ = Item_Detail::BIND;
				item_vec.push_back(item_detail);
				ibi.reset();
				item_detail.set_item_basic_info(ibi);
				item_vec_all.push_back(ibi);
				break;
			}
		}
		if (item_vec_all.empty()) {
			continue;
		}
		const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
		if(!mailc) {
			continue;
		}
		std::string contents;
		const int size_buf = mailc->mail_content.size() + \
				sizeof(it_ra->second.rank) + 10;
		char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), it_ra->second.rank);
		contents.assign(content_buf);

		for (Gang_DB_View::Member_Map::iterator it_mm = gang_view->member_map.begin();
				it_mm != gang_view->member_map.end(); ++it_mm) {
			Logic_Player *pl = LOGIC_MONITOR->find_logic_player_by_role_id(it_mm->first);
			if (pl) {
				if (all_contribute > 0) {
					pl->add_contribute(all_contribute);
				}
//						if (self_mercit > 0) {
//							pl->change_exploit_val(self_mercit);
//						}
			}
			LOGIC_PUBLIC->send_mail(GANG_WAR_RANK_AWARD, it_mm->first, item_vec_all, mailc->mail_title, contents);
		}
	}
	gang_war_info_.rank_award_.clear();
	gang_war_info_.gang_id_rank_award_.clear();
	gang_war_info_.save();
}
void Gang_Manager::gang_war_handle_no_battle_spot(void) {
	for (std::map<int8_t, Gang_War_Spot>::const_iterator it_gwrm = gang_war_info_.gang_war_result_map_.begin();
			it_gwrm != gang_war_info_.gang_war_result_map_.end(); ++it_gwrm) {
		for (std::vector<Gang_Basic_Detail>::const_iterator it = it_gwrm->second.gang_vec.begin(); it != it_gwrm->second.gang_vec.end(); ++it) {
			if (0 == gang_war_info_.gang_rank_map.count(it->gang_id)) {
				gang_war_info_.gang_rank_map[it->gang_id].gang_id = it->gang_id;
				gang_war_info_.gang_rank_map[it->gang_id].gang_name = it->gang_name;
				gang_war_info_.gang_rank_map[it->gang_id].zone = it->zone;
				const Gang_War_Matched_Gang_Info *gwmgi = gang_war_find_matched_gang_info(it->gang_id);
				if (gwmgi) {
					gang_war_info_.gang_rank_map[it->gang_id].force = gwmgi->force;
				}
			}
		}
	}
}
