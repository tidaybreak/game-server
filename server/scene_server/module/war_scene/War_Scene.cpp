/*
 * War_Scene.cpp
 *
 *  Created on: 2014年7月1日
 *      Author: root
 */

#include "War_Scene.h"
#include "Scene_Player.h"
#include "Msg_Active_Struct.h"
#include "Msg_Battle_Scene_Struct.h"
#include "Battle_Scene.h"
#include "NPC/monster/Monster.h"
#include "war/Scene_War_Manager.h"
#include "Config_Cache_Dungeon.h"
#include "Err_Code.h"
#include "Config_Cache_NPC.h"
#include "NPC/NPC_Manager.h"
#include "Msg_Inner_Struct.h"
#include "announcement/Annoucement.h"
#include "announcement/Announcement_Struct.h"
#include "chater/Chater_Def.h"
#include "Scene_Public.h"

const int SRC_RECOVER_DISTANCE = 128;
const int WAR_SCENE_WIN_SCORE = 10000;
const int WAR_SCENE_FINISH_TIME = 60*27;
const int WAR_SCENE_READY_TIME = 60*2;

const int WAR_SCENE_LIGHT_MOVE_ABLE = 21;
const int WAR_SCENE_DARK_MOVE_ABLE = 22;

const int OFFLINE_KICK_TIME = 60*2;

const int NOTICE_KICK_TIME = 120;
const int FIRST_INACTIVE_KICK_TIME = 420;

//const int L_REVIVE_BALL_B = 1011;
//const int L_REVIVE_BALL_E = 1018;
//const int D_REVIVE_BALL_B = 2011;
//const int D_REVIVE_BALL_E = 2018;
//
//const int L_MINE_B = 1111;
//const int L_MINE_E = 1116;
//const int D_MINE_B = 2111;
//const int D_MINE_E = 2116;

bool War_Rank_Greater(const War_Player_Rank_Info& player1, const War_Player_Rank_Info& player2){
	return player1.score > player2.score;
}


War_Scene::War_Scene(void){
	reset();
}

War_Scene::~War_Scene(void){

}

void War_Scene::reset(void){
	Move_Scene::reset();

	player_initial_score_ = 100;

	light_score_ = 0;
	light_num_ = 0;
	dark_score_ = 0;
	dark_num_ = 0;

	gain_first_blood_ = false;
	check_over_ = false;
	war_over_time_ = Time_Value::max;
	offline_mover_map_.clear();

	src_recover_coord_[0].reset();
	src_recover_coord_[1].reset();
	random_coord_.reset();
	war_total_time_ = 0;
	act_time_ = 0;
	ready_time_ = 0;
	spe_mine_coord_.reset();
	spe_mine_cfg_ref_time_ = 0;
	spe_mine_ref_time_ = Time_Value::zero;
	is_war_over_ = false;
	check_is_war_over_ = Time_Value::zero;
}

int War_Scene::tick(const Time_Value &now){
	check_is_war_over_player(now);
	if(this->is_recover_status()){
		return 0;
	}
	Move_Scene::tick(now);
	if(this->scene_status_ == SCENE_NORMAL_STATUS){
		if(this->war_over_time_ - now <= Time_Value(act_time_-ready_time_)){
			this->scene_status_ = SCENE_READY_STATUS;
			change_move_value(WAR_SCENE_LIGHT_MOVE_ABLE, true);
			change_move_value(WAR_SCENE_DARK_MOVE_ABLE, true);
		}
		return 0;
	}
	check_recover_mine();
	check_inactive_player(now);
	//check_offline_player(now);
	if(now >= war_over_time_){
		int win_league = 1;
		if(light_score_ < dark_score_){
			win_league = 2;
		}
		war_over(win_league);
		return 0;
	}
	if(check_over_){
		this->check_over();
	}
	return 0;
}

int War_Scene::enter_scene(Mover *mover){
	Move_Scene::enter_scene(mover);
	if(mover->player_self()){
		Scene_Player* player = mover->player_self();
		if(this->is_recover_status()){
			//this->kick_player(player, 0);
			player->reset_war_info();
			int dest = 0;
			player->inner_transmit(dest,0,0);
			return 0;
		}
		player->add_reduce_cure_buf();
		Scene_War_Rank_Info* rank_info = find_offline_mover(mover->role_id());
		if(rank_info){
			player->set_war_info(rank_info);
			player->war_info()->src_hp = player->blood_current();
			double dif = player->blood_current() - player->war_info()->hp;
			if(dif < 0.00001 && dif > -0.00001){
				// nothing
			}else{
				// 恢复血量
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->war_info()->hp;
				fight_setter.push_back(setter);
				// player->fight_modify_fight_prop(fight_setter, true);
				player->modify_props_normal(fight_setter, true, 22);
			}
			if(player->war_info()->league == 1){
				++light_num_;
				if(light_num_ > 15){
					light_num_ = 15;
				}
			}else{
				++dark_num_;
				if(dark_num_ > 15){
					dark_num_ = 15;
				}
			}
			this->del_offline_mover(mover->role_id());
		}else{
			player->war_info()->reset();
			Honour_Player* h_player = SCENE_WAR_MANAGER->get_honour_player(mover->role_id());
			if(h_player){
				player->war_info()->server_name = h_player->server_name;
				player->war_info()->league = h_player->league+1;
				player->war_info()->total_merit = h_player->total_merit;
				if(h_player->remain_income_num == 0){
					player->war_info()->merit_coe = 0;
					player->war_info()->rep_coe = 0;
					player->war_info()->chest_pieces_coe = 0;
					if(player->war_info()->total_merit >= MAX_WAR_SCENE_MERIT){
						player->war_info()->merit_coe = 0;
					}
				}else {
					player->war_info()->merit_coe = h_player->income_rate;
					player->war_info()->rep_coe = h_player->income_rate;
					player->war_info()->chest_pieces_coe = h_player->income_rate;
				}
			}else{
				// 战场进入问题
				MSG_TRACE_ABORT("war_scene_enter error: Honour_Player NULL , %ld", mover->role_id());
			}
			if(player->war_info()->league == 1){
				++light_num_;
				if(light_num_ > 15){
					light_num_ = 15;
				}
			}else{
				++dark_num_;
				if(dark_num_ > 15){
					dark_num_ = 15;
				}
			}
			// 记录进入战场的血量
			player->war_info()->src_hp = player->blood_current();
			player->war_info()->hp = player->blood_current();
			// 满血
			if(player->current_blood_rate() < 0.99999){
				Prop_Setter_Vec fight_setter;
				Prop_Setter setter;
				setter.prop_type = PT_BLOOD_CURRENT;
				setter.operate = O_SET;
				setter.addi_type = AT_BASIC;
				setter.basic = player->blood_max();
				fight_setter.push_back(setter);
				//player->fight_modify_fight_prop(fight_setter, true);
				player->modify_props_normal(fight_setter, true, 31);
			}
			// 初始积分
			player->war_info()->score = player_initial_score_;
			player->war_info()->total_score = 0;
			player->war_info()->inactive_kick_time = Time_Value::gettimeofday() + Time_Value(FIRST_INACTIVE_KICK_TIME);
			player->war_info()->notice_kick_time = true;
		}
		this->broadcast_war_info();
		mover->player_self()->update_mover_war_info();
	}else if(mover->monster_self()){
		if(mover->mover_type() == MOVER_T_MONSTER_WAR_LIGHT_SRC){
			this->src_recover_coord_[0] = mover->grid_coord();
			// 资源官复活广播
			war_scene_announce(WAR_SCENE_ANNOUNCE_L_OFFICE_REVIVE, 0, mover->role_name());
		}else if(mover->mover_type() == MOVER_T_MONSTER_WAR_DARK_SRC){
			this->src_recover_coord_[1] = mover->grid_coord();
			// 资源官复活广播
			war_scene_announce(WAR_SCENE_ANNOUNCE_D_OFFICE_REVIVE, 0, mover->role_name());
		}
	}else if(mover->material_self()){
		if(mover->mover_type() == MOVER_T_MATERIAL_WAR_SPE_MINE){
			// 紫晶矿刷出
			war_scene_announce(WAR_SCENE_ANNOUNCE_SPE_MINE_REFRESH, 0, mover->role_name());
			spe_mine_coord_ = mover->pixel_coord();
			spe_mine_cfg_ref_time_ = mover->material_self()->npc_record()->config.refresh_time;
			spe_mine_ref_time_ = Time_Value::zero;
			this->broadcast_war_info();
		}
	}
	return 0;
}


