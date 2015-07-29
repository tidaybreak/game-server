/*
 * ThorLottery_Struct.h
 *
 *  Created on: 2014年11月11日
 *      Author: root
 */

#ifndef THORLOTTERY_STRUCT_H_
#define THORLOTTERY_STRUCT_H_

#include "Base_Struct.h"
#include "Game_Typedef.h"
#include "Server_Struct.h"
#include "pack/Pack_Struct.h"

struct ThorLottery_Item_Log {
	ThorLottery_Item_Log() { reset(); }
	void reset(void);

	Time_Value time_;
	std::string role_name_;
	role_id_t role_id_;
	int item_id_;
	int item_num_;
	int bind_state_;
};
typedef std::vector<ThorLottery_Item_Log> ThorLottery_Log_Vec;

struct ThorLottery_Item {
	ThorLottery_Item() { reset(); }
	void reset(void);

	int index_;
	int counter_;
};
typedef boost::unordered_map<int, ThorLottery_Item> ThorLottery_Item_Map;

struct ThorLottery_Item_Config;
struct ThorLottery_Item_Return {
	ThorLottery_Item_Return() { reset(); }
	void reset(void) {
		item.reset();
		num = 0;
		cfg_thorlottery_item = 0;
	}

	int num;
	Item_Detail item;
	ThorLottery_Item_Config const* cfg_thorlottery_item;
};
typedef std::vector<ThorLottery_Item_Return> ThorLottery_Item_Return_Vec;

struct ThorLottery_Manager_Detail {
	ThorLottery_Manager_Detail(void) { reset(); }
	void reset(void);

	int load(void);
	int save(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	bool is_change_;
	int season_id_;
	int state_;
	int icon_id_;
	int lv_limit;
	ThorLottery_Item_Map thorlottery_item_map_;
	ThorLottery_Log_Vec lottery_log_vec_;
};

struct ThorLottery_Detail : public Detail{
	ThorLottery_Detail() { reset(); }
	void reset(void);

	int load(void);
	int save(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	role_id_t role_id;
	int season_id;
	int score;
	int daily_lottery_times_;
	Time_Value lottery_time_;
	ThorLottery_Item_Map thorlottery_item_map_;
};

#endif /* THORLOTTERY_STRUCT_H_ */
