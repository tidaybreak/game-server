/*
 * Team_Func.cpp
 *
 *  Created on: Sep 28, 2012
 *      Author: "dingxinglong"
 */

#include "Team.h"
#include "Logic_Player.h"
#include "Team_Func.h"
#include "Team_Struct.h"
#include "Utility_Func.h"
#include "Fighter_Detail.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "team/Team_Manager.h"

//void cpy_arena_teammer_info(const Inner_Arena_Teammer &teammer_detail, Arena_Teammer &teammer, int server) {
//	get_player_basic_info(teammer_detail.basic_info.role_id, teammer.basic_info, server);
//	teammer.combat_power = teammer_detail.combat_power;
//	if (!server) {
//		Logic_Player *player = LOGIC_MANAGER_INSTANCE->find_role_id_logic_player(teammer.basic_info.role_id);
//		if (player) {
//			teammer.hp_max = player->fighter_detail().fetch_property(Fighter_Detail::HP_MAX);
//		}
//	} else {
//		Map_Player *player = MAP_MANAGER_INSTANCE->find_role_id_player(teammer.basic_info.role_id);
//		if (player) {
//			teammer.hp_max = player->fighter_detail().fetch_property(Fighter_Detail::HP_MAX);
//		}
//	}
//
//}
//
//void cpy_arena_team_info(const Arena_Team_Common_Detail &team_detail, Arena_Team_Info &info, int server) {
//	cpy_arena_teammer_info(team_detail.leader, info.leader, server);
//	Arena_Teammer member;
//	for (RoleID_ArenaTeammer_Map::const_iterator it = team_detail.member_map.begin(); it != team_detail.member_map.end(); ++it) {
//		member.reset();
//		cpy_arena_teammer_info(it->second, member, server);
//		info.member.push_back(member);
//	}
//	info.team_id = team_detail.team_id;
//	info.name = team_detail.name;
//	info.combat_rank = team_detail.rank;
//	info.icon = team_detail.icon;
//	info.avg_combat_power = team_detail.combat_power_avg;
//	info.point_rank = CHARTS_MANAGER->get_charts_rank_by_type(Charts_Manager::CHARTS_MULTI_ARENA_TEAM_RANK, team_detail.team_id);
//	info.point = team_detail.point;
//	const Json::Value &agent_conf = CONFIG_INSTANCE->agent_common_conf();
//	if (agent_conf["inter_server_name"] != Json::Value::null) {
//		info.server_name = agent_conf["inter_server_name"].asString();
//	}
//}
//
//void get_arena_teamer_detail(const Logic_Player &player, Inner_Arena_Teammer &detail) {
//	get_player_basic_info(player, detail.basic_info);
//	detail.combat_power = player.fighter_detail().combat_power;
//}
//
//void get_arena_teamer_info(const Logic_Player &player, Arena_Teammer &info) {
//	get_player_basic_info(player, info.basic_info);
//	info.combat_power = player.fighter_detail().combat_power;
//	info.hp_max = player.fighter_detail().fetch_property(Fighter_Detail::HP_MAX);
//}

//void get_online_teamer_info(Logic_Player &player, Online_Teamer_Info &online_teamer_info) {
//	online_teamer_info.online = 1;
//	online_teamer_info.ready = player.teamer_detail().ready;
//	online_teamer_info.role.role.role_id = player.role_id();
//	online_teamer_info.role.role.role_name = player.role_name();
//	online_teamer_info.role.role.career = player.career();
//	online_teamer_info.role.role.level = player.level();
//	online_teamer_info.role.role.gender = player.base_detail().gender;
//	online_teamer_info.role.combat = 0;//(int)player.fighter_detail().fetch_fight_property(Property_Type::PT_FORCE);
//}

//void get_player_basic_info(const Logic_Player &player, Player_Basic_Info &basic_info) {
//	basic_info.role_id = player.base_detail().role_id;
//	basic_info.name = player.base_detail().role_name;
//	basic_info.gender = player.base_detail().gender;
//	basic_info.career = player.base_detail().career;
//	basic_info.level = player.base_detail().level;
//	basic_info.vip = player.base_detail().vip;
//	basic_info.country = player.base_detail().country_detail.country_id;
//}

void get_team_info(const Team &team, Team_Basic_Info &info) {
	info.index = team.get_team_index();
	Logic_Player *leader = team.team_leader();
	if (leader != NULL) {
//		get_player_basic_info(*leader, info.leader);
	} else {
		MSG_USER("");
	}
	info.teamer_nums = team.get_all_team_member_nums();
}

//void get_teamer_pos_info(const Map_Player &player, Teamer_Pos_Info &pos_info) {
//	pos_info.reset();
//	pos_info.basic_info.role_id = player.base_detail().role_id;
//	pos_info.basic_info.name = player.base_detail().role_name;
//	pos_info.basic_info.level = player.base_detail().level;
//	pos_info.basic_info.career = player.base_detail().career;
//	pos_info.basic_info.gender = player.base_detail().gender;
//	pos_info.basic_info.country = player.base_detail().country_detail.country_id;
//	pos_info.scene_id = player.const_mover_detail().scene_id;
//	pos_info.px = player.const_mover_detail().grid_coord.coord_x;
//	pos_info.py = player.const_mover_detail().grid_coord.coord_y;
//}

