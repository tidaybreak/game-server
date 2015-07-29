/*
 * Dungeon_Config.h
 *
 *  Created on: Apr 16, 2014
 *      Author: ti
 */

#ifndef DUNGEON_CONFIG_H_
#define DUNGEON_CONFIG_H_

#include "Base_Struct.h"

struct Single_Copy_Vip_Config{
	int vip_level;
	int rate;
	Single_Copy_Vip_Config(void){
		reset();
	}
	void reset(void){
		vip_level = 0;
		rate = 0;
	}
};
typedef boost::unordered_map<int, Single_Copy_Vip_Config> Single_Copy_Vip_Config_Map;

// 副本章节奖励
struct Chapter_Award_Config{
	int chapter;
	int finish_scene;
	std::vector<Int_Int> award_vec;

	Chapter_Award_Config(void){
		reset();
	}
	void reset(void){
		chapter = 0;
		finish_scene = 0;
		award_vec.clear();
	}
};

typedef boost::unordered_map<int, Chapter_Award_Config> Chapter_Award_Map;

struct Elite_Copy_Config{
	int reset_vip_lv;
	int reset_cost;
	int hor_lv_limit;		//噩梦开启等级
	Elite_Copy_Config(void){
		reset();
	}
	void reset(void){
		reset_vip_lv = 0;
		reset_cost = 0;
		hor_lv_limit = 0;
	}
};

struct War_Base_Config{
	int initial_score;
	int fb_merit;
	double fb_score_pct;
	int fb_score;
	double bk_score_pct;
	double assists_merit;
	int heal_cost_score;
	int guard_score;
	int guard_merit;
	int officer_score;
	int officer_merit;
	int level_limit;
	int ready_time;
	std::vector<Int_Int> open_time;
	War_Base_Config(void){
		reset();
	}

	void reset(void){
		initial_score = 0;
		fb_merit = 0;
		fb_score_pct = 0;
		fb_score = 0;
		bk_score_pct = 0;
		assists_merit = 0;
		heal_cost_score = 0;
		guard_score = 0;
		guard_merit = 0;
		officer_score = 0;
		officer_merit = 0;
		level_limit = 0;
		ready_time = 0;
		open_time.clear();
	}
};

struct War_Streak_Kill_Award_Config{
	int streak_kill_num;
	int merit;
	int continuity_merit;
	int rep;
	int continuity_rep;
	double score_pct;
	int score;

	War_Streak_Kill_Award_Config(void){
		reset();
	}

	void reset(void){
		streak_kill_num = 0;
		merit = 0;
		continuity_merit = 0;
		rep = 0;
		continuity_rep = 0;
		score_pct = 0;
		score = 0;
	}
};

typedef boost::unordered_map<int, War_Streak_Kill_Award_Config> War_Streak_Kill_Award_Map;

struct War_Mine_Award_Config{
	std::vector<int> nor_pieces;
	std::vector<int> nor_merit;
	std::vector<int> nor_score;
	std::vector<int> nor_rep;
	std::vector<int> nor_rate;
	std::vector<int> spe_pieces;
	std::vector<int> spe_merit;
	std::vector<int> spe_score;
	std::vector<int> spe_rep;
	std::vector<int> spe_rate;
	War_Mine_Award_Config(void){
		reset();
	}

	void reset(void){
		nor_pieces.clear();
		nor_merit.clear();
		nor_score.clear();
		nor_rep.clear();
		nor_rate.clear();
		spe_pieces.clear();
		spe_merit.clear();
		spe_score.clear();
		spe_rep.clear();
		spe_rate.clear();
	}
};

struct War_Balance_Config{
	int lose_merit;
	int lose_piece;
	int win_merit;
	int win_piece;
	std::map<int, std::vector<Id_Int_Int>> rank_merit;
	War_Balance_Config(void){
		reset();
	}
	void reset(void){
		lose_merit = 0;
		lose_piece = 0;
		win_merit = 0;
		win_piece = 0;
		rank_merit.clear();
	}
};

