/*
 * Honor_Arena_Config.h
 *
 *  Created on: 2015年3月16日
 *      Author: root
 */

#ifndef HONOR_ARENA_CONFIG_H_
#define HONOR_ARENA_CONFIG_H_

#include "Game_Header.h"
#include "Base_Struct.h"
#include "status/Status_Struct.h"

struct Honor_Arena_Base_Config{
	Honor_Arena_Base_Config(void){
		reset();
	}
	~Honor_Arena_Base_Config(void){

	}
	void reset(void){
		base_score = 0;
		succ_score = 0;
		succ_merit = 0;
		succ_gold = 0;
		fail_score = 0;
		fail_merit = 0;
		fail_gold = 0;
		score_match.clear();
		buff_argv.reset();
	}

	int base_score;
	int succ_score;
	int succ_merit;
	int succ_gold;
	int fail_score;
	int fail_merit;
	int fail_gold;
	Int_Vec score_match;
	Status_Effect_Argv buff_argv;
};

struct Honor_Arena_Add_Config{
	Honor_Arena_Add_Config(void){
		reset();
	}
	~Honor_Arena_Add_Config(void){

	}
	void reset(void){
		continuity_succ = 0;
		continuity_score = 0;
		continuity_merit = 0;
		kill_continuity_score = 0;
		kill_continuity_merit = 0;
	}

	int continuity_succ;
	int continuity_score;
	int continuity_merit;
	int kill_continuity_score;
	int kill_continuity_merit;
};
typedef boost::unordered_map<int, Honor_Arena_Add_Config> Honor_Arena_Add_Config_Map;

struct Honor_Arena_Show_Config{
	Honor_Arena_Show_Config(void){
		reset();
	}
	~Honor_Arena_Show_Config(void){

	}
	void reset(void){
		continuity = 0;
		continuity_award.clear();
	}

	int continuity;
	Int_Int_Vec continuity_award;
};
typedef boost::unordered_map<int, Honor_Arena_Show_Config> Honor_Arena_Show_Config_Map;

struct Honor_Arena_Rank_Config{
	Honor_Arena_Rank_Config(void){
		reset();
	}
	~Honor_Arena_Rank_Config(void){

	}
	void reset(void){
		rank.reset();
		award.clear();
	}

	Int_Int rank;
	Int_Int_Vec award;
};
typedef boost::unordered_map<int, Honor_Arena_Rank_Config> Honor_Arena_Rank_Config_Map;

#endif /* HONOR_ARENA_CONFIG_H_ */