int War_Scene::exit_scene(Mover *mover, bool offline){
	Move_Scene::exit_scene(mover, offline);
	if(this->is_recover_status()){
		if(mover->player_self()){
			mover->player_self()->reset_war_info();
		}
		return 0;
	}
	if(mover->player_self()){
		if(offline){
			mover->player_self()->war_info()->hp = mover->player_self()->blood_current();
			add_offline_mover(mover->role_id(), mover->player_self()->war_info());
		}else{
			// 5分钟逃跑限制
			mover->player_self()->add_escape_buf();
			Time_Value forbid_time = Time_Value::gettimeofday() + Time_Value(WAR_SCENE_FORBID_TIME);
			mover->player_self()->sync_forbid_time_to_logic(forbid_time);
			SCENE_WAR_MANAGER->req_leave_war_scene(mover->role_id());
		}
		int league = mover->player_self()->war_info()->league;
		if(league == 1){
			light_num_ -= 1;
			if(light_num_ < 0){
				light_num_ = 0;
			}
		}else{
			dark_num_ -= 1;
			if(dark_num_ < 0){
				dark_num_ = 0;
			}
		}
		// 恢复进入战场的血量
		double src_hp = mover->player_self()->war_info()->src_hp;
		mover->player_self()->reset_war_info();
		mover->player_self()->reset_morale_and_hp(-1.0f, src_hp, false);
		this->broadcast_war_info();
		check_player_empty();
	}else if(mover->monster_self()){
		if(mover->monster_self()->mover_type() == MOVER_T_MONSTER_WAR_LIGHT_SRC){
			this->src_recover_coord_[0].reset();
		}else if(mover->monster_self()->mover_type() == MOVER_T_MONSTER_WAR_DARK_SRC){
			this->src_recover_coord_[1].reset();
		}
	}
	return 0;
}

int War_Scene::client_scene_complete(Fighter &fighter) {
	Move_Scene::client_scene_complete(fighter);
	if(fighter.player_self()){
		if(is_war_over_){// 返回主城
			int dest = 0;
			fighter.player_self()->inner_transmit(dest,0,0);
		}else{
			fighter.player_self()->war_info()->role_id = fighter.player_self()->role_id();
			fighter.player_self()->war_info()->role_name = fighter.player_self()->role_name();
			send_war_info(fighter.player_self());
			send_player_info(fighter.player_self());
		}
	}
	return 0;
}

void War_Scene::npc_init(int scene_id, int sort, int sub_sort){
	section_init(scene_id, sort, sub_sort);
	NPC_MANAGER->section_refresh(&npc_section_);
}
void War_Scene::section_init(int scene_id, int sort, int sub_sort){
	random_coord_.reset();
	const War_Random_Coord_Config* rd_coord_cfg = CONFIG_CACHE_DUNGEON->war_random_coord_config();
	if(rd_coord_cfg){
		random_coord_.light_revive_ball = rd_coord_cfg->light_revive_ball;
		random_coord_.dark_revive_ball = rd_coord_cfg->dark_revive_ball;
		random_coord_.light_mine = rd_coord_cfg->light_mine;
		random_coord_.dark_mine = rd_coord_cfg->dark_mine;
	}
	Monster_Point_Config revive_ball_cfg;
	Monster_Point_Config nor_mine_cfg;

	npc_section_.scene_ = this;
	npc_section_.is_team = scene_config()->dungeon.can_team > 0 ? true:false;
	Monster_Section_Config* s_config = CONFIG_CACHE_NPC->get_monster_section_config(scene_id);
	if(s_config == NULL){

	}
	Monster_Point_Config_Map::iterator p_it;
	for(p_it = s_config->point_map.begin(); p_it != s_config->point_map.end(); ++p_it){
		if(p_it->second.point_type == REFRESH_POINT_TYPE_MATERIAL && p_it->second.npc_vec.empty() == false){
			const NPC_Config_Cache* m_cfg = CONFIG_CACHE_NPC->find_npc_config_cache(p_it->second.npc_vec[0].val_1);
			if(m_cfg){
				if(m_cfg->type == MOVER_T_MATERIAL_WAR_MINE){
					nor_mine_cfg = p_it->second;
					continue;
				}else if(m_cfg->type == MOVER_T_MATERIAL_WAR_REVIVE_BALL){
					revive_ball_cfg = p_it->second;
					continue;
				}
			}
		}
		NPC_Record record;
		record.config = p_it->second;
		record.scene = this;
		if(record.config.refresh_con == 1){
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday() +
					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
		}
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(p_it->first, record));
	}
	int stage = 0;
	role_id_t war_id = this->create_scene_args_.id;
	Honour_Field* h_field = SCENE_WAR_MANAGER->get_honour_field(war_id);
	if(h_field){
		stage = h_field->zone_id - WAR_SCENE_LEVEL_LIMIT/WAR_SCENE_ZONE_STAGE;
	}
	Monster_Group_Config_Map::iterator g_it;
	for(g_it = s_config->group_map.begin(); g_it != s_config->group_map.end(); ++g_it){
		NPC_Group group;
		group.config = g_it->second;
		// 卫兵、资源官动态成长 怪物类型id递减1000
		int m_count = group.config.monster.size();
		for(int i = 0; i < m_count; ++i){
			int new_monster_id = group.config.monster[i]-stage*1000;
			if(CONFIG_CACHE_NPC->find_npc_config_cache(new_monster_id)){
				group.config.monster[i] = new_monster_id;
			}
		}
		npc_section_.group_map.insert(std::make_pair(g_it->first, group));
	}
	npc_section_.path_guide_vec = s_config->path_guide_vec;
	//
	nor_mine_init(&nor_mine_cfg);
	revive_ball_init(&revive_ball_cfg);
}

