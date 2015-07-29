/*
* Generate by devtool
*/

#ifndef _MSG_GUIDE_H_
#define _MSG_GUIDE_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求新手引导
*/
struct MSG_10104000  {
	uint16_t id;
	MSG_10104000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求新手引导(返回)
备注：

status  :是否可以引导。1: 可以引导，0:不可以引导。
*/
struct MSG_50104000 : public Base_Msg  {
	uint16_t id;
	uint8_t status;
	MSG_50104000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(id);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(id)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50104000;
		id = 0;
		status = 0;
	}
};

/*
请求新手引导设置
*/
struct MSG_10104001  {
	uint16_t guide_id;
	uint8_t step;
	MSG_10104001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(guide_id);
		w.write_uint8(step);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(guide_id)  ||  r.read_uint8(step)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		guide_id = 0;
		step = 0;
	}
};

/*
请求保存已播放的剧情
备注：客户端先播放剧情，然后，发这条消息给服务端用来存库
*/
struct MSG_10104002  {
	uint32_t plot_id;
	MSG_10104002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(plot_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(plot_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		plot_id = 0;
	}
};

/*
请求保存已播放的剧情(返回)
*/
struct MSG_50104002 : public Base_Msg  {
	uint32_t plot_id;
	MSG_50104002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(plot_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(plot_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50104002;
		plot_id = 0;
	}
};

/*
请求设置任务引导进度
*/
struct MSG_10104003  {
	uint32_t guide_id;
	MSG_10104003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(guide_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(guide_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		guide_id = 0;
	}
};



#endif