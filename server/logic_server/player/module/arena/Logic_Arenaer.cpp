/*
 * Logic_Arenaer.cpp
 *
 *  Created on: 2014年5月19日
 *      Author: root
 */

#include "Logic_Arenaer.h"
#include "Msg_Arena_Struct.h"
#include "arena/Arena_Manager.h"
#include "Err_Code.h"
#include "pack/Packer.h"
#include "Logic_Player.h"
#include "Config_Cache_Arena.h"
#include "pack/Pack_Struct.h"
#include "Msg_Active_Struct.h"
#include "operating/Operating_Struct.h"
#include "operating/Operating.h"
#include "Config_Cache_Item.h"
#include "helper/Helper_Def.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Arena.h"
#include "Logic_Player_Detail.h"
#include "pack/Pack_Func.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Config_Cache_Public.h"

#define SOULS_PROP_ID 20703201
#define SOULS_PROP_COUNT 5000

#define COPPER_PROP_ID 20701201
#define COPPER_PROP_COUNT 5000

#define EXPLOIT_SOURCE_ID 503003
#define EXPLOIT_PROP_ID 20706401
#define EXPLOIT_PROP_COUNT 50

const int ARENA_MAX_GROUP = 6;

const int ARENA_SEASON_AWARD_RANK_NUM = 20;


Logic_Arenaer::Logic_Arenaer(void):arenaer_detail_(0),notice_client_(false){

}

Logic_Arenaer::~Logic_Arenaer(void){

}

void Logic_Arenaer::reset(void){
	this->arenaer_detail_ = 0;
	notice_client_ = false;
	arena_player_data[0].reset();
	arena_player_data[1].reset();
	arena_player_data[2].reset();
}

void Logic_Arenaer::load_detail(Arenaer_Detail &detail){
	arenaer_detail_ = &detail;
}
void Logic_Arenaer::save_detail(Arenaer_Detail &detail){
	if(this->arenaer_detail_->is_change){
		detail = *arenaer_detail_;
		this->arenaer_detail_->is_change = false;
	}
}


void Logic_Arenaer::daily_refresh(bool sync_client/* = false*/){
	if(this->local_rank() == 0){
		return;
	}
	//补偿剩余次数
	int com_times = compensate_times();
	int total_num;
	int enter_num;
	get_arena_enter_num_info(total_num, enter_num);
	int free_times = total_num - enter_num;	//
	if(free_times < 0){
		free_times = 0;
	}
	com_times += free_times;
	if(com_times > 10){
		com_times = 10;
	}
	set_compensate_times(com_times);
	set_enter_num(0);
	set_pay_times(0);
	set_buy_pay_times(0);
	set_arena_cd(0);
	//notice client
	if(sync_client){
		notice_arena_info();
	}
}

int Logic_Arenaer::trigger_daily_zero(const Time_Value &now, bool logic_check/* = false*/){
	return 0;
}

int Logic_Arenaer::trigger_daily_six(const Time_Value &now, bool logic_check/* = false*/){
	daily_refresh(!logic_check);
	return 0;
}

void Logic_Arenaer::module_init(void){

}

int Logic_Arenaer::create_init(Arenaer_Detail& detail){
	return 0;
}

void Logic_Arenaer::sign_in(void) {
	bool running = ARENA_MANAGER->is_arena_running();
	if(running){

	}
	ARENA_MANAGER->sign_in(this->player_self());
}

void Logic_Arenaer::sign_out(void){
	ARENA_MANAGER->sign_out(this->player_self());
}

void Logic_Arenaer::update_offline_reward(void){
	std::vector<int> reward_vec;
	if(this->three_day_rank() != 0){
		reward_vec.push_back(this->three_day_rank());
	}
	int three_day_count = this->player_self()->operating_detail().arena_three_day_reward.size();
	for(int i=0; i<three_day_count; ++i){
		int rank = this->player_self()->operating_detail().arena_three_day_reward[i].rank;
		reward_vec.push_back(rank);
	}
	int reward_count = reward_vec.size();
	if(reward_count == 1){
		if(three_day_count > 0){
			this->set_three_day_rank(reward_vec[0]);
		}
	}
	else if(reward_count >= 2){
		Time_Value now = Time_Value::gettimeofday();
		for(int i=0; i<reward_count; ++i){
			if(i == reward_count -1){
				this->set_three_day_rank(reward_vec[i]);
			}else{
				this->email_three_day_reward(reward_vec[i],
						player_self()->operating_detail().arena_three_day_reward[i].rank_time);
			}
		}
	}
}

void Logic_Arenaer::hook_fighter_level(const int new_level, const int src_level) {
	Arena_Rank_Player* rank_player = ARENA_MANAGER->find_rank_player(this->role_id());
	if(!rank_player){
		ARENA_MANAGER->sign_in(this->player_self());
	}
}

void Logic_Arenaer::hook_fighter_force(int new_force){

}

void Logic_Arenaer::hook_fighter_ttl_force(int new_force){
	ARENA_MANAGER->hook_fighter_ttl_force(role_id(), new_force);
}

void Logic_Arenaer::arena_listen_vip_level_change(uint32_t new_vip_level, uint32_t old_vip_level){
//	// vip等级更改达到无cd，清除cd
//	int no_cd_vip_lv = CONFIG_CACHE_ARENA->get_arena_no_cd_vip_lv();
//	int vip_lv = new_vip_level;
//	if(vip_lv >= no_cd_vip_lv){
//		int cd_value = this->get_arena_cd();
//		if(cd_va > 0){
//			this->reset_arena_cd();
//			this->notice_arena_info();
//		}
//	}
	return;
}

int Logic_Arenaer::req_arena_match(void){
	return ARENA_MANAGER->req_arena_match(role_id(), this->player_self()->get_highest_force());
}