void War_Scene::nor_mine_init(Monster_Point_Config* point_cfg){
	for(int i = L_MINE_B; i <= L_MINE_E; ++i){
		NPC_Record record;
		record.config = *point_cfg;
		record.config.id = i;
		pop_random_coord(i, record.config.birth_coord);
		record.scene = this;
		if(record.config.refresh_con == 1){
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday() +
					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
		}
		npc_section_.record_map.insert(std::make_pair(i, record));
	}
	for(int i = D_MINE_B; i <= D_MINE_E; ++i){
		NPC_Record record;
		record.config = *point_cfg;
		record.config.id = i;
		pop_random_coord(i, record.config.birth_coord);
		record.scene = this;
		if(record.config.refresh_con == 1){
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday() +
					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
		}
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(i, record));
	}
}
void War_Scene::revive_ball_init(Monster_Point_Config* point_cfg){
	for(int i = L_REVIVE_BALL_B; i <= L_REVIVE_BALL_E; ++i){
		NPC_Record record;
		record.config = *point_cfg;
		record.config.id = i;
		pop_random_coord(i, record.config.birth_coord);
		record.scene = this;
		if(record.config.refresh_con == 1){
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday() +
					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
		}
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(i, record));
	}
	for(int i = D_REVIVE_BALL_B; i <= D_REVIVE_BALL_E; ++i){
		NPC_Record record;
		record.config = *point_cfg;
		record.config.id = i;
		pop_random_coord(i, record.config.birth_coord);
		record.scene = this;
		if(record.config.refresh_con == 1){
			record.check_refresh = true;
			record.birth_freq = Time_Value::gettimeofday() +
					Time_Value(record.config.con_time/1000, record.config.con_time%1000);
		}
		record.init_share_hp();
		npc_section_.record_map.insert(std::make_pair(i, record));
	}
}

void War_Scene::pop_random_coord(role_id_t point_id, Coord& coord){
	std::vector<Coord> coord_vec;
	if(point_id >= L_REVIVE_BALL_B && point_id <= L_REVIVE_BALL_E){
		coord_vec = random_coord_.light_revive_ball;
	}else if(point_id >= D_REVIVE_BALL_B && point_id <= D_REVIVE_BALL_E){
		coord_vec = random_coord_.dark_revive_ball;
	}else if(point_id >= L_MINE_B && point_id <= L_MINE_E){
		coord_vec = random_coord_.light_mine;
	}else if(point_id >= D_MINE_B && point_id <= D_MINE_E){
		coord_vec = random_coord_.dark_mine;
	}
	int count = coord_vec.size();
	if(count <= 0){
		return;
	}
	int index = random()%count;
	coord = coord_vec[index];
	int tm_index = 0;
	std::vector<Coord>::iterator it;
	for(it = coord_vec.begin(); it != coord_vec.end(); ++it){
		if(index == tm_index){
			coord_vec.erase(it);
			break;
		}
		++tm_index;
	}
	if(point_id >= L_REVIVE_BALL_B && point_id <= L_REVIVE_BALL_E){
		random_coord_.light_revive_ball = coord_vec;
	}else if(point_id >= D_REVIVE_BALL_B && point_id <= D_REVIVE_BALL_E){
		random_coord_.dark_revive_ball = coord_vec;
	}else if(point_id >= L_MINE_B && point_id <= L_MINE_E){
		random_coord_.light_mine = coord_vec;
	}else if(point_id >= D_MINE_B && point_id <= D_MINE_E){
		random_coord_.dark_mine = coord_vec;
	}
}
void War_Scene::push_random_coord(role_id_t point_id, const Coord& coord){
	if(point_id >= L_REVIVE_BALL_B && point_id <= L_REVIVE_BALL_E){
		random_coord_.light_revive_ball.push_back(coord);
	}else if(point_id >= D_REVIVE_BALL_B && point_id <= D_REVIVE_BALL_E){
		random_coord_.dark_revive_ball.push_back(coord);
	}else if(point_id >= L_MINE_B && point_id <= L_MINE_E){
		random_coord_.light_mine.push_back(coord);
	}else if(point_id >= D_MINE_B && point_id <= D_MINE_E){
		random_coord_.dark_mine.push_back(coord);
	}
}

void War_Scene::change_random_coord(role_id_t point_id){
	NPC_Record* record = get_npc_record(point_id);
	if(record){
		Coord src_coord = record->config.birth_coord;
		Coord rd_coord;
		this->pop_random_coord(point_id, rd_coord);
		record->config.birth_coord = rd_coord;
		push_random_coord(point_id, src_coord);
	}
}

int War_Scene::init_scene_normal(int scene_id, int scene_order, int map_id/* = 0*/){
	Move_Scene_Base::init_scene_normal(scene_id, scene_order, map_id);
	transmit_point = scene_config()->transmit_point;
	Honour_Field* h_field = SCENE_WAR_MANAGER->get_honour_field(create_scene_args_.id);
	if(h_field){
		war_total_time_ = h_field->remain_time;
		act_time_ = h_field->act_time;
		ready_time_ = CONFIG_CACHE_DUNGEON->war_base_config()->ready_time;
	}else{
		war_total_time_ = WAR_SCENE_FINISH_TIME;
		act_time_ = WAR_SCENE_FINISH_TIME;
		ready_time_ = WAR_SCENE_READY_TIME;
	}
	scene_begin_time_= Time_Value::gettimeofday();
	npc_init(scene_id, 0, 0);
	war_over_time_ = scene_begin_time_ + Time_Value(war_total_time_);
	player_initial_score_ = CONFIG_CACHE_DUNGEON->war_base_config()->initial_score;
	return 0;
}

int War_Scene::release_scene_normal(void){
	MSG_DEBUG("battle scene release,scene_id:%d", scene_id());
	Move_Scene::release_scene_normal();

	return 0;
}

int War_Scene::scene_pass_to_logic(void){
	return 0;
}

void War_Scene::hook_player_enter_battle(Scene_Player* player){
	return;
}

