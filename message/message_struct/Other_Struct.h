/*
* Generate by devtool
*/

#ifndef _OTHER_H_
#define _OTHER_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
购买体力
*/
struct MSG_10120000  {
	MSG_10120000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
购买体力返回
*/
struct MSG_50120000 : public Base_Msg  {
	uint8_t result;
	MSG_50120000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120000;
		result = 0;
	}
};

/*
请求体力信息
*/
struct MSG_10120001  {
	MSG_10120001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求体力信息返回
*/
struct MSG_50120001 : public Base_Msg  {
	uint16_t phy_power_cur;
	uint16_t phy_power_max;
	uint8_t phy_power_buy_times;
	int32_t phy_power_recover_time;
	MSG_50120001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(phy_power_cur);
		w.write_uint16(phy_power_max);
		w.write_uint8(phy_power_buy_times);
		w.write_int32(phy_power_recover_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(phy_power_cur)  ||  r.read_uint16(phy_power_max)  ||  r.read_uint8(phy_power_buy_times)  ||  r.read_int32(phy_power_recover_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120001;
		phy_power_cur = 0;
		phy_power_max = 0;
		phy_power_buy_times = 0;
		phy_power_recover_time = 0;
	}
};



#endif