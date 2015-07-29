/*
 * Answer_Config.h
 *
 *  Created on: Apr 16, 2014
 *      Author: ti
 *
 *      配制文件结构
 */

#ifndef ANSWER_CONFIG_H_
#define ANSWER_CONFIG_H_

struct Answer_Reward_Money {
	Answer_Reward_Money(uint32_t id_, uint32_t num_) {
		id = id_;
		num = num_;
	}
	Answer_Reward_Money() { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
	}
	uint32_t id;
	uint32_t num;
};

struct Answer_Reward_Item {
	Answer_Reward_Item(uint32_t id_, uint32_t num_) {
		id = id_;
		num = num_;
	}
	Answer_Reward_Item(void) { reset(); }
	void reset(void) {
		id = 0;
		num = 0;
	}
	uint32_t id;
	uint32_t num;
};

typedef std::vector<Answer_Reward_Money> Answer_Reward_Money_Vec;
typedef std::vector<Answer_Reward_Item> Answer_Reward_Item_Vec;

struct Answer_Reward_Config {
	Answer_Reward_Config(void) { reset(); }
	void reset(void) {
		exp = 0;
		money.clear();
		item.clear();
	}
	uint32_t exp;
	Answer_Reward_Money_Vec money;
	Answer_Reward_Item_Vec item;
};


struct Answer_Question_Config {
	Answer_Question_Config(void){
		reset();
	}
	void reset(void) {
		id = 0;
		answer_a.clear();
		answer_b.clear();
		true_answer = 0;
		next_id = 0;
		integral = 0;
	}
	uint32_t id;//题目ID
	std::string answer_a;//答案选项A
	std::string answer_b;//答案选项B
	uint8_t true_answer;//正确答案 1.a, 2.b
	uint32_t next_id; //捆绑题目ID
	uint32_t integral; //题目积分
};

typedef boost::unordered_map<uint32_t, Answer_Question_Config> Answer_Question_Config_Map;

struct Answer_Integral_Reward_Config {
	Answer_Integral_Reward_Config(void) {
		reset();
	}
	void reset(void) {
		level = 0;
		reward.reset();
	}
	uint32_t level;
	Answer_Reward_Config reward;
};
typedef boost::unordered_map<uint32_t, Answer_Integral_Reward_Config> Answer_Integral_Reward_Config_Map;

struct Answer_Rank_Reward_Config {
	Answer_Rank_Reward_Config(void) {
		reset();
	}
	void reset(void) {
		start_rank = 0;
		end_rank = 0;
		reward.reset();
	}
	uint32_t start_rank;
	uint32_t end_rank;
	Answer_Reward_Config reward;
};
typedef std::list<Answer_Rank_Reward_Config> Answer_Rank_Reward_Config_List;




#endif /* ANSWER_CONFIG_H_ */