void War_Scene::hook_battle_finish(int battle_type, int win_pos, std::vector<Mover*> win_mover_vec, std::vector<Mover*> lose_mover_vec, std::vector<int> kill_monsters){
	if(is_recover_status()){
		return;
	}
	const War_Base_Config* base_cfg = CONFIG_CACHE_DUNGEON->war_base_config();
	if(!base_cfg){
		return;
	}
	Scene_Player* win_player = NULL;
	Scene_Player* lose_player = NULL;
	Scene_Player* assists_player = NULL;
	int lose_score = 0;
	int win_merit = 0;
	int win_rep = 0;
	int win_score = 0;
	int assists_merit = 0;
	if(battle_type == Battle_Scene::BATTLE_TYPE_PVP){
		int win_count = win_mover_vec.size();
		for(int i=0; i<win_count; ++i){
			Scene_Player* player  = win_mover_vec[i]->player_self();
			if(player){
				win_player = player;
				break;
			}
		}

		int lose_count = lose_mover_vec.size();
		for(int i=0; i<lose_count; ++i){
			Scene_Player* player  = lose_mover_vec[i]->player_self();
			if(player){
				lose_player = player;
				break;
			}
		}
		if(!win_player || !lose_player){
			return;
		}
		// assist
		if(lose_player->war_info()->last_attack_role_id != 0 &&
				lose_player->war_info()->last_attack_role_id != win_player->role_id()){
			Mover* mover = this->find_mover_with_layer(0, lose_player->war_info()->last_attack_role_id);
			if(mover && mover->player_self()){
				assists_player = mover->player_self();
			}
		}

		//
		int lose_player_base_score = lose_player->war_info()->score;
		// 首杀奖励
		if(!gain_first_blood_){
			gain_first_blood_ = true;
			win_merit += base_cfg->fb_merit;
			win_score = lose_player_base_score*base_cfg->fb_score_pct + base_cfg->fb_score;
		}
		// 击杀奖励 每次击杀获取一定的积分（被击杀者损失+固定获取）及功勋奖励（根据被击杀者的连杀数配置）
		int streak_merit = 0;
		int end_streak_merit = 0;
		int streak_rep = 0;
		int end_streak_rep = 0;
		int kill_rep = 0;
		int streak_score = 0;
		double streak_score_pct = 0;
		int streak_num = win_player->war_info()->streak_win_num;
		int end_streak_num = lose_player->war_info()->streak_win_num;;
		CONFIG_CACHE_DUNGEON->get_streak_kill_award(streak_num,end_streak_num,
				streak_merit, end_streak_merit, streak_rep, end_streak_rep, streak_score_pct, streak_score);
		//CONFIG_CACHE_DUNGEON->get_kill_num_award(win_player->war_info()->kill_num + 1, kill_rep);
		win_merit += streak_merit+end_streak_merit;
		win_score += lose_player_base_score*streak_score_pct + streak_score;
		win_rep += streak_rep + end_streak_rep + kill_rep;
		//

		int add_win_score = win_player->war_info()->add_score(win_score);
		int add_win_merit = win_player->war_info()->add_merit(win_merit);
		int add_win_rep = win_player->war_info()->add_rep(win_rep);
		win_player->war_info()->kill_num += 1;
		win_player->war_info()->streak_win_num += 1;
		if(win_player->war_info()->streak_win_num > win_player->war_info()->max_streak_win_num){
			win_player->war_info()->max_streak_win_num = win_player->war_info()->streak_win_num;
		}
		win_player->war_info()->streak_lose_num = 0;
		win_player->war_info()->last_attack_role_id = lose_player->role_id();
		lose_player->war_info()->last_attack_role_id = 0;

		if(win_player->dead_status()){
			//recover hp
			Prop_Setter_Vec fight_setter;
			Prop_Setter setter;
			setter.prop_type = PT_BLOOD_CURRENT;
			setter.operate = O_SET;
			setter.addi_type = AT_BASIC;
			setter.basic = 1;
			fight_setter.push_back(setter);
			win_player->modify_props_normal(fight_setter, true, 23);
		}
		//英雄血量
		Hero *win_hero = SCENE_PUBLIC->find_fight_hero(win_player->role_id());
		if(win_hero){
			int hero_hp = win_hero->blood_current();
			if(hero_hp <= 0){
				hero_hp = 1;
			}
			win_player->war_info()->hero_hp_map[win_hero->hero_base_detail().hero_id] = hero_hp;
		}
		win_player->update_mover_war_info();
		//sync to logic
		if(add_win_merit > 0 || add_win_rep){
			std::vector<Id_Int_Int> temp_props;
			win_player->add_merit_piece_to_logic(add_win_merit, 0, add_win_rep, 0, temp_props);
		}

		{
			MSG_20200201 msg_1;
			msg_1.act_id = War_Scene_Type;
			msg_1.type = 1;
			msg_1.result = 1;
			msg_1.value1 = 1;
			msg_1.value2 = win_player->war_info()->league;
			Block_Buffer buf_1;
			buf_1.make_message(msg_1.msg_id);
			msg_1.serialize(buf_1);
			buf_1.finish_message();
			win_player->send_to_logic(buf_1);
		}
		// 飘字通知 击杀信息:你击杀了XX，获取了20功勋，20积分
		win_player->war_scene_notice(WAR_SCENE_NOTICE_KILL, lose_player->role_name(), lose_player->role_name(),
				add_win_score, add_win_merit, 0, add_win_rep, 0, true);
		// 连杀广播
		int streak_ann = get_ann_id_from_streak_win_num(win_player->war_info()->streak_win_num);
		if(streak_ann > 0){
			win_player->war_scene_announce(streak_ann, lose_player->role_id(),
					lose_player->role_name(), win_player->war_info()->streak_win_num,  0, 0, true);
		}
		// 终结广播：XX终结了XX的12连杀，获得XX功勋，110积分
		if(lose_player->war_info()->streak_win_num >= 3){
			win_player->war_scene_announce(WAR_SCENE_ANNOUNCE_END_STREAK_WIN, lose_player->role_id(),
					lose_player->role_name(), lose_player->war_info()->streak_win_num,  add_win_merit, add_win_rep, true);
		}
		send_player_info(win_player);
		if(win_player->war_info()->league == 1){
			this->light_score_ += win_score;
		}else{
			this->dark_score_ += win_score;
		}
		win_player->del_bless_buf();

		// 被击杀后，损失一定积分：当前拥有的个人积分*15%去零。
		lose_score = lose_player->war_info()->score*base_cfg->bk_score_pct;
		int add_lose_score = lose_score;
		lose_player->war_info()->dec_score(lose_score);
		lose_player->war_info()->die_num += 1;
		lose_player->war_info()->streak_win_num = 0;
		lose_player->war_info()->add_streak_lose_num = true;
		//英雄血量
		Hero *lose_hero = SCENE_PUBLIC->find_fight_hero(lose_player->role_id());
		if(lose_hero){
			lose_player->war_info()->hero_hp_map[lose_hero->hero_base_detail().hero_id] = 0;
		}
		// back to
		lose_player->war_die_revive();
		lose_player->update_mover_war_info();

		// 飘字通知 被击杀信息：损失25积分
		lose_player->war_scene_notice(WAR_SCENE_NOTICE_BE_KILL, win_player->role_name(), win_player->role_name(),
				add_lose_score, 0, 0, 0, 0, true);

		if(assists_player){
			assists_merit = win_merit*base_cfg->assists_merit;
			assists_player->war_info()->assists_num += 1;
			int add_assists_merit = assists_player->war_info()->add_merit(assists_merit);
			send_player_info(assists_player);
			// 飘字通知 助攻：您协助XXX击杀了XX，获得了10功勋
			assists_player->war_scene_notice(WAR_SCENE_NOTICE_ASSISTS, win_player->role_name(), lose_player->role_name(),
					0, add_assists_merit, 0, 0, 0);
			// sync to logic
			if(add_assists_merit > 0){
				std::vector<Id_Int_Int> temp_props;
				assists_player->add_merit_piece_to_logic(add_assists_merit, 0, 0, 0, temp_props);
			}
		}
		broadcast_war_info();

	}else if(battle_type == Battle_Scene::BATTLE_TYPE_PVE){
		int monster_type_id = 0;
		int win_count = win_mover_vec.size();
		for(int i=0; i<win_count; ++i){
			Scene_Player* player  = win_mover_vec[i]->player_self();
			if(player){
				win_player = player;
				break;
			}
		}
		int lose_count = lose_mover_vec.size();
		for(int i=0; i<lose_count; ++i){
			Scene_Player* player  = lose_mover_vec[i]->player_self();
			if(player){
				lose_player = player;
				break;
			}
		}
		int monster_count = kill_monsters.size();
		for(int i=0; i<monster_count; ++i){
			if(kill_monsters[i] != 0){
				monster_type_id = kill_monsters[i];
				break;
			}
		}
		const NPC_Config_Cache * monster_cfg = CONFIG_CACHE_NPC->find_npc_config_cache(monster_type_id);
		if((!win_player && !lose_player) || !monster_cfg){
			return;
		}
		int monster_type = monster_cfg->type;
		std::string monster_name = monster_cfg->name;
		if(win_player){
			win_score = 0;
			win_merit = 0;
			if(monster_type == MOVER_T_MONSTER_WAR_LIGHT_GUARD ||
					monster_type == MOVER_T_MONSTER_WAR_DARK_GUARD){
				win_score = base_cfg->guard_score;
				win_merit = base_cfg->guard_merit;
			}else if(monster_type == MOVER_T_MONSTER_WAR_LIGHT_SRC ||
					monster_type == MOVER_T_MONSTER_WAR_DARK_SRC){
				win_score = base_cfg->officer_score;
				win_merit = base_cfg->officer_merit;
			}
			int add_win_score = win_player->war_info()->add_score(win_score);
			int add_win_merit = win_player->war_info()->add_merit(win_merit);
			win_player->war_info()->streak_lose_num = 0;
			win_player->del_bless_buf();
			//英雄血量
			Hero *win_hero = SCENE_PUBLIC->find_fight_hero(win_player->role_id());
			if(win_hero){
				int hero_hp = win_hero->blood_current();
				if(hero_hp <= 0){
					hero_hp = 1;
				}
				win_player->war_info()->hero_hp_map[win_hero->hero_base_detail().hero_id] = hero_hp;
			}
			win_player->update_mover_war_info();
			send_player_info(win_player);
			if(win_player->war_info()->league == 1){
				this->light_score_ += add_win_score;
			}else{
				this->dark_score_ += add_win_score;
			}
			// 击杀信息：你击杀了XX，获取了20功勋，20积分
			win_player->war_scene_notice(WAR_SCENE_NOTICE_KILL, monster_name, monster_name,
							add_win_score, add_win_merit, 0, 0, 0, true);
			//sync to logic
			if(add_win_merit > 0){
				std::vector<Id_Int_Int> temp_props;
				win_player->add_merit_piece_to_logic(add_win_merit, 0, 0, 0, temp_props);
			}
			// 击杀卫兵军需官广播
			if(monster_type == MOVER_T_MONSTER_WAR_LIGHT_GUARD ||
					monster_type == MOVER_T_MONSTER_WAR_DARK_GUARD){
				win_player->war_scene_announce(WAR_SCENE_ANNOUNCE_KILL_GUARD,
						0, monster_name, 0, 0, 0, true);
			}else if(monster_type == MOVER_T_MONSTER_WAR_LIGHT_SRC ||
					monster_type == MOVER_T_MONSTER_WAR_DARK_SRC){
				if(monster_type == MOVER_T_MONSTER_WAR_LIGHT_SRC){
					win_player->war_scene_announce(WAR_SCENE_ANNOUNCE_KILL_L_OFFICER,
											0, monster_name, 0, 0, 0, true);
				}else{
					win_player->war_scene_announce(WAR_SCENE_ANNOUNCE_KILL_D_OFFICER,
											0, monster_name, 0, 0, 0, true);
				}

			}
			broadcast_war_info();
		}
		if(lose_player){
			lose_score = lose_player->war_info()->score*base_cfg->bk_score_pct;
			int add_lose_score = lose_score;
			lose_player->war_info()->dec_score(lose_score);
			lose_player->war_info()->die_num += 1;
			lose_player->war_info()->streak_win_num = 0;
			// back to
			lose_player->war_die_revive();
			//英雄血量
			Hero *lose_hero = SCENE_PUBLIC->find_fight_hero(lose_player->role_id());
			if(lose_hero){
				lose_player->war_info()->hero_hp_map[lose_hero->hero_base_detail().hero_id] = 0;
			}
			// 被击杀信息：损失25积分。
			lose_player->war_scene_notice(WAR_SCENE_NOTICE_BE_KILL, monster_name, monster_name,
							add_lose_score, 0, 0, 0, 0, true);
		}
	}

	return;
}