struct War_Kill_Num_Award_Config{
	War_Kill_Num_Award_Config(void){
		reset();
	}
	~War_Kill_Num_Award_Config(void){

	}
	void reset(void){
		kill_num = 0;
		rep = 0;
		award.clear();
	}
	int kill_num;
	int rep;
	std::vector<Id_Int_Int> award;
};
typedef boost::unordered_map<int, War_Kill_Num_Award_Config> War_Kill_Num_Award_Map;


struct War_Random_Coord_Config{
	enum{
		LIGHT_REVIVE_BALL_COORD = 10,
		DARK_REVIVE_BALL_COORD 	= 20,
		LIGHT_MINE_COORD			= 11,
		DARK_MINE_COORD         = 21,
	};
	std::vector<Coord> light_revive_ball;
	std::vector<Coord> dark_revive_ball;
	std::vector<Coord> light_mine;
	std::vector<Coord> dark_mine;

	War_Random_Coord_Config(void){
		reset();
	}
	void reset(void){
		light_revive_ball.clear();
		dark_revive_ball.clear();
		light_mine.clear();
		dark_mine.clear();
	}
};

struct Wipe_Base_Config{
	int single_time;
	int single_cost;
	int single_level_limit;
	int elite_time;
	int elite_cost;
	int elite_level_limit;
	int nor_elite_wipe_id;
	int hor_elite_wipe_id;
	int single_wipe_no_cd_lv;
	int elite_cost_item;
	int elite_cost_money;
	int elite_cost_type;
	int elite_base_num;
	int elite_cost_num;
	int single_ver;
	int elite_ver;
	int lord_temple_ver;
	int lord_open_id;
	Wipe_Base_Config(void){
		reset();
	}
	void reset(void){
		single_time = 0;
		single_cost = 0;
		single_level_limit = 0;
		elite_time = 0;
		elite_cost = 0;
		elite_level_limit = 0;
		nor_elite_wipe_id = 0;
		hor_elite_wipe_id = 0;
		single_wipe_no_cd_lv = 0;
		elite_cost_item = 0;
		elite_cost_money = 0;
		elite_cost_type = 0;
		elite_base_num = 0;
		elite_cost_num = 0;
		single_ver = 0;
		elite_ver = 0;
		lord_temple_ver = 0;
		lord_open_id = 0;
	}
};

struct Single_Card_Config{
	int id;
	int amount;
	int rate;
};

static bool Single_Card_Greater(Single_Card_Config card1, Single_Card_Config card2){
	return card1.rate < card2.rate;
}

