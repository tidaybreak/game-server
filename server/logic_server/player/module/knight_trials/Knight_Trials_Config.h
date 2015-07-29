/*
 * Knight_Trials_Config.h
 *
 *  Created on: 2014-09-12
 *      Author: xiaoliang
 *
 *      配制文件结构
 */

#ifndef KNIGHT_TRIALS_CONFIG_H_
#define KNIGHT_TRIALS_CONFIG_H_

#include "Game_Typedef.h"

struct Knight_Trials_Points_Conf {
	Knight_Trials_Points_Conf(void) { reset(); }
	void reset(void) {
		points = 0;
		spro = 0;
		epro = 0;
		fdpro = 0;
		fdnum = 0;
		boxType = 0;
		num = 0;
	}
	uint8_t points;//关卡
	uint8_t spro;//起始匹配比例
	uint8_t epro;//终止匹配比例
	uint8_t fdpro;//匹配失败下降比例
	uint8_t fdnum;//匹配失败下降限制次数
	uint32_t boxType;//宝箱类型
	uint32_t num;//试炼场战斗后怒气恢复

};

typedef boost::unordered_map<uint8_t, Knight_Trials_Points_Conf> Knight_Trials_Points_Conf_Map;

struct Knight_Trials_Level_Conf {
	Knight_Trials_Level_Conf(void) { reset(); }
	void reset(void) {
		slevel = 0;
		elevel = 0;
	}

	uint32_t slevel; //开始等级
	uint32_t elevel; //结束等级
};

typedef std::list<Knight_Trials_Level_Conf> Knight_Trials_Level_Conf_List;

struct Knight_Trials_Exp_Reward_Conf {
	Knight_Trials_Exp_Reward_Conf(void) { reset(); }
	void reset(void) {
		level = 0;
		exp_reward.clear();
	}
	uint32_t level;
	UInt_Vec exp_reward;
};
typedef boost::unordered_map<uint32_t, Knight_Trials_Exp_Reward_Conf> Knight_Trials_Exp_Reward_Conf_Map;

struct Knight_Trials_Box_Reward_Conf {
	Knight_Trials_Box_Reward_Conf(void) { reset(); }
	void reset(void) {
		box_type = 0;
		reward_id = 0;
		slevel = 0;
		elevel = 0;
		type = 0;
		rate = 0;
		start = 0;
		end = 0;
	}
	uint32_t box_type;

	uint32_t reward_id;//奖励ID
	uint32_t slevel;//开始等级
	uint32_t elevel;//结束等级
	uint32_t type;//奖励类型(1.资源,2.道具)
	uint32_t rate;//奖励权重
	uint32_t start;//奖励区间开始值
	uint32_t end;//奖励区间结束值
};
typedef std::list<Knight_Trials_Box_Reward_Conf> Knight_Trials_Box_Reward_Conf_List;
typedef std::vector<Knight_Trials_Box_Reward_Conf> Knight_Trials_Box_Reward_Conf_Vec;
typedef boost::unordered_map<uint32_t, Knight_Trials_Box_Reward_Conf_List> Knight_Trials_Box_Reward_Conf_Map;

#endif /* KNIGHT_TRIALS_CONFIG_H_ */