void War_Scene::hook_gather_success(Mover* mover, int point_id, int material_type){
	Scene_Player* player = mover->player_self();
	if(!player){
		return;
	}
	// 采矿
	if(material_type == MOVER_T_MATERIAL_WAR_MINE ||
		material_type == MOVER_T_MATERIAL_WAR_SPE_MINE){
		int mine_type = 1;
		if(material_type ==MOVER_T_MATERIAL_WAR_SPE_MINE){
			spe_mine_ref_time_ = Time_Value::gettimeofday() + Time_Value(spe_mine_cfg_ref_time_);
			broadcast_war_info();
			mine_type = 2;
		}else{
			// 更改刷新点
			change_random_coord(point_id);
		}
		if(player->war_info()->is_pull_mine == 0){
			player->add_pull_mine_buf();
		}
		player->war_info()->is_pull_mine = mine_type;
		player->update_mover_war_info();
	}else if(material_type == MOVER_T_MATERIAL_WAR_REVIVE_BALL){
	// 采集复活加速球
		player->gather_revive_ball();
		// 更改刷新点
		change_random_coord(point_id);
	}

	return;
}

int War_Scene::req_war_scene_fetch_rank(Scene_Player* player){
	Block_Buffer buf;
	MSG_50211001 msg;
	std::vector<War_Player_Rank_Info> info_vec;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				War_Player_Rank_Info info;
				Scene_War_Rank_Info* war_info = player->war_info();
				info.assists_num = war_info->assists_num;
				info.kill_num = war_info->kill_num;
				info.max_kill = war_info->max_streak_win_num;
				info.league = war_info->league;
				info.merit = war_info->merit;
				info.role_id = player->role_id();
				info.role_name = player->role_name();
				info.score = war_info->total_score;
				info.server_name = war_info->server_name;
				info.reputation = war_info->rep;
				info_vec.push_back(info);
			}
		}
	}
	std::sort(info_vec.begin(), info_vec.end(), War_Rank_Greater);
	int info_count = info_vec.size();
	for(int i=0; i<info_count; ++i){
		info_vec[i].rank = i+1;
	}
	msg.rank_vec = info_vec;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	return 0;
}

