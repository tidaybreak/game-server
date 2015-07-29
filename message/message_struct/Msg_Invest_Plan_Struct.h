/*
* Generate by devtool
*/

#ifndef _MSG_INVEST_PLAN_H_
#define _MSG_INVEST_PLAN_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求打开月卡投资页面
*/
struct MSG_10101200  {
	MSG_10101200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开月卡投资页面(返回)
*/
struct MSG_50101200 : public Base_Msg  {
	uint8_t is_invest;
	uint8_t left_day;
	uint32_t left_money;
	std::vector<Month_Card_Invest_Info> button_llist;
	MSG_50101200(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_invest);
		w.write_uint8(left_day);
		w.write_uint32(left_money);
		uint16_t __button_llist_vec_size = button_llist.size();
		w.write_uint16(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				button_llist[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_invest)  ||  r.read_uint8(left_day)  ||  r.read_uint32(left_money)  )
				return -1;
 		uint16_t __button_llist_vec_size;
		if(r.read_uint16(__button_llist_vec_size)  )
				return -1;
		button_llist.reserve(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				Month_Card_Invest_Info v;
				if(v.deserialize(r))
						return -1;
				button_llist.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101200;
		is_invest = 0;
		left_day = 0;
		left_money = 0;
		button_llist.clear();
	}
};

/*
请求领取月卡投资的收益
*/
struct MSG_10101201  {
	uint8_t day;
	MSG_10101201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(day);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(day)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		day = 0;
	}
};

/*
请求领取月卡投资的收益(返回)
*/
struct MSG_50101201 : public Base_Msg  {
	uint8_t day;
	uint8_t is_invest;
	uint8_t left_day;
	uint32_t left_money;
	MSG_50101201(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(day);
		w.write_uint8(is_invest);
		w.write_uint8(left_day);
		w.write_uint32(left_money);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(day)  ||  r.read_uint8(is_invest)  ||  r.read_uint8(left_day)  ||  r.read_uint32(left_money)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50101201;
		day = 0;
		is_invest = 0;
		left_day = 0;
		left_money = 0;
	}
};