int Logic_Arenaer::req_arena_fight(int index){
	if(player_self() == NULL){
		return -1;
	}
	const Scene_Config *scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if(scene_cfg && scene_cfg->type != Main_Scene_Type ){
		return ERROR_COPY_IS_GOING;
	}
	int arena_open_lv = CONFIG_CACHE_ARENA->get_arena_open_lv();
	if(player_self()->level() < arena_open_lv){
		return ERROR_ARENA_LEVEL_LIMIT;
	}
	if(ARENA_MANAGER->is_arena_running() == false){
		return ERROR_ARENA_NO_RUNNING;
	}
	int total_num;
	int enter_num;
	this->get_arena_enter_num_info(total_num, enter_num);
	if(enter_num >= total_num){
		return ERROR_ARENA_NUM_LIMIT;
	}
	int min_cd = CONFIG_CACHE_ARENA->get_arena_min_cd();
	if(this->get_arena_cd() > min_cd){
		return ERROR_ARENA_CD_LIMIT;
	}
	return ARENA_MANAGER->req_arena_fight(role_id(), index);
}

int Logic_Arenaer::req_arena_match_refresh(void){
	return ARENA_MANAGER->req_arena_match_refresh(role_id());
}

int Logic_Arenaer::req_arena_role_panel(role_id_t role_id, int8_t index){
	return ARENA_MANAGER->req_arena_role_panel(this->player_self(), role_id, index);
}