int War_Scene::req_war_scene_heal(Scene_Player* player){
	//check
	const War_Base_Config* base_cfg = CONFIG_CACHE_DUNGEON->war_base_config();
	if(!base_cfg){
		return -1;
	}
	// 积分不够
	if(player->war_info()->heal_num > 0 &&
			player->war_info()->score < base_cfg->heal_cost_score){
		return ERROR_WAR_SCENE_SCORE_LIMIT;
	}
	// 复活中
	if(player->war_info()->revive != 0){
		return ERROR_WAR_SCENE_REVIVE_LIMIT;
	}

	std::vector<Scene_Player*> heal_player;
	Block_Buffer buf;
	MSG_81000403 msg;
	msg.role_id = player->role_id();
	Scene_Player_Set player_set;
	collect_round_fight_player(0, player->grid_coord(), player_set, 200);
	Scene_Player_Set::iterator it;
	for(it = player_set.begin(); it != player_set.end(); ++it){
		if((*it)->role_id() != player->role_id()){
			msg.healed_player_vec.push_back((*it)->role_id());
		}
		heal_player.push_back(*it);
	}
	int heal_count = heal_player.size();
	double heal_rate = 0.3;
	for(int i=0; i<heal_count; ++i){
		heal_player[i]->recover_war_hp(heal_rate);
		heal_player[i]->update_mover_war_info();
	}
	MAKE_MSG_BUF(msg, buf);
	this->broadcast_around(player->mover_detail().layer_id, player->mover_detail().block_index,
			buf);
	// 扣取积分
	if(player->war_info()->heal_num > 0){
		player->war_info()->dec_score(base_cfg->heal_cost_score);
	}
	player->war_info()->heal_num += 1;
	send_player_info(player);
	return 0;
}

int War_Scene::update_move_able_info(Mover* mover){
	mover->clear_moveable_value();
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		if((*it).move_value.empty() == false){
			Block_Buffer buf;
			MSG_81000022 msg;
			Move_Value::iterator s_it;
			for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
				if(s_it->val_2 == MOVE_ABLE_FROM){
					if(s_it->val_1 == WAR_SCENE_LIGHT_MOVE_ABLE){
						if(mover->player_self()){
							if(mover->player_self()->war_info()->league == 1 &&
									mover->player_self()->war_info()->revive == 0){
								msg.moveable_info_vec.push_back(s_it->val_1);
								uint8_t val1 = s_it->val_1;
								mover->add_moveable_value(val1);
							}
						}
					}else if(s_it->val_1 == WAR_SCENE_DARK_MOVE_ABLE){
						if(mover->player_self()->war_info()->league == 2 &&
								mover->player_self()->war_info()->revive == 0){
							msg.moveable_info_vec.push_back(s_it->val_1);
							uint8_t val1 = s_it->val_1;
							mover->add_moveable_value(val1);
						}
					}else {
						msg.moveable_info_vec.push_back(s_it->val_1);
						uint8_t val1 = s_it->val_1;
						mover->add_moveable_value(val1);
					}
				}
			}
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}else{
			MSG_81000022 msg;
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}
		break;
	}
	return 0;
}

void War_Scene::send_war_info(Mover* mover){
	MSG_81000400 msg;
	msg.dark_num = dark_num_;
	msg.dark_score = dark_score_;
	msg.light_num = light_num_;
	msg.light_score = light_score_;

	Time_Value now = Time_Value::gettimeofday();
	msg.remain_time = this->war_over_time_.sec() - now.sec();
	msg.fight_time = this->act_time_ - this->ready_time_;
	msg.spe_mine_coord = spe_mine_coord_;
	int spe_mine_ref_time = spe_mine_ref_time_.sec() - now.sec();
	if(spe_mine_ref_time < 0){
		spe_mine_ref_time = 0;
	}
	msg.spe_mine_ref_time = spe_mine_ref_time;
	OBJ_SEND_TO_CLIENT(msg, (*mover));
}

void War_Scene::send_player_info(Scene_Player* player){
	player->send_war_scene_player_info();
}

void War_Scene::broadcast_war_info(void){
	Block_Buffer buf;
	MSG_81000404 msg;
	msg.dark_num = dark_num_;
	msg.dark_score = dark_score_;
	msg.light_num = light_num_;
	msg.light_score = light_score_;
	msg.spe_mine_coord = spe_mine_coord_;
	int spe_mine_ref_time = spe_mine_ref_time_.sec() - Time_Value::gettimeofday().sec();
	if(spe_mine_ref_time < 0){
		spe_mine_ref_time = 0;
	}
	msg.spe_mine_ref_time = spe_mine_ref_time;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	this->broadcast_all_within_battle(0, buf);
	check_over_ = true;
}

Scene_War_Rank_Info* War_Scene::find_offline_mover(role_id_t role_id){
	Offline_Mover_Map::iterator it = offline_mover_map_.find(role_id);
	if(it != offline_mover_map_.end()){
		return &(it->second);
	}
	return NULL;
}

void War_Scene::add_offline_mover(role_id_t role_id, Scene_War_Rank_Info* info){
	info->offline_kick_time = Time_Value::gettimeofday() + Time_Value(OFFLINE_KICK_TIME);
	offline_mover_map_.insert(std::make_pair(role_id, *info));
}
void War_Scene::del_offline_mover(role_id_t role_id){
	offline_mover_map_.erase(role_id);
}

void War_Scene::check_recover_mine(void){
	for(int i=0; i<2; ++i){
		if(src_recover_coord_[i] != Coord()){
			Scene_Player_Set player_set;
			this->collect_round_player(0, src_recover_coord_[i], player_set, SRC_RECOVER_DISTANCE);
			recover_mine(i+1, player_set);
		}
	}
}

void War_Scene::check_over(void){
	check_over_ = false;
	int win_league = 0;
	if(this->light_score_ >= WAR_SCENE_WIN_SCORE){
		win_league = 1;
	}
	if(this->dark_score_ >= WAR_SCENE_WIN_SCORE){
		win_league = 2;
	}
	if(win_league != 0){
		war_over(win_league);
	}
}