/*
请求月卡投资
*/
struct MSG_10101202  {
	MSG_10101202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求月卡投资(返回)
*/
struct MSG_50101202 : public Base_Msg  {
	uint8_t left_day;
	uint32_t left_money;
	std::vector<Month_Card_Invest_Info> button_llist;
	MSG_50101202(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(left_day);
		w.write_uint32(left_money);
		uint16_t __button_llist_vec_size = button_llist.size();
		w.write_uint16(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				button_llist[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(left_day)  ||  r.read_uint32(left_money)  )
				return -1;
 		uint16_t __button_llist_vec_size;
		if(r.read_uint16(__button_llist_vec_size)  )
				return -1;
		button_llist.reserve(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				Month_Card_Invest_Info v;
				if(v.deserialize(r))
						return -1;
				button_llist.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101202;
		left_day = 0;
		left_money = 0;
		button_llist.clear();
	}
};

/*
请求打开升级投资页面
*/
struct MSG_10101203  {
	MSG_10101203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开升级投资页面(返回)
*/
struct MSG_50101203 : public Base_Msg  {
	uint8_t is_full;
	uint32_t invested_money;
	uint32_t can_profit;
	uint8_t invest_button;
	std::vector<Upgrade_Invest_Info> button_list;
	std::vector<uint32_t> invest_list;
	std::vector<Upgrade_Invest_Profit_Info> upgrade_invest_profit_list;
	MSG_50101203(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_full);
		w.write_uint32(invested_money);
		w.write_uint32(can_profit);
		w.write_uint8(invest_button);
		uint16_t __button_list_vec_size = button_list.size();
		w.write_uint16(__button_list_vec_size);
		for(uint16_t i = 0; i < __button_list_vec_size; ++i) {
				button_list[i].serialize(w);
		}

		uint16_t __invest_list_vec_size = invest_list.size();
		w.write_uint16(__invest_list_vec_size);
		for(uint16_t i = 0; i < __invest_list_vec_size; ++i) {
				w.write_uint32(invest_list[i]);
		}

		uint16_t __upgrade_invest_profit_list_vec_size = upgrade_invest_profit_list.size();
		w.write_uint16(__upgrade_invest_profit_list_vec_size);
		for(uint16_t i = 0; i < __upgrade_invest_profit_list_vec_size; ++i) {
				upgrade_invest_profit_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_full)  ||  r.read_uint32(invested_money)  ||  r.read_uint32(can_profit)  ||  r.read_uint8(invest_button)  )
				return -1;
 		uint16_t __button_list_vec_size;
		if(r.read_uint16(__button_list_vec_size)  )
				return -1;
		button_list.reserve(__button_list_vec_size);
		for(uint16_t i = 0; i < __button_list_vec_size; ++i) {
				Upgrade_Invest_Info v;
				if(v.deserialize(r))
						return -1;
				button_list.push_back(v);
		}

		uint16_t __invest_list_vec_size;
		if(r.read_uint16(__invest_list_vec_size)  )
				return -1;
		invest_list.reserve(__invest_list_vec_size);
		for(uint16_t i = 0; i < __invest_list_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				invest_list.push_back(v);
		}

		uint16_t __upgrade_invest_profit_list_vec_size;
		if(r.read_uint16(__upgrade_invest_profit_list_vec_size)  )
				return -1;
		upgrade_invest_profit_list.reserve(__upgrade_invest_profit_list_vec_size);
		for(uint16_t i = 0; i < __upgrade_invest_profit_list_vec_size; ++i) {
				Upgrade_Invest_Profit_Info v;
				if(v.deserialize(r))
						return -1;
				upgrade_invest_profit_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101203;
		is_full = 0;
		invested_money = 0;
		can_profit = 0;
		invest_button = 0;
		button_list.clear();
		invest_list.clear();
		upgrade_invest_profit_list.clear();
	}
};

/*
请求领取升级投资的收益
*/
struct MSG_10101204  {
	uint32_t level;
	MSG_10101204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
	}
};

/*
请求领取升级投资的收益(返回)
*/
struct MSG_50101204 : public Base_Msg  {
	uint32_t level;
	uint32_t can_profit;
	std::vector<Upgrade_Invest_Profit_Info> upgrade_invest_profit_list;
	MSG_50101204(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
		w.write_uint32(can_profit);
		uint16_t __upgrade_invest_profit_list_vec_size = upgrade_invest_profit_list.size();
		w.write_uint16(__upgrade_invest_profit_list_vec_size);
		for(uint16_t i = 0; i < __upgrade_invest_profit_list_vec_size; ++i) {
				upgrade_invest_profit_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  ||  r.read_uint32(can_profit)  )
				return -1;
 		uint16_t __upgrade_invest_profit_list_vec_size;
		if(r.read_uint16(__upgrade_invest_profit_list_vec_size)  )
				return -1;
		upgrade_invest_profit_list.reserve(__upgrade_invest_profit_list_vec_size);
		for(uint16_t i = 0; i < __upgrade_invest_profit_list_vec_size; ++i) {
				Upgrade_Invest_Profit_Info v;
				if(v.deserialize(r))
						return -1;
				upgrade_invest_profit_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101204;
		level = 0;
		can_profit = 0;
		upgrade_invest_profit_list.clear();
	}
};

/*
请求升级投资
*/
struct MSG_10101205  {
	uint32_t money;
	uint8_t type;
	MSG_10101205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(money);
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(money)  ||  r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		money = 0;
		type = 0;
	}
};

/*
请求升级投资(返回)
*/
struct MSG_50101205 : public Base_Msg  {
	uint8_t is_full;
	uint32_t invested_money;
	uint32_t can_profit;
	uint8_t invest_button;
	std::vector<Upgrade_Invest_Info> button_llist;
	std::vector<uint32_t> invest_list;
	MSG_50101205(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_full);
		w.write_uint32(invested_money);
		w.write_uint32(can_profit);
		w.write_uint8(invest_button);
		uint16_t __button_llist_vec_size = button_llist.size();
		w.write_uint16(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				button_llist[i].serialize(w);
		}

