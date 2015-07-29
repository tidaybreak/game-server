/*
 * Active_Content_Manager.cpp
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#include "Active_Content_Manager.h"
#include "DB_Operator.h"
#include "Game_Typedef.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Msg_Struct.h"
#include "Global_Timer.h"
#include "Config_Cache_Active_Content.h"
#include "Active_Def.h"
#include "Config_Cache.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Logic_Player_Struct.h"
#include "ranking/Ranking_Manager.h"
#include "Logic_Public.h"
#include "Config_Cache_Mail.h"
#include "Active_Content_Def.h"
#include "arena/Arena_Manager.h"
#include "DB_Def.h"
#include "gang/Gang_Manager.h"
#include "announce/Logic_Announce_Manager.h"

Active_Content_Manager::Active_Content_Manager() {
	reset();
}

Active_Content_Manager::~Active_Content_Manager() {

}

void Active_Content_Manager::reset(void) {
	can_reset_count = false;
	need_save_db = false;
	icon_time_status.reset();
	icon_limit_level = 0;
	//限时抢购 - start
	limit_buy_num.clear(); //ID,对应购买次数
	limit_buy_start_time = Time_Value::zero; //开始时间
	limit_buy_end_time = Time_Value::zero; //结束时间
	//限时抢购 - end

	//VIP俱乐部 - start
	vip_club_start_time = Time_Value::zero;//开始时间
	vip_club_end_time = Time_Value::zero;//结束时间
	vip_club_level_num.clear(); //VIP等级;对应的人数;
	//VIP俱乐部 - end

	//冲级高手 - start
	level_ace_start_time = Time_Value::zero;//开始时间
	level_ace_end_time = Time_Value::zero;//结束时间
	//冲级高手 - end

	//套装收集 - start
	suit_collect_start_time = Time_Value::zero;//开始时间
	suit_collect_end_time = Time_Value::zero;//结束时间
	//套装收集 - end

	//公会升级 - start
	guild_level_start_time = Time_Value::zero;//开始时间
	guild_level_end_time = Time_Value::zero;//结束时间
//	guild_announce_ed_level.clear();
	//公会升级 - end

	//等级排名 - start
	level_rank_start_time = Time_Value::zero;//开始时间
	level_rank_count_time = Time_Value::zero;//结算时间
	level_rank_end_time = Time_Value::zero;//结束时间
	//等级排名 -end

	//竞技场排名 - start
	arena_rank_start_time = Time_Value::zero;//开始时间
	arena_rank_count_time = Time_Value::zero;//结算时间
	arena_rank_end_time = Time_Value::zero;//结束时间
	//竞技场排名 - end

	//战斗力排名 - start
	force_rank_start_time = Time_Value::zero;//开始时间
	force_rank_count_time = Time_Value::zero;//结算时间
	force_rank_end_time = Time_Value::zero;//结束时间
	//战斗力排名 - end

	//上古副本排名 - start
	sg_rank_start_time = Time_Value::zero;//开始时间
	sg_rank_count_time = Time_Value::zero;//结算时间
	sg_rank_end_time = Time_Value::zero;//结束时间
	//上古副本排名 - end

	mount_rank_start_time = Time_Value::zero;//开始时间
	mount_rank_count_time = Time_Value::zero;//结算时间
	mount_rank_end_time = Time_Value::zero;//结束时间

	wing_rank_start_time = Time_Value::zero;//开始时间
	wing_rank_count_time = Time_Value::zero;//结算时间
	wing_rank_end_time = Time_Value::zero;//结束时间

	//竞技场段位 - start
	arena_dan_start_time = Time_Value::zero;//开始时间
	arena_dan_end_time = Time_Value::zero;//结束时间
	//竞技场段位 - end

	common_start_time.clear();
	common_type_start_time.clear();
	common_end_time.clear();
	comm_act_type_id.clear();

	common_icon_info.clear();
}

void Active_Content_Manager::start(void) {
	MONGO_OPERATOR->load_active_content(c);
	count_vip_num_on_start();
}
void Active_Content_Manager::reset_vip_announce_ed_level(void) {
	c.vip_announce_ed_level.clear();
	need_save_db = true;
}

void Active_Content_Manager::reset_count_flag(void) {
	can_reset_count = true;
}

void Active_Content_Manager::tick(void) {
	if(need_save_db) {
		need_save_db = false;
		Block_Buffer buf;
		buf.make_message(MONGO_DATA_ACTIVE_CONTENT);
		buf.write_uint32(ACTIVE_CONTENT_MONGO_SAVE);
		c.serialize(buf);
		buf.finish_message();
		DB_MANAGER->push_data_block(buf, 0);
	}
}

void Active_Content_Manager::open_icon(const uint32_t icon_id,const Time_Value &close_time, const uint32_t level, const uint32_t act_id) {
	icon_time_status.icon_id = icon_id;
	if(close_time.sec() > icon_time_status.over_time.sec()) {
		icon_time_status.act_id = act_id;
	}
	icon_time_status.over_time = close_time;
	icon_limit_level = level;
	//效果buf
	MSG_81000408 effec_msg;
	effec_msg.open = 1;
	effec_msg.icon_id = icon_id;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服效果,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(icon_limit_level);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}
void Active_Content_Manager::close_icon(uint32_t icon_id, const uint32_t act_id) {
	if(icon_time_status.act_id != act_id) {
		return;
	}
	icon_time_status.reset();
	//效果buf
	MSG_81000408 effec_msg;
	effec_msg.open = 0;
	effec_msg.icon_id = icon_id;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服效果,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(0);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}


void Active_Content_Manager::common_open_icon(const uint32_t icon_id, const Time_Value &close_time, const uint32_t level,const Time_Value & dtime, const int8_t active) {
	int time =  dtime.sec() - Time_Value::gettimeofday().sec();
	if(time < 0) { time = 0; }
	Common_Active_Icon_Info icon_;
	icon_.reset();
	icon_.icon_id = icon_id;
	icon_.over_time = close_time;
	icon_.level_limit = level;
	icon_.active = active;
	icon_.down_time = dtime;
	common_icon_info[icon_id] = icon_;
	//效果buf
	MSG_81000408 effec_msg;
	effec_msg.open = 1;
	effec_msg.icon_id = icon_id;
	effec_msg.active = active;
	effec_msg.time = time;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服效果,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(icon_limit_level);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}
void Active_Content_Manager::common_close_icon(const uint32_t icon_id) {
	Common_Active_Icon_Info_Map::iterator it = common_icon_info.find(icon_id);
	if(it != common_icon_info.end()) {
		common_icon_info.erase(it);
	}
	//效果buf
	MSG_81000408 effec_msg;
	effec_msg.open = 0;
	effec_msg.icon_id = icon_id;
	Block_Buffer effec_buf;
	effec_buf.make_message(effec_msg.msg_id);
	effec_msg.serialize(effec_buf);
	effec_buf.finish_message();
	//全服效果,由聊天服转发
	Block_Buffer buf;
	buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
	buf.write_int32(0);
	buf.copy(&effec_buf);
	buf.finish_message();
	LOGIC_MONITOR->send_to_local_chat(buf);
}

Common_Active_Icon_Info_Map &Active_Content_Manager::get_common_icon(void) {
	return common_icon_info;
}

Icon_Time_Info &Active_Content_Manager::get_icon_time_status(void) {
	return icon_time_status;
}

uint32_t Active_Content_Manager::get_icon_limit_level(void) {
	return icon_limit_level;
}

void Active_Content_Manager::open_limit_buy(const Time_Value &start, const Time_Value &end) {
	limit_buy_num.clear();
	limit_buy_start_time = start;
	limit_buy_end_time = end;
}

void Active_Content_Manager::close_limit_buy(void) {
	limit_buy_end_time = Time_Value::gettimeofday();
}

//void Active_Content_Manager::add_limit_buy_num(uint32_t id, uint32_t num) {
//	//增加购买次数
//	UInt_UInt_Map::iterator num_iter = limit_buy_num.find(id);
//	if(num_iter != limit_buy_num.end()) {
//		++num_iter->second;
//	} else {
//		limit_buy_num.insert(std::make_pair(id, 1));
//	}
//}

//Limit_Buy_Config* Active_Content_Manager::get_limit_buy_info(uint32_t id) {
//	const Limit_Buy_Config *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_limit_buy_conf_by_id(id);
//	if(conf) {
//		Limit_Buy_Config info = *conf;
//		UInt_UInt_Map::iterator num_iter = limit_buy_num.find(id);
//		if(num_iter != limit_buy_num.end()) {
//			info.num -= num_iter->second;
//		}
//		return &info;
//	}
//	return 0;
//}

const Time_Value& Active_Content_Manager::get_limit_buy_start_time(void) {
	return limit_buy_start_time;
}

const Time_Value& Active_Content_Manager::get_limit_buy_end_time(void) {
	return limit_buy_end_time;
}

const Time_Value& Active_Content_Manager::get_vip_club_start_time(void) {
	return vip_club_start_time;
}

const Time_Value& Active_Content_Manager::get_vip_club_end_time(void) {
	return vip_club_end_time;
}

const UInt_UInt_Map& Active_Content_Manager::get_vip_club_level_num(void) {
	return vip_club_level_num;
}

const uint32_t Active_Content_Manager::get_vip_club_num_by_vip_level(uint32_t level) {
	uint32_t num = 0;
	for(UInt_UInt_Map::iterator it = vip_club_level_num.begin(); it != vip_club_level_num.end(); ++it) {
		if(it->first >= level) {
			num += it->second;
		}
	}
	return num;
}

void Active_Content_Manager::open_vip_club(const Time_Value &start, const Time_Value &end) {
	vip_club_start_time = start;
	vip_club_end_time = end;
	common_start_time[ACTION_ACTIVE_VIP_CLUB] = start;
	common_end_time[ACTION_ACTIVE_VIP_CLUB] = end;
	c.common_lstime[ACTION_ACTIVE_VIP_CLUB] = start;
	need_save_db = true;
}

void Active_Content_Manager::close_vip_club(void) {
	vip_club_end_time = Time_Value::gettimeofday();
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->receive_common_reward_in_mail(ACTION_ACTIVE_VIP_CLUB);
	}
}

void Active_Content_Manager::listen_all_player_vip_level_change(uint32_t new_level, uint32_t old_level, Logic_Player *player) {
	{//删除旧等级人数
		UInt_UInt_Map::iterator vit = vip_club_level_num.find(old_level);
		if(vit != vip_club_level_num.end()) {
			--vit->second;
		}
	}
	{//加新等级人数
		UInt_UInt_Map::iterator vit = vip_club_level_num.find(new_level);
		if(vit != vip_club_level_num.end()) {
			++vit->second;
		} else {
			vip_club_level_num.insert(std::make_pair(new_level, 1));
		}
	}
	{//VIP俱乐部-vip等级监听
		const Vip_Club_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_vip_club_conf_map();
		Time_Value now = Time_Value::gettimeofday();
		if(conf && vip_club_end_time.sec() > now.sec()) {
			for(Vip_Club_Conf_Map::const_iterator dit = conf->begin(); dit != conf->end(); ++dit) {
				if(c.vip_announce_ed_level.find(dit->second.vip_level) != c.vip_announce_ed_level.end()) {
					continue;
				}
				uint32_t num = ACTIVE_CONTENT_MANAGER->get_vip_club_num_by_vip_level(dit->second.vip_level);
				if(num > dit->second.needs_num) {
					player->create_ann_active_content_vip_addnum(dit->second.vip_level, dit->second.needs_num);
					c.vip_announce_ed_level.insert(dit->second.vip_level);
					need_save_db = true;
				}
			}
		}
	}
}

void Active_Content_Manager::count_vip_num_on_start(void) {
	vip_club_level_num.clear();
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
	for (;iter != LOGIC_MONITOR->db_view()->id_player_view_map.end(); ++iter) {
		uint32_t vip_level = iter->second->vip();
		if(!vip_level) {
			continue;
		}

		UInt_UInt_Map::iterator vit = vip_club_level_num.find(vip_level);
		if(vit != vip_club_level_num.end()) {
			++vit->second;
		} else {
			vip_club_level_num.insert(std::make_pair(vip_level, 1));
		}

	}//遍历所有玩家
}

void Active_Content_Manager::open_level_ace(const Time_Value &start, const Time_Value &end) {
	level_ace_start_time = start;
	level_ace_end_time = end;
	common_start_time[	ACTION_ACTIVE_LEVEL_ACE] = start;
	common_end_time[ACTION_ACTIVE_LEVEL_ACE] = end;
	c.common_lstime[ACTION_ACTIVE_LEVEL_ACE] = start;
	need_save_db = true;
}

void Active_Content_Manager::close_level_ace(void) {
	level_ace_end_time = Time_Value::gettimeofday();
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->receive_common_reward_in_mail(ACTION_ACTIVE_LEVEL_ACE);
	}
}

const Time_Value& Active_Content_Manager::get_level_ace_start_time(void) {
	return level_ace_start_time;
}

const Time_Value& Active_Content_Manager::get_level_ace_end_time(void) {
	return level_ace_end_time;
}

void Active_Content_Manager::open_wing_levelup(const Time_Value &start, const Time_Value &end) {
	wing_levelup_start_time = start;
	wing_levelup_end_time = end;
}

void Active_Content_Manager::close_wing_levelup(void) {
	wing_levelup_end_time = Time_Value::gettimeofday();
}

const Time_Value& Active_Content_Manager::get_wing_levelup_start_time(void) {
	return wing_levelup_start_time;
}

const Time_Value& Active_Content_Manager::get_wing_levelup_end_time(void) {
	return wing_levelup_end_time;
}

void Active_Content_Manager::open_suit_collect(const Time_Value &start, const Time_Value &end) {
	suit_collect_start_time = start;
	suit_collect_end_time = end;
	common_start_time[	ACTION_ACTIVE_SUIT_COLLECT] = start;
	common_end_time[ACTION_ACTIVE_SUIT_COLLECT] = end;
	c.common_lstime[ACTION_ACTIVE_SUIT_COLLECT] = start;
	need_save_db = true;
}

void Active_Content_Manager::close_suit_collect(void) {
	suit_collect_end_time = Time_Value::gettimeofday();
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->receive_common_reward_in_mail(ACTION_ACTIVE_SUIT_COLLECT);
	}
}

const Time_Value& Active_Content_Manager::get_suit_collect_start_time(void) {
	return suit_collect_start_time;
}

const Time_Value& Active_Content_Manager::get_suit_collect_end_time(void) {
	return suit_collect_end_time;
}

void Active_Content_Manager::open_guild_level(const Time_Value &start, const Time_Value &end) {
	guild_level_start_time = start;
	guild_level_end_time = end;
	common_start_time[	ACTION_ACTIVE_GUILD_LEVEL] = start;
	common_end_time[ACTION_ACTIVE_GUILD_LEVEL] = end;
	c.common_lstime[ACTION_ACTIVE_GUILD_LEVEL] = start;
	need_save_db = true;
}

void Active_Content_Manager::close_guild_level(void) {
	guild_level_end_time = Time_Value::gettimeofday();
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
		it->second->receive_common_reward_in_mail(ACTION_ACTIVE_GUILD_LEVEL);
	}
}

const Time_Value& Active_Content_Manager::get_guild_level_start_time(void) {
	return guild_level_start_time;
}

const Time_Value& Active_Content_Manager::get_guild_level_end_time(void) {
	return guild_level_end_time;
}

bool Active_Content_Manager::have_announce_guild_of_level(int64_t gang_id, uint32_t level) {
	Time_Value now = Time_Value::gettimeofday();
	if(guild_level_end_time.sec() > now.sec()) {
		const Guild_Level_Conf_Map *conf = CONFIG_CACHE_ACTIVE_CONTENT->get_guild_level_conf_map();
		if(conf) {
			for(Guild_Level_Conf_Map::const_iterator cit = conf->begin(); cit != conf->end(); ++cit) {
				if(cit->second.type == 1 ||
						GANG_MANAGER->in_active_annon_ed(gang_id, cit->second.level)) {
					continue;
				}
				if(cit->second.level <= level) {
					GANG_MANAGER->set_active_annon_ed(gang_id, cit->second.level);
					need_save_db = true;
					return true;
				}
			}
		}
	}
	return false;
}

void Active_Content_Manager::open_level_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time) {
	level_rank_start_time = start;
	level_rank_end_time = end;
	level_rank_count_time.sec(start.sec() + count_time * Time_Value::ONE_MINUTE_IN_SECS);
	if(level_rank_start_time != c.lr_last_start_time) {
		c.lr_last_start_time = level_rank_start_time;
		c.level_rank_count = false;
		need_save_db = true;
	}
}

void Active_Content_Manager::count_level_rank(void) {
	if(c.level_rank_count && !can_reset_count) { return; }
	level_rank_count_time = Time_Value::gettimeofday();
	c.level_rank_count_role_info.clear();
	Ranking *conf = RANKING_MANAGER->get_rank_by_type(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING);
	if(conf) {
		int rank = 1;
		for(Ranking_Sort_List::const_iterator rit = conf->ranking_.begin(); rit != conf->ranking_.end(); ++rit) {
			const Rank_Count_Config *rank_conf = CONFIG_CACHE_ACTIVE_CONTENT->get_level_rank_count_conf_by_rank(rank);
			if(rank_conf) {
				Role_Info role_info;
				fill_role_info(role_info, rit->role_id);
				receive_count_rank_reward(rank_conf->reward_info, rit->role_id, ACTIVE_CONTENT_RANK_RECEIVE_MAIL_LEVEL_RANK, rank);
				c.level_rank_count_role_info.push_back(role_info);
			}
			++rank;
		}
	}
	c.level_rank_count = true;
	can_reset_count = false;
	need_save_db = true;
	LOGIC_ANN_MANAGER->create_ann_active_content_count(1386);
}

void Active_Content_Manager::close_level_rank(void) {
	level_rank_end_time = Time_Value::gettimeofday();
}

const Time_Value& Active_Content_Manager::get_level_rank_start_time(void) {
	return level_rank_start_time;
}

const Time_Value& Active_Content_Manager::get_level_rank_end_time(void) {
	return level_rank_end_time;
}

const Time_Value& Active_Content_Manager::get_level_rank_count_time(void) {
	return level_rank_count_time;
}

void Active_Content_Manager::get_three_level_rank_on_count(std::vector<Role_Info> &role_info_list) {
	int i = 0;
	for(std::vector<Role_Info>::iterator it = c.level_rank_count_role_info.begin(); it != c.level_rank_count_role_info.end(); ++it) {
		role_info_list.push_back(*it);
		++i;
		if(i > 3) {
			break;
		}
	}
}

int Active_Content_Manager::get_level_rank_on_count_rank(role_id_t role_id) {
	int rank = 1;
	for(std::vector<Role_Info>::iterator it = c.level_rank_count_role_info.begin(); it != c.level_rank_count_role_info.end(); ++it) {
		if(it->role_id == role_id) {
			return rank;
		}
		++rank;
	}
	return -1;
}

bool Active_Content_Manager::is_count_level_rank(void) {
	return c.level_rank_count;
}

void Active_Content_Manager::open_arena_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time) {
	arena_rank_start_time = start;
	arena_rank_end_time = end;
	arena_rank_count_time.sec(start.sec() + count_time * Time_Value::ONE_MINUTE_IN_SECS);
	if(arena_rank_start_time != c.ar_last_start_time) {
		c.ar_last_start_time = arena_rank_start_time;
		c.arena_rank_count = false;
		need_save_db = true;
	}
}

void Active_Content_Manager::count_arena_rank(void) {
	if(c.arena_rank_count && !can_reset_count) { return; }
	arena_rank_count_time = Time_Value::gettimeofday();
	c.arena_rank_count_role_info.clear();
	Arena_Manager::Arena_Ranking_Vec &arena_rank = ARENA_MANAGER->arena_ranking_vec();
	int rank = 1;
	for(Arena_Manager::Arena_Ranking_Vec::const_iterator rit = arena_rank.begin(); rit != arena_rank.end(); ++rit) {
		const Rank_Count_Config *rank_conf = CONFIG_CACHE_ACTIVE_CONTENT->get_arena_rank_count_conf_by_rank(rank);
		if(rank_conf) {
			Role_Info role_info;
			fill_role_info(role_info, (*rit)->role_id());
			receive_count_rank_reward(rank_conf->reward_info, (*rit)->role_id(), ACTIVE_CONTENT_RANK_RECEIVE_MAIL_ARENA_RANK, rank);
			c.arena_rank_count_role_info.push_back(role_info);
		} else {
			break;
		}
		++rank;
	}
	c.arena_rank_count = true;
	can_reset_count = false;
	need_save_db = true;
	LOGIC_ANN_MANAGER->create_ann_active_content_count(1352);
}

void Active_Content_Manager::close_arena_rank(void) {
	arena_rank_end_time = Time_Value::gettimeofday();
}

const Time_Value& Active_Content_Manager::get_arena_rank_start_time(void) {
	return arena_rank_start_time;
}

const Time_Value& Active_Content_Manager::get_arena_rank_end_time(void) {
	return arena_rank_end_time;
}

const Time_Value& Active_Content_Manager::get_arena_rank_count_time(void) {
	return arena_rank_count_time;
}

void Active_Content_Manager::get_three_arena_rank_on_count(std::vector<Role_Info> &role_info_list) {
	int i = 0;
	for(std::vector<Role_Info>::iterator it = c.arena_rank_count_role_info.begin(); it != c.arena_rank_count_role_info.end(); ++it) {
		role_info_list.push_back(*it);
		++i;
		if(i > 3) {
			break;
		}
	}
}

int Active_Content_Manager::get_arena_rank_on_count_rank(role_id_t role_id) {
	int rank = 1;
	for(std::vector<Role_Info>::iterator it = c.arena_rank_count_role_info.begin(); it != c.arena_rank_count_role_info.end(); ++it) {
		if(it->role_id == role_id) {
			return rank;
		}
		++rank;
	}
	return -1;
}

bool Active_Content_Manager::is_count_arena_rank(void) {
	return c.arena_rank_count;
}

void Active_Content_Manager::open_force_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time) {
	force_rank_start_time = start;
	force_rank_end_time = end;
	force_rank_count_time.sec(start.sec() + count_time * Time_Value::ONE_MINUTE_IN_SECS);
	if(force_rank_start_time != c.fr_last_start_time) {
		c.fr_last_start_time = force_rank_start_time;
		c.force_rank_count = false;
		need_save_db = true;
	}
}

void Active_Content_Manager::open_sg_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time) {
	sg_rank_start_time = start;
	sg_rank_end_time = end;
	sg_rank_count_time.sec(start.sec() + count_time * Time_Value::ONE_MINUTE_IN_SECS);
	if(sg_rank_start_time != c.sgr_last_start_time) {
		c.sgr_last_start_time = sg_rank_start_time;
		c.sg_rank_count = false;
		need_save_db = true;
	}
}

void Active_Content_Manager::open_mount_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time) {
	mount_rank_start_time = start;
	mount_rank_end_time = end;
	mount_rank_count_time.sec(start.sec() + count_time * Time_Value::ONE_MINUTE_IN_SECS);
	if(mount_rank_start_time != c.mount_last_start_time) {
		c.mount_last_start_time = mount_rank_start_time;
		c.mount_rank_count = false;
		need_save_db = true;
	}
}

void Active_Content_Manager::open_wing_rank(const Time_Value &start, const Time_Value &end, const uint32_t count_time) {
	wing_rank_start_time = start;
	wing_rank_end_time = end;
	wing_rank_count_time.sec(start.sec() + count_time * Time_Value::ONE_MINUTE_IN_SECS);
	if(wing_rank_start_time != c.wing_last_start_time) {
		c.wing_last_start_time = wing_rank_start_time;
		c.wing_rank_count = false;
		need_save_db = true;
	}
}

void Active_Content_Manager::count_force_rank(void) {
	if(c.force_rank_count && !can_reset_count) { return; }
	force_rank_count_time = Time_Value::gettimeofday();
	c.force_rank_count_role_info.clear();
	Ranking *conf = RANKING_MANAGER->get_rank_by_type(RankingDef::LOCAL_PERSONAL_FORCE_RANKING);
	if(conf) {
		int rank = 1;
		for(Ranking_Sort_List::const_iterator rit = conf->ranking_.begin(); rit != conf->ranking_.end(); ++rit) {
			const Rank_Count_Config *rank_conf = CONFIG_CACHE_ACTIVE_CONTENT->get_force_rank_count_conf_by_rank(rank);
			if(rank_conf) {
				Role_Info role_info;
				fill_role_info(role_info, rit->role_id);
				receive_count_rank_reward(rank_conf->reward_info, rit->role_id, ACTIVE_CONTENT_RANK_RECEIVE_MAIL_FORCE_RANK, rank);
				c.force_rank_count_role_info.push_back(role_info);
			}
			++rank;
		}
	}
	c.force_rank_count = true;
	can_reset_count = false;
	need_save_db = true;
	LOGIC_ANN_MANAGER->create_ann_active_content_count(1385);
}

void Active_Content_Manager::count_sg_rank(void) {
	if(c.sg_rank_count && !can_reset_count) { return; }
	sg_rank_count_time = Time_Value::gettimeofday();
	c.sg_rank_count_role_info.clear();
	Ranking *conf = RANKING_MANAGER->get_rank_by_type(RankingDef::LOCAL_DUNGEON_SG_RANKING);
	if(conf) {
		int rank = 1;
		for(Ranking_Sort_List::const_iterator rit = conf->ranking_.begin(); rit != conf->ranking_.end(); ++rit) {
			const Rank_Count_Config *rank_conf = CONFIG_CACHE_ACTIVE_CONTENT->get_sg_rank_count_conf_by_rank(rank);
			if(rank_conf) {
				Role_Info role_info;
				fill_role_info(role_info, rit->role_id);
				receive_count_rank_reward(rank_conf->reward_info, rit->role_id, ACTIVE_CONTENT_RANK_RECEIVE_MAIL_SG_RANK, rank);
				c.sg_rank_count_role_info.push_back(role_info);
			}
			++rank;
		}
	}
	c.sg_rank_count = true;
	can_reset_count = false;
	need_save_db = true;
	LOGIC_ANN_MANAGER->create_ann_active_content_count(1387);
}

void Active_Content_Manager::count_mount_rank(void) {
	if(c.mount_rank_count && !can_reset_count) { return; }
	mount_rank_count_time = Time_Value::gettimeofday();
	c.mount_rank_count_role_info.clear();
	Ranking *conf = RANKING_MANAGER->get_rank_by_type(RankingDef::LOCAL_PERSONAL_MOUNT_RANKING);
	if(conf) {
		int rank = 1;
		for(Ranking_Sort_List::const_iterator rit = conf->ranking_.begin(); rit != conf->ranking_.end(); ++rit) {
			const Rank_Count_Config *rank_conf = CONFIG_CACHE_ACTIVE_CONTENT->get_mount_rank_count_conf_by_rank(rank);
			if(rank_conf) {
				Role_Info role_info;
				fill_role_info(role_info, rit->role_id);
				receive_count_rank_reward(rank_conf->reward_info, rit->role_id, ACTIVE_CONTENT_RANK_RECEIVE_MAIL_MOUNT_RANK, rank);
				c.mount_rank_count_role_info.push_back(role_info);
			}
			++rank;
		}
	}
	c.mount_rank_count = true;
	can_reset_count = false;
	need_save_db = true;
	LOGIC_ANN_MANAGER->create_ann_active_content_count(1394);
}

void Active_Content_Manager::count_wing_rank(void) {
	if(c.wing_rank_count && !can_reset_count) { return; }
	wing_rank_count_time = Time_Value::gettimeofday();
	c.wing_rank_count_role_info.clear();
	Ranking *conf = RANKING_MANAGER->get_rank_by_type(RankingDef::LOCAL_PERSONAL_WING_RANKING);
	if(conf) {
		int rank = 1;
		for(Ranking_Sort_List::const_iterator rit = conf->ranking_.begin(); rit != conf->ranking_.end(); ++rit) {
			const Rank_Count_Config *rank_conf = CONFIG_CACHE_ACTIVE_CONTENT->get_wing_rank_count_conf_by_rank(rank);
			if(rank_conf) {
				Role_Info role_info;
				fill_role_info(role_info, rit->role_id);
				receive_count_rank_reward(rank_conf->reward_info, rit->role_id, ACTIVE_CONTENT_RANK_RECEIVE_MAIL_WING_RANK, rank);
				c.wing_rank_count_role_info.push_back(role_info);
			}
			++rank;
		}
	}
	c.wing_rank_count = true;
	can_reset_count = false;
	need_save_db = true;
	LOGIC_ANN_MANAGER->create_ann_active_content_count(1395);
}

void Active_Content_Manager::close_force_rank(void) {
	force_rank_end_time = Time_Value::gettimeofday();
}

void Active_Content_Manager::close_sg_rank(void) {
	sg_rank_end_time = Time_Value::gettimeofday();
}

void Active_Content_Manager::close_mount_rank(void) {
	mount_rank_end_time = Time_Value::gettimeofday();
}

void Active_Content_Manager::close_wing_rank(void) {
	wing_rank_end_time = Time_Value::gettimeofday();
}

const Time_Value& Active_Content_Manager::get_force_rank_start_time(void) {
	return force_rank_start_time;
}

const Time_Value& Active_Content_Manager::get_force_rank_end_time(void) {
	return force_rank_end_time;
}

const Time_Value& Active_Content_Manager::get_force_rank_count_time(void) {
	return force_rank_count_time;
}

const Time_Value& Active_Content_Manager::get_sg_rank_start_time(void) {
	return sg_rank_start_time;
}

const Time_Value& Active_Content_Manager::get_sg_rank_end_time(void) {
	return sg_rank_end_time;
}

const Time_Value& Active_Content_Manager::get_sg_rank_count_time(void) {
	return sg_rank_count_time;
}

const Time_Value& Active_Content_Manager::get_mount_rank_start_time(void) {
	return mount_rank_start_time;
}

const Time_Value& Active_Content_Manager::get_mount_rank_end_time(void) {
	return mount_rank_end_time;
}

const Time_Value& Active_Content_Manager::get_mount_rank_count_time(void) {
	return mount_rank_count_time;
}

const Time_Value& Active_Content_Manager::get_wing_rank_start_time(void) {
	return wing_rank_start_time;
}

const Time_Value& Active_Content_Manager::get_wing_rank_end_time(void) {
	return wing_rank_end_time;
}

const Time_Value& Active_Content_Manager::get_wing_rank_count_time(void) {
	return wing_rank_count_time;
}

void Active_Content_Manager::get_three_force_rank_on_count(std::vector<Role_Info> &role_info_list) {
	int i = 0;
	for(std::vector<Role_Info>::iterator it = c.force_rank_count_role_info.begin(); it != c.force_rank_count_role_info.end(); ++it) {
		role_info_list.push_back(*it);
		++i;
		if(i > 3) {
			break;
		}
	}
}

void Active_Content_Manager::get_three_sg_rank_on_count(std::vector<Role_Info> &role_info_list) {
	int i = 0;
	for(std::vector<Role_Info>::iterator it = c.sg_rank_count_role_info.begin(); it != c.sg_rank_count_role_info.end(); ++it) {
		role_info_list.push_back(*it);
		++i;
		if(i > 3) {
			break;
		}
	}
}

void Active_Content_Manager::get_three_mount_rank_on_count(std::vector<Role_Info> &role_info_list) {
	int i = 0;
	for(std::vector<Role_Info>::iterator it = c.mount_rank_count_role_info.begin(); it != c.mount_rank_count_role_info.end(); ++it) {
		role_info_list.push_back(*it);
		++i;
		if(i > 3) {
			break;
		}
	}
}

void Active_Content_Manager::get_three_wing_rank_on_count(std::vector<Role_Info> &role_info_list) {
	int i = 0;
	for(std::vector<Role_Info>::iterator it = c.wing_rank_count_role_info.begin(); it != c.wing_rank_count_role_info.end(); ++it) {
		role_info_list.push_back(*it);
		++i;
		if(i > 3) {
			break;
		}
	}
}

int Active_Content_Manager::get_force_rank_on_count_rank(role_id_t role_id) {
	int rank = 1;
	for(std::vector<Role_Info>::iterator it = c.force_rank_count_role_info.begin(); it != c.force_rank_count_role_info.end(); ++it) {
		if(it->role_id == role_id) {
			return rank;
		}
		++rank;
	}
	return -1;
}

int Active_Content_Manager::get_sg_rank_on_count_rank(role_id_t role_id) {
	int rank = 1;
	for(std::vector<Role_Info>::iterator it = c.sg_rank_count_role_info.begin(); it != c.sg_rank_count_role_info.end(); ++it) {
		if(it->role_id == role_id) {
			return rank;
		}
		++rank;
	}
	return -1;
}

int Active_Content_Manager::get_mount_rank_on_count_rank(role_id_t role_id) {
	int rank = 1;
	for(std::vector<Role_Info>::iterator it = c.mount_rank_count_role_info.begin(); it != c.mount_rank_count_role_info.end(); ++it) {
		if(it->role_id == role_id) {
			return rank;
		}
		++rank;
	}
	return -1;
}

int Active_Content_Manager::get_wing_rank_on_count_rank(role_id_t role_id) {
	int rank = 1;
	for(std::vector<Role_Info>::iterator it = c.wing_rank_count_role_info.begin(); it != c.wing_rank_count_role_info.end(); ++it) {
		if(it->role_id == role_id) {
			return rank;
		}
		++rank;
	}
	return -1;
}

bool Active_Content_Manager::is_count_force_rank(void) {
	return c.force_rank_count;
}

bool Active_Content_Manager::is_count_sg_rank(void) {
	return c.sg_rank_count;
}

bool Active_Content_Manager::is_count_mount_rank(void) {
	return c.mount_rank_count;
}

bool Active_Content_Manager::is_count_wing_rank(void) {
	return c.wing_rank_count;
}

int Active_Content_Manager::receive_count_rank_reward(const Active_Reward_Info_Config &reward_info, role_id_t role_id, uint32_t rank_type, uint32_t rank) {
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(rank_type);
	if(!mailc) {
		return 0;
	}
	Item_Vec item_vec;
	item_vec.clear();
	for(Active_Item_Vec::const_iterator it = reward_info.item.begin(); it != reward_info.item.end(); ++it) {
		item_vec.push_back(Item_Detail(it->item_id, it->num, Item_Detail::BIND));
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
	LOGIC_PUBLIC->send_mail(rank_type, role_id, item_vec, mailc->mail_title, contents);
	return 0;
}

void Active_Content_Manager::fill_role_info(Role_Info &role, role_id_t role_id) {
	role.reset();
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if(iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		Player_DB_View *player = iter->second;
		role.role_id = player->role_id;
		role.role_name = player->role_name;
		role.career = player->career;
		role.combat = player->force;
		role.gender = player->gender;
		role.head_photo_id = player->friend_.head_photo_id;
		role.level = player->level;
		role.vip_level = player->vip_level;
		role.guild_name = player->gang_name;
	}
}

void Active_Content_Manager::open_arena_dan(const uint32_t id, const Time_Value &start, const Time_Value &end) {
	arena_dan_start_time = start;
	arena_dan_end_time = end;
	c.arena_dan_start_time = start;
	c.arena_dan_last_id = id;
	need_save_db = true;
}
void Active_Content_Manager::close_arena_dan(void) {
	arena_dan_end_time = Time_Value::gettimeofday();
	const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
	for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
			it->second->receive_arena_dan_reward_in_mail();
	}
}
const Time_Value& Active_Content_Manager::get_arena_dan_start_time(void) {
	return arena_dan_start_time;
}

const Time_Value& Active_Content_Manager::get_arena_dan_last_start_time(void) {
	return c.arena_dan_start_time;
}

const uint32_t Active_Content_Manager::get_arena_dan_last_act_id(void) {
	return c.arena_dan_last_id;
}

const Time_Value& Active_Content_Manager::get_arena_dan_end_time(void) {
	return arena_dan_end_time;
}

void Active_Content_Manager::open_common(const uint32_t id,const uint32_t type,const Time_Value &start, const Time_Value &end) {
//	switch(id) {
//		case ACTION_ACTIVE_CONTENT_TEAM_ARENA: //组队竞技
//		case ACTION_ACTIVE_CONTENT_TEAM_ARENA2: {//组队竞技2 {
//			comm_act_type_id[ACTIVE_CONTENT_REWARD_ACT_TYPE_TEAM_ARENA]=id;
//		}
//	}
	comm_act_type_id[type]=id;
	common_type_start_time[type]=start;

	common_start_time[id] = start;
	common_end_time[id] = end;
	c.common_lstime[id] = start;
	need_save_db = true;
}
void Active_Content_Manager::close_common(const uint32_t id) {
	common_end_time[id] = Time_Value::gettimeofday();
//	switch(id) {
//		case ACTION_ACTIVE_CONTENT_WORLD_BOSS: //世界BOSS(开服活动)
//		case ACTION_ACTIVE_CONTENT_SUIT_COLLECT_TWO: //套装收集2
//		case ACTION_ACTIVE_CONTENT_HEROES_DOOR: //英雄之门
//		case ACTION_ACTIVE_CONTENT_TEAM_ARENA: //组队竞技
//		case ACTION_ACTIVE_CONTENT_TEAM_ARENA2://组队竞技2
//		case ACTION_ACTIVE_CONTENT_WARER ://战场
//		case ACTION_ACTIVE_CONTENT_REWARD_TASK: //悬赏
//		case ACTION_ACTIVE_CONTENT_FM_X_CI ://累计附魔X次
//		case ACTION_ACTIVE_CONTENT_X_X_QUALITY_HEROER ://累计X个X品质英雄
//		case ACTION_ACTIVE_CONTENT_MOUNT_LVUP_X ://坐骑进阶X次
//		case ACTION_ACTIVE_CONTENT_FUSION_X_ELITE_CARD ://合成X张精英卡牌
//		case ACTION_ACTIVE_CONTENT_SG_FB : {//：上古副本
//			const Logic_Monitor::Logic_Player_Role_Id_Map &player_role_id_map_ = LOGIC_MONITOR->player_role_id_map();
//			for (Logic_Monitor::Logic_Player_Role_Id_Map::const_iterator it = player_role_id_map_.begin(); it != player_role_id_map_.end(); ++it) {
//				it->second->receive_common_reward_in_mail(id);
//			}
//			break;
//		}
//	}
}
const Time_Value& Active_Content_Manager::get_common_start_time(const uint32_t id) {
	return common_start_time[id];
}
const Time_Value& Active_Content_Manager::get_common_last_start_time(const uint32_t id) {
	return c.common_lstime[id];
}

const Int_Time_Map& Active_Content_Manager::get_common_last_start_time_map(void) {
	return c.common_lstime;
}

const Time_Value& Active_Content_Manager::get_common_end_time(const uint32_t id) {
	return common_end_time[id];
}

const uint32_t Active_Content_Manager::get_comm_act_type_id(uint32_t type) {
	return comm_act_type_id[type];
}

void Active_Content_Manager::active_content_listen_world_boss_end_info(role_id_t role_id, uint32_t damage, uint32_t rank, uint32_t damage_rate) {
	Logic_Player* player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player){
		player->active_content_listen_world_boss_end_info(damage, rank, damage_rate);
	} else {
		Logic_Player::offline_active_content_listen_world_boss_end_info(role_id, damage, rank, damage_rate);
	}
}