bool War_Scene::check_player_empty(void){
	if(this->offline_mover_map_.size() > 0){
		return false;
	}
	bool empty = true;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				empty = false;
				break;
			}
		}
	}
	if(empty){
		this->set_scene_status(SCENE_RECOVER_STATUS);
		recover_time_ = Time_Value::gettimeofday() + Time_Value(300);
		SCENE_WAR_MANAGER->honour_field_finish(create_scene_args_.id);
		return true;
	}
	return false;
}

void War_Scene::check_offline_player(const Time_Value& now){
	Offline_Mover_Map::iterator it;
	for(it = offline_mover_map_.begin(); it != offline_mover_map_.end(); ++it){
		if(now >= it->second.offline_kick_time){
			SCENE_WAR_MANAGER->req_leave_war_scene(it->second.role_id);
			this->broadcast_war_info();
			offline_mover_map_.erase(it);
			return;
		}
	}
}

void War_Scene::check_inactive_player(const Time_Value& now){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				if(player->war_info()->notice_kick_time){
					 if(now + Time_Value(NOTICE_KICK_TIME) > player->war_info()->inactive_kick_time){
						 player->war_info()->notice_kick_time = false;
						 MSG_81000410 msg;
						 OBJ_SEND_TO_CLIENT(msg, (*player));
					 }
				}else if(now > player->war_info()->inactive_kick_time){
					if(player->is_in_mover_scene()){
						kick_player(player, 0);
						return;
					}
				}
			}
		}
	}
}

void War_Scene::check_is_war_over_player(const Time_Value& now){
	if(is_war_over_ && now > check_is_war_over_){
		is_war_over_ = false;
		std::vector<Scene_Player*> player_vec;
		Scene_Layer *layer = scene_layer(0);
		if (layer) {
			for (Mover_Map::iterator it = layer->mover_map_.begin();
					it != layer->mover_map_.end(); ++it) {
				if (it->second && it->second->player_self()) {
					player_vec.push_back(it->second->player_self());
				}
			}
		}
		int count = player_vec.size();
		for(int i = 0; i < count; ++i){
			int dest = 0;
			player_vec[i]->inner_transmit(dest, 0, 0);
		}
	}
}

void War_Scene::war_over(int win_league){
	Block_Buffer buf;
	MSG_81000402 msg;
	msg.win_league = win_league;
	std::vector<War_Player_Rank_Info> info_vec;
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				Scene_Player* player = it->second->player_self();
				// battle
				if(player->battle_scene()){
					//player->battle_scene()->interrupt_game(-1);
					player->force_finish_battle_scene();
				}
				War_Player_Rank_Info info;
				Scene_War_Rank_Info* war_info = player->war_info();
				info.server_name = war_info->server_name;
				info.assists_num = war_info->assists_num;
				info.kill_num = war_info->kill_num;
				info.max_kill = war_info->max_streak_win_num;
				info.league = war_info->league;
				info.merit = war_info->merit;
				info.role_id = player->role_id();
				info.role_name = player->role_name();
				info.score = war_info->total_score;
				info.reputation = war_info->rep;
				info_vec.push_back(info);
			}
		}
	}
	std::sort(info_vec.begin(), info_vec.end(), War_Rank_Greater);
	int info_count = info_vec.size();
	for(int i=0; i<info_count; ++i){
		info_vec[i].rank = i+1;
	}
	msg.rank_vec = info_vec;

	// 战场获胜广播：光明联盟获取的此次战场的胜利！
	if(win_league == 1){
		war_scene_announce(WAR_SCENE_ANNOUNCE_WAR_L_WIN);
	}else{
		war_scene_announce(WAR_SCENE_ANNOUNCE_WAR_D_WIN);
	}

	// 战场关闭广播：战场关闭，所有勇士自动退出战场。
	war_scene_announce(WAR_SCENE_ANNOUNCE_WAR_CLOSE);

	// 胜负奖励
	int b_count = info_vec.size();
	for(int i=0; i<b_count; ++i){
		Mover* mover = this->find_mover_with_layer(0, info_vec[i].role_id);
		if(mover){
			Scene_Player* player = mover->player_self();
			if(player){
				int gain_merite = 0;
				int add_piece = 0;
				int add_merite = 0;
				int add_arena_coin = 0;
				std::vector<Id_Int_Int> rank_props;
				if(player->war_info()->league == win_league){
					gain_merite += CONFIG_CACHE_DUNGEON->get_war_win_merit();
					//CONFIG_CACHE_DUNGEON->get_war_win_prop(rank_props);
					int rank_merit = 0;//CONFIG_CACHE_DUNGEON->get_war_over_rank_merit(info_vec[i].rank, rank_props);
					gain_merite += rank_merit;
					add_merite = player->war_info()->add_merit(gain_merite);
					add_piece = player->war_info()->add_piece(CONFIG_CACHE_DUNGEON->get_war_win_piece());
					add_arena_coin = player->war_info()->add_arean_coin(CONFIG_CACHE_DUNGEON->get_war_win_coin());
					{
						MSG_20200201 msg_1;
						msg_1.act_id = War_Scene_Type;
						msg_1.type = 0;
						msg_1.result = 1;
						msg_1.value1 = player->war_info()->kill_num;
						msg_1.value2 = player->war_info()->league;
						msg_1.value3 = player->war_info()->rep_coe;
						Block_Buffer buf_1;
						buf_1.make_message(msg_1.msg_id);
						msg_1.serialize(buf_1);
						buf_1.finish_message();
						player->send_to_logic(buf_1);
					}
				}else{
					gain_merite += CONFIG_CACHE_DUNGEON->get_war_lose_merit();
					//CONFIG_CACHE_DUNGEON->get_war_lose_prop(rank_props);
					int rank_merit = 0;//CONFIG_CACHE_DUNGEON->get_war_over_rank_merit(info_vec[i].rank, rank_props);
					gain_merite += rank_merit;
					add_merite = player->war_info()->add_merit(gain_merite);
					add_piece = player->war_info()->add_piece(CONFIG_CACHE_DUNGEON->get_war_lose_piece());
					add_arena_coin = player->war_info()->add_arean_coin(CONFIG_CACHE_DUNGEON->get_war_lose_coin());
					{
						MSG_20200201 msg_1;
						msg_1.act_id = 1;
						msg_1.act_id = War_Scene_Type;
						msg_1.type = 0;
						msg_1.result = 2;
						msg_1.value1 = player->war_info()->kill_num;
						msg_1.value2 = player->war_info()->league;
						msg_1.value3 = player->war_info()->rep_coe;
						Block_Buffer buf_1;
						buf_1.make_message(msg_1.msg_id);
						msg_1.serialize(buf_1);
						buf_1.finish_message();
						player->send_to_logic(buf_1);
					}
				}
				// 飘字通知 战场获胜，你累计获得了110功勋，200积分
				if(player->war_info()->league == win_league){
					player->war_scene_notice(WAR_SCENE_NOTICE_WIN, "", "", player->war_info()->total_score,
							player->war_info()->merit, player->war_info()->chest_pieces, player->war_info()->rep, player->war_info()->arena_coin);
				}else{
					player->war_scene_notice(WAR_SCENE_NOTICE_LOSE, "", "", player->war_info()->total_score,
							player->war_info()->merit, player->war_info()->chest_pieces, player->war_info()->rep, player->war_info()->arena_coin);
				}

				// sync to logic
				if(player->war_info()->chest_pieces_coe == 0){
					rank_props.clear();
				}else{
					int rank_p_count = rank_props.size();
					for(int i = 0; i < rank_p_count; ++i){
						rank_props[i].val1 = (double)(rank_props[i].val1*player->war_info()->chest_pieces_coe)/100.0f+0.5f;
					}
				}


				if(add_merite > 0 || add_piece > 0 || add_arena_coin > 0 || rank_props.empty() == false){
					player->add_merit_piece_to_logic(add_merite, add_piece, 0, add_arena_coin, rank_props);
				}
				msg.merit_coe = player->war_info()->merit_coe;
				msg.piece_coe = player->war_info()->chest_pieces_coe;
				OBJ_SEND_TO_CLIENT(msg, (*player));

				if(info_vec[i].rank == 1){// 第一名广播
					std::vector<Chat_Content> ann_contents;
					Role_Info role_info;
					role_info.role_id = player->role_id();
					role_info.role_name = player->role_name();
					role_info.level = player->level();
					Chat_Content new_content;
					new_content.type = ANN_CTTYPE_ROLE_INFO;
					new_content.role_Info = role_info;
					ann_contents.push_back(new_content);

					MSG_20500000 msg;
					msg.district = ANNOUNCE_DISTRICT_SYSTEM;
					msg.announce_id = 1393;
					msg.contents = ann_contents;
					Block_Buffer buf;
					MAKE_MSG_BUF(msg, buf);
					player->send_to_logic(buf);
				}
			}
		}
	}
	kick_all_player();
	this->set_scene_status(SCENE_RECOVER_STATUS);
	recover_time_ = Time_Value::gettimeofday() + Time_Value(300);
	SCENE_WAR_MANAGER->honour_field_finish(create_scene_args_.id);
	is_war_over_ = true;
	check_is_war_over_ = Time_Value::gettimeofday() + Time_Value(60);
}