struct Wipe_Award_Config{
	int scene_id;
	int exp;
	int copper;
	std::vector<Int_Int> first_reward1;
	std::vector<Int_Int> first_reward2;
	std::vector<Int_Int> first_reward3;
	std::vector<Int_Int> copy_reward1;
	std::vector<Int_Int> copy_reward2;
	std::vector<Int_Int> copy_reward3;
	std::vector<Int_Int> monsters;
	std::vector<Int_Int> box;
	std::vector<Int_Int> vip_box;
	std::vector<Single_Card_Config> card_reward1;
	std::vector<Single_Card_Config> card_reward2;
	std::vector<Single_Card_Config> card_reward3;
	Wipe_Award_Config(void){
		reset();
	}
	void reset(void){
		scene_id = 0;
		first_reward1.clear();
		first_reward2.clear();
		first_reward3.clear();
		copy_reward1.clear();
		copy_reward2.clear();
		copy_reward3.clear();
		monsters.clear();
		box.clear();
		vip_box.clear();
		card_reward1.clear();
		card_reward2.clear();
		card_reward3.clear();
	}
	const std::vector<Int_Int>& first_reward(int8_t player_career) const{
		if(player_career == 1){
			return first_reward1;
		}else if(player_career == 2){
			return first_reward2;
		}else if(player_career == 3){
			return first_reward3;
		}
		return first_reward1;
	}
	const std::vector<Int_Int>& copy_reward(int8_t player_career) const{
		if(player_career == 1){
			return copy_reward1;
		}else if(player_career == 2){
			return copy_reward2;
		}else if(player_career == 3){
			return copy_reward3;
		}
		return copy_reward1;
	}
	const std::vector<Single_Card_Config>& card_reward(int8_t player_career) const{
		if(player_career == 1){
			return card_reward1;
		}else if(player_career == 2){
			return card_reward2;
		}else if(player_career == 3){
			return card_reward3;
		}
		return card_reward1;
	}
	int get_card_reward(const int8_t player_career, int& id, int& amount) const{
		std::vector<Single_Card_Config> cards = card_reward(player_career);
		int max_nums = 0;
		int count = cards.size();
		for(int i=0; i<count; ++i){
			max_nums += cards[i].rate;
		}
		if(max_nums <= 0){
			return -1;
		}
		int resIndex = -1;
		int random_nums = random() % max_nums;
		int total_nums = 0;
		for(int i = 0; i < count; ++i){
			total_nums += cards[i].rate;
			if(random_nums < total_nums){
				id = cards[i].id;
				amount = cards[i].amount;
				resIndex = i;
				break;
			}
		}
		return resIndex;
	}
	int get_show_card(const int8_t player_career, const int index, const int get_index, std::vector<Single_Card_Config>& show_cards) const {
		std::vector<Single_Card_Config> cards = card_reward(player_career);
		int count = cards.size();
		if(index < 0 || index >= count){
			return 0;
		}
		if(get_index < 0 || get_index >= count){
			return 0;
		}
		int id = 0;
		int amount = 0;
		int count_index = 0;
		std::vector<Single_Card_Config>::iterator it;
		for(it = cards.begin(); it != cards.end(); ++it){
			if(count_index == get_index){
				id = it->id;
				amount = it->amount;
				cards.erase(it);
				break;
			}
			++count_index;
		}
		std::sort(cards.begin(), cards.end(), Single_Card_Greater);
		std::random_shuffle(cards.begin(), cards.end());
		for(int i = 0; i < 4; ++i){
			if(i == 0){
				Single_Card_Config card_info;
				card_info.id = id;
				card_info.amount = amount;
				show_cards.push_back(card_info);
			}else{
				Single_Card_Config card_info;
				std::vector<Single_Card_Config>::iterator it = cards.begin();
				if(it != cards.end()){
					card_info.id = it->id;
					card_info.amount = it->amount;
					cards.erase(it);
				}
				show_cards.push_back(card_info);
			}
		}
		return 0;
	}
};
typedef boost::unordered_map<int, Wipe_Award_Config> Wipe_Award_Config_Map;


struct Team_Award_Config{
	int scene_id;
	int exp;
	int copper;
	int soul;
	int friendship;

	int drop_type;
	int mult_drop_type;
	double drop_mult_coe;
	int drop_lv_limit;
	double drop_lv_coe;
	double res_mult_coe;
	int res_lv_limit;
	double res_lv_coe;
	Int_Vec drops;
	Team_Award_Config(void){
		reset();
	}
	void reset(void){
		scene_id = 0;
		exp = 0;
		copper = 0;
		soul = 0;
		friendship = 0;
		drop_type = 0;
		mult_drop_type = 0;
		drop_mult_coe = 0.0;
		drop_lv_limit = 0;
		drop_lv_coe = 0.0;
		res_mult_coe = 0.0;
		res_lv_limit = 0;
		res_lv_coe = 0.0;
		drops.clear();
	}
};
typedef boost::unordered_map<int, Team_Award_Config> Team_Award_Config_Map;

#endif /* DUNGEON_CONFIG_H_ */