		uint16_t __invest_list_vec_size = invest_list.size();
		w.write_uint16(__invest_list_vec_size);
		for(uint16_t i = 0; i < __invest_list_vec_size; ++i) {
				w.write_uint32(invest_list[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_full)  ||  r.read_uint32(invested_money)  ||  r.read_uint32(can_profit)  ||  r.read_uint8(invest_button)  )
				return -1;
 		uint16_t __button_llist_vec_size;
		if(r.read_uint16(__button_llist_vec_size)  )
				return -1;
		button_llist.reserve(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				Upgrade_Invest_Info v;
				if(v.deserialize(r))
						return -1;
				button_llist.push_back(v);
		}

		uint16_t __invest_list_vec_size;
		if(r.read_uint16(__invest_list_vec_size)  )
				return -1;
		invest_list.reserve(__invest_list_vec_size);
		for(uint16_t i = 0; i < __invest_list_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				invest_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101205;
		is_full = 0;
		invested_money = 0;
		can_profit = 0;
		invest_button = 0;
		button_llist.clear();
		invest_list.clear();
	}
};

/*
一键领取所有升级投资收益
*/
struct MSG_10101206  {
	MSG_10101206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
一键领取所有升级投资收益(返回)
*/
struct MSG_50101206 : public Base_Msg  {
	uint32_t can_profit;
	uint8_t invest_button;
	std::vector<Upgrade_Invest_Info> button_llist;
	std::vector<Upgrade_Invest_Profit_Info> upgrade_invest_profit_list;
	MSG_50101206(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(can_profit);
		w.write_uint8(invest_button);
		uint16_t __button_llist_vec_size = button_llist.size();
		w.write_uint16(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				button_llist[i].serialize(w);
		}

		uint16_t __upgrade_invest_profit_list_vec_size = upgrade_invest_profit_list.size();
		w.write_uint16(__upgrade_invest_profit_list_vec_size);
		for(uint16_t i = 0; i < __upgrade_invest_profit_list_vec_size; ++i) {
				upgrade_invest_profit_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(can_profit)  ||  r.read_uint8(invest_button)  )
				return -1;
 		uint16_t __button_llist_vec_size;
		if(r.read_uint16(__button_llist_vec_size)  )
				return -1;
		button_llist.reserve(__button_llist_vec_size);
		for(uint16_t i = 0; i < __button_llist_vec_size; ++i) {
				Upgrade_Invest_Info v;
				if(v.deserialize(r))
						return -1;
				button_llist.push_back(v);
		}

		uint16_t __upgrade_invest_profit_list_vec_size;
		if(r.read_uint16(__upgrade_invest_profit_list_vec_size)  )
				return -1;
		upgrade_invest_profit_list.reserve(__upgrade_invest_profit_list_vec_size);
		for(uint16_t i = 0; i < __upgrade_invest_profit_list_vec_size; ++i) {
				Upgrade_Invest_Profit_Info v;
				if(v.deserialize(r))
						return -1;
				upgrade_invest_profit_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101206;
		can_profit = 0;
		invest_button = 0;
		button_llist.clear();
		upgrade_invest_profit_list.clear();
	}
};

/*
请求获取投资计划的历史记录
*/
struct MSG_10101207  {
	MSG_10101207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求获取投资计划的历史记录(返回)
*/
struct MSG_50101207 : public Base_Msg  {
	std::vector<INVEST_PLAN_HISTORY_RECORD> history_list;
	MSG_50101207(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __history_list_vec_size = history_list.size();
		w.write_uint16(__history_list_vec_size);
		for(uint16_t i = 0; i < __history_list_vec_size; ++i) {
				history_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __history_list_vec_size;
		if(r.read_uint16(__history_list_vec_size)  )
				return -1;
		history_list.reserve(__history_list_vec_size);
		for(uint16_t i = 0; i < __history_list_vec_size; ++i) {
				INVEST_PLAN_HISTORY_RECORD v;
				if(v.deserialize(r))
						return -1;
				history_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50101207;
		history_list.clear();
	}
};



#endif