void War_Scene::recover_mine(int league, Scene_Player_Set& player_set){
	bool war_info_change = false;
	Scene_Player_Set::iterator it;
	for(it = player_set.begin(); it != player_set.end(); ++it){
		Scene_Player* player = (*it);
		if(player && player->war_info()->is_pull_mine != 0 && player->war_info()->league == league){
			int mine_type = player->war_info()->is_pull_mine;
			int pieces = 0;
			int merit = 0;
			int score = 0;
			int rep = 0;
			CONFIG_CACHE_DUNGEON->get_war_mine_award(mine_type, pieces, merit, score, rep);
			player->war_info()->is_pull_mine = 0;
			int add_score = player->war_info()->add_score(score);
			int add_merit = player->war_info()->add_merit(merit);
			int add_pieces = player->war_info()->add_piece(pieces);
			int add_rep = player->war_info()->add_rep(rep);
			player->update_mover_war_info();
			// 同步功勋、添加宝箱碎片
			if(add_merit > 0 || add_pieces > 0 || add_rep > 0){
				std::vector<Id_Int_Int> temp_props;
				player->add_merit_piece_to_logic(add_merit, add_pieces, add_rep, 0, temp_props);
			}
			// 飘字通知
			player->war_scene_notice(WAR_SCENE_NOTICE_MINE, "", "", add_score, add_merit, add_pieces, add_rep, 0);
			//
			send_player_info(player);
			if(league == 1){
				this->light_score_ += score;
			}else{
				this->dark_score_ += score;
			}
			war_info_change = true;
			player->del_pull_mine_buf();
		}
	}
	if(war_info_change){
		this->broadcast_war_info();
	}
}

void War_Scene::kick_all_player(void){
	Scene_Layer *layer = scene_layer(0);
	if (layer) {
		std::vector<Scene_Player*> player_vec;
		for (Mover_Map::iterator it = layer->mover_map_.begin();
				it != layer->mover_map_.end(); ++it) {
			if (it->second && it->second->player_self()) {
				// 恢复进入战场的血量
				double src_hp = it->second->player_self()->war_info()->src_hp;
				it->second->player_self()->restore_hp(src_hp, false);
				if(it->second->is_in_mover_scene()){//
					player_vec.push_back(it->second->player_self());
				}
			}
		}
		int count = player_vec.size();
		for(int i = 0; i < count; ++i){
			int dest = 0;
			player_vec[i]->inner_transmit(dest,0,0);
		}
	}
	//
}

void War_Scene::kick_player(Scene_Player* player, int reason){
	check_over_ = true;

	MSG_81000407 msg;
	msg.reason = reason;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	int dest = 0;
	player->inner_transmit(dest,0,0);
}

void War_Scene::war_battle_finish(int win_camp){
	int win_league = win_camp;
	if(win_camp == 0){
		if(this->light_score_ >= this->dark_score_){
			win_league = 1;
		}else{
			win_league = 2;
		}
	}else if(win_camp == 1){
		win_league = 1;
	}else{
		win_league = 2;
	}
	war_over(win_league);
}


int War_Scene::war_scene_announce(int type, role_id_t role_id, std::string name, int value1, int value2, int value3){
	std::vector<Chat_Content> ann_contents;
	Role_Info role_info;
	Chat_Content new_content;
	switch(type){
		case WAR_SCENE_ANNOUNCE_L_OFFICE_REVIVE:
		case WAR_SCENE_ANNOUNCE_D_OFFICE_REVIVE:
		case WAR_SCENE_ANNOUNCE_WAR_CLOSE:
		case WAR_SCENE_ANNOUNCE_SPE_MINE_REFRESH:
		case WAR_SCENE_ANNOUNCE_WAR_L_WIN:
		case WAR_SCENE_ANNOUNCE_WAR_D_WIN:{
			break;
		}
		default:{
			break;
		}
	}
	MSG_83000000 msg;
	msg.chat_type = chat_scene;
	msg.system_type = type;
	msg.content = ann_contents;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	broadcast_all_within_battle(0, buf);
	return 0;
}

int War_Scene::get_ann_id_from_streak_win_num(const int num){
	if(num < 3){
		return 0;
	}
	switch(num){
		case 3:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_3;
			break;
		}
		case 4:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_4;
			break;
		}
		case 5:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_5;
			break;
		}
		case 6:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_6;
			break;
		}
		case 7:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_7;
			break;
		}
		case 8:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_8;
			break;
		}
		default:{
			return WAR_SCENE_ANNOUNCE_STREAK_WIN_8;
			break;
		}
	}
	return 0;
}

void War_Scene::gm_kick_player(const role_id_t role_id){
	Mover* mover = this->find_mover_with_layer(0, role_id);
	if(mover && mover->player_self()){
		this->kick_player(mover->player_self(), 1);
		return;
	}
	return;
}