int get_online_teamer_info_from_logic(Logic_Player &player, Online_Teamer_Info &online_teamer_info) {
	online_teamer_info.online = 1;
	online_teamer_info.level = TEAM_MANAGER->get_role_team_level(player.role_id());
//	online_teamer_info.ready = player.teamer_detail().ready;
	get_team_role_info_from_logic(&player, online_teamer_info.role);
	player.heroer_detail().get_hero_info(player.heroer_detail().fighter_index, online_teamer_info.hero);
	for (std::vector<Int_Double>::const_iterator it = online_teamer_info.hero.prop_value.begin();
			it != online_teamer_info.hero.prop_value.end(); ++it) {
		if (it->id == PT_FORCE) {
			online_teamer_info.role.combat += it->value;
			break;
		}
	}
	return 0;
}

int get_team_role_info_from_logic(Logic_Player *player, Role_Team_Info &role) {
	role.reset();
	if (!player) {
		return -1;
	}
	role.role_id = player->role_id();
	role.role_name = player->role_name();
	role.head_photo_id = player->friend_detail().head_photo_id;
	role.career = player->career();
	role.level = player->level();
	role.gender = player->base_detail().gender;
	role.combat = static_cast<int32_t> (player->fighter_detail().fetch_fight_property(Property_Type::PT_FORCE));
	role.ready = player->teamer_detail().ready;

	return 0;
}


int team_card_process_drop_mode(int8_t player_career, Drop_Vec &drops_all, const Drop_Config* drop_config,
		double mult_coe/* = 1.0*/, double lv_coe /*= 1.0*/){
	int rate_type = drop_config->rate_type;
	if(rate_type == 0){
		int count = drop_config->box_vec.size();
		for(int i=0; i<count; ++i){
			if(Utility_Func::lottery_success(drop_config->box_rate_vec[i])){
				team_card_process_drop_box_mode(player_career, drops_all, drop_config->box_vec[i], mult_coe, lv_coe);
			}
		}
	}else{
		// 王彬需求：随机一个
		int i = team_card_drop_success_index(drop_config->box_rate_vec);
		if(i != -1) {
			team_card_process_drop_box_mode(player_career, drops_all, drop_config->box_vec[i], mult_coe, lv_coe);
		}
		// 谢韬需求：抽到惟一箱子，需配5个箱子
//		Int_Int_Std_Map pos_rate;
//		Int8_Vec pos_vec;
//		Double_Vec rate_vec;
//		int count = drop_config->box_rate_vec.size();
//		for(int i=0; i<count; ++i){
//			pos_rate.insert(std::make_pair(i, drop_config->box_rate_vec[i]));
//		}
//		Int_Int_Set_Map::iterator it = this->used_box_.find(drop_config->id);
//		if (it != this->used_box_.end()) {
//			for (Int_Set::iterator it_box = it->second.begin();
//					it_box != it->second.end(); ++it_box) {
//				pos_rate.erase(*it_box);
//			}
//		}
//		if (pos_rate.empty()) {
//			MSG_TRACE_ABORT("ERROR:drop_id(%d) not enough award for team.", drop_config->id);
//			return ERROR_CONFIG_NOT_EXIST;
//		}
//		for (Int_Int_Std_Map::const_iterator it_pos_rate = pos_rate.begin();
//				it_pos_rate != pos_rate.end(); ++it_pos_rate) {
//			pos_vec.push_back(it_pos_rate->first);
//			rate_vec.push_back(it_pos_rate->second);
//		}
//		int index_box_vec = this->team_card_drop_success_index(rate_vec);
//		if(index_box_vec != -1 && (int)pos_vec.size() > index_box_vec) {
//			int index_pos_vec = pos_vec[index_box_vec];
//			this->used_box_[drop_config->id].insert(index_pos_vec);
//			team_card_process_drop_box_mode(player_career, drops_all, drop_config->box_vec[index_pos_vec], mult_coe, lv_coe);
//		}
	}
	return 0;
}

int team_card_process_drop_box_mode(int8_t player_career, Drop_Vec &drops_all, const Drop_Box_Config& drop_box_config,
		double mult_coe /*= 1.0*/, double lv_coe /*= 1.0*/){
	int rate_type = drop_box_config.rate_type;
	int count = drop_box_config.item_id(player_career).size();
	int rate_count = drop_box_config.item_rate_vec.size();
	int bind_count = drop_box_config.item_bind_vec.size();
	int amount_count = drop_box_config.item_amount_vec.size();
	if(count != rate_count || count != bind_count || count != amount_count)
	{
		return -1;
	}
	if(rate_type == 0){
		for(int i=0; i<count; ++i)
		{
			if(Utility_Func::lottery_success(drop_box_config.item_rate_vec[i])){
				int item_count = round((double)drop_box_config.item_amount_vec[i]*mult_coe*lv_coe);
				if(item_count > 0)
				{
					Drop_One drop_one;
					drop_one.item = drop_box_config.item_id(player_career)[i];
					drop_one.amount = item_count;
					drop_one.bind = drop_box_config.item_bind_vec[i];
					drops_all.push_back(drop_one);
				}
			}
		}
	}else{
		int item_index = team_card_drop_success_index(drop_box_config.item_rate_vec);
		if(item_index != -1){
			int item_count = round((double)drop_box_config.item_amount_vec[item_index]*mult_coe*lv_coe);
			if( item_count > 0){
				Drop_One drop_one;
				drop_one.item = drop_box_config.item_id(player_career)[item_index];
				drop_one.amount = item_count;
				drop_one.bind = drop_box_config.item_bind_vec[item_index];
				drops_all.push_back(drop_one);
			}
		}
	}
	return 0;
}

int team_card_drop_success_index(const std::vector<double>& rate_vec){
	int resIndex = -1;
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	int random_nums = random() % 10000;
	int total_nums = 0;
	int count = rate_vec.size();
	for(int i=0; i<count; ++i){
		total_nums += rate_vec[i]*10000;
		if(random_nums < total_nums){
			resIndex = i;
			break;
		}
	}
	return resIndex;
}