int Logic_Arenaer::req_arena_panel_refresh(void) {

	MSG_50100417 msg;
	Arena_Rank_Player* arena_player = ARENA_MANAGER->get_local_first_player();
	if (arena_player ) {

	    msg.first_arena_player.fight = arena_player->fight();
	    msg.first_arena_player.group = arena_player->group();
		msg.first_arena_player.level = arena_player->level();
		msg.first_arena_player.name = arena_player->role_name();
		msg.first_arena_player.rank = arena_player->local_rank();
		msg.first_arena_player.role_id = arena_player->role_id();
		msg.first_arena_player.score = arena_player->score();
		msg.first_arena_player.seg = arena_player->seg();
		msg.be_worship = arena_player->be_worship();
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(arena_player->role_id());
		if (player ) {
			msg.avatar_vec = player->base_detail().avatar_vec;
			msg.career = player->base_detail().career;
			msg.gender = player->base_detail().gender;
		}else {
			Player_DB_View *db_view = LOGIC_MONITOR->find_player_db_view_by_role_id(arena_player->role_id());
			if ( db_view ) {
				msg.avatar_vec = db_view->avatar_vec;
				msg.career = db_view->career;
				msg.gender = db_view->gender;
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Logic_Arenaer::req_arena_worship(const role_id_t role_id,const int8_t type) {

	const Arena_Adore_Public &arena_data = CONFIG_CACHE_PUBLIC->get_arena_adore_data();
//	if ( NULL == arena_set_conf )
//		return ERROR_CONFIG_NOT_EXIST;
    if ( player_self()->get_arena_worship() >= arena_data.adore_times ) {
    	return ERROR_WORSHIP_NUM_LIMIT;
    }
    Arena_Rank_Player* rank_player = ARENA_MANAGER->find_rank_player(role_id);
    if ( NULL == rank_player )
    	return ERROR_CLIENT_PARAM;
//    ARENA_MANAGER->inner_arena_save_be_worship(role_id);
    int result = 0;
    switch(type) {
    case 0:
    	result = worship_cost_get(role_id,arena_data.adore_cost_1,arena_data.adore_item_1);
    	break;
    case 1:
    	result = worship_cost_get(role_id,arena_data.adore_cost_2,arena_data.adore_item_2);
    	break;
    case 2:
    	result = worship_cost_get(role_id,arena_data.adore_cost_3,arena_data.adore_item_3);
        break;
    default:
    	result = ERROR_CLIENT_PARAM;
    	break;
    }
    if ( 0 != result )
    	return result;
    MSG_50100416 msg;
    msg.arena_worship = arena_data.adore_times - player_self()->get_arena_worship();
    Arena_Rank_Player *player = ARENA_MANAGER->find_rank_player(role_id);
    if ( player )
    	msg.be_worship = player->rank_info_.be_worship;
    else
    	msg.be_worship = 1;
    THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Logic_Arenaer::fetch_arena_rank_info(int type){
	if(type == 0){
		MSG_50100400 msg;
		int result = ARENA_MANAGER->fetch_arena_local_rank_info(role_id(), msg.rank, msg.rank_info_vec);
		if(result != 0){
			return -1;
		}
		THIS_SEND_TO_CLIENT(msg);
		notice_client_ = true;
	}else{
		ARENA_MANAGER->fetch_arena_inner_rank_info(role_id());
	}
	return 0;
}

int Logic_Arenaer::fetch_arena_info(bool panel_open/* = false*/){
	MSG_50100405 msg;
	int result = ARENA_MANAGER->fetch_arena_info(role_id(), msg);
	int total_num = 0;
	int enter_num = 0;
	get_arena_enter_num_info(total_num, enter_num);
	int free_times = total_num - enter_num - pay_times();	//
	if(free_times < 0){
		free_times = 0;
	}
	msg.personal_info.free_times = free_times;
	int total_free_times = total_num - buy_pay_times();
	if(total_free_times < 0){
		total_free_times = 0;
	}
	msg.personal_info.total_free_times = total_free_times;
	msg.personal_info.pay_times = pay_times();									//
	msg.personal_info.buy_pay_times = buy_pay_times();
	msg.personal_info.three_day_rank = this->three_day_rank();
	msg.personal_info.season_rank = this->get_season_rank();
	msg.personal_info.report_vec = arenaer_detail()->report_vec;
	msg.personal_info.rise = this->is_arena_rise();
	if(msg.personal_info.rise == 1){
		if(this->streak_lose() > 0){
			if(this->rise_win() > 0){
				msg.personal_info.rise_result_vec.push_back(1);
			}
			if(this->rise_lose() > 0){
				msg.personal_info.rise_result_vec.push_back(0);
			}
		}else{
			if(this->rise_lose() > 0){
				msg.personal_info.rise_result_vec.push_back(0);
			}
			if(this->rise_win() > 0){
				msg.personal_info.rise_result_vec.push_back(1);
			}
		}

	}
	msg.personal_info.remain_cd_time = get_arena_cd();
	THIS_SEND_TO_CLIENT(msg);
	if(panel_open && result == 0){
		notice_client_ = true;
	}
	return 0;
}

int Logic_Arenaer::close_arena_panel(void){
	ARENA_MANAGER->close_arena_panel(role_id());
	notice_client_ = false;
	return 0;
}

int Logic_Arenaer::req_match_arena_battle(void){
	if(player_self() == NULL){
		return -1;
	}
	const Scene_Config *scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if(scene_cfg && scene_cfg->type != Main_Scene_Type ){
		return ERROR_COPY_IS_GOING;
	}
	int arena_open_lv = CONFIG_CACHE_ARENA->get_arena_open_lv();
	if(player_self()->level() < arena_open_lv){
		return ERROR_ARENA_LEVEL_LIMIT;
	}
	if(ARENA_MANAGER->is_arena_running() == false){
		return ERROR_ARENA_NO_RUNNING;
	}
//	int total_num;
//	int enter_num;
//	this->get_arena_enter_num_info(total_num, enter_num);
//	if(enter_num >= total_num){
//		return ERROR_ARENA_NUM_LIMIT;
//	}
//	if(this->get_arena_cd() > 0){
//		return ERROR_ARENA_CD_LIMIT;
//	}

	ARENA_MANAGER->req_arena_match(role_id(), this->player_self()->get_highest_force());
	if(player_self()){
		player_self()->helper_listen_join_arena_force(player_self()->fighter_detail().force);
	}
	return 0;
}

int Logic_Arenaer::req_buy_arena_pay_times(const uint8_t& buy_type){
	int vip_limit = CONFIG_CACHE_ARENA->get_arena_pay_time_vip_lv();
	int player_vip = player_self()->vip();
	if(player_vip < vip_limit){
		return ERROR_ARENA_VIP_LEVEL_LIMIT;
	}
	int result = 0;
	int max_times = ARENA_MANAGER->max_buy_pay_times();
	if(buy_pay_times() >= max_times){
		result = ERROR_ARENA_BUY_PAY_TIMES_OVER;
	}
	if(result != 0){
		MSG_50100420 msg;
		msg.result = 0;
		msg.buy_pay_times = buy_pay_times();
		msg.pay_times = pay_times();
		THIS_SEND_TO_CLIENT(msg);
		return result;
	}
	int buy_times = buy_pay_times();
	int buy_count = 1;
	if(buy_type != 0){
		buy_count = max_times-buy_times;
	}
	if(buy_count > 1){// 一次最多购买1次
		buy_count = 1;
	}
	int price = 0;
	int cost;
	int increase;
	ARENA_MANAGER->get_pay_times_cost(cost, increase);
	for(int i=0; i<buy_count; ++i){
		price += cost + (buy_times+i)*increase;
	}
	//money
	result = this->player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST), price, MONEY_SUB_ARENA_BUY_PAY_TIMES));
	if(result != 0){
		MSG_50100420 msg;
		msg.result = 0;
		msg.buy_pay_times = buy_pay_times();
		msg.pay_times = pay_times();
		THIS_SEND_TO_CLIENT(msg);
		return result;
	}
	add_buy_pay_times(buy_count);
	MSG_50100420 msg;
	msg.result = 1;
	msg.buy_pay_times = buy_pay_times();
	msg.pay_times = pay_times();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Logic_Arenaer::req_reset_arena_cd(void){
	// 竞技场清除CD消耗根据和时间向上取整,如:每清除1分钟->1钻石
	int cost = CONFIG_CACHE_ARENA->get_arena_cd_cost();
	if(cost <= 0){
		return -1;
	}
	int remain_cd = this->get_arena_cd();
	if(remain_cd <= 0){
		return -1;
	}
	int num = remain_cd/60;
	if(remain_cd%60 != 0){
		num += 1;
	}
	int total_cost = cost*num;
//	if(player_self()->vip() > 0){
//		return ERROR_ARENA_RESET_CD_VIP;
//	}
	//money
	int result = this->player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), total_cost, MONEY_SUB_ARENA_REFRESH_CD));
	if(result != 0){
		MSG_50100407 msg;
		msg.result = 0;
		THIS_SEND_TO_CLIENT(msg);
		return result;
	}
	reset_arena_cd();
	{
		MSG_50100407 msg;
		msg.result = 1;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Logic_Arenaer::arena_battle_result(uint8_t result){
	if(this->player_self()){
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_ARENA);
		player_self()->task_listen_single_arena_all();
		if(result == 1){
			player_self()->task_listen_single_arena_win();
		}
	}
	ARENA_MANAGER->battle_result(role_id(), result);
	return 0;
}

int Logic_Arenaer::receive_reward(int8_t type){
	MSG_81000102 res_msg;
	res_msg.type = 0;
	if(type == 0){//day_reward
		int day_rank = this->three_day_rank();
		int season_rank = this->get_season_rank();
		if(day_rank == 0){
			MSG_50100401 msg;
			msg.type = type;
			msg.result = 0;
			msg.three_day_rank = day_rank;
			msg.seven_day_rank = season_rank;
			THIS_SEND_TO_CLIENT(msg);
			return ERROR_ARENA_NO_REWARD;
		}
		Arena_Award_Config* reward_config = CONFIG_CACHE_ARENA->get_arena_award_conifg(day_rank);
		if(!reward_config){
			return ERROR_ARENA_NO_REWARD;
		}
		double level_fix = 1.0;
		level_fix = CONFIG_CACHE_ARENA->get_arena_levelfix(this->level());
		Item_Vec pack_item_vec;
		Money_Add_List m_a_list;
		int souls = 0;
		int gold = 0;
		int exploit = 0;
		std::vector<Int_Int>::iterator it;
		for(it = reward_config->rank_award_item_vec.begin();
				it != reward_config->rank_award_item_vec.end(); ++it){
			if(it->val_2 <= 0){
				continue;
			}
			if(it->val_1 == COPPER ){
				gold += it->val_2;
			}else if(it->val_1 == SOULS){
				souls += it->val_2;
			}else if(it->val_1 == DRAGON_SOUL){
				m_a_list.push_back(Money_Add_Info(DRAGON_SOUL, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
			}else if(it->val_1 == FRIENDSHIP){
				m_a_list.push_back(Money_Add_Info(FRIENDSHIP, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
			}else if(it->val_1 == BIND_GOLD){
				m_a_list.push_back(Money_Add_Info(BIND_GOLD, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
			}else if(it->val_1 == EXPLOIT_SOURCE_ID){
				exploit += it->val_2;
			}else{
				Item_Detail item(it->val_1, it->val_2, Item_Detail::BIND);
				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				res_msg.item_info.push_back(item_base);
				pack_item_vec.push_back(item);
			}
		}
		//soul
		souls = (reward_config->rank_award_soul+souls)*level_fix;
		if(souls > 0){
			res_msg.property.push_back(Property(SOULS, souls));
			m_a_list.push_back(Money_Add_Info(SOULS, souls, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
		}
		gold = (reward_config->rank_award_gold+gold)*level_fix;
		// gold
		if(gold > 0){
			res_msg.property.push_back(Property(COPPER, gold));
			m_a_list.push_back(Money_Add_Info(COPPER, gold, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
		}
		// 功勋
		exploit = (reward_config->rank_award_exploit+exploit)*level_fix;

		if(exploit > 0){
			res_msg.property.push_back(Property(EXPLOIT_SOURCE_ID, exploit));
			this->player_self()->change_exploit_val(exploit);
		}
		if(m_a_list.empty() == false){
			this->player_self()->pack_add_money(m_a_list, MONEY_OPT_REAL);
		}
		int ret = 0;
		if(pack_item_vec.empty() == false){
			ret = this->player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX,
											pack_item_vec, Gain_Item_DM_Info(ITEM_GAIN_ARENA_RANK));
		}
		if(ret != 0){
			MSG_50100401 msg;
			msg.type = type;
			msg.result = 0;
			msg.three_day_rank = day_rank;
			msg.seven_day_rank = season_rank;
			THIS_SEND_TO_CLIENT(msg);
			return ret;
		}else{
			this->set_three_day_rank(0);
			MSG_50100401 msg;
			msg.type = type;
			msg.result = 1;
			msg.three_day_rank = 0;
			msg.seven_day_rank = season_rank;
			THIS_SEND_TO_CLIENT(msg);
			if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
				THIS_SEND_TO_CLIENT(res_msg);
			}
			return 0;
		}
	}else{
		int day_rank = this->three_day_rank();
		int season_rank = 0;
		int season_group = 0;
		int season_level = 0;
		this->get_season_result(season_rank, season_group, season_level);
		if(season_rank == 0){
			MSG_50100401 msg;
			msg.type = type;
			msg.result = 0;
			msg.three_day_rank = day_rank;
			msg.seven_day_rank = season_rank;
			THIS_SEND_TO_CLIENT(msg);
			return ERROR_ARENA_NO_REWARD;
		}
		// 王者组前20名
		Arena_Season_Award_Config* reward_config;
		if(season_rank <= ARENA_SEASON_AWARD_RANK_NUM && season_group == 61){
			reward_config = CONFIG_CACHE_ARENA->get_arena_season_rank_award_conifg(season_rank);
		}else{
			reward_config = CONFIG_CACHE_ARENA->get_arena_season_group_award_conifg(season_group);
		}
		if(!reward_config){
			return ERROR_ARENA_NO_REWARD;
		}
		double level_fix = 1.0;
		level_fix = CONFIG_CACHE_ARENA->get_arena_levelfix(this->level());
		Item_Vec pack_item_vec;
		Money_Add_List m_a_list;
		int souls = 0;
		int gold = 0;
		int exploit = 0;
		std::vector<Int_Int>::iterator it;
		for(it = reward_config->rank_award_item_vec.begin();
				it != reward_config->rank_award_item_vec.end(); ++it){
			if(it->val_2 <= 0){
				continue;
			}
			if(it->val_1 == COPPER ){
				gold += it->val_2;
			}else if(it->val_1 == SOULS){
				souls += it->val_2;
			}else if(it->val_1 == DRAGON_SOUL){
				m_a_list.push_back(Money_Add_Info(DRAGON_SOUL, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_SEASON)));
			}else if(it->val_1 == FRIENDSHIP){
				m_a_list.push_back(Money_Add_Info(FRIENDSHIP, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_SEASON)));
			}else if(it->val_1 == BIND_GOLD){
				m_a_list.push_back(Money_Add_Info(BIND_GOLD, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_SEASON)));
			}else if(it->val_1 == EXPLOIT_SOURCE_ID){
				exploit += it->val_2;
			}else{
				Item_Detail item(it->val_1, it->val_2, Item_Detail::BIND);
				Item_Basic_Info item_base;
				item.set_item_basic_info(item_base);
				res_msg.item_info.push_back(item_base);
				pack_item_vec.push_back(item);
			}
		}
		//soul
		souls = (reward_config->rank_award_soul+souls)*level_fix;
		if(souls > 0){
			res_msg.property.push_back(Property(SOULS, souls));
			m_a_list.push_back(Money_Add_Info(SOULS, souls, Money_DM_Info(MONEY_ADD_ARENA_SEASON)));
		}
		//gold
		gold = (reward_config->rank_award_gold+gold)*level_fix;
		if(gold > 0){
			res_msg.property.push_back(Property(COPPER, gold));
			m_a_list.push_back(Money_Add_Info(COPPER, gold, Money_DM_Info(MONEY_ADD_ARENA_SEASON)));
		}
		// 功勋
		exploit = exploit*level_fix;

		if(exploit > 0){
			res_msg.property.push_back(Property(EXPLOIT_SOURCE_ID, exploit));
			this->player_self()->change_exploit_val(exploit);
		}
		if(m_a_list.empty() == false){
			this->player_self()->pack_add_money(m_a_list, MONEY_OPT_REAL);
		}
		int ret = 0;
		if(pack_item_vec.empty() == false){
			ret = this->player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX,
											pack_item_vec, Gain_Item_DM_Info(ITEM_GAIN_ARENA_SEASON));
		}
		if(ret != 0){
			MSG_50100401 msg;
			msg.type = type;
			msg.result = 0;
			msg.three_day_rank = day_rank;
			msg.seven_day_rank = season_rank;
			THIS_SEND_TO_CLIENT(msg);
			return ret;
		}else{
			this->clear_season_rank();
			MSG_50100401 msg;
			msg.type = type;
			msg.result = 1;
			msg.three_day_rank = day_rank;
			msg.seven_day_rank = 0;
			THIS_SEND_TO_CLIENT(msg);
			if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
				THIS_SEND_TO_CLIENT(res_msg);
			}
			return 0;
		}
	}
	return 0;
}

int Logic_Arenaer::req_fetch_arena_award_info(void){
	MSG_50100402 msg;
	msg.local_rank = this->three_day_rank();
	msg.next_local_award_time = ARENA_MANAGER->get_next_local_award_time();
	int season_rank = 0;
	int season_group = 0;
	int season_level = 0;
	this->get_season_result(season_rank, season_group, season_level);
	msg.season_rank = season_rank;
	msg.season_group = season_group;
	msg.season_level = season_level;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}


bool Logic_Arenaer::can_join_arena(void){
	int arena_open_lv = CONFIG_CACHE_ARENA->get_arena_open_lv();
	if(this->level() < arena_open_lv){
		return false;
	}
	int total_num = 0;
	int enter_num = 0;
	get_arena_enter_num_info(total_num, enter_num);
	if( enter_num >= total_num){
		return false;
	}
	return true;
}

void Logic_Arenaer::get_arena_enter_num_info(int& total_num, int& enter_num){
	if(arenaer_detail_){
		int max_num = ARENA_MANAGER->max_free_match_times();
		int add_num = 0;
		if(player_self()){
			add_num = player_self()->achievement_arena_times();
		}
		total_num = max_num + arenaer_detail_->buy_pay_times + add_num;
		enter_num = arenaer_detail_->enter_num;
	}
}

int Logic_Arenaer::get_arena_season_remain_day(void){
	return ARENA_MANAGER->get_season_remain_day();
}

int Logic_Arenaer::get_arena_free_num(void){
	int max_num = ARENA_MANAGER->max_free_match_times();
	int add_num = 0;
	if(player_self()){
		add_num = player_self()->achievement_arena_times();
	}
	return max_num + add_num;
}

int Logic_Arenaer::group(void){
	return arenaer_detail_->group;
}

int Logic_Arenaer::seg(void){
	return arenaer_detail_->seg;
}

int Logic_Arenaer::score(void){
	return arenaer_detail_->score;
}

int Logic_Arenaer::local_rank(void){
	return arenaer_detail_->rank;
}

int Logic_Arenaer::inner_rank(void){
	return arenaer_detail_->inner_rank;
}

int Logic_Arenaer::rise_fail(void){
	return arenaer_detail_->rise_fail;
}
int Logic_Arenaer::rise_win(void){
	return arenaer_detail_->rise_win;
}
int Logic_Arenaer::rise_lose(void){
	return arenaer_detail_->rise_lose;
}
int Logic_Arenaer::enter_num(void){
	return arenaer_detail_->enter_num;
}
int Logic_Arenaer::pay_times(void){
	return arenaer_detail_->pay_times;
}
int Logic_Arenaer::buy_pay_times(void){
	return arenaer_detail_->buy_pay_times;
}

int Logic_Arenaer:: streak_win(void){
	return arenaer_detail_->streak_win;
}

int Logic_Arenaer:: streak_lose(void){
	return arenaer_detail_->streak_lose;
}


int Logic_Arenaer::compensate_times(void){
	return arenaer_detail_->compensate_times;
}

int Logic_Arenaer::three_day_rank(void){
	return arenaer_detail_->three_day_rank;
}

int Logic_Arenaer::seven_day_rank(void){
	return arenaer_detail_->seven_day_rank;
}


Time_Value Logic_Arenaer::arena_cd(void){
	return arenaer_detail_->cd;
}

bool Logic_Arenaer::match_player_info(role_id_t &match_player1, role_id_t &match_player2, role_id_t &match_player3,
		int8_t &match_dlv_1, int8_t &match_dlv_2, int8_t &match_dlv_3){
	if(arenaer_detail_->match_time != Time_Value::zero){
		if(Time_Value::gettimeofday() < arenaer_detail_->match_time){
			match_player1 = arenaer_detail_->match_player_1;
			match_player2 = arenaer_detail_->match_player_2;
			match_player3 = arenaer_detail_->match_player_3;
			match_dlv_1 = arenaer_detail_->match_dlv_1;
			match_dlv_2 = arenaer_detail_->match_dlv_2;
			match_dlv_3 = arenaer_detail_->match_dlv_3;
			return true;
		}else{
			set_match_player_info(Time_Value::zero, 0, 0, 0, 0, 0, 0);
		}
	}
	return false;
}

void Logic_Arenaer::get_arena_match_remain_time(bool& match, int& remain_time){
	if(arenaer_detail_->match_time != Time_Value::zero){
		match = true;
		Time_Value now = Time_Value::gettimeofday();
		remain_time = arenaer_detail_->match_time.sec() - now.sec();
		if(remain_time < 0){
			remain_time = 0;
		}
	}else{
		match = false;
		remain_time = 0;
	}
}

void Logic_Arenaer::set_fight_times(int value){
	arenaer_detail_->fight_times = value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_group_seg(const int value1, const int value2){
	arenaer_detail_->group = value1;
	arenaer_detail_->seg = value2;
	arenaer_detail_->detail_change();
	player_self()->achievement_listen_arena_lvl(value1 * 10 + value2);
	return;
}

void Logic_Arenaer::set_score(int value){
	arenaer_detail_->score = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_local_rank(int value){
	arenaer_detail_->rank = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_inner_rank(int value){
	if(arenaer_detail_->inner_rank != value){
		arenaer_detail_->inner_rank = value;
		arenaer_detail_->detail_change();
	}
	return;
}

void Logic_Arenaer::set_king_time(int value){
	if(arenaer_detail_->king_time != value){
		arenaer_detail_->king_time = value;
		arenaer_detail_->detail_change();
	}
}

void Logic_Arenaer::set_rise_fail(int value){
	arenaer_detail_->rise_fail = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_rise_win(int value){
	arenaer_detail_->rise_win = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_rise_lose(int value){
	arenaer_detail_->rise_lose = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_enter_num(int value){
	arenaer_detail_->enter_num = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_pay_times(int value){
	if(value < 0){
		value = 0;
	}
	arenaer_detail_->pay_times = value;
	arenaer_detail_->detail_change();
	return;
}
void Logic_Arenaer::set_buy_pay_times(int value){
	arenaer_detail_->buy_pay_times = value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_streak_win(int value){
	arenaer_detail_->streak_win = value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_streak_lose(int value){
	arenaer_detail_->streak_lose = value;
	arenaer_detail_->detail_change();
	return;
}


void Logic_Arenaer::set_compensate_times(int value){
	arenaer_detail_->compensate_times = value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_three_day_rank(int value){
	arenaer_detail_->three_day_rank = value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_seven_day_rank(int value){
	arenaer_detail_->seven_day_rank = value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_be_worship(int value) {
	arenaer_detail_->be_worship = value;
    arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::set_match_player_info(Time_Value time, const role_id_t player1, const role_id_t player2, const role_id_t player3,
		const int dlv_1, const int dlv_2, const int dlv_3){
	arenaer_detail_->match_time = time;
	arenaer_detail_->match_player_1 = player1;
	arenaer_detail_->match_player_2 = player2;
	arenaer_detail_->match_player_3 = player3;

	arenaer_detail_->match_dlv_1 = dlv_1;
	arenaer_detail_->match_dlv_2 = dlv_2;
	arenaer_detail_->match_dlv_3 = dlv_3;
	arenaer_detail_->detail_change();
	return;
}

int Logic_Arenaer::add_buy_pay_times(const int& value){
	arenaer_detail_->buy_pay_times += value;
	if(arenaer_detail_->pay_times < 0){
		arenaer_detail_->pay_times = 0;
	}
	arenaer_detail_->pay_times += value;
	arenaer_detail_->detail_change();
	return 0;
}

void Logic_Arenaer::add_battle_report(Arena_Battle_Report& report){
	if(arenaer_detail_->report_vec.size() > 10){
		std::vector<Arena_Battle_Report>::iterator it = arenaer_detail_->report_vec.begin();
		arenaer_detail_->report_vec.erase(it);
	}
	arenaer_detail_->report_vec.push_back(report);
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::add_battle_report_vec(std::vector<Arena_Battle_Report>& report_vec){
	if(report_vec.empty()){
		return;
	}
	if(report_vec.size() > 10){
		return;
	}
	int add_count = report_vec.size();
	int count = arenaer_detail_->report_vec.size();
	if(add_count+count > 10){
		int del_count = add_count+count - 10;
		std::vector<Arena_Battle_Report>::iterator it = arenaer_detail_->report_vec.begin();
		std::vector<Arena_Battle_Report>::iterator it_end = it + del_count;
		arenaer_detail_->report_vec.erase(it, it_end);
	}
	for(int i=0; i<add_count; ++i){
		arenaer_detail_->report_vec.push_back(report_vec[i]);
	}
	arenaer_detail_->detail_change();
}

void Logic_Arenaer::add_score(int value){
	arenaer_detail_->score += value;
	arenaer_detail_->detail_change();
	return;
}

void Logic_Arenaer::notice_arena_info(void){
	if(!notice_client_){
		return;
	}
	MSG_81000304 msg;
	msg.personal_info.score = this->score();
	msg.personal_info.buy_pay_times = this->buy_pay_times();
	int total_num = 0;
	int enter_num = 0;
	get_arena_enter_num_info(total_num, enter_num);
	int free_times = total_num - enter_num - pay_times();	//
	if(free_times < 0){
		free_times = 0;
	}
	msg.personal_info.free_times = free_times;	//
	int total_free_times = total_num - buy_pay_times();
	if(total_free_times < 0){
		total_free_times = 0;
	}
	msg.personal_info.total_free_times = total_free_times;
	msg.personal_info.group = this->group();
	msg.personal_info.seg = this->seg();
	msg.personal_info.local_rank = this->local_rank();
	msg.personal_info.max_pay_times = ARENA_MANAGER->max_buy_pay_times();
	msg.personal_info.pay_times = this->pay_times();

	msg.personal_info.reward_times1 = ARENA_MANAGER->get_next_local_award_time();
	msg.personal_info.three_day_rank = this->three_day_rank();
	msg.personal_info.season_rank = this->get_season_rank();
	msg.personal_info.rise = this->is_arena_rise();
	if(msg.personal_info.rise == 1){
		if(this->streak_lose() > 0){
			if(this->rise_win() > 0){
				msg.personal_info.rise_result_vec.push_back(1);
			}
			if(this->rise_lose() > 0){
				msg.personal_info.rise_result_vec.push_back(0);
			}
		}else{
			if(this->rise_lose() > 0){
				msg.personal_info.rise_result_vec.push_back(0);
			}
			if(this->rise_win() > 0){
				msg.personal_info.rise_result_vec.push_back(1);
			}
		}
	}
	msg.personal_info.remain_cd_time = get_arena_cd();
	// 前三名玩家
	ARENA_MANAGER->get_the_first_three(msg.the_first_three);
	THIS_SEND_TO_CLIENT(msg);
}

void Logic_Arenaer::notice_award_info(void){
	MSG_50100402 msg;
	msg.local_rank = this->three_day_rank();
	msg.next_local_award_time = ARENA_MANAGER->get_next_local_award_time();
	int season_rank = 0;
	int season_group = 0;
	int season_level = 0;
	this->get_season_result(season_rank, season_group, season_level);
	msg.season_rank = season_rank;
	msg.season_group = season_group;
	msg.season_level = season_level;
	THIS_SEND_TO_CLIENT(msg);
	return;
}

int Logic_Arenaer::is_arena_rise(void){
	Arena_Base_Config* config = CONFIG_CACHE_ARENA->get_arena_base_conifg(group()*10+seg());
	if(config){
		if(group() != ARENA_MAX_GROUP && score() >= config->arena_promote){
			return 1;
		}
	}
	return 0;
}

int Logic_Arenaer::email_three_day_reward(int rank, const Time_Value& now){
	Arena_Award_Config* award_config;
	award_config = CONFIG_CACHE_ARENA->get_arena_award_conifg(rank);
	if(award_config){
		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.receiver_id = this->role_id();
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.show_send_time = now;

		double level_fix = CONFIG_CACHE_ARENA->get_arena_levelfix(level());
		int copper = award_config->rank_award_gold;
		int soul = award_config->rank_award_soul;
		int exploit = award_config->rank_award_exploit;

		for(std::vector<Int_Int>::iterator item_iter  = award_config->rank_award_item_vec.begin();
					item_iter != award_config->rank_award_item_vec.end(); item_iter++) {
			if(item_iter->val_2 <= 0){
				continue;
			}
			if(item_iter->val_1 == COPPER ){
				copper += item_iter->val_2;
			}else if(item_iter->val_1 == SOULS){
				soul += item_iter->val_2;
			}else if(item_iter->val_1 == DRAGON_SOUL){
				//m_a_list.push_back(Money_Add_Info(DRAGON_SOUL, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
			}else if(item_iter->val_1 == FRIENDSHIP){
				//m_a_list.push_back(Money_Add_Info(FRIENDSHIP, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
			}else if(item_iter->val_1 == BIND_GOLD){
				//m_a_list.push_back(Money_Add_Info(BIND_GOLD, it->val_2, Money_DM_Info(MONEY_ADD_ARENA_RANK)));
			}else if(item_iter->val_1 == EXPLOIT_SOURCE_ID){
				exploit += item_iter->val_2;
			}else{
				send_info.item_vector.push_back(Item_Detail(item_iter->val_1, item_iter->val_2, Item_Detail::BIND));
			}
		}
		copper = copper*level_fix;
		int copper_item_count = copper/COPPER_PROP_COUNT;
		if(copper >0){
			if(copper_item_count == 0){
				copper_item_count = 1;
			}
			send_info.item_vector.push_back(Item_Detail(COPPER_PROP_ID, copper_item_count, Item_Detail::BIND));
		}
		soul = soul*level_fix;
		int soul_item_count = soul/SOULS_PROP_COUNT;
		if(soul > 0){
			if(soul_item_count == 0){
				soul_item_count = 1;
			}
			send_info.item_vector.push_back(Item_Detail(SOULS_PROP_ID, soul_item_count, Item_Detail::BIND));
		}
		exploit = exploit*level_fix;
		int exploit_item_count = exploit/EXPLOIT_PROP_COUNT;
		if(exploit > 0){
			if(exploit_item_count == 0){
				exploit_item_count = 1;
			}
			send_info.item_vector.push_back(Item_Detail(EXPLOIT_PROP_ID, exploit_item_count, Item_Detail::BIND));
		}
		time_t time_v = now.sec();
		struct tm tm_v;
		localtime_r(&time_v, &tm_v);
		std::stringstream stream;
		std::string text_content = CONFIG_CACHE_ARENA->get_mail_content();
		char mail_content[256] = {'\0'};
		snprintf(mail_content, sizeof(mail_content), text_content.c_str(), rank);
		stream<<mail_content;

		for(std::vector<Int_Int>::iterator item_iter  = award_config->rank_award_item_vec.begin();
					item_iter != award_config->rank_award_item_vec.end(); item_iter++) {
			if(item_iter->val_2 <= 0){
				continue;
			}
			if(item_iter->val_1 == COPPER ){
				//
			}else if(item_iter->val_1 == SOULS){
				//
			}else if(item_iter->val_1 == DRAGON_SOUL){
				//
			}else if(item_iter->val_1 == FRIENDSHIP){
				//
			}else if(item_iter->val_1 == BIND_GOLD){
				//
			}else if(item_iter->val_1 == EXPLOIT_SOURCE_ID){
				//
			}else{
				const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_iter->val_1);
				if(item_config){
					stream << item_config->name<<" *"<< item_iter->val_2<< "\r";
				}
			}
		}

		if(copper > 0){
			std::string text_gold = CONFIG_CACHE_ARENA->get_text_copper();
			stream <<text_gold<< " *"<< copper_item_count*COPPER_PROP_COUNT<< "\r";
		}
		if(soul > 0){
			std::string text_souls = CONFIG_CACHE_ARENA->get_text_soul();
			stream <<text_souls<< " *"<< soul_item_count*SOULS_PROP_COUNT<< "\r";
		}
		if(exploit > 0){
			std::string text_exploit = CONFIG_CACHE_ARENA->get_text_exploit();
			stream <<text_exploit<< " *"<< exploit_item_count*EXPLOIT_PROP_COUNT<< "\r";
		}
		send_info.content = stream.str();
		std::string mail_title = CONFIG_CACHE_ARENA->get_mail_title();
		std::string mail_sender = CONFIG_CACHE_ARENA->get_mail_sender();
		send_info.sender_name = mail_sender;
		send_info.title = mail_title;
	   this->player_self()->send_mail_to_self(send_info);
	}
	return 0;
}

void Logic_Arenaer::get_arena_level(int& group, int& seg){
	group = this->group();
	seg = this->seg();
}

void Logic_Arenaer::set_arena_cd(const int cd){
	if(arenaer_detail_){
		if(cd <= 0){
			arenaer_detail_->cd = Time_Value::zero;
		}else{
			arenaer_detail_->cd = Time_Value::gettimeofday() + Time_Value(cd);
		}
		arenaer_detail_->detail_change();
	}
}

void Logic_Arenaer::reset_arena_cd(void){
	if(arenaer_detail_){
		arenaer_detail_->cd = Time_Value::zero;
		arenaer_detail_->detail_change();
	}
}

int Logic_Arenaer::get_arena_cd(void){
	if(arenaer_detail_){
		if(arenaer_detail_->cd == Time_Value::zero){
			return 0;
		}else{
			Time_Value now = Time_Value::gettimeofday();
			int remain_time = arenaer_detail_->cd.sec() - now.sec();
			if(remain_time < 0){
				remain_time = 0;
			}
			return remain_time;
		}
	}
	return 0;
}

int Logic_Arenaer::notice_inner_rank(void){
	if(notice_client_){
		MSG_81000301 msg;
		msg.rank = this->inner_rank();
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Logic_Arenaer::notice_local_rank(void){
	if(notice_client_){
		MSG_81000300 msg;
		msg.rank = this->local_rank();
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}
void Logic_Arenaer::arena_season_clear_data(void){
	this->arenaer_detail()->reset();
}

void Logic_Arenaer::clear_season_rank(void){
	return ARENA_MANAGER->clear_season_rank(this->role_id());
}

int Logic_Arenaer::get_season_rank(void){
	return ARENA_MANAGER->get_season_rank(this->role_id());
}

void Logic_Arenaer::get_season_result(int &rank, int &group_index, int &level){
	return ARENA_MANAGER->get_season_result(this->role_id(), rank, group_index, level);
}

void Logic_Arenaer::set_arena_match_data(Logic_Player_Detail* match_data, int pos){
	if(!match_data){
		return;
	}
	if(pos < 0 || pos > 2){
		return;
	}
	arena_player_data[pos].reset();
	arena_player_data[pos].base_detail = match_data->base_detail;
	arena_player_data[pos].fighter_detail = match_data->fighter_detail;
	arena_player_data[pos].heroer_detail = match_data->heroer_detail;
}

Arena_Match_Player_Data* Logic_Arenaer::get_arena_match_player_data(const role_id_t match_id){
	if(arena_player_data[0].base_detail.role_id == match_id){
		return &arena_player_data[0];
	}
	if(arena_player_data[1].base_detail.role_id == match_id){
		return &arena_player_data[1];
	}
	if(arena_player_data[2].base_detail.role_id == match_id){
		return &arena_player_data[2];
	}
	return NULL;
}

int Logic_Arenaer::be_worship(void) {
	return arenaer_detail_->be_worship;
}

int Logic_Arenaer::worship_cost_get(const role_id_t role_id ,const std::vector<Int_Int> &cost,const std::vector<Int_Int> &get) {
	int result = 0;
	Money_Sub_List money_sub_list;
	int add_exploit_val = 0;
	if ( cost.size() > 0) {
		for (uint16_t i = 0;i < cost.size();++i ) {
			Money_DM_Info dm_info(Pack::MONEY_SUB_ARENA_WORSHIP);
			Pack::Money_Sub_Type sub_type;
			if (ERROR_MONEY_TYPE_ILLEGAL == get_sub_type_by_money_type(static_cast<Pack::Money_Type>(cost[i].val_1),sub_type))
				return ERROR_MONEY_TYPE_ILLEGAL;
			Money_Sub_Info sub_info(sub_type,cost[i].val_2,dm_info);
			money_sub_list.push_back(sub_info);
		}
	}
	if (money_sub_list.size() > 0 ) {
		result = player_self()->pack_try_sub_money(money_sub_list);
	}
	if ( 0 == result ) {
		//
		for (uint16_t i = 0;i < get.size();++i) {
			add_exploit_val += get[i].val_2;
		}
//		if ( add_exploit_val > 0 )
	}
    if ( 0 != result )
    	return result;
    if (money_sub_list.size() > 0 )
        player_self()->pack_sub_money(money_sub_list);
    ARENA_MANAGER->arena_save_be_worship(role_id);
    if ( add_exploit_val > 0 ) {
    	MSG_81000102 res_msg;
    	player_self()->change_exploit_val(add_exploit_val);
    	res_msg.property.push_back(Property(EXPLOIT_SOURCE_ID, add_exploit_val));
    	THIS_SEND_TO_CLIENT(res_msg);
    }

    player_self()->set_arena_worship(player_self()->get_arena_worship() + 1);

	return 0;
}